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
#include "plugins/propclass/move/movefact.h"
#include "propclass/mesh.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "physicallayer/databhlp.h"
#include "behaviourlayer/behave.h"
#include "celtool/stdparams.h"
#include "cstool/collider.h"
#include "csutil/util.h"
#include "csutil/debug.h"
#include "csutil/csobject.h"
#include "csutil/scanstr.h"
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
#include "csgeom/polymesh.h"
#include "igeom/polymesh.h"
#include "igeom/objmodel.h"
#include "imesh/object.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY(Movable, "pcmovable")
CEL_IMPLEMENT_FACTORY(Solid, "pcsolid")
CEL_IMPLEMENT_FACTORY(MovableConstraintCD, "pcmovableconst_cd")
CEL_IMPLEMENT_FACTORY(Gravity, "pcgravity")

void MoveReport (iObjectRegistry* object_reg, const char* msg, ...)
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

void MoveNotify (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_NOTIFY, "cel.persistance",
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
  DG_TYPE (this, "celPcMovable()");
}

celPcMovable::~celPcMovable ()
{
  RemoveAllConstraints ();
}

#define MOVABLE_SERIAL 1

csPtr<iCelDataBuffer> celPcMovable::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MOVABLE_SERIAL);
  databuf->SetDataCount (1+1+constraints.Length ());
  size_t i, j = 0;
  csRef<iCelPropertyClass> pc;
  if (pcmesh) pc = SCF_QUERY_INTERFACE (pcmesh, iCelPropertyClass);
  databuf->GetData (j++)->Set (pc);
  databuf->GetData (j++)->Set ((uint16)constraints.Length ());
  for (i = 0 ; i < (size_t)constraints.Length () ; i++)
  {
    iPcMovableConstraint* pcm = constraints[i];
    csRef<iCelPropertyClass> pc (SCF_QUERY_INTERFACE (pcm, iCelPropertyClass));
    databuf->GetData (j++)->Set (pc);
  }
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMovable::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MOVABLE_SERIAL)
  {
    MoveReport (object_reg,"serialnr != MOVABLE_SERIAL.  Cannot load.");
    return false;
  }

  int cnt_total = databuf->GetDataCount ();
  celData* cd;
  RemoveAllConstraints ();
  int i, j = 0;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    MoveReport (object_reg,"Property class not specified.  Cannot load.");
    return false;
  }
  iCelPropertyClass* pc = cd->value.pc;
  csRef<iPcMesh> pcm;
  if (pc)
  {
    pcm = SCF_QUERY_INTERFACE (pc, iPcMesh);
    CS_ASSERT (pcm != 0);
  }
  SetMesh (pcm);

  cd = databuf->GetData (j++);
  if (!cd)
  {
    MoveReport (object_reg,"cnt_constraints not specified.  Cannot load.");
    return false;
  }
  int cnt_constraints = cd->value.uw;
  if (cnt_total != 1+1+cnt_constraints)
  {
    MoveReport (object_reg,"Cnt_total is %d and should be %d.  Cannot load.",cnt_total, 2+cnt_constraints);
    return false;
  }

  for (i = 0 ; i < cnt_constraints ; i++)
  {
    cd = databuf->GetData (j++);
    if (!cd)
    {
      MoveReport (object_reg,"iPcMovableConstraint not specified for element %d.  Cannot load.",i);
      return false;
    }
    csRef<iPcMovableConstraint> pcm (SCF_QUERY_INTERFACE (cd->value.pc,
    	iPcMovableConstraint));
    CS_ASSERT (pcm != 0);
    AddConstraint (pcm);
  }

  return true;
}

void celPcMovable::SetMesh (iPcMesh* mesh)
{
  pcmesh = mesh;
}

iPcMesh* celPcMovable::GetMesh ()
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
  return pcmesh;
}

