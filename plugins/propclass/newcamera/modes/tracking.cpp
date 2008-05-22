/*
    Crystal Space Entity Layer
    Copyright (C) 2007 by Amir Taaki

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
#include "physicallayer/pl.h"
#include "propclass/mesh.h"
#include "propclass/solid.h"
#include "propclass/zone.h"
#include "plugins/propclass/newcamera/modes/tracking.h"
#include "propclass/newcamera.h"
#include "iengine/movable.h"
#include "iengine/mesh.h"

namespace celCameraMode
{

SCF_IMPLEMENT_FACTORY (Tracking)

Tracking::Tracking (iBase* p)
	: scfImplementationType (this, p)
{
}

Tracking::Tracking (iCelPlLayer* pl, iVirtualClock* vc)
	: scfImplementationType (this), pl (pl), vc (vc)
{
  posoffset.Set (0, 2, 5);

  init_reset = false;

  tracktarget = 0;
  targetstate = TARGET_BASE;
  targetyoffset = 2;

  pandir = PAN_NONE;
  panspeed = 0.3f;
}

Tracking::~Tracking ()
{
}

void Tracking::SetPositionOffset (const csVector3 &offset)
{
  posoffset = offset;
}
bool Tracking::DrawAttachedMesh () const
{
  return true;
}

const csVector3 &Tracking::GetAnchorPosition ()
{
  return parent->GetBaseOrigin ();
}
const csVector3 &Tracking::GetAnchorDirection ()
{
  return parent->GetBaseDir ();
}
const csVector3 &Tracking::GetTargetPosition ()
{
  return tracktarget->GetPosition ();
}

bool Tracking::DecideCameraState ()
{
  if (!parent)
    return false;
  if (!init_reset)
    init_reset = ResetCamera ();

  const csVector3 playpos (GetAnchorPosition ());
  if (targetstate == TARGET_BASE)
  {
    // get flat 2D vector (zero out y) of camera to the player
    csVector3 camplay (playpos - origin);
    camplay.y = 0.0f;
    float dist = camplay.Norm ();
    camplay.Normalize ();
    // Now get a 2D vector of the camera's direction
    csVector3 camdir (target - origin);
    camdir.y = 0.0f;
    camdir.Normalize ();
    // plug it into our simplified equation to get the movement needed
    //   cos (x) = camdir . camplay
    //   move = dist * cos (x)
    float move = dist * camdir * camplay - posoffset.z;
    origin += move * camdir;
    // lock y axis to fixed distance above player
    origin.y = playpos.y + posoffset.y;
    // perform a rotation around the character
    csTicks elapsedtime = vc->GetElapsedTicks ();
    if (pandir != PAN_NONE)
    {
      float angle = (pandir == PAN_LEFT) ? panspeed : -panspeed;  // other direction is PAN_RIGHT
      angle *= elapsedtime / 100.0f;
      //origin += csVector3 (camdir.z, 0, -camdir.x);
      // x' = x cos a - y sin a
      // y' = x sin a + y cos a
      csVector3 pc (origin - playpos);
      origin.x = pc.x * cos (angle) - pc.z * sin (angle) + playpos.x;
      origin.z = pc.x * sin (angle) + pc.z * cos (angle) + playpos.z;
    }
    // setup the target
    target = playpos;
    target.y += targetyoffset;
  }
  else if (targetstate == TARGET_NONE)
  {
    // Get a 2D vector of the camera's direction
    csVector3 camdir (target - origin);
    camdir.Normalize ();
    // stay lined up but move to behind the player
    origin = playpos - camdir * posoffset.z;
    // lock y axis to fixed distance above player
    origin.y = playpos.y + posoffset.y;
    // update target to continue facing old direction
    target = origin + camdir;
  }
  else if (targetstate == TARGET_OBJ)
  {
    const csVector3 tarpos (GetTargetPosition ());
    // project from player to the target to get the vector
    // so we can project backwards for the camera
    csVector3 camdir (origin - tarpos);
    camdir.Normalize ();
    origin = playpos + camdir * posoffset.z;
    // lock y axis to fixed distance above player
    origin.y = playpos.y + posoffset.y;
    // setup the target
    target = tarpos;
    target.y += targetyoffset;
  }

  up  = parent->GetBaseUp ();
  return true;
}

bool Tracking::ResetCamera ()
{
  if (!parent)
    return false;
  // no, we don't need to realign the camera. we're already doing this.
  if (targetstate == TARGET_OBJ)
    return true;
  // get the transform and position of our anchor object...
  const csReversibleTransform &basetrans = parent->GetBaseTrans ();
  const csVector3 &basepos (parent->GetBaseOrigin ());
  // compute our z offset from it, back along from its direction
  csVector3 offset (basetrans.This2OtherRelative (
  	csVector3 (0,0,posoffset.z)));
  // offset.y = 0; (assuming its up is (0,1,0))
  origin = basepos + offset;
  // setup the target
  target = basepos;
  target.y += targetyoffset;
  return true;
}

bool Tracking::SetTargetEntity (const char* name)
{
  // lookup entity
  iCelEntity* ent = pl->FindEntity (name);
  if (!ent)
    return false;
  // lookup mesh
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity <iPcMesh> (ent);
  if (!pcmesh || !pcmesh->GetMesh ())
    return false;
  // get movable
  tracktarget = pcmesh->GetMesh ()->GetMovable ();
  // great success!
  return true;
}
void Tracking::SetTargetState (TargetState targetstate)
{
  Tracking::targetstate = targetstate;
}
void Tracking::SetTargetYOffset (float targetyoffset)
{
  Tracking::targetyoffset = targetyoffset;
}
void Tracking::Pan (PanningDirection pdir)
{
  pandir = pdir;
}
void Tracking::SetPanningSpeed (float pspeed)
{
  panspeed = pspeed;
}

iPcmNewCamera::Tracking::TargetState Tracking::GetTargetState ()
{
  return targetstate;
}

}
