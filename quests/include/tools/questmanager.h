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

#ifndef __CEL_MGR_QUEST__
#define __CEL_MGR_QUEST__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "csgeom/vector3.h"
#include "csutil/refarr.h"
#include "tools/rewards.h"
#include "tools/sequences.h"
#include "tools/triggers.h"
#include "physicallayer/pl.h"

struct iDocumentNode;
struct iChangePropertyQuestRewardFactory;
struct iChangePropertyRewardFactory;
struct iCelDataBuffer;
struct iCelParameterBlock;
struct iQuest;

class celQuestManager;

/**
 * This interface represents a parameter to a sequence,
 * trigger, or reward for a quest. The quest manager understands
 * different types of parameters (constant, static parameters ($notation),
 * dynamic parameters (@notation), or expressions (=notation).
 * This interface makes abstraction of those.
 */
struct iQuestParameter : public virtual iBase
{
  SCF_INTERFACE (iQuestParameter, 0, 0, 2);

  /**
   * Get this variable as its correct datatype.
   */
  virtual const celData* GetData (iCelParameterBlock* params) = 0;

  /**
   * Get the value of this expression as a string.
   * \param params is an optional parameter block given to the reward.
   */
  virtual const char* Get (iCelParameterBlock* params) = 0;

  /**
   * Get the value of this expression as a string.
   * \param params is an optional parameter block given to the reward.
   * \param changed is set to true if the returned value is different
   * from the last time Get() was called. Note! This doesn't work
   * if you call the Get() with only one parameter above!
   */
  virtual const char* Get (iCelParameterBlock* params, bool& changed) = 0;

  /**
   * Get the value of this expression as a long.
   * \param params is an optional parameter block given to the reward.
   */
  virtual int32 GetLong (iCelParameterBlock* params) = 0;
};


/*
<quest name="test">
    <state name="notstarted">
    </state>

    <state name="start">
        <trigger type="ininventory">
	    <fireon entity="$actor_entity" new_entity="computer_part" />
	    <reward type="newstate" state="state2" />
	    <reward type="increasestats" statname="HP" value="3" />
	</trigger>
	<trigger type="objectdestroyed">
	    <fireon entity="barrel" />
	    <reward type="newstate" state="finalstate" />
	    <reward type="increasestats" statname="HP" value="10" />
	</trigger>
    </state>

    <state name="state2">
        <trigger type="clickonentity">
	    <fireon entity="" />
	</trigger>
        <trigger type="cel.questtrigger.entersector">
	    <fireon entity="player" sector="room" />
	</trigger>
    </state>

    <sequence name="seqTest">
    	<op type="movemesh" duration="2000" entity="door"
		x="0" y="4" z="0" />
    	<op type="lightcolor" duration="2000" entity="lightbulb"
		red="1" green="1" blue="1" />
    	<delay time="2000" />
    </sequence>

    <start>notstarted</start>
</quest>
*/

typedef csHash<csStringBase,csStringBase> celQuestParams;

//-------------------------------------------------------------------------
// Triggers
//-------------------------------------------------------------------------

struct iQuestTrigger;
class celVariableParameterBlock;

//struct celParSpec
//{
 // celDataType type;
  //csStringID id;
  //csString name;
  //csString value;
//};

/**
 * A quest trigger will get pointers to call back the quest when
 * the trigger fires through this interface. Application code will
 * typically not implement this interface as it is implemented
 * by the quest manager itself.
 */
struct iQuestTriggerCallback : public virtual iBase
{
  SCF_INTERFACE (iQuestTriggerCallback, 0, 0, 1);

  /// Trigger fired.
  virtual void TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params) = 0;
};

/**
 * This is a trigger for a quest. When a trigger fires the quest
 * can go to another state and/or activate rewards. The quest manager
 * knows a few standard triggers but you can also create your own
 * triggers here. Triggers are created by trigger factories. To
 * be able to use your own trigger implementations you must also make
 * associated trigger factories which you then register with the quest
 * manager. See iQuestTriggerFactory.
 *
 * Triggers must start in deactivated state. They will be activated by
 * the quest when the 'state' containing this trigger is activated.
 */
struct iQuestTrigger : public virtual iBase
{
  SCF_INTERFACE (iQuestTrigger, 0, 0, 1);

  /**
   * Register a callback with this trigger. When the trigger fires
   * it will call this callback. A trigger supports only one callback.
   */
  virtual void RegisterCallback (iQuestTriggerCallback* callback) = 0;

  /**
   * Clear the callback.
   */
  virtual void ClearCallback () = 0;

  /**
   * Activate the trigger. Note that triggers start deactivated.
   */
  virtual void ActivateTrigger () = 0;

  /**
   * Deactivate the trigger. Note that triggers start deactivated.
   */
  virtual void DeactivateTrigger () = 0;

  /**
   * Check the trigger immediatelly and return true if it is valid.
   * This function does nothing else. Only the condition of the
   * trigger is tested. If this function returns true then it should
   * also have deactivated the trigger and fired the callbacks. This
   * is not the responsability of the caller.
   */
  virtual bool Check () = 0;

  /**
   * Activate the trigger and load state from databuf (persistence).
   * \return false on failure (data in buffer doesn't match what we
   * expect).
   */
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf) = 0;

  /**
   * Save trigger state.
   */
  virtual void SaveTriggerState (iCelDataBuffer* databuf) = 0;
};

/**
 * A factory to create triggers. Trigger factories are created
 * by an iQuestTriggerType instance.
 */
struct iQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestTriggerFactory, 0, 0, 1);

  /**
   * Create a trigger.
   * \param quest is the quest for which this trigger is created.
   * \param params are the parameters with which this reward is
   * instantiated.
   */
  virtual csPtr<iQuestTrigger> CreateTrigger (iQuest* quest,
  	const celQuestParams& params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<fireon\> node in a trigger description.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;
};

/**
 * The trigger type is responsible for the creation of trigger factories.
 * If you want to define a new type of trigger then you must implement
 * iQuestTriggerType, iQuestTriggerFactory, and iQuestTrigger.
 * You register trigger types with the quest manager so that they can
 * be used by quest factories.
 */
struct iQuestTriggerType : public virtual iBase
{
  SCF_INTERFACE (iQuestTriggerType, 0, 0, 1);

  /**
   * Return the name for this trigger type.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Create a trigger factory.
   */
  virtual csPtr<iQuestTriggerFactory> CreateTriggerFactory () = 0;
};

//-------------------------------------------------------------------------
// Rewards
//-------------------------------------------------------------------------

/**
 * This is a reward for a quest. The quest manager can issue
 * rewards in response to some trigger. Just like triggers you can
 * implement your own rewards in addition to the standard rewards
 * already implemented in the quest manager. You also need to implement
 * a quest reward factory then (see iQuestRewardFactory).
 */
struct iQuestReward : public virtual iBase
{
  SCF_INTERFACE (iQuestReward, 0, 0, 1);
  /**
   * Perform this reward.
   */
  virtual void Reward (iCelParameterBlock* params) = 0;
};

/**
 * A factory to create rewards. Reward factories are created
 * by an iQuestRewardType instance.
 */
struct iQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestRewardFactory, 0, 0, 1);

  /**
   * Create a reward.
   * \param quest is the quest for which we are creating this reward.
   * \param params are the parameters with which this reward is
   * instantiated.
   */
  virtual csPtr<iQuestReward> CreateReward (iQuest* quest,
  	const celQuestParams& params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<reward\> node in a trigger description.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;
};

