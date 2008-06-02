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

// CS Includes
#include "csgeom/poly3d.h"
#include "cstool/collider.h"
#include "iengine/movable.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"
#include "ivaria/collider.h"

// CEL Includes
#include "physicallayer/pl.h"
#include "propclass/mesh.h"
#include "propclass/solid.h"
#include "propclass/zone.h"
#include "plugins/propclass/newcamera/modes/tracking.h"
#include "propclass/newcamera.h"

namespace celCameraMode
{

SCF_IMPLEMENT_FACTORY (Tracking)

Tracking::Tracking (iBase* p)
  : scfImplementationType (this, p)
{
}

Tracking::Tracking (iCelPlLayer* pl, iVirtualClock* vc, iCollideSystem* cdsys)
  : scfImplementationType (this), pl (pl), vc (vc), cdsys (cdsys)
{
  posoff.angle = M_PI / 6;
  posoff.dist = 6.5f;
  relaxspringlen = 2.0f;
  minspring = 0.01f;

  init_reset = false;

  tracktarget = 0;
  targetstate = TARGET_BASE;
  targetyoffset = 2;

  pandir = PAN_NONE;
  pan.topspeed = 3.0f;
  pan.speed = 0.0f;
  pan.accel = 8.0f;
  tiltdir = TILT_NONE;
  tilt.topspeed = 1.0f;
  tilt.speed = 0.0f;
  tilt.accel = 3.0f;
}

Tracking::~Tracking ()
{
}

void Tracking::SetOffsetAngle (float angle)
{
  posoff.angle = angle;
}
float Tracking::GetOffsetAngle () const
{
  return posoff.angle;
}
void Tracking::SetOffsetDistance (float dist)
{
  posoff.dist = dist;
}
float Tracking::GetOffsetDistance () const
{
  return posoff.dist;
}

void Tracking::SetFollowSpringLength (float slen)
{
  relaxspringlen = slen;
}
float Tracking::GetFollowSpringLength () const
{
  return relaxspringlen;
}

void Tracking::SetFollowMinimumSpringFactor (float smin)
{
  minspring = smin;
}
float Tracking::SetFollowMinimumSpringFactor () const
{
  return minspring;
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

float Tracking::SpringForce (const float movement)
{
  // so now we add the little bit of springiness to our camera
  // you can visualise the camera <--> player connection as a hard pole with a spring on the end
  //   C----|oooP
  // the ---- (hard pole) can never be compressed except by collision detection
  // whereas the springs ooo can be decompressed and stretched between 0 and 2 * normal spring length
  // ... the spring is relaxed at the normal spring length
  float spring = fabs (movement / relaxspringlen);
  if (spring > 1.0f)
    spring = 1.0f;
  // we use a quadratic function because it makes the spring more realistic and noticeable
  spring = spring * spring * spring * spring;
  // cut off the bottom values to avoid stupidly slow movement... kinda epsilon value
  if (spring < minspring)
    spring = minspring;
  return spring;
}

void Tracking::Accelerator::Accelerate (int direction, float elapsedsecs)
{
  // calculate current acceleration... do we go left... right? nothing?
  float cacc;
  // no direction but have some speed, then slowdown in opposite direction
  if (!direction && fabs (speed) > EPSILON)
    cacc = (speed < 0) ? accel : -accel;
  else  // normal
    cacc = direction * accel;

  // this is to actually stop if we're slowing down.
  if (!direction &&
    ((speed > 0 && speed + cacc < 0) || (speed < 0 && speed + cacc > 0)))
    speed = 0.0f;
  // otherwise we can just speed up using v = a t
  else
    speed += 0.5 * cacc * elapsedsecs;

  // cap speed to limits
  if (speed > topspeed)
    speed = topspeed;
  else if (speed < -topspeed)
    speed = -topspeed;
}

void Tracking::PanAroundPlayer (const csVector3 &playpos)
{
  // perform a rotation around the character
  float elapsedsecs = vc->GetElapsedTicks () / 1000.0f;

  // accelerate speed in desired direction
  switch (pandir)
  {
    case PAN_NONE:
      pan.Accelerate (0, elapsedsecs);
      break;
    case PAN_LEFT:
      pan.Accelerate (-1, elapsedsecs);
      break;
    case PAN_RIGHT:
      pan.Accelerate (1, elapsedsecs);
      break;
  }

  float angle = pan.speed * elapsedsecs;
  // minor optimisation
  if (fabs (angle) > EPSILON)
  {
    // x' = x cos a - y sin a
    // y' = x sin a + y cos a
    csVector3 pc (origin - playpos);
    origin.x = pc.x * cos (angle) - pc.z * sin (angle) + playpos.x;
    origin.z = pc.x * sin (angle) + pc.z * cos (angle) + playpos.z;
  }

  switch (tiltdir)
  {
    case TILT_NONE:
      tilt.Accelerate (0, elapsedsecs);
      break;
    case TILT_UP:
      tilt.Accelerate (1, elapsedsecs);
      break;
    case TILT_DOWN:
      tilt.Accelerate (-1, elapsedsecs);
      break;
  }

  posoff.angle += tilt.speed * elapsedsecs;
  // we limit the angles between epsilon and M_PI/2 - epsilon
  // to stop the evilness of rotating to the front of the character!!
  if (posoff.angle < 0.1)
    posoff.angle = 0.1f;
  else if (posoff.angle > M_PI / 2 - 0.1f)
    posoff.angle = M_PI / 2 - 0.1f;
}

void Tracking::FindCorrectedTransform ()
{
  const csTraceBeamResult beam = csColliderHelper::TraceBeam (cdsys, parent->GetBaseSector (),
    origin, target, true);
  if (beam.sqdistance > 0)
    corrorigin = beam.closest_isect;
  else
    corrorigin = origin;
  corrtarget = target;
}

bool Tracking::DecideCameraState ()
{
  if (!parent)
    return false;
  if (!init_reset)
    init_reset = ResetCamera ();

  // a bit of fun, but not really needed :)
  // might keep it since it looks nice though
  float dxf = 5 * posoff.dist * posoff.angle / M_PI;
  float
    posoffset_y = dxf * sin (posoff.angle),
    posoffset_z = dxf * cos (posoff.angle);

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
    // in case you don't realise, it's the distance along camdir until
    // there's a perpendicular bisecting camera -> player...
    // ... this is so the camera only follows player in and out of the screen
    float move = dist * camdir * camplay - posoffset_z;

    origin += SpringForce (move) * move * camdir;
    // lock y axis to fixed distance above player
    origin.y = playpos.y + posoffset_y;

    PanAroundPlayer (playpos);

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
    origin = playpos - camdir * posoffset_z;
    // lock y axis to fixed distance above player
    origin.y = playpos.y + posoffset_y;
    // update target to continue facing old direction
    target = origin + camdir;

    PanAroundPlayer (playpos);
  }
  else if (targetstate == TARGET_OBJ)
  {
    const csVector3 tarpos (GetTargetPosition ());
    // project from player to the target to get the vector
    // so we can project backwards for the camera
    csVector3 camdir (origin - tarpos);
    camdir.Normalize ();
    origin = playpos + camdir * posoffset_z;
    // lock y axis to fixed distance above player
    origin.y = playpos.y + posoffset_y;
    // setup the target
    target = tarpos;
    target.y += targetyoffset;
  }

  FindCorrectedTransform ();

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
  	csVector3 (0,0,posoff.dist)));
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
void Tracking::SetTargetYOffset (float yoff)
{
  targetyoffset = yoff;
}

