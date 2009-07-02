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
#include "tools/expression.h"

#include "celtool/stdparams.h"
#include "physicallayer/pl.h"

struct iObjectRegistry;
struct iEvent;
class celQuestManager;
class celQuestFactory;
class celQuest;

//---------------------------------------------------------------------------

class celQuestConstantParameter : public scfImplementation1<celQuestConstantParameter,
  iQuestParameter>
{
private:
  celData data;
  csString str;	// This string is used to hold temporary conversion to string.

public:
  celQuestConstantParameter () : scfImplementationType (this) { }
  celQuestConstantParameter (const char* c) : scfImplementationType (this)
  {
    data.Set (c);
  }
  virtual ~celQuestConstantParameter () { }
  virtual const celData* GetData (iCelParameterBlock*)
  {
    return &data;
  }
  virtual const char* Get (iCelParameterBlock*);
  virtual const char* Get (iCelParameterBlock*, bool& changed)
  {
    changed = false;
    return Get (0);
  }
  virtual int32 GetLong (iCelParameterBlock*);
};

//---------------------------------------------------------------------------

class celQuestDynamicParameter : public scfImplementation1<celQuestDynamicParameter,
  iQuestParameter>
{
private:
  iObjectRegistry* object_reg;
  csStringID dynamic_id;
  csString parname;
  csString oldvalue;
  csString str;	// This string is used to hold temporary conversion to string.

public:
  celQuestDynamicParameter (iObjectRegistry* object_reg, csStringID dynamic_id,
      const char* parname) : scfImplementationType (this), object_reg (object_reg),
      dynamic_id (dynamic_id), parname (parname) { }
  virtual ~celQuestDynamicParameter () { }
  virtual const char* Get (iCelParameterBlock* params);
  virtual const char* Get (iCelParameterBlock* params, bool& changed);
  virtual const celData* GetData (iCelParameterBlock* params);
  virtual int32 GetLong (iCelParameterBlock* params);
};

//---------------------------------------------------------------------------

class celQuestExpressionParameter : public scfImplementation1<celQuestExpressionParameter,
  iQuestParameter>
{
private:
  iObjectRegistry* object_reg;
  iCelEntity* entity;
  celData data;
  csRef<iCelExpression> expression;
  csString parname;
  csString oldvalue;
  csString str;	// This string is used to hold temporary conversion to string.

public:
  celQuestExpressionParameter (iObjectRegistry* object_reg, iCelEntity* entity,
      iCelExpression* expression, const char* parname)
    : scfImplementationType (this), object_reg (object_reg),
      entity (entity), expression (expression), parname (parname) { }
  virtual ~celQuestExpressionParameter () { }
  virtual const char* Get (iCelParameterBlock* params);
  virtual const char* Get (iCelParameterBlock* params, bool& changed);
  virtual const celData* GetData (iCelParameterBlock* params);
  virtual int32 GetLong (iCelParameterBlock* params);
};

//---------------------------------------------------------------------------

/**
 * Sequence operation.
 */
struct celSeqOp
{
  csRef<iQuestSeqOp> seqop;
  size_t idx;
  csTicks start;
  csTicks end;
};

/**
 * A sequence.
 */
class celQuestSequence : public scfImplementation2<celQuestSequence,
	iQuestSequence,iCelTimerListener>
{
private:
  csArray<celSeqOp> seqops;
  csArray<celSeqOp> ops_in_progress;
  csString name;
  iCelPlLayer* pl;
  iVirtualClock* vc;
  size_t idx;
  csTicks start_time;
  csTicks total_time;

  csRefArray<iQuestSequenceCallback> callbacks;

  // Perform the sequence upto relative time 'rel'.
  void Perform (csTicks rel);

  // Fire the sequence callbacks.
  void FireSequenceCallbacks ();

public:
  celQuestSequence (const char* name, iCelPlLayer* pl,
  	iVirtualClock* vc);
  virtual ~celQuestSequence ();

  /**
   * Add a new seqop. Warning! Seqops must be added in ascending order
   * based on 'start'.
   */
  void AddSeqOp (iQuestSeqOp* seqop, csTicks start, csTicks end);

  /**
   * Set total time.
   */
  void SetTotalTime (csTicks total_time)
  {
    celQuestSequence::total_time = total_time;
  }

  /**
   * Save state of this sequence.
   */
  void SaveState (iCelDataBuffer* databuf);
  /**
   * Load state of this sequence.
   */
  bool LoadState (iCelDataBuffer* databuf);

  // --- For iQuestSequence -------------------------------
  virtual const char* GetName () const { return name; }
  virtual bool Start (csTicks delay);
  virtual void Finish ();
  virtual void Abort ();
  virtual bool IsRunning ();
  virtual void AddSequenceCallback (iQuestSequenceCallback* cb);
  virtual void RemoveSequenceCallback (iQuestSequenceCallback* cb);

  // --- For iCelTimerListener ----------------------------
  virtual void TickEveryFrame ();
  virtual void TickOnce () { }
};

