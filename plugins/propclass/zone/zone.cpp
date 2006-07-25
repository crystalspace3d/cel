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
#include "csutil/debug.h"
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
#include "iengine/region.h"
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

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (ZoneManager, "pczonemanager")

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistence",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

//---------------------------------------------------------------------------

void cameraSectorListener::NewSector (iCamera* /*camera*/, iSector* sector)
{
  zonemgr->ActivateSector (sector);
}

SCF_IMPLEMENT_IBASE (cameraSectorListener)
  SCF_IMPLEMENTS_INTERFACE (iCameraSectorListener)
SCF_IMPLEMENT_IBASE_END

void meshmoveListener::MovableChanged (iMovable* movable)
{
  if (movable->GetSectors ()->GetCount () > 0)
    zonemgr->ActivateSector (movable->GetSectors ()->Get (0));
}

SCF_IMPLEMENT_IBASE (meshmoveListener)
  SCF_IMPLEMENTS_INTERFACE (iMovableListener)
SCF_IMPLEMENT_IBASE_END

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celMapFile)
  SCF_IMPLEMENTS_INTERFACE (iCelMapFile)
SCF_IMPLEMENT_IBASE_END

void celMapFile::SetFile (const char* file)
{
  delete[] celMapFile::file;
  delete[] celMapFile::sectorname;
  celMapFile::file = csStrNew (file);
  celMapFile::sectorname = 0;
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

SCF_IMPLEMENT_IBASE (celRegion)
  SCF_IMPLEMENTS_INTERFACE (iCelRegion)
  SCF_IMPLEMENTS_INTERFACE (iCelNewEntityCallback)
  SCF_IMPLEMENTS_INTERFACE (iEngineSectorCallback)
SCF_IMPLEMENT_IBASE_END

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
  mapfiles.SetLength (0);
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
  iLoader* loader = mgr->GetLoader ();
  iRegion* cur_region = engine->CreateRegion (name);
  cur_region->DeleteAll ();

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
  bool rc = true;
  for (i = 0 ; i < mapfiles.Length () ; i++)
  {
    celMapFile* mf = mapfiles[i];
    if (mf->GetSectorName ())
    {
      iSector* sector = engine->CreateSector (mf->GetSectorName ());
      cur_region->Add (sector->QueryObject ());
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
      rc = loader->LoadMapFile (mf->GetFile (), false, cur_region,
      	false, true);
      if (mf->GetPath ())
      {
        mgr->GetVFS ()->PopDir ();
      }
      if (!rc)
        return false;
    }
    else break;
  }

  // Restore everything.
  pl->RemoveNewEntityCallback ((iCelNewEntityCallback*)this);
  engine->RemoveEngineSectorCallback ((iEngineSectorCallback*)this);
  if (!allow_entity_addon)
    pl->SetEntityAddonAllowed (prev_allow_entity_addon);
  if (!rc) return false;

  cur_region->Prepare ();
  engine->PrecacheDraw (cur_region);

  if (mgr->IsColliderWrappers ())
  {
    // Create colliders for all meshes in this region.
    csColliderHelper::InitializeCollisionWrappers (mgr->GetCDSystem (),
  	engine, cur_region);
  }

  mgr->SendZoneMessage ((iCelRegion*)this, "pczonemanager_addregion");

  loaded = true;
  return true;
}

