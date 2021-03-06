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

#include "cssysdef.h"
#include <math.h>
#include "propclass/mesh.h"
#include "plugins/propclass/zone/zone.h"
#include "propclass/camera.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/flags.h"
#include "csutil/xmltiny.h"
#include "csutil/databuf.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "iutil/virtclk.h"
#include "iutil/csinput.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/string.h"
#include "iutil/stringarray.h"
#include "iutil/document.h"
#include "imap/loader.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/camera.h"
#include "iengine/collection.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/csview.h"
#include "cstool/collider.h"
#include "ivaria/collider.h"
#include "ivaria/view.h"
#include "ivideo/graph3d.h"
#include "csqsqrt.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (ZoneManager, "pcworld.zonemanager", "pczonemanager")

//---------------------------------------------------------------------------

void cameraSectorListener::NewSector (iCamera* /*camera*/, iSector* sector)
{
  if (zonemgr)
    zonemgr->ActivateSector (sector);
}

void meshmoveListener::MovableChanged (iMovable* movable)
{
  if (!zonemgr) return;
  if (movable->GetSectors ()->GetCount () > 0)
    zonemgr->ActivateSector (movable->GetSectors ()->Get (0));
}

//---------------------------------------------------------------------------

void celMapFile::SetFile (const char* file)
{
  delete[] celMapFile::file;
  delete[] celMapFile::sectorname;
  celMapFile::file = csStrNew (file);
  celMapFile::sectorname = 0;
}

void celMapFile::SetName (const char* name)
{
  delete[] celMapFile::name;
  celMapFile::name = csStrNew (name);
}

void celMapFile::SetPath (const char* path)
{
  delete[] celMapFile::path;
  celMapFile::path = csStrNew (path);
}

void celMapFile::SetSectorName (const char* name)
{
  delete[] celMapFile::path;
  delete[] celMapFile::file;
  delete[] celMapFile::sectorname;
  celMapFile::path = 0;
  celMapFile::file = 0;
  celMapFile::sectorname = csStrNew (name);
}

//---------------------------------------------------------------------------

void celRegion::SetEntityName (const char* entname)
{
  cscollectionName = entname;
  cscollectionName += "_";
  cscollectionName += name;
}

iCelMapFile* celRegion::CreateMapFile ()
{
  celMapFile* mapfile = new celMapFile ();
  mapfiles.Push (mapfile);
  mapfile->DecRef ();
  return mapfile;
}

bool celRegion::RemoveMapFile (iCelMapFile* mapfile)
{
  size_t idx = mapfiles.Find ((celMapFile*)mapfile);
  if (idx == csArrayItemNotFound) return false;
  mapfiles.DeleteIndex (idx);
  return true;
}

void celRegion::RemoveAllMapFiles ()
{
  mapfiles.SetSize (0);
}

void celRegion::NewEntity (iCelEntity* entity)
{
  // There was an entity attached. This entity is probably
  // created by an addon. We will register this entity as
  // one that needs to be deleted when the region is unloaded.
  entities.Push (entity);
}

void celRegion::NewSector (iEngine*, iSector* sector)
{
  sectors.Add (sector);
}

void celRegion::RemoveSector (iEngine*, iSector*)
{
}

void celRegion::SetCachePath (const char* path)
{
  cache_path = path;
}

