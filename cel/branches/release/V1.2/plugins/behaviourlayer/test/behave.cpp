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
#include "propclass/inv.h"
#include "propclass/gravity.h"
#include "propclass/actormove.h"
#include "propclass/timer.h"
#include "propclass/mechsys.h"
#include "propclass/wheeled.h"
#include "propclass/meshdeform.h"
#include "plugins/behaviourlayer/test/behave.h"
#include "celtool/stdparams.h"
#include <iostream>

//---------------------------------------------------------------------------

celBehaviourGeneral::celBehaviourGeneral (iCelEntity* entity,
	iObjectRegistry* object_reg) : scfImplementationType (this)
{
  celBehaviourGeneral::entity = entity;
  celBehaviourGeneral::object_reg = object_reg;
  name = 0;
}

celBehaviourGeneral::~celBehaviourGeneral ()
{
  delete[] name;
}

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

//---------------------------------------------------------------------------

celBehaviourPrinter::celBehaviourPrinter (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourPrinter::SendMessageV (const char* msg_id,
	iCelPropertyClass* /*pc*/,
	celData&, iCelParameterBlock* params, va_list arg)
{
  (void)arg; (void)params;
  //printf ("Got message '%s'\n", msg_id);
  return false;
}

//---------------------------------------------------------------------------

celBehaviourRoom::celBehaviourRoom (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourRoom::SendMessageV (const char* msg_id,
	iCelPropertyClass*,
	celData&, iCelParameterBlock* params, va_list arg)
{
#if 0
  (void)arg;
  // @@@ Should store the id's for the parameters below.
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl) return false;
  const celData* butdata = params ? params->GetParameter (pl->FetchStringID (
  	"cel.parameter.button")) : 0;
  if (butdata)
  {
    iCelEntity* ent = params->GetParameter (pl->FetchStringID (
  	  "cel.parameter.entity"))->value.ent;
    int x = params->GetParameter (pl->FetchStringID (
  	  "cel.parameter.x"))->value.l;
    int y = params->GetParameter (pl->FetchStringID (
  	  "cel.parameter.y"))->value.l;
    int but = butdata->value.l;
    if (!strcmp (msg_id, "pcmeshsel_up"))
    {
      printf ("  UP '%s' (%d,%d,%d)\n", ent->GetName (),
      	x, y, but);
      csRef<iPcMesh> pcmesh (
      	CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (), iPcMesh));
      csRef<iPcCamera> pccamera (CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcCamera));
      csRef<iCelPlLayer> pl (csQueryRegistry<iCelPlLayer> (object_reg));
      pcmesh->Hide ();
      iCelEntity* drop_ent = pl->GetHitEntity (pccamera->GetCamera (), x, y);
      pcmesh->Show ();
      if (drop_ent && !strncmp (drop_ent->GetName (), "box", 3))
      {
        csRef<iPcInventory> pcinv (CEL_QUERY_PROPCLASS (
		drop_ent->GetPropertyClassList (), iPcInventory));
        if (pcinv)
	  if (pcinv->AddEntity (ent))
	  {
	    pcmesh->Hide ();
	  }
      }
    }
    else if (!strcmp (msg_id, "pcmeshsel_down"))
      printf ("  DOWN '%s' (%d,%d,%d)\n", ent->GetName (),
      	x, y, but);
  }

  if (butdata && !strcmp (msg_id, "pcmeshsel_move"))
  {
    iCelEntity* ent = params->GetParameter (pl->FetchStringID (
  	  "cel.parameter.entity"))->value.ent;
    csRef<iPcTooltip> pctooltip (CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcTooltip));
    if (ent)
    {
      pctooltip->SetText (ent->GetName ());
      pctooltip->Show (50, 50);
    }
    else
     pctooltip->Hide ();
  }

  fflush (stdout);
#endif
  return false;
}

//---------------------------------------------------------------------------

