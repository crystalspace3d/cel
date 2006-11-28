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

#include "cssysdef.h"
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/quests/reward_cssequence.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(CsSequence)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.quests.reward.sequence",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

//---------------------------------------------------------------------------

celCsSequenceRewardFactory::celCsSequenceRewardFactory (
	celCsSequenceRewardType* type) : scfImplementationType (this)
{
  celCsSequenceRewardFactory::type = type;
}

celCsSequenceRewardFactory::~celCsSequenceRewardFactory ()
{
}

csPtr<iQuestReward> celCsSequenceRewardFactory::CreateReward (
    iQuest*, const csHash<csStrKey,csStrKey>& params)
{
  celCsSequenceReward* trig = new celCsSequenceReward (type,
  	params, sequence_par, delay_par);
  return trig;
}

bool celCsSequenceRewardFactory::Load (iDocumentNode* node)
{
  sequence_par = node->GetAttributeValue ("sequence");
  delay_par = node->GetAttributeValue ("delay");
  if (!sequence_par)
    Report (type->object_reg,
      "'sequence' attribute is missing for the cssequence reward!");
  return true;
}

void celCsSequenceRewardFactory::SetSequenceParameter (
	const char* sequence)
{
  sequence_par = sequence;
}

void celCsSequenceRewardFactory::SetDelayParameter (
	const char* delay)
{
  delay_par = delay;
}

//---------------------------------------------------------------------------

celCsSequenceReward::celCsSequenceReward (
	celCsSequenceRewardType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* sequence_par,
	const char* delay_par) : scfImplementationType (this)
{
  celCsSequenceReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  eseqmgr = csQueryRegistry<iEngineSequenceManager> (type->object_reg);
  sequence = qm->ResolveParameter (params, sequence_par);
  delay = 0;
  if (delay_par)
  {
    const char* s = qm->ResolveParameter (params, delay_par);
    if (s) sscanf (s, "%d", &delay);
  }
}

celCsSequenceReward::~celCsSequenceReward ()
{
}

void celCsSequenceReward::Reward ()
{
  if (!eseqmgr->RunSequenceByName (sequence, delay))
    Report (type->object_reg, "Can't find sequence '%s'!",
	(const char*)sequence);
}

//---------------------------------------------------------------------------