bool celRegion::Load (bool allow_entity_addon)
{
  if (loaded) return true;

  iEngine* engine = mgr->GetEngine ();
  csRef<iThreadedLoader> tloader = mgr->GetLoader ();

  iCollection* cur_collection = engine->CreateCollection (cscollectionName.GetData());
  cur_collection->ReleaseAllObjects ();

  iCelPlLayer* pl = mgr->GetPL ();

  // First we register ourselves as a callback to the physical layer so
  // that we get to know about entities created during loading. Those
  // entities are to be registered so we can unload them later.
  pl->AddNewEntityCallback ((iCelNewEntityCallback*)this);
  // We also need a callback to find new sectors.
  engine->AddEngineSectorCallback ((iEngineSectorCallback*)this);

  // If we don't allow the entity addon to work then we mark this here
  // in the physical layer.
  bool prev_allow_entity_addon = false;
  if (!allow_entity_addon)
  {
    prev_allow_entity_addon = pl->IsEntityAddonAllowed ();
    pl->SetEntityAddonAllowed (false);
  }

  // If there is a cache path we use that for the cachemanager.
  if (!cache_path.IsEmpty ())
  {
    mgr->GetVFS ()->PushDir ();
    mgr->GetVFS ()->ChDir (cache_path);
    engine->SetCacheManager (0);
    engine->GetCacheManager ();
    mgr->GetVFS ()->PopDir ();
  }

  size_t i;
  for (i = 0 ; i < mapfiles.GetSize () ; i++)
  {
    celMapFile* mf = mapfiles[i];
    if (mf->GetSectorName ())
    {
      iSector* sector = engine->CreateSector (mf->GetSectorName ());
      cur_collection->Add (sector->QueryObject ());
    }
    else if (mf->GetFile ())
    {
      if (mf->GetPath ())
      {
        mgr->GetVFS ()->PushDir ();
        mgr->GetVFS ()->ChDir (mf->GetPath ());
      }
      if (cache_path.IsEmpty ())
      {
        engine->SetCacheManager (0);
        engine->GetCacheManager ();
      }

printf ("1\n"); fflush (stdout);
      csRef<iThreadReturn> ret = tloader->LoadMapFileWait (mgr->GetVFS()->GetCwd(), mf->GetFile (), false, cur_collection);
printf ("2\n"); fflush (stdout);
      if (mf->GetPath ())
      {
        mgr->GetVFS ()->PopDir ();
      }
      if (!ret->WasSuccessful())
      {
        return false;
      }
    }
    else break;
  }

  // Restore everything.
  pl->RemoveNewEntityCallback ((iCelNewEntityCallback*)this);
  engine->RemoveEngineSectorCallback ((iEngineSectorCallback*)this);
  if (!allow_entity_addon)
    pl->SetEntityAddonAllowed (prev_allow_entity_addon);

  engine->PrecacheDraw (cur_collection);

  if (mgr->IsColliderWrappers ())
  {
    if (!mgr->GetCDSystem ())
    {
      return mgr->Error ("No iCollideSystem plugin!");
    }
    // Create colliders for all meshes in this region.
    csColliderHelper::InitializeCollisionWrappers (mgr->GetCDSystem (),
    	engine, cur_collection);
  }

  mgr->SendZoneMessage ((iCelRegion*)this, "pczonemanager_addregion",
      "cel.region.add", mgr->dispatcher_add);

  loaded = true;
  return true;
}

void celRegion::Unload ()
{
  if (!loaded) return;

  mgr->SendZoneMessage ((iCelRegion*)this, "pczonemanager_remregion",
      "cel.region.remove", mgr->dispatcher_remove);

  iEngine* engine = mgr->GetEngine ();
  iCollection* cur_collection = engine->CreateCollection (cscollectionName);

  iCelPlLayer* pl = mgr->GetPL ();
  if (pl)
  {
    size_t i;
    for (i = 0 ; i < entities.GetSize () ; i++)
      if (entities[i])
        pl->RemoveEntity (entities[i]);

    // We now scan every sector to see if there are entities
    // in that that are not deleted yet. We will delete them here.
    csSet<csRef<iSector> >::GlobalIterator it = sectors.GetIterator ();
    while (it.HasNext ())
    {
      csRef<iSector> s = it.Next ();
      //if (engine->GetSectors()->Find(s) < 0) continue;
      iMeshList* ml = s->GetMeshes ();
      int i;
      for (i = 0 ; i < ml->GetCount () ; i++)
      {
        iMeshWrapper* m = ml->Get (i);
        iCelEntity* e = pl->FindAttachedEntity (m->QueryObject ());
        if (e)
        {
          // Check if the entity is also in other sectors. If so we
          // will not remove it.
          if (m->GetMovable ()->GetSectors ()->GetCount () <= 1)
            pl->RemoveEntity (e);
        }
      }
    }
  }

  entities.DeleteAll ();
  sectors.DeleteAll ();

  cur_collection->ReleaseAllObjects ();
  engine->RemoveCollection (cur_collection);
  loaded = false;
}

void celRegion::AssociateEntity (iCelEntity* entity)
{
  entities.Push (entity);
}

void celRegion::DissociateEntity (iCelEntity* entity)
{
  entities.Delete (entity);
}

iCollection* celRegion::GetCollection ()
{
  iEngine* engine = mgr->GetEngine ();
  return engine->GetCollection (cscollectionName.GetData());
}

bool celRegion::ContainsEntity (iCelEntity* entity)
{
  return (entities.Contains (entity) != csArrayItemNotFound);
}

iCelMapFile* celRegion::FindMapFile (const char* name) const
{
  size_t i;
  for (i = 0; i < GetMapFileCount (); i ++)
    if (!strcmp (name, mapfiles[i]->GetName ()))
      return (iCelMapFile*)mapfiles[i];
  return 0;
}

//---------------------------------------------------------------------------

void celZone::LinkRegion (iCelRegion* region)
{
  size_t idx = regions.Find ((celRegion*)region);
  if (idx != csArrayItemNotFound) return;
  regions.Push ((celRegion*)region);
}

