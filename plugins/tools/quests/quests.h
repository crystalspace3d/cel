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
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

struct iObjectRegistry;
struct iEvent;

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
  char* name;
  csHash<csRef<celQuestStateFactory>,csStrKey,
  	csConstCharHashKeyHandler> states;

public:
  celQuestFactory (const char* name);
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
 * This is a manager for quests.
 */
class celQuestManager : public iQuestManager
{
private:
  iObjectRegistry* object_reg;
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
  virtual bool RegisterRewardType (iQuestRewardType* reward);
  virtual iQuestFactory* GetQuestFactory (const char* name);
  virtual iQuestFactory* CreateQuestFactory (const char* name);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celQuestManager);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_TOOLS_QUESTS__

