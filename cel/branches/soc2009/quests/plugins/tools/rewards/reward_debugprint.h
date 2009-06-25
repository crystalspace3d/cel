/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_REWARDS_REWARD_DEBUGPRINT__
#define __CEL_TOOLS_REWARDS_REWARD_DEBUGPRINT__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/rewards.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This reward type listens to the name 'cel.questreward.debugprint'.
 */

CEL_DECLARE_REWARDTYPE_NEW(DebugPrint,"cel.rewards.debugprint")


/**
 * The 'debugprint' reward factory.
 */
class celDebugPrintRewardFactory : public scfImplementation2<
	celDebugPrintRewardFactory, iRewardFactory,
	iDebugPrintRewardFactory>
{
private:
  celDebugPrintRewardType* type;
  csString msg_par;

public:
  celDebugPrintRewardFactory (celDebugPrintRewardType* type);
  virtual ~celDebugPrintRewardFactory ();

  virtual csPtr<iReward> CreateReward ();
	  //iQuest*,
      //const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iDebugPrintQuestRewardFactory -----------------------
  virtual void SetMessageParameter (const char* msg);
};

/**
 * The 'printdebug' reward.
 */
class celDebugPrintReward : public scfImplementation1<
	celDebugPrintReward, iReward>
{
private:
  celDebugPrintRewardType* type;
  //csRef<iQuestParameter> msg;
  const char* msg;

public:
  celDebugPrintReward (celDebugPrintRewardType* type,
  	//const celQuestParams& params,
	const char* msg_par);
  virtual ~celDebugPrintReward ();

  virtual void Reward (iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_REWARDS_REWARD_DEBUGPRINT__

