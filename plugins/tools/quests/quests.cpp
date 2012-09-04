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
#include "csutil/xmltiny.h"
#include "csutil/scanstr.h"
#include "cstool/objectcomment.h"

#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"
#include "imap/ldrctxt.h"
#include "iengine/collection.h"
#include "iengine/engine.h"

#include "physicallayer/persist.h"
#include "tools/parameters.h"

#include "plugins/tools/quests/quests.h"
#include "plugins/tools/quests/reward_newstate.h"


//---------------------------------------------------------------------------

//CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celQuestManager)

//---------------------------------------------------------------------------

void celQuestTriggerResponseFactory::SetTriggerFactory (
	iTriggerFactory* trigger_fact)
{
  trigger_factory = trigger_fact;
}

void celQuestTriggerResponseFactory::AddRewardFactory (
	iRewardFactory* reward_fact)
{
  reward_factories.Push (reward_fact);
}

csRef<iRewardFactoryArray> celQuestTriggerResponseFactory::GetRewardFactories ()
{
  csRef<iRewardFactoryArray> array;
  array.AttachNew (new scfArrayWrap<iRewardFactoryArray,
      csRefArray<iRewardFactory> > (reward_factories));
  return array;
}

//---------------------------------------------------------------------------

celQuestStateFactory::celQuestStateFactory (const char* name) :
	scfImplementationType (this)
{
  celQuestStateFactory::name = name;
}

iQuestTriggerResponseFactory* celQuestStateFactory::CreateTriggerResponseFactory ()
{
  celQuestTriggerResponseFactory* resp = new celQuestTriggerResponseFactory ();
  responses.Push (resp);
  resp->DecRef ();
  return resp;
}

csRef<iQuestTriggerResponseFactoryArray> celQuestStateFactory::GetTriggerResponseFactories ()
{
  csRef<iQuestTriggerResponseFactoryArray> array;
  array.AttachNew (new scfArrayWrap<iQuestTriggerResponseFactoryArray,
      csRefArray<iQuestTriggerResponseFactory> > (responses));
  return array;
}

void celQuestStateFactory::AddInitRewardFactory (iRewardFactory* reward_fact)
{
  oninit_reward_factories.Push (reward_fact);
}

csRef<iRewardFactoryArray> celQuestStateFactory::GetInitRewardFactories ()
{
  csRef<iRewardFactoryArray> array;
  array.AttachNew (new scfArrayWrap<iRewardFactoryArray,
      csRefArray<iRewardFactory> > (oninit_reward_factories));
  return array;
}

void celQuestStateFactory::AddExitRewardFactory (iRewardFactory* reward_fact)
{
  onexit_reward_factories.Push (reward_fact);
}

csRef<iRewardFactoryArray> celQuestStateFactory::GetExitRewardFactories ()
{
  csRef<iRewardFactoryArray> array;
  array.AttachNew (new scfArrayWrap<iRewardFactoryArray,
      csRefArray<iRewardFactory> > (onexit_reward_factories));
  return array;
}

//---------------------------------------------------------------------------

class celQuestStateFactoryIterator : public scfImplementation1<celQuestStateFactoryIterator,
	iQuestStateFactoryIterator>
{
private:
  celQuestFactoryStates::ConstGlobalIterator it;

public:
  celQuestStateFactoryIterator (const celQuestFactoryStates::ConstGlobalIterator it) :
	  scfImplementationType (this), it (it)
  {
  }
  virtual ~celQuestStateFactoryIterator () { }
  virtual bool HasNext () const { return it.HasNext (); }
  virtual iQuestStateFactory* Next ()
  {
    csRef<iQuestStateFactory> par = it.Next ();
    return par;
  }
};

class celSequenceFactoryIterator : public scfImplementation1<celSequenceFactoryIterator,
	iCelSequenceFactoryIterator>
{
private:
  celFactorySequences::ConstGlobalIterator it;

public:
  celSequenceFactoryIterator (const celFactorySequences::ConstGlobalIterator it) :
	  scfImplementationType (this), it (it)
  {
  }
  virtual ~celSequenceFactoryIterator () { }
  virtual bool HasNext () const { return it.HasNext (); }
  virtual iCelSequenceFactory* Next ()
  {
    csRef<iCelSequenceFactory> par = it.Next ();
    return par;
  }
};

class celQuestFactoryIterator : public scfImplementation1<celQuestFactoryIterator,
	iQuestFactoryIterator>
{
private:
  csHash<csRef<celQuestFactory>,csStringBase>::ConstGlobalIterator it;

public:
  celQuestFactoryIterator (const csHash<csRef<celQuestFactory>,csStringBase>::ConstGlobalIterator it) :
	  scfImplementationType (this), it (it)
  {
  }
  virtual ~celQuestFactoryIterator () { }
  virtual bool HasNext () const { return it.HasNext (); }
  virtual iQuestFactory* Next ()
  {
    csRef<iQuestFactory> par = it.Next ();
    return par;
  }
};

//---------------------------------------------------------------------------

