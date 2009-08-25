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
 * This is a property class used to control movement of an hovercraft.
 * Allows changing yaw and pitch, as well as thruster and afterburner,
 * and supresses angular velocity of object.
 *
 * This property class supports the following actions:
 * - SetSliding: parameters 'enabled' (bool: optional). If 'enabled' is not
 *   given then default is true.
 * - SetBraking: parameters 'enabled' (bool: optional). If 'enabled' is not
 *   given then default is true.
 * - SetThruster: parameters 'enabled' (bool: optional). If 'enabled' is not
 *   given then default is true.
 * - SetAfterBurner: parameters 'enabled' (bool: optional). If 'enabled' is not
 *   given then default is true.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - turnmax (float, read/write): maximum turning.
 * - turnacc (float, read/write): turning rate.
 * - pitchmax (float, read/write): maximum pitch.
 * - pitchacc (float, read/write): pitch rate.
 * - roll (float, read/write): rolling ratio.
 * - thrust (float, read/write): thruster force.
 * - topspeed (float, read/write): thruster top speed.
 * - atopspeed (float, read/write): afterburner top speed.
 * - brakingspeed (float, read/write): braking speed.
 * - decelrate (float, read/write): deceleration rate.
 * - rvelratio (float, read/write): redirected velocity ratio.
 */
struct iPcCraftController : public virtual iBase
{
  SCF_INTERFACE (iPcCraftController, 0, 0, 1);

  /**
   * Start the object turning left.
   */
  virtual void StartTurnLeft() = 0;
  /**
   * Stop the object turning left.
   */
  virtual void StopTurnLeft() = 0;
  /**
   * Start the object turning right.
   */
  virtual void StartTurnRight() = 0;
  /**
   * Stop the object turning right.
   */
  virtual void StopTurnRight() = 0;

  /**
   * Start the object turning up.
   */
  virtual void StartTurnUp() = 0;
  /**
   * Stop the object turning up.
   */
  virtual void StopTurnUp() = 0;
  /**
   * Start the object turning down.
   */
  virtual void StartTurnDown() = 0;
  /**
   * Stop the object turning down.
   */
  virtual void StopTurnDown() = 0;

  /**
   * Set the objects left and right turning acceleration.
   */
  virtual void SetAccTurn(float tacc) = 0;
  /**
   * Set the objects up and down turning acceleration.
   */
  virtual void SetAccPitch(float uacc) = 0;

  /**
   * Set the objects maximum left and right turning velocity.
   */
  virtual void SetMaxTurn(float turn) = 0;
  /**
   * Set the objects maximum up and down turning velocity.
   */
  virtual void SetMaxPitch(float mud) = 0;
  /**
   * Set the roll factor.
   * Roll is how much a craft rolls when turning left and right.
   */
  virtual void SetRoll (float roll) = 0;
  /**
   * Set the thrust force of the craft.
   */
  virtual void SetThrustForce(float tf) = 0;
  /**
   * Set the Top Speed of the thruster.
   * Above this speed the thruster will be disabled.
   */
  virtual void SetTopSpeed(float tspeed) = 0;
  /**
   * @@@FIXME@@@ Document me !
   */
  virtual void SetRedirectVelocityRatio (float rdvr) = 0;
  /**
   * Set the deceleration rate.
   * It is used to slow down the craft when thruster is off.
   */
  virtual void SetDecelerationRate (float decr) = 0;
  /**
   * Set the braking force.
   * It is used to slow down the craft when brakes are on.
   */
  virtual void SetBrakingSpeed (float bspd) = 0;

  /**
   * Turn on thruster.
   */
  virtual void ThrustOn() = 0;
  /**
   * Turn off thruster.
   */
  virtual void ThrustOff() = 0;
  /**
   * Report whether thruster is on (true) or turned off (false).
   */
  virtual bool IsThrusterOn () = 0;
  /**
   * Set the top speed when afterburner is on.
   * Above this speed the afterburner will be disabled.
   */ 
  virtual void SetAfterBurnerTopSpeed (float tspeed) = 0;
  /**
   * Turn on afterburner.
   */
  virtual void AfterBurnerOn () = 0;
  /**
   * Turn off afterburner.
   */
  virtual void AfterBurnerOff () = 0;
  /**
   * Turn on brakes.
   */
  virtual void BrakesOn () = 0;
  /**
   * Turn off brakes.
   */
  virtual void BrakesOff () = 0;
  /**
   * Turn on sliding.
   * When sliding, the craft velocity is independent of its orientation.
   */
  virtual void SlideOn () = 0;
  /**
   * Turn off sliding.
   */
  virtual void SlideOff () = 0;
};

#endif
