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
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/engine.h"
#include "imap/parser.h"
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
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistance",
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
  fileName = 0;
  factName = 0;
  factory_ptr = 0;

  if (action_loadmesh == csInvalidStringID)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    action_loadmesh = pl->FetchStringID ("cel.property.LoadMesh");
  }
}

celPcMesh::~celPcMesh ()
{
  Clear ();
}

void celPcMesh::Clear ()
{
  delete[] fileName; fileName = 0;
  delete[] factName; factName = 0;
  if (mesh)
  {
    csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
    if (pl)
      pl->UnattachEntity (mesh->QueryObject (), entity);

    csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
    CS_ASSERT (engine != 0);
    engine->RemoveObject (mesh);
    mesh = 0;
    FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
  }
  factory_ptr = 0;
}

bool celPcMesh::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_loadmesh)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    const celData* p_file = params->GetParameter (pl->FetchStringID (
    	"cel.parameter.filename"));
    if (!p_file) return false;
    if (p_file->type != CEL_DATA_STRING) return false;
    const celData* p_fact = params->GetParameter (pl->FetchStringID (
    	"cel.parameter.factoryname"));
    if (!p_fact) return false;
    if (p_fact->type != CEL_DATA_STRING) return false;
    bool rc = SetMesh ((const char*)*(p_fact->value.s),
    		       (const char*)*(p_file->value.s));
    // @@@ Error report!
    (void)rc;
    return true;
  }
  return false;
}

#define MESH_SERIAL 1

csPtr<iCelDataBuffer> celPcMesh::Save ()
{
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (MESH_SERIAL));
  iMovable* mov = mesh->GetMovable ();
  iSectorList* sl = mov->GetSectors ();
  databuf->SetDataCount (4+1+sl->GetCount ()+1+9);
  int i, j = 0;
  databuf->GetData (j++)->Set (factName);
  databuf->GetData (j++)->Set (fileName);
  databuf->GetData (j++)->Set (GetAction ());
  databuf->GetData (j++)->SetBool (visible);

  databuf->GetData (j++)->Set ((uint16)(sl->GetCount ()));
  for (i = 0 ; i < sl->GetCount () ; i++)
  {
    databuf->GetData (j++)->Set (sl->Get (i)->QueryObject ()->GetName ());
  }
  csReversibleTransform& tr = mov->GetTransform ();
  databuf->GetData (j++)->Set (tr.GetO2TTranslation ());
  databuf->GetData (j++)->Set (tr.GetO2T ().m11);
  databuf->GetData (j++)->Set (tr.GetO2T ().m12);
  databuf->GetData (j++)->Set (tr.GetO2T ().m13);
  databuf->GetData (j++)->Set (tr.GetO2T ().m21);
  databuf->GetData (j++)->Set (tr.GetO2T ().m22);
  databuf->GetData (j++)->Set (tr.GetO2T ().m23);
  databuf->GetData (j++)->Set (tr.GetO2T ().m31);
  databuf->GetData (j++)->Set (tr.GetO2T ().m32);
  databuf->GetData (j++)->Set (tr.GetO2T ().m33);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMesh::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MESH_SERIAL)
  {
    Report (object_reg,"Serialnr != MESH_SERIAL.  Cannot load.");
    return false;
  }

  //int cnt_total = databuf->GetDataCount ();
  Clear ();
  visible = true;
  celData* cd;
  cd = databuf->GetData (0);
  if (!cd)
  {
    Report (object_reg,"Factory name not specified.  Cannot load.");
    return false;
  }
  char* factn = cd->value.s ? csStrNew (*cd->value.s) : 0;
  cd = databuf->GetData (1);
  if (!cd)
  {
    Report (object_reg,"Filename not specified.  Cannot load.");
    return false;
  }
  char* filen = cd->value.s ? csStrNew (*cd->value.s) : 0;

  SetMesh (factn, filen);
  delete[] factn;
  delete[] filen;

  cd = databuf->GetData (2);
  if (!cd)
  {
    Report (object_reg,"Action not specified.  Cannot load.");
    return false;
  }
  SetAction (*cd->value.s, true);
  cd = databuf->GetData (3); 
  if (!cd)
  {
    Report (object_reg,"Visibility flag not specified.  Cannot load.");
    return false;
  }
  if (cd->value.bo) Show ();
  else Hide ();

  cd = databuf->GetData (4);
  if (!cd)
  {
    Report (object_reg,"# of sectors in sectorlist not specified.  Cannot load.");
    return false;
  }
  uint16 cnt = cd->value.uw;
  mesh->GetMovable ()->ClearSectors ();
  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
  CS_ASSERT (engine != 0);
  int i, j = 5;
  for (i = 0 ; i < cnt ; i++)
  {
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"Sector name not specified for element %d.  Cannot load.",i);
      return false;
    }
    iSector* s = engine->GetSectors ()->FindByName (*cd->value.s);
    CS_ASSERT (s != 0);
    mesh->GetMovable ()->GetSectors ()->Add (s);
  }

  csMatrix3 m_o2t;
  csVector3 v_o2t;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"Transform vector not specified.  Cannot load.");
    return false;
  }
  v_o2t.x = cd->value.v.x;
  v_o2t.y = cd->value.v.y;
  v_o2t.z = cd->value.v.z;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"o2t.m11 not specified.  Cannot load.");
    return false;
  }

  m_o2t.m11 = cd->value.f;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"o2t.m12 not specified.  Cannot load.");
    return false;
  }
  m_o2t.m12 = cd->value.f;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"o2t.m13 not specified.  Cannot load.");
    return false;
  }
  m_o2t.m13 = cd->value.f;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"o2t.m21 not specified.  Cannot load.");
    return false;
  }
  m_o2t.m21 = cd->value.f;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"o2t.m22 not specified.  Cannot load.");
    return false;
  }
  m_o2t.m22 = cd->value.f;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"o2t.m23 not specified.  Cannot load.");
    return false;
  }
  m_o2t.m23 = cd->value.f;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"o2t.m31 not specified.  Cannot load.");
    return false;
  }
  m_o2t.m31 = cd->value.f;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"o2t.m32 not specified.  Cannot load.");
    return false;
  }
  m_o2t.m32 = cd->value.f;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"o2t.m33 not specified.  Cannot load.");
    return false;
  }
  m_o2t.m33 = cd->value.f;
  csReversibleTransform tr (m_o2t, v_o2t);
  mesh->GetMovable ()->SetTransform (tr);
  mesh->GetMovable ()->UpdateMove ();

  return true;
}

