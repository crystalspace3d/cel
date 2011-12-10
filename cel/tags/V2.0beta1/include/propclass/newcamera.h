/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
    Copyright (C) 2007 by Dariusz Dawidowski

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

#ifndef __CEL_PF_NEW_CAMERA__
#define __CEL_PF_NEW_CAMERA__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/weakrefarr.h"
#include "csutil/refarr.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "celtool/camera.h"
#include "propclass/region.h"
#include "propclass/mesh.h"

struct iCelEntity;
struct iObjectRegistry;
struct iGraphics3D;
struct iEngine;
struct iVirtualClock;
struct iKeyboardDriver;
struct iMouseDriver;
struct iCollideSystem;
struct iView;
class csView;
class celPcNewCamera;
class csReversibleTransform;
class csOrthoTransform;
struct iPcZoneManager;

struct iPcNewCamera;

namespace iPcmNewCamera
{
struct General : public virtual iBase
{
  SCF_INTERFACE (General, 0, 1, 0);

  /**
   * Tells the camera mode what camera has it attached.
   * \param camera The parent camera.
   */
  virtual void SetParentCamera (iPcNewCamera* camera) = 0;

  CS_DEPRECATED_METHOD_MSG("Use UseSpringOrigin () instead")
  /**
   * Decides if this camera mode should use spring physics for the camera's
   * position.
   * \return True if this camera mode uses spring physics.
   */
  virtual bool UseSpringPos () const = 0;

  /**
   * Decides if this camera mode should use spring physics for the camera's
   * origin.
   * \return True if this camera mode uses spring physics.
   */
  virtual bool UseSpringOrigin () const = 0;

  /**
   * Decides if this camera mode should use spring physics for the camera's
   * target.
   * \return True if this camera mode uses spring physics.
   */
  virtual bool UseSpringTarget () const = 0;

  /**
   * Decides if this camera mode should use spring physics for the camera's
   * up vector.
   * \return True if this camera mode uses spring physics.
   */
  virtual bool UseSpringUp () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use GetCollisionDetection () instead")
  /**
   * Decides if collision detection should be allowed if for this mode the
   * camera wants it.
   * \return True if this camera mode allows collision detection.
   */
  virtual bool AllowCollisionDetection () const = 0;

  /**
   * Decides if collision detection should be allowed if for this mode the
   * camera wants it.
   * \return True if this camera mode allows collision detection.
   */
  virtual bool GetCollisionDetection () const = 0;

  /**
   * Decides if the mesh the camera is attached to should be drawn or not in
   * this mode.
   * \return True if the attached mesh should be drawn.
   */
  virtual bool DrawAttachedMesh () const = 0;

  /**
   * Gets the spring coefficient to use for the spring physics.
   * \return The spring coefficient of this camera mode.
   */
  virtual float GetSpringCoefficient () const = 0;

  /**
   * Sets the spring coefficient to use for the spring physics.
   * @param s The spring coefficient of this camera mode.
   */
  virtual void SetSpringCoefficient (float s) = 0;

  /**
   * Sets the origin spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetOriginSpringCoefficient (float springCoef) = 0;

  /**
   * Returns the origin spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetOriginSpringCoefficient () const = 0;

  /**
   * Sets the target spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetTargetSpringCoefficient (float springCoef) = 0;

  /**
   * Returns the target spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetTargetSpringCoefficient () const = 0;

  /**
   * Sets the up vector spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetUpSpringCoefficient (float springCoef) = 0;

  /**
   * Returns the up vector spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetUpSpringCoefficient () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use GetOrigin () instead")
  /**
   * Gets the desired camera position.
   * \return The desired camera position.
   */
  virtual const csVector3& GetPosition () const = 0;

  /**
   * Gets the desired camera origin.
   * \return The desired camera origin.
   */
  virtual const csVector3& GetOrigin () const = 0;

  /**
   * Gets the desired camera target.
   * \return The desired camera target.
   */
  virtual const csVector3& GetTarget () const = 0;

  /**
   * Gets the desired camera up vector.
   * \return The desired camera up vector.
   */
  virtual const csVector3& GetUp () const = 0;

