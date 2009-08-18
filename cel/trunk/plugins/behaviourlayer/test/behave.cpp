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
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/actormove.h"
#include "propclass/mesh.h"
#include "propclass/meshsel.h"
#include "propclass/tooltip.h"
#include "propclass/camera.h"
#include "propclass/defcam.h"
#include "propclass/newcamera.h"
#include "propclass/delegcam.h"
#include "propclass/cameras/tracking.h"
#include "propclass/analogmotion.h"
#include "propclass/jump.h"
#include "propclass/grab.h"
#include "propclass/inv.h"
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "propclass/mechsys.h"
#include "propclass/wheeled.h"
#include "propclass/linmove.h"
#include "plugins/behaviourlayer/test/behave.h"
#include "celtool/stdparams.h"
#include <iostream>

//---------------------------------------------------------------------------

celBehaviourGeneral::celBehaviourGeneral (iCelEntity* entity,
	iObjectRegistry* object_reg) : scfImplementationType (this)
{
  celBehaviourGeneral::entity = entity;
  celBehaviourGeneral::object_reg = object_reg;
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  name = 0;
  entity->QueryMessageChannel ()->Subscribe (this, "cel.");
}

celBehaviourGeneral::~celBehaviourGeneral ()
{
  delete[] name;
}

#if 0
// Old messages no longer supported.
bool celBehaviourGeneral::SendMessage (const char* msg_id,
	iCelPropertyClass* pc,
	celData& ret, iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, pc, ret, params, arg);
  va_end (arg);
  return rc;
}
#endif

//---------------------------------------------------------------------------

celBehaviourPrinter::celBehaviourPrinter (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

//---------------------------------------------------------------------------

celBehaviourRoom::celBehaviourRoom (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

//---------------------------------------------------------------------------

celBehaviourQuest::celBehaviourQuest (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

//---------------------------------------------------------------------------


celBehaviourBox::celBehaviourBox (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
  msgid_meshsel_down = pl->FetchStringID ("cel.mesh.select.down");
  msgid_pctimer_wakeup = pl->FetchStringID ("cel.timer.wakeup");
  id_param_entity = pl->FetchStringID ("cel.parameter.entity");
}

bool celBehaviourBox::ReceiveMessage (csStringID msg_id,
	iMessageSender*,
	celData&, iCelParameterBlock* params)
{
  iCelEntity* ent = 0;
  if (params)
  {
    const celData* cd = params->GetParameter (id_param_entity);
    if (cd)
      ent = cd->value.ent;
  }

  if (ent && msg_id == msgid_meshsel_down)
  {
    csRef<iPcMesh> pcmesh (CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcMesh));
    CS_ASSERT (pcmesh != 0);
    const char* curact = pcmesh->GetAction ();
    if (!strcmp (curact, "open"))
      pcmesh->SetAction ("closed");
    else
    {
      pcmesh->SetAction ("open");
      // If the box is opened we remove everything from it.
      csRef<iPcTimer> pctimer (CEL_QUERY_PROPCLASS (
		entity->GetPropertyClassList (), iPcTimer));
      CS_ASSERT (pctimer != 0);
      pctimer->WakeUp (200, false);
    }
  }
  else if (msg_id == msgid_pctimer_wakeup)
  {
    csRef<iPcTimer> pctimer (CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcTimer));
    CS_ASSERT (pctimer != 0);
    csRef<iPcMesh> pcmesh (CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcMesh));
    CS_ASSERT (pcmesh != 0);
    // Remove one entity from the box.
    csRef<iPcInventory> pcinv (CEL_QUERY_PROPCLASS (
		entity->GetPropertyClassList (), iPcInventory));
    CS_ASSERT (pcinv != 0);
    if (pcinv->GetEntityCount () > 0)
    {
      iCelEntity* inv_ent = pcinv->GetEntity (0);
      csRef<iPcGravity> inv_ent_gravity (CEL_QUERY_PROPCLASS (
	    	inv_ent->GetPropertyClassList (), iPcGravity));
      csRef<iPcMesh> inv_ent_mesh (CEL_QUERY_PROPCLASS (
	    	inv_ent->GetPropertyClassList (), iPcMesh));
      if (inv_ent_mesh)
      {
	inv_ent_mesh->Show ();
	inv_ent_mesh->MoveMesh (pcmesh->GetMesh ()->GetMovable ()->
	      	GetSectors ()->Get (0), pcmesh->GetMesh ()->GetMovable ()->
		GetTransform ().GetOrigin ()+csVector3 (0, 1.3f, 0));
	if (inv_ent_gravity)
	{
	  inv_ent_gravity->ResetSpeed ();
	  inv_ent_gravity->ClearForces ();
	  float dx = 3 + 4*float ((rand () >> 3) % 10000) / 10000.;
	  if (((rand () >> 3) & 1) == 0) dx = -dx;
	  float dy = 3 + 4*float ((rand () >> 3) % 10000) / 10000.;
	  if (((rand () >> 3) & 1) == 0) dy = -dy;
	  printf ("%g,%g,%g\n", dx, 4.0, dy); fflush (stdout);
	  inv_ent_gravity->ApplyForce (csVector3 (dx, 4, dy), .5);
	}
      }
      pcinv->RemoveEntity (inv_ent);
    }
    if (pcinv->GetEntityCount () > 0)
    {
      // Restart timer.
      pctimer->WakeUp (200, false);
    }
  }

  return false;
}