/**
 * The reward type is responsible for the creation of reward factories.
 * If you want to define a new type of reward then you must implement
 * iQuestRewardType, iQuestRewardFactory, and iQuestReward.
 * You register reward types with the quest manager so that they can
 * be used by quest factories.
 */
struct iQuestRewardType : public virtual iBase
{
  SCF_INTERFACE (iQuestRewardType, 0, 0, 1);

  /**
   * Return the name for this reward type.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Create a reward factory.
   */
  virtual csPtr<iQuestRewardFactory> CreateRewardFactory () = 0;
};

//-------------------------------------------------------------------------
// Sequence operations
//-------------------------------------------------------------------------

/**
 * This is a sequence operation in a quest. A sequence operation represents
 * an operation that has a duration or an operation that is a single-shot.
 * You can combine different sequence operations in a sequence and then use
 * a reward to fire of the sequence as result of a trigger.
 */
struct iQuestSeqOp : public virtual iBase
{
  SCF_INTERFACE (iQuestSeqOp, 0, 0, 1);
  
  /**
   * Init the operation. This is called before the operation is actually
   * performed the first time. This function is not called in case the
   * operation is loaded from persisted data. In that case Load()
   * is called instead.
   */
  virtual void Init () = 0;

  /**
   * Load the sequence operation from persisted data.
   */
  virtual bool Load (iCelDataBuffer* databuf) = 0;

  /**
   * Save the sequence operation to persisted data.
   */
  virtual void Save (iCelDataBuffer* databuf) = 0;

  /**
   * Do the operation. The parameter is a value between 0 and 1 which
   * will be interpolated over a specified time (specified in the sequence).
   * In case this is a single-shot operation the value will always be 1.
   */
  virtual void Do (float time) = 0;
};

/**
 * A factory to create sequence operators. Factories are created
 * by an iQuestSeqOpType instance.
 */
struct iQuestSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestSeqOpFactory, 0, 0, 1);

  /**
   * Create a sequence operation.
   * \param params are the parameters with which this reward is
   * instantiated.
   */
  virtual csPtr<iQuestSeqOp> CreateSeqOp (const celQuestParams& params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the node for the operation.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;
};

/**
 * The seqop type is responsible for the creation of seqop factories.
 * If you want to define a new type of sequence operation then you must
 * implement iQuestSeqOpType, iQuestSeqOpFactory, and iQuestSeqOp.
 * You register seqop types with the quest manager so that they can
 * be used by seqop factories.
 */
struct iQuestSeqOpType : public virtual iBase
{
  SCF_INTERFACE (iQuestSeqOpType, 0, 0, 1);

  /**
   * Return the name for this sequence operation type.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Create a sequence operation factory.
   */
  virtual csPtr<iQuestSeqOpFactory> CreateSeqOpFactory () = 0;
};

struct iQuestSequence;

/**
 * This callback is fired when the sequences finished running properly.
 * It is not called it the sequence is aborted!
 */
struct iQuestSequenceCallback : public virtual iBase
{
  SCF_INTERFACE (iQuestSequenceCallback, 0, 0, 1);

  /// Sequence finishes.
  virtual void SequenceFinished (iQuestSequence* sequence) = 0;
};

/**
 * A sequence.
 */
struct iQuestSequence : public virtual iBase
{
  SCF_INTERFACE (iQuestSequence, 0, 0, 1);

  /**
   * Get the name of this sequence.
   */
  virtual const char* GetName () const = 0;

  /**
   * Fire this sequence.
   * \param delay is a delay before the sequence will really start.
   * \return false if the sequence is already running.
   */
  virtual bool Start (csTicks delay) = 0;

  /**
   * Finish this sequence. The sequence will be executed at end condition
   * which means that all operations that have not yet fully executed
   * will be executed at relative time index '1'. So the end result
   * will be as if the sequence completed normally.
   */
  virtual void Finish () = 0;

  /**
   * Abort this sequence. This will simply abort the sequence without
   * doing any further operation. This can leave the objects on which
   * this sequence operates in undefined states.
   */
  virtual void Abort () = 0;

  /**
   * Return true if this sequence is currently running.
   */
  virtual bool IsRunning () = 0;

  /**
   * Register a callback that is fired when this sequence finishes.
   */
  virtual void AddSequenceCallback (iQuestSequenceCallback* cb) = 0;
  /**
   * Remove a callback.
   */
  virtual void RemoveSequenceCallback (iQuestSequenceCallback* cb) = 0;
};

//-------------------------------------------------------------------------
// The Quest
//-------------------------------------------------------------------------

/**
 * A quest instance. This is created (by the quest manager) from a quest
 * factory using the trigger and reward factories.
 */
struct iQuest : public virtual iBase
{
  SCF_INTERFACE (iQuest, 0, 0, 1);

  /**
   * Switch this quest to some specific state.
   * Returns false if state doesn't exist (nothing happens then).
   */
  virtual bool SwitchState (const char* state) = 0;

  /**
   * Get current state name of this quest.
   */
  virtual const char* GetCurrentState () const = 0;

  /**
   * This is a special version of SwitchState() that needs to be used
   * in case you are loading a quest from a CEL databuffer (persistence).
   * \return false on failure (data in buffer doesn't match what we want).
   */
  virtual bool LoadState (const char* state, iCelDataBuffer* databuf) = 0;

  /**
   * Save the state to the persistence layer. To restore later use
   * LoadState().
   */
  virtual void SaveState (iCelDataBuffer* databuf) = 0;

  /**
   * Find a sequence.
   */
  virtual iQuestSequence* FindSequence (const char* name) = 0;
  virtual iCelSequence*   FindSequence_NEW (const char* name) = 0;
};

/**
 * A trigger with response as used in a quest state.
 * This is basically the representation of one trigger and one
 * or more rewards.
 */
struct iQuestTriggerResponseFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestTriggerResponseFactory, 0, 0, 1);

  /**
   * Set the trigger factory that is managed by this
   * response factory.
   */
  virtual void SetTriggerFactory (iQuestTriggerFactory* trigger_fact) = 0;
  virtual void SetTriggerFactory_NEW (iTriggerFactory* trigger_fact) = 0;

  /**
   * Add a reward factory. A reward of this factory will be obtained
   * when the trigger fires.
   */
  virtual void AddRewardFactory (iQuestRewardFactory* reward_fact) = 0;
  virtual void AddRewardFactory_NEW (iRewardFactory* reward_fact) = 0;
};

/**
 * A representation of a quest state in a quest factory.
 * A state is basically a collection of trigger response factories.
 */
struct iQuestStateFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestStateFactory, 0, 0, 1);

  /**
   * Get the name of this state.
   */
  virtual const char* GetName () const = 0;

  /**
   * Create a new trigger response.
   */
  virtual iQuestTriggerResponseFactory* CreateTriggerResponseFactory () = 0;

  /**
   * Add a new reward to be fired on state initialization.
   */
  virtual void AddInitRewardFactory (iQuestRewardFactory* reward_fact) = 0;

  /**
   * Add a new reward to be fired on state exit.
   */
  virtual void AddExitRewardFactory (iQuestRewardFactory* reward_fact) = 0;
};

/**
 * A representation of a sequence factory.
 * A sequence factory is basically a sequence of sequence operation factories.
 */
