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

#ifndef __CEL_TOOLS_QUESTS__
#define __CEL_TOOLS_QUESTS__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/strhash.h"
#include "csutil/weakref.h"
#include "csutil/parray.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

#include "physicallayer/pl.h"

struct iObjectRegistry;
struct iEvent;
class celQuestManager;
class celQuestFactory;

//---------------------------------------------------------------------------

/**
 * Sequence operation.
 */
struct celSeqOp
{
  csRef<iQuestSeqOp> seqop;
  csTicks start;
  csTicks end;
};

/**
 * A sequence.
 */
class celQuestSequence
{
private:
  csArray<celSeqOp> seqops;

public:
  celQuestSequence () { }
  ~celQuestSequence () { }

  /**
   * Add a new seqop. Warning! Seqops must be added in ascending order
   * based on 'start'.
   */
  void AddSeqOp (iQuestSeqOp* seqop, csTicks start, csTicks end);
};

/**
 * Sequence operation factory.
 */
struct celSeqOpFact
{
  csRef<iQuestSeqOpFactory> seqop;
  csTicks start;
  csTicks end;
};

CS_SPECIALIZE_TEMPLATE
class csComparator<celSeqOpFact, celSeqOpFact>
{
public:
  static int Compare (const celSeqOpFact& r1, const celSeqOpFact& r2)
  {
    if (r1.start < r2.start) return -1;
    else if (r1.start > r2.start) return 1;
    else return 0;
  }
};

/**
 * A sequence factory.
 */
class celQuestSequenceFactory : public iQuestSequenceFactory
{
private:
  celQuestFactory* parent_factory;
  char* name;
  // The array of sequence operations will be sorted due to the
  // presence of the csComparator specialization above.
  csArray<celSeqOpFact> seqops;
  csTicks total_time;

public:
  celQuestSequenceFactory (const char* name, celQuestFactory* fact);
  virtual ~celQuestSequenceFactory ();

  SCF_DECLARE_IBASE;

  /// Caller is responsible for deleting returned instance.
  celQuestSequence* CreateSequence (const celQuestParams& params);

  virtual const char* GetName () const { return name; }
  virtual bool Load (iDocumentNode* node);
  virtual void AddSeqOpFactory (iQuestSeqOpFactory* seqopfact,
  	csTicks start, csTicks end);
  virtual void SetTotalTime (csTicks total_time)
  {
    celQuestSequenceFactory::total_time = total_time;
  }
  virtual csTicks GetTotalTime () const { return total_time; }
};

//---------------------------------------------------------------------------

/**
 * A quest trigger response factory.
 */
class celQuestTriggerResponseFactory : public iQuestTriggerResponseFactory
{
private:
  csRef<iQuestTriggerFactory> trigger_factory;
  csRefArray<iQuestRewardFactory> reward_factories;

public:
  celQuestTriggerResponseFactory ();
  virtual ~celQuestTriggerResponseFactory ();

  iQuestTriggerFactory* GetTriggerFactory () const
  {
    return trigger_factory;
  }
  const csRefArray<iQuestRewardFactory>& GetRewardFactories () const
  {
    return reward_factories;
  }

  SCF_DECLARE_IBASE;

  virtual void SetTriggerFactory (iQuestTriggerFactory* trigger_fact);
  virtual void AddRewardFactory (iQuestRewardFactory* reward_fact);
};

/**
 * A quest state.
 */
class celQuestStateFactory : public iQuestStateFactory
{
private:
  char* name;
  csRefArray<celQuestTriggerResponseFactory> responses;

public:
  celQuestStateFactory (const char* name);
  virtual ~celQuestStateFactory ();

  const csRefArray<celQuestTriggerResponseFactory>& GetResponses () const
  {
    return responses;
  }

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual iQuestTriggerResponseFactory* CreateTriggerResponseFactory ();
};

typedef csHash<csRef<celQuestStateFactory>,csStrKey> celQuestFactoryStates;
typedef csHash<csRef<celQuestSequenceFactory>,csStrKey>
	celQuestFactorySequences;

/**
 * A quest factory.
 */
class celQuestFactory : public iQuestFactory
{
private:
  celQuestManager* questmgr;
  char* name;
  celQuestFactoryStates states;
  celQuestFactorySequences sequences;

  bool LoadTriggerResponse (iQuestTriggerResponseFactory* respfact,
  	iQuestTriggerFactory* trigfact, iDocumentNode* node);
  bool LoadState (iQuestStateFactory* statefact, iDocumentNode* node);

public:
  csStringHash xmltokens;
#define CS_TOKEN_ITEM_FILE "plugins/tools/quests/quests.tok"
#include "cstool/tokenlist.h"

public:
  celQuestFactory (celQuestManager* questmgr, const char* name);
  virtual ~celQuestFactory ();

