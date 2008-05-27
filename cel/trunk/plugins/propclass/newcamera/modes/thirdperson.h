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

#ifndef __CEL_THIRD_PERSON_CAMERA_MODE_FACTORY__
#define __CEL_THIRD_PERSON_CAMERA_MODE_FACTORY__

#include "plugins/propclass/newcamera/celcameramode.h"

namespace celCameraMode
{

class ThirdPerson : public scfImplementation2<ThirdPerson, iPcmNewCamera::ThirdPerson,
  scfFakeInterface<iPcmNewCamera::General> >, public celCameraMode
{
private:
  csVector3 posoff;
  float targetyoff;
public:
  ThirdPerson ();
  ~ThirdPerson ();

  void SetTargetYOffset (float targetyoffset);
  void SetPositionOffset (const csVector3 & offset);

  // deprecated
  bool UseSpringPos () const;
  bool UseSpringOrigin () const;
  bool UseSpringTarget () const;
  bool UseSpringUp () const;
  bool DrawAttachedMesh () const;
  bool DecideCameraState ();

  void SetParentCamera (iPcNewCamera * camera)
  {
    celCameraMode::SetParentCamera (camera);
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
  const csVector3& GetPosition () const
  {
    return celCameraMode::GetPosition ();
  }
  const csVector3& GetOrigin () const
  {
    return celCameraMode::GetOrigin ();
  }
  const csVector3& GetTarget () const
  {
    return celCameraMode::GetTarget ();
  }
  const csVector3& GetUp () const
  {
    return celCameraMode::GetUp ();
  }
};

}

#endif // __CEL_THIRD_PERSON_CAMERA_MODE_FACTORY__
