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

#ifndef __CEL_TOOLS_QUESTS_REWARD_NEWSTATE__
#define __CEL_TOOLS_QUESTS_REWARD_NEWSTATE__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"
#include "physicallayer/entity.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This reward type listens to the name 'cel.rewards.newstate'.
 */
CEL_DECLARE_QUESTREWARDTYPE(NewState,"cel.rewards.newstate")

/**
 * The 'newstate' reward factory.
 */
class celNewStateRewardFactory : public scfImplementation2<
	celNewStateRewardFactory, iRewardFactory,
	iNewStateQuestRewardFactory>
{
private:
  csRef<celNewStateRewardType> type;
  csString state_par;
  csString entity_par;
  csString class_par;
  csString tag_par;

public:
  celNewStateRewardFactory (celNewStateRewardType* type);
  virtual ~celNewStateRewardFactory () {};

  virtual csPtr<iReward> CreateReward (iQuest* q, iCelParameterBlock* params);
  virtual iRewardType* GetRewardType () const { return type; }
  virtual bool Save (iDocumentNode* node);
  virtual bool Load (iDocumentNode* node);

  //----------------- iNewStateQuestRewardFactory -----------------------
  virtual void SetStateParameter (const char* state);
  virtual const char* GetStateParameter () const { return state_par; }
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual const char* GetEntityParameter () const { return entity_par; }
  virtual void SetTagParameter (const char* tag) { tag_par = tag; };
  virtual const char* GetTagParameter () const { return tag_par; }
  virtual void SetClassParameter (const char* pclass) { class_par = pclass; };
  virtual const char* GetClassParameter () const { return class_par; }
};

/**
 * The 'newstate' reward.
 */
class celNewStateReward : public scfImplementation2<
	celNewStateReward, iReward, iCelTimerListener>
{
private:
  csRef<celNewStateRewardType> type;
  csRef<iParameter> state;
  csRef<iParameter> entity;
  csRef<iParameter> tag;
  csWeakRef<iCelEntity> ent;
  csWeakRef<iQuest> quest;
  // Keep reward parameters for next frame.
  csRef<iCelParameterBlock> reward_params;

public:
  celNewStateReward (celNewStateRewardType* type,
  	iQuest* q, 
	iCelParameterBlock* params,
	const char* state_par,
	const char* entity_par, const char* tag_par);
  virtual ~celNewStateReward () {};

  virtual void Reward (iCelParameterBlock* params);

  virtual void TickEveryFrame ();
  virtual void TickOnce () { }
};

/**
 * The 'newstate' reward for classes.
 */
class celClassNewStateReward : public scfImplementation2<
	celClassNewStateReward, iReward, iCelTimerListener>
{
private:
  csRef<celNewStateRewardType> type;
  csRef<iParameter> state;
  csRef<iParameter> tag;
  csRef<iParameter> clazz;
  csRef<iCelEntityList> entlist;
  // Keep reward parameters for next frame.
  csRef<iCelParameterBlock> reward_params;

public:
  celClassNewStateReward (celNewStateRewardType* type,
  	iCelParameterBlock* params,
	const char* state_par,
	const char* entity_par, const char* tag_par);
  virtual ~celClassNewStateReward () {};

  virtual void Reward (iCelParameterBlock* params);

  virtual void TickEveryFrame ();
  virtual void TickOnce () { }
};


#endif // __CEL_TOOLS_QUESTS_REWARD_NEWSTATE__

