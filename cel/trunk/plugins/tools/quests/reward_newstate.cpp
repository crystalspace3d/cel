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
#include "propclass/quest.h"

#include "plugins/tools/quests/reward_newstate.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(NewState)

//---------------------------------------------------------------------------

celNewStateRewardFactory::celNewStateRewardFactory (
	celNewStateRewardType* type) : scfImplementationType (this)
{
  celNewStateRewardFactory::type = type;
}

csPtr<iQuestReward> celNewStateRewardFactory::CreateReward (
    iQuest* q, const celQuestParams& params)
{
  iQuestReward* reward;
  if (!class_par.IsEmpty())
  {
    reward = new celClassNewStateReward (type,
  	params, state_par, class_par, tag_par);
  }
  else
  {
    reward = new celNewStateReward (type,
  	q, params, state_par, entity_par, tag_par);
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
	celNewStateRewardType* type, iQuest* q,
  	const celQuestParams& params,
	const char* state_par,
	const char* entity_par, const char* tag_par)
	: scfImplementationType (this)
{
  celNewStateReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  state = qm->ResolveParameter (params, state_par);
  entity = qm->ResolveParameter (params, entity_par);
  tag = qm->ResolveParameter (params, tag_par);
  if (!entity && !tag)
    quest = q;
}

void celNewStateReward::Reward ()
{
  if (!quest)
  {
    if (!ent)
    {
      iCelPlLayer* pl = type->pl;
      ent = pl->FindEntity (entity);
      if (!ent) return;
    }
    csWeakRef<iPcQuest> pcquest = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcQuest,
    	tag);
    if (!pcquest) return;
    quest = pcquest->GetQuest ();
    if (!quest) return;
  }
  quest->SwitchState (state);
}

//---------------------------------------------------------------------------

celClassNewStateReward::celClassNewStateReward (
	celNewStateRewardType* type,
  	const celQuestParams& params,
	const char* state_par,
	const char* class_par, const char* tag_par)
	: scfImplementationType (this)
{
  csPrintf("new class state reward %s\n!",state_par);
  celClassNewStateReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);

  csStringID classid = 
	type->pl->FetchStringID(qm->ResolveParameter (params, class_par));
  entlist = type->pl->GetClassEntitiesList(classid);

  state = qm->ResolveParameter (params, state_par);
  tag = qm->ResolveParameter (params, tag_par);
}

void celClassNewStateReward::Reward ()
{
  iCelEntity *ent;
  for (int i = entlist->GetCount()-1; i>=0; i--)
  {
    ent = entlist->Get(i);
    csRef<iPcQuest> pcquest = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcQuest,tag);
    if (pcquest)
      pcquest->GetQuest()->SwitchState (state);
  }
}

//---------------------------------------------------------------------------
