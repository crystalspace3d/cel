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

#include <math.h>
#include "cssysdef.h"
#include "pf/mesh.h"
#include "pf/solid.h"
#include "pf/engine/engfact.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/persist.h"
#include "pl/datatype.h"
#include "pl/databhlp.h"
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
  SetupEventHandler ();
  cammode = iPcCamera::freelook;
  use_cd = false;
  rect_set = false;
  region = NULL;
  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  mouse = CS_QUERY_REGISTRY (object_reg, iMouseDriver);
  CS_ASSERT (kbd != NULL);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != NULL);
  angle_xz = angle_yz = _yz = _xz = 0.0;    // staring angles to the object (in radians)
  _dist = dist_y = 10.0;				// starting distance from the object
  alter_angle = alter_dist = false;
  DG_TYPE (this, "celPcCamera()");
}

celPcCamera::~celPcCamera ()
{
  if (kbd) kbd->DecRef ();
  if (vc) vc->DecRef ();
  if (iview) iview->DecRef ();
  if (engine) engine->DecRef ();
  if (g3d) g3d->DecRef ();
  if (region) region->DecRef();
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
	  c->GetTransform ().RotateThis (CS_VEC_ROT_RIGHT, speed);
	if (kbd->GetKeyState (CSKEY_LEFT))
	  c->GetTransform ().RotateThis (CS_VEC_ROT_LEFT, speed);
	if (kbd->GetKeyState (CSKEY_PGUP))
	  c->GetTransform ().RotateThis (CS_VEC_TILT_UP, speed);
	if (kbd->GetKeyState (CSKEY_PGDN))
	  c->GetTransform ().RotateThis (CS_VEC_TILT_DOWN, speed);
	if (kbd->GetKeyState (CSKEY_UP))
	  c->Move (CS_VEC_FORWARD * 4 * speed);
	if (kbd->GetKeyState (CSKEY_DOWN))
	  c->Move (CS_VEC_BACKWARD * 4 * speed);
	break;
      }
      case iPcCamera::follow:
      {
	iPcMesh* pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList(),
	    iPcMesh);
	if (!pcmesh) break;
	csVector3 pos = pcmesh->GetMesh()->GetMovable()->GetPosition();
	pcmesh->DecRef();

	iCamera* c = view->GetCamera();
	c->GetTransform().SetOrigin(pos);
	
	// eventually enable collision detection for camera
	c->OnlyPortals(!use_cd);
	c->Move (CS_VEC_BACKWARD*2/*+CS_VEC_UP*2*/);
	c->OnlyPortals(true);
	c->Move (CS_VEC_FORWARD*.2/*+CS_VEC_DOWN*.2*/);
	break;
      }
      case iPcCamera::rotational:
      {
	iPcMesh* pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList(), iPcMesh);
	if (!pcmesh) break;
	csBox3 b;
	csVector3 pos;
	pcmesh->GetMesh()->GetWorldBoundingBox(b);
	pos = b.GetCenter();
	float min_dist = sqrt(pow(pos.x - b.Max().x, 2) + pow(pos.y - b.Max().y,2) + pow(pos.z - b.Max().z,2));

	iCamera* c = view->GetCamera();

	if (mouse->GetLastButton(1) && mouse->GetLastButton(2)) {
		int _delta_y, _current_y;

		if (!alter_dist) {
			alter_dist = true;
			base_y_d = _current_y = mouse->GetLastY();
		} else
			_current_y = mouse->GetLastY();

		_delta_y = base_y_d - _current_y;
		float delta_dist = _delta_y/100.0;
		_dist = dist_y + delta_dist;
		if (_dist < min_dist) _dist = min_dist;
	} else {
		alter_dist = false;
		dist_y = _dist;
	}
		
	if (!mouse->GetLastButton(1) && mouse->GetLastButton(2)) {
		int delta_x, delta_y, current_x, current_y;

		if (!alter_angle) {
			alter_angle = true;
			base_x = current_x = mouse->GetLastX();
			base_y = current_y = mouse->GetLastY();
			//csVector camera = c->GetOrigin();
			
		} else {
			current_x = mouse->GetLastX();
			current_y = mouse->GetLastY();
		}

		delta_x = base_x - current_x;
		delta_y = base_y - current_y;

		float delta_yz = delta_x/200.0;
		float delta_xz = delta_y/300.0;

		_yz = angle_yz + delta_yz;
		_xz = angle_xz + delta_xz;
	} else {
		alter_angle = false;
		
		angle_xz = _xz;
		angle_yz = _yz;
	}
	
	if (_xz > 1.3) _xz = 1.3;
	if (_xz < -1.3) _xz = -1.3;

	csVector3 V(cos(_yz)*cos(_xz),sin(_xz),sin(_yz)*cos(_xz));
	csVector3 result = pos + V * _dist;

	c->GetTransform().SetOrigin(result);
	c->GetTransform().LookAt(pos-result,csVector3(0,1,0));
	}
    }

    // Tell 3D driver we're going to display 3D things.
    if (g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS))
      view->Draw ();
  }

  return true;
}


bool celPcCamera::SetRegion (iPcRegion* newregion, bool point,const char *name)
{
  newregion->IncRef();
  if (region)
    region->DecRef();
  region = newregion;

  if (point)
  {
    iPcCamera* camera = SCF_QUERY_INTERFACE_FAST(this, iPcCamera);
    region->PointCamera(camera, name);
    camera->DecRef();
  }

  return true;
}