//---------------------------------------------------------------------------

celBehaviourActor::celBehaviourActor (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
  bhroom = csPtr<celBehaviourRoom> (new celBehaviourRoom (entity, object_reg));
  fpscam=0;
  speed=1;
  id_input_mouseaxis0 = pl->FetchStringID ("cel.input.mouseaxis0");
  id_input_joyaxis0 = pl->FetchStringID ("cel.input.joyaxis0");
  id_input_joyaxis1 = pl->FetchStringID ("cel.input.joyaxis1");
  id_input_left_down = pl->FetchStringID ("cel.input.left.down");
  id_input_left_up = pl->FetchStringID ("cel.input.left.up");
  id_input_right_down = pl->FetchStringID ("cel.input.right.down");
  id_input_right_up = pl->FetchStringID ("cel.input.right.up");
  id_input_up_up = pl->FetchStringID ("cel.input.up.up");
  id_input_up_down = pl->FetchStringID ("cel.input.up.down");
  id_input_down_up = pl->FetchStringID ("cel.input.down.up");
  id_input_down_down = pl->FetchStringID ("cel.input.down.down");
  id_input_jump_up = pl->FetchStringID ("cel.input.jump.up");
  id_input_jump_down = pl->FetchStringID ("cel.input.jump.down");
  id_input_freeze_down = pl->FetchStringID ("cel.input.freeze.down");
  id_input_roll_down = pl->FetchStringID ("cel.input.roll.down");
  id_input_roll_up = pl->FetchStringID ("cel.input.roll.up");
  id_input_showstates_up = pl->FetchStringID ("cel.input.showstates.up");
  id_input_camleft_down = pl->FetchStringID ("cel.input.camleft.down");
  id_input_camleft_up = pl->FetchStringID ("cel.input.camleft.up");
  id_input_camright_down = pl->FetchStringID ("cel.input.camright.down");
  id_input_camright_up = pl->FetchStringID ("cel.input.camright.up");
  id_input_camdown_down = pl->FetchStringID ("cel.input.camdown.down");
  id_input_camdown_up = pl->FetchStringID ("cel.input.camdown.up");
  id_input_camup_up = pl->FetchStringID ("cel.input.camup.up");
  id_input_camup_down = pl->FetchStringID ("cel.input.camup.down");
  id_input_ready_down = pl->FetchStringID ("cel.input.ready.down");
  id_input_ready_up = pl->FetchStringID ("cel.input.ready.up");
  id_input_lockon_down = pl->FetchStringID ("cel.input.lockon.down");
  id_input_lockon_up = pl->FetchStringID ("cel.input.lockon.up");
  id_input_resetcam_down = pl->FetchStringID ("cel.input.resetcam.down");
  id_input_tiltcam = pl->FetchStringID ("cel.input.tiltcam");
  id_input_pancam = pl->FetchStringID ("cel.input.pancam");
  id_move_jump_landed = pl->FetchStringID ("cel.move.jump.landed");
  id_move_jump_started = pl->FetchStringID ("cel.move.jump.started");
  id_timer_wakeup = pl->FetchStringID ("cel.timer.wakeup");
  id_param_x = pl->FetchStringID ("cel.parameter.x");
  id_param_y = pl->FetchStringID ("cel.parameter.y");
  id_param_value = pl->FetchStringID ("cel.parameter.value");
}

