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

#include <math.h>
#include "cssysdef.h"
#include "propclass/mesh.h"
#include "propclass/solid.h"
#include "propclass/zone.h"
#include "plugins/propclass/engine/engfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "physicallayer/databhlp.h"
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
#include "imap/parser.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/camera.h"
#include "iengine/region.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/csview.h"
#include "ivaria/view.h"
#include "ivideo/graph3d.h"
#include "qsqrt.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Camera, "pccamera")
CEL_IMPLEMENT_FACTORY (Region, "pcregion")

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistance",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//---------------------------------------------------------------------------

static float FixAngle (float angle)
{
  while (angle >= 3.14159f)
    angle -= 2*3.14159f;
  while (angle < -3.14169f)
    angle += 2*3.14159f;
  return angle;
}

static csVector3 CalculateEyePos (const csVector3& actor_pos,
	float actor_yrot, const csVector3& offset)
{
  // Calculate the eye position of the actor according to his eye offset.
  csVector3 eye_pos = actor_pos + csVector3 (
    	sin (actor_yrot) * offset.z,
	offset.y,
	cos (actor_yrot) * offset.z);
  return eye_pos;
}

static float CalculateNewYaw (const csVector3& dir)
{
  if (dir.x == 0.0f)
    return atan2 (-0.00001f, -dir.z);
  else
    return atan2 (-dir.x, -dir.z);
}

static csVector3 CalcElasticPos (const csVector3& currPos,
	const csVector3& idealPos, const csVector3& deltaIdealPos,
	float deltaTime, float springCoef, float dampCoef, float springLength)
{
  csVector3 deltaPos;
  csVector3 vel;
  float force;

  deltaPos = currPos - idealPos;
  if (deltaPos.SquaredNorm() == 0)
    return currPos;

  vel = deltaIdealPos * deltaTime;
  force = springCoef * (springLength - deltaPos.Norm())
  	+ dampCoef * (deltaPos * vel) / deltaPos.Norm();

  float dist = deltaPos.Norm();
  if (-dist < force * deltaTime)
    deltaPos *= force * deltaTime / dist;
  else
    deltaPos *= -1;

  return currPos + deltaPos;
}

//---------------------------------------------------------------------------


void CAFirstPerson::DoCameraCalculations (const csTicks elapsedTicks,
	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot)
{
  parent->SetPosition (actor_eye);
  parent->SetYaw (actor_yrot);
  parent->CalculateFromYawPitchRoll ();
}

void CAFirstPerson::SetupMode ()
{
}

void CAThirdPerson::DoCameraCalculations (const csTicks elapsedTicks,
	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot)
{
  parent->SetTarget (actor_eye);
  parent->SetYaw (actor_yrot);
  parent->CalculatePositionFromYawPitchRoll ();
}

void CAThirdPerson::SetupMode ()
{
  // Try to get position and sector from either linmove or mesh if
  // linmove is not used.
  csVector3 actor_pos;
  float actor_yrot;
  iSector* actor_sector;
  parent->GetLastPosition (actor_pos, actor_yrot, actor_sector);
  parent->SetPosition (CalculateEyePos (actor_pos, actor_yrot,
    parent->thirdPersonPositionOffset));
  parent->SetYaw (actor_yrot);
}

void CAM64ThirdPerson::DoCameraCalculations (const csTicks elapsedTicks,
	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot)
{
  parent->SetTarget (actor_eye);
  parent->SetDistance ((parent->GetTarget ()-parent->GetPosition ()).Norm());
  parent->EnsureCameraDistance ();
  parent->CalculatePositionFromYawPitchRoll ();
}

void CAM64ThirdPerson::SetupMode ()
{
  parent->SetYaw (parent->GetYaw (parent->prev_cammode));
  parent->SetDistance (parent->GetMaxDistance ());
  parent->EnsureCameraDistance ();
  parent->CalculatePositionFromYawPitchRoll ();
}

void CALaraThirdPerson::DoCameraCalculations (const csTicks elapsedTicks,
	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot)
{
  float velNormSquared;
  if (parent->GetLinMove ())
  {
    csVector3 velocity;
    parent->GetLinMove ()->GetVelocity (velocity);
    velNormSquared = velocity.SquaredNorm ();
  }
  else
  {
    velNormSquared = 0;
  }
  parent->SetTarget (actor_eye);
  if (velNormSquared > 0.01f)
  {
    // When the player is running (only), a new position is interpolated.
    // So with this camera mode, two springs are modelled, but the second
    // spring is ignored unless the actor is moving.

    // Calculate where the camera would be if there weren't a swing effect.
    csVector3 newIdealPos = CalculateEyePos (actor_pos, actor_yrot,
		parent->thirdPersonPositionOffset);

    // Interpolate to the new calculated position.
    parent->SetPosition (CalcElasticPos (parent->GetPosition (), newIdealPos,
		0, (float)elapsedTicks/1000.0f, parent->GetSwingCoef (),
		0.0f, parent->GetSpringLength ()));
    parent->SetYaw (CalculateNewYaw (parent->GetTarget ()
    	-parent->GetPosition ()));
  }

  // Ensure valid distance.
  parent->SetDistance ((parent->GetTarget ()-parent->GetPosition ()).Norm ());
  parent->EnsureCameraDistance ();

  // This allows pitch to work
  // Note that this doesn't really use the yaw calculation,
  // because whenever the position is modified (above), a new yaw that
  // represents the (position - target) vector is calculated. This ensures
  // that this function won't change the yaw at all and only handle
  // the pitch.
  parent->CalculatePositionFromYawPitchRoll ();
}

