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


#include "plugins/tools/rewards/reward_sequence.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celSequenceRewardType)
CEL_IMPLEMENT_REWARDTYPE(Sequence)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.rewards.sequence",
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

csPtr<iReward> celSequenceRewardFactory::CreateReward (
    iQuest* q, iCelParameterBlock* params)
{
  iReward *reward;
  if (!class_par.IsEmpty())
  {
    reward = new celClassSequenceReward (type,
  	  params, class_par, tag_par, sequence_par, delay_par);
  }
  else
  {
    if (seq.IsValid())
    {
      reward = new celSequenceReward (type,
	params, entity_par, tag_par, sequence_par, delay_par, seq);
    }
    else
    {
      reward = new celSequenceReward (type,
	params, entity_par, tag_par, sequence_par, delay_par);
    }
  }

  return reward;
}

bool celSequenceRewardFactory::Save (iDocumentNode* node)
{
  if (!entity_par.IsEmpty ()) node->SetAttribute ("entity", entity_par);
  if (!class_par.IsEmpty ()) node->SetAttribute ("class", class_par);
  if (!sequence_par.IsEmpty ()) node->SetAttribute ("sequence", sequence_par);
  if (!delay_par.IsEmpty ()) node->SetAttribute ("delay", delay_par);
  if (!tag_par.IsEmpty ()) node->SetAttribute ("entity_tag", tag_par);
  return true;
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

void celSequenceRewardFactory::SetSequence (
	iCelSequence* sequence)
{
  seq = sequence;
}

void celSequenceRewardFactory::SetDelayParameter (
	const char* delay)
{
  delay_par = delay;
}

//---------------------------------------------------------------------------

celSequenceReward::celSequenceReward (
	celSequenceRewardType* type,
  	iCelParameterBlock* params,
	const char* entity_par,
	const char* tag_par,
	const char* sequence_par,
	const char* delay_par,
	iCelSequence* sequence) : scfImplementationType (this)
{
  celSequenceReward::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->GetParameter (params, entity_par);
  tag = pm->GetParameter (params, tag_par);
  sequence_name = pm->GetParameter (params, sequence_par);
  pdelay = pm->GetParameter (params, delay_par);

  seq = sequence;
}

void celSequenceReward::Reward (iCelParameterBlock* params)
{
  if (!seq)
  {
    iCelEntity* newent = pm->ResolveEntityParameter (type->pl, params, entity, ent);
    if (!newent) return;
    if (newent != ent) { quest = 0; ent = newent; }

    bool changed;
    const char* t = tag->Get (params, changed);
    if (changed) quest = 0;

    if (!quest)
    {
      quest = celQueryPropertyClassTagEntity<iPcQuest> (ent, t);
      if (!quest)
      {
	csString p = entity->Get (params);
	if (!p) p = entity->GetOriginalExpression ();
	csString s = sequence_name->Get (params);
	if (!s) s = sequence_name->GetOriginalExpression ();
        Report (type->object_reg,
      	  "Can't find quest in entity '%s' and tag '%s' (for sequence '%s')!",
	      p.GetData (), t, s.GetData ());
	return;
      }
    }

    // @@@
    // Remove Quest Dependency?
    iQuest* q = quest->GetQuest ();
    const char* s = sequence_name->Get (params);
    if (!s)
    {
      csString p = entity->Get (params);
      if (!p) p = entity->GetOriginalExpression ();
      Report (type->object_reg,
      	  "Can't find sequence name in entity '%s' and tag '%s'!",
	      p.GetData (), t);
      return;
    }
    seq = q->FindSequence (s);
    if (!seq)
    {
      if (t)
        Report (type->object_reg,
      	  "Can't find sequence '%s' in entity '%s' and tag '%s'!",
	      s, entity->Get (params), t);
      else
        Report (type->object_reg, "Can't find sequence '%s' in entity '%s'!",
    	  s, entity->Get (params));
      return;
    }
  }

  csTicks delay = pdelay->GetLong (params);
  if (!seq->Start (delay, params))
  {
    Report (type->object_reg, "Sequence fails to start!");
    return;
  }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

celClassSequenceReward::celClassSequenceReward (
	celSequenceRewardType* type,
  	iCelParameterBlock* params,
	const char* class_par,
	const char* tag_par,
	const char* sequence_par,
	const char* delay_par) : scfImplementationType (this)
{
  celClassSequenceReward::type = type;

  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  clazz = pm->GetParameter (params, class_par);
  tag = pm->GetParameter (params, tag_par);
  sequence_name = pm->GetParameter (params, sequence_par);
  pdelay = pm->GetParameter (params, delay_par);
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

  const char* s = sequence_name->Get (params);
  if (!s) return;
  csTicks delay = pdelay->GetLong (params);
  const char* t = tag->Get (params);
  iCelEntity *ent;
  csRef<iPcQuest> quest;
  size_t i = entlist->GetCount();
  while (i>0)
  {
    i--;
    ent = entlist->Get(i);
    quest = celQueryPropertyClassTagEntity<iPcQuest> (ent, t);
    if (quest)
    {
      iCelSequence* seq = quest->GetQuest()->FindSequence (s);
      if (!seq)
      {
        if (t)
          Report (type->object_reg,
      	    "Can't find sequence '%s' in entity '%s' and tag '%s'!", s, ent->GetName (), t);
        else
          Report (type->object_reg, "Can't find sequence '%s' in entity '%s'!", s, ent->GetName ());
      }
      if (!seq->Start (delay, params))
      {
        Report (type->object_reg, "Sequence '%s' in entity '%s' fails to start!", s, ent->GetName ());
      }
    }
  }
}

