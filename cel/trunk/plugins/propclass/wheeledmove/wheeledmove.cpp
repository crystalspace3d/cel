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
#include "plugins/propclass/wheeledmove/wheeledmove.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "propclass/defcam.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (WheeledMove, "pcmove.actor.wheeled")

//---------------------------------------------------------------------------

csStringID celPcWheeledMove::id_input_accelerate_up = csInvalidStringID;
csStringID celPcWheeledMove::id_input_accelerate_down = csInvalidStringID;
csStringID celPcWheeledMove::id_input_reverse_up = csInvalidStringID;
csStringID celPcWheeledMove::id_input_reverse_down = csInvalidStringID;
csStringID celPcWheeledMove::id_input_steerleft_up = csInvalidStringID;
csStringID celPcWheeledMove::id_input_steerleft_down = csInvalidStringID;
csStringID celPcWheeledMove::id_input_steerright_up = csInvalidStringID;
csStringID celPcWheeledMove::id_input_steerright_down = csInvalidStringID;
csStringID celPcWheeledMove::id_input_handbrake_up = csInvalidStringID;
csStringID celPcWheeledMove::id_input_handbrake_down = csInvalidStringID;
csStringID celPcWheeledMove::id_input_lookup_up = csInvalidStringID;
csStringID celPcWheeledMove::id_input_lookup_down = csInvalidStringID;
csStringID celPcWheeledMove::id_input_lookdown_up = csInvalidStringID;
csStringID celPcWheeledMove::id_input_lookdown_down = csInvalidStringID;
csStringID celPcWheeledMove::id_input_center_down = csInvalidStringID;
csStringID celPcWheeledMove::id_mech_collision = csInvalidStringID;
csStringID celPcWheeledMove::id_mech_par_position = csInvalidStringID;
csStringID celPcWheeledMove::id_mech_par_normal = csInvalidStringID;
csStringID celPcWheeledMove::id_mech_par_depth = csInvalidStringID;

celPcWheeledMove::celPcWheeledMove (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_input_accelerate_up == csInvalidStringID)
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
    id_mech_par_position = pl->FetchStringID ("position");
    id_mech_par_normal = pl->FetchStringID ("normal");
    id_mech_par_depth = pl->FetchStringID ("depth");
  }
}

celPcWheeledMove::~celPcWheeledMove ()
{
}

void celPcWheeledMove::SetEntity (iCelEntity* entity)
{
  celPcCommon::SetEntity (entity);
  if (entity)
    entity->QueryMessageChannel ()->Subscribe (this, "cel.");
}

bool celPcWheeledMove::SetPropertyIndexed (int idx, long b)
{
  return false;
}

bool celPcWheeledMove::GetPropertyIndexed (int idx, long& l)
{
  return false;
}

bool celPcWheeledMove::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  return false;
}

void celPcWheeledMove::GetPCS ()
{
  if (!pcmechobj) pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (entity);
  if (!pcwheeled) pcwheeled = celQueryPropertyClassEntity<iPcWheeled> (entity);
  if (!pcdefcamera) pcdefcamera = celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
  if (!pcmeshdeform) pcmeshdeform = celQueryPropertyClassEntity<iPcMeshDeform> (entity);
}

bool celPcWheeledMove::ReceiveMessage (csStringID msgid, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  if (celPcCommon::ReceiveMessage (msgid, sender, ret, params))
    return true;

  GetPCS ();

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
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (1.0f);
    return true;
  }
  else if (msgid == id_input_lookup_up)
  {
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_down)
  {
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (-1.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_up)
  {
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_center_down)
  {
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
      if (pcmeshdeform)
        pcmeshdeform->DeformMesh(pos, norm * depth, true);
    }
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------

