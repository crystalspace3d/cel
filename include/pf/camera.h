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

#ifndef __CEL_PF_CAMERA__
#define __CEL_PF_CAMERA__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCamera;
struct iView;
struct iPcRegion;

SCF_DECLARE_FAST_INTERFACE (iPcCamera)

SCF_VERSION (iPcCamera, 0, 0, 1);

/**
 * The representation of a camera.
 */
struct iPcCamera : public iBase
{
  /**
   * Camera modes
   */
  enum CameraMode
  {
    freelook = 0,
    follow
  };

  /**
   * Set Camera to a Region
   */
  virtual bool SetRegion (iPcRegion* region, bool point = true,
      const char* name = NULL) = 0;

  /**
   * Set camera mode
   */
  virtual bool SetMode (CameraMode m, bool use_cd = true) = 0;

  /**
   * Set the view rectangle to use on screen.
   */
  virtual void SetRectangle (int x, int y, int w, int h) = 0;

  /**
   * Get the camera.
   */
  virtual iCamera* GetCamera () const = 0;

  /**
   * Get the view.
   */
  virtual iView* GetView () const = 0;
};

#endif // __CEL_PF_CAMERA__

