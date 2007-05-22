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
#include "plugins/propclass/newcamera/modes/laratrack.h"
#include "propclass/newcamera.h"

#include "csgeom/transfrm.h"

namespace celCameraMode
{

SCF_IMPLEMENT_FACTORY (LaraTrack)

LaraTrack::LaraTrack (iBase* p)
  : scfImplementationType (this, p)
{
  posoffset.Set (0, 3, 7);
}

LaraTrack::~LaraTrack ()
{
}

bool LaraTrack::DrawAttachedMesh() const
{
  return true;
}
bool LaraTrack::DecideCameraState()
{
  if (!parent)
    return false;

  csVector3 playpos (parent->GetBasePos ());

  pos = csVector3 (0, 0, parent->GetBasePos().z) + posoffset;
  target = parent->GetBasePos();
  up  = parent->GetBaseUp();
  return true;
}

}
