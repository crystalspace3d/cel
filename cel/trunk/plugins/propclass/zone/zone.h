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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PF_ZONEMGR__
#define __CEL_PF_ZONEMGR__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/weakrefarr.h"
#include "csutil/refarr.h"
#include "csutil/util.h"
#include "csutil/hash.h"
#include "csutil/set.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "physicallayer/entity.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/camera.h"
#include "propclass/zone.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/movable.h"

struct iCelEntity;
struct iObjectRegistry;
struct iEngine;
struct iLoader;
struct iVFS;
struct iCollideSystem;
struct iCelPlLayer;
struct iPcCamera;
struct iPcMesh;
class celPcZoneManager;

/**
 * Factory for zone manager.
 */
CEL_DECLARE_FACTORY(ZoneManager)

class cameraSectorListener : public iCameraSectorListener
{
private:
  celPcZoneManager* zonemgr;

public:
  cameraSectorListener (celPcZoneManager* zonemgr)
  {
    SCF_CONSTRUCT_IBASE (0);
    cameraSectorListener::zonemgr = zonemgr;
  }
  virtual ~cameraSectorListener ()
  {
    SCF_DESTRUCT_IBASE ();
  }
  SCF_DECLARE_IBASE;
  virtual void NewSector (iCamera* camera, iSector* sector);
};

class meshmoveListener : public iMovableListener
{
private:
  celPcZoneManager* zonemgr;

public:
  meshmoveListener (celPcZoneManager* zonemgr)
  {
    SCF_CONSTRUCT_IBASE (0);
    meshmoveListener::zonemgr = zonemgr;
  }
  virtual ~meshmoveListener ()
  {
    SCF_DESTRUCT_IBASE ();
  }
  SCF_DECLARE_IBASE;
  virtual void MovableChanged (iMovable* movable);
  virtual void MovableDestroyed (iMovable*) { }
};

class celMapFile : public iCelMapFile
{
private:
  char* path;
  char* file;
  char* sectorname;

public:
  celMapFile ()
  {
    SCF_CONSTRUCT_IBASE (0);
    path = 0;
    file = 0;
    sectorname = 0;
  }
  virtual ~celMapFile ()
  {
    delete[] path;
    delete[] file;
    delete[] sectorname;
    SCF_DESTRUCT_IBASE ();
  }

  SCF_DECLARE_IBASE;

  virtual void SetPath (const char* path);
  virtual void SetFile (const char* file);
  virtual const char* GetPath () const { return path; }
  virtual const char* GetFile () const { return file; }
  virtual void SetSectorName (const char* name);
  virtual const char* GetSectorName () const { return sectorname; }
};

class celRegion : public iCelRegion,
	public iCelNewEntityCallback,
	public iEngineSectorCallback
{
private:
  celPcZoneManager* mgr;
  char* name;
  csString cache_path;
  bool loaded;
  csRefArray<celMapFile> mapfiles;
  csSet<csPtrKey<iSector> > sectors;

  // This property class maintains private child entities.
  csWeakRefArray<iCelEntity> entities;

public:
  celRegion (celPcZoneManager* mgr, const char* name)
  {
    SCF_CONSTRUCT_IBASE (0);
    celRegion::mgr = mgr;
    celRegion::name = csStrNew (name);
    loaded = false;
  }
  virtual ~celRegion ()
  {
    delete[] name;
    SCF_DESTRUCT_IBASE ();
  }

  /**
   * \param allow_entity_addon if false then entity addons will not
   * be allowed to create entities. This is used when loading this
   * region. The persistence layer will take care of creating the other
   * entities so it is not needed (or even desired) to load them here.
   */
  bool Load (bool allow_entity_addon);
  void Unload ();
  bool IsLoaded () const { return loaded; }

  bool ContainsSector (iSector* sector) { return sectors.In (sector); }

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual void SetCachePath (const char* path);
  virtual const char* GetCachePath () const { return cache_path; }
  virtual iCelMapFile* CreateMapFile ();
  virtual size_t GetMapFileCount () const { return mapfiles.Length (); }
  virtual iCelMapFile* GetMapFile (int idx) const
  {
    return (iCelMapFile*)mapfiles[idx];
  }
  virtual bool RemoveMapFile (iCelMapFile* mapfile);
  virtual void RemoveAllMapFiles ();
  virtual void AssociateEntity (iCelEntity* entity);

  // For iCelNewEntityCallback.
  virtual void NewEntity (iCelEntity* entity);

  // For iEngineSectorCallback.
  virtual void NewSector (iEngine* engine, iSector* sector);
  virtual void RemoveSector (iEngine* engine, iSector* sector);
};