bool celZone::UnlinkRegion (iCelRegion* region)
{
  size_t idx = regions.Find ((celRegion*)region);
  if (idx == csArrayItemNotFound) return false;
  regions.DeleteIndex (idx);
  return true;
}

void celZone::UnlinkAllRegions ()
{
  regions.SetSize (0);
}

iCelRegion* celZone::FindRegion (const char* name) const
{
  size_t i;
  for (i = 0; i < regions.GetSize (); i ++)
    if (!strcmp (name, regions[i]->GetName ()))
      return (iCelRegion*)regions[i];
  return 0;
}

bool celZone::ContainsRegion (celRegion* region)
{
  size_t idx = regions.Find (region);
  return idx != csArrayItemNotFound;
}

//---------------------------------------------------------------------------

csStringID celPcZoneManager::id_region = csInvalidStringID;
csStringID celPcZoneManager::id_path = csInvalidStringID;
csStringID celPcZoneManager::id_file = csInvalidStringID;
csStringID celPcZoneManager::id_entityname = csInvalidStringID;
csStringID celPcZoneManager::id_regionname = csInvalidStringID;
csStringID celPcZoneManager::id_startname = csInvalidStringID;
csStringID celPcZoneManager::id_zonename = csInvalidStringID;
csStringID celPcZoneManager::id_mode = csInvalidStringID;
csStringID celPcZoneManager::id_name = csInvalidStringID;

PropertyHolder celPcZoneManager::propinfo;

celPcZoneManager::celPcZoneManager (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  engine = csQueryRegistry<iEngine> (object_reg);
  if (!engine)
  {
    Error ("No iEngine plugin!");
    return;
  }
  tloader = csQueryRegistry<iThreadedLoader> (object_reg);
  vfs = csQueryRegistry<iVFS> (object_reg);
  if (!vfs)
  {
    Error ("No iVFS plugin!");
    return;
  }
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);

  do_colliderwrappers = true;
  loading_mode = CEL_ZONE_NORMAL;

  if (id_region == csInvalidStringID)
  {
    id_region = pl->FetchStringID ("region");
    id_path = pl->FetchStringID ("path");
    id_file = pl->FetchStringID ("file");
    id_entityname = pl->FetchStringID ("entity");
    id_regionname = pl->FetchStringID ("region");
    id_startname = pl->FetchStringID ("start");
    id_zonename = pl->FetchStringID ("zone");
    id_mode = pl->FetchStringID ("mode");
    id_name = pl->FetchStringID ("name");
  }
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_region);

  InitTokenTable (xmltokens);

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.zone.action.");
    AddAction (action_disablecd, "DisableCD");
    AddAction (action_enablecd, "EnableCD");
    AddAction (action_load, "Load");
    AddAction (action_pointmesh, "PointMesh");
    AddAction (action_pointcamera, "PointCamera");
    AddAction (action_setloadingmode, "SetLoadingMode");
    AddAction (action_activateregion, "ActivateRegion");
    AddAction (action_createregion, "CreateRegion");
    AddAction (action_removeregion, "RemoveRegion");
    AddAction (action_createzone, "CreateZone");
    AddAction (action_removezone, "RemoveZone");
    AddAction (action_createmap, "CreateMap");
    AddAction (action_removemap, "RemoveMap");
    AddAction (action_setcache, "SetCache");
    AddAction (action_linkregion, "LinkRegion");
    AddAction (action_unlinkregion, "UnlinkRegion");
  }

  // For properties.
  propinfo.SetCount (2);
  AddProperty (propid_laststart, "laststart",
  	CEL_DATA_STRING, true, "Last used start location.", 0);
  AddProperty (propid_lastregion, "lastregion",
  	CEL_DATA_STRING, true, "Last used region name.", 0);
}

celPcZoneManager::~celPcZoneManager ()
{
  // Unload everything.
  loading_mode = CEL_ZONE_NORMAL;
  ActivateRegion (0);
  delete params;
}

bool celPcZoneManager::GetPropertyIndexed (int idx, const char*& b)
{
  switch (idx)
  {
    case propid_laststart:
      b = last_startname.GetData ();
      return true;
    case propid_lastregion:
      b = last_regionname.GetData ();
      return true;
    default:
      return false;
  }
}

