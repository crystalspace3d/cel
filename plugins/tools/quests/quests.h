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
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

struct iObjectRegistry;
struct iEvent;
class celQuestManager;

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

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual iQuestTriggerResponseFactory* CreateTriggerResponseFactory ();
};

/**
 * A quest factory.
 */
class celQuestFactory : public iQuestFactory
{
private:
  celQuestManager* questmgr;
  char* name;
  csHash<csRef<celQuestStateFactory>,csStrKey,
  	csConstCharHashKeyHandler> states;

  csStringHash xmltokens;
#define CS_TOKEN_ITEM_FILE "plugins/tools/quests/quests.tok"
#include "cstool/tokenlist.h"

  bool LoadTriggerResponse (iQuestTriggerResponseFactory* respfact,
  	iQuestTriggerFactory* trigfact, iDocumentNode* node);
  bool LoadState (iQuestStateFactory* statefact, iDocumentNode* node);

public:
  celQuestFactory (celQuestManager* questmgr, const char* name);
  virtual ~celQuestFactory ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual csPtr<iQuest> CreateQuest (
      const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params);
  virtual bool Load (iDocumentNode* node);
  virtual iQuestStateFactory* GetState (const char* name);
  virtual iQuestStateFactory* CreateState (const char* name);
};

/**
 * A trigger and rewards. This is basically a response for a quest.
 */
struct celQuestStateResponse : public iQuestTriggerCallback
{
private:
  csWeakRef<iQuestTrigger> trigger;
  csRefArray<iQuestReward> rewards;

public:
  celQuestStateResponse ();
  virtual ~celQuestStateResponse ();

  void SetTrigger (iQuestTrigger* trigger);
  iQuestTrigger* GetTrigger () const { return trigger; }
  void AddReward (iQuestReward* reward);

  SCF_DECLARE_IBASE;

  virtual void TriggerFired (iQuestTrigger* trigger);
};

/**
 * A state in a quest.
 */
class celQuestState
{
private:
  char* name;
  csRefArray<celQuestStateResponse> responses;

public:
  celQuestState (const char* name)
  {
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
  csArray<celQuestState> states;
  int current_state;

public:
  celQuest ();
  virtual ~celQuest ();

  SCF_DECLARE_IBASE;
  
  virtual bool SwitchState (const char* state);
  virtual const char* GetCurrentState () const;

  // Add a state, returns the state index.
  int AddState (const char* name);
  // Add a response for a state. Return response index.
  int AddStateResponse (int stateidx);
  // Set trigger for a state and response.
  void SetStateTrigger (int stateidx, int responseidx, iQuestTrigger* trigger);
  // Add reward for a state and response.
  void AddStateReward (int stateidx, int responseidx, iQuestReward* reward);
};

/**
 * This is a manager for quests.
 */
class celQuestManager : public iQuestManager
{
public:
  iObjectRegistry* object_reg;

private:
  csHash<csRef<iQuestTriggerType>,csStrKey,
  	csConstCharHashKeyHandler> trigger_types;
  csHash<csRef<iQuestRewardType>,csStrKey,
  	csConstCharHashKeyHandler> reward_types;
  csHash<csRef<celQuestFactory>,csStrKey,
  	csConstCharHashKeyHandler> quest_factories;

public:
  celQuestManager (iBase* parent);
  virtual ~celQuestManager ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual bool RegisterTriggerType (iQuestTriggerType* trigger);
  virtual iQuestTriggerType* GetTriggerType (const char* name);
  virtual bool RegisterRewardType (iQuestRewardType* reward);
  virtual iQuestRewardType* GetRewardType (const char* name);
  virtual iQuestFactory* GetQuestFactory (const char* name);
  virtual iQuestFactory* CreateQuestFactory (const char* name);
  virtual const char* ResolveParameter (
  	const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params,
	const char* param);
  virtual bool Load (iDocumentNode* node);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celQuestManager);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_TOOLS_QUESTS__

