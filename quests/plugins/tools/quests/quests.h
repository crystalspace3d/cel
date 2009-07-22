/*
    Crystal Space Entity Layer
    Copyright (C) 2004-2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_QUESTS__
#define __CEL_TOOLS_QUESTS__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/strhash.h"
#include "csutil/weakref.h"
#include "csutil/csstring.h"
#include "csutil/parray.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"
#include "tools/rewards.h"
#include "tools/sequences.h"
#include "tools/expression.h"

#include "celtool/stdparams.h"
#include "physicallayer/pl.h"

struct iObjectRegistry;
struct iEvent;
class celQuestManager;
class celQuestFactory;
class celQuest;


//---------------------------------------------------------------------------

/**
 * A quest trigger response factory.
 */
class celQuestTriggerResponseFactory : public scfImplementation1<
	celQuestTriggerResponseFactory, iQuestTriggerResponseFactory>
{
private:
  csRef<iTriggerFactory> trigger_factory;
  csRefArray<iRewardFactory> reward_factories;

public:
  bool refactored_trigger;

  celQuestTriggerResponseFactory () : scfImplementationType (this) { }
  virtual ~celQuestTriggerResponseFactory () { }

  iTriggerFactory* GetTriggerFactory () const
  {
	return trigger_factory;
  }

  const csRefArray<iRewardFactory>& GetRewardFactories () const
  {
    return reward_factories;
  }

  virtual void SetTriggerFactory (iTriggerFactory* trigger_fact);
  virtual void AddRewardFactory (iRewardFactory* reward_fact);
};

/**
 * A quest state.
 */
class celQuestStateFactory : public scfImplementation1<
	celQuestStateFactory, iQuestStateFactory>
{
private:
  csString name;
  csRefArray<iRewardFactory> oninit_reward_factories;
  csRefArray<iRewardFactory> onexit_reward_factories;
  csRefArray<celQuestTriggerResponseFactory> responses;

public:
  celQuestStateFactory (const char* name);
  virtual ~celQuestStateFactory () { }

  const csRefArray<celQuestTriggerResponseFactory>& GetResponses () const
  {
    return responses;
  }
  const csRefArray<iRewardFactory>& GetOninitRewardFactories () const
  {
    return oninit_reward_factories;
  }
  const csRefArray<iRewardFactory>& GetOnexitRewardFactories () const
  {
    return onexit_reward_factories;
  }

  virtual const char* GetName () const { return name; }
  virtual iQuestTriggerResponseFactory* CreateTriggerResponseFactory ();
  virtual void AddInitRewardFactory (iRewardFactory* reward_fact);
  virtual void AddExitRewardFactory (iRewardFactory* reward_fact);
};

typedef csHash<csRef<celQuestStateFactory>,csStringBase> celQuestFactoryStates;
typedef csHash<csRef<iCelSequenceFactory>,csStringBase>  celFactorySequences;

/**
 * A quest factory.
 */
class celQuestFactory : public scfImplementation1<celQuestFactory,
	iQuestFactory>
{
private:
  celQuestManager* questmgr;
  csString name;
  celQuestFactoryStates states;
  celFactorySequences sequences;
  celParams defaults;

  csRef<iRewardFactory> LoadReward (iDocumentNode* child);
  bool LoadRewards (iQuestStateFactory* statefact, bool oninit,
  	iDocumentNode* node);
  bool LoadTriggerResponse (iQuestTriggerResponseFactory* respfact,
  	iTriggerFactory* trigfact, iDocumentNode* node);
  bool LoadState (iQuestStateFactory* statefact, iDocumentNode* node);

public:
  csStringHash xmltokens;
#define CS_TOKEN_ITEM_FILE "plugins/tools/quests/quests.tok"
#include "cstool/tokenlist.h"

public:
  celQuestFactory (celQuestManager* questmgr, const char* name);
  virtual ~celQuestFactory () { }

  virtual celQuestManager* GetQuestManager () const { return questmgr; }

  virtual const char* GetName () const { return name; }
  virtual csPtr<iQuest> CreateQuest (
      const celParams& params);
  virtual bool Load (iDocumentNode* node);
  virtual iQuestStateFactory* GetState (const char* name);
  virtual iQuestStateFactory* CreateState (const char* name);
  virtual iCelSequenceFactory* GetSequence (const char* name);
  virtual iCelSequenceFactory* CreateSequence (const char* name);
  virtual const char* GetDefaultParameter (const char* name) const;
  virtual void SetDefaultParameter (const char* name,const char* value);
  virtual void ClearDefaultParameters ();
};

