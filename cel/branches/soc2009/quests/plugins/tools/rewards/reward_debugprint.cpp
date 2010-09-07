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

#include "cssysdef.h"
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/camera.h"

#include "plugins/tools/rewards/reward_debugprint.h"

//---------------------------------------------------------------------------

//CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celDebugPrintRewardType)
CEL_IMPLEMENT_REWARDTYPE(DebugPrint)

//---------------------------------------------------------------------------

celDebugPrintRewardFactory::celDebugPrintRewardFactory (
	celDebugPrintRewardType* type) : scfImplementationType (this)
{
  celDebugPrintRewardFactory::type = type;
}

celDebugPrintRewardFactory::~celDebugPrintRewardFactory ()
{
}

csPtr<iReward> celDebugPrintRewardFactory::CreateReward (const celParams& params)
{
  celDebugPrintReward* trig = new celDebugPrintReward (type, params, msg_par);
  return trig;
}

bool celDebugPrintRewardFactory::Load (iDocumentNode* node)
{
  msg_par = node->GetAttributeValue ("message");

  if (!msg_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.rewards.debugprint",
      "'message' attribute is missing for the debugprint reward!");
    return false;
  }
  return true;
}

void celDebugPrintRewardFactory::SetMessageParameter (const char* msg)
{
  msg_par = msg;
}


celDebugPrintReward::celDebugPrintReward (
	celDebugPrintRewardType* type,
  	const celParams& params,
	const char* msg_par) : scfImplementationType (this)
{
  celDebugPrintReward::type = type;

  csRef<iPluginManager> plugin_mgr = 
   csQueryRegistry<iPluginManager> (type->object_reg);
  pm = csLoadPlugin<iParameterManager> (plugin_mgr,
    "cel.parameters.manager");

  if (pm.IsValid()){
	  msg = pm->GetParameter(params, msg_par);
  } else {
	  printf("reward.debugprint: Failed To Load Parameter Manager");
  }
}

celDebugPrintReward::~celDebugPrintReward ()
{
}

void celDebugPrintReward::Reward (iCelParameterBlock* params)
{
  const char* m = msg->Get (params); 
  if (!m) return;
  printf ("%s\n", m);
  fflush (stdout);
}
