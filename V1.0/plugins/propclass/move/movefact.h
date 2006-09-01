/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2003 by Jorrit Tyberghein

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

#ifndef __CEL_PF_MOVEFACT__
#define __CEL_PF_MOVEFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/refarr.h"
#include "csutil/parray.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "propclass/move.h"
#include "propclass/solid.h"
#include "propclass/gravity.h"

struct iVirtualClock;
struct iCelPlLayer;
struct iCelEntity;
struct iCelEntityList;
struct iCollideSystem;
class csVector3;
class csReversibleTransform;
class csColliderWrapper;

/**
 * Factory for move.
 */
CEL_DECLARE_FACTORY(Movable)
CEL_DECLARE_FACTORY(Solid)
CEL_DECLARE_FACTORY(MovableConstraintCD)
CEL_DECLARE_FACTORY(Gravity)
CEL_DECLARE_FACTORY(Gravity2)

/**
 * This is a move property class.
 */
class celPcMovable : public scfImplementationExt1<
		     celPcMovable, celPcCommon, iPcMovable>
{
private:
  csWeakRef<iPcMesh> pcmesh;
  csRefArray<iPcMovableConstraint> constraints;

public:
  celPcMovable (iObjectRegistry* object_reg);
  virtual ~celPcMovable ();
  virtual void SetMesh (iPcMesh* mesh);
  virtual iPcMesh* GetMesh ();
  virtual int Move (iSector* sector, const csVector3& pos);
  virtual int Move (const csVector3& relpos);
  virtual void AddConstraint (iPcMovableConstraint* constraint);
  virtual void RemoveConstraint (iPcMovableConstraint* constraint);
  virtual void RemoveAllConstraints ();

  virtual const char* GetName () const { return "pcmovable"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
};

/**
 * This is a solid property class.
 */
class celPcSolid : public scfImplementationExt1<
		   celPcSolid, celPcCommon, iPcSolid>
{
private:
  csWeakRef<iPcMesh> pcmesh;
  csRef<csColliderWrapper> collider_wrap;
  bool no_collider;

public:
  celPcSolid (iObjectRegistry* object_reg);
  virtual ~celPcSolid ();
  virtual void SetMesh (iPcMesh* mesh);
  virtual iPcMesh* GetMesh () const { return pcmesh; }
  virtual iCollider* GetCollider ();

  virtual const char* GetName () const { return "pcsolid"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
};

/**
 * This is a movable constraint based on collision detection (iPcSolid).
 */
class celPcMovableConstraintCD : public scfImplementationExt1<
				 celPcMovableConstraintCD, celPcCommon,
				 iPcMovableConstraint>
{
private:
  csRef<iCollideSystem> cdsys;

public:
  celPcMovableConstraintCD (iObjectRegistry* object_reg);
  virtual ~celPcMovableConstraintCD ();
  virtual int CheckMove (iSector* sector, const csVector3& start,
  	const csVector3& end, csVector3& pos);

  virtual const char* GetName () const { return "pcmovableconst_cd"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
};

/**
 * This is a gravity property class.
 */
class celPcGravity : public scfImplementationExt1<
		     celPcGravity, celPcCommon, iPcGravity>
{
private:
  csWeakRef<iPcMovable> pcmovable;
  csWeakRef<iPcSolid> pcsolid;
  csRef<iCollider> gravity_collider;
  csRef<iCollideSystem> cdsys;
  csRef<iVirtualClock> vc;

  bool active;
  bool is_resting;
  float weight;
  csVector3 current_speed;

  struct celForce
  {
    csVector3 force;
    float time_remaining;
  };
  csPDelArray<celForce> forces;	// Forces plus duration.

  csVector3 infinite_forces;	// Infinite duration forces.

  // Information in order to be able to save from what the
  // gravity collider was created.
  bool has_gravity_collider;
  iPcMesh* gravity_mesh;
  csVector3 gravity_dim, gravity_offs;

  // Generate two arrays of colliders and transforms
  // given an entity list. Return the number of elements in the
  // arrays. Free arrays with delete[] after using.
  int GetColliderArray (iCelEntityList* cd_list,
  	iCollider**& colliders, csReversibleTransform**& transforms);

  int TestMove (iCollider* this_collider,
    int num_colliders,
    iCollider** colliders,
    csReversibleTransform** transforms,
    const csReversibleTransform& w2o,
    csVector3& newpos,
    csVector3& collider_normal);
  bool HandleForce (float delta_t, iCollider* this_collider,
    iCelEntityList* cd_list, const csVector3& force);
  bool HandleForce (float delta_t, iCollider* this_collider,
    iCelEntityList* cd_list);

  enum actionids
  {
    action_applypermanentforce = 0
  };
  enum propids
  {
    propid_weight = 0,
    propid_applyforce
  };
  static PropertyHolder propinfo;

  static csStringID id_force;
 
public:
  celPcGravity (iObjectRegistry* object_reg);
  virtual ~celPcGravity ();

  virtual void CreateGravityCollider (iPcMesh* mesh);
  virtual void CreateGravityCollider (const csVector3& dim,
  	const csVector3& offs);
  virtual iCollider* GetGravityCollider () { return gravity_collider; }
  virtual void SetMovable (iPcMovable* movable);
  virtual iPcMovable* GetMovable ();
  virtual void SetSolid (iPcSolid* solid);
  virtual iPcSolid* GetSolid ();
  virtual void SetWeight (float w) { weight = w; }
  virtual float GetWeight () const { return weight; }
  virtual void ClearForces ();
  virtual void ClearPermanentForces ();
  virtual void ResetSpeed () { current_speed.Set (0, 0, 0); }
  virtual void ApplyForce (const csVector3& force, float time);
  virtual void ApplyPermanentForce (const csVector3& force);

  virtual const char* GetName () const { return "pcgravity"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
  virtual void TickEveryFrame ();

  virtual bool IsResting () const { return is_resting; }
  virtual void SetActive (bool activate) { active = activate; }
  virtual bool IsActive () const { return active; }
};

#endif // __CEL_PF_MOVEFACT__

