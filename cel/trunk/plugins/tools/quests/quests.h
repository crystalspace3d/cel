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
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A quest factory.
 */
class celQuestFactory : public iQuestFactory
{
private:
  char* name;

public:
  celQuestFactory (const char* name);
  virtual ~celQuestFactory ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual csPtr<iQuest> CreateQuest (
      const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params);
  virtual bool Load (iDocumentNode* node);
};
/**
 * This is a manager for quests.
 */
class celQuestManager : public iQuestManager
{
private:
  iObjectRegistry* object_reg;
  csHash<csRef<iQuestTriggerFactory>,csStrKey,
  	csConstCharHashKeyHandler> trigger_factories;
  csHash<csRef<iQuestRewardFactory>,csStrKey,
  	csConstCharHashKeyHandler> reward_factories;
  csHash<csRef<celQuestFactory>,csStrKey,
  	csConstCharHashKeyHandler> quest_factories;

public:
  celQuestManager (iBase* parent);
  virtual ~celQuestManager ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual bool RegisterTriggerFactory (iQuestTriggerFactory* trigger);
  virtual bool RegisterRewardFactory (iQuestRewardFactory* reward);
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

