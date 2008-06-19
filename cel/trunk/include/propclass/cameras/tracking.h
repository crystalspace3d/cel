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

#ifndef __CEL_PF_TRACKINGCAMERA__
#define __CEL_PF_TRACKINGCAMERA__

#include "propclass/delegcam.h"

struct iPcTrackingCamera : public virtual iPcCameraMode
{
  SCF_INTERFACE (iPcTrackingCamera, 0, 0, 1);

  virtual bool DecideState () = 0;
  // position, target, up
  virtual const csVector3 &GetPosition () = 0;
  virtual const csVector3 &GetTarget () = 0;
  virtual const csVector3 &GetUp () = 0;

  /**
   * Resets this camera to its original state facing the same direction
   * as the anchor fixed on the target,
   */
  virtual bool ResetCamera () = 0;

  enum TargetState
  {
    TARGET_BASE = 0,
    TARGET_OBJ,
    TARGET_NONE
  };

  /**
   * Sets the target entity that we focus on when targetstate == TARGET_OBJ
   * \param The name of the entity.
   */
  virtual bool SetTargetEntity (const char* name) = 0;

  /**
   * Sets the state of the targetting.
   * - TARGET_BASE: focus on the entity we're following.
   * - TARGET_OBJ: focus on the entity set by SetTargetEntity (...)
   * - TARGET_NONE: focus on nothing and also follow the on the x axis
   */
  virtual void SetTargetState (TargetState targetstate) = 0;

  /**
   * \return The target state
   */
  virtual TargetState GetTargetState () = 0;

  /**
   * Since position is often set at the 'feet' of an object, set
   * a fixed offset upwards
   */
  virtual void SetTargetYOffset (float targetyoffset) = 0;

  /**
   * Since position is often set at the 'feet' of an object, get
   * the fixed offset upwards
   */
  virtual float GetTargetYOffset () const = 0;

  /**
   * Camera offset from the player is described as an angle and a distance.
   * Here you can set the angle in radians.
   */
  virtual void SetOffsetAngle (float angle) = 0;

  /**
   * Camera offset from the player is described as an angle and a distance.
   * Here you can get the angle in radians.
   */
  virtual float GetOffsetAngle () const = 0;

  /**
   * Camera offset from the player is described as an angle and a distance.
   * Here you can set the distance from the player.
   */
  virtual void SetOffsetDistance (float dist) = 0;

  /**
   * Camera offset from the player is described as an angle and a distance.
   * Here you can get the distance from the player.
   */
  virtual float GetOffsetDistance () const = 0;

  /**
   * Set the length of the spring that follows the player in and out of the camera.
   */
  virtual void SetFollowSpringLength (float slen) = 0;

  /**
   * Get the length of the spring that follows the player in and out of the camera.
   */
  virtual float GetFollowSpringLength () const = 0;

  /**
   * The follow spring can have stupid bottom values that greatly slow the speed to 0.
   * You can set a minmum cutoff to stop this.
   */
  virtual void SetFollowMinimumSpringFactor (float smin) = 0;

  /**
   * Get the minimum cutoff for the follow spring.
   */
  virtual float SetFollowMinimumSpringFactor () const = 0;

  enum PanDirection
  {
    PAN_LEFT = 0,
    PAN_NONE,
    PAN_RIGHT
  };

  /**
   * Pan the camera around the player in the direction specified.
   */
  virtual void Pan (PanDirection pandir) = 0;

  /**
   * Get the direction that the camera is panning in.
   */
  virtual PanDirection GetPanDirection () const = 0;

  /**
   * Set the panning speed. Angle to move per frame.
   */
  virtual void SetPanSpeed (float panspeed) = 0;

  /**
   * Get the panning speed. Angle to move per frame.
   */
  virtual float GetPanSpeed () const = 0;

  /**
   * Set the acceleration to approach the panning speed.
   */
  virtual void SetPanAcceleration (float paccel) = 0;

  /**
   * Get the acceleration to approach the panning speed.
   */
  virtual float GetPanAcceleration () const = 0;

  enum TiltDirection
  {
    TILT_UP = 0,
    TILT_NONE,
    TILT_DOWN
  };

  /**
   * Tilt camera in the direction specified.
   */
  virtual void Tilt (TiltDirection tiltdir) = 0;

  /**
   * Get the direction that the camera is tilting in.
   */
  virtual TiltDirection GetTiltDirection () const = 0;

  /**
   * Set the tilting speed. Angle to move per frame.
   */
  virtual void SetTiltSpeed (float tiltspeed) = 0;

  /**
   * Get the tilting speed. Angle to move per frame.
   */
  virtual float GetTiltSpeed () const = 0;

  /**
   * Set the acceleration to approach the tilting speed.
   */
  virtual void SetTiltAcceleration (float paccel) = 0;

  /**
   * Get the acceleration to approach the tilting speed.
   */
  virtual float GetTiltAcceleration () const = 0;

  /**
   * Zoom out speed after camera has been pushed in by the wall.
   */
  virtual void SetZoomOutCorrectionSpeed (float zoomspeed) = 0;

  /**
   * Zoom out speed after camera has been pushed in by the wall.
   */
  virtual float GetZoomOutCorrectionSpeed () const = 0;
};

#endif // __CEL_PF_TRACKINGCAMERA__
