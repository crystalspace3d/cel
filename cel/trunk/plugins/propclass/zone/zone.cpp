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

#include <math.h>
#include "cssysdef.h"
#include "propclass/mesh.h"
#include "propclass/solid.h"
#include "plugins/propclass/zone/zone.h"
#include "propclass/camera.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "physicallayer/databhlp.h"
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
#include "imap/parser.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/camera.h"
#include "iengine/region.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/csview.h"
#include "ivaria/view.h"
#include "ivideo/graph3d.h"
#include "qsqrt.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (ZoneManager, "pczonemanager")

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistance",
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

void cameraSectorListener::NewSector (iCamera* /*camera*/, iSector* sector)
{
  zonemgr->ActivateSector (sector);
}

SCF_IMPLEMENT_IBASE (cameraSectorListener)
  SCF_IMPLEMENTS_INTERFACE (iCameraSectorListener)
SCF_IMPLEMENT_IBASE_END

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celMapFile)
  SCF_IMPLEMENTS_INTERFACE (iCelMapFile)
SCF_IMPLEMENT_IBASE_END

void celMapFile::SetPath (const char* path)
{
  delete[] celMapFile::path;
  delete[] celMapFile::sectorname;
  celMapFile::path = csStrNew (path);
  celMapFile::sectorname = 0;
}

void celMapFile::SetSectorName (const char* name)
{
  delete[] celMapFile::path;
  delete[] celMapFile::sectorname;
  celMapFile::path = 0;
  celMapFile::sectorname = csStrNew (name);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celRegion)
  SCF_IMPLEMENTS_INTERFACE (iCelRegion)
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
  int idx = mapfiles.Find ((celMapFile*)mapfile);
  if (idx == -1) return false;
  mapfiles.DeleteIndex (idx);
  return true;
}

void celRegion::RemoveAllMapFiles ()
{
  mapfiles.SetLength (0);
}

bool celRegion::Load ()
{
  if (loaded) return true;

  iEngine* engine = mgr->GetEngine ();
  iLoader* loader = mgr->GetLoader ();
  iRegion* cur_region = engine->CreateRegion (name);
  cur_region->DeleteAll ();

  size_t i;
  for (i = 0 ; i < mapfiles.Length () ; i++)
  {
    celMapFile* mf = mapfiles[i];
    if (mf->GetSectorName ())
    {
      iSector* sector = engine->CreateSector (mf->GetSectorName ());
      cur_region->Add (sector->QueryObject ());
    }
    else if (mf->GetPath ())
    {
      if (!loader->LoadMapFile (mf->GetPath (), false, cur_region, false, true))
        return false;
    }
    else
    {
      return false;
    }
  }

  cur_region->Prepare ();

  // Create entities for all meshes in this region unless there is already
  // an entity for them (an addon may have created them for example).
  iCelPlLayer* pl = mgr->GetPL ();
  iCelPropertyClass* pc;
  csRef<iObjectIterator> iter = cur_region->QueryObject ()->GetIterator ();
  while (iter->HasNext ())
  {
    iObject* o = iter->Next ();

    csRef<iSector> sector = SCF_QUERY_INTERFACE (o, iSector);
    if (sector)
    {
      sectors.Add (sector);
      continue;
    }

    iCelEntity* e = pl->FindAttachedEntity (o);
    if (e)
    {
      // There was already an entity attached. This entity is probably
      // created by an addon. We will also register this entity as
      // one that needs to be deleted when the region is unloaded.
      entities.Push (e);
    }
    else
    {
      csRef<iMeshWrapper> m = SCF_QUERY_INTERFACE (o, iMeshWrapper);
      if (m)
      {
        csRef<iCelEntity> ent = pl->CreateEntity ();
        ent->SetName ("");

        pc = pl->CreatePropertyClass (ent, "pcmesh");
        csRef<iPcMesh> pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);
        pcmesh->SetMesh (m);

        pc = pl->CreatePropertyClass (ent, "pcsolid");
        entities.Push (ent);
      }
    }
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
  size_t i;
  for (i = 0 ; i < entities.Length () ; i++)
    if (entities[i])
    {
      pl->RemoveEntity (entities[i]);
    }
  entities.DeleteAll ();

  // We now scan every sector to see if there are entities
  // in that that are not deleted yet. We will delete them here.
  csSet<iSector*>::GlobalIterator it = sectors.GetIterator ();
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

  sectors.DeleteAll ();

  cur_region->DeleteAll ();
  engine->GetRegions ()->Remove (cur_region);
  loaded = false;
}

void celRegion::AssociateEntity (iCelEntity* entity)
{
  entities.Push (entity);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celZone)
  SCF_IMPLEMENTS_INTERFACE (iCelZone)
SCF_IMPLEMENT_IBASE_END

void celZone::LinkRegion (iCelRegion* region)
{
  int idx = regions.Find ((celRegion*)region);
  if (idx != -1) return;
  regions.Push ((celRegion*)region);
}

bool celZone::UnlinkRegion (iCelRegion* region)
{
  int idx = regions.Find ((celRegion*)region);
  if (idx == -1) return false;
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
  int idx = regions.Find (region);
  return idx != -1;
}

//---------------------------------------------------------------------------

