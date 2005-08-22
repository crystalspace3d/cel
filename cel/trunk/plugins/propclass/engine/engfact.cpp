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
#include "csutil/debug.h"
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
#include "iengine/region.h"
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

CEL_IMPLEMENT_FACTORY (Region, "pcregion")

void EngReport (iObjectRegistry* object_reg, const char* msg, ...)
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
}

//---------------------------------------------------------------------------

celPcRegion::celPcRegion (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];

  props = properties;
  propcount = &propertycount;

  propdata[propid_worlddir] = &worlddir;
  propdata[propid_worldfile] = &worldfile;
  propdata[propid_regionname] = &regionname;

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

size_t celPcRegion::propertycount = 0;
Property* celPcRegion::properties = 0;

void celPcRegion::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY( object_reg, iCelPlLayer );
    CS_ASSERT( pl != 0 );

    propertycount = 4;
    properties = new Property[propertycount];

    properties[propid_worlddir].id = pl->FetchStringID (
    	"cel.property.worlddir");
    properties[propid_worlddir].datatype = CEL_DATA_STRING;
    properties[propid_worlddir].readonly = false;
    properties[propid_worlddir].desc = "Map VFS path.";

    properties[propid_worldfile].id = pl->FetchStringID (
    	"cel.property.worldfile");
    properties[propid_worldfile].datatype = CEL_DATA_STRING;
    properties[propid_worldfile].readonly = false;
    properties[propid_worldfile].desc = "Map VFS file name.";

    properties[propid_regionname].id = pl->FetchStringID (
    	"cel.property.regionname");
    properties[propid_regionname].datatype = CEL_DATA_STRING;
    properties[propid_regionname].readonly = false;
    properties[propid_regionname].desc = "Name of this region.";

    properties[propid_load].id = pl->FetchStringID (
    	"cel.action.Load");
    properties[propid_load].datatype = CEL_DATA_ACTION;
    properties[propid_load].readonly = true;
    properties[propid_load].desc = "Load the map.\nNo parameters";
  }
}

bool celPcRegion::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == properties[propid_load].id)
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
  return false;
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
    EngReport (object_reg,"Entity '%s' already loaded.", entity->GetName());
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

  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != 0);
  iRegion* cur_region = engine->CreateRegion (regionname);
  cur_region->DeleteAll ();

  if (empty_sector)
  {
    iSector* sector = engine->CreateSector (worldfile);
    cur_region->Add (sector->QueryObject ());
    loaded = true;
    return true;
  }

  csRef<iLoader> loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  CS_ASSERT (loader != 0);
  csRef<iVFS> VFS = CS_QUERY_REGISTRY (object_reg, iVFS);
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
  bool rc = loader->LoadMapFile (worldfile, false, cur_region, false, true);

  // Restore everything.
  pl->RemoveNewEntityCallback ((iCelNewEntityCallback*)this);
  if (!allow_entity_addon)
    pl->SetEntityAddonAllowed (prev_allow_entity_addon);

  if (!rc)
  {
    EngReport (object_reg, "Could not load map file '%s/%s'.",
    	worlddir, worldfile);
    VFS->PopDir ();
    return false;
  }

  cur_region->Prepare ();
  engine->PrecacheDraw (cur_region);
  VFS->PopDir ();
  loaded = true;
  printf ("LoadOK!\n");

  // Create colliders for all meshes in this region.
  csRef<iCollideSystem> cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
  csColliderHelper::InitializeCollisionWrappers (cdsys, engine, cur_region);

  return true;
}

void celPcRegion::Unload ()
{
  if (!loaded) return;
  loaded = false;
  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != 0);

  iRegion* cur_region = engine->CreateRegion (regionname);

  if (pl)
  {
    size_t i;
    for (i = 0 ; i < entities.Length () ; i++)
      if (entities[i])
      {
        pl->RemoveEntity (entities[i]);
      }
  }
  entities.DeleteAll ();

  cur_region->DeleteAll ();
  engine->GetRegions ()->Remove (cur_region);
}

iSector* celPcRegion::FindSector (const char* name)
{
  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  iSector* temp = engine->FindSector (name, GetRegionInternal(engine));
  return temp;
}

iSector* celPcRegion::GetStartSector (const char* name)
{
  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != 0);
  if (empty_sector)
  {
    iRegion* reg = GetRegionInternal(engine);
    return engine->FindSector (worldfile, reg);
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
    sector = engine->FindSector("room", GetRegionInternal(engine));
  }
  return sector;
}

csVector3 celPcRegion::GetStartPosition (const char* name)
{
  if (empty_sector) return csVector3 (0, 0, 0);
  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
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

  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (engine->GetCameraPositions()->GetCount() > 0)
  {
    iCameraPosition* campos =
        name ? engine->GetCameraPositions()->FindByName(name)
       : engine->GetCameraPositions()->Get(0);
    if (campos)
    {
      campos->Load(pccamera->GetCamera(), engine);
      return;
    }
  }

  iSector* s = GetStartSector (name);
  pccamera->GetCamera ()->SetSector (s);
  pccamera->GetCamera ()->GetTransform ().SetOrigin (csVector3(0,0,0));
}

iRegion* celPcRegion::GetRegionInternal(csRef<iEngine> engine)
{
  return engine->GetRegions()->FindByName(regionname);
}

iRegion* celPcRegion::GetRegion()
{
  if (!loaded)
    return 0;

  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  iRegion* region = GetRegionInternal(engine);
  CS_ASSERT(region);
  return region;
}

//---------------------------------------------------------------------------

