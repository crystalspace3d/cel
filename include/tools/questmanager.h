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

/*
<quest>
    <state name="notstarted">
    </state>
    <state name="start">
        <trigger type="in_inventory">
	    <on>
	        <entity name="player" />
	        <new_entity name="computer_part" />
	    </on>
	    <action>
	    	<state>state2</state>
	    </action>
	</trigger>
    </state>
    <state name="state2">
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
   */
  virtual csPtr<iQuestTrigger> CreateTrigger () = 0;
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
};

#endif // __CEL_MGR_QUEST__