void Tracking::Pan (PanDirection pdir)
{
  pandir = pdir;
}
Tracking::PanDirection Tracking::GetPanDirection () const
{
  return pandir;
}

void Tracking::SetPanSpeed (float pspeed)
{
  pan.topspeed = pspeed;
}
float Tracking::GetPanSpeed () const
{
  return pan.topspeed;
}

void Tracking::SetPanAcceleration (float paccel)
{
  pan.accel = paccel;
}
float Tracking::GetPanAcceleration () const
{
  return pan.accel;
}

void Tracking::Tilt (TiltDirection tdir)
{
  tiltdir = tdir;
}
Tracking::TiltDirection Tracking::GetTiltDirection () const
{
  return tiltdir;
}

void Tracking::SetTiltSpeed (float tspeed)
{
  tilt.topspeed = tspeed;;
}
float Tracking::GetTiltSpeed () const
{
  return tilt.topspeed;
}

void Tracking::SetTiltAcceleration (float taccel)
{
  tilt.accel = taccel;
}
float Tracking::GetTiltAcceleration () const
{
  return tilt.accel;
}

iPcmNewCamera::Tracking::TargetState Tracking::GetTargetState ()
{
  return targetstate;
}

// Interface Functions
void Tracking::SetParentCamera (iPcNewCamera* camera)
{
  parent = camera;;
}
bool Tracking::UseSpringPos () const
{
  return false;
}
bool Tracking::UseSpringOrigin () const
{
  return false;
}
bool Tracking::UseSpringTarget () const
{
  return false;
}
bool Tracking::UseSpringUp () const
{
  return false;
}
bool Tracking::AllowCollisionDetection () const
{
  return GetCollisionDetection ();
}
bool Tracking::GetCollisionDetection () const
{
  return false;
}
float Tracking::GetSpringCoefficient () const
{
  return false;
}
void Tracking::SetSpringCoefficient (float s)
{
}
float Tracking::GetOriginSpringCoefficient () const
{
  return 0.0f;
}
void Tracking::SetOriginSpringCoefficient (float s)
{
}
float Tracking::GetTargetSpringCoefficient () const
{
  return 0.0f;
}
void Tracking::SetTargetSpringCoefficient (float s)
{
}
float Tracking::GetUpSpringCoefficient () const
{
  return 0.0f;
}
void Tracking::SetUpSpringCoefficient (float s)
{
}
const csVector3 &Tracking::GetPosition () const
{
  return corrorigin;
}
const csVector3 &Tracking::GetOrigin () const
{
  return corrorigin;
}
const csVector3 &Tracking::GetTarget () const
{
  return corrtarget;
}
const csVector3 &Tracking::GetUp () const
{
  return up;
}

}
