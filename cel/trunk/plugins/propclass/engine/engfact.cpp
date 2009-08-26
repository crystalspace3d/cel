/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2005 by Jorrit Tyberghein

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
#include "propclass/solid.h"
#include "propclass/zone.h"
#include "propclass/camera.h"
#include "plugins/propclass/engine/engfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/flags.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "iutil/virtclk.h"
#include "iutil/csinput.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
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
#include "ivaria/view.h"
#include "ivaria/collider.h"
#include "ivaria/reporter.h"
#include "ivideo/graph3d.h"
#include "csqsqrt.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY_ALT (Region, "pcworld.region", "pcregion")

void EngReport (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.pcworld.region",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//---------------------------------------------------------------------------

PropertyHolder celPcRegion::propinfo;

celPcRegion::celPcRegion (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  propholder = &propinfo;

  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.region.action.");
    AddAction (action_load, "Load");
    AddAction (action_unload, "Unload");
  }

  propinfo.SetCount (3);
  AddProperty (propid_worlddir, "worlddir",
  	CEL_DATA_STRING, false, "Map VFS path.", &worlddir);
  AddProperty (propid_worldfile, "worldfile",
  	CEL_DATA_STRING, false, "Map VFS file name.", &worldfile);
  AddProperty (propid_regionname, "regionname",
  	CEL_DATA_STRING, false, "Name of this region.", &regionname);

  worlddir = 0;
  worldfile = 0;
  regionname = 0;
  loaded = false;
  empty_sector = true;
}

celPcRegion::~celPcRegion ()
{
  Unload ();
  delete[] worlddir;
  delete[] worldfile;
  delete[] regionname;
}

#define REGION_SERIAL 2

csPtr<iCelDataBuffer> celPcRegion::SaveFirstPass ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (REGION_SERIAL);

  databuf->Add (empty_sector);
  databuf->Add (worlddir);
  databuf->Add (worldfile);
  databuf->Add (regionname);
  databuf->Add (loaded);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcRegion::LoadFirstPass (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != REGION_SERIAL)
  {
    EngReport (object_reg, "serialnr != REGION_SERIAL.  Cannot load.");
    return false;
  }

  Unload ();
  delete[] worlddir; worlddir = 0;
  delete[] worldfile; worldfile = 0;
  delete[] regionname; regionname = 0;

  empty_sector = databuf->GetBool ();
  worlddir = csStrNew (databuf->GetString ()->GetData ());
  worldfile = csStrNew (databuf->GetString ()->GetData ());
  regionname = csStrNew (databuf->GetString ()->GetData ());
  bool load = databuf->GetBool ();

  if (load && !Load (false))
  {
    EngReport (object_reg,
    	"Could not load the specified map into the region.  Cannot load.");
    return false;
  }

  return true;
}

csPtr<iCelDataBuffer> celPcRegion::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (REGION_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcRegion::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != REGION_SERIAL)
  {
    EngReport (object_reg, "serialnr != REGION_SERIAL.  Cannot load.");
    return false;
  }
  return true;
}

bool celPcRegion::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_load:
    {
      if ((empty_sector || worldfile) && regionname)
        Load ();
      else
      {
        printf ("World filename or region name not set!\n");
        return false;
      }
      return true;
    }
    case action_unload:
    {
      if ((empty_sector || worldfile) && regionname)
        Unload ();
      else
      {
        printf ("World filename or region name not set!\n");
        return false;
      }
      return true;
    }
    default:
      return false;
  }
}

void celPcRegion::CreateEmptySector (const char* name)
{
  empty_sector = true;
  delete[] worldfile;
  worldfile = csStrNew (name);
}

void celPcRegion::SetWorldFile (const char* vfsdir, const char* name)
{
  empty_sector = false;
  if (worlddir != vfsdir)
  {
    delete[] worlddir;
    worlddir = csStrNew (vfsdir);
  }
  if (worldfile != name)
  {
    delete[] worldfile;
    worldfile = csStrNew (name);
  }
}

void celPcRegion::SetRegionName (const char* name)
{
  if (regionname != name)
  {
    delete[] regionname;
    regionname = csStrNew (name);
  }
}

void celPcRegion::NewEntity (iCelEntity* entity)
{
  // There was an entity attached. This entity is probably
  // created by an addon. We will register this entity as
  // one that needs to be deleted when the region is unloaded.
  entities.Push (entity);
}

