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

#ifndef __CEL_PF_TESTFACT__
#define __CEL_PF_TESTFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/delegcam.h"

struct iCelEntity;
struct iObjectRegistry;
struct iMovable;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (DelegateCamera)

/**
 * This is a test property class.
 */
class celPcDelegateCamera : public scfImplementationExt2<celPcDelegateCamera, celPcCameraCommon,
  iPcDelegateCamera, scfFakeInterface<iPcCamera> >
{
public:
  celPcDelegateCamera (iObjectRegistry* object_reg);
  virtual ~celPcDelegateCamera ();

  bool PerformActionIndexed (int idx, iCelParameterBlock* params, celData& ret);

  void SetCurrentMode (iPcCameraMode* mode);
  iPcCameraMode* GetCurrentMode () const;
  void SetTransitionTime (csTicks t);
  csTicks GetTransitionTime () const;

  void Draw ();
  void UpdateCamera ();
  int GetDrawFlags ();

  // crap needed by celtool camera ------------------
  bool SetRegion (iPcRegion* region, bool point = true, const char* name = 0)
  {
    return celPcCameraCommon::SetRegion (region, point, name);
  }
  bool SetZoneManager (iPcZoneManager* zonemgr, bool point,
    const char* regionname, const char* name = 0)
  {
    return celPcCameraCommon::SetZoneManager (zonemgr, point, regionname, name);
  }
  bool SetZoneManager (const char* entityname, bool point,
    const char* regionname, const char* name = 0)
  {
    return celPcCameraCommon::SetZoneManager (entityname, point, regionname, name);
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
    celPcCameraCommon::EnableAdaptiveDistanceClipping (min_fps, max_fps, min_dist);
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

  virtual float GetContinousTransitionSpeed () const
  {
    return continuous_transition_speed;
  }
  virtual void SetContinousTransitionSpeed (float s)
  {
    continuous_transition_speed = s;
  }

private:
  // currently transitioning to new mode?
  bool in_transition;
  // time to transition to a new mode
  csTicks transtime;
  // current transition between 0 and 1
  float currtrans;

  struct CameraDescription
  {
    csVector3 pos, tar, up;
  } prev, curr;

  // Speed for continuous transition.
  float continuous_transition_speed;
  // 'last' is used for continuous transitions.
  struct CameraDescription last;

  csRef<iPcCameraMode> currmode;

  // used just for reading the current active sector
  iMovable* player;

  // For action arguments
  static csStringID id_pclass;

  // For actions.
  enum actionids
  {
    action_setcurrmode = 0
  };

  // For properties.
  enum propids
  {
    propid_trans_in = 0,
    propid_trans_time,
    propid_trans_step,
    propid_prev_pos,
    propid_prev_tar,
    propid_prev_up,
    propid_pos,
    propid_tar,
    propid_up,
    propid_trans_speed,
  };
  static PropertyHolder propinfo;
};

#endif // __CEL_PF_TESTFACT__