bool celPcZoneManager::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_disablecd:
      EnableColliderWrappers (false);
      return true;
    case action_enablecd:
      EnableColliderWrappers (true);
      return true;
    case action_activateregion:
      {
	csString regionname;
	if (!Fetch (regionname, params, id_regionname)) return false;
        iCelRegion* region = FindRegion (regionname);
        if (!region) return  Error (
        	"Can't find '%s' region for ActivateRegion!",
        	(const char*)regionname);
        ActivateRegion (region);
        return true;
      }
    case action_load:
      {
	csString path, file;
	if (!Fetch (path, params, id_path, true, "")) return false;
	if (!Fetch (file, params, id_file)) return false;
        if (!Load (path, file))
          return false;
        return true;
      }
    case action_setloadingmode:
      {
	csString mode;
	if (!Fetch (mode, params, id_mode)) return false;
        if (mode == "normal")
        {
          SetLoadingMode (CEL_ZONE_NORMAL);
          return true;
        }
        else if (mode == "keep")
        {
          SetLoadingMode (CEL_ZONE_KEEP);
          return true;
        }
        else if (mode == "loadall")
        {
          SetLoadingMode (CEL_ZONE_LOADALL);
          return true;
        }
        else
          return Error ("Unknown mode '%s' for SetLoadingMode!", mode.GetData ());
      }
    case action_pointmesh:
      {
	csString entityname, regionname, startname;
	if (!Fetch (entityname, params, id_entityname)) return false;
	if (!Fetch (regionname, params, id_regionname)) return false;
	if (!Fetch (startname, params, id_startname)) return false;
        if (PointMesh (entityname, regionname, startname) != CEL_ZONEERROR_OK)
          return Error ("PointMesh failed (entity=%s,region=%s,start=%s)!",
	      (const char*)entityname, (const char*)regionname, (const char*)startname);
        return true;
      }
    case action_pointcamera:
      {
	csString entityname, regionname, startname;
	if (!Fetch (entityname, params, id_entityname)) return false;
	if (!Fetch (regionname, params, id_regionname)) return false;
	if (!Fetch (startname, params, id_startname)) return false;
        if (PointCamera (entityname, regionname, startname) != CEL_ZONEERROR_OK)
          return Error ("PointCamera failed!");
        return true;
      }
    case action_createregion:
      {
	csString regname;
	if (!Fetch (regname, params, id_name)) return false;
        iCelRegion* region = CreateRegion (regname);
        if (!region) return Error ("Can't create region for CreateRegion!");
        return true;
      }
    case action_removeregion:
      {
	csString regname;
	if (!Fetch (regname, params, id_name)) return false;
        iCelRegion* region = FindRegion (regname);
        if (!region) return  Error (
        	"Can't find '%s' region for RemoveRegion!",
        	(const char*)regname);
        return RemoveRegion (region);
      }
    case action_createzone:
      {
	csString zoname;
	if (!Fetch (zoname, params, id_name)) return false;
        iCelZone* zone = CreateZone (zoname);
        if (!zone) return Error (
        	"Can't create zone '%s' for CreateZone!",
        	(const char*)zoname);
        return true;
      }
    case action_removezone:
      {
	csString zoname;
	if (!Fetch (zoname, params, id_name)) return false;
        iCelZone* zone = FindZone (zoname);
        if (!zone) return  Error (
        	"Can't find '%s' zone for RemoveZone!",
        	(const char*)zoname);
        return RemoveZone (zone);
      }
    case action_createmap:
      {
	csString regionname, mname, path, filename;
	if (!Fetch (regionname, params, id_regionname)) return false;
	if (!Fetch (mname, params, id_name)) return false;
	if (!Fetch (path, params, id_path)) return false;
	if (!Fetch (filename, params, id_file)) return false;
        iCelRegion* region = FindRegion (regionname);
        if (!region) return Error (
        	"Can't find '%s' region for CreateMap!",
        	(const char*)regionname);
        iCelMapFile* mapfile = region->CreateMapFile ();
        if (!mapfile) return Error (
        	"Error creating map '%s' for CreateMap!",
        	(const char*)filename);
        mapfile->SetName (mname);
        mapfile->SetPath (path);
        mapfile->SetFile (filename);
        return true;
      }
    case action_removemap:
      {
	csString regionname, mname;
	if (!Fetch (regionname, params, id_regionname)) return false;
	if (!Fetch (mname, params, id_name)) return false;
        iCelRegion* region = FindRegion (regionname);
        if (!region) return Error (
        	"Can't find '%s' region for RemoveMap!",
        	(const char*)regionname);
        iCelMapFile* mapfile = region->FindMapFile (mname);
        if (!mapfile) return Error (
        	"Error searching map '%s' for RemoveMap!",
        	(const char*)mname);
        return region->RemoveMapFile (mapfile);
      }
    case action_setcache:
      {
	csString regionname, path;
	if (!Fetch (regionname, params, id_regionname)) return false;
	if (!Fetch (path, params, id_path)) return false;
        iCelRegion* region = FindRegion (regionname);
        if (!region) return Error (
        	"Can't find '%s' region for SetCache!",
        	(const char*)regionname);
        region->SetCachePath (path);
        return true;
      }
    case action_linkregion:
      {
	csString zonename, regionname;
	if (!Fetch (zonename, params, id_zonename)) return false;
	if (!Fetch (regionname, params, id_regionname)) return false;

        iCelRegion* region = FindRegion (regionname);
        if (!region) return Error (
        	"Can't find '%s' region for LinkRegion!",
        	(const char*)regionname);
        iCelZone* zone = FindZone (zonename);
        if (!zone) return Error (
        	"Can't find '%s' zone for LinkRegion!",
        	(const char*)zonename);
        zone->LinkRegion (region);
        return true;
      }
    case action_unlinkregion:
      {
	csString zonename, regionname;
	if (!Fetch (zonename, params, id_zonename)) return false;
	if (!Fetch (regionname, params, id_regionname)) return false;
        iCelRegion* region = FindRegion (regionname);
        if (!region) return Error (
        	"Can't find '%s' region for UnlinkRegion!",
        	(const char*)regionname);
        iCelZone* zone = FindZone (zonename);
        if (!zone) return Error (
        	"Can't find '%s' zone for UnlinkRegion!",
        	(const char*)zonename);
        zone->UnlinkRegion (region);
        return true;
      }
    default:
      return false;
  }
}

