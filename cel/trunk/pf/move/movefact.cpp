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
#include "pf/move/movefact.h"
#include "pf/mesh.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "bl/behave.h"
#include "csutil/util.h"
#include "csutil/csobject.h"
#include "iutil/object.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/objreg.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/engine.h"
#include "imap/parser.h"
#include "ivaria/reporter.h"
#include "ivaria/collider.h"
#include "ivaria/polymesh.h"
#include "imesh/object.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPfMove)

SCF_EXPORT_CLASS_TABLE (pfmove)
  SCF_EXPORT_CLASS (celPfMove, "cel.pcfactory.move",
  	"CEL Movable Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfMove)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfMove::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfMove::celPfMove (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfMove::~celPfMove ()
{
}

bool celPfMove::Initialize (iObjectRegistry* object_reg)
{
  celPfMove::object_reg = object_reg;
  return true;
}

iCelPropertyClass* celPfMove::CreatePropertyClass (const char* type)
{
  if (!strcmp (type, "pcmovable"))
    return new celPcMovable (object_reg);
  else if (!strcmp (type, "pcsolid"))
    return new celPcSolid (object_reg);
  else if (!strcmp (type, "pcmovableconst_cd"))
    return new celPcMovableConstraintCD (object_reg);
  else if (!strcmp (type, "pcgravity"))
    return new celPcGravity (object_reg);
  else return NULL;
}

const char* celPfMove::GetTypeName (int idx) const
{
  switch (idx)
  {
    case 0: return "pcmovable";
    case 1: return "pcsolid";
    case 2: return "pcmovableconst_cd";
    case 3: return "pcgravity";
    default: return NULL;
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcMovable)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMovable)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMovable::PcMovable)
  SCF_IMPLEMENTS_INTERFACE (iPcMovable)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMovable::celPcMovable (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMovable);
  pcmesh = NULL;
  celPcMovable::object_reg = object_reg;
}

celPcMovable::~celPcMovable ()
{
  if (pcmesh) pcmesh->DecRef ();
  RemoveAllConstraints ();
}

void celPcMovable::SetEntity (iCelEntity* entity)
{
  celPcMovable::entity = entity;
}

iCelDataBuffer* celPcMovable::GetDataBuffer ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer ();
  pl->DecRef ();
  return databuf;
}

void celPcMovable::Save (iCelDataBuffer* databuf)
{
  (void)databuf;
}

void celPcMovable::Load (iCelDataBuffer* databuf)
{
  (void)databuf;
}

void celPcMovable::SetMesh (iPcMesh* mesh)
{
  if (mesh) mesh->IncRef ();
  if (pcmesh) pcmesh->DecRef ();
  pcmesh = mesh;
}

iPcMesh* celPcMovable::GetMesh ()
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  return pcmesh;
}

int celPcMovable::Move (iSector* sector, const csVector3& pos)
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  CS_ASSERT (pcmesh != NULL);
  csVector3 realpos;
  int i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    iPcMovableConstraint* c = (iPcMovableConstraint*)constraints[i];
    int rc = c->CheckMove (sector, pos, pos, realpos);
    if (rc == CEL_MOVE_FAIL) return rc;
  }
  pcmesh->MoveMesh (sector, pos);
  return CEL_MOVE_SUCCEED;
}

int celPcMovable::Move (const csVector3& relpos)
{
//@@@ TODO IMPLEMENT TRAVERSAL THROUGH PORTALS!
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  CS_ASSERT (pcmesh != NULL);
  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  iSector* sector = movable->GetSectors ()->Get (0);
  csVector3 start = movable->GetPosition ();
  csVector3 end = start + relpos;
  csVector3 realpos = end;
  bool partial = false;
  int i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    iPcMovableConstraint* c = (iPcMovableConstraint*)constraints[i];
    int rc = c->CheckMove (sector, start, end, realpos);
    if (rc == CEL_MOVE_FAIL) return rc;
    if (rc == CEL_MOVE_PARTIAL) { end = realpos; partial = true; }
  }
  pcmesh->MoveMesh (sector, realpos);
  return partial ? CEL_MOVE_PARTIAL : CEL_MOVE_SUCCEED;
}

