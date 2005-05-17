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

SCF_IMPLEMENT_IBASE (celTriggerTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iTriggerQuestTriggerFactory)
SCF_IMPLEMENT_IBASE_END

celTriggerTriggerFactory::celTriggerTriggerFactory (
	celTriggerTriggerType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celTriggerTriggerFactory::type = type;
  entity_par = 0;
  do_leave = false;
}

celTriggerTriggerFactory::~celTriggerTriggerFactory ()
{
  delete[] entity_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestTrigger> celTriggerTriggerFactory::CreateTrigger (
    const celQuestParams& params)
{
  celTriggerTrigger* trig = new celTriggerTrigger (type,
  	params, entity_par, do_leave);
  return trig;
}

bool celTriggerTriggerFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));

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
	const char* entity)
{
  if (entity_par == entity) 
    return;

  delete[] entity_par;
  entity_par = csStrNew (entity);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celTriggerTrigger)
  SCF_IMPLEMENTS_INTERFACE (iQuestTrigger)
  SCF_IMPLEMENTS_INTERFACE (iPcTriggerListener)
SCF_IMPLEMENT_IBASE_END

celTriggerTrigger::celTriggerTrigger (
	celTriggerTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par,
	bool do_leave)
{
  SCF_CONSTRUCT_IBASE (0);
  celTriggerTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  celTriggerTrigger::do_leave = do_leave;
}

celTriggerTrigger::~celTriggerTrigger ()
{
  DeactivateTrigger ();
  delete[] entity;
  SCF_DESTRUCT_IBASE ();
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
  if (!ent)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
    ent = pl->FindEntity (entity);
  }
}

void celTriggerTrigger::ActivateTrigger ()
{
  FindEntities ();
  if (ent)
  {
    csRef<iPcTrigger> pctrigger = CEL_QUERY_PROPCLASS_ENT (ent, iPcTrigger);
    if (pctrigger)
      pctrigger->AddTriggerListener ((iPcTriggerListener*)this);
  }
}

void celTriggerTrigger::DeactivateTrigger ()
{
  if (ent)
  {
    csRef<iPcTrigger> pctrigger = CEL_QUERY_PROPCLASS_ENT (ent, iPcTrigger);
    if (pctrigger)
      pctrigger->RemoveTriggerListener ((iPcTriggerListener*)this);
  }
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

