/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_ZONE__
#define __CEL_PF_ZONE__

#include "cstypes.h"
#include "csgeom/vector3.h"
#include "csutil/scf.h"

struct iSector;
struct iPcCamera;

SCF_VERSION (iCelMapFile, 0, 1, 0);

/**
 * A representation of a map file for a region.
 */
struct iCelMapFile : public iBase
{
  /// Set the VFS path for the world file associated with this map.
  virtual void SetPath (const char* path) = 0;

  /**
   * Get the VFS path for the world file associated with this map.
   * Returns 0 if SetSectorName() is used.
   */
  virtual const char* GetPath () const = 0;

  /**
   * This map file corresponds with a sector instead of a VFS path.
   * Use this instead of SetPath() if you want the region to have a
   * single empty sector. The given name is the name
   * of the sector.
   */
  virtual void SetSectorName (const char* name) = 0;

  /**
   * Return the sector name if this map is represented by a single
   * sector (i.e. SetSectorName()). Returns 0 if SetPath() is used.
   */
  virtual const char* GetSectorName () const = 0;
};

SCF_VERSION (iCelRegion, 0, 1, 0);

/**
 * A region. A region is a collection of map files
 * which will be loaded in one CS region (iRegion).
 */
struct iCelRegion : public iBase
{
  /// Get the name of this region.
  virtual const char* GetName () const = 0;

  /**
   * Create a map file for this region and associate it
   * with this region.
   */
  virtual iCelMapFile* CreateMapFile () = 0;

  /**
   * Get the count of map files in this region.
   */
  virtual int GetMapFileCount () const = 0;

  /**
   * Get the specified map file.
   */
  virtual iCelMapFile* GetMapFile (int idx) const = 0;

  /**
   * Delete the given map file from this region. Returns
   * false if the map file could not be found in this region.
   */
  virtual bool RemoveMapFile (iCelMapFile* mapfile) = 0;

  /**
   * Remove all map files from this region.
   */
  virtual void RemoveAllMapFiles () = 0;

  /**
   * Load the region (load all map files). This will create the empty
   * sector if CreateEmptySector() is used.
   */
  virtual bool Load () = 0;

  /**
   * Unload the region.
   */
  virtual void Unload () = 0;

  /*
   * Returns a sector for this region.
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

SCF_VERSION (iCelZone, 0, 1, 0);

/**
 * A zone. A zone represents a collection of regions
 * that are all in memory at the same time.
 */
struct iCelZone : public iBase
{
  /// Get the name of this zone.
  virtual const char* GetName () const = 0;

  /**
   * Create a region for this zone and associate it
   * with this zone.
   */
  virtual iCelRegion* CreateRegion (const char* name) = 0;

  /**
   * Get the count of regions in this zone.
   */
  virtual int GetRegionCount () const = 0;

  /**
   * Get the specified region.
   */
  virtual iCelRegion* GetRegion (int idx) const = 0;

  /**
   * Get the specified region.
   */
  virtual iCelRegion* FindRegion (const char* name) const = 0;

  /**
   * Delete the given region from this zone. Returns
   * false if the region could not be found in this zone.
   */
  virtual bool RemoveRegion (iCelRegion* region) = 0;

  /**
   * Remove all regions from this zone.
   */
  virtual void RemoveAllRegions () = 0;

  /**
   * Load this zone. This will load all regions in this
   * zone.
   */
  virtual bool Load () = 0;

  /**
   * Unload this zone. This will unload all regions in this
   * zone unless they are needed in another zone that is
   * still in memory.
   */
  virtual void Unload () = 0;

};


SCF_VERSION (iPcZoneManager, 0, 1, 0);

/**
 * This is the zone manager. In this property class you can define
 * zones. A zone is a collection of regions that are either all together
 * in memory or not at all.
 * A region is made from a set of maps and represents the actual geometry
 * that is in memory. A region is loaded or unloaded as one unit.
 * A region can be in multiple zones. In that case if you are in some region
 * then all zones that contain that region will be in memory,
 * The representation of a region.
 */
struct iPcZoneManager : public iBase
{
  /**
   * Create a zone.
   */
  virtual iCelZone* CreateZone (const char* name) = 0;

  /**
   * Get the count of zones.
   */
  virtual int GetZoneCount () const = 0;

  /**
   * Get the specified zone.
   */
  virtual iCelZone* GetZone (int idx) const = 0;

  /**
   * Get the specified zone.
   */
  virtual iCelZone* FindZone (const char* name) const = 0;

  /**
   * Delete the given zone. Returns false if the zone
   * could not be found.
   */
  virtual bool RemoveZone (iCelZone* zone) = 0;

  /**
   * Remove all zones.
   */
  virtual void RemoveAllZones () = 0;





  /**
   * Load the region. This will create the empty sector if CreateEmptySector()
   * is used.
   */
  virtual bool Load () = 0;

  /**
   * Unload the region.
   */
  virtual void Unload () = 0;

  /*
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

#endif // __CEL_PF_ZONE__

