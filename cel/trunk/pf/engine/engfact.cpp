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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "pf/mesh.h"
#include "pf/solid.h"
#include "pf/engine/engfact.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/persist.h"
#include "bl/behave.h"
#include "csutil/util.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "imap/parser.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/camera.h"
#include "iengine/region.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/csview.h"
#include "ivaria/view.h"
#include "ivideo/graph3d.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPfEngine)

SCF_EXPORT_CLASS_TABLE (pfengine)
  SCF_EXPORT_CLASS (celPfEngine, "cel.pcfactory.engine",
  	"CEL Engine Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfEngine)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfEngine::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfEngine::celPfEngine (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfEngine::~celPfEngine ()
{
}

bool celPfEngine::Initialize (iObjectRegistry* object_reg)
{
  celPfEngine::object_reg = object_reg;
  return true;
}

const char* celPfEngine::GetTypeName (int idx) const
{
  switch (idx)
  {
    case 0: return "pccamera";
    case 1: return "pcregion";
    default: return NULL;
  }
}

iCelPropertyClass* celPfEngine::CreatePropertyClass (const char* type)
{
  if (!strcmp (type, "pccamera"))
    return new celPcCamera (object_reg);
  else if (!strcmp (type, "pcregion"))
    return new celPcRegion (object_reg);
  else
    return NULL;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcCamera)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCamera)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcCamera::PcCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcCamera)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcCamera::celPcCamera (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCamera);
  celPcCamera::object_reg = object_reg;
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  iGraphics3D* g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  view = new csView (engine, g3d);
  iview = SCF_QUERY_INTERFACE (view, iView);
  view->DecRef ();
  g3d->DecRef ();
  engine->DecRef ();
}

celPcCamera::~celPcCamera ()
{
  if (iview) iview->DecRef ();
}

void celPcCamera::SetEntity (iCelEntity* entity)
{
  celPcCamera::entity = entity;
}

#define CAMERA_SERIAL 1

iCelDataBuffer* celPcCamera::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (CAMERA_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (0);
  return databuf;
}

bool celPcCamera::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != CAMERA_SERIAL) return false;
  if (databuf->GetDataCount () != 0) return false;
  return true;
}

iCamera* celPcCamera::GetCamera () const
{
  return iview->GetCamera ();
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcRegion)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcRegion)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcRegion::PcRegion)
  SCF_IMPLEMENTS_INTERFACE (iPcRegion)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcRegion::celPcRegion (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcRegion);
  celPcRegion::object_reg = object_reg;
  worlddir = NULL;
  worldfile = NULL;
  regionname = NULL;
  loaded = false;
}

celPcRegion::~celPcRegion ()
{
  Unload ();
  delete[] worlddir;
  delete[] worldfile;
  delete[] regionname;
}

void celPcRegion::SetEntity (iCelEntity* entity)
{
  celPcRegion::entity = entity;
}

#define REGION_SERIAL 1

iCelDataBuffer* celPcRegion::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (REGION_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (4);
  databuf->GetData (0)->Set (worlddir);
  databuf->GetData (1)->Set (worldfile);
  databuf->GetData (2)->Set (regionname);
  databuf->GetData (3)->Set (loaded);
  return databuf;
}

bool celPcRegion::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != REGION_SERIAL) return false;
  if (databuf->GetDataCount () != 4) return false;
  celData* cd;
  Unload ();
  delete[] worlddir; worlddir = NULL;
  delete[] worldfile; worldfile = NULL;
  delete[] regionname; regionname = NULL;
  cd = databuf->GetData (0); if (!cd) return false;
  worlddir = csStrNew (cd->value.s);
  cd = databuf->GetData (1); if (!cd) return false;
  worldfile = csStrNew (cd->value.s);
  cd = databuf->GetData (2); if (!cd) return false;
  regionname = csStrNew (cd->value.s);
  cd = databuf->GetData (3); if (!cd) return false;
  bool load = cd->value.bo;

  if (load) return Load ();
  else Unload ();

  return true;
}

