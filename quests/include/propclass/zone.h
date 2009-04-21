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
struct iCollection;
struct iPcCamera;
struct iPcMesh;
struct iCelEntity;
struct iStringArray;
struct iString;
struct iDocumentNode;

/**
 * A representation of a map file for a region.
 */
struct iCelMapFile : public virtual iBase
{
  SCF_INTERFACE (iCelMapFile, 0, 2, 0);

  /**
   * Set the name associated with this map.
   */
  virtual void SetName (const char* name) = 0;

  /**
   * Set the VFS path for the world file associated with this map.
   * If not 0 then the VFS current dir will be set to this path and
   * filename will be relative to that. Otherwise path is ignored.
   */
  virtual void SetPath (const char* path) = 0;

  /**
   * Set the VFS filename for the world file associated with this map.
   * This is relative to the path if given.
   */
  virtual void SetFile (const char* file) = 0;

  /**
   * Get name associated with this map.
   * Returns 0 if name is not used.
   */
  virtual const char* GetName () const = 0;

  /**
   * Get the VFS path for the world file associated with this map.
   * Returns 0 if path is not used.
   */
  virtual const char* GetPath () const = 0;

  /**
   * Get the VFS filename for the world file associated with this map.
   * Returns 0 if SetSectorName() is used.
   */
  virtual const char* GetFile () const = 0;

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

/**
 * A region. A region is a collection of map files
 * which will be loaded in one CS region (iRegion).
 */
struct iCelRegion : public virtual iBase
{
  SCF_INTERFACE (iCelRegion, 0, 3, 0);

  /// Get the name of this region.
  virtual const char* GetName () const = 0;

  /**
   * Get the name of the CS region that is used for this region.
   * This will be the name of the entity containing the zone
   * manager appended with the name of the region. i.e. like:
   * <entity>_<region>
   */
  virtual const char* GetCsCollectionName () const = 0;

  /**
   * Set the VFS path that will be used during the call to engine->Prepare()
   * after loading this region. By default this is not set which means that
   * the lightmaps are supposed to come from every individual map file.
   * If you set this path then you say that the cache manager is setup at
   * this directory instead.
   */
  virtual void SetCachePath (const char* path) = 0;

  /**
   * Get the VFS path for the cache manager.
   * Returns 0 if path is not used.
   */
  virtual const char* GetCachePath () const = 0;

  /**
   * Create a map file for this region and associate it
   * with this region.
   */
  virtual iCelMapFile* CreateMapFile () = 0;

  /**
   * Get the count of map files in this region.
   */
  virtual size_t GetMapFileCount () const = 0;

  /**
   * Get the specified map file.
   */
  virtual iCelMapFile* GetMapFile (int idx) const = 0;

  /**
   * Find the specified map file.
   */
  virtual iCelMapFile* FindMapFile (const char* name) const = 0;

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
   * Register an entity that should be removed as soon as this
   * region is unloaded.
   */
  virtual void AssociateEntity (iCelEntity* entity) = 0;

  /**
   * Unregister an entity from this region.
   */
  virtual void DissociateEntity (iCelEntity* entity) = 0;

  /**
   * Determine whether the given entity is in this region.
   */
  virtual bool ContainsEntity (iCelEntity* entity) = 0;

  /**
   * Get the CS region that is used for this region.
   */
  virtual iCollection* GetCollection () = 0;
};

/**
 * A zone. A zone represents a collection of regions
 * that are all in memory at the same time.
 */
struct iCelZone : public virtual iBase
{
  SCF_INTERFACE (iCelZone, 0, 1, 0);

  /// Get the name of this zone.
  virtual const char* GetName () const = 0;

  /**
   * Link a region with this zone.
   */
  virtual void LinkRegion (iCelRegion* region) = 0;

  /**
   * Get the count of regions in this zone.
   */
  virtual size_t GetRegionCount () const = 0;

  /**
   * Get the specified region.
   */
  virtual iCelRegion* GetRegion (size_t idx) const = 0;

  /**
   * Get the specified region.
   */
  virtual iCelRegion* FindRegion (const char* name) const = 0;

