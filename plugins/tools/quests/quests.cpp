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

#include "cssysdef.h"
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"

#include "plugins/tools/quests/quests.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celQuestManager)

SCF_IMPLEMENT_IBASE (celQuestManager)
  SCF_IMPLEMENTS_INTERFACE (iQuestManager)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celQuestManager::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celQuestManager::celQuestManager (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celQuestManager::~celQuestManager ()
{
}

bool celQuestManager::Initialize (iObjectRegistry* object_reg)
{
  celQuestManager::object_reg = object_reg;

  return true;
}

bool celQuestManager::RegisterTriggerFactory (iQuestTriggerFactory* trigger)
{
  const char* name = trigger->GetName ();
  if (trigger == trigger_factories.Get (name, 0))
    return false;
  trigger_factories.Put (name, trigger);
  return true;
}

bool celQuestManager::RegisterRewardFactory (iQuestRewardFactory* reward)
{
  const char* name = reward->GetName ();
  if (reward == reward_factories.Get (name, 0))
    return false;
  reward_factories.Put (name, reward);
  return true;
}

//---------------------------------------------------------------------------
