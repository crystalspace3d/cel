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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "iutil/objreg.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/propclas.h"
#include "pf/mesh.h"
#include "pf/meshsel.h"
#include "pf/tooltip.h"
#include "pf/camera.h"
#include "pf/inv.h"
#include "pf/gravity.h"
#include "pf/timer.h"
#include "bltest/behave.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celBehaviourGeneral)
  SCF_IMPLEMENTS_INTERFACE (iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

celBehaviourGeneral::celBehaviourGeneral (iCelEntity* entity,
	iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  celBehaviourGeneral::entity = entity;
  celBehaviourGeneral::object_reg = object_reg;
  name = NULL;
}

celBehaviourGeneral::~celBehaviourGeneral ()
{
  delete[] name;
}

bool celBehaviourGeneral::SendMessage (const char* msg_id, iBase* msg_info, ...)
{
  va_list arg;
  va_start (arg, msg_info);
  bool rc = SendMessageV (msg_id, msg_info, arg);
  va_end (arg);
  return rc;
}

//---------------------------------------------------------------------------

celBehaviourPrinter::celBehaviourPrinter (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourPrinter::SendMessageV (const char* msg_id, iBase* msg_info,
	va_list arg)
{
  (void)arg; (void)msg_info;
  printf ("Got message '%s'\n", msg_id);
  return false;
}

//---------------------------------------------------------------------------

celBehaviourRoom::celBehaviourRoom (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourRoom::SendMessageV (const char* msg_id, iBase* msg_info,
	va_list arg)
{
  (void)arg;
  iPcMeshSelectData* dat = NULL;
  if (msg_info) dat = SCF_QUERY_INTERFACE_FAST (msg_info,
    	iPcMeshSelectData);
  int x, y, but;
  iCelEntity* ent = NULL;
  if (dat)
  {
    ent = dat->GetEntity ();
    dat->GetMousePosition (x, y);
    but = dat->GetMouseButton ();
  }
  if (ent)
  {
    if (!strcmp (msg_id, "selectmesh_up"))
    {
      printf ("  UP '%s' (%d,%d,%d)\n", ent->GetName (),
      	x, y, dat->GetMouseButton ());
      iPcMesh* pcmesh = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
      	iPcMesh);
      iPcCamera* pccamera = CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcCamera);
      iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      pcmesh->Hide ();
      iCelEntity* drop_ent = pl->GetHitEntity (pccamera->GetCamera (), x, y);
      pcmesh->Show ();
      pl->DecRef ();
      pccamera->DecRef ();
      if (drop_ent && !strcmp (drop_ent->GetName (), "box"))
      {
        iPcInventory* pcinv = CEL_QUERY_PROPCLASS (
		drop_ent->GetPropertyClassList (), iPcInventory);
        if (pcinv)
	{
	  if (pcinv->AddEntity (ent))
	  {
	    pcmesh->Hide ();
	  }
	  pcinv->DecRef ();
	}
      }
      pcmesh->DecRef ();
    }
    else if (!strcmp (msg_id, "selectmesh_down"))
      printf ("  DOWN '%s' (%d,%d,%d)\n", ent->GetName (),
      	x, y, dat->GetMouseButton ());
  }

  if (dat && !strcmp (msg_id, "selectmesh_move"))
  {
    iPcTooltip* pctooltip = CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcTooltip);
    if (ent)
    {
      pctooltip->SetText (ent->GetName ());
      pctooltip->Show (50, 50);
    }
    else
     pctooltip->Hide ();
    pctooltip->DecRef ();
  }

  if (dat) dat->DecRef ();
  fflush (stdout);
  return false;
}

//---------------------------------------------------------------------------

celBehaviourBox::celBehaviourBox (iCelEntity* entity,
	iObjectRegistry* object_reg) : celBehaviourGeneral (entity, object_reg)
{
}

bool celBehaviourBox::SendMessageV (const char* msg_id, iBase* msg_info,
	va_list arg)
{
  (void)arg;
  iPcMeshSelectData* dat = NULL;
  if (msg_info) dat = SCF_QUERY_INTERFACE_FAST (msg_info,
    	iPcMeshSelectData);
  iCelEntity* ent = NULL;
  if (dat) ent = dat->GetEntity ();
  if (ent && !strcmp (msg_id, "selectmesh_down"))
  {
    iPcMesh* pcmesh = CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcMesh);
    CS_ASSERT (pcmesh != NULL);
    const char* curact = pcmesh->GetAction ();
    if (!strcmp (curact, "open"))
      pcmesh->SetAction ("closed");
    else
    {
      pcmesh->SetAction ("open");
      // If the box is opened we remove everything from it.
      iPcTimer* pctimer = CEL_QUERY_PROPCLASS (
		entity->GetPropertyClassList (), iPcTimer);
      CS_ASSERT (pctimer != NULL);
      pctimer->WakeUp (200, false);
      pctimer->DecRef ();
    }
    pcmesh->DecRef ();
  }
  else if (!strcmp (msg_id, "timer_wakeup"))
  {
    iPcTimer* pctimer = CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcTimer);
    CS_ASSERT (pctimer != NULL);
    iPcMesh* pcmesh = CEL_QUERY_PROPCLASS (
      	entity->GetPropertyClassList (), iPcMesh);
    CS_ASSERT (pcmesh != NULL);
    // Remove one entity from the box.
    iPcInventory* pcinv = CEL_QUERY_PROPCLASS (
		entity->GetPropertyClassList (), iPcInventory);
    CS_ASSERT (pcinv != NULL);
    if (pcinv->GetEntityCount () > 0)
    {
      iCelEntity* inv_ent = pcinv->GetEntity (0);
      iPcGravity* inv_ent_gravity = CEL_QUERY_PROPCLASS (
	    	inv_ent->GetPropertyClassList (), iPcGravity);
      iPcMesh* inv_ent_mesh = CEL_QUERY_PROPCLASS (
	    	inv_ent->GetPropertyClassList (), iPcMesh);
      if (inv_ent_mesh)
      {
	inv_ent_mesh->Show ();
	inv_ent_mesh->MoveMesh (pcmesh->GetMesh ()->GetMovable ()->
	      	GetSectors ()->Get (0), pcmesh->GetMesh ()->GetMovable ()->
		GetTransform ().GetOrigin ()+csVector3 (0, 1.3, 0));
	if (inv_ent_gravity)
	{
	  inv_ent_gravity->ClearForces ();
	  float dx = 29 + 34*float ((rand () >> 3) % 10000) / 10000.;
	  if (((rand () >> 3) & 1) == 0) dx = -dx;
	  float dy = 29 + 34*float ((rand () >> 3) % 10000) / 10000.;
	  if (((rand () >> 3) & 1) == 0) dy = -dy;
	  printf ("%g,%g,%g\n", dx, 2.0, dy); fflush (stdout);
	  inv_ent_gravity->ApplyForce (csVector3 (dx, 40, dy), 800);
	}
	inv_ent_mesh->DecRef ();
      }
      if (inv_ent_gravity) inv_ent_gravity->DecRef ();
      pcinv->RemoveEntity (inv_ent);
    }
    if (pcinv->GetEntityCount () > 0)
    {
      // Restart timer.
      pctimer->WakeUp (200, false);
    }
    pcinv->DecRef ();
    pcmesh->DecRef ();
    pctimer->DecRef ();
  }

  if (dat) dat->DecRef ();
  return false;
}

//---------------------------------------------------------------------------

