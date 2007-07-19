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

#ifndef __CEL_PF_ACTORLARA__
#define __CEL_PF_ACTORLARA__

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
struct iPcActorAnalog : public virtual iBase
{
  SCF_INTERFACE (iPcActorAnalog, 0, 0, 1);

  /**
   * Set axis to a value.
   * \param axis This is the axis. 0 for left/right axis,
   * 1 for forwards/backwards axis, 2 or above for both together.
   * \param value The value in the range [-1,1] to set it to. Anything
   * else is invalid.
   */
  virtual void SetAxis (size_t axis, float value) = 0;

  /**
   * Set the movement speed of the character.
   * \param movespeed How fast moves in the direction its facing.
   */
  virtual void SetMovementSpeed (float movespeed) = 0;

  /**
   * Set the turning speed of the character.
   * \param turnspeed How fast turns to face its target.
   */
  virtual void SetTurningSpeed (float turnspeed) = 0;
};

#endif // __CEL_PF_ACTORLARA__
