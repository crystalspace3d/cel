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
#include "propclass/newcamera.h"
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
class iPcZoneManager;

/**
 * Factory for engine stuff.
 */
CEL_DECLARE_FACTORY(NewCamera)

/**
 * This is a camera property class.
 */
class celPcNewCamera : public celPcCommon, public iPcNewCamera
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

  csWeakRef<iPcMesh> pcmesh;
  iSector* lastActorSector;

  void UpdateMeshVisibility();

  void GetActorTransform(csReversibleTransform & 
      actor_trans, iSector *& actor_sector);

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
  virtual size_t AttachCameraMode(CEL_CAMERA_MODE mode);

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
};

#endif // __CEL_PF_NEW_CAMERA_FACTORY__

