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

#ifndef __CEL_TOOLS_REWARD_ACTION__
#define __CEL_TOOLS_REWARD_ACTION__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "physicallayer/entity.h"
#include "tools/rewards.h"

struct iObjectRegistry;
struct iEvent;
class celVariableParameterBlock;

/**
 * A standard reward type that sends an action to an entity.
 * This reward type listens to the name 'cel.rewards.action'.
 */
CEL_DECLARE_REWARDTYPE(Action,"cel.rewards.action")

/**
 * The 'action' reward factory.
 */
class celActionRewardFactory : public scfImplementation2<
	celActionRewardFactory, iRewardFactory, iActionRewardFactory>
{
private:
  friend class celActionReward;
  friend class celClassActionReward;

  csRef<celActionRewardType> type;
  csString entity_par;
  csString class_par;
  csString id_par;
  csString pcclass_par;
  csString tag_par;
  csArray<celParSpec> parameters;

public:
  celActionRewardFactory (celActionRewardType* type);
  virtual ~celActionRewardFactory ();

  virtual csPtr<iReward> CreateReward (iQuest* q, iCelParameterBlock* params);
  virtual iRewardType* GetRewardType () const { return type; }
  virtual bool Save (iDocumentNode* node);
  virtual bool Load (iDocumentNode* node);

  //----------------- iActionRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity);
  virtual void SetClassParameter (const char* entity_class);
  virtual void SetIDParameter (const char* id);
  virtual void SetPropertyClassParameter (const char* propertyclass);
  virtual void SetTagParameter (const char* pctag);
  virtual void AddParameter (celDataType type, csStringID id, const char* value);
};

/**
 * The 'action' reward.
 */
class celActionReward : public scfImplementation1<celActionReward,
	iReward>
{
private:
  csRef<celActionRewardFactory> factory;
  //csWeakRef<iQuestManager> qm;
  csRef<iParameterManager> pm;
  csRef<iParameter> pcclass;
  csRef<iParameter> tag;
  csRef<iParameter> entity;
  csRef<iParameter> id;
  csWeakRef<iCelEntity> ent;
  csRef<celVariableParameterBlock> act_params;
  csRefArray<iParameter> quest_parameters;

public:
  celActionReward (celActionRewardFactory* factory,
  	iCelParameterBlock* params,
	const char* entity_par,
	const char* id_par,
	const char* pcclass_par,
	const char* tag_par);
  virtual ~celActionReward ();

  virtual void Reward (iCelParameterBlock* params);
};

/**
 * The 'action' reward working on classes
 */
class celClassActionReward : public scfImplementation1<celClassActionReward,
	iReward>
{
private:
  csRef<celActionRewardFactory> factory;
  //csWeakRef<iQuestManager> qm;
  csRef<iParameterManager> pm;
  csRef<iParameter> pcclass;
  csRef<iParameter> tag;
  csRef<iParameter> clazz;
  csRef<iParameter> id;
  csStringID actionID;

  csRef<iCelEntityList> entlist;
  csRef<celVariableParameterBlock> act_params;
  csRefArray<iParameter> quest_parameters;

public:
  celClassActionReward (celActionRewardFactory* factory,
  	iCelParameterBlock* params,
	const char* class_par,
	const char* id_par,
	const char* pcclass_par,
	const char* tag_par);
  virtual ~celClassActionReward ();

  virtual void Reward (iCelParameterBlock* params);
};



#endif // __CEL_TOOLS_REWARD_ACTION__

