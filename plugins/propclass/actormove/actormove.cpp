/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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

#include <math.h>

#include "plugins/propclass/actormove/actormove.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "csutil/debug.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivaria/reporter.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"
#include "iengine/mesh.h"
#include "imesh/object.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (ActorMove, "pcactormove")
CEL_IMPLEMENT_FACTORY (NpcMove, "pcnpcmove")

//---------------------------------------------------------------------------

csStringID celPcActorMove::action_setspeed = csInvalidStringID;
csStringID celPcActorMove::action_forward = csInvalidStringID;
csStringID celPcActorMove::action_backward = csInvalidStringID;
csStringID celPcActorMove::action_strafeleft = csInvalidStringID;
csStringID celPcActorMove::action_straferight = csInvalidStringID;
csStringID celPcActorMove::action_rotateleft = csInvalidStringID;
csStringID celPcActorMove::action_rotateright = csInvalidStringID;
csStringID celPcActorMove::action_rotateto = csInvalidStringID;
csStringID celPcActorMove::action_run = csInvalidStringID;
csStringID celPcActorMove::action_autorun = csInvalidStringID;
csStringID celPcActorMove::action_jump = csInvalidStringID;
csStringID celPcActorMove::action_togglecameramode = csInvalidStringID;
csStringID celPcActorMove::id_movement = csInvalidStringID;
csStringID celPcActorMove::id_running = csInvalidStringID;
csStringID celPcActorMove::id_rotation = csInvalidStringID;
csStringID celPcActorMove::id_jumping = csInvalidStringID;
csStringID celPcActorMove::id_start = csInvalidStringID;
csStringID celPcActorMove::id_yrot = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcActorMove)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcActorMove)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcActorMove::PcActorMove)
  SCF_IMPLEMENTS_INTERFACE (iPcActorMove)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcActorMove::celPcActorMove (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcActorMove);

  if (action_setspeed == csInvalidStringID)
  {
    action_setspeed = pl->FetchStringID ("cel.action.SetSpeed");
    action_forward = pl->FetchStringID ("cel.action.Forward");
    action_backward = pl->FetchStringID ("cel.action.Backward");
    action_strafeleft = pl->FetchStringID ("cel.action.StrafeLeft");
    action_straferight = pl->FetchStringID ("cel.action.StrafeRight");
    action_rotateleft = pl->FetchStringID ("cel.action.RotateLeft");
    action_rotateright = pl->FetchStringID ("cel.action.RotateRight");
    action_rotateto = pl->FetchStringID ("cel.action.RotateTo");
    action_run = pl->FetchStringID ("cel.action.Run");
    action_autorun = pl->FetchStringID ("cel.action.AutoRun");
    action_jump = pl->FetchStringID ("cel.action.Jump");
    action_togglecameramode = pl->FetchStringID ("cel.action.ToggleCameraMode");
    id_movement = pl->FetchStringID ("cel.parameter.movement");
    id_running = pl->FetchStringID ("cel.parameter.running");
    id_rotation = pl->FetchStringID ("cel.parameter.rotation");
    id_jumping = pl->FetchStringID ("cel.parameter.jumping");
    id_start = pl->FetchStringID ("cel.parameter.start");
    id_yrot = pl->FetchStringID ("cel.parameter.yrot");
  }

  movement_speed = 2.0f;
  running_speed = 5.0f;
  rotating_speed = 1.75f;
  jumping_velocity = 6.31f;
  forward = false;
  backward = false;
  strafeleft = false;
  straferight = false;
  rotateleft = false;
  rotateright = false;
  rotatetoreached = true;
  running = false;
  autorun = false;
  checked_spritestate = false;
}

celPcActorMove::~celPcActorMove ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcActorMove);
}

#define ACTORMOVE_SERIAL 1

csPtr<iCelDataBuffer> celPcActorMove::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (ACTORMOVE_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}


bool celPcActorMove::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != ACTORMOVE_SERIAL) return false;
  return true;
}

