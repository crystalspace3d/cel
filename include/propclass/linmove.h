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


SCF_VERSION (iPcLinearMovement, 0, 0, 1);

struct iDataBuffer;
struct iSector;
struct iPath;
struct iPcCollisionDetection;

/**
 * This property class controls movement of an entity in a realistic
 * manner. It should be used in combination with iPcMesh but otherwise
 * doesn't depend on any other property classes. So in that sense
 * it is unrelated to the other movement property classes.
 * <p>
 * This property class supports dead reckoning which is useful for
 * networking.
 */
struct iPcLinearMovement : public iBase
{
  /**
   * Set the orientation of the mesh with three given euler angles.
   */
  virtual void SetRotation (const csVector3& angle) = 0;

  /**
   * Set the current speed.
   */
  virtual void SetSpeed (float speedZ) = 0;

  /**
   * Set the speed of camera movement.
   */
  virtual void SetCameraPitchSpeed (float angle) = 0;

  /**
   * Set the current velocity vector.
   */
  virtual void SetVelocity (const csVector3& vel) = 0;
  /**
   * Get the current velocity vector.
   */
  virtual void GetVelocity (csVector3& v) = 0;

  /**
   * Initialize CD box for the object.
   * The two parameters are the dimensions of the body and the legs
   * collider boxes.
   * The 'shift' vector is used to shift the box. By default (with shift
   * equal to the 0 vector) the colliders are created assuming the 0,0,0
   * origin is at the bottom center of the actor.
   */
  virtual bool InitCD (const csVector3& body, const csVector3& legs,
  	const csVector3& shift,iPcCollisionDetection *pc_cd=NULL)=0;

  /**
   * Initialize CD again?@@@
   */
  virtual bool InitCD (iPcCollisionDetection *pc_cd=NULL) = 0;

  /**
   * Returns data useful for dead reckoning.
   */
  virtual csPtr<iDataBuffer> GetDRData(csStringHash* msgstrings = 0) = 0;

  /**
   * Applies dead reckoning data to this object.
   */
  virtual bool SetDRData (iDataBuffer* data, bool detectcheat, csStringHash* msgstrings = 0) = 0;

  /**
   * Checks whether a DR packet needs to be sent.
   */
  virtual bool NeedDRData (uint8& priority) = 0;

  /**
   * Set position and sector.
   */
  virtual void SetPosition (const csVector3& pos, float yrot,
  	const iSector* sector) = 0;
  /**
   * Get position and sector.
   */
  virtual void GetLastPosition (csVector3& pos, float& yrot,
  	iSector*& sector) = 0;

  /**
   * Get sector.
   */
  virtual iSector* GetSector () = 0;

  /**
   * Set ready? @@@
   */
  virtual void SetReady (bool flag) = 0;    
  /**
   * Is ready? @@@
   */
  virtual bool IsReady() const = 0;    

  /**
   * This function actually moves and rotates the mesh, relighting
   * if necessary.
   */
  virtual void ExtrapolatePosition (float delta) = 0;

  /**
   * This function calls ExtrapolatePosition with a certain time
   * but calculates the delta relative to the last update, not
   * a fixed delta.  This allows all entities linmoves to be
   * synchronized to the same ticks, even if updates are all
   * happening at different times.
   */
  virtual void UpdateDR (csTicks ticks) = 0;

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

  virtual bool IsOnGround () const = 0;
};

#endif

