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
#include "plugins/propclass/analogmotion/analogmotion.h"
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

CEL_IMPLEMENT_FACTORY_ALT (AnalogMotion, "pcmove.analogmotion", "pcmove.actor.analog")

//---------------------------------------------------------------------------

csStringID celPcAnalogMotion::id_axis = csInvalidStringID;
csStringID celPcAnalogMotion::id_value = csInvalidStringID;
csStringID celPcAnalogMotion::id_enabled = csInvalidStringID;

PropertyHolder celPcAnalogMotion::propinfo;

celPcAnalogMotion::celPcAnalogMotion (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  vc = csQueryRegistry<iVirtualClock> (object_reg);

  // For SendMessage parameters.
  if (id_axis == csInvalidStringID)
  {
    id_axis = pl->FetchStringID ("axis");
    id_value = pl->FetchStringID ("value");
    id_enabled = pl->FetchStringID ("enabled");
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_setaxis, "SetAxis");
    AddAction (action_addaxis, "AddAxis");
    AddAction (action_setmovespeed, "SetMovementSpeed");
    AddAction (action_setmoveaccel, "SetMovementAcceleration");
    AddAction (action_setmoveaccel, "SetMovementDeceleration");
    AddAction (action_setminturnspeed, "SetMinimumTurningSpeed");
    AddAction (action_setmaxturnspeed, "SetMaximumTurningSpeed");
    AddAction (action_enable, "Enable");
  }

  // For properties.
  propinfo.SetCount (8);
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
  AddProperty (propid_minturnspeed, "cel.property.minturnspeed",
    CEL_DATA_FLOAT, false, "Minimum turning speed.",
    &minturnspeed);
  AddProperty (propid_maxturnspeed, "cel.property.maxturnspeed",
    CEL_DATA_FLOAT, false, "Maximum turning speed.",
    &maxturnspeed);
  AddProperty (propid_enabled, "cel.property.enabled",
    CEL_DATA_BOOL, false, "Is this component updating the player every frame?",
    &enabled);

  // Tick every so often so we can update the state
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);

  target_axis.Set (0.0f);
  minturnspeed = 3.0f;
  maxturnspeed = 15.0f;
  movespeed = 10;
  moveaccel = 50;
  movedecel = 40;
  enabled = true;
}

celPcAnalogMotion::~celPcAnalogMotion ()
{
}

const size_t actorlara_serial = 8;

csPtr<iCelDataBuffer> celPcAnalogMotion::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (actorlara_serial);
  databuf->Add (target_axis.x);
  databuf->Add (target_axis.y);
  databuf->Add (minturnspeed);
  databuf->Add (maxturnspeed);
  databuf->Add (movespeed);
  databuf->Add (moveaccel);
  databuf->Add (movedecel);
  databuf->Add (enabled);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcAnalogMotion::Load (iCelDataBuffer* databuf)
{
  size_t serialnr = databuf->GetSerialNumber ();
  if (serialnr != actorlara_serial)
    return false;
  target_axis.x = databuf->GetFloat ();
  target_axis.y = databuf->GetFloat ();
  minturnspeed = databuf->GetFloat ();
  maxturnspeed = databuf->GetFloat ();
  movespeed = databuf->GetFloat ();
  moveaccel = databuf->GetFloat ();
  movedecel = databuf->GetFloat ();
  enabled = databuf->GetBool ();
  return true;
}

bool celPcAnalogMotion::PerformActionIndexed (int idx,
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
    case action_setminturnspeed:
    {
      CEL_FETCH_FLOAT_PAR (value, params, id_value);
      if (!p_value)
        return false;
      SetMinimumTurningSpeed (value);
      return true;
    }
    case action_setmaxturnspeed:
    {
      CEL_FETCH_FLOAT_PAR (value, params, id_value);
      if (!p_value)
        return false;
      SetMaximumTurningSpeed (value);
      return true;
    }
    case action_enable:
    {
      CEL_FETCH_BOOL_PAR (value, params, id_enabled);
      if (!p_value)
        value = true;
      enabled = value;
      return true;
    }
    default:
      return false;
  }
  return false;
}

