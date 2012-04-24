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
    csVector3 pos, tar, up;
  };

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

  // the minimum distance the camera is allowed to the player
  float minoffset;

  csWeakRef<iPcMesh> pcmesh;
  iSector* lastActorSector;

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

  bool inTransition;
  bool usecrappyspring;
  float transitionSpringCoef;
  float transitionCutoffOriginDist;
  float transitionCutoffTargetDist;
  float transtime, currtrans;
  CameraState lastmode;

  // Has this camera been initialised yet?
  bool init_reset;

  void UpdateMeshVisibility ();

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
    propid_colldet_yfocusoff,
    propid_colldet_corrmult,
    propid_colldet_avoid_radsq,
    propid_colldet_avoid_yoff,
    propid_colldet_avoid_inter,
    propid_offset,
    propid_offset_origin,
    propid_offset_target,
    propid_offset_min,
    propid_spring,
    propid_spring_origin,
    propid_spring_target,
    propid_spring_up,
    propid_trans_spring,
    propid_trans_cutofforigin,
    propid_trans_cutofftarget,
    propid_trans_time,
    propid_trans_on
  };
  static PropertyHolder propinfo;

public:
  celPcNewCamera (iObjectRegistry* object_reg);
  virtual ~celPcNewCamera ();

  bool PerformActionIndexed (int, iCelParameterBlock* params,
  	celData& ret);
  bool SetPropertyIndexed (int, bool);
  bool GetPropertyIndexed (int, bool&);
  bool SetPropertyIndexed (int, float);
  bool GetPropertyIndexed (int, float&);

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
  void SetCollisionDetection (bool detectCollisions);
  bool GetCollisionDetection () const;
  void SetCollisionSpringCoefficient (float springCoef);
  float GetCollisionSpringCoefficient () const;
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
  iCelCameraMode* GetCameraMode (int idx = -1);
  bool SetCurrentCameraMode (size_t modeIndex);
  void NextCameraMode ();
  void PrevCameraMode ();

  void Draw ();
  void TickEveryFrame ();

  // get the transform corresponding to this camera
  const csOrthoTransform &GetTransform ();

  bool Reset ();

  void UpdateCamera ();
  int GetDrawFlags ();

  bool SetRegion (iPcRegion* region, bool point = true,
  	const char* name = 0)
  {
    return celPcCameraCommon::SetRegion (region, point, name);
  }
  bool SetZoneManager (iPcZoneManager* zonemgr, bool point,
  	const char* regionname, const char* name = 0)
  {
    return celPcCameraCommon::SetZoneManager (zonemgr, point, regionname,
    	name);
  }
  bool SetZoneManager (const char* entityname, bool point,
  	const char* regionname, const char* name = 0)
  {
    return celPcCameraCommon::SetZoneManager (entityname, point, regionname,
    	name);
  }
  void SetRectangle (int x, int y, int w, int h)
  {
    celPcCameraCommon::SetRectangle (x, y, w, h);
  }
  void SetPerspectiveCenter (float x, float y)
  {
    celPcCameraCommon::SetPerspectiveCenter (x, y);
  }
  iCamera* GetCamera () const
  {
    return celPcCameraCommon::GetCamera ();
  }
  iView* GetView () const
  {
    return celPcCameraCommon::GetView ();
  }
  void SetClearZBuffer (bool flag)
  {
  }
  bool GetClearZBuffer () const
  {
    return false;
  }
  void SetClearScreen (bool flag)
  {
  }
  bool GetClearScreen () const
  {
    return false;
  }
  void DisableDistanceClipping ()
  {
    celPcCameraCommon::DisableDistanceClipping ();
  }
  void EnableFixedDistanceClipping (float dist)
  {
    celPcCameraCommon::EnableFixedDistanceClipping (dist);
  }
  void EnableAdaptiveDistanceClipping (float min_fps,
  	float max_fps, float min_dist)
  {
    celPcCameraCommon::EnableAdaptiveDistanceClipping (min_fps, max_fps,
    	min_dist);
  }
  bool UseDistanceClipping () const
  {
    return celPcCameraCommon::UseDistanceClipping ();
  }
  bool UseFixedDistanceClipping () const
  {
    return celPcCameraCommon::UseFixedDistanceClipping ();
  }
  float GetFixedDistance () const
  {
    return celPcCameraCommon::GetFixedDistance ();
  }
  float GetAdaptiveMinFPS () const
  {
    return celPcCameraCommon::GetAdaptiveMinFPS ();
  }
  float GetAdaptiveMaxFPS () const
  {
    return celPcCameraCommon::GetAdaptiveMaxFPS ();
  }
  float GetAdaptiveMinDistance () const
  {
    return celPcCameraCommon::GetAdaptiveMinDistance ();
  }
  void SetAutoDraw (bool auto_draw)
  {
    celPcCameraCommon::SetAutoDraw (auto_draw);
  }
};

#endif // __CEL_PF_NEW_CAMERA_FACTORY__