celBehaviourQuest::celBehaviourQuest (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourQuest::SendMessageV (const char* msg_id,
	iCelPropertyClass*,
	celData&, iCelParameterBlock* params, va_list arg)
{
  return false;
}

//---------------------------------------------------------------------------


celBehaviourBox::celBehaviourBox (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourBox::SendMessageV (const char* msg_id,
	iCelPropertyClass*,
	celData&, iCelParameterBlock* params, va_list arg)
{
#if 1
  (void)arg;
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  iCelEntity* ent = 0;
  if (params)
  {
    const celData* cd = params->GetParameter (
    	pl->FetchStringID ("cel.parameter.entity"));
    if (cd)
      ent = cd->value.ent;
  }

  if (ent && !strcmp (msg_id, "pcmeshsel_down"))
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
  else if (!strcmp (msg_id, "pctimer_wakeup"))
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

#endif
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

bool celBehaviourActor::SendMessageV (const char* msg_id,
	iCelPropertyClass* pc,
	celData& ret, iCelParameterBlock* params, va_list arg)
{
  bool pcinput_msg = strncmp (msg_id, "pccommandinput_", 15) == 0;

  if (pcinput_msg)
  {
    csRef<iPcActorMove> pcactormove = CEL_QUERY_PROPCLASS_ENT (entity,
    	iPcActorMove);
    if (!pcactormove)
      return false;

    if (!strcmp (msg_id+15, "forward1"))
      pcactormove->Forward (true);
    else if (!strcmp (msg_id+15, "forward0"))
      pcactormove->Forward (false);
    else if (!strcmp (msg_id+15, "backward1"))
      pcactormove->Backward (true);
    else if (!strcmp (msg_id+15, "backward0"))
      pcactormove->Backward (false);
    else if (!strcmp (msg_id+15, "strafeleft1"))
      pcactormove->StrafeLeft (true);
    else if (!strcmp (msg_id+15, "strafeleft0"))
      pcactormove->StrafeLeft (false);
    else if (!strcmp (msg_id+15, "straferight1"))
      pcactormove->StrafeRight (true);
    else if (!strcmp (msg_id+15, "straferight0"))
      pcactormove->StrafeRight (false);
    else if (!strcmp (msg_id+15, "rotateleft1"))
      pcactormove->RotateLeft (true);
    else if (!strcmp (msg_id+15, "rotateleft0"))
      pcactormove->RotateLeft (false);
    else if (!strcmp (msg_id+15, "rotateright1"))
      pcactormove->RotateRight (true);
    else if (!strcmp (msg_id+15, "rotateright0"))
      pcactormove->RotateRight (false);
    else if (!strcmp (msg_id+15, "jump1"))
      pcactormove->Jump ();
    else if (!strcmp (msg_id+15, "run1"))
      pcactormove->Run (true);
    else if (!strcmp (msg_id+15, "run0"))
      pcactormove->Run (false);
    else if (!strcmp (msg_id+15, "lookup1"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (1.0f);
    }
    else if (!strcmp (msg_id+15, "lookup0"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+15, "lookdown1"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (-1.0f);
    }
    else if (!strcmp (msg_id+15, "lookdown0"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+15, "center1"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->CenterCamera ();
    }
    else if (!strcmp (msg_id+15, "cammode1"))
    {
      pcactormove->ToggleCameraMode ();
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      if (pcdefcamera)
        printf ("%s\n", pcdefcamera->GetModeName ()); fflush (stdout);
    }
    return true;
  }

  return bhroom->SendMessageV (msg_id, pc, ret, params, arg);
}

//---------------------------------------------------------------------------

celBehaviourDynActor::celBehaviourDynActor (iCelEntity* entity,
    iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

celBehaviourDynActor::~celBehaviourDynActor()
{
}

bool celBehaviourDynActor::SendMessageV (const char* msg_id,
	iCelPropertyClass* pc,
	celData& ret, iCelParameterBlock* params, va_list arg)
{
  bool pcinput_msg = strncmp (msg_id, "pccommandinput_", 15) == 0;

  if (pcinput_msg)
  {
    csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity,
    	iPcMechanicsObject);
    if (!pcmechobj)
      return false;

    if (!strcmp (msg_id+15, "forward1"))
      pcmechobj->AddForceDuration (csVector3 (0, 0, -25.0f), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+15, "backward1"))
      pcmechobj->AddForceDuration (csVector3 (0, 0, 25.0f), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+15, "strafeleft1"))
      pcmechobj->AddForceDuration (csVector3 (25.0f, 0, 0), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+15, "straferight1"))
      pcmechobj->AddForceDuration (csVector3 (-25.0f, 0, 0), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+15, "jump1"))
      pcmechobj->AddForceDuration (csVector3 (0, 25.0f, 0), false,
      	csVector3 (0, 0, 0), .2f);
    else if (!strcmp (msg_id+15, "lookup1"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (1.0f);
    }
    else if (!strcmp (msg_id+15, "lookup0"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+15, "lookdown1"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (-1.0f);
    }
    else if (!strcmp (msg_id+15, "lookdown0"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+15, "center1"))
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
  pl= csQueryRegistry<iCelPlLayer> (object_reg);
  pcmeshdeform = 0;
}

celBehaviourWheeled::~celBehaviourWheeled()
{
}

bool celBehaviourWheeled::SendMessageV (const char* msg_id,
	iCelPropertyClass* pc,
	celData& ret, iCelParameterBlock* params, va_list arg)
{
  bool pcinput_msg = strncmp (msg_id, "pccommandinput_", 15) == 0;
  if (pcinput_msg)
  {
    csRef<iPcWheeled> pcwheeled = CEL_QUERY_PROPCLASS_ENT (entity,
    	iPcWheeled);
    if (!pcwheeled)
      return false;

    if (!strcmp (msg_id+15, "accelerate1"))
	pcwheeled->Accelerate();

//Autoreverse handles putting the car in reverse once it is slow enough.
    else if (!strcmp (msg_id+15, "reverse1"))
	pcwheeled->Brake();

    else if (!strcmp (msg_id+15, "steerleft1"))
	pcwheeled->SteerLeft();

    else if (!strcmp (msg_id+15, "steerright1"))
	pcwheeled->SteerRight();

    else if (!strcmp (msg_id+15, "steerleft0"))
	pcwheeled->SteerStraight();

    else if (!strcmp (msg_id+15, "steerright0"))
	pcwheeled->SteerStraight();

    else if (!strcmp (msg_id+15, "handbrake1"))
	pcwheeled->Handbrake(true);

    else if (!strcmp (msg_id+15, "handbrake0"))
	pcwheeled->Handbrake(false);

    if (!strcmp (msg_id+15, "accelerate0"))
	pcwheeled->Accelerate(0.0f);

    if (!strcmp (msg_id+15, "reverse0"))
    {
	pcwheeled->Brake(0.0f);
    }
    else if (!strcmp (msg_id+15, "lookup1"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (1.0f);
    }
    else if (!strcmp (msg_id+15, "lookup0"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+15, "lookdown1"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (-1.0f);
    }
    else if (!strcmp (msg_id+15, "lookdown0"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+15, "center1"))
    {
      csRef<iPcDefaultCamera> pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity,
      	iPcDefaultCamera);
      pcdefcamera->CenterCamera ();
    }
    return true;
  }
  else if (!strcmp (msg_id, "pcdynamicbody_collision"))
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