int celPcMovable::Move (iSector* sector, const csVector3& pos)
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
  CS_ASSERT (pcmesh != 0);
  csVector3 realpos;
  size_t i;
  for (i = 0 ; i < (size_t)constraints.Length () ; i++)
  {
    iPcMovableConstraint* c = constraints[i];
    int rc = c->CheckMove (sector, pos, pos, realpos);
    if (rc == CEL_MOVE_FAIL) return rc;
  }
  pcmesh->MoveMesh (sector, pos);
  FirePropertyChangeCallback(CEL_PCMOVABLE_PROPERTY_POSITION);
  return CEL_MOVE_SUCCEED;
}

int celPcMovable::Move (const csVector3& relpos)
{
//@@@ TODO IMPLEMENT TRAVERSAL THROUGH PORTALS!
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  CS_ASSERT (pcmesh != 0);
  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  iSector* sector = movable->GetSectors ()->Get (0);
  csVector3 start = movable->GetPosition ();
  csVector3 end = start + relpos;
  csVector3 realpos = end;
  bool partial = false;
  size_t i;
  for (i = 0 ; i < (size_t)constraints.Length () ; i++)
  {
    iPcMovableConstraint* c = constraints[i];
    int rc = c->CheckMove (sector, start, end, realpos);
    if (rc == CEL_MOVE_FAIL) return rc;
    if (rc == CEL_MOVE_PARTIAL) { end = realpos; partial = true; }
  }
  pcmesh->MoveMesh (sector, realpos);
  FirePropertyChangeCallback(CEL_PCMOVABLE_PROPERTY_POSITION);
  return partial ? CEL_MOVE_PARTIAL : CEL_MOVE_SUCCEED;
}

void celPcMovable::AddConstraint (iPcMovableConstraint* constraint)
{
  int idx = constraints.Find (constraint);
  if (idx != -1) return;
  constraints.Push (constraint);
}

void celPcMovable::RemoveConstraint (iPcMovableConstraint* constraint)
{
  constraints.Delete (constraint);
}

void celPcMovable::RemoveAllConstraints ()
{
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
  DG_TYPE (this, "celPcSolid()");
  no_collider = false;
}

celPcSolid::~celPcSolid ()
{
}

#define SOLID_SERIAL 1

csPtr<iCelDataBuffer> celPcSolid::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (SOLID_SERIAL);
  databuf->SetDataCount (1);
  csRef<iCelPropertyClass> pc;
  if (pcmesh) pc = SCF_QUERY_INTERFACE (pcmesh, iCelPropertyClass);
  databuf->GetData (0)->Set (pc);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcSolid::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != SOLID_SERIAL)
  {
    MoveReport (object_reg,"serialnr != SOLID_SERIAL.  Cannot load.");
    return false;
  }
  if (databuf->GetDataCount () != 1)
  {
    MoveReport (object_reg,"Msg does not specify the correct data.  Cannot load.");
    return false;
  }
  celData* cd;
  collider_wrap = 0;
  no_collider = false;
  cd = databuf->GetData (0);
  if (!cd)
  {
    MoveReport (object_reg,"iPcMesh prop class not specified.  Cannot load.");
    return false;
  }
  csRef<iPcMesh> pcm;
  if (cd->value.pc)
    pcm = SCF_QUERY_INTERFACE (cd->value.pc, iPcMesh);
  SetMesh (pcm);
  return true;
}

void celPcSolid::SetMesh (iPcMesh* mesh)
{
  pcmesh = mesh;
  collider_wrap = 0;
  no_collider = false;
}