void celPcMovable::AddConstraint (iPcMovableConstraint* constraint)
{
  int idx = constraints.Find (constraint);
  if (idx != -1) return;
  constraints.Push (constraint);
  constraint->IncRef ();
}

void celPcMovable::RemoveConstraint (iPcMovableConstraint* constraint)
{
  int idx = constraints.Find (constraint);
  if (idx == -1) return;
  constraints.Delete (idx);
  constraint->DecRef ();
}

void celPcMovable::RemoveAllConstraints ()
{
  int i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    iPcMovableConstraint* c = (iPcMovableConstraint*)constraints[i];
    c->DecRef ();
  }
  constraints.DeleteAll ();
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcSolid)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcSolid)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcSolid::PcSolid)
  SCF_IMPLEMENTS_INTERFACE (iPcSolid)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcSolid::celPcSolid (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcSolid);
  pcmesh = NULL;
  collider = NULL;
  celPcSolid::object_reg = object_reg;
}

celPcSolid::~celPcSolid ()
{
  if (pcmesh) pcmesh->DecRef ();
  if (collider) collider->DecRef ();
}

void celPcSolid::SetEntity (iCelEntity* entity)
{
  celPcSolid::entity = entity;
}

iCelDataBuffer* celPcSolid::GetDataBuffer ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer ();
  pl->DecRef ();
  return databuf;
}

void celPcSolid::Save (iCelDataBuffer* databuf)
{
  (void)databuf;
}

void celPcSolid::Load (iCelDataBuffer* databuf)
{
  (void)databuf;
}

void celPcSolid::SetMesh (iPcMesh* mesh)
{
  if (mesh) mesh->IncRef ();
  if (pcmesh) pcmesh->DecRef ();
  pcmesh = mesh;
  if (collider) { collider->DecRef (); collider = NULL; }
}

iCollider* celPcSolid::GetCollider ()
{
  if (collider) return collider;
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  CS_ASSERT (pcmesh != NULL);
  iPolygonMesh* pmesh = SCF_QUERY_INTERFACE (pcmesh->GetMesh ()->
  	GetMeshObject (), iPolygonMesh);
  if (pmesh)
  {
    iCollideSystem* cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
    CS_ASSERT (cdsys != NULL);
    collider = cdsys->CreateCollider (pmesh);
    pmesh->DecRef ();
    cdsys->DecRef ();
  }
  return collider;
}

//---------------------------------------------------------------------------

// Private class implementing iPolygonMesh for one triangle.
// This will be used for detecting collision in the movement
// vector.
class celPolygonMeshTriangle : public iPolygonMesh
{
private:
  csVector3 vertices[3];
  csMeshedPolygon polygons[1];
  int vertex_indices[3];

public:
  celPolygonMeshTriangle (const csVector3& start, const csVector3& end)
  {
    SCF_CONSTRUCT_IBASE (NULL);
    vertices[0] = start;
    vertices[1] = start;
    vertices[2] = end;
    polygons[0].num_vertices = 3;
    polygons[0].vertices = vertex_indices;
    vertex_indices[0] = 0;
    vertex_indices[1] = 1;
    vertex_indices[2] = 2;
  }
  virtual ~celPolygonMeshTriangle () { }

  SCF_DECLARE_IBASE;

  virtual int GetVertexCount () { return 3; }
  virtual csVector3* GetVertices () { return vertices; }
  virtual int GetPolygonCount () { return 1; }
  virtual csMeshedPolygon* GetPolygons () { return polygons; }
  virtual void Cleanup () { }
};

SCF_IMPLEMENT_IBASE (celPolygonMeshTriangle)
  SCF_IMPLEMENTS_INTERFACE (iPolygonMesh)
