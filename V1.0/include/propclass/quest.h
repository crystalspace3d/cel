/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_QUEST__
#define __CEL_PF_QUEST__

#include "cstypes.h"
#include "csutil/scf.h"

#include "tools/questmanager.h"

SCF_VERSION (iPcQuest, 0, 0, 1);

/**
 * This property class represents a quest. It uses the quest manager
 * to manage quests. Note that this property class will automatically
 * load the quest manager if it is missing.
 *
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * - pcquest_???: ??? (???)
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - NewQuest: parameters 'name' (string). All other string parameters will be
 *     passed to the quest itself.
 * - StopQuest: no parameters.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - name (string, read only): name of the quest factory or null if no quest.
 * - state (string, read/write): name of the current state.
 */
struct iPcQuest : public iBase
{
  /**
   * Initialize this quest with a new named quest. The quest manager
   * will be used to find a quest factory that has the given name. Then
   * that quest factory will be used to make a new quest instance
   * that is then managed by this property class. If there is already
   * a quest then that quest will be stopped.
   * \return false if quest factory could not be found or if the
   * quest could not be created for another reason.
   */
  virtual bool NewQuest (const char* name, celQuestParams& params) = 0;

  /**
   * Stop the current quest. Does nothing if there is no quest.
   */
  virtual void StopQuest () = 0;

  /**
   * Get the quest represented by this property class.
   */
  virtual iQuest* GetQuest () const = 0;

  /**
   * Get the name of the running quest.
   */
  virtual const char* GetQuestName () const = 0;
};

#endif // __CEL_PF_QUEST__

