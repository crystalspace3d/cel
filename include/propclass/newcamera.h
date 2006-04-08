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
struct iPcZoneManager;

struct iPcNewCamera;

struct iCelCameraMode
{
  /** Tells the camera mode what camera has it attached.
   *  \param camera 	The parent camera.
   */
  virtual void SetParentCamera(iPcNewCamera * camera) = 0;

  /** Decides if this camera mode should use spring physics for the camera's position.
   *  \return 	True if this camera mode uses spring physics.
   */
  virtual bool UseSpringPos() const = 0;

  /** Decides if this camera mode should use spring physics for the camera's target.
   *  \return 	True if this camera mode uses spring physics.
   */
  virtual bool UseSpringTarget() const = 0;

  /** Decides if this camera mode should use spring physics for the camera's up vector.
   *  \return 	True if this camera mode uses spring physics.
   */
  virtual bool UseSpringUp() const = 0;

  /** Decides if collision detection should be allowed if for this mode the camera wants it.
   *  \return   True if this camera mode allows collision detection.
   */
  virtual bool AllowCollisionDetection() const = 0;

  /** 
   * Decides if the mesh the camera is attached to should be drawn or not in
   * this mode.
   * \return 	True if the attached mesh should be drawn.
   */
  virtual bool DrawAttachedMesh() const = 0;
  
  /** Gets the spring coefficient to use for the spring physics.
   *  \return 	The spring coefficient of this camera mode.
   */
  virtual float GetSpringCoefficient() const = 0;

  /** Gets the desired camera position.
   *  \return 	The desired camera position.
   */
  virtual const csVector3 & GetPosition() const = 0;

  /** Gets the desired camera target.
   *  \return 	The desired camera target.
   */
  virtual const csVector3 & GetTarget() const = 0;
  
  /** Gets the desired camera up vector.
   *  \return 	The desired camera up vector.
   */
  virtual const csVector3 & GetUp() const = 0;

  /** Informs the camera mode that it should compute the desired position,
   *  target, up, etc. of the camera now.
   *  \return 	True on success.
   */
  virtual bool DecideCameraState() = 0; 
};

SCF_VERSION(iPcNewCamera, 0, 0, 1);
/**
 * This is a camera property class.
 */
struct iPcNewCamera : public iPcCamera
{
  /**
   * Gets the base position of the camera in world coordinates.
   * \return 	The base position of the camera in world coordinates.
   */
  virtual const csVector3 & GetBasePos() const = 0;

  /**
   * Gets the base direction of the camera.
   * \return 	The base direction of the camera.
   */
  virtual const csVector3 & GetBaseDir() const = 0;

  /** 
   * Gets the base up vector of the camera.
   * \return 	The base up vector of the camera.
   */
  virtual const csVector3 & GetBaseUp() const = 0;

  /**
   * Gets the base transform of the camera.
   * \return	The base transform of the camera.
   */
  virtual const csReversibleTransform & GetBaseTrans() const = 0;

  /**
   * Gets the current position of the camera.
   * \return    The current position of the camera.
   */
  virtual const csVector3 & GetPos() const = 0;

  /**
   * Gets the current target of the camera.
   * \return    The current target of the camera.
   */
  virtual const csVector3 & GetTarget() const = 0;

  /**
   * Gets the current up vector of the camera.
   * \return    The current up vector of the camera.
   */
  virtual const csVector3 & GetUp() const = 0;

  /**
   * Sets the offset from the center of the mesh's iMovable to the position of
   * the camera.
   * \param offset the offset from the center of the mesh to the camera
   *        position.
   */
  virtual void SetPositionOffset(const csVector3 & offset) = 0;

  /**
   * Returns whether the camera will use collision detection to avoid moving through walls.
   * \return True if collision detection is enabled.
   */
  virtual bool DetectCollisions() const = 0;

  /**
   * Sets whether the camera will use collision detection to avoid moving through walls.
   * \param detectCollisions True if the camera should detect collisions.
   */
  virtual void SetCollisionDetection(bool detectCollisions) = 0;

  /**
   * Sets the spring coefficient that will be used when a collision is detected.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetCollisionSpringCoefficient(float springCoef) = 0;

  /**
   * Returns the spring coefficient that is used when a collision is detection.
   * \return The collision detection spring coefficient.
   */
  virtual float GetCollisionSpringCoefficient() const = 0;

  /**
   * Determines whether the camera is currently in a transition from one camera
   * mode to another.
   * \return True if the camera is currently in a transition.
   */
  virtual bool InCameraTransition() const = 0;

  /**
   * This controls the springyness of the transition to a new camera mode when
   * a new camera mode is selected.
   * \param springCoef The new spring coefficient of camera transitions.
   */
  virtual void SetTransitionSpringCoefficient(float springCoef) = 0;

  /**
   * This gets the springyness of the transition to a new camera mode when a new
   * camera mode is selected.
   * \return The spring coefficient of the camera transitions.
   */
  virtual float GetTransitionSpringCoefficient() const = 0;

  /**
   * If the distance between the current camera position and the new camera mode
   * is within this cutoff distance, then the camera will cease to be in a transition
   * and be in the new camera mode.
   * \param cutOffPosDist The camera transition mode cutoff distance from position to position.
   * \param cutOffTargetDist The camera transition mode cutoff distance from target to target.
   */
  virtual void SetTransitionCutoffDistance(float cutOffPosDist, float cutOffTargetDist) = 0;

  /**
   * Grabs the camera transition cutoff distance from position to position between the camera 
   * and the camera mode.
   * \return The camera transition cutoff distance from target to target.
   */
  virtual float GetTransitionCutoffPosDistance() const = 0;

  /**
   * Grabs the camera transition cutoff distance from target to target between the camera 
   * and the camera mode.
   * \return The camera transition cutoff distance from position to position.
   */
  virtual float GetTransitionCutoffTargetDistance() const = 0;

  /** Attaches a camera mode to this camera.
   *  \param mode 	The camera mode to attach.
   *  \return 		A unique id for the attached camera mode.
   */
  virtual size_t AttachCameraMode(iCelCameraMode * mode) = 0;

  typedef enum _CEL_CAMERA_MODE
  {
    CCM_FIRST_PERSON,
    CCM_THIRD_PERSON,

    CCM_COUNT
  } CEL_CAMERA_MODE;

  /** Attaches a built-in camera mode to this camera.
   *  \param mode 	The id of the built-in camera mode to attach.
   *  \return 		A unique id for the attached camera mode.
   */
  virtual size_t AttachCameraMode(CEL_CAMERA_MODE mode) = 0;

  /** Gets the index of the current camera mode.
   *  \return 	The index of the current camera mode.
   */
  virtual size_t GetCurrentCameraModeIndex() const = 0;

  /** Gets the current camera mode.
   *  \return 	The current camera mode.
   */
  virtual iCelCameraMode * GetCurrentCameraMode() = 0;

  /** Sets the current camera mode.
   *  \param modeIndex 	The index of the current camera mode.
   *  \return 		True on successful camera mode change.
   */
  virtual bool SetCurrentCameraMode(size_t modeIndex) = 0;

  /** Sets the current camera mode to the next available mode.
   */
  virtual void NextCameraMode() = 0;

  /** Sets the current camera mode to the previous available mode.
   */
  virtual void PrevCameraMode() = 0;

  /**
   * Render. This will clear the screen then draw on top of it.
   */
  virtual void Draw() = 0;
};

#endif // __CEL_PF_NEW_CAMERA__