  /**
   * Informs the camera mode that it should compute the desired origin,
   * target, up, etc. of the camera now.
   * \return True on success.
   */
  virtual bool DecideCameraState () = 0;
};

/**
 * This is a camera to be used with actoranalog. It tracks left and right movement and follows
 * movement in and out of the camera.
 * you can visualise the camera <--> player connection as a hard pole with a spring on the end
 *   C----|oooP
 * the ---- (hard pole) can never be compressed except by collision detection
 * whereas the springs ooo can be decompressed and stretched between 0 and 2 * normal spring length
 * ... the spring is relaxed at the normal spring length
 */
struct Tracking : public virtual General
{
  SCF_INTERFACE (Tracking, 0, 0, 1);

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
};
} // iPcmNewCamera

typedef iPcmNewCamera::General iCelCameraMode;

/**
 * This is a camera property class.
 */
struct iPcNewCamera : public virtual iPcCamera
{
  SCF_INTERFACE (iPcNewCamera, 0, 1, 0);

  CS_DEPRECATED_METHOD_MSG("Use GetBaseOrigin () instead")
  /**
   * Gets the base position of the camera in world coordinates.
   * \return The base position of the camera in world coordinates.
   */
  virtual const csVector3& GetBasePos () const = 0;

  /**
   * Gets the base origin of the camera in world coordinates.
   * \return The base origin of the camera in world coordinates.
   */
  virtual const csVector3& GetBaseOrigin () const = 0;

  /**
   * Gets the base direction of the camera.
   * \return The base direction of the camera.
   */
  virtual const csVector3& GetBaseDir () const = 0;

  /**
   * Gets the base up vector of the camera.
   * \return The base up vector of the camera.
   */
  virtual const csVector3& GetBaseUp () const = 0;

  /**
   * Gets the base transform of the camera.
   * \return The base transform of the camera.
   */
  virtual const csReversibleTransform& GetBaseTrans () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use GetOrigin () instead")
  /**
   * Gets the current position of the camera.
   * \return The current position of the camera.
   */
  virtual const csVector3& GetPos () const = 0;

  /**
   * Gets the current origin of the camera.
   * \return The current origin of the camera.
   */
  virtual const csVector3& GetOrigin () const = 0;

  /**
   * Gets the current target of the camera.
   * \return The current target of the camera.
   */
  virtual const csVector3& GetTarget () const = 0;

  /**
   * Gets the current up vector of the camera.
   * \return The current up vector of the camera.
   */
  virtual const csVector3& GetUp () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use SetTargetPositionOffset () instead")
  /**
   * Sets the offset from the center of the mesh's iMovable
   * (deprecated).
   * \param offset the offset from the center of the mesh
   */
  virtual void SetPositionOffset (const csVector3& offset) = 0;

  /**
   * Sets the offset from the center of the mesh's iMovable to the position of
   * the camera.
   * \param offset the offset from the center of the mesh to the camera
   *        position.
   */
  virtual void SetTargetPositionOffset (const csVector3& offset) = 0;

  /**
   * Sets the minimum allowed offset up until the target.
   */
  virtual void SetTargetMinimumOffset (float minoff) = 0;

  /**
   * Sets the offset from the center of the mesh's iMovable to the position of
   * the camera.
   * \param offset the offset from the center of the mesh to the camera
   *        position.
   */
  virtual void SetCameraPositionOffset (const csVector3& offset) = 0;

  /**
   * Sets the common spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetSpringCoefficient (float springCoef) = 0;

  /**
   * Returns the common spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetSpringCoefficient () const = 0;

  /**
   * Sets the origin spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetOriginSpringCoefficient (float springCoef) = 0;

  /**
   * Returns the origin spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetOriginSpringCoefficient () const = 0;

  /**
   * Sets the target spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetTargetSpringCoefficient (float springCoef) = 0;

  /**
   * Returns the target spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetTargetSpringCoefficient () const = 0;

  /**
   * Sets the up vector spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetUpSpringCoefficient (float springCoef) = 0;

  /**
   * Returns the up vector spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetUpSpringCoefficient () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use GetCollisionDetection () instead")
  /**
   * Returns whether the camera will use collision detection to avoid moving
   * through walls.
   * \return True if collision detection is enabled.
   */
  virtual bool DetectCollisions () const = 0;

