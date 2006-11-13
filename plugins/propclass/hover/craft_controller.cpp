/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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
#include "iutil/evdefs.h"

#include "plugins/propclass/hover/craft_controller.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

#include <propclass/mechsys.h>
#include <propclass/mesh.h>

#include <iengine/mesh.h>
#include <ivaria/dynamics.h>

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY(CraftController, "pccraft")

SCF_IMPLEMENT_IBASE_EXT (celPcCraftController)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCraftController)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcCraftController::PcCraftController)
  SCF_IMPLEMENTS_INTERFACE (iPcCraftController)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcCraftController::celPcCraftController (iObjectRegistry* object_reg)
	: celPcCommon (object_reg), celPeriodicTimer (pl)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCraftController);

  turn_left = false;
  turn_right = false;
  turn_up = false;
  turn_down = false;

  current_turning_left = 0;
  current_turning_right = 0;
  current_up = 0;
  current_down = 0;

  turn_acc = 0.4f;
  pitch_acc = 0.4f;
  turn_max = 1.5f;
  pitch_max = 0.5f;
  roll = 0.8f;

  thrust_on = false;
  thrust = 10.0f;
  topspeed = 20.0f;

  after_burner = false;
  topburnerspeed = 40.0f;

  slide_on = false;
  redirect_vel_ratio = 0.0;
  deceleration_rate = 1.0f - 0.01f;

  brakes_on = false;
  braking_speed = 1.0f - 0.1f;
}

celPcCraftController::~celPcCraftController ()
{
}

csPtr<iCelDataBuffer> celPcCraftController::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (1);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcCraftController::Load (iCelDataBuffer* databuf)
{
  return true;
}

bool celPcCraftController::PerformActionIndexed (int idx,
    iCelParameterBlock* params, celData& ret)
{
  return false;
}

void celPcCraftController::Tick ()
{
  UpdateBody ();
}

void celPcCraftController::DoTurningCalc (bool isturning, float &turn,
    float acc, float max)
{
  if(isturning)
    turn += acc;
  else
    //turn -= acc;
    turn = 0.0f;

  if(turn > max)
    turn = max;
  else if(turn < 0.0f)
    turn = 0.0f;
}

void celPcCraftController::UpdateBody ()
{
  csRef<iPcMechanicsObject> ship_mech = CEL_QUERY_PROPCLASS_ENT (GetEntity(),
        iPcMechanicsObject);

  DoTurningCalc (turn_left, current_turning_left, turn_acc, turn_max);
  DoTurningCalc (turn_right, current_turning_right, turn_acc, turn_max);
  DoTurningCalc (turn_up, current_up, pitch_acc, pitch_max);
  DoTurningCalc (turn_down, current_down, pitch_acc, pitch_max);

  /*! at the moment roll in the ships turning is disabled
      until I write a self angular stabilising component */
  float xrot = current_up - current_down;
  float yrot = current_turning_right - current_turning_left;
  csVector3 turning = ship_mech->LocalToWorld (csVector3 (xrot, 0, roll * yrot)) +
        csVector3 (0, yrot, 0);

  float height = 2.0;   // disable height dependence for now

  // angular suppressant is dependant on height
  // could use multiplier here
  csVector3 angular_supressant = ship_mech->GetAngularVelocity() * 1;

  if(height > 0.1f)
    angular_supressant /= height;
  else
    angular_supressant /= 0.1f;

  ship_mech->SetAngularVelocity(turning + angular_supressant);

  // normalize velocity vector
  if (!slide_on && redirect_vel_ratio > 0.0)
  {
    float speed = ship_mech->GetLinearVelocity ().Norm ();
    ship_mech->SetLinearVelocity ((1.0 - redirect_vel_ratio) * ship_mech
    	->GetLinearVelocity () + redirect_vel_ratio
	* ship_mech->LocalToWorld (csVector3 (0,0,-speed)));
  }

  float hspeed = after_burner ? topburnerspeed : topspeed;

  // I hope to move this to another interface
  csVector3 lv = ship_mech->WorldToLocal (ship_mech->GetLinearVelocity ());
  if (thrust_on && !(-lv.z > hspeed))
    ship_mech->AddForceDuration (csVector3 (0,0,-thrust), true,
      csVector3 (0,0,0), 0.1f);
  if (!thrust_on)
    ship_mech->SetLinearVelocity (deceleration_rate *
      ship_mech->GetLinearVelocity ());
  // remember: brakes and thruster can be on at same time
  if (brakes_on)
    ship_mech->SetLinearVelocity (braking_speed *
      ship_mech->GetLinearVelocity ());
}