csStringID celPcZoneManager::id_region = csInvalidStringID;

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
  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);

  if (id_region == csInvalidStringID)
  {
    id_region = pl->FetchStringID ("cel.parameter.region");
  }
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_region, "region");
}

celPcZoneManager::~celPcZoneManager ()
{
  // Unload everything.
  ActivateRegion (0);
  delete params;
}

#define ZONEMANAGER_SERIAL 1

csPtr<iCelDataBuffer> celPcZoneManager::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (ZONEMANAGER_SERIAL);
  databuf->SetDataCount (0);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcZoneManager::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != ZONEMANAGER_SERIAL)
  {
    Report (object_reg, "serialnr != ZONEMANAGER_SERIAL.  Cannot load.");
    return false;
  }
  if (databuf->GetDataCount () != 0)
  {
    Report (object_reg, "0 data elements required, not %d.  Cannot load.",
    	databuf->GetDataCount () );
    return false;
  }

  return true;
}

bool celPcZoneManager::PerformAction (csStringID /*actionId*/,
	iCelParameterBlock* /*params*/)
{
  return false;
}

void celPcZoneManager::SendZoneMessage (iCelRegion* region, const char* msgid)
{
  if (region) params->GetParameter (0).SetIBase (region);
  celData ret;
  entity->GetBehaviour ()->SendMessage (msgid, ret, params);
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
  int idx = zones.Find ((celZone*)zone);
  if (idx == -1) return false;
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
  int idx = regions.Find ((celRegion*)region);
  if (idx == -1) return false;
  regions.DeleteIndex (idx);
  return true;
}

void celPcZoneManager::RemoveAllRegions ()
{
  regions.SetLength (0);
}

bool celPcZoneManager::ActivateRegion (celRegion* region)
{
  // The 'first' flag is used to see if we need to do some loading
  // work. In that case we send a message to the behaviour layer so that
  // it may possible open up a loading screen or something.
  bool first = true;
  size_t i;

  // First we make a set of all regions that we need to load.
  csSet<celRegion*> loadable_regions;
  for (i = 0 ; i < zones.Length () ; i++)
    if (zones[i]->ContainsRegion (region))
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
	if (!r->Load ())
	{
	  SendZoneMessage ((iCelRegion*)r, "pczonemanager_errorloading");
          return false;
	}
      }
    }
    else
    {
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
  // If we have a mesh we take the sector from the mesh instead
  // of the camera. That's more stable.
  if (pcmesh) sector = pcmesh->GetMesh ()->GetMovable ()->GetSectors ()
  	->Get (0);
  size_t i;
  for (i = 0 ; i < regions.Length () ; i++)
  {
    if (regions[i]->ContainsSector (sector))
      return ActivateRegion (regions[i]);
  }
  return true;
}

int celPcZoneManager::PointCamera (iPcCamera* pccamera, const char* regionname,
  	const char* startname)
{
  if (!camlistener)
    camlistener.AttachNew (new cameraSectorListener (this));
  if (celPcZoneManager::pccamera)
    celPcZoneManager::pccamera->GetCamera ()
    	->RemoveCameraSectorListener (camlistener);
  celPcZoneManager::pccamera = pccamera;
  pccamera->GetCamera ()->AddCameraSectorListener (camlistener);

  iCelRegion* region = FindRegion (regionname);
  if (!region) return CEL_ZONEERROR_BADREGION;
  if (!ActivateRegion ((celRegion*)region))
    return CEL_ZONEERROR_LOAD;

  // Find the created region.
  iRegion* cur_region = engine->CreateRegion (regionname);

  // Find the right start position.
  iCameraPosition* campos;
  if (startname)
  {
    campos = cur_region->FindCameraPosition (startname);
    if (!campos) return CEL_ZONEERROR_BADSTART;
    campos->Load (pccamera->GetCamera (), engine);
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
      campos->Load (pccamera->GetCamera (), engine);
    }
    else
    {
      // We don't have a valid camera position. Try to see if there
      // is a sector called 'room'.
      iSector* room = cur_region->FindSector ("room");
      if (!room)
	return CEL_ZONEERROR_BADSTART;
      pccamera->GetCamera ()->SetSector (room);
      pccamera->GetCamera ()->GetTransform ().SetOrigin (csVector3 (0,0,0));
    }
  }

  return CEL_ZONEERROR_OK;
}

int celPcZoneManager::PointMesh (iPcMesh* pcmesh, const char* regionname,
  	const char* startname)
{
  celPcZoneManager::pcmesh = pcmesh;

  iCelRegion* region = FindRegion (regionname);
  if (!region) return CEL_ZONEERROR_BADREGION;
  if (!ActivateRegion ((celRegion*)region))
    return CEL_ZONEERROR_LOAD;

  // Find the created region.
  iRegion* cur_region = engine->CreateRegion (regionname);

  // Find the right start position.
  iCameraPosition* campos;
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
      // We don't have a valid camera position. Try to see if there
      // is a sector called 'room'.
      sector = cur_region->FindSector ("room");
      pos.Set (0, 0, 0);
    }
  }
  if (!sector)
    return CEL_ZONEERROR_BADSTART;
  pcmesh->GetMesh ()->GetMovable ()->SetSector (sector);
  pcmesh->GetMesh ()->GetMovable ()->SetPosition (pos);
  pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();

  return CEL_ZONEERROR_OK;
}

//---------------------------------------------------------------------------

