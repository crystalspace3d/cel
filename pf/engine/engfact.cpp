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
#include "qsqrt.h"

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
  SetupEventHandler ();
  cammode = iPcCamera::freelook;
  use_cd = false;
  rect_set = false;
  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  mouse = CS_QUERY_REGISTRY (object_reg, iMouseDriver);
  CS_ASSERT (kbd != NULL);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != NULL);
  followpos.Set(0,0,0);
  followat.Set(0,0,0);

  // Starting angles to the object (in radians)
  roll = pitch = yaw = angle_xz = angle_yz = _yz = _xz = 0.0;
  // starting distance from the object
  _dist = dist_y = 10.0;

  alter_angle = alter_dist = false;
  DG_TYPE (this, "celPcCamera()");
}

celPcCamera::~celPcCamera ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    if (q)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
}

void celPcCamera::SetupEventHandler ()
{
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
  CS_ASSERT (q != NULL);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);
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
	csRef<iPcMesh> pcmesh (
		CEL_QUERY_PROPCLASS (entity->GetPropertyClassList(), iPcMesh));
	if (!pcmesh) break;
	iMovable* movable = pcmesh->GetMesh()->GetMovable();

	iCamera* c = view->GetCamera();

	csReversibleTransform rt = movable->GetFullTransform();
	csMatrix3 mat = rt.GetT2O();

	c->GetTransform().SetOrigin(movable->GetPosition());
	c->SetSector (movable->GetSectors ()->Get (0));
	c->GetTransform().SetT2O(mat);

	// move camera to followpos
	c->OnlyPortals(!use_cd);
	c->Move (followpos);
	c->OnlyPortals(true);
	c->Move (followpos*-0.1);

	// calculate the lookat vector
	csVector3 lookat = (movable->GetPosition() -
	    c->GetTransform().GetOrigin()) + (mat * followat);

    // Set in camera and adjust for roll in one step
	c->GetTransform().LookAt(lookat, csVector3(0,cos(roll),sin(roll) ));

    // rotation to adjust for pitch angle
    c->GetTransform().RotateThis(csXRotMatrix3(pitch));
    // rotation to adjust for yaw angle
    c->GetTransform().RotateThis(csYRotMatrix3(yaw));
	break;
      }
      case iPcCamera::firstperson:
      {
        csRef<iPcMesh> pcmesh (
		CEL_QUERY_PROPCLASS (entity->GetPropertyClassList(), iPcMesh));
        if (!pcmesh) break;
        iMovable* movable = pcmesh->GetMesh()->GetMovable();

        iCamera* c = view->GetCamera();

        csReversibleTransform rt = movable->GetFullTransform();
        csMatrix3 mat = rt.GetT2O();

	c->GetTransform().SetOrigin(movable->GetPosition());
	c->SetSector(movable->GetSectors()->Get(0));
        c->GetTransform().SetT2O(mat);

	// move camera to followpos
	c->OnlyPortals(!use_cd);
        c->Move (followpos);
        c->OnlyPortals(true);
        c->Move (followpos*-0.1);

	//rotate camera 180 degs
	c->GetTransform().RotateThis(csVector3(0,1,0), 3.14f);

    // rotation to adjust for pitch angle
    c->GetTransform().RotateThis(csXRotMatrix3(pitch));
    // rotation to adjust for yaw angle
    c->GetTransform().RotateThis(csYRotMatrix3(yaw));
        break;
      }
      case iPcCamera::rotational:
      {
      	csRef<iPcMesh> pcmesh (
		CEL_QUERY_PROPCLASS (entity->GetPropertyClassList(), iPcMesh));
	if (!pcmesh) break;
	csBox3 b;
	csVector3 pos;
	pcmesh->GetMesh()->GetWorldBoundingBox(b);
	pos = b.GetCenter();
	float min_dist = qsqrt (pow(pos.x - b.Max().x, 2)
		+ pow(pos.y - b.Max().y,2) + pow(pos.z - b.Max().z,2));

	iCamera* c = view->GetCamera();

	if (mouse->GetLastButton(1) && mouse->GetLastButton(2))
	{
	  int _delta_y, _current_y;

	  if (!alter_dist)
	  {
	    alter_dist = true;
	    base_y_d = _current_y = mouse->GetLastY();
	  }
	  else
	  {
	    _current_y = mouse->GetLastY();
	  }

	  _delta_y = base_y_d - _current_y;
	  float delta_dist = _delta_y/100.0;
	  _dist = dist_y + delta_dist;
	  if (_dist < min_dist) _dist = min_dist;
	}
	else
	{
	  alter_dist = false;
	  dist_y = _dist;
	}

	if (!mouse->GetLastButton(1) && mouse->GetLastButton(2))
	{
	  int delta_x, delta_y, current_x, current_y;

	  if (!alter_angle)
	  {
	    alter_angle = true;
	    base_x = current_x = mouse->GetLastX();
	    base_y = current_y = mouse->GetLastY();
	    //csVector camera = c->GetOrigin();
	  }
	  else
	  {
	    current_x = mouse->GetLastX();
	    current_y = mouse->GetLastY();
	  }

	  delta_x = base_x - current_x;
	  delta_y = base_y - current_y;

	  float delta_yz = delta_x/200.0;
	  float delta_xz = delta_y/300.0;

	  _yz = angle_yz + delta_yz;
	  _xz = angle_xz + delta_xz;
	}
	else
	{
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
  region = newregion;

  if (point)
  {
    csRef<iPcCamera> camera (SCF_QUERY_INTERFACE (this, iPcCamera));
    region->PointCamera(camera, name);
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

csPtr<iCelDataBuffer> celPcCamera::Save ()
{
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (CAMERA_SERIAL));
  databuf->SetDataCount (3+11+7);
  celDataBufHelper db(databuf);

  csRef<iCelPropertyClass> pc;
  if (region) pc = SCF_QUERY_INTERFACE (region, iCelPropertyClass);
  db.Set (pc);
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

  db.Set (followat);
  db.Set (followpos);

  db.Set ((uint8)cammode);
  db.SetBool (use_cd);
  db.SetBool (rect_set);
  db.Set ((uint16)rect_x);
  db.Set ((uint16)rect_y);
  db.Set ((uint16)rect_w);
  db.Set ((uint16)rect_h);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcCamera::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != CAMERA_SERIAL) return false;
  if (databuf->GetDataCount () != 3+11+7) return false;
  celDataBufHelper db(databuf);

  csMatrix3 m_o2t;
  csVector3 v_o2t;

  iCelPropertyClass* pc;
  if (!db.Get(pc)) return false;
  if (pc) region = SCF_QUERY_INTERFACE (pc, iPcRegion);
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

  db.Get(followpos);
  db.Get(followat);

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
  return view->GetCamera ();
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

  UpdateProperties (object_reg);
  propdata = new void* [propertycount];

  props = properties;
  propcount = &propertycount;

  propdata[propid_worlddir] = &worlddir;
  propdata[propid_worldfile] = &worldfile;
  propdata[propid_regionname] = &regionname;

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

csPtr<iCelDataBuffer> celPcRegion::Save ()
{
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (REGION_SERIAL));
  databuf->SetDataCount (4);
  celDataBufHelper db(databuf);

  db.Set (worlddir);
  db.Set (worldfile);
  db.Set (regionname);
  db.SetBool (loaded);

  return csPtr<iCelDataBuffer> (databuf);
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

int celPcRegion::propertycount = 0;
Property* celPcRegion::properties = NULL;

void celPcRegion::UpdateProperties( iObjectRegistry* object_reg )
{
  if( propertycount == 0 )
  {
    csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY( object_reg, iCelPlLayer ));
    CS_ASSERT( pl != NULL );

    propertycount = 4;
    properties = new Property[propertycount];

    properties[propid_worlddir].id = pl->FetchStringID( "cel.property.pcregion.worlddir" );
    properties[propid_worlddir].datatype = CEL_DATA_STRING;
    properties[propid_worlddir].readonly = false;
    properties[propid_worlddir].desc = "Map VFS path.";

    properties[propid_worldfile].id = pl->FetchStringID( "cel.property.pcregion.worldfile" );
    properties[propid_worldfile].datatype = CEL_DATA_STRING;
    properties[propid_worldfile].readonly = false;
    properties[propid_worldfile].desc = "Map VFS file name.";

    properties[propid_regionname].id = pl->FetchStringID( "cel.property.pcregion.regionname" );
    properties[propid_regionname].datatype = CEL_DATA_STRING;
    properties[propid_regionname].readonly = false;
    properties[propid_regionname].desc = "Name of this region.";

    properties[propid_load].id = pl->FetchStringID( "cel.action.pcregion.load" );
    properties[propid_load].datatype = CEL_DATA_ACTION;
    properties[propid_load].readonly = true;
    properties[propid_load].desc = "Load the map.\nNo parameters";
  }
}

bool celPcRegion::PerformAction( csStringID actionId, const char* params )
{
  if( actionId == properties[propid_load].id ) {
    if( worldfile && regionname )
      Load();
    else {
      printf( "World filename or region name not set!\n" );
      return false;
    }
    return true;
  }
  return false;
}

void celPcRegion::SetWorldFile (const char* vfsdir, const char* name)
{
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

bool celPcRegion::Load ()
{
  bool rc = true;

  if (loaded) return true;
  if (!worlddir) return false;
  if (!worldfile) return false;
  if (!regionname) return false;

  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
  CS_ASSERT (engine != NULL);
  iRegion* old_region = engine->GetCurrentRegion ();
  engine->SelectRegion (regionname);
  iRegion* cur_region = engine->GetCurrentRegion ();
  cur_region->DeleteAll ();

  csRef<iLoader> loader (CS_QUERY_REGISTRY (object_reg, iLoader));
  CS_ASSERT (loader != NULL);
  csRef<iVFS> VFS (CS_QUERY_REGISTRY (object_reg, iVFS));
  CS_ASSERT (VFS != NULL);
  VFS->ChDir (worlddir);
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
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  CS_ASSERT (pl != NULL);
  iCelPropertyClass* pc;
  csRef<iObjectIterator> iter (cur_region->QueryObject ()->GetIterator ());
  while (!iter->IsFinished ())
  {
    iObject* o = iter->GetObject ();
    csRef<iMeshWrapper> m (SCF_QUERY_INTERFACE (o, iMeshWrapper));
    if (m)
    {
      csRef<iCelEntity> ent (pl->CreateEntity ());
      ent->SetName ("");

      pc = pl->CreatePropertyClass (ent, "pcmesh");
      csRef<iPcMesh> pcmesh (SCF_QUERY_INTERFACE (pc, iPcMesh));
      pcmesh->SetMesh (m);

      pc = pl->CreatePropertyClass (ent, "pcsolid");
      entities.Push (ent);
      DG_LINK (this, ent->QueryObject ());
    }
    iter->Next ();
  }
  }

cleanup:
  engine->SelectRegion (old_region);
  return rc;
}

void celPcRegion::Unload ()
{
  if (!loaded) return;
  loaded = false;
  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
  CS_ASSERT (engine != NULL);

  iRegion* old_region = engine->GetCurrentRegion ();
  engine->SelectRegion (regionname);

  int i;
  for (i = 0 ; i < entities.Length () ; i++)
    DG_UNLINK (this, entities[i]);
  entities.DeleteAll ();

  iRegion* cur_region = engine->GetCurrentRegion ();
  engine->GetCurrentRegion ()->DeleteAll ();
  engine->SelectRegion (old_region);
  engine->GetRegions ()->Remove (cur_region);
}

iSector* celPcRegion::FindSector (const char* name)
{
  csRef<iEngine> engine (CS_QUERY_REGISTRY(object_reg, iEngine));
  iSector* temp = engine->GetSectors()->FindByName (name);
  return temp;
}

iSector* celPcRegion::GetStartSector (const char* name)
{
  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
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
  return sector;
}

csVector3 celPcRegion::GetStartPosition (const char* name)
{
  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
  CS_ASSERT (engine != NULL);
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
  CS_ASSERT(pccamera != NULL);

  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
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

//---------------------------------------------------------------------------