struct iQuestSequenceFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestSequenceFactory, 0, 0, 1);

  /**
   * Get the name of this factory.
   */
  virtual const char* GetName () const = 0;

  /**
   * Load this sequence factory from a document node.
   * \param node is the \<sequence\> node in a quest.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;

  /**
   * Add a new operation factory at the specified time.
   * \param seqopfact is the factory to add.
   * \param duration is the duration of this operation. This can be
   * a string representing a numerical value or a parameter ($x notation).
   * The duration can be 0 in which case we have a single-stop event.
   */
  virtual void AddSeqOpFactory (iQuestSeqOpFactory* seqopfact,
  	const char* duration) = 0;

  /**
   * Add a delay.
   * \param delay is a the delay or a parameter.
   */
  virtual void AddDelay (const char* delay) = 0;
};

/**
 * A quest factory. A quest factory is a template to create a quest
 * from. All interfaces ending with 'Factory' are relevant in the concept
 * of a quest factory.
 * <ul>
 * <li>A quest factory is made out of a number of quest state
 *     factories (iQuestStateFactory) (from which the states in the quest
 *     are made).
 * <li>Every quest state factory has one or more trigger response
 *     factories. These response factories describe how the firing of
 *     one trigger (as described by a trigger factory) can cause a number
 *     of rewards (as described by reward factories).
 * </ul>
 */
struct iQuestFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestFactory, 0, 0, 1);

  virtual celQuestManager* GetQuestManager () const = 0;

  /**
   * Get the name of this factory.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Instantiate a quest from this factory. This instance will have
   * the same structure as this factory but it will be made out of
   * non-factory objects (iQuestTrigger, iQuestReward, ...).
   * \param params are the parameters with which this quest is
   * instantiated.
   */
  virtual csPtr<iQuest> CreateQuest (
	  const celQuestParams& params,
	  const celParams& params_NEW) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<quest\> node.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;

  /**
   * Get a state from this factory.
   * Return 0 if the state doesn't exist.
   */
  virtual iQuestStateFactory* GetState (const char* name) = 0;

  /**
   * Create a new state in this factory.
   * Return 0 on failure (name already exists).
   */
  virtual iQuestStateFactory* CreateState (const char* name) = 0;

  /**
   * Get a sequence factory in this factory.
   * Return 0 if the factory doesn't exist.
   */
  virtual iQuestSequenceFactory* GetSequence (const char* name) = 0;
  virtual iCelSequenceFactory* GetSequence_NEW (const char* name) = 0;

  /**
   * Create a new sequence factory in this factory.
   * Return 0 on failure (name already exists).
   */
  virtual iQuestSequenceFactory* CreateSequence (const char* name) = 0;
  virtual iCelSequenceFactory* CreateSequence_NEW (const char* name) = 0;

  /**
   * Get a default parameter from this factory.
   * Return 0 if the parameter does not exist.
   */
  virtual const char* GetDefaultParameter (const char* name) const = 0;

  /**
   * Set a default parameter on this factory.
   * If the parameter exists it will be overwritten.
   */
  virtual void SetDefaultParameter (const char* name,const char* value) = 0;
  
  /**
   * Clear all default parameters on this factory.
   */
  virtual void ClearDefaultParameters () = 0;

};

//-------------------------------------------------------------------------
// The Quest Manager
//-------------------------------------------------------------------------

/**
 * This is the quest manager. This manager can be used to handle quests.
 * A quest is basically a state machine which can advance from state to
 * state when a trigger activates. It can also hand out 'rewards'.
 */
struct iQuestManager : public virtual iBase
{
  SCF_INTERFACE (iQuestManager, 2, 0, 0);

  /**
   * Register a quest trigger type. Quest triggers can be used
   * by quests to decide when to go to another state or when
   * to activate a reward. Returns false on failure (trigger
   * type with that name already exists).
   *
   * The following predefined trigger types are automatically
   * registered in the quest manager:
   * - cel.questtrigger.entersector: triggers when a camera (from
   *   entity) enters a sector. See iEnterSectorQuestTriggerFactory.
   * - cel.questtrigger.meshentersector: triggers when a mesh (from
   *   entity) enters a sector. See iEnterSectorQuestTriggerFactory.
   * - cel.questtrigger.timeout: triggers after a specified time.
   *   See iTimeoutQuestTriggerFactory.
   * - cel.questtrigger.propertychange: triggers when a property changes.
   *   See iPropertyChangeQuestTriggerFactory.
   * - cel.questtrigger.sequencefinish: triggers when a sequence finishes.
   *   See iSequenceFinishQuestTriggerFactory.
   * - cel.questtrigger.trigger: triggers when a pctrigger fires.
   *   See iTriggerQuestTriggerFactory.
   * - cel.questtrigger.message: triggers when a message is received.
   *   See iMessageQuestTriggerFactory.
   * - cel.questtrigger.inventory: triggers when an object enters inventory.
   *   See iInventoryQuestTriggerFactory.
   * - cel.questtrigger.meshselect: triggers when a mesh is selected.
   *   See iMeshSelectQuestTriggerFactory.
   * - cel.questtrigger.watch: triggers when a mesh becomes visible.
   *   See iWatchQuestTriggerFactory.
   * - cel.questtrigger.operation: perform a logical operation on several
   *   child nodes.
   *   See iOperationQuestTriggerFactory.
   */
  virtual bool RegisterTriggerType (iQuestTriggerType* trigger) = 0;
  virtual bool RegisterTriggerType_NEW (iTriggerType* trigger) = 0;

  /**
   * Get a trigger type from the quest manager.
   * Returns 0 if no such trigger type exists.
   */
  virtual iQuestTriggerType* GetTriggerType (const char* name) = 0;
  virtual iTriggerType* GetTriggerType_NEW (const char* name) = 0;

  /**
   * Register a quest reward type. Quest rewards can be used
   * by quests to give out some kind of reward to the game.
   * Returns false on failure (reward type with that name
   * already exists).
   *
   * The following predefined reward types are automatically
   * registered in the quest manager:
   * - cel.questreward.debugprint: print a debug message on stdout.
   *   See iDebugPrintQuestRewardFactory.
   * - cel.questreward.newstate: switch to a new state.
   *   See iNewStateQuestRewardFactory.
   * - cel.questreward.changeproperty: change a property.
   *   See iChangePropertyQuestRewardFactory.
   * - cel.questreward.inventory: manipulate inventory.
   *   See iInventoryQuestRewardFactory.
   * - cel.questreward.sequence: fire sequence.
   *   See iSequenceQuestRewardFactory.
   * - cel.questreward.sequencefinish: finish sequence.
   *   See iSequenceFinishQuestRewardFactory.
   * - cel.questreward.message: send a message to some entity.
   *   See iMessageQuestRewardFactory.
   * - cel.questreward.action: send an action to some property class
   *   on an entity. See iActionQuestRewardFactory.
   * - cel.questreward.destroyentity: remove an entity from the physical.
   *   layer. See iDestroyEntityQuestRewardFactory.
   * - cel.questreward.createentity: create an entity from a template.
   *   See iCreateEntityQuestRewardFactory.
   */
  virtual bool RegisterRewardType (iQuestRewardType* trigger) = 0;
  virtual bool RegisterRewardType_NEW (iRewardType* trigger) = 0;

  /**
   * Get a reward type from the quest manager.
   * Returns 0 if no such reward type exists.
   */
  virtual iQuestRewardType* GetRewardType (const char* name) = 0;
  virtual iRewardType* GetRewardType_NEW (const char* name) = 0;

