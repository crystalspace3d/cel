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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "pf/mesh/meshfact.h"
#include "pf/camera.h"
#include "pf/move.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/persist.h"
#include "bl/behave.h"
#include "csutil/util.h"
#include "csutil/debug.h"
#include "csutil/csobject.h"
#include "csutil/flags.h"
#include "iutil/object.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/objreg.h"
#include "iutil/eventq.h"
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

SCF_IMPLEMENT_FACTORY (celPfMesh)

SCF_EXPORT_CLASS_TABLE (pfmesh)
  SCF_EXPORT_CLASS (celPfMesh, "cel.pcfactory.mesh",
  	"CEL Mesh Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfMesh)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfMesh::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfMesh::celPfMesh (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfMesh::~celPfMesh ()
{
}

bool celPfMesh::Initialize (iObjectRegistry* object_reg)
{
  celPfMesh::object_reg = object_reg;
  return true;
}

iCelPropertyClass* celPfMesh::CreatePropertyClass (const char* type)
{
  if (!strcmp (type, "pcmesh"))
    return new celPcMesh (object_reg);
  else if (!strcmp (type, "pcmeshselect"))
    return new celPcMeshSelect (object_reg);
  else return NULL;
}

const char* celPfMesh::GetTypeName (int idx) const
{
  switch (idx)
  {
    case 0: return "pcmesh";
    case 1: return "pcmeshselect";
    default: return NULL;
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcMesh)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMesh)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMesh::PcMesh)
  SCF_IMPLEMENTS_INTERFACE (iPcMesh)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMesh::celPcMesh (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMesh);
  mesh = NULL;
  celPcMesh::object_reg = object_reg;
  visible = true;
  fileName = NULL;
  factName = NULL;
  factory_ptr = NULL;
  entity = NULL;
  DG_ADDI (this, "celPcMesh()");
}

celPcMesh::~celPcMesh ()
{
  Clear ();
  DG_REM (this);
}

void celPcMesh::Clear ()
{
  delete[] fileName; fileName = NULL;
  delete[] factName; factName = NULL;
  if (mesh)
  {
    iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    if (pl)
    {
      pl->UnattachEntity (mesh->QueryObject (), entity);
      pl->DecRef ();
    }

    iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
    CS_ASSERT (engine != NULL);
    engine->GetMeshes ()->Remove (mesh);
    mesh->DecRef ();
    engine->DecRef ();
    mesh = NULL;
  }
  if (factory_ptr) { factory_ptr->DecRef (); factory_ptr = NULL; }
}

void celPcMesh::SetEntity (iCelEntity* entity)
{
  celPcMesh::entity = entity;
}

#define MESH_SERIAL 1

iCelDataBuffer* celPcMesh::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (MESH_SERIAL);
  pl->DecRef ();
  iMovable* mov = mesh->GetMovable ();
  iSectorList* sl = mov->GetSectors ();
  databuf->SetDataCount (4+1+sl->GetCount ()+3+9);
  int i, j = 0;
  databuf->GetData (j++)->Set (factName);
  databuf->GetData (j++)->Set (fileName);
  databuf->GetData (j++)->Set (GetAction ());
  databuf->GetData (j++)->Set (visible);

  databuf->GetData (j++)->Set ((uint16)(sl->GetCount ()));
  for (i = 0 ; i < sl->GetCount () ; i++)
  {
    databuf->GetData (j++)->Set (sl->Get (i)->QueryObject ()->GetName ());
  }
  csReversibleTransform& tr = mov->GetTransform ();
  databuf->GetData (j++)->Set (tr.GetO2TTranslation ().x);
  databuf->GetData (j++)->Set (tr.GetO2TTranslation ().y);
  databuf->GetData (j++)->Set (tr.GetO2TTranslation ().z);
  databuf->GetData (j++)->Set (tr.GetO2T ().m11);
  databuf->GetData (j++)->Set (tr.GetO2T ().m12);
  databuf->GetData (j++)->Set (tr.GetO2T ().m13);
  databuf->GetData (j++)->Set (tr.GetO2T ().m21);
  databuf->GetData (j++)->Set (tr.GetO2T ().m22);
  databuf->GetData (j++)->Set (tr.GetO2T ().m23);
  databuf->GetData (j++)->Set (tr.GetO2T ().m31);
  databuf->GetData (j++)->Set (tr.GetO2T ().m32);
  databuf->GetData (j++)->Set (tr.GetO2T ().m33);

  return databuf;
}