iCollider* celPcSolid::GetCollider ()
{
  if (collider_wrap) return collider_wrap->GetCollider ();
  if (no_collider) return 0;
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  CS_ASSERT (pcmesh != 0);
  iPolygonMesh* pmesh = pcmesh->GetMesh ()->GetMeshObject ()->
	GetObjectModel ()->GetPolygonMeshColldet ();
  if (pmesh)
  {
    csRef<iCollideSystem> cdsys = CS_QUERY_REGISTRY (object_reg,
    	iCollideSystem);
    CS_ASSERT (cdsys != 0);
    collider_wrap = csPtr<csColliderWrapper> (new csColliderWrapper (
    	pcmesh->GetMesh ()->QueryObject (), cdsys, pmesh));
    return collider_wrap->GetCollider ();
  }
  no_collider = true;
  return 0;
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
  csFlags flags;

public:
  celPolygonMeshTriangle (const csVector3& start, const csVector3& end)
  {
    SCF_CONSTRUCT_IBASE (0);
    vertices[0] = start;
    vertices[1] = start;
    vertices[2] = end;
    polygons[0].num_vertices = 3;
    polygons[0].vertices = vertex_indices;
    vertex_indices[0] = 0;
    vertex_indices[1] = 1;
    vertex_indices[2] = 2;
    flags.Set (CS_POLYMESH_CONVEX);
  }
  virtual ~celPolygonMeshTriangle () { }

  SCF_DECLARE_IBASE;

  virtual int GetVertexCount () { return 3; }
  virtual csVector3* GetVertices () { return vertices; }
  virtual int GetPolygonCount () { return 1; }
  virtual csMeshedPolygon* GetPolygons () { return polygons; }
  virtual int GetTriangleCount () { return 1; }
  virtual csTriangle* GetTriangles () { return (csTriangle*)vertex_indices; }
  virtual void Lock () { }
  virtual void Unlock () { }
  virtual csFlags& GetFlags () { return flags; }
  virtual uint32 GetChangeNumber () const { return 0; }
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
  CS_ASSERT (cdsys != 0);
  DG_TYPE (this, "celPcMovableConstraintCD()");
}

celPcMovableConstraintCD::~celPcMovableConstraintCD ()
{
}

#define MOVABLECONST_CD_SERIAL 1

csPtr<iCelDataBuffer> celPcMovableConstraintCD::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MOVABLECONST_CD_SERIAL);
  databuf->SetDataCount (0);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMovableConstraintCD::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MOVABLECONST_CD_SERIAL)
  {
    MoveReport (object_reg,"serialnr != MOVABLECONST_CD_SERIAL.  Cannot load.");
    return false;
  }
  if (databuf->GetDataCount () != 0)
  {
    MoveReport (object_reg,"Msg does not specify correct data.  Cannot load.");
    return false;
  }
  return true;
}

int celPcMovableConstraintCD::CheckMove (iSector* sector,
	const csVector3& start, const csVector3& end, csVector3& pos)
{
  csRef<iPcMesh> pcmesh (CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
  	iPcMesh));
  CS_ASSERT (pcmesh != 0);
  csRef<iPcSolid> pcsolid (CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
  	iPcSolid));
  CS_ASSERT (pcsolid != 0);

  // See if we're handling a single point.
  bool single_point = csSquaredDist::PointPoint (start, end) < EPSILON;

  int rc = CEL_MOVE_SUCCEED;
  pos = end;
  if (pcsolid->GetCollider ())
  {
    // Create a collider for detecting collision detection along the
    // movement vector.
    csRef<iCollider> path_collider;
    csReversibleTransform path_trans;	// Identity
    if (!single_point)
    {
      csRef<celPolygonMeshTriangle> pmtri =
	csPtr<celPolygonMeshTriangle> (new celPolygonMeshTriangle (start,end));
      path_collider = cdsys->CreateCollider (pmtri);
    }

    // Change the transform from the mesh to the new position.
    csReversibleTransform trans = pcmesh->GetMesh ()->GetMovable ()->
    	GetTransform ();
    trans.SetOrigin (end);

    cdsys->ResetCollisionPairs ();
    csRef<iCelEntityList> list (
    	pl->FindNearbyEntities (sector, start, 10/*@@@*/));
    size_t i;
    for (i = 0 ; i < list->GetCount () ; i++)
    {
      iCelEntity* ent = list->Get (i);
      if (ent == entity) continue;	// Ignore collisions with ourselves.
      csRef<iPcSolid> pcsolid_ent (CEL_QUERY_PROPCLASS (
      	ent->GetPropertyClassList (), iPcSolid));
      if (pcsolid_ent)
      {
        if (pcsolid_ent->GetCollider ())
        {
          csRef<iPcMesh> pcmesh_ent (CEL_QUERY_PROPCLASS (
		ent->GetPropertyClassList (), iPcMesh));
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
	    if (ret) rc = CEL_MOVE_FAIL;
	  }
	}
      }
    }
  }
  return rc;
}

