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

#include "plugins/tools/quests/quests.h"
#include "plugins/tools/quests/reward_sequence.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(Sequence)

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

celSequenceRewardFactory::celSequenceRewardFactory (
	celSequenceRewardType* type) : scfImplementationType (this)
{
  celSequenceRewardFactory::type = type;
}

csPtr<iQuestReward> celSequenceRewardFactory::CreateReward (
    iQuest*, const celQuestParams& params)
{
  iQuestReward *reward;
  if (!class_par.IsEmpty())
  {
    reward = new celClassSequenceReward (type,
  	params, class_par, tag_par, sequence_par, delay_par);
  }
  else
    reward = new celSequenceReward (type,
  	params, entity_par, tag_par, sequence_par, delay_par);
  return reward;
}

bool celSequenceRewardFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  class_par = node->GetAttributeValue ("class");
  sequence_par = node->GetAttributeValue ("sequence");
  delay_par = node->GetAttributeValue ("delay");
  tag_par = node->GetAttributeValue ("entity_tag");
  if (entity_par.IsEmpty() && class_par.IsEmpty())
    return Report (type->object_reg,
      "'entity' or 'class' attribute required for the sequence reward!");
  if (sequence_par.IsEmpty())
    Report (type->object_reg,
      "'sequence' attribute is missing for the sequence reward!");
  return true;
}

void celSequenceRewardFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celSequenceRewardFactory::SetSequenceParameter (
	const char* sequence)
{
  sequence_par = sequence;
}

void celSequenceRewardFactory::SetDelayParameter (
	const char* delay)
{
  delay_par = delay;
}

//---------------------------------------------------------------------------

celSequenceReward::celSequenceReward (
	celSequenceRewardType* type,
  	const celQuestParams& params,
	const char* entity_par,
	const char* tag_par,
	const char* sequence_par,
	const char* delay_par) : scfImplementationType (this)
{
  celSequenceReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  entity = qm->GetParameter (params, entity_par);
  tag = qm->GetParameter (params, tag_par);
  sequence = qm->GetParameter (params, sequence_par);
  pdelay = qm->GetParameter (params, delay_par);
}

void celSequenceReward::Reward (iCelParameterBlock* params)
{
  bool changed;
  const char* e = entity->Get (params, changed);
  if (changed) { quest = 0; ent = 0; }
  const char* t = tag->Get (params, changed);
  if (changed) quest = 0;

  if (!quest)
  {
    if (!ent)
    {
      iCelPlLayer* pl = type->pl;
      ent = pl->FindEntity (e);
      if (!ent) return;
    }
    quest = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcQuest, t);
    if (!quest) return;
  }

  iQuest* q = quest->GetQuest ();
  const char* s = sequence->Get (params);
  if (!s) return;
  iQuestSequence* seq = q->FindSequence (s);
  if (!seq)
  {
    if (t)
      Report (type->object_reg,
      	"Can't find sequence '%s' in entity '%s' and tag '%s'!",
	s, e, t);
    else
      Report (type->object_reg, "Can't find sequence '%s' in entity '%s'!",
    	  s, e);
    return;
  }
  csTicks delay = pdelay->GetLong (params);
  if (!seq->Start (delay))
  {
    Report (type->object_reg, "Sequence '%s' in entity '%s' fails to start!",
    	  s, e);
    return;
  }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

celClassSequenceReward::celClassSequenceReward (
	celSequenceRewardType* type,
  	const celQuestParams& params,
	const char* class_par,
	const char* tag_par,
	const char* sequence_par,
	const char* delay_par) : scfImplementationType (this)
{
  celClassSequenceReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  clazz = qm->GetParameter (params, class_par);
  tag = qm->GetParameter (params, tag_par);
  sequence = qm->GetParameter (params, sequence_par);
  pdelay = qm->GetParameter (params, delay_par);
}

void celClassSequenceReward::Reward (iCelParameterBlock* params)
{
  bool changed;
  const char* clz = clazz->Get (params, changed);
  if (changed || !entlist)
  {
    csStringID ent_class = type->pl->FetchStringID (clz);
    entlist = type->pl->GetClassEntitiesList (ent_class);
  }

  const char* s = sequence->Get (params);
  if (!s) return;
  csTicks delay = pdelay->GetLong (params);
  const char* t = tag->Get (params);
  iCelEntity *ent;
  csRef<iPcQuest> quest;
  for (size_t i = entlist->GetCount()-1; i>=0; i--)
  {
    ent = entlist->Get(i);
    quest = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcQuest, t);
    if (quest)
    {
      iQuestSequence* seq = quest->GetQuest()->FindSequence (s);
      if (!seq)
      {
        if (t)
          Report (type->object_reg,
      	    "Can't find sequence '%s' in entity '%s' and tag '%s'!", s, ent->GetName (), t);
        else
          Report (type->object_reg, "Can't find sequence '%s' in entity '%s'!", s, ent->GetName ());
      }
      if (!seq->Start (delay))
      {
        Report (type->object_reg, "Sequence '%s' in entity '%s' fails to start!", s, ent->GetName ());
      }
    }
  }
}

