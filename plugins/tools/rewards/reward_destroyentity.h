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

#ifndef __CEL_TOOLS_REWARD_DESTROYENTITY__
#define __CEL_TOOLS_REWARD_DESTROYENTITY__


#include "physicallayer/entity.h"
#include "iutil/comp.h"
#include "tools/rewards.h"
#include "tools/parameters.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that removes an entity entity from the physical.
 * layer. This reward type listens to the name 'cel.rewards.destroyentity'.
 */
CEL_DECLARE_REWARDTYPE(DestroyEntity,"cel.rewards.destroyentity")


/**
 * The 'destroyentity' reward factory.
 */
class celDestroyEntityRewardFactory : public scfImplementation2<
	celDestroyEntityRewardFactory, iRewardFactory,
	iDestroyEntityRewardFactory>
{
private:
  csRef<celDestroyEntityRewardType> type;
  csString entity_par;
  csString class_par;

public:
  celDestroyEntityRewardFactory (celDestroyEntityRewardType* type);
  virtual ~celDestroyEntityRewardFactory ();

  virtual csPtr<iReward> CreateReward (iQuest* q, iCelParameterBlock* params);
  virtual iRewardType* GetRewardType () const { return type; }
  virtual bool Save (iDocumentNode* node);
  virtual bool Load (iDocumentNode* node);

  //----------------- iDestroyEntityRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity);
  virtual void SetClassParameter (const char* ent_class);
};

/**
 * The 'destroyentity' reward that works on entities.
 */
class celDestroyEntityReward : public scfImplementation1<
	celDestroyEntityReward, iReward>
{
private:
  csRef<celDestroyEntityRewardType> type;
  csRef<iParameterManager> pm;
  csRef<iParameter> entity;

public:
  celDestroyEntityReward (celDestroyEntityRewardType* type,
  	iCelParameterBlock* params,
	const char* entity_par);
  virtual ~celDestroyEntityReward ();

  virtual void Reward (iCelParameterBlock* params);
};

/**
 * The 'destroyentity' reward that works on classes.
 */
class celDestroyClassReward : public scfImplementation1<
	celDestroyClassReward, iReward>
{
private:
  csRef<celDestroyEntityRewardType> type;
  csRef<iParameterManager> pm;
  csRef<iParameter> ent_class;

public:
  celDestroyClassReward (celDestroyEntityRewardType* type,
  	iCelParameterBlock* params,
	const char* class_par);
  virtual ~celDestroyClassReward ();

  virtual void Reward (iCelParameterBlock* params);
};


#endif // __CEL_TOOLS_REWARD_DESTROYENTITY__

