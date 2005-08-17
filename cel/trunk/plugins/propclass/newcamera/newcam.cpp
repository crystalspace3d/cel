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
#include "plugins/propclass/newcamera/newcam.h"
#include "plugins/propclass/newcamera/firstpersoncameramode.h"
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

CEL_IMPLEMENT_FACTORY (NewCamera, "pcnewcamera")

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcNewCamera)
SCF_IMPLEMENT_IBASE_EXT_END

void celPcNewCamera::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

void celPcNewCamera::GetActorTransform (csReversibleTransform& actor_trans,
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

celPcNewCamera::celPcNewCamera (iObjectRegistry* object_reg)
  : celPcCommon (object_reg)
{
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  view = csPtr<iView> (new csView (engine, g3d));

  pl->CallbackEveryFrame ((iCelTimerListener*)this, cscmdProcess);

  AttachCameraMode (new celFirstPersonCameraMode ());
}

celPcNewCamera::~celPcNewCamera ()
{
}

const csVector3 & celPcNewCamera::GetBasePos() const
{
  return basePos;
}

const csVector3 & celPcNewCamera::GetBaseDir() const
{
  return baseDir;
}

const csVector3 & celPcNewCamera::GetBaseUp() const
{
  return baseUp;
}

void celPcNewCamera::SetCameraOffset (const csVector3& offset)
{
  basePosOffset = offset;
}

size_t celPcNewCamera::AttachCameraMode (iCelCameraMode * mode)
{
  cameraModes.Push(mode);
  mode->SetParentCamera(this);
  
  if (currMode < 0 || currMode >= cameraModes.Length())
    currMode = cameraModes.Length()-1;

  return (cameraModes.Length()-1);
}

size_t celPcNewCamera::GetCurrentCameraModeIndex () const
{
  return currMode;
}

iCelCameraMode * celPcNewCamera::GetCurrentCameraMode ()
{
  return cameraModes.Top();
}

bool celPcNewCamera::SetCurrentCameraMode (size_t modeIndex)
{
  if (modeIndex < 0 || modeIndex >= cameraModes.Length())
    return false;

  currMode = modeIndex;
  return true;
}

void celPcNewCamera::NextCameraMode ()
{
  if (cameraModes.Length() == 0)
    return;

  ++currMode;
  if (currMode >= cameraModes.Length())
    currMode = 0;
}

void celPcNewCamera::PrevCameraMode ()
{
  if (cameraModes.Length() == 0)
    return;

  --currMode;
  if (currMode < 0)
    currMode = cameraModes.Length()-1;
}

void celPcNewCamera::Draw ()
{
  if (currMode < 0 || currMode >= cameraModes.Length())
    return;
  iCelCameraMode * mode = cameraModes[currMode];
  
  csReversibleTransform actor_trans;
  iSector* cam_sector;
  GetActorTransform (actor_trans, cam_sector);
  if (cam_sector)
  {
    basePos = actor_trans.GetOrigin () + basePosOffset;
    baseDir = actor_trans.This2OtherRelative 
      (csVector3(0,0,1));
    baseUp = actor_trans.This2OtherRelative 
      (csVector3(0,1,0));
    
    // Adjust camera transform for relative position and lookat position.
    csReversibleTransform cam_trans;
    cam_trans.SetOrigin (mode->GetPositionOffset());
    cam_trans.LookAt (cam_trans.GetOrigin() + mode->GetDirection(),
	mode->GetUp());

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
    	| CSDRAW_CLEARZBUFFER))
    view->Draw ();
}

void celPcNewCamera::TickEveryFrame ()
{
    Draw();
}