  celQuestManager* GetQuestManager () const { return questmgr; }

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual csPtr<iQuest> CreateQuest (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);
  virtual iQuestStateFactory* GetState (const char* name);
  virtual iQuestStateFactory* CreateState (const char* name);
  virtual iQuestSequenceFactory* GetSequence (const char* name);
  virtual iQuestSequenceFactory* CreateSequence (const char* name);
};

/**
 * A trigger and rewards. This is basically a response for a quest.
 */
struct celQuestStateResponse :
	public iQuestTriggerCallback,
	public iCelTimerListener
{
private:
  iCelPlLayer* pl;
  csRef<iQuestTrigger> trigger;
  csRefArray<iQuestReward> rewards;

  // Count how many rewards we still have to hand out.
  size_t reward_counter;

public:
  celQuestStateResponse (iCelPlLayer* pl);
  virtual ~celQuestStateResponse ();

  void SetTrigger (iQuestTrigger* trigger);
  iQuestTrigger* GetTrigger () const { return trigger; }
  void AddReward (iQuestReward* reward);

  SCF_DECLARE_IBASE;

  // --- For iQuestTriggerCallback ------------------------
  virtual void TriggerFired (iQuestTrigger* trigger);
  // --- For iCelTimerListener ----------------------------
  virtual void TickEveryFrame ();
  virtual void TickOnce () { }
};

/**
 * A state in a quest.
 */
class celQuestState
{
private:
  iCelPlLayer* pl;
  char* name;
  csRefArray<celQuestStateResponse> responses;

public:
  celQuestState (iCelPlLayer* pl, const char* name)
  {
    celQuestState::pl = pl;
    celQuestState::name = csStrNew (name);
  }
  ~celQuestState () { delete[] name; }
  const char* GetName () const { return name; }
  size_t AddResponse ();
  size_t GetResponseCount () const { return responses.Length (); }
  celQuestStateResponse* GetResponse (size_t idx) const
  {
    return responses[idx];
  }
};

/**
 * A quest implementation.
 */
class celQuest : public iQuest
{
private:
  iCelPlLayer* pl;

  csPDelArray<celQuestState> states;
  size_t current_state;

  /// Deactivate a state (deactivate all triggers).
  void DeactivateState (size_t stateidx);

  /// Load/switch state.
  bool SwitchState (const char* state, iCelDataBuffer* databuf);

  csPDelArray<celQuestSequence> sequences;

public:
  celQuest (iCelPlLayer* pl);
  virtual ~celQuest ();

  SCF_DECLARE_IBASE;
  
  virtual bool SwitchState (const char* state);
  virtual const char* GetCurrentState () const;

  virtual bool LoadState (const char* state, iCelDataBuffer* databuf);
  virtual void SaveState (iCelDataBuffer* databuf);

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

  /**
   * Add a sequence. This class will assume ownership and delete
   * the sequence on destruction.
   */
  void AddSequence (celQuestSequence* sequence);
};

/**
 * This is a manager for quests.
 */
class celQuestManager : public iQuestManager
{
public:
  iObjectRegistry* object_reg;
  csWeakRef<iCelPlLayer> pl;

private:
  csHash<csRef<iQuestTriggerType>,csStrKey> trigger_types;
  csHash<csRef<iQuestRewardType>,csStrKey> reward_types;
  csHash<csRef<iQuestSeqOpType>,csStrKey> seqop_types;
  csHash<csRef<celQuestFactory>,csStrKey> quest_factories;

public:
  celQuestManager (iBase* parent);
  virtual ~celQuestManager ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual bool RegisterTriggerType (iQuestTriggerType* trigger);
  virtual iQuestTriggerType* GetTriggerType (const char* name);
  virtual bool RegisterRewardType (iQuestRewardType* reward);
  virtual iQuestRewardType* GetRewardType (const char* name);
  virtual bool RegisterSeqOpType (iQuestSeqOpType* seqop);
  virtual iQuestSeqOpType* GetSeqOpType (const char* name);

  virtual iQuestFactory* GetQuestFactory (const char* name);
  virtual iQuestFactory* CreateQuestFactory (const char* name);
  virtual const char* ResolveParameter (
  	const celQuestParams& params,
	const char* param);
  virtual bool Load (iDocumentNode* node);

  virtual iQuestRewardFactory* AddNewStateReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* state_par);
  virtual iQuestRewardFactory* AddDebugPrintReward (
  	iQuestTriggerResponseFactory* response,
  	const char* msg_par);
  virtual iChangePropertyQuestRewardFactory* AddChangePropertyReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par);
  virtual iQuestRewardFactory* AddInventoryReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par);

  virtual iQuestTriggerFactory* SetTimeoutTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* timeout_par);
  virtual iQuestTriggerFactory* SetEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par);
  virtual iQuestTriggerFactory* SetMeshEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par);
  virtual iQuestTriggerFactory* SetPropertyChangeTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par, const char* value_par);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celQuestManager);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_TOOLS_QUESTS__

