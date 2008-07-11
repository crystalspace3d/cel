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
#include "plugins/propclass/jump/jump.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "propclass/mesh.h"

// CEL Includes
#include "propclass/analogmotion.h"
#include "propclass/linmove.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Jump, "pcmove.jump")

//---------------------------------------------------------------------------

PropertyHolder celPcJump::propinfo;

celPcJump::celPcJump (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  propholder = &propinfo;
  // For states.
  if (!propinfo.actions_done)
  {
    AddAction (action_jump, "cel.state.Jump");
  }

  // For properties.
  propinfo.SetCount (0);

  currstate = STAND;
  startact = STAND;
  jumpspeed = 10.0f;
  doublejumpspeed = 0.0f;
  gravity = 25.0f;
  fixedjump = true;
}

celPcJump::~celPcJump ()
{
}

#define TEST_SERIAL 0

csPtr<iCelDataBuffer> celPcJump::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcJump::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL) return false;
  return true;
}

bool celPcJump::PerformActionIndexed (int idx, iCelParameterBlock* params, celData& ret)
{
  switch (idx)
  {
    case action_jump:
    {
      Jump ();
      return true;
    }
    default:
      return false;
  }
  return false;
}

//---------------------------------------------------------------------------

void celPcJump::Jump ()
{
  if (!FindSiblingPropertyClasses ())
    return;
  if (currstate == STAND)
    DoJump ();
  else if (currstate == JUMP)
  {
    if (falling)
      return;
    if (doublejumpspeed < EPSILON)
      //Glide ();
      startact = GLIDE;
    else
      //DoDoubleJump ();
      startact = DOUBLEJUMP;
  }
  else if (currstate == DOUBLEJUMP)
  {
    if (falling)
      return;
    startact = GLIDE;
  }
}
void celPcJump::Freeze (bool frozen)
{
  if (frozen)
  {
    currstate = FROZEN;
    linmove->ClearWorldVelocity ();
    linmove->SetBodyVelocity (csVector3 (0));
    linmove->SetGravity (0.0f);
  }
  else
  {
    currstate = JUMP;
    linmove->SetGravity (gravity);
  }
}
celPcJump::State celPcJump::GetState () const
{
  return currstate;
}

void celPcJump::GlideTurn (GlideTurnDirection gtur)
{
  g_turn = gtur;
}
void celPcJump::GlidePitch (GlidePitchDirection gpit)
{
  g_pitch = gpit;
}

void celPcJump::SetJumpSpeed (float spd)
{
  jumpspeed = spd;
}
float celPcJump::GetJumpSpeed () const
{
  return jumpspeed;
}
void celPcJump::SetJumpHeight (float height)
{
  // get current time to reach current maximum of the jump
  // assume velocity at maximum = 0, then S = 0.5 * g * t^2
  float thalf = sqrt (2.0f * height / gravity);
  // we reuse this time
  jumpspeed = height / thalf + 0.5f * gravity * thalf;
}
float celPcJump::GetJumpHeight () const
{
  // v = u - gt;  t = u / g
  // S = ut - 0.5 * gt^2
  //   = 0.5 * u * u / g
  return 0.5f * jumpspeed * jumpspeed / gravity;
}
csTicks celPcJump::GetAirTime () const
{
  return 2 * 1000.0f * jumpspeed / gravity;
}
void celPcJump::SetDoubleJumpSpeed (float spd)
{
  doublejumpspeed = spd;
}
float celPcJump::GetDoubleJumpSpeed () const
{
  return doublejumpspeed;
}
void celPcJump::SetGravity (float grav)
{
  gravity = grav;
  if (currstate != FROZEN && FindSiblingPropertyClasses ())
    linmove->SetGravity (gravity);
}
float celPcJump::GetGravity () const
{
  return gravity;
}
void celPcJump::SetFixedJump (bool fixjump)
{
  fixedjump = fixjump;
}
bool celPcJump::GetFixedJump () const
{
  return fixedjump;
}

bool celPcJump::ReceiveMessage (csStringID msg_id, iMessageSender *sender, celData &ret, iCelParameterBlock *params)
{
  if (!FindSiblingPropertyClasses ())
    return false;
  if (currstate != STAND && currstate != FROZEN)
  {
    csRef<iPcAnalogMotion> motion = celQueryPropertyClassEntity<iPcAnalogMotion> (entity);
    if (motion)
      motion->Enable (false);
    linmove->SetGravity (gravity);
    linmove->SetBodyVelocity (csVector3 (0));
  }
  return true;
}