celQuestFactory::celQuestFactory (celQuestManager* questmgr, const char* name) :
	scfImplementationType (this)
{
  celQuestFactory::questmgr = questmgr;
  SetName (name);
  InitTokenTable (xmltokens);
}

void celQuestFactory::SetDefaultParameter (const char* name, const char* value)
{
  if (!defaults)
    defaults.AttachNew (new celVariableParameterBlock ());
  defaults->AddParameter (questmgr->pl->FetchStringID (name)).Set (value);
}

void celQuestFactory::ClearDefaultParameters ()
{
  defaults = 0;
}

csPtr<iQuest> celQuestFactory::CreateQuest (iCelParameterBlock* params)
{
  celQuest* q = new celQuest (questmgr->pl);

  // Set defaults
  csRef<celVariableParameterBlock> result_params;
  result_params.AttachNew (new celVariableParameterBlock ());
  if (defaults) result_params->Merge (defaults);
  result_params->Merge (params);

  // Set states
  celQuestFactoryStates::GlobalIterator sta_it = states.GetIterator ();
  while (sta_it.HasNext ())
  {
    celQuestStateFactory* sf = sta_it.Next ();
    const csRefArray<iRewardFactory>& oninit_reward_Factories
        = sf->GetOninitRewardFactories ();
    const csRefArray<iRewardFactory>& onexit_reward_Factories
        = sf->GetOnexitRewardFactories ();
    const csRefArray<iQuestTriggerResponseFactory>& responses
    	= sf->GetResponses ();
    size_t stateidx = q->AddState (sf->GetName ());
    size_t i;
    for (i = 0 ; i < oninit_reward_Factories.GetSize () ; i++)
    {
      csRef<iReward> rew = oninit_reward_Factories[i]->CreateReward (q, result_params);
      q->AddOninitReward (stateidx, rew);
    }
    for (i = 0 ; i < onexit_reward_Factories.GetSize () ; i++)
    {
      csRef<iReward> rew = onexit_reward_Factories[i]->CreateReward (q, result_params);
      q->AddOnexitReward (stateidx, rew);
    }
    for (i = 0 ; i < responses.GetSize () ; i++)
    {
      celQuestTriggerResponseFactory* respfact = static_cast<celQuestTriggerResponseFactory*> (responses[i]);
      const csRefArray<iRewardFactory>& rewfacts = respfact->GetRewardFactoriesInt ();

      size_t respidx = q->AddStateResponse (stateidx);
      
      iTriggerFactory* trigfact = respfact->GetTriggerFactory ();
      csRef<iTrigger> trig = trigfact->CreateTrigger (q, result_params);
      if (!trig) return 0;	// @@@ Report
      q->SetStateTrigger (stateidx, respidx, trig);
	  
      size_t j;
      for (j = 0 ; j < rewfacts.GetSize () ; j++)
      {
        csRef<iReward> rew = rewfacts[j]->CreateReward (q, result_params);

	if (!rew) return 0;
        q->AddStateReward (stateidx, respidx, rew);
      }
    }
  }

  // Set sequences
  celFactorySequences::GlobalIterator seq_it = sequences.GetIterator ();
  while (seq_it.HasNext ())
  {
    iCelSequenceFactory* sf = seq_it.Next ();
    csRef<iCelSequence> seq = sf->CreateSequence (q, result_params);
    q->AddSequence (seq);
  }

  return csPtr<iQuest> (q);
}

csRef<iRewardFactory> celQuestFactory::LoadReward (iDocumentNode* child)
{
  csString type = child->GetAttributeValue ("type");
  iRewardType* rewardtype = questmgr->GetRewardType ("cel.rewards."+type);
  if (!rewardtype)
    rewardtype = questmgr->GetRewardType (type);
  if (!rewardtype)
  {
    csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown reward type '%s' while loading quest '%s'!",
		(const char*)type, (const char*)Name);
    return 0;
  }
  csRef<iRewardFactory> rewardfact = rewardtype->CreateRewardFactory ();
  if (!rewardfact->Load (child))
    return 0;
  return rewardfact;
}

bool celQuestFactory::SaveRewardArray (
    const csRefArray<iRewardFactory>& rewards, iDocumentNode* node, const char* nodename)
{
  csRef<iDocumentNode> n;
  if (nodename && rewards.GetSize () > 0)
  {
    n = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    n->SetValue (nodename);
  }
  else
    n = node;
  for (size_t i = 0 ; i < rewards.GetSize () ; i++)
  {
    csRef<iDocumentNode> rewardNode = n->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    rewardNode->SetValue ("reward");
    rewardNode->SetAttribute ("type", rewards[i]->GetRewardType ()->GetName ());
    if (!rewards[i]->Save (rewardNode))
      return false;
  }
  return true;
}

bool celQuestFactory::SaveRewards (
	iQuestStateFactory* statefact, bool oninit,
  	iDocumentNode* node)
{
  celQuestStateFactory* statefactT = static_cast<celQuestStateFactory*> (statefact);
  if (oninit)
    return SaveRewardArray (statefactT->GetOninitRewardFactories (), node, "oninit");
  else
    return SaveRewardArray (statefactT->GetOnexitRewardFactories (), node, "onexit");
}