iMeshFactoryWrapper* celPcMesh::LoadMeshFactory ()
{
  csRef<iLoader> loader (CS_QUERY_REGISTRY (object_reg, iLoader));
  CS_ASSERT (loader != 0);
  csRef<iMeshFactoryWrapper> imeshfact (
  	loader->LoadMeshObjectFactory (fileName));
  if (imeshfact == 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pfmesh.loadmeshfactory",
    	"Error loading mesh object factory '%s'!", fileName);
    return 0;
  }
  return imeshfact;
}

bool celPcMesh::SetMesh (const char* factname, const char* filename)
{
  if (filename != fileName)
  {
    delete[] fileName;
    fileName = csStrNew (filename);
  }
  if (factname != factName)
  {
    delete[] factName;
    factName = csStrNew (factname);
  }

  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != 0);
  if (mesh)
  {
    engine->RemoveObject (mesh);
    mesh = 0;
    FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
  }

  if (factname && filename)
  {
    csRef<iMeshFactoryWrapper> meshfact = engine->GetMeshFactories ()
				        	->FindByName (factname);
    if (!meshfact)
    {
      meshfact = LoadMeshFactory ();
      if (meshfact)
      {
        // Cache the factory.
        csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
        CS_ASSERT (pl != 0);
	    pl->Cache (meshfact);
      }
    }

    if (factory_ptr)
      factory_ptr = 0;
    if (meshfact)
    {
      factory_ptr = meshfact;

      mesh = engine->CreateMeshWrapper (meshfact, factname/*@@@?*/);
      csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
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
  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
  CS_ASSERT (engine != 0);
  if (mesh)
  {
    engine->RemoveObject (mesh);
    mesh = 0;
    FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
  }

  mesh = m;
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  pl->AttachEntity (mesh->QueryObject (), entity);
  FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
}

void celPcMesh::CreateEmptyThing ()
{
  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
  CS_ASSERT (engine != 0);
  if (mesh)
  {
    engine->RemoveObject (mesh);
    mesh = 0;
    FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
  }

  mesh = engine->CreateThingMesh (0, 0);

  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  pl->AttachEntity (mesh->QueryObject (), entity);
  FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
}

void celPcMesh::MoveMesh (iSector* sector, const csVector3& pos)
{
  CS_ASSERT (mesh != 0);
  if (sector)
      mesh->GetMovable ()->SetSector (sector);

  mesh->GetMovable ()->SetPosition (pos);
  mesh->GetMovable ()->UpdateMove ();
}

void celPcMesh::SetAction (const char* actionName, bool resetaction)
{
  if (!actionName) return;
  CS_ASSERT (mesh != 0);
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
  CS_ASSERT (mesh != 0);
  csRef<iSprite3DState> state (SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
  	iSprite3DState));
  if (state)
  {
      state->SetReverseAction(reverse);
  }
}

