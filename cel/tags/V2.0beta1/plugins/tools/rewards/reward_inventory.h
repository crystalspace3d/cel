/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin
  
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

#ifndef __CEL_TOOLS_REWARD_INVENTORY__
#define __CEL_TOOLS_REWARD_INVENTORY__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "physicallayer/entity.h"
#include "propclass/inv.h"
#include "tools/rewards.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that mainpulates the inventory
 * This reward type listens to the name 'cel.rewards.inventory'.
 */
CEL_DECLARE_REWARDTYPE(Inventory,"cel.rewards.inventory")

/**
 * The 'inventory' reward factory.
 */
class celInventoryRewardFactory : public scfImplementation2<
	celInventoryRewardFactory, iRewardFactory,
	iInventoryRewardFactory>
{
private:
  csRef<celInventoryRewardType> type;
  csString entity_par;
  csString tag_par;
  csString child_entity_par;
  csString child_tag_par;

public:
  celInventoryRewardFactory (celInventoryRewardType* type);
  virtual ~celInventoryRewardFactory ();

  virtual csPtr<iReward> CreateReward (const celParams& params);
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
	celInventoryReward, iReward>
{
private:
  csRef<celInventoryRewardType> type;
  csRef<iParameterManager> pm;
  csRef<iParameter> entity;
  csRef<iParameter> tag;
  csRef<iParameter> child_entity;
  csRef<iParameter> child_tag;
  csWeakRef<iCelEntity> ent;
  csWeakRef<iPcInventory> inventory;

public:
  celInventoryReward (celInventoryRewardType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* child_entity_par, const char* child_tag_par);
  virtual ~celInventoryReward ();

  virtual void Reward (iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_REWARD_INVENTORY__

