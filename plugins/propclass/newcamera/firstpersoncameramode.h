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

#ifndef __CEL_FIRST_PERSON_CAMERA_MODE_FACTORY__
#define __CEL_FIRST_PERSON_CAMERA_MODE_FACTORY__

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

class celFirstPersonCameraMode : public iCelCameraMode
{
private:
  csVector3 posOffset;
  iPcNewCamera * parentCamera;

public:
  celFirstPersonCameraMode();
  virtual ~celFirstPersonCameraMode();

  /** Tells the camera mode what camera has it attached.
   *  \param camera 	The parent camera.
   */
  virtual void SetParentCamera(iPcNewCamera * camera);

  /** Decides if this camera mode should use spring physics or not.
   *  \return 	True if this camera mode uses spring physics.
   */
  virtual bool UseSpringPhysics() const;
  
  /** 
   * Decides if the mesh the camera is attached to should be drawn or not in
   * this mode.
   * \return 	True if the attached mesh should be drawn.
   */
  virtual bool DrawAttachedMesh() const;

  /** Gets the spring coefficient to use for the spring physics.
   *  \return 	The spring coefficient of this camera mode.
   */
  virtual float GetSpringCoefficient() const;

  /** Gets the spring length to use for the spring physics.
   *  \return 	The spring length of this camera mode.
   */
  virtual float GetSpringLength() const;

  /** Gets the inertial dampening coefficient to use for the spring physics.
   *  \return 	The inertial dampening coefficient of this camera.
   */
  virtual float GetInertialDampeningCoefficient() const;
  
  /** Gets the desired camera position as an offset of the player's position.
   *  \return 	The desired camera position as an offset of the player's
   *  		position.
   */
  virtual const csVector3 & GetPositionOffset() const;

  /** Gets the desired camera direction.
   *  \return 	The desired camera direction.
   */
  virtual const csVector3 & GetDirection() const;
  
  /** Gets the desired camera up vector.
   *  \return 	The desired camera up vector.
   */
  virtual const csVector3 & GetUp() const;

  /** Informs the camera mode that it should compute the desired position,
   *  direction, up, etc. of the camera now.
   *  \return 	True on success.
   */
  virtual bool DecideCameraState(); 
};

#endif // __CEL_FIRST_PERSON_CAMERA_MODE_FACTORY__