bool celPcJump::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    linmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
    linmove->SetGravity (gravity);  // our gravity is the only gravity
  }
  return linmove;
}

void celPcJump::TickEveryFrame ()
{
  UpdateMovement ();
}

void celPcJump::UpdateMovement ()
{
  if (!FindSiblingPropertyClasses ())
    return;
  if (linmove->GetVelocity ().y < 0)
    falling = true;
  if ((startact == DOUBLEJUMP || startact == GLIDE) && ABS (linmove->GetVelocity ().y) < 0.1f)
  {
    // now we can validly perform the doublejump/glide :)
    currstate = startact;
    startact = STAND;
    if (currstate == DOUBLEJUMP)
    {
      linmove->ClearWorldVelocity ();
      linmove->AddVelocity (csVector3 (0, doublejumpspeed, 0));
    }
    else if (currstate == GLIDE)
    {
      linmove->SetGravity (0.2f);
      float glidespeed = linmove->GetVelocity ().z;
      if (glidespeed > -5)
        glidespeed = -5;
      linmove->SetVelocity (csVector3 (0, 0, glidespeed));
      csRef<iPcAnalogMotion> motion = celQueryPropertyClassEntity<iPcAnalogMotion> (entity);
      if (motion)
        motion->Enable (false);
    }
  }

  if (currstate == GLIDE)
    GlideControl ();  // :)

  // check if we landed from our jump
  if (linmove->IsOnGround () && falling)
  {
    if (currstate == GLIDE)
    {
      // ... look we need to correct x roll
      csRef<iPcMesh> mesh = celQueryPropertyClassEntity<iPcMesh> (entity);
      iMovable* movable = mesh->GetMesh ()->GetMovable ();
      csReversibleTransform trans (movable->GetTransform ());
      //movable->SetTransform (csXRotMatrix3 (0) * movable->GetTransform ());
      csVector3 camvec = trans.GetT2O () * csVector3 (0, 0, 1);
      camvec.y = 0;
      trans.LookAt (camvec, csVector3 (0, 1, 0));
      movable->SetTransform (trans);
    }
    currstate = STAND;
    pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);

    linmove->SetGravity (gravity);
    csRef<iPcAnalogMotion> motion = celQueryPropertyClassEntity<iPcAnalogMotion> (entity);
    if (motion)
      motion->Enable (true);

    if (!dispatcher.landed)
    {
      dispatcher.landed = entity->QueryMessageChannel ()->
        CreateMessageDispatcher (this, "cel.move.jump.landed");
      if (!dispatcher.landed)
        return;
    }
    dispatcher.landed->SendMessage (0);
  }
}

void celPcJump::DoJump ()
{
  // cannot jump from mid-air
  if (!linmove->IsOnGround ())
    return;
  // we need the falling variable otherwise we sometimes detect a landed before
  // leaving the ground
  falling = false;
  currstate = JUMP;
  // fixed length style jump
  csRef<iPcAnalogMotion> motion = celQueryPropertyClassEntity<iPcAnalogMotion> (entity);
  if (motion && fixedjump)
  {
    // deactivate moving and set a fixed length forward speed
    if (motion->GetAxis ().SquaredNorm () > EPSILON)
      linmove->SetBodyVelocity (csVector3 (0, 0, -motion->GetMovementSpeed ()));
    motion->Enable (false);
  }
  // actually perform the jump if we're on the ground
  linmove->AddVelocity (csVector3 (0, jumpspeed, 0));
  // we use a cheat... this should skip this current frame
  // and only callback once we have left the ground.
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  // in case we get stuck
  // ... won't resubscribe if already subscribed
  entity->QueryMessageChannel ()->Subscribe (this, "cel.move.impossible");

  // send a signal that the jump was successfully executed
  if (!dispatcher.started)
  {
    dispatcher.started = entity->QueryMessageChannel ()->
      CreateMessageDispatcher (this, "cel.move.jump.started");
    if (!dispatcher.started)
      return;
  }
  dispatcher.started->SendMessage (0);
}

void celPcJump::GlideControl ()
{
  csVector3 angvel (0);
  if (g_turn == GLIDE_LEFT)
    angvel.y = 2;
  else if (g_turn == GLIDE_RIGHT)
    angvel.y = -2;
  if (g_pitch == GLIDE_UP)
    angvel.x = -2;
  else if (g_pitch == GLIDE_DOWN)
    angvel.x = 2;
  linmove->SetAngularVelocity (angvel);
}
