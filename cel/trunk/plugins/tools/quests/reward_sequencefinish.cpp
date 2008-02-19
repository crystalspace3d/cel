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
}

csPtr<iQuestReward> celSequenceFinishRewardFactory::CreateReward (
    iQuest*, const celQuestParams& params)
{
  iQuestReward *reward;
  if (!class_par.IsEmpty())
  {
    reward = new celClassSequenceFinishReward (type,
  	params, class_par, tag_par, sequence_par); 
  }
  else
  {
    reward = new celSequenceFinishReward (type,
  	params, entity_par, tag_par, sequence_par); 
  }
  return reward;
}

bool celSequenceFinishRewardFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  class_par = node->GetAttributeValue ("class");
  tag_par = node->GetAttributeValue ("entity_tag");
  sequence_par = node->GetAttributeValue ("sequence");

  if (entity_par.IsEmpty() && class_par.IsEmpty())
    return Report (type->object_reg,
      "'entity' or 'class' attribute is required for the sequencefinish reward!");
  if (sequence_par.IsEmpty())
    return Report (type->object_reg,
      "'sequence' attribute is missing for the sequencefinish reward!");
  return true;
}

void celSequenceFinishRewardFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celSequenceFinishRewardFactory::SetSequenceParameter (
	const char* sequence)
{
  sequence_par = sequence;
}

//---------------------------------------------------------------------------

celSequenceFinishReward::celSequenceFinishReward (
	celSequenceFinishRewardType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* sequence_par) : scfImplementationType (this)
{
  celSequenceFinishReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);

  entity = qm->ResolveParameter (params, entity_par);
  tag = qm->ResolveParameter (params, tag_par);
  sequence = qm->ResolveParameter (params, sequence_par);
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

celClassSequenceFinishReward::celClassSequenceFinishReward (
	celSequenceFinishRewardType* type,
  	const celQuestParams& params,
	const char* class_par, const char* tag_par,
	const char* sequence_par) : scfImplementationType (this)
{
  celClassSequenceFinishReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);

  csStringID classid =
        type->pl->FetchStringID(qm->ResolveParameter (params, class_par));
  entlist = type->pl->GetClassEntitiesList(classid);

  tag = qm->ResolveParameter (params, tag_par);
  sequence = qm->ResolveParameter (params, sequence_par);
}

void celClassSequenceFinishReward::Reward ()
{

  iCelEntity *ent;
  csRef<iPcQuest> quest;
  for (int i = entlist->GetCount()-1; i>=0; i--)
  {
    ent = entlist->Get(i);
    quest = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcQuest, tag);
    if (quest)
    {
      iQuestSequence* seq = quest->GetQuest()->FindSequence (sequence);
      if (!seq)
      {
        if (tag)
          Report (type->object_reg,
      	    "Can't find sequence '%s' in entity '%s' and tag '%s'!",
	     (const char*)sequence, (const char*)entity, (const char*)tag);
        else
          Report (type->object_reg, "Can't find sequence '%s' in entity '%s'!",
    	   (const char*)sequence, (const char*)entity);
      }
      seq->Finish ();
    }
  }
}

//---------------------------------------------------------------------------