/**
 * A trigger and rewards. This is basically a response for a quest.
 */
struct celQuestStateResponse : public scfImplementation1<
	celQuestStateResponse, iTriggerCallback>
{
private:
  iCelPlLayer* pl;
  csRef<iTrigger> trigger;
  csRefArray<iReward> rewards;
  celQuest* quest;

public:
  celQuestStateResponse (iCelPlLayer* pl, celQuest* quest);
  virtual ~celQuestStateResponse () { }

  void SetTrigger (iTrigger* trigger);
  iTrigger* GetTrigger () const { return trigger; }
  void AddReward (iReward* reward);

  // --- For iTriggerCallback ------------------------
  virtual void TriggerFired (iTrigger* trigger, iCelParameterBlock* params);
};

/**
 * A state in a quest.
 */
class celQuestState
{
private:
  iCelPlLayer* pl;
  csString name;
  csRefArray<iReward> oninit_rewards;
  csRefArray<iReward> onexit_rewards;
  csRefArray<celQuestStateResponse> responses;

public:
  celQuestState (iCelPlLayer* pl, const char* name)
  {
    celQuestState::pl = pl;
    celQuestState::name = name;
  }
  ~celQuestState () { }
  const char* GetName () const { return name; }
  size_t AddResponse (celQuest* quest);
  size_t GetResponseCount () const { return responses.GetSize (); }
  celQuestStateResponse* GetResponse (size_t idx) const
  {
    return responses[idx];
  }
  void AddOninitReward (iReward* reward)
  {
    oninit_rewards.Push (reward);
  }
  void AddOnexitReward (iReward* reward)
  {
    onexit_rewards.Push (reward);
  }
  size_t GetOninitRewardCount () const { return oninit_rewards.GetSize (); }
  iReward* GetOninitReward (size_t idx) const
  {
    return oninit_rewards[idx];
  }
  size_t GetOnexitRewardCount () const { return onexit_rewards.GetSize (); }
  iReward* GetOnexitReward (size_t idx) const
  {
    return onexit_rewards[idx];
  }
};

/**
 * A quest implementation.
 */
class celQuest : public scfImplementation1<celQuest, iQuest>
{
private:
  iCelPlLayer* pl;

  csPDelArray<celQuestState> states;
  size_t current_state;

  /**
   * Deactivate a state (deactivate all triggers).
   * If exec_onexit is true then the onexit rewards are fired.
   */
  void DeactivateState (size_t stateidx, bool exec_onexit);

  /// Load/switch state.
  bool SwitchState (const char* state, iCelDataBuffer* databuf);

  csRefArray<iCelSequence> sequences;
  iCelSequence* FindCelSequence (const char* name);

public:
  celQuest (iCelPlLayer* pl);
  virtual ~celQuest ();
  
  virtual bool SwitchState (const char* state);
  virtual const char* GetCurrentState () const;

  virtual bool LoadState (const char* state, iCelDataBuffer* databuf);
  virtual void SaveState (iCelDataBuffer* databuf);

  virtual iCelSequence* FindSequence (const char* name)
  {
    return FindCelSequence (name);
  }



  /// Add a state, returns the state index.
  size_t AddState (const char* name);
  /// Add a response for a state. Return response index.
  size_t AddStateResponse (size_t stateidx);
  /// Set trigger for a state and response.
  void SetStateTrigger (size_t stateidx, size_t responseidx,
  	iTrigger* trigger);
  /// Add reward for a state and response.
  void AddStateReward (size_t stateidx, size_t responseidx,
  	iReward* reward);

