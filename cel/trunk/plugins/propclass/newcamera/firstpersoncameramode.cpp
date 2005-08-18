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
#include "plugins/propclass/newcamera/firstpersoncameramode.h"
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

celFirstPersonCameraMode::celFirstPersonCameraMode()
{
  posOffset = csVector3(0,0,0);

  parentCamera = 0;
}

celFirstPersonCameraMode::~celFirstPersonCameraMode()
{
}

void celFirstPersonCameraMode::SetParentCamera(iPcNewCamera * camera)
{
  parentCamera = camera;
}

bool celFirstPersonCameraMode::UseSpringPhysics() const
{
  return false;
}

bool celFirstPersonCameraMode::DrawAttachedMesh() const
{
  return false;
}

float celFirstPersonCameraMode::GetSpringCoefficient() const
{
  return 0;
}

float celFirstPersonCameraMode::GetSpringLength() const
{
  return 0;
}

float celFirstPersonCameraMode::GetInertialDampeningCoefficient() const
{
  return 0;
}

const csVector3 & celFirstPersonCameraMode::GetPositionOffset() const
{
  return posOffset;
}

const csVector3 & celFirstPersonCameraMode::GetDirection() const
{
  return parentCamera->GetBaseDir();
}

const csVector3 & celFirstPersonCameraMode::GetUp() const
{
  return parentCamera->GetBaseUp();
}

bool celFirstPersonCameraMode::DecideCameraState()
{
  if (!parentCamera)
    return false;
  return true;
}

