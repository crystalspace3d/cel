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
#include "pl/datatype.h"
#include "pl/persist.h"
#include "bl/behave.h"
#include "csutil/util.h"
#include "csutil/debug.h"
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

CEL_IMPLEMENT_FACTORY(Movable, "pcmovable")
CEL_IMPLEMENT_FACTORY(Solid, "pcsolid")
CEL_IMPLEMENT_FACTORY(MovableConstraintCD, "pcmovableconst_cd")
CEL_IMPLEMENT_FACTORY(Gravity, "pcgravity")
CEL_IMPLEMENT_FACTORY(Gravity2, "pcgravity2")

SCF_EXPORT_CLASS_TABLE (pfmove)
  SCF_EXPORT_CLASS_DEP (celPfMovable, "cel.pcfactory.movable",
  	"CEL Movable Property Class Factory", "cel.physicallayer")
  SCF_EXPORT_CLASS_DEP (celPfSolid, "cel.pcfactory.solid",
	"CEL Solid Property Class Factory", "cel.physicallayer")
  SCF_EXPORT_CLASS_DEP (celPfMovableConstraintCD, "cel.pcfactory.movableconst_cd",
	"CEL MovableConstraintCD Class Factory", "cel.physicallayer")
  SCF_EXPORT_CLASS_DEP (celPfGravity, "cel.pcfactory.gravity",
	"CEL Gravity Class Factory", "cel.physicallayer")
  SCF_EXPORT_CLASS_DEP (celPfGravity2, "cel.pcfactory.gravity2",
	"CEL Gravity Class Factory 2", "cel.physicallayer")
SCF_EXPORT_CLASS_TABLE_END

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcMovable)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMovable)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMovable::PcMovable)
  SCF_IMPLEMENTS_INTERFACE (iPcMovable)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMovable::celPcMovable (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMovable);
  pcmesh = NULL;
  DG_TYPE (this, "celPcMovable()");
}

celPcMovable::~celPcMovable ()
{
  if (pcmesh) pcmesh->DecRef ();
  RemoveAllConstraints ();
}

#define MOVABLE_SERIAL 1

iCelDataBuffer* celPcMovable::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (MOVABLE_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (1+1+constraints.Length ());
  int i, j = 0;
  iCelPropertyClass* pc = NULL;
  if (pcmesh) pc = SCF_QUERY_INTERFACE_FAST (pcmesh, iCelPropertyClass);
  databuf->GetData (j++)->Set (pc);
  if (pc) pc->DecRef ();
  databuf->GetData (j++)->Set ((uint16)constraints.Length ());
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    iPcMovableConstraint* pcm = (iPcMovableConstraint*)constraints[i];
    iCelPropertyClass* pc = SCF_QUERY_INTERFACE_FAST (pcm, iCelPropertyClass);
    databuf->GetData (j++)->Set (pc);
    pc->DecRef ();
  }
  return databuf;
}

bool celPcMovable::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MOVABLE_SERIAL) return false;
  int cnt_total = databuf->GetDataCount ();
  celData* cd;
  RemoveAllConstraints ();
  int i, j = 0;
  cd = databuf->GetData (j++); if (!cd) return false;
  iCelPropertyClass* pc = cd->value.pc;
  iPcMesh* pcm = NULL;
  if (pc)
  {
    pcm = SCF_QUERY_INTERFACE_FAST (pc, iPcMesh);
    CS_ASSERT (pcm != NULL);
  }
  SetMesh (pcm);
  if (pcm) pcm->DecRef ();

  cd = databuf->GetData (j++); if (!cd) return false;
  int cnt_constraints = cd->value.uw;
  if (cnt_total != 1+1+cnt_constraints) return false;

  for (i = 0 ; i < cnt_constraints ; i++)
  {
    cd = databuf->GetData (j++); if (!cd) return false;
    iPcMovableConstraint* pcm = SCF_QUERY_INTERFACE_FAST (cd->value.pc,
    	iPcMovableConstraint);
    CS_ASSERT (pcm != NULL);
    AddConstraint (pcm);
    pcm->DecRef ();
  }

  return true;
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