  /**
   * Unlink the region from this zone. Returns
   * false if the region could not be found in this zone.
   */
  virtual bool UnlinkRegion (iCelRegion* region) = 0;

  /**
   * Unlink all regions from this zone.
   */
  virtual void UnlinkAllRegions () = 0;
};

/** \name PointCamera error codes
 * @{ */

/// No error.
#define CEL_ZONEERROR_OK 0

/// Couldn't find the region.
#define CEL_ZONEERROR_BADREGION 1

/// Couldn't find the given start location.
#define CEL_ZONEERROR_BADSTART 2

/// Error loading zone!
#define CEL_ZONEERROR_LOAD 3
/** @} */

/** \name SetLoadingMode error codes
 * @{ */

/// Load/unload regions as needed (default).
#define CEL_ZONE_NORMAL 0

/// Load regions as needed but keep them in memory.
#define CEL_ZONE_KEEP 1

/// Load all regions immediatelly.
#define CEL_ZONE_LOADALL 2

/** @} */

/**
 * This is the zone manager. In this property class you can define
 * zones. A zone is a collection of regions that are either all together
 * in memory or not at all.
 * A region is made from a set of maps and represents the actual geometry
 * that is in memory. A region is loaded or unloaded as one unit.
 * A region can be in multiple zones. In that case if you are in some region
 * then all zones that contain that region will be in memory,
 * The representation of a region.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - DisableCD: no parameters. Don't create collider wrappers.
 * - EnableCD: no parameters. Create collider wrappers (default).
 * - Load: parameters 'path' (string: optional), and 'file' (string).
 * - PointMesh: parameters 'entity' (string), 'region' (string),
 *   and 'start' (string).
 * - PointCamera: parameters 'entity' (string), 'region' (string),
 *   and 'start' (string).
 * - SetLoadingMode: parameters 'mode' (string: one of 'loadall',
 *   'normal', or 'keep').
 * - ActivateRegion: parameters 'region' (string).
 * - CreateRegion: parameters 'name' (string).
 * - RemoveRegion: parameters 'name' (string).
 * - CreateZone: parameters 'name' (string).
 * - RemoveZone: parameters 'name' (string).
 * - CreateMap: parameters 'region' (string) 'name' (string) 'path' (string) 'file' (string).
 * - RemoveMap: parameters 'region' (string) 'name' (string).
 * - SetCache: parameters 'region' (string) 'path' (string).
 * - LinkRegion: parameters 'zone' (string) 'region' (string).
 * - UnlinkRegion: parameters 'zone' (string) 'region' (string).
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - laststart (string, read only): last used start location.
 * - lastregion (string, read only): last used region.
 *
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * - pczonemanager_addregion: new region has been added (region)
 * - pczonemanager_remregion: region will be removed (region)
 * - pczonemanager_startloading: loading of regions starts
 * - pczonemanager_stoploading: loading of regions stops
 * - pczonemanager_errorloading: loading of regions stops with error (region)
 */
struct iPcZoneManager : public virtual iBase
{
  SCF_INTERFACE (iPcZoneManager, 0, 1, 3);

  /**
   * By default the zone manager will create collider wrappers
   * (csColliderWrapper) for all objects that are loaded. If you don't want
   * that behaviour you can disable it here.
   */
  virtual void EnableColliderWrappers (bool en) = 0;

  /**
   * Is collider wrapper creation enabled?
   */
  virtual bool IsColliderWrappers () const = 0;

  /**
   * Set different loading mode:
   * - CEL_ZONE_NORMAL: load/unload regions as needed (default).
   * - CEL_ZONE_KEEP: load regions as needed but keep them in memory.
   * - CEL_ZONE_LOADALL: load all regions immediatelly.
   * Setting CEL_ZONE_LOADALL will immediatelly cause all regions
   * to be loaded.
   */
  virtual void SetLoadingMode (int mode) = 0;

  /**
   * Get the current loading mode.
   */
  virtual int GetLoadingMode () const = 0;

  /**
   * Load a zone definition from an XML node.
   * \param node is the node containing region and zone definitions.
   * \return true on success. On failure the error will be reported to
   * the reporter and false will be returned.
   */
  virtual bool Load (iDocumentNode* node) = 0;

