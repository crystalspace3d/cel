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

namespace celCameraMode
{

SCF_IMPLEMENT_FACTORY (LaraTrack)

LaraTrack::LaraTrack (iBase* p)
  : scfImplementationType (this, p)
{
  posoffset.Set (0, 3, 3);
  pos = posoffset;

  init_reset = false;
}

LaraTrack::~LaraTrack ()
{
}

void LaraTrack::SetPositionOffset(const csVector3 &offset)
{
  posoffset = offset;
}
bool LaraTrack::DrawAttachedMesh() const
{
  return true;
}

bool LaraTrack::DecideCameraState()
{
  if (!parent)
    return false;
  //init_reset = false;
  if (!ResetCamera ())
    return false;

  const csVector3 &basepos (parent->GetBasePos ());
  const csVector3 &playpos (camtrans.Other2This (basepos));
  csVector3 cammove (camtrans.This2OtherRelative (csVector3 (0,0,playpos.z - 5)));
  cammove.y = 0.0f;
  camtrans.SetOrigin (camtrans.GetOrigin () + cammove);
  camtrans.LookAt (basepos - camtrans.GetOrigin (), parent->GetBaseUp ());
  pos = camtrans.GetOrigin ();
  target = parent->GetBasePos ();
  up = parent->GetBaseUp ();
  return true;
}

bool LaraTrack::ResetCamera ()
{
  if (!parent)
    return false;
  if (init_reset)
    return true;
  init_reset = true;
  const csReversibleTransform &basetrans = parent->GetBaseTrans ();
  const csVector3 &basepos (parent->GetBasePos ());
  csVector3 offset (basetrans.This2OtherRelative (posoffset));
  offset.y = 0;
  camtrans.SetOrigin (basepos + offset);
  camtrans.LookAt (offset, parent->GetBaseUp ());
  return true;
}

}
