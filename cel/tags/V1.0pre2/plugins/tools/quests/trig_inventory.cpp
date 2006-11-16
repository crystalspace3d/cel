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

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.quests.trigger.inventory", msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

//---------------------------------------------------------------------------

celInventoryTriggerFactory::celInventoryTriggerFactory (
	celInventoryTriggerType* type) : scfImplementationType (this)
{
  celInventoryTriggerFactory::type = type;
  entity_par = 0;
  tag_par = 0;
  child_entity_par = 0;
}

celInventoryTriggerFactory::~celInventoryTriggerFactory ()
{
  delete[] entity_par;
  delete[] child_entity_par;
  delete[] tag_par;
}

csPtr<iQuestTrigger> celInventoryTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  celInventoryTrigger* trig = new celInventoryTrigger (type,
  	params, entity_par, tag_par, child_entity_par);
  return trig;
}

bool celInventoryTriggerFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] child_entity_par; child_entity_par = 0;

  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));
  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the inventory trigger!");
  child_entity_par = csStrNew (node->GetAttributeValue ("child_entity"));
  return true;
}

void celInventoryTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
  if (tag_par != tag)
  {
    delete[] tag_par;
    tag_par = csStrNew (tag);
  }
}

void celInventoryTriggerFactory::SetChildEntityParameter (
	const char* entity)
{
  if (child_entity_par != entity)
  {
    delete[] child_entity_par;
    child_entity_par = csStrNew (entity);
  }
}

//---------------------------------------------------------------------------

celInventoryTrigger::celInventoryTrigger (
	celInventoryTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* child_entity_par) : scfImplementationType (this)
{
  celInventoryTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  if (child_entity_par)
    child_entity = csStrNew (qm->ResolveParameter (params, child_entity_par));
  else
    child_entity = 0;
}

celInventoryTrigger::~celInventoryTrigger ()
{
  DeactivateTrigger ();
  delete[] entity;
  delete[] child_entity;
  delete[] tag;
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
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return;
  inventory = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcInventory, tag);
}

void celInventoryTrigger::ActivateTrigger ()
{
  FindInventory ();
  if (!inventory)
  {
    Report (type->object_reg, "Can't find pcinventory for inventory trigger!");
    return;
  }
  // First remove to make sure we don't register ourselves multiple
  // times.
  inventory->RemoveInventoryListener ((iPcInventoryListener*)this);
  inventory->AddInventoryListener ((iPcInventoryListener*)this);
}

bool celInventoryTrigger::Check ()
{
  if (!inventory) return false;
  if (!child_entity) return false;
  size_t i;
  for (i = 0 ; i < inventory->GetEntityCount () ; i++)
  {
    iCelEntity* ent = inventory->GetEntity (i);
    if (ent->GetName () && strcmp (child_entity, ent->GetName ()) == 0)
      return true;
  }
  return false;
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
  if (!child_entity)
  {
    // Always fire in case we're not monitoring a specific entity.
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
    return;
  }
  if (entity->GetName () && strcmp (child_entity, entity->GetName ()) == 0)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
  }
}

void celInventoryTrigger::RemoveChild (iPcInventory*, iCelEntity*)
{
  if (!child_entity)
  {
    // Always fire in case we're not monitoring a specific entity.
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
    return;
  }
}

//---------------------------------------------------------------------------

