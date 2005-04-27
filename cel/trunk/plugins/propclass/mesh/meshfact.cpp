/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include "cssysdef.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "plugins/propclass/mesh/meshfact.h"
#include "propclass/camera.h"
#include "propclass/move.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "celtool/stdparams.h"
#include "csutil/util.h"
#include "csutil/debug.h"
#include "csutil/csobject.h"
#include "csutil/flags.h"
#include "iutil/object.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/objreg.h"
#include "iutil/eventq.h"
#include "iutil/plugin.h"
#include "iutil/vfs.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/engine.h"
#include "imap/loader.h"
#include "ivaria/reporter.h"
#include "imesh/object.h"
#include "imesh/sprite3d.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Mesh, "pcmesh")
CEL_IMPLEMENT_FACTORY (MeshSelect, "pcmeshselect")

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistence",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//---------------------------------------------------------------------------

csStringID celPcMesh::action_loadmesh = csInvalidStringID;
csStringID celPcMesh::action_loadmeshpath = csInvalidStringID;
csStringID celPcMesh::id_path = csInvalidStringID;
csStringID celPcMesh::id_filename = csInvalidStringID;
csStringID celPcMesh::id_factoryname = csInvalidStringID;
csStringID celPcMesh::action_movemesh = csInvalidStringID;
csStringID celPcMesh::id_sector = csInvalidStringID;
csStringID celPcMesh::id_position = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcMesh)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMesh)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMesh::PcMesh)
  SCF_IMPLEMENTS_INTERFACE (iPcMesh)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMesh::celPcMesh (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMesh);
  visible = true;
  factory_ptr = 0;
  creation_flag = CEL_CREATE_NONE;
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);

  if (action_loadmesh == csInvalidStringID)
  {
    action_loadmesh = pl->FetchStringID ("cel.action.LoadMesh");
    action_loadmeshpath = pl->FetchStringID ("cel.action.LoadMeshPath");
    id_path = pl->FetchStringID ("cel.parameter.path");
    id_filename = pl->FetchStringID ("cel.parameter.filename");
    id_factoryname = pl->FetchStringID ("cel.parameter.factoryname");
    action_movemesh = pl->FetchStringID ("cel.action.MoveMesh");
    id_sector = pl->FetchStringID ("cel.parameter.sector");
    id_position = pl->FetchStringID ("cel.parameter.position");
  }
}

celPcMesh::~celPcMesh ()
{
  Clear ();
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcMesh);
}

void celPcMesh::Clear ()
{
  fileName.Empty ();
  factName.Empty ();
  path.Empty ();
  RemoveMesh ();
  factory_ptr = 0;
}

void celPcMesh::RemoveMesh ()
{
  if (mesh)
  {
    if (pl)
      pl->UnattachEntity (mesh->QueryObject (), entity);
    if (creation_flag != CEL_CREATE_MESH)
      engine->RemoveObject (mesh);
    mesh = 0;
    FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
  }
  creation_flag = CEL_CREATE_NONE;
}

bool celPcMesh::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_loadmesh)
  {
    CEL_FETCH_STRING_PAR (file,params,id_filename);
    if (!file) return false;
    CEL_FETCH_STRING_PAR (factory,params,id_factoryname);
    if (!factory) return false;
    bool rc = SetMesh (factory, file);
    // @@@ Error report!
    (void)rc;
    return true;
  }
  else if (actionId == action_loadmeshpath)
  {
    CEL_FETCH_STRING_PAR (pa,params,id_path);
    if (!pa) return false;
    CEL_FETCH_STRING_PAR (file,params,id_filename);
    if (!file) return false;
    CEL_FETCH_STRING_PAR (factory,params,id_factoryname);
    if (!factory) return false;
    SetPath (pa);
    bool rc = SetMesh (factory, file);
    // @@@ Error report!
    (void)rc;
    return true;
  }
  else if (actionId == action_movemesh)
  {
    CEL_FETCH_STRING_PAR (sector,params,id_sector);
    if (!sector) return false;
    CEL_FETCH_VECTOR3_PAR (position,params,id_position);
    if (!p_position) return false;
    iSector* sect = engine->FindSector (sector);
    if (!sect)
    {
      // @@@ Error
      return false;
    }
    MoveMesh (sect, position);
    return true;
  }
  return false;
}