void CALaraThirdPerson::SetupMode ()
{
  parent->SetYaw (parent->GetYaw (parent->prev_cammode));
  parent->SetDistance (parent->GetMaxDistance ());
  parent->EnsureCameraDistance ();
  parent->CalculatePositionFromYawPitchRoll ();
}

void CAFreeLook::DoCameraCalculations (const csTicks elapsedTicks,
	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot)
{
  parent->SetTarget (actor_eye);
  parent->EnsureCameraDistance ();
  parent->CalculatePositionFromYawPitchRoll ();
}

void CAFreeLook::SetupMode ()
{
  parent->SetYaw (parent->GetYaw (parent->prev_cammode));
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcCamera)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCamera)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcCamera::PcCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcCamera)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcCamera::celPcCamera (iObjectRegistry* object_reg)
  : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCamera);

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  view = csPtr<iView> (new csView (engine, g3d));

  modeset_needed = false;
  cammode = iPcCamera::freelook;
  camalgo = 0;

  use_cd = false;
  rect_set = false;
  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  mouse = CS_QUERY_REGISTRY (object_reg, iMouseDriver);
  CS_ASSERT (kbd != 0);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != 0);

  clear_zbuf = false;
  clear_screen = false;

  // Most of these are default values that will be overwritten
  firstPersonPositionOffset = csVector3 (0, 1, 0);
  thirdPersonPositionOffset = csVector3 (0, 1, 3);

  camData[iPcCamera::firstperson].springCoef = 10.0f;
  camData[iPcCamera::firstperson].InertialDampeningCoef = 0.1f;
  camData[iPcCamera::firstperson].springLength = 0.01f;

  camData[iPcCamera::thirdperson].springCoef = 3.5f;
  camData[iPcCamera::thirdperson].InertialDampeningCoef = 0.25f;
  camData[iPcCamera::thirdperson].springLength = 0.01f;

  camData[iPcCamera::m64_thirdperson].springCoef = 3.5f;
  camData[iPcCamera::m64_thirdperson].InertialDampeningCoef = 0.25f;
  camData[iPcCamera::m64_thirdperson].springLength = 0.01f;
  camData[iPcCamera::m64_thirdperson].minDistance = 2.0f;
  camData[iPcCamera::m64_thirdperson].maxDistance = 6.0f;
  camData[iPcCamera::m64_thirdperson].turnSpeed = 1.0f;

  camData[iPcCamera::lara_thirdperson].springCoef = 3.5f;
  camData[iPcCamera::lara_thirdperson].InertialDampeningCoef = 0.25f;
  camData[iPcCamera::lara_thirdperson].springLength = 0.01f;
  camData[iPcCamera::lara_thirdperson].minDistance = 2.0f;
  camData[iPcCamera::lara_thirdperson].maxDistance = 6.0f;
  camData[iPcCamera::lara_thirdperson].turnSpeed = 1.0f;
  camData[iPcCamera::lara_thirdperson].swingCoef = 0.7f;

  camData[iPcCamera::freelook].springCoef = 3.5f;
  camData[iPcCamera::freelook].InertialDampeningCoef = 0.25f;
  camData[iPcCamera::freelook].springLength = 0.01f;
  camData[iPcCamera::freelook].minDistance = 2.0f;
  camData[iPcCamera::freelook].maxDistance = 16.0f;

  camData[iPcCamera::transition].springCoef = 3.5f;
  camData[iPcCamera::transition].InertialDampeningCoef = 0.25f;
  camData[iPcCamera::transition].springLength = 0.01f;

  int i;
  csVector3 pos (0);
  for (i=0 ; i < iPcCamera::CameraMode_count ; i++)
  {
    SetPosition (pos, i);
    SetTarget (pos, i);
    SetUp (csVector3 (0, 1, 0), i);
    SetDistance (5.0f, i);
    SetPitch (0.0f, i);
    SetDefaultPitch (0.0f, i);
    SetYaw (0.0f, i);
  }

  transitionThresholdSquared = 1.0f;
  cameraHasBeenPositioned = false;

  DisableDistanceClipping ();

  useCameraCD = true;

  SetMode (iPcCamera::firstperson);

  pl->CallbackPCEveryFrame (this, cscmdProcess);
}

celPcCamera::~celPcCamera ()
{
}

void celPcCamera::DisableDistanceClipping ()
{
  fp.use_farplane = false;
  if (view && view->GetCamera ())
    view->GetCamera ()->SetFarPlane (0);
}

void celPcCamera::EnableFixedDistanceClipping (float dist)
{
  fp.use_farplane = true;
  fp.fixed_distance = dist;
  SetDistanceClipping (dist);
}

void celPcCamera::EnableAdaptiveDistanceClipping (float min_fps,
	float max_fps, float min_dist)
{
  fp.use_farplane = true;
  fp.fixed_distance = -1.0f;
  fp.min_fps = min_fps;
  fp.max_fps = max_fps;
  fp.min_dist = min_dist;
  fp.fps_valid = false;
  fp.smooth_fps = 30.0f;
  fp.accumulated_elapsed = 0;
  fp.current_distance = -1.0f;
}

void celPcCamera::SetDistanceClipping (float dist)
{
  csVector3 v1 (0, 0, dist), v2 (0, 1, dist), v3 (1, 0, dist);
  csPlane3 p (v1, v2, v3);
  view->GetCamera ()->SetFarPlane (&p);
  fp.current_distance = dist;
}

// 1. qsqrt vs sqrt
// 2. don't use static fields, may conflict with other camera instances
// 3. use virtual clock and not csGetTicks(), movie recorder, speed, elapsed
// 4. GetDistanceClipping() is expensive, better remember current distance.
// 5. Adapts very slow to lower clipping plane.

