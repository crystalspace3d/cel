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
#include "plugins/propclass/newcamera/celcameramode.h"
#include "propclass/newcamera.h"
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

celCameraMode::celCameraMode()
{
  parentCamera = 0;

  pos = csVector3(0,0,0);
  dir = csVector3(0,0,0);
  up = csVector3(0,1,0);
}

celCameraMode::~celCameraMode()
{
}

void celCameraMode::SetParentCamera(iPcNewCamera * camera)
{
  parentCamera = camera;
}

bool celCameraMode::UseSpringPos() const
{
  return false;
}

bool celCameraMode::UseSpringDir() const
{
  return false;
}

bool celCameraMode::UseSpringUp() const
{
  return false;
}

bool celCameraMode::DrawAttachedMesh() const
{
  return false;
}

float celCameraMode::GetSpringCoefficient() const
{
  return 1.0f;
}

const csVector3 & celCameraMode::GetPosition() const
{
  return pos;
}

const csVector3 & celCameraMode::GetDirection() const
{
  return dir;
}

const csVector3 & celCameraMode::GetUp() const
{
  return up;
}

bool celCameraMode::DecideCameraState()
{
  if (!parentCamera)
    return false;

  pos = parentCamera->GetBasePos();
  dir = parentCamera->GetBaseDir();
  up  = parentCamera->GetBaseUp();
  return true;
}