SCF_IMPLEMENT_IBASE_END

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcMovableConstraintCD)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMovableConstraint)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMovableConstraintCD::PcMovableConstraintCD)
  SCF_IMPLEMENTS_INTERFACE (iPcMovableConstraint)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMovableConstraintCD::celPcMovableConstraintCD (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMovableConstraint);
  celPcMovableConstraintCD::object_reg = object_reg;
  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
  CS_ASSERT (cdsys != NULL);
}

celPcMovableConstraintCD::~celPcMovableConstraintCD ()
{
  if (cdsys) cdsys->DecRef ();
}

void celPcMovableConstraintCD::SetEntity (iCelEntity* entity)
{
  celPcMovableConstraintCD::entity = entity;
}

iCelDataBuffer* celPcMovableConstraintCD::GetDataBuffer ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer ();
  pl->DecRef ();
  return databuf;
}

void celPcMovableConstraintCD::Save (iCelDataBuffer* databuf)
{
  (void)databuf;
}

void celPcMovableConstraintCD::Load (iCelDataBuffer* databuf)
{
  (void)databuf;
}

int celPcMovableConstraintCD::CheckMove (iSector* sector,
	const csVector3& start, const csVector3& end, csVector3& pos)
{
  iPcMesh* pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
  	iPcMesh);
  CS_ASSERT (pcmesh != NULL);
  iPcSolid* pcsolid = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
  	iPcSolid);
  CS_ASSERT (pcsolid != NULL);

  // See if we're handling a single point.
  bool single_point = csSquaredDist::PointPoint (start, end) < EPSILON;

  int rc = CEL_MOVE_SUCCEED;
  pos = end;
  if (pcsolid->GetCollider ())
  {
    // Create a collider for detecting collision detection along the
    // movement vector.
    iCollider* path_collider = NULL;
    csReversibleTransform path_trans;	// Identity
    if (!single_point)
    {
      celPolygonMeshTriangle* pmtri = new celPolygonMeshTriangle (start, end);
      path_collider = cdsys->CreateCollider (pmtri);
      pmtri->DecRef ();
    }

    // Change the transform from the mesh to the new position.
    csReversibleTransform trans = pcmesh->GetMesh ()->GetMovable ()->
    	GetTransform ();
    trans.SetOrigin (end);

    cdsys->ResetCollisionPairs ();
    iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT (pl != NULL);
    iCelEntityList* list = pl->FindNearbyEntities (sector, start, 10/*@@@*/);
    int i;
    for (i = 0 ; i < list->GetCount () ; i++)
    {
      iCelEntity* ent = list->Get (i);
      if (ent == entity) continue;	// Ignore collisions with ourselves.
      iPcSolid* pcsolid_ent = CEL_QUERY_PROPCLASS (
      	ent->GetPropertyClassList (), iPcSolid);
      if (pcsolid_ent && pcsolid_ent->GetCollider ())
      {
        iPcMesh* pcmesh_ent = CEL_QUERY_PROPCLASS (
		ent->GetPropertyClassList (), iPcMesh);
	if (pcmesh_ent)
	{
	  csReversibleTransform& trans_ent = pcmesh_ent->GetMesh ()->
	  	GetMovable ()->GetTransform ();
	  bool ret;
	  if (path_collider)
	    ret = cdsys->Collide (path_collider, &path_trans,
	  	  pcsolid_ent->GetCollider (), &trans_ent);
	  else
	    ret = false;
	  if (!ret)
	    ret = cdsys->Collide (pcsolid->GetCollider (), &trans,
	    	pcsolid_ent->GetCollider (), &trans_ent);
	  pcmesh_ent->DecRef ();
	  if (ret) rc = CEL_MOVE_FAIL;
	}
	pcsolid_ent->DecRef ();
      }
    }
    list->DecRef ();
    pl->DecRef ();
    if (path_collider) path_collider->DecRef ();
  }
  pcsolid->DecRef ();
  pcmesh->DecRef ();
  return rc;
}

