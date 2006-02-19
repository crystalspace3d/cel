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

#ifndef __CEL_TOOLS_QUESTS_REWARD_MESSAGE__
#define __CEL_TOOLS_QUESTS_REWARD_MESSAGE__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"
#include "physicallayer/entity.h"
#include "propclass/quest.h"

struct iObjectRegistry;
struct iEvent;
class celVariableParameterBlock;

/**
 * A standard reward type that sends a message to an entity.
 * This reward type listens to the name 'cel.questreward.message'.
 */
CEL_DECLARE_REWARDTYPE(Message,"cel.questreward.message")

struct parSpec
{
  celDataType type;
  csStringID id;
  csString name;
  csString value;
};

/**
 * The 'message' reward factory.
 */
class celMessageRewardFactory :
	public iQuestRewardFactory,
	public iMessageQuestRewardFactory
{
private:
  celMessageRewardType* type;
  char* entity_par;
  char* id_par;
  csArray<parSpec> parameters;

public:
  celMessageRewardFactory (celMessageRewardType* type);
  virtual ~celMessageRewardFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestReward> CreateReward (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iMessageQuestRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity);
  virtual void SetIDParameter (const char* id);
  virtual void AddParameter (celDataType type, csStringID id,
      const char* name, const char* value);
};

/**
 * The 'message' reward.
 */
class celMessageReward :
	public iQuestReward
{
private:
  celMessageRewardType* type;
  char* entity;
  char* id;
  csWeakRef<iCelEntity> ent;
  celVariableParameterBlock* msg_params;

public:
  celMessageReward (celMessageRewardType* type,
  	const celQuestParams& params,
	const char* entity_par,
	const char* id_par,
	const csArray<parSpec>& parameters);
  virtual ~celMessageReward ();

  SCF_DECLARE_IBASE;

  virtual void Reward ();
};

#endif // __CEL_TOOLS_QUESTS_REWARD_MESSAGE__