bool celQuestFactory::LoadRewards (
	iQuestStateFactory* statefact, bool oninit,
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
      case XMLTOKEN_REWARD:
        {
	  csRef<iRewardFactory> rewardfact = LoadReward (child);
	  if (!rewardfact) return false;
	  if (oninit)
	    statefact->AddInitRewardFactory (rewardfact);
	  else
	    statefact->AddExitRewardFactory (rewardfact);
	}
        break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading '%s' in quest '%s'!",
		(const char*)value, oninit ? "oninit" : "onexit", (const char*)Name);
        return false;
    }
  }
  return true;
}

bool celQuestFactory::SaveSequenceFactory (iCelSequenceFactory* seqFact,
    iDocumentNode* node)
{
  for (size_t i = 0 ; i < seqFact->GetSeqOpFactoryCount () ; i++)
  {
    iSeqOpFactory* f = seqFact->GetSeqOpFactory (i);
    csString duration = seqFact->GetSeqOpFactoryDuration (i);
    if (f)
    {
      csRef<iDocumentNode> opNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
      opNode->SetValue ("op");
      opNode->SetAttribute ("duration", duration);
      opNode->SetAttribute ("type", f->GetSeqOpType ()->GetName ());
      f->Save (opNode);
    }
    else
    {
      csRef<iDocumentNode> delayNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
      delayNode->SetValue ("delay");
      delayNode->SetAttribute ("time", duration);
    }
  }

  return true;
}

bool celQuestFactory::LoadSequenceFactory (iCelSequenceFactory* seqFact, iDocumentNode* node)
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
      case XMLTOKEN_OP:
        {
	  csString type = child->GetAttributeValue ("type");
	  iSeqOpType* seqoptype = questmgr->GetSeqOpType ("cel.seqops."+type);
	  if (!seqoptype)
	    seqoptype = questmgr->GetSeqOpType (type);
	  if (!seqoptype)
	  {
	    csReport (questmgr->object_reg,
		  CS_REPORTER_SEVERITY_ERROR, "cel.questmanager.load",
		  "Unknown sequence type '%s' while loading quest '%s'!",
		  (const char*)type, (const char*)Name);
	    return false;
	  }
	  csRef<iSeqOpFactory> seqopfact = seqoptype->CreateSeqOpFactory ();
	  if (!seqopfact->Load (child)) return false;
	  const char* duration = child->GetAttributeValue ("duration");
	  seqFact->AddSeqOpFactory (seqopfact, duration);
	}
        break;
      case XMLTOKEN_DELAY:
        {
	  const char* time = child->GetAttributeValue ("time");
	  seqFact->AddDelay (time); 
	}
        break;

      default:
        csReport (questmgr->object_reg,
		  CS_REPORTER_SEVERITY_ERROR, "cel.questmanager.load",
		  "Unknown token '%s' while loading sequence!",
		  value);
        return false;
    }
  }
  return true;
}

bool celQuestFactory::SaveTriggerResponse (
	iQuestTriggerResponseFactory* respfact,
  	iDocumentNode* node)
{
  csRef<iDocumentNode> fireonNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
  fireonNode->SetValue ("fireon");
  if (!respfact->GetTriggerFactory ()->Save (fireonNode))
    return false;
  celQuestTriggerResponseFactory* respfactT = static_cast<celQuestTriggerResponseFactory*> (respfact);
  const csRefArray<iRewardFactory>& rewards = respfactT->GetRewardFactoriesInt ();
  return SaveRewardArray (rewards, node, 0);
}

bool celQuestFactory::LoadTriggerResponse (
	iQuestTriggerResponseFactory* respfact,
  	iTriggerFactory* trigfact, iDocumentNode* node)
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
	  csRef<iRewardFactory> rewardfact = LoadReward (child);
	  if (!rewardfact) return false;
	  respfact->AddRewardFactory (rewardfact);
	}
        break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading trigger in quest '%s'!",
		(const char*)value, (const char*)Name);
        return false;
    }
  }
  return true;
}

bool celQuestFactory::SaveState (celQuestStateFactory* statefact,
	iDocumentNode* node)
{
  SaveRewards (statefact, true, node);
  SaveRewards (statefact, false, node);
  const csRefArray<iQuestTriggerResponseFactory>& responses = statefact->GetResponses ();
  for (size_t i = 0 ; i < responses.GetSize () ; i++)
  {
    csRef<iDocumentNode> trigNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    trigNode->SetValue ("trigger");

    iQuestTriggerResponseFactory* resp = responses[i];
    iTriggerType* triggertype = resp->GetTriggerFactory ()->GetTriggerType ();
    trigNode->SetAttribute ("type", triggertype->GetName ());

    if (!SaveTriggerResponse (resp, trigNode))
      return false;
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
      case XMLTOKEN_ONINIT:
	if (!LoadRewards (statefact, true, child))
	  return false;
	break;
      case XMLTOKEN_ONEXIT:
	if (!LoadRewards (statefact, false, child))
	  return false;
	break;
      case XMLTOKEN_TRIGGER:
        {
	  csString type = child->GetAttributeValue ("type");
	  iTriggerType* triggertype = questmgr->GetTriggerType (
	  	"cel.triggers."+type);
	  if (!triggertype)
	    triggertype = questmgr->GetTriggerType (type);
	  if (!triggertype)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown trigger type '%s' while loading state '%s/%s'!",
		(const char*)type, (const char*)Name, statefact->GetName ());
	    return false;
	  }
	  // First we create a trigger response factory.
	  csRef<iQuestTriggerResponseFactory> respfact = statefact
	  	->CreateTriggerResponseFactory ();
	  // We create the actual trigger factory for that response factory.
	  csRef<iTriggerFactory> triggerfact = triggertype
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
		(const char*)value, (const char*)Name, statefact->GetName ());
        return false;
    }
  }
  return true;
}