/**
 * Sequence operation factory.
 */
struct celSeqOpFact
{
  csRef<iQuestSeqOpFactory> seqop;	// If 0 this is a delay.
  csString duration;			// Duration or parameter.
};

/**
 * A sequence factory.
 */
class celQuestSequenceFactory : public scfImplementation1<
	celQuestSequenceFactory, iQuestSequenceFactory>
{
private:
  celQuestFactory* parent_factory;
  csString name;
  // The array of sequence operations will be sorted due to the
  // presence of the csComparator specialization above.
  csArray<celSeqOpFact> seqops;

public:
  celQuestSequenceFactory (const char* name, celQuestFactory* fact);
  virtual ~celQuestSequenceFactory () { }

  csPtr<celQuestSequence> CreateSequence (const celQuestParams& params);

  virtual const char* GetName () const { return name; }
  virtual bool Load (iDocumentNode* node);
  virtual void AddSeqOpFactory (iQuestSeqOpFactory* seqopfact,
  	const char* duration);
  virtual void AddDelay (const char* delay);
};

//---------------------------------------------------------------------------

/**
 * A quest trigger response factory.
 */
class celQuestTriggerResponseFactory : public scfImplementation1<
	celQuestTriggerResponseFactory, iQuestTriggerResponseFactory>
{
private:
  csRef<iQuestTriggerFactory> trigger_factory;
  csRefArray<iQuestRewardFactory> reward_factories;
  csRefArray<iRewardFactory> reward_factories_NEW;

public:
  celQuestTriggerResponseFactory () : scfImplementationType (this) { }
  virtual ~celQuestTriggerResponseFactory () { }

  iQuestTriggerFactory* GetTriggerFactory () const
  {
    return trigger_factory;
  }
  const csRefArray<iQuestRewardFactory>& GetRewardFactories () const
  {
    return reward_factories;
  }

  const csRefArray<iRewardFactory>& GetRewardFactories_NEW () const
  {
    return reward_factories_NEW;
  }

  virtual void SetTriggerFactory (iQuestTriggerFactory* trigger_fact);
  virtual void AddRewardFactory (iQuestRewardFactory* reward_fact);
  virtual void AddRewardFactory_NEW (iRewardFactory* reward_fact);
};

/**
 * A quest state.
 */
class celQuestStateFactory : public scfImplementation1<
	celQuestStateFactory, iQuestStateFactory>
{
private:
  csString name;
  csRefArray<iQuestRewardFactory> oninit_reward_factories;
  csRefArray<iQuestRewardFactory> onexit_reward_factories;
  csRefArray<celQuestTriggerResponseFactory> responses;

public:
  celQuestStateFactory (const char* name);
  virtual ~celQuestStateFactory () { }

  const csRefArray<celQuestTriggerResponseFactory>& GetResponses () const
  {
    return responses;
  }
  const csRefArray<iQuestRewardFactory>& GetOninitRewardFactories () const
  {
    return oninit_reward_factories;
  }
  const csRefArray<iQuestRewardFactory>& GetOnexitRewardFactories () const
  {
    return onexit_reward_factories;
  }

  virtual const char* GetName () const { return name; }
  virtual iQuestTriggerResponseFactory* CreateTriggerResponseFactory ();
  virtual void AddInitRewardFactory (iQuestRewardFactory* reward_fact);
  virtual void AddExitRewardFactory (iQuestRewardFactory* reward_fact);
};

