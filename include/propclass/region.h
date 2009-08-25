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

#ifndef __CEL_PF_REGION__
#define __CEL_PF_REGION__

#include "cstypes.h"
#include "csgeom/vector3.h"
#include "csutil/scf.h"

struct iSector;
struct iPcCamera;
struct iCollection;

/**
 * The representation of a region.
 *
 * This property class supports the following actions:
 * - Load: no parameters.
 * - Unload: no parameters.
 *
 * This property class supports the following properties:
 * - worlddir (string, read/write): the VFS directory of the world file.
 * - worldfile (string, read/write): the world filename.
 * - regionname (string, read/write): region name.
 */
struct iPcRegion : public virtual iBase
{
  SCF_INTERFACE (iPcRegion, 0, 1, 1);

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
   * Create empty sector. Use this instead of SetWorldFile() if you want
   * the region to have a single empty sector. The given name is the name
   * of the sector.
   * Note! In the current implementation you can only call this function
   * once for a given region!
   */
  virtual void CreateEmptySector (const char* name) = 0;

  /**
   * Load the region. This will create the empty sector if CreateEmptySector()
   * is used.
   */
  virtual bool Load () = 0;

  /**
   * Unload the region.
   */
  virtual void Unload () = 0;

  /**
   * Returns a sector
   */
  virtual iSector* FindSector (const char* sectorname) = 0;

  /**
   * Get start sector. The given name is the name
   * of the starting position to use. If 0 is
   * given then the default position will be used.
   */
  virtual iSector* GetStartSector (const char* name = 0) = 0;

  /**
   * Get start position. The given name is the name
   * of the starting position to use. If 0 is
   * given then the default position will be used.
   */
  virtual csVector3 GetStartPosition (const char* name = 0) = 0;

  /**
   * Update the given pccamera so that it points to the
   * starting position in this region. The given name is
   * the name of the starting position to use. If 0
   * is given then the default position will be used.
   */
  virtual void PointCamera (iPcCamera* pccamera, const char* name = 0) = 0;

  /**
   * Returns the CS collection associated to this propclass. This function has a
   * meaning only if the region has been loaded.
   */
  virtual iCollection* GetCollection() = 0;
};

#endif // __CEL_PF_REGION__