bool celPcMesh::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MESH_SERIAL) return false;
  //int cnt_total = databuf->GetDataCount ();
  Clear ();
  visible = true;
  celData* cd;
  cd = databuf->GetData (0); if (!cd) return false;
  char* factn = cd->value.s ? csStrNew (cd->value.s) : NULL;
  cd = databuf->GetData (1); if (!cd) return false;
  char* filen = cd->value.s ? csStrNew (cd->value.s) : NULL;

  SetMesh (factn, filen);
  delete[] factn;
  delete[] filen;

  cd = databuf->GetData (2); if (!cd) return false;
  SetAction (cd->value.s);
  cd = databuf->GetData (3); if (!cd) return false;
  if (cd->value.bo) Show ();
  else Hide ();

  cd = databuf->GetData (4); if (!cd) return false;
  uint16 cnt = cd->value.uw;
  mesh->GetMovable ()->ClearSectors ();
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
  int i, j = 5;
  for (i = 0 ; i < cnt ; i++)
  {
    cd = databuf->GetData (j++); if (!cd) return false;
    iSector* s = engine->GetSectors ()->FindByName (cd->value.s);
    CS_ASSERT (s != NULL);
    mesh->GetMovable ()->GetSectors ()->Add (s);
  }
  engine->DecRef ();

  csMatrix3 m_o2t;
  csVector3 v_o2t;
  cd = databuf->GetData (j++); if (!cd) return false;
  v_o2t.x = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  v_o2t.y = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  v_o2t.z = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m11 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m12 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m13 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m21 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m22 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m23 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m31 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m32 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m33 = cd->value.f;
  csReversibleTransform tr (m_o2t, v_o2t);
  mesh->GetMovable ()->SetTransform (tr);
  mesh->GetMovable ()->UpdateMove ();
  mesh->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);

  return true;
}


iMeshFactoryWrapper* celPcMesh::LoadMeshFactory ()
{
  iLoader* loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  CS_ASSERT (loader != NULL);
  iMeshFactoryWrapper* imeshfact = loader->LoadMeshObjectFactory (fileName);
  loader->DecRef ();
  if (imeshfact == NULL)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pfmesh.loadmeshfactory",
    	"Error loading mesh object factory '%s'!", fileName);
    return NULL;
  }
  imeshfact->DecRef ();
  return imeshfact;
}

void celPcMesh::SetMesh (const char* factname, const char* filename)
{
  delete[] fileName;
  fileName = csStrNew (filename);
  delete[] factName;
  factName = csStrNew (factname);

  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
  if (mesh)
  {
    mesh->DecRef ();
    engine->GetMeshes ()->Remove (mesh);
    mesh = NULL;
  }

  if (factname && filename)
  {
    iMeshFactoryWrapper* meshfact = engine->GetMeshFactories ()
    	->FindByName (factname);
    if (!meshfact)
    {
      meshfact = LoadMeshFactory ();
      if (meshfact)
      {
        // Cache the factory.
        iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
        CS_ASSERT (pl != NULL);
	pl->Cache (meshfact);
        pl->DecRef ();
        meshfact->IncRef ();
      }
    }
    else
    {
      meshfact->IncRef ();
    }

    if (factory_ptr) { factory_ptr->DecRef (); factory_ptr = NULL; }
    if (meshfact)
    {
      factory_ptr = meshfact;

      mesh = engine->CreateMeshWrapper (meshfact, factname/*@@@?*/);
      iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      pl->AttachEntity (mesh->QueryObject (), entity);
      pl->DecRef ();
    }
  }

  engine->DecRef ();
}

