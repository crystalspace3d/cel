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
  posoffset.Set (0, 3, 3);
  pos = posoffset;
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


  const csOrthoTransform &camtr = parent->GetTransform ();
  csVector3 playpos (parent->GetBasePos ());
  playpos.x = 0.0f;
  playpos.y = 0.0f;
  /*const csVector3 &playpos (camtr.GetO2T () * parent->GetBasePos ());
  const csVector3 &relative = camtr.GetT2O () * csVector3 (0, 0, playpos.z);*/
  //printf ("%f (%s)\n", playpos.z, relative.Description().GetData ());
  printf ("player (%s)\n", parent->GetBasePos ().Description ().GetData ());

  pos = playpos + csVector3 (0, 3, -3);
  target = parent->GetBasePos();
  up  = parent->GetBaseUp();
  return true;
}

}
