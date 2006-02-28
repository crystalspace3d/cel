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

#ifndef __CEL_TOOLS_QUESTS_REWARD_DESTROYENTITY__
#define __CEL_TOOLS_QUESTS_REWARD_DESTROYENTITY__

#include "tools/questmanager.h"
#include "physicallayer/entity.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that removes an entity entity from the physical.
 * layer. This reward type listens to the name 'cel.questreward.destroyentity'.
 */
CEL_DECLARE_REWARDTYPE(DestroyEntity,"cel.questreward.destroyentity")


/**
 * The 'destroyentity' reward factory.
 */
class celDestroyEntityRewardFactory :
	public iQuestRewardFactory,
	public iDestroyEntityQuestRewardFactory
{
private:
  celDestroyEntityRewardType* type;
  char* entity_par;

public:
  celDestroyEntityRewardFactory (celDestroyEntityRewardType* type);
  virtual ~celDestroyEntityRewardFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestReward> CreateReward (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iDestroyEntityQuestRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity);
};

/**
 * The 'destroyentity' reward.
 */
class celDestroyEntityReward :
	public iQuestReward
{
private:
  celDestroyEntityRewardType* type;
  char* entity;

public:
  celDestroyEntityReward (celDestroyEntityRewardType* type,
  	const celQuestParams& params,
	const char* entity_par);
  virtual ~celDestroyEntityReward ();

  SCF_DECLARE_IBASE;

  virtual void Reward ();
};

#endif // __CEL_TOOLS_QUESTS_REWARD_DESTROYENTITY__