  /**
   * Register a seqop reward type. Seqop rewards can be used
   * by quest sequences to define operations in the sequence.
   * Returns false on failure (swqop type with that name
   * already exists).
   *
   * The following predefined sequence operation types are automatically
   * registered in the quest manager:
   * - cel.questseqop.debugprint: print a debug message on stdout.
   *   See iDebugPrintQuestSeqOpFactory.
   * - cel.questseqop.transform: transform a mesh.
   *   See iTransformQuestSeqOpFactory.
   * - cel.questseqop.movepath: move a mesh along a path.
   *   See iMovePathQuestSeqOpFactory.
   * - cel.questseqop.light: animate a light color.
   *   See iLightQuestSeqOpFactory.
   * - cel.questseqop.property: animate a property class property.
   *   See iPropertyQuestSeqOpFactory.
   */
  virtual bool RegisterSeqOpType (iQuestSeqOpType* seqop) = 0;
  virtual bool RegisterSeqOpType_NEW (iSeqOpType* seqop) = 0;

  /**
   * Get a seqop type from the quest manager.
   * Returns 0 if no such seqop type exists.
   */
  virtual iQuestSeqOpType* GetSeqOpType (const char* name) = 0;
  virtual iSeqOpType* GetSeqOpType_NEW (const char* name) = 0;

  /**
   * Get a quest factory by name.
   */
  virtual iQuestFactory* GetQuestFactory (const char* name) = 0;

  /**
   * Delete a quest factory by name.
   */
  virtual void RemoveQuestFactory (const char* name) = 0;

  /**
   * Delete all quest factories.
   */
  virtual void RemoveQuestFactories () = 0;

  /**
   * Create an empty quest factory.
   * Returns 0 on failure (quest factory with that name
   * already exists).
   */
  virtual iQuestFactory* CreateQuestFactory (const char* name) = 0;

  /**
   * Get a parameter that can be evalulated later on an as-needed basis.
   * Returns 0 for an illegal parameter (error reporting has been done).
   */
  virtual csPtr<iQuestParameter> GetParameter (
  	const celQuestParams& params,
	const char* param) = 0;

  /**
   * This is a convenience function to resolve a quest parameter during
   * creation of rewards, triggers, and sequence operations. This routine
   * knows how to recognize parameter usage (starting with '$') and will in
   * that case try to resolve the parameter by finding it in 'params'. Otherwise
   * it will just return the unmodified string. This version doesn't
   * support dynamic parameters.
   */
  virtual const char* ResolveParameter (
  	const celQuestParams& params,
	const char* param) = 0;

  /**
   * This is a convenience function to get a quest parameter block during
   * creation of rewards, triggers, and sequence operations. This routine
   * knows how to recognize parameter usage (starting with '$' or '@') and will in
   * that case try to resolve the parameter by finding it in 'params'.
   * \param params is the quest parameters.
   * \param paramspec is the parameter specifications and unparsed values.
   * \param quest_parameters is an array that should have the same length
   * as the 'paramspec' array. It will be filled with the parameters.
   */
  virtual csPtr<celVariableParameterBlock> GetParameterBlock (
  	const celQuestParams& params,
	const csArray<celParSpec>& parameters,
	csRefArray<iQuestParameter>& quest_parameters) = 0;

  /**
   * Fill in the dynamic parameters in a parameter block.
   * \param params is the parameter block given to the reward.
   * \param msg_params is the resolved parameter block as returned by
   * GetParameterBlock().
   * \param parameters is the parameter specifications and unparsed values.
   * \param quest_parameters is an array with quest parameters.
   */
  virtual void FillParameterBlock (
        iCelParameterBlock* params,
	celVariableParameterBlock* act_params,
	const csArray<celParSpec>& parameters,
	const csRefArray<iQuestParameter>& quest_parameters) = 0;

  /**
   * Load a bunch of quest factories.
   * \param node is a node containing \<quest\> children.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;

  /**
   * Convenience method to add a 'newstate' reward factory
   * to a response factory.
   */
  virtual iQuestRewardFactory* AddNewStateReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* state_par) = 0;

  /**
   * Convenience method to add a 'debugprint' reward factory
   * to a response factory.
   */
  virtual iQuestRewardFactory* AddDebugPrintReward (
  	iQuestTriggerResponseFactory* response,
  	const char* msg_par) = 0;
  	
  virtual iRewardFactory* AddDebugPrintReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* msg) = 0;

  /**
   * Convenience method to add an 'inventory' reward factory
   * to a response factory.
   */
  virtual iQuestRewardFactory* AddInventoryReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par) = 0;
  virtual iRewardFactory* AddInventoryReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par) = 0;

  /**
   * Convenience method to add an 'sequence' reward factory
   * to a response factory.
   */
  virtual iQuestRewardFactory* AddSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par,
	const char* delay_par) = 0;
  virtual iRewardFactory* AddSequenceReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par,
	const char* delay_par) = 0;


  /**
   * Convenience method to add an 'cssequence' reward factory
   * to a response factory.
   */
  virtual iQuestRewardFactory* AddCsSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* sequence_par, const char* delay_par) = 0;
  virtual iRewardFactory* AddCsSequenceReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* sequence_par, const char* delay_par) = 0;

  /**
   * Convenience method to add an 'sequencefinish' reward factory
   * to a response factory.
   */
  virtual iQuestRewardFactory* AddSequenceFinishReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par) = 0;
  virtual iRewardFactory* AddSequenceFinishReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par) = 0;

  /**
   * Convenience method to add a 'changeproperty' reward factory
   * to a response factory. You need to specify exactly which value
   * should be modified after calling this.
   */
  virtual iChangePropertyQuestRewardFactory* AddChangePropertyReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par) = 0;
  virtual iChangePropertyRewardFactory* AddChangePropertyReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par) = 0;

  /**
   * Convenience method to add a 'createentity' reward factory
   * to a response factory. You need to specify exactly which template
   * to create a copy of, the name of the created entity and any other 
   * parameters required by the template.
   */
  virtual iQuestRewardFactory* AddCreateEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* template_par,
	const char* name_par,
    const celEntityTemplateParams &tpl_params) = 0;
  virtual iRewardFactory* AddCreateEntityReward_NEW (
  	iQuestTriggerResponseFactory* response,
	const char* template_par,
	const char* name_par,
    const celEntityTemplateParams &tpl_params) = 0;

    /**
   * Convenience method to add a 'destroyentity' reward factory
   * to a response factory. You need to specify exactly which entity
   * to destroy
   */
  virtual iQuestRewardFactory* AddDestroyEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par) = 0;
  virtual iRewardFactory* AddDestroyEntityReward_NEW (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par) = 0;

  /**
   * Convenience method to set a 'timeout' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetTimeoutTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* timeout_par) = 0;
  virtual iTriggerFactory* SetTimeoutTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* timeout_par) = 0;

  /**
   * Convenience method to set an 'entersector' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par) = 0;
  virtual iTriggerFactory* SetEnterSectorTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par) = 0;

  /**
   * Convenience method to set a 'meshentersector' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetMeshEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par) = 0;
  virtual iTriggerFactory* SetMeshEnterSectorTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par) = 0;

  /**
   * Convenience method to set a 'sequencefinish' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetSequenceFinishTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par) = 0;
  virtual iTriggerFactory* SetSequenceFinishTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par) = 0;

  /**
   * Convenience method to set a 'propertychange' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetPropertyChangeTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par,
	const char* value_par) = 0;
  virtual iTriggerFactory* SetPropertyChangeTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par,
	const char* value_par) = 0;

  /**
   * Convenience method to set a 'trigger' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetTriggerTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, bool do_leave = false) = 0;
  virtual iTriggerFactory* SetTriggerTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, bool do_leave = false) = 0;

  /**
   * Convenience method to set a 'watch' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetWatchTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* target_entity_par,
	const char* checktime_par,
	const char* radius_par) = 0;
  virtual iTriggerFactory* SetWatchTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* target_entity_par,
	const char* checktime_par,
	const char* radius_par) = 0;

  /**
   * Convenience method to set a 'operation' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetOperationTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* operation_par, 
	csRefArray<iQuestTriggerFactory> &trigger_factories) = 0;
  virtual iTriggerFactory* SetOperationTrigger_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* operation_par, 
	csRefArray<iTriggerFactory> &trigger_factories) = 0;
};

//-------------------------------------------------------------------------
// Specific trigger implementations.
//-------------------------------------------------------------------------

/**
 * This interface is implemented by the trigger that fires
 * when a certain time has expired. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.questtrigger.timeout'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>timeout</em>: timeout value.
 */
