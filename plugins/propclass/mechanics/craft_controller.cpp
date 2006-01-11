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

#include "plugins/propclass/mechanics/craft_controller.h"
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

  turn_acc = 0.04;
  updwn_acc = 0.04;
  turn_max = 3.5;
  updwn_max = 0.5;
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
bool celPcCraftController::PerformAction (csStringID actionId, iCelParameterBlock* params)
{
  return true;
}

void celPcCraftController::Tick ()
{
  UpdateBody ();
}


void celPcCraftController::DoTurningCalc (bool isturning, float &turn, float acc, float max)
{
  if(isturning)
    turn += acc;
  else
    turn -= acc;

  if(turn > max)
    turn = max;
  else if(turn < 0.0f)
    turn = 0.0f;
}

void celPcCraftController::UpdateBody ()
{
  DoTurningCalc (turn_left, current_turning_left, turn_acc, turn_max);
  DoTurningCalc (turn_right, current_turning_right, turn_acc, turn_max);
  DoTurningCalc (turn_up, current_up, updwn_acc, updwn_max);
  DoTurningCalc (turn_down, current_down, updwn_acc, updwn_max);

  /*! at the moment roll in the ships turning is disabled
      until I write a self angular stabilising component */
  csVector3 turning(current_up - current_down, current_turning_right - current_turning_left, 0);

  csRef<iPcMechanicsObject> ship_mech = CEL_QUERY_PROPCLASS_ENT (GetEntity(), iPcMechanicsObject);
  float height = 2.0;   // disable height dependence for now

  // angular suppressant is dependant on height
  csVector3 angular_supressant = ship_mech->GetAngularVelocity() * 1;   // could use multiplier here

  if(height > 0.1f)
    angular_supressant /= height;
  else
    angular_supressant /= 0.1;

  ship_mech->SetAngularVelocity(ship_mech->LocalToWorld(turning) + angular_supressant);
}
