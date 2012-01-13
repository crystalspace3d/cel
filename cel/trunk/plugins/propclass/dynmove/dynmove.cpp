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
#include "csgeom/transfrm.h"
#include "iutil/objreg.h"
#include "plugins/propclass/dynmove/dynmove.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "propclass/defcam.h"
#include "propclass/mesh.h"

#include "iengine/mesh.h"
#include "iengine/movable.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (DynamicMove, "pcmove.actor.dynamic")

//---------------------------------------------------------------------------

csStringID celPcDynamicMove::id_input_forward_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_backward_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_forward_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_backward_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_strafeleft_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_straferight_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_strafeleft_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_straferight_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_jump_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_lookup_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_lookup_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_lookdown_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_lookdown_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_center_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_rotateleft_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_rotateleft_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_rotateright_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_rotateright_up = csInvalidStringID;

PropertyHolder celPcDynamicMove::propinfo;

celPcDynamicMove::celPcDynamicMove (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_input_forward_down == csInvalidStringID)
  {
    id_input_forward_down = pl->FetchStringID ("cel.input.forward.down");
    id_input_backward_down = pl->FetchStringID ("cel.input.backward.down");
    id_input_forward_up = pl->FetchStringID ("cel.input.forward.up");
    id_input_backward_up = pl->FetchStringID ("cel.input.backward.up");
    id_input_strafeleft_down = pl->FetchStringID ("cel.input.strafeleft.down");
    id_input_straferight_down = pl->FetchStringID ("cel.input.straferight.down");
    id_input_strafeleft_up = pl->FetchStringID ("cel.input.strafeleft.up");
    id_input_straferight_up = pl->FetchStringID ("cel.input.straferight.up");
    id_input_jump_down = pl->FetchStringID ("cel.input.jump.down");
    id_input_lookup_down = pl->FetchStringID ("cel.input.lookup.down");
    id_input_lookup_up = pl->FetchStringID ("cel.input.lookup.up");
    id_input_lookdown_down = pl->FetchStringID ("cel.input.lookdown.down");
    id_input_lookdown_up = pl->FetchStringID ("cel.input.lookdown.up");
    id_input_center_down = pl->FetchStringID ("cel.input.center.down");
    id_input_rotateleft_down = pl->FetchStringID ("cel.input.rotateleft.down");
    id_input_rotateleft_up = pl->FetchStringID ("cel.input.rotateleft.up");
    id_input_rotateright_down = pl->FetchStringID ("cel.input.rotateright.down");
    id_input_rotateright_up = pl->FetchStringID ("cel.input.rotateright.up");
  }

  propholder = &propinfo;
  propinfo.SetCount (1);
  AddProperty (propid_speed, "speed",
	CEL_DATA_FLOAT, false, "Movement speed", &speed);
  speed = 1.0f;
}

celPcDynamicMove::~celPcDynamicMove ()
{
}

void celPcDynamicMove::SetEntity (iCelEntity* entity)
{
  celPcCommon::SetEntity (entity);
  if (entity)
    entity->QueryMessageChannel ()->Subscribe (this, "cel.input.");
}

bool celPcDynamicMove::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  return false;
}

void celPcDynamicMove::GetPCS ()
{
  if (!pcmechobj) pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (entity);
}

bool celPcDynamicMove::ReceiveMessage (csStringID msgid, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  if (celPcCommon::ReceiveMessage (msgid, sender, ret, params))
    return true;

  GetPCS ();

  csReversibleTransform trans = pcmechobj->GetMesh ()->GetMesh ()->
    GetMovable ()->GetFullTransform ();

  if (msgid == id_input_forward_down)
  {
    pcmechobj->SetLinearVelocity (trans.This2OtherRelative (csVector3 (0, 0, -25.0f * speed)));
    return true;
  }
  else if (msgid == id_input_forward_up)
  {
    pcmechobj->SetLinearVelocity (csVector3 (0));
    return true;
  }
  else if (msgid == id_input_backward_down)
  {
    pcmechobj->SetLinearVelocity (trans.This2OtherRelative (csVector3 (0, 0, 25.0f * speed)));
    return true;
  }
  else if (msgid == id_input_backward_up)
  {
    pcmechobj->SetLinearVelocity (csVector3 (0));
    return true;
  }
  else if (msgid == id_input_strafeleft_down)
  {
    pcmechobj->SetLinearVelocity (trans.This2OtherRelative (csVector3 (25.0f * speed, 0, 0)));
    return true;
  }
  else if (msgid == id_input_strafeleft_up)
  {
    pcmechobj->SetLinearVelocity (csVector3 (0));
    return true;
  }
  else if (msgid == id_input_straferight_down)
  {
    pcmechobj->SetLinearVelocity (trans.This2OtherRelative (csVector3 (-25.0f * speed, 0, 0)));
    return true;
  }
  else if (msgid == id_input_straferight_up)
  {
    pcmechobj->SetLinearVelocity (csVector3 (0));
    return true;
  }
  else if (msgid == id_input_rotateleft_down)
  {
    pcmechobj->SetAngularVelocity (csVector3 (0, -25.0f * speed, 0));
    return true;
  }
  else if (msgid == id_input_rotateleft_up)
  {
    pcmechobj->SetAngularVelocity (csVector3 (0));
    return true;
  }
  else if (msgid == id_input_rotateright_down)
  {
    pcmechobj->SetAngularVelocity (csVector3 (0, 25.0f * speed, 0));
    return true;
  }
  else if (msgid == id_input_rotateright_up)
  {
    pcmechobj->SetAngularVelocity (csVector3 (0));
    return true;
  }
  else if (msgid == id_input_jump_down)
  {
    pcmechobj->AddForceDuration (csVector3 (0, 5.0f * speed, 0), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_lookup_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (1.0f);
    return true;
  }
  else if (msgid == id_input_lookup_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (-1.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_center_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    if (pcdefcamera)
      pcdefcamera->CenterCamera ();
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------

