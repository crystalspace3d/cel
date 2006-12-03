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
  state_par = 0;
  entity_par = 0;
  tag_par = 0;
}

celNewStateRewardFactory::~celNewStateRewardFactory ()
{
  delete[] state_par;
  delete[] entity_par;
  delete[] tag_par;
}

csPtr<iQuestReward> celNewStateRewardFactory::CreateReward (
    iQuest* q, const celQuestParams& params)
{
  celNewStateReward* trig = new celNewStateReward (type,
  	q, params, state_par, entity_par, tag_par);
  return trig;
}

bool celNewStateRewardFactory::Load (iDocumentNode* node)
{
  delete[] state_par; state_par = 0;
  delete[] entity_par; entity_par = 0;
  state_par = csStrNew (node->GetAttributeValue ("state"));
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));

  if (!state_par)
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
  if (state_par == state) 
    return;

  delete[] state_par;
  state_par = csStrNew (state);
}

void celNewStateRewardFactory::SetEntityParameter (const char* entity,
	const char* tag)
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

celNewStateReward::celNewStateReward (
	celNewStateRewardType* type, iQuest* q,
  	const celQuestParams& params,
	const char* state_par,
	const char* entity_par, const char* tag_par)
	: scfImplementationType (this)
{
  celNewStateReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  state = csStrNew (qm->ResolveParameter (params, state_par));
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  quest = q;
}

celNewStateReward::~celNewStateReward ()
{
  delete[] state;
  delete[] entity;
  delete[] tag;
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

