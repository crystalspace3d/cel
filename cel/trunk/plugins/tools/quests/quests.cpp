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
#include "plugins/tools/quests/trig_propertychange.h"
#include "plugins/tools/quests/reward_debugprint.h"
#include "plugins/tools/quests/reward_newstate.h"
#include "plugins/tools/quests/reward_changeproperty.h"
#include "plugins/tools/quests/reward_inventory.h"
#include "plugins/tools/quests/seqop_debugprint.h"

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

SCF_IMPLEMENT_IBASE (celQuestSequence)
  SCF_IMPLEMENTS_INTERFACE (iQuestSequence)
  SCF_IMPLEMENTS_INTERFACE (iCelTimerListener)
SCF_IMPLEMENT_IBASE_END

celQuestSequence::celQuestSequence (const char* name,
	csTicks total_time, iCelPlLayer* pl,
	iVirtualClock* vc)
{
  SCF_CONSTRUCT_IBASE (0);
  celQuestSequence::name = csStrNew (name);
  celQuestSequence::total_time = total_time;
  celQuestSequence::pl = pl;
  celQuestSequence::vc = vc;
  idx = csArrayItemNotFound;
}

celQuestSequence::~celQuestSequence ()
{
  Abort ();
  delete[] name;
  SCF_DESTRUCT_IBASE ();
}

void celQuestSequence::AddSeqOp (iQuestSeqOp* seqop, csTicks start, csTicks end)
{
  celSeqOp seq;
  seq.seqop = seqop;
  seq.start = start;
  seq.end = end;
  seqops.Push (seq);
}

bool celQuestSequence::Start (csTicks delay)
{
  if (IsRunning ()) return false;
  idx = 0;
  pl->CallbackEveryFrame ((iCelTimerListener*)this, cscmdPreProcess);
  start_time = vc->GetCurrentTicks () + delay;
  return true;
}

void celQuestSequence::Finish ()
{
  if (!IsRunning ()) return;
  Perform (total_time+1);
}

void celQuestSequence::Abort ()
{
  if (!IsRunning ()) return;
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, cscmdPreProcess);
  idx = csArrayItemNotFound;
  ops_in_progress.Empty ();
}

bool celQuestSequence::IsRunning ()
{
  return idx != csArrayItemNotFound;
}

void celQuestSequence::Perform (csTicks rel)
{
  if (rel > total_time)
  {
    // Sequence has ended.
    Abort ();
    return;
  }

  // Find all operations that have to be performed.
  while (idx < seqops.Length () && seqops[idx].start >= rel)
  {
    if (rel >= seqops[idx].end)
    {
      // Single shot operation or operation has already ended. Will not
      // be put in the progress array.
      seqops[idx].seqop->Do (1.0f);
    }
    else
    {
      ops_in_progress.Push (seqops[idx]);
    }
    idx++;
  }

  // Perform the operations that are still in progress.
  size_t i = 0;
  while (i < ops_in_progress.Length ())
  {
    if (rel >= ops_in_progress[i].end)
    {
      ops_in_progress[i].seqop->Do (1.0f);
      ops_in_progress.DeleteIndex (i);
    }
    else
    {
      float dt = float (rel - ops_in_progress[i].start)
      	/ float (ops_in_progress[i].end - ops_in_progress[i].start);
      ops_in_progress[i].seqop->Do (dt);
      i++;
    }
  }
}

void celQuestSequence::TickEveryFrame ()
{
  csTicks current_time = vc->GetCurrentTicks ();
  if (current_time < start_time) return;
  csTicks rel = current_time - start_time;
  Perform (rel);
}

SCF_IMPLEMENT_IBASE (celQuestSequenceFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestSequenceFactory)
SCF_IMPLEMENT_IBASE_END

celQuestSequenceFactory::celQuestSequenceFactory (const char* name,
	celQuestFactory* parent)
{
  SCF_CONSTRUCT_IBASE (0);
  celQuestSequenceFactory::name = csStrNew (name);
  total_time = 0;
  parent_factory = parent;
}