void celPcRegion::SetWorldFile (const char* vfsdir, const char* name)
{
  delete[] worlddir;
  delete[] worldfile;
  worlddir = csStrNew (vfsdir);
  worldfile = csStrNew (name);
}

void celPcRegion::SetRegionName (const char* name)
{
  delete[] regionname;
  regionname = csStrNew (name);
}

bool celPcRegion::Load ()
{
  bool rc = true;

  if (loaded) return true;
  if (!worlddir) return false;
  if (!worldfile) return false;
  if (!regionname) return false;

  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
  iRegion* old_region = engine->GetCurrentRegion ();
  engine->SelectRegion (regionname);
  iRegion* cur_region = engine->GetCurrentRegion ();
  cur_region->DeleteAll ();

  iLoader* loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  CS_ASSERT (loader != NULL);
  iVFS* VFS = CS_QUERY_REGISTRY (object_reg, iVFS);
  CS_ASSERT (VFS != NULL);
  VFS->ChDir (worlddir);
  VFS->DecRef ();
  // Load the level file which is called 'world'.
  if (!loader->LoadMapFile (worldfile, false, true))
  {
    rc = false;
    goto cleanup;
  }
  cur_region->Prepare ();
  loaded = true;

  {
    // Create entities for all meshes in this region.
    iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT (pl != NULL);
    iCelPropertyClassFactory* pfmove = pl->FindPropertyClassFactory ("pfmove");
    CS_ASSERT (pfmove != NULL);
    iCelPropertyClassFactory* pfmesh = pl->FindPropertyClassFactory ("pfmesh");
    CS_ASSERT (pfmesh != NULL);
    iCelPropertyClass* pc;
    iObjectIterator* iter = cur_region->QueryObject ()->GetIterator ();
    while (!iter->IsFinished ())
    {
      iObject* o = iter->GetObject ();
      iMeshWrapper* m = SCF_QUERY_INTERFACE (o, iMeshWrapper);
      if (m)
      {
        iCelEntity* ent = pl->CreateEntity ();
        ent->SetName ("__dummy__");

        pc = pfmesh->CreatePropertyClass ("pcmesh");
        ent->GetPropertyClassList ()->Add (pc);
        iPcMesh* pcmesh = SCF_QUERY_INTERFACE_FAST (pc, iPcMesh);
        pcmesh->SetMesh (m);
        pcmesh->DecRef ();
        pc->DecRef ();

        pc = pfmove->CreatePropertyClass ("pcsolid");
        ent->GetPropertyClassList ()->Add (pc);
        pc->DecRef ();

        entities.Push (ent);

        m->DecRef ();
      }
      iter->Next ();
    }
    iter->DecRef ();
    pl->DecRef ();
  }

cleanup:
  engine->SelectRegion (old_region);
  engine->DecRef ();
  loader->DecRef ();
  return rc;
}

void celPcRegion::Unload ()
{
  if (!loaded) return;
  loaded = false;
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);

  iRegion* old_region = engine->GetCurrentRegion ();
  engine->SelectRegion (regionname);

  int i;
  for (i = 0 ; i < entities.Length () ; i++)
  {
    iCelEntity* ent = (iCelEntity*)entities[i];
    ent->DecRef ();
  }
  entities.SetLength (0);

  engine->GetCurrentRegion ()->DeleteAll ();
  engine->SelectRegion (old_region);

  engine->DecRef ();
}

iSector* celPcRegion::GetStartSector ()
{
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
  iSector* sector;
  if (engine->GetCameraPositions ()->GetCount () > 0)
  {
    iCameraPosition* campos = engine->GetCameraPositions ()->Get (0);
    sector = engine->GetSectors ()->FindByName (campos->GetSector ());
  }
  else
  {
    sector = engine->GetSectors ()->FindByName ("room");
  }
  engine->DecRef ();
  return sector;
}

csVector3 celPcRegion::GetStartPosition ()
{
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
  csVector3 pos (0);
  if (engine->GetCameraPositions ()->GetCount () > 0)
  {
    iCameraPosition* campos = engine->GetCameraPositions ()->Get (0);
    pos = campos->GetPosition ();
  }
  engine->DecRef ();
  return pos;
}

//---------------------------------------------------------------------------

