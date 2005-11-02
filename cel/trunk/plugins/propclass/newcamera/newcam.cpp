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
#include "plugins/propclass/newcamera/thirdpersoncameramode.h"
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

CEL_IMPLEMENT_FACTORY(NewCamera, "pcnewcamera")

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcNewCamera)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcNewCamera)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCamera)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcNewCamera::PcNewCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcNewCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcCamera)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

void celPcNewCamera::UpdateMeshVisibility()
{
  if (!pcmesh)
    return;

  if (currMode >= cameraModes.Length())
    return;

  if (cameraModes[currMode]->DrawAttachedMesh())
    pcmesh->GetMesh()->SetFlagsRecursive(CS_ENTITY_INVISIBLE, 0);
  else
    pcmesh->GetMesh()->SetFlagsRecursive(CS_ENTITY_INVISIBLE,
	CS_ENTITY_INVISIBLE);
}

void celPcNewCamera::GetActorTransform()
{
  // Try to get position and sector from the mesh.
  if (pcmesh)
  {
    iMovable* movable = pcmesh->GetMesh()->GetMovable();
    baseTrans = movable->GetFullTransform();
    baseSector = movable->GetSectors()->Get(0);
  }
  else
  {
    baseTrans.SetT2O(csMatrix3());
    baseSector = 0;
  }
}

void celPcNewCamera::CalcElasticVec(const csVector3 & curr, const csVector3 & ideal,
    float deltaTime, float springCoef, csVector3 & newVec)
{
  csVector3 deltaVec;

  deltaVec = curr - ideal;
  if (deltaVec.SquaredNorm() <= 0.001f)
  {
    newVec = curr;
	return;
  }
  
  float len = deltaVec.Norm();
  deltaVec *= springCoef * len * deltaTime;
  if (deltaVec.SquaredNorm() >= len*len)
    newVec = curr;
  else
	newVec = curr - deltaVec;
}

celPcNewCamera::celPcNewCamera(iObjectRegistry* object_reg)
  : celPcCameraCommon(object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcNewCamera);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCamera);

  engine = CS_QUERY_REGISTRY(object_reg, iEngine);
  g3d = CS_QUERY_REGISTRY(object_reg, iGraphics3D);
  vc = CS_QUERY_REGISTRY(object_reg, iVirtualClock);
  view = csPtr<iView>(new csView(engine, g3d));

  pl->CallbackEveryFrame((iCelTimerListener*)this, cscmdProcess);

  basePosOffset.Set(0,0,0);

  lastIdealPos.Set(0,0,0);
  lastIdealDir.Set(0,0,0);
  lastIdealUp.Set(0,0,0);

  currMode = (size_t)-1;
}

celPcNewCamera::~celPcNewCamera()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcNewCamera);
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcCamera);
}