  /**
   * Load a zone definition from an XML file. The XML file should
   * contain a 'level' or 'world' node. In case it is a 'world' file then
   * the zone manager will automatically create a region called 'main'
   * and a zone called 'main' with a default start location. This is a
   * convenience.
   * \param path is a VFS path. Current VFS dir will be set to that path.
   * Can be 0 if no path is desired.
   * \param file is a VFS file.
   * \return true on success. On failure the error will be reported to
   * the reporter and false will be returned.
   */
  virtual bool Load (const char* path, const char* file) = 0;

  /**
   * Create a zone.
   */
  virtual iCelZone* CreateZone (const char* name) = 0;

  /**
   * Get the count of zones.
   */
  virtual size_t GetZoneCount () const = 0;

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
   * Create a region.
   */
  virtual iCelRegion* CreateRegion (const char* name) = 0;

  /**
   * Get the count of regions.
   */
  virtual size_t GetRegionCount () const = 0;

  /**
   * Get the specified region.
   */
  virtual iCelRegion* GetRegion (int idx) const = 0;

  /**
   * Get the specified region.
   */
  virtual iCelRegion* FindRegion (const char* name) const = 0;

  /**
   * Delete the given region. Returns false if the region could
   * not be found in this zone.
   */
  virtual bool RemoveRegion (iCelRegion* region) = 0;

  /**
   * Remove all regions from this zone.
   */
  virtual void RemoveAllRegions () = 0;

  /**
   * Find all valid starting locations in all current loaded regions.
   */
  virtual void FindStartLocations (iStringArray* regionnames,
  	iStringArray* startnames) = 0;

  /**
   * Return the last used region and startname for PointCamera() or
   * PointMesh(). Returns an empty region name if those functions
   * have not been called yet.
   */
  virtual void GetLastStartLocation (iString* regionname,
  	iString* startname) = 0;
  /**
   * Return the last used region name for PointCamera() or
   * PointMesh(). Returns an empty region name if those functions
   * have not been called yet.
   */
  virtual const char *GetLastStartRegionName () = 0;

  /**
   * Return the last used startname name for PointCamera() or
   * PointMesh(). 
   */
  virtual const char *GetLastStartName () = 0;

  /**
   * Take the specified region (by name), load all zones associated
   * with that region and then setup the given pccamera so that it
   * points to the specified starting position in this region. If 0
   * is given then the default position will be used.
   * Returns one of:
   * - #CEL_ZONEERROR_OK: no error
   * - #CEL_ZONEERROR_BADREGION: can't find region
   * - #CEL_ZONEERROR_BADSTART: can't find start
   * - #CEL_ZONEERROR_LOAD: error loading zone
   *
   * \param entity is the name of the entity that contains the
   * pccamera property class.
   * \param regionname the name of the region to load.
   * \param startname the name of the starting position.
   */
  virtual int PointCamera (const char* entity, const char* regionname,
  	const char* startname = 0) = 0;

  /**
   * Take the specified region (by name), load all zones associated
   * with that region and then place the given pcmesh so that it
   * is positioned at the specified starting position in this region. If 0
   * is given then the default position will be used.
   * Returns one of:
   * - #CEL_ZONEERROR_OK: no error
   * - #CEL_ZONEERROR_BADREGION: can't find region
   * - #CEL_ZONEERROR_BADSTART: can't find start
   * - #CEL_ZONEERROR_LOAD: error loading zone
   *
   * \param entity is the name of the entity that contains the
   * pcmesh property class.
   * \param regionname the name of the region to load.
   * \param startname the name of the starting position.
   */
  virtual int PointMesh (const char* entity, const char* regionname,
  	const char* startname = 0) = 0;

  /**
   * Activate some region. This will load all zones that contain
   * the given region and unload all other zones. It is safe to call
   * this too many times as it will check if a region or zone is already
   * loaded or not and avoid doing unneeded work.
   */
  virtual bool ActivateRegion (iCelRegion* region,
      bool allow_entity_addon = true) = 0;

  /**
   * Get the region containing the specified entity.
   */
  virtual iCelRegion* FindRegionContaining (iCelEntity* ent) = 0;
};

#endif // __CEL_PF_ZONE__