bool celPcActorMove::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_setspeed)
  {
    CEL_FETCH_FLOAT_PAR (movement,params,id_movement);
    if (p_movement) SetMovementSpeed (movement);
    CEL_FETCH_FLOAT_PAR (running,params,id_running);
    if (p_running) SetRunningSpeed (running);
    CEL_FETCH_FLOAT_PAR (rotation,params,id_rotation);
    if (p_rotation) SetRotationSpeed (rotation);
    CEL_FETCH_FLOAT_PAR (jumping,params,id_jumping);
    if (p_jumping) SetJumpingVelocity (jumping);
    return true;
  }
  else if (actionId == action_forward)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    Forward (start);
    return true;
  }
  else if (actionId == action_backward)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    Backward (start);
    return true;
  }
  else if (actionId == action_strafeleft)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    StrafeLeft (start);
    return true;
  }
  else if (actionId == action_straferight)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    StrafeRight (start);
    return true;
  }
  else if (actionId == action_rotateleft)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    RotateLeft (start);
    return true;
  }
  else if (actionId == action_rotateright)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    RotateRight (start);
    return true;
  }
  else if (actionId == action_rotateto)
  {
    CEL_FETCH_FLOAT_PAR (yrot,params,id_yrot);
    if (!p_yrot) return false;
    RotateTo (yrot);
    return true;
  }
  else if (actionId == action_run)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    Run (start);
    return true;
  }
  else if (actionId == action_autorun)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    AutoRun (start);
    return true;
  }
  else if (actionId == action_jump)
  {
    Jump ();
    return true;
  }
  else if (actionId == action_togglecameramode)
  {
    ToggleCameraMode ();
    return true;
  }
  return false;
}

void celPcActorMove::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pccamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcCamera);
    pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcDefaultCamera);
    checked_spritestate = false;
  }
}

void celPcActorMove::GetSpriteStates ()
{
  if (checked_spritestate) return;
  if (!pcmesh) return;
  iMeshWrapper* m = pcmesh->GetMesh ();
  if (!m) return;
  iMeshObject* o = m->GetMeshObject ();
  if (!o) return;
  checked_spritestate = true;
  sprcal3d = SCF_QUERY_INTERFACE (o, iSpriteCal3DState);
  if (sprcal3d) return;
  spr3d = SCF_QUERY_INTERFACE (o, iSprite3DState);
}

void celPcActorMove::RotateTo (float yrot)
{
  FindSiblingPropertyClasses ();
  rotatetoreached = false;

  if (!pclinmove)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		    "cel.pcactormove", "pclinmove is missing!");
    return;
  }
  csVector3 current_position;
  iSector* current_sector;
  float current_yrot;
  pclinmove->GetLastPosition (current_position, current_yrot, current_sector);

  current_yrot = atan2f (sin (current_yrot), cos (current_yrot));
  rotate_to = atan2f (sin (yrot), cos (yrot));
  float delta_angle = atan2f (sin (rotate_to - current_yrot),
  	cos (rotate_to - current_yrot));

  if (fabs(delta_angle) < SMALL_EPSILON)
  {
    rotateright = false;
    rotateleft = false;
    return;
  }

  if (current_yrot < 0)
  {
    rotateright = (rotate_to > current_yrot)
    	&& (rotate_to < (current_yrot + PI));
    rotateleft = !rotateright;
  }
  else
  {
    rotateleft = (rotate_to > (current_yrot - PI))
    	&& (rotate_to < current_yrot);
    rotateright = !rotateleft;
  }

  HandleMovement (false);
}

