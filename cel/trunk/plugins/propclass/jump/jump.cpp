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
    if (currstate == GLIDE)
      linmove->SetGravity (0.2f);
    else
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
      // sort this out...
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

static float AngleBetweenVectors (const csVector3 &a, const csVector3 &b)
{
  return acos ((a * b) / (a.Norm () * b.Norm ()));
}
inline static bool IsEqual (float a, float b)
{
  return ABS (a - b) < EPSILON;
}

void celPcJump::GlideControl ()
{
  const float glide_pitch_limit = PI * 40.0 / 360.0,
    glide_speed_limit = 8.0,    // Faster you can go the higher you can fly
    glide_speed_pitch = 2.5,    // how fast you can change your up dwn pitch
    glide_speed_fall_accel = 0.009, // How fast we drop when gliding
    glide_speed_fall = -7.0,    // -10 makes you fly!
    glide_speed_fall_time_fac = 0.05,  // 2.66 # increase GLIDE_SPEED_FALL with the jump speed over time.
    glide_accel = 0.65,         // WATCH THIS ONE!
    glide_ease_in_time = 0.5,   // Ease in from current velocity
    time_offset = 1000.0;

  csRef<iPcMesh> mesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  iMovable* movable = mesh->GetMesh ()->GetMovable ();

  // get local up vector
  csVector3 own_y = movable->GetTransform ().This2OtherRelative (csVector3 (0, 1, 0));
  //float pitch_angle = AngleBetweenVectors (own_y, csVector3 (0, 1, 0));
  csVector3 upasloc = movable->GetTransform ().Other2ThisRelative (csVector3 (0, 1, 0));
  float pitch_angle = AngleBetweenVectors (csVector3 (0, 1, 0), upasloc);
  if (pitch_angle > glide_pitch_limit)
    pitch_angle = glide_pitch_limit;

  float pitch_rotate = 0;
  if (g_pitch == GLIDE_UP && (!IsEqual (pitch_angle, glide_pitch_limit) || upasloc.z < 0))
  {
    pitch_rotate = -glide_speed_pitch;
  }
  else if (g_pitch == GLIDE_DOWN && (!IsEqual (pitch_angle, glide_pitch_limit) || upasloc.z > 0))
  {
    pitch_rotate = glide_speed_pitch;
  }
  else if (g_pitch == GLIDE_NOPITCH)
  {
    pitch_rotate = glide_speed_pitch * pitch_angle / glide_pitch_limit;
    if (pitch_rotate > glide_speed_pitch)
      pitch_rotate = glide_speed_pitch;
    if (upasloc.z < 0)
      pitch_rotate = -pitch_rotate;
  }

  csMatrix3 own_mat (movable->GetTransform ().GetT2O ());
  csVector3 own_pitch_axis (own_mat * csVector3 (1, 0, 0));
  own_pitch_axis.z = 0.0;   // no roll

  float speed;
  // are we gliding up or down??
  if (upasloc.z < 0)
  {
    speed = (glide_pitch_limit - pitch_angle) / (2.0 * glide_pitch_limit);
  }
  else
  {
    speed = (glide_pitch_limit + pitch_angle) / (2.0 * glide_pitch_limit);
    //pitch_angle = -pitch_angle;
  }
  // Speed is now between 0.0 and 1.0 based on the angle
  float speed_inv = 1 - speed;

  csXRotMatrix3 pitch_mat (pitch_angle);
  own_mat = own_mat * pitch_mat;
  //movable->SetTransform (own_mat);
  //movable->UpdateMove ();

  csVector3 angvel (0);
  if (g_turn == GLIDE_LEFT)
    angvel.y = 2;
  else if (g_turn == GLIDE_RIGHT)
    angvel.y = -2;
  angvel.x = pitch_rotate;
  linmove->SetAngularVelocity (angvel);
  linmove->SetBodyVelocity (csVector3 (0, 0, -speed * 6));
}
