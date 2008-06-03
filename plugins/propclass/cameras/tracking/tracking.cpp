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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "cssysdef.h"
#include "iutil/objreg.h"
#include "plugins/propclass/cameras/tracking/tracking.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

// CS Includes
#include "cstool/collider.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "iutil/virtclk.h"
#include "ivaria/collider.h"

// CEL Includes
#include "propclass/mesh.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (TrackingCamera, "pccamera.mode.tracking")

//---------------------------------------------------------------------------

celPcTrackingCamera::celPcTrackingCamera (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);

  posoff.angle = M_PI / 6;
  posoff.dist = 6.5f;
  relaxspringlen = 2.0f;
  minspring = 0.01f;

  init_reset = false;

  parent = 0;
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

  was_corrected = false;
  zoomoutcorrspeed = 1.0f;
}

celPcTrackingCamera::~celPcTrackingCamera ()
{
}

#define TEST_SERIAL 0

csPtr<iCelDataBuffer> celPcTrackingCamera::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTrackingCamera::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL) return false;
  return true;
}

bool celPcTrackingCamera::PerformActionIndexed (int idx,
  iCelParameterBlock* params,
  celData& ret)
{
  switch (idx)
  {
    default:
      return false;
  }
  return false;
}

//---------------------------------------------------------------------------

void celPcTrackingCamera::SetOffsetAngle (float angle)
{
  posoff.angle = angle;
}
float celPcTrackingCamera::GetOffsetAngle () const
{
  return posoff.angle;
}
void celPcTrackingCamera::SetOffsetDistance (float dist)
{
  posoff.dist = dist;
}
float celPcTrackingCamera::GetOffsetDistance () const
{
  return posoff.dist;
}

void celPcTrackingCamera::SetFollowSpringLength (float slen)
{
  relaxspringlen = slen;
}
float celPcTrackingCamera::GetFollowSpringLength () const
{
  return relaxspringlen;
}

void celPcTrackingCamera::SetFollowMinimumSpringFactor (float smin)
{
  minspring = smin;
}
float celPcTrackingCamera::SetFollowMinimumSpringFactor () const
{
  return minspring;
}

void celPcTrackingCamera::SetZoomOutCorrectionSpeed (float zoomspeed)
{
  zoomoutcorrspeed = zoomspeed;
}
float celPcTrackingCamera::GetZoomOutCorrectionSpeed () const
{
  return zoomoutcorrspeed;
}

float celPcTrackingCamera::SpringForce (const float movement)
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

void celPcTrackingCamera::Accelerator::Accelerate (int direction, float elapsedsecs)
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