bool celQuestFactory::Save (iDocumentNode* node)
{
  CS::Persistence::SaveComment (questmgr->object_reg, QueryObject (), node);

  if (defaults)
  {
    for (size_t i = 0 ; i < defaults->GetParameterCount () ; i++)
    {
      celDataType t;
      csStringID parID = defaults->GetParameterDef (i, t);
      celData& par = defaults->GetParameter (i);
      csString parS;
      celParameterTools::ToString (par, parS);

      csRef<iDocumentNode> defaultNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
      defaultNode->SetValue ("default");
      defaultNode->SetAttribute ("name", questmgr->pl->FetchString (parID));
      defaultNode->SetAttribute ("value", parS);
    }
  }

  celQuestFactoryStates::GlobalIterator stateIt = states.GetIterator ();
  while (stateIt.HasNext ())
  {
    csString key;
    celQuestStateFactory* state = stateIt.Next (key);
    csRef<iDocumentNode> stateNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    stateNode->SetValue ("state");
    stateNode->SetAttribute ("name", state->GetName ());

    if (!SaveState (state, stateNode))
      return false;
  }

  celFactorySequences::GlobalIterator seqIt = sequences.GetIterator ();
  while (seqIt.HasNext ())
  {
    csString key;
    iCelSequenceFactory* seq = seqIt.Next (key);
    csRef<iDocumentNode> seqNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    seqNode->SetValue ("sequence");
    seqNode->SetAttribute ("name", seq->GetName ());

    if (!SaveSequenceFactory (seq, seqNode))
      return false;
  }

  return true;
}

bool celQuestFactory::Load (iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();

    if (child->GetType () == CS_NODE_COMMENT)
      CS::Persistence::LoadComment (questmgr->object_reg, QueryObject (), child);

    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case celQuestFactory::XMLTOKEN_DEFAULT:
        {
	  const char* name = child->GetAttributeValue ("name");
	  if (!name)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"'name' missing for default quest parameter!");
	    return false;
	  }
	  const char* value = child->GetAttributeValue ("value");
	  if (!value)
	  {
	    value = child->GetAttributeValue ("string");
	    if (!value)
	    {
              csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.questmanager.load",
		"'string' is deprecated for quest default parameters. Use 'value'!");
	      return false;
	    }
	    else
	    {
              csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"'value' missing for default quest parameter!");
	    }
	  }
	  SetDefaultParameter (name, value);
	}
        break;
      case XMLTOKEN_STATE:
        {
	  const char* statename = child->GetAttributeValue ("name");
	  iQuestStateFactory* statefact = CreateState (statename);
	  if (!statefact)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		  "cel.questmanager.load",
		  "Couldn't load state '%s' while loading quest '%s'!",
		  (const char*)statename, (const char*)Name);
	    return false;
	  }
	  if (!LoadState (statefact, child))
	    return false;
	}
        break;
      case XMLTOKEN_START:
        break;
      case XMLTOKEN_SEQUENCE:
        {
	  const char* seqname = child->GetAttributeValue ("name");
	  iCelSequenceFactory* seqfact = CreateSequence (seqname);
	  if (!seqfact)
	  {
	    csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		  "cel.questmanager.load",
		  "Couldn't load sequence '%s' while loading quest '%s'!",
		(  const char*)seqname, (const char*)Name);
	    return false;
	  }
	  if (!LoadSequenceFactory (seqfact, child))
	    return false;
	}
	break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading quest '%s'!",
		value, (const char*)Name);
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

bool celQuestFactory::RemoveState (const char* name)
{
  if (!states.Contains (name)) return false;
  states.DeleteAll (name);
  return true;
}

csRef<iQuestStateFactoryIterator> celQuestFactory::GetStates () const
{
  csRef<celQuestStateFactoryIterator> it;
  it.AttachNew (new celQuestStateFactoryIterator (states.GetIterator ()));
  return it;
}

iCelSequenceFactory* celQuestFactory::GetSequence (const char* name)
{
  iCelSequenceFactory* seq = sequences.Get (name, 0);
  return (iCelSequenceFactory*)seq;
}

