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

#ifndef __CEL_PF_CAMERA__
#define __CEL_PF_CAMERA__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCamera;
struct iView;
struct iPcRegion;
struct iPcZoneManager;

SCF_VERSION (iPcCamera, 0, 0, 2);

/**
 * The representation of a camera.
 */
struct iPcCamera : public iBase
{
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
   * Set Camera to a Region
   */
  virtual bool SetRegion (iPcRegion* region, bool point = true,
      const char* name = 0) = 0;

  /**
   * Set Camera to a zone manager
   */
  virtual bool SetZoneManager (iPcZoneManager* zonemgr, bool point,
      const char* regionname, const char* name = 0) = 0;

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
   * Control velocity of pitch. This can be useful for looking up or
   * down.
   */
  virtual void SetPitchVelocity (float pitchVel) = 0;

  /**
   * Set the view rectangle to use on screen.
   */
  virtual void SetRectangle (int x, int y, int w, int h) = 0;

  /**
   * Get the camera.
   */
  virtual iCamera* GetCamera () const = 0;

  /**
   * Get the view.
   */
  virtual iView* GetView () const = 0;

  /**
   * Set flag indicating if the camera should clear z-buffer every frame.
   * Off by default unless the loaded map specifies this.
   */
  virtual void SetClearZBuffer (bool flag) = 0;

  /// Get the clear zbuffer flag.
  virtual bool GetClearZBuffer () const = 0;

  /**
   * Set flag indicating if the camera should clear screen every frame.
   * Off by default unless the loaded map specifies this.
   */
  virtual void SetClearScreen (bool flag) = 0;

  /// Get the clear screen flag.
  virtual bool GetClearScreen () const = 0;

  /// Disable distance clipping.
  virtual void DisableDistanceClipping () = 0;
  /**
   * Enable fixed distance clipping.
   * In this mode there is a fixed plane at the specified distance.
   * All geometry that is fully behind that plane will not be rendered.
   * This can speed up rendering.
   */
  virtual void EnableFixedDistanceClipping (float dist) = 0;
  /**
   * Enable adaptive distance clipping. In this mode the clipping plane
   * will vary depending on the desired minimum and maximum fps.
   * \param min_fps is the minimum fps that we would like to have. If we go
   *        beyond that then we will move the clipping plane closer.
   * \param max_fps is the maximum fps that is ok. If we go beyond that we
   *        will move the clipping plane further away.
   * \param min_dist is the minimum clipping plane distance. We will never
   *        move the clipping closer to the camera then this distance.
   */
  virtual void EnableAdaptiveDistanceClipping (float min_fps,
	float max_fps, float min_dist) = 0;
  /**
   * Returns true if we use distance clipping (either fixed or adaptive).
   */
  virtual bool UseDistanceClipping () const = 0;
  /**
   * Returns true if we use fixed distance clipping (as opposed to adaptive).
   */
  virtual bool UseFixedDistanceClipping () const = 0;
  /**
   * Get the fixed distance (returns < 0 if adaptive is used).
   */
  virtual float GetFixedDistance () const = 0;
  /**
   * Get the minimum fps used for adaptive distance clipping.
   */
  virtual float GetAdaptiveMinFPS () const = 0;
  /**
   * Get the maximum fps used for adaptive distance clipping.
   */
  virtual float GetAdaptiveMaxFPS () const = 0;
  /**
   * Get the minimum distance used for adaptive distance clipping.
   */
  virtual float GetAdaptiveMinDistance () const = 0;
};

#endif // __CEL_PF_CAMERA__

