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

class cameraSectorListener : public scfImplementation1<
	cameraSectorListener, iCameraSectorListener>
{
private:
  csWeakRef<celPcZoneManager> zonemgr;

public:
  cameraSectorListener (celPcZoneManager* zonemgr) :
    scfImplementationType (this)
  {
    cameraSectorListener::zonemgr = zonemgr;
  }
  virtual ~cameraSectorListener ()
  {
  }
  virtual void NewSector (iCamera* camera, iSector* sector);
};

class meshmoveListener : public scfImplementation1<
	meshmoveListener, iMovableListener>
{
private:
  csWeakRef<celPcZoneManager> zonemgr;

public:
  meshmoveListener (celPcZoneManager* zonemgr) : scfImplementationType (this)
  {
    meshmoveListener::zonemgr = zonemgr;
  }
  virtual ~meshmoveListener ()
  {
  }
  virtual void MovableChanged (iMovable* movable);
  virtual void MovableDestroyed (iMovable*) { }
};

class celMapFile : public scfImplementation1<
	celMapFile, iCelMapFile>
{
private:
  char* name;
  char* path;
  char* file;
  char* sectorname;

public:
  celMapFile () : scfImplementationType (this)
  {
    name = 0;
    path = 0;
    file = 0;
    sectorname = 0;
  }
  virtual ~celMapFile ()
  {
    delete[] name;
    delete[] path;
    delete[] file;
    delete[] sectorname;
  }

  virtual void SetName (const char* name);
  virtual void SetPath (const char* path);
  virtual void SetFile (const char* file);
  virtual const char* GetName () const { return name; }
  virtual const char* GetPath () const { return path; }
  virtual const char* GetFile () const { return file; }
  virtual void SetSectorName (const char* name);
  virtual const char* GetSectorName () const { return sectorname; }
};

class celRegion : public scfImplementation3<
	celRegion, iCelRegion, iCelNewEntityCallback, iEngineSectorCallback>
{
private:
  celPcZoneManager* mgr;
  csString name;
  csString csregionname;
  csString cache_path;
  bool loaded;
  csRef<iRegion> csregion;
  csRefArray<celMapFile> mapfiles;
  csSet<csPtrKey<iSector> > sectors;

  // This property class maintains private child entities.
  csWeakRefArray<iCelEntity> entities;

public:
  celRegion (celPcZoneManager* mgr, const char* name) :
    scfImplementationType (this)
  {
    celRegion::mgr = mgr;
    celRegion::name = name;
    loaded = false;
  }
  virtual ~celRegion ()
  {
  }
  void SetEntityName (const char* entname);

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

  virtual const char* GetName () const { return name; }
  virtual const char* GetCsRegionName () const { return csregionname; }
  virtual void SetCachePath (const char* path);
  virtual const char* GetCachePath () const { return cache_path; }
  virtual iCelMapFile* CreateMapFile ();
  virtual size_t GetMapFileCount () const { return mapfiles.GetSize (); }
  virtual iCelMapFile* GetMapFile (int idx) const
  {
    return (iCelMapFile*)mapfiles[idx];
  }
  virtual iCelMapFile* FindMapFile (const char* name) const;
  virtual bool RemoveMapFile (iCelMapFile* mapfile);
  virtual void RemoveAllMapFiles ();
  virtual void AssociateEntity (iCelEntity* entity);
  virtual void DissociateEntity (iCelEntity* entity);
  virtual bool ContainsEntity (iCelEntity* entity);
  virtual iRegion* GetCsRegion ();

  // For iCelNewEntityCallback.
  virtual void NewEntity (iCelEntity* entity);

  // For iEngineSectorCallback.
  virtual void NewSector (iEngine* engine, iSector* sector);
  virtual void RemoveSector (iEngine* engine, iSector* sector);
};