SCF_IMPLEMENT_IBASE_EXT (celPcSolid)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcSolid)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcSolid::PcSolid)
  SCF_IMPLEMENTS_INTERFACE (iPcSolid)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcSolid::celPcSolid (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcSolid);
  pcmesh = NULL;
  collider = NULL;
  DG_TYPE (this, "celPcSolid()");
}

celPcSolid::~celPcSolid ()
{
  if (pcmesh) pcmesh->DecRef ();
  if (collider) collider->DecRef ();
}

#define SOLID_SERIAL 1

iCelDataBuffer* celPcSolid::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (SOLID_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (1);
  iCelPropertyClass* pc = NULL;
  if (pcmesh) pc = SCF_QUERY_INTERFACE_FAST (pcmesh, iCelPropertyClass);
  databuf->GetData (0)->Set (pc);
  if (pc) pc->DecRef ();
  return databuf;
}

bool celPcSolid::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != SOLID_SERIAL) return false;
  if (databuf->GetDataCount () != 1) return false;
  celData* cd;
  if (collider) { collider->DecRef (); collider = NULL; }
  cd = databuf->GetData (0); if (!cd) return false;
  iPcMesh* pcm = SCF_QUERY_INTERFACE_FAST (cd->value.pc, iPcMesh);
  SetMesh (pcm);
  if (pcm) pcm->DecRef ();
  return true;
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

SCF_IMPLEMENT_IBASE_EXT (celPcMovableConstraintCD)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMovableConstraint)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMovableConstraintCD::PcMovableConstraintCD)
  SCF_IMPLEMENTS_INTERFACE (iPcMovableConstraint)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMovableConstraintCD::celPcMovableConstraintCD (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMovableConstraint);
  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
  CS_ASSERT (cdsys != NULL);
  DG_TYPE (this, "celPcMovableConstraintCD()");
}

celPcMovableConstraintCD::~celPcMovableConstraintCD ()
{
  if (cdsys) cdsys->DecRef ();
}

#define MOVABLECONST_CD_SERIAL 1

iCelDataBuffer* celPcMovableConstraintCD::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (MOVABLECONST_CD_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (0);
  return databuf;
}

bool celPcMovableConstraintCD::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MOVABLECONST_CD_SERIAL) return false;
  if (databuf->GetDataCount () != 0) return false;
  return true;
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
      if (pcsolid_ent)
      {
        if (pcsolid_ent->GetCollider ())
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

SCF_IMPLEMENT_IBASE_EXT (celPcGravity)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcGravity)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcGravity::PcGravity)
  SCF_IMPLEMENTS_INTERFACE (iPcGravity)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcGravity::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcGravity::celPcGravity (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcGravity);
  scfiEventHandler = NULL;
  pcmovable = NULL;
  pcsolid = NULL;
  gravity_collider = NULL;
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

  has_gravity_collider = false;
  gravity_mesh = NULL;

  scfiEventHandler = new EventHandler (this);
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);
  q->DecRef ();
  DG_TYPE (this, "celPcGravity()");
}

celPcGravity::~celPcGravity ()
{
  if (pcmovable) pcmovable->DecRef ();
  if (pcsolid) pcsolid->DecRef ();
  if (gravity_collider) gravity_collider->DecRef ();
  if (scfiEventHandler)
  {
    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q != NULL)
    {
      q->RemoveListener (scfiEventHandler);
      q->DecRef ();
    }
    scfiEventHandler->DecRef ();
  }
  if (cdsys) cdsys->DecRef ();
  if (pl) pl->DecRef ();
  if (vc) vc->DecRef ();
}

#define GRAVITY_SERIAL 1