void celPcActorMove::HandleMovement (bool jump)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		    "cel.pcactormove", "pclinmove is missing!");
    return;
  }
  GetSpriteStates ();
  if (!pcmesh)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		    "cel.pcactormove", "pcmesh is missing!");
    return;
  }

  float speed;
  if (running)
    speed = running_speed;
  else
    speed = movement_speed;

  static csVector3 velocity(0,0,0);

  if ((autorun || forward) && straferight)
  {
    velocity.x = -0.75 * speed;
    velocity.z = -0.75 * speed;
  }
  else if ((autorun || forward) && strafeleft)
  {
    velocity.x = 0.75 * speed;
    velocity.z = -0.75 * speed;
  }
  else if (autorun || forward)
  {
    velocity.x = 0;
    velocity.z = -speed;
  }
  else if (backward && straferight)
  {
    velocity.x = -0.75 * speed;
    velocity.z = 0.75 * speed;
  }
  else if (backward && strafeleft)
  {
    velocity.x = 0.75 * speed;
    velocity.z = 0.75 * speed;
  }
  else if (backward)
  {
    velocity.x = 0;
    velocity.z = speed;
  }
  else if (straferight)
  {
    velocity.x = -speed;
    velocity.z = 0;
  }
  else if (strafeleft)
  {
    velocity.x = speed;
    velocity.z = 0;
  }
  else
  {
    velocity.x = 0;
    velocity.z = 0;
  }
  pclinmove->SetVelocity (velocity);
  if (sprcal3d) sprcal3d->SetVelocity (-velocity.z);
  // @@@ do spr3d!

  float actual_rotating_speed = 0;
  if (rotateright)
    actual_rotating_speed = -rotating_speed;
  else if (rotateleft)
    actual_rotating_speed = rotating_speed;
  if (rotatetoreached)
  {
    pclinmove->SetAngularVelocity (csVector3 (0, actual_rotating_speed, 0));
  }
  else
  {
    pclinmove->SetAngularVelocity (csVector3 (0, actual_rotating_speed, 0),
				   csVector3 (0, rotate_to, 0));
  }

  if (jump && pclinmove->IsOnGround ())
  {
    csVector3 velocity (0,jumping_velocity, 0);
    pclinmove->AddVelocity (velocity);
    if (sprcal3d) sprcal3d->SetVelocity (-velocity.z);
    // @@@ do spr3d!
  }
}

void celPcActorMove::ToggleCameraMode ()
{
  FindSiblingPropertyClasses ();
  if (!pcdefcamera)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		    "cel.pcactormove", "pcdefaultcamera is missing!");
    return;
  }
  pcdefcamera->SetMode (pcdefcamera->GetNextMode ());
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcNpcMove)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcNpcMove)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcNpcMove::PcNpcMove)
  SCF_IMPLEMENTS_INTERFACE (iPcNpcMove)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcNpcMove::celPcNpcMove (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcNpcMove);
  checked_spritestate = false;
  pl->CallbackOnce ((iCelTimerListener*)this, 50, cscmdPreProcess);
}

celPcNpcMove::~celPcNpcMove ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcNpcMove);
}

void celPcNpcMove::TickOnce ()
{
  FindSiblingPropertyClasses ();
  GetSpriteStates ();

  if (sprcal3d)
    sprcal3d->SetAnimCycle ("stand", 1.0);
  else if (spr3d)
    spr3d->SetAction ("stand");
}

#define NPCMOVE_SERIAL 1

csPtr<iCelDataBuffer> celPcNpcMove::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (NPCMOVE_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}


bool celPcNpcMove::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != NPCMOVE_SERIAL) return false;
  return true;
}

void celPcNpcMove::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    checked_spritestate = false;
  }
}

void celPcNpcMove::GetSpriteStates ()
{
  if (checked_spritestate) return;
  if (!pcmesh) return;
  iMeshWrapper* m = pcmesh->GetMesh ();
  if (!m) return;
  iMeshObject* o = m->GetMeshObject ();
  if (!o) return;
  checked_spritestate = true;
  sprcal3d = SCF_QUERY_INTERFACE (o, iSpriteCal3DState);
  if (sprcal3d) return;
  spr3d = SCF_QUERY_INTERFACE (o, iSprite3DState);
}

//---------------------------------------------------------------------------