  /// Add oninit reward for a state.
  void AddOninitReward (size_t stateidx, iReward* reward);
  /// Add onexit reward for a state.
  void AddOnexitReward (size_t stateidx, iReward* reward);

  /**
   * Add a sequence. This will increase the ref count.
   */
  void AddSequence (iCelSequence* sequence);
};

/**
 * This is a manager for quests.
 */
class celQuestManager : public scfImplementation2<celQuestManager,
	iQuestManager, iComponent>
{
public:
  iObjectRegistry* object_reg;
  csWeakRef<iCelPlLayer> pl;
  csWeakRef<iVirtualClock> vc;
  csRef<iCelExpressionParser> expparser;

private:
  csHash<csRef<iTriggerType>,csStringBase> trigger_types;
  csHash<csRef<iRewardType>,csStringBase> reward_types;
  csHash<csRef<iSeqOpType>,csStringBase> seqop_types;
  csHash<csRef<celQuestFactory>,csStringBase> quest_factories;

public:
  celQuestManager (iBase* parent);
  virtual ~celQuestManager ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  virtual bool RegisterTriggerType (iTriggerType* trigger);
  virtual iTriggerType* GetTriggerType (const char* name);
  virtual bool RegisterRewardType (iRewardType* reward);
  virtual iRewardType* GetRewardType (const char* name);
  virtual bool RegisterSeqOpType (iSeqOpType* seqop);
  virtual iSeqOpType* GetSeqOpType (const char* name);

  virtual iQuestFactory* GetQuestFactory (const char* name);
  virtual iQuestFactory* CreateQuestFactory (const char* name);

  virtual void RemoveQuestFactory (const char* name);
  virtual void RemoveQuestFactories ();

  virtual bool Load (iDocumentNode* node);

  virtual iRewardFactory* AddNewStateReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* state_par);
  virtual iRewardFactory* AddDebugPrintReward (
  	iQuestTriggerResponseFactory* response,
  	const char* msg);
  virtual iChangePropertyRewardFactory* AddChangePropertyReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par);
  virtual iRewardFactory* AddInventoryReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par);
  virtual iRewardFactory* AddSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par,
	const char* delay_par);
  virtual iRewardFactory* AddCsSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* sequence_par, const char* delay_par);
  virtual iRewardFactory* AddSequenceFinishReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par);
  virtual iRewardFactory* AddCreateEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* template_par,
	const char* name_par,
    const celEntityTemplateParams &tpl_params);
  virtual iRewardFactory* AddDestroyEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par); 
  virtual iRewardFactory* AddMessageReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par,
	const char* id_par);
  virtual iRewardFactory* AddActionReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par,
	const char* id_par,
	const char* pcclass_par);
  virtual iTriggerFactory* SetTimeoutTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* timeout_par);
  virtual iTriggerFactory* SetEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par);
  virtual iTriggerFactory* SetMeshEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par);
  virtual iTriggerFactory* SetSequenceFinishTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par);
  virtual iTriggerFactory* SetPropertyChangeTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par, const char* value_par);
  virtual iTriggerFactory* SetTriggerTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, bool do_leave = false);
  virtual iTriggerFactory* SetWatchTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* target_entity_par,
	const char* checktime_par,
	const char* radius_par);
  virtual iTriggerFactory* SetOperationTrigger (
        iQuestTriggerResponseFactory* response,
        const char* operation_par,
        csRefArray<iTriggerFactory> &trigger_factories);
  virtual iTriggerFactory* SetInventoryTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par,
	const char* child_par);
  virtual iTriggerFactory* SetMessageTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par,
	const char* mask_par);
  virtual iTriggerFactory* SetMeshSelectTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par);
};

#endif // __CEL_TOOLS_QUESTS__

