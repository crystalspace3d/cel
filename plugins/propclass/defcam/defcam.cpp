/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2005 by Jorrit Tyberghein

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
#include "propclass/region.h"
#include "plugins/propclass/defcam/defcam.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/debug.h"
#include "csutil/flags.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "iutil/virtclk.h"
#include "iutil/csinput.h"
#include "imap/loader.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/campos.h"
#include "iengine/movable.h"
#include "iengine/camera.h"
#include "iengine/region.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/collider.h"
#include "ivaria/view.h"
#include "ivaria/collider.h"
#include "ivaria/reporter.h"
#include "ivideo/graph3d.h"
#include "csqsqrt.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (DefaultCamera, "pcdefaultcamera")

//---------------------------------------------------------------------------

static float FixAngle (float angle)
{
  while (angle >= PI)
    angle -= 2*PI;
  while (angle < -PI)
    angle += 2*PI;
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
  parent->GetLastFullPosition (actor_pos, actor_yrot, actor_sector);
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
  else if (parent->GetMechObj ())
  {
    csVector3 velocity = parent->GetMechObj ()->GetLinearVelocity ();
    velNormSquared = velocity.SquaredNorm ();
  }
  else
  {
    velNormSquared = 1.0f;
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

csStringID celPcDefaultCamera::id_modename = csInvalidStringID;
csStringID celPcDefaultCamera::id_spring = csInvalidStringID;
csStringID celPcDefaultCamera::id_turnspeed = csInvalidStringID;
csStringID celPcDefaultCamera::id_swingcoef = csInvalidStringID;
csStringID celPcDefaultCamera::id_fpoffset = csInvalidStringID;
csStringID celPcDefaultCamera::id_tpoffset = csInvalidStringID;
csStringID celPcDefaultCamera::id_pitch = csInvalidStringID;
csStringID celPcDefaultCamera::id_pitchvelocity = csInvalidStringID;
csStringID celPcDefaultCamera::id_yaw = csInvalidStringID;
csStringID celPcDefaultCamera::id_yawvelocity = csInvalidStringID;
csStringID celPcDefaultCamera::id_distance = csInvalidStringID;
csStringID celPcDefaultCamera::id_entityname = csInvalidStringID;
csStringID celPcDefaultCamera::id_regionname = csInvalidStringID;
csStringID celPcDefaultCamera::id_startname = csInvalidStringID;

PropertyHolder celPcDefaultCamera::propinfo;

SCF_IMPLEMENT_IBASE_EXT (celPcDefaultCamera)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcDefaultCamera)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCamera)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcDefaultCamera::PcDefaultCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcDefaultCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcCamera)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcDefaultCamera::celPcDefaultCamera (iObjectRegistry* object_reg)
  : celPcCameraCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcDefaultCamera);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCamera);

  modeset_needed = false;
  cammode = iPcDefaultCamera::freelook;
  camalgo = 0;

  use_cd = false;
  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  mouse = CS_QUERY_REGISTRY (object_reg, iMouseDriver);
  CS_ASSERT (kbd != 0);
  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);

  // Most of these are default values that will be overwritten
  firstPersonPositionOffset = csVector3 (0, 1, 0);
  thirdPersonPositionOffset = csVector3 (0, 1, 3);

  camData[iPcDefaultCamera::firstperson].springCoef = 10.0f;
  camData[iPcDefaultCamera::firstperson].InertialDampeningCoef = 0.1f;
  camData[iPcDefaultCamera::firstperson].springLength = 0.01f;

  camData[iPcDefaultCamera::thirdperson].springCoef = 3.5f;
  camData[iPcDefaultCamera::thirdperson].InertialDampeningCoef = 0.25f;
  camData[iPcDefaultCamera::thirdperson].springLength = 0.01f;

  camData[iPcDefaultCamera::m64_thirdperson].springCoef = 3.5f;
  camData[iPcDefaultCamera::m64_thirdperson].InertialDampeningCoef = 0.25f;
  camData[iPcDefaultCamera::m64_thirdperson].springLength = 0.01f;
  camData[iPcDefaultCamera::m64_thirdperson].minDistance = 2.0f;
  camData[iPcDefaultCamera::m64_thirdperson].maxDistance = 6.0f;
  camData[iPcDefaultCamera::m64_thirdperson].turnSpeed = 1.0f;

  camData[iPcDefaultCamera::lara_thirdperson].springCoef = 3.5f;
  camData[iPcDefaultCamera::lara_thirdperson].InertialDampeningCoef = 0.25f;
  camData[iPcDefaultCamera::lara_thirdperson].springLength = 0.01f;
  camData[iPcDefaultCamera::lara_thirdperson].minDistance = 2.0f;
  camData[iPcDefaultCamera::lara_thirdperson].maxDistance = 6.0f;
  camData[iPcDefaultCamera::lara_thirdperson].turnSpeed = 1.0f;
  camData[iPcDefaultCamera::lara_thirdperson].swingCoef = 0.7f;

  camData[iPcDefaultCamera::freelook].springCoef = 3.5f;
  camData[iPcDefaultCamera::freelook].InertialDampeningCoef = 0.25f;
  camData[iPcDefaultCamera::freelook].springLength = 0.01f;
  camData[iPcDefaultCamera::freelook].minDistance = 2.0f;
  camData[iPcDefaultCamera::freelook].maxDistance = 16.0f;

  camData[iPcDefaultCamera::transition].springCoef = 3.5f;
  camData[iPcDefaultCamera::transition].InertialDampeningCoef = 0.25f;
  camData[iPcDefaultCamera::transition].springLength = 0.01f;

  int i;
  csVector3 origin (0);
  for (i=0 ; i < iPcDefaultCamera::CameraMode_count ; i++)
  {
    SetPosition (origin, i);
    SetTarget (origin, i);
    SetUp (csVector3 (0, 1, 0), i);
    SetDistance (5.0f, i);
    SetPitch (0.0f, i);
    SetDefaultPitch (0.0f, i);
    SetYaw (0.0f, i);
  }

  // The error has to be set to 0 to begin with.
  SetPosition (origin, iPcDefaultCamera::camerror);
  SetTarget (origin, iPcDefaultCamera::camerror);
  SetUp (origin, iPcDefaultCamera::camerror);

  transitionThresholdSquared = 1.0f;
  cameraHasBeenPositioned = false;

  useCameraCD = true;
  lastActorSector = 0;

  if (id_modename == csInvalidStringID)
  {
    id_modename = pl->FetchStringID ("cel.parameter.modename");
    id_spring = pl->FetchStringID ("cel.parameter.spring");
    id_turnspeed = pl->FetchStringID ("cel.parameter.turnspeed");
    id_swingcoef = pl->FetchStringID ("cel.parameter.swingcoef");
    id_fpoffset = pl->FetchStringID ("cel.parameter.fpoffset");
    id_tpoffset = pl->FetchStringID ("cel.parameter.tpoffset");
    id_pitch = pl->FetchStringID ("cel.parameter.pitch");
    id_pitchvelocity = pl->FetchStringID ("cel.parameter.pitchvelocity");
    id_yaw = pl->FetchStringID ("cel.parameter.yaw");
    id_yawvelocity = pl->FetchStringID ("cel.parameter.yawvelocity");
    id_distance = pl->FetchStringID ("cel.parameter.distance");
    id_entityname = pl->FetchStringID ("cel.parameter.entity");
    id_regionname = pl->FetchStringID ("cel.parameter.region");
    id_startname = pl->FetchStringID ("cel.parameter.start");
  }

  SetMode (iPcDefaultCamera::firstperson);

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_setcamera, "cel.action.SetCamera");
    AddAction (action_pointcamera, "cel.action.PointCamera");
    AddAction (action_setzonemanager, "cel.action.SetZoneManager");
    AddAction (action_centercamera, "cel.action.CenterCamera");
  }

  // For properties.
  propinfo.SetCount (1);
  AddProperty (propid_pitchvelocity, "cel.property.pitchvelocity",
	CEL_DATA_FLOAT, false, "Pitch velocity.", &pitchVelocity);
}