//---------------------------------------------------------------------------

Property* celPcGravity::properties = 0;
int celPcGravity::propertycount = 0;
csStringID celPcGravity::action_applypermanentforce = csInvalidStringID;
csStringID celPcGravity::id_force = csInvalidStringID;

void celPcGravity::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT( pl != 0 );

    propertycount = 1;
    properties = new Property[propertycount];

    properties[propid_weight].id = pl->FetchStringID (
    	"cel.property.weight");
    properties[propid_weight].datatype = CEL_DATA_FLOAT;
    properties[propid_weight].readonly = false;
    properties[propid_weight].desc = "Weight of this object";

    action_applypermanentforce = pl->FetchStringID (
    	"cel.action.ApplyPermanentForce");
    id_force = pl->FetchStringID (
    	"cel.parameter.force");
  }
}

SCF_IMPLEMENT_IBASE_EXT (celPcGravity)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcGravity)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcGravity::PcGravity)
  SCF_IMPLEMENTS_INTERFACE (iPcGravity)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcGravity::celPcGravity (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcGravity);
  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
  CS_ASSERT (cdsys != 0);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != 0);
  weight = 1;
  is_resting = false;
  active = true;

  current_speed.Set (0, 0, 0);
  infinite_forces.Set (0, 0, 0);

  has_gravity_collider = false;
  gravity_mesh = 0;

  pl->CallbackPCEveryFrame (this, cscmdPreProcess);

  UpdateProperties (object_reg);
  propdata = new void* [propertycount];

  props = properties;
  propcount = &propertycount;

  propdata[propid_weight] = &weight;
}

celPcGravity::~celPcGravity ()
{
  ClearForces ();
}

#define GRAVITY2_SERIAL 1

csPtr<iCelDataBuffer> celPcGravity::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (GRAVITY2_SERIAL);
  databuf->SetDataCount (8+forces.Length ()*2);
  celDataBufHelper db(databuf);

  csRef<iCelPropertyClass> pc;
  if (pcmovable) pc = SCF_QUERY_INTERFACE (pcmovable, iCelPropertyClass);
  else pc = 0;
  db.Set (pc);
  if (pcsolid) pc = SCF_QUERY_INTERFACE (pcsolid, iCelPropertyClass);
  else pc = 0;
  db.Set (pc);

  db.Set (weight);
  db.Set (current_speed);
  db.Set (infinite_forces);
  db.Set (is_resting);
  db.Set (active);

  db.Set ((uint16)forces.Length ());
  size_t i;
  for (i = 0 ; i < (size_t)forces.Length () ; i++)
  {
    celForce* f = forces[i];
    db.Set (f->force);
    db.Set (f->time_remaining);
  }
  
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcGravity::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != GRAVITY2_SERIAL)
  {
    MoveReport (object_reg,"serialnr != GRAVITY2_SERIAL.  Cannot load.");
    return false;
  }
  celDataBufHelper db(databuf);
  iCelPropertyClass* pc;

  db.Get(pc);
  csRef<iPcMovable> pcm;
  if (pc) pcm = SCF_QUERY_INTERFACE (pc, iPcMovable);
  SetMovable (pcm);

  db.Get(pc);
  csRef<iPcSolid> pcs;
  if (pc) pcs = SCF_QUERY_INTERFACE (pc, iPcSolid);
  SetSolid (pcs);

  if (!db.AllOk())
  {
    MoveReport (object_reg,"databuf not ok.  Cannot load.");
    return false;
  }

  db.Get(weight);
  db.Get(current_speed);
  db.Get(infinite_forces);
  db.Get(is_resting);
  db.Get(active);

  if (!db.AllOk())
  {
    MoveReport (object_reg,"databuf not ok.  Cannot load.");
    return false;
  }

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
  {
    MoveReport (object_reg,"databuf not ok.  Cannot load.");
    return false;
  }

  return true;
}

