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
class celPcNewCamera : public celPcCameraCommon
{
private:
  csRef<iGraphics3D> g3d;
  csRef<iEngine> engine;
  csRef<iVirtualClock> vc;
  csRef<iView> view;
  csRef<iCollideSystem> cdsys;

  csArray<iCelCameraMode *> cameraModes;
  
  size_t currMode;

  csVector3 basePos;
  csVector3 baseDir;
  csVector3 baseUp;
  csVector3 basePosOffset;
  csReversibleTransform baseTrans;
  iSector * baseSector;
  csVector3 camPos, camTarget, camUp;
  csVector3 lastIdealPos, lastIdealTarget, lastIdealUp;
  csWeakRef<iPcMesh> pcmesh;
  iSector* lastActorSector;

  bool detectCollisions;
  float collisionSpringCoef;

  bool inTransition;
  float transitionSpringCoef;
  float transitionCutoffPosDist;
  float transitionCutoffTargetDist;

  void UpdateMeshVisibility();

  void GetActorTransform();

  /** Calculates an elastic vector based on an ideal vector and a current one.
   *  \param curr         The current vector value.
   *  \param ideal        The ideal/target vector value.
   *  \param deltaIdeal   The change in ideal value since last frame.
   *  \param deltaTime    The change in time since last frame.
   *  \param springCoef   The spring coefficient to use in our calculations.
   *  \param newVec       A container to hold the new value of the vector.
   */
  static void CalcElasticVec(const csVector3 & curr, const csVector3 & ideal,
      const csVector3 & deltaIdeal, float deltaTime, float springCoef, csVector3 & newVec);

public:
  celPcNewCamera(iObjectRegistry * object_reg);
  virtual ~celPcNewCamera();

  virtual const char * GetName() const
  {
    return "New Camera";
  }
  virtual csPtr<iCelDataBuffer> Save()
  {
    return 0;
  }
  virtual bool Load(iCelDataBuffer * databuf)
  {
    return true;
  }
  void PropertyClassesHaveChanged();

  /**
   * Gets the base position of the camera in world coordinates.
   * \return 	The base position of the camera in world coordinates.
   */
  virtual const csVector3 & GetBasePos() const;

  /**
   * Gets the base direction of the camera.
   * \return 	The base direction of the camera.
   */
  virtual const csVector3 & GetBaseDir() const;

  /** 
   * Gets the base up vector of the camera.
   * \return 	The base up vector of the camera.
   */
  virtual const csVector3 & GetBaseUp() const;
  
  /**
   * Gets the base transform of the camera.
   * \return	The base transform of the camera.
   */
  virtual const csReversibleTransform & GetBaseTrans() const;

  /**
   * Gets the current position of the camera.
   * \return    The current position of the camera.
   */
  virtual const csVector3 & GetPos() const;

  /**
   * Gets the current target of the camera.
   * \return    The current target of the camera.
   */
  virtual const csVector3 & GetTarget() const;

  /**
   * Gets the current up vector of the camera.
   * \return    The current up vector of the camera.
   */
  virtual const csVector3 & GetUp() const;

  /**
   * Sets the offset from the center of the mesh's iMovable to the position of
   * the camera.
   * \param offset the offset from the center of the mesh to the camera
   *        position.
   */
  virtual void SetPositionOffset(const csVector3 & offset);

  /**
   * Returns whether the camera will use collision detection to avoid moving through walls.
   * \return True if collision detection is enabled.
   */
  virtual bool DetectCollisions() const;

  /**
   * Sets whether the camera will use collision detection to avoid moving through walls.
   * \param detectCollisions True if the camera should detect collisions.
   */
  virtual void SetCollisionDetection(bool detectCollisions);

  /**
   * Sets the spring coefficient that will be used when a collision is detected.
   * \param springCoef The new spring coefficient.
   */
  virtual void SetCollisionSpringCoefficient(float springCoef);

  /**
   * Returns the spring coefficient that is used when a collision is detection.
   * \return The collision detection spring coefficient.
   */
  virtual float GetCollisionSpringCoefficient() const;

  /**
   * Determines whether the camera is currently in a transition from one camera
   * mode to another.
   * \return True if the camera is currently in a transition.
   */
  virtual bool InCameraTransition() const;

  /**
   * This controls the springyness of the transition to a new camera mode when
   * a new camera mode is selected.
   * \param springCoef The new spring coefficient of camera transitions.
   */
  virtual void SetTransitionSpringCoefficient(float springCoef);