#define MESH_SERIAL 2

csPtr<iCelDataBuffer> celPcMesh::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MESH_SERIAL);
  int i;
  databuf->Add ((uint8)creation_flag);
  if (creation_flag == CEL_CREATE_FACTORY)
  {
    databuf->Add (factName);
    databuf->Add (fileName);
    databuf->Add (path);
  }
  else if (creation_flag == CEL_CREATE_MESH)
  {
    /// @@@ Note: this requires a unique name for the mesh!
    databuf->Add (mesh->QueryObject ()->GetName ());
  }
  else if (creation_flag == CEL_CREATE_THING)
  {
    // @@@ Loading or saving meshes with this creation option is
    // not going to work properly as we can't easily save the thing data itself.
    // Perhaps we should consider combining this with the thing saver somehow.
  }

  databuf->Add (visible);

  if (mesh)
  {
    databuf->Add (GetAction ());
    iMovable* mov = mesh->GetMovable ();
    iSectorList* sl = mov->GetSectors ();
    databuf->Add ((uint16)(sl->GetCount ()));
    for (i = 0 ; i < sl->GetCount () ; i++)
    {
      databuf->Add (sl->Get (i)->QueryObject ()->GetName ());
    }
    csReversibleTransform& tr = mov->GetTransform ();
    databuf->Add (tr.GetO2TTranslation ());
    databuf->Add (tr.GetO2T ().m11);
    databuf->Add (tr.GetO2T ().m12);
    databuf->Add (tr.GetO2T ().m13);
    databuf->Add (tr.GetO2T ().m21);
    databuf->Add (tr.GetO2T ().m22);
    databuf->Add (tr.GetO2T ().m23);
    databuf->Add (tr.GetO2T ().m31);
    databuf->Add (tr.GetO2T ().m32);
    databuf->Add (tr.GetO2T ().m33);
  }

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMesh::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MESH_SERIAL)
  {
    Report (object_reg, "Serialnr != MESH_SERIAL.  Cannot load.");
    return false;
  }

  Clear ();
  visible = true;

  creation_flag = (celPcMeshCreationFlag)(databuf->GetUInt8 ());
  if (creation_flag == CEL_CREATE_FACTORY)
  {
    const char* factn = databuf->GetString ()->GetData ();
    const char* filen = databuf->GetString ()->GetData ();
    const char* pathn = databuf->GetString ()->GetData ();
    SetPath (pathn);
    SetMesh (factn, filen);
  }
  else if (creation_flag == CEL_CREATE_MESH)
  {
    const char* meshname = databuf->GetString ()->GetData ();
    iMeshWrapper* m = engine->FindMeshObject (meshname);
    if (!m)
    {
      Report (object_reg, "Can't find mesh '%s' for loading entity!",
      	meshname);
      return false;
    }
    SetMesh (m);
  }
  else if (creation_flag == CEL_CREATE_THING)
  {
    CreateEmptyThing ();
  }

  if (databuf->GetBool ()) Show ();
  else Hide ();

  if (mesh)
  {
    SetAction (databuf->GetString ()->GetData (), true);
    uint16 cnt = databuf->GetUInt16 ();
    mesh->GetMovable ()->ClearSectors ();
    int i;
    for (i = 0 ; i < cnt ; i++)
    {
      iSector* s = engine->GetSectors ()->FindByName (
    	  databuf->GetString ()->GetData ());
      CS_ASSERT (s != 0);
      mesh->GetMovable ()->GetSectors ()->Add (s);
    }

    csMatrix3 m_o2t;
    csVector3 v_o2t;
    databuf->GetVector3 (v_o2t);
    m_o2t.m11 = databuf->GetFloat ();
    m_o2t.m12 = databuf->GetFloat ();
    m_o2t.m13 = databuf->GetFloat ();
    m_o2t.m21 = databuf->GetFloat ();
    m_o2t.m22 = databuf->GetFloat ();
    m_o2t.m23 = databuf->GetFloat ();
    m_o2t.m31 = databuf->GetFloat ();
    m_o2t.m32 = databuf->GetFloat ();
    m_o2t.m33 = databuf->GetFloat ();
    csReversibleTransform tr (m_o2t, v_o2t);
    mesh->GetMovable ()->SetTransform (tr);
    mesh->GetMovable ()->UpdateMove ();
  }

  return true;
}

