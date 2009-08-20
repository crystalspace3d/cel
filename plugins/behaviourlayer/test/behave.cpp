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
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/actormove.h"
#include "propclass/mesh.h"
#include "propclass/meshsel.h"
#include "propclass/tooltip.h"
#include "propclass/camera.h"
#include "propclass/defcam.h"
#include "propclass/newcamera.h"
#include "propclass/delegcam.h"
#include "propclass/inv.h"
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "propclass/mechsys.h"
#include "propclass/wheeled.h"
#include "plugins/behaviourlayer/test/behave.h"
#include "celtool/stdparams.h"
#include <iostream>

//---------------------------------------------------------------------------

celBehaviourGeneral::celBehaviourGeneral (iCelEntity* entity,
	iObjectRegistry* object_reg) : scfImplementationType (this)
{
  celBehaviourGeneral::entity = entity;
  celBehaviourGeneral::object_reg = object_reg;
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  name = 0;
  entity->QueryMessageChannel ()->Subscribe (this, "cel.");
}

celBehaviourGeneral::~celBehaviourGeneral ()
{
  delete[] name;
}

#if 0
// Old messages no longer supported.
bool celBehaviourGeneral::SendMessage (const char* msg_id,
	iCelPropertyClass* pc,
	celData& ret, iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, pc, ret, params, arg);
  va_end (arg);
  return rc;
}
#endif

//---------------------------------------------------------------------------

celBehaviourPrinter::celBehaviourPrinter (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

//---------------------------------------------------------------------------

celBehaviourRoom::celBehaviourRoom (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

//---------------------------------------------------------------------------

celBehaviourQuest::celBehaviourQuest (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

//---------------------------------------------------------------------------


celBehaviourBox::celBehaviourBox (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
  msgid_meshsel_down = pl->FetchStringID ("cel.mesh.select.down");
  msgid_pctimer_wakeup = pl->FetchStringID ("cel.timer.wakeup");
  id_param_entity = pl->FetchStringID ("cel.parameter.entity");
}

bool celBehaviourBox::ReceiveMessage (csStringID msg_id,
	iMessageSender*,
	celData&, iCelParameterBlock* params)
{
  iCelEntity* ent = 0;
  if (params)
  {
    const celData* cd = params->GetParameter (id_param_entity);
    if (cd)
      ent = cd->value.ent;
  }

  if (ent && msg_id == msgid_meshsel_down)
  {
    csRef<iPcMesh> pcmesh (CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcMesh));
    CS_ASSERT (pcmesh != 0);
    const char* curact = pcmesh->GetAction ();
    if (!strcmp (curact, "open"))
      pcmesh->SetAction ("closed");
    else
    {
      pcmesh->SetAction ("open");
      // If the box is opened we remove everything from it.
      csRef<iPcTimer> pctimer (CEL_QUERY_PROPCLASS (
		entity->GetPropertyClassList (), iPcTimer));
      CS_ASSERT (pctimer != 0);
      pctimer->WakeUp (200, false);
    }
  }
  else if (msg_id == msgid_pctimer_wakeup)
  {
    csRef<iPcTimer> pctimer (CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcTimer));
    CS_ASSERT (pctimer != 0);
    csRef<iPcMesh> pcmesh (CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcMesh));
    CS_ASSERT (pcmesh != 0);
    // Remove one entity from the box.
    csRef<iPcInventory> pcinv (CEL_QUERY_PROPCLASS (
		entity->GetPropertyClassList (), iPcInventory));
    CS_ASSERT (pcinv != 0);
    if (pcinv->GetEntityCount () > 0)
    {
      iCelEntity* inv_ent = pcinv->GetEntity (0);
      csRef<iPcGravity> inv_ent_gravity (CEL_QUERY_PROPCLASS (
	    	inv_ent->GetPropertyClassList (), iPcGravity));
      csRef<iPcMesh> inv_ent_mesh (CEL_QUERY_PROPCLASS (
	    	inv_ent->GetPropertyClassList (), iPcMesh));
      if (inv_ent_mesh)
      {
	inv_ent_mesh->Show ();
	inv_ent_mesh->MoveMesh (pcmesh->GetMesh ()->GetMovable ()->
	      	GetSectors ()->Get (0), pcmesh->GetMesh ()->GetMovable ()->
		GetTransform ().GetOrigin ()+csVector3 (0, 1.3f, 0));
	if (inv_ent_gravity)
	{
	  inv_ent_gravity->ResetSpeed ();
	  inv_ent_gravity->ClearForces ();
	  float dx = 3 + 4*float ((rand () >> 3) % 10000) / 10000.;
	  if (((rand () >> 3) & 1) == 0) dx = -dx;
	  float dy = 3 + 4*float ((rand () >> 3) % 10000) / 10000.;
	  if (((rand () >> 3) & 1) == 0) dy = -dy;
	  printf ("%g,%g,%g\n", dx, 4.0, dy); fflush (stdout);
	  inv_ent_gravity->ApplyForce (csVector3 (dx, 4, dy), .5);
	}
      }
      pcinv->RemoveEntity (inv_ent);
    }
    if (pcinv->GetEntityCount () > 0)
    {
      // Restart timer.
      pctimer->WakeUp (200, false);
    }
  }

  return false;
}

