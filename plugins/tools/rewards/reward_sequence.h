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

#ifndef __CEL_TOOLS_REWARD_SEQUENCE__
#define __CEL_TOOLS_REWARD_SEQUENCE__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/rewards.h"
#include "physicallayer/entity.h"
#include "propclass/quest.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that starts a sequence
 * This reward type listens to the name 'cel.rewards.sequence'.
 */
CEL_DECLARE_REWARDTYPE(Sequence,"cel.rewards.sequence")

/**
 * The 'sequence' reward factory.
 */
class celSequenceRewardFactory : public scfImplementation2<
	celSequenceRewardFactory, iRewardFactory,
	iSequenceRewardFactory>
{
private:
  csRef<celSequenceRewardType> type;
  csString entity_par;
  csString class_par;
  csString tag_par;
  csString sequence_par;
  csString delay_par;
  csRef<iCelSequence> seq;

public:
  celSequenceRewardFactory (celSequenceRewardType* type);
  virtual ~celSequenceRewardFactory () {};

  virtual csPtr<iReward> CreateReward (iQuest* q, iCelParameterBlock* params);
  virtual iRewardType* GetRewardType () const { return type; }
  virtual bool Save (iDocumentNode* node);
  virtual bool Load (iDocumentNode* node);

  //----------------- iSequenceRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetClassParameter (const char* ent_class) {class_par=ent_class;};
  virtual void SetTagParameter (const char* tag) {tag_par = tag;};
  virtual void SetSequenceParameter (const char* sequence);
  virtual void SetSequence (iCelSequence* sequence);
  virtual void SetDelayParameter (const char* delay);
};

/**
 * The 'sequence' reward.
 */
class celSequenceReward : public scfImplementation1<
	celSequenceReward, iReward>
{
private:
  csRef<celSequenceRewardType> type;
  csRef<iParameter> entity;
  csRef<iParameter> tag;
  csRef<iParameter> sequence_name;
  csRef<iParameter> pdelay;
  csWeakRef<iCelEntity> ent;
  csWeakRef<iPcQuest> quest;
  csWeakRef<iCelSequence> seq;
  csRef<iParameterManager> pm;

public:
  celSequenceReward (celSequenceRewardType* type,
  	iCelParameterBlock* params,
	const char* entity_par,
	const char* tag_par,
	const char* sequence_par,
	const char* delay_par,
	iCelSequence* sequence = 0);
  virtual ~celSequenceReward () {};

  virtual void Reward (iCelParameterBlock* params);
};

/**
 * The 'sequence' reward for classes.
 */
class celClassSequenceReward : public scfImplementation1<
	celClassSequenceReward, iReward>
{
private:
  csRef<celSequenceRewardType> type;
  csRef<iParameter> tag;
  csRef<iParameter> sequence_name;
  csRef<iParameter> clazz;
  csRef<iParameter> pdelay;
  csRef<iCelEntityList> entlist;

public:
  celClassSequenceReward (celSequenceRewardType* type,
  	iCelParameterBlock* params,
	const char* class_par,
	const char* tag_par,
	const char* sequence_par,
	const char* delay_par);
  virtual ~celClassSequenceReward () {};

  virtual void Reward (iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_REWARD_SEQUENCE__

