/*
    Crystal Space Entity Layer
    Copyright (C) 2001 PlaneShift Team (info@planeshift.it,
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

/*
 * This code is heavily based on pslinmove from the PlaneShift project.
 * Thanks a lot for making this!
 */

#ifndef __CEL_PF_LINEAR_MOVE__
#define __CEL_PF_LINEAR_MOVE__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/strhash.h"

class csReversibleTransform;
struct iDataBuffer;
struct iSector;
struct iPath;
struct iPcCollisionDetection;
struct iPcMesh;
struct iMeshWrapper;


/**
 * Inherit this class if you want to know when gravity is applied to a certain
 * iPcLinearMovement. This is usefully if you for example want to apply
 * certain gravity or something on the way up, but not on the way down
 * (Parachutes and things like that).
 * Register this callback with iPcLinearMovement::AddGravityCallback(), and
 * remove it with iPcLinearMovement::RemoveGravityCallback()
 */
struct iPcGravityCallback : public virtual iBase
{
  SCF_INTERFACE (iPcGravityCallback, 0, 0, 2);

  /**
   * The gravity is applied on this frame on the iPcLinearMovementiPcLinearMovement
   * of this entity, ie its vertical velocity is negative.
   */
  virtual void Callback () = 0;
};

/**
 * This property class controls movement of an entity in a realistic
 * manner. It should be used in combination with iPcMesh but otherwise
 * doesn't depend on any other property classes. So in that sense
 * it is unrelated to the other movement property classes.
 *
 * This property class supports dead reckoning which is useful for
 * networking.
 *
 * This property class can send out the following messages:
 * - 'cel.move.impossible' (old 'pclinearmovement_stuck'):
 *   sent when couldn't move at all.
 * - 'cel.move.interrupted' (old 'pclinearmovement_collision'):
 *   sent when we could move but not all the way.
 * - 'cel.move.arrived' (old 'pclinearmovement_arrived'):
 *   when we arrived without problems.
 *
 * This property class supports the following actions (add prefix 'cel.move.linear.action.'
 * if you want to access this action through a message):
 * - InitCD: parameters 'body' (vector3), 'legs' (vector3), and 'offset'
 *     (vector3 default=0,0,0).
 * - InitCDMesh: parameters 'percentage' (float).
 * - SetPosition: parameters 'sector' (string), 'position' (vector3 or
 *     string (name of mapnode in that case)), and
 *     'yrot' (y rotation default=0).
 * - SetVelocity: parameters 'velocity' (vector3) in body coordinates.
 * - AddVelocity: parameters 'velocity' (vector3) in world coordinates.
 * - SetAngularVelocity: parameters 'velocity' (vector3).
 *
 * This property class supports the following properties:
 * - speed (float, read/write): control speed (default 1.0).
 * - anchor (string, read/write): name of the entity on which we are
 *   anchored.
 * - gravity (float, read/write): gravity of the entity (default=19.6).
 * - hug (bool, read/write): hug mesh to ground (default=false).
 */
struct iPcLinearMovement : public virtual iBase
{
  SCF_INTERFACE (iPcLinearMovement, 0, 0, 3);

  /**
   * Set an anchor for this movement class. When this linmove is
   * anchored it basically means that all movement will be done relative
   * to the anchor. You can use this to do movement on an object that is
   * also moving on its own. If 'pcmesh' is 0 this will clear the anchor.
   */
  virtual void SetAnchor (iPcMesh* pcmesh) = 0;

  /**
   * Get the current anchor or 0 if movement is not anchored.
   */
  virtual iPcMesh* GetAnchor () const = 0;

  /**
   * Set the orientation of the mesh with three given euler angles.
   */
  virtual void SetAngularVelocity (const csVector3& angle) = 0;

  /**
   * Set the orientation of the mesh with three given euler angles
   * with a maximum to reached.
   */
  virtual void SetAngularVelocity (const csVector3& angle,
  	const csVector3& angle_to_reach) = 0;

  /**
   * Set the current speed.
   */
  virtual void SetSpeed (float speedZ) = 0;
  /// Get the current speed.
  virtual float GetSpeed () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use void SetBodyVelocity () instead.")
  /**
   * Set the current velocity vector in body coordinates.
   */
  virtual void SetVelocity (const csVector3& vel) = 0;

  /**
   * Set the current velocity vector in body coordinates.
   */
  virtual void SetBodyVelocity (const csVector3& vel) = 0;

  /**
   * Set the current world velocity vector in body coordinates.
   */
  virtual void SetWorldVelocity (const csVector3& vel) = 0;

  /**
   * Exerts a velocity on the body in world coordinates
   */
  virtual void AddVelocity (const csVector3& vel) = 0;