void celPcCamera::AdaptDistanceClipping (csTicks elapsed_time)
{
  if (!fp.use_farplane) return;
  if (!fp.fixed_distance > 0.0f) return;

  float curr_fps;		// FPS calculated from the last frame.

  // When we are called for the first time, we just initialize some variables
  // and exit.
  if (!fp.fps_valid)
  {
    fp.fps_valid = true;
    return;
  }

  if (!elapsed_time)
    curr_fps = 1000.0;
  else
    curr_fps = 1000.0 / float (elapsed_time);
  fp.smooth_fps = 0.5*curr_fps + 0.5*fp.smooth_fps;

  fp.accumulated_elapsed += elapsed_time;
  if (fp.accumulated_elapsed > 1000)
  {
    fp.accumulated_elapsed = 0;
    float change;

    float curr_dist = fp.current_distance;
    if (curr_dist < 0.0f)
      curr_dist = FP_INIT_DIST;

    if (fp.smooth_fps < fp.min_fps)
    {
      change = 2.0 * (fp.min_fps - fp.smooth_fps);
      curr_dist -= change;
      if (curr_dist < fp.min_dist) curr_dist = fp.min_dist;
      SetDistanceClipping (curr_dist);
    }
    else if (fp.smooth_fps > fp.max_fps)
    {
      change = 2.0 * (fp.smooth_fps - fp.max_fps);
      curr_dist += change;
      if (curr_dist > FP_MAX_DIST) curr_dist = FP_MAX_DIST;
      SetDistanceClipping (curr_dist);
    }
  }
}

void celPcCamera::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

void celPcCamera::GetLastPosition (csVector3& actor_pos,
	float& actor_yrot, iSector*& actor_sector)
{
  // Try to get position and sector from either linmove or mesh if
  // linmove is not used.
  FindSiblingPropertyClasses ();
  if (pclinmove)
  {
    pclinmove->GetLastPosition (actor_pos, actor_yrot, actor_sector);
    actor_yrot = FixAngle (actor_yrot);
  }
  else if (pcmesh)
  {
    iMovable* movable = pcmesh->GetMesh()->GetMovable();
    actor_pos = movable->GetPosition ();
    actor_sector = movable->GetSectors ()->Get (0);
    actor_yrot = 0;
  }
  else
  {
    actor_pos.Set (0, 0, 0);
    actor_yrot = 0;
    actor_sector = 0;
  }
}

void celPcCamera::CalculateFromYawPitchRoll (int mode)
{
  float cosYaw, sinYaw;
  float cosPit, sinPit;
  float cosRol, sinRol;

  cosYaw = (float)cos(GetYaw(mode));    sinYaw = (float)sin(GetYaw(mode));
  cosPit = (float)cos(GetPitch(mode));  sinPit = (float)sin(GetPitch(mode));
  // At this point, our camera doesn't support Roll
  cosRol = 1.0f;                        sinRol = 0.0f;

  if (cosPit == 0.0f) cosPit = 0.001f;

  SetTarget (GetPosition (mode) + (GetDistance (mode)
  	* csVector3 (-sinYaw * cosPit, sinPit, cosPit * -cosYaw)));
}

void celPcCamera::CalculatePositionFromYawPitchRoll (int mode)
{
  float cosYaw, sinYaw;
  float cosPit, sinPit;
  float cosRol, sinRol;

  cosYaw = (float)cos(GetYaw(mode));    sinYaw = (float)sin(GetYaw(mode));
  cosPit = (float)cos(GetPitch(mode));  sinPit = (float)sin(GetPitch(mode));
  // At this point, our camera doesn't support Roll
  cosRol = 1.0f;                        sinRol = 0.0f;

  if (cosPit == 0.0f) cosPit = 0.001f;

  SetPosition (GetTarget (mode) - (GetDistance (mode)
  	* csVector3 (-sinYaw * cosPit, sinPit,
	// we have to reverse the vertical thing
		cosPit * -cosYaw)));
}

void celPcCamera::EnsureCameraDistance (int mode)
{
  if (GetDistance (mode) > GetMaxDistance (mode))
    SetDistance (GetMaxDistance (mode), mode);
  else if (GetDistance (mode) < GetMinDistance (mode))
    SetDistance (GetMinDistance (mode), mode);
}

void celPcCamera::ResetActualCameraData ()
{
  SetPosition (GetPosition (), iPcCamera::actual_data);
  SetTarget (GetTarget (), iPcCamera::actual_data);
  SetUp (GetUp (), iPcCamera::actual_data);
}

void celPcCamera::DoCameraTransition ()
{
  if (inTransitionPhase)
  {
    if ((GetPosition () - GetPosition (iPcCamera::actual_data)).SquaredNorm()
    	< transitionThresholdSquared)
    {
      inTransitionPhase = false;
    }
  }
}

csVector3 celPcCamera::CalcCollisionPos (const csVector3& pseudoTarget,
	const csVector3& pseudoPosition, iSector* sector) const
{
  if (!useCameraCD)
    return pseudoPosition; // no collision detection

  if (!pcmesh)
    return pseudoPosition; // no mesh

  pcmesh->GetMesh()->GetFlags().Set (CS_ENTITY_NOHITBEAM);
  switch (GetMode ())
  {
    case iPcCamera::thirdperson:
    case iPcCamera::m64_thirdperson:
    case iPcCamera::lara_thirdperson:
    case iPcCamera::freelook:
    {
      csVector3 isect;
      int sel;
      csVector3 modifiedTarget = pseudoTarget;

      iMeshWrapper* mesh = sector->HitBeamPortals (modifiedTarget,
      	pseudoPosition, isect, &sel);
      if (mesh)
      {
        pcmesh->GetMesh()->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
        return isect;
      }
      break;
    }
    default:
      break;
  }
  pcmesh->GetMesh()->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
  return pseudoPosition;
}