const char* celPcMesh::GetAction ()
{
  CS_ASSERT (mesh != 0);
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
  mesh->GetFlags ().Set (CS_ENTITY_INVISIBLE);
}

void celPcMesh::Show ()
{
  if (visible) return;
  visible = true;
  mesh->GetFlags ().Reset (CS_ENTITY_INVISIBLE);
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
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
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
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (MESHSEL_SERIAL));
  databuf->SetDataCount (13);
  csRef<iCelPropertyClass> pc;
  if (pccamera) pc = SCF_QUERY_INTERFACE (pccamera, iCelPropertyClass);
  databuf->GetData (0)->Set (pc);
  databuf->GetData (1)->Set (sel_entity);
  databuf->GetData (2)->SetBool (cur_on_top);
  databuf->GetData (3)->Set ((uint32)mouse_buttons);
  databuf->GetData (4)->SetBool (do_global);
  databuf->GetData (5)->SetBool (do_drag);
  databuf->GetData (6)->Set (drag_normal);
  databuf->GetData (7)->SetBool (drag_normal_camera);
  databuf->GetData (8)->SetBool (do_follow);
  databuf->GetData (9)->SetBool (do_follow_always);
  databuf->GetData (10)->SetBool (do_sendmove);
  databuf->GetData (11)->SetBool (do_sendup);
  databuf->GetData (12)->SetBool (do_senddown);
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
  if (databuf->GetDataCount () != 13)
  {
    Report (object_reg,"Databuf does not have 13 data elements.  Cannot load.");
    return false;
  }
  celData* cd;
  cd = databuf->GetData (0);
  if (!cd)
  {
    Report (object_reg,"No camera specified.  Cannot load.");
    return false;
  }
  csRef<iPcCamera> pcm;
  if (cd->value.pc) pcm = SCF_QUERY_INTERFACE (cd->value.pc, iPcCamera);
  SetCamera (pcm);

  cd = databuf->GetData (1);
  if (!cd)
  {
    Report (object_reg,"Sel_entity not specified.  Cannot load.");
    return false;
  }
  sel_entity = cd->value.ent;
  cd = databuf->GetData (2);
  if (!cd)
  {
    Report (object_reg,"cur_on_top not specified.  Cannot load.");
    return false;
  }
  cur_on_top = cd->value.bo;
  cd = databuf->GetData (3);
  if (!cd) 
  {
    Report (object_reg,"mouse_buttons not specified.  Cannot load.");
    return false;
  }
  mouse_buttons = cd->value.ul;
  cd = databuf->GetData (4);
  if (!cd)
  {
    Report (object_reg,"do_global not specified.  Cannot load.");
    return false;
  }
  do_global = cd->value.bo;
  cd = databuf->GetData (5);
  if (!cd)
  {
    Report (object_reg,"do_drag not specified.  Cannot load.");
    return false;
  }
  do_drag = cd->value.bo;
  cd = databuf->GetData (6);
  if (!cd)
  {
    Report (object_reg,"drag_normal not specified.  Cannot load.");
    return false;
  }
  drag_normal.x = cd->value.v.x;
  drag_normal.y = cd->value.v.y;
  drag_normal.z = cd->value.v.z;
  cd = databuf->GetData (7);
  if (!cd)
  {
    Report (object_reg,"drag_normal_camera not specified.  Cannot load.");
    return false;
  }
  drag_normal_camera = cd->value.bo;
  cd = databuf->GetData (8);
  if (!cd)
  {
    Report (object_reg,"do_follow not specified.  Cannot load.");
    return false;
  }
  do_follow = cd->value.bo;
  cd = databuf->GetData (9);
  if (!cd)
  {
    Report (object_reg,"do_follow not specified.  Cannot load.");
    return false;
  }
  do_follow_always = cd->value.bo;
  cd = databuf->GetData (10);
  if (!cd)
  {
    Report (object_reg,"do_sendmove not specified.  Cannot load.");
    return false;
  }
  do_sendmove = cd->value.bo;
  cd = databuf->GetData (11);
  if (!cd)
  {
    Report (object_reg,"do_sendup not specified.  Cannot load.");
    return false;
  }
  do_sendup = cd->value.bo;
  cd = databuf->GetData (12);
  if (!cd)
  {
    Report (object_reg,"do_senddown not specified.  Cannot load.");
    return false;
  }
  do_senddown = cd->value.bo;

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
  bh->SendMessage (msg, ret, params);
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
        csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
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
    if (csIntersect3::Plane (v0, v1, drag_normal, mp, isect, dist))
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

