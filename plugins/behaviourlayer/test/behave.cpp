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
#include "propclass/inv.h"
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "plugins/behaviourlayer/test/behave.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celBehaviourGeneral)
  SCF_IMPLEMENTS_INTERFACE (iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

celBehaviourGeneral::celBehaviourGeneral (iCelEntity* entity,
	iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (0);
  celBehaviourGeneral::entity = entity;
  celBehaviourGeneral::object_reg = object_reg;
  name = 0;
}

celBehaviourGeneral::~celBehaviourGeneral ()
{
  delete[] name;
}

bool celBehaviourGeneral::SendMessage (const char* msg_id,
	iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, params, arg);
  va_end (arg);
  return rc;
}

//---------------------------------------------------------------------------

celBehaviourPrinter::celBehaviourPrinter (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourPrinter::SendMessageV (const char* msg_id,
	iCelParameterBlock* params, va_list arg)
{
  (void)arg; (void)params;
  printf ("Got message '%s'\n", msg_id);
  return false;
}

//---------------------------------------------------------------------------

celBehaviourRoom::celBehaviourRoom (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourRoom::SendMessageV (const char* msg_id,
	iCelParameterBlock* params, va_list arg)
{
  (void)arg;
  // @@@ Should store the id's for the parameters below.
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  const celData* butdata = params ? params->GetParameter (pl->FetchStringID (
  	"cel.behaviour.parameter.button")) : 0;
  if (butdata)
  {
    iCelEntity* ent = params->GetParameter (pl->FetchStringID (
  	  "cel.behaviour.parameter.entity"))->value.ent;
    int x = params->GetParameter (pl->FetchStringID (
  	  "cel.behaviour.parameter.x"))->value.l;
    int y = params->GetParameter (pl->FetchStringID (
  	  "cel.behaviour.parameter.y"))->value.l;
    int but = butdata->value.l;
    if (!strcmp (msg_id, "pcmeshsel_up"))
    {
      printf ("  UP '%s' (%d,%d,%d)\n", ent->GetName (),
      	x, y, but);
      csRef<iPcMesh> pcmesh (
      	CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (), iPcMesh));
      csRef<iPcCamera> pccamera (CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcCamera));
      csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
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
  	  "cel.behaviour.parameter.entity"))->value.ent;
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
  return false;
}

//---------------------------------------------------------------------------

celBehaviourBox::celBehaviourBox (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourBox::SendMessageV (const char* msg_id,
	iCelParameterBlock* params, va_list arg)
{
  (void)arg;
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelEntity* ent = params ? params->GetParameter (pl->FetchStringID (
  	"cel.behaviour.parameter.entity"))->value.ent : 0;

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
		GetTransform ().GetOrigin ()+csVector3 (0, 1.3, 0));
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

bool celBehaviourActor::SendMessageV (const char* msg_id,
	iCelParameterBlock* params, va_list arg)
{
  bool pcinput_msg = strncmp (msg_id, "pckeyinput_", 11) == 0;

  if (pcinput_msg)
  {
    csRef<iPcGravity> pcgravity (
    	CEL_QUERY_PROPCLASS (entity->GetPropertyClassList(), iPcGravity));
    if (!pcgravity)
      return false;

    if (!strcmp (msg_id+11, "forward1"))
    {
      pcgravity->ApplyForce(csVector3(0,0,-1*speed), 100000);
    }
    else if (!strcmp (msg_id+11, "forward0"))
    {
      pcgravity->ResetSpeed();
      pcgravity->ClearForces();
    }
    else if (!strcmp (msg_id+11, "backward1"))
    {
      pcgravity->ApplyForce(csVector3(0,0,1*speed), 1000000);
    }
    else if (!strcmp (msg_id+11, "backward0"))
    {
      pcgravity->ResetSpeed();
      pcgravity->ClearForces();
    }
    else if (!strcmp (msg_id+11, "strafeleft1"))
    {
      pcgravity->ApplyForce(csVector3(-1*speed,0,0), 100000);
    }
    else if (!strcmp (msg_id+11, "strafeleft0"))
    {
      pcgravity->ResetSpeed();
      pcgravity->ClearForces();
    }
    else if (!strcmp (msg_id+11, "straferight1"))
    {
      pcgravity->ApplyForce(csVector3(1*speed,0,0), 100000);
    }
    else if (!strcmp (msg_id+11, "straferight0"))
    {
      pcgravity->ResetSpeed();
      pcgravity->ClearForces();
    }
    else if (!strcmp (msg_id+11, "run1"))
    {
      speed=2.5;
    }
    else if (!strcmp (msg_id+11, "run0"))
    {
      speed=1;
    }
    else if (!strcmp (msg_id+11, "cammode1"))
    {
      fpscam = fpscam ? 0 : 1;
      csRef<iPcCamera> pccam (
      	CEL_QUERY_PROPCLASS(entity->GetPropertyClassList(), iPcCamera));
      if (!pccam)
      {
        return false;
      }

      if (fpscam)
      {
        printf ("Switching to 3rd person view!\n");
        pccam->SetMode (iPcCamera::follow, true);
	pccam->SetFollowPos (csVector3(0,0,-1), csVector3(0,0,0));
      }
      else
      {
        printf ("Free look mode\n");
        pccam->SetMode (iPcCamera::freelook, false);
      }
    }
  }

  return bhroom->SendMessageV (msg_id, params, arg);
}