celBehaviourActor::~celBehaviourActor()
{
}

bool celBehaviourActor::ReceiveMessage (csStringID msgid,
	iMessageSender* sender,
	celData& ret, iCelParameterBlock* params)
{
  csRef<iPcAnalogMotion> pcactor = celQueryPropertyClassEntity
    <iPcAnalogMotion> (entity);
  if (!pcactor)
    return false;
  csRef<iPcJump> jump = celQueryPropertyClassEntity<iPcJump> (entity);
  if (!jump)
    return false;
  csRef<iPcGrab> grab = celQueryPropertyClassEntity<iPcGrab> (entity);
  if (!grab)
    return false;
  csRef<iPcLinearMovement> linmove =
    celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  if (!pcactor && !linmove)
    return false;

  if (msgid == id_move_jump_landed)
  {
    puts ("The eagle has landed");
    grab->SetState (iPcGrab::DISABLED);
    return true;
  }
  else if (msgid == id_move_jump_started)
  {
    puts ("Doing a jump");
    return true;
  }
  else if (msgid == id_timer_wakeup)
  {
    puts ("cel.timer.wakeup WAKEUP! WAKEUP! WAKEY WAKEY!");
    // finished rolling
    pcactor->Enable (true);
    return true;
  }
  /*else if (!strcmp (msg_id, "cel.timer.wakeup.frame"))
  {
    char *baa;
    if (linmove->IsOnGround ())
      printf ("%c%c\n", baa[0], baa[1]);
    //puts (linmove->IsOnGround () ? "Ground" : "Air");
    return true;
  }*/
  else if (msgid == id_input_mouseaxis0)
  {
    CEL_FETCH_FLOAT_PAR (x, params, id_param_x);
    CEL_FETCH_FLOAT_PAR (y, params, id_param_y);
    csRef<iPcTrackingCamera> trackcam =
      celQueryPropertyClassEntity<iPcTrackingCamera> (entity);
    trackcam->SetPanDirection (-x * 200);
    trackcam->SetTiltDirection (-y * 200);
    return true;
  }
  else if (msgid == id_input_joyaxis0)
  {
    CEL_FETCH_FLOAT_PAR (value, params, id_param_value);
    pcactor->SetAxis (0, value);
    return true;
  }
  else if (msgid == id_input_joyaxis1)
  {
    CEL_FETCH_FLOAT_PAR (value, params, id_param_value);
    pcactor->SetAxis (1, -value);
    return true;
  }
  else if (msgid == id_input_left_down)
  {
    if (jump->GetState () == iPcJump::FROZEN)
    {
      grab->SetShimmyDirection (-1);
    }
    else
    {
      pcactor->AddAxis (0, -1);
      jump->GlideTurn (iPcJump::GLIDE_LEFT);
    }
    return true;
  }
  else if (msgid == id_input_left_up)
  {
    if (jump->GetState () == iPcJump::FROZEN)
    {
      grab->SetShimmyDirection (0);
    }
    else
    {
      pcactor->AddAxis (0, 1);
      jump->GlideTurn (iPcJump::GLIDE_NOTURN);
    }
    return true;
  }
  else if (msgid == id_input_right_down)
  {
    if (jump->GetState () == iPcJump::FROZEN)
    {
      grab->SetShimmyDirection (1);
    }
    else
    {
      pcactor->AddAxis (0, 1);
      jump->GlideTurn (iPcJump::GLIDE_RIGHT);
    }
    return true;
  }
  else if (msgid == id_input_right_up)
  {
    if (jump->GetState () == iPcJump::FROZEN)
    {
      grab->SetShimmyDirection (0);
    }
    else
    {
      pcactor->AddAxis (0, -1);
      jump->GlideTurn (iPcJump::GLIDE_NOTURN);
    }
    return true;
  }
  else if (msgid == id_input_up_down)
  {
    pcactor->AddAxis (1, 1);
    jump->GlidePitch (iPcJump::GLIDE_UP);
    return true;
  }
  else if (msgid == id_input_up_up)
  {
    pcactor->AddAxis (1, -1);
    jump->GlidePitch (iPcJump::GLIDE_NOPITCH);
    return true;
  }
  else if (msgid == id_input_down_down)
  {
    pcactor->AddAxis (1, -1);
    jump->GlidePitch (iPcJump::GLIDE_DOWN);
    return true;
  }
  else if (msgid == id_input_down_up)
  {
    pcactor->AddAxis (1, 1);
    jump->GlidePitch (iPcJump::GLIDE_NOPITCH);
    return true;
  }
  else if (msgid == id_input_jump_down)
  {
    switch (jump->GetState ())
    {
      case iPcJump::STAND:
        puts ("Jump: Stand");
        break;
      case iPcJump::JUMP:
        puts ("Jump: Jump");
        break;
      case iPcJump::DOUBLEJUMP:
        puts ("Jump: Double Jump");
        break;
      case iPcJump::GLIDE:
        puts ("Jump: Glide");
        break;
      case iPcJump::FROZEN:
        puts ("Jump: Frozen");
        break;
    }
    /*if (jump->GetState () == iPcJump::JUMP)
    {
      if (ABS (linmove->GetVelocity ().y) < 3.0)
      {
        linmove->SetGravity (3.0f);
        float glidespeed = linmove->GetVelocity ().z;
        if (glidespeed > -5)
          glidespeed = -5;
        linmove->SetVelocity (csVector3 (0, 0, glidespeed));
        pcactor->Enable (false);
      }
      else
        printf ("Downward velocity %f\n", linmove->GetVelocity ().y);
    }
    else
      puts ("Not jumping");*/
    grab->SetState (iPcGrab::SEARCHING);
    jump->Jump ();
    // perform a glide if mid air and near peak of the jump
    /*if (jump->IsJumping () && ABS (linmove->GetVelocity ().y) < 1.5f)
    {
      linmove->SetGravity (3.0f);
      float glidespeed = linmove->GetVelocity ().z;
      if (glidespeed > -5)
        glidespeed = -5;
      linmove->SetVelocity (csVector3 (0, 0, glidespeed));
      pcactor->Activate (false);
    }*/
    return true;
  }
  else if (msgid == id_input_freeze_down)
  {
    jump->Freeze (true);
    return true;
  }
  else if (msgid == id_input_roll_down)
  {
    if (jump->GetState () == iPcJump::FROZEN)
    {
      //jump->Enable (true);
      grab->SetState (iPcGrab::DISABLED);
      jump->Freeze (false);
    }
    else if (linmove->IsOnGround ())
    {
      // perform a roll
      if (!(pcactor->GetAxis () < EPSILON))
      {
        csRef<iPcTimer> timer = celQueryPropertyClassEntity<iPcTimer> (entity);
        timer->WakeUp (700, false);
        linmove->SetVelocity (csVector3 (0, 0, -pcactor->GetMovementSpeed ()));
        pcactor->Enable (false);
      }
      // do a crouch (target set downwards)
      else
      {
        csRef<iPcTrackingCamera> trackcam =
          celQueryPropertyClassEntity<iPcTrackingCamera> (entity);
        trackcam->SetTargetYOffset (0.5f);
      }
    }
    return true;
  }
  // end crouch action
  else if (msgid == id_input_roll_up)
  {
    if (pcactor->GetAxis () < EPSILON)
    {
      csRef<iPcTrackingCamera> trackcam =
        celQueryPropertyClassEntity<iPcTrackingCamera> (entity);
      trackcam->SetTargetYOffset (1.5f);
    }
    return true;
  }
  else if (msgid == id_input_showstates_up)
  {
    if (pcactor->IsEnabled ())
      puts ("Actor: Enabled");
    else
      puts ("Actor: Disabled");
    /*if (jump->IsEnabled ())
      puts ("Jump: Enabled");
    else
      puts ("Jump: Disabled");*/
    switch (jump->GetState ())
    {
      case iPcJump::STAND:
        puts ("Jump: Stand");
        break;
      case iPcJump::JUMP:
        puts ("Jump: Jump");
        break;
      case iPcJump::DOUBLEJUMP:
        puts ("Jump: Double Jump");
        break;
      case iPcJump::GLIDE:
        puts ("Jump: Glide");
        break;
      case iPcJump::FROZEN:
        puts ("Jump: Frozen");
        break;
    }
    return true;
  }

  csRef<iPcTrackingCamera> trackcam =
    celQueryPropertyClassEntity<iPcTrackingCamera> (entity);
  if (!trackcam)
    return false;

  if (msgid == id_input_camleft_down)
  {
    trackcam->SetPanDirection (-1);
    return true;
  }
  else if (msgid == id_input_camleft_up)
  {
    trackcam->SetPanDirection (0);
    return true;
  }
  else if (msgid == id_input_camright_down)
  {
    trackcam->SetPanDirection (1);
    return true;
  }
  else if (msgid == id_input_camright_up)
  {
    trackcam->SetPanDirection (0);
    return true;
  }
  else if (msgid == id_input_camup_down)
  {
    trackcam->SetTiltDirection (-1);
    return true;
  }
  else if (msgid == id_input_camup_up)
  {
    trackcam->SetTiltDirection (0);
    return true;
  }
  else if (msgid == id_input_camdown_down)
  {
    trackcam->SetTiltDirection (1);
    return true;
  }
  else if (msgid == id_input_camdown_up)
  {
    trackcam->SetTiltDirection (0);
    return true;
  }
  else if (msgid == id_input_ready_down)
  {
    trackcam->SetTargetState (iPcTrackingCamera::TARGET_NONE);
    return true;
  }
  else if (msgid == id_input_ready_up)
  {
    trackcam->SetTargetState (iPcTrackingCamera::TARGET_BASE);
    return true;
  }
  else if (msgid == id_input_lockon_down)
  {
    if (trackcam->GetTargetState () == iPcTrackingCamera::TARGET_NONE)
    {
      trackcam->SetTargetEntity ("dummy2b");
      trackcam->SetTargetState (iPcTrackingCamera::TARGET_OBJ);
    }
    return true;
  }
  else if (msgid == id_input_lockon_up)
  {
    if (trackcam->GetTargetState () == iPcTrackingCamera::TARGET_OBJ)
      trackcam->SetTargetState (iPcTrackingCamera::TARGET_NONE);
    return true;
  }
  else if (msgid == id_input_resetcam_down)
  {
    trackcam->ResetCamera ();
    return true;
  }
  else if (msgid == id_input_tiltcam)
  {
    CEL_FETCH_FLOAT_PAR (value, params, id_param_value);
    puts ("tilting camera (not implemented)");
    return true;
  }
  else if (msgid == id_input_pancam)
  {
    CEL_FETCH_FLOAT_PAR (value, params, id_param_value);
    if (value < -EPSILON)
      trackcam->SetPanDirection (-1);
    else if (value > EPSILON)
      trackcam->SetPanDirection (1);
    else
      trackcam->SetPanDirection (0);
    return true;
  }

  return bhroom->ReceiveMessage (msgid, sender, ret, params);
}