celQuestSequenceFactory::~celQuestSequenceFactory ()
{
  delete[] name;
  SCF_DESTRUCT_IBASE ();
}

bool celQuestSequenceFactory::Load (iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  csTicks current_time = 0;
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = parent_factory->xmltokens.Request (value);
    switch (id)
    {
      case celQuestFactory::XMLTOKEN_OP:
        {
	  csString type = child->GetAttributeValue ("type");
	  iQuestSeqOpType* seqoptype = parent_factory->GetQuestManager ()
	  	->GetSeqOpType ("cel.questseqop."+type);
	  if (!seqoptype)
	    seqoptype = parent_factory->GetQuestManager ()->GetSeqOpType (type);
	  if (!seqoptype)
	  {
            csReport (parent_factory->GetQuestManager ()->object_reg,
	    	CS_REPORTER_SEVERITY_ERROR, "cel.questmanager.load",
		"Unknown sequence type '%s' while loading quest '%s'!",
		(const char*)type, name);
	    return false;
	  }
	  int duration = child->GetAttributeValueAsInt ("duration");
	  csRef<iQuestSeqOpFactory> seqopfact = seqoptype
		->CreateSeqOpFactory ();
	  if (!seqopfact->Load (child))
	    return false;
	  celSeqOpFact s;
	  s.seqop = seqopfact;
	  s.start = current_time;
	  s.end = current_time+duration;
	  if (s.end > total_time) total_time = s.end;
	  seqops.InsertSorted (s);
	}
        break;
      case celQuestFactory::XMLTOKEN_DELAY:
        {
	  int time = child->GetAttributeValueAsInt ("time");
	  current_time += time;
	  if (current_time > total_time) total_time = current_time;
	}
        break;
      default:
        csReport (parent_factory->GetQuestManager ()->object_reg,
		CS_REPORTER_SEVERITY_ERROR, "cel.questmanager.load",
		"Unknown token '%s' while loading sequence!",
		value);
        return false;
    }
  }
  return true;
}

void celQuestSequenceFactory::AddSeqOpFactory (iQuestSeqOpFactory* seqopfact,
  	csTicks start, csTicks end)
{
  celSeqOpFact s;
  s.seqop = seqopfact;
  s.start = start;
  s.end = end;
  seqops.InsertSorted (s);
  if (end > total_time) total_time = end;
}

csPtr<celQuestSequence> celQuestSequenceFactory::CreateSequence (
	const celQuestParams& params)
{
  celQuestSequence* seq = new celQuestSequence (name,
  	total_time,
  	parent_factory->GetQuestManager ()->pl,
	parent_factory->GetQuestManager ()->vc);
  size_t i;
  for (i = 0 ; i < seqops.Length () ; i++)
  {
    csRef<iQuestSeqOp> seqop = seqops[i].seqop->CreateSeqOp (params);
    seq->AddSeqOp (seqop, seqops[i].start, seqops[i].end);
  }
  return csPtr<celQuestSequence> (seq);
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
  InitTokenTable (xmltokens);
}