iCelDataBuffer* celPcGravity::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (GRAVITY_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (12);

  iCelPropertyClass* pc;
  if (pcmovable) pc = SCF_QUERY_INTERFACE_FAST (pcmovable, iCelPropertyClass);
  else pc = NULL;
  databuf->GetData (0)->Set (pc);
  if (pc) pc->DecRef ();
  if (pcsolid) pc = SCF_QUERY_INTERFACE_FAST (pcsolid, iCelPropertyClass);
  else pc = NULL;
  databuf->GetData (1)->Set (pc);
  if (pc) pc->DecRef ();

  databuf->GetData (2)->Set (weight);

  databuf->GetData (3)->Set (has_gravity_collider);
  if (has_gravity_collider && gravity_mesh)
    pc = SCF_QUERY_INTERFACE_FAST (gravity_mesh, iCelPropertyClass);
  else
    pc = NULL;
  databuf->GetData (4)->Set (pc);
  if (pc) pc->DecRef ();
  databuf->GetData (5)->Set (gravity_dim);
  databuf->GetData (6)->Set (gravity_offs);

  databuf->GetData (7)->Set (on_ground);
  databuf->GetData (8)->Set (accel);
  databuf->GetData (9)->Set (speed);
  databuf->GetData (10)->Set (grav_speed);
  databuf->GetData (11)->Set (force_time);
  
  return databuf;
}

bool celPcGravity::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != GRAVITY_SERIAL) return false;
  if (databuf->GetDataCount () != 12) return false;
  celData* cd;

  cd = databuf->GetData (0); if (!cd) return false;
  iPcMovable* pcm = NULL;
  if (cd->value.pc) pcm = SCF_QUERY_INTERFACE_FAST (cd->value.pc, iPcMovable);
  SetMovable (pcm);
  if (pcm) pcm->DecRef ();

  cd = databuf->GetData (1); if (!cd) return false;
  iPcSolid* pcs = NULL;
  if (cd->value.pc) pcs = SCF_QUERY_INTERFACE_FAST (cd->value.pc, iPcSolid);
  SetSolid (pcs);
  if (pcs) pcs->DecRef ();

  cd = databuf->GetData (2); if (!cd) return false;
  weight = cd->value.f;

  cd = databuf->GetData (3); if (!cd) return false;
  has_gravity_collider = cd->value.bo;
  gravity_mesh = NULL;
  if (has_gravity_collider)
  {
    cd = databuf->GetData (4); if (!cd) return false;
    if (cd->value.pc)
    {
      iPcMesh* gm = SCF_QUERY_INTERFACE_FAST (cd->value.pc, iPcMesh);
      CreateGravityCollider (gm);
      gm->DecRef ();
    }
    else
    {
      cd = databuf->GetData (5); if (!cd) return false;
      gravity_dim.x = cd->value.v.x;
      gravity_dim.y = cd->value.v.y;
      gravity_dim.z = cd->value.v.z;
      cd = databuf->GetData (6); if (!cd) return false;
      gravity_offs.x = cd->value.v.x;
      gravity_offs.y = cd->value.v.y;
      gravity_offs.z = cd->value.v.z;
      CreateGravityCollider (gravity_dim, gravity_offs);
    }
  }

  cd = databuf->GetData (7); if (!cd) return false;
  on_ground = cd->value.bo;
  cd = databuf->GetData (8); if (!cd) return false;
  accel.x = cd->value.v.x;
  accel.y = cd->value.v.y;
  accel.z = cd->value.v.z;
  cd = databuf->GetData (9); if (!cd) return false;
  speed.x = cd->value.v.x;
  speed.y = cd->value.v.y;
  speed.z = cd->value.v.z;
  cd = databuf->GetData (10); if (!cd) return false;
  grav_speed.x = cd->value.v.x;
  grav_speed.y = cd->value.v.y;
  grav_speed.z = cd->value.v.z;

  cd = databuf->GetData (11); if (!cd) return false;
  force_time = cd->value.f;

  return true;
}

void celPcGravity::CreateGravityCollider (iPcMesh* /*mesh*/)
{
  // @@@ NOT IMPLEMENTED YET!
}

