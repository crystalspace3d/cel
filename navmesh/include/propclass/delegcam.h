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

#ifndef __CEL_PF_DELEGATECAMERA__
#define __CEL_PF_DELEGATECAMERA__

// CS Includes
#include "cstypes.h"
#include "csutil/scf.h"

// CEL Includes
#include "celtool/camera.h"

struct iPcCameraMode : public virtual iBase
{
  SCF_INTERFACE (iPcCameraMode, 0, 0, 1);

  /**
   * Informs the camera mode that it should compute the desired position,
   * target and up of the camera now.
   * \return True on success.
   */
  virtual bool DecideState () = 0;

  /**
   * Gets the desired camera position.
   * \return The desired camera position.
   */
  virtual const csVector3 &GetPosition () = 0;

  /**
   * Gets the desired camera target.
   * \return The desired camera target.
   */
  virtual const csVector3 &GetTarget () = 0;

  /**
   * Gets the desired camera up vector.
   * \return The desired camera up vector.
   */
  virtual const csVector3 &GetUp () = 0;
};

struct iPcDelegateCamera : public virtual iPcCamera
{
  SCF_INTERFACE (iPcDelegateCamera, 0, 0, 2);

  virtual void SetCurrentMode (iPcCameraMode* mode) = 0;
  virtual iPcCameraMode* GetCurrentMode () const = 0;
  virtual void SetTransitionTime (csTicks t) = 0;
  virtual csTicks GetTransitionTime () const = 0;
  virtual float GetContinousTransitionSpeed () const = 0;
  virtual void SetContinousTransitionSpeed (float s) = 0;
};

#endif // __CEL_PF_DELEGATECAMERA__
