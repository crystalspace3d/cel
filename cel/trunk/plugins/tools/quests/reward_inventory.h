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
class celInventoryRewardFactory : public scfImplementation2<
	celInventoryRewardFactory, iQuestRewardFactory,
	iInventoryQuestRewardFactory>
{
private:
  celInventoryRewardType* type;
  csString entity_par;
  csString tag_par;
  csString child_entity_par;
  csString child_tag_par;

public:
  celInventoryRewardFactory (celInventoryRewardType* type);
  virtual ~celInventoryRewardFactory ();

  virtual csPtr<iQuestReward> CreateReward (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iInventoryQuestRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetChildEntityParameter (const char* child_entity,
  	const char* tag = 0);
};

/**
 * The 'inventory' reward.
 */
class celInventoryReward : public scfImplementation1<
	celInventoryReward, iQuestReward>
{
private:
  celInventoryRewardType* type;
  csString entity;
  csStringID entity_dynamic;
  csString tag;
  csStringID tag_dynamic;
  csString child_entity;
  csStringID child_entity_dynamic;
  csString child_tag;
  csStringID child_tag_dynamic;
  csWeakRef<iCelEntity> ent;
  csWeakRef<iPcInventory> inventory;

public:
  celInventoryReward (celInventoryRewardType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* child_entity_par, const char* child_tag_par);
  virtual ~celInventoryReward ();

  virtual void Reward (iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_QUESTS_REWARD_INVENTORY__

