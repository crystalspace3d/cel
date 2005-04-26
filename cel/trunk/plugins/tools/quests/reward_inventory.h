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

#ifndef __CEL_TOOLS_QUESTS_REWARD_INVENTORY__
#define __CEL_TOOLS_QUESTS_REWARD_INVENTORY__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"
#include "physicallayer/entity.h"
#include "propclass/inv.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This reward type listens to the name 'cel.questreward.inventory'.
 */
CEL_DECLARE_REWARDTYPE(Inventory,"cel.questreward.inventory")

/**
 * The 'inventory' reward factory.
 */
class celInventoryRewardFactory :
	public iQuestRewardFactory,
	public iInventoryQuestRewardFactory
{
private:
  celInventoryRewardType* type;
  char* entity_name_par;
  char* child_entity_name_par;

public:
  celInventoryRewardFactory (celInventoryRewardType* type);
  virtual ~celInventoryRewardFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestReward> CreateReward (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iInventoryQuestRewardFactory -----------------------
  virtual void SetEntityNameParameter (const char* entity_name);
  virtual void SetChildEntityNameParameter (const char* child_entity_name);
};

/**
 * The 'inventory' reward.
 */
class celInventoryReward :
	public iQuestReward
{
private:
  celInventoryRewardType* type;
  char* entity_name;
  char* child_entity_name;
  csWeakRef<iCelEntity> entity;
  csWeakRef<iPcInventory> inventory;

public:
  celInventoryReward (celInventoryRewardType* type,
  	const celQuestParams& params,
	const char* entity_name_par,
	const char* child_entity_name_par);
  virtual ~celInventoryReward ();

  SCF_DECLARE_IBASE;

  virtual void Reward ();
};

#endif // __CEL_TOOLS_QUESTS_REWARD_INVENTORY__

