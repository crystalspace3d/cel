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

SCF_VERSION (iPcActorMove, 0, 0, 1);

/**
 * ActorMove property class. This property class makes a connection
 * between pclinmove and pccamera and makes it easier to control
 * movement for actors in a game. Typical usage would be to combine
 * it with pckeyinput and then in the behaviour layer send out
 * the appropriate calls here depending on the keys that are pressed.
 * This class is also aware of animation actions using sprcal3d or
 * spr3d.
 */
struct iPcActorMove : public iBase
{
  /// Start/stop going forward.
  virtual void Forward (bool start) = 0;
  /// Start/stop going backward.
  virtual void Backward (bool start) = 0;
  /// Start/stop strafing left.
  virtual void StrafeLeft (bool start) = 0;
  /// Start/stop strafing right.
  virtual void StrafeRight (bool start) = 0;
  /// Start/stop rotating left.
  virtual void RotateLeft (bool start) = 0;
  /// Start/stop rotating right.
  virtual void RotateRight (bool start) = 0;
  /// Start/stop running.
  virtual void Run (bool start) = 0;
  /// Start/stop auto-running.
  virtual void AutoRun (bool start) = 0;
  /// Jump.
  virtual void Jump () = 0;
  /// Toggle camera mode.
  virtual void ToggleCameraMode () = 0;

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
};

#endif // __CEL_PF_ACTORMOVE__