  /**
   * This gets the springyness of the transition to a new camera mode when a new
   * camera mode is selected.
   * \return The spring coefficient of the camera transitions.
   */
  virtual float GetTransitionSpringCoefficient() const;

  /**
   * If the distance between the current camera position and the new camera mode
   * is within this cutoff distance, then the camera will cease to be in a transition
   * and be in the new camera mode.
   * \param cutOffPosDist The camera transition mode cutoff distance from position to position.
   * \param cutOffTargetDist The camera transition mode cutoff distance from target to target.
   */
  virtual void SetTransitionCutoffDistance(float cutOffPosDist, float cutOffTargetDist);

  /**
   * Grabs the camera transition cutoff distance from position to position between the camera 
   * and the camera mode.
   * \return The camera transition cutoff distance from target to target.
   */
  virtual float GetTransitionCutoffPosDistance() const;

  /**
   * Grabs the camera transition cutoff distance from target to target between the camera 
   * and the camera mode.
   * \return The camera transition cutoff distance from position to position.
   */
  virtual float GetTransitionCutoffTargetDistance() const;

  /** Attaches a camera mode to this camera.
   *  \param mode 	The camera mode to attach.
   *  \return 		The index that the camera mode was stored.
   */
  virtual size_t AttachCameraMode(iCelCameraMode * mode);

  /** Attaches a built-in camera mode to this camera.
   *  \param mode 	The id of the built-in camera mode to attach.
   *  \return 		A unique id for the attached camera mode.
   */
  virtual size_t AttachCameraMode(iPcNewCamera::CEL_CAMERA_MODE mode);

  /** Gets the index of the current camera mode.
   *  \return 	The index of the current camera mode.
   */
  virtual size_t GetCurrentCameraModeIndex() const;

  /** Gets the current camera mode.
   *  \return 	The current camera mode.
   */
  virtual iCelCameraMode * GetCurrentCameraMode();

  /** Sets the current camera mode.
   *  \param modeIndex 	The index of the current camera mode.
   *  \return 		True on successful camera mode change.
   */
  virtual bool SetCurrentCameraMode(size_t modeIndex);

  /** Sets the current camera mode to the next available mode.
   */
  virtual void NextCameraMode();

  /** Sets the current camera mode to the previous available mode.
   */
  virtual void PrevCameraMode();

  /**
   * Render. This will clear the screen then draw on top of it.
   */
  virtual void Draw();
  virtual void TickEveryFrame();
  
  void UpdateCamera ();
  int GetDrawFlags ();

  SCF_DECLARE_IBASE_EXT(celPcCommon);