  /**
   * Sets whether the camera will use collision detection to avoid
   * moving through walls.
   * \param detectCollisions True if the camera should detect collisions.
   */
  virtual void SetCollisionDetection (bool detectCollisions) = 0;

  /**
   * Returns whether the camera will use collision detection to avoid moving
   * through walls.
   * \return True if collision detection is enabled.
   */
  virtual bool GetCollisionDetection () const = 0;

  /**
   * Set the y offset from player position for collision detection focus.
   */
  virtual void SetCollisionYFocusOffset (float yoff) = 0;

  /**
   * Get the y focus offset used in collision detection.
   */
  virtual float GetCollisionYFocusOffset () const = 0;

  /**
   * multiplier for projected beam used in collision detection to place
   * camera. Values greater than 1.0f are desireable but project the
   * camera position further away from it's true point the larger it is.
   * A value of about 1.5f is usually good and is set as default anyway.
   * 1.0f will give the true collision intersection of the camera, but you
   * might end up seeing through walls ;)
   */
  virtual void SetCollisionCorrection (float corr) = 0;

  /**
   * Get collision correction multiplier... returns a float. is const \o/ woo!
   */
  virtual float GetCollisionCorrection () const = 0;

  /**
   * Set the player collision avoidance radius. Moves up over the player
   * when camera gets too close.
   * \param radsq The squared radius value
   */
  virtual void SetCollisionAvoidanceRadiusSq (float radsq) = 0;

  /**
   * Get the player collision avoidance radius
   */
  virtual float GetCollisionAvoidanceRadiusSq () const = 0;

  /**
   * Set the y offset for camera to move upwards by for avoidance
   * when it comes too close.
   */
  virtual void SetCollisionYAvoidance (float yavoid) = 0;

  /**
   * Get the Y offset avoidance value.
   */
  virtual float GetCollisionYAvoidance () const = 0;

  /**
   * Set the interpolation used in the collision avoidance. Hard to describe how the value works
   * but values close to 0 will give a fast jump, 1.0 is more realistic but higher is nicer and slower.
   */
  virtual void SetCollisionAvoidanceInterpolation (float aint) = 0;

  /**
   * Get the collision avoidance movement interpolation value.
   */
  virtual float GetCollisionAvoidanceInterpolation () const = 0;

  /**
   * Sets the spring coefficient that will be used when a collision is detected.
   * \param springCoef The new spring coefficient.
   */
  CS_DEPRECATED_METHOD_MSG("Non existant. Use the new collision detection system.")
  virtual void SetCollisionSpringCoefficient (float springCoef) = 0;

  /**
   * Returns the spring coefficient that is used when a collision is detection.
   * \return The collision detection spring coefficient.
   */
  CS_DEPRECATED_METHOD_MSG("Non existant. Use the new collision detection system.")
  virtual float GetCollisionSpringCoefficient () const = 0;

  /**
   * Determines whether the camera is currently in a transition from one camera
   * mode to another.
   * \return True if the camera is currently in a transition.
   */
  virtual bool InCameraTransition () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use SetTransitionTime (float) instead.")
  /**
   * This controls the springyness of the transition to a new camera mode when
   * a new camera mode is selected.
   * \param springCoef The new spring coefficient of camera transitions.
   */
  virtual void SetTransitionSpringCoefficient (float springCoef) = 0;

  CS_DEPRECATED_METHOD_MSG("Use SetTransitionTime (float) instead.")
  /**
   * This gets the springyness of the transition to a new camera mode when a new
   * camera mode is selected.
   * \return The spring coefficient of the camera transitions.
   */
  virtual float GetTransitionSpringCoefficient () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use GetTransitionTime () instead.")
  /**
   * If the distance between the current camera position and the new camera
   * mode is within this cutoff distance, then the camera will cease to be
   * in a transition and be in the new camera mode.
   * \param cutOffOriginDist The camera transition mode cutoff distance from
   * origin to origin.
   * \param cutOffTargetDist The camera transition mode cutoff distance from
   * target to target.
   */
  virtual void SetTransitionCutoffDistance (float cutOffOriginDist,
  	float cutOffTargetDist) = 0;