//---------------------------------------------------------------------------

celBehaviourActorCameraOld::celBehaviourActorCameraOld (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
  bhroom = csPtr<celBehaviourRoom> (new celBehaviourRoom (entity, object_reg));
  fpscam=0;
  speed=1;
  id_input_forward1 = pl->FetchStringID ("cel.input.forward.down");
  id_input_forward0 = pl->FetchStringID ("cel.input.forward.up");
  id_input_backward1 = pl->FetchStringID ("cel.input.backward.down");
  id_input_backward0 = pl->FetchStringID ("cel.input.backward.up");
  id_input_strafeleft1 = pl->FetchStringID ("cel.input.strafeleft.down");
  id_input_strafeleft0 = pl->FetchStringID ("cel.input.strafeleft.up");
  id_input_straferight1 = pl->FetchStringID ("cel.input.straferight.down");
  id_input_straferight0 = pl->FetchStringID ("cel.input.straferight.up");
  id_input_rotateleft1 = pl->FetchStringID ("cel.input.rotateleft.down");
  id_input_rotateleft0 = pl->FetchStringID ("cel.input.rotateleft.up");
  id_input_rotateright1 = pl->FetchStringID ("cel.input.rotateright.down");
  id_input_rotateright0 = pl->FetchStringID ("cel.input.rotateright.up");
  id_input_jump1 = pl->FetchStringID ("cel.input.jump.down");
  id_input_run1 = pl->FetchStringID ("cel.input.run.down");
  id_input_run0 = pl->FetchStringID ("cel.input.run.up");
  id_input_lookup1 = pl->FetchStringID ("cel.input.lookup.down");
  id_input_lookup0 = pl->FetchStringID ("cel.input.lookup.up");
  id_input_lookdown1 = pl->FetchStringID ("cel.input.lookdown.down");
  id_input_lookdown0 = pl->FetchStringID ("cel.input.lookdown.up");
  id_input_center1 = pl->FetchStringID ("cel.input.center.down");
  id_input_cammode1 = pl->FetchStringID ("cel.input.cammode.down");
}