celPcDefaultCamera::~celPcDefaultCamera ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcDefaultCamera);
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcCamera);
}

bool celPcDefaultCamera::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_centercamera:
      CenterCamera ();
      return true;
    case action_pointcamera:
      {
        CEL_FETCH_STRING_PAR (startname,params,id_startname);
        if (!p_startname) return false;
        return PointCamera (startname);
      }
    case action_setcamera:
      {
        CEL_FETCH_STRING_PAR (modename,params,id_modename);
        if (p_modename)
        {
          SetModeName (modename);
        }
        CEL_FETCH_VECTOR3_PAR (spring,params,id_spring);
        if (p_spring)
        {
          SetSpringCoef (spring.x);
          SetDampeningCoef (spring.y);
          SetSpringLength (spring.z);
        }
        CEL_FETCH_FLOAT_PAR (turnspeed,params,id_turnspeed);
        if (p_turnspeed)
        {
          SetTurnSpeed (turnspeed);
        }
        CEL_FETCH_FLOAT_PAR (swingcoef,params,id_swingcoef);
        if (p_swingcoef)
        {
          SetSwingCoef (swingcoef);
        }
        CEL_FETCH_VECTOR3_PAR (fpoffset,params,id_fpoffset);
        if (p_fpoffset)
        {
          firstPersonPositionOffset = fpoffset;
        }
        CEL_FETCH_VECTOR3_PAR (tpoffset,params,id_tpoffset);
        if (p_tpoffset)
        {
          thirdPersonPositionOffset = tpoffset;
        }
        CEL_FETCH_FLOAT_PAR (pitch,params,id_pitch);
        if (p_pitch)
        {
          SetPitch (pitch);
        }
        CEL_FETCH_FLOAT_PAR (pitchvelocity,params,id_pitchvelocity);
        if (p_pitchvelocity)
        {
          SetPitchVelocity (pitchvelocity);
        }
        CEL_FETCH_FLOAT_PAR (yaw,params,id_yaw);
        if (p_yaw)
        {
          SetYaw (yaw);
        }
        CEL_FETCH_FLOAT_PAR (yawvelocity,params,id_yawvelocity);
        if (p_yawvelocity)
        {
          SetYawVelocity (yawvelocity);
        }
        CEL_FETCH_VECTOR3_PAR (distance,params,id_distance);
        if (p_distance)
        {
          SetMinDistance (distance.x);
          SetDistance (distance.y);
          SetMaxDistance (distance.z);
        }
        return true;
      }
    case action_setzonemanager:
      {
        CEL_FETCH_STRING_PAR (entityname,params,id_entityname);
        if (!p_entityname) return false;
        CEL_FETCH_STRING_PAR (regionname,params,id_regionname);
        if (!p_regionname) return false;
        CEL_FETCH_STRING_PAR (startname,params,id_startname);
        if (!p_startname) return false;
        SetZoneManager (entityname, true, regionname, startname);
        return true;
      }
    default:
      return false;
  }
}

