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
#include "propclass/test.h"
#include "propclass/cameras/tracking.h"

struct iCollideSystem;
struct iObjectRegistry;
struct iMovable;
struct iCelEntity;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (TrackingCamera)

/**
 * This is a test property class.
 */
class celPcTrackingCamera : public scfImplementationExt2<celPcTrackingCamera, celPcCommon,
  iPcTrackingCamera, scfFakeInterface<iPcCameraMode> >
{
public:
  celPcTrackingCamera (iObjectRegistry* object_reg);
  virtual ~celPcTrackingCamera ();

  csPtr<iCelDataBuffer> Save ();
  bool Load (iCelDataBuffer* databuf);
  bool PerformActionIndexed (int idx, iCelParameterBlock* params, celData& ret);
  // support for tilt/pan direction properties
  bool SetPropertyIndexed (int idx, float f);
  bool GetPropertyIndexed (int idx, float &f);

  // --------------------------

  bool SetTargetEntity (const char* name);
  void SetTargetState (TargetState targetstate);
  //virtual void SetTargetMesh (const char* name);
  //virtual void SetTargetPosition (const csVector3 &pos);
  void SetTargetYOffset (float targetyoffset);
  float GetTargetYOffset () const;
  void SetTargetInterpolationTime (csTicks t);
  csTicks GetTargetInterpolationTime () const;

  void SetPanDirection (float pdir);
  float GetPanDirection () const;
  void SetPanSpeed (float pspeed);
  float GetPanSpeed () const;
  void SetPanAcceleration (float paccel);
  float GetPanAcceleration () const;

  void SetTiltDirection (float tdir);
  float GetTiltDirection () const;
  void SetTiltSpeed (float tspeed);
  float GetTiltSpeed () const;
  void SetTiltAcceleration (float taccel);
  float GetTiltAcceleration () const;

  TargetState GetTargetState ();

  void SetZoomOutCorrectionSpeed (float zoomspeed);
  float GetZoomOutCorrectionSpeed () const;

  void SetOffsetAngle (float angle);
  float GetOffsetAngle () const;
  void SetOffsetDistance (float dist);
  float GetOffsetDistance () const;
  void SetFollowSpringLength (float slen);
  float GetFollowSpringLength () const;
  void SetFollowMinimumSpringFactor (float smin);
  float SetFollowMinimumSpringFactor () const;
  bool DecideState ();

  bool ResetCamera ();

  // position, target, up
  const csVector3 &GetPosition ();
  const csVector3 &GetTarget ();
  const csVector3 &GetUp ();

private:
  // calculate spring force based on spring stretched length
  float SpringForce (const float movement);
  // pan camera around a target
  void PanAroundPlayer (const csVector3 &playpos, float elapsedsecs);
  // do your collision detection maagick!
  void FindCorrectedTransform (float elapsedsecs);
  // target settings was changed, so need to do a transition
  void TransitionTarget ();

  csVector3 pos, tar, up;
  csVector3 corrpos, corrtar;

  iMovable* parent;
  // the target we track if targetstate == TARGET_OBJ
  iMovable* tracktarget;
  TargetState targetstate;

  struct Accelerator
  {
    void Accelerate (int direction, float elapsedsecs);
    float topspeed;
    float speed, accel;
  } pan, tilt;
  // panning direction and speed
  float pandir;
  // tilt direction and speed
  float tiltdir;

  // for target object
  csVector3 cam_dir;

  // because you don't want to be looking at the targets feet
  float targetyoffset;
  // target is transitioning?
  bool in_tartransition;
  // time for target to interpolate to a new position
  csTicks tarintime;
  // current target transition between 0 and 1
  float currtartrans;
  // the old target position before the transition was started
  csVector3 prevtar;

  // offset for origin position. Described as elevation angle and distance outwards
  struct
  {
    float angle, dist;
  } posoff;
  // the in and out springs relaxed normal length
  float relaxspringlen;
  // the minimum value for calculated spring... to avoid stupid values
  float minspring;

  // -- collision detection related stuff
  // did we correct the camera last frame? used for interpolating outwards movement
  bool was_corrected;
  // speed of zoom out correction
  float zoomoutcorrspeed;

  // to compute elapsed time
  csRef<iVirtualClock> vc;
  // used for hitbeam in collision detection
  csRef<iCollideSystem> cdsys;

  // Has this camera been initialised yet?
  bool init_reset;

  // For actions.
  enum actionids
  {
    action_reset = 0
  };

  // For properties.
  enum propids
  {
    propid_pos = 0,
    propid_tar,
    propid_up,
    propid_pan_topspeed,
    propid_pan_currspeed,
    propid_pan_accel,
    propid_pan_dir,
    propid_tilt_topspeed,
    propid_tilt_currspeed,
    propid_tilt_accel,
    propid_tilt_dir,
    propid_taryoff,
    propid_tarintrans,
    propid_tarintime,
    propid_currtartrans,
    propid_posoff_angle,
    propid_posoff_dist,
    propid_spring_relaxlen,
    propid_spring_minlen,
    propid_zoomoutcorrspeed
  };
  static PropertyHolder propinfo;
};

#endif // __CEL_PF_TESTFACT__

