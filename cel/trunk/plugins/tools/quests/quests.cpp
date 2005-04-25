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

#include "plugins/tools/quests/quests.h"
#include "plugins/tools/quests/trig_entersector.h"
#include "plugins/tools/quests/trig_meshentersector.h"
#include "plugins/tools/quests/trig_timeout.h"
#include "plugins/tools/quests/reward_debugprint.h"
#include "plugins/tools/quests/reward_newstate.h"
#include "plugins/tools/quests/reward_changeproperty.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celQuestManager)

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuestTriggerResponseFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerResponseFactory)
SCF_IMPLEMENT_IBASE_END

celQuestTriggerResponseFactory::celQuestTriggerResponseFactory ()
{
  SCF_CONSTRUCT_IBASE (0);
}

celQuestTriggerResponseFactory::~celQuestTriggerResponseFactory ()
{
  SCF_DESTRUCT_IBASE ();
}

void celQuestTriggerResponseFactory::SetTriggerFactory (
	iQuestTriggerFactory* trigger_fact)
{
  trigger_factory = trigger_fact;
}

void celQuestTriggerResponseFactory::AddRewardFactory (
	iQuestRewardFactory* reward_fact)
{
  reward_factories.Push (reward_fact);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuestStateFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestStateFactory)
SCF_IMPLEMENT_IBASE_END

celQuestStateFactory::celQuestStateFactory (const char* name)
{
  SCF_CONSTRUCT_IBASE (0);
  celQuestStateFactory::name = csStrNew (name);
}

celQuestStateFactory::~celQuestStateFactory ()
{
  delete[] name;
  SCF_DESTRUCT_IBASE ();
}

iQuestTriggerResponseFactory* celQuestStateFactory::
	CreateTriggerResponseFactory ()
{
  celQuestTriggerResponseFactory* resp = new celQuestTriggerResponseFactory ();
  responses.Push (resp);
  resp->DecRef ();
  return resp;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuestFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestFactory)
SCF_IMPLEMENT_IBASE_END

celQuestFactory::celQuestFactory (celQuestManager* questmgr, const char* name)
{
  SCF_CONSTRUCT_IBASE (0);
  celQuestFactory::questmgr = questmgr;
  celQuestFactory::name = csStrNew (name);
}

celQuestFactory::~celQuestFactory ()
{
  delete[] name;
  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuest> celQuestFactory::CreateQuest (
      const celQuestParams& params)
{
  celQuest* q = new celQuest ();
  celQuestFactoryStates::GlobalIterator it = states.GetIterator ();
  while (it.HasNext ())
  {
    celQuestStateFactory* sf = it.Next ();
    const csRefArray<celQuestTriggerResponseFactory>& responses
    	= sf->GetResponses ();
    int stateidx = q->AddState (sf->GetName ());
    size_t i;
    for (i = 0 ; i < responses.Length () ; i++)
    {
      celQuestTriggerResponseFactory* respfact = responses[i];
      iQuestTriggerFactory* trigfact = respfact->GetTriggerFactory ();
      const csRefArray<iQuestRewardFactory>& rewfacts
        = respfact->GetRewardFactories ();

      int respidx = q->AddStateResponse (stateidx);
      csRef<iQuestTrigger> trig = trigfact->CreateTrigger (params);
      if (!trig) return 0;	// @@@ Report
      q->SetStateTrigger (stateidx, respidx, trig);
      size_t j;
      for (j = 0 ; j < rewfacts.Length () ; j++)
      {
        csRef<iQuestReward> rew = rewfacts[j]->CreateReward (params);
	if (!rew) return 0;
        q->AddStateReward (stateidx, respidx, rew);
      }
    }
  }
  return csPtr<iQuest> (q);
}

bool celQuestFactory::LoadTriggerResponse (
	iQuestTriggerResponseFactory* respfact,
  	iQuestTriggerFactory* trigfact, iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_FIREON:
	if (!trigfact->Load (child))
	  return false;
        break;
      case XMLTOKEN_REWARD:
        {
	  const char* type = child->GetAttributeValue ("type");
	  iQuestRewardType* rewardtype = questmgr->GetRewardType (type);
	  if (!rewardtype)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown reward type '%s' while loading quest '%s'!",
		type, name);
	    return false;
	  }
	  csRef<iQuestRewardFactory> rewardfact = rewardtype
		->CreateRewardFactory ();
	  if (!rewardfact->Load (child))
	    return false;
	  respfact->AddRewardFactory (rewardfact);
	}
        break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading trigger in quest '%s'!",
		value, name);
        return false;
    }
  }
  return true;
}