void celRegion::Unload ()
{
  if (!loaded) return;

  mgr->SendZoneMessage ((iCelRegion*)this, "pczonemanager_remregion");

  iEngine* engine = mgr->GetEngine ();
  iRegion* cur_region = engine->CreateRegion (name);

  iCelPlLayer* pl = mgr->GetPL ();
  if (pl)
  {
    size_t i;
    for (i = 0 ; i < entities.Length () ; i++)
      if (entities[i])
        pl->RemoveEntity (entities[i]);

    // We now scan every sector to see if there are entities
    // in that that are not deleted yet. We will delete them here.
    csSet<csPtrKey<iSector> >::GlobalIterator it = sectors.GetIterator ();
    while (it.HasNext ())
    {
      iSector* s = it.Next ();
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

  cur_region->DeleteAll ();
  engine->GetRegions ()->Remove (cur_region);
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

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celZone)
  SCF_IMPLEMENTS_INTERFACE (iCelZone)
SCF_IMPLEMENT_IBASE_END

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
  regions.SetLength (0);
}

iCelRegion* celZone::FindRegion (const char* name) const
{
  size_t i;
  for (i = 0 ; i < regions.Length () ; i++)
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
csStringID celPcZoneManager::action_disablecd = csInvalidStringID;
csStringID celPcZoneManager::action_enablecd = csInvalidStringID;
csStringID celPcZoneManager::action_load = csInvalidStringID;
csStringID celPcZoneManager::id_path = csInvalidStringID;
csStringID celPcZoneManager::id_file = csInvalidStringID;
csStringID celPcZoneManager::action_pointmesh = csInvalidStringID;
csStringID celPcZoneManager::action_pointcamera = csInvalidStringID;
csStringID celPcZoneManager::id_entityname = csInvalidStringID;
csStringID celPcZoneManager::id_regionname = csInvalidStringID;
csStringID celPcZoneManager::id_startname = csInvalidStringID;
csStringID celPcZoneManager::action_setloadingmode = csInvalidStringID;
csStringID celPcZoneManager::id_mode = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcZoneManager)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcZoneManager)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcZoneManager::PcZoneManager)
  SCF_IMPLEMENTS_INTERFACE (iPcZoneManager)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcZoneManager::celPcZoneManager (iObjectRegistry* object_reg)
  : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcZoneManager);
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);

  do_colliderwrappers = true;
  loading_mode = CEL_ZONE_NORMAL;

  if (id_region == csInvalidStringID)
  {
    id_region = pl->FetchStringID ("cel.parameter.region");
    action_disablecd = pl->FetchStringID ("cel.action.DisableCD");
    action_enablecd = pl->FetchStringID ("cel.action.EnableCD");
    action_load = pl->FetchStringID ("cel.action.Load");
    id_path = pl->FetchStringID ("cel.parameter.path");
    id_file = pl->FetchStringID ("cel.parameter.file");
    action_pointmesh = pl->FetchStringID ("cel.action.PointMesh");
    action_pointcamera = pl->FetchStringID ("cel.action.PointCamera");
    id_entityname = pl->FetchStringID ("cel.parameter.entity");
    id_regionname = pl->FetchStringID ("cel.parameter.region");
    id_startname = pl->FetchStringID ("cel.parameter.start");
    action_setloadingmode = pl->FetchStringID ("cel.action.SetLoadingMode");
    id_mode = pl->FetchStringID ("cel.parameter.mode");
  }
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_region, "region");

  InitTokenTable (xmltokens);
 
  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_laststart] = 0;
  propdata[propid_lastregion] = 0;
}

celPcZoneManager::~celPcZoneManager ()
{
  // Unload everything.
  loading_mode = CEL_ZONE_NORMAL;
  ActivateRegion (0);
  delete params;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcZoneManager);
}

Property* celPcZoneManager::properties = 0;
size_t celPcZoneManager::propertycount = 0;

void celPcZoneManager::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    propertycount = 2;
    properties = new Property[propertycount];

    properties[propid_laststart].id = pl->FetchStringID (
    	"cel.property.laststart");
    properties[propid_laststart].datatype = CEL_DATA_STRING;
    properties[propid_laststart].readonly = true;
    properties[propid_laststart].desc = "Last used start location.";

    properties[propid_lastregion].id = pl->FetchStringID (
    	"cel.property.lastregion");
    properties[propid_lastregion].datatype = CEL_DATA_STRING;
    properties[propid_lastregion].readonly = true;
    properties[propid_lastregion].desc = "Last used region name.";
  }
}

const char* celPcZoneManager::GetPropertyString (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_laststart].id)
  {
    return last_startname.GetData ();
  }
  else if (propertyId == properties[propid_lastregion].id)
  {
    return last_regionname.GetData ();
  }
  else
  {
    return celPcCommon::GetPropertyString (propertyId);
  }
}

#define ZONEMANAGER_SERIAL 2

