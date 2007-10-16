/*
    Crystal Space Entity Layer
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

#ifndef __CEL_ISOMETRIC_CAMERA_MODE_FACTORY__
#define __CEL_ISOMETRIC_CAMERA_MODE_FACTORY__

#include "plugins/propclass/newcamera/celcameramode.h"

namespace celCameraMode
{

class Isometric : public scfImplementation1<Isometric, iCelCameraMode>,
	public celCameraMode
{
public:
  Isometric ();
  virtual ~Isometric ();

  /**
   * Decides if this camera mode should use spring physics for the camera's
   * position.
   * \return True if this camera mode uses spring physics.
   */
  virtual bool UseSpringPos () const;

  /**
   * Decides if this camera mode should use spring physics for the camera's
   * origin.
   * \return True if this camera mode uses spring physics.
   */
  virtual bool UseSpringOrigin () const;

  /**
   * Decides if this camera mode should use spring physics for the camera's
   * target.
   * \return True if this camera mode uses spring physics.
   */
  virtual bool UseSpringTarget () const;

  /**
   * Decides if this camera mode should use spring physics for the camera's
   * up vector.
   * \return True if this camera mode uses spring physics.
   */
  virtual bool UseSpringUp () const;

  /**
   * Decides if the mesh the camera is attached to should be drawn or not in
   * this mode.
   * \return True if the attached mesh should be drawn.
   */
  virtual bool DrawAttachedMesh () const;

  /**
   * Informs the camera mode that it should compute the desired position,
   * target, up, etc. of the camera now.
   * \return True on success.
   */
  virtual bool DecideCameraState ();

  /**
   * Gets the desired camera position.
   * \return The desired camera position.
   */
  virtual const csVector3& GetPosition () const;

  /**
   * Gets the desired camera origin.
   * \return The desired camera origin.
   */
  virtual const csVector3& GetOrigin () const;

  /**
   * Gets the desired camera target.
   * \return The desired camera target.
   */
  virtual const csVector3& GetTarget () const;

  /**
   * Gets the desired camera up vector.
   * \return The desired camera up vector.
   */
  virtual const csVector3& GetUp () const;

  virtual void SetParentCamera (iPcNewCamera* camera)
  {
    celCameraMode::SetParentCamera (camera);
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
};

}

#endif // __CEL_ISOMETRIC_CAMERA_MODE_FACTORY__