class celZone : public scfImplementation1<
	celZone, iCelZone>
{
private:
  celPcZoneManager* mgr;
  char* name;
  csRefArray<celRegion> regions;

public:
  celZone (celPcZoneManager* mgr, const char* name) :
    scfImplementationType (this)
  {
    celZone::mgr = mgr;
    celZone::name = csStrNew (name);
  }
  virtual ~celZone ()
  {
    delete[] name;
  }

  bool ContainsRegion (celRegion* region);

  virtual const char* GetName () const { return name; }
  virtual void LinkRegion (iCelRegion* region);
  virtual size_t GetRegionCount () const { return regions.GetSize (); }
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
class celPcZoneManager : public scfImplementationExt1<
	celPcZoneManager, celPcCommon, iPcZoneManager>
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
  int loading_mode;

  // The active sector.
  csWeakRef<iSector> active_sector;

  static csStringID id_path;
  static csStringID id_file;
  static csStringID id_region;
  static csStringID id_entityname;
  static csStringID id_regionname;
  static csStringID id_startname;
  static csStringID id_zonename;
  static csStringID id_mode;
  static csStringID id_name;

  enum actionids
  {
    action_disablecd = 0,
    action_enablecd,
    action_load,
    action_pointmesh,
    action_pointcamera,
    action_setloadingmode,
    action_activateregion,
    action_createregion,
    action_removeregion,
    action_createzone,
    action_removezone,
    action_createmap,
    action_removemap,
    action_setcache,
    action_linkregion,
    action_unlinkregion
  };

  celOneParameterBlock* params;

  // Last used region and start name (for PointCamera).
  csString last_regionname;
  csString last_startname;

  csStringArray region_names;
  csStringArray start_names;

  // If Load(path,file) is used then the following two will be set.
  csString path, file;

  csStringHash xmltokens;
#define CS_TOKEN_ITEM_FILE "plugins/propclass/zone/zone.tok"
#include "cstool/tokenlist.h"

  bool ParseRegion (iDocumentNode* regionnode, iCelRegion* region);
  bool ParseZone (iDocumentNode* zonenode, iCelZone* zone);
  bool ParseStart (iDocumentNode* startnode);

  // For properties.
  enum propids
  {
    propid_laststart = 0,
    propid_lastregion
  };
  static PropertyHolder propinfo;

public:
  celPcZoneManager (iObjectRegistry* object_reg);
  virtual ~celPcZoneManager ();

  iEngine* GetEngine () const { return engine; }
  iLoader* GetLoader () const { return loader; }
  iVFS* GetVFS () const { return vfs; }
  iCelPlLayer* GetPL () const { return pl; }
  iCollideSystem* GetCDSystem () const { return cdsys; }

  // Activate some region. This will load all zones that contain
  // the given region and unload all other zones. It is safe to call
  // this too many times as it will check if a region or zone is already
  // loaded or not and avoid doing unneeded work.
  virtual bool ActivateRegion (iCelRegion* region,
      bool allow_entity_addon = true);

  // Activate some sector. This will first find the region that this
  // sector is in and then it will activate that region.
  bool ActivateSector (iSector* sector);

  virtual csPtr<iCelDataBuffer> SaveFirstPass ();
  virtual bool LoadFirstPass (iCelDataBuffer* databuf);
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool GetPropertyIndexed (int, const char*&);

  virtual bool PerformActionIndexed (int, iCelParameterBlock* params,
      celData& ret);

  void SendZoneMessage (iCelRegion* region, const char* msgid);

  virtual void EnableColliderWrappers (bool en) { do_colliderwrappers = en; }
  virtual bool IsColliderWrappers () const { return do_colliderwrappers; }
  virtual void SetLoadingMode (int mode);
  virtual int GetLoadingMode () const { return loading_mode; }

  virtual bool Load (iDocumentNode* node);
  virtual bool Load (const char* path, const char* file);

  virtual iCelZone* CreateZone (const char* name);
  virtual size_t GetZoneCount () const { return zones.GetSize (); }
  virtual iCelZone* GetZone (int idx) const { return (iCelZone*)zones[idx]; }
  virtual iCelZone* FindZone (const char* name) const;
  virtual bool RemoveZone (iCelZone* zone);
  virtual void RemoveAllZones ();

  virtual iCelRegion* CreateRegion (const char* name);
  virtual size_t GetRegionCount () const { return regions.GetSize (); }
  virtual iCelRegion* GetRegion (int idx) const
  { return (iCelRegion*)regions[idx]; }
  virtual iCelRegion* FindRegion (const char* name) const;
  virtual bool RemoveRegion (iCelRegion* region);
  virtual void RemoveAllRegions ();
  virtual iCelRegion* FindRegionContaining (iCelEntity* ent);

  virtual void FindStartLocations (iStringArray* regionnames,
      iStringArray* startnames);
  virtual void GetLastStartLocation (iString* regionname,
      iString* startname);
  virtual const char *GetLastStartRegionName ();
  virtual const char *GetLastStartName ();

  virtual int PointCamera (const char* entity, const char* regionname,
  	const char* startname = 0);
  virtual int PointMesh (const char* entity, const char* regionname,
  	const char* startname = 0);
  int UpdateMeshCamera ();
};

#endif // __CEL_PF_ZONEMGR__