iCelSequenceFactory* celQuestFactory::CreateSequence (const char* name)
{
  iCelSequenceFactory* iseq = GetSequence (name);
  if (iseq) return 0;

  //csRef<iPluginManager> plugin_mgr = 
    //csQueryRegistry<iPluginManager> (questmgr->object_reg);
  csRef<iCelSequenceFactoryGenerator> seqgen = csQueryRegistryOrLoad<iCelSequenceFactoryGenerator> 
	  (questmgr->object_reg, "cel.sequence.factory");  
  csRef<iCelSequenceFactory> seq = seqgen->CreateSequenceFactory ();
  seq->SetName(name);
  sequences.Put (name, seq);
  return seq;
}

bool celQuestFactory::RemoveSequence (const char* name)
{
  if (!sequences.Contains (name)) return false;
  sequences.DeleteAll (name);
  return true;
}

csRef<iCelSequenceFactoryIterator> celQuestFactory::GetSequences () const
{
  csRef<celSequenceFactoryIterator> it;
  it.AttachNew (new celSequenceFactoryIterator (sequences.GetIterator ()));
  return it;
}

//---------------------------------------------------------------------------

celQuestStateResponse::celQuestStateResponse (iCelPlLayer* pl,
	celQuest* quest) : scfImplementationType (this)
{
  celQuestStateResponse::pl = pl;
  celQuestStateResponse::quest = quest;
}


void celQuestStateResponse::SetTrigger(iTrigger* trigger)
{
  celQuestStateResponse::trigger = trigger;
  trigger->RegisterCallback (this);
}

void celQuestStateResponse::AddReward (iReward* reward)
{
  rewards.Push (reward);
}


void celQuestStateResponse::TriggerFired (iTrigger* trigger,
    iCelParameterBlock* params)
{
  size_t i;
  for (i = 0 ; i < rewards.GetSize () ; i++)
    rewards[i]->Reward (params);

  return;
}

//---------------------------------------------------------------------------

size_t celQuestState::AddResponse (celQuest* quest)
{
  celQuestStateResponse* response = new celQuestStateResponse (pl, quest);
  size_t idx = responses.Push (response);
  response->DecRef ();
  return idx;
}

void celQuestState::Activate ()
{
  for (size_t i = 0 ; i <responses.GetSize () ; i++)
    responses[i]->GetTrigger ()->Activate ();
}

void celQuestState::Deactivate ()
{
  for (size_t i = 0 ; i <responses.GetSize () ; i++)
    responses[i]->GetTrigger ()->Deactivate ();
}

//---------------------------------------------------------------------------

celQuest::celQuest (iCelPlLayer* pl) : scfImplementationType (this)
{
  celQuest::pl = pl;
  current_state = csArrayItemNotFound;
}

celQuest::~celQuest ()
{
  DeactivateState (current_state, false);
}

void celQuest::DeactivateState (size_t stateidx, bool exec_onexit)
{
  if (stateidx == (size_t)-1) return;
  celQuestState* st = states[stateidx];
  size_t j;
  for (j = 0 ; j < st->GetResponseCount () ; j++)
  {
    csRef<celQuestStateResponse> r = st->GetResponse (j);
    r->GetTrigger ()->DeactivateTrigger ();
  }

  if (exec_onexit)
    for (j = 0 ; j < st->GetOnexitRewardCount () ; j++)
      st->GetOnexitReward (j)->Reward (0);
}

bool celQuest::SwitchState (const char* state)
{
  // @@@ This code could be slow with really complex
  // quests that have lots of states. In practice most quests
  // will probably only have few states and will not switch
  // THAT often either.

  // Check if we are going to switch to the same state. In that case we don't
  // fire the oninit/onexit rewards.
  bool samestate = current_state != (size_t)-1 && strcmp (state, states[current_state]->GetName ()) == 0;

  size_t i, j;
  for (i = 0 ; i < states.GetSize () ; i++)
  {
    if (strcmp (state, states[i]->GetName ()) == 0)
    {
      DeactivateState (current_state, !samestate);
      current_state = i;
      celQuestState* st = states[current_state];
      for (j = 0 ; j < st->GetResponseCount () ; j++)
      {
	csRef<celQuestStateResponse> r = st->GetResponse (j);
	iTrigger* trigger = r->GetTrigger ();
	trigger->ActivateTrigger ();
	if (trigger->Check ()) return true;
      }
      if (!samestate)
        for (j = 0 ; j < st->GetOninitRewardCount () ; j++)
          st->GetOninitReward (j)->Reward (0);
      return true;
    }
  }
  return false;
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
  return states[stateidx]->AddResponse (this);
}


void celQuest::SetStateTrigger (size_t stateidx, size_t responseidx,
	iTrigger* trigger)
{
  states[stateidx]->GetResponse (responseidx)->SetTrigger (trigger);
}


void celQuest::AddStateReward (size_t stateidx, size_t responseidx,
	iReward* reward)
{
  states[stateidx]->GetResponse (responseidx)->AddReward (reward);
}

void celQuest::AddOninitReward (size_t stateidx, iReward* reward)
{
  states[stateidx]->AddOninitReward (reward);
}