void celPcCamera::DoElasticPhysics (bool isElastic,
	const csTicks elapsedTicks,
	const CameraData& deltaIdeal,
	iSector* sector)
{
  // If the camera mode is elastic then progress gradually to the ideal pos
  if (isElastic)
  {
    float cameraSpringCoef, cameraInertialDampeningCoef, cameraSpringLength;
    if (!inTransitionPhase)
    {
      cameraSpringCoef = GetSpringCoef();
      cameraInertialDampeningCoef = GetDampeningCoef();
      cameraSpringLength = GetSpringLength();
    }
    else
    {
      cameraSpringCoef = GetSpringCoef (iPcCamera::transition);
      cameraInertialDampeningCoef = GetDampeningCoef(iPcCamera::transition);
      cameraSpringLength = GetSpringLength(iPcCamera::transition);
    }

    csVector3 newPos, newTar, newUp;

    newPos = CalcElasticPos (GetPosition (iPcCamera::actual_data),
    	CalcCollisionPos (GetTarget(), GetPosition(), sector),
	deltaIdeal.worldPos,
	(float)elapsedTicks/1000.0f, cameraSpringCoef,
	cameraInertialDampeningCoef, cameraSpringLength);
    SetPosition (newPos, iPcCamera::actual_data);

    newTar = CalcElasticPos (GetTarget (iPcCamera::actual_data), GetTarget (),
    	deltaIdeal.worldTar, (float)elapsedTicks/1000.0f, cameraSpringCoef,
	cameraInertialDampeningCoef, cameraSpringLength);
    SetTarget(newTar, iPcCamera::actual_data);

    newUp = CalcElasticPos (GetUp (iPcCamera::actual_data), GetUp (),
    	deltaIdeal.worldUp, (float)elapsedTicks/1000.0f, cameraSpringCoef,
	cameraInertialDampeningCoef, cameraSpringLength);
    SetUp (newUp, iPcCamera::actual_data);
  }
  else
  {
    // Camera isn't elastic, so no interpolation is done between ideal and
    // actual.
    SetPosition (CalcCollisionPos (GetTarget (), GetPosition (), sector),
    	iPcCamera::actual_data);
    SetTarget (GetTarget (), iPcCamera::actual_data);
    SetUp (GetUp (), iPcCamera::actual_data);
  }
}

void celPcCamera::TickEveryFrame ()
{
  CheckModeChange ();

  // First get elapsed time from the virtual clock.
  csTicks elapsed_time = vc->GetElapsedTicks ();
  float elapsed_sec = elapsed_time / 1000.0f;

  AdaptDistanceClipping (elapsed_time);

  // Velocity calculations.
  MovePitch (pitchVelocity * elapsed_sec);
  MoveYaw (yawVelocity * elapsed_sec);

  // Try to get position and sector from either linmove or mesh if
  // linmove is not used.
  csVector3 actor_pos;
  float actor_yrot;
  iSector* actor_sector;
  GetLastPosition (actor_pos, actor_yrot, actor_sector);
  if (!actor_sector)
  {
    // We have no actor, so just display with current camera settings.
    if (g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS
    	  | (clear_zbuf ? CSDRAW_CLEARZBUFFER : 0)
	  | (clear_screen ? CSDRAW_CLEARSCREEN : 0)))
      view->Draw ();
    return;	// Can't do anything.
  }

  // Store previous frame ideal camera data. It will be compared against
  // current frame later to become the delta part.
  CameraData deltaIdeal;
  deltaIdeal.worldPos = GetPosition ();
  deltaIdeal.worldTar = GetTarget ();
  deltaIdeal.worldUp = GetUp ();

  // Calculate the eye position of the actor according to his eye offset.
  csVector3 actor_eye = CalculateEyePos (actor_pos, actor_yrot,
    	firstPersonPositionOffset);

  // Calculate the camera data without updating it for real.
  camalgo->DoCameraCalculations (
    	elapsed_time, actor_pos, actor_eye, actor_yrot);

  if (!cameraHasBeenPositioned)
  {
    // Start off in an appropriate place.
    cameraHasBeenPositioned = true;
    ResetActualCameraData ();
  }

  // Transition phase calculations.
  DoCameraTransition ();

  // This makes the deltaIdeal data true to it's delta wording by
  // subtracting the current ideal data.
  deltaIdeal.worldPos -= GetPosition ();
  deltaIdeal.worldTar -= GetTarget ();
  deltaIdeal.worldUp -= GetUp ();

  // Interpolate between ideal and actual camera data.
  DoElasticPhysics (true, elapsed_time, deltaIdeal, actor_sector);

  if (pcmesh)
  {
    if (cammode != iPcCamera::firstperson || inTransitionPhase)
    {
      if ((GetPosition (iPcCamera::actual_data)
		- GetTarget (iPcCamera::actual_data)).SquaredNorm () > 0.3f)
        pcmesh->GetMesh ()->GetFlags ().Reset (CS_ENTITY_INVISIBLE);
    }
    else
    {
      pcmesh->GetMesh ()->GetFlags ().Set (CS_ENTITY_INVISIBLE);
    }
  }


  iCamera* c = view->GetCamera ();

  // First set the camera back on where the sector is.
  if (c->GetSector () != actor_sector)
    c->SetSector (actor_sector);
  //c->GetTransform ().SetOrigin (actor_pos+c->GetTransform ().
		    //This2OtherRelative (csVector3 (0, 0, .1)));
  c->GetTransform ().SetOrigin (actor_pos+csVector3 (0, 0.1f, 0));
  c->OnlyPortals (true);

  // Now move it to the new position.
  c->MoveWorld (GetPosition (iPcCamera::actual_data) - actor_pos);
  c->GetTransform ().LookAt (GetTarget (iPcCamera::actual_data) -
    	GetPosition (iPcCamera::actual_data), GetUp (iPcCamera::actual_data));

  // Tell 3D driver we're going to display 3D things.
  if (g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS
    	| (clear_zbuf ? CSDRAW_CLEARZBUFFER : 0)
	| (clear_screen ? CSDRAW_CLEARSCREEN : 0)))
    view->Draw ();
}