  /// Resets the velocity of this body in world coordinates.
  virtual void ClearWorldVelocity () = 0;

  /**
   * Get the current velocity vector.
   * \deprecated Use csVector3 GetVelocity () instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use csVector3 GetVelocity () instead.")
  virtual void GetVelocity (csVector3& v) const = 0;

  /**
   * Get the body's velocity- velocity minus physical effects like gravity.
   */
  virtual const csVector3 &GetBodyVelocity () const = 0;

  /**
   * Get the world's velocity- velocity for simulating physical effects like gravity.
   */
  virtual const csVector3 &GetWorldVelocity () const = 0;

  /**
   * Get the current velocity vector in local coordinate space:
   *   WorldToObject(worldvel) + bodyvel.
   */
  virtual const csVector3 GetVelocity () const = 0;

  /**
   * Get the current angular velocity vector.
   */
  virtual void GetAngularVelocity (csVector3& v) const = 0;

  /**
   * Initialize CD box for the object from a mesh.
   * \param mesh is the mesh from which to calculate the entire box.
   * \param percentage is a number between 0 and 100 indicating where
   *        the 'legs' collider stops and the 'body' collider starts. 0 means
   *        a very thin 'legs' collider and 100 a very thin 'body' collider.
   * \param pc_cd is a pointer to the collision detection property class.
   */
  virtual bool InitCD (iMeshWrapper* mesh, float percentage,
  	iPcCollisionDetection* pc_cd = 0) = 0;

  /**
   * Initialize CD box for the object.
   * The two parameters are the dimensions of the body and the legs
   * collider boxes.
   * The 'shift' vector is used to shift the box. By default (with shift
   * equal to the 0 vector) the colliders are created assuming the 0,0,0
   * origin is at the bottom center of the actor.
   */
  virtual bool InitCD (const csVector3& body, const csVector3& legs,
  	const csVector3& shift, iPcCollisionDetection *pc_cd = 0) = 0;

  /**
   * Get CD box for the object.
   */
  virtual void GetCDDimensions (csVector3& body, csVector3& legs,
  	csVector3& shift, iPcCollisionDetection*& pc_cd) = 0;

  /**
   * Initialize CD again?@@@
   */
  virtual bool InitCD (iPcCollisionDetection *pc_cd=0) = 0;

  /**
   * Returns the difference in time between now and when the last DR update
   * or extrapolation took place
   */
  virtual csTicks TimeDiff (void) = 0;

  /// Return all necessary data for Dead Reckoning
  virtual void GetDRData (bool& on_ground, float& speed, csVector3& pos,
  	float& yrot, iSector*& sector, csVector3& vel, csVector3& worldVel,
  	float& ang_vel) = 0;


  /// Sets all relevant dead reckoning data on this entity
  virtual void SetDRData (bool on_ground,float speed, csVector3& pos,
  	float yrot, iSector *sector, csVector3& vel, csVector3& worldVel,
  	float ang_vel) = 0;

  /**
   * Sets dead reckoning data in a 'soft' way.  Instead of immediately
   * setting the position, an error (offset) is measured between the
   * current position and the new position.  Over the period of 1 second,
   * the position will be offset until this position error is zero.
   */
  virtual void SetSoftDRData (bool on_ground,float speed, csVector3& pos,
  	float yrot,iSector *sector, csVector3& vel, csVector3& worldVel,
  	float ang_vel) = 0;

  /**
   * Set full position and sector. The position is absolute and will
   * be corrected to fit on the anchor if there is one.
   */
  virtual void SetFullPosition (const csVector3& pos, float yrot,
  	const iSector* sector) = 0;
  /**
   * Set position and sector. The position is relative to the
   * anchor (if there is one).
   */
  virtual void SetPosition (const csVector3& pos, float yrot,
  	const iSector* sector) = 0;
  /**
   * Get position and sector. If there is an anchor then this
   * position is relative to the anchor. Use SetLastFullPosition()
   * if you want an absolute position.
   */
  virtual void GetLastPosition (csVector3& pos, float& yrot,
  	iSector*& sector) = 0;
  /**
   * Get full position and sector. This is an absolute position which
   * is not relative to the anchor.
   */
  virtual void GetLastFullPosition (csVector3& pos, float& yrot,
  	iSector*& sector) = 0;

  /**
   * Get Y Rotation.
   */
  virtual float GetYRotation () = 0;
  /**
   * Get position and sector. If there is an anchor then this
   * position is relative to the anchor. Use GetFullPosition()
   * if you want an absolute position.
   */
  virtual const csVector3 GetPosition () = 0;
  /**
   * Get full position. This is an absolute position which
   * is not relative to the anchor.
   */
  virtual const csVector3 GetFullPosition () = 0;

