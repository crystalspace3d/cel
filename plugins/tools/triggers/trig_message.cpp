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
#include "propclass/mesh.h"

#include "plugins/tools/triggers/trig_message.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celMessageTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE(Message)

//---------------------------------------------------------------------------

celMessageTriggerFactory::celMessageTriggerFactory (
	celMessageTriggerType* type) : scfImplementationType (this)
{
  celMessageTriggerFactory::type = type;
}

celMessageTriggerFactory::~celMessageTriggerFactory ()
{
}

csPtr<iTrigger> celMessageTriggerFactory::CreateTrigger (
    iQuest* q, iCelParameterBlock* params)
{
  celMessageTrigger* trig = new celMessageTrigger (type, params,
  	entity_par, mask_par);
  return trig;
}

bool celMessageTriggerFactory::Load (iDocumentNode* node)
{
  entity_par.Empty ();
  mask_par.Empty ();

  entity_par = node->GetAttributeValue ("entity");
  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.message",
      "'entity' attribute is missing for the message trigger!");
    return false;
  }

  mask_par = node->GetAttributeValue ("mask");
  if (!mask_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.message",
      "'mask' attribute is missing for the message trigger!");
    return false;
  }

  return true;
}

void celMessageTriggerFactory::SetEntityParameter (
	const char* entity)
{
  entity_par = entity;
}

void celMessageTriggerFactory::SetMaskParameter (
	const char* mask)
{
  mask_par = mask;
}

//---------------------------------------------------------------------------

celMessageTrigger::celMessageTrigger (
	celMessageTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* mask_par)
	: scfImplementationType (this)
{
  celMessageTrigger::type = type;

  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->ResolveParameter (params, entity_par);
  mask = pm->ResolveParameter (params, mask_par);
}

celMessageTrigger::~celMessageTrigger ()
{
  DeactivateTrigger ();
}

void celMessageTrigger::RegisterCallback (iTriggerCallback* callback)
{
  celMessageTrigger::callback = callback;
}

void celMessageTrigger::ClearCallback ()
{
  callback = 0;
}

void celMessageTrigger::FindEntity ()
{
  if (!ent)
  {
    iCelPlLayer* pl = type->pl;
    ent = pl->FindEntity (entity);
  }
}

bool celMessageTrigger::ReceiveMessage (csStringID /*msgid*/,
	iMessageSender* /*sender*/,
	celData& /*ret*/, iCelParameterBlock* params)
{
  DeactivateTrigger ();
  callback->TriggerFired ((iTrigger*)this, params);
  return true;
}

void celMessageTrigger::ActivateTrigger ()
{
  FindEntity ();
  if (ent)
    ent->QueryMessageChannel ()->Subscribe (this, mask);
}

bool celMessageTrigger::Check ()
{
  return false;
}

void celMessageTrigger::DeactivateTrigger ()
{
  if (ent)
    ent->QueryMessageChannel ()->Unsubscribe (this, mask);
}

bool celMessageTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celMessageTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

//---------------------------------------------------------------------------