void celPcNewCamera::PropertyClassesHaveChanged()
{
  celPcCommon::PropertyClassesHaveChanged();

  pcmesh = CEL_QUERY_PROPCLASS_ENT(entity, iPcMesh);

  // reset the camera values
  if (pcmesh && pcmesh->GetMesh())
  {
    iMovable * movable = pcmesh->GetMesh()->GetMovable();
    camPos = lastIdealPos = movable->GetTransform().GetOrigin();
    camDir = lastIdealDir = movable->GetTransform().This2OtherRelative(csVector3(0,0,-1));
    camUp  = lastIdealUp = movable->GetTransform().This2OtherRelative(csVector3(0,1,0));
  }

  UpdateMeshVisibility();
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

const csReversibleTransform & celPcNewCamera::GetBaseTrans() const
{
  return baseTrans;
}

const csVector3 & celPcNewCamera::GetPos() const
{
  return camPos;
}

const csVector3 & celPcNewCamera::GetDir() const
{
  return camDir;
}

const csVector3 & celPcNewCamera::GetUp() const
{
  return camUp;
}

void celPcNewCamera::SetPositionOffset(const csVector3& offset)
{
  basePosOffset = offset;
}

size_t celPcNewCamera::AttachCameraMode(iCelCameraMode * mode)
{
  cameraModes.Push(mode);
  mode->SetParentCamera((iPcNewCamera *)&scfiPcNewCamera);

  return (cameraModes.Length()-1);
}

size_t celPcNewCamera::AttachCameraMode(iPcNewCamera::CEL_CAMERA_MODE mode)
{
  switch (mode)
  {
    case iPcNewCamera::CCM_FIRST_PERSON:
      return AttachCameraMode(new celFirstPersonCameraMode());
    case iPcNewCamera::CCM_THIRD_PERSON:
      return AttachCameraMode(new celThirdPersonCameraMode());
    default:
      return (size_t)-1;
  }
}

size_t celPcNewCamera::GetCurrentCameraModeIndex() const
{
  return currMode;
}

iCelCameraMode * celPcNewCamera::GetCurrentCameraMode()
{
  return cameraModes.Top();
}

bool celPcNewCamera::SetCurrentCameraMode(size_t modeIndex)
{
  if (modeIndex >= cameraModes.Length())
    return false;

  currMode = modeIndex;
  UpdateMeshVisibility();

  return true;
}

void celPcNewCamera::NextCameraMode()
{
  if (cameraModes.Length() == 0)
    return;

  size_t newMode = currMode+1;
  if (newMode >= cameraModes.Length())
    newMode = 0;
  SetCurrentCameraMode(newMode);
}

void celPcNewCamera::PrevCameraMode()
{
  if (cameraModes.Length() == 0)
    return;

  size_t newMode = currMode-1;
  if (newMode == (size_t)-1)
    newMode = cameraModes.Length()-1;
  SetCurrentCameraMode(newMode);
}

void celPcNewCamera::Draw()
{
  csTicks elapsedTime = vc->GetElapsedTicks();
  float elapsedSecs = elapsedTime / 1000.0f;

  if (currMode >= cameraModes.Length())
  {
    SetCurrentCameraMode(cameraModes.Length()-1);
    if (currMode >= cameraModes.Length())
      return;
  }
  iCelCameraMode * mode = cameraModes[currMode];
  
  GetActorTransform();
  if (!baseSector)
    return;

  basePos = baseTrans.GetOrigin() + baseTrans.This2OtherRelative(basePosOffset);
  baseDir = baseTrans.This2OtherRelative(csVector3(0,0,-1));
  baseUp  = baseTrans.This2OtherRelative(csVector3(0,1,0));
    
  if (!mode->DecideCameraState())
    return;

  // Adjust camera transform for relative position and lookat position.
  csReversibleTransform camTrans;
  camTrans.SetOrigin(baseTrans.GetOrigin());
  camTrans.LookAt(mode->GetDirection(), mode->GetUp());

  iCamera * c = view->GetCamera();

  if (mode->UseSpringPos())
    CalcElasticVec(camPos, mode->GetPosition(), elapsedSecs, mode->GetSpringCoefficient(), camPos);
  else
	camPos = mode->GetPosition();

  if (mode->UseSpringDir())
    CalcElasticVec(camDir, mode->GetDirection(), elapsedSecs, mode->GetSpringCoefficient(), camDir);
  else
    camDir = mode->GetDirection();

  if (mode->UseSpringUp())
    CalcElasticVec(camUp, mode->GetUp(), elapsedSecs, mode->GetSpringCoefficient(), camUp);
  else
    camUp = mode->GetUp();
  
  // First set the camera back on where the sector is.
  // We assume here that normal camera movement is good.
  if (c->GetSector() != baseSector)
    c->SetSector(baseSector);
  c->SetTransform(camTrans);
  c->OnlyPortals(true);

  // to increase the chances of the camera being in the correct sector, first
  // move to from the attached mesh origin to the camera's base position, then
  // move to the desired position traversing portals as we go
  c->MoveWorld(basePos - c->GetTransform().GetOrigin(), false);
  c->MoveWorld(camPos - c->GetTransform().GetOrigin(), false);

  // Tell 3D driver we're going to display 3D things.
  if (g3d->BeginDraw(engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS
    	| CSDRAW_CLEARZBUFFER))
    view->Draw();
}

void celPcNewCamera::TickEveryFrame()
{
    Draw();
}

