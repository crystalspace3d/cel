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

#ifndef __CEL_PF_GRAVITY__
#define __CEL_PF_GRAVITY__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCollider;
struct iPcMesh;
struct iPcMovable;
struct iPcSolid;
class csVector3;

SCF_DECLARE_FAST_INTERFACE (iPcGravity)

SCF_VERSION (iPcGravity, 0, 0, 1);

/**
 * Gravity handling property class.
 */
struct iPcGravity : public iBase
{
  /**
   * Create a gravity collider from the given mesh.
   * A gravity collider is a small box at the bottom of the
   * mesh.
   */
  virtual void CreateGravityCollider (iPcMesh* mesh) = 0;

  /**
   * Create a gravity collider with the specified dimensions.
   */
  virtual void CreateGravityCollider (const csVector3& dim,
  	const csVector3& offs) = 0;

  /**
   * Get the gravity collider. If this returns NULL we have
   * no gravity collider.
   */
  virtual iCollider* GetGravityCollider () = 0;

  /**
   * Set the movable on which we will operate. If not set then
   * a default movable will be found and used instead.
   */
  virtual void SetMovable (iPcMovable* movable) = 0;
  /**
   * Get the current movable.
   */
  virtual iPcMovable* GetMovable () = 0;

  /**
   * Set the solid representation which we will additionally
   * use for collision detection. If not set then we will
   * find the default solid.
   */
  virtual void SetSolid (iPcSolid* solid) = 0;
  /**
   * Get the current solid.
   */
  virtual iPcSolid* GetSolid () = 0;

  /**
   * Set the weight of this object.
   */
  virtual void SetWeight (float w) = 0;
  /**
   * Get the weight of this object.
   */
  virtual float GetWeight () const = 0;

  /**
   * Clear all non-permanent forces.
   */
  virtual void ClearForces () = 0;
  /**
   * Clear all permanent forces.
   */
  virtual void ClearPermanentForces () = 0;
  /**
   * Reset speed (but leave forces alone).
   */
  virtual void ResetSpeed () = 0;
  /**
   * Apply a non-permanent force.
   */
  virtual void ApplyForce (const csVector3& force, float time) = 0;
  /**
   * Apply an permanent force.
   */
  virtual void ApplyPermanentForce (const csVector3& force) = 0;

  /**
   * Return true if object is resting. This happens when the
   * object did not move the last time physics was checked.
   */
  virtual bool IsResting () const = 0;

  /**
   * Activate/deactivate physics handling for this object.
   */
  virtual void SetActive (bool activate) = 0;
  /**
   * Check activation status.
   */
  virtual bool IsActive () const = 0;
};

#endif // __CEL_PF_GRAVITY__