//---------------------------------------------------------------------------

celBehaviourActor::celBehaviourActor (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

//---------------------------------------------------------------------------

celBehaviourDynActor::celBehaviourDynActor (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
  id_input_forward_down = pl->FetchStringID ("cel.input.forward.down");
  id_input_backward_down = pl->FetchStringID ("cel.input.backward.down");
  id_input_strafeleft_down = pl->FetchStringID ("cel.input.strafeleft.down");
  id_input_straferight_down = pl->FetchStringID ("cel.input.straferight.down");
  id_input_jump_down = pl->FetchStringID ("cel.input.jump.down");
  id_input_lookup_down = pl->FetchStringID ("cel.input.lookup.down");
  id_input_lookup_up = pl->FetchStringID ("cel.input.lookup.up");
  id_input_lookdown_down = pl->FetchStringID ("cel.input.lookdown.down");
  id_input_lookdown_up = pl->FetchStringID ("cel.input.lookdown.up");
  id_input_center_down = pl->FetchStringID ("cel.input.center.down");
}

bool celBehaviourDynActor::ReceiveMessage (csStringID msgid,
	iMessageSender* sender,
	celData& ret, iCelParameterBlock* params)
{

  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity,
    iPcMechanicsObject);
  if (!pcmechobj)
    return false;

  if (msgid == id_input_forward_down)
  {
    pcmechobj->AddForceDuration (csVector3 (0, 0, -25.0f), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_backward_down)
  {
    pcmechobj->AddForceDuration (csVector3 (0, 0, 25.0f), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_strafeleft_down)
  {
    pcmechobj->AddForceDuration (csVector3 (25.0f, 0, 0), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_straferight_down)
  {
    pcmechobj->AddForceDuration (csVector3 (-25.0f, 0, 0), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_jump_down)
  {
    pcmechobj->AddForceDuration (csVector3 (0, 25.0f, 0), false,
      csVector3 (0, 0, 0), .2f);
    return true;
  }
  else if (msgid == id_input_lookup_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (1.0f);
    return true;
  }
  else if (msgid == id_input_lookup_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (-1.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_center_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->CenterCamera ();
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------

celBehaviourWheeled::celBehaviourWheeled (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
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
  id_mech_par_position = pl->FetchStringID ("cel.parameter.position");
  id_mech_par_normal = pl->FetchStringID ("cel.parameter.normal");
  id_mech_par_depth = pl->FetchStringID ("cel.parameter.depth");
}

bool celBehaviourWheeled::ReceiveMessage (csStringID msgid,
	iMessageSender* sender,
	celData& ret, iCelParameterBlock* params)
{
  csRef<iPcWheeled> pcwheeled = CEL_QUERY_PROPCLASS_ENT (entity,
    iPcWheeled);
  if (!pcwheeled)
    return false;

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
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (1.0f);
    return true;
  }
  else if (msgid == id_input_lookup_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (-1.0f);
    return true;
  }
  else if (msgid == id_input_lookdown_up)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
    return true;
  }
  else if (msgid == id_input_center_down)
  {
    csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcDefaultCamera);
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
      if(!pcmeshdeform)
        pcmeshdeform = CEL_QUERY_PROPCLASS_ENT (entity, iPcMeshDeform);
      if (pcmeshdeform)
        pcmeshdeform->DeformMesh(pos, norm * depth, true);
    }
    return true;
  }

  return false;
}