csPtr<iCelDataBuffer> celPcZoneManager::SaveFirstPass ()
{
  // @@@ Currently persistence for zone manager only works when
  // the zone manager was setup using Load(path,file) function.
  if (file.IsEmpty ())
  {
    Report (object_reg, "Currently we only support saving of zone manager when the zone manager is loaded using Load(path,file)!");
    return 0;
  }

  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (ZONEMANAGER_SERIAL);
  databuf->Add (do_colliderwrappers);
  databuf->Add ((uint16)loading_mode);
  databuf->Add (camera_entity);
  databuf->Add (mesh_entity);
  databuf->Add (last_regionname);
  databuf->Add (last_startname);
  databuf->Add (path);
  databuf->Add (file);
  size_t i;
  for (i = 0 ; i < regions.Length () ; i++)
  {
    celRegion* r = regions[i];
    if (r->IsLoaded ())
    {
      databuf->Add (r->GetName ());
    }
  }
  databuf->Add ((const char*)0);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcZoneManager::LoadFirstPass (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != ZONEMANAGER_SERIAL)
    return Report (object_reg, "serialnr != ZONEMANAGER_SERIAL.  Cannot load.");

  iString* s;

  do_colliderwrappers = databuf->GetBool ();
  loading_mode = databuf->GetUInt16 ();

  s = databuf->GetString ();
  camera_entity.Empty ();
  if (s) camera_entity = s->GetData ();

  s = databuf->GetString ();
  mesh_entity.Empty ();
  if (s) mesh_entity = s->GetData ();

  s = databuf->GetString ();
  last_regionname.Empty ();
  if (s) last_regionname = s->GetData ();

  s = databuf->GetString ();
  last_startname.Empty ();
  if (s) last_startname = s->GetData ();

  csString p, f;
  s = databuf->GetString ();
  if (s) p = s->GetData ();
  s = databuf->GetString ();
  if (s) f = s->GetData ();
  if (!Load (p, f))
    return false;

  s = databuf->GetString ();
  while (s && s->GetData ())
  {
    iCelRegion* r = FindRegion (s->GetData ());
    ActivateRegion (r, false);
    s = databuf->GetString ();
  }

  return true;
}

csPtr<iCelDataBuffer> celPcZoneManager::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (ZONEMANAGER_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcZoneManager::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != ZONEMANAGER_SERIAL)
    return Report (object_reg, "serialnr != ZONEMANAGER_SERIAL.  Cannot load.");

  if (!mesh_entity.IsEmpty ())
    if (PointMesh (mesh_entity, last_regionname, last_startname)
    	!= CEL_ZONEERROR_OK)
      return false;
  if (!camera_entity.IsEmpty ())
    if (PointCamera (camera_entity, last_regionname, last_startname)
    	!= CEL_ZONEERROR_OK)
      return false;

  return true;
}

bool celPcZoneManager::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_disablecd)
  {
    EnableColliderWrappers (false);
    return true;
  }
  else if (actionId == action_enablecd)
  {
    EnableColliderWrappers (true);
    return true;
  }
  else if (actionId == action_load)
  {
    CEL_FETCH_STRING_PAR (path,params,id_path);
    CEL_FETCH_STRING_PAR (file,params,id_file);
    if (!p_file) return false;
    if (!Load (path, file))
      return false;
    return true;
  }
  else if (actionId == action_setloadingmode)
  {
    CEL_FETCH_STRING_PAR (mode,params,id_mode);
    if (!p_mode)
      return Report (object_reg,
	    	"'mode' is missing for SetLoadingMode!");
    if (!strcmp ("normal", mode))
    {
      SetLoadingMode (CEL_ZONE_NORMAL);
      return true;
    }
    else if (!strcmp ("keep", mode))
    {
      SetLoadingMode (CEL_ZONE_KEEP);
      return true;
    }
    else if (!strcmp ("loadall", mode))
    {
      SetLoadingMode (CEL_ZONE_LOADALL);
      return true;
    }
    else
      return Report (object_reg,
	    	"Unknown mode '%s' for SetLoadingMode!", mode);
  }
  else if (actionId == action_pointmesh)
  {
    CEL_FETCH_STRING_PAR (entityname,params,id_entityname);
    if (!p_entityname) return false;
    CEL_FETCH_STRING_PAR (regionname,params,id_regionname);
    if (!p_regionname) return false;
    CEL_FETCH_STRING_PAR (startname,params,id_startname);
    if (!p_startname) return false;
    if (!PointMesh (entityname, regionname, startname))
      return false;
    return true;
  }
  else if (actionId == action_pointcamera)
  {
    CEL_FETCH_STRING_PAR (entityname,params,id_entityname);
    if (!p_entityname) return false;
    CEL_FETCH_STRING_PAR (regionname,params,id_regionname);
    if (!p_regionname) return false;
    CEL_FETCH_STRING_PAR (startname,params,id_startname);
    if (!p_startname) return false;
    if (!PointCamera (entityname, regionname, startname))
      return false;
    return true;
  }
  return false;
}