bool celPcRegion::Load (bool allow_entity_addon)
{
  if (loaded)
  {
    EngReport (object_reg,"Entity '%s' already loaded.", entity->GetName ());
    return true;
  }
  if (!empty_sector && !worlddir)
  {
    EngReport (object_reg, "World dir not specified.");
    return false;
  }
  if (!worldfile)
  {
    EngReport (object_reg, "World file not specified.");
    return false;
  }
  if (!regionname)
  {
    EngReport (object_reg, "Region name not specified.");
    return false;
  }

  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
  CS_ASSERT (engine != 0);
  iCollection* cur_collection = engine->CreateCollection (regionname);
  cur_collection->ReleaseAllObjects ();

  if (empty_sector)
  {
    iSector* sector = engine->CreateSector (worldfile);
    cur_collection->Add (sector->QueryObject ());
    loaded = true;
    return true;
  }

  csRef<iThreadedLoader> tloader = csQueryRegistry<iThreadedLoader> (object_reg);
  csRef<iVFS> VFS = csQueryRegistry<iVFS> (object_reg);
  CS_ASSERT (VFS != 0);
  VFS->PushDir ();
  VFS->ChDir (worlddir);

  // First we register ourselves as a callback to the physical layer so
  // that we get to know about entities created during loading. Those
  // entities are to registered so we can unload them later.
  pl->AddNewEntityCallback ((iCelNewEntityCallback*)this);

  // If we don't allow the entity addon to work then we mark this here
  // in the physical layer.
  bool prev_allow_entity_addon = false;
  if (!allow_entity_addon)
  {
    prev_allow_entity_addon = pl->IsEntityAddonAllowed ();
    pl->SetEntityAddonAllowed (false);
  }

  // Load the level file which is called 'world'.
  csRef<iThreadReturn> ret = tloader->LoadMapFileWait (VFS->GetCwd(), worldfile, false, cur_collection);

  // Restore everything.
  pl->RemoveNewEntityCallback ((iCelNewEntityCallback*)this);
  if (!allow_entity_addon)
    pl->SetEntityAddonAllowed (prev_allow_entity_addon);

  if (!ret->WasSuccessful())
  {
    EngReport (object_reg, "Could not load map file '%s/%s'.",
    	worlddir, worldfile);
    VFS->PopDir ();
    VFS->SetSyncDir(VFS->GetCwd());
    return false;
  }

  engine->PrecacheDraw (cur_collection);
  VFS->PopDir ();
  loaded = true;
  printf ("LoadOK!\n");

  // Create colliders for all meshes in this region.
  csRef<iCollideSystem> cdsys = csQueryRegistry<iCollideSystem> (object_reg);
  csColliderHelper::InitializeCollisionWrappers (cdsys, engine, cur_collection);

  return true;
}

void celPcRegion::Unload ()
{
  if (!loaded) return;
  loaded = false;
  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
  CS_ASSERT (engine != 0);

  iCollection* cur_collection = engine->CreateCollection (regionname);

  if (pl)
  {
    size_t i;
    for (i = 0 ; i < entities.GetSize () ; i++)
      if (entities[i])
      {
        pl->RemoveEntity (entities[i]);
      }
  }
  entities.DeleteAll ();

  cur_collection->ReleaseAllObjects ();
  engine->RemoveCollection (cur_collection);
}

iSector* celPcRegion::FindSector (const char* name)
{
  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
  iSector* temp = engine->FindSector (name, GetCollection(engine));
  return temp;
}

iSector* celPcRegion::GetStartSector (const char* name)
{
  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
  CS_ASSERT (engine != 0);
  if (empty_sector)
  {
    iCollection* coll = GetCollection (engine);
    return engine->FindSector (worldfile, coll);
  }
  iSector* sector;
  if (engine->GetCameraPositions ()->GetCount () > 0)
  {
    iCameraPosition* campos =
    	name ? engine->GetCameraPositions ()->FindByName (name)
       : engine->GetCameraPositions ()->Get (0);
    sector = engine->GetSectors ()->FindByName (
    	campos ? campos->GetSector () : "room");
  }
  else
  {
    sector = engine->FindSector ("room", GetCollection (engine));
  }
  return sector;
}

csVector3 celPcRegion::GetStartPosition (const char* name)
{
  if (empty_sector) return csVector3 (0, 0, 0);
  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
  CS_ASSERT (engine != 0);
  csVector3 pos (0);
  if (engine->GetCameraPositions ()->GetCount () > 0)
  {
    iCameraPosition* campos =
    	name ? engine->GetCameraPositions ()->FindByName (name)
    	: engine->GetCameraPositions ()->Get (0);
    if (campos) pos = campos->GetPosition ();
  }
  return pos;
}

void celPcRegion::PointCamera (iPcCamera* pccamera, const char* name)
{
  CS_ASSERT(pccamera != 0);

  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
  if (engine->GetCameraPositions ()->GetCount () > 0)
  {
    iCameraPosition* campos =
    	name ? engine->GetCameraPositions ()->FindByName (name)
    	: engine->GetCameraPositions ()->Get (0);
    if (campos)
    {
      campos->Load (pccamera->GetCamera (), engine);
      return;
    }
  }

  iSector* s = GetStartSector (name);
  pccamera->GetCamera ()->SetSector (s);
  pccamera->GetCamera ()->GetTransform ().SetOrigin (csVector3 (0,0,0));
}

iCollection* celPcRegion::GetCollection (csRef<iEngine> engine)
{
  return engine->GetCollection (regionname);
}

iCollection* celPcRegion::GetCollection ()
{
  if (!loaded)
    return 0;

  csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
  iCollection* region = GetCollection (engine);
  CS_ASSERT(region);
  return region;
}

//---------------------------------------------------------------------------