  struct PcNewCamera : public iPcNewCamera
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcNewCamera);
    virtual bool SetRegion (iPcRegion* region, bool point = true,
        const char* name = 0)
    {
      return scfParent->SetRegion (region, point, name);
    }
    virtual bool SetZoneManager (iPcZoneManager* zonemgr, bool point,
        const char* regionname, const char* name = 0)
    {
      return scfParent->SetZoneManager (zonemgr, point, regionname, name);
    }
    virtual bool SetZoneManager (const char* entityname, bool point,
        const char* regionname, const char* name = 0)
    {
      return scfParent->SetZoneManager (entityname, point, regionname, name);
    }
    virtual void SetRectangle (int x, int y, int w, int h)
    {
      scfParent->SetRectangle (x, y, w, h);
    }
    virtual iCamera* GetCamera () const
    {
      return scfParent->GetCamera ();
    }
    virtual iView* GetView () const
    {
      return scfParent->GetView ();
    }
    virtual void SetClearZBuffer (bool flag)
    {
      scfParent->SetClearZBuffer (flag);
    }
    virtual bool GetClearZBuffer () const
    {
      return scfParent->GetClearZBuffer ();
    }
    virtual void SetClearScreen (bool flag)
    {
      scfParent->SetClearScreen (flag);
    }
    virtual bool GetClearScreen () const
    {
      return scfParent->GetClearScreen ();
    }
    virtual void DisableDistanceClipping ()
    {
      scfParent->DisableDistanceClipping ();
    }
    virtual void EnableFixedDistanceClipping (float dist)
    {
      scfParent->EnableFixedDistanceClipping (dist);
    }
    virtual void EnableAdaptiveDistanceClipping (float min_fps,
        float max_fps, float min_dist)
    {
      scfParent->EnableAdaptiveDistanceClipping (min_fps, max_fps, min_dist);
    }
    virtual bool UseDistanceClipping () const
    {
      return scfParent->UseDistanceClipping ();
    }
    virtual bool UseFixedDistanceClipping () const
    {
      return scfParent->UseFixedDistanceClipping ();
    }
    virtual float GetFixedDistance () const
    {
      return scfParent->GetFixedDistance ();
    }
    virtual float GetAdaptiveMinFPS () const
    {
      return scfParent->GetAdaptiveMinFPS ();
    }
    virtual float GetAdaptiveMaxFPS () const
    {
      return scfParent->GetAdaptiveMaxFPS ();
    }
    virtual float GetAdaptiveMinDistance () const
    {
      return scfParent->GetAdaptiveMinDistance ();
    }
    virtual void SetAutoDraw (bool auto_draw)
    {
      scfParent->SetAutoDraw (auto_draw);
    }
    virtual const csVector3 & GetBasePos() const
    {
      return scfParent->GetBasePos();
    }
    virtual const csVector3 & GetBaseDir() const
    {
      return scfParent->GetBaseDir();
    }
    virtual const csVector3 & GetBaseUp() const
    {
      return scfParent->GetBaseUp();
    }
    virtual const csReversibleTransform & GetBaseTrans() const
    {
      return scfParent->GetBaseTrans();
    }
    virtual const csVector3 & GetPos() const
    {
      return scfParent->GetPos();
    }
    virtual const csVector3 & GetTarget() const
    {
      return scfParent->GetTarget();
    }
    virtual const csVector3 & GetUp() const
    {
      return scfParent->GetUp();
    }
    virtual void SetPositionOffset(const csVector3 & offset)
    {
      scfParent->SetPositionOffset(offset);
    }
    virtual bool DetectCollisions() const
    {
      return scfParent->DetectCollisions();
    }
    virtual void SetCollisionDetection(bool detectCollisions)
    {
      scfParent->SetCollisionDetection(detectCollisions);
    }
    virtual void SetCollisionSpringCoefficient(float springCoef)
    {
      scfParent->SetCollisionSpringCoefficient(springCoef);
    }
    virtual float GetCollisionSpringCoefficient() const
    {
      return scfParent->GetCollisionSpringCoefficient();
    }
    virtual bool InCameraTransition() const
    {
      return scfParent->InCameraTransition();
    }
    virtual void SetTransitionSpringCoefficient(float springCoef)
    {
      scfParent->SetTransitionSpringCoefficient(springCoef);
    }
    virtual float GetTransitionSpringCoefficient() const
    {
      return scfParent->GetTransitionSpringCoefficient();
    }
    virtual void SetTransitionCutoffDistance(float cutOffPosDist, float cutOffTargetDist)
    {
      scfParent->SetTransitionCutoffDistance(cutOffPosDist, cutOffTargetDist);
    }
    virtual float GetTransitionCutoffPosDistance() const
    {
      return scfParent->GetTransitionCutoffPosDistance();
    }
    virtual float GetTransitionCutoffTargetDistance() const
    {
      return scfParent->GetTransitionCutoffTargetDistance();
    }
    virtual size_t AttachCameraMode(iCelCameraMode * mode)
    {
      return scfParent->AttachCameraMode(mode);
    }
    virtual size_t AttachCameraMode(CEL_CAMERA_MODE mode)
    {
      return scfParent->AttachCameraMode(mode);
    }
    virtual size_t GetCurrentCameraModeIndex() const
    {
      return scfParent->GetCurrentCameraModeIndex();
    }
    virtual iCelCameraMode * GetCurrentCameraMode()
    {
      return scfParent->GetCurrentCameraMode();
    }
    virtual bool SetCurrentCameraMode(size_t modeIndex)
    {
      return scfParent->SetCurrentCameraMode(modeIndex);
    }
    virtual void NextCameraMode()
    {
      scfParent->NextCameraMode();
    }
    virtual void PrevCameraMode()
    {
      scfParent->PrevCameraMode();
    }
    virtual void Draw()
    {
      scfParent->Draw();
    }
    virtual void UpdateCamera ()
    {
      scfParent->UpdateCamera ();
    }
    virtual int GetDrawFlags ()
    {
      return scfParent->GetDrawFlags ();
    }
  } scfiPcNewCamera, scfiPcCamera;
};

#endif // __CEL_PF_NEW_CAMERA_FACTORY__