bool celPcDefaultCamera::PointCamera (const char* start)
{
  iCameraPosition* campos = engine->FindCameraPosition (start);
  if (!campos) return false;
  iCamera* c = view->GetCamera ();
  campos->Load (c, engine);
  return true;
}

void celPcDefaultCamera::SetFollowEntity (iCelEntity* entity)
{
  follow_entity = entity;
  if (follow_entity)
  {
    pclinmove = CEL_QUERY_PROPCLASS_ENT (follow_entity, iPcLinearMovement);
    pcmechobj = CEL_QUERY_PROPCLASS_ENT (follow_entity, iPcMechanicsObject);
    pcmesh = CEL_QUERY_PROPCLASS_ENT (follow_entity, iPcMesh);
  }
  else
  {
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity, iPcMechanicsObject);
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

void celPcDefaultCamera::FindSiblingPropertyClasses ()
{
  if (follow_entity) return;
  if (HavePropertyClassesChanged ())
  {
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity, iPcMechanicsObject);
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

static float GetAngle (float x, float y)
{
  if (x > 1.0f)  x = 1.0f;
  if (x < -1.0f) x = -1.0f;

  float angle = acos (x);
  if (y < 0)
    angle = 2*PI - angle;

  return angle;
}

void celPcDefaultCamera::GetLastFullPosition (csVector3& actor_pos,
	float& actor_yrot, iSector*& actor_sector)
{
  // Try to get position and sector from either linmove or mesh if
  // linmove is not used.
  FindSiblingPropertyClasses ();
  if (pclinmove)
  {
    pclinmove->GetLastFullPosition (actor_pos, actor_yrot, actor_sector);
    actor_yrot = FixAngle (actor_yrot);
  }
  else if (pcmechobj)
  {
    iPcMesh* pcmesh = pcmechobj->GetMesh();
    iMeshWrapper* mesh = 0;
    if (pcmesh) mesh = pcmesh->GetMesh ();
    if (!mesh)
    {
      actor_pos.Set (0, 0, 0);
      actor_yrot = 0;
      actor_sector = 0;
      return;
    }
    iMovable* movable = mesh->GetMovable();
    actor_pos = movable->GetFullPosition ();
    actor_sector = movable->GetSectors ()->Get (0);
    csVector3 fwd = movable->GetFullTransform ().GetFront ();
    float a = GetAngle (fwd.z, fwd.x);
    actor_yrot = FixAngle (a);
  }
  else if (pcmesh)
  {
    iMovable* movable = pcmesh->GetMesh()->GetMovable();
    actor_pos = movable->GetFullPosition ();
    actor_sector = movable->GetSectors ()->Get (0);
    csVector3 fwd = movable->GetFullTransform ().GetFront ();
    float a = GetAngle (fwd.z, fwd.x);
    actor_yrot = FixAngle (a);
  }
  else
  {
    actor_pos.Set (0, 0, 0);
    actor_yrot = 0;
    actor_sector = 0;
  }
}

void celPcDefaultCamera::CalculateFromYawPitchRoll (int mode)
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

void celPcDefaultCamera::CalculatePositionFromYawPitchRoll (int mode)
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

void celPcDefaultCamera::EnsureCameraDistance (int mode)
{
  if (GetDistance (mode) > GetMaxDistance (mode))
    SetDistance (GetMaxDistance (mode), mode);
  else if (GetDistance (mode) < GetMinDistance (mode))
    SetDistance (GetMinDistance (mode), mode);
}

void celPcDefaultCamera::ResetActualCameraData ()
{
  SetPosition (GetPosition (), iPcDefaultCamera::actual_data);
  SetTarget (GetTarget (), iPcDefaultCamera::actual_data);
  SetUp (GetUp (), iPcDefaultCamera::actual_data);
}

void celPcDefaultCamera::DoCameraTransition ()
{
  if (inTransitionPhase)
  {
    if ((GetPosition () - GetPosition (iPcDefaultCamera::actual_data))
    	.SquaredNorm() < transitionThresholdSquared)
    {
      inTransitionPhase = false;
    }
  }
}

csVector3 celPcDefaultCamera::CalcCollisionPos (const csVector3& pseudoTarget,
	const csVector3& pseudoPosition, iSector* sector) const
{
  if (!useCameraCD)
    return pseudoPosition; // no collision detection

  if (!pcmesh)
    return pseudoPosition; // no mesh

  pcmesh->GetMesh()->GetFlags().Set (CS_ENTITY_NOHITBEAM);
  switch (GetMode ())
  {
    case iPcDefaultCamera::thirdperson:
    case iPcDefaultCamera::m64_thirdperson:
    case iPcDefaultCamera::lara_thirdperson:
    case iPcDefaultCamera::freelook:
    {
#if 0
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
#else
      csVector3 isect;
      csVector3 modifiedTarget = pseudoTarget;

      csIntersectingTriangle closest_tri;
      float sqdist = csColliderHelper::TraceBeam (cdsys, sector,
      	modifiedTarget, pseudoPosition, true, closest_tri, isect);

      if (sqdist >= 0)
      {
        pcmesh->GetMesh()->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
        return isect;
      }
#endif
      break;
    }
    default:
      break;
  }
  pcmesh->GetMesh()->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
  return pseudoPosition;
}

void celPcDefaultCamera::DoElasticPhysics (bool isElastic,
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
      cameraSpringCoef = GetSpringCoef (iPcDefaultCamera::transition);
      cameraInertialDampeningCoef = GetDampeningCoef (
      	iPcDefaultCamera::transition);
      cameraSpringLength = GetSpringLength(iPcDefaultCamera::transition);
    }

    csVector3 newPos, newTar, newUp;

    newPos = CalcElasticPos (GetPosition (iPcDefaultCamera::actual_data),
    	CalcCollisionPos (GetTarget(), GetPosition(), sector),
	deltaIdeal.worldPos,
	(float)elapsedTicks/1000.0f, cameraSpringCoef,
	cameraInertialDampeningCoef, cameraSpringLength);
    SetPosition (newPos, iPcDefaultCamera::actual_data);

    newTar = CalcElasticPos (GetTarget (iPcDefaultCamera::actual_data),
    	GetTarget (),
    	deltaIdeal.worldTar, (float)elapsedTicks/1000.0f, cameraSpringCoef,
	cameraInertialDampeningCoef, cameraSpringLength);
    SetTarget(newTar, iPcDefaultCamera::actual_data);

    newUp = CalcElasticPos (GetUp (iPcDefaultCamera::actual_data), GetUp (),
    	deltaIdeal.worldUp, (float)elapsedTicks/1000.0f, cameraSpringCoef,
	cameraInertialDampeningCoef, cameraSpringLength);
    SetUp (newUp, iPcDefaultCamera::actual_data);
  }
  else
  {
    // Camera isn't elastic, so no interpolation is done between ideal and
    // actual.
    SetPosition (CalcCollisionPos (GetTarget (), GetPosition (), sector),
    	iPcDefaultCamera::actual_data);
    SetTarget (GetTarget (), iPcDefaultCamera::actual_data);
    SetUp (GetUp (), iPcDefaultCamera::actual_data);
  }
}