void celPcZoneManager::SendZoneMessage (iCelRegion* region,
    const char* msgidold, const char* msgid,
    csRef<iMessageDispatcher>& dispatcher)
{
  if (!entity) return;
  if (region) params->GetParameter (0).SetIBase (region);
  celData ret;
  if (entity->GetBehaviour ())
    entity->GetBehaviour ()->SendMessage (msgidold, this, ret, params);
  if (!dispatcher)
  {
    dispatcher = entity->QueryMessageChannel ()->
      CreateMessageDispatcher (this, pl->FetchStringID (msgid));
    if (!dispatcher) return;
  }
  dispatcher->SendMessage (params);
}

bool celPcZoneManager::ParseRegion (iDocumentNode* regionnode,
	iCelRegion* region)
{
  csRef<iDocumentNodeIterator> it = regionnode->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_MAP:
        {
          const char* name = child->GetAttributeValue ("name");
          if (!name)
            return Error (
            	"'name' attribute is missing for the map!");
          const char* file = child->GetAttributeValue ("file");
          if (!file)
            return Error (
            	"'file' attribute is missing for the map!");
          const char* path = child->GetAttributeValue ("path");
          iCelMapFile* mapfile = region->CreateMapFile ();
          if (!mapfile)
            return Error ("Error creating map '%s'!", file);
          mapfile->SetName (name);
          mapfile->SetPath (path);
          mapfile->SetFile (file);
        }
        break;
      case XMLTOKEN_CACHE:
        {
          const char* path = child->GetAttributeValue ("path");
          if (!path)
            return Error (
            	"'path' attribute is missing for the cache!");
          region->SetCachePath (path);
        }
        break;
      default:
        return Error ("Unknown token '%s' in the region!", value);
    }
  }

  return true;
}

bool celPcZoneManager::ParseZone (iDocumentNode* zonenode, iCelZone* zone)
{
  csRef<iDocumentNodeIterator> it = zonenode->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_REGION:
        {
          const char* regionname = child->GetContentsValue ();
          if (!regionname)
            return Error ("Region name missing for zone!");
          iCelRegion* region = FindRegion (regionname);
          if (!region)
            return Error ("Can't find region '%s'!", regionname);
          zone->LinkRegion (region);
        }
        break;
      default:
        return Error ("Unknown token '%s' in the zone!", value);
    }
  }

  return true;
}

bool celPcZoneManager::ParseStart (iDocumentNode* startnode)
{
  csRef<iDocumentNodeIterator> it = startnode->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_REGION:
        {
          const char* regionname = child->GetContentsValue ();
          if (!regionname)
            return Error ("Region name missing for start!");
          iCelRegion* region = FindRegion (regionname);
          if (!region)
            return Error ("Can't find region '%s'!", regionname);
          last_regionname = regionname;
        }
        break;
      case XMLTOKEN_NAME:
        {
          const char* startname = child->GetContentsValue ();
          if (!startname)
            return Error ("Name missing for start!");
          last_startname = startname;
        }
        break;
      default:
        return Error ("Unknown token '%s' in the start section!",
        	value);
    }
  }

  region_names.Push (last_regionname);
  start_names.Push (last_startname);

  return true;
}

void celPcZoneManager::SetLoadingMode (int mode)
{
  loading_mode = mode;
  if (loading_mode == CEL_ZONE_LOADALL)
    ActivateRegion (0);
}

