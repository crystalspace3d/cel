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
#include "plugins/propclass/actoranalog/actoranalog.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

// CS Includes
#include "iengine/camera.h"
#include "iengine/engine.h"
#include "iutil/virtclk.h"
#include "csgeom/transfrm.h"

// CEL Includes
#include "propclass/mesh.h"
#include "propclass/linmove.h"
#include "propclass/newcamera.h"
#include "propclass/cameras/tracking.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (ActorAnalog, "pcmove.actor.analog")

//---------------------------------------------------------------------------

csStringID celPcActorAnalog::id_axis = csInvalidStringID;
csStringID celPcActorAnalog::id_value = csInvalidStringID;

PropertyHolder celPcActorAnalog::propinfo;

celPcActorAnalog::celPcActorAnalog (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  vc = csQueryRegistry<iVirtualClock> (object_reg);

  // For SendMessage parameters.
  if (id_axis == csInvalidStringID)
  {
    id_axis = pl->FetchStringID ("cel.parameter.axis");
    id_value = pl->FetchStringID ("cel.parameter.value");
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_setaxis, "cel.action.SetAxis");
    AddAction (action_addaxis, "cel.action.AddAxis");
    AddAction (action_setmovespeed, "cel.action.SetMovementSpeed");
    AddAction (action_setmoveaccel, "cel.action.SetMovementAcceleration");
    AddAction (action_setmoveaccel, "cel.action.SetMovementDeceleration");
    AddAction (action_setturnspeed, "cel.action.SetTurningSpeed");
  }

  // For properties.
  propinfo.SetCount (6);
  AddProperty (propid_axisx, "cel.property.axisx",
    CEL_DATA_FLOAT, false, "Left/Right axis value (-1.0 to 1.0).",
    &target_axis.x);
  AddProperty (propid_axisy, "cel.property.axisy",
    CEL_DATA_FLOAT, false, "Forward/Backwards axis value (-1.0 to 1.0).",
    &target_axis.y);
  AddProperty (propid_movespeed, "cel.property.movespeed",
    CEL_DATA_FLOAT, false, "Movement speed.",
    &movespeed);
  AddProperty (propid_moveaccel, "cel.property.moveaccel",
    CEL_DATA_FLOAT, false, "Movement acceleration.",
    &moveaccel);
  AddProperty (propid_movedecel, "cel.property.movedecel",
    CEL_DATA_FLOAT, false, "Movement deceleration.",
    &movedecel);
  AddProperty (propid_turnspeed, "cel.property.turnspeed",
    CEL_DATA_FLOAT, false, "Turning speed.",
    &turnspeed);

  // Tick every so often so we can update the state
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);

  target_axis.Set (0.0f);
  turnspeed = 30.0f;
  movespeed = 10;
  moveaccel = 50;
  movedecel = 100;
}

celPcActorAnalog::~celPcActorAnalog ()
{
}

const size_t actorlara_serial = 4;

csPtr<iCelDataBuffer> celPcActorAnalog::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (actorlara_serial);
  databuf->Add (target_axis.x);
  databuf->Add (target_axis.y);
  databuf->Add (turnspeed);
  databuf->Add (movespeed);
  databuf->Add (moveaccel);
  databuf->Add (movedecel);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcActorAnalog::Load (iCelDataBuffer* databuf)
{
  size_t serialnr = databuf->GetSerialNumber ();
  if (serialnr != actorlara_serial) return false;

  target_axis.x = databuf->GetFloat ();
  target_axis.y = databuf->GetFloat ();
  turnspeed = databuf->GetFloat ();
  movespeed = databuf->GetFloat ();
  moveaccel = databuf->GetFloat ();
  movedecel = databuf->GetFloat ();

  return true;
}

bool celPcActorAnalog::PerformActionIndexed (int idx,
  iCelParameterBlock* params,
  celData& ret)
{
  switch (idx)
  {
    case action_setaxis:
    {
      CEL_FETCH_LONG_PAR (axis, params, id_axis);
      CEL_FETCH_FLOAT_PAR (value, params, id_value);
      if (!p_axis || !p_value)
        return false;
      SetAxis (axis, value);
      return true;
    }
    case action_addaxis:
    {
      CEL_FETCH_LONG_PAR (axis, params, id_axis);
      CEL_FETCH_FLOAT_PAR (value, params, id_value);
      if (!p_axis || !p_value)
        return false;
      AddAxis (axis, value);
      return true;
    }
    case action_setmovespeed:
    {
      CEL_FETCH_FLOAT_PAR (value, params, id_value);
      if (!p_value)
        return false;
      SetMovementSpeed (value);
      return true;
    }
    case action_setmoveaccel:
    {
      CEL_FETCH_FLOAT_PAR (value, params, id_value);
      if (!p_value)
        return false;
      SetMovementAcceleration (value);
      return true;
    }
    case action_setmovedecel:
    {
      CEL_FETCH_FLOAT_PAR (value, params, id_value);
      if (!p_value)
        return false;
      SetMovementDeceleration (value);
      return true;
    }
    case action_setturnspeed:
    {
      CEL_FETCH_FLOAT_PAR (value, params, id_value);
      if (!p_value)
        return false;
      SetTurningSpeed (value);
      return true;
    }
    default:
      return false;
  }
  return false;
}