void celPcGravity::CreateGravityCollider (const csVector3& dim,
  	const csVector3& offs)
{
  gravity_mesh = NULL;
  has_gravity_collider = true;
  gravity_dim = dim;
  gravity_offs = offs;

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

    float delta_t1, delta_t2;
    csTicks elapsed_time = vc->GetElapsedTicks ();
    if (!elapsed_time) return false;
    delta_t1 = elapsed_time/1000.0;

    iCelEntityList* cd_list = pl->FindNearbyEntities (movable->
    	GetSectors ()->Get (0),
    	w2o.GetOrigin (), 10/*@@@*/);

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
  static const csReversibleTransform identity;

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

    csReversibleTransform test (w2o);
    test.SetOrigin (test.GetOrigin () + move);

    for (i = 0 ; i < cd_list->GetCount () ; i++)
    {
      iCelEntity* ent = cd_list->Get (i);
      iPcSolid* solid_ent = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcSolid);
      if (!solid_ent) continue;
      iPcMovable* mov_ent = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcMovable);
      const csReversibleTransform* coltrans;
      if (mov_ent) coltrans = &mov_ent->GetMesh ()->GetMesh ()->GetMovable ()->
      	GetTransform ();
      else
        coltrans = &identity;

      //@@@ More than one collider for pcsolid?
      iCollider* collider = solid_ent->GetCollider ();
      int num_colliders = 1;
      if (!collider) num_colliders = 0;
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

SCF_IMPLEMENT_IBASE_EXT (celPcGravity2)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcGravity)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcGravity2::PcGravity)
  SCF_IMPLEMENTS_INTERFACE (iPcGravity)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcGravity2::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcGravity2::celPcGravity2 (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcGravity);
  scfiEventHandler = NULL;
  pcmovable = NULL;
  pcsolid = NULL;
  gravity_collider = NULL;
  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
  CS_ASSERT (cdsys != NULL);
  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  CS_ASSERT (pl != NULL);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != NULL);
  weight = 1;

  current_speed.Set (0, 0, 0);
  infinite_forces.Set (0, 0, 0);

  has_gravity_collider = false;
  gravity_mesh = NULL;

  scfiEventHandler = new EventHandler (this);
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);
  q->DecRef ();
  DG_TYPE (this, "celPcGravity2()");
}

celPcGravity2::~celPcGravity2 ()
{
  ClearForces ();
  if (pcmovable) pcmovable->DecRef ();
  if (pcsolid) pcsolid->DecRef ();
  if (gravity_collider) gravity_collider->DecRef ();
  if (scfiEventHandler)
  {
    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q != NULL)
    {
      q->RemoveListener (scfiEventHandler);
      q->DecRef ();
    }
    scfiEventHandler->DecRef ();
  }
  if (cdsys) cdsys->DecRef ();
  if (pl) pl->DecRef ();
  if (vc) vc->DecRef ();
}

#define GRAVITY2_SERIAL 1

iCelDataBuffer* celPcGravity2::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (GRAVITY2_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (6+forces.Length ()*2);

  int j = 0;
  iCelPropertyClass* pc;
  if (pcmovable) pc = SCF_QUERY_INTERFACE_FAST (pcmovable, iCelPropertyClass);
  else pc = NULL;
  databuf->GetData (j++)->Set (pc);
  if (pc) pc->DecRef ();
  if (pcsolid) pc = SCF_QUERY_INTERFACE_FAST (pcsolid, iCelPropertyClass);
  else pc = NULL;
  databuf->GetData (j++)->Set (pc);
  if (pc) pc->DecRef ();

  databuf->GetData (j++)->Set (weight);
  databuf->GetData (j++)->Set (current_speed);
  databuf->GetData (j++)->Set (infinite_forces);

  databuf->GetData (j++)->Set ((uint16)forces.Length ());
  int i;
  for (i = 0 ; i < forces.Length () ; i++)
  {
    celForce* f = (celForce*)forces[i];
    databuf->GetData (j++)->Set (f->force);
    databuf->GetData (j++)->Set (f->time_remaining);
  }
  
  return databuf;
}