void celPcDefaultCamera::CenterCamera ()
{
  csVector3 actor_pos;
  float actor_yrot;
  iSector* actor_sector;
  GetLastFullPosition (actor_pos, actor_yrot, actor_sector);
  SetPosition (actor_pos +
  	csVector3 (
  	  sin (actor_yrot) * GetMaxDistance (),
  	  0.0,
	  cos (actor_yrot) * GetMaxDistance ()));
  SetYaw (actor_yrot);
  if (cammode == iPcDefaultCamera::freelook)
    SetPitch (0);
}

void celPcDefaultCamera::UpdateCamera ()
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
  GetLastFullPosition (actor_pos, actor_yrot, actor_sector);
  if (!actor_sector)
  {
    // We have no actor, so just keep current camera settings.
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
    if (cammode != iPcDefaultCamera::firstperson || inTransitionPhase)
    {
      if ((GetPosition (iPcDefaultCamera::actual_data)
	  - GetTarget (iPcDefaultCamera::actual_data)).SquaredNorm () > 0.3f)
        pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE, 0);
    }
    else
    {
      pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE,
		      CS_ENTITY_INVISIBLE);
    }
  }
  iCamera* c = view->GetCamera ();

  // First set the camera back on where the sector is.
  // We assume here that normal camera movement is good.
  if (c->GetSector () != actor_sector)
    c->SetSector (actor_sector);
  //c->GetTransform ().SetOrigin (actor_pos+c->GetTransform ().
		    //This2OtherRelative (csVector3 (0, 0, .1)));
  c->GetTransform ().SetOrigin (actor_pos+csVector3 (0, 0.1f, 0));
  c->OnlyPortals (true);

  // Now move it to the new position.
  c->MoveWorld (GetPosition (iPcDefaultCamera::actual_data) - actor_pos);

  // If the actor changed sectors then ensure that the position is correct
  // in case it is a warping portal.
  if (actor_sector != lastActorSector && lastActorSector != 0)
  {
    SetPosition (GetPosition () + GetPosition (iPcDefaultCamera::camerror),
    	iPcDefaultCamera::actual_data);
    SetTarget (GetTarget () + GetTarget (iPcDefaultCamera::camerror),
    	iPcDefaultCamera::actual_data);
    SetUp (GetUp () + GetUp (iPcDefaultCamera::camerror),
    	iPcDefaultCamera::actual_data);
  }

  c->GetTransform ().LookAt (GetTarget (iPcDefaultCamera::actual_data) -
    	GetPosition (iPcDefaultCamera::actual_data),
	GetUp (iPcDefaultCamera::actual_data));

  // Now calculate the error of the camera.
  SetPosition (GetPosition (iPcDefaultCamera::actual_data) - GetPosition (),
  	iPcDefaultCamera::camerror);
  SetTarget (GetTarget (iPcDefaultCamera::actual_data) - GetTarget (),
  	iPcDefaultCamera::camerror);
  SetUp (GetUp (iPcDefaultCamera::actual_data) - GetUp (),
  	iPcDefaultCamera::camerror);
}