bool celPcCamera::SetMode (iPcCamera::CameraMode cammode, bool use_cd)
{
    celPcCamera::cammode=cammode;
    celPcCamera::use_cd=use_cd;
    view->GetCamera()->OnlyPortals(!use_cd);
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
  celDataBufHelper db(databuf);

  iCelPropertyClass* pc = NULL;
  if (region) pc = SCF_QUERY_INTERFACE_FAST (region, iCelPropertyClass);
  db.Set (pc);
  if (pc) pc->DecRef();
  db.Set (view->GetCamera()->GetSector()->QueryObject()->GetName());
  const csTransform& tr = view->GetCamera ()->GetTransform ();
  db.Set (tr.GetO2TTranslation ());

  db.Set (tr.GetO2T ().m11);
  db.Set (tr.GetO2T ().m12);
  db.Set (tr.GetO2T ().m13);
  db.Set (tr.GetO2T ().m21);
  db.Set (tr.GetO2T ().m22);
  db.Set (tr.GetO2T ().m23);
  db.Set (tr.GetO2T ().m31);
  db.Set (tr.GetO2T ().m32);
  db.Set (tr.GetO2T ().m33);

  db.Set ((uint8)cammode);
  db.Set (use_cd);
  db.Set (rect_set);
  db.Set ((uint16)rect_x);
  db.Set ((uint16)rect_y);
  db.Set ((uint16)rect_w);
  db.Set ((uint16)rect_h);

  return databuf;
}

bool celPcCamera::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != CAMERA_SERIAL) return false;
  if (databuf->GetDataCount () != 3+9+7) return false;
  celDataBufHelper db(databuf);

  csMatrix3 m_o2t;
  csVector3 v_o2t;

  iCelPropertyClass* pc;
  if (!db.Get(pc)) return false;
  if (pc) region = SCF_QUERY_INTERFACE_FAST (pc, iPcRegion);
  if (region)
      SetRegion(region, false, NULL);

  const char* sectname;
  db.Get(sectname);
  iSector* sector = region->FindSector(sectname);
  if (!sector) 
    return false;
  
  db.Get(v_o2t);

  db.Get(m_o2t.m11);
  db.Get(m_o2t.m12);
  db.Get(m_o2t.m13);
  db.Get(m_o2t.m21);
  db.Get(m_o2t.m22);
  db.Get(m_o2t.m23);
  db.Get(m_o2t.m31);
  db.Get(m_o2t.m32);
  db.Get(m_o2t.m33);

  if (!db.AllOk())
    return false;
  
  view->GetCamera ()->SetSector(sector);
  csOrthoTransform tr (m_o2t, v_o2t);
  view->GetCamera ()->SetTransform (tr);

  db.Get((uint8&)cammode);
  db.Get(use_cd);
  db.Get(rect_set);
  db.Get((uint16&)rect_x);
  db.Get((uint16&)rect_y);
  db.Get((uint16&)rect_w);
  db.Get((uint16&)rect_h);

  if (!db.AllOk())
    return false;

  if (rect_set)
    view->SetRectangle (rect_x, rect_y, rect_w, rect_h);

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
  DG_TYPE (this, "celPcRegion()");
}

celPcRegion::~celPcRegion ()
{
  Unload ();
  delete[] worlddir;
  delete[] worldfile;
  delete[] regionname;
}

#define REGION_SERIAL 1

iCelDataBuffer* celPcRegion::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (REGION_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (4);
  celDataBufHelper db(databuf);
  
  db.Set (worlddir);
  db.Set (worldfile);
  db.Set (regionname);
  db.Set (loaded);
  
  return databuf;
}

bool celPcRegion::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != REGION_SERIAL) return false;
  if (databuf->GetDataCount () != 4) return false;
  celDataBufHelper db(databuf);
  
  Unload ();
  delete[] worlddir; worlddir = NULL;
  delete[] worldfile; worldfile = NULL;
  delete[] regionname; regionname = NULL;
  
  const char* strp;
  if (!db.Get(strp)) return false;
  worlddir = csStrNew(strp);
  if (!db.Get(strp)) return false;
  worldfile = csStrNew(strp);
  if (!db.Get(strp)) return false;
  regionname = csStrNew(strp);
  bool load;
  if (!db.Get(load)) return false;

  if (load && !Load ())
      return false;

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

celDataType celPcRegion::GetPropertyOrActionType (csStringID propertyID)
{
  UpdatePropIDS (object_reg);
  if (propertyID == propid_worldfile) return CEL_DATA_STRING;
  else if (propertyID == propid_worlddir) return CEL_DATA_STRING;
  else if (propertyID == propid_regionname) return CEL_DATA_STRING;
  return CEL_DATA_NONE;
}

bool celPcRegion::IsPropertyReadOnly (csStringID propertyID)
{
  (void)propertyID;
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
  printf ("LoadOK!\n");

  // Create entities for all meshes in this region.
  {
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

iSector* celPcRegion::FindSector (const char* name)
{
  iEngine* engine = CS_QUERY_REGISTRY(object_reg, iEngine);
  iSector* temp = engine->GetSectors()->FindByName (name);
  engine->DecRef();
  return temp;
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
  CS_ASSERT(pccamera != NULL);
  
  iEngine* engine = CS_QUERY_REGISTRY(object_reg, iEngine);
  if (engine->GetCameraPositions()->GetCount() > 0)
  {
    iCameraPosition* campos =
        name ? engine->GetCameraPositions()->FindByName(name)
	     : engine->GetCameraPositions()->Get(0);
    if (campos)
    {
      campos->Load(pccamera->GetCamera(), engine);
      engine->DecRef();
      return;
    }
  }
  
  iSector* s = GetStartSector (name);
  pccamera->GetCamera ()->SetSector (s);
  pccamera->GetCamera ()->GetTransform ().SetOrigin (csVector3(0,0,0));
  engine->DecRef();
}

//---------------------------------------------------------------------------