bool celQuestFactory::LoadState (iQuestStateFactory* statefact,
	iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_TRIGGER:
        {
	  const char* type = child->GetAttributeValue ("type");
	  iQuestTriggerType* triggertype = questmgr->GetTriggerType (type);
	  if (!triggertype)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown trigger type '%s' while loading state '%s/%s'!",
		type, name, statefact->GetName ());
	    return false;
	  }
	  // First we create a trigger response factory.
	  csRef<iQuestTriggerResponseFactory> respfact = statefact
	  	->CreateTriggerResponseFactory ();
	  // We create the actual trigger factory for that response factory.
	  csRef<iQuestTriggerFactory> triggerfact = triggertype
		->CreateTriggerFactory ();
	  respfact->SetTriggerFactory (triggerfact);
	  if (!LoadTriggerResponse (respfact, triggerfact, child))
	    return false;
	}
        break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading state '%s/%s'!",
		value, name, statefact->GetName ());
        return false;
    }
  }
  return true;
}

bool celQuestFactory::Load (iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_STATE:
        {
	  const char* statename = child->GetAttributeValue ("name");
	  iQuestStateFactory* statefact = CreateState (statename);
	  if (!statefact)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Couldn't load state '%s' while loading quest '%s'!",
		statename, name);
	    return false;
	  }
	  if (!LoadState (statefact, child))
	    return false;
	}
        break;
      case XMLTOKEN_START:
        break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading quest '%s'!",
		value, name);
        return false;
    }
  }
  return true;
}

iQuestStateFactory* celQuestFactory::GetState (const char* name)
{
  celQuestStateFactory* state = states.Get (name, 0);
  return (iQuestStateFactory*)state;
}

iQuestStateFactory* celQuestFactory::CreateState (const char* name)
{
  iQuestStateFactory* istate = GetState (name);
  if (istate) return 0;

  celQuestStateFactory* state = new celQuestStateFactory (name);
  states.Put (name, state);
  state->DecRef ();
  return state;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuestStateResponse)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerCallback)
SCF_IMPLEMENT_IBASE_END

celQuestStateResponse::celQuestStateResponse ()
{
  SCF_CONSTRUCT_IBASE (0);
}

celQuestStateResponse::~celQuestStateResponse ()
{
  SCF_DESTRUCT_IBASE ();
}

void celQuestStateResponse::SetTrigger (iQuestTrigger* trigger)
{
  celQuestStateResponse::trigger = trigger;
  trigger->RegisterCallback (this);
}

void celQuestStateResponse::AddReward (iQuestReward* reward)
{
  rewards.Push (reward);
}

void celQuestStateResponse::TriggerFired (iQuestTrigger* trigger)
{
  size_t i;
  for (i = 0 ; i < rewards.Length () ; i++)
    rewards[i]->Reward ();
}

//---------------------------------------------------------------------------

size_t celQuestState::AddResponse ()
{
  celQuestStateResponse* response = new celQuestStateResponse ();
  size_t idx = responses.Push (response);
  response->DecRef ();
  return idx;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuest)
  SCF_IMPLEMENTS_INTERFACE (iQuest)
SCF_IMPLEMENT_IBASE_END

celQuest::celQuest ()
{
  SCF_CONSTRUCT_IBASE (0);
  current_state = -1;
}

celQuest::~celQuest ()
{
  SCF_DESTRUCT_IBASE ();
}

bool celQuest::SwitchState (const char* state)
{
  // @@@ This code could be slow with really complex
  // quests that have lots of states. In practice most quests
  // will probably only have few states and will not switch
  // THAT often either.
  size_t i, j;
  for (i = 0 ; i < states.Length () ; i++)
  {
    if (strcmp (state, states[i]->GetName ()) == 0)
    {
      if (i == (size_t)current_state) return true;	// Nothing happens.
      if (current_state != -1)
      {
	celQuestState* st = states[current_state];
        for (j = 0 ; j < st->GetResponseCount () ; j++)
          st->GetResponse (j)->GetTrigger ()->DeactivateTrigger ();
      }
      current_state = i;
      celQuestState* st = states[current_state];
      for (j = 0 ; j < st->GetResponseCount () ; j++)
      {
        st->GetResponse (j)->GetTrigger ()->ActivateTrigger ();
      }
      return true;
    }
  }
  return false;
}

const char* celQuest::GetCurrentState () const
{
  if (current_state == -1) return 0;
  return states[current_state]->GetName ();
}

int celQuest::AddState (const char* name)
{
  return states.Push (new celQuestState (name));
}

int celQuest::AddStateResponse (int stateidx)
{
  return states[stateidx]->AddResponse ();
}

