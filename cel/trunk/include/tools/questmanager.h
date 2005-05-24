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

#ifndef __CEL_MGR_QUEST__
#define __CEL_MGR_QUEST__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/hash.h"
#include "csgeom/vector3.h"

struct iDocumentNode;
struct iChangePropertyQuestRewardFactory;
struct iCelDataBuffer;

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

typedef csHash<csStrKey,csStrKey> celQuestParams;

//-------------------------------------------------------------------------
// Triggers
//-------------------------------------------------------------------------

struct iQuestTrigger;

SCF_VERSION (iQuestTriggerCallback, 0, 0, 1);

/**
 * A quest trigger will get pointers to call back the quest when
 * the trigger fires through this interface. Application code will
 * typically not implement this interface as it is implemented
 * by the quest manager itself.
 */
struct iQuestTriggerCallback : public iBase
{
  /// Trigger fired.
  virtual void TriggerFired (iQuestTrigger* trigger) = 0;
};

SCF_VERSION (iQuestTrigger, 0, 0, 1);

/**
 * This is a trigger for a quest. When a trigger fires the quest
 * can go to another state and/or activate rewards. The quest manager
 * knows a few standard triggers but you can also create your own
 * triggers here. Triggers are created by trigger factories. To
 * be able to use your own trigger implementations you must also make
 * associated trigger factories which you then register with the quest
 * manager. See iQuestTriggerFactory.
 * <p>
 * Triggers must start in deactivated state. They will be activated by
 * the quest when the 'state' containing this trigger is activated.
 */
struct iQuestTrigger : public iBase
{
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

SCF_VERSION (iQuestTriggerFactory, 0, 0, 1);

/**
 * A factory to create triggers. Trigger factories are created
 * by an iQuestTriggerType instance.
 */
struct iQuestTriggerFactory : public iBase
{
  /**
   * Create a trigger.
   * \param params are the parameters with which this reward is
   * instantiated.
   */
  virtual csPtr<iQuestTrigger> CreateTrigger (const celQuestParams& params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<fireon\> node in a trigger description.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;
};

SCF_VERSION (iQuestTriggerType, 0, 0, 1);

/**
 * The trigger type is responsible for the creation of trigger factories.
 * If you want to define a new type of trigger then you must implement
 * iQuestTriggerType, iQuestTriggerFactory, and iQuestTrigger.
 * You register trigger types with the quest manager so that they can
 * be used by quest factories.
 */
struct iQuestTriggerType : public iBase
{
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

SCF_VERSION (iQuestReward, 0, 0, 1);

/**
 * This is a reward for a quest. The quest manager can issue
 * rewards in response to some trigger. Just like triggers you can
 * implement your own rewards in addition to the standard rewards
 * already implemented in the quest manager. You also need to implement
 * a quest reward factory then (see iQuestRewardFactory).
 */
struct iQuestReward : public iBase
{
  /**
   * Perform this reward.
   */
  virtual void Reward () = 0;
};

SCF_VERSION (iQuestRewardFactory, 0, 0, 1);

/**
 * A factory to create rewards. Reward factories are created
 * by an iQuestRewardType instance.
 */
struct iQuestRewardFactory : public iBase
{
  /**
   * Create a reward.
   * \param params are the parameters with which this reward is
   * instantiated.
   */
  virtual csPtr<iQuestReward> CreateReward (const celQuestParams& params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<reward\> node in a trigger description.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;
};

SCF_VERSION (iQuestRewardType, 0, 0, 1);

/**
 * The reward type is responsible for the creation of reward factories.
 * If you want to define a new type of reward then you must implement
 * iQuestRewardType, iQuestRewardFactory, and iQuestReward.
 * You register reward types with the quest manager so that they can
 * be used by quest factories.
 */
struct iQuestRewardType : public iBase
{
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

SCF_VERSION (iQuestSeqOp, 0, 0, 1);

/**
 * This is a sequence operation in a quest. A sequence operation represents
 * an operation that has a duration or an operation that is a single-shot.
 * You can combine different sequence operations in a sequence and then use
 * a reward to fire of the sequence as result of a trigger.
 */
struct iQuestSeqOp : public iBase
{
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

SCF_VERSION (iQuestSeqOpFactory, 0, 0, 1);

/**
 * A factory to create sequence operators. Factories are created
 * by an iQuestSeqOpType instance.
 */
struct iQuestSeqOpFactory : public iBase
{
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

SCF_VERSION (iQuestSeqOpType, 0, 0, 1);

/**
 * The seqop type is responsible for the creation of seqop factories.
 * If you want to define a new type of sequence operation then you must
 * implement iQuestSeqOpType, iQuestSeqOpFactory, and iQuestSeqOp.
 * You register seqop types with the quest manager so that they can
 * be used by seqop factories.
 */
struct iQuestSeqOpType : public iBase
{
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

SCF_VERSION (iQuestSequenceCallback, 0, 0, 1);

/**
 * This callback is fired when the sequences finished running properly.
 * It is not called it the sequence is aborted!
 */
struct iQuestSequenceCallback : public iBase
{
  /// Sequence finishes.
  virtual void SequenceFinished (iQuestSequence* sequence) = 0;
};

SCF_VERSION (iQuestSequence, 0, 0, 1);

/**
 * A sequence.
 */
struct iQuestSequence : public iBase
{
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

SCF_VERSION (iQuest, 0, 0, 1);

/**
 * A quest instance. This is created (by the quest manager) from a quest
 * factory using the trigger and reward factories.
 */
struct iQuest : public iBase
{
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
};

SCF_VERSION (iQuestTriggerResponseFactory, 0, 0, 1);

/**
 * A trigger with response as used in a quest state.
 * This is basically the representation of one trigger and one
 * or more rewards.
 */
struct iQuestTriggerResponseFactory : public iBase
{
  /**
   * Set the trigger factory that is managed by this
   * response factory.
   */
  virtual void SetTriggerFactory (iQuestTriggerFactory* trigger_fact) = 0;

  /**
   * Add a reward factory. A reward of this factory will be obtained
   * when the trigger fires.
   */
  virtual void AddRewardFactory (iQuestRewardFactory* reward_fact) = 0;
};

SCF_VERSION (iQuestStateFactory, 0, 0, 1);

/**
 * A representation of a quest state in a quest factory.
 * A state is basically a collection of trigger response factories.
 */
struct iQuestStateFactory : public iBase
{
  /**
   * Get the name of this state.
   */
  virtual const char* GetName () const = 0;

  /**
   * Create a new trigger response.
   */
  virtual iQuestTriggerResponseFactory* CreateTriggerResponseFactory () = 0;
};

SCF_VERSION (iQuestSequenceFactory, 0, 0, 1);

/**
 * A representation of a sequence factory.
 * A sequence factory is basically a sequence of sequence operation factories.
 */
struct iQuestSequenceFactory : public iBase
{
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
   * \param start is the time (relative to the beginning of the
   * execution of the entire sequence) when the operation should start.
   * \param end is the time when it should end. If start is equal to end
   * then this is a single-shot event.
   */
  virtual void AddSeqOpFactory (iQuestSeqOpFactory* seqopfact,
  	csTicks start, csTicks end) = 0;

  /**
   * Set the total time for this sequence. This is used if you manually
   * setup the sequence as opposed to calling Load(). Note that
   * AddSeqOpFactory() will automatically bump up the total_time to the
   * maximum of all 'end' parameters.
   */
  virtual void SetTotalTime (csTicks total_time) = 0;

  /**
   * Get the total time for this sequence.
   */
  virtual csTicks GetTotalTime () const = 0;
};

SCF_VERSION (iQuestFactory, 0, 0, 1);

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
struct iQuestFactory : public iBase
{
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
  virtual csPtr<iQuest> CreateQuest (const celQuestParams& params) = 0;

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

  /**
   * Create a new sequence factory in this factory.
   * Return 0 on failure (name already exists).
   */
  virtual iQuestSequenceFactory* CreateSequence (const char* name) = 0;
};

//-------------------------------------------------------------------------
// The Quest Manager
//-------------------------------------------------------------------------

SCF_VERSION (iQuestManager, 0, 0, 1);

/**
 * This is the quest manager. This manager can be used to handle quests.
 * A quest is basically a state machine which can advance from state to
 * state when a trigger activates. It can also hand out 'rewards'.
 */
struct iQuestManager : public iBase
{
  /**
   * Register a quest trigger type. Quest triggers can be used
   * by quests to decide when to go to another state or when
   * to activate a reward. Returns false on failure (trigger
   * type with that name already exists).
   * <p>
   * The following predefined trigger types are automatically
   * registered in the quest manager:
   * <ul>
   * <li>cel.questtrigger.entersector: triggers when a camera (from
   *     entity) enters a sector. See iEnterSectorQuestTriggerFactory.
   * <li>cel.questtrigger.meshentersector: triggers when a mesh (from
   *     entity) enters a sector. See iEnterSectorQuestTriggerFactory.
   * <li>cel.questtrigger.timeout: triggers after a specified time.
   *     See iTimeoutQuestTriggerFactory.
   * <li>cel.questtrigger.propertychange: triggers when a property changes.
   *     See iPropertyChangeQuestTriggerFactory.
   * <li>cel.questtrigger.sequencefinish: triggers when a sequence finishes.
   *     See iSequenceFinishQuestTriggerFactory.
   * <li>cel.questtrigger.trigger: triggers when a pctrigger files.
   *     See iTriggerQuestTriggerFactory.
   * <li>cel.questtrigger.inventory: triggers when an object enters inventory.
   *     See iInventoryQuestTriggerFactory.
   * <li>cel.questtrigger.meshselect: triggers when a mesh is selected.
   *     See iMeshSelectQuestTriggerFactory.
   * </ul>
   */
  virtual bool RegisterTriggerType (iQuestTriggerType* trigger) = 0;

  /**
   * Get a trigger type from the quest manager.
   * Returns 0 if no such trigger type exists.
   */
  virtual iQuestTriggerType* GetTriggerType (const char* name) = 0;

  /**
   * Register a quest reward type. Quest rewards can be used
   * by quests to give out some kind of reward to the game.
   * Returns false on failure (reward type with that name
   * already exists).
   * <p>
   * The following predefined reward types are automatically
   * registered in the quest manager:
   * <ul>
   * <li>cel.questreward.debugprint: print a debug message on stdout.
   *     See iDebugPrintQuestRewardFactory.
   * <li>cel.questreward.newstate: switch to a new state.
   *     See iNewStateQuestRewardFactory.
   * <li>cel.questreward.changeproperty: change a property.
   *     See iChangePropertyQuestRewardFactory.
   * <li>cel.questreward.inventory: manipulate inventory.
   *     See iInventoryQuestRewardFactory.
   * <li>cel.questreward.sequence: fire sequence.
   *     See iSequenceQuestRewardFactory.
   * <li>cel.questreward.sequencefinish: finish sequence.
   *     See iSequenceFinishQuestRewardFactory.
   * </ul>
   */
  virtual bool RegisterRewardType (iQuestRewardType* trigger) = 0;

  /**
   * Get a reward type from the quest manager.
   * Returns 0 if no such reward type exists.
   */
  virtual iQuestRewardType* GetRewardType (const char* name) = 0;

  /**
   * Register a seqop reward type. Seqop rewards can be used
   * by quest sequences to define operations in the sequence.
   * Returns false on failure (swqop type with that name
   * already exists).
   * <p>
   * The following predefined sequence operation types are automatically
   * registered in the quest manager:
   * <ul>
   * <li>cel.questseqop.debugprint: print a debug message on stdout.
   *     See iDebugPrintQuestSeqOpFactory.
   * <li>cel.questseqop.transform: transform a mesh.
   *     See iTransformQuestSeqOpFactory.
   * </ul>
   */
  virtual bool RegisterSeqOpType (iQuestSeqOpType* seqop) = 0;

  /**
   * Get a seqop type from the quest manager.
   * Returns 0 if no such seqop type exists.
   */
  virtual iQuestSeqOpType* GetSeqOpType (const char* name) = 0;

  /**
   * Get a quest factory by name.
   */
  virtual iQuestFactory* GetQuestFactory (const char* name) = 0;

  /**
   * Create an empty quest factory.
   * Returns 0 on failure (quest factory with that name
   * already exists).
   */
  virtual iQuestFactory* CreateQuestFactory (const char* name) = 0;

  /**
   * This is a convenience function to resolve a quest parameter during
   * creation of rewards, triggers, and sequence operations. This routine
   * knows how to recognize parameter usage (starting with '$') and will in
   * that case try to resolve the parameter by finding it in 'params'. Otherwise
   * it will just return the unmodified string.
   */
  virtual const char* ResolveParameter (
  	const celQuestParams& params,
	const char* param) = 0;

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

  /**
   * Convenience method to add an 'inventory' reward factory
   * to a response factory.
   */
  virtual iQuestRewardFactory* AddInventoryReward (
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

  /**
   * Convenience method to add an 'sequencefinish' reward factory
   * to a response factory.
   */
  virtual iQuestRewardFactory* AddSequenceFinishReward (
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

  /**
   * Convenience method to set a 'timeout' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetTimeoutTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* timeout_par) = 0;

  /**
   * Convenience method to set an 'entersector' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par) = 0;

  /**
   * Convenience method to set a 'meshentersector' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetMeshEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par) = 0;

  /**
   * Convenience method to set a 'sequencefinish' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetSequenceFinishTrigger (
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

  /**
   * Convenience method to set a 'trigger' trigger factory
   * to a response factory.
   */
  virtual iQuestTriggerFactory* SetTriggerTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, bool do_leave = false) = 0;
};

//-------------------------------------------------------------------------
// Specific trigger implementations.
//-------------------------------------------------------------------------

SCF_VERSION (iTimeoutQuestTriggerFactory, 0, 0, 1);

/**
 * This interface is implemented by the trigger that fires
 * when a certain time has expired. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this trigger type is 'cel.questtrigger.timeout'.
 * <p>
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * <ul>
 * <li><em>timeout</em>: timeout value.
 * </ul>
 */
struct iTimeoutQuestTriggerFactory : public iBase
{
  /**
   * Set the timeout on which this trigger will fire.
   * \param timeout_par is the timeout value (in ms) or a parameter (starts
   * with '$').
   */
  virtual void SetTimeoutParameter (const char* timeout_par) = 0;
};

SCF_VERSION (iPropertyChangeQuestTriggerFactory, 0, 0, 1);

/**
 * This interface is implemented by the trigger that fires
 * when a certain property gets some value. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this trigger type is
 * 'cel.questtrigger.propertychange'.
 * <p>
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity that contains the
 *     pcproperties property class.
 * <li><em>property</em>: the name of the property.
 * <li><em>value</em>: the value on which this trigger will fire.
 * </ul>
 */
struct iPropertyChangeQuestTriggerFactory : public iBase
{
  /**
   * Set the name of the entity containing the pcproperties property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the property on which this trigger will fire.
   * \param prop is the name of the property or a parameter (starts
   * with '$').
   */
  virtual void SetPropertyParameter (const char* prop) = 0;

  /**
   * Set the value of the property on which this trigger will fire.
   * \param value is the varlue or a parameter (starts with '$').
   */
  virtual void SetValueParameter (const char* value) = 0;
};

SCF_VERSION (iMeshSelectQuestTriggerFactory, 0, 0, 1);

/**
 * This interface is implemented by the trigger that fires
 * when a certain mesh is selected. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this trigger type is
 * 'cel.questtrigger.meshselect'.
 * <p>
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity that contains the
 *     pcmeshselect property class.
 * </ul>
 */
struct iMeshSelectQuestTriggerFactory : public iBase
{
  /**
   * Set the name of the entity containing the pcmeshselect property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;
};

SCF_VERSION (iInventoryQuestTriggerFactory, 0, 0, 1);

/**
 * This interface is implemented by the trigger that fires
 * when a certain inventory gets some entity. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this trigger type is
 * 'cel.questtrigger.inventory'.
 * <p>
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity that contains the
 *     pcinventory property class.
 * <li><em>child_entity</em>: the name of the entity to watch for.
 * </ul>
 */
struct iInventoryQuestTriggerFactory : public iBase
{
  /**
   * Set the name of the entity containing the pcinventory property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity to watch for.
   * \param child_entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetChildEntityParameter (const char* child_entity) = 0;
};

SCF_VERSION (iEnterSectorQuestTriggerFactory, 0, 0, 1);

/**
 * This interface is implemented by the trigger that fires
 * when a certain sector is entered. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this trigger type is 'cel.questtrigger.entersector'
 * for the version that listens to camera changes and
 * 'cel.questtrigger.meshentersector' for the version that listens to mesh
 * changes.
 * <p>
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity that contains the
 *     pccamera or pcmesh property class.
 * <li><em>sector</em>: the name of the sector. As soon as the camera
 *     or mesh enters that sector this trigger will fire.
 * </ul>
 */
struct iEnterSectorQuestTriggerFactory : public iBase
{
  /**
   * Set the name of the entity containing the pccamera or pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the sector on which this trigger will fire
   * as soon as the camera enters that sector.
   * \param sector is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetSectorParameter (const char* sector) = 0;
};

SCF_VERSION (iSequenceFinishQuestTriggerFactory, 0, 0, 1);

/**
 * This interface is implemented by the trigger that fires
 * when a certain sequence finishes. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this trigger type is
 * 'cel.questtrigger.sequencefinish'.
 * <p>
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity that contains the
 *     pcquest property class.
 * <li><em>sequence</em>: the name of the sequence on which to listen.
 * </ul>
 */
struct iSequenceFinishQuestTriggerFactory : public iBase
{
  /**
   * Set the name of the entity containing the pcquest property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

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
 * <p>
 * The predefined name of this trigger type is 'cel.questtrigger.trigger'.
 * <p>
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity that contains the
 *     pctrigger property class.
 * <li><em>leave</em>: if present the trigger will register on 'leaves' instead
 *     of 'enters'.
 * </ul>
 */
struct iTriggerQuestTriggerFactory : public iBase
{
  /**
   * Set the name of the entity containing the pccamera or pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * If this function is called then the trigger will fire on 'leaves'
   * instead of 'enters'.
   */
  virtual void EnableLeave () = 0;
};

//-------------------------------------------------------------------------
// Specific reward implementations.
//-------------------------------------------------------------------------

SCF_VERSION (iDebugPrintQuestRewardFactory, 0, 0, 1);

/**
 * This interface is implemented by the reward that prints
 * debug messages on standard output. You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this reward type is 'cel.questreward.debugprint'.
 * <p>
 * In XML, factories recognize the following attributes on the 'reward' node:
 * <ul>
 * <li><em>message</em>: the message to print.
 * </ul>
 */
struct iDebugPrintQuestRewardFactory : public iBase
{
  /**
   * Set the message parameter to print (either a message string
   * or a parameter if it starts with '$').
   */
  virtual void SetMessageParameter (const char* msg) = 0;
};

SCF_VERSION (iNewStateQuestRewardFactory, 0, 0, 1);

/**
 * This interface is implemented by the reward that switches
 * to another state. You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this reward type is 'cel.questreward.newstate'.
 * <p>
 * In XML, factories recognize the following attributes on the 'reward' node:
 * <ul>
 * <li><em>state</em>: the new state.
 * <li><em>entity</em>: the name of the entity containing the
 *     pcquest property class.
 * </ul>
 */
struct iNewStateQuestRewardFactory : public iBase
{
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
   */
  virtual void SetEntityParameter (const char* entity) = 0;
};

SCF_VERSION (iChangePropertyQuestRewardFactory, 0, 0, 1);

/**
 * This interface is implemented by the reward that changes the value
 * of a property (either on a property from pcproperties or a generic
 * property on any property class). You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this reward type is 'cel.questreward.changeproperty'.
 * <p>
 * In XML, factories recognize the following attributes on the 'reward' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity containing the
 *     pcproperties property class.
 * <li><em>pc</em>: the name of the property class. If this is not
 *     given then pcproperties is used.
 * <li><em>tag</em>: used together with 'pc' to specify an optional tag.
 * <li><em>property</em>: the name of the property.
 * <li><em>string</em>: the new string value of the property.
 * <li><em>long</em>: the new long value of the property.
 * <li><em>float</em>: the new float value of the property.
 * <li><em>bool</em>: the new bool value of the property.
 * <li><em>diff</em>: this long or float value will be added to the property.
 * <li><em>toggle</em>: toggle the boolean or long value.
 * </ul>
 */
struct iChangePropertyQuestRewardFactory : public iBase
{
  /**
   * Set the name of the entity containing the pcproperties property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

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

SCF_VERSION (iInventoryQuestRewardFactory, 0, 0, 1);

/**
 * This interface is implemented by the reward that manipulates the inventory.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 * <p>
 * The predefined name of this reward type is 'cel.questreward.inventory'.
 * <p>
 * In XML, factories recognize the following attributes on the 'reward' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity containing the
 *     pcinventory property class.
 * <li><em>child_entity</em>: the name of the entity that will
 *     be added to or removed from the inventory.
 * </ul>
 */
struct iInventoryQuestRewardFactory : public iBase
{
  /**
   * Set the name of the entity containing the pcinventory property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity that will be put in or out the inventory.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetChildEntityParameter (const char* entity) = 0;

};

SCF_VERSION (iSequenceQuestRewardFactory, 0, 0, 1);

/**
 * This interface is implemented by the reward that fires a sequence.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 * <p>
 * The predefined name of this reward type is 'cel.questreward.sequence'.
 * <p>
 * In XML, factories recognize the following attributes on the 'op' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity containing the
 *     pcquest property class.
 * <li><em>sequence</em>: the name of the sequence.
 * <li><em>delay</em>: delay before we start the sequence. Default is 0.
 * </ul>
 */
struct iSequenceQuestRewardFactory : public iBase
{
  /**
   * Set the name of the entity containing the pcquest property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

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

SCF_VERSION (iSequenceFinishQuestRewardFactory, 0, 0, 1);

/**
 * This interface is implemented by the reward that finishes a sequence.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 * <p>
 * The predefined name of this reward type is 'cel.questreward.sequencefinish'.
 * <p>
 * In XML, factories recognize the following attributes on the 'op' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity containing the
 *     pcquest property class.
 * <li><em>sequence</em>: the name of the sequence.
 * </ul>
 */
struct iSequenceFinishQuestRewardFactory : public iBase
{
  /**
   * Set the name of the entity containing the pcquest property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the sequence.
   * \param sequence is the name of the sequence or a parameter (starts
   * with '$').
   */
  virtual void SetSequenceParameter (const char* sequence) = 0;
};

//-------------------------------------------------------------------------
// Specific sequence operation implementations.
//-------------------------------------------------------------------------

SCF_VERSION (iDebugPrintQuestSeqOpFactory, 0, 0, 1);

/**
 * This interface is implemented by the seqop that prints
 * debug messages on standard output. You can query this interface
 * from the seqop factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this seqop type is 'cel.questseqop.debugprint'.
 * <p>
 * In XML, factories recognize the following attributes on the 'op' node:
 * <ul>
 * <li><em>message</em>: the message to print.
 * </ul>
 */
struct iDebugPrintQuestSeqOpFactory : public iBase
{
  /**
   * Set the message parameter to print (either a message string
   * or a parameter if it starts with '$').
   */
  virtual void SetMessageParameter (const char* msg) = 0;
};

SCF_VERSION (iTransformQuestSeqOpFactory, 0, 0, 1);

/**
 * This interface is implemented by the seqop that transforms meshes.
 * You can query this interface from the seqop factory if
 * you want to manually control this factory as opposed to loading
 * its definition from an XML document.
 * <p>
 * The predefined name of this seqop type is 'cel.questseqop.transform'.
 * <p>
 * In XML, factories recognize the following attributes on the 'op' node:
 * <ul>
 * <li><em>entity</em>: the name of the entity containing the pcmesh
 *     property class.
 * <li><em>v</em>: optional movement vector. This node has 'x', 'y, and 'z'
 *     attributes.
 * <li><em>rotx</em>: optional rotation along x axis. This node has
 *     an 'angle' parameter in radians.
 * <li><em>roty</em>: optional rotation along y axis. This node has
 *     an 'angle' parameter in radians.
 * <li><em>rotz</em>: optional rotation along z axis. This node has
 *     an 'angle' parameter in radians.
 * </ul>
 */
struct iTransformQuestSeqOpFactory : public iBase
{
  /**
   * Set the entity containing the pcmesh (either entity name
   * or a parameter if it starts with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the relative movement vector.
   */
  virtual void SetVector (const csVector3& vector) = 0;

  /**
   * Set the relative rotation.
   */
  virtual void SetRotation (int rot_axis, float rot_angle) = 0;
};

//-------------------------------------------------------------------------

/**
 * Convenience to declare a new reward type class.
 */
#define CEL_DECLARE_REWARDTYPE(name,id)					\
class cel##name##RewardType : public iQuestRewardType			\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  cel##name##RewardType (iObjectRegistry* object_reg);			\
  virtual ~cel##name##RewardType ();					\
  SCF_DECLARE_IBASE;							\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iQuestRewardFactory> CreateRewardFactory ();		\
};

/**
 * Convenience to implement a new reward type class.
 */
#define CEL_IMPLEMENT_REWARDTYPE(name)					\
SCF_IMPLEMENT_IBASE (cel##name##RewardType)				\
  SCF_IMPLEMENTS_INTERFACE (iQuestRewardType)				\
SCF_IMPLEMENT_IBASE_END							\
cel##name##RewardType::cel##name##RewardType (				\
	iObjectRegistry* object_reg)					\
{									\
  SCF_CONSTRUCT_IBASE (0);						\
  cel##name##RewardType::object_reg = object_reg;			\
}									\
cel##name##RewardType::~cel##name##RewardType ()			\
{									\
  SCF_DESTRUCT_IBASE ();						\
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
class cel##name##TriggerType : public iQuestTriggerType			\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  cel##name##TriggerType (iObjectRegistry* object_reg);			\
  virtual ~cel##name##TriggerType ();					\
  SCF_DECLARE_IBASE;							\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iQuestTriggerFactory> CreateTriggerFactory ();		\
};

/**
 * Convenience to implement a new trigger type class.
 */
#define CEL_IMPLEMENT_TRIGGERTYPE(name)					\
SCF_IMPLEMENT_IBASE (cel##name##TriggerType)				\
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerType)				\
SCF_IMPLEMENT_IBASE_END							\
cel##name##TriggerType::cel##name##TriggerType (			\
	iObjectRegistry* object_reg)					\
{									\
  SCF_CONSTRUCT_IBASE (0);						\
  cel##name##TriggerType::object_reg = object_reg;			\
}									\
cel##name##TriggerType::~cel##name##TriggerType ()			\
{									\
  SCF_DESTRUCT_IBASE ();						\
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
class cel##name##SeqOpType : public iQuestSeqOpType			\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  cel##name##SeqOpType (iObjectRegistry* object_reg);			\
  virtual ~cel##name##SeqOpType ();					\
  SCF_DECLARE_IBASE;							\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iQuestSeqOpFactory> CreateSeqOpFactory ();		\
};

/**
 * Convenience to implement a new sequence operation type class.
 */
#define CEL_IMPLEMENT_SEQOPTYPE(name)					\
SCF_IMPLEMENT_IBASE (cel##name##SeqOpType)				\
  SCF_IMPLEMENTS_INTERFACE (iQuestSeqOpType)				\
SCF_IMPLEMENT_IBASE_END							\
cel##name##SeqOpType::cel##name##SeqOpType (				\
	iObjectRegistry* object_reg)					\
{									\
  SCF_CONSTRUCT_IBASE (0);						\
  cel##name##SeqOpType::object_reg = object_reg;			\
}									\
cel##name##SeqOpType::~cel##name##SeqOpType ()				\
{									\
  SCF_DESTRUCT_IBASE ();						\
}									\
csPtr<iQuestSeqOpFactory> cel##name##SeqOpType::CreateSeqOpFactory ()	\
{									\
  cel##name##SeqOpFactory* fact = new cel##name##SeqOpFactory (this);	\
  return fact;								\
}

#endif // __CEL_MGR_QUEST__