void celPcActorAnalog::SetAxis (size_t axis, float value)
{
  // make sure value is in desired range of [-1,1]
  if (value < -1.0f)
    value = -1.0f;
  else if (value > 1.0f)
    value = 1.0f;

  // if axis is valid, set the axis otherwise set both to value
  if (axis < 2)
    target_axis[axis] = value;
  else
    target_axis.Set (value);
  // keep the movement synced
  UpdateMovement ();
}
float celPcActorAnalog::GetAxis (size_t axis) const
{
  if (axis != 0 && axis != 1)
    return 0.0f;
  return target_axis[axis];
}
void celPcActorAnalog::AddAxis (size_t axis, float value)
{
  SetAxis (axis, target_axis[axis] + value);
}
void celPcActorAnalog::SetMovementSpeed (float speed)
{
  movespeed = speed;
}
float celPcActorAnalog::GetMovementSpeed () const
{
  return movespeed;
}
void celPcActorAnalog::SetMovementAcceleration (float accel)
{
  moveaccel = accel;
}
float celPcActorAnalog::GetMovementAcceleration () const
{
  return moveaccel;
}
void celPcActorAnalog::SetMovementDeceleration (float decel)
{
  movedecel = decel;
}
float celPcActorAnalog::GetMovementDeceleration () const
{
  return movedecel;
}
void celPcActorAnalog::SetTurningSpeed (float speed)
{
  turnspeed = speed;
}
float celPcActorAnalog::GetTurningSpeed () const
{
  return turnspeed;
}

void celPcActorAnalog::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
    camera = celQueryPropertyClassEntity<iPcCamera> (entity);
  }
}

void celPcActorAnalog::TickEveryFrame ()
{
  UpdateMovement ();
}

void celPcActorAnalog::UpdateMovement ()
{
  FindSiblingPropertyClasses ();
  // check if we're missing any needed property classes
  if (!pclinmove || !pcmesh || !camera)
    return;

  csVector2 curr_axis (target_axis);
  // if we're not moving then stop and idle
  if (curr_axis < EPSILON)
  {
    // ... deccelerate the player down
    float elapsedsecs = vc->GetElapsedTicks () / 1000.0f;
    float newspeed = pclinmove->GetVelocity ().Norm () - 2 * moveaccel * elapsedsecs;
    if (newspeed < EPSILON)
      newspeed = 0.0f;
    pclinmove->SetVelocity (csVector3 (0, 0, -newspeed));
    pcmesh->SetAnimation ("stand", true);
    return;
  }

  // Get the transform of the current camera
  const csMatrix3 &cammat = camera->GetCamera ()->GetTransform ().GetT2O ();
  // We use the transformation to calculate the axis relative to the camera
  const csVector3 camvec = cammat * -csVector3 (curr_axis.x, 0, curr_axis.y);
  // And we calculate using atan2, the rotation to face that axis relative
  // to the camera
  csVector3 rotvel (0, -turnspeed, 0),
    target (0, atan2 (camvec.x, camvec.z), 0);
  // To see which way to rotate we get our rotation
  const float yrot = pclinmove->GetYRotation ();
  // This is quicker than using atan2... Just make sure that
  // target is higher than yrot before calculating the delta
  while (target.y < yrot)
    target.y += 2.0f * PI;
  // we calculate the delta, and if its greater than half a circle
  // we are obviously turning in the wrong direction
  float delta_rot = target.y - yrot;
  if (delta_rot > PI)
    rotvel.y *= -1;
  pclinmove->SetAngularVelocity (rotvel, target);

  // Now minimise the angle to get the absolute rotation around 0
  while (delta_rot > PI)
    delta_rot -= 2.0f * PI;
  // This is to stop the character moving while they're rotating to
  // face the direction they're turning towards.
  if (ABS(delta_rot) < 0.1)
  {
    // because joystick gives the movement in ranges [0,1] on both axis
    // setting movement of [1,1] will make the player go faster than with [1,0]
    // so we project the square onto a circle to fix this
    float grad = curr_axis.y / curr_axis.x;
    // calculate bisected point on square first
    csVector2 bisect_point;
    // which side of the square do we calculate a bisection on?
    if (-curr_axis.x < curr_axis.y)
    {
      // bisect x = 1
      if (curr_axis.x > curr_axis.y)
        bisect_point.Set (1, grad);
      // bisect y = 1
      else // if (curr_axis.x < curr_axis.y)
        bisect_point.Set (1/grad, 1);
    }
    else
    {
      // bisect x = -1
      if (curr_axis.x < curr_axis.y)
        bisect_point.Set (-1, -grad);
      // bisect y = -1
      else // if (curr_axis.x > curr_axis.y)
        bisect_point.Set (-1/grad, -1);
    }
    float i = curr_axis.Norm () / bisect_point.Norm ();
    //csVector2 movecir = i * (curr_axis / curr_axis.Norm ());
    // move forwards
    // if haven't reached destination movement speed yet...
    if (pclinmove->GetVelocity ().SquaredNorm () < movespeed * movespeed)
    {
      // ... then accelerate the player forward
      float elapsedsecs = vc->GetElapsedTicks () / 1000.0f;
      // not real, but then again forget realism over responsiveness
      float newspeed = moveaccel * i * elapsedsecs + pclinmove->GetVelocity ().Norm ();
      // clipping in case acceleration oversteps the target speed.
      if (newspeed > movespeed * i)
        newspeed = movespeed * i;
      pclinmove->SetVelocity (csVector3 (0, 0, -newspeed));
    }
    pcmesh->SetAnimation ("walk", true);
  }
  else
  {
    // we could also slow down forward velocity gradually ...
    // ... more conditionals :)
    pcmesh->SetAnimation ("turning", true);
  }
}