void celPcMesh::SetMesh (iMeshWrapper* m)
{
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
  if (mesh)
  {
    mesh->DecRef ();
    engine->GetMeshes ()->Remove (mesh);
    mesh = NULL;
  }

  mesh = m;
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  pl->AttachEntity (mesh->QueryObject (), entity);
  pl->DecRef ();

  engine->DecRef ();
}

void celPcMesh::CreateEmptyThing ()
{
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
  if (mesh)
  {
    mesh->DecRef ();
    engine->GetMeshes ()->Remove (mesh);
    mesh = NULL;
  }

  iMeshObjectType* thing_type = engine->GetThingType ();
  iMeshObjectFactory* thing_fact = thing_type->NewFactory ();
  iMeshObject* thing_obj = SCF_QUERY_INTERFACE_FAST (thing_fact, iMeshObject);
  thing_fact->DecRef ();

  mesh = engine->CreateMeshWrapper (thing_obj, entity->GetName (), NULL,
		  csVector3 (0));

  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  pl->AttachEntity (mesh->QueryObject (), entity);
  pl->DecRef ();

  thing_obj->DecRef ();
}

void celPcMesh::MoveMesh (iSector* sector, const csVector3& pos)
{
  CS_ASSERT (mesh != NULL);
  mesh->GetMovable ()->SetSector (sector);
  mesh->GetMovable ()->SetPosition (pos);
  mesh->GetMovable ()->UpdateMove ();
  mesh->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);
}

void celPcMesh::SetAction (const char* actionName)
{
  if (!actionName) return;
  CS_ASSERT (mesh != NULL);
  iSprite3DState* state = SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
  	iSprite3DState);
  if (state)
  {
    state->SetAction (actionName);
    state->DecRef ();
  }
}

const char* celPcMesh::GetAction ()
{
  CS_ASSERT (mesh != NULL);
  iSprite3DState* state = SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
  	iSprite3DState);
  if (state)
  {
    const char* act = state->GetCurAction ()->GetName ();
    state->DecRef ();
    return act;
  }
  return NULL;
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

SCF_IMPLEMENT_IBASE (celPcMeshSelect)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMeshSelect)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMeshSelect::PcMeshSelect)
  SCF_IMPLEMENTS_INTERFACE (iPcMeshSelect)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcMeshSelect::PcMeshSelectData)
  SCF_IMPLEMENTS_INTERFACE (iPcMeshSelectData)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_IBASE (celPcMeshSelect::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcMeshSelect::celPcMeshSelect (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMeshSelect);
  celPcMeshSelect::object_reg = object_reg;
  scfiEventHandler = NULL;
  pccamera = NULL;

  sel_entity = NULL;
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

  SetupEventHandler ();
  DG_ADDI (this, "celPcMeshSelect()");
}

celPcMeshSelect::~celPcMeshSelect ()
{
  if (scfiEventHandler)
  {
    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q)
    {
      q->RemoveListener (scfiEventHandler);
      q->DecRef ();
    }
    scfiEventHandler->DecRef ();
  }
  SetCamera (NULL);
  DG_REM (this);
}

void celPcMeshSelect::SetupEventHandler ()
{
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_MouseDown | CSMASK_MouseUp;
  if (do_drag || do_follow || do_sendmove) trigger |= CSMASK_MouseMove;
  q->RegisterListener (scfiEventHandler, trigger);
  q->DecRef ();
}

void celPcMeshSelect::SetEntity (iCelEntity* entity)
{
  celPcMeshSelect::entity = entity;
}

#define MESHSEL_SERIAL 1