bool celPcGravity2::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != GRAVITY2_SERIAL) return false;
  celData* cd;

  int j = 0;
  cd = databuf->GetData (j++); if (!cd) return false;
  iPcMovable* pcm = NULL;
  if (cd->value.pc) pcm = SCF_QUERY_INTERFACE_FAST (cd->value.pc, iPcMovable);
  SetMovable (pcm);
  if (pcm) pcm->DecRef ();

  cd = databuf->GetData (j++); if (!cd) return false;
  iPcSolid* pcs = NULL;
  if (cd->value.pc) pcs = SCF_QUERY_INTERFACE_FAST (cd->value.pc, iPcSolid);
  SetSolid (pcs);
  if (pcs) pcs->DecRef ();

  cd = databuf->GetData (j++); if (!cd) return false;
  weight = cd->value.f;

  cd = databuf->GetData (j++); if (!cd) return false;
  current_speed.x = cd->value.v.x;
  current_speed.y = cd->value.v.y;
  current_speed.z = cd->value.v.z;
  cd = databuf->GetData (j++); if (!cd) return false;
  infinite_forces.x = cd->value.v.x;
  infinite_forces.y = cd->value.v.y;
  infinite_forces.z = cd->value.v.z;

  cd = databuf->GetData (j++); if (!cd) return false;
  int num_forces = cd->value.uw;
  int i;
  for (i = 0 ; i < num_forces ; i++)
  {
    celForce* f = new celForce ();
    cd = databuf->GetData (j++); if (!cd) return false;
    f->force.x = cd->value.v.x;
    f->force.y = cd->value.v.y;
    f->force.z = cd->value.v.z;
    cd = databuf->GetData (j++); if (!cd) return false;
    f->time_remaining = cd->value.f;
  }

  return true;
}

void celPcGravity2::CreateGravityCollider (iPcMesh* /*mesh*/)
{
  // @@@ NOT IMPLEMENTED YET!
}

void celPcGravity2::CreateGravityCollider (const csVector3& dim,
  	const csVector3& offs)
{
  gravity_mesh = NULL;
  has_gravity_collider = true;
  gravity_dim = dim;
  gravity_offs = offs;

  if (gravity_collider) gravity_collider->DecRef ();
  celPolygonMeshCube* pmcube = new celPolygonMeshCube (dim, offs);
  gravity_collider = cdsys->CreateCollider (pmcube);
  pmcube->DecRef ();
}

void celPcGravity2::SetMovable (iPcMovable* movable)
{
  if (movable) movable->IncRef ();
  if (pcmovable) pcmovable->DecRef ();
  pcmovable = movable;
}

iPcMovable* celPcGravity2::GetMovable ()
{
  if (!pcmovable)
  {
    pcmovable = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
    	iPcMovable);
    CS_ASSERT (pcmovable != NULL);
  }
  return pcmovable;
}

void celPcGravity2::SetSolid (iPcSolid* solid)
{
  if (solid) solid->IncRef ();
  if (pcsolid) pcsolid->DecRef ();
  pcsolid = solid;
}

iPcSolid* celPcGravity2::GetSolid ()
{
  if (!pcsolid)
  {
    pcsolid = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
    	iPcSolid);
    CS_ASSERT (pcsolid != NULL);
  }
  return pcsolid;
}

void celPcGravity2::ClearForces ()
{
  current_speed.Set (0, 0, 0);
  //@@@ Seperate clear for infinite_forces!
  //@@@infinite_forces.Set (0, 0, 0);
  int i;
  for (i = 0 ; i < forces.Length () ; i++)
  {
    celForce* f = (celForce*)forces[i];
    delete f;
  }
  forces.DeleteAll ();
}

void celPcGravity2::ApplyForce (const csVector3& force, float time)
{
  if (time > 100000000)	// @@@ INFINITE! NEED NEW API
  {
    infinite_forces += force;
    return;
  }

  celForce* f = new celForce ();
  f->force = force;
  f->time_remaining = time;
  forces.Push (f);
}

bool celPcGravity2::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdPreProcess)
  {
    GetMovable ();
    iMovable* movable = pcmovable->GetMesh ()->GetMesh ()->GetMovable ();
    csReversibleTransform& w2o = movable->GetTransform ();
    GetSolid ();
    iCollider* collider = pcsolid->GetCollider ();

    float delta_t1;
    csTicks elapsed_time = vc->GetElapsedTicks ();
    if (!elapsed_time) return false;
    delta_t1 = elapsed_time/1000.0;

    iCelEntityList* cd_list = pl->FindNearbyEntities (movable->
    	GetSectors ()->Get (0),
    	w2o.GetOrigin (), 10/*@@@*/);

    // Handle physics so that we only call HandleForce for 0.1 second
    // maximum. This ensures that it will work ok on slower systems.
    float dt1 = delta_t1;
    while (dt1 > 0)
    {
      float dt = MIN (dt1, .04);	//@@@ TEMPORARY ONLY! SHOULD BE DONE WITH MORE ACCURATE CD.
      dt1 -= dt;
      HandleForce (dt, collider, cd_list);
    }

    cd_list->DecRef ();
  }
  return false;
}

