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

#include "iengine/engine.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (TrackingCamera, "pccamera.mode.tracking")

PropertyHolder celPcTrackingCamera::propinfo;

//---------------------------------------------------------------------------

celPcTrackingCamera::celPcTrackingCamera (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);

  posoff.angle = PI / 6;
  posoff.dist = 8.0f;
  relaxspringlen = 2.0f;
  minspring = 0.01f;

  init_reset = false;

  parent = 0;
  tracktarget = 0;
  targetstate = TARGET_BASE;
  targetyoffset = 1.5f;
  in_tartransition = false;
  tarintime = 100;
  currtartrans = 0.0f;

  pandir = 0.0f;
  pan.topspeed = 3.0f;
  pan.speed = 0.0f;
  pan.accel = 8.0f;
  tiltdir = 0.0f;
  tilt.topspeed = 1.0f;
  tilt.speed = 0.0f;
  tilt.accel = 3.0f;

  corrpos.Set (0.0f);
  corrtar.Set (0.0f);

  was_corrected = false;
  zoomoutcorrspeed = 1.0f;

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.camera.tracking.action.");
    AddAction (action_reset, "ResetCamera");
  }

  // For properties.
  propinfo.SetCount (20);
  AddProperty (propid_pos, "position",
    CEL_DATA_VECTOR3, false, "Position.", &corrpos);
  AddProperty (propid_tar, "target",
    CEL_DATA_VECTOR3, false, "Target position.", &corrtar);
  AddProperty (propid_up, "up",
    CEL_DATA_VECTOR3, false, "Up direction.", &up);
  AddProperty (propid_pan_topspeed, "pan_topspeed",
    CEL_DATA_FLOAT, true, "Top speed limit for panning.", &pan.topspeed);
  AddProperty (propid_pan_currspeed, "pan_currspeed",
    CEL_DATA_FLOAT, false, "Current panning speed.", &pan.speed);
  AddProperty (propid_pan_accel, "pan_accel",
    CEL_DATA_FLOAT, true, "Pan acceleration.", &pan.accel);
  AddProperty (propid_pan_dir, "pan_dir",
    CEL_DATA_LONG, true, "Pan direction -1 left, 0 none, 1 right.", &pandir);
  AddProperty (propid_tilt_topspeed, "tilt_topspeed",
    CEL_DATA_FLOAT, true, "Top speed limit for tilting.", &tilt.topspeed);
  AddProperty (propid_tilt_currspeed, "tilt_currspeed",
    CEL_DATA_FLOAT, false, "Current tilt speed.", &tilt.speed);
  AddProperty (propid_tilt_accel, "tilt_accel",
    CEL_DATA_FLOAT, true, "Tilt acceleration.", &tilt.accel);
  AddProperty (propid_tilt_dir, "tilt_dir",
    CEL_DATA_LONG, true, "Tilt direction -1 down, 0 none, 1 up.", &tiltdir);
  AddProperty (propid_taryoff, "targetyoffset",
    CEL_DATA_FLOAT, true, "Y offset from target for lookat.", 0);
  AddProperty (propid_tarintrans, "target_intransition",
    CEL_DATA_BOOL, false, "Is target transitioning?", &in_tartransition);
  AddProperty (propid_tarintime, "targettranstime",
    CEL_DATA_LONG, true, "Time to transition to a new target.", &tarintime);
  AddProperty (propid_currtartrans, "targetcurrtrans",
    CEL_DATA_FLOAT, true, "Current transition of target between 0 and 1.", &currtartrans);
  AddProperty (propid_posoff_angle, "posoff_angle",
    CEL_DATA_FLOAT, true, "Position offset elevation angle.", &posoff.angle);
  AddProperty (propid_posoff_dist, "posoff_dist",
    CEL_DATA_FLOAT, true, "Position offset distance.", &posoff.dist);
  AddProperty (propid_spring_relaxlen, "spring_relaxlen",
    CEL_DATA_FLOAT, true, "Relaxed length of spring that follows player.", &relaxspringlen);
  AddProperty (propid_spring_minlen, "spring_minlen",
    CEL_DATA_FLOAT, true, "Minimum length of the spring (small values are good).", &minspring);
  AddProperty (propid_zoomoutcorrspeed, "zoomoutcorrspeed",
    CEL_DATA_FLOAT, true, "Zooming out correction speed.", &zoomoutcorrspeed);
}

celPcTrackingCamera::~celPcTrackingCamera ()
{
}

bool celPcTrackingCamera::SetPropertyIndexed (int idx, float f)
{
  if (idx == propid_taryoff)
  {
    SetTargetYOffset (f);
    return true;
  }
  return false;
}
bool celPcTrackingCamera::GetPropertyIndexed (int idx, float &f)
{
  if (idx == propid_taryoff)
  {
    f = targetyoffset;
    return true;
  }
  return false;
}

