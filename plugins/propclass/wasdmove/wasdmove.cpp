/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein

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
#include "plugins/propclass/wasdmove/wasdmove.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (WasdMove, "pcmove.actor.wasd")

//---------------------------------------------------------------------------

csStringID celPcWasdMove::id_input_mouseaxis0 = csInvalidStringID;
csStringID celPcWasdMove::id_input_joyaxis0 = csInvalidStringID;
csStringID celPcWasdMove::id_input_joyaxis1 = csInvalidStringID;
csStringID celPcWasdMove::id_input_left_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_left_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_right_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_right_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_up_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_up_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_down_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_down_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_jump_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_jump_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_freeze_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_roll_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_roll_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_showstates_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_camleft_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_camleft_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_camright_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_camright_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_camdown_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_camdown_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_camup_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_camup_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_ready_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_ready_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_lockon_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_lockon_up = csInvalidStringID;
csStringID celPcWasdMove::id_input_resetcam_down = csInvalidStringID;
csStringID celPcWasdMove::id_input_tiltcam = csInvalidStringID;
csStringID celPcWasdMove::id_input_pancam = csInvalidStringID;

csStringID celPcWasdMove::id_move_jump_landed = csInvalidStringID;
csStringID celPcWasdMove::id_move_jump_started = csInvalidStringID;

csStringID celPcWasdMove::id_param_x = csInvalidStringID;
csStringID celPcWasdMove::id_param_y = csInvalidStringID;
csStringID celPcWasdMove::id_param_value = csInvalidStringID;

csStringID celPcWasdMove::id_timer_wakeup = csInvalidStringID;

celPcWasdMove::celPcWasdMove (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_input_mouseaxis0 == csInvalidStringID)
  {
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

    id_param_x = pl->FetchStringID ("x");
    id_param_y = pl->FetchStringID ("y");
    id_param_value = pl->FetchStringID ("value");

    id_timer_wakeup = pl->FetchStringID ("cel.timer.wakeup");
  }

  try_grab_jump = true;
}

celPcWasdMove::~celPcWasdMove ()
{
}

void celPcWasdMove::SetEntity (iCelEntity* entity)
{
  celPcCommon::SetEntity (entity);
  if (entity)
    entity->QueryMessageChannel ()->Subscribe (this, "cel.");
}

bool celPcWasdMove::SetPropertyIndexed (int idx, long b)
{
  return false;
}

bool celPcWasdMove::GetPropertyIndexed (int idx, long& l)
{
  return false;
}

bool celPcWasdMove::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  return false;
}

void celPcWasdMove::GetPCS ()
{
  if (!trackcam) trackcam = celQueryPropertyClassEntity<iPcTrackingCamera> (entity);
  if (!pcactor) pcactor = celQueryPropertyClassEntity<iPcAnalogMotion> (entity);
  if (!linmove) linmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  if (!timer) timer = celQueryPropertyClassEntity<iPcTimer> (entity);

  if (HavePropertyClassesChanged ())
    try_grab_jump = true;
  if (try_grab_jump)
  {
    if (!jump) jump = celQueryPropertyClassEntity<iPcJump> (entity);
    if (!grab) grab = celQueryPropertyClassEntity<iPcGrab> (entity);
    try_grab_jump = false;
  }
}

bool celPcWasdMove::ReceiveMessage (csStringID msgid, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  if (celPcCommon::ReceiveMessage (msgid, sender, ret, params))
    return true;

  GetPCS ();

  if (msgid == id_move_jump_landed)
  {
    grab->SetState (iPcGrab::DISABLED);
    return true;
  }
  else if (msgid == id_move_jump_started)
  {
    return true;
  }
  else if (msgid == id_timer_wakeup)
  {
    // finished rolling
    pcactor->Enable (true);
    return true;
  }
  else if (msgid == id_input_mouseaxis0)
  {
    float x, y;
    if (!Fetch (x, params, id_param_x, true, 0.0f)) return false;
    if (!Fetch (y, params, id_param_y, true, 0.0f)) return false;
    trackcam->SetPanDirection (-x * 200);	//@@@Config
    trackcam->SetTiltDirection (-y * 200);	//@@@Config
    return true;
  }
  else if (msgid == id_input_joyaxis0)
  {
    float value;
    if (!Fetch (value, params, id_param_value, true, 0.0f)) return false;
    pcactor->SetAxis (0, value);
    return true;
  }
  else if (msgid == id_input_joyaxis1)
  {
    float value;
    if (!Fetch (value, params, id_param_value, true, 0.0f)) return false;
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
        linmove->SetBodyVelocity (csVector3 (0, 0, glidespeed));
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
      linmove->SetBodyVelocity (csVector3 (0, 0, glidespeed));
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
        timer->WakeUp (700, false);
        linmove->SetBodyVelocity (csVector3 (0, 0, -pcactor->GetMovementSpeed ()));
        pcactor->Enable (false);
      }
      // do a crouch (target set downwards)
      else
      {
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
  else if (msgid == id_input_camleft_down)
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
    float value;
    if (!Fetch (value, params, id_param_value, true, 0.0f)) return false;
    puts ("tilting camera (not implemented)");
    return true;
  }
  else if (msgid == id_input_pancam)
  {
    float value;
    if (!Fetch (value, params, id_param_value, true, 0.0f)) return false;
    if (value < -EPSILON)
      trackcam->SetPanDirection (-1);
    else if (value > EPSILON)
      trackcam->SetPanDirection (1);
    else
      trackcam->SetPanDirection (0);
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------