struct iTimeoutQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iTimeoutQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the timeout on which this trigger will fire.
   * \param timeout_par is the timeout value (in ms) or a parameter (starts
   * with '$').
   */
  virtual void SetTimeoutParameter (const char* timeout_par) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a certain property passes a test on some value. Test can be any of
 * the operations below.
 * You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is
 * 'cel.questtrigger.propertychange'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pcproperties property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>property</em>: the name of the property.
 * - <em>value</em>: the value on which this trigger will fire. If this
 *   value is not given then the trigger will fire whenever the value
 * - <em>operation</em>: the value on which this trigger will test. If this
 *   value is not given then the trigger will fire on equality.
 * - <em>onchange</em>: if true the trigger will fire only when the condition
 *   becomes true (so it has to be initially false, and become true after
 *   that), otherwise the trigger will fire whenever the condition is met
 *   (even initially).
 *
 * Valid operations:
 * - <em>eq</em>: Equals (==).
 * - <em>lt</em>: Lower than (<).
 * - <em>gt</em>: Greater than (>).
 * - <em>ne</em>: Not equal (!=).
 * - <em>le</em>: Lower or equal (<=).
 * - <em>ge</em>: Greater or equal (>=).
 * Note string and bool properties can only be tested with "ne" or "eq", as
 * greater or lesser doesn't really make sense for them.
 */
struct iPropertyChangeQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iPropertyChangeQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcproperties property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the property on which this trigger will fire.
   * \param prop is the name of the property or a parameter (starts
   * with '$').
   */
  virtual void SetPropertyParameter (const char* prop) = 0;

  /**
   * Set the value of the property on which this trigger will fire. If
   * this value is not given the the trigger will fire whenever the value
   * changes.
   * \param value is the value or a parameter (starts with '$').
   */
  virtual void SetValueParameter (const char* value) = 0;

  /**
   * Set the operation this trigger will test with. If operation
   * is not set equality will be checked.
   * \param op one of: eq, lt, gt, ne, le, ge. See above for more details.
   */
  virtual void SetOperationParameter (const char* op) = 0;

  /**
   * Set whether the trigger will fire only on an actual change (so the
   * condition has to become true), or whenever the condition is met (even
   * initially).
   * \param on_change true for reporting only on change.
   */
  virtual void SetOnChangeOnly (bool on_change) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a certain mesh is selected. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.questtrigger.meshselect'.
 *
 * This trigger sends out 'entity' parameter to the reward (containing the
 * name of the entity that was selected).
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pcmeshselect property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 */
struct iMeshSelectQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iMeshSelectQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcmeshselect property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a certain inventory gets some entity. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.questtrigger.inventory'.
 *
 * This trigger sends out 'child' parameter to the reward (containing the
 * name of the entity that was added or removed).
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pcinventory property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>child_entity</em>: the name of the entity to watch for. If this
 *   is not given then you will get triggere whenever something is
 *   added or removed from the inventory.
 */
struct iInventoryQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iInventoryQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcinventory property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the entity to watch for. If this
   * is not given then you will get triggered whenever something is
   * added or removed from the inventory.
   * \param child_entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetChildEntityParameter (const char* child_entity) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a certain sector is entered. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.questtrigger.entersector'
 * for the version that listens to camera changes and
 * 'cel.questtrigger.meshentersector' for the version that listens to mesh
 * changes.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pccamera or pcmesh property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>sector</em>: the name of the sector. As soon as the camera
 *   or mesh enters that sector this trigger will fire.
 */
struct iEnterSectorQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iEnterSectorQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pccamera or pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the sector on which this trigger will fire
   * as soon as the camera enters that sector.
   * \param sector is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetSectorParameter (const char* sector) = 0;
};

/**
 * This interface is implemented by the operation trigger, that allows
 * to combine several triggers using a logical operation.
 * You can query this interface from the trigger factory if you want to
 * manually control this factory as opposed to loading its definition from an
 * XML document.
 *
 * The predefined name of this trigger type is 'cel.questtrigger.operation'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>operation</em>: the logical operation to perform. Can be one of
 *   'or', 'and', or 'xor', see below for a more detailed description.
 *
 * Fireon node can also hold any number of child 'trigger' nodes, which are
 * defined just as usual.
 *
 * Operations:
 * - <em>and</em>: Trigger will fire when all the child triggers are true.
 * - <em>or</em>: Trigger will fire when at least one of the child triggers
 *   is true.
 * - <em>xor</em>: Trigger will fire when one and only one of the child 
 *   triggers is true.
 */
struct iOperationQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iOperationQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the operation this trigger will use to combine child triggers.
   * \param operation is the name of the operation or a parameter (starts
   * with '$').
   * Operation must be one of 'or', 'and', or 'xor'.
   */
  virtual void SetOperationParameter (const char* operation) = 0;
  /**
   * Return the trigger factory list so that user can add new ones, remove
   * them or clear the list.
   */
  virtual csRefArray<iQuestTriggerFactory> &GetTriggerFactories () = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a certain sequence finishes. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is
 * 'cel.questtrigger.sequencefinish'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pcquest property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>sequence</em>: the name of the sequence on which to listen.
 */
struct iSequenceFinishQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iSequenceFinishQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcquest property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the sequence.
   * \param sequence is the name of the sequence or a parameter (starts
   * with '$').
   */
  virtual void SetSequenceParameter (const char* sequence) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a pctrigger fires. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.questtrigger.trigger'.
 *
 * This trigger sends out 'entity' parameter to the reward (containing the
 * name of the entity caused the trigger to fire).
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pctrigger property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>leave</em>: if present the trigger will register on 'leaves' instead
 *   of 'enters'.
 */
struct iTriggerQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iTriggerQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pccamera or pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * If this function is called then the trigger will fire on 'leaves'
   * instead of 'enters'.
   */
  virtual void EnableLeave () = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a message is sent. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.questtrigger.message'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity from which we want to listen
 *   to messages.
 * - <em>mask</em>: the message mask to listen too.
 */
struct iMessageQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iMessageQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity from which we want to listen to messages.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the message mask that we want to receive messages
   * for.
   * \param mask is the message mask or a parameter (starts
   * with '$').
   */
  virtual void SetMaskParameter (const char* mask) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a mesh becomes visible. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.questtrigger.watch'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the source entity that is watching
 *   out for other entity. This should contain a pcmesh to get a position
 *   from.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>target</em>: the name of the target entity to watch. This
 *   should contain a pcmesh to get a position from.
 * - <em>target_tag</em>: optional tag used to find the right property
 *   class from the target entity.
 * - <em>checktime</em>: optional tag to specify the check interval.
 * - <em>radius</em>: optional tag to specify the maximum radius.
 * - <em>offset</em>: optional offset vector.
 *   This node has 'x', 'y, and 'z' attributes. Each of these attributes
 *   can be a parameter.
 */