int celPcDefaultCamera::GetDrawFlags ()
{
  return engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS
    | (clear_zbuf ? CSDRAW_CLEARZBUFFER : 0)
    | (clear_screen ? CSDRAW_CLEARSCREEN : 0);
}

void celPcDefaultCamera::Draw()
{
  UpdateCamera ();

  // Tell 3D driver we're going to display 3D things.
  if (g3d->BeginDraw (GetDrawFlags ()))
    view->Draw ();
}

void celPcDefaultCamera::SetPosition (const csVector3& pos, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].worldPos = pos;
}

const csVector3& celPcDefaultCamera::GetPosition (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].worldPos;
}

void celPcDefaultCamera::SetTarget (const csVector3& tar, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].worldTar = tar;
}

const csVector3& celPcDefaultCamera::GetTarget (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].worldTar;
}

void celPcDefaultCamera::SetUp (const csVector3& up, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].worldUp = up;
  camData[mode].worldUp.Normalize();
}

const csVector3& celPcDefaultCamera::GetUp (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].worldUp;
}

void celPcDefaultCamera::SetPitch (float pitch, int mode)
{
  if (mode < 0) mode = cammode;

  if (pitch > 3.1415f/2.1f)
    pitch = 3.1415f/2.1f;
  if (pitch < -3.1415f/2.1f)
    pitch = -3.1415f/2.1f;

  camData[mode].pitch = pitch;
}

