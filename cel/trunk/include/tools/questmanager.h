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
<quest>
    <params>
    	<par name="actor_entity" />
    </params>

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
    </state>

    <start>notstarted</start>
</quest>
*/


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
 * triggers here.
 */
struct iQuestTrigger : public iBase
{
  /**
   * Register a callback with this trigger. When the trigger fires
   * it will call this callback.
   */
  virtual void RegisterCallback (iQuestTriggerCallback* callback) = 0;

  /**
   * Remove a callback.
   */
  virtual void UnregisterCallback (iQuestTriggerCallback* callback) = 0;
};

SCF_VERSION (iQuestTriggerFactory, 0, 0, 1);

/**
 * A factory to create triggers. You register these factories to
 * the quest manager and the quest manager can use these factories
 * to create triggers for a quest.
 */
struct iQuestTriggerFactory : public iBase
{
  /**
   * Return the name for this trigger factory.
   */
  virtual const char* GetName () const = 0;
  
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

SCF_VERSION (iQuestReward, 0, 0, 1);

/**
 * This is a reward for a quest. The quest manager can issue
 * rewards in response to some trigger.
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
 * A factory to create rewards. You register these factories to
 * the quest manager and the quest manager can use these factories
 * to create rewards for a quest.
 */
struct iQuestRewardFactory : public iBase
{
  /**
   * Return the name for this reward factory.
   */
  virtual const char* GetName () const = 0;
  
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

SCF_VERSION (iQuest, 0, 0, 1);

/**
 * A quest.
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

SCF_VERSION (iQuestFactory, 0, 0, 1);

/**
 * A quest factory.
 */
struct iQuestFactory : public iBase
{
  /**
   * Get the name of this quest.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Create a quest from this factory.
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
};

SCF_VERSION (iQuestManager, 0, 0, 1);

/**
 * This is the quest manager. This manager can be used to handle quests.
 * A quest is basically a state machine which can advance from state to
 * state when a trigger activates. It can also hand out 'rewards'.
 */
struct iQuestManager : public iBase
{
  /**
   * Register a quest trigger factory. Quest triggers can be used
   * by quests to decide when to go to another state or when
   * to activate a reward. Returns false on failure (trigger
   * factory with that name already exists).
   */
  virtual bool RegisterTriggerFactory (iQuestTriggerFactory* trigger) = 0;

  /**
   * Register a quest reward factory. Quest rewards can be used
   * by quests to give out some kind of reward to the game.
   * Returns false on failure (reward factory with that name
   * already exists).
   */
  virtual bool RegisterRewardFactory (iQuestRewardFactory* trigger) = 0;
};

#endif // __CEL_MGR_QUEST__

