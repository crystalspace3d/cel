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

SCF_VERSION (iPcRegion, 0, 1, 0);

/**
 * The representation of a region.
 * This property class supports the following actions (add prefix
 * 'cel.property.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>Load: no parameters.
 * </ul>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * <ul>
 * <li>worlddir (string, read/write): the VFS directory of the world file.
 * <li>worldfile (string, read/write): the world filename.
 * <li>regionname (string, read/write): region name.
 * </ul>
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
};

#endif // __CEL_PF_REGION__

