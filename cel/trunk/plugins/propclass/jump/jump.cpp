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

// CS Includes
#include "ivaria/reporter.h"

// CEL Includes
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

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_jump, "cel.action.Jump");
  }

  // For properties.
  propinfo.SetCount (2);
  AddProperty (propid_isjumping, "cel.property.isjumping",
    CEL_DATA_BOOL, true, "Whether character is in the process of a jump.",
    &jumping);
  AddProperty (propid_jumpspeed, "cel.property.jumpspeed",
    CEL_DATA_FLOAT, false, "Jumping speed.",
    &jumpspeed);

  jumping = false;
  jumpspeed = 10.0f;
}

celPcJump::~celPcJump ()
{
}

#define TEST_SERIAL 2

csPtr<iCelDataBuffer> celPcJump::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  databuf->Add (jumping);
  databuf->Add (jumpspeed);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcJump::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL) return false;
  jumping = databuf->GetBool ();
  jumpspeed = databuf->GetFloat ();
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
  // already doing a jump
  if (jumping)
    return;
  jumping = true;
  FindSiblingPropertyClasses ();
  // actually perform the jump if we're on the ground
  if (linmove && linmove->IsOnGround ())
    linmove->AddVelocity (csVector3 (0, jumpspeed, 0));
  // we use a cheat... this should skip this current frame
  // and only callback once we have left the ground.
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
}
bool celPcJump::IsJumping () const
{
  return jumping;
}
void celPcJump::SetJumpSpeed (float jspd)
{
  jumpspeed = jspd;
}
float celPcJump::GetJumpSpeed () const
{
  return jumpspeed;
}

void celPcJump::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
    linmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
}

void celPcJump::TickEveryFrame ()
{
  UpdateMovement ();
}

void celPcJump::UpdateMovement ()
{
  // should never happen
  if (!jumping)
  {
    csRef<iReporter> rep = csQueryRegistry<iReporter>(object_reg);
    if (rep)
      rep->ReportError("cel.pcmove.jump", "Callback was improperly removed!");
    else
      csPrintf("cel.pcmove.jump: Callback was improperly removed!");
    return;
  }
  FindSiblingPropertyClasses ();
  // check if we landed from our jump
  if (jumping && linmove && linmove->IsOnGround ())
  {
    jumping = false;
    pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);

    if (!dispatcher_landed)
    {
      dispatcher_landed = entity->QueryMessageChannel ()->
        CreateMessageDispatcher (this, "cel.move.jump.landed");
      if (!dispatcher_landed)
        return;
    }
    dispatcher_landed->SendMessage (0);
  }
}