  CS_DEPRECATED_METHOD_MSG("Use GetTransitionTime () instead.")
  /**
   * Grabs the camera transition cutoff distance from position to position
   * between the camera and the camera mode.
   * \return The camera transition cutoff distance from target to target.
   */
  virtual float GetTransitionCutoffPosDistance () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use GetTransitionTime () instead.")
  /**
   * Grabs the camera transition cutoff distance from origin to origin
   * between the camera and the camera mode.
   * \return The camera transition cutoff distance from target to target.
   */
  virtual float GetTransitionCutoffOriginDistance () const = 0;

  CS_DEPRECATED_METHOD_MSG("Use GetTransitionTime () instead.")
  /**
   * Grabs the camera transition cutoff distance from target to target
   * between the camera and the camera mode.
   * \return The camera transition cutoff distance from position to position.
   */
  virtual float GetTransitionCutoffTargetDistance () const = 0;

  /**
   * Set the time to perform a transition.
   * \param time Time to transition to the new mode.
   */
  virtual void SetTransitionTime (float time) = 0;

  /**
   * Get the time to transition to a new mode.
   */
  virtual float GetTransitionTime () const = 0;

  /**
   * Attaches a camera mode to this camera.
   * \param mode The camera mode to attach.
   * \return A unique id for the attached camera mode.
   */
  virtual size_t AttachCameraMode (iCelCameraMode* mode) = 0;

  enum CEL_CAMERA_MODE
  {
    CCM_FIRST_PERSON = 0,
    CCM_THIRD_PERSON,
    CCM_TRACKING,
    CCM_HORIZONTAL,
    CCM_ISOMETRIC,
    CCM_COUNT
  };

  /**
   * Attaches a built-in camera mode to this camera.
   * \param mode The id of the built-in camera mode to attach.
   * \return A unique id for the attached camera mode.
   */
  virtual size_t AttachCameraMode (CEL_CAMERA_MODE modetype) = 0;

  /**
   * Gets the index of the current camera mode.
   * \return The index of the current camera mode.
   */
  virtual size_t GetCurrentCameraModeIndex () const = 0;

  /**
   * Gets the current camera mode.
   * \return The current camera mode.
   */
  virtual iCelCameraMode* GetCurrentCameraMode () = 0;

  /**
   * Sets the current camera mode.
   * \param modeIndex The index of the current camera mode.
   * \return True on successful camera mode change.
   */
  virtual bool SetCurrentCameraMode (size_t modeIndex) = 0;

  /**
   * Sets the current camera mode to the next available mode.
   */
  virtual void NextCameraMode () = 0;

  /**
   * Sets the current camera mode to the previous available mode.
   */
  virtual void PrevCameraMode () = 0;

  /**
   * Render. This will clear the screen then draw on top of it.
   */
  virtual void Draw () = 0;

  /**
    * Get the transform corresponding to this camera.
    * In this transform, 'other' is world space and 'this' is camera
    * space.
    */
  virtual const csOrthoTransform &GetTransform () = 0;

  /**
   * Gets the specified camera mode.
   * \param idx If -1 it will return the current mode
   * \return The current camera mode.
   */
  virtual iCelCameraMode* GetCameraMode (int idx = -1) = 0;

  /**
   * Gets the camera mode's specialised interface. If the specified
   * mode does not implement that interface will return null.
   * \param T Template parameter for camera interface
   * \param idx If -1 it will return the current mode
   * \return The current camera mode's interface.
   */
  template <typename T>
  csPtr<T> QueryModeInterface (int idx = -1)
  {
    iBase* cmode = GetCameraMode (idx);
    if (!cmode)
      return 0;
    return scfQueryInterface<T> (cmode);
  }

  /**
   * Reset the camera position.
   */
  virtual bool Reset () = 0;
};

#endif // __CEL_PF_NEW_CAMERA__
