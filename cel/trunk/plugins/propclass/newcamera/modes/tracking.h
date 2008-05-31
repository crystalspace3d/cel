/*
    Crystal Space Entity Layer
    Copyright (C) 2007 by Amir Taaki

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

#ifndef __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__
#define __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__

// CS Includes
#include "csgeom/transfrm.h"
#include "iutil/virtclk.h"

// CEL Includes
#include "propclass/newcamera.h"

struct iMovable;
struct iCelPlLayer;

namespace celCameraMode
{

class Tracking : public scfImplementation2<Tracking, iPcmNewCamera::Tracking,
  scfFakeInterface<iPcmNewCamera::General> >
{
private:
  // calculate spring force based on spring stretched length
  float SpringForce (const float movement);
  // pan camera around a target
  void PanAroundPlayer (const csVector3 &playpos);
  // do your collision detection maagick!
  void FindCorrectedTransform () {};

  const csVector3 &GetAnchorPosition ();
  const csVector3 &GetAnchorDirection ();
  // this function should absolutely never be called unless
  // targetstate == TARGET_OBJ
  const csVector3 &GetTargetPosition ();

  iPcNewCamera* parent;
  csVector3 origin, target, up;
  csVector3 corrorigin, corrtarget;

  // the target we track if targetstate == TARGET_OBJ
  iMovable* tracktarget;
  TargetState targetstate;
  // panning direction and speed
  PanningDirection pandir;
  float panaccel, panspeed;
  float currpanspeed;   // current panning speed
  // because you don't want to be looking at the targets feet
  float targetyoffset;
  // offset for origin position. Usually only y and z is used.
  csVector3 posoffset;
  // the in and out springs relaxed normal length
  float relaxspringlen;
  // the minimum value for calculated spring... to avoid stupid values
  float minspring;

  // used to lookup entities
  csWeakRef<iCelPlLayer> pl;
  // to compute elapsed time
  csRef<iVirtualClock> vc;

  // Has this camera been initialised yet?
  bool init_reset;
public:
  Tracking (iBase* p);
  Tracking (iCelPlLayer* pl, iVirtualClock* vc);
  virtual ~Tracking ();

  bool SetTargetEntity (const char* name);
  void SetTargetState (TargetState targetstate);
  //virtual void SetTargetMesh (const char* name);
  //virtual void SetTargetPosition (const csVector3 &pos);

  void SetTargetYOffset (float targetyoffset);
  void Pan (PanningDirection pdir);
  PanningDirection GetPanDirection () const;
  void SetPanSpeed (float pspeed);
  float GetPanSpeed () const;
  void SetPanAcceleration (float paccel);
  float GetPanAcceleration () const;

  TargetState GetTargetState ();

  void SetPositionOffset (const csVector3 &offset);
  const csVector3 &GetPositionOffset () const;
  void SetFollowSpringLength (float slen);
  float GetFollowSpringLength () const;
  void SetFollowMinimumSpringFactor (float smin);
  float SetFollowMinimumSpringFactor () const;
  bool DrawAttachedMesh () const;
  bool DecideCameraState ();

  bool ResetCamera ();

  void SetParentCamera (iPcNewCamera* camera)
  {
    parent = camera;;
  }
  bool UseSpringPos () const
  {
    return false;
  }
  bool UseSpringOrigin () const
  {
    return false;
  }
  bool UseSpringTarget () const
  {
    return false;
  }
  bool UseSpringUp () const
  {
    return false;
  }
  bool AllowCollisionDetection () const
  {
    return true;
  }
  bool GetCollisionDetection () const
  {
    return true;
  }
  float GetSpringCoefficient () const
  {
    return false;
  }
  void SetSpringCoefficient (float s)
  {
  }
  float GetOriginSpringCoefficient () const
  {
    return 0.0f;
  }
  void SetOriginSpringCoefficient (float s)
  {
  }
  float GetTargetSpringCoefficient () const
  {
    return 0.0f;
  }
  void SetTargetSpringCoefficient (float s)
  {
  }
  float GetUpSpringCoefficient () const
  {
    return 0.0f;
  }
  void SetUpSpringCoefficient (float s)
  {
  }
  const csVector3 &GetPosition () const
  {
    return corrorigin;
  }
  const csVector3& GetOrigin () const
  {
    return corrorigin;
  }
  const csVector3 &GetTarget () const
  {
    return corrtarget;
  }
  const csVector3 &GetUp () const
  {
    return up;
  }
};

}

#endif // __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__