celQuestFactory::~celQuestFactory ()
{
  delete[] name;
  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuest> celQuestFactory::CreateQuest (
      const celQuestParams& params)
{
  celQuest* q = new celQuest (questmgr->pl);

  celQuestFactoryStates::GlobalIterator sta_it = states.GetIterator ();
  while (sta_it.HasNext ())
  {
    celQuestStateFactory* sf = sta_it.Next ();
    const csRefArray<celQuestTriggerResponseFactory>& responses
    	= sf->GetResponses ();
    size_t stateidx = q->AddState (sf->GetName ());
    size_t i;
    for (i = 0 ; i < responses.Length () ; i++)
    {
      celQuestTriggerResponseFactory* respfact = responses[i];
      iQuestTriggerFactory* trigfact = respfact->GetTriggerFactory ();
      const csRefArray<iQuestRewardFactory>& rewfacts
        = respfact->GetRewardFactories ();

      size_t respidx = q->AddStateResponse (stateidx);
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

  celQuestFactorySequences::GlobalIterator seq_it = sequences.GetIterator ();
  while (seq_it.HasNext ())
  {
    celQuestSequenceFactory* sf = seq_it.Next ();
    csRef<celQuestSequence> seq = sf->CreateSequence (params);
    q->AddSequence (seq);
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
	  csString type = child->GetAttributeValue ("type");
	  iQuestRewardType* rewardtype = questmgr->GetRewardType (
	  	"cel.questreward."+type);
	  if (!rewardtype)
	    rewardtype = questmgr->GetRewardType (type);
	  if (!rewardtype)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown reward type '%s' while loading quest '%s'!",
		(const char*)type, name);
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
	  csString type = child->GetAttributeValue ("type");
	  iQuestTriggerType* triggertype = questmgr->GetTriggerType (
	  	"cel.questtrigger."+type);
	  if (!triggertype)
	    triggertype = questmgr->GetTriggerType (type);
	  if (!triggertype)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown trigger type '%s' while loading state '%s/%s'!",
		(const char*)type, name, statefact->GetName ());
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

iQuestSequenceFactory* celQuestFactory::GetSequence (const char* name)
{
  celQuestSequenceFactory* seq = sequences.Get (name, 0);
  return (iQuestSequenceFactory*)seq;
}

iQuestSequenceFactory* celQuestFactory::CreateSequence (const char* name)
{
  iQuestSequenceFactory* iseq = GetSequence (name);
  if (iseq) return 0;

  celQuestSequenceFactory* seq = new celQuestSequenceFactory (name,
  	this);
  sequences.Put (name, seq);
  seq->DecRef ();
  return seq;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuestStateResponse)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerCallback)
SCF_IMPLEMENT_IBASE_END

celQuestStateResponse::celQuestStateResponse (iCelPlLayer* pl)
{
  SCF_CONSTRUCT_IBASE (0);
  reward_counter = 0;
  celQuestStateResponse::pl = pl;
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
  reward_counter++;
  if (reward_counter == 1)
  {
    pl->CallbackEveryFrame ((iCelTimerListener*)this, cscmdPreProcess);
  }
}

void celQuestStateResponse::TickEveryFrame ()
{
  while (reward_counter > 0)
  {
    size_t i;
    for (i = 0 ; i < rewards.Length () ; i++)
      rewards[i]->Reward ();
    reward_counter--;
  }
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, cscmdPreProcess);
}

//---------------------------------------------------------------------------

size_t celQuestState::AddResponse ()
{
  celQuestStateResponse* response = new celQuestStateResponse (pl);
  size_t idx = responses.Push (response);
  response->DecRef ();
  return idx;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuest)
  SCF_IMPLEMENTS_INTERFACE (iQuest)
SCF_IMPLEMENT_IBASE_END

celQuest::celQuest (iCelPlLayer* pl)
{
  SCF_CONSTRUCT_IBASE (0);
  celQuest::pl = pl;
  current_state = csArrayItemNotFound;
}

celQuest::~celQuest ()
{
  DeactivateState (current_state);
  SCF_DESTRUCT_IBASE ();
}

void celQuest::DeactivateState (size_t stateidx)
{
  if (stateidx == (size_t)-1) return;
  celQuestState* st = states[stateidx];
  size_t j;
  for (j = 0 ; j < st->GetResponseCount () ; j++)
    st->GetResponse (j)->GetTrigger ()->DeactivateTrigger ();
}

bool celQuest::SwitchState (const char* state, iCelDataBuffer* databuf)
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
      DeactivateState (current_state);
      current_state = i;
      celQuestState* st = states[current_state];
      for (j = 0 ; j < st->GetResponseCount () ; j++)
	if (databuf)
	{
          if (!st->GetResponse (j)->GetTrigger ()
	  	->LoadAndActivateTrigger (databuf))
	    return false;	// @@@ Report?
	}
	else
	{
          st->GetResponse (j)->GetTrigger ()->ActivateTrigger ();
        }
      return true;
    }
  }
  return false;
}