bool celPcTrackingCamera::PerformActionIndexed (int idx,
  iCelParameterBlock* params,
  celData& ret)
{
  switch (idx)
  {
    case action_reset:
      ResetCamera ();
      return true;
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
float celPcTrackingCamera::GetFollowMinimumSpringFactor () const
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

void celPcTrackingCamera::Accelerator::Accelerate (float direction, float elapsedsecs)
{
  // calculate current acceleration... do we go left... right? nothing?
  float cacc;
  // no direction but have some speed, then slowdown in opposite direction
  if (fabs (direction) <= 0.0000001 && fabs (speed) > EPSILON)
    cacc = (speed < 0) ? accel : -accel;
  else  // normal
    cacc = direction * accel;

  // this is to actually stop if we're slowing down.
  if (fabs (direction) <= 0.0000001 &&
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
  pan.Accelerate (pandir, elapsedsecs);
  float angle = pan.speed * elapsedsecs;
  // minor optimisation
  if (fabs (angle) > EPSILON)
  {
    // x' = x cos a - y sin a
    // y' = x sin a + y cos a
    csVector3 pc (pos - playpos);
    pos.x = pc.x * cos (angle) - pc.z * sin (angle) + playpos.x;
    pos.z = pc.x * sin (angle) + pc.z * cos (angle) + playpos.z;
  }

  tilt.Accelerate (tiltdir, elapsedsecs);
  posoff.angle -= tilt.speed * elapsedsecs;
  // we limit the angles between epsilon and PI/2 - epsilon
  // to stop the evilness of rotating to the front of the character!!
  if (posoff.angle < 0.1)
    posoff.angle = 0.1f;
  else if (posoff.angle > PI / 2 - 0.1f)
    posoff.angle = PI / 2 - 0.1f;
}

void celPcTrackingCamera::FindCorrectedTransform (float elapsedsecs)
{
  // get this value before it's lost
  float old_reallen = (corrtar - corrpos).Norm ();
  // do collision test
  const csTraceBeamResult beam = csColliderHelper::TraceBeam (cdsys, parent->GetSectors ()->Get (0),
    pos, tar, true);
  if (beam.sqdistance > 0)
  {
    const csVector3 lookat (tar - pos);
    float lookat_len = lookat.Norm ();
    const csVector3 dir (lookat / lookat_len);
    float beam_distance = sqrt (beam.sqdistance);
    // 0.1f is our epsilon value
    // we add a correction to the beam when it collides with the wall by 10%
    // but to avoid a jump when camera first hits the wall and is moved 10% down the beam
    // we fade in this 10% during the first 10% of the beam.
    float corr = 0.1f, linear_falloff = beam_distance / lookat_len;
    if (linear_falloff < 0.1f)  // we are in the first 0.1 of the beam
      corr *= linear_falloff / 0.1f;
    // so we offset a proportional amount down the beam towards the player so as not to be
    // inside the wall.
    corrpos = beam.closest_isect + corr * (lookat_len - beam_distance) * dir;
    // so camera can start a slow zoom out as soon as the lookat_len starts increasing
    was_corrected = true;
  }
  else
    corrpos = pos;
  // target unchanged
  corrtar = tar;

  if (was_corrected)
  {
    // reverse lookat vector
    const csVector3 clookat (corrtar - corrpos);
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
      corrpos = corrtar - (clookat / corrlen) * (i * corrlen + (1.0f - i) * old_reallen);
    }
    // so switch this off if we finished the interpolating at last
    else
      was_corrected = false;
  }
}
void celPcTrackingCamera::TransitionTarget ()
{
  in_tartransition = true;
  currtartrans = 0.0f;
  prevtar = corrtar;
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
  float finp = 1 + posoff.angle / PI, fexp = finp;
  for (size_t exiter = 0; exiter < 4; exiter++)
    fexp *= finp;
  // we multiply out the fexp value to make function a lot faster so
  // it's not so huge difference between pi/2 and pi
  float dxf = posoff.dist * (1 - (1 / fexp));
  float
    posoffset_y = dxf * sin (posoff.angle),
    posoffset_z = dxf * cos (posoff.angle);

  const csVector3 playpos (parent->GetFullPosition ());
  if (targetstate == TARGET_BASE)
  {
    // get flat 2D vector (zero out y) of camera to the player
    csVector3 camplay (playpos - pos);
    camplay.y = 0.0f;
    float dist = camplay.Norm ();
    camplay /= dist;
    // Now get a 2D vector of the camera's direction
    csVector3 camdir (tar - pos);
    camdir.y = 0.0f;
    camdir.Normalize ();

    // plug it into our simplified equation to get the movement needed
    //   cos (x) = camdir . camplay
    //   move = dist * cos (x)
    // in case you don't realise, it's the distance along camdir until
    // there's a perpendicular bisecting camera -> player...
    // ... this is so the camera only follows player in and out of the screen
    float move = dist * camdir * camplay - posoffset_z;

    pos += SpringForce (move) * move * camdir;
    // lock y axis to fixed distance above player
    pos.y = playpos.y + posoffset_y;

    PanAroundPlayer (playpos, elapsedsecs);

    // setup the target
    tar = playpos;
    tar.y += targetyoffset;
  }
  else if (targetstate == TARGET_NONE)
  {
    PanAroundPlayer (playpos, elapsedsecs);

    pos = playpos - cam_dir * posoffset_z;
    pos.y = playpos.y + posoffset_y;

    tar = playpos;
    tar.y += targetyoffset;

    // get flat 2D vector (zero out y) of camera to the player
    /*csVector3 camplay (playpos - pos);
    camplay.y = 0.0f;
    float dist = camplay.Norm ();
    camplay.Normalize ();
    // Now get a 2D vector of the camera's direction
    csVector3 camdir (tar - pos);
    camdir.y = 0.0f;
    camdir.Normalize ();

    // plug it into our simplified equation to get the movement needed
    //   cos (x) = camdir . camplay
    //   move = dist * cos (x)
    // in case you don't realise, it's the distance along camdir until
    // there's a perpendicular bisecting camera -> player...
    // ... this is so the camera only follows player in and out of the screen
    float move = dist * camdir * camplay - posoffset_z;

    pos += SpringForce (move) * move * camdir;
    // lock y axis to fixed distance above player
    pos.y = playpos.y + posoffset_y;

    PanAroundPlayer (playpos, elapsedsecs);

    // setup the target
    tar = (dist * camdir * camplay) * camdir + pos;
    tar.y = playpos.y + targetyoffset;

    // --------------
    camdir = pos - tar;
    camdir.y = 0.0;
    CS::Swap(camdir.x, camdir.z);
    //camdir.Normalize ();
    //tar += camdir;

    csVector3 offset (pos - tar);*/
    //tar = playpos + csVector3 (0, targetyoffset, 0);

    //pos -= (offset >> csVector3 (1, 0, 0));
    //pos = playpos + csVector3 (0, targetyoffset, 0) + offset;

    //tar += offset;
    //pos += offset.Norm () * camdir;
    /*csRef<iEngine> engine = csQueryRegistry<iEngine> (object_reg);
    iMovable *corn = engine->FindMeshObject ("Corner0")->GetMovable ();
    iMovable *corn1 = engine->FindMeshObject ("Corner1")->GetMovable ();
    corn->SetPosition (playpos + csVector3 (0, targetyoffset, 0));
    corn1->SetPosition (tar);*/
  }
  else if (targetstate == TARGET_OBJ)
  {
    const csVector3 tarpos (tracktarget->GetFullPosition ());
    // project from player to the target to get the vector
    // so we can project backwards for the camera
    csVector3 camdir (pos - tarpos);
    camdir.Normalize ();
    pos = playpos + camdir * posoffset_z;
    // lock y axis to fixed distance above player
    pos.y = playpos.y + posoffset_y;
    // setup the target
    tar = tarpos;
    tar.y += targetyoffset;
  }

  FindCorrectedTransform (elapsedsecs);

  const csReversibleTransform &trans = parent->GetFullTransform ();
  up  = trans.This2OtherRelative (csVector3 (0.0f, 1.0f, 0.0f));

  // do the target interpolation thing
  if (in_tartransition)
  {
    // update counter
    currtartrans += 1000.0f * elapsedsecs / float (tarintime);
    if (currtartrans > 1.0f)
    {
      // switch transition off
      in_tartransition = false;
      // and clip this for usage below
      currtartrans = 1.0f;
    }
    // a classic
    corrtar = currtartrans * (corrtar - prevtar) + prevtar;
  }
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
  pos = basepos + offset;
  // setup the target
  tar = basepos;
  tar.y += targetyoffset;
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
  TransitionTarget ();
  // great success!
  return true;
}
void celPcTrackingCamera::SetTargetState (TargetState ntstate)
{
  // save the camera direction since it's locked now
  if (ntstate != TARGET_BASE)
  {
    cam_dir = tar - pos;
    cam_dir.Normalize();
  }
  //if (targetstate != TARGET_OBJ && ntstate != TARGET_NONE)
    //TransitionTarget ();
  targetstate = ntstate;
}
celPcTrackingCamera::TargetState celPcTrackingCamera::GetTargetState ()
{
  return targetstate;
}
void celPcTrackingCamera::SetTargetYOffset (float yoff)
{
  targetyoffset = yoff;
  TransitionTarget ();
}
float celPcTrackingCamera::GetTargetYOffset () const
{
  return targetyoffset;
}
void celPcTrackingCamera::SetTargetInterpolationTime (csTicks t)
{
  tarintime = t;
}
csTicks celPcTrackingCamera::GetTargetInterpolationTime () const
{
  return tarintime;
}

void celPcTrackingCamera::SetPanDirection (float pdir)
{
  pandir = pdir;
}
float celPcTrackingCamera::GetPanDirection () const
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

void celPcTrackingCamera::SetTiltDirection (float tdir)
{
  tiltdir = tdir;
}
float celPcTrackingCamera::GetTiltDirection () const
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

const csVector3 &celPcTrackingCamera::GetPosition ()
{
  return corrpos;
}
const csVector3 &celPcTrackingCamera::GetTarget ()
{
  return corrtar;
}
const csVector3 &celPcTrackingCamera::GetUp ()
{
  return up;
}
