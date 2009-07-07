/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_REWARD_CSSEQUENCE__
#define __CEL_TOOLS_REWARD_CSSEQUENCE__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/rewards.h"
#include "physicallayer/entity.h"
//#include "propclass/quest.h"
#include "ivaria/engseq.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that starts a cssequence
 * This reward type listens to the name 'cel.rewards.cssequence'.
 */
CEL_DECLARE_REWARDTYPE_NEW(CsSequence,"cel.rewards.cssequence")

/**
 * The 'cssequence' reward factory.
 */
class celCsSequenceRewardFactory : public scfImplementation2<
	celCsSequenceRewardFactory, iRewardFactory,
	iCsSequenceRewardFactory>
{
private:
  celCsSequenceRewardType* type;
  csString sequence_par;
  csString delay_par;

public:
  celCsSequenceRewardFactory (celCsSequenceRewardType* type);
  virtual ~celCsSequenceRewardFactory ();

  virtual csPtr<iReward> CreateReward (const celParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iCsSequenceRewardFactory -----------------------
  virtual void SetSequenceParameter (const char* sequence);
  virtual void SetDelayParameter (const char* delay);
};

/**
 * The 'cssequence' reward.
 */
class celCsSequenceReward : public scfImplementation1<
	celCsSequenceReward, iReward>
{
private:
  celCsSequenceRewardType* type;
  csRef<iParameter> sequence;
  csRef<iParameter> pdelay;
  csWeakRef<iEngineSequenceManager> eseqmgr;

public:
  celCsSequenceReward (celCsSequenceRewardType* type,
  	const celParams& params,
	const char* sequence_par,
	const char* delay_par);
  virtual ~celCsSequenceReward ();

  virtual void Reward (iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_REWARD_CSSEQUENCE__