//---------------------------------------------------------------------------

// Private class implementing iPolygonMesh for one cube.
// This will be used for gravity collider.
class celPolygonMeshCube : public iPolygonMesh
{
private:
  csVector3 vertices[8];
  csMeshedPolygon polygons[6];
  int vertex_indices[4*6];

public:
  celPolygonMeshCube (const csVector3& dim, const csVector3& offs)
  {
    SCF_CONSTRUCT_IBASE (NULL);
    csVector3 d = dim * .5;
    vertices[0].Set (-d.x, -d.y, -d.z);
    vertices[1].Set ( d.x, -d.y, -d.z);
    vertices[2].Set (-d.x, -d.y,  d.z);
    vertices[3].Set ( d.x, -d.y,  d.z);
    vertices[4].Set (-d.x,  d.y, -d.z);
    vertices[5].Set ( d.x,  d.y, -d.z);
    vertices[6].Set (-d.x,  d.y,  d.z);
    vertices[7].Set ( d.x,  d.y,  d.z);
    int i;
    for (i = 0 ; i < 8 ; i++)
      vertices[i] += offs;
    for (i = 0 ; i < 6 ; i++)
    {
      polygons[i].num_vertices = 4;
      polygons[i].vertices = &vertex_indices[i*4];
    }
    vertex_indices[0*4+0] = 4;
    vertex_indices[0*4+1] = 5;
    vertex_indices[0*4+2] = 1;
    vertex_indices[0*4+3] = 0;
    vertex_indices[1*4+0] = 5;
    vertex_indices[1*4+1] = 7;
    vertex_indices[1*4+2] = 3;
    vertex_indices[1*4+3] = 1;
    vertex_indices[2*4+0] = 7;
    vertex_indices[2*4+1] = 6;
    vertex_indices[2*4+2] = 2;
    vertex_indices[2*4+3] = 3;
    vertex_indices[3*4+0] = 6;
    vertex_indices[3*4+1] = 4;
    vertex_indices[3*4+2] = 0;
    vertex_indices[3*4+3] = 2;
    vertex_indices[4*4+0] = 6;
    vertex_indices[4*4+1] = 7;
    vertex_indices[4*4+2] = 5;
    vertex_indices[4*4+3] = 4;
    vertex_indices[5*4+0] = 0;
    vertex_indices[5*4+1] = 1;
    vertex_indices[5*4+2] = 3;
    vertex_indices[5*4+3] = 2;
  }
  virtual ~celPolygonMeshCube () { }

  SCF_DECLARE_IBASE;

  virtual int GetVertexCount () { return 8; }
  virtual csVector3* GetVertices () { return vertices; }
  virtual int GetPolygonCount () { return 6; }
  virtual csMeshedPolygon* GetPolygons () { return polygons; }
  virtual void Cleanup () { }
};

SCF_IMPLEMENT_IBASE (celPolygonMeshCube)
  SCF_IMPLEMENTS_INTERFACE (iPolygonMesh)
SCF_IMPLEMENT_IBASE_END

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcGravity)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcGravity)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcGravity::PcGravity)
  SCF_IMPLEMENTS_INTERFACE (iPcGravity)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcGravity::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcGravity::celPcGravity (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcGravity);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiEventHandler);
  pcmovable = NULL;
  pcsolid = NULL;
  gravity_collider = NULL;
  celPcGravity::object_reg = object_reg;
  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
  CS_ASSERT (cdsys != NULL);
  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  CS_ASSERT (pl != NULL);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != NULL);
  weight = 1;

  on_ground = false;
  accel.Set (0, 0, 0);
  speed.Set (0, 0, 0);
  grav_speed.Set (0, 0, 0);

  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (&scfiEventHandler, trigger);
  q->DecRef ();
}

