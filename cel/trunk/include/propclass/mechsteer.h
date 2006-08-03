/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_MECHANICS_STEER__
#define __CEL_PF_MECHANICS_STEER__

#include "cstypes.h"
#include "csutil/scf.h"

#include "propclass/mechcommon.h"

class csVector3;
struct iPcMechanicsObject;

SCF_VERSION (iPcMechanicsSteering, 0, 0, 1);

/**
 * Property class representing a steering mechanism that affects forces on the
 * attached object, dependent on the forward velocity of the object.
 */
struct iPcMechanicsSteering : public iBase
{
  /**
   * Set this thruster's identifier.
   */
  virtual void SetID (const char* id) = 0;

  /**
   * Get this thruster's identifier.
   */
  virtual const char* GetID () = 0;

  /**
   * Set the mechanics object to affect. This is required.
   */
  virtual void SetMechanicsObject (iPcMechanicsObject* mechsys) = 0;

  /**
   * Get the associated mechanics object.
   */
  virtual iPcMechanicsObject* GetMechanicsObject () = 0;


  ////
  //MechanicsSteering properties

  /**
   * Set the position of this steering mechanism.
   */
  virtual void SetPosition (const csVector3 position) = 0;

  /**
   * Get the position of this steering mechanism.
   */
  virtual const csVector3 GetPosition () = 0;


  /**
   * Set the axis of rotation for this steering mechanism.
   */
  virtual void SetAxis (const csVector3 axis) = 0;

  /**
   * Get the axis of rotation for this steering mechanism.
   */
  virtual const csVector3 GetAxis () = 0;


  ////
  //Angle

  /**
   * Set the angle of the steering mechanism.
   */
  virtual void SetAngle (percentage angle) = 0;

  /**
   * Get the angle of the steering mechanism.
   */
  virtual percentage GetAngle () = 0;

  /**
   * Set the maximum angle of the steering mechanism.
   */
  virtual void SetMaxAngle (float maxangle) = 0;

  /**
   * Get the maximum angle of the steering mechanism.
   */
  virtual float GetMaxAngle () = 0;


  ////
  //Turning

  /**
   * Turn the steering mechanism. (speed from -1.0 to 1.0, negative means
   * clockwise.)
   */
  virtual void Turn (percentage speed = 0.0) = 0;

  /**
   * Turn the steering mechanism to the given angle percentage.
   */
  virtual void TurnTo (percentage angle, percentage speed = 1.0) = 0;

  /**
   * Set the maximum turning speed of this steering mechanism.
   */
  virtual void SetMaxTurnSpeed (float maxspeed) = 0;

  /**
   * Get the maximum turning speed this steering mechanism.
   */
  virtual float GetMaxTurnSpeed () = 0;
};

#endif //__CEL_PF_MECHANICS_STEER__