iCelDataBuffer* celPcMeshSelect::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (MESHSEL_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (15);
  iCelPropertyClass* pc = NULL;
  if (pccamera) pc = SCF_QUERY_INTERFACE_FAST (pccamera, iCelPropertyClass);
  databuf->GetData (0)->Set (pc);
  if (pc) pc->DecRef ();
  databuf->GetData (1)->Set (sel_entity);
  databuf->GetData (2)->Set (cur_on_top);
  databuf->GetData (3)->Set ((uint32)mouse_buttons);
  databuf->GetData (4)->Set (do_global);
  databuf->GetData (5)->Set (do_drag);
  databuf->GetData (6)->Set (drag_normal.x);
  databuf->GetData (7)->Set (drag_normal.y);
  databuf->GetData (8)->Set (drag_normal.z);
  databuf->GetData (9)->Set (drag_normal_camera);
  databuf->GetData (10)->Set (do_follow);
  databuf->GetData (11)->Set (do_follow_always);
  databuf->GetData (12)->Set (do_sendmove);
  databuf->GetData (13)->Set (do_sendup);
  databuf->GetData (14)->Set (do_senddown);
  return databuf;
}

bool celPcMeshSelect::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MESHSEL_SERIAL) return false;
  if (databuf->GetDataCount () != 15) return false;
  celData* cd;
  cd = databuf->GetData (0); if (!cd) return false;
  iPcCamera* pcm = NULL;
  if (cd->value.pc) pcm = SCF_QUERY_INTERFACE_FAST (cd->value.pc, iPcCamera);
  SetCamera (pcm);
  if (pcm) pcm->DecRef ();

  cd = databuf->GetData (1); if (!cd) return false;
  sel_entity = cd->value.ent;
  cd = databuf->GetData (2); if (!cd) return false;
  cur_on_top = cd->value.bo;
  cd = databuf->GetData (3); if (!cd) return false;
  mouse_buttons = cd->value.ul;
  cd = databuf->GetData (4); if (!cd) return false;
  do_global = cd->value.bo;
  cd = databuf->GetData (5); if (!cd) return false;
  do_drag = cd->value.bo;
  cd = databuf->GetData (6); if (!cd) return false;
  drag_normal.x = cd->value.f;
  cd = databuf->GetData (7); if (!cd) return false;
  drag_normal.y = cd->value.f;
  cd = databuf->GetData (8); if (!cd) return false;
  drag_normal.z = cd->value.f;
  cd = databuf->GetData (9); if (!cd) return false;
  drag_normal_camera = cd->value.bo;
  cd = databuf->GetData (10); if (!cd) return false;
  do_follow = cd->value.bo;
  cd = databuf->GetData (11); if (!cd) return false;
  do_follow_always = cd->value.bo;
  cd = databuf->GetData (12); if (!cd) return false;
  do_sendmove = cd->value.bo;
  cd = databuf->GetData (13); if (!cd) return false;
  do_sendup = cd->value.bo;
  cd = databuf->GetData (14); if (!cd) return false;
  do_senddown = cd->value.bo;

  SetupEventHandler ();

  return true;
}