iMeshFactoryWrapper* celPcMesh::LoadMeshFactory ()
{
  csRef<iVFS> vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  if (!path.IsEmpty ())
  {
    // If we have a path then we first ChDir to that.
    vfs->PushDir ();
    vfs->ChDir (path);
  }

  csRef<iLoader> loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  CS_ASSERT (loader != 0);
  iBase* result;
  bool success = loader->Load (fileName, result, 0, false, true);
  if (!path.IsEmpty ())
  {
    vfs->PopDir ();
  }
  if (!success)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pfmesh.loadmeshfactory",
    	"Error loading mesh object factory or library '%s'!",
	(const char*)fileName);
    return 0;
  }

  csRef<iMeshFactoryWrapper> imeshfact;
  if (result == 0)
  {
    // We have a library.
    imeshfact = engine->FindMeshFactory (factName);
  }
  else
  {
    imeshfact = SCF_QUERY_INTERFACE (result, iMeshFactoryWrapper);
    if (!imeshfact)
    {
      // Perhaps it is a world file?
      csRef<iEngine> eng = SCF_QUERY_INTERFACE (result, iEngine);
      if (eng)
      {
        imeshfact = engine->FindMeshFactory (factName);
      }
    }
  }

  if (imeshfact == 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pfmesh.loadmeshfactory",
    	"Error loading mesh object factory '%s'!",
	(const char*)fileName);
    return 0;
  }
  return imeshfact;
}

void celPcMesh::SetPath (const char* path)
{
  celPcMesh::path = path;
}

bool celPcMesh::SetMesh (const char* factname, const char* filename)
{
  fileName = filename;
  factName = factname;

  RemoveMesh ();
  creation_flag = CEL_CREATE_FACTORY;

  if (!factName.IsEmpty () && !fileName.IsEmpty ())
  {
    csRef<iMeshFactoryWrapper> meshfact = engine->GetMeshFactories ()
				        	->FindByName (factname);
    if (!meshfact)
    {
      meshfact = LoadMeshFactory ();
      if (meshfact)
      {
        // Cache the factory.
	pl->Cache (meshfact);
      }
    }

    if (factory_ptr)
      factory_ptr = 0;
    if (meshfact)
    {
      factory_ptr = meshfact;

      mesh = engine->CreateMeshWrapper (meshfact, factname/*@@@?*/);
      pl->AttachEntity (mesh->QueryObject (), entity);
      FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
      return true;
    }
    return false;
  }
  return true;
}

void celPcMesh::SetMesh (iMeshWrapper* m)
{
  RemoveMesh ();
  creation_flag = CEL_CREATE_MESH;
  mesh = m;
  if (mesh)
    pl->AttachEntity (mesh->QueryObject (), entity);
  FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
}

void celPcMesh::CreateEmptyThing ()
{
  RemoveMesh ();
  creation_flag = CEL_CREATE_THING;

  mesh = engine->CreateThingMesh (0, 0);
  pl->AttachEntity (mesh->QueryObject (), entity);
  FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
}

void celPcMesh::MoveMesh (iSector* sector, const csVector3& pos)
{
  if (!mesh) return;
  if (sector)
    mesh->GetMovable ()->SetSector (sector);

  mesh->GetMovable ()->SetPosition (pos);
  mesh->GetMovable ()->UpdateMove ();
}

