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
#include "csutil/debug.h"
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

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Camera, "pccamera")
CEL_IMPLEMENT_FACTORY (Region, "pcregion")

SCF_EXPORT_CLASS_TABLE (pfengine)
  SCF_EXPORT_CLASS (celPfCamera, "cel.pcfactory.camera",
  	"CEL Camera Property Class Factory")
  SCF_EXPORT_CLASS (celPfRegion, "cel.pcfactory.region",
	"CEL Region Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcCamera)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCamera)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcCamera::PcCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcCamera)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcCamera::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcCamera::celPcCamera (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCamera);
  scfiEventHandler = NULL;
  
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  view = new csView (engine, g3d);
  iview = SCF_QUERY_INTERFACE (view, iView);
  view->DecRef ();
  g3d->DecRef ();
  SetupEventHandler ();
  cammode = iPcCamera::freelook;
  use_cd = false;
  rect_set = false;
  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  CS_ASSERT (kbd != NULL);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != NULL);

  DG_TYPE (this, "celPcCamera()");
}

celPcCamera::~celPcCamera ()
{
  if (kbd) kbd->DecRef ();
  if (vc) vc->DecRef ();
  if (iview) iview->DecRef ();
  if (engine) engine->DecRef ();
  if (g3d) g3d->DecRef ();
  if (scfiEventHandler)
  {
    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q)
    {
      q->RemoveListener (scfiEventHandler);
      q->DecRef ();
    }
    scfiEventHandler->DecRef ();
  }
}

void celPcCamera::SetupEventHandler ()
{
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);
  q->DecRef ();
}

bool celPcCamera::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdProcess)
  {
    // First get elapsed time from the virtual clock.
    csTicks elapsed_time = vc->GetElapsedTicks ();
  
    // Now rotate the camera according to its mode
    switch (cammode)
    {
      case iPcCamera::freelook:
      {
	float speed = (elapsed_time / 1000.0) * (0.03 * 20);
	
	iCamera* c = view->GetCamera();
	if (kbd->GetKeyState (CSKEY_SHIFT))
	  speed*=2.5;
	if (kbd->GetKeyState (CSKEY_RIGHT))
	  c->GetTransform ().RotateThis (VEC_ROT_RIGHT, speed);
	if (kbd->GetKeyState (CSKEY_LEFT))
	  c->GetTransform ().RotateThis (VEC_ROT_LEFT, speed);
	if (kbd->GetKeyState (CSKEY_PGUP))
	  c->GetTransform ().RotateThis (VEC_TILT_UP, speed);
	if (kbd->GetKeyState (CSKEY_PGDN))
	  c->GetTransform ().RotateThis (VEC_TILT_DOWN, speed);
	if (kbd->GetKeyState (CSKEY_UP))
	  c->Move (VEC_FORWARD * 4 * speed);
	if (kbd->GetKeyState (CSKEY_DOWN))
	  c->Move (VEC_BACKWARD * 4 * speed);
	break;
      }
      case iPcCamera::follow:
      {
	iPcMesh* pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList(),
	    iPcMesh);
	csVector3 pos = pcmesh->GetMesh()->GetMovable()->GetPosition();
	pcmesh->DecRef();

	iCamera* c = view->GetCamera();
	c->GetTransform().SetOrigin(pos);
	
	// eventually enable collision detection for camera
	c->OnlyPortals(!use_cd);
	c->Move (VEC_BACKWARD*2/*+VEC_UP*2*/);
	c->OnlyPortals(true);
	c->Move (VEC_FORWARD*.2/*+VEC_DOWN*.2*/);
	break;
      }
    }

    // Tell 3D driver we're going to display 3D things.
    if (g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS))
      view->Draw ();
  }

  return true;
}

void celPcCamera::SetRectangle (int x, int y, int w, int h)
{
  rect_x = x;
  rect_y = y;
  rect_w = w;
  rect_h = h;
  view->SetRectangle (x, y, w, h);
  rect_set = true;
}

#define CAMERA_SERIAL 2

