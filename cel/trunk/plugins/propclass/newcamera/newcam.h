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
  struct CameraState
  {
    csVector3 pos, dir, up;
  };

  csRef<iCollideSystem> cdsys;

  csRefArray<iCelCameraMode> cameramodes;

  size_t currmode;

  CameraState base;
  csReversibleTransform basetrans;
  iSector* basesector;
  CameraState currcam;

  // the minimum distance the camera is allowed to the player
  float minoffset;

  csWeakRef<iPcMesh> pcmesh;

  bool docolldet;
  // y offset for collision detection focus
  float collyfocusoff;
  // how much correction to do in collision detection
  float corrmult;
  // collision avoidance for player testing radius
  float avoidradsq;
  // vertical y offset for when camera nears player
  float avoidyoff;
  // avoidance interpolation is how smooth the player avoidance movement should be
  float avoidinter;

  bool in_transition;
  float transtime, currtrans;
  CameraState lastmode;

  // Has this camera been initialised yet?
  bool init_reset;

  void UpdateMeshVisibility ();

  /** Calculates an elastic vector based on an ideal vector and a current one.
   *  \param curr         The current vector value. This value gets updated.
   *  \param ideal        The ideal/target vector value.
   *  \param deltaTime    The change in time since last frame.
   *  \param springCoef   The spring coefficient to use in our calculations.
   */
  static csVector3 InterpolateVector (float i, const csVector3& curr, const csVector3& next);

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
  };
  static PropertyHolder propinfo;

public:
  celPcNewCamera (iObjectRegistry* object_reg);
  virtual ~celPcNewCamera ();

  csPtr<iCelDataBuffer> Save ()
  {
    return 0;
  }
  bool Load (iCelDataBuffer* databuf)
  {
    return true;
  }
  bool PerformActionIndexed (int, iCelParameterBlock* params,
  	celData& ret);
  bool SetPropertyIndexed (int, bool);
  bool GetPropertyIndexed (int, bool&);

  const csVector3& GetBasePos () const;
  const csVector3& GetBaseOrigin () const;
  const csVector3& GetBaseDir () const;
  const csVector3& GetBaseUp () const;
  const csReversibleTransform& GetBaseTrans () const;

  const csVector3& GetPos () const;
  const csVector3& GetOrigin () const;
  const csVector3& GetTarget () const;
  const csVector3& GetUp () const;

  void SetPositionOffset (const csVector3& offset);
  void SetTargetPositionOffset (const csVector3& offset);
  void SetTargetMinimumOffset (float minoff);
  void SetCameraPositionOffset (const csVector3& offset);

  void SetSpringCoefficient (float springCoef);
  float GetSpringCoefficient () const;
  void SetOriginSpringCoefficient (float springCoef);
  float GetOriginSpringCoefficient () const;
  void SetTargetSpringCoefficient (float springCoef);
  float GetTargetSpringCoefficient () const;
  void SetUpSpringCoefficient (float springCoef);
  float GetUpSpringCoefficient () const;

  bool DetectCollisions () const;
  void SetCollisionDetection (bool docolldet);
  bool GetCollisionDetection () const;
  void SetCollisionYFocusOffset (float yoff);
  float GetCollisionYFocusOffset () const;
  void SetCollisionCorrection (float corr);
  float GetCollisionCorrection () const;
  void SetCollisionAvoidanceRadiusSq (float radsq);
  float GetCollisionAvoidanceRadiusSq () const;
  void SetCollisionYAvoidance (float yavoid);
  float GetCollisionYAvoidance () const;
  void SetCollisionAvoidanceInterpolation (float aint);
  float GetCollisionAvoidanceInterpolation () const;

  void SetCollisionSpringCoefficient (float springCoef) {}
  float GetCollisionSpringCoefficient () const { return -1; }

  bool InCameraTransition () const;
  void SetTransitionSpringCoefficient (float springCoef);
  float GetTransitionSpringCoefficient () const;
  void SetTransitionCutoffDistance (float cutOffOriginDist,
  	float cutOffTargetDist);
  float GetTransitionCutoffPosDistance () const;
  float GetTransitionCutoffOriginDistance () const;
  float GetTransitionCutoffTargetDistance () const;
  void SetTransitionTime (float time);
  float GetTransitionTime () const;

  size_t AttachCameraMode (iCelCameraMode* mode);
  size_t AttachCameraMode (iPcNewCamera::CEL_CAMERA_MODE modetype);
  size_t GetCurrentCameraModeIndex () const;
  iCelCameraMode* GetCurrentCameraMode ();
  bool SetCurrentCameraMode (size_t modeindex);
  iCelCameraMode* GetCameraMode (int idx = -1);
  void NextCameraMode ();
  void PrevCameraMode ();

  void Draw ();
  void TickEveryFrame ();

  // get the transform corresponding to this camera
  const csOrthoTransform &GetTransform ();

  bool Reset ();

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