void celQuest::SetStateTrigger (int stateidx, int responseidx,
	iQuestTrigger* trigger)
{
  states[stateidx]->GetResponse (responseidx)->SetTrigger (trigger);
}

void celQuest::AddStateReward (int stateidx, int responseidx,
	iQuestReward* reward)
{
  states[stateidx]->GetResponse (responseidx)->AddReward (reward);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuestManager)
  SCF_IMPLEMENTS_INTERFACE (iQuestManager)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celQuestManager::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celQuestManager::celQuestManager (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celQuestManager::~celQuestManager ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celQuestManager::Initialize (iObjectRegistry* object_reg)
{
  celQuestManager::object_reg = object_reg;

  {
    celTimeoutTriggerType* type = new celTimeoutTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celEnterSectorTriggerType* type = new celEnterSectorTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celMeshEnterSectorTriggerType* type = new celMeshEnterSectorTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celDebugPrintRewardType* type = new celDebugPrintRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    celNewStateRewardType* type = new celNewStateRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    celChangePropertyRewardType* type = new celChangePropertyRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  return true;
}

iQuestFactory* celQuestManager::GetQuestFactory (const char* name)
{
  celQuestFactory* fact = quest_factories.Get (name, 0);
  return (iQuestFactory*)fact;
}

iQuestFactory* celQuestManager::CreateQuestFactory (const char* name)
{
  iQuestFactory* ifact = GetQuestFactory (name);
  if (ifact) return 0;

  celQuestFactory* fact = new celQuestFactory (this, name);
  quest_factories.Put (name, fact);
  fact->DecRef ();
  return fact;
}

bool celQuestManager::RegisterTriggerType (iQuestTriggerType* trigger)
{
  const char* name = trigger->GetName ();
  if (trigger_types.Get (name, 0) != 0)
    return false;
  trigger_types.Put (name, trigger);
  return true;
}

iQuestTriggerType* celQuestManager::GetTriggerType (const char* name)
{
  return trigger_types.Get (name, 0);
}

bool celQuestManager::RegisterRewardType (iQuestRewardType* reward)
{
  const char* name = reward->GetName ();
  if (reward_types.Get (name, 0) != 0)
    return false;
  reward_types.Put (name, reward);
  return true;
}

iQuestRewardType* celQuestManager::GetRewardType (const char* name)
{
  return reward_types.Get (name, 0);
}

const char* celQuestManager::ResolveParameter (
  	const celQuestParams& params,
	const char* param)
{
  if (param == 0) return param;
  if (*param != '$') return param;
  return params.Get (param+1, 0);
}

bool celQuestManager::Load (iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (strcmp ("quest", value) == 0)
    {
      const char* questname = child->GetAttributeValue ("name");
      iQuestFactory* questfact = CreateQuestFactory (questname);
      if (!questfact->Load (child))
        return false;
    }
    else
    {
        csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading quests!",
		value);
        return false;
    }
  }
  return true;
}

iQuestRewardFactory* celQuestManager::AddNewStateReward (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* state_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.newstate");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iNewStateQuestRewardFactory> newstate = SCF_QUERY_INTERFACE (rewfact,
  	iNewStateQuestRewardFactory);
  newstate->SetStateParameter (state_par);
  newstate->SetEntityNameParameter (entity_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iQuestRewardFactory* celQuestManager::AddDebugPrintReward (
	iQuestTriggerResponseFactory* response,
  	const char* msg_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.debugprint");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iDebugPrintQuestRewardFactory> newstate = SCF_QUERY_INTERFACE (rewfact,
  	iDebugPrintQuestRewardFactory);
  newstate->SetMessageParameter (msg_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iQuestTriggerFactory* celQuestManager::SetTimeoutTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* timeout_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.timeout");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iTimeoutQuestTriggerFactory> newstate = SCF_QUERY_INTERFACE (trigfact,
  	iTimeoutQuestTriggerFactory);
  newstate->SetTimeoutParameter (timeout_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iQuestTriggerFactory* celQuestManager::SetEnterSectorTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.entersector");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iEnterSectorQuestTriggerFactory> newstate = SCF_QUERY_INTERFACE (
  	trigfact, iEnterSectorQuestTriggerFactory);
  newstate->SetEntityNameParameter (entity_par);
  newstate->SetSectorNameParameter (sector_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iQuestTriggerFactory* celQuestManager::SetMeshEnterSectorTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.meshentersector");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iEnterSectorQuestTriggerFactory> newstate = SCF_QUERY_INTERFACE (
  	trigfact, iEnterSectorQuestTriggerFactory);
  newstate->SetEntityNameParameter (entity_par);
  newstate->SetSectorNameParameter (sector_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

//---------------------------------------------------------------------------