void celPcDefaultCamera::MovePitch (float deltaPitch, int mode)
{
  if (mode < 0) mode = cammode;
  SetPitch (GetPitch(mode) + deltaPitch, mode);
}

float celPcDefaultCamera::GetPitch (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].pitch;
}

void celPcDefaultCamera::SetYaw (float yaw, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].yaw = FixAngle (yaw);
}

void celPcDefaultCamera::MoveYaw (float deltaYaw, int mode)
{
  if (mode < 0) mode = cammode;
  SetYaw (GetYaw (mode) + deltaYaw, mode);
}

float celPcDefaultCamera::GetYaw (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].yaw;
}

void celPcDefaultCamera::SetYawVelocity (float yawVel)
{
  yawVelocity = yawVel;
}

float celPcDefaultCamera::GetYawVelocity () const
{
  return yawVelocity;
}

void celPcDefaultCamera::SetDistance (float distance, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].distance = distance;
}

void celPcDefaultCamera::MoveDistance (float deltaDistance, int mode)
{
  SetDistance (GetDistance (mode) + deltaDistance, mode);
}

float celPcDefaultCamera::GetDistance (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].distance;
}

csVector3 celPcDefaultCamera::GetForwardVector (int mode) const
{
  if (mode < 0) mode = cammode;
  csVector3 dir = GetTarget (mode) - GetPosition (mode);
  dir.Normalize();
  return dir;
}

csVector3 celPcDefaultCamera::GetRightVector (int mode) const
{
  if (mode < 0) mode = cammode;
  csVector3 dir = GetTarget (mode) - GetPosition(mode);
  csVector3 right = GetUp (mode) % dir;
  right.Normalize();
  return right;
}

float celPcDefaultCamera::GetMinDistance (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].minDistance;
}

void celPcDefaultCamera::SetMinDistance (float dist, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].minDistance = dist;
}

float celPcDefaultCamera::GetMaxDistance (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].maxDistance;
}

void celPcDefaultCamera::SetMaxDistance (float dist, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].maxDistance = dist;
}

float celPcDefaultCamera::GetTurnSpeed (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].turnSpeed;
}

void celPcDefaultCamera::SetTurnSpeed (float speed, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].turnSpeed = speed;
}

float celPcDefaultCamera::GetSpringCoef (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].springCoef;
}

void celPcDefaultCamera::SetSpringCoef (float coef, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].springCoef = coef;
}

float celPcDefaultCamera::GetDampeningCoef (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].InertialDampeningCoef;
}

void celPcDefaultCamera::SetDampeningCoef (float coef, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].InertialDampeningCoef = coef;
}

float celPcDefaultCamera::GetSpringLength (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].springLength;
}

void celPcDefaultCamera::SetSpringLength (float length, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].springLength = length;
}

bool celPcDefaultCamera::CheckCameraCD () const
{
  return useCameraCD;
}

void celPcDefaultCamera::SetCameraCD (bool useCD)
{
  useCameraCD = useCD;
}

float celPcDefaultCamera::GetTransitionThreshold () const
{
  return sqrt (transitionThresholdSquared);
}

void celPcDefaultCamera::SetTransitionThreshold (float threshold)
{
  transitionThresholdSquared = threshold*threshold;
}

float celPcDefaultCamera::GetDefaultPitch (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].defaultPitch;
}

void celPcDefaultCamera::SetDefaultPitch (float pitch, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].defaultPitch = FixAngle (pitch);
}

float celPcDefaultCamera::GetDefaultYaw (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].defaultYaw;
}