bool celPcZoneManager::Load (const char* path, const char* file)
{
  celPcZoneManager::path = path;
  celPcZoneManager::file = file;

  csRef<iDocumentSystem> docsys = 
  	csQueryRegistry<iDocumentSystem> (object_reg);
  if (!docsys)
    docsys.AttachNew (new csTinyDocumentSystem ());
  csRef<iDocument> doc = docsys->CreateDocument ();
  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  if (path)
  {
    vfs->PushDir ();
    vfs->ChDir (path);
  }
  csRef<iDataBuffer> buf = vfs->ReadFile (file);
  if (path)
    vfs->PopDir ();

  if (!buf)
    return Error ("Error opening file '%s'!", file);
  const char* error = doc->Parse (buf, true);
  if (error != 0)
    return Error ("XML parse error for file '%s': %s!",
    	file, error);

  csRef<iDocumentNode> levelnode = doc->GetRoot ()->GetNode ("level");
  if (levelnode)
  {
    bool rc = Load (levelnode);
    if (!rc) return false;
    if (loading_mode == CEL_ZONE_LOADALL)
      if (!ActivateRegion (0))
        return false;
    return true;
  }

  csRef<iDocumentNode> worldnode = doc->GetRoot ()->GetNode ("world");
  if (worldnode)
  {
    // Special case. Here we simulate a simple region and zone that
    // will load the world file.
    iCelRegion* region = CreateRegion ("main");
    iCelMapFile* mapfile = region->CreateMapFile ();
    mapfile->SetPath (path);
    mapfile->SetFile (file);
    iCelZone* zone = CreateZone ("main");
    zone->LinkRegion (region);
    last_regionname = "main";
    last_startname.Empty ();
    if (loading_mode == CEL_ZONE_LOADALL)
      if (!ActivateRegion (0))
        return false;
    return true;
  }

  return Error (
  	"Malformed XML file, 'level' or 'world' node is missing in '%s'!",
  	file);
}

bool celPcZoneManager::Load (iDocumentNode* levelnode)
{
  csRef<iDocumentNodeIterator> it = levelnode->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_MOUNT:
        {
          const char* vfsname = child->GetAttributeValue ("vfs");
          const char* realname = child->GetAttributeValue ("real");
          if (!vfs->Mount (vfsname, realname))
            return Error ("Error mounting '%s' on '%s'!",
            	realname, vfsname);
        }
        break;
      case XMLTOKEN_REGION:
        {
          const char* regionname = child->GetAttributeValue ("name");
          if (!regionname)
            return Error ("Region name missing!");
          iCelRegion* region = CreateRegion (regionname);
          if (!region)
            return Error ("Error creating region '%s'!",
            	regionname);
          if (!ParseRegion (child, region))
            return false;
        }
        break;
      case XMLTOKEN_ZONE:
        {
          const char* zonename = child->GetAttributeValue ("name");
          if (!zonename)
            return Error ("Zone name missing!");
          iCelZone* zone = CreateZone (zonename);
          if (!zone)
            return Error ("Error creating zone '%s'!", zonename);
          if (!ParseZone (child, zone))
            return false;
        }
        break;
      case XMLTOKEN_START:
        if (!ParseStart (child))
          return false;
        break;
      default:
        return Error ("Unknown token '%s' in the level!", value);
    }
  }

  return true;
}

iCelZone* celPcZoneManager::CreateZone (const char* name)
{
  celZone* zone = new celZone (this, name);
  zones.Push (zone);
  zone->DecRef ();
  return zone;
}

bool celPcZoneManager::RemoveZone (iCelZone* zone)
{
  size_t idx = zones.Find ((celZone*)zone);
  if (idx == csArrayItemNotFound) return false;
  zones.DeleteIndex (idx);
  return true;
}

void celPcZoneManager::RemoveAllZones ()
{
  zones.SetSize (0);
}

iCelZone* celPcZoneManager::FindZone (const char* name) const
{
  size_t i;
  for (i = 0 ; i < zones.GetSize () ; i++)
    if (!strcmp (name, zones[i]->GetName ()))
      return (iCelZone*)zones[i];
  return 0;
}

iCelRegion* celPcZoneManager::CreateRegion (const char* name)
{
  celRegion* region = new celRegion (this, name);
  regions.Push (region);
  region->SetEntityName (entity->GetName ());
  region->DecRef ();
  return region;
}

iCelRegion* celPcZoneManager::FindRegion (const char* name) const
{
  size_t i;
  for (i = 0 ; i < regions.GetSize () ; i++)
    if (!strcmp (name, regions[i]->GetName ()))
      return (iCelRegion*)regions[i];
  return 0;
}

bool celPcZoneManager::RemoveRegion (iCelRegion* region)
{
  size_t idx = regions.Find ((celRegion*)region);
  if (idx == csArrayItemNotFound) return false;
  regions.DeleteIndex (idx);
  return true;
}