bool celQuest::SwitchState (const char* state)
{
  return SwitchState (state, 0);
}

bool celQuest::LoadState (const char* state, iCelDataBuffer* databuf)
{
  return SwitchState (state, databuf);
}

void celQuest::SaveState (iCelDataBuffer* databuf)
{
  if (current_state != csArrayItemNotFound)
  {
    celQuestState* st = states[current_state];
    size_t i;
    for (i = 0 ; i < st->GetResponseCount () ; i++)
      st->GetResponse (i)->GetTrigger ()->SaveTriggerState (databuf);
  }
}

const char* celQuest::GetCurrentState () const
{
  if (current_state == csArrayItemNotFound) return 0;
  return states[current_state]->GetName ();
}

size_t celQuest::AddState (const char* name)
{
  return states.Push (new celQuestState (pl, name));
}

size_t celQuest::AddStateResponse (size_t stateidx)
{
  return states[stateidx]->AddResponse ();
}

void celQuest::SetStateTrigger (size_t stateidx, size_t responseidx,
	iQuestTrigger* trigger)
{
  states[stateidx]->GetResponse (responseidx)->SetTrigger (trigger);
}

void celQuest::AddStateReward (size_t stateidx, size_t responseidx,
	iQuestReward* reward)
{
  states[stateidx]->GetResponse (responseidx)->AddReward (reward);
}

void celQuest::AddSequence (celQuestSequence* sequence)
{
  sequences.Push (sequence);
}

iQuestSequence* celQuest::FindSequence (const char* name)
{
  size_t i;
  for (i = 0 ; i < sequences.Length () ; i++)
    if (!strcmp (name, sequences[i]->GetName ()))
      return sequences[i];
  return 0;
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
  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);

  {
    celPropertyChangeTriggerType* type = new celPropertyChangeTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

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

  {
    celInventoryRewardType* type = new celInventoryRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    celDebugPrintSeqOpType* type = new celDebugPrintSeqOpType (
    	object_reg);
    RegisterSeqOpType (type);
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

bool celQuestManager::RegisterSeqOpType (iQuestSeqOpType* seqop)
{
  const char* name = seqop->GetName ();
  if (seqop_types.Get (name, 0) != 0)
    return false;
  seqop_types.Put (name, seqop);
  return true;
}

iQuestSeqOpType* celQuestManager::GetSeqOpType (const char* name)
{
  return seqop_types.Get (name, 0);
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
      // It is possible that questfact is 0. That means that the factory
      // already existed.
      if (questfact)
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

iQuestRewardFactory* celQuestManager::AddInventoryReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.inventory");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iInventoryQuestRewardFactory> newstate = SCF_QUERY_INTERFACE (rewfact,
  	iInventoryQuestRewardFactory);
  newstate->SetEntityNameParameter (entity_par);
  newstate->SetChildEntityNameParameter (child_entity_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iChangePropertyQuestRewardFactory* celQuestManager::AddChangePropertyReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.changeproperty");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iChangePropertyQuestRewardFactory> newstate = SCF_QUERY_INTERFACE (
  	rewfact, iChangePropertyQuestRewardFactory);
  newstate->SetEntityNameParameter (entity_par);
  newstate->SetPropertyNameParameter (prop_par);
  response->AddRewardFactory (rewfact);
  return newstate;
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

iQuestTriggerFactory* celQuestManager::SetPropertyChangeTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par, const char* value_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.propertychange");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iPropertyChangeQuestTriggerFactory> newstate = SCF_QUERY_INTERFACE (
  	trigfact, iPropertyChangeQuestTriggerFactory);
  newstate->SetEntityNameParameter (entity_par);
  newstate->SetPropertyNameParameter (prop_par);
  newstate->SetValueParameter (value_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

//---------------------------------------------------------------------------
