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

#ifndef __CEL_PF_MESHSEL__
#define __CEL_PF_MESHSEL__

#include "cstypes.h"
#include "csutil/scf.h"

SCF_DECLARE_FAST_INTERFACE (iPcMeshSelect)

struct iCamera;

SCF_VERSION (iPcMeshSelect, 0, 0, 1);

/**
 * This is a property class for selecting meshes.
 */
struct iPcMeshSelect : public iBase
{
  /**
   * Set the camera to use for mesh selection.
   */
  virtual void SetCamera (iCamera* camera) = 0;

  /**
   * Set global/local selection. With local selection
   * the Behaviour will only get messages when it's
   * own mesh entity is selected (default). With global
   * selection the Behaviour will get messages for selection
   * of all mesh entities.
   */
  virtual void SetGlobalSelection (bool glob) = 0;

  /**
   * Get value of global/local selection.
   */
  virtual bool HasGlobalSelection () const = 0;
};

#endif // __CEL_PF_MESHSEL__