void celPcZoneManager::RemoveAllRegions ()
{
  regions.SetSize (0);
}

bool celPcZoneManager::ActivateRegion (iCelRegion* region,
	bool allow_entity_addon)
{
  // The 'first' flag is used to see if we need to do some loading
  // work. In that case we send a message to the behaviour layer so that
  // it may possible open up a loading screen or something.
  bool first = true;
  size_t i;

  // First we make a set of all regions that we need to load.
  csSet<csPtrKey<celRegion> > loadable_regions;
  for (i = 0 ; i < zones.GetSize () ; i++)
    if (loading_mode == CEL_ZONE_LOADALL
    	|| zones[i]->ContainsRegion ((celRegion*) region))
    {
      size_t j;
      for (j = 0 ; j < zones[i]->GetRegionCount () ; j++)
        loadable_regions.Add ((celRegion*)zones[i]->GetRegion (j));
    }

  celRegion* r;
  for (i = 0 ; i < regions.GetSize () ; i++)
  {
    r = regions[i];
    if (loadable_regions.In (r))
    {
      if (!r->IsLoaded ())
      {
        if (first)
        {
          first = false;
          SendZoneMessage (0, "pczonemanager_startloading",
	      "cel.region.load.start", dispatcher_loadstart);
        }
        if (!r->Load (allow_entity_addon))
        {
          SendZoneMessage ((iCelRegion*)r, "pczonemanager_errorloading",
	      "cel.region.load.error", dispatcher_loaderror);
          return false;
        }
      }
    }
    else
    {
      if (loading_mode == CEL_ZONE_NORMAL)
        r->Unload ();
    }
  }

  // If first is false that means we sent a message indiciating that loading
  // started. So we have to send a message indicating that loading finished
  // too.
  if (!first)
    SendZoneMessage (0, "pczonemanager_stoploading",
	"cel.region.load.stop", dispatcher_loadstop);

  return true;
}

iCelRegion* celPcZoneManager::FindRegionContaining (iCelEntity* ent)
{
  celRegion* r;
  for (size_t i = 0 ; i < regions.GetSize () ; i++)
  {
    r = regions[i];
    if (r->ContainsEntity (ent))
    {
      return r;
    }
  }
  return 0;
}

bool celPcZoneManager::ActivateSector (iSector* sector)
{
  if (active_sector == sector) return true;	// Nothing to do.
  active_sector = sector;
  size_t i;
  for (i = 0 ; i < regions.GetSize () ; i++)
  {
    if (regions[i]->ContainsSector (sector))
      return ActivateRegion (regions[i]);
  }
  return true;
}

void celPcZoneManager::FindStartLocations (iStringArray* regionnames,
	iStringArray* startnames)
{
  regionnames->Empty ();
  startnames->Empty ();
  size_t i;
  for (i = 0 ; i < region_names.GetSize () ; i++)
  {
    regionnames->Push (region_names.Get (i));
    startnames->Push (start_names.Get (i));
  }
}

void celPcZoneManager::GetLastStartLocation (iString* regionname,
	iString* startname)
{
  regionname->Truncate (0);
  regionname->Append (last_regionname);
  startname->Truncate (0);
  startname->Append (last_startname);
}

const char *celPcZoneManager::GetLastStartRegionName ()
{
  return last_regionname;
}

const char *celPcZoneManager::GetLastStartName ()
{
  return last_startname;
}