struct iWatchQuestTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iWatchQuestTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the source entity containing the pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the target entity containing the pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetTargetEntityParameter (const char* entity,
      const char* tag = 0) = 0;

  /**
   * Set the interval (in milliseconds) after which this trigger
   * will check visibility again. By default this is equal to 1000
   * which means there will be a check every second.
   * \param time is the interval in milliseconds or a parameter (starts
   * with '$').
   */
  virtual void SetChecktimeParameter (const char* time) = 0;

  /**
   * Set the maximum radius for visibility checking.
   * By default this is equal to infinity.
   * \param radius is the maximum radius or a parameter (starts
   * with '$').
   */
  virtual void SetRadiusParameter (const char* radius) = 0;

  /**
   * Optional offset parameter. This offset will be added to the
   * position of the start and target locations.
   * \param offsetx is the x offset or a parameter (starts with '$').
   * \param offsety is the y offset or a parameter (starts with '$').
   * \param offsetz is the z offset or a parameter (starts with '$').
   */
  virtual void SetOffsetParameter (const char* offsetx,
      const char* offsety, const char* offsetz) = 0;
};

//-------------------------------------------------------------------------
// Specific reward implementations.
//-------------------------------------------------------------------------

/**
 * This interface is implemented by the reward that prints
 * debug messages on standard output. You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this reward type is 'cel.questreward.debugprint'.
 *
 * In XML, factories recognize the following attributes on the 'reward' node:
 * - <em>message</em>: the message to print.
 */
struct iDebugPrintQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iDebugPrintQuestRewardFactory, 0, 0, 1);

  /**
   * Set the message parameter to print (either a message string
   * or a parameter if it starts with '$').
   */
  virtual void SetMessageParameter (const char* msg) = 0;
};

/**
 * This interface is implemented by the reward that switches
 * to another state. You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this reward type is 'cel.questreward.newstate'.
 *
 * In XML, factories recognize the following attributes on the 'reward' node:
 * - <em>state</em>: the new state.
 * - <em>entity</em>: the name of the entity containing the
 *   pcquest property class.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 */
struct iNewStateQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iNewStateQuestRewardFactory, 0, 0, 1);

  /**
   * Set the state to go to (either a state string
   * or a parameter if it starts with '$').
   * \param state is the name of the state or a parameter (starts
   * with '$').
   */
  virtual void SetStateParameter (const char* state) = 0;

  /**
   * Set the name of the entity containing the pcquest property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the tag of the property class this reward will apply to.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetTagParameter (const char* tag_par) = 0;

  /**
   * Set the name of the entity class containing the property
   * class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;
};

/**
 * This interface is implemented by the reward that changes the value
 * of a property (either on a property from pcproperties or a generic
 * property on any property class). You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this reward type is 'cel.questreward.changeproperty'.
 *
 * In XML, factories recognize the following attributes on the 'reward' node:
 * - <em>entity</em>: the name of the entity containing the
 *   pcproperties property class.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 * - <em>pc</em>: the name of the property class. If this is not
 *   given then pcproperties is used.
 * - <em>tag</em>: used together with 'pc' to specify an optional tag.
 * - <em>property</em>: the name of the property.
 * - <em>string</em>: the new string value of the property.
 * - <em>long</em>: the new long value of the property.
 * - <em>float</em>: the new float value of the property.
 * - <em>bool</em>: the new bool value of the property.
 * - <em>diff</em>: this long or float value will be added to the property.
 * - <em>toggle</em>: toggle the boolean or long value.
 */
struct iChangePropertyQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iChangePropertyQuestRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcproperties property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity class containing the property
   * class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the name of the property class and tag. If this is not
   * given (or property class name is 0) then pcproperties will
   * be used.
   * \param pc is the name of the property class or a parameter (starts
   * with '$').
   * \param tag is the name of the tag or a parameter (starts
   * with '$').
   */
  virtual void SetPCParameter (const char* pc, const char* tag) = 0;

  /**
   * Set the name of the property.
   * \param prop is the name of the property or a parameter (starts
   * with '$').
   */
  virtual void SetPropertyParameter (const char* prop) = 0;

  /**
   * Set the string value.
   * \param pstring is the string or a parameter (starts with '$').
   */
  virtual void SetStringParameter (const char* pstring) = 0;

  /**
   * Set the long value.
   * \param plong is the long or a parameter (starts with '$').
   */
  virtual void SetLongParameter (const char* plong) = 0;

  /**
   * Set the float value.
   * \param pfloat is the float or a parameter (starts with '$').
   */
  virtual void SetFloatParameter (const char* pfloat) = 0;

  /**
   * Set the boolean value.
   * \param pbool is the bool or a parameter (starts with '$').
   */
  virtual void SetBoolParameter (const char* pbool) = 0;

  /**
   * Set the diff.
   * \param pdiff is the long/float or a parameter (starts with '$').
   */
  virtual void SetDiffParameter (const char* pdiff) = 0;

  /**
   * Set the toggle.
   */
  virtual void SetToggle () = 0;
};

/**
 * This interface is implemented by the reward that manipulates the inventory.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.questreward.inventory'.
 *
 * In XML, factories recognize the following attributes on the 'reward' node:
 * - <em>entity</em>: the name of the entity containing the
 *   pcinventory property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>child_entity</em>: the name of the entity that will
 *   be added to or removed from the inventory.
 * - <em>child_entity_tag</em>: optional tag used to find the right
 *   property class from the entity. This will be used to find the
 *   pcmesh for hiding the mesh.
 */
struct iInventoryQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iInventoryQuestRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcinventory property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the entity that will be put in or out the inventory.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetChildEntityParameter (const char* entity,
  	const char* tag = 0) = 0;
};

/**
 * This interface is implemented by the reward that fires a Crystal
 * Space sequence.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.questreward.cssequence'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>sequence</em>: the name of the Crystal Space sequence.
 * - <em>delay</em>: delay before we start the sequence. Default is 0.
 */
struct iCsSequenceQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iCsSequenceQuestRewardFactory, 0, 0, 1);

  /**
   * Set the name of the sequence.
   * \param sequence is the name of the sequence or a parameter (starts
   * with '$').
   */
  virtual void SetSequenceParameter (const char* sequence) = 0;

  /**
   * Set the delay.
   * \param delay is delay or a parameter (starts with '$').
   */
  virtual void SetDelayParameter (const char* delay) = 0;
};

/**
 * This interface is implemented by the reward that fires a sequence.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.questreward.sequence'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the
 *   pcquest property class.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>sequence</em>: the name of the sequence.
 * - <em>delay</em>: delay before we start the sequence. Default is 0.
 */
struct iSequenceQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iSequenceQuestRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcquest property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the tag of the property class this reward will apply to.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetTagParameter (const char* tag_par) = 0;

  /**
   * Set the name of the entity class containing the property
   * class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the name of the sequence.
   * \param sequence is the name of the sequence or a parameter (starts
   * with '$').
   */
  virtual void SetSequenceParameter (const char* sequence) = 0;

  /**
   * Set the delay.
   * \param delay is delay or a parameter (starts with '$').
   */
  virtual void SetDelayParameter (const char* delay) = 0;
};

/**
 * This interface is implemented by the reward that finishes a sequence.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.questreward.sequencefinish'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the
 *   pcquest property class.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>sequence</em>: the name of the sequence.
 */
struct iSequenceFinishQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iSequenceFinishQuestRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcquest property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the tag of the property class this reward will apply to.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetTagParameter (const char* tag_par) = 0;

  /**
   * Set the name of the entity class containing the property
   * class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the name of the sequence.
   * \param sequence is the name of the sequence or a parameter (starts
   * with '$').
   */
  virtual void SetSequenceParameter (const char* sequence) = 0;
};