typedef csHash<csRef<celQuestStateFactory>,csStringBase> celQuestFactoryStates;
typedef csHash<csRef<celQuestSequenceFactory>,csStringBase>
	celQuestFactorySequences;

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
  celQuestFactorySequences sequences;
  celQuestParams defaults;

  csRef<iQuestRewardFactory> LoadReward (iDocumentNode* child);
  bool LoadRewards (iQuestStateFactory* statefact, bool oninit,
  	iDocumentNode* node);
  bool LoadTriggerResponse (iQuestTriggerResponseFactory* respfact,
  	iQuestTriggerFactory* trigfact, iDocumentNode* node);
  bool LoadState (iQuestStateFactory* statefact, iDocumentNode* node);

public:
  csStringHash xmltokens;
#define CS_TOKEN_ITEM_FILE "plugins/tools/quests/quests.tok"
#include "cstool/tokenlist.h"

public:
  celQuestFactory (celQuestManager* questmgr, const char* name);
  virtual ~celQuestFactory () { }

  celQuestManager* GetQuestManager () const { return questmgr; }

  virtual const char* GetName () const { return name; }
  virtual csPtr<iQuest> CreateQuest (
      const celQuestParams& params,
	  const celParams& params_NEW);
  virtual bool Load (iDocumentNode* node);
  virtual iQuestStateFactory* GetState (const char* name);
  virtual iQuestStateFactory* CreateState (const char* name);
  virtual iQuestSequenceFactory* GetSequence (const char* name);
  virtual iQuestSequenceFactory* CreateSequence (const char* name);
  virtual const char* GetDefaultParameter (const char* name) const;
  virtual void SetDefaultParameter (const char* name,const char* value);
  virtual void ClearDefaultParameters ();
};

/**
 * A trigger and rewards. This is basically a response for a quest.
 */
struct celQuestStateResponse : public scfImplementation1<
	celQuestStateResponse, iQuestTriggerCallback>
{
private:
  iCelPlLayer* pl;
  csRef<iQuestTrigger> trigger;
  csRefArray<iQuestReward> rewards;
  csRefArray<iReward> rewards_NEW;
  celQuest* quest;

public:
  celQuestStateResponse (iCelPlLayer* pl, celQuest* quest);
  virtual ~celQuestStateResponse () { }

  void SetTrigger (iQuestTrigger* trigger);
  iQuestTrigger* GetTrigger () const { return trigger; }
  void AddReward (iQuestReward* reward);
  void AddReward_NEW (iReward* reward);

  // --- For iQuestTriggerCallback ------------------------
  virtual void TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params);
};

/**
 * A state in a quest.
 */
class celQuestState
{
private:
  iCelPlLayer* pl;
  csString name;
  csRefArray<iQuestReward> oninit_rewards;
  csRefArray<iQuestReward> onexit_rewards;
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
  void AddOninitReward (iQuestReward* reward)
  {
    oninit_rewards.Push (reward);
  }
  void AddOnexitReward (iQuestReward* reward)
  {
    onexit_rewards.Push (reward);
  }
  size_t GetOninitRewardCount () const { return oninit_rewards.GetSize (); }
  iQuestReward* GetOninitReward (size_t idx) const
  {
    return oninit_rewards[idx];
  }
  size_t GetOnexitRewardCount () const { return onexit_rewards.GetSize (); }
  iQuestReward* GetOnexitReward (size_t idx) const
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

  csRefArray<celQuestSequence> sequences;
  celQuestSequence* FindCelSequence (const char* name);

public:
  celQuest (iCelPlLayer* pl);
  virtual ~celQuest ();
  
  virtual bool SwitchState (const char* state);
  virtual const char* GetCurrentState () const;

  virtual bool LoadState (const char* state, iCelDataBuffer* databuf);
  virtual void SaveState (iCelDataBuffer* databuf);

  virtual iQuestSequence* FindSequence (const char* name)
  {
    return FindCelSequence (name);
  }

  /// Add a state, returns the state index.
  size_t AddState (const char* name);
  /// Add a response for a state. Return response index.
  size_t AddStateResponse (size_t stateidx);
  /// Set trigger for a state and response.
  void SetStateTrigger (size_t stateidx, size_t responseidx,
  	iQuestTrigger* trigger);
  /// Add reward for a state and response.
  void AddStateReward (size_t stateidx, size_t responseidx,
  	iQuestReward* reward);
  void AddStateReward_NEW (size_t stateidx, size_t responseidx,
  	iReward* reward);

  /// Add oninit reward for a state.
  void AddOninitReward (size_t stateidx, iQuestReward* reward);
  /// Add onexit reward for a state.
  void AddOnexitReward (size_t stateidx, iQuestReward* reward);

