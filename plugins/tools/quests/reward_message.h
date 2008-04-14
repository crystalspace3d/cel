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
#include "physicallayer/messaging.h"
#include "propclass/quest.h"

struct iObjectRegistry;
struct iEvent;
class celVariableParameterBlock;

/**
 * A standard reward type that sends a message to an entity.
 * This reward type listens to the name 'cel.questreward.message'.
 */
CEL_DECLARE_REWARDTYPE(Message,"cel.questreward.message")



/**
 * The 'message' reward factory.
 */
class celMessageRewardFactory : public scfImplementation2<
	celMessageRewardFactory, iQuestRewardFactory,
	iMessageQuestRewardFactory>
{
private:
  celMessageRewardType* type;
  csString entity_par;
  csString class_par;
  csString id_par;
  csArray<celParSpec> parameters;

public:
  celMessageRewardFactory (celMessageRewardType* type);
  virtual ~celMessageRewardFactory () {};

  virtual csPtr<iQuestReward> CreateReward (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iMessageQuestRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity);
  virtual void SetClassParameter (const char* ent_class);
  virtual void SetIDParameter (const char* id);
  virtual void AddParameter (celDataType type, csStringID id,
      const char* name, const char* value);
};

/**
 * The 'message' reward that affects entities.
 */
class celMessageReward : public scfImplementation2<
	celMessageReward, iQuestReward, iMessageSender>
{
private:
  celMessageRewardType* type;
  csWeakRef<iQuestManager> qm;

  csRef<iQuestParameter> msg_id;
  csRef<iQuestParameter> entity;
  csRef<celVariableParameterBlock> msg_params;
  csArray<csStringID> msg_params_dynamic;

  csWeakRef<iCelEntity> ent;
  csRef<iMessageDispatcher> dispatcher;

public:
  celMessageReward (celMessageRewardType* type,
  	const celQuestParams& params,
	const char* entity_par,
	const char* id_par,
	const csArray<celParSpec>& parameters);
  virtual ~celMessageReward () {};

  virtual void Reward (iCelParameterBlock* params);

  // --- For iMessageSender --------------------------------------------
  virtual void MessageDispatcherRemoved (
      iMessageDispatcher* dispatcher) { }
};

/**
 * The 'message' reward that affects entity classes.
 */
class celClassMessageReward : public scfImplementation1<
	celClassMessageReward, iQuestReward>
{
private:
  celMessageRewardType* type;
  csWeakRef<iQuestManager> qm;

  csRef<iQuestParameter> msg_id;
  csRef<iQuestParameter> clazz;

  csRef<celVariableParameterBlock> msg_params;
  csArray<csStringID> msg_params_dynamic;

  csRef<iCelEntityList> entlist;

public:
  celClassMessageReward (celMessageRewardType* type,
  	const celQuestParams& params,
	const char* class_par,
	const char* id_par,
	const csArray<celParSpec>& parameters);
  virtual ~celClassMessageReward () {};

  virtual void Reward (iCelParameterBlock* params);

};

#endif // __CEL_TOOLS_QUESTS_REWARD_MESSAGE__

