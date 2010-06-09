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
#include "propclass/quest.h"

#include "plugins/tools/quests/trig_sequencefinish.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(SequenceFinish)

//---------------------------------------------------------------------------

celSequenceFinishTriggerFactory::celSequenceFinishTriggerFactory (
	celSequenceFinishTriggerType* type) : scfImplementationType (this)
{
  celSequenceFinishTriggerFactory::type = type;
}

celSequenceFinishTriggerFactory::~celSequenceFinishTriggerFactory ()
{
}

csPtr<iQuestTrigger> celSequenceFinishTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  celSequenceFinishTrigger* trig = new celSequenceFinishTrigger (type,
  	params, entity_par, tag_par, sequence_par);
  return trig;
}

bool celSequenceFinishTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_tag");

  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.sequencefinish",
      "'entity' attribute is missing for the sequencefinish trigger!");
    return false;
  }
  sequence_par = node->GetAttributeValue ("sequence");
  if (!sequence_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.sequencefinish",
      "'sequence' attribute is missing for the sequencefinish trigger!");
    return false;
  }
  return true;
}

void celSequenceFinishTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celSequenceFinishTriggerFactory::SetSequenceParameter (
	const char* sequence)
{
  sequence_par = sequence;
}

//---------------------------------------------------------------------------

celSequenceFinishTrigger::celSequenceFinishTrigger (
	celSequenceFinishTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* sequence_par) : scfImplementationType (this)
{
  celSequenceFinishTrigger::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  entity = qm->ResolveParameter (params, entity_par);
  tag = qm->ResolveParameter (params, tag_par);
  sequence = qm->ResolveParameter (params, sequence_par);

  finished = false;
}

celSequenceFinishTrigger::~celSequenceFinishTrigger ()
{
  DeactivateTrigger ();
}

void celSequenceFinishTrigger::RegisterCallback (iQuestTriggerCallback* callback)
{
  celSequenceFinishTrigger::callback = callback;
}

void celSequenceFinishTrigger::ClearCallback ()
{
  callback = 0;
}

void celSequenceFinishTrigger::SequenceFinished (iQuestSequence* seq)
{
  finished = true;
  DeactivateTrigger ();
  callback->TriggerFired ((iQuestTrigger*)this, 0);
}

void celSequenceFinishTrigger::FindSequence ()
{
  if (seq) return;
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return;
  csRef<iPcQuest> pcquest = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcQuest, tag);
  if (!pcquest) return;
  iQuest* quest = pcquest->GetQuest ();
  seq = quest->FindSequence (sequence);
}

void celSequenceFinishTrigger::ActivateTrigger ()
{
  finished = false;
  FindSequence ();
  if (!seq) return;
  // First remove to make sure we don't register ourselves multiple
  // times.
  seq->RemoveSequenceCallback ((iQuestSequenceCallback*)this);
  seq->AddSequenceCallback ((iQuestSequenceCallback*)this);
}

bool celSequenceFinishTrigger::Check ()
{
  if (finished)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this, 0);
  }
  return finished;
}

void celSequenceFinishTrigger::DeactivateTrigger ()
{
  FindSequence ();
  if (!seq) return;
  seq->RemoveSequenceCallback ((iQuestSequenceCallback*)this);
}

bool celSequenceFinishTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celSequenceFinishTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

//---------------------------------------------------------------------------

