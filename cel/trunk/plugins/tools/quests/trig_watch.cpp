/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein

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

#include "plugins/tools/quests/trig_watch.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(Watch)

//---------------------------------------------------------------------------

celWatchTriggerFactory::celWatchTriggerFactory (
	celWatchTriggerType* type) : scfImplementationType (this)
{
  celWatchTriggerFactory::type = type;
}

celWatchTriggerFactory::~celWatchTriggerFactory ()
{
}

csPtr<iQuestTrigger> celWatchTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  celWatchTrigger* trig = new celWatchTrigger (type, params,
  	entity_par, tag_par, target_entity_par, target_tag_par,
	time_par);
  return trig;
}

bool celWatchTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_tag");
  if (entity_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.watch",
      "'entity' attribute is missing for the watch trigger!");
    return false;
  }

  target_entity_par = node->GetAttributeValue ("target");
  target_tag_par = node->GetAttributeValue ("target_tag");
  if (target_entity_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.watch",
      "'target' attribute is missing for the watch trigger!");
    return false;
  }

  time_par = node->GetAttributeValue ("checktime");

  return true;
}

void celWatchTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celWatchTriggerFactory::SetTargetEntityParameter (
	const char* entity, const char* tag)
{
  target_entity_par = entity;
  target_tag_par = tag;
}

void celWatchTriggerFactory::SetChecktimeParameter (
	const char* time)
{
  time_par = time;
}

//---------------------------------------------------------------------------

celWatchTrigger::celWatchTrigger (
	celWatchTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* target_entity_par, const char* target_tag_par,
	const char* time_par) : scfImplementationType (this)
{
  celWatchTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = qm->ResolveParameter (params, entity_par);
  tag = qm->ResolveParameter (params, tag_par);
  target_entity = qm->ResolveParameter (params, target_entity_par);
  target_tag = qm->ResolveParameter (params, target_tag_par);
  const char* t = qm->ResolveParameter (params, time_par);
  if (t)
    sscanf (t, "%d", &time);
  else
    time = 1000;
}

celWatchTrigger::~celWatchTrigger ()
{
  DeactivateTrigger ();
}

void celWatchTrigger::RegisterCallback (iQuestTriggerCallback* callback)
{
  celWatchTrigger::callback = callback;
}

void celWatchTrigger::ClearCallback ()
{
  callback = 0;
}

void celWatchTrigger::FindEntities ()
{
#if 0
  if (!pctrigger)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
    iCelEntity* ent = pl->FindEntity (entity);
    if (!ent) return;
    pctrigger = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcTrigger, tag);
  }
#endif
}

void celWatchTrigger::ActivateTrigger ()
{
  FindEntities ();
  //if (pctrigger)
    //pctrigger->AddTriggerListener ((iPcTriggerListener*)this);
}

bool celWatchTrigger::Check ()
{
  //if (!pctrigger) return false;
  //return pctrigger->Check ();
  return false;
}

void celWatchTrigger::DeactivateTrigger ()
{
  //if (pctrigger)
    //pctrigger->RemoveTriggerListener ((iPcTriggerListener*)this);
}

bool celWatchTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celWatchTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

//---------------------------------------------------------------------------

