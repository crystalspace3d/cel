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

#include "plugins/tools/triggers/trig_sequencefinish.h"

// @@@
// Remove Quest Dependency
#include "propclass/quest.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celSequenceFinishTriggerType)
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

csPtr<iTrigger> celSequenceFinishTriggerFactory::CreateTrigger (
    iQuest* q, iCelParameterBlock* params)
{
  celSequenceFinishTrigger* trig;
  if (seq.IsValid())
  {
    trig = new celSequenceFinishTrigger (type,
  	  params, entity_par, tag_par, sequence_par, seq);
  }
  else
  {
    trig = new celSequenceFinishTrigger (type,
  	  params, entity_par, tag_par, sequence_par);
  }

  return trig;
}

bool celSequenceFinishTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_tag");

  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.sequencefinish",
      "'entity' attribute is missing for the sequencefinish trigger!");
    return false;
  }
  sequence_par = node->GetAttributeValue ("sequence");
  if (!sequence_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.sequencefinish",
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

void celSequenceFinishTriggerFactory::SetSequence (iCelSequence* sequence)
{
  seq = sequence;
}

//---------------------------------------------------------------------------

celSequenceFinishTrigger::celSequenceFinishTrigger (
	celSequenceFinishTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	const char* sequence_par,
	iCelSequence* sequence) : scfImplementationType (this)
{
  celSequenceFinishTrigger::type = type;

  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->ResolveParameter (params, entity_par);
  tag = pm->ResolveParameter (params, tag_par);
  sequence_name = pm->ResolveParameter (params, sequence_par);

  seq = sequence;

  finished = false;
}

celSequenceFinishTrigger::~celSequenceFinishTrigger ()
{
  DeactivateTrigger ();
}

void celSequenceFinishTrigger::RegisterCallback (iTriggerCallback* callback)
{
  celSequenceFinishTrigger::callback = callback;
}

void celSequenceFinishTrigger::ClearCallback ()
{
  callback = 0;
}

void celSequenceFinishTrigger::SequenceFinished (iCelSequence* seq)
{
  finished = true;
  DeactivateTrigger ();
  callback->TriggerFired ((iTrigger*)this, 0);
}

void celSequenceFinishTrigger::FindSequence ()
{
  if (seq) return;
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return;
  csRef<iPcQuest> pcquest = celQueryPropertyClassTagEntity<iPcQuest> (ent, tag);
  if (!pcquest) return;
  iQuest* quest = pcquest->GetQuest ();
  seq = quest->FindSequence (sequence_name);
}

void celSequenceFinishTrigger::ActivateTrigger ()
{
  finished = false;
  FindSequence ();
  if (!seq) return;
  // First remove to make sure we don't register ourselves multiple
  // times.
  seq->RemoveSequenceCallback ((iCelSequenceCallback*)this);
  seq->AddSequenceCallback ((iCelSequenceCallback*)this);
}

bool celSequenceFinishTrigger::Check ()
{
  if (finished)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iTrigger*)this, 0);
  }
  return finished;
}

void celSequenceFinishTrigger::DeactivateTrigger ()
{
  FindSequence ();
  if (!seq) return;
  seq->RemoveSequenceCallback ((iCelSequenceCallback*)this);
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

