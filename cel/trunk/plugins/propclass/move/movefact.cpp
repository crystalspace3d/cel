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
#include "csqsqrt.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "csgeom/trimesh.h"
#include "plugins/propclass/move/movefact.h"
#include "propclass/mesh.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "celtool/stdparams.h"
#include "csutil/util.h"
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
#include "imap/loader.h"
#include "ivaria/reporter.h"
#include "ivaria/collider.h"
#include "imesh/objmodel.h"
#include "imesh/object.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (Movable, "pcmove.movable", "pcmovable")
CEL_IMPLEMENT_FACTORY_ALT (Solid, "pcmove.solid", "pcsolid")
CEL_IMPLEMENT_FACTORY_ALT (MovableConstraintCD, "pcmove.movableconst_cd", "pcmovableconst_cd")
CEL_IMPLEMENT_FACTORY_ALT (Gravity, "pcmove.gravity", "pcgravity")

bool MoveReport (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.pcmove",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

void MoveNotify (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_NOTIFY, "cel.pcmove",
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

celPcMovable::celPcMovable (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
}

celPcMovable::~celPcMovable ()
{
  RemoveAllConstraints ();
}

void celPcMovable::SetMesh (iPcMesh* mesh)
{
  pcmesh = mesh;
}

iPcMesh* celPcMovable::GetMesh ()
{
  if (!pcmesh)
  {
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  }
  return pcmesh;
}

int celPcMovable::Move (iSector* sector, const csVector3& pos)
{
  if (!pcmesh)
  {
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  }
  CS_ASSERT (pcmesh != 0);
  csVector3 realpos;
  size_t i;
  for (i = 0 ; i < constraints.GetSize () ; i++)
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
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  }
  CS_ASSERT (pcmesh != 0);
  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  iSector* sector = movable->GetSectors ()->Get (0);
  csVector3 start = movable->GetPosition ();
  csVector3 end = start + relpos;
  csVector3 realpos = end;
  bool partial = false;
  size_t i;
  for (i = 0 ; i < constraints.GetSize () ; i++)
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
  size_t idx = constraints.Find (constraint);
  if (idx != csArrayItemNotFound) return;
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

csStringID celPcSolid::id_min = csInvalidStringID;
csStringID celPcSolid::id_max = csInvalidStringID;
PropertyHolder celPcSolid::propinfo;

celPcSolid::celPcSolid (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  no_collider = false;

  if (id_min == csInvalidStringID)
  {
    id_min = pl->FetchStringID ("min");
    id_max = pl->FetchStringID ("max");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.move.solid.action.");
    AddAction (action_setup, "Setup");
    AddAction (action_setupbox, "SetupBox");
  }
}

celPcSolid::~celPcSolid ()
{
  if (collider_wrap && collider_wrap->GetObjectParent ())
  {
    collider_wrap->GetObjectParent ()->ObjRemove (static_cast<iObject*> (
	  collider_wrap));
  }
}

void celPcSolid::SetMesh (iPcMesh* mesh)
{
  pcmesh = mesh;
  collider_wrap = 0;
  no_collider = false;
}

void celPcSolid::SetupBox (const csBox3& box)
{
  no_collider = false;
  if (!pcmesh)
  {
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  }
  collider_wrap = 0;
  if (!pcmesh->GetMesh ())
  {
    no_collider = true;
    return;
  }
  CS_ASSERT (pcmesh != 0);
  csTriangleMeshBox pmbox (box);
  csRef<iCollideSystem> cdsys = 
    	csQueryRegistry<iCollideSystem> (object_reg);
  collider_wrap.AttachNew (new csColliderWrapper (
	pcmesh->GetMesh ()->QueryObject (),
	cdsys, &pmbox));
}

iCollider* celPcSolid::GetCollider ()
{
  if (collider_wrap) return collider_wrap->GetCollider ();
  if (no_collider) return 0;
  if (!pcmesh)
  {
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  }
  CS_ASSERT (pcmesh != 0);
  if (pcmesh->GetMesh ())
  {
    csRef<iCollideSystem> cdsys = 
    	csQueryRegistry<iCollideSystem> (object_reg);
    CS_ASSERT (cdsys != 0);
    collider_wrap = csColliderHelper::InitializeCollisionWrapper (
    	cdsys, pcmesh->GetMesh ());
    return collider_wrap->GetCollider ();
  }
  no_collider = true;
  return 0;
}

bool celPcSolid::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setup:
      {
        Setup ();
        return true;
      }
    case action_setupbox:
      {
        CEL_FETCH_VECTOR3_PAR (min,params,id_min);
        if (!p_min)
          return MoveReport (object_reg,
          	"'min' parameter missing for SetupBox!");
        CEL_FETCH_VECTOR3_PAR (max,params,id_max);
        if (!p_max)
          return MoveReport (object_reg,
          	"'max' parameter missing for SetupBox!");
        SetupBox (csBox3 (min, max));
        return true;
      }
    default:
      return false;
  }
}

//---------------------------------------------------------------------------

