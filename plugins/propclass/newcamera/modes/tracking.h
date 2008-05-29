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

#include "plugins/propclass/newcamera/celcameramode.h"

#include "csgeom/transfrm.h"
#include "iutil/virtclk.h"

struct iMovable;
struct iCelPlLayer;

namespace celCameraMode
{

class Tracking : public scfImplementation2<Tracking, iPcmNewCamera::Tracking,
  scfFakeInterface<iPcmNewCamera::General> >, public celCameraMode
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
  // panning direction and speed
  PanningDirection pandir;
  float panspeed;
  // because you don't want to be looking at the targets feet
  float targetyoffset;
  // offset for origin position. Usually only y and z is used.
  csVector3 posoffset;

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
  void SetPanningSpeed (float pspeed);

  TargetState GetTargetState ();

  void SetPositionOffset (const csVector3 & offset);
  bool DrawAttachedMesh () const;
  bool DecideCameraState ();

  bool ResetCamera ();

  void SetParentCamera (iPcNewCamera * camera)
  {
    celCameraMode::SetParentCamera (camera);
  }
  bool UseSpringPos () const
  {
    return celCameraMode::UseSpringPos ();
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
    return celCameraMode::AllowCollisionDetection ();
  }
  bool GetCollisionDetection () const
  {
    return celCameraMode::GetCollisionDetection ();
  }
  float GetSpringCoefficient () const
  {
    return celCameraMode::GetSpringCoefficient ();
  }
  void SetSpringCoefficient (float s)
  {
    return celCameraMode::SetSpringCoefficient (s);
  }
  float GetOriginSpringCoefficient () const
  {
    return celCameraMode::GetOriginSpringCoefficient ();
  }
  void SetOriginSpringCoefficient (float s)
  {
    return celCameraMode::SetOriginSpringCoefficient (s);
  }
  float GetTargetSpringCoefficient () const
  {
    return celCameraMode::GetTargetSpringCoefficient ();
  }
  void SetTargetSpringCoefficient (float s)
  {
    return celCameraMode::SetTargetSpringCoefficient (s);
  }
  float GetUpSpringCoefficient () const
  {
    return celCameraMode::GetUpSpringCoefficient ();
  }
  void SetUpSpringCoefficient (float s)
  {
    return celCameraMode::SetUpSpringCoefficient (s);
  }
  const csVector3 &GetPosition () const
  {
    return celCameraMode::GetPosition ();
  }
  const csVector3& GetOrigin () const
  {
    return celCameraMode::GetOrigin ();
  }
  const csVector3 &GetTarget () const
  {
    return celCameraMode::GetTarget ();
  }
  const csVector3 &GetUp () const
  {
    return celCameraMode::GetUp ();
  }
};

}

#endif // __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__