celPcGravity::~celPcGravity ()
{
  if (pcmovable) pcmovable->DecRef ();
  if (pcsolid) pcsolid->DecRef ();
  if (gravity_collider) gravity_collider->DecRef ();
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  if (q != NULL)
    q->RemoveListener (&scfiEventHandler);
  if (cdsys) cdsys->DecRef ();
  if (pl) pl->DecRef ();
  if (vc) vc->DecRef ();
}

void celPcGravity::SetEntity (iCelEntity* entity)
{
  celPcGravity::entity = entity;
}

iCelDataBuffer* celPcGravity::GetDataBuffer ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer ();
  pl->DecRef ();
  return databuf;
}

void celPcGravity::Save (iCelDataBuffer* databuf)
{
  (void)databuf;
}

void celPcGravity::Load (iCelDataBuffer* databuf)
{
  (void)databuf;
}

void celPcGravity::CreateGravityCollider (iPcMesh* mesh)
{
  // @@@ NOT IMPLEMENTED YET!
}

void celPcGravity::CreateGravityCollider (const csVector3& dim,
  	const csVector3& offs)
{
  if (gravity_collider) gravity_collider->DecRef ();
  celPolygonMeshCube* pmcube = new celPolygonMeshCube (dim, offs);
  gravity_collider = cdsys->CreateCollider (pmcube);
  pmcube->DecRef ();
}

void celPcGravity::SetMovable (iPcMovable* movable)
{
  if (movable) movable->IncRef ();
  if (pcmovable) pcmovable->DecRef ();
  pcmovable = movable;
}

iPcMovable* celPcGravity::GetMovable ()
{
  if (!pcmovable)
  {
    pcmovable = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
    	iPcMovable);
    CS_ASSERT (pcmovable != NULL);
  }
  return pcmovable;
}

void celPcGravity::SetSolid (iPcSolid* solid)
{
  if (solid) solid->IncRef ();
  if (pcsolid) pcsolid->DecRef ();
  pcsolid = solid;
}

iPcSolid* celPcGravity::GetSolid ()
{
  if (!pcsolid)
  {
    pcsolid = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
    	iPcSolid);
    CS_ASSERT (pcsolid != NULL);
  }
  return pcsolid;
}

void celPcGravity::ClearForces ()
{
  on_ground = false;
  accel.Set (0, 0, 0);
  speed.Set (0, 0, 0);
  grav_speed.Set (0, 0, 0);
}

void celPcGravity::ApplyForce (const csVector3& force, float time)
{
  GetMovable ();
  iMovable* movable = pcmovable->GetMesh ()->GetMesh ()->GetMovable ();
  csReversibleTransform& w2o = movable->GetTransform ();
  csVector3 forceworld = w2o.This2OtherRelative (force);

  if (force_time > SMALL_EPSILON)
  {
    // There is still a force in action. Combine them.
    float max_time = MAX (time, force_time);
    csVector3 orig_force = accel * .1;
    csVector3 new_force = (orig_force * force_time + forceworld * time)
    	/ max_time;
    accel = new_force / .1;
    force_time = max_time;
  }
  else
  {
    accel = forceworld / .1;	// Acceleration = force / mass.
    force_time = time;
  }
}

