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

#ifndef __CEL_PF_JUMP__
#define __CEL_PF_JUMP__

#include "cstypes.h"
#include "csutil/scf.h"

struct iPcJump : public virtual iBase
{
  SCF_INTERFACE (iPcJump, 0, 0, 1);

  /**
   * Do a jump. When called again while still jumping,
   * if near the top of a jump a double jump is performed.
   * By default the double jump is disabled- to enable call
   * SetDoubleJumpSpeed () with a positive non zero value.
   */
  virtual void Jump () = 0;

  /**
   * If the boost is being used, then this will finish it...
   * for when the player releases the jump button.
   */
  virtual void FinishBoost () = 0;

  /**
   * Freeze the character mid-air for grabbing ledges.
   */
  virtual void Freeze (bool frozen) = 0;

  enum State
  {
    STAND = 0,
    JUMP,
    DOUBLEJUMP,
    GLIDE,
    FROZEN
  };

  /**
   * Get the current state.
   */
  virtual State GetState () const = 0;

  enum GlideTurnDirection
  {
    GLIDE_NOTURN = 0,
    GLIDE_LEFT,
    GLIDE_RIGHT
  };

  /**
   * If gliding then turn in a certain direction.
   */
  virtual void GlideTurn (GlideTurnDirection gtur) = 0;

  enum GlidePitchDirection
  {
    GLIDE_NOPITCH = 0,
    GLIDE_UP,
    GLIDE_DOWN
  };

  /**
   * If gliding then pitch the player up/down
   */
  virtual void GlidePitch (GlidePitchDirection gpit) = 0;

  /**
    * Set the gravity during gliding
    */
  virtual void SetGlideGravity (float glidegrav) = 0;

  /**
    * Get the gravity during gliding
    */
  virtual float GetGlideGravity () const = 0;

  /**
    * Set the pitch limits during gliding in radians
    */
  virtual void SetGlidePitchLimit (float gptlim) = 0;

  /**
    * Get the pitch limits during gliding in radians
    */
  virtual float GetGlidePitchLimit () const = 0;

  /**
    * Set the pitch speed in radians while gliding
    */
  virtual void SetGlidePitchSpeed (float gptspd) = 0;

  /**
    * Get the pitch speed in radians while gliding
    */
  virtual float GetGlidePitchSpeed () const = 0;

  /**
    * Set the turn speed in radians while gliding
    */
  virtual void SetGlideTurnSpeed (float gtrspd) = 0;

  /**
    * Get the turn speed in radians while gliding
    */
  virtual float GetGlideTurnSpeed () const = 0;

  /**
   * Set the jumping speed.
   */
  virtual void SetJumpSpeed (float spd) = 0;

  /**
   * Get the jumping speed.
   */
  virtual float GetJumpSpeed () const = 0;

  /**
   * Set the maximum height of the jump. Will change jumpspeed.
   */
  virtual void SetJumpHeight (float height) = 0;

  /**
   * Get the maximum height of the jump. Actually calculated from
   * the jumpspeed.
   */
  virtual float GetJumpHeight () const = 0;

  /**
   * Calculate the time it takes from jump to land on same plane.
   */
  virtual csTicks GetAirTime () const = 0;

  /**
   * Set the speed of the double jump- normally is lower than normal jump.
   */
  virtual void SetDoubleJumpSpeed (float spd) = 0;

  /**
   * Get the double jump.
   */
  virtual float GetDoubleJumpSpeed () const = 0;

  /**
   * Set the gravity acting on the player. This gravity value is only applied
   * when hitting the wall, landing from a jump and going from FROZEN->JUMP
   * Otherwise changes to linmove->SetGravity () are left untouched.
   */
  virtual void SetGravity (float grav) = 0;

  /**
   * Get the gravity acting on the player.
   */
  virtual float GetGravity () const = 0;

  /**
   * Should the jumps be fixed length?
   */
  virtual void SetFixedJump (bool fixjump) = 0;

  /**
   * Are the jumps be fixed length?
   */
  virtual bool GetFixedJump () const = 0;

  /**
   * Boost the jump. So longer you hold jump, the higher you go.
   */
  virtual void SetBoostJump (bool boost) = 0;

  /**
   * Boost the jump. So longer you hold jump, the higher you go.
   */
  virtual bool GetBoostJump () const = 0;

  virtual void SetBoostTime (float t) = 0;
  virtual float GetBoostTime () const = 0;
  virtual void SetBoostAcceleration (float a) = 0;
  virtual float GetBoostAcceleration () const = 0;
};

#endif // __CEL_PF_JUMP__