  /**
   * Add a sequence. This will increase the ref count.
   */
  void AddSequence (celQuestSequence* sequence);
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
  csHash<csRef<iQuestTriggerType>,csStringBase> trigger_types;
  csHash<csRef<iQuestRewardType>,csStringBase> reward_types;
  csHash<csRef<iRewardType>,csStringBase> reward_types_NEW;
  csHash<csRef<iQuestSeqOpType>,csStringBase> seqop_types;
  csHash<csRef<celQuestFactory>,csStringBase> quest_factories;

  iCelExpressionParser* GetParser ();

public:
  celQuestManager (iBase* parent);
  virtual ~celQuestManager ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  virtual bool RegisterTriggerType (iQuestTriggerType* trigger);
  virtual iQuestTriggerType* GetTriggerType (const char* name);
  virtual bool RegisterRewardType (iQuestRewardType* reward);
  virtual bool RegisterRewardType_NEW (iRewardType* reward);
  virtual iQuestRewardType* GetRewardType (const char* name);
  virtual iRewardType* GetRewardType_NEW (const char* name);
  virtual bool RegisterSeqOpType (iQuestSeqOpType* seqop);
  virtual iQuestSeqOpType* GetSeqOpType (const char* name);

  virtual iQuestFactory* GetQuestFactory (const char* name);
  virtual iQuestFactory* CreateQuestFactory (const char* name);
  virtual csPtr<iQuestParameter> GetParameter (
  	const celQuestParams& params,
	const char* param);
  virtual void RemoveQuestFactory (const char* name);
  virtual void RemoveQuestFactories ();

  virtual const char* ResolveParameter (
  	const celQuestParams& params,
	const char* param);

  virtual csPtr<celVariableParameterBlock> GetParameterBlock (
  	const celQuestParams& params,
	const csArray<celParSpec>& parameters,
	csRefArray<iQuestParameter>& quest_parameters);
  virtual void FillParameterBlock (
        iCelParameterBlock* params,
	celVariableParameterBlock* act_params,
	const csArray<celParSpec>& parameters,
	const csRefArray<iQuestParameter>& quest_parameters);

  virtual bool Load (iDocumentNode* node);

  virtual iQuestRewardFactory* AddNewStateReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* state_par);
  virtual iQuestRewardFactory* AddDebugPrintReward (
  	iQuestTriggerResponseFactory* response,
  	const char* msg_par);
  virtual iRewardFactory* AddDebugPrintReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* msg);
  virtual iChangePropertyQuestRewardFactory* AddChangePropertyReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par);
  virtual iChangePropertyRewardFactory* AddChangePropertyReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par);
  virtual iQuestRewardFactory* AddInventoryReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par);
  virtual iQuestRewardFactory* AddSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par,
	const char* delay_par);
  virtual iQuestRewardFactory* AddCsSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* sequence_par, const char* delay_par);
  virtual iQuestRewardFactory* AddSequenceFinishReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par);
  virtual iQuestRewardFactory* AddCreateEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* template_par,
	const char* name_par,
    const celEntityTemplateParams &tpl_params);
  virtual iRewardFactory* AddCreateEntityReward_NEW (
  	iQuestTriggerResponseFactory* response,
	const char* template_par,
	const char* name_par,
    const celEntityTemplateParams &tpl_params);
  virtual iQuestRewardFactory* AddDestroyEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par);
 virtual iRewardFactory* AddDestroyEntityReward_NEW (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par);
  virtual iQuestTriggerFactory* SetTimeoutTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* timeout_par);
  virtual iQuestTriggerFactory* SetEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par);
  virtual iQuestTriggerFactory* SetMeshEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par);
  virtual iQuestTriggerFactory* SetSequenceFinishTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par);
  virtual iQuestTriggerFactory* SetPropertyChangeTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par, const char* value_par);
  virtual iQuestTriggerFactory* SetTriggerTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, bool do_leave = false);
  virtual iQuestTriggerFactory* SetWatchTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* target_entity_par,
	const char* checktime_par,
	const char* radius_par);
  virtual iQuestTriggerFactory* SetOperationTrigger (
        iQuestTriggerResponseFactory* response,
        const char* operation_par,
        csRefArray<iQuestTriggerFactory> &trigger_factories);
};

#endif // __CEL_TOOLS_QUESTS__