void celQuest::AddOnexitReward (size_t stateidx, iReward* reward)
{
  states[stateidx]->AddOnexitReward (reward);
}

void celQuest::AddSequence (iCelSequence* sequence)
{
  sequences.Push (sequence);
}

iCelSequence* celQuest::FindCelSequence (const char* name)
{
  size_t i;
  for (i = 0 ; i < sequences.GetSize () ; i++)
    if (!strcmp (name, sequences[i]->GetName ()))
      return sequences[i];
  return 0;
}

void celQuest::Activate ()
{
  if (current_state == csArrayItemNotFound) return;
  states[current_state]->Activate ();
  for (size_t i = 0 ; i < sequences.GetSize () ; i++)
    sequences[i]->Activate ();
}

void celQuest::Deactivate ()
{
  if (current_state == csArrayItemNotFound) return;
  states[current_state]->Deactivate ();
  for (size_t i = 0 ; i < sequences.GetSize () ; i++)
    sequences[i]->Deactivate ();
}

//---------------------------------------------------------------------------

celQuestManager::celQuestManager (iBase* parent) : scfImplementationType (this,
	parent)
{
}

celQuestManager::~celQuestManager ()
{
}

bool celQuestManager::Initialize (iObjectRegistry* object_reg)
{
  celQuestManager::object_reg = object_reg;
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  vc = csQueryRegistry<iVirtualClock> (object_reg);

  //--- Triggers -----------------------------------------------------
  csRef<iPluginManager> plugin_mgr = 
    csQueryRegistry<iPluginManager> (object_reg);

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.propertychange");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.meshselect");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.inventory");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.timeout");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.entersector");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.meshentersector");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.sequencefinish");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.trigger");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.watch");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.message");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  {
    csRef<iTriggerType> type = csLoadPlugin<iTriggerType> (plugin_mgr,
      "cel.triggers.operation");        
    if (type.IsValid()) RegisterTriggerType (type);
  }

  //--- Rewards ------------------------------------------------------

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.debugprint");        
    if (type.IsValid()) RegisterRewardType (type);
  }

  {
    celNewStateRewardType* type = new celNewStateRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.changeproperty");        
    if (type.IsValid()) RegisterRewardType (type);
  }

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.inventory");        
    if (type.IsValid()) RegisterRewardType (type);
  }

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.cssequence");        
    if (type.IsValid()) RegisterRewardType (type);
  }

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.sequence");        
    if (type.IsValid()) RegisterRewardType (type);
  }

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.sequencefinish");        
    if (type.IsValid()) RegisterRewardType (type);
  }

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.message");        
    if (type.IsValid()) RegisterRewardType (type);
  }

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.action");        
    if (type.IsValid()) RegisterRewardType (type);
  }

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.createentity");        
    if (type.IsValid()) RegisterRewardType (type);
  }

  {
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.destroyentity");        
    if (type.IsValid()) RegisterRewardType (type);
  }
  //--- Sequence Operations ------------------------------------------
  {
    csRef<iSeqOpType> type = csLoadPlugin<iSeqOpType> (plugin_mgr,
      "cel.seqops.debugprint");        
    if (type.IsValid()) RegisterSeqOpType(type);
  }

  {
    csRef<iSeqOpType> type = csLoadPlugin<iSeqOpType> (plugin_mgr,
      "cel.seqops.transform");        
    if (type.IsValid()) RegisterSeqOpType (type);
  }

  {
    csRef<iSeqOpType> type = csLoadPlugin<iSeqOpType> (plugin_mgr,
      "cel.seqops.movepath");        
    if (type.IsValid()) RegisterSeqOpType (type);
  }

  {
    csRef<iSeqOpType> type = csLoadPlugin<iSeqOpType> (plugin_mgr,
      "cel.seqops.light");        
    if (type.IsValid()) RegisterSeqOpType (type);
  }

  {
    csRef<iSeqOpType> type = csLoadPlugin<iSeqOpType> (plugin_mgr,
      "cel.seqops.ambientmesh");        
    if (type.IsValid()) RegisterSeqOpType (type);
  }

  {
    csRef<iSeqOpType> type = csLoadPlugin<iSeqOpType> (plugin_mgr,
      "cel.seqops.property");        
    if (type.IsValid()) RegisterSeqOpType (type);
  }

  return true;
}

iQuestFactory* celQuestManager::GetQuestFactory (const char* name)
{
  celQuestFactory* fact = quest_factories.Get (name, 0);
  return (iQuestFactory*)fact;
}

csRef<iQuestFactoryIterator> celQuestManager::GetQuestFactories () const
{
  csRef<celQuestFactoryIterator> it;
  it.AttachNew (new celQuestFactoryIterator (quest_factories.GetIterator ()));
  return it;
}

void celQuestManager::RemoveQuestFactories ()
{
  quest_factories.DeleteAll();
}

