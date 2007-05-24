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
#include "iengine/movable.h"

namespace celCameraMode
{

SCF_IMPLEMENT_FACTORY (LaraTrack)

LaraTrack::LaraTrack (iBase* p)
  : scfImplementationType (this, p)
{
  posoffset.Set (0, 3, 3);
  pos = posoffset;

  init_reset = false;
  up.Set (0,1,0);

  tracktarget = 0;
  targetstate = TARGET_BASE;
  targetyoffset = 2;
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

const csVector3 &LaraTrack::GetAnchorPosition ()
{
  return parent->GetBasePos ();
}
const csVector3 &LaraTrack::GetAnchorDirection ()
{
  return parent->GetBaseDir ();
}
const csVector3 &LaraTrack::GetTargetPosition ()
{
  return tracktarget->GetPosition ();
}

bool LaraTrack::DecideCameraState()
{
  if (!parent)
    return false;
  if (!init_reset)
    init_reset = ResetCamera ();

  csVector3 tarpos;
  switch (targetstate)
  {
    case (TARGET_BASE):
      tarpos = GetAnchorPosition ();
      break;
    case (TARGET_OBJ):
      tarpos = GetTargetPosition ();
      break;
    case (TARGET_NONE):
    default:
      break;
  }
  // get the position of the object we are anchored to in camera space
  const csVector3 &playpos (camtrans.Other2This (GetAnchorPosition ()));
  // how much does the camera have to move to be within the z offset
  csVector3 cammove (camtrans.This2OtherRelative (
    csVector3 (0,0,playpos.z - posoffset.z)));
  // enforce the rule to keep everything flat in the transform
  cammove.y = 0.0f;
  // actually move the camera
  camtrans.SetOrigin (camtrans.GetOrigin () + cammove);
  // track the target
  camtrans.LookAt (tarpos - camtrans.GetOrigin (), up);

  // since the camera transform exists in the same plane as the anchor
  // and up is fixed to (0,1,0) (our assumptions), then offset in y
  // (we ignore posoffset.x totally)
  pos = camtrans.GetOrigin () + csVector3(0,posoffset.y,0);
  // from transform, recompute target
  target = camtrans.This2Other (csVector3 (0,0,posoffset.z));
  target.y += targetyoffset;
  return true;
}

bool LaraTrack::ResetCamera ()
{
  if (!parent)
    return false;
  const csReversibleTransform &basetrans = parent->GetBaseTrans ();
  const csVector3 &basepos (parent->GetBasePos ());
  csVector3 offset (basetrans.This2OtherRelative (posoffset));
  offset.y = 0;
  camtrans.SetOrigin (basepos + offset);
  camtrans.LookAt (offset, parent->GetBaseUp ());
  return true;
}

bool LaraTrack::SetTargetEntity (const char* name)
{
  puts ("BOOM!");
  return false;
#if 0
  if (!name || name[0] = 0)
    do_track = false;
  iCelEntity* ent = pl->FindEntity (name);
  if (!ent)
    return false;
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity <iPcMesh> (ent);
  if (!pcmesh || !pcmesh->GetMesh ())
    return false;
  tracktarget = pcmesh->GetMesh ()->GetMovable ();
  do_track = true;
#endif
}
void LaraTrack::SetTargetState (TargetState targetstate)
{
  LaraTrack::targetstate = targetstate;
}
void LaraTrack::SetTargetYOffset (float targetyoffset)
{
  LaraTrack::targetyoffset = targetyoffset;
}

}
