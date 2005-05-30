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

#include "plugins/tools/quests/reward_sequence.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(Sequence)

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celSequenceRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iSequenceQuestRewardFactory)
SCF_IMPLEMENT_IBASE_END

celSequenceRewardFactory::celSequenceRewardFactory (
	celSequenceRewardType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celSequenceRewardFactory::type = type;
  entity_par = 0;
  tag_par = 0;
  sequence_par = 0;
  delay_par = 0;
}

celSequenceRewardFactory::~celSequenceRewardFactory ()
{
  delete[] entity_par;
  delete[] tag_par;
  delete[] sequence_par;
  delete[] delay_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestReward> celSequenceRewardFactory::CreateReward (
    const csHash<csStrKey,csStrKey>& params)
{
  celSequenceReward* trig = new celSequenceReward (type,
  	params, entity_par, tag_par, sequence_par, delay_par);
  return trig;
}

bool celSequenceRewardFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] sequence_par; sequence_par = 0;
  delete[] delay_par; delay_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  sequence_par = csStrNew (node->GetAttributeValue ("sequence"));
  delay_par = csStrNew (node->GetAttributeValue ("delay"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));
  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.sequence",
      "'entity' attribute is missing for the sequence reward!");
    return false;
  }
  if (!sequence_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.sequence",
      "'sequence' attribute is missing for the sequence reward!");
    return false;
  }
  return true;
}

void celSequenceRewardFactory::SetEntityParameter (
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

void celSequenceRewardFactory::SetSequenceParameter (
	const char* sequence)
{
  if (sequence_par == sequence) return;
  delete[] sequence_par;
  sequence_par = csStrNew (sequence);
}

void celSequenceRewardFactory::SetDelayParameter (
	const char* delay)
{
  if (delay_par == delay) return;
  delete[] delay_par;
  delay_par = csStrNew (delay);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celSequenceReward)
  SCF_IMPLEMENTS_INTERFACE (iQuestReward)
SCF_IMPLEMENT_IBASE_END

celSequenceReward::celSequenceReward (
	celSequenceRewardType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par,
	const char* tag_par,
	const char* sequence_par,
	const char* delay_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celSequenceReward::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  sequence = csStrNew (qm->ResolveParameter (params, sequence_par));
  delay = 0;
  if (delay_par)
  {
    const char* s = qm->ResolveParameter (params, delay_par);
    if (s) sscanf (s, "%d", &delay);
  }
}

celSequenceReward::~celSequenceReward ()
{
  delete[] entity;
  delete[] tag;
  delete[] sequence;
  SCF_DESTRUCT_IBASE ();
}

void celSequenceReward::Reward ()
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  if (!quest)
  {
    if (!ent)
    {
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
    // @@@ Report error!
    return;
  }
  if (!seq->Start (delay))
  {
    // @@@ Report error!
    return;
  }
}

//---------------------------------------------------------------------------