void celPcMesh::SetAction (const char* actionName, bool resetaction)
{
  if (!actionName) return;
  if (!mesh) return;
  csRef<iSprite3DState> state (SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
  	iSprite3DState));
  if (state)
  {
    if (resetaction || strcmp (actionName, state->GetCurAction()->GetName ()))
	state->SetAction (actionName);
  }
}

void celPcMesh::SetReverseAction (bool reverse)
{
  if (!mesh) return;
  csRef<iSprite3DState> state (SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
  	iSprite3DState));
  if (state)
  {
      state->SetReverseAction(reverse);
  }
}

const char* celPcMesh::GetAction ()
{
  if (!mesh) return 0;
  csRef<iSprite3DState> state (SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
  	iSprite3DState));
  if (state)
  {
    const char* act = state->GetCurAction ()->GetName ();
    return act;
  }
  return 0;
}

void celPcMesh::Hide ()
{
  if (!visible) return;
  visible = false;
  if (mesh) mesh->GetFlags ().Set (CS_ENTITY_INVISIBLE);
}

void celPcMesh::Show ()
{
  if (visible) return;
  visible = true;
  if (mesh) mesh->GetFlags ().Reset (CS_ENTITY_INVISIBLE);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcMeshSelect)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMeshSelect)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMeshSelect::PcMeshSelect)
  SCF_IMPLEMENTS_INTERFACE (iPcMeshSelect)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcMeshSelect::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

csStringID celPcMeshSelect::id_x = csInvalidStringID;
csStringID celPcMeshSelect::id_y = csInvalidStringID;
csStringID celPcMeshSelect::id_button = csInvalidStringID;
csStringID celPcMeshSelect::id_entity = csInvalidStringID;

celPcMeshSelect::celPcMeshSelect (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMeshSelect);
  scfiEventHandler = 0;
  pccamera = 0;

  sel_entity = 0;
  cur_on_top = false;
  mouse_buttons = CEL_MOUSE_BUTTON1;

  drag_normal.Set (0, 0, 1);
  drag_normal_camera = true;

  // Initialize default behaviour.
  do_global = false;
  do_drag = false;
  do_follow = false;
  do_follow_always = false;
  do_sendup = true;
  do_senddown = true;
  do_sendmove = false;
  
  // Initialize the maximum selection distance to a very large number
  max_distance = 100000;

  if (id_x == csInvalidStringID)
  {
    id_x = pl->FetchStringID ("cel.parameter.x");
    id_y = pl->FetchStringID ("cel.parameter.y");
    id_button = pl->FetchStringID ("cel.parameter.button");
    id_entity = pl->FetchStringID ("cel.parameter.entity");
  }
  params = new celGenericParameterBlock (4);
  params->SetParameterDef (0, id_x, "x");
  params->SetParameterDef (1, id_y, "y");
  params->SetParameterDef (2, id_button, "button");
  params->SetParameterDef (3, id_entity, "entity");

  SetupEventHandler ();
  DG_TYPE (this, "celPcMeshSelect()");
}

celPcMeshSelect::~celPcMeshSelect ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    if (q)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
  SetCamera (0);
  delete params;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcMeshSelect);
}

void celPcMeshSelect::SetupEventHandler ()
{
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_MouseDown | CSMASK_MouseUp;
  if (do_drag || do_follow || do_sendmove) trigger |= CSMASK_MouseMove;
  q->RegisterListener (scfiEventHandler, trigger);
}

#define MESHSEL_SERIAL 1

