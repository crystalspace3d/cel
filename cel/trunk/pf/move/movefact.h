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

#ifndef __CEL_PF_MOVEFACT__
#define __CEL_PF_MOVEFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/csvector.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pl/facttmpl.h"
#include "pf/common/stdpcimp.h"
#include "pf/move.h"
#include "pf/solid.h"
#include "pf/gravity.h"

struct iVirtualClock;
struct iCelPlLayer;
struct iCelEntity;
struct iCelEntityList;
struct iCollideSystem;
class csVector3;
class csReversibleTransform;

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
class celPcMovable : public celPcCommon
{
private:
  iPcMesh* pcmesh;
  csVector constraints;

public:
  celPcMovable (iObjectRegistry* object_reg);
  virtual ~celPcMovable ();
  void SetMesh (iPcMesh* mesh);
  iPcMesh* GetMesh ();
  int Move (iSector* sector, const csVector3& pos);
  int Move (const csVector3& relpos);
  void AddConstraint (iPcMovableConstraint* constraint);
  void RemoveConstraint (iPcMovableConstraint* constraint);
  void RemoveAllConstraints ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmovable"; }
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcMovable : public iPcMovable
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMovable);
    virtual void SetMesh (iPcMesh* mesh)
    {
      scfParent->SetMesh (mesh);
    }
    virtual iPcMesh* GetMesh ()
    {
      return scfParent->GetMesh ();
    }
    virtual int Move (iSector* sector, const csVector3& pos)
    {
      return scfParent->Move (sector, pos);
    }
    virtual int Move (const csVector3& relpos)
    {
      return scfParent->Move (relpos);
    }
    virtual void AddConstraint (iPcMovableConstraint* constraint)
    {
      scfParent->AddConstraint (constraint);
    }
    virtual void RemoveConstraint (iPcMovableConstraint* constraint)
    {
      scfParent->RemoveConstraint (constraint);
    }
    virtual void RemoveAllConstraints ()
    {
      scfParent->RemoveAllConstraints ();
    }
  } scfiPcMovable;
};

/**
 * This is a solid property class.
 */
class celPcSolid : public celPcCommon
{
private:
  iPcMesh* pcmesh;
  iCollider* collider;

public:
  celPcSolid (iObjectRegistry* object_reg);
  virtual ~celPcSolid ();
  void SetMesh (iPcMesh* mesh);
  iPcMesh* GetMesh () const { return pcmesh; }
  virtual iCollider* GetCollider ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcsolid"; }
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcSolid : public iPcSolid
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcSolid);
    virtual void SetMesh (iPcMesh* mesh)
    {
      scfParent->SetMesh (mesh);
    }
    virtual iPcMesh* GetMesh () const
    {
      return scfParent->GetMesh ();
    }
    virtual iCollider* GetCollider ()
    {
      return scfParent->GetCollider ();
    }
  } scfiPcSolid;
};

/**
 * This is a movable constraint based on collision detection (iPcSolid).
 */
class celPcMovableConstraintCD : public celPcCommon
{
private:
  iCollideSystem* cdsys;

public:
  celPcMovableConstraintCD (iObjectRegistry* object_reg);
  virtual ~celPcMovableConstraintCD ();
  int CheckMove (iSector* sector, const csVector3& start,
  	const csVector3& end, csVector3& pos);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmovableconst_cd"; }
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcMovableConstraintCD : public iPcMovableConstraint
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMovableConstraintCD);
    virtual int CheckMove (iSector* sector, const csVector3& start,
		    const csVector3& end, csVector3& pos)
    {
      return scfParent->CheckMove (sector, start, end, pos);
    }
  } scfiPcMovableConstraint;
};

/**
 * This is a gravity property class.
 */
class celPcGravity : public celPcCommon
{
private:
  iPcMovable* pcmovable;
  iPcSolid* pcsolid;
  iCollider* gravity_collider;
  iCollideSystem* cdsys;
  iCelPlLayer* pl;
  iVirtualClock* vc;

  bool active;
  bool is_resting;
  float weight;
  csVector3 current_speed;

  struct celForce
  {
    csVector3 force;
    float time_remaining;
  };
  csVector forces;		// Forces plus duration.

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

public:
  celPcGravity (iObjectRegistry* object_reg);
  virtual ~celPcGravity ();
  bool HandleEvent (iEvent& ev);

  void CreateGravityCollider (iPcMesh* mesh);
  void CreateGravityCollider (const csVector3& dim,
  	const csVector3& offs);
  iCollider* GetGravityCollider () { return gravity_collider; }
  void SetMovable (iPcMovable* movable);
  iPcMovable* GetMovable ();
  void SetSolid (iPcSolid* solid);
  iPcSolid* GetSolid ();
  void SetWeight (float w) { weight = w; }
  float GetWeight () const { return weight; }
  void ClearForces ();
  void ClearPermanentForces ();
  void ResetSpeed () { current_speed.Set (0, 0, 0); }
  void ApplyForce (const csVector3& force, float time);
  void ApplyPermanentForce (const csVector3& force);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcgravity2"; }
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcGravity : public iPcGravity
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcGravity);
    virtual void CreateGravityCollider (iPcMesh* mesh)
    {
      scfParent->CreateGravityCollider (mesh);
    }
    virtual void CreateGravityCollider (const csVector3& dim,
  	const csVector3& offs)
    {
      scfParent->CreateGravityCollider (dim, offs);
    }
    virtual iCollider* GetGravityCollider ()
    {
      return scfParent->GetGravityCollider ();
    }
    virtual void SetMovable (iPcMovable* movable)
    {
      scfParent->SetMovable (movable);
    }
    virtual iPcMovable* GetMovable ()
    {
      return scfParent->GetMovable ();
    }
    virtual void SetSolid (iPcSolid* solid)
    {
      scfParent->SetSolid (solid);
    }
    virtual iPcSolid* GetSolid ()
    {
      return scfParent->GetSolid ();
    }
    virtual void SetWeight (float w)
    {
      scfParent->SetWeight (w);
    }
    virtual float GetWeight () const
    {
      return scfParent->GetWeight ();
    }
    virtual void ClearForces ()
    {
      scfParent->ClearForces ();
    }
    virtual void ClearPermanentForces ()
    {
      scfParent->ClearPermanentForces ();
    }
    virtual void ResetSpeed ()
    {
      scfParent->ResetSpeed ();
    }
    virtual void ApplyForce (const csVector3& force, float time)
    {
      scfParent->ApplyForce (force, time);
    }
    virtual void ApplyPermanentForce (const csVector3& force)
    {
      scfParent->ApplyPermanentForce (force);
    }
    virtual bool IsResting () const
    {
      return scfParent->is_resting;
    }
    virtual void SetActive (bool activate)
    {
      scfParent->active = activate;
    }
    virtual bool IsActive () const
    {
      return scfParent->active;
    }
  } scfiPcGravity;
  friend struct PcGravity;

  // Not an embedded event handler to avoid circular references!!!
  class EventHandler : public iEventHandler
  {
  private:
    celPcGravity* parent;

  public:
    EventHandler (celPcGravity* parent)
    {
      SCF_CONSTRUCT_IBASE (NULL);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler () { }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
};

#endif // __CEL_PF_MOVEFACT__

