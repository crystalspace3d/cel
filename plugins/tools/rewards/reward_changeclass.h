/*
    Crystal Space Entity Layer
    Copyright (C) 2013 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_REWARD_CHANGECLASS__
#define __CEL_TOOLS_REWARD_CHANGECLASS__


#include "physicallayer/entity.h"
#include "iutil/comp.h"
#include "tools/rewards.h"
#include "tools/parameters.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that can change a class for an entity.
 * This reward type listens to the name 'cel.rewards.changeclass'.
 */
CEL_DECLARE_REWARDTYPE(ChangeClass,"cel.rewards.changeclass")


/**
 * The 'changeclass' reward factory.
 */
class celChangeClassRewardFactory : public scfImplementation2<
	celChangeClassRewardFactory, iRewardFactory,
	iChangeClassRewardFactory>
{
private:
  csRef<celChangeClassRewardType> type;
  csString entity_par;
  csString class_par;
  bool remove;

public:
  celChangeClassRewardFactory (celChangeClassRewardType* type);
  virtual ~celChangeClassRewardFactory ();

  virtual csPtr<iReward> CreateReward (iQuest* q, iCelParameterBlock* params);
  virtual iRewardType* GetRewardType () const { return type; }
  virtual bool Save (iDocumentNode* node);
  virtual bool Load (iDocumentNode* node);

  //----------------- iChangeClassRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity);
  virtual const char* GetEntity () const { return entity_par; }
  virtual void SetClassParameter (const char* ent_class);
  virtual const char* GetClass () const { return class_par; }
  virtual void SetRemove (bool r) { remove = r; }
  virtual bool IsRemove () const { return remove; }
};

/**
 * The 'changeclass' reward.
 */
class celChangeClassReward : public scfImplementation1<
	celChangeClassReward, iReward>
{
private:
  csRef<celChangeClassRewardType> type;
  csRef<iParameterManager> pm;
  csRef<iParameter> entity;
  csRef<iParameter> clazz;
  bool remove;

public:
  celChangeClassReward (celChangeClassRewardType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* class_par, bool remove);
  virtual ~celChangeClassReward ();

  virtual void Reward (iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_REWARD_CHANGECLASS__