void celPcDefaultCamera::SetDefaultYaw (float yaw, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].defaultYaw = FixAngle (yaw);
}

float celPcDefaultCamera::GetSwingCoef (int mode) const
{
  if (mode < 0) mode = cammode;
  return camData[mode].swingCoef;
}

void celPcDefaultCamera::SetSwingCoef (float swingCoef, int mode)
{
  if (mode < 0) mode = cammode;
  camData[mode].swingCoef = swingCoef;
}

void celPcDefaultCamera::CheckModeChange ()

{
  if (!modeset_needed) return;
  modeset_needed = false;
  camalgo->SetupMode ();
  // Enable transition phase.
  inTransitionPhase = true;
}

bool celPcDefaultCamera::SetMode (iPcDefaultCamera::CameraMode cammode,
	bool use_cd)
{
  // Reset camera velocities.
  SetYawVelocity (0.0f);
  SetPitchVelocity (0.0f);

  delete camalgo;
  switch (cammode)
  {
    case iPcDefaultCamera::freelook:
      camalgo = new CAFreeLook (this);
      break;
    case iPcDefaultCamera::firstperson:
      camalgo = new CAFirstPerson (this);
      break;
    case iPcDefaultCamera::thirdperson:
      camalgo = new CAThirdPerson (this);
      break;
    case iPcDefaultCamera::m64_thirdperson:
      camalgo = new CAM64ThirdPerson (this);
      break;
    case iPcDefaultCamera::lara_thirdperson:
      camalgo = new CALaraThirdPerson (this);
      break;
    default:
      camalgo = 0;
      break;
  }

  if (!modeset_needed) prev_cammode = cammode;
  celPcDefaultCamera::cammode = cammode;
  celPcDefaultCamera::use_cd = use_cd;
  view->GetCamera ()->OnlyPortals (!use_cd);
  modeset_needed = true;

  return true;
}

bool celPcDefaultCamera::SetModeName (const char* m, bool use_cd)
{
  iPcDefaultCamera::CameraMode mode;
  if (!strcmp (m, "freelook"))
    mode = iPcDefaultCamera::freelook;
  else if (!strcmp (m, "firstperson"))
    mode = iPcDefaultCamera::firstperson;
  else if (!strcmp (m, "thirdperson"))
    mode = iPcDefaultCamera::thirdperson;
  else if (!strcmp (m, "m64_thirdperson"))
    mode = iPcDefaultCamera::m64_thirdperson;
  else if (!strcmp (m, "lara_thirdperson"))
    mode = iPcDefaultCamera::lara_thirdperson;
  else if (!strcmp (m, "freelook"))
    mode = iPcDefaultCamera::freelook;
  else
    return false;
  return SetMode (mode, use_cd);
}

const char* celPcDefaultCamera::GetModeName () const
{
  return camalgo->GetName ();
}

iPcDefaultCamera::CameraMode celPcDefaultCamera::GetNextMode () const
{
  switch (cammode)
  {
    case iPcDefaultCamera::freelook:
      return iPcDefaultCamera::firstperson;
    case iPcDefaultCamera::firstperson:
      return iPcDefaultCamera::thirdperson;
    case iPcDefaultCamera::thirdperson:
      return iPcDefaultCamera::m64_thirdperson;
    case iPcDefaultCamera::m64_thirdperson:
      return iPcDefaultCamera::lara_thirdperson;
    case iPcDefaultCamera::lara_thirdperson:
      return iPcDefaultCamera::freelook;
    default:
      return cammode;
  }
}

#define DEFAULT_CAMERA_SERIAL 3

csPtr<iCelDataBuffer> celPcDefaultCamera::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (DEFAULT_CAMERA_SERIAL);
  SaveCommon (databuf);

  databuf->Add ((uint8)cammode);
  databuf->Add (use_cd);

  // @@@ TODO: save cammode specific parameters.

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcDefaultCamera::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != DEFAULT_CAMERA_SERIAL)
  {
    Report (object_reg, "serialnr != DEFAULT_CAMERA_SERIAL.  Cannot load.");
    return false;
  }

  if (!LoadCommon (databuf)) return false;

  iPcDefaultCamera::CameraMode mode = (iPcDefaultCamera::CameraMode)databuf
  	->GetUInt8 ();
  bool cd = databuf->GetBool ();
  SetMode (mode, cd);

  return true;
}

//---------------------------------------------------------------------------