/**
 * This interface is implemented by the reward that sends a message
 * to some entity (the behaviour will get this message).
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.questreward.message'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity to send the message too.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the
 *   given entity class.
 * - <em>id</em>: id of the message to send.
 */
struct iMessageQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iMessageQuestRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the message id.
   * \param id is the message id or a parameter (starts with '$').
   */
  virtual void SetIDParameter (const char* id) = 0;

  /**
   * Add a parameter to send with the message.
   * \param type is one of CEL_DATA_STRING, CEL_DATA_FLOAT, CEL_DATA_LONG,
   * CEL_DATA_BOOL, or CEL_DATA_VECTOR3.
   * \param id is the id of the parameter (calculated from the string
   * "cel.parameter." appended with the name).
   * \param name is the name of the parameter.
   * \param value is the value string or a parameter for it (starts with '$').
   */
  virtual void AddParameter (celDataType type, csStringID id,
      const char* name, const char* value) = 0;
};

/**
 * This interface is implemented by the reward that sends an action
 * to some entity or entity class property class with an optional tag.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.questreward.action'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity to send the action to.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the 
 *   given entity class.
 * - <em>id</em>: name of the action to activate.
 * - <em>pc</em>: the name of the property class to send the action to.
 * - <em>tag</em>: the tag of the property class to send the action to.
 */
struct iActionQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iActionQuestRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the action name (without the cel.action part).
   * \param id is the action name or a parameter (starts with '$').
   */
  virtual void SetIDParameter (const char* id) = 0;

  /**
   * Set the name of the property class.
   * \param propertyclass is the name of the propertyclass or a parameter
   * (starts with '$').
   */
  virtual void SetPropertyClassParameter (const char* propertyclass) = 0;

  /**
   * Set the tag for the propertyclass.
   * \param tag is the tag for the propertyclass or a parameter (starts
   * with '$').
   */
  virtual void SetTagParameter (const char* tag) = 0;

  /**
   * Add a parameter to send with the action.
   * \param type is one of CEL_DATA_STRING, CEL_DATA_FLOAT, CEL_DATA_LONG,
   * CEL_DATA_BOOL, or CEL_DATA_VECTOR3.
   * \param id is the id of the parameter (calculated from the string
   * "cel.parameter." appended with the name).
   * \param name is the name of the parameter.
   * \param value is the value string or a parameter for it (starts with '$').
   */
  virtual void AddParameter (celDataType type, csStringID id,
      const char* name, const char* value) = 0;
};

/**
 * This interface is implemented by the reward that destroys an entity.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.questreward.destroyentity'.
 *
 * In XML, factories recognize the following attribute on the 'op' node:
 * - <em>entity</em>: the name of the entity to send the message too.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 */
struct iDestroyEntityQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iDestroyEntityQuestRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;


};

/**
 * This interface is implemented by the createentity reward, which can create
 * entities from entity templates.
 *
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.questreward.createentity'.
 *
 * In XML, factories recognize the following attribute on the 'op' node:
 * - <em>template</em>: the name of the template that will be used to create
 *   the entity from. It can be a parameter if starting with '$'.
 * - <em>name</em>: optional name for the entity that will be created. It
 *   can be a parameter if starting with '$'.
 * Also, the following subnodes are supported:
 * - <em>par</em>: a parameter for the template. It needs 'name' and 'value'
 *   attributes. The value can be a parameter if starting with '$'. As many
 *   par nodes as needed can be used.
 */
struct iCreateEntityQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iCreateEntityQuestRewardFactory, 0, 0, 1);

  /**
   * Set the name of the template that will be used to create a new entity
   * from.
   * \param entity_tpl is the name of the entity template or a parameter 
   * (starts with '$').
   */
  virtual void SetEntityTemplateParameter (const char* entity_tpl) = 0;

  /**
   * Set the name of the entity that will be created.
   * \param name is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetNameParameter (const char* name) = 0;

  /**
   * Add a parameter for the template.
   * \param name is the name for the parameter.
   * \param value is the value for the parameter or a quest manager parameter 
   * (starts with '$').
   */
  virtual void AddParameter (const char* name, const char* value) = 0;
};

//-------------------------------------------------------------------------
// Specific sequence operation implementations.
//-------------------------------------------------------------------------

/**
 * This interface is implemented by the seqop that prints
 * debug messages on standard output. You can query this interface
 * from the seqop factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this seqop type is 'cel.questseqop.debugprint'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>message</em>: the message to print.
 */
struct iDebugPrintQuestSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iDebugPrintQuestSeqOpFactory, 0, 0, 1);

  /**
   * Set the message parameter to print (either a message string
   * or a parameter if it starts with '$').
   */
  virtual void SetMessageParameter (const char* msg) = 0;
};

/**
 * This interface is implemented by the seqop that transforms meshes.
 * You can query this interface from the seqop factory if
 * you want to manually control this factory as opposed to loading
 * its definition from an XML document.
 *
 * The predefined name of this seqop type is 'cel.questseqop.transform'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the pcmesh
 *   property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>v</em>: optional movement vector.
 *   This node has 'x', 'y, and 'z' attributes. Each of these attributes
 *   can be a parameter.
 * - <em>rotx</em>: optional rotation along x axis. This node has
 *   an 'angle' parameter in radians. Angle can be a parameter.
 * - <em>roty</em>: optional rotation along y axis. This node has
 *   an 'angle' parameter in radians. Angle can be a parameter.
 * - <em>rotz</em>: optional rotation along z axis. This node has
 *   an 'angle' parameter in radians. Angle can be a parameter.
 */
struct iTransformQuestSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iTransformQuestSeqOpFactory, 0, 0, 1);

  /**
   * Set the entity containing the pcmesh (either entity name
   * or a parameter if it starts with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the relative movement vector parameter.
   */
  virtual void SetVectorParameter (const char* vectorx, const char* vectory,
  	const char* vectorz) = 0;

  /**
   * Set the relative rotation parameter.
   * \param rot_axis is 0, 1, or 2 for x, y, or z axis.
   * \param rot_angle the amount of rotation.
   */
  virtual void SetRotationParameter (int rot_axis, const char* rot_angle) = 0;
};

/**
 * This interface is implemented by the seqop that moves meshes along
 * a path. You can query this interface from the seqop factory if
 * you want to manually control this factory as opposed to loading
 * its definition from an XML document.
 *
 * The predefined name of this seqop type is 'cel.questseqop.movepath'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the pcmesh
 *   property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - several <em>pathnode</em> tags: one for every node with the following
 *   attributes: sector, node, and time. All of those can be a parameter.
 *   The 'node' refers to a node object in the given sector from which
 *   the position, up ('yvector'), and forward ('zvector') will be used.
 *   'time' should be an increasing time value.
 */
struct iMovePathQuestSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iMovePathQuestSeqOpFactory, 0, 0, 1);

  /**
   * Set the entity containing the pcmesh (either entity name
   * or a parameter if it starts with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Add a node.
   * \param sectorname is the name of the sector in which the node can
   * be found. Warning! In the current implementation the sector should
   * be the same for all nodes.
   * \param node is the name of the node (position, yvector, and zvector
   * are used).
   * \param time is an increasing time value.
   */
  virtual void AddPathNode (const char* sectorname, const char* node,
  	const char* time) = 0;
};

