/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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

#ifndef __CEL_PF_ACTORMOVE__
#define __CEL_PF_ACTORMOVE__

#include "cstypes.h"
#include "csutil/scf.h"

/**
 * ActorMove property class. This property class makes a connection
 * between pclinmove and pccamera and makes it easier to control
 * movement for actors in a game. Typical usage would be to combine
 * it with pccommandinput and then in the behaviour layer send out
 * the appropriate calls here depending on the keys that are pressed.
 * This class is also aware of animation actions using sprcal3d or
 * spr3d.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - SetSpeed: parameters 'movement' (float), 'running' (float),
 *   'rotation' (float) and 'jumping' (float).
 * - Forward: parameters 'start' (bool).
 * - Backward: parameters 'start' (bool).
 * - StrafeLeft: parameters 'start' (bool).
 * - StrafeRight: parameters 'start' (bool).
 * - RotateLeft: parameters 'start' (bool).
 * - RotateRight: parameters 'start' (bool).
 * - RotateTo: parameters 'yrot' (float).
 * - MouseMove: parameters 'x' (float) and 'y' (float).
 * - Run: parameters 'start' (bool).
 * - AutoRun: parameters 'start' (bool).
 * - Jump.
 * - ToggleCameraMode.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - mousemove (bool, read/write): enable/disable mousemove.
 * - mousemove_inverted (bool, read/write): enable/disable inverted mousemove.
 * - mousemove_xfactor (float, read/write): horizontal mousemove speed.
 * - mousemove_yfactor (float, read/write): vertical mousemove speed.
 */
struct iPcActorMove : public virtual iBase
{
  SCF_INTERFACE (iPcActorMove, 0, 0, 1);

  /// Start/stop going forward.
  virtual void Forward (bool start) = 0;
  /// Returns the toogle value of moving forward.
  virtual bool IsMovingForward () = 0;
  /// Start/stop going backward.
  virtual void Backward (bool start) = 0;
  /// Returns the toogle value of moving backward.
  virtual bool IsMovingBackward () = 0;
  /// Start/stop strafing left.
  virtual void StrafeLeft (bool start) = 0;
  /// Returns the toogle value of strafing left.
  virtual bool IsStrafingLeft () = 0;
  /// Start/stop strafing right.
  virtual void StrafeRight (bool start) = 0;
  /// Returns the toogle value of strafing right.
  virtual bool IsStrafingRight () = 0;
  /// Start/stop rotating left.
  virtual void RotateLeft (bool start) = 0;
  /// Returns the toogle value of rotating left.
  virtual bool IsRotatingLeft () = 0;
  /// Start/stop rotating right.
  virtual void RotateRight (bool start) = 0;
  /// Returns the toogle value of rotating right.
  virtual bool IsRotatingRight () = 0;
  /// Rotate to a specified angle.
  virtual void RotateTo (float yrot) = 0;
  /// Start/stop running.
  virtual void Run (bool start) = 0;
  /// Returns the toogle value of running.
  virtual bool IsRunning () = 0;
  /// Start/stop auto-running.
  virtual void AutoRun (bool start) = 0;
  /// Returns the toogle value of autorunning.
  virtual bool IsAutoRunning () = 0;
  /**
   * Handle mouse movement. Use this in combination with EnableMouseMove().
   * Note that you should set screenspace property to false in
   * pccommandinput to get the right coordinates (i.e. coordinates centered
   * around 0,0 being the center of the screen).
   */
  virtual void MouseMove (float x, float y) = 0;
  /// Jump.
  virtual void Jump () = 0;
  /// Toggle camera mode.
  virtual void ToggleCameraMode () = 0;

  /**
   * Enable mouse move. To use this the behaviour must
   * call 'MouseMove()' whenever the mouse moves.
   */
  virtual void EnableMouseMove (bool en) = 0;
  /// Test if mouse move is enabled.
  virtual bool IsMoveMoveEnabled () const = 0;

  /**
   * Enable mouse move inverted mode. In this mode the
   * up-down mousemove will be inverted. By default this is disabled.
   */
  virtual void SetMouseMoveInverted (bool en) = 0;
  /// Test if mouse move is inverted.
  virtual bool IsMouseMoveInverted () const = 0;

  /**
   * Set mouse move speed factor. Default is 1,1.
   */
  virtual void SetMouseMoveSpeed (float xs, float ys) = 0;
  /// Get mouse move x factor.
  virtual float GetMouseMoveSpeedX () const = 0;
  /// Get mouse move y factor.
  virtual float GetMouseMoveSpeedY () const = 0;

  /// Set the movement speed (default 2.0).
  virtual void SetMovementSpeed (float speed) = 0;
  /// Get the movement speed (default 2.0).
  virtual float GetMovementSpeed () const = 0;

  /// Set the running speed (default 5.0).
  virtual void SetRunningSpeed (float speed) = 0;
  /// Get the running speed (default 5.0).
  virtual float GetRunningSpeed () const = 0;

  /// Set the rotation speed (default 1.75).
  virtual void SetRotationSpeed (float speed) = 0;
  /// Get the rotation speed (default 1.75).
  virtual float GetRotationSpeed () const = 0;

  /// Set the jumping velocity (default 6.31).
  virtual void SetJumpingVelocity (float speed) = 0;
  /// Get the jumping velocity (default 6.31).
  virtual float GetJumpingVelocity () const = 0;
  /// Set current animation on underlying pcmesh.
  virtual void SetAnimation (const char *name, bool cycle = true) = 0;
};

#endif // __CEL_PF_ACTORMOVE__

