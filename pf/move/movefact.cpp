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
#include "qsqrt.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "pf/move/movefact.h"
#include "pf/mesh.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/datatype.h"
#include "pl/persist.h"
#include "pl/databhlp.h"
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

SCF_EXPORT_CLASS_TABLE (pfmove)
  SCF_EXPORT_CLASS_DEP (celPfMovable, "cel.pcfactory.movable",
  	"CEL Movable Property Class Factory", "cel.physicallayer")
  SCF_EXPORT_CLASS_DEP (celPfSolid, "cel.pcfactory.solid",
	"CEL Solid Property Class Factory", "cel.physicallayer")
  SCF_EXPORT_CLASS_DEP (celPfMovableConstraintCD, "cel.pcfactory.movableconst_cd",
	"CEL MovableConstraintCD Class Factory", "cel.physicallayer")
  SCF_EXPORT_CLASS_DEP (celPfGravity, "cel.pcfactory.gravity",
	"CEL Gravity Class Factory", "cel.physicallayer")
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
  iPcMesh* pcm = NULL;
  if (cd->value.pc)
      pcm = SCF_QUERY_INTERFACE_FAST (cd->value.pc, iPcMesh);
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
  is_resting = false;
  active = true;

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
  DG_TYPE (this, "celPcGravity()");
}

celPcGravity::~celPcGravity ()
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

iCelDataBuffer* celPcGravity::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (GRAVITY2_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (8+forces.Length ()*2);
  celDataBufHelper db(databuf);

  iCelPropertyClass* pc;
  if (pcmovable) pc = SCF_QUERY_INTERFACE_FAST (pcmovable, iCelPropertyClass);
  else pc = NULL;
  db.Set (pc);
  if (pc) pc->DecRef ();
  if (pcsolid) pc = SCF_QUERY_INTERFACE_FAST (pcsolid, iCelPropertyClass);
  else pc = NULL;
  db.Set (pc);
  if (pc) pc->DecRef ();

  db.Set (weight);
  db.Set (current_speed);
  db.Set (infinite_forces);
  db.SetBool (is_resting);
  db.SetBool (active);

  db.Set ((uint16)forces.Length ());
  int i;
  for (i = 0 ; i < forces.Length () ; i++)
  {
    celForce* f = (celForce*)forces[i];
    db.Set (f->force);
    db.Set (f->time_remaining);
  }
  
  return databuf;
}

bool celPcGravity::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != GRAVITY2_SERIAL) return false;
  celDataBufHelper db(databuf);
  iCelPropertyClass* pc;

  db.Get(pc);
  iPcMovable* pcm = NULL;
  if (pc) pcm = SCF_QUERY_INTERFACE_FAST (pc, iPcMovable);
  SetMovable (pcm);
  if (pcm) pcm->DecRef ();

  db.Get(pc);
  iPcSolid* pcs = NULL;
  if (pc) pcs = SCF_QUERY_INTERFACE_FAST (pc, iPcSolid);
  SetSolid (pcs);
  if (pcs) pcs->DecRef ();

  if (!db.AllOk())
    return false;

  db.Get(weight);
  db.Get(current_speed);
  db.Get(infinite_forces);
  db.Get(is_resting);
  db.Get(active);

  if (!db.AllOk())
    return false;

  uint16 num_forces;
  int i;
  db.Get(num_forces);
  for (i = 0 ; i < num_forces ; i++)
  {
    celForce* f = new celForce ();
    db.Get(f->force);
    db.Get(f->time_remaining);
  }
  if (!db.AllOk())
    return false;

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
  int i;
  for (i = 0 ; i < forces.Length () ; i++)
  {
    celForce* f = (celForce*)forces[i];
    delete f;
  }
  forces.DeleteAll ();
}

void celPcGravity::ClearPermanentForces ()
{
  infinite_forces.Set (0, 0, 0);
}

void celPcGravity::ApplyPermanentForce (const csVector3& force)
{
  infinite_forces += force;
}

void celPcGravity::ApplyForce (const csVector3& force, float time)
{
  celForce* f = new celForce ();
  f->force = force;
  f->time_remaining = time;
  forces.Push (f);
}

bool celPcGravity::HandleEvent (iEvent& ev)
{
  if (!active) return false;

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
    is_resting = true;
    float dt1 = delta_t1;
    while (dt1 > 0)
    {
      float dt = MIN (dt1, .02);	//@@@ TEMPORARY ONLY! SHOULD BE DONE WITH MORE ACCURATE CD.
      dt1 -= dt;
      HandleForce (dt, collider, cd_list);
    }

    cd_list->DecRef ();
  }
  return false;
}

