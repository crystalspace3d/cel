/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/quests/trig_inventory.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(Inventory)

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celInventoryTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iInventoryQuestTriggerFactory)
SCF_IMPLEMENT_IBASE_END

celInventoryTriggerFactory::celInventoryTriggerFactory (
	celInventoryTriggerType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celInventoryTriggerFactory::type = type;
  entity_par = 0;
  child_entity_par = 0;
}

celInventoryTriggerFactory::~celInventoryTriggerFactory ()
{
  delete[] entity_par;
  delete[] child_entity_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestTrigger> celInventoryTriggerFactory::CreateTrigger (
    const celQuestParams& params)
{
  celInventoryTrigger* trig = new celInventoryTrigger (type,
  	params, entity_par, child_entity_par);
  return trig;
}

bool celInventoryTriggerFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] child_entity_par; child_entity_par = 0;

  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.inventory",
      "'entity' attribute is missing for the inventory trigger!");
    return false;
  }
  child_entity_par = csStrNew (node->GetAttributeValue ("child_entity"));
  if (!child_entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.inventory",
      "'child_entity' attribute is missing for the inventory trigger!");
    return false;
  }
  return true;
}

void celInventoryTriggerFactory::SetEntityParameter (
	const char* entity)
{
  if (entity_par == entity) 
    return;

  delete[] entity_par;
  entity_par = csStrNew (entity);
}

void celInventoryTriggerFactory::SetChildEntityParameter (
	const char* entity)
{
  if (child_entity_par == entity) 
    return;

  delete[] child_entity_par;
  child_entity_par = csStrNew (entity);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celInventoryTrigger)
  SCF_IMPLEMENTS_INTERFACE (iQuestTrigger)
  SCF_IMPLEMENTS_INTERFACE (iPcInventoryListener)
SCF_IMPLEMENT_IBASE_END

celInventoryTrigger::celInventoryTrigger (
	celInventoryTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* child_entity_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celInventoryTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  child_entity = csStrNew (qm->ResolveParameter (params, child_entity_par));
}

celInventoryTrigger::~celInventoryTrigger ()
{
  DeactivateTrigger ();
  delete[] entity;
  delete[] child_entity;
  SCF_DESTRUCT_IBASE ();
}

void celInventoryTrigger::RegisterCallback (iQuestTriggerCallback* callback)
{
  celInventoryTrigger::callback = callback;
}

void celInventoryTrigger::ClearCallback ()
{
  callback = 0;
}

void celInventoryTrigger::FindInventory ()
{
  if (inventory) return;
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return;
  inventory = CEL_QUERY_PROPCLASS_ENT (ent, iPcInventory);
}

void celInventoryTrigger::ActivateTrigger ()
{
  FindInventory ();
  if (!inventory) return;	// @@@ Report error!
  // First remove to make sure we don't register ourselves multiple
  // times.
  inventory->RemoveInventoryListener ((iPcInventoryListener*)this);
  inventory->AddInventoryListener ((iPcInventoryListener*)this);
}

void celInventoryTrigger::DeactivateTrigger ()
{
  if (!inventory) return;
  inventory->RemoveInventoryListener ((iPcInventoryListener*)this);
}

bool celInventoryTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celInventoryTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

void celInventoryTrigger::AddChild (iPcInventory* inventory, iCelEntity* entity)
{
  if (entity->GetName () && strcmp (child_entity, entity->GetName ()) == 0)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
  }
}

void celInventoryTrigger::RemoveChild (iPcInventory*, iCelEntity*)
{
}

//---------------------------------------------------------------------------

