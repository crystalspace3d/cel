/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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
#include "propclass/mesh.h"

#include "plugins/tools/quests/trig_trigger.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(Trigger)

//---------------------------------------------------------------------------

celTriggerTriggerFactory::celTriggerTriggerFactory (
	celTriggerTriggerType* type) : scfImplementationType (this)
{
  celTriggerTriggerFactory::type = type;
  entity_par = 0;
  tag_par = 0;
  do_leave = false;
}

celTriggerTriggerFactory::~celTriggerTriggerFactory ()
{
  delete[] entity_par;
  delete[] tag_par;
}

csPtr<iQuestTrigger> celTriggerTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  celTriggerTrigger* trig = new celTriggerTrigger (type, params,
  	entity_par, tag_par, do_leave);
  return trig;
}

bool celTriggerTriggerFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));

  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.trigger",
      "'entity' attribute is missing for the trigger trigger!");
    return false;
  }

  if (node->GetAttributeValue ("leave"))
    do_leave = true;

  return true;
}

void celTriggerTriggerFactory::SetEntityParameter (
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

//---------------------------------------------------------------------------

celTriggerTrigger::celTriggerTrigger (
	celTriggerTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	bool do_leave) : scfImplementationType (this)
{
  celTriggerTrigger::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  celTriggerTrigger::do_leave = do_leave;
}

celTriggerTrigger::~celTriggerTrigger ()
{
  DeactivateTrigger ();
  delete[] entity;
  delete[] tag;
}

void celTriggerTrigger::RegisterCallback (iQuestTriggerCallback* callback)
{
  celTriggerTrigger::callback = callback;
}

void celTriggerTrigger::ClearCallback ()
{
  callback = 0;
}

void celTriggerTrigger::EntityEnters (iPcTrigger* trigger, iCelEntity* entity)
{
  if (!do_leave)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
  }
}

void celTriggerTrigger::EntityLeaves (iPcTrigger* trigger, iCelEntity* entity)
{
  if (do_leave)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
  }
}

void celTriggerTrigger::EnterTrigger (iPcTrigger*, iCelEntity*)
{
}

void celTriggerTrigger::LeaveTrigger (iPcTrigger*, iCelEntity*)
{
}

void celTriggerTrigger::FindEntities ()
{
  if (!pctrigger)
  {
    iCelPlLayer* pl = type->pl;
    iCelEntity* ent = pl->FindEntity (entity);
    if (!ent) return;
    pctrigger = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcTrigger, tag);
  }
}

void celTriggerTrigger::ActivateTrigger ()
{
  FindEntities ();
  if (pctrigger)
    pctrigger->AddTriggerListener ((iPcTriggerListener*)this);
}

bool celTriggerTrigger::Check ()
{
  if (!pctrigger) return false;
  return pctrigger->Check ();
}

void celTriggerTrigger::DeactivateTrigger ()
{
  if (pctrigger)
    pctrigger->RemoveTriggerListener ((iPcTriggerListener*)this);
}

bool celTriggerTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celTriggerTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

//---------------------------------------------------------------------------

