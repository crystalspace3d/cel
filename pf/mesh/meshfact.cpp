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
#include "pf/move.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "bl/behave.h"
#include "csutil/util.h"
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
}

celPcMesh::~celPcMesh ()
{
  if (mesh)
  {
    mesh->DecRef ();
    iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
    CS_ASSERT (engine != NULL);
    engine->GetMeshes ()->Remove (mesh);
    engine->DecRef ();
  }
}

void celPcMesh::SetEntity (iCelEntity* entity)
{
  celPcMesh::entity = entity;
}

iCelDataBuffer* celPcMesh::GetDataBuffer ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer ();
  pl->DecRef ();
  return databuf;
}

void celPcMesh::Save (iCelDataBuffer* databuf)
{
  (void)databuf;
}

void celPcMesh::Load (iCelDataBuffer* databuf)
{
  (void)databuf;
}

iMeshFactoryWrapper* celPcMesh::LoadMeshFactory (const char* fileName)
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
    if (!meshfact) meshfact = LoadMeshFactory (filename);
    if (meshfact)
    {
      mesh = engine->CreateMeshWrapper (meshfact, factname/*@@@?*/);
      iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      pl->AttachEntity (mesh->QueryObject (), entity);
      pl->DecRef ();
    }
  }

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
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMeshSelect::PcMeshSelect)
  SCF_IMPLEMENTS_INTERFACE (iPcMeshSelect)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMeshSelect::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcMeshSelect::PcMeshSelectData)
  SCF_IMPLEMENTS_INTERFACE (iPcMeshSelectData)
SCF_IMPLEMENT_IBASE_END

celPcMeshSelect::celPcMeshSelect (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMeshSelect);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiEventHandler);
  celPcMeshSelect::object_reg = object_reg;
  camera = NULL;

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
}

celPcMeshSelect::~celPcMeshSelect ()
{
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  if (q)
  {
    q->RemoveListener (&scfiEventHandler);
    q->DecRef ();
  }
}

void celPcMeshSelect::SetupEventHandler ()
{
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  q->RemoveListener (&scfiEventHandler);
  unsigned int trigger = CSMASK_MouseDown | CSMASK_MouseUp;
  if (do_drag || do_follow || do_sendmove) trigger |= CSMASK_MouseMove;
  q->RegisterListener (&scfiEventHandler, trigger);
  q->DecRef ();
}

void celPcMeshSelect::SetEntity (iCelEntity* entity)
{
  celPcMeshSelect::entity = entity;
}

iCelDataBuffer* celPcMeshSelect::GetDataBuffer ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer ();
  pl->DecRef ();
  return databuf;
}

void celPcMeshSelect::Save (iCelDataBuffer* databuf)
{
  (void)databuf;
}

void celPcMeshSelect::Load (iCelDataBuffer* databuf)
{
  (void)databuf;
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
  if (!camera) return false;

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

void celPcMeshSelect::SetCamera (iCamera* camera)
{
  celPcMeshSelect::camera = camera;
}

//---------------------------------------------------------------------------