void celQuestManager::RemoveQuestFactory (const char* name)
{
  celQuestFactory* fact = quest_factories.Get (name, 0);
  if (fact)
    quest_factories.Delete(name,fact);
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

bool celQuestManager::RegisterTriggerType (iTriggerType* trigger)
{
  const char* name = trigger->GetName ();
  if (trigger_types.Get (name, 0) != 0)
    return false;
  trigger_types.Put (name, trigger);
  return true;
}


iTriggerType* celQuestManager::GetTriggerType (const char* name)
{
  return trigger_types.Get (name, 0);
}



bool celQuestManager::RegisterRewardType (iRewardType* reward)
{
  const char* name = reward->GetName ();
  if (reward_types.Get (name, 0) != 0)
    return false;
  reward_types.Put (name, reward);
  return true;
}

iRewardType* celQuestManager::GetRewardType (const char* name)
{
  return reward_types.Get (name, 0);
}


bool celQuestManager::RegisterSeqOpType (iSeqOpType* seqop)
{
  const char* name = seqop->GetName ();
  if (seqop_types.Get (name, 0) != 0)
    return false;
  seqop_types.Put (name, seqop);
  return true;
}


iSeqOpType* celQuestManager::GetSeqOpType (const char* name)
{
  return seqop_types.Get (name, 0);
}


bool celQuestManager::Save (iDocumentNode* node, iCollection* collection)
{
  csHash<csRef<celQuestFactory>,csStringBase>::GlobalIterator it = quest_factories.GetIterator ();
  while (it.HasNext ())
  {
    csString name;
    celQuestFactory* questFact = it.Next (name);
    if (!collection || collection->IsParentOf (questFact->QueryObject ()))
    {
      csRef<iDocumentNode> questNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
      questNode->SetValue ("quest");
      questNode->SetAttribute ("name", questFact->GetName ());
      if (!questFact->Save (questNode))
        return false;
    }
  }
  return true;
}

bool celQuestManager::Load (iDocumentNode* node, iLoaderContext* context)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (strcmp ("quest", value) == 0)
    {
      const char* filename = child->GetAttributeValue ("file");
      csRef<iDocumentNode> questnode;
      csRef<iDocument> doc;
      if (filename)
      {
        // Load quest from a file.
	csRef<iDocumentSystem> xml = 
		csQueryRegistry<iDocumentSystem> (object_reg);
	if (!xml)
	  xml.AttachNew (new csTinyDocumentSystem ());
	doc = xml->CreateDocument ();
	const char* error = doc->Parse (filename, true);
	if (error != 0)
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.questmanager",
		"Can't open file '%s': %s!", filename, error);
        }
	questnode = doc->GetRoot ()->GetNode ("quest");
      }
      else
        questnode = child;

      const char* questname = questnode->GetAttributeValue ("name");
      iQuestFactory* questfact = CreateQuestFactory (questname);
      // It is possible that questfact is 0. That means that the factory
      // already existed.
      if (questfact)
      {
	if (context)
          context->AddToCollection (questfact->QueryObject ());
        if (!questfact->Load (questnode))
          return false;
      }
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