iCelDataBuffer* celPcCamera::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (CAMERA_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (3+9+7);
  const csTransform& tr = view->GetCamera ()->GetTransform ();
  int j = 0;
  databuf->GetData (j++)->Set (tr.GetO2TTranslation ().x);
  databuf->GetData (j++)->Set (tr.GetO2TTranslation ().y);
  databuf->GetData (j++)->Set (tr.GetO2TTranslation ().z);

  databuf->GetData (j++)->Set (tr.GetO2T ().m11);
  databuf->GetData (j++)->Set (tr.GetO2T ().m12);
  databuf->GetData (j++)->Set (tr.GetO2T ().m13);
  databuf->GetData (j++)->Set (tr.GetO2T ().m21);
  databuf->GetData (j++)->Set (tr.GetO2T ().m22);
  databuf->GetData (j++)->Set (tr.GetO2T ().m23);
  databuf->GetData (j++)->Set (tr.GetO2T ().m31);
  databuf->GetData (j++)->Set (tr.GetO2T ().m32);
  databuf->GetData (j++)->Set (tr.GetO2T ().m33);

  databuf->GetData (j++)->Set ((uint8)cammode);
  databuf->GetData (j++)->Set (use_cd);
  databuf->GetData (j++)->Set (rect_set);
  databuf->GetData (j++)->Set ((uint16)rect_x);
  databuf->GetData (j++)->Set ((uint16)rect_y);
  databuf->GetData (j++)->Set ((uint16)rect_w);
  databuf->GetData (j++)->Set ((uint16)rect_h);

  return databuf;
}

bool celPcCamera::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != CAMERA_SERIAL) return false;
  if (databuf->GetDataCount () != 3+9+7) return false;

  csMatrix3 m_o2t;
  csVector3 v_o2t;
  int j = 0;
  celData* cd;
  cd = databuf->GetData (j++); if (!cd) return false;
  v_o2t.x = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  v_o2t.y = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  v_o2t.z = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m11 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m12 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m13 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m21 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m22 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m23 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m31 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m32 = cd->value.f;
  cd = databuf->GetData (j++); if (!cd) return false;
  m_o2t.m33 = cd->value.f;
  csOrthoTransform tr (m_o2t, v_o2t);
  view->GetCamera ()->SetTransform (tr);

  cd = databuf->GetData (j++); if (!cd) return false;
  cammode = (iPcCamera::CameraMode)cd->value.ub;
  cd = databuf->GetData (j++); if (!cd) return false;
  use_cd = cd->value.bo;
  cd = databuf->GetData (j++); if (!cd) return false;
  rect_set = cd->value.bo;
  int rx, ry, rw, rh;
  cd = databuf->GetData (j++); if (!cd) return false;
  rx = cd->value.uw;
  cd = databuf->GetData (j++); if (!cd) return false;
  ry = cd->value.uw;
  cd = databuf->GetData (j++); if (!cd) return false;
  rw = cd->value.uw;
  cd = databuf->GetData (j++); if (!cd) return false;
  rh = cd->value.uw;
  if (rect_set) view->SetRectangle (rx, ry, rw, rh);

  return true;
}

iCamera* celPcCamera::GetCamera () const
{
  return iview->GetCamera ();
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcRegion)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcRegion)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcRegion::PcRegion)
  SCF_IMPLEMENTS_INTERFACE (iPcRegion)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcRegion::celPcRegion (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcRegion);
  worlddir = NULL;
  worldfile = NULL;
  regionname = NULL;
  loaded = false;
  pointcamera = NULL;
  startname = NULL;
  DG_TYPE (this, "celPcRegion()");
}

celPcRegion::~celPcRegion ()
{
  Unload ();
  delete[] worlddir;
  delete[] worldfile;
  delete[] regionname;
  if (pointcamera) pointcamera->DecRef ();
  delete[] startname;
}

#define REGION_SERIAL 1

iCelDataBuffer* celPcRegion::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (REGION_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (6);
  databuf->GetData (0)->Set (worlddir);
  databuf->GetData (1)->Set (worldfile);
  databuf->GetData (2)->Set (regionname);
  databuf->GetData (3)->Set (loaded);
  iCelPropertyClass* pc = NULL;
  if (pointcamera)
    pc = SCF_QUERY_INTERFACE_FAST (pointcamera, iCelPropertyClass);
  databuf->GetData (4)->Set (pc);
  if (pc) pc->DecRef ();
  databuf->GetData (5)->Set (startname);
  return databuf;
}

bool celPcRegion::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != REGION_SERIAL) return false;
  if (databuf->GetDataCount () != 6) return false;
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
  cd = databuf->GetData (4); if (!cd) return false;
  iPcCamera* pcm = NULL;
  if (cd->value.pc) pcm = SCF_QUERY_INTERFACE_FAST (cd->value.pc, iPcCamera);
  cd = databuf->GetData (5); if (!cd) return false;

  if (load)
  {
    bool rc = Load ();
    if (rc)
    {
      PointCamera (pcm, cd->value.s);
      if (pcm) pcm->DecRef ();
    }
    return rc;
  }
  else Unload ();

  return true;
}

