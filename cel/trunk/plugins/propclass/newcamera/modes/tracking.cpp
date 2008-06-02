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
#include "iengine/movable.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"

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

Tracking::Tracking (iCelPlLayer* pl, iVirtualClock* vc)
  : scfImplementationType (this), pl (pl), vc (vc)
{
  posoffset.Set (0, 2.5f, 6);
  relaxspringlen = 2.0f;
  minspring = 0.01f;

  init_reset = false;

  tracktarget = 0;
  targetstate = TARGET_BASE;
  targetyoffset = 2;

  pandir = PAN_NONE;
  pan.topspeed = 1.0f;
  pan.speed = 0.0f;
  pan.accel = 3.0f;
  tiltdir = TILT_NONE;
  tilt.topspeed = 1.0f;
  tilt.speed = 0.0f;
  tilt.accel = 3.0f;
}

Tracking::~Tracking ()
{
}

void Tracking::SetPositionOffset (const csVector3 &offset)
{
  posoffset = offset;
}
const csVector3 &Tracking::GetPositionOffset () const
{
  return posoffset;
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
  // v = ut + 0.5 * a t^2
  float cacc;
  if (!direction && fabs (speed) > EPSILON)
    cacc = (speed < 0) ? accel : -accel;
  else
    cacc = direction * accel;

  if (!direction &&
    ((speed > 0 && speed + cacc < 0) || (speed < 0 && speed + cacc > 0)))
    speed = 0.0f;
  else
    speed += 0.5 * cacc * elapsedsecs;

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
      tilt.Accelerate (-1, elapsedsecs);
      break;
    case TILT_DOWN:
      tilt.Accelerate (1, elapsedsecs);
      break;
  }

  // avoid needless calculation if so
  if (fabs (tilt.speed) > EPSILON)
  {
    // get current angle...
    angle = atan2 (posoffset.z, posoffset.y);
    // .. and increment
    angle += tilt.speed * elapsedsecs;
    // some maths now to transform posoffset through our angle :]
    float ratio = tan (angle);
    float sqnorm = posoffset.SquaredNorm ();
    // A^2 = B^2 + C^2
    // r = tan(a) = C / B
    // => A^2 = r^2 C^2 + C^2
    // => A^2 / (r^2 + 1) = C^2
    // now plug C back into first equation to find B
    posoffset.y = sqrt (sqnorm / (ratio*ratio + 1));
    // correct the fucking y if needed
    if (angle > M_PI/2)
      posoffset.y *= -1;
    posoffset.z = sqrt (sqnorm - posoffset.y * posoffset.y);
  }
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
    // in case you don't realise, it's the distance along camdir until
    // there's a perpendicular bisecting camera -> player...
    // ... this is so the camera only follows player in and out of the screen
    float move = dist * camdir * camplay - posoffset.z;

    origin += SpringForce (move) * move * camdir;
    // lock y axis to fixed distance above player
    origin.y = playpos.y + posoffset.y;

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

  FindCorrectedTransform ();
  corrorigin = origin;
  corrtarget = target;

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
