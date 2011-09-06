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
#include "propclass/quest.h"

#include "plugins/tools/quests/quests.h"
#include "plugins/tools/quests/reward_newstate.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_QUESTREWARDTYPE(NewState)

//---------------------------------------------------------------------------

celNewStateRewardFactory::celNewStateRewardFactory (
	celNewStateRewardType* type) : scfImplementationType (this)
{
  celNewStateRewardFactory::type = type;
}

csPtr<iReward> celNewStateRewardFactory::CreateReward (
    const celParams& params)
{
  iReward* reward;
  if (!class_par.IsEmpty())
  {
    reward = new celClassNewStateReward (type,
  	params, state_par, class_par, tag_par);
  }
  else
  {
    reward = new celNewStateReward (type,
  	//q, 
	params, state_par, entity_par, tag_par);
  }
  return reward;
}

bool celNewStateRewardFactory::Load (iDocumentNode* node)
{
  state_par = node->GetAttributeValue ("state");
  entity_par = node->GetAttributeValue ("entity");
  class_par = node->GetAttributeValue ("class");
  tag_par = node->GetAttributeValue ("entity_tag");

  if (state_par.IsEmpty())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.debugprint",
      "'state' attribute is missing for the newstate reward!");
    return false;
  }
  return true;
}

void celNewStateRewardFactory::SetStateParameter (const char* state)
{
  state_par = state;
}

void celNewStateRewardFactory::SetEntityParameter (const char* entity,
	const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

//---------------------------------------------------------------------------

celNewStateReward::celNewStateReward (
	celNewStateRewardType* type, //iQuest* q,
  	const celParams& params,
	const char* state_par,
	const char* entity_par, const char* tag_par)
	: scfImplementationType (this)
{
  celNewStateReward::type = type;
  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  state = pm->GetParameter (params, state_par);
  if (entity_par) entity = pm->GetParameter (params, entity_par);
  if (tag_par) tag = pm->GetParameter (params, tag_par);
  //if (!entity_par && !tag_par)
  //  quest = q;
}

void celNewStateReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = type->pl;
  reward_params = params;
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_POST);
}

void celNewStateReward::TickEveryFrame ()
{
  iCelPlLayer* pl = type->pl;
  csRef<iCelParameterBlock> params = reward_params;
  reward_params = 0;  // Clear ref at exit.
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_POST);

  if (!quest)
  {
    bool changed;
    const char* e = entity->Get (params, changed);
    if (changed) ent = 0;
    if (!ent)
    {
      ent = pl->FindEntity (e);
      if (!ent) return;
    }
    const char* t = 0;
    if (tag) 
      t = tag->Get (params);
    csWeakRef<iPcQuest> pcquest = celQueryPropertyClassTagEntity<iPcQuest> (ent, t);
    if (!pcquest) return;
    quest = pcquest->GetQuest ();
    if (!quest) return;
  }
  const char* st = state->Get (params);
  quest->SwitchState (st);
  if (entity || tag)
    quest = 0;
}

//---------------------------------------------------------------------------

celClassNewStateReward::celClassNewStateReward (
	celNewStateRewardType* type,
  	const celParams& params,
	const char* state_par,
	const char* class_par, const char* tag_par)
	: scfImplementationType (this)
{
  csPrintf("new class state reward %s\n!",state_par);
  celClassNewStateReward::type = type;
  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  state = pm->GetParameter (params, state_par);
  tag = pm->GetParameter (params, tag_par);
  clazz = pm->GetParameter (params, class_par);
}

void celClassNewStateReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = type->pl;
  reward_params = params;
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_POST);
}

void celClassNewStateReward::TickEveryFrame ()
{
  iCelPlLayer* pl = type->pl;
  csRef<iCelParameterBlock> params = reward_params;
  reward_params = 0;  // Clear ref at exit.
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_POST);

  bool changed;
  const char* clz = clazz->Get (params, changed);
  if (changed || !entlist)
  {
    csStringID ent_class = pl->FetchStringID (clz);
    entlist = pl->GetClassEntitiesList (ent_class);
  }

  const char* st = state->Get (params);
  if (!st) return;

  const char* t = 0;
  if (tag)
    t = tag->Get (params);

  iCelEntity *ent;
  size_t i = entlist->GetCount();
  while (i>0)
  {
    i--;
    ent = entlist->Get(i);
    csRef<iPcQuest> pcquest = celQueryPropertyClassTagEntity<iPcQuest> (ent, t);
    if (pcquest)
      pcquest->GetQuest()->SwitchState (st);
  }
}

//---------------------------------------------------------------------------

