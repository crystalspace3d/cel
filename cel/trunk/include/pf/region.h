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

#ifndef __CEL_PF_REGION__
#define __CEL_PF_REGION__

#include "cstypes.h"
#include "csgeom/vector3.h"
#include "csutil/scf.h"

struct iSector;

SCF_DECLARE_FAST_INTERFACE (iPcRegion)

SCF_VERSION (iPcRegion, 0, 0, 1);

/**
 * The representation of a region.
 */
struct iPcRegion : public iBase
{
  /**
   * Set the world file to use for this region.
   */
  virtual void SetWorldFile (const char* vfsdir, const char* name) = 0;
  /**
   * Get the world dir that is used for this region.
   */
  virtual const char* GetWorldDir () const = 0;
  /**
   * Get the world file that is used for this region.
   */
  virtual const char* GetWorldFile () const = 0;

  /**
   * Set the name of this region.
   */
  virtual void SetRegionName (const char* name) = 0;
  /**
   * Get the name of this region.
   */
  virtual const char* GetRegionName () const = 0;

  /**
   * Load the region.
   */
  virtual bool Load () = 0;

  /**
   * Unload the region.
   */
  virtual void Unload () = 0;

  /**
   * Get start sector.
   */
  virtual iSector* GetStartSector () = 0;

  /**
   * Get start position.
   */
  virtual csVector3 GetStartPosition () = 0;
};

#endif // __CEL_PF_REGION__

