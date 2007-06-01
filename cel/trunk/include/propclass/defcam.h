/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2005 by Jorrit Tyberghein

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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PF_DEFAULT_CAMERA__
#define __CEL_PF_DEFAULT_CAMERA__

#include "propclass/camera.h"

struct iCelEntity;

/**
 * This is a property class that wraps a standard first-person and
 * third-person camera and related functionality. It depends on either iPcMesh
 * or iPcLinearMovement.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - SetCamera: parameters 'modename' (string), 'spring' (vector3),
 *   'turnspeed' (float), 'swingcoef' (float), 'fpoffset' (vector3),
 *   'tpoffset' (vector3), 'pitch' (float), 'pitchvelocity' (float),
 *   'yaw' (float), 'yawvelocity' (float) and 'distance'
 *   (vector3=minimal, default, maximal).
 * - SetZoneManager: parameters 'entity' (string), 'region' (string)
 *   and 'start' (string).
 * - SetFollowEntity: parameters 'entity' (string).
 * - CenterCamera: no parameters.
 * - PointCamera: parameters 'start' (map start name).
 * - SetRectangle: parameters 'x' (long), 'y' (long), 'w' (long), 'h' (long).
 * - SetPerspectiveCenter: parameters 'x' (float), 'y' (float).
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - pitchvelocity (float, read/write): pitch velocity.
 * - yawvelocity (float, read/write): yaw velocity.
 * - distancevelocity (float, read/write): distance (zoom) velocity.
 * - pitch (float, read/write): pitch.
 * - yaw (float, read/write): yaw.
 * - distance (float, read/write): distance (zoom).
 */
struct iPcDefaultCamera : public virtual iPcCamera
{
  SCF_INTERFACE (iPcDefaultCamera, 0, 0, 3);

  /**
   * Camera modes
   */
  enum CameraMode
  {
    freelook = 0,
    firstperson,
    thirdperson,
    m64_thirdperson,
    lara_thirdperson,

    actual_data,	// The actual camera data
    last_actual,	// Keep reference to the actual data last frame
    transition,
    camerror,		// Error between actual camera pos and ideal.

    CameraMode_count
  };

  /**
   * Set the destination entity manually. If this is not set then the
   * camera will attempt to fetch mesh, linmove, mechobj from current entity.
   * If 'entity' is 0 it will go back to use the ones from the current entity.
   */
  virtual void SetFollowEntity (iCelEntity* entity) = 0;

  /**
   * Set camera mode.
   */
  virtual bool SetMode (CameraMode m, bool use_cd = true) = 0;

  /**
   * Get camera mode.
   */
  virtual CameraMode GetMode () const = 0;

  /**
   * Set camera mode by name.
   */
  virtual bool SetModeName (const char* m, bool use_cd = true) = 0;

  /**
   * Get camera mode name.
   */
  virtual const char* GetModeName () const = 0;

  /**
   * Get next possible camera mode. This is useful for looping over
   * all camera modes with some key in a game.
   */
  virtual CameraMode GetNextMode () const = 0;

  /**
   * Point the camera to some given location.
   * Returns false if the location could not be found.
   */
  virtual bool PointCamera (const char* start) = 0;

  /**
   * Set spring parameters for the current camera mode.
   */
  virtual void SetSpringParameters (float springCoef,
  	float intertialDampeningCoef, float springLength) = 0;

  /**
   * Set the minimum and maximum distance between camera and
   * player. Only used by m64_thirdperson, lara_thirdperson, and
   * freelook.
   */
  virtual void SetMinMaxCameraDistance (float minDistance,
  	float maxDistance) = 0;

  /**
   * Set the turn speed for the camera. Only used by lara_thirdperson
   * and m64_thirdperson.
   */
  virtual void SetTurnSpeed (float turnSpeed) = 0;

  /**
   * Set the swing coefficient for the camera. Only used by lara_thirdperson.
   */
  virtual void SetSwingCoef (float swingCoef) = 0;

  /**
   * Set offset for first person camera (offset for camera
   * relative to player model).
   */
  virtual void SetFirstPersonOffset (const csVector3& offset) = 0;

  /**
   * Set offset for third person camera (offset for camera
   * relative to player model).
   */
  virtual void SetThirdPersonOffset (const csVector3& offset) = 0;

  /**
   * Center camera in any mode except first person mode. This will
   * basically force the camera behind the actor.
   */
  virtual void CenterCamera () = 0;

  /**
   * Control pitch.
   */
  virtual void SetPitch (float pitch) = 0;

  /**
   * Get current pitch.
   */
  virtual float GetPitch () const = 0;

  /**
   * Control velocity of pitch. This can be useful for looking up or
   * down.
   */
  virtual void SetPitchVelocity (float pitchVel) = 0;

  /**
   * Get current pitch velocity.
   */
  virtual float GetPitchVelocity () const = 0;

  /**
   * Moves the pitch (up/down) of the camera
   * \param deltaPitch the amount to move from the current pitch
   * \param mode Optional - the camera mode to apply it to (leave blank for
   * current)
   */
  virtual void MovePitch (float deltaPitch, int mode = -1) = 0;

  /**
   * Sets the yaw (left/right) of the camera
   * \param yaw the new yaw of the camera
   * \param mode Optional - the camera mode to apply it to (leave blank for
   * current)
   */
  virtual void SetYaw (float yaw, int mode = -1) = 0;

  /**
   * Moves the yaw (left/right) of the camera
   * \param deltaYaw the amount to move from the current yaw
   * \param mode Optional - the camera mode to apply it to (leave blank for
   * current)
   */
  virtual void MoveYaw (float deltaYaw, int mode = -1) = 0;

  /**
   * Returns the yaw (left/right) of the camera
   * \param mode Optional - the camera mode to get it from (leave blank for
   * current)
   * \return the yaw (left/right) of the camera
   */
  virtual float GetYaw (int mode = -1) const = 0;

  /**
   * Sets the yaw (up/down) velocity of the camera
   * \param yawVel the velocity of the yaw
   */
  virtual void SetYawVelocity (float yawVel) = 0;

  /**
   * Gets the yaw (up/down) velocity of the camera
   * \return the yaw (up/down) of the camera
   */
  virtual float GetYawVelocity () const = 0;

  /**
   * Set the distance between the character's eyes and the camera.
   * The camera algorithms are responsible for calculating the actual
   * position of the camera.
   */
  virtual void SetDistance (float distance, int mode=-1) = 0;

  /**
   * Get the distance between the character's eyes and the camera.
   * The camera algorithms are responsible for calculating the actual
   * position of the camera.
   */
  virtual float GetDistance (int mode=-1) = 0;

  /**
   * Sets the distance (zoom) velocity of the camera
   * \param distanceVel the velocity of the distance
   */
  virtual void SetDistanceVelocity (float distanceVel) = 0;

  /**
   * Gets the distance (zoom) velocity of the camera
   * \return the distance (zoom) velocity of the camera
   */
  virtual float GetDistanceVelocity () const = 0;

};

#endif // __CEL_PF_DEFAULT_CAMERA_FACTORY__

