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
  // because you don't want to be looking at the targets feet
  float targetyoffset;
  // offset for origin position. Usually only y and z is used.
  csVector3 posoffset;

  // used to lookup entities
  csWeakRef<iCelPlLayer> pl;

  // Has this camera been initialised yet?
  bool init_reset;
public:
  Tracking (iBase* p);
  Tracking (csWeakRef<iCelPlLayer> pl);
  virtual ~Tracking ();

  virtual bool SetTargetEntity (const char* name);
  virtual void SetTargetState (TargetState targetstate);
  //virtual void SetTargetMesh (const char* name);
  //virtual void SetTargetPosition (const csVector3 &pos);

  virtual void SetTargetYOffset (float targetyoffset);

  virtual TargetState GetTargetState ();

  virtual void SetPositionOffset (const csVector3 & offset);
  virtual bool DrawAttachedMesh () const;
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
  virtual bool UseSpringOrigin () const
  {
    return celCameraMode::UseSpringOrigin ();
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
  virtual bool GetCollisionDetection () const
  {
    return celCameraMode::GetCollisionDetection ();
  }
  virtual float GetSpringCoefficient () const
  {
    return celCameraMode::GetSpringCoefficient ();
  }
  virtual void SetSpringCoefficient (float s)
  {
    return celCameraMode::SetSpringCoefficient (s);
  }
  virtual float GetOriginSpringCoefficient () const
  {
    return celCameraMode::GetOriginSpringCoefficient ();
  }
  virtual void SetOriginSpringCoefficient (float s)
  {
    return celCameraMode::SetOriginSpringCoefficient (s);
  }
  virtual float GetTargetSpringCoefficient () const
  {
    return celCameraMode::GetTargetSpringCoefficient ();
  }
  virtual void SetTargetSpringCoefficient (float s)
  {
    return celCameraMode::SetTargetSpringCoefficient (s);
  }
  virtual float GetUpSpringCoefficient () const
  {
    return celCameraMode::GetUpSpringCoefficient ();
  }
  virtual void SetUpSpringCoefficient (float s)
  {
    return celCameraMode::SetUpSpringCoefficient (s);
  }
  virtual const csVector3 &GetPosition () const
  {
    return celCameraMode::GetPosition ();
  }
  virtual const csVector3& GetOrigin () const
  {
    return celCameraMode::GetOrigin ();
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