void celPcCamera::SetPosition (const csVector3& pos, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].worldPos = pos;
}

const csVector3& celPcCamera::GetPosition (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].worldPos;
}

void celPcCamera::SetTarget (const csVector3& tar, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].worldTar = tar;
}

const csVector3& celPcCamera::GetTarget (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].worldTar;
}

void celPcCamera::SetUp (const csVector3& up, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].worldUp = up;
  camData[mode].worldUp.Normalize();
}

const csVector3& celPcCamera::GetUp (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].worldUp;
}

void celPcCamera::SetPitch (float pitch, int mode)
{
  if (mode < 0) mode = cammode;

  if (pitch > 3.1415f/2.1f)
    pitch = 3.1415f/2.1f;
  if (pitch < -3.1415f/2.1f)
    pitch = -3.1415f/2.1f;

  camData[mode].pitch = pitch;
}

void celPcCamera::MovePitch (float deltaPitch, int mode)
{
  if (mode < 0) mode = cammode;
  SetPitch (GetPitch(mode) + deltaPitch, mode);
}

float celPcCamera::GetPitch (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].pitch;
}

void celPcCamera::SetPitchVelocity (float pitchVel)
{
  pitchVelocity = pitchVel;
}

float celPcCamera::GetPitchVelocity () const
{
  return pitchVelocity;
}

void celPcCamera::SetYaw (float yaw, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].yaw = FixAngle (yaw);
}

void celPcCamera::MoveYaw (float deltaYaw, int mode)
{
  if (mode < 0) mode = cammode;
  SetYaw (GetYaw (mode) + deltaYaw, mode);
}

float celPcCamera::GetYaw (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].yaw;
}

void celPcCamera::SetYawVelocity (float yawVel)
{
  yawVelocity = yawVel;
}

float celPcCamera::GetYawVelocity () const
{
  return yawVelocity;
}

void celPcCamera::SetDistance (float distance, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].distance = distance;
}

void celPcCamera::MoveDistance (float deltaDistance, int mode)
{
  SetDistance (GetDistance (mode) + deltaDistance, mode);
}

float celPcCamera::GetDistance (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].distance;
}

csVector3 celPcCamera::GetForwardVector (int mode) const
{
  if (mode < 0) mode = cammode;
  csVector3 dir = GetTarget (mode) - GetPosition (mode);
  dir.Normalize();
  return dir;
}

csVector3 celPcCamera::GetRightVector (int mode) const
{
  if (mode < 0) mode = cammode;
  csVector3 dir = GetTarget (mode) - GetPosition(mode);
  csVector3 right = GetUp (mode) % dir;
  right.Normalize();
  return right;
}

float celPcCamera::GetMinDistance (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].minDistance;
}

void celPcCamera::SetMinDistance (float dist, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].minDistance = dist;
}

float celPcCamera::GetMaxDistance (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].maxDistance;
}

void celPcCamera::SetMaxDistance (float dist, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].maxDistance = dist;
}

float celPcCamera::GetTurnSpeed (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].turnSpeed;
}

void celPcCamera::SetTurnSpeed (float speed, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].turnSpeed = speed;
}

float celPcCamera::GetSpringCoef (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].springCoef;
}

void celPcCamera::SetSpringCoef (float coef, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].springCoef = coef;
}

float celPcCamera::GetDampeningCoef (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].InertialDampeningCoef;
}

void celPcCamera::SetDampeningCoef (float coef, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].InertialDampeningCoef = coef;
}

float celPcCamera::GetSpringLength (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].springLength;
}

void celPcCamera::SetSpringLength (float length, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].springLength = length;
}

bool celPcCamera::CheckCameraCD () const
{
  return useCameraCD;
}

void celPcCamera::SetCameraCD (bool useCD)
{
  useCameraCD = useCD;
}

float celPcCamera::GetTransitionThreshold () const
{
  return sqrt (transitionThresholdSquared);
}

void celPcCamera::SetTransitionThreshold (float threshold)
{
  transitionThresholdSquared = threshold*threshold;
}

float celPcCamera::GetDefaultPitch (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].defaultPitch;
}

void celPcCamera::SetDefaultPitch (float pitch, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].defaultPitch = FixAngle (pitch);
}

float celPcCamera::GetDefaultYaw (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].defaultYaw;
}

void celPcCamera::SetDefaultYaw (float yaw, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].defaultYaw = FixAngle (yaw);
}

float celPcCamera::GetSwingCoef (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].swingCoef;
}

void celPcCamera::SetSwingCoef (float swingCoef, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].swingCoef = swingCoef;
}


bool celPcCamera::SetZoneManager (iPcZoneManager* newzonemgr,
	bool point, const char* regionname, const char *name)
{
  region = 0;
  zonemgr = newzonemgr;

  if (point)
  {
    csRef<iPcCamera> camera = SCF_QUERY_INTERFACE (this, iPcCamera);

    if (zonemgr)
      zonemgr->PointCamera (camera, regionname, name);
    else
    {
      // camera->GetCamera ()->SetSector (0);
      camera->GetCamera ()->GetTransform ().SetOrigin (csVector3(0,0,0));
    }
  }

  return true;
}