class celZone : public iCelZone
{
private:
  celPcZoneManager* mgr;
  char* name;
  csRefArray<celRegion> regions;

public:
  celZone (celPcZoneManager* mgr, const char* name)
  {
    SCF_CONSTRUCT_IBASE (0);
    celZone::mgr = mgr;
    celZone::name = csStrNew (name);
  }
  virtual ~celZone ()
  {
    delete[] name;
    SCF_DESTRUCT_IBASE ();
  }

  bool ContainsRegion (celRegion* region);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual void LinkRegion (iCelRegion* region);
  virtual size_t GetRegionCount () const { return regions.Length (); }
  virtual iCelRegion* GetRegion (size_t idx) const
  {
    return (iCelRegion*)regions[idx];
  }
  virtual iCelRegion* FindRegion (const char* name) const;
  virtual bool UnlinkRegion (iCelRegion* region);
  virtual void UnlinkAllRegions ();
};

/**
 * This is the zone manager property class.
 */
class celPcZoneManager : public celPcCommon
{
private:
  csRef<iEngine> engine;
  csRef<iLoader> loader;
  csRef<iVFS> vfs;
  csRef<iCollideSystem> cdsys;

  csRef<cameraSectorListener> camlistener;
  csRef<meshmoveListener> meshlistener;

  csString camera_entity;
  csWeakRef<iPcCamera> pccamera;
  csString mesh_entity;
  csWeakRef<iPcMesh> pcmesh;

  csRefArray<celZone> zones;
  csRefArray<celRegion> regions;

  bool do_colliderwrappers;

  // The active sector.
  csWeakRef<iSector> active_sector;

  static csStringID action_disablecd;
  static csStringID action_enablecd;
  static csStringID action_load;
  static csStringID id_path;
  static csStringID id_file;
  static csStringID id_region;
  static csStringID action_pointmesh;
  static csStringID action_pointcamera;
  static csStringID id_entityname;
  static csStringID id_regionname;
  static csStringID id_startname;

  celOneParameterBlock* params;

  // Activate some region. This will load all zones that contain
  // the given region and unload all other zones. It is safe to call
  // this too many times as it will check if a region or zone is already
  // loaded or not and avoid doing unneeded work.
  bool ActivateRegion (celRegion* region, bool allow_entity_addon = true);

  // Last used region and start name (for PointCamera).
  csString last_regionname;
  csString last_startname;

  // If Load(path,file) is used then the following two will be set.
  csString path, file;

  csStringHash xmltokens;
#define CS_TOKEN_ITEM_FILE "plugins/propclass/zone/zone.tok"
#include "cstool/tokenlist.h"

  bool ParseRegion (iDocumentNode* regionnode, iCelRegion* region);
  bool ParseZone (iDocumentNode* zonenode, iCelZone* zone);
  bool ParseStart (iDocumentNode* startnode);

public:
  celPcZoneManager (iObjectRegistry* object_reg);
  virtual ~celPcZoneManager ();

  iEngine* GetEngine () const { return engine; }
  iLoader* GetLoader () const { return loader; }
  iVFS* GetVFS () const { return vfs; }
  iCelPlLayer* GetPL () const { return pl; }
  iCollideSystem* GetCDSystem () const { return cdsys; }

  // Activate some sector. This will first find the region that this
  // sector is in and then it will activate that region.
  bool ActivateSector (iSector* sector);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pczonemanager"; }
  virtual csPtr<iCelDataBuffer> SaveFirstPass ();
  virtual bool LoadFirstPass (iCelDataBuffer* databuf);
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  virtual bool PerformAction (csStringID, iCelParameterBlock* params);

