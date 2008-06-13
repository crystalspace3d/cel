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
#include "propclass/mesh.h"
#include "propclass/meshsel.h"
#include "propclass/tooltip.h"
#include "propclass/camera.h"
#include "propclass/defcam.h"
#include "propclass/newcamera.h"
#include "propclass/delegcam.h"
#include "propclass/cameras/tracking.h"
#include "propclass/actoranalog.h"
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
}

bool celBehaviourBox::ReceiveMessage (csStringID msg_id,
	iMessageSender*,
	celData&, iCelParameterBlock* params)
{
  iCelEntity* ent = 0;
  if (params)
  {
    const celData* cd = params->GetParameter (
    	pl->FetchStringID ("cel.parameter.entity"));
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
  bhroom = csPtr<celBehaviourRoom> (new celBehaviourRoom (entity, object_reg));
  fpscam=0;
  speed=1;
}

celBehaviourActor::~celBehaviourActor()
{
}

bool celBehaviourActor::ReceiveMessage (csStringID msgid,
	iMessageSender* sender,
	celData& ret, iCelParameterBlock* params)
{
  // @@@ Clumsy, best to work on id's instead of converting to string.
  csString msg_id_str = pl->FetchString (msgid);
  const char* msg_id = (const char*)msg_id_str;
  bool pcinput_msg = strncmp (msg_id, "cel.input.", 10) == 0;

  if (pcinput_msg)
  {
    csRef<iPcActorAnalog> pcactor = celQueryPropertyClassEntity
      <iPcActorAnalog> (entity);
    if (!pcactor)
      return false;

    if (!strcmp (msg_id+10, "joyaxis0"))
    {
      CEL_FETCH_FLOAT_PAR (value, params, pl->FetchStringID("cel.parameter.value"));
      pcactor->SetAxis (0, value);
    }
    else if (!strcmp (msg_id+10, "joyaxis1"))
    {
      CEL_FETCH_FLOAT_PAR (value, params, pl->FetchStringID("cel.parameter.value"));
      pcactor->SetAxis (1, -value);
    }

    else if (!strcmp (msg_id+10, "left.down"))
      pcactor->AddAxis (0, -1);
    else if (!strcmp (msg_id+10, "left.up"))
      pcactor->AddAxis (0, 1);
    else if (!strcmp (msg_id+10, "right.down"))
      pcactor->AddAxis (0, 1);
    else if (!strcmp (msg_id+10, "right.up"))
      pcactor->AddAxis (0, -1);
    else if (!strcmp (msg_id+10, "up.down"))
      pcactor->AddAxis (1, 1);
    else if (!strcmp (msg_id+10, "up.up"))
      pcactor->AddAxis (1, -1);
    else if (!strcmp (msg_id+10, "down.down"))
      pcactor->AddAxis (1, -1);
    else if (!strcmp (msg_id+10, "down.up"))
      pcactor->AddAxis (1, 1);

    csRef<iPcTrackingCamera> trackcam = celQueryPropertyClassEntity<iPcTrackingCamera> (entity);
    if (!trackcam)
      return false;

    if (!strcmp (msg_id+10, "camleft.down"))
    {
      trackcam->Pan (iPcTrackingCamera::PAN_LEFT);
    }
    else if (!strcmp (msg_id+10, "camleft.up"))
    {
      trackcam->Pan (iPcTrackingCamera::PAN_NONE);
    }
    else if (!strcmp (msg_id+10, "camright.down"))
    {
      trackcam->Pan (iPcTrackingCamera::PAN_RIGHT);
    }
    else if (!strcmp (msg_id+10, "camright.up"))
    {
      trackcam->Pan (iPcTrackingCamera::PAN_NONE);
    }
    else if (!strcmp (msg_id+10, "camup.down"))
    {
      trackcam->Tilt (iPcTrackingCamera::TILT_UP);
    }
    else if (!strcmp (msg_id+10, "camup.up"))
    {
      trackcam->Tilt (iPcTrackingCamera::TILT_NONE);
    }
    else if (!strcmp (msg_id+10, "camdown.down"))
    {
      trackcam->Tilt (iPcTrackingCamera::TILT_DOWN);
    }
    else if (!strcmp (msg_id+10, "camdown.up"))
    {
      trackcam->Tilt (iPcTrackingCamera::TILT_NONE);
    }
    else if (!strcmp (msg_id+10, "ready.down"))
      trackcam->SetTargetState (iPcTrackingCamera::TARGET_NONE);
    else if (!strcmp (msg_id+10, "ready.up"))
      trackcam->SetTargetState (iPcTrackingCamera::TARGET_BASE);
    else if (!strcmp (msg_id+10, "lockon.down"))
    {
      if (trackcam->GetTargetState () == iPcTrackingCamera::TARGET_NONE)
      {
        trackcam->SetTargetEntity ("dummy2b");
        trackcam->SetTargetState (iPcTrackingCamera::TARGET_OBJ);
      }
    }
    else if (!strcmp (msg_id+10, "lockon.up"))
    {
      if (trackcam->GetTargetState () == iPcTrackingCamera::TARGET_OBJ)
        trackcam->SetTargetState (iPcTrackingCamera::TARGET_NONE);
    }
    else if (!strcmp (msg_id+10, "resetcam.down"))
      trackcam->ResetCamera ();

    if (!strcmp (msg_id+10, "tiltcam"))
    {
      CEL_FETCH_FLOAT_PAR (value, params, pl->FetchStringID("cel.parameter.value"));
      puts ("tilting camera");
    }
    else if (!strcmp (msg_id+10, "pancam"))
    {
      CEL_FETCH_FLOAT_PAR (value, params, pl->FetchStringID("cel.parameter.value"));
      if (value < -EPSILON)
        trackcam->Pan (iPcTrackingCamera::PAN_LEFT);
      else if (value > EPSILON)
        trackcam->Pan (iPcTrackingCamera::PAN_RIGHT);
      else
        trackcam->Pan (iPcTrackingCamera::PAN_NONE);
    }
    return true;
  }

  return bhroom->ReceiveMessage (msgid, sender, ret, params);
}

//---------------------------------------------------------------------------

celBehaviourDynActor::celBehaviourDynActor (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourDynActor::ReceiveMessage (csStringID msgid,
	iMessageSender* sender,
	celData& ret, iCelParameterBlock* params)
{
  // @@@ Clumsy, best to work on id's instead of converting to string.
  csString msg_id_str = pl->FetchString (msgid);
  const char* msg_id = (const char*)msg_id_str;
  bool pcinput_msg = strncmp (msg_id, "cel.input.", 10) == 0;

  if (pcinput_msg)
  {
    csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity,
    	iPcMechanicsObject);
    if (!pcmechobj)
      return false;

    if (!strcmp (msg_id+10, "forward.down"))
      pcmechobj->AddForceDuration (csVector3 (0, 0, -25.0f), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+10, "backward.down"))
      pcmechobj->AddForceDuration (csVector3 (0, 0, 25.0f), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+10, "strafeleft.down"))
      pcmechobj->AddForceDuration (csVector3 (25.0f, 0, 0), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+10, "straferight.down"))
      pcmechobj->AddForceDuration (csVector3 (-25.0f, 0, 0), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+10, "jump.down"))
      pcmechobj->AddForceDuration (csVector3 (0, 25.0f, 0), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+10, "lookup.down"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (1.0f);
    }
    else if (!strcmp (msg_id+10, "lookup.up"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+10, "lookdown.down"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (-1.0f);
    }
    else if (!strcmp (msg_id+10, "lookdown.up"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+10, "center.down"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->CenterCamera ();
    }
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------

celBehaviourWheeled::celBehaviourWheeled (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourWheeled::ReceiveMessage (csStringID msgid,
	iMessageSender* sender,
	celData& ret, iCelParameterBlock* params)
{
  // @@@ Clumsy, best to work on id's instead of converting to string.
  csString msg_id_str = pl->FetchString (msgid);
  const char* msg_id = (const char*)msg_id_str;
  bool pcinput_msg = strncmp (msg_id, "cel.input.", 10) == 0;
  if (pcinput_msg)
  {
    csRef<iPcWheeled> pcwheeled = CEL_QUERY_PROPCLASS_ENT (entity,
    	iPcWheeled);
    if (!pcwheeled)
      return false;

    if (!strcmp (msg_id+10, "accelerate.down"))
	pcwheeled->Accelerate();

//Autoreverse handles putting the car in reverse once it is slow enough.
    else if (!strcmp (msg_id+10, "reverse.down"))
	pcwheeled->Brake();

    else if (!strcmp (msg_id+10, "steerleft.down"))
	pcwheeled->SteerLeft();

    else if (!strcmp (msg_id+10, "steerright.down"))
	pcwheeled->SteerRight();

    else if (!strcmp (msg_id+10, "steerleft.up"))
	pcwheeled->SteerStraight();

    else if (!strcmp (msg_id+10, "steerright.up"))
	pcwheeled->SteerStraight();

    else if (!strcmp (msg_id+10, "handbrake.down"))
	pcwheeled->Handbrake(true);

    else if (!strcmp (msg_id+10, "handbrake.up"))
	pcwheeled->Handbrake(false);

    if (!strcmp (msg_id+10, "accelerate.up"))
	pcwheeled->Accelerate(0.0f);

    if (!strcmp (msg_id+10, "reverse.up"))
    {
	pcwheeled->Brake(0.0f);
    }
    else if (!strcmp (msg_id+10, "lookup.down"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (1.0f);
    }
    else if (!strcmp (msg_id+10, "lookup.up"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+10, "lookdown.down"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (-1.0f);
    }
    else if (!strcmp (msg_id+10, "lookdown.up"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+10, "center.down"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->CenterCamera ();
    }
    return true;
  }
  else if (!strcmp (msg_id, "cel.mechanics.collision"))
  {
    CEL_FETCH_VECTOR3_PAR(pos, params, pl->FetchStringID("cel.parameter.position"));
    CEL_FETCH_VECTOR3_PAR(norm, params, pl->FetchStringID("cel.parameter.normal"));
    CEL_FETCH_FLOAT_PAR(depth, params, pl->FetchStringID("cel.parameter.depth"));
    if (depth > 0.005f)
    {
      if(!pcmeshdeform)
        pcmeshdeform = CEL_QUERY_PROPCLASS_ENT (entity, iPcMeshDeform);
      if (pcmeshdeform)
        pcmeshdeform->DeformMesh(pos, norm * depth, true);}
    return true;
  }

  return false;
}