celBehaviourActorCameraOld::~celBehaviourActorCameraOld()
{
}

bool celBehaviourActorCameraOld::ReceiveMessage (csStringID msgid,
	iMessageSender* sender,
	celData& ret, iCelParameterBlock* params)
{
  csRef<iPcActorMove> pcactormove =
    celQueryPropertyClassEntity<iPcActorMove> (entity);
  if (!pcactormove)
    return false;

  if (msgid == id_input_forward1)
    pcactormove->Forward (true);
  else if (msgid == id_input_forward0)
    pcactormove->Forward (false);
  else if (msgid == id_input_backward1)
    pcactormove->Backward (true);
  else if (msgid == id_input_backward0)
    pcactormove->Backward (false);
  else if (msgid == id_input_strafeleft1)
    pcactormove->StrafeLeft (true);
  else if (msgid == id_input_strafeleft0)
    pcactormove->StrafeLeft (false);
  else if (msgid == id_input_straferight1)
    pcactormove->StrafeRight (true);
  else if (msgid == id_input_straferight0)
    pcactormove->StrafeRight (false);
  else if (msgid == id_input_rotateleft1)
    pcactormove->RotateLeft (true);
  else if (msgid == id_input_rotateleft0)
    pcactormove->RotateLeft (false);
  else if (msgid == id_input_rotateright1)
    pcactormove->RotateRight (true);
  else if (msgid == id_input_rotateright0)
    pcactormove->RotateRight (false);
  else if (msgid == id_input_jump1)
    pcactormove->Jump ();
  else if (msgid == id_input_run1)
    pcactormove->Run (true);
  else if (msgid == id_input_run0)
    pcactormove->Run (false);
  else if (msgid == id_input_lookup1)
  {
    csRef<iPcDefaultCamera> pcdefcam =
      celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcam)
      pcdefcam->SetPitchVelocity (1.0f);
  }
  else if (msgid == id_input_lookup0)
  {
    csRef<iPcDefaultCamera> pcdefcam =
      celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcam)
      pcdefcam->SetPitchVelocity (0.0f);
  }
  else if (msgid == id_input_lookdown1)
  {
    csRef<iPcDefaultCamera> pcdefcam =
      celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcam)
      pcdefcam->SetPitchVelocity (-1.0f);
  }
  else if (msgid == id_input_lookdown0)
  {
    csRef<iPcDefaultCamera> pcdefcam =
      celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcam)
      pcdefcam->SetPitchVelocity (0.0f);
  }
  else if (msgid == id_input_center1)
  {
    csRef<iPcDefaultCamera> pcdefcam =
      celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcam)
      pcdefcam->CenterCamera ();
  }
  else if (msgid == id_input_cammode1)
  {
    pcactormove->ToggleCameraMode ();
    csRef<iPcDefaultCamera> pcdefcam =
      celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcam)
    {
      printf ("%s\n", pcdefcam->GetModeName ());
      fflush (stdout);
    }
  }
  else
    return bhroom->ReceiveMessage (msgid, sender, ret, params);

  return true;
}