void celPcAnalogMotion::SetAxis (size_t axis, float value)
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
float celPcAnalogMotion::GetAxis (size_t axis) const
{
  if (axis != 0 && axis != 1)
    return 0.0f;
  return target_axis[axis];
}
void celPcAnalogMotion::SetAxis (const csVector2 &axis)
{
  target_axis = axis;
  // keep the movement synced
  UpdateMovement ();
}
const csVector2 &celPcAnalogMotion::GetAxis () const
{
  return target_axis;
}
void celPcAnalogMotion::AddAxis (size_t axis, float value)
{
  SetAxis (axis, target_axis[axis] + value);
}
void celPcAnalogMotion::SetMovementSpeed (float speed)
{
  movespeed = speed;
}
float celPcAnalogMotion::GetMovementSpeed () const
{
  return movespeed;
}
void celPcAnalogMotion::SetMovementAcceleration (float accel)
{
  moveaccel = accel;
}
float celPcAnalogMotion::GetMovementAcceleration () const
{
  return moveaccel;
}
void celPcAnalogMotion::SetMovementDeceleration (float decel)
{
  movedecel = decel;
}
float celPcAnalogMotion::GetMovementDeceleration () const
{
  return movedecel;
}
void celPcAnalogMotion::SetTurningSpeed (float speed)
{
  maxturnspeed = minturnspeed = speed;
}
float celPcAnalogMotion::GetTurningSpeed () const
{
  return minturnspeed;
}
void celPcAnalogMotion::SetMinimumTurningSpeed (float speed)
{
  minturnspeed = speed;
}
float celPcAnalogMotion::GetMinimumTurningSpeed () const
{
  return minturnspeed;
}
void celPcAnalogMotion::SetMaximumTurningSpeed (float speed)
{
  maxturnspeed = speed;
}
float celPcAnalogMotion::GetMaximumTurningSpeed () const
{
  return maxturnspeed;
}

void celPcAnalogMotion::Enable (bool en)
{
  enabled = en;
}
bool celPcAnalogMotion::IsEnabled () const
{
  return enabled;
}

bool celPcAnalogMotion::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
    camera = celQueryPropertyClassEntity<iPcCamera> (entity);
  }
  // check if we're missing any needed property classes
  return pclinmove && pcmesh && camera;
}

void celPcAnalogMotion::TickEveryFrame ()
{
  UpdateMovement ();
}

void celPcAnalogMotion::UpdateMovement ()
{
  if (!enabled || !FindSiblingPropertyClasses ())
    return;

  csVector2 curr_axis (target_axis);
  // if we're not moving then stop and idle
  if (curr_axis < EPSILON)
  {
    /// @@@ (GE) deceleration should be relative to camera
    // ... deccelerate the player down
    float elapsedsecs = vc->GetElapsedTicks () / 1000.0f;
    float newspeed = -pclinmove->GetBodyVelocity ().z - 2 * movedecel * elapsedsecs;
    if (newspeed < EPSILON)
      newspeed = 0.0f;
    // we only modify the z component really
    csVector3 cvel (pclinmove->GetBodyVelocity ());
    cvel.z = -newspeed;
    pclinmove->SetVelocity (cvel);
    pclinmove->SetAngularVelocity (csVector3 (0));
    pcmesh->SetAnimation ("stand", true);
    return;
  }

  // Get the transform of the current camera
  const csMatrix3 &cammat = camera->GetCamera ()->GetTransform ().GetT2O ();
  // We use the transformation to calculate the axis relative to the camera
  const csVector3 camvec = cammat * -csVector3 (curr_axis.x, 0, curr_axis.y);
  // And we calculate using atan2, the rotation to face that axis relative
  // to the camera
  csVector3 rotvel (0, -1, 0),
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
  // Now minimise the angle to get the absolute rotation around 0
  while (delta_rot > PI)
    delta_rot -= 2.0f * PI;
  delta_rot = ABS (delta_rot);  // we only ever use the absolute value

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

  // This is to stop the character moving while they're rotating to
  // face the direction they're turning towards.
  if (delta_rot < 0.1f)
  {
    // move forwards
    // if haven't reached destination movement speed yet...
    if (pclinmove->GetBodyVelocity ().SquaredNorm () < movespeed * movespeed)
    {
      // ... then accelerate the player forward
      float elapsedsecs = vc->GetElapsedTicks () / 1000.0f;
      // not real, but then again forget realism over responsiveness
      float newspeed = moveaccel * i * elapsedsecs + pclinmove->GetBodyVelocity ().Norm ();
      // clipping in case acceleration oversteps the target speed.
      if (newspeed > movespeed * i)
        newspeed = movespeed * i;
      pclinmove->SetVelocity (csVector3 (0, 0, -newspeed));
    }
  }
  // slide effect when you suddenly move in opposite direction
  else if (delta_rot > PI - PI/4)
  {
    pclinmove->SetVelocity (csVector3 (0, 0, 0));
  }

  // the faster you move, the slower you turn
  float turnspeed = (1.0f - pclinmove->GetBodyVelocity ().Norm () / movespeed) * (maxturnspeed - minturnspeed) + minturnspeed;
  // do rotation based on calculated velocity
  pclinmove->SetAngularVelocity (rotvel * turnspeed, target);
}