bool celPcCamera::SetRegion (iPcRegion* newregion, bool point, const char *name)
{
  region = newregion;
  zonemgr = 0;

  if (point)
  {
    csRef<iPcCamera> camera = SCF_QUERY_INTERFACE (this, iPcCamera);

    if (region)
      region->PointCamera (camera, name);
    else
    {
      // camera->GetCamera ()->SetSector (0);
      camera->GetCamera ()->GetTransform ().SetOrigin (csVector3(0,0,0));
    }
  }

  return true;
}

void celPcCamera::CheckModeChange ()
{
  if (!modeset_needed) return;
  modeset_needed = false;
  camalgo->SetupMode ();
  // Enable transition phase.
  inTransitionPhase = true;
}

bool celPcCamera::SetMode (iPcCamera::CameraMode cammode, bool use_cd)
{
  // Reset camera velocities.
  SetYawVelocity (0.0f);
  SetPitchVelocity (0.0f);

  delete camalgo;
  switch (cammode)
  {
    case iPcCamera::freelook:
      camalgo = new CAFreeLook (this);
      break;
    case iPcCamera::firstperson:
      camalgo = new CAFirstPerson (this);
      break;
    case iPcCamera::thirdperson:
      camalgo = new CAThirdPerson (this);
      break;
    case iPcCamera::m64_thirdperson:
      camalgo = new CAM64ThirdPerson (this);
      break;
    case iPcCamera::lara_thirdperson:
      camalgo = new CALaraThirdPerson (this);
      break;
    default:
      camalgo = 0;
      break;
  }

  if (!modeset_needed) prev_cammode = cammode;
  celPcCamera::cammode = cammode;
  celPcCamera::use_cd = use_cd;
  view->GetCamera ()->OnlyPortals (!use_cd);
  modeset_needed = true;

  return true;
}

bool celPcCamera::SetModeName (const char* m, bool use_cd)
{
  iPcCamera::CameraMode mode;
  if (!strcmp (m, "freelook")) mode = iPcCamera::freelook;
  else if (!strcmp (m, "firstperson")) mode = iPcCamera::firstperson;
  else if (!strcmp (m, "thirdperson")) mode = iPcCamera::thirdperson;
  else if (!strcmp (m, "m64_thirdperson")) mode = iPcCamera::m64_thirdperson;
  else if (!strcmp (m, "lara_thirdperson")) mode = iPcCamera::lara_thirdperson;
  else if (!strcmp (m, "freelook")) mode = iPcCamera::freelook;
  else return false;
  return SetMode (mode, use_cd);
}

const char* celPcCamera::GetModeName () const
{
  return camalgo->GetName ();
}

iPcCamera::CameraMode celPcCamera::GetNextMode () const
{
  switch (cammode)
  {
    case iPcCamera::freelook: return iPcCamera::firstperson;
    case iPcCamera::firstperson: return iPcCamera::thirdperson;
    case iPcCamera::thirdperson: return iPcCamera::m64_thirdperson;
    case iPcCamera::m64_thirdperson: return iPcCamera::lara_thirdperson;
    case iPcCamera::lara_thirdperson: return iPcCamera::freelook;
    default:
      return cammode;
  }
}

void celPcCamera::SetRectangle (int x, int y, int w, int h)
{
  rect_x = x;
  rect_y = y;
  rect_w = w;
  rect_h = h;
  view->SetRectangle (x, y, w, h);
  rect_set = true;
}

#define CAMERA_SERIAL 3

csPtr<iCelDataBuffer> celPcCamera::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (CAMERA_SERIAL);
  databuf->SetDataCount (3+11+7+2);
  celDataBufHelper db(databuf);

  csRef<iCelPropertyClass> pc;
  if (region) pc = SCF_QUERY_INTERFACE (region, iCelPropertyClass);
  db.Set (pc);
  db.Set (view->GetCamera()->GetSector()->QueryObject()->GetName());
  const csTransform& tr = view->GetCamera ()->GetTransform ();
  db.Set (tr.GetO2TTranslation ());

  db.Set (tr.GetO2T ().m11);
  db.Set (tr.GetO2T ().m12);
  db.Set (tr.GetO2T ().m13);
  db.Set (tr.GetO2T ().m21);
  db.Set (tr.GetO2T ().m22);
  db.Set (tr.GetO2T ().m23);
  db.Set (tr.GetO2T ().m31);
  db.Set (tr.GetO2T ().m32);
  db.Set (tr.GetO2T ().m33);

  db.Set ((uint8)cammode);
  db.Set (use_cd);
  db.Set (rect_set);
  db.Set ((uint16)rect_x);
  db.Set ((uint16)rect_y);
  db.Set ((uint16)rect_w);
  db.Set ((uint16)rect_h);

  db.Set (clear_zbuf);
  db.Set (clear_screen);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcCamera::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != CAMERA_SERIAL)
  {
    Report (object_reg, "serialnr != CAMERA_SERIAL.  Cannot load.");
    return false;
  }
  if (databuf->GetDataCount () != 3+11+7+2)
  {
    Report (object_reg, "23 data elements required.  Cannot load.");
    return false;
  }
  celDataBufHelper db(databuf);

  csMatrix3 m_o2t;
  csVector3 v_o2t;

  iCelPropertyClass* pc;
  if (!db.Get(pc))
  {
    Report (object_reg,"Cannot load property class.");
    return false;
  }
  if (pc)
    region = SCF_QUERY_INTERFACE (pc, iPcRegion);
  if (region)
    SetRegion (region, false, 0);

  const char* sectname;
  db.Get (sectname);
  iSector* sector = region->FindSector (sectname);
  if (!sector)
  {
    Report (object_reg,"Illegal sector specified.  Cannot load.");
    return false;
  }
  db.Get (v_o2t);

  db.Get (m_o2t.m11);
  db.Get (m_o2t.m12);
  db.Get (m_o2t.m13);
  db.Get (m_o2t.m21);
  db.Get (m_o2t.m22);
  db.Get (m_o2t.m23);
  db.Get (m_o2t.m31);
  db.Get (m_o2t.m32);
  db.Get (m_o2t.m33);

  if (!db.AllOk ())
  {
    Report (object_reg,"transformation matrix badly specified.  Cannot load.");
    return false;
  }
  view->GetCamera ()->SetSector(sector);
  csOrthoTransform tr (m_o2t, v_o2t);
  view->GetCamera ()->SetTransform (tr);

  db.Get ((uint8&)cammode);
  db.Get (use_cd);
  db.Get (rect_set);
  db.Get ((uint16&)rect_x);
  db.Get ((uint16&)rect_y);
  db.Get ((uint16&)rect_w);
  db.Get ((uint16&)rect_h);

  db.Get (clear_zbuf);
  db.Get (clear_screen);

  if (!db.AllOk())
  {
    Report (object_reg,
    	"Camera transformation matrix badly specified.  Cannot load.");
    return false;
  }

  if (rect_set)
    view->SetRectangle (rect_x, rect_y, rect_w, rect_h);

  return true;
}