csPtr<iCelDataBuffer> celPcMeshSelect::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MESHSEL_SERIAL);
  csRef<iCelPropertyClass> pc;
  if (pccamera) pc = SCF_QUERY_INTERFACE (pccamera, iCelPropertyClass);
  databuf->Add (pc);
  databuf->Add (sel_entity);
  databuf->Add (cur_on_top);
  databuf->Add ((uint32)mouse_buttons);
  databuf->Add (do_global);
  databuf->Add (do_drag);
  databuf->Add (drag_normal);
  databuf->Add (drag_normal_camera);
  databuf->Add (do_follow);
  databuf->Add (do_follow_always);
  databuf->Add (do_sendmove);
  databuf->Add (do_sendup);
  databuf->Add (do_senddown);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMeshSelect::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MESHSEL_SERIAL)
  {
    Report (object_reg,"serialnr != MESHSEL_SERIAL.  Cannot load.");
    return false;
  }

  csRef<iPcCamera> pcm;
  pcm = SCF_QUERY_INTERFACE (databuf->GetPC (), iPcCamera);
  SetCamera (pcm);

  sel_entity = databuf->GetEntity ();
  cur_on_top = databuf->GetBool ();
  mouse_buttons = databuf->GetUInt32 ();
  do_global = databuf->GetBool ();
  do_drag = databuf->GetBool ();
  databuf->GetVector3 (drag_normal);
  drag_normal_camera = databuf->GetBool ();
  do_follow = databuf->GetBool ();
  do_follow_always = databuf->GetBool ();
  do_sendmove = databuf->GetBool ();
  do_sendup = databuf->GetBool ();
  do_senddown = databuf->GetBool ();

  SetupEventHandler ();

  return true;
}

void celPcMeshSelect::SendMessage (const char* msg, iCelEntity* ent,
	int x, int y, int but)
{
  iCelBehaviour* bh = entity->GetBehaviour ();
  CS_ASSERT (bh != 0);
  params->GetParameter (0).Set ((int32)x);
  params->GetParameter (1).Set ((int32)y);
  params->GetParameter (2).Set ((int32)but);
  params->GetParameter (3).Set (ent);
  celData ret;
  bh->SendMessage (msg, this, ret, params);
}


