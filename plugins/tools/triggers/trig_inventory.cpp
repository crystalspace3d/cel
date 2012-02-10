/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin

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
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "celtool/stdparams.h"
#include "plugins/tools/triggers/trig_inventory.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celInventoryTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE(Inventory)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.triggers.inventory", msg, arg);
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
}

celInventoryTriggerFactory::~celInventoryTriggerFactory ()
{
}

csPtr<iTrigger> celInventoryTriggerFactory::CreateTrigger (
    iQuest* q, iCelParameterBlock* params)
{
  celInventoryTrigger* trig = new celInventoryTrigger (type,
  	params, entity_par, tag_par, child_entity_par);
  return trig;
}

bool celInventoryTriggerFactory::Save (iDocumentNode* node)
{
  if (!entity_par.IsEmpty ()) node->SetAttribute ("entity", entity_par);
  if (!tag_par.IsEmpty ()) node->SetAttribute ("entity_tag", tag_par);
  if (!child_entity_par.IsEmpty ()) node->SetAttribute ("child_entity", child_entity_par);
  return true;
}

bool celInventoryTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_tag");
  if (entity_par.IsEmpty ())
    return Report (type->object_reg,
      "'entity' attribute is missing for the inventory trigger!");
  child_entity_par = node->GetAttributeValue ("child_entity");
  return true;
}

void celInventoryTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celInventoryTriggerFactory::SetChildEntityParameter (
	const char* entity)
{
  child_entity_par = entity;
}

//---------------------------------------------------------------------------

celInventoryTrigger::celInventoryTrigger (
	celInventoryTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	const char* child_entity_par) : scfImplementationType (this)
{
  celInventoryTrigger::type = type;

  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->ResolveEntityParameter (params, entity_par, entityID);
  tag = pm->ResolveParameter (params, tag_par);
  if (child_entity_par)
    child_entity = pm->ResolveParameter (params, child_entity_par);
  params_entity.AttachNew (new celOneParameterBlock ());
  params_entity->SetParameterDef (type->pl->FetchStringID ("child"));
}

celInventoryTrigger::~celInventoryTrigger ()
{
  DeactivateTrigger ();
}

void celInventoryTrigger::RegisterCallback (iTriggerCallback* callback)
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
  iCelEntity* ent;
  if (!entity.IsEmpty ())
    ent = pl->FindEntity (entity);
  else
    ent = pl->GetEntity (entityID);
  if (!ent) return;
  inventory = celQueryPropertyClassTagEntity<iPcInventory> (ent, tag);
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

void celInventoryTrigger::FireTrigger (iCelEntity* child)
{
  DeactivateTrigger ();
  params_entity->GetParameter (0).Set (child->GetName ());
  callback->TriggerFired ((iTrigger*)this, params_entity);
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
    {
      FireTrigger (ent);
      return true;
    }
  }
  return false;
}

void celInventoryTrigger::DeactivateTrigger ()
{
  if (!inventory) return;
  inventory->RemoveInventoryListener ((iPcInventoryListener*)this);
}

void celInventoryTrigger::AddChild (iPcInventory* inventory, iCelEntity* entity)
{
  if (!child_entity)
  {
    // Always fire in case we're not monitoring a specific entity.
    FireTrigger (entity);
    return;
  }
  if (entity->GetName () && strcmp (child_entity, entity->GetName ()) == 0)
  {
    FireTrigger (entity);
    callback->TriggerFired ((iTrigger*)this, 0);
  }
}

void celInventoryTrigger::RemoveChild (iPcInventory*, iCelEntity* entity)
{
  if (!child_entity)
  {
    // Always fire in case we're not monitoring a specific entity.
    FireTrigger (entity);
    return;
  }
}

void celInventoryTrigger::AddChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount)
{
  // @@@ TODO! Support for templates!
}

void celInventoryTrigger::RemoveChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount)
{
  // @@@ TODO! Support for templates!
}

//---------------------------------------------------------------------------

