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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_ANALOGMOTION__
#define __CEL_PF_ANALOGMOTION__

#include "cstypes.h"
#include "csutil/scf.h"

/**
 * This is a property class for analog character movement using a joystick
 * or analog pad.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - SetAxis: parameters 'axis' (long), 'value' (float).
 * - AddAxis: parameters 'axis' (long), 'value' (float).
 * - SetMovementSpeed: parameters 'value' (float).
 * - SetTurningSpeed: parameters 'value' (float).
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - axisx (float, read/write): Left/Right axis value (-1.0 to 1.0).
 * - axisy (float, read/write): Forward/Backwards axis value (-1.0 to 1.0).
 * - movespeed (float, read/write): Movement speed.
 * - turnspeed (float, read/write): Turning speed.
 */
struct iPcAnalogMotion : public virtual iBase
{
  SCF_INTERFACE (iPcAnalogMotion, 0, 0, 1);

  /**
   * Set axis to a value.
   * \param axis This is the axis. 0 for left/right axis,
   * 1 for forwards/backwards axis, 2 or above for both together.
   * \param value The value in the range [-1,1] to set it to. Anything
   * else is invalid and is clipped.
   */
  virtual void SetAxis (size_t axis, float value) = 0;

  /**
   * Get an axis' value.
   * \param axis This is the axis. 0 for left/right axis,
   * 1 for forwards/backwards axis.
   * \param value The value in the range [-1,1].
   */
  virtual float GetAxis (size_t axis) const = 0;

  /**
   * Set both the axis values using a vector.
   */
  virtual void SetAxis (const csVector2 &axis) = 0;

  /**
   * Get both the axis values as a vector.
   */
  virtual const csVector2 &GetAxis () const = 0;

  /**
   * Increase axis by a value. This is for keyboard keys to avoid
   * 'deadlock'
   * \param axis This is the axis. 0 for left/right axis,
   * 1 for forwards/backwards axis, 2 or above for both together.
   * \param value The value to add to it. Anything resulting in the
   * target axis value being out of the range [-1,1] will be clipped.
   */
  virtual void AddAxis (size_t axis, float value) = 0;

  /**
   * Set the movement speed of the character.
   * \param movespeed How fast the character moves in the direction its facing.
   */
  virtual void SetMovementSpeed (float movespeed) = 0;

  /**
   * Get the movement speed of the character.
   * \return How fast the character moves in the direction its facing.
   */
  virtual float GetMovementSpeed () const = 0;

  /**
   * Set the movement acceleration of the character for when you start moving.
   * \param moveaccel Acceleration of character.
   */
  virtual void SetMovementAcceleration (float moveaccel) = 0;

  /**
   * Get the movement acceleration of the character for when you start moving.
   * \return Acceleration of character.
   */
  virtual float GetMovementAcceleration () const = 0;

  /**
   * Set the movement deceleration of the character for when you stop moving.
   * \param movedecel Deceleration of character.
   */
  virtual void SetMovementDeceleration (float movedecel) = 0;

  /**
   * Get the movement deceleration of the character for when you stop moving.
   * \return Deceleration of character.
   */
  virtual float GetMovementDeceleration () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use Set(Minimum|Maximum)TurningSpeed () instead")
  /**
   * Set the turning speed of the character.
   * \param turnspeed How fast the player turns to face its target.
   */
  virtual void SetTurningSpeed (float turnspeed) = 0;

  CS_DEPRECATED_METHOD_MSG("Use Get(Minimum|Maximum)TurningSpeed () instead")
  /**
   * Get the turning speed of the character.
   * \return How fast the player turns to face its target.
   */
  virtual float GetTurningSpeed () const = 0;

  /**
   * Set the minimum turning speed of the character. This is the speed of turning
   * when moving at full speed.
   * Turning is decelerating in one direction and accelerating in another. Faster
   * movement means a slower turning speed, and vice versa.
   * \param turnspeed How fast the player turns to face its target.
   */
  virtual void SetMinimumTurningSpeed (float turnspeed) = 0;

  /**
   * Get the minimum turning speed of the character. This is the speed of turning
   * when moving at full speed.
   * Turning is decelerating in one direction and accelerating in another. Faster
   * movement means a slower turning speed, and vice versa.
   * \return How fast the player turns to face its target.
   */
  virtual float GetMinimumTurningSpeed () const = 0;

  /**
   * Set the maximum turning speed of the character. This is the speed of turning
   * when not moving at all.
   * Turning is decelerating in one direction and accelerating in another. Slower
   * movement means a faster turning speed, and vice versa.
   * \param turnspeed How fast the player turns to face its target.
   */
  virtual void SetMaximumTurningSpeed (float turnspeed) = 0;

  /**
   * Get the maximum turning speed of the character. This is the speed of turning
   * when not moving at all.
   * Turning is decelerating in one direction and accelerating in another. Slower
   * movement means a faster turning speed, and vice versa.
   * \return How fast the player turns to face its target.
   */
  virtual float GetMaximumTurningSpeed () const = 0;

  /**
   * Whether this component is updating the player's position every frame. Turning
   * this off effectively makes this component deactivated.
   */
  virtual void Activate (bool ac = true) = 0;

  /**
   * Get whether this component is actively updating the player.
   */
  virtual bool IsActive () const = 0;
};

#endif // __CEL_PF_ANALOGMOTION__
