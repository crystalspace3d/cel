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
class iPcZoneManager;

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

  csArray<iCelCameraMode *> cameraModes;
  
  size_t currMode;

  csVector3 basePos;
  csVector3 baseDir;
  csVector3 baseUp;
  csVector3 basePosOffset;
  csReversibleTransform baseTrans;
  iSector * baseSector;
  csVector3 camPos, camDir, camUp;
  csVector3 lastIdealPos, lastIdealDir, lastIdealUp;
  csWeakRef<iPcMesh> pcmesh;
  iSector* lastActorSector;

  void UpdateMeshVisibility();

  void GetActorTransform();

  /** Calculates an elastic vector based on an ideal vector and a current one.
   *  \param curr         The current vector value.
   *  \param ideal        The ideal/target vector value.
   *  \param deltaTime    The change in time since last frame.
   *  \param springCoef   The spring coefficient to use in our calculations.
   *  \param newVec       A container to hold the new value of the vector.
   */
  static void CalcElasticVec(const csVector3 & curr, const csVector3 & ideal,
                      float deltaTime, float springCoef, csVector3 & newVec);

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
   * Gets the current direction of the camera.
   * \return    The current direction of the camera.
   */
  virtual const csVector3 & GetDir() const;

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
    virtual const csVector3 & GetDir() const
    {
      return scfParent->GetDir();
    }
    virtual const csVector3 & GetUp() const
    {
      return scfParent->GetUp();
    }
    virtual void SetPositionOffset(const csVector3 & offset)
    {
      scfParent->SetPositionOffset(offset);
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
  } scfiPcNewCamera, scfiPcCamera;
};

#endif // __CEL_PF_NEW_CAMERA_FACTORY__