iCamera* celPcCamera::GetCamera () const
{
  return view->GetCamera ();
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcRegion)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcRegion)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcRegion::PcRegion)
  SCF_IMPLEMENTS_INTERFACE (iPcRegion)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcRegion::celPcRegion (iObjectRegistry* object_reg)
  : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcRegion);

  UpdateProperties (object_reg);
  propdata = new void* [propertycount];

  props = properties;
  propcount = &propertycount;

  propdata[propid_worlddir] = &worlddir;
  propdata[propid_worldfile] = &worldfile;
  propdata[propid_regionname] = &regionname;

  worlddir = 0;
  worldfile = 0;
  regionname = 0;
  loaded = false;
  empty_sector = true;
}

celPcRegion::~celPcRegion ()
{
  Unload ();
  delete[] worlddir;
  delete[] worldfile;
  delete[] regionname;
}

#define REGION_SERIAL 2

csPtr<iCelDataBuffer> celPcRegion::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (REGION_SERIAL);
  databuf->SetDataCount (5);
  celDataBufHelper db (databuf);

  db.Set (empty_sector);
  db.Set (worlddir);
  db.Set (worldfile);
  db.Set (regionname);
  db.Set (loaded);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcRegion::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != REGION_SERIAL)
  {
    Report (object_reg, "serialnr != REGION_SERIAL.  Cannot load.");
    return false;
  }
  if (databuf->GetDataCount () != 5)
  {
    Report (object_reg, "5 data elements required, not %d.  Cannot load.",
    	databuf->GetDataCount () );
    return false;
  }
  celDataBufHelper db (databuf);

  Unload ();
  delete[] worlddir; worlddir = 0;
  delete[] worldfile; worldfile = 0;
  delete[] regionname; regionname = 0;

  if (!db.Get (empty_sector))
  {
    Report (object_reg, "Empty sector not specified.  Cannot load.");
    return false;
  }
  const char* strp;
  if (!db.Get (strp))
  {
    Report (object_reg, "Worlddir not specified.  Cannot load.");
    return false;
  }
  worlddir = csStrNew (strp);
  if (!db.Get (strp))
  {
    Report (object_reg, "Worldfile not specified.  Cannot load.");
    return false;
  }
  worldfile = csStrNew (strp);
  if (!db.Get (strp))
  {
    Report (object_reg, "Regionname not specified.  Cannot load.");
    return false;
  }
  regionname = csStrNew (strp);
  bool load;
  if (!db.Get (load))
  {
    Report (object_reg, "load flag not specified.  Cannot load.");
    return false;
  }

  if (load && !Load ())
  {
    Report (object_reg,
    	"Could not load the specified map into the region.  Cannot load.");
    return false;
  }

  return true;
}

int celPcRegion::propertycount = 0;
Property* celPcRegion::properties = 0;

void celPcRegion::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY( object_reg, iCelPlLayer );
    CS_ASSERT( pl != 0 );

    propertycount = 4;
    properties = new Property[propertycount];

    properties[propid_worlddir].id = pl->FetchStringID (
    	"cel.property.worlddir");
    properties[propid_worlddir].datatype = CEL_DATA_STRING;
    properties[propid_worlddir].readonly = false;
    properties[propid_worlddir].desc = "Map VFS path.";

    properties[propid_worldfile].id = pl->FetchStringID (
    	"cel.property.worldfile");
    properties[propid_worldfile].datatype = CEL_DATA_STRING;
    properties[propid_worldfile].readonly = false;
    properties[propid_worldfile].desc = "Map VFS file name.";

    properties[propid_regionname].id = pl->FetchStringID (
    	"cel.property.regionname");
    properties[propid_regionname].datatype = CEL_DATA_STRING;
    properties[propid_regionname].readonly = false;
    properties[propid_regionname].desc = "Name of this region.";

    properties[propid_load].id = pl->FetchStringID (
    	"cel.action.Load");
    properties[propid_load].datatype = CEL_DATA_ACTION;
    properties[propid_load].readonly = true;
    properties[propid_load].desc = "Load the map.\nNo parameters";
  }
}

bool celPcRegion::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == properties[propid_load].id)
  {
    if ((empty_sector || worldfile) && regionname)
      Load ();
    else
    {
      printf ("World filename or region name not set!\n");
      return false;
    }
    return true;
  }
  return false;
}

