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

#ifndef __CEL_PF_COLL_DET__
#define __CEL_PF_COLL_DET__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/strhash.h"


SCF_VERSION (iPcCollisionDetection, 0, 0, 1);

/**
 * This property class controls collision detection of an entity with
 * the world map and other meshes. It should be used in combination
 * with iPcLinearMovement but otherwise doesn't depend on any other
 * property classes. So in that sense it is unrelated to the other 
 * movement property classes.
 */
struct iPcCollisionDetection : public iBase
{
  /**
   * Initialize CD box for the object.
   * The two parameters are the dimensions of the body and the legs
   * collider boxes.
   * The 'shift' vector is used to shift the box. By default (with shift
   * equal to the 0 vector) the colliders are created assuming the 0,0,0
   * origin is at the bottom center of the actor.
   */
  virtual bool Init (const csVector3& body, const csVector3& legs,
  	const csVector3& shift)=0;

  virtual bool Init ()=0;

  /**
   * Check if mesh is on ground.
   */
  virtual bool IsOnGround () const = 0;

  /// Set on Ground flag
  virtual void SetOnGround (bool flag) = 0;

  virtual void UseCD(bool flag) = 0;

  /**
   * This function takes a position vector, checks against all known
   * colliders, and returns the adjusted position in the same variable.
   */
  virtual bool AdjustForCollisions (csVector3& oldpos,
                                    csVector3& newpos,
                                    csVector3& vel,
                                    float delta,
                                    iMovable* movable) = 0;
  virtual bool QueryRevert() = 0;
};

#endif

