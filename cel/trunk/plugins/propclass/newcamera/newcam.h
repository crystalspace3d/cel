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

#ifndef __CEL_PF_NEW_CAMERA_FACTORY__
#define __CEL_PF_NEW_CAMERA_FACTORY__

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
#include "propclass/newcamera.h"
#include "propclass/region.h"
#include "propclass/mesh.h"
#include "csgeom/transfrm.h"

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
struct iPcZoneManager;

/**
 * Factory for engine stuff.
 */
CEL_DECLARE_FACTORY(NewCamera)

/**
 * This is a camera property class.
 */
class celPcNewCamera : public scfImplementationExt2<celPcNewCamera,
	celPcCameraCommon, iPcNewCamera, scfFakeInterface<iPcCamera> >
{
private:
  csRef<iCollideSystem> cdsys;

  csRefArray<iCelCameraMode> cameraModes;

  size_t currMode;

  csVector3 baseOrigin, baseDir, baseUp;
  csReversibleTransform baseTrans;
  iSector* baseSector;
  csVector3 camOrigin, camTarget, camUp;
  float originSpringCoef, targetSpringCoef, upSpringCoef;

  // offsetOrigin is where to transform the camera along the direction
  // it is set to after first pass.
  csVector3 offsetOrigin;
  csVector3 offsetTarget;
//  csVector3 offsetOrigin, offsetTarget;

  csWeakRef<iPcMesh> pcmesh;
  iSector* lastActorSector;

  bool detectCollisions;
  float collisionSpringCoef;
  float collisionOriginRadius, collisionTargetRadius;

  bool inTransition;
  float transitionSpringCoef;
  float transitionCutoffOriginDist;
  float transitionCutoffTargetDist;

  // Has this camera been initialised yet?
  bool init_reset;

  void UpdateMeshVisibility ();

  /** Calculates an elastic vector based on an ideal vector and a current one.
   *  \param curr         The current vector value. This value gets updated.
   *  \param ideal        The ideal/target vector value.
   *  \param deltaTime    The change in time since last frame.
   *  \param springCoef   The spring coefficient to use in our calculations.
   */
  static void CalcElasticVec (csVector3& curr, const csVector3& ideal,
  	float deltaTime, float springCoef);

  // action parameters
  static csStringID id_name;
  static csStringID id_nr;
  static csStringID id_x;
  static csStringID id_y;
  static csStringID id_w;
  static csStringID id_h;
  static csStringID id_enable;
  static csStringID id_minfps;
  static csStringID id_maxfps;
  static csStringID id_mindist;
  static csStringID id_dist;
  celOneParameterBlock* params;

  // actions
  enum actionids
  {
    action_attachcameramode = 0,
    action_setcameramode,
    action_nextcameramode,
    action_prevcameramode,
    action_setrectangle,
    action_setperspcenter,
    action_adaptiveclipping,
    action_fixedclipping
  };

  // properties
  enum propids
  {
    propid_colldet = 0,
    propid_colldet_spring,
    propid_colldet_origin_radius,
    propid_colldet_target_radius,
    propid_offset,
    propid_offset_origin,
    propid_offset_target,
    propid_spring,
    propid_spring_origin,
    propid_spring_target,
    propid_spring_up,
    propid_trans_spring,
    propid_trans_cutofforigin,
    propid_trans_cutofftarget
  };
  static PropertyHolder propinfo;

public:
  celPcNewCamera (iObjectRegistry* object_reg);
  virtual ~celPcNewCamera ();

  virtual csPtr<iCelDataBuffer> Save ()
  {
    return 0;
  }
  virtual bool Load (iCelDataBuffer* databuf)
  {
    return true;
  }
  virtual bool PerformActionIndexed (int, iCelParameterBlock* params,
  	celData& ret);
  virtual bool SetPropertyIndexed (int, bool);
  virtual bool GetPropertyIndexed (int, bool&);
  virtual bool SetPropertyIndexed (int, float);
  virtual bool GetPropertyIndexed (int, float&);

  /**
   * Gets the base position of the camera in world coordinates.
   * \return The base position of the camera in world coordinates.
   */
  virtual const csVector3& GetBasePos () const;

  /**
   * Gets the base origin of the camera in world coordinates.
   * \return The base origin of the camera in world coordinates.
   */
  virtual const csVector3& GetBaseOrigin () const;

  /**
   * Gets the base direction of the camera.
   * \return The base direction of the camera.
   */
  virtual const csVector3& GetBaseDir () const;

  /**
   * Gets the base up vector of the camera.
   * \return The base up vector of the camera.
   */
  virtual const csVector3& GetBaseUp () const;

  /**
   * Gets the base transform of the camera.
   * \return The base transform of the camera.
   */
  virtual const csReversibleTransform& GetBaseTrans () const;

  /**
   * Gets the current position of the camera.
   * \return The current position of the camera.
   */
  virtual const csVector3& GetPos () const;

  /**
   * Gets the current origin of the camera.
   * \return The current origin of the camera.
   */
  virtual const csVector3& GetOrigin () const;

  /**
   * Gets the current target of the camera.
   * \return The current target of the camera.
   */
  virtual const csVector3& GetTarget () const;

  /**
   * Gets the current up vector of the camera.
   * \return The current up vector of the camera.
   */
  virtual const csVector3& GetUp () const;

  /**
   * Sets the offset from the center of the mesh's iMovable
   * (deprecated).
   * \param offset the offset from the center of the mesh
   */
  virtual void SetPositionOffset (const csVector3& offset);

  /**
   * Sets the offset from the center of the mesh's iMovable to the position of
   * the camera.
   * \param offset the offset from the center of the mesh to the camera
   *        position.
   */
  virtual void SetTargetPositionOffset (const csVector3& offset);

  /**
   * Sets the offset from the center of the mesh's iMovable to the position of
   * the camera.
   * \param offset the offset from the center of the mesh to the camera
   *        position.
   */
  virtual void SetCameraPositionOffset (const csVector3& offset);

  /**
   * Sets the common spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetSpringCoefficient (float springCoef);

  /**
   * Returns the common spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetSpringCoefficient () const;

  /**
   * Sets the origin spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetOriginSpringCoefficient (float springCoef);

  /**
   * Returns the origin spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetOriginSpringCoefficient () const;

  /**
   * Sets the target spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetTargetSpringCoefficient (float springCoef);

  /**
   * Returns the target spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetTargetSpringCoefficient () const;

  /**
   * Sets the up vector spring coefficient that will be used for attached
   * camera mode.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetUpSpringCoefficient (float springCoef);

  /**
   * Returns the up vector spring coefficient that will be used for attached
   * camera mode.
   */
  virtual float GetUpSpringCoefficient () const;

  /**
   * Returns whether the camera will use collision detection to avoid
   * moving through walls.
   * \return True if collision detection is enabled.
   */
  virtual bool DetectCollisions () const;

  /**
   * Sets whether the camera will use collision detection to avoid moving
   * through walls.
   * \param detectCollisions True if the camera should detect collisions.
   */
  virtual void SetCollisionDetection (bool detectCollisions);

  /**
   * Returns whether the camera will use collision detection to avoid
   * moving through walls.
   * \return True if collision detection is enabled.
   */
  virtual bool GetCollisionDetection () const;

  /**
   * Sets the spring coefficient that will be used when a collision is detected.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetCollisionSpringCoefficient (float springCoef);

  /**
   * Returns the spring coefficient that is used when a collision is detection.
   * \return The collision detection spring coefficient.
   */
  virtual float GetCollisionSpringCoefficient () const;

  /**
   * Determines whether the camera is currently in a transition from one camera
   * mode to another.
   * \return True if the camera is currently in a transition.
   */
  virtual bool InCameraTransition () const;

  /**
   * This controls the springyness of the transition to a new camera mode when
   * a new camera mode is selected.
   * \param springCoef The new spring coefficient of camera transitions.
   */
  virtual void SetTransitionSpringCoefficient (float springCoef);

  /**
   * This gets the springyness of the transition to a new camera mode when a new
   * camera mode is selected.
   * \return The spring coefficient of the camera transitions.
   */
  virtual float GetTransitionSpringCoefficient () const;

  /**
   * If the distance between the current camera position and the new camera
   * mode is within this cutoff distance, then the camera will cease to be
   * in a transition and be in the new camera mode.
   * \param cutOffOriginDist The camera transition mode cutoff distance from
   * origin to origin.
   * \param cutOffTargetDist The camera transition mode cutoff distance
   * from target to target.
   */
  virtual void SetTransitionCutoffDistance (float cutOffOriginDist,
  	float cutOffTargetDist);

  /**
   * Grabs the camera transition cutoff distance from position to position
   * between the camera and the camera mode.
   * \return The camera transition cutoff distance from target to target.
   */
  virtual float GetTransitionCutoffPosDistance () const;

  /**
   * Grabs the camera transition cutoff distance from origin to origin
   * between the camera and the camera mode.
   * \return The camera transition cutoff distance from target to target.
   */
  virtual float GetTransitionCutoffOriginDistance () const;

  /**
   * Grabs the camera transition cutoff distance from target to target
   * between the camera and the camera mode.
   * \return The camera transition cutoff distance from position to position.
   */
  virtual float GetTransitionCutoffTargetDistance () const;

  /**
   * Attaches a camera mode to this camera.
   * \param mode The camera mode to attach.
   * \return The index that the camera mode was stored.
   */
  virtual size_t AttachCameraMode (iCelCameraMode* mode);

  /**
   * Attaches a built-in camera mode to this camera.
   * \param mode The id of the built-in camera mode to attach.
   * \return A unique id for the attached camera mode.
   */
  virtual size_t AttachCameraMode (iPcNewCamera::CEL_CAMERA_MODE modetype);

  /**
   * Gets the index of the current camera mode.
   * \return The index of the current camera mode.
   */
  virtual size_t GetCurrentCameraModeIndex () const;

  /**
   * Gets the current camera mode.
   * \return The current camera mode.
   */
  virtual iCelCameraMode* GetCurrentCameraMode ();

  /**
   * Sets the current camera mode.
   * \param modeIndex The index of the current camera mode.
   * \return True on successful camera mode change.
   */
  virtual bool SetCurrentCameraMode (size_t modeIndex);

  /**
   * Sets the current camera mode to the next available mode.
   */
  virtual void NextCameraMode ();

  /**
   * Sets the current camera mode to the previous available mode.
   */
  virtual void PrevCameraMode ();

  /**
   * Render. This will clear the screen then draw on top of it.
   */
  virtual void Draw ();
  virtual void TickEveryFrame ();

  // get the transform corresponding to this camera
  virtual const csOrthoTransform &GetTransform ();

  /**
   * Gets the specified camera mode.
   * \param idx If -1 it will return the current mode
   * \return The current camera mode.
   */
  virtual iCelCameraMode* GetCameraMode (int idx = -1);

  /**
   * Reset the camera position.
   */
  virtual bool Reset ();

  void UpdateCamera ();
  int GetDrawFlags ();

  virtual bool SetRegion (iPcRegion* region, bool point = true,
  	const char* name = 0)
  {
    return celPcCameraCommon::SetRegion (region, point, name);
  }
  virtual bool SetZoneManager (iPcZoneManager* zonemgr, bool point,
  	const char* regionname, const char* name = 0)
  {
    return celPcCameraCommon::SetZoneManager (zonemgr, point, regionname,
    	name);
  }
  virtual bool SetZoneManager (const char* entityname, bool point,
  	const char* regionname, const char* name = 0)
  {
    return celPcCameraCommon::SetZoneManager (entityname, point, regionname,
    	name);
  }
  virtual void SetRectangle (int x, int y, int w, int h)
  {
    celPcCameraCommon::SetRectangle (x, y, w, h);
  }
  virtual void SetPerspectiveCenter (float x, float y)
  {
    celPcCameraCommon::SetPerspectiveCenter (x, y);
  }
  virtual iCamera* GetCamera () const
  {
    return celPcCameraCommon::GetCamera ();
  }
  virtual iView* GetView () const
  {
    return celPcCameraCommon::GetView ();
  }
  virtual void SetClearZBuffer (bool flag)
  {
    celPcCameraCommon::SetClearZBuffer (flag);
  }
  virtual bool GetClearZBuffer () const
  {
    return celPcCameraCommon::GetClearZBuffer ();
  }
  virtual void SetClearScreen (bool flag)
  {
    celPcCameraCommon::SetClearScreen (flag);
  }
  virtual bool GetClearScreen () const
  {
    return celPcCameraCommon::GetClearScreen ();
  }
  virtual void DisableDistanceClipping ()
  {
    celPcCameraCommon::DisableDistanceClipping ();
  }
  virtual void EnableFixedDistanceClipping (float dist)
  {
    celPcCameraCommon::EnableFixedDistanceClipping (dist);
  }
  virtual void EnableAdaptiveDistanceClipping (float min_fps,
  	float max_fps, float min_dist)
  {
    celPcCameraCommon::EnableAdaptiveDistanceClipping (min_fps, max_fps,
    	min_dist);
  }
  virtual bool UseDistanceClipping () const
  {
    return celPcCameraCommon::UseDistanceClipping ();
  }
  virtual bool UseFixedDistanceClipping () const
  {
    return celPcCameraCommon::UseFixedDistanceClipping ();
  }
  virtual float GetFixedDistance () const
  {
    return celPcCameraCommon::GetFixedDistance ();
  }
  virtual float GetAdaptiveMinFPS () const
  {
    return celPcCameraCommon::GetAdaptiveMinFPS ();
  }
  virtual float GetAdaptiveMaxFPS () const
  {
    return celPcCameraCommon::GetAdaptiveMaxFPS ();
  }
  virtual float GetAdaptiveMinDistance () const
  {
    return celPcCameraCommon::GetAdaptiveMinDistance ();
  }
  virtual void SetAutoDraw (bool auto_draw)
  {
    celPcCameraCommon::SetAutoDraw (auto_draw);
  }
};

#endif // __CEL_PF_NEW_CAMERA_FACTORY__