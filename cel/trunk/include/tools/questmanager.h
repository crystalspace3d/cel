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
#include "csutil/hashhandlers.h"

struct iDocumentNode;

/*
<quest name="test">
    <state name="notstarted">
    </state>

    <state name="start">
        <trigger type="ininventory">
	    <fireon entity_name="$actor_entity" new_entity_name="computer_part" />
	    <reward type="newstate" state="state2" />
	    <reward type="increasestats" statname="HP" value="3" />
	</trigger>
	<trigger type="objectdestroyed">
	    <fireon entity_name="barrel" />
	    <reward type="newstate" state="finalstate" />
	    <reward type="increasestats" statname="HP" value="10" />
	</trigger>
    </state>

    <state name="state2">
        <trigger type="clickonentity">
	    <fireon entity_name="" />
	</trigger>
        <trigger type="cel.questtrigger.entersector">
	    <fireon entity_name="player" sector_name="room" />
	</trigger>
    </state>

    <start>notstarted</start>
</quest>
*/

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
  virtual csPtr<iQuestTrigger> CreateTrigger (
      const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params) = 0;

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
  virtual csPtr<iQuestReward> CreateReward (
      const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params) = 0;

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
  virtual bool SwitchSate (const char* state) = 0;

  /**
   * Get current state name of this quest.
   */
  virtual const char* GetCurrentState () const = 0;
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
  virtual csPtr<iQuest> CreateQuest (
      const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<quest\> node in a quest.
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
   * </ul>
   */
  virtual bool RegisterRewardType (iQuestRewardType* trigger) = 0;

  /**
   * Get a reward type from the quest manager.
   * Returns 0 if no such reward type exists.
   */
  virtual iQuestRewardType* GetRewardType (const char* name) = 0;

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
   * This is a conveniance function to resolve a quest parameter during
   * creation of the rewards and triggers. This routine knows how to
   * recognize parameter usage (starting with '$') and will in that case
   * try to resolve the parameter by finding it in 'params'. Otherwise
   * it will just return the unmodified string.
   */
  virtual const char* ResolveParameter (
  	const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params,
	const char* param) = 0;

  /**
   * Load a bunch of quests from this factory.
   * \param node is a node containing \<quest\> children.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;
};

//-------------------------------------------------------------------------
// Specific trigger implementations.
//-------------------------------------------------------------------------

SCF_VERSION (iEnterSectorQuestTriggerFactory, 0, 0, 1);

/**
 * This interface is implemented by the trigger that fires
 * when a certain sector is entered. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 * <p>
 * The predefined name of this trigger type is 'cel.questtrigger.entersector'.
 * <p>
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * <ul>
 * <li><em>entity_name</em>: the name of the entity that contains the
 *     pccamera property class.
 * <li><em>sector_name</em>: the name of the sector. As soon as the camera
 *     enters that sector this trigger will fire.
 * </ul>
 */
struct iEnterSectorQuestTriggerFactory : public iBase
{
  /**
   * Set the name of the entity containing the pccamera property class
   * on which this trigger will fire.
   * \param entity_name is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityNameParameter (const char* entity_name) = 0;

  /**
   * Set the name of the sector on which this trigger will fire
   * as soon as the camera enters that sector.
   * \param sector_name is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetSectorNameParameter (const char* sector_name) = 0;
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

#endif // __CEL_MGR_QUEST__

