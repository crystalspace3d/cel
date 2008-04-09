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

#ifndef __CEL_TOOLS_QUESTS_REWARD_SEQUENCE__
#define __CEL_TOOLS_QUESTS_REWARD_SEQUENCE__

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

/**
 * A standard reward type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This reward type listens to the name 'cel.questreward.sequence'.
 */
CEL_DECLARE_REWARDTYPE(Sequence,"cel.questreward.sequence")

/**
 * The 'sequence' reward factory.
 */
class celSequenceRewardFactory : public scfImplementation2<
	celSequenceRewardFactory, iQuestRewardFactory,
	iSequenceQuestRewardFactory>
{
private:
  celSequenceRewardType* type;
  csString entity_par;
  csString class_par;
  csString tag_par;
  csString sequence_par;
  csString delay_par;

public:
  celSequenceRewardFactory (celSequenceRewardType* type);
  virtual ~celSequenceRewardFactory () {};

  virtual csPtr<iQuestReward> CreateReward (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iSequenceQuestRewardFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetClassParameter (const char* ent_class) {class_par=ent_class;};
  virtual void SetTagParameter (const char* tag) {tag_par = tag;};
  virtual void SetSequenceParameter (const char* sequence);
  virtual void SetDelayParameter (const char* delay);
};

/**
 * The 'sequence' reward.
 */
class celSequenceReward : public scfImplementation1<
	celSequenceReward, iQuestReward>
{
private:
  celSequenceRewardType* type;
  csString entity;
  csStringID entity_dynamic;
  csString tag;
  csStringID tag_dynamic;
  csString sequence;
  csStringID sequence_dynamic;
  csTicks delay;
  csStringID delay_dynamic;
  csWeakRef<iCelEntity> ent;
  csWeakRef<iPcQuest> quest;

public:
  celSequenceReward (celSequenceRewardType* type,
  	const celQuestParams& params,
	const char* entity_par,
	const char* tag_par,
	const char* sequence_par,
	const char* delay_par);
  virtual ~celSequenceReward () {};

  virtual void Reward (iCelParameterBlock* params);
};

/**
 * The 'sequence' reward for classes.
 */
class celClassSequenceReward : public scfImplementation1<
	celClassSequenceReward, iQuestReward>
{
private:
  celSequenceRewardType* type;
  csString tag;
  csStringID tag_dynamic;
  csString sequence;
  csStringID sequence_dynamic;
  csTicks delay;
  csStringID delay_dynamic;
  csRef<iCelEntityList> entlist;
  csString clazz;
  csStringID clazz_dynamic;

public:
  celClassSequenceReward (celSequenceRewardType* type,
  	const celQuestParams& params,
	const char* class_par,
	const char* tag_par,
	const char* sequence_par,
	const char* delay_par);
  virtual ~celClassSequenceReward () {};

  virtual void Reward (iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_QUESTS_REWARD_SEQUENCE__

