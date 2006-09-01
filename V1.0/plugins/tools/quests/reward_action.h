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

#ifndef __CEL_TOOLS_QUESTS_REWARD_ACTION__
#define __CEL_TOOLS_QUESTS_REWARD_ACTION__

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
 * A standard reward type that sends an action to an entity.
 * This reward type listens to the name 'cel.questreward.action'.
 */
CEL_DECLARE_REWARDTYPE(Action,"cel.questreward.action")

/**
 * The 'action' reward factory.
 */
class celActionRewardFactory : public scfImplementation2<
	celActionRewardFactory, iQuestRewardFactory, iActionQuestRewardFactory>
{
private:
  celActionRewardType* type;
  char* entity_par;
  char* id_par;
  char* pcclass_par;
  char* tag_par;
  csArray<parSpec> parameters;

public:
  celActionRewardFactory (celActionRewardType* type);
  virtual ~celActionRewardFactory ();

  virtual csPtr<iQuestReward> CreateReward (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iActionQuestRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity);
  virtual void SetIDParameter (const char* id);
  virtual void SetPropertyClassParameter (const char* propertyclass);
  virtual void SetTagParameter (const char* pctag);
  virtual void AddParameter (celDataType type, csStringID id,
      const char* name, const char* value);
};

/**
 * The 'action' reward.
 */
class celActionReward : public scfImplementation1<celActionReward,
	iQuestReward>
{
private:
  celActionRewardType* type;
  char* pcclass;
  char* tag;
  char* entity;
  char* id;
  csWeakRef<iCelEntity> ent;
  celVariableParameterBlock* act_params;

public:
  celActionReward (celActionRewardType* type,
  	const celQuestParams& params,
	const char* entity_par,
	const char* id_par,
	const char* pcclass_par,
	const char* tag_par,
	const csArray<parSpec>& parameters);
  virtual ~celActionReward ();

  virtual void Reward ();
};

#endif // __CEL_TOOLS_QUESTS_REWARD_ACTION__