//---------------------------------------------------------------------------

celBehaviourDynActor::celBehaviourDynActor (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
  id_input_forward_down = pl->FetchStringID ("cel.input.forward.down");
  id_input_backward_down = pl->FetchStringID ("cel.input.backward.down");
  id_input_strafeleft_down = pl->FetchStringID ("cel.input.strafeleft.down");
  id_input_straferight_down = pl->FetchStringID ("cel.input.straferight.down");
  id_input_jump_down = pl->FetchStringID ("cel.input.jump.down");
  id_input_lookup_down = pl->FetchStringID ("cel.input.lookup.down");
  id_input_lookup_up = pl->FetchStringID ("cel.input.lookup.up");
  id_input_lookdown_down = pl->FetchStringID ("cel.input.lookdown.down");
  id_input_lookdown_up = pl->FetchStringID ("cel.input.lookdown.up");
  id_input_center_down = pl->FetchStringID ("cel.input.center.down");
}

bool celBehaviourDynActor::ReceiveMessage (csStringID msgid,
	iMessageSender* sender,
	celData& ret, iCelParameterBlock* params)
{

  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity,
    iPcMechanicsObject);
  if (!pcmechobj)
    return false;

  if (msgid == id_input_forward_down)
  {
    pcmechobj->AddForceDuration (csVector3 (0, 0, -25.0f), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_backward_down)
  {
    pcmechobj->AddForceDuration (csVector3 (0, 0, 25.0f), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_strafeleft_down)
  {
    pcmechobj->AddForceDuration (csVector3 (25.0f, 0, 0), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_straferight_down)
  {
    pcmechobj->AddForceDuration (csVector3 (-25.0f, 0, 0), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_jump_down)
  {
    pcmechobj->AddForceDuration (csVector3 (0, 25.0f, 0), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_lookup_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (1.0f);
    return true;
  }
  else if (msgid == id_input_lookup_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (-1.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_center_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->CenterCamera ();
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------

celBehaviourWheeled::celBehaviourWheeled (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
  id_input_accelerate_up = pl->FetchStringID ("cel.input.accelerate.up");
  id_input_accelerate_down = pl->FetchStringID ("cel.input.accelerate.down");
  id_input_reverse_up = pl->FetchStringID ("cel.input.reverse.up");
  id_input_reverse_down = pl->FetchStringID ("cel.input.reverse.down");
  id_input_steerleft_up = pl->FetchStringID ("cel.input.steerleft.up");
  id_input_steerleft_down = pl->FetchStringID ("cel.input.steerleft.down");
  id_input_steerright_up = pl->FetchStringID ("cel.input.steerright.up");
  id_input_steerright_down = pl->FetchStringID ("cel.input.steerright.down");
  id_input_handbrake_up = pl->FetchStringID ("cel.input.handbrake.up");
  id_input_handbrake_down = pl->FetchStringID ("cel.input.handbrake.down");
  id_input_lookup_up = pl->FetchStringID ("cel.input.lookup.up");
  id_input_lookup_down = pl->FetchStringID ("cel.input.lookup.down");
  id_input_lookdown_up = pl->FetchStringID ("cel.input.lookdown.up");
  id_input_lookdown_down = pl->FetchStringID ("cel.input.lookdown.down");
  id_input_center_down = pl->FetchStringID ("cel.input.center.down");
  id_mech_collision = pl->FetchStringID ("cel.mechanics.collision");
  id_mech_par_position = pl->FetchStringID ("cel.parameter.position");
  id_mech_par_normal = pl->FetchStringID ("cel.parameter.normal");
  id_mech_par_depth = pl->FetchStringID ("cel.parameter.depth");
}

bool celBehaviourWheeled::ReceiveMessage (csStringID msgid,
	iMessageSender* sender,
	celData& ret, iCelParameterBlock* params)
{
  csRef<iPcWheeled> pcwheeled = CEL_QUERY_PROPCLASS_ENT (entity,
    iPcWheeled);
  if (!pcwheeled)
    return false;

  if (msgid == id_input_accelerate_down)
  {
    pcwheeled->Accelerate();
    return true;
  }
  //Autoreverse handles putting the car in reverse once it is slow enough.
  else if (msgid == id_input_reverse_down)
  {
    pcwheeled->Brake();
    return true;
  }
  else if (msgid == id_input_steerleft_down)
  {
    pcwheeled->SteerLeft();
    return true;
  }
  else if (msgid == id_input_steerright_down)
  {
    pcwheeled->SteerRight();
    return true;
  }
  else if (msgid == id_input_steerleft_up)
  {
    pcwheeled->SteerStraight();
    return true;
  }
  else if (msgid == id_input_steerright_up)
  {
    pcwheeled->SteerStraight();
    return true;
  }
  else if (msgid == id_input_handbrake_down)
  {
    pcwheeled->Handbrake(true);
    return true;
  }
  else if (msgid == id_input_handbrake_up)
  {
    pcwheeled->Handbrake(false);
    return true;
  }
  else if (msgid == id_input_accelerate_up)
  {
    pcwheeled->Accelerate(0.0f);
    return true;
  }
  else if (msgid == id_input_reverse_up)
  {
    pcwheeled->Brake(0.0f);
    return true;
  }
  else if (msgid == id_input_lookup_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (1.0f);
    return true;
  }
  else if (msgid == id_input_lookup_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (-1.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_center_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->CenterCamera ();
    return true;
  }
  else if (msgid == id_mech_collision)
  {
    CEL_FETCH_VECTOR3_PAR(pos, params, id_mech_par_position);
    CEL_FETCH_VECTOR3_PAR(norm, params, id_mech_par_normal);
    CEL_FETCH_FLOAT_PAR(depth, params, id_mech_par_depth);
    if (depth > 0.005f)
    {
      if(!pcmeshdeform)
        pcmeshdeform = CEL_QUERY_PROPCLASS_ENT (entity, iPcMeshDeform);
      if (pcmeshdeform)
        pcmeshdeform->DeformMesh(pos, norm * depth, true);
    }
    return true;
  }

  return false;
}