bool celPcMeshSelect::HandleEvent (iEvent& ev)
{
  if (!pccamera) return false;
  iCamera* camera = pccamera->GetCamera ();

  int mouse_but = ev.Mouse.Button;
  int but = 1<<(mouse_but-1);
  if (do_follow || do_drag)
  {
    if (mouse_but != 0 && !(mouse_buttons & but)) return false;
  }
  else
  {
    if (!(mouse_buttons & but)) return false;
  }

  bool mouse_down = ev.Type == csevMouseDown;
  bool mouse_up = ev.Type == csevMouseUp;
  int mouse_x = ev.Mouse.x;
  int mouse_y = ev.Mouse.y;

  iCelEntity* new_sel = 0;

  // The following vectors are only set if needed.

  // Vector from (0,0,0) to 'vc' in camera space corresponding to 
  // the point we clicked on.
  csVector3 vc;
  // Vector from 'vo' to 'vw' in world space corresponding to
  // same vector.
  csVector3 vo, vw;

  if (mouse_down || do_follow_always || ((do_follow || do_drag) && sel_entity))
  {
    // Setup perspective vertex, invert mouse Y axis.
    csVector2 p (mouse_x, camera->GetShiftY() * 2 - mouse_y);

    camera->InvPerspective (p, 1, vc);
    vw = camera->GetTransform ().This2Other (vc);

    iSector* sector = camera->GetSector ();
    if (sector)
    {
      vo = camera->GetTransform ().GetO2TTranslation ();
      csVector3 isect, end = vo + (vw - vo) * (int)max_distance;

      iMeshWrapper* sel = sector->HitBeam (vo, end, isect, 0, true);
      if (sel)
      {
        iObject* sel_obj = sel->QueryObject ();
        new_sel = pl->FindAttachedEntity (sel_obj);
      }
    } 
  }

  if (do_drag && sel_entity)
  {
    csRef<iPcMovable> pcmovable (CEL_QUERY_PROPCLASS (
    	sel_entity->GetPropertyClassList (), iPcMovable));
    csRef<iPcMesh> pcmesh;
    if (pcmovable)
      pcmesh = pcmovable->GetMesh ();
    else
      pcmesh = CEL_QUERY_PROPCLASS (
      	sel_entity->GetPropertyClassList (), iPcMesh);
    CS_ASSERT (pcmesh != 0);
    iMeshWrapper* mesh = pcmesh->GetMesh ();
    CS_ASSERT (mesh != 0);
    csVector3 mp = mesh->GetMovable ()->GetPosition ();

    csVector3 v0, v1;
    if (drag_normal_camera)
    {
      v0.Set (0.0f);
      v1 = vc;
      mp = camera->GetTransform ().Other2This (mp);
    }
    else
    {
      v0 = vo;
      v1 = vw;
    }
    csVector3 isect;
    float dist;
    if (csIntersect3::SegmentPlane (v0, v1, drag_normal, mp, isect, dist))
    {
      if (drag_normal_camera)
        isect = camera->GetTransform ().This2Other (isect);
      if (pcmovable)
      {
        pcmovable->Move (isect-mp);
      }
      else
      {
        iSector* sector = mesh->GetMovable ()->GetSectors ()->Get (0);
        pcmesh->MoveMesh (sector, isect);
      }
    }
  }

  if (do_follow)
  {
    if (mouse_down)
    {
      if (do_global || new_sel == entity)
        sel_entity = new_sel;
      if (do_senddown && sel_entity)
        SendMessage ("pcmeshsel_down", sel_entity,
		mouse_x, mouse_y, mouse_but);
      if (sel_entity) cur_on_top = true;
      else cur_on_top = false;
    }
    else if (sel_entity) // For mouse_up and move.
    {
      bool old_cur_on_top = cur_on_top;
      cur_on_top = (new_sel == sel_entity);
      if (do_senddown && cur_on_top && (cur_on_top != old_cur_on_top))
        SendMessage ("pcmeshsel_down", sel_entity,
		mouse_x, mouse_y, mouse_but);
      else if (do_sendup && ((mouse_up && cur_on_top) ||
      		!cur_on_top && (cur_on_top != old_cur_on_top)))
        SendMessage ("pcmeshsel_up", sel_entity,
		mouse_x, mouse_y, mouse_but);
      else if (do_sendmove)
        SendMessage ("pcmeshsel_move", sel_entity,
		mouse_x, mouse_y, mouse_but);
      if (mouse_up) sel_entity = 0;
    }
    else if (do_follow_always && do_sendmove && new_sel)
      SendMessage ("pcmeshsel_move", new_sel,
		mouse_x, mouse_y, mouse_but);
  }
  else
  {
    if (mouse_down)
    {
      if (do_global || new_sel == entity)
        sel_entity = new_sel;
      if (do_senddown && sel_entity)
        SendMessage ("pcmeshsel_down", sel_entity,
		mouse_x, mouse_y, mouse_but);
    }
    else if (mouse_up)
    {
      if (do_sendup && sel_entity)
        SendMessage ("pcmeshsel_up", sel_entity,
		mouse_x, mouse_y, mouse_but);
      sel_entity = 0;
    }
    else
    {
      if (do_sendmove)
	if (sel_entity)
          SendMessage ("pcmeshsel_move", sel_entity,
		mouse_x, mouse_y, mouse_but);
        else if (new_sel)
          SendMessage ("pcmeshsel_move", new_sel,
		mouse_x, mouse_y, mouse_but);
    }
  }

  return false;
}

void celPcMeshSelect::SetCamera (iPcCamera* pccamera)
{
#if defined (CS_DEBUG) && defined (CS_USE_GRAPHDEBUG)
  if (celPcMeshSelect::pccamera)
  {
    csRef<iCelPropertyClass> pc (
       	SCF_QUERY_INTERFACE (celPcMeshSelect::pccamera,
  	  iCelPropertyClass));
    DG_UNLINK (this, pc);
  }
#endif                                                   
  celPcMeshSelect::pccamera = pccamera;
#if defined (CS_DEBUG) && defined (CS_USE_GRAPHDEBUG)
  if (pccamera)
  {
    csRef<iCelPropertyClass> pc2 (SCF_QUERY_INTERFACE (pccamera,
      	  iCelPropertyClass));
    DG_LINK (this, pc2);
  }
#endif
}

//---------------------------------------------------------------------------