  void SendZoneMessage (iCelRegion* region, const char* msgid);

  void EnableColliderWrappers (bool en) { do_colliderwrappers = en; }
  bool IsColliderWrappers () const { return do_colliderwrappers; }
  bool Load (iDocumentNode* node);
  bool Load (const char* path, const char* file);

  iCelZone* CreateZone (const char* name);
  size_t GetZoneCount () const { return zones.Length (); }
  iCelZone* GetZone (int idx) const { return (iCelZone*)zones[idx]; }
  iCelZone* FindZone (const char* name) const;
  bool RemoveZone (iCelZone* zone);
  void RemoveAllZones ();

  iCelRegion* CreateRegion (const char* name);
  size_t GetRegionCount () const { return regions.Length (); }
  iCelRegion* GetRegion (int idx) const { return (iCelRegion*)regions[idx]; }
  iCelRegion* FindRegion (const char* name) const;
  bool RemoveRegion (iCelRegion* region);
  void RemoveAllRegions ();

  void FindStartLocations (iStringArray* regionnames, iStringArray* startnames);
  void GetLastStartLocation (iString* regionname, iString* startname);

  int PointCamera (const char* entity, const char* regionname,
  	const char* startname);
  int PointMesh (const char* entity, const char* regionname,
  	const char* startname);
  int UpdateMeshCamera ();

  struct PcZoneManager : public iPcZoneManager
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcZoneManager);
    virtual void EnableColliderWrappers (bool en)
    {
      scfParent->EnableColliderWrappers (en);
    }
    virtual bool IsColliderWrappers () const
    {
      return scfParent->IsColliderWrappers ();
    }
    virtual bool Load (iDocumentNode* node)
    {
      return scfParent->Load (node);
    }
    virtual bool Load (const char* path, const char* file)
    {
      return scfParent->Load (path, file);
    }
    virtual iCelZone* CreateZone (const char* name)
    {
      return scfParent->CreateZone (name);
    }
    virtual size_t GetZoneCount () const
    {
      return scfParent->GetZoneCount ();
    }
    virtual iCelZone* GetZone (int idx) const
    {
      return scfParent->GetZone (idx);
    }
    virtual iCelZone* FindZone (const char* name) const
    {
      return scfParent->FindZone (name);
    }
    virtual bool RemoveZone (iCelZone* zone)
    {
      return scfParent->RemoveZone (zone);
    }
    virtual void RemoveAllZones ()
    {
      scfParent->RemoveAllZones ();
    }
    virtual iCelRegion* CreateRegion (const char* name)
    {
      return scfParent->CreateRegion (name);
    }
    virtual size_t GetRegionCount () const
    {
      return scfParent->GetRegionCount ();
    }
    virtual iCelRegion* GetRegion (int idx) const
    {
      return scfParent->GetRegion (idx);
    }
    virtual iCelRegion* FindRegion (const char* name) const
    {
      return scfParent->FindRegion (name);
    }
    virtual bool RemoveRegion (iCelRegion* region)
    {
      return scfParent->RemoveRegion (region);
    }
    virtual void RemoveAllRegions ()
    {
      scfParent->RemoveAllRegions ();
    }
    virtual void FindStartLocations (iStringArray* regionnames,
  	iStringArray* startnames)
    {
      scfParent->FindStartLocations (regionnames, startnames);
    }
    virtual void GetLastStartLocation (iString* regionname,
  	iString* startname)
    {
      scfParent->GetLastStartLocation (regionname, startname);
    }
    virtual int PointCamera (const char* entity, const char* regionname,
  	const char* startname = 0)
    {
      return scfParent->PointCamera (entity, regionname, startname);
    }
    virtual int PointMesh (const char* entity, const char* regionname,
  	const char* startname = 0)
    {
      return scfParent->PointMesh (entity, regionname, startname);
    }
  } scfiPcZoneManager;
};

#endif // __CEL_PF_ZONEMGR__

