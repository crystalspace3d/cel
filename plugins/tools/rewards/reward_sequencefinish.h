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

#ifndef __CEL_TOOLS_REWARD_SEQUENCEFINISH__
#define __CEL_TOOLS_REWARD_SEQUENCEFINISH__

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
 * A standard reward type that finishes a sequence
 * This reward type listens to the name 'cel.rewards.sequencefinish'.
 */
CEL_DECLARE_REWARDTYPE(SequenceFinish,"cel.rewards.sequencefinish")

/**
 * The 'sequencefinish' reward factory.
 */
class celSequenceFinishRewardFactory : public scfImplementation2<
	celSequenceFinishRewardFactory, iRewardFactory,
	iSequenceFinishRewardFactory>
{
private:
  csRef<celSequenceFinishRewardType> type;
  csString entity_par;
  csString class_par;
  csString tag_par;
  csString sequence_par;
  csRef<iCelSequence> seq;

public:
  celSequenceFinishRewardFactory (celSequenceFinishRewardType* type);
  virtual ~celSequenceFinishRewardFactory () {};

  virtual csPtr<iReward> CreateReward (const celParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iSequenceFinishRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetClassParameter (const char* pclass) { class_par = pclass; };
  virtual void SetTagParameter (const char* tag) { tag_par = tag; };
  virtual void SetSequenceParameter (const char* sequence);
  virtual void SetSequence (iCelSequence* sequence);
};

/**
 * The 'sequencefinish' reward.
 */
class celSequenceFinishReward : public scfImplementation1<
	celSequenceFinishReward, iReward>
{
private:
  csRef<celSequenceFinishRewardType> type;
  csRef<iParameter> entity;
  csRef<iParameter> tag;
  csRef<iParameter> sequence_name;
  csWeakRef<iCelEntity> ent;
  csWeakRef<iPcQuest> quest;
  csWeakRef<iCelSequence> seq;

public:
  celSequenceFinishReward (celSequenceFinishRewardType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* sequence_par,
	iCelSequence* sequence = 0);
  virtual ~celSequenceFinishReward () {};

  virtual void Reward (iCelParameterBlock* params);
};

/**
 * The 'sequencefinish' reward that works on classes.
 */
class celClassSequenceFinishReward : public scfImplementation1<
	celClassSequenceFinishReward, iReward>
{
private:
  csRef<celSequenceFinishRewardType> type;
  csRef<iParameter> tag;
  csRef<iParameter> sequence_name;
  csRef<iParameter> clazz;
  csRef<iCelEntityList> entlist;

public:
  celClassSequenceFinishReward (celSequenceFinishRewardType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* sequence_par);
  virtual ~celClassSequenceFinishReward () {};

  virtual void Reward (iCelParameterBlock* params);
};


#endif // __CEL_TOOLS_REWARD_SEQUENCEFINISH__

