/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_DYNMOVE__
#define __CEL_PF_DYNMOVE__

#include "cstypes.h"
#include "csutil/scf.h"

class csVector3;
class csBox3;
class csOrthoTransform;
class csPlane3;
struct iDynamicSystem;
struct iRigidBody;
struct iPcMesh;
struct iPcDynamicBody;

SCF_VERSION (iPcDynamicSystem, 0, 0, 1);

/**
 * Property class representing a dynamic system.
 */
struct iPcDynamicSystem : public iBase
{
  /// Get the dynamic system.
  virtual iDynamicSystem* GetDynamicSystem () = 0;
  /**
   * Define the time we want for one step. By default this is 0.01 milliseconds.
   */
  virtual void SetStepTime (float delta) = 0;
  /// Get the step time.
  virtual float GetStepTime () const = 0;

  /**
   * During the specified time (in milliseconds) add the force every
   * step to the given body. This function is called if you do
   * pcbody->AddForceDuration().
   */
  virtual void AddForceDuration (iPcDynamicBody* pcbody,
  	const csVector3& force, float seconds) = 0;

  /**
   * During the next frame add the force every step. This function is called
   * if you do pcbody->AddForceFrame().
   */
  virtual void AddForceFrame (iPcDynamicBody* pcbody,
  	const csVector3& force) = 0;

  /**
   * Remove the given body from the force queues (filled with AddForceFrame()
   * and AddForceDuration()).
   */
  virtual void ClearForces (iPcDynamicBody* pcbody) = 0;

  /**
   * Clear all forces.
   */
  virtual void ClearAllForces () = 0;
};

SCF_VERSION (iPcDynamicBody, 0, 1, 0);

/**
 * A dynamic body.
 */
struct iPcDynamicBody : public iBase
{
  /**
   * Set the mesh which this body will control.
   * If not called then this property class will automatically
   * look for 'pcmesh'.
   */
  virtual void SetMesh (iPcMesh* mesh) = 0;

  /// Get the mesh.
  virtual iPcMesh* GetMesh () = 0;

  /**
   * Set the dynamic system to use. This is required.
   */
  virtual void SetDynamicSystem (iPcDynamicSystem* dynsys) = 0;

  /// Get the dynamic system.
  virtual iPcDynamicSystem* GetDynamicSystem () = 0;

  /// Get the rigid body.
  virtual iRigidBody* GetBody () = 0;

  /**
   * Setup various parameters for the collider. You must call this
   * before calling Attach.... otherwise you just get the defaults
   * (friction=1, mass=1, elasticity=0, density=1, softness=0).
   */
  virtual void SetParameters (float friction, float density,
  	float elasticity, float softness, float mass) = 0;

  /**
   * Make static.
   */
  virtual void MakeStatic (bool stat) = 0;
  /// Return true if static.
  virtual bool IsStatic () const = 0;

  /// Create a sphere rigid body.
  virtual void AttachColliderSphere (float radius, const csVector3& offset) = 0;

  /// Create a cylinder rigid body.
  virtual void AttachColliderCylinder (float length, float radius,
  	const csOrthoTransform& trans) = 0;

  /// Create a box rigid body.
  virtual void AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans) = 0;

  /// Create a plane rigid body.
  virtual void AttachColliderPlane (const csPlane3& plane) = 0;

  /// Create a mesh rigid body.
  virtual void AttachColliderMesh () = 0;

  /**
   * Add a force once to the given object.
   */
  virtual void AddForceOnce (const csVector3& force) = 0;

  /**
   * During the specified time (in seconds) add the force every
   * step.
   */
  virtual void AddForceDuration (const csVector3& force, float seconds) = 0;

  /**
   * During the next frame add the force every step.
   */
  virtual void AddForceFrame (const csVector3& force) = 0;

  /**
   * Clear the permanent forces on this body.
   */
  virtual void ClearForces () = 0;
};

#endif // __CEL_PF_DYNMOVE__