void celPcRegion::CreateEmptySector (const char* name)
{
  empty_sector = true;
  delete[] worldfile;
  worldfile = csStrNew (name);
}

void celPcRegion::SetWorldFile (const char* vfsdir, const char* name)
{
  empty_sector = false;
  if (worlddir != vfsdir)
  {
    delete[] worlddir;
    worlddir = csStrNew (vfsdir);
  }
  if (worldfile != name)
  {
    delete[] worldfile;
    worldfile = csStrNew (name);
  }
}

void celPcRegion::SetRegionName (const char* name)
{
  if (regionname != name)
  {
    delete[] regionname;
    regionname = csStrNew (name);
  }
}

bool celPcRegion::Load ()
{
  if (loaded)
  {
    Report (object_reg,"Entity '%s' already loaded.", entity->GetName());
    return true;
  }
  if (!empty_sector && !worlddir)
  {
    Report (object_reg, "World dir not specified.");
    return false;
  }
  if (!worldfile)
  {
    Report (object_reg, "World file not specified.");
    return false;
  }
  if (!regionname)
  {
    Report (object_reg, "Region name not specified.");
    return false;
  }

  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != 0);
  iRegion* cur_region = engine->CreateRegion (regionname);
  cur_region->DeleteAll ();

  if (empty_sector)
  {
    iSector* sector = engine->CreateSector (worldfile);
    cur_region->Add (sector->QueryObject ());
    loaded = true;
    return true;
  }

  csRef<iLoader> loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  CS_ASSERT (loader != 0);
  csRef<iVFS> VFS = CS_QUERY_REGISTRY (object_reg, iVFS);
  CS_ASSERT (VFS != 0);
  VFS->ChDir (worlddir);
  // Load the level file which is called 'world'.
  if (!loader->LoadMapFile (worldfile, false, cur_region, true))
  {
    Report (object_reg, "Could not load map file '%s'.", worldfile);
    return false;
  }
  cur_region->Prepare ();
  loaded = true;
  printf ("LoadOK!\n");

  // Create entities for all meshes in this region unless there is already
  // an entity for them (an addon may have created them for example).
  {
  iCelPropertyClass* pc;
  csRef<iObjectIterator> iter = cur_region->QueryObject ()->GetIterator ();
  while (iter->HasNext ())
  {
    iObject* o = iter->Next ();
    iCelEntity* e = pl->FindAttachedEntity (o);
    if (e)
    {
      // There was already an entity attached. This entity is probably
      // created by an addon. We will also register this entity as
      // one that needs to be deleted when the region is unloaded.
      entities.Push (e);
    }
    else
    {
      csRef<iMeshWrapper> m = SCF_QUERY_INTERFACE (o, iMeshWrapper);
      if (m)
      {
        csRef<iCelEntity> ent = pl->CreateEntity ();
        ent->SetName ("");

        pc = pl->CreatePropertyClass (ent, "pcmesh");
        csRef<iPcMesh> pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);
        pcmesh->SetMesh (m);

        pc = pl->CreatePropertyClass (ent, "pcsolid");
        entities.Push (ent);
      }
    }
  }
  }

  return true;
}

void celPcRegion::Unload ()
{
  if (!loaded) return;
  loaded = false;
  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != 0);

  iRegion* cur_region = engine->CreateRegion (regionname);

  if (pl)
  {
    size_t i;
    for (i = 0 ; i < entities.Length () ; i++)
      if (entities[i])
      {
        pl->RemoveEntity (entities[i]);
      }
  }
  entities.DeleteAll ();

  cur_region->DeleteAll ();
  engine->GetRegions ()->Remove (cur_region);
}

iSector* celPcRegion::FindSector (const char* name)
{
  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  iSector* temp = engine->GetSectors()->FindByName (name);
  return temp;
}

iSector* celPcRegion::GetStartSector (const char* name)
{
  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != 0);
  if (empty_sector)
  {
    iRegion* reg = engine->GetRegions ()->FindByName (regionname);
    return engine->FindSector (worldfile, reg);
  }
  iSector* sector;
  if (engine->GetCameraPositions ()->GetCount () > 0)
  {
    iCameraPosition* campos =
    	name ? engine->GetCameraPositions ()->FindByName (name)
       : engine->GetCameraPositions ()->Get (0);
    sector = engine->GetSectors ()->FindByName (
    	campos ? campos->GetSector () : "room");
  }
  else
  {
    sector = engine->GetSectors ()->FindByName ("room");
  }
  return sector;
}

csVector3 celPcRegion::GetStartPosition (const char* name)
{
  if (empty_sector) return csVector3 (0, 0, 0);
  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != 0);
  csVector3 pos (0);
  if (engine->GetCameraPositions ()->GetCount () > 0)
  {
    iCameraPosition* campos =
    	name ? engine->GetCameraPositions ()->FindByName (name)
       : engine->GetCameraPositions ()->Get (0);
    if (campos) pos = campos->GetPosition ();
  }
  return pos;
}

void celPcRegion::PointCamera (iPcCamera* pccamera, const char* name)
{
  CS_ASSERT(pccamera != 0);

  csRef<iEngine> engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (engine->GetCameraPositions()->GetCount() > 0)
  {
    iCameraPosition* campos =
        name ? engine->GetCameraPositions()->FindByName(name)
       : engine->GetCameraPositions()->Get(0);
    if (campos)
    {
      campos->Load(pccamera->GetCamera(), engine);
      return;
    }
  }

  iSector* s = GetStartSector (name);
  pccamera->GetCamera ()->SetSector (s);
  pccamera->GetCamera ()->GetTransform ().SetOrigin (csVector3(0,0,0));
}

//---------------------------------------------------------------------------

