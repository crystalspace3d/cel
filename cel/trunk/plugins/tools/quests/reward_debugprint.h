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

#ifndef __CEL_TOOLS_QUESTS_REWARD_DEBUGPRINT__
#define __CEL_TOOLS_QUESTS_REWARD_DEBUGPRINT__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard reward type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This reward type listens to the name 'cel.questreward.debugprint'.
 */
CEL_DECLARE_REWARDTYPE(DebugPrint,"cel.questreward.debugprint")

/**
 * The 'debugprint' reward factory.
 */
class celDebugPrintRewardFactory :
	public iQuestRewardFactory,
	public iDebugPrintQuestRewardFactory
{
private:
  celDebugPrintRewardType* type;
  char* msg_par;

public:
  celDebugPrintRewardFactory (celDebugPrintRewardType* type);
  virtual ~celDebugPrintRewardFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestReward> CreateReward (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iDebugPrintQuestRewardFactory -----------------------
  virtual void SetMessageParameter (const char* msg);
};

/**
 * The 'printdebug' reward.
 */
class celDebugPrintReward :
	public iQuestReward
{
private:
  celDebugPrintRewardType* type;
  char* msg;

public:
  celDebugPrintReward (celDebugPrintRewardType* type,
  	const celQuestParams& params,
	const char* msg_par);
  virtual ~celDebugPrintReward ();

  SCF_DECLARE_IBASE;

  virtual void Reward ();
};

#endif // __CEL_TOOLS_QUESTS_REWARD_DEBUGPRINT__