void celPcGravity::CreateGravityCollider (iPcMesh* /*mesh*/)
{
  // @@@ NOT IMPLEMENTED YET!
}

void celPcGravity::CreateGravityCollider (const csVector3& dim,
  	const csVector3& offs)
{
  gravity_mesh = 0;
  has_gravity_collider = true;
  gravity_dim = dim;
  gravity_offs = offs;

  csVector3 d = dim * .5;
  csBox3 box (-d + offs, d + offs);
  csPolygonMeshBox* pmcube = new csPolygonMeshBox (box);
  gravity_collider = cdsys->CreateCollider (pmcube);
  pmcube->DecRef ();
}

void celPcGravity::SetMovable (iPcMovable* movable)
{
  pcmovable = movable;
}

iPcMovable* celPcGravity::GetMovable ()
{
  if (!pcmovable)
  {
    pcmovable = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
    	iPcMovable);
    CS_ASSERT (pcmovable != 0);
  }
  return pcmovable;
}

void celPcGravity::SetSolid (iPcSolid* solid)
{
  pcsolid = solid;
}

iPcSolid* celPcGravity::GetSolid ()
{
  if (!pcsolid)
  {
    pcsolid = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
    	iPcSolid);
    CS_ASSERT (pcsolid != 0);
  }
  return pcsolid;
}

void celPcGravity::ClearForces ()
{
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

void celPcGravity::TickEveryFrame ()
{
  if (!active) return;

  GetMovable ();
  iMovable* movable = pcmovable->GetMesh ()->GetMesh ()->GetMovable ();
  csReversibleTransform& w2o = movable->GetTransform ();
  GetSolid ();
  iCollider* collider = pcsolid->GetCollider ();

  float delta_t1;
  csTicks elapsed_time = vc->GetElapsedTicks ();
  if (!elapsed_time) return;
  delta_t1 = elapsed_time/1000.0;

  csRef<iCelEntityList> cd_list (pl->FindNearbyEntities (movable->
    	GetSectors ()->Get (0),
    	w2o.GetOrigin (), 10/*@@@*/));

  // Handle physics so that we only call HandleForce for 0.1 second
  // maximum. This ensures that it will work ok on slower systems.
  is_resting = true;
  float dt1 = delta_t1;
  while (dt1 > 0)
  {
    //@@@ TEMPORARY ONLY! SHOULD BE DONE WITH MORE ACCURATE CD.
    float dt = MIN (dt1, .02);
    dt1 -= dt;
    HandleForce (dt, collider, cd_list);
  }
}

int celPcGravity::GetColliderArray (iCelEntityList* cd_list,
  	iCollider**& colliders, csReversibleTransform**& transforms)
{
  static csReversibleTransform identity;

  int num_cdlist = cd_list->GetCount ();
  colliders = 0;
  transforms = 0;
  if (!num_cdlist) return 0;
  colliders = new iCollider* [num_cdlist];
  transforms = new csReversibleTransform* [num_cdlist];
  int i;
  int num_colliders = 0;
  for (i = 0 ; i < num_cdlist ; i++)
  {
    iCelEntity* ent = cd_list->Get (i);
    csRef<iPcSolid> solid_ent (
    	CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcSolid));
    if (!solid_ent)
      continue;
    if (!solid_ent->GetCollider ())
      continue;

    csRef<iPcMovable> mov_ent (
    	CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcMovable));
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
  int rc = csColliderHelper::CollidePath (cdsys, this_collider, &test,
  	.3f/*@@@ Get from bbox!*/, newpos, num_colliders, colliders, transforms);
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
    size_t i;
    for (i = 0 ; i < (size_t)forces.Length () ; i++)
    {
      celForce* f = forces[i];
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
    while (i < (size_t)forces.Length ())
    {
      celForce* f = forces[i];
      f->time_remaining -= smallest_time;
      if (f->time_remaining < EPSILON)
      {
	forces.DeleteIndex (i);
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

bool celPcGravity::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_applypermanentforce)
  {
    CEL_FETCH_VECTOR3_PAR (force,params,id_force);
    if (!p_force) return false;
    ApplyPermanentForce (force);
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------