void celPcTrackingCamera::PanAroundPlayer (const csVector3 &playpos, float elapsedsecs)
{
  // perform a rotation around the character
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

void celPcTrackingCamera::FindCorrectedTransform (float elapsedsecs)
{
  // get this value before it's lost
  float old_reallen = (corrtarget - corrorigin).Norm ();
  // do collision test
  const csTraceBeamResult beam = csColliderHelper::TraceBeam (cdsys, parent->GetSectors ()->Get (0),
    origin, target, true);
  if (beam.sqdistance > 0)
  {
    const csVector3 lookat (target - origin), dir (lookat.Unit ());
    float lookat_len = lookat.Norm ();
    // so we offset a proportional amount down the beam towards the player so as not to be
    // inside the wall.
    corrorigin = beam.closest_isect + 0.1f * (lookat_len - sqrt (beam.sqdistance)) * dir;
    // so camera can start a slow zoom out as soon as the lookat_len starts increasing
    was_corrected = true;
  }
  else
    corrorigin = origin;
  // target unchanged
  corrtarget = target;

  if (was_corrected)
  {
    // reverse lookat vector
    const csVector3 clookat (corrtarget - corrorigin);
    // if the old length and this one do not match
    if (clookat.SquaredNorm () - old_reallen * old_reallen > EPSILON)
    {
      // then interpolate the lengths to the target
      float i = zoomoutcorrspeed * elapsedsecs;
      // clamp within [0, 1] range as usual
      if (i > 1.0f)
        i = 1.0f;
      else if (i < 0.0f)
        i = 0.0f;
      float corrlen = clookat.Norm ();
      // go down from the target because i felt like coding this a bit differently for fun
      corrorigin = corrtarget - clookat.Unit () * (i * corrlen + (1.0f - i) * old_reallen);
    }
    // so switch this off if we finished the interpolating at last
    else
      was_corrected = false;
  }
}

bool celPcTrackingCamera::DecideState ()
{
  if (!parent)
  {
    csRef<iPcMesh> mesh = celQueryPropertyClassEntity<iPcMesh> (GetEntity ());
    if (mesh && mesh->GetMesh ())
      parent = mesh->GetMesh ()->GetMovable ();
  }
  if (!parent)
    return false;
  if (!init_reset)
    init_reset = ResetCamera ();

  float elapsedsecs = vc->GetElapsedTicks () / 1000.0f;

  // a bit of fun, but not really needed :)
  // might keep it since it looks nice though
  float dxf = 5 * posoff.dist * posoff.angle / M_PI;
  float
    posoffset_y = dxf * sin (posoff.angle),
    posoffset_z = dxf * cos (posoff.angle);

  const csVector3 playpos (parent->GetFullPosition ());
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

    PanAroundPlayer (playpos, elapsedsecs);

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

    PanAroundPlayer (playpos, elapsedsecs);
  }
  else if (targetstate == TARGET_OBJ)
  {
    const csVector3 tarpos (tracktarget->GetFullPosition ());
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

  FindCorrectedTransform (elapsedsecs);

  const csReversibleTransform &trans = parent->GetFullTransform ();
  up  = trans.This2OtherRelative (csVector3 (0.0f, 1.0f, 0.0f));
  return true;
}

bool celPcTrackingCamera::ResetCamera ()
{
  if (!parent)
    return false;
  // no, we don't need to realign the camera. we're already doing this.
  if (targetstate == TARGET_OBJ)
    return true;
  // get the transform and position of our anchor object...
  const csReversibleTransform &basetrans = parent->GetFullTransform ();
  const csVector3 &basepos (basetrans.GetOrigin ());
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

bool celPcTrackingCamera::SetTargetEntity (const char* name)
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
void celPcTrackingCamera::SetTargetState (TargetState targetstate)
{
  celPcTrackingCamera::targetstate = targetstate;
}
void celPcTrackingCamera::SetTargetYOffset (float yoff)
{
  targetyoffset = yoff;
}

void celPcTrackingCamera::Pan (PanDirection pdir)
{
  pandir = pdir;
}
celPcTrackingCamera::PanDirection celPcTrackingCamera::GetPanDirection () const
{
  return pandir;
}

void celPcTrackingCamera::SetPanSpeed (float pspeed)
{
  pan.topspeed = pspeed;
}
float celPcTrackingCamera::GetPanSpeed () const
{
  return pan.topspeed;
}

void celPcTrackingCamera::SetPanAcceleration (float paccel)
{
  pan.accel = paccel;
}
float celPcTrackingCamera::GetPanAcceleration () const
{
  return pan.accel;
}

void celPcTrackingCamera::Tilt (TiltDirection tdir)
{
  tiltdir = tdir;
}
celPcTrackingCamera::TiltDirection celPcTrackingCamera::GetTiltDirection () const
{
  return tiltdir;
}

void celPcTrackingCamera::SetTiltSpeed (float tspeed)
{
  tilt.topspeed = tspeed;;
}
float celPcTrackingCamera::GetTiltSpeed () const
{
  return tilt.topspeed;
}

void celPcTrackingCamera::SetTiltAcceleration (float taccel)
{
  tilt.accel = taccel;
}
float celPcTrackingCamera::GetTiltAcceleration () const
{
  return tilt.accel;
}

celPcTrackingCamera::TargetState celPcTrackingCamera::GetTargetState ()
{
  return targetstate;
}

const csVector3 &celPcTrackingCamera::GetPosition ()
{
  return corrorigin;
}
const csVector3 &celPcTrackingCamera::GetTarget ()
{
  return corrtarget;
}
const csVector3 &celPcTrackingCamera::GetUp ()
{
  return up;
}
