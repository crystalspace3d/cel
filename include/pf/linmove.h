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


SCF_VERSION (iPcLinearMovement, 0, 0, 1);

struct iDataBuffer;
struct iSector;

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
   */
  virtual bool InitCD (csVector3& top, csVector3& bottom)=0;

  /**
   * Initialize CD again?@@@
   */
  virtual bool InitCD () = 0;

  /**
   * Returns data useful for dead reckoning.
   */
  virtual csPtr<iDataBuffer> GetDRData() = 0;

  /**
   * Applies dead reckoning data to this object.
   */
  virtual bool SetDRData (iDataBuffer* data, bool detectcheat) = 0;

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
   * Check if player is on ground.
   */
  virtual bool IsOnGround () const = 0;

  /**
   * This function actually moves and rotates the mesh, relighting
   * if necessary.
   */
  virtual void ExtrapolatePosition (float delta) = 0;
};

#endif