/**
 * This interface is implemented by the seqop that animates light colors.
 * You can query this interface from the seqop factory if
 * you want to manually control this factory as opposed to loading
 * its definition from an XML document.
 *
 * The predefined name of this seqop type is 'cel.questseqop.light'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the pclight
 *   property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>relcolor</em>: relative color animation vector.
 *   This node has 'red', 'green, and 'blue' attributes. Each of these
 *   attributes can be a parameter.
 * - <em>abscolor</em>: absolute color.
 *   This node has 'red', 'green, and 'blue' attributes. Each of these
 *   attributes can be a parameter.
 */
struct iLightQuestSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iLightQuestSeqOpFactory, 0, 0, 1);

  /**
   * Set the entity containing the pclight (either entity name
   * or a parameter if it starts with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the relative color animation vector.
   */
  virtual void SetRelColorParameter (const char* red, const char* green,
  	const char* blue) = 0;

  /**
   * Set the absolute color.
   */
  virtual void SetAbsColorParameter (const char* red, const char* green,
  	const char* blue) = 0;
};

/**
 * This interface is implemented by the seqop that transforms property
 * class properties. Assuming it is read and write, and of the appropriate type
 * any property can be controlled using this sequence.
 *
 * You can query this interface from the seqop factory if
 * you want to manually control this factory as opposed to loading
 * its definition from an XML document.
 *
 * The predefined name of this seqop type is 'cel.questseqop.property'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the affected property
 *   class.
 * - <em>pc</em>: name for the property class holding the property.
 * - <em>tag</em>: optional tag used to find the right property class 
 *   from the entity.
 * - <em>relative</em>: whether the sequence is relative (default false).
 * - <em>float</em>: optional end value as a float.
 * - <em>long</em>: optional end value as an integer.
 * And the following sub nodes:
 * - <em>v</em>: optional end value as a vector.
 *   This node has 'x', 'y, and (optionally for vector3) 'z' attributes. 
 *
 * Note you must only provide one of 'v', 'float' and 'long' parameters to the
 * sequence, otherwise the result will be undefined. Also, the parameter
 * must fit the actual property type.
 */
struct iPropertyQuestSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iPropertyQuestSeqOpFactory, 0, 0, 1);

  /**
   * Set the entity containing the property (either entity name
   * or a parameter if it starts with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the property class and tag to search for.
   * \param pc is the property class name or a parameter (starts with a '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetPCParameter (const char* pc, const char* tag = 0) = 0;

  /**
   * Set the property name for this sequence.
   * \param property_name is the property name (like cel.property.gravity).
   * It can also be a parameter if it starts with '$'.
   */
  virtual void SetPropertyParameter (const char* property_name) = 0;

  /**
   * Set the end value for the property as a float.
   * \param pfloat is the value to be set.
   * It can also be a parameter if it starts with '$'.
   */
  virtual void SetFloatParameter (const char* pfloat) = 0;

  /**
   * Set the end value for the property as a long.
   * \param plong is the value to be set.
   * It can also be a parameter if it starts with '$'.
   */
  virtual void SetLongParameter (const char* plong) = 0;

  /**
   * Set the end value for the property as a vector2.
   * \param pvectorx is the x component for the vector.
   * \param pvectory is the y component for the vector.
   * Both pvectorx and pvectory can be parameters if they start with '$'.
   */
  virtual void SetVector2Parameter (const char* vectorx, 
	const char* vectory) = 0;

  /**
   * Set the end value for the property as a vector3.
   * \param pvectorx is the x component for the vector.
   * \param pvectory is the y component for the vector.
   * \param pvectorz is the z component for the vector.
   * Both pvectorx, pvectory and pvectorz can be a parameters if they 
   * start with '$'.
   */
  virtual void SetVector3Parameter (const char* vectorx, const char* vectory,
        const char* vectorz) = 0;

   /**
   * Set whether the sequence will be relative:
   *  (end value = specified value + starting value)
   * or absolute.
   *  (end value = specified value)
   * \param is_relative whether the sequence is relative. can't be a parameter.
   */
  virtual void SetRelative (bool is_relative) = 0;
};



//-------------------------------------------------------------------------

/**
 * Convenience to declare a new reward type class.
 */
#define CEL_DECLARE_REWARDTYPE(name,id)					\
class cel##name##RewardType : public scfImplementation1<		\
		cel##name##RewardType,iQuestRewardType>			\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  csWeakRef<iCelPlLayer> pl;						\
  cel##name##RewardType (iObjectRegistry* object_reg);			\
  virtual ~cel##name##RewardType () { }					\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iQuestRewardFactory> CreateRewardFactory ();		\
};

/**
 * Convenience to implement a new reward type class.
 */
#define CEL_IMPLEMENT_REWARDTYPE(name)					\
cel##name##RewardType::cel##name##RewardType (				\
	iObjectRegistry* object_reg) : scfImplementationType (this)	\
{									\
  cel##name##RewardType::object_reg = object_reg;			\
  pl = csQueryRegistry<iCelPlLayer> (object_reg);			\
}									\
csPtr<iQuestRewardFactory> cel##name##RewardType::CreateRewardFactory ()\
{									\
  cel##name##RewardFactory* fact = new					\
  	cel##name##RewardFactory (this);				\
  return fact;								\
}

/**
 * Convenience to declare a new trigger type class.
 */
#define CEL_DECLARE_TRIGGERTYPE(name,id)				\
class cel##name##TriggerType : public scfImplementation1<		\
		cel##name##TriggerType,iQuestTriggerType>		\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  csWeakRef<iCelPlLayer> pl;						\
  cel##name##TriggerType (iObjectRegistry* object_reg);			\
  virtual ~cel##name##TriggerType () { }				\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iQuestTriggerFactory> CreateTriggerFactory ();		\
};

/**
 * Convenience to implement a new trigger type class.
 */
#define CEL_IMPLEMENT_TRIGGERTYPE(name)					\
cel##name##TriggerType::cel##name##TriggerType (			\
	iObjectRegistry* object_reg) : scfImplementationType (this)	\
{									\
  cel##name##TriggerType::object_reg = object_reg;			\
  pl = csQueryRegistry<iCelPlLayer> (object_reg);			\
}									\
csPtr<iQuestTriggerFactory> cel##name##TriggerType::CreateTriggerFactory () \
{									\
  cel##name##TriggerFactory* fact = new cel##name##TriggerFactory (this); \
  return fact;								\
}

/**
 * Convenience to declare a new sequence operation type class.
 */
#define CEL_DECLARE_SEQOPTYPE(name,id)					\
class cel##name##SeqOpType : public scfImplementation1<			\
	cel##name##SeqOpType,iQuestSeqOpType>				\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  csWeakRef<iCelPlLayer> pl;						\
  cel##name##SeqOpType (iObjectRegistry* object_reg);			\
  virtual ~cel##name##SeqOpType () { }					\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iQuestSeqOpFactory> CreateSeqOpFactory ();		\
};

/**
 * Convenience to implement a new sequence operation type class.
 */
#define CEL_IMPLEMENT_SEQOPTYPE(name)					\
cel##name##SeqOpType::cel##name##SeqOpType (				\
	iObjectRegistry* object_reg) : scfImplementationType (this)	\
{									\
  cel##name##SeqOpType::object_reg = object_reg;			\
  pl = csQueryRegistry<iCelPlLayer> (object_reg);			\
}									\
csPtr<iQuestSeqOpFactory> cel##name##SeqOpType::CreateSeqOpFactory ()	\
{									\
  cel##name##SeqOpFactory* fact = new cel##name##SeqOpFactory (this);	\
  return fact;								\
}

#endif // __CEL_MGR_QUEST__