bool celPcGravity2::TestMove (iCollider* this_collider,
    iCelEntityList* cd_list,
    const csReversibleTransform& w2o,
    const csVector3& relmove,
    csVector3& collider_normal)
{
  static const csReversibleTransform identity;
  csReversibleTransform test (w2o);
  test.SetOrigin (test.GetOrigin () + relmove);
  int i;
  for (i = 0 ; i < cd_list->GetCount () ; i++)
  {
    iCelEntity* ent = cd_list->Get (i);
    iPcSolid* solid_ent = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcSolid);
    if (!solid_ent) continue;
    iPcMovable* mov_ent = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcMovable);
    const csReversibleTransform* coltrans;
    if (mov_ent) coltrans = &mov_ent->GetMesh ()->GetMesh ()->GetMovable ()->
      	GetTransform ();
    else
      coltrans = &identity;

    //@@@ More than one collider for pcsolid?
    iCollider* collider = solid_ent->GetCollider ();
    int num_colliders = 1;
    if (!collider) num_colliders = 0;
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
	  collider_normal = ((cp.c2-cp.b2) % (cp.b2-cp.a2)).Unit ();
          if (mov_ent) mov_ent->DecRef ();
          solid_ent->DecRef ();
	  return true;
	}
      }
    }
    if (mov_ent) mov_ent->DecRef ();
    solid_ent->DecRef ();
  }

  return false;
}

bool celPcGravity2::HandleForce (float delta_t, iCollider* this_collider,
    	iCelEntityList* cd_list, const csVector3& force)
{
  int i;

  GetMovable ();
  iMovable* movable = pcmovable->GetMesh ()->GetMesh ()->GetMovable ();
  csReversibleTransform& w2o = movable->GetTransform ();

  csVector3 acceleration = force / weight;
  const csVector3& oldpos = w2o.GetOrigin ();

  float dt = delta_t;
  csVector3 relmove;
  csVector3 relspeed;
  while (dt > EPSILON)
  {
    relmove = acceleration;
    relmove *= dt;
    relspeed = relmove;
    relmove += current_speed;
    relmove *= dt;

    csVector3 collider_normal;
    if (TestMove (this_collider, cd_list, w2o, relmove, collider_normal))
    {
      dt /= 2.0;
    }
    else break;
  }
  if (dt <= EPSILON) return false;	// Movement not possible.
  else
  {
    current_speed += relspeed;
    pcmovable->Move (relmove);
    return true;
  }
}

bool celPcGravity2::HandleForce (float delta_t, iCollider* this_collider,
    	iCelEntityList* cd_list)
{
  while (delta_t > EPSILON)
  {
    // Find the smallest force duration we still have.
    csVector3 force (infinite_forces);
    float smallest_time = 1000000000;
    int i;
    for (i = 0 ; i < forces.Length () ; i++)
    {
      celForce* f = (celForce*)forces[i];
      if (f->time_remaining < smallest_time)
        smallest_time = f->time_remaining;
      force += f->force;
    }

    // This time we are going to handle should not exceed
    // the delta time.
    if (smallest_time > delta_t)
      smallest_time = delta_t;

    // Handle the force for this time.
    HandleForce (smallest_time, this_collider, cd_list, force);

    // Remove all forces that are done and update the remaining
    // time of the others.
    i = 0;
    while (i < forces.Length ())
    {
      celForce* f = (celForce*)forces[i];
      f->time_remaining -= smallest_time;
      if (f->time_remaining < EPSILON)
      {
        delete f;
	forces.Delete (i);
      }
      else
      {
        i++;
      }
    }

    // Continue with the remaining time.
    delta_t -= smallest_time;
  }

  return true;
}

//---------------------------------------------------------------------------