  /**
   * Get the full transform describing the world to this transform.
   */
  virtual const csReversibleTransform GetFullTransform () = 0;

  /// Is a csPath active now or standard DR movement?
  virtual bool IsPath () const = 0;

  /**
   * Get sector.
   */
  virtual iSector* GetSector () = 0;

  /**
   * This function actually moves and rotates the mesh, relighting
   * if necessary.
   */
  virtual void ExtrapolatePosition (float delta) = 0;

  /**
   * This function calls ExtrapolatePosition with a certain time
   * but uses a fixed delta.  This allows all entities linmoves to be
   * synchronized to the same ticks, even if updates are all
   * happening at different times.
   */
  virtual void UpdateDRDelta (csTicks ticksdelta) = 0;

  /**
   * This function calls ExtrapolatePosition with a certain time
   * but calculates the delta relative to the last update, not
   * a fixed delta.  This allows all entities linmoves to be
   * synchronized to the same ticks, even if updates are all
   * happening at different times.
   */
  virtual void UpdateDR (csTicks ticks) = 0;

  /**
   * This function calls ExtrapolatePosition with by calculating
   * the delta from now relative to the last update, not
   * a fixed delta.  This allows all entities linmoves to be
   * synchronized to the same ticks, even if updates are all
   * happening at different times.
   */
  virtual void UpdateDR () = 0;

  /**
   * This function lets linmove store a ref to the supplied
   * iPath.  If this path is present, it will be used for
   * movement instead of linear velocity vector.
   */
  virtual void SetPath (iPath *newpath) = 0;

  /**
   * This function sets the current position on the path
   * for use when time deltas are added later.
   */
  virtual void SetPathTime (float timeval) = 0;

  /**
   * This relates the movement of the entity
   * along the path to the time values specified
   * by the path.  Speed=0 makes the entity
   * stationary.  Speed=1 is normal traversal
   * of the path.
   */
  virtual void SetPathSpeed (float speed) = 0;

  /**
   * This relates a particular action name
   * to be used between two points in the path.
   * This allows linmove to automatically
   * switch a bird from "fly" to "glide", for
   * example, during downward segments of the
   * flight path.
   */
  virtual void SetPathAction (int which, const char *action) = 0;

  /**
   * This sets the sector which will be used
   * for the entire path.
   */
  virtual void SetPathSector (const char *sectorname) = 0;

  /**
   * Check if mesh is on ground.
   */
  virtual bool IsOnGround () const = 0;

  /**
   * Set mesh onground flag (will assume it is on ground until it moves).
   */
  virtual void SetOnGround (bool onground) = 0;

  /**
   * Set if pcmesh should be transformed to follow the contour of the ground
   */
  virtual void SetHugGround (bool hugGround) = 0;

  /*
   * Set maximum time delta to use when extrapolating positions.
   */
  virtual void SetDeltaLimit (float deltaLimit) = 0;

  /// Get the delta limit.
  virtual float GetDeltaLimit () const = 0;

  /*
   * Rotates the mesh by a certain amount. This function is intended to be
   * used internally and should not be used.
   */ 
  virtual bool RotateV (float delta) = 0;

  /*
   * Set value of gravity affecting this object.
   */
  virtual void SetGravity (float grav) = 0;

  /*
   * Get current value of gravity affecting this object.
   */
  virtual float GetGravity () = 0;

  /*
   * Reset gravity to default (19.6f) for this object.
   */
  virtual void ResetGravity () = 0;

  /**
   * Shedules a callback when gravity has grasped an object
   * and is dragging it down. I.e, calls when the object changes
   * to negative Y velocity
   * \param callback The callback object
   */
  virtual void AddGravityCallback (iPcGravityCallback* callback) = 0;

  /**
   * Remove a gravity callback.
   */
  virtual void RemoveGravityCallback (iPcGravityCallback* callback) = 0;

  /// Get the total displacement caused by space warping portals.
  virtual csVector3 GetPortalDisplacement () = 0;

  /// Clear the total displacement caused by space warping portals.
  virtual void ClearPortalDisplacement () = 0;

  /**
   * Set full position and sector. The position is absolute and will
   * be corrected to fit on the anchor if there is one.
   * Use node name as center.
   */
  virtual void SetFullPosition (const char* center_name, float yrot,
  	iSector* sector) = 0;
  /**
   * Set position and sector. The position is relative to the
   * anchor (if there is one). Use node name as center.
   */
  virtual void SetPosition (const char* center_name, float yrot,
  	iSector* sector) = 0;
};

#endif

