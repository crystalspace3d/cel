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
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/propclas.h"
#include "pf/mesh.h"
#include "pf/meshsel.h"
#include "pf/tooltip.h"
#include "pf/camera.h"
#include "pf/inv.h"
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
}

celBehaviourGeneral::~celBehaviourGeneral ()
{
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
    if (pcmesh)
    {
      const char* curact = pcmesh->GetAction ();
      if (!strcmp (curact, "open"))
        pcmesh->SetAction ("closed");
      else
        pcmesh->SetAction ("open");
      pcmesh->DecRef ();
    }
  }

  if (dat) dat->DecRef ();
  return false;
}

//---------------------------------------------------------------------------