int celPcGravity::GetColliderArray (iCelEntityList* cd_list,
  	iCollider**& colliders, csReversibleTransform**& transforms)
{
  static csReversibleTransform identity;

  int num_cdlist = cd_list->GetCount ();
  colliders = NULL;
  transforms = NULL;
  if (!num_cdlist) return 0;
  colliders = new iCollider* [num_cdlist];
  transforms = new csReversibleTransform* [num_cdlist];
  int i;
  int num_colliders = 0;
  for (i = 0 ; i < num_cdlist ; i++)
  {
    iCelEntity* ent = cd_list->Get (i);
    iPcSolid* solid_ent = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcSolid);
    if (!solid_ent)
      continue;
    if (!solid_ent->GetCollider ())
    {
      solid_ent->DecRef ();
      continue;
    }

    iPcMovable* mov_ent = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcMovable);
    csReversibleTransform* coltrans;
    // @@@ Should use GetFullTransform()???
    if (mov_ent) coltrans = &mov_ent->GetMesh ()->GetMesh ()->GetMovable ()->
      	GetTransform ();
    else
      coltrans = &identity;
    //@@@ More than one collider for pcsolid?
    colliders[num_colliders] = solid_ent->GetCollider ();
    transforms[num_colliders] = coltrans;
    num_colliders++;
    solid_ent->DecRef ();
    if (mov_ent) mov_ent->DecRef ();
  }
  return num_colliders;
}

int celPcGravity::TestMove (iCollider* this_collider,
    int num_colliders,
    iCollider** colliders,
    csReversibleTransform** transforms,
    const csReversibleTransform& w2o,
    csVector3& newpos,
    csVector3& collider_normal)
{
  csReversibleTransform test (w2o);
  int rc = cdsys->CollidePath (this_collider, &test, newpos,
  	num_colliders, colliders, transforms);
  if (rc == -1) return -1;	// Stuck!!!
  if (rc == 0)			// Can move partially.
  {
    csCollisionPair* colpairs = cdsys->GetCollisionPairs ();
    int num_pairs = cdsys->GetCollisionPairCount ();
    for (int k = 0 ; k < num_pairs ; k++)
    {
      csCollisionPair& cp = colpairs[k];
      collider_normal = ((cp.c2-cp.b2) % (cp.b2-cp.a2)).Unit ();
      return 0;
    }
    return 0;
  }
  return 1;
}

bool celPcGravity::HandleForce (float delta_t, iCollider* this_collider,
    	iCelEntityList* cd_list, const csVector3& force)
{
  GetMovable ();
  iMovable* movable = pcmovable->GetMesh ()->GetMesh ()->GetMovable ();
  csReversibleTransform& w2o = movable->GetTransform ();
  const csVector3& oldpos = w2o.GetOrigin ();

  csVector3 acceleration = force / weight;
  csVector3 relmove = acceleration;
  relmove *= delta_t;
  csVector3 relspeed = relmove;
  relmove += current_speed;
  relmove *= delta_t;

  iCollider** colliders;
  csReversibleTransform** transforms;
  int num_colliders = GetColliderArray (cd_list, colliders, transforms);
  if (!num_colliders)
  {
    delete[] colliders;
    delete[] transforms;
    // Since there are no colliders we can surely move.
    current_speed += relspeed;
    is_resting = false;
    pcmovable->Move (relmove);
    return true;
  }

  csVector3 newpos = oldpos + relmove;
  csVector3 desired_endpos = newpos;
  csVector3 collider_normal;
  int rc = TestMove (this_collider, num_colliders, colliders, transforms,
  	w2o, newpos, collider_normal);
  delete[] colliders;
  delete[] transforms;

  if (rc == -1)
  {
    // Stuck at start position...
    return false;
  }
  else if (rc == 1)
  {
    // Can move without obstruction.
    current_speed += relspeed;
    is_resting = false;
    pcmovable->Move (relmove);
    return true;
  }
  else
  {
    // We can move but there is some obstruction.
    // Here we have to find out where exactly.
    // @@@ Here we also have to calculate impuse: for later...
    float disttot = qsqrt (csSquaredDist::PointPoint (oldpos, desired_endpos));
    float dist = qsqrt (csSquaredDist::PointPoint (oldpos, newpos));
    delta_t = delta_t * dist / disttot;

    relspeed = acceleration;
    relspeed *= delta_t;
    current_speed += relspeed;
    is_resting = false;
    pcmovable->Move (newpos-oldpos);

    return true;
  }
}

bool celPcGravity::HandleForce (float delta_t, iCollider* this_collider,
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

