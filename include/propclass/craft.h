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

#ifndef __CEL_PF_CRAFTCONTROLLER__
#define __CEL_PF_CRAFTCONTROLLER__

#include "cstypes.h"
#include "csutil/ref.h"
#include "csutil/scf.h"

/**
 * Craft controller property class. "pccraft"
 * Allows a Craft with turning left and right,
 * and supresses angular velocity of object.
 * Thruster and functions to control supression
 * to be added.
 */
struct iPcCraftController : public virtual iBase
{
  SCF_INTERFACE (iPcCraftController, 0, 0, 1);

  /**
   * Start the object turning left
   */
  virtual void StartTurnLeft() = 0;
  /**
   * Stop the object turning left
   */
  virtual void StopTurnLeft() = 0;
  /**
   * Start the object turning right
   */
  virtual void StartTurnRight() = 0;
  /**
   * Stop the object turning right
   */
  virtual void StopTurnRight() = 0;

  /**
   * Start the object turning up
   */
  virtual void StartTurnUp() = 0;
  /**
   * Stop the object turning up
   */
  virtual void StopTurnUp() = 0;
  /**
   * Start the object turning down
   */
  virtual void StartTurnDown() = 0;
  /**
   * Stop the object turning down
   */
  virtual void StopTurnDown() = 0;

  /**
   * Set the objects left and right
   * turning acceleration;
   */
  virtual void SetAccTurn(float tacc) = 0;
  /**
   * Set the objects up and down
   * turning acceleration;
   */
  virtual void SetAccUpDown(float uacc) = 0;

  /**
   * Set the objects maximum
   * left and right turning velocity;
   */
  virtual void SetMaxTurn(float turn) = 0;
  /**
   * Set the objects maximum
   * up and down turning velocity;
   */
  virtual void SetMaxUpDown(float mud) = 0;
  /**
   * Set the thrust force of the craft
   */
  virtual void SetThrustForce(float tf) = 0;
  /**
   * Set the Top Speed of the thruster.
   * Above this speed the thruster will not work.
   */
  virtual void SetTopSpeed(float tspeed) = 0;

  /**
   * Turn on thruster
   */
  virtual void ThrustOn() = 0;
  /**
   * Turn off thruster
   */
  virtual void ThrustOff() = 0;
};

#endif