// Private class implementing iTriangleMesh for one triangle.
// This will be used for detecting collision in the movement
// vector.
class celTriangleMeshTriangle : public scfImplementation1<
	celTriangleMeshTriangle, iTriangleMesh>
{
private:
  csVector3 vertices[3];
  csTriangle triangle;
  csFlags flags;

public:
  celTriangleMeshTriangle (const csVector3& start, const csVector3& end) :
  	scfImplementationType (this)
  {
    vertices[0] = start;
    vertices[1] = start;
    vertices[2] = end;
    triangle.a = 0;
    triangle.b = 0;
    triangle.c = 0;
    flags.Set (CS_TRIMESH_CONVEX);
  }
  virtual ~celTriangleMeshTriangle ()
  {
  }

  virtual size_t GetVertexCount () { return 3; }
  virtual csVector3* GetVertices () { return vertices; }
  virtual size_t GetTriangleCount () { return 1; }
  virtual csTriangle* GetTriangles () { return &triangle; }
  virtual void Lock () { }
  virtual void Unlock () { }
  virtual csFlags& GetFlags () { return flags; }
  virtual uint32 GetChangeNumber () const { return 0; }
};

//---------------------------------------------------------------------------

celPcMovableConstraintCD::celPcMovableConstraintCD (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);
  CS_ASSERT (cdsys != 0);
}

celPcMovableConstraintCD::~celPcMovableConstraintCD ()
{
}

int celPcMovableConstraintCD::CheckMove (iSector* sector,
	const csVector3& start, const csVector3& end, csVector3& pos)
{
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  CS_ASSERT (pcmesh != 0);
  csRef<iPcSolid> pcsolid = celQueryPropertyClassEntity<iPcSolid> (entity);
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
      csRef<celTriangleMeshTriangle> pmtri =
      	csPtr<celTriangleMeshTriangle> (new celTriangleMeshTriangle (
	      start,end));
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
      csRef<iPcSolid> pcsolid_ent = celQueryPropertyClassEntity<iPcSolid> (ent);
      if (pcsolid_ent)
      {
        if (pcsolid_ent->GetCollider ())
        {
          csRef<iPcMesh> pcmesh_ent = celQueryPropertyClassEntity<iPcMesh> (ent);
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

csStringID celPcGravity::id_force = csInvalidStringID;

PropertyHolder celPcGravity::propinfo;

celPcGravity::celPcGravity (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);
  CS_ASSERT (cdsys != 0);
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  CS_ASSERT (vc != 0);
  weight = 1;
  is_resting = false;
  active = true;

  current_speed.Set (0, 0, 0);
  infinite_forces.Set (0, 0, 0);

  has_gravity_collider = false;
  gravity_mesh = 0;

  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);

  propholder = &propinfo;

  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.move.gravity.action.");
    AddAction (action_applypermanentforce, "ApplyPermanentForce");
  }

  propinfo.SetCount (1);
  AddProperty (propid_weight, "weight",
  	CEL_DATA_FLOAT, false, "Weight of this object", &weight);

  if (id_force == csInvalidStringID)
  {
    id_force = pl->FetchStringID ("force");
  }
}

celPcGravity::~celPcGravity ()
{
  ClearForces ();
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
  csTriangleMeshBox* pmcube = new csTriangleMeshBox (box);
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
    pcmovable = celQueryPropertyClassEntity<iPcMovable> (entity);
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
    pcsolid = celQueryPropertyClassEntity<iPcSolid> (entity);
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
    float dt = csMin (dt1, .02f);
    dt1 -= dt;
    HandleForce (dt, collider, cd_list);
  }
}

int celPcGravity::GetColliderArray (iCelEntityList* cd_list,
	iCollider**& colliders, csReversibleTransform**& transforms)
{
  static csReversibleTransform identity;

  size_t num_cdlist = cd_list->GetCount ();
  colliders = 0;
  transforms = 0;
  if (!num_cdlist) return 0;
  colliders = new iCollider* [num_cdlist];
  transforms = new csReversibleTransform* [num_cdlist];
  size_t i;
  int num_colliders = 0;
  for (i = 0 ; i < num_cdlist ; i++)
  {
    iCelEntity* ent = cd_list->Get (i);
    csRef<iPcSolid> solid_ent = celQueryPropertyClassEntity<iPcSolid> (ent);
    if (!solid_ent)
      continue;
    if (!solid_ent->GetCollider ())
      continue;

    csRef<iPcMovable> mov_ent = celQueryPropertyClassEntity<iPcMovable> (ent);
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
    size_t num_pairs = cdsys->GetCollisionPairCount ();
    for (size_t k = 0 ; k < num_pairs ; k++)
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
    float disttot = csQsqrt (csSquaredDist::PointPoint (oldpos,
    	  desired_endpos));
    float dist = csQsqrt (csSquaredDist::PointPoint (oldpos, newpos));
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
    for (i = 0 ; i < forces.GetSize () ; i++)
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
    while (i < forces.GetSize ())
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

bool celPcGravity::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  if (idx == action_applypermanentforce)
  {
    CEL_FETCH_VECTOR3_PAR (force,params,id_force);
    if (!p_force) return false;
    ApplyPermanentForce (force);
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------

