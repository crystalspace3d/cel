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

#ifndef __CEL_THIRD_PERSON_CAMERA_MODE_FACTORY__
#define __CEL_THIRD_PERSON_CAMERA_MODE_FACTORY__

#include "plugins/propclass/newcamera/celcameramode.h"

class celThirdPersonCameraMode : public celCameraMode
{
private:
  csVector3 posOffset;
public:
  celThirdPersonCameraMode();
  virtual ~celThirdPersonCameraMode();

  /** Sets the position as an offset from the camera's base position.
   *   \offset  The offset from the camera's base position to the 
   *            desired position.
   */
  void SetPositionOffset(const csVector3 & offset);

  /** Informs the camera mode that it should compute the desired position,
   *  direction, up, etc. of the camera now.
   *  \return 	True on success.
   */
  virtual bool DecideCameraState(); 
};

#endif // __CEL_THIRD_PERSON_CAMERA_MODE_FACTORY__
