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
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"

#include "plugins/tools/quests/quests.h"
#include "plugins/tools/quests/trig_entersector.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celQuestManager)

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuestTriggerResponseFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerResponseFactory)
SCF_IMPLEMENT_IBASE_END

celQuestTriggerResponseFactory::celQuestTriggerResponseFactory ()
{
  SCF_CONSTRUCT_IBASE (0);
}

celQuestTriggerResponseFactory::~celQuestTriggerResponseFactory ()
{
  SCF_DESTRUCT_IBASE ();
}

void celQuestTriggerResponseFactory::SetTriggerFactory (
	iQuestTriggerFactory* trigger_fact)
{
  trigger_factory = trigger_fact;
}

void celQuestTriggerResponseFactory::AddRewardFactory (
	iQuestRewardFactory* reward_fact)
{
  reward_factories.Push (reward_fact);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuestStateFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestStateFactory)
SCF_IMPLEMENT_IBASE_END

celQuestStateFactory::celQuestStateFactory (const char* name)
{
  SCF_CONSTRUCT_IBASE (0);
  celQuestStateFactory::name = csStrNew (name);
}

celQuestStateFactory::~celQuestStateFactory ()
{
  delete[] name;
  SCF_DESTRUCT_IBASE ();
}

iQuestTriggerResponseFactory* celQuestStateFactory::
	CreateTriggerResponseFactory ()
{
  celQuestTriggerResponseFactory* resp = new celQuestTriggerResponseFactory ();
  responses.Push (resp);
  resp->DecRef ();
  return resp;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celQuestFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestFactory)
SCF_IMPLEMENT_IBASE_END

celQuestFactory::celQuestFactory (const char* name)
{
  SCF_CONSTRUCT_IBASE (0);
  celQuestFactory::name = csStrNew (name);
}

celQuestFactory::~celQuestFactory ()
{
  delete[] name;
  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuest> celQuestFactory::CreateQuest (
      const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params)
{
  return 0;
}

bool celQuestFactory::Load (iDocumentNode* node)
{
  return false;
}

iQuestStateFactory* celQuestFactory::GetState (const char* name)
{
  celQuestStateFactory* state = states.Get (name, 0);
  return (iQuestStateFactory*)state;
}

iQuestStateFactory* celQuestFactory::CreateState (const char* name)
{
  iQuestStateFactory* istate = GetState (name);
  if (istate) return 0;

  celQuestStateFactory* state = new celQuestStateFactory (name);
  states.Put (name, state);
  state->DecRef ();
  return state;
}

//---------------------------------------------------------------------------

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
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celQuestManager::Initialize (iObjectRegistry* object_reg)
{
  celQuestManager::object_reg = object_reg;

  celEnterSectorTriggerType* type_es = new celEnterSectorTriggerType (
  	object_reg);
  RegisterTriggerType (type_es);
  type_es->DecRef ();

  return true;
}

iQuestFactory* celQuestManager::GetQuestFactory (const char* name)
{
  celQuestFactory* fact = quest_factories.Get (name, 0);
  return (iQuestFactory*)fact;
}

iQuestFactory* celQuestManager::CreateQuestFactory (const char* name)
{
  iQuestFactory* ifact = GetQuestFactory (name);
  if (ifact) return 0;

  celQuestFactory* fact = new celQuestFactory (name);
  quest_factories.Put (name, fact);
  fact->DecRef ();
  return fact;
}

bool celQuestManager::RegisterTriggerType (iQuestTriggerType* trigger)
{
  const char* name = trigger->GetName ();
  if (trigger_types.Get (name, 0) != 0)
    return false;
  trigger_types.Put (name, trigger);
  return true;
}

bool celQuestManager::RegisterRewardType (iQuestRewardType* reward)
{
  const char* name = reward->GetName ();
  if (reward_types.Get (name, 0) != 0)
    return false;
  reward_types.Put (name, reward);
  return true;
}

//---------------------------------------------------------------------------