void celPcMeshSelect::SendMessage (const char* msg, iCelEntity* ent,
	int x, int y, int but)
{
  iCelBehaviour* bh = entity->GetBehaviour ();
  CS_ASSERT (bh != NULL);
  mesh_sel_data.Select (ent, x, y, but);
  bh->SendMessage (msg, &mesh_sel_data);
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

  iCelEntity* new_sel = NULL;

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
    vo = camera->GetTransform ().GetO2TTranslation ();
    csVector3 isect, end = vo + (vw - vo) * 60;

    iMeshWrapper* sel = sector->HitBeam (vo, end, isect, NULL);
    if (sel)
    {
      iObject* sel_obj = sel->QueryObject ();
      iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      new_sel = pl->FindAttachedEntity (sel_obj);
      pl->DecRef ();
    }
  }

  if (do_drag && sel_entity)
  {
    iPcMovable* pcmovable = CEL_QUERY_PROPCLASS (
    	sel_entity->GetPropertyClassList (), iPcMovable);
    iPcMesh* pcmesh = NULL;
    if (pcmovable)
      pcmesh = pcmovable->GetMesh ();
    else
      pcmesh = CEL_QUERY_PROPCLASS (
      	sel_entity->GetPropertyClassList (), iPcMesh);
    CS_ASSERT (pcmesh != NULL);
    iMeshWrapper* mesh = pcmesh->GetMesh ();
    CS_ASSERT (mesh != NULL);
    csVector3 mp = mesh->GetMovable ()->GetPosition ();

    csVector3 v0, v1;
    if (drag_normal_camera)
    {
      v0.Set (0);
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

    if (pcmovable) pcmovable->DecRef ();
    else if (pcmesh) pcmesh->DecRef ();
  }

  if (do_follow)
  {
    if (mouse_down)
    {
      if (do_global || new_sel == entity)
        sel_entity = new_sel;
      if (do_senddown && sel_entity)
        SendMessage ("selectmesh_down", sel_entity,
		mouse_x, mouse_y, mouse_but);
      if (sel_entity) cur_on_top = true;
      else cur_on_top = false;
    }
    else if (sel_entity) // For mouse_up and move.
    {
      bool old_cur_on_top = cur_on_top;
      cur_on_top = (new_sel == sel_entity);
      if (do_senddown && cur_on_top && (cur_on_top != old_cur_on_top))
        SendMessage ("selectmesh_down", sel_entity,
		mouse_x, mouse_y, mouse_but);
      else if (do_sendup && ((mouse_up && cur_on_top) ||
      		!cur_on_top && (cur_on_top != old_cur_on_top)))
        SendMessage ("selectmesh_up", sel_entity,
		mouse_x, mouse_y, mouse_but);
      else if (do_sendmove)
        SendMessage ("selectmesh_move", sel_entity,
		mouse_x, mouse_y, mouse_but);
      if (mouse_up) sel_entity = NULL;
    }
    else if (do_follow_always && do_sendmove && new_sel)
      SendMessage ("selectmesh_move", new_sel,
		mouse_x, mouse_y, mouse_but);
  }
  else
  {
    if (mouse_down)
    {
      if (do_global || new_sel == entity)
        sel_entity = new_sel;
      if (do_senddown && sel_entity)
        SendMessage ("selectmesh_down", sel_entity,
		mouse_x, mouse_y, mouse_but);
    }
    else if (mouse_up)
    {
      if (do_sendup && sel_entity)
        SendMessage ("selectmesh_up", sel_entity,
		mouse_x, mouse_y, mouse_but);
      sel_entity = NULL;
    }
    else
    {
      if (do_sendmove)
	if (sel_entity)
          SendMessage ("selectmesh_move", sel_entity,
		mouse_x, mouse_y, mouse_but);
        else if (new_sel)
          SendMessage ("selectmesh_move", new_sel,
		mouse_x, mouse_y, mouse_but);
    }
  }

  return false;
}

void celPcMeshSelect::SetCamera (iPcCamera* pccamera)
{
  if (pccamera == celPcMeshSelect::pccamera) return;
  if (pccamera)
  {
    pccamera->IncRef ();
#if defined (CS_DEBUG) && defined (CS_USE_GRAPHDEBUG)
    iCelPropertyClass* pc = SCF_QUERY_INTERFACE_FAST (pccamera,
    	iCelPropertyClass);
    DG_LINK (this, pc);
    pc->DecRef ();
#endif
  }
  if (celPcMeshSelect::pccamera)
  {
#if defined (CS_DEBUG) && defined (CS_USE_GRAPHDEBUG)
    iCelPropertyClass* pc = SCF_QUERY_INTERFACE_FAST (celPcMeshSelect::pccamera,
    	iCelPropertyClass);
    DG_UNLINK (this, pc);
    pc->DecRef ();
#endif
    celPcMeshSelect::pccamera->DecRef ();
  }
  celPcMeshSelect::pccamera = pccamera;
}

//---------------------------------------------------------------------------