csStringID celPcRegion::propid_worlddir = csInvalidStringID;
csStringID celPcRegion::propid_worldfile = csInvalidStringID;
csStringID celPcRegion::propid_regionname = csInvalidStringID;

void celPcRegion::UpdatePropIDS (iObjectRegistry* object_reg)
{
  if (propid_worlddir == csInvalidStringID)
  {
    iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT (pl != NULL);
    propid_worlddir = pl->FetchStringID ("cel.property.pcregion.worlddir");
    propid_worldfile = pl->FetchStringID ("cel.property.pcregion.worldfile");
    propid_regionname = pl->FetchStringID ("cel.property.pcregion.regionname");
    pl->DecRef ();
  }
}

bool celPcRegion::SetProperty (csStringID propertyID, const char* value)
{
  UpdatePropIDS (object_reg);
  if (propertyID == propid_worldfile)
  {
    SetWorldFile (worlddir, value);
    return true;
  }
  else if (propertyID == propid_worlddir)
  {
    SetWorldFile (value, worldfile);
    return true;
  }
  else if (propertyID == propid_regionname)
  {
    SetRegionName (value);
    return true;
  }
  return false;
}

celPropertyActionType celPcRegion::GetPropertyOrActionType (
	csStringID propertyID)
{
  UpdatePropIDS (object_reg);
  if (propertyID == propid_worldfile) return type_string;
  else if (propertyID == propid_worlddir) return type_string;
  else if (propertyID == propid_regionname) return type_string;
  return type_none;
}

bool celPcRegion::IsPropertyReadOnly (csStringID propertyID)
{
  return false;
}

const char* celPcRegion::GetPropertyString (csStringID propertyID)
{
  UpdatePropIDS (object_reg);
  if (propertyID == propid_worldfile) return worldfile;
  else if (propertyID == propid_worlddir) return worlddir;
  else if (propertyID == propid_regionname) return regionname;
  return NULL;
}

csStringID celPcRegion::GetPropertyOrActionID (int idx)
{
  UpdatePropIDS (object_reg);
  switch (idx)
  {
    case 0: return propid_worldfile;
    case 1: return propid_worlddir;
    case 2: return propid_regionname;
    default: return csInvalidStringID;
  }
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

        pc = pl->CreatePropertyClass (ent, "pcmesh");
        iPcMesh* pcmesh = SCF_QUERY_INTERFACE_FAST (pc, iPcMesh);
        pcmesh->SetMesh (m);
        pcmesh->DecRef ();
        
        pc = pl->CreatePropertyClass (ent, "pcsolid");
        entities.Push (ent);
	DG_LINK (this, ent->QueryObject ());
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
    DG_UNLINK (this, ent->QueryObject ());
    ent->DecRef ();
  }
  entities.SetLength (0);

  iRegion* cur_region = engine->GetCurrentRegion ();
  engine->GetCurrentRegion ()->DeleteAll ();
  engine->SelectRegion (old_region);
  engine->GetRegions ()->Remove (cur_region);

  engine->DecRef ();
}

iSector* celPcRegion::GetStartSector (const char* name)
{
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
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
    sector = engine->GetSectors ()->FindByName ("room");
  }
  engine->DecRef ();
  return sector;
}

csVector3 celPcRegion::GetStartPosition (const char* name)
{
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
  csVector3 pos (0);
  if (engine->GetCameraPositions ()->GetCount () > 0)
  {
    iCameraPosition* campos =
    	name ? engine->GetCameraPositions ()->FindByName (name)
	     : engine->GetCameraPositions ()->Get (0);
    if (campos) pos = campos->GetPosition ();
  }
  engine->DecRef ();
  return pos;
}

void celPcRegion::PointCamera (iPcCamera* pccamera, const char* name)
{
  delete[] startname;
  if (name) startname = csStrNew (name);
  else startname = NULL;
  if (pccamera) pccamera->IncRef ();
  if (pointcamera) pointcamera->DecRef ();
  pointcamera = pccamera;
  if (pccamera)
  {
    iSector* s = GetStartSector (name);
    csVector3 p = GetStartPosition (name);
    pccamera->GetCamera ()->SetSector (s);
    pccamera->GetCamera ()->GetTransform ().SetOrigin (p);
  }
}

//---------------------------------------------------------------------------

