/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
    Copyright (C) 2009 by Pablo Martin
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

#ifndef __CEL_TOOLS_REWARD_CREATEENTITY__
#define __CEL_TOOLS_REWARD_CREATEENTITY__

#include "physicallayer/entity.h"
#include "iutil/comp.h"
#include "tools/rewards.h"
#include "tools/parameters.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that removes an entity entity from the physical.
 * layer. This reward type listens to the name 'cel.rewards.createentity'.
 */
CEL_DECLARE_REWARDTYPE(CreateEntity,"cel.rewards.createentity")


/**
 * The 'createentity' reward factory.
 */
class celCreateEntityRewardFactory : public scfImplementation2<
	celCreateEntityRewardFactory, iRewardFactory,
	iCreateEntityRewardFactory>
{
private:
  csRef<celCreateEntityRewardType> type;
  csString template_par;
  csString name_par;
  csRef<celVariableParameterBlock> params;

public:
  celCreateEntityRewardFactory (celCreateEntityRewardType* type);
  virtual ~celCreateEntityRewardFactory ();

  virtual csPtr<iReward> CreateReward (iQuest* q, iCelParameterBlock* params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iCreateEntityRewardFactory -----------------------
  virtual void SetEntityTemplateParameter (const char* entity_tpl);
  virtual void SetNameParameter (const char* name);
  virtual void AddParameter (const char* name, const char* value);
};

/**
 * The 'createentity' reward.
 */
class celCreateEntityReward : public scfImplementation1<
	celCreateEntityReward, iReward>
{
private:
  csRef<celCreateEntityRewardType> type;
  csRef<iParameter> name;
  csRef<iParameter> entity_tpl;
  csRef<celVariableParameterBlock> params;
  csRef<iParameterManager> pm;

public:
  celCreateEntityReward (celCreateEntityRewardType* type,
  	iCelParameterBlock* params,
	const char* template_par,
	const char* name_par,
        iCelParameterBlock* tpl_params);
  virtual ~celCreateEntityReward ();

  virtual void Reward (iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_REWARD_CREATEENTITY__

