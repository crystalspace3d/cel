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
#include <math.h>
#include "propclass/mesh.h"
#include "propclass/solid.h"
#include "propclass/zone.h"
#include "plugins/propclass/simpcam/simpcam.h"
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
#include "csgeom/transfrm.h"
#include "ivaria/view.h"
#include "ivaria/collider.h"
#include "ivaria/reporter.h"
#include "ivideo/graph3d.h"
#include "csqsqrt.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (SimpleCamera, "pcsimplecamera")

//---------------------------------------------------------------------------

// Actions
csStringID celPcSimpleCamera::action_initcam = csInvalidStringID;

// Parameters for action_initcam
csStringID celPcSimpleCamera::param_campos = csInvalidStringID;
csStringID celPcSimpleCamera::param_lookat = csInvalidStringID;
csStringID celPcSimpleCamera::param_drawmesh = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcSimpleCamera)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcSimpleCamera)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCamera)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcSimpleCamera::PcSimpleCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcSimpleCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcCamera)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcSimpleCamera::celPcSimpleCamera (iObjectRegistry* object_reg)
  : celPcCameraCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcSimpleCamera);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCamera);

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  view = csPtr<iView> (new csView (engine, g3d));

  rect_set = false;
  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  mouse = CS_QUERY_REGISTRY (object_reg, iMouseDriver);
  CS_ASSERT (kbd != 0);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != 0);
  //cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);

  clear_zbuf = false;
  clear_screen = false;

  objectlookat = csVector3 ();
  objectcampos = csVector3 ();

  lastActorSector = 0;

  // Actions
  if (action_initcam == csInvalidStringID)
    action_initcam = pl->FetchStringID ("cel.action.InitCamera");

  // Parameters for action_initcam
  if (param_campos == csInvalidStringID)
    param_campos = pl->FetchStringID ("cel.parameter.campos");
  if (param_lookat == csInvalidStringID)
    param_lookat = pl->FetchStringID ("cel.parameter.lookat");
  if (param_drawmesh == csInvalidStringID)
    param_drawmesh = pl->FetchStringID ("cel.parameter.drawmesh");

  pl->CallbackEveryFrame ((iCelTimerListener*)this, cscmdProcess);
}

celPcSimpleCamera::~celPcSimpleCamera ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcSimpleCamera);
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcCamera);
}

void celPcSimpleCamera::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

void celPcSimpleCamera::GetActorTransform (csReversibleTransform& actor_trans,
	iSector*& actor_sector)
{
  // Try to get position and sector from the mesh.
  FindSiblingPropertyClasses ();
  if (pcmesh)
  {
    iMovable* movable = pcmesh->GetMesh()->GetMovable();
    actor_trans = movable->GetFullTransform ();
    actor_sector = movable->GetSectors ()->Get (0);
  }
  else
  {
    actor_trans.SetT2O (csMatrix3 ());
    actor_sector = 0;
  }
}

void celPcSimpleCamera::TickEveryFrame ()
{
  Draw();
}

bool celPcSimpleCamera::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_initcam)
  {
    CEL_FETCH_VECTOR3_PAR (campos,params,param_campos);
    if (p_campos)
      SetCameraOffset (campos);
    else
      Report (object_reg, "Couldn't get campos!");
    CEL_FETCH_VECTOR3_PAR (lookat,params,param_lookat);
    if (p_lookat)
      SetLookAtOffset (lookat);
    else
      Report (object_reg, "Couldn't get lookat!");
    CEL_FETCH_BOOL_PAR (drawmesh,params,param_drawmesh);
    if (p_drawmesh)
      SetDrawMesh (drawmesh);
    else
      Report (object_reg, "Couldn't get campos!");
    return true;
  }
  return false;
}

void celPcSimpleCamera::SetDrawMesh (bool draw)
{
  drawmesh = draw;
  if (pcmesh)
  {
    if (drawmesh)
    {
      pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE, 0);
    }
    else
    {
      pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE,
		      CS_ENTITY_INVISIBLE);
    }
  }
}

void celPcSimpleCamera::Draw()
{
  // First get elapsed time from the virtual clock.
  //csTicks elapsed_time = vc->GetElapsedTicks ();
  //float elapsed_sec = elapsed_time / 1000.0f;

  // Try to get position and sector from mesh.
  csReversibleTransform actor_trans;
  iSector* cam_sector;
  GetActorTransform (actor_trans, cam_sector);
  if (cam_sector)
  {
    // Adjust camera transform for relative position and lookat position.
    csReversibleTransform cam_trans;
    cam_trans.SetOrigin (actor_trans.This2Other (objectcampos));
    cam_trans.LookAt (cam_trans.Other2This (actor_trans.This2Other (
    	objectlookat)), actor_trans.This2OtherRelative (csVector3 (0, 1, 0)));

    iCamera* c = view->GetCamera ();
  
    // First set the camera back on where the sector is.
    // We assume here that normal camera movement is good.
    if (c->GetSector () != cam_sector)
      c->SetSector (cam_sector);
    c->SetTransform (cam_trans);
    c->OnlyPortals (true);
  }

  // Tell 3D driver we're going to display 3D things.
  if (g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS
    	| (clear_zbuf ? CSDRAW_CLEARZBUFFER : 0)
	| (clear_screen ? CSDRAW_CLEARSCREEN : 0)))
    view->Draw ();
}

void celPcSimpleCamera::SetCameraOffset (const csVector3& offset)
{
  objectcampos = offset;
}

void celPcSimpleCamera::SetLookAtOffset (const csVector3& lookat)
{
  objectlookat = lookat;
}

#define SIMPLE_CAMERA_SERIAL 3

csPtr<iCelDataBuffer> celPcSimpleCamera::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (SIMPLE_CAMERA_SERIAL);
  SaveCommon (databuf);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcSimpleCamera::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != SIMPLE_CAMERA_SERIAL)
  {
    Report (object_reg, "serialnr != SIMPLE_CAMERA_SERIAL.  Cannot load.");
    return false;
  }

  if (!LoadCommon (databuf)) return false;

  return true;
}

//---------------------------------------------------------------------------