void celPcZoneManager::SendZoneMessage (iCelRegion* region, const char* msgid)
{
  if (region) params->GetParameter (0).SetIBase (region);
  celData ret;
  if (entity->GetBehaviour ())
    entity->GetBehaviour ()->SendMessage (msgid, this, ret, params);
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
	  const char* file = child->GetAttributeValue ("file");
	  if (!file)
            return Report (object_reg,
	    	"'file' attribute is missing for the map!");
	  const char* path = child->GetAttributeValue ("path");
	  iCelMapFile* mapfile = region->CreateMapFile ();
          if (!mapfile)
            return Report (object_reg, "Error creating map '%s'!", file);
	  mapfile->SetPath (path);
	  mapfile->SetFile (file);
	}
        break;
      case XMLTOKEN_CACHE:
        {
	  const char* path = child->GetAttributeValue ("path");
	  if (!path)
            return Report (object_reg,
	    	"'path' attribute is missing for the cache!");
	  region->SetCachePath (path);
        }
        break;
      default:
        return Report (object_reg, "Unknown token '%s' in the region!", value);
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
            return Report (object_reg, "Region name missing for zone!");
          iCelRegion* region = FindRegion (regionname);
          if (!region)
            return Report (object_reg, "Can't find region '%s'!", regionname);
	  zone->LinkRegion (region);
	}
        break;
      default:
        return Report (object_reg, "Unknown token '%s' in the zone!", value);
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
            return Report (object_reg, "Region name missing for start!");
          iCelRegion* region = FindRegion (regionname);
          if (!region)
            return Report (object_reg, "Can't find region '%s'!", regionname);
	  last_regionname = regionname;
	}
        break;
      case XMLTOKEN_NAME:
	{
	  const char* startname = child->GetContentsValue ();
	  if (!startname)
            return Report (object_reg, "Name missing for start!");
	  last_startname = startname;
	}
        break;
      default:
        return Report (object_reg, "Unknown token '%s' in the start section!",
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

  csRef<iDocumentSystem> docsys = CS_QUERY_REGISTRY (object_reg,
  	iDocumentSystem);
  if (!docsys)
    docsys.AttachNew (new csTinyDocumentSystem ());
  csRef<iDocument> doc = docsys->CreateDocument ();
  csRef<iVFS> vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  if (path)
  {
    vfs->PushDir ();
    vfs->ChDir (path);
  }
  csRef<iDataBuffer> buf = vfs->ReadFile (file);
  if (path)
    vfs->PopDir ();

  if (!buf)
    return Report (object_reg, "Error opening file '%s'!", file);
  const char* error = doc->Parse (buf, true);
  if (error != 0)
    return Report (object_reg, "XML parse error for file '%s': %s!",
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

  return Report (object_reg,
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
	    return Report (object_reg, "Error mounting '%s' on '%s'!",
	    	realname, vfsname);
	}
	break;
      case XMLTOKEN_REGION:
	{
	  const char* regionname = child->GetAttributeValue ("name");
	  if (!regionname)
            return Report (object_reg, "Region name missing!");
          iCelRegion* region = CreateRegion (regionname);
          if (!region)
            return Report (object_reg, "Error creating region '%s'!",
	    	regionname);
	  if (!ParseRegion (child, region))
	    return false;
	}
        break;
      case XMLTOKEN_ZONE:
        {
	  const char* zonename = child->GetAttributeValue ("name");
	  if (!zonename)
            return Report (object_reg, "Zone name missing!");
          iCelZone* zone = CreateZone (zonename);
          if (!zone)
            return Report (object_reg, "Error creating zone '%s'!", zonename);
	  if (!ParseZone (child, zone))
	    return false;
	}
        break;
      case XMLTOKEN_START:
	if (!ParseStart (child))
	  return false;
        break;
      default:
        return Report (object_reg, "Unknown token '%s' in the level!", value);
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
  zones.SetLength (0);
}

iCelZone* celPcZoneManager::FindZone (const char* name) const
{
  size_t i;
  for (i = 0 ; i < zones.Length () ; i++)
    if (!strcmp (name, zones[i]->GetName ()))
      return (iCelZone*)zones[i];
  return 0;
}

iCelRegion* celPcZoneManager::CreateRegion (const char* name)
{
  celRegion* region = new celRegion (this, name);
  regions.Push (region);
  region->DecRef ();
  return region;
}

iCelRegion* celPcZoneManager::FindRegion (const char* name) const
{
  size_t i;
  for (i = 0 ; i < regions.Length () ; i++)
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
  regions.SetLength (0);
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
  for (i = 0 ; i < zones.Length () ; i++)
    if (loading_mode == CEL_ZONE_LOADALL
	|| zones[i]->ContainsRegion ((celRegion*) region))
    {
      size_t j;
      for (j = 0 ; j < zones[i]->GetRegionCount () ; j++)
        loadable_regions.Add ((celRegion*)zones[i]->GetRegion (j));
    }

  for (i = 0 ; i < regions.Length () ; i++)
  {
    celRegion* r = regions[i];
    if (loadable_regions.In (r))
    {
      if (!r->IsLoaded ())
      {
        if (first)
	{
	  first = false;
	  SendZoneMessage (0, "pczonemanager_startloading");
	}
	if (!r->Load (allow_entity_addon))
	{
	  SendZoneMessage ((iCelRegion*)r, "pczonemanager_errorloading");
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
    SendZoneMessage (0, "pczonemanager_stoploading");

  return true;
}

bool celPcZoneManager::ActivateSector (iSector* sector)
{
  if (active_sector == sector) return true;	// Nothing to do.
  active_sector = sector;
  size_t i;
  for (i = 0 ; i < regions.Length () ; i++)
  {
    if (regions[i]->ContainsSector (sector))
      return ActivateRegion (regions[i]);
  }
  return true;
}

void celPcZoneManager::FindStartLocations (iStringArray* regionnames,
	iStringArray* startnames)
{
  regionnames->DeleteAll ();
  startnames->DeleteAll ();
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
  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (ent, iPcCamera);
  if (!pccamera) return CEL_ZONEERROR_OK;

  if (!camlistener)
    camlistener.AttachNew (new cameraSectorListener (this));
  if (celPcZoneManager::pccamera)
    celPcZoneManager::pccamera->GetCamera ()
    	->RemoveCameraSectorListener (camlistener);
  celPcZoneManager::pccamera = pccamera;
  // If there is a pcmesh then we use the mesh movable listener instead of
  // the camera listener.
  if (!pcmesh)
    pccamera->GetCamera ()->AddCameraSectorListener (camlistener);

  iCelRegion* region = FindRegion (regionname);
  if (!region) return CEL_ZONEERROR_BADREGION;
  if (!ActivateRegion (region))
    return CEL_ZONEERROR_LOAD;

  // Find the created region.
  iRegion* cur_region = engine->CreateRegion (regionname);

  // Find the right start position.
  iCameraPosition* campos = 0;
  if (startname)
  {
    campos = cur_region->FindCameraPosition (startname);
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
      		&& cur_region->IsInRegion (o))
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
        if (cur_region->IsInRegion (sl->Get (i)->QueryObject ()))
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
  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (ent, iPcMesh);
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
    celPcZoneManager::pccamera->GetCamera ()
    	->RemoveCameraSectorListener (camlistener);

  iCelRegion* region = FindRegion (regionname);
  if (!region) return CEL_ZONEERROR_BADREGION;
  if (!ActivateRegion ((celRegion*)region))
    return CEL_ZONEERROR_LOAD;

  // Find the created region.
  iRegion* cur_region = engine->CreateRegion (regionname);

  // Find the right start position.
  iCameraPosition* campos = 0;
  iSector* sector;
  csVector3 pos;
  if (startname)
  {
    campos = cur_region->FindCameraPosition (startname);
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
      		&& cur_region->IsInRegion (o))
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
        if (cur_region->IsInRegion (sl->Get (i)->QueryObject ()))
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

