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

#ifndef __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__
#define __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__

#include "plugins/propclass/newcamera/celcameramode.h"

#include "csgeom/transfrm.h"

struct iMovable;

namespace celCameraMode
{

class LaraTrack : public scfImplementation2<LaraTrack, iTrackCameraMode,
  scfFakeInterface<iCelCameraMode> >, public celCameraMode
{
private:
  const csVector3 &GetAnchorPosition ();
  const csVector3 &GetAnchorDirection ();
  // this function should absolutely never be called unless
  // targetstate == TARGET_OBJ
  const csVector3 &GetTargetPosition ();

  // the target we track if targetstate == TARGET_OBJ
  iMovable* tracktarget;
  TargetState targetstate;
  // because you don't want to be looking at the targets feet
  float targetyoffset;

  csVector3 posoffset;
  // we store a local transform we keep updated
  csReversibleTransform camtrans;

  // Has this camera been initialised yet?
  bool init_reset;
public:
  LaraTrack (iBase* p = 0);
  virtual ~LaraTrack ();

  virtual bool SetTargetEntity (const char* name);
  virtual void SetTargetState (TargetState targetstate);
  //virtual void SetTargetMesh (const char* name);
  //virtual void SetTargetPosition (const csVector3 &pos);

  virtual void SetTargetYOffset (float targetyoffset);

  virtual TargetState GetTargetState ();

  virtual void SetPositionOffset(const csVector3 & offset);
  virtual bool DrawAttachedMesh() const;
  virtual bool DecideCameraState ();

  virtual bool ResetCamera ();

  virtual void SetParentCamera (iPcNewCamera * camera)
  {
    celCameraMode::SetParentCamera (camera);
  }
  virtual bool UseSpringPos () const
  {
    return celCameraMode::UseSpringPos ();
  }
  virtual bool UseSpringTarget () const
  {
    return celCameraMode::UseSpringTarget ();
  }
  virtual bool UseSpringUp () const
  {
    return celCameraMode::UseSpringUp ();
  }
  virtual bool AllowCollisionDetection () const
  {
    return celCameraMode::AllowCollisionDetection ();
  }
  virtual float GetSpringCoefficient () const
  {
    return celCameraMode::GetSpringCoefficient ();
  }
  virtual const csVector3 &GetPosition () const
  {
    return celCameraMode::GetPosition ();
  }
  virtual const csVector3 &GetTarget () const
  {
    return celCameraMode::GetTarget ();
  }
  virtual const csVector3 &GetUp () const
  {
    return celCameraMode::GetUp ();
  }
};

}

#endif // __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__
