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

#include "plugins/tools/quests/reward_sequencefinish.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(SequenceFinish)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.quests.reward.sequencefinish", msg, arg);
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

celSequenceFinishRewardFactory::celSequenceFinishRewardFactory (
	celSequenceFinishRewardType* type) :
	scfImplementationType (this)
{
  celSequenceFinishRewardFactory::type = type;
  entity_par = 0;
  tag_par = 0;
  sequence_par = 0;
}

celSequenceFinishRewardFactory::~celSequenceFinishRewardFactory ()
{
  delete[] entity_par;
  delete[] tag_par;
  delete[] sequence_par;
}

csPtr<iQuestReward> celSequenceFinishRewardFactory::CreateReward (
    iQuest*, const csHash<csStrKey,csStrKey>& params)
{
  celSequenceFinishReward* trig = new celSequenceFinishReward (type,
  	params, entity_par, tag_par, sequence_par);
  return trig;
}

bool celSequenceFinishRewardFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] sequence_par; sequence_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));
  sequence_par = csStrNew (node->GetAttributeValue ("sequence"));
  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the sequencefinish reward!");
  if (!sequence_par)
    return Report (type->object_reg,
      "'sequence' attribute is missing for the sequencefinish reward!");
  return true;
}

void celSequenceFinishRewardFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
  if (tag_par != tag)
  {
    delete[] tag_par;
    tag_par = csStrNew (tag);
  }
}

void celSequenceFinishRewardFactory::SetSequenceParameter (
	const char* sequence)
{
  if (sequence_par == sequence) return;
  delete[] sequence_par;
  sequence_par = csStrNew (sequence);
}

//---------------------------------------------------------------------------

celSequenceFinishReward::celSequenceFinishReward (
	celSequenceFinishRewardType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par, const char* tag_par,
	const char* sequence_par) : scfImplementationType (this)
{
  celSequenceFinishReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  sequence = csStrNew (qm->ResolveParameter (params, sequence_par));
}

celSequenceFinishReward::~celSequenceFinishReward ()
{
  delete[] entity;
  delete[] tag;
  delete[] sequence;
}

void celSequenceFinishReward::Reward ()
{
  if (!quest)
  {
    if (!ent)
    {
      iCelPlLayer* pl = type->pl;
      ent = pl->FindEntity (entity);
      if (!ent) return;
    }
    quest = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcQuest, tag);
    if (!quest) return;
  }

  iQuest* q = quest->GetQuest ();
  iQuestSequence* seq = q->FindSequence (sequence);
  if (!seq)
  {
    if (tag)
      Report (type->object_reg,
      	"Can't find sequence '%s' in entity '%s' and tag '%s'!",
	(const char*)sequence, (const char*)entity, (const char*)tag);
    else
      Report (type->object_reg, "Can't find sequence '%s' in entity '%s'!",
    	  (const char*)sequence, (const char*)entity);
    return;
  }
  seq->Finish ();
}

//---------------------------------------------------------------------------