iRewardFactory* celQuestManager::AddNewStateReward (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* state_par)
{
  iRewardType* type = GetRewardType ("cel.rewards.newstate");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iNewStateQuestRewardFactory> newstate = scfQueryInterface<iNewStateQuestRewardFactory> (rewfact);
  newstate->SetStateParameter (state_par);
  newstate->SetEntityParameter (entity_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}


iRewardFactory* celQuestManager::AddDebugPrintReward (
	iQuestTriggerResponseFactory* response,
  	const char* msg_par)
{
  iRewardType* type = GetRewardType ("cel.rewards.debugprint");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iDebugPrintRewardFactory> newstate = scfQueryInterface<iDebugPrintRewardFactory> (rewfact);
  newstate->SetMessageParameter (msg_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddInventoryReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par)
{
  iRewardType* type = GetRewardType("cel.rewards.inventory");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iInventoryRewardFactory> newstate = scfQueryInterface<iInventoryRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetChildEntityParameter (child_entity_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}


iRewardFactory* celQuestManager::AddSequenceReward(
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par,
	const char* delay_par)
{
  iRewardType* type = GetRewardType ("cel.rewards.sequence");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iSequenceRewardFactory> newstate = scfQueryInterface<iSequenceRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSequenceParameter (sequence_par);
  newstate->SetDelayParameter (delay_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddCsSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* sequence_par, const char* delay_par)
{
  iRewardType* type = GetRewardType ("cel.rewards.cssequence");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iCsSequenceRewardFactory> newstate = scfQueryInterface<iCsSequenceRewardFactory> (rewfact);
  newstate->SetSequenceParameter (sequence_par);
  newstate->SetDelayParameter (delay_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddSequenceFinishReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par)
{
  iRewardType* type = GetRewardType ("cel.rewards.sequencefinish");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iSequenceFinishRewardFactory> newstate = 
  	scfQueryInterface<iSequenceFinishRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSequenceParameter (sequence_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iChangePropertyRewardFactory* celQuestManager::AddChangePropertyReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par)
{
  iRewardType* type = GetRewardType ("cel.rewards.changeproperty");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iChangePropertyRewardFactory> newstate = 
  	scfQueryInterface<iChangePropertyRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetPropertyParameter (prop_par);
  response->AddRewardFactory (rewfact);
  return newstate;
}



iRewardFactory* celQuestManager::AddCreateEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* template_par,
	const char* name_par,
        iCelParameterBlock* tpl_params)
{
  iRewardType* type = GetRewardType ("cel.rewards.createentity");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iCreateEntityRewardFactory> newstate = scfQueryInterface<iCreateEntityRewardFactory> (rewfact);
  newstate->SetEntityTemplateParameter (template_par);
  newstate->SetNameParameter (name_par);

  for (size_t i = 0 ; i < tpl_params->GetParameterCount () ; i++)
  {
    celDataType t;
    csStringID id = tpl_params->GetParameterDef (i, t);
    const char* name = pl->FetchString (id);
    // @@@ Support dynamic parameters?
    const celData* data = tpl_params->GetParameterByIndex (i);
    csString val;
    celParameterTools::ToString (*data, val);
    newstate->AddParameter (name, val);
  }

  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddDestroyEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par)
{
  iRewardType* type = GetRewardType ("cel.rewards.destroyentity");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iDestroyEntityRewardFactory> newstate = 
  	scfQueryInterface<iDestroyEntityRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddMessageReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par,
	const char* id_par)
{
  iRewardType* type = GetRewardType ("cel.rewards.message");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iMessageRewardFactory> newstate = 
  	scfQueryInterface<iMessageRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetIDParameter (id_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddActionReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par,
	const char* id_par,
	const char* pcclass_par)
{
  iRewardType* type = GetRewardType ("cel.rewards.action");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iActionRewardFactory> newstate = 
  	scfQueryInterface<iActionRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetIDParameter (id_par);
  newstate->SetPropertyClassParameter (pcclass_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}


iTriggerFactory* celQuestManager::SetTimeoutTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* timeout_par)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.timeout");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iTimeoutTriggerFactory> newstate = scfQueryInterface<iTimeoutTriggerFactory> (trigfact);
  newstate->SetTimeoutParameter (timeout_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iTriggerFactory* celQuestManager::SetEnterSectorTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.entersector");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iEnterSectorTriggerFactory> newstate = 
  	scfQueryInterface<iEnterSectorTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSectorParameter (sector_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iTriggerFactory* celQuestManager::SetMeshEnterSectorTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.meshentersector");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iEnterSectorTriggerFactory> newstate = 
  	scfQueryInterface<iEnterSectorTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSectorParameter (sector_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iTriggerFactory* celQuestManager::SetSequenceFinishTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.sequencefinish");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iSequenceFinishTriggerFactory> newstate = 
  	scfQueryInterface<iSequenceFinishTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSequenceParameter (sequence_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iTriggerFactory* celQuestManager::SetPropertyChangeTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par, const char* value_par)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.propertychange");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iPropertyChangeTriggerFactory> newstate = 
  	scfQueryInterface<iPropertyChangeTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetPropertyParameter (prop_par);
  newstate->SetValueParameter (value_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iTriggerFactory* celQuestManager::SetTriggerTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, bool do_leave)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.trigger");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iTriggerTriggerFactory> newstate = 
  	scfQueryInterface<iTriggerTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->EnableLeave (do_leave);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iTriggerFactory* celQuestManager::SetWatchTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* target_entity_par,
	const char* checktime_par,
	const char* radius_par)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.watch");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iWatchTriggerFactory> newstate = 
  	scfQueryInterface<iWatchTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetTargetEntityParameter (target_entity_par);
  newstate->SetChecktimeParameter (checktime_par);
  newstate->SetRadiusParameter (radius_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iTriggerFactory* celQuestManager::SetOperationTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* operation_par,
	csRefArray<iTriggerFactory> &trigger_factories)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.operation");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iOperationTriggerFactory> newstate = 
  	scfQueryInterface<iOperationTriggerFactory> (trigfact);
  newstate->SetOperationParameter (operation_par);
  csRefArray<iTriggerFactory> trigger_factories_list = 
	newstate->GetTriggerFactories();
  csRefArray<iTriggerFactory>::Iterator iter = 
	trigger_factories.GetIterator();
  while (iter.HasNext())
  {
    trigger_factories_list.Push(iter.Next());
  }
  return trigfact;
}



iTriggerFactory* celQuestManager::SetInventoryTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_par)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.inventory");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iInventoryTriggerFactory> newstate = 
  	scfQueryInterface<iInventoryTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetChildEntityParameter (child_par);
  response->SetTriggerFactory(trigfact);
  return trigfact;
}

iTriggerFactory* celQuestManager::SetMessageTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* mask_par)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.message");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iMessageTriggerFactory> newstate = 
  	scfQueryInterface<iMessageTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetMaskParameter (mask_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iTriggerFactory* celQuestManager::SetMeshSelectTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par)
{
  iTriggerType* type = GetTriggerType ("cel.triggers.meshselect");
  csRef<iTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iMeshSelectTriggerFactory> newstate = 
  	scfQueryInterface<iMeshSelectTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

//---------------------------------------------------------------------------