bool celPcGravity::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdPreProcess)
  {
    GetMovable ();
    iMovable* movable = pcmovable->GetMesh ()->GetMesh ()->GetMovable ();
    csReversibleTransform& w2o = movable->GetTransform ();
    GetSolid ();
    iCollider* collider = pcsolid->GetCollider ();

    iCelEntityList* cd_list = pl->FindNearbyEntities (movable->
    	GetSectors ()->Get (0),
    	w2o.GetOrigin (), 10/*@@@*/);

    float delta_t1, delta_t2;
    csTicks elapsed_time = vc->GetElapsedTicks ();
    if (!elapsed_time) return false;
    delta_t1 = elapsed_time/1000.0;

    grav_speed += csVector3 (0, -9.8, 0) * delta_t1;
    // Handle gravity so that we only call HandleForce for 0.1 second
    // maximum. This ensures that it will work ok on slower systems.
    float dt1 = delta_t1;
    while (dt1 > 0)
    {
      float dt = MIN (dt1, .1);
      dt1 -= dt;
      SetOnGround (!HandleForce (dt,
        gravity_collider ? gravity_collider : collider,
        cd_list, grav_speed));
    }

    delta_t2 = elapsed_time;
    if (!(accel < SMALL_EPSILON))
    {
      if (delta_t2 >= force_time)
      {
        delta_t2 -= force_time;
        speed += accel * force_time/1000.0;
        force_time = 0;
        accel.Set (0, 0, 0);
      }
      else
      {
        force_time -= delta_t2;
        speed += accel * delta_t2/1000.0;
        delta_t2 = 0;
      }
    }
    if (accel < SMALL_EPSILON && delta_t2 > SMALL_EPSILON)
    {
      speed /= pow (1.5, delta_t2/1000.0 * 15.0);
    }

    HandleForce (delta_t1, collider, cd_list, speed);
    cd_list->DecRef ();
  }
  return false;
}

bool celPcGravity::HandleForce (float delta_t, iCollider* this_collider,
    	iCelEntityList* cd_list, csVector3& force)
{
  int i;
  float maxstep = .15;
  static const csTransform identity;

  GetMovable ();
  iMovable* movable = pcmovable->GetMesh ()->GetMesh ()->GetMovable ();
  csReversibleTransform& w2o = movable->GetTransform ();

  // Calculate movement and do CD.
  // We do movement in smaller steps to ensure that collision
  // detection works ok.
  csVector3 totforce = force;
  csVector3 move;
  while (!(totforce < 0.01f))
  {
    move = totforce * delta_t;
    if (move.x > maxstep) move.x = maxstep;
    else if (move.x < -maxstep) move.x = -maxstep;
    if (move.y > maxstep) move.y = maxstep;
    else if (move.y < -maxstep) move.y = -maxstep;
    if (move.z > maxstep) move.z = maxstep;
    else if (move.z < -maxstep) move.z = -maxstep;
    force = move / delta_t;
    totforce -= force;

    csTransform test (w2o);
    test.SetOrigin (test.GetOrigin () + move);

    for (i = 0 ; i < cd_list->GetCount () ; i++)
    {
      iCelEntity* ent = cd_list->Get (i);
      iPcSolid* solid_ent = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcSolid);
      if (!solid_ent) continue;
      iPcMovable* mov_ent = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcMovable);
      const csTransform* coltrans;
      if (mov_ent) coltrans = &mov_ent->GetMesh ()->GetMesh ()->GetMovable ()->
      	GetTransform ();
      else
        coltrans = &identity;

      //@@@ More than one collider for pcsolid?
      iCollider* collider = solid_ent->GetCollider ();
      int num_colliders = 1;
      for (int j = 0 ; j < num_colliders ; j++)
      {
        iCollider* col = collider;
        cdsys->ResetCollisionPairs ();
	bool rc = false;
        if (this_collider)
	  rc = cdsys->Collide (this_collider, &test, col, coltrans);
        if (rc)
        {
          csCollisionPair* colpairs = cdsys->GetCollisionPairs ();
          int num_pairs = cdsys->GetCollisionPairCount ();
	  for (int k = 0 ; k < num_pairs ; k++)
	  {
	    csCollisionPair& cp = colpairs[k];
	    csVector3 n = ((cp.c2-cp.b2) % (cp.b2-cp.a2)).Unit ();
	    if (n * force < 0)
	      continue;
	    force = -(force % n) % n;
	  }
        }
      }
      if (mov_ent) mov_ent->DecRef ();
      solid_ent->DecRef ();
    }

    if (force < .01f) return false;
    move = force * delta_t;
    pcmovable->Move (move);
  }
  return true;
}

void celPcGravity::SetOnGround (bool og)
{
  if (og == on_ground) return;
  on_ground = og;
  // @@@ Send message!
}

//---------------------------------------------------------------------------

