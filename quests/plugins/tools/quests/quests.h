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
  csRef<iTriggerFactory> trigger_factory_NEW;
  csRefArray<iRewardFactory> reward_factories;

public:
  bool refactored_trigger;

  celQuestTriggerResponseFactory () : scfImplementationType (this) { }
  virtual ~celQuestTriggerResponseFactory () { }

  iQuestTriggerFactory* GetTriggerFactory () const
  {
	return trigger_factory;
  }

  iTriggerFactory* GetTriggerFactory_NEW () const
  {
	return trigger_factory_NEW;
  }

  const csRefArray<iRewardFactory>& GetRewardFactories () const
  {
    return reward_factories;
  }


  virtual void SetTriggerFactory (iQuestTriggerFactory* trigger_fact);
  virtual void SetTriggerFactory_NEW (iTriggerFactory* trigger_fact);
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
typedef csHash<csRef<celQuestSequenceFactory>,csStringBase>
	celQuestFactorySequences;
typedef csHash<csRef<iCelSequenceFactory>,csStringBase>
	celFactorySequences;

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
  celFactorySequences sequences_NEW;
  celQuestParams defaults;

  csRef<iRewardFactory> LoadReward (iDocumentNode* child);
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

  virtual celQuestManager* GetQuestManager () const { return questmgr; }

  virtual const char* GetName () const { return name; }
  virtual csPtr<iQuest> CreateQuest (
      const celQuestParams& params,
	  const celParams& params_NEW);
  virtual bool Load (iDocumentNode* node);
  virtual iQuestStateFactory* GetState (const char* name);
  virtual iQuestStateFactory* CreateState (const char* name);
  virtual iQuestSequenceFactory* GetSequence (const char* name);
  virtual iQuestSequenceFactory* CreateSequence (const char* name);
  virtual iCelSequenceFactory* GetSequence_NEW (const char* name);
  virtual iCelSequenceFactory* CreateSequence_NEW (const char* name);
  virtual const char* GetDefaultParameter (const char* name) const;
  virtual void SetDefaultParameter (const char* name,const char* value);
  virtual void ClearDefaultParameters ();
};

/**
 * A trigger and rewards. This is basically a response for a quest.
 */
struct celQuestStateResponse : public scfImplementation2<
	celQuestStateResponse, iQuestTriggerCallback, iTriggerCallback>
{
private:
  iCelPlLayer* pl;
  csRef<iQuestTrigger> trigger;
  csRef<iTrigger> trigger_NEW;
  csRefArray<iReward> rewards;
  celQuest* quest;

public:
  bool refactored_trigger;

  celQuestStateResponse (iCelPlLayer* pl, celQuest* quest);
  virtual ~celQuestStateResponse () { }

  void SetTrigger (iQuestTrigger* trigger);
  iQuestTrigger* GetTrigger () const { return trigger; }
  void SetTrigger_NEW (iTrigger* trigger);
  iTrigger* GetTrigger_NEW () const { return trigger_NEW; }
  void AddReward (iReward* reward);

  // --- For iQuestTriggerCallback ------------------------
  virtual void TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params);

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

  csRefArray<celQuestSequence> sequences;
  csRefArray<iCelSequence> sequences_NEW;
  celQuestSequence* FindCelSequence (const char* name);
  iCelSequence* FindCelSequence_NEW (const char* name);

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

  virtual iCelSequence* FindSequence_NEW (const char* name)
  {
    return FindCelSequence_NEW (name);
  }

  /// Add a state, returns the state index.
  size_t AddState (const char* name);
  /// Add a response for a state. Return response index.
  size_t AddStateResponse (size_t stateidx);
  /// Set trigger for a state and response.
  void SetStateTrigger (size_t stateidx, size_t responseidx,
  	iQuestTrigger* trigger);
  void SetStateTrigger_NEW (size_t stateidx, size_t responseidx,
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
  void AddSequence (celQuestSequence* sequence);
  void AddSequence_NEW (iCelSequence* sequence);
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
  csHash<csRef<iTriggerType>,csStringBase> trigger_types_NEW;
  csHash<csRef<iRewardType>,csStringBase> reward_types;
  csHash<csRef<iQuestSeqOpType>,csStringBase> seqop_types;
  csHash<csRef<iSeqOpType>,csStringBase> seqop_types_NEW;
  csHash<csRef<celQuestFactory>,csStringBase> quest_factories;

  iCelExpressionParser* GetParser ();

public:
  celQuestManager (iBase* parent);
  virtual ~celQuestManager ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  virtual bool RegisterTriggerType (iQuestTriggerType* trigger);
  virtual bool RegisterTriggerType_NEW (iTriggerType* trigger);
  virtual iQuestTriggerType* GetTriggerType (const char* name);
  virtual iTriggerType* GetTriggerType_NEW (const char* name);
  virtual bool RegisterRewardType (iRewardType* reward);
  virtual iRewardType* GetRewardType (const char* name);
  virtual bool RegisterSeqOpType (iQuestSeqOpType* seqop);
  virtual bool RegisterSeqOpType_NEW (iSeqOpType* seqop);
  virtual iQuestSeqOpType* GetSeqOpType (const char* name);
  virtual iSeqOpType* GetSeqOpType_NEW (const char* name);

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
  virtual iQuestTriggerFactory* SetTimeoutTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* timeout_par);
  virtual iTriggerFactory* SetTimeoutTrigger_NEW (
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
  virtual iTriggerFactory* SetEnterSectorTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par);
  virtual iTriggerFactory* SetMeshEnterSectorTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par);
  virtual iTriggerFactory* SetSequenceFinishTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par);
  virtual iQuestTriggerFactory* SetPropertyChangeTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par, const char* value_par);
  virtual iTriggerFactory* SetPropertyChangeTrigger_NEW (
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
  virtual iTriggerFactory* SetTriggerTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, bool do_leave = false);
  virtual iTriggerFactory* SetWatchTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* target_entity_par,
	const char* checktime_par,
	const char* radius_par);
  virtual iTriggerFactory* SetOperationTrigger_NEW (
        iQuestTriggerResponseFactory* response,
        const char* operation_par,
        csRefArray<iTriggerFactory> &trigger_factories);
  virtual iQuestTriggerFactory* SetInventoryTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, 
	const char* child_par);
  virtual iTriggerFactory* SetInventoryTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par,
	const char* child_par);
  virtual iQuestTriggerFactory* SetMessageTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, 
	const char* mask_par);
  virtual iTriggerFactory* SetMessageTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par,
	const char* mask_par);
  virtual iQuestTriggerFactory* SetMeshSelectTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par);
  virtual iTriggerFactory* SetMeshSelectTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par);
};

#endif // __CEL_TOOLS_QUESTS__