int celPcZoneManager::PointCamera (const char* entity, const char* regionname,
	const char* startname)
{
  camera_entity = entity;

  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return CEL_ZONEERROR_OK;
  csRef<iPcCamera> pccamera = celQueryPropertyClassEntity<iPcCamera> (ent);
  if (!pccamera) return CEL_ZONEERROR_OK;

  if (!camlistener)
    camlistener.AttachNew (new cameraSectorListener (this));
  if (celPcZoneManager::pccamera)
    celPcZoneManager::pccamera->GetCamera ()->RemoveCameraListener (camlistener);
  celPcZoneManager::pccamera = pccamera;
  // If there is a pcmesh then we use the mesh movable listener instead of
  // the camera listener.
  if (!pcmesh)
    pccamera->GetCamera ()->AddCameraListener (camlistener);

  iCelRegion* region = FindRegion (regionname);
  if (!region) return CEL_ZONEERROR_BADREGION;
  if (!ActivateRegion (region))
    return CEL_ZONEERROR_LOAD;

  // Find the created region.
  iCollection* cur_collection = engine->CreateCollection (region->GetCsCollectionName ());

  // Find the right start position.
  iCameraPosition* campos = 0;
  if (startname)
  {
    campos = cur_collection->FindCameraPosition (startname);
    if (!campos) return CEL_ZONEERROR_BADSTART;
    if (!campos->Load (pccamera->GetCamera (), engine))
      return CEL_ZONEERROR_LOAD;
  }
  else
  {
    int i;
    for (i = 0 ; i < engine->GetCameraPositions ()->GetCount () ; i++)
    {
      campos = engine->GetCameraPositions ()->Get (i);
      iObject* o = campos->QueryObject ();
      if ((o->GetName () == 0 || !strcmp (o->GetName (), "Start"))
      	&& cur_collection->IsParentOf (o))
        break;
      campos = 0;
    }
    if (campos)
    {
      if (!campos->Load (pccamera->GetCamera (), engine))
        return CEL_ZONEERROR_LOAD;
    }
    else
    {
      // We don't have a valid camera position. Just take one of the
      // sectors in this region and use position 0,0,0. We assume that
      // in this case the user of this function will set the position
      // manually to a better place.
      iSectorList* sl = engine->GetSectors ();
      iSector* room = 0;
      for (i = 0 ; i < sl->GetCount () ; i++)
        if (cur_collection->IsParentOf (sl->Get (i)->QueryObject ()))
        {
          room = sl->Get (i);
          break;
        }
      if (!room)
        return CEL_ZONEERROR_BADSTART;
      pccamera->GetCamera ()->SetSector (room);
      pccamera->GetCamera ()->GetTransform ().SetOrigin (csVector3 (0,0,0));
    }
  }

  last_regionname = regionname;
  last_startname = startname;

  return CEL_ZONEERROR_OK;
}

int celPcZoneManager::PointMesh (const char* entity, const char* regionname,
	const char* startname)
{
  mesh_entity = entity;

  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return CEL_ZONEERROR_OK;
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (ent);
  if (!pcmesh) return CEL_ZONEERROR_OK;

  if (!meshlistener)
    meshlistener.AttachNew (new meshmoveListener (this));
  if (celPcZoneManager::pcmesh)
    celPcZoneManager::pcmesh->GetMesh ()->GetMovable ()
    	->RemoveListener (meshlistener);
  celPcZoneManager::pcmesh = pcmesh;
  pcmesh->GetMesh ()->GetMovable ()->AddListener (meshlistener);
  // If there is a pcmesh then we use the mesh movable listener instead of
  // the camera listener.
  if (celPcZoneManager::pccamera && camlistener)
    celPcZoneManager::pccamera->GetCamera ()->RemoveCameraListener (camlistener);

  iCelRegion* region = FindRegion (regionname);
  if (!region) return CEL_ZONEERROR_BADREGION;
  if (!ActivateRegion ((celRegion*)region))
    return CEL_ZONEERROR_LOAD;

  // Find the created region.
  iCollection* cur_collection = engine->CreateCollection (region->GetCsCollectionName ());

  // Find the right start position.
  iCameraPosition* campos = 0;
  iSector* sector;
  csVector3 pos;
  if (startname)
  {
    campos = cur_collection->FindCameraPosition (startname);
    if (!campos) return CEL_ZONEERROR_BADSTART;
    sector = engine->FindSector (campos->GetSector ());
    pos = campos->GetPosition ();
  }
  else
  {
    int i;
    for (i = 0 ; i < engine->GetCameraPositions ()->GetCount () ; i++)
    {
      campos = engine->GetCameraPositions ()->Get (i);
      iObject* o = campos->QueryObject ();
      if ((o->GetName () == 0 || !strcmp (o->GetName (), "Start"))
      	&& cur_collection->IsParentOf (o))
        break;
      campos = 0;
    }
    if (campos)
    {
      sector = engine->FindSector (campos->GetSector ());
      pos = campos->GetPosition ();
    }
    else
    {
      // We don't have a valid camera position. Just take one of the
      // sectors in this region and use position 0,0,0. We assume that
      // in this case the user of this function will set the position
      // manually to a better place.
      iSectorList* sl = engine->GetSectors ();
      sector = 0;
      for (i = 0 ; i < sl->GetCount () ; i++)
        if (cur_collection->IsParentOf (sl->Get (i)->QueryObject ()))
        {
          sector = sl->Get (i);
          break;
        }
      pos.Set (0, 0, 0);
    }
  }
  if (!sector)
    return CEL_ZONEERROR_BADSTART;
  pcmesh->GetMesh ()->GetMovable ()->SetSector (sector);
  pcmesh->GetMesh ()->GetMovable ()->SetPosition (pos);
  pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();

  last_regionname = regionname;
  last_startname = startname;

  return CEL_ZONEERROR_OK;
}

//---------------------------------------------------------------------------

