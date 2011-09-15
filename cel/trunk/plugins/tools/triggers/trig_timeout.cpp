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
#include "physicallayer/persist.h"
#include "propclass/camera.h"

#include "plugins/tools/triggers/trig_timeout.h"

//---------------------------------------------------------------------------

//CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celTimeoutTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE(Timeout)

//---------------------------------------------------------------------------

celTimeoutTriggerFactory::celTimeoutTriggerFactory (
	celTimeoutTriggerType* type) : scfImplementationType (this)
{
  celTimeoutTriggerFactory::type = type;
}

celTimeoutTriggerFactory::~celTimeoutTriggerFactory ()
{
}

csPtr<iTrigger> celTimeoutTriggerFactory::CreateTrigger (
    iQuest* q, iCelParameterBlock* params)
{
  celTimeoutTrigger* trig = new celTimeoutTrigger (type,
  	params, timeout_par);
  return trig;
}

bool celTimeoutTriggerFactory::Load (iDocumentNode* node)
{
  timeout_par = node->GetAttributeValue ("timeout");

  if (!timeout_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.timeout",
      "'timeout' attribute is missing for the timeout trigger!");
    return false;
  }
  return true;
}

void celTimeoutTriggerFactory::SetTimeoutParameter (
	const char* timeout)
{
  timeout_par = timeout;
}

//---------------------------------------------------------------------------

celTimeoutTrigger::celTimeoutTrigger (
	celTimeoutTriggerType* type,
  	iCelParameterBlock* params,
	const char* timeout_par) : scfImplementationType (this)
{
  celTimeoutTrigger::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  const char* to = pm->ResolveParameter (params, timeout_par);
  fired = false;
  if (!to)
    timeout = 1;
  else
    sscanf (to, "%d", &timeout);
  deactivatedTicksLeft = 0;
}

celTimeoutTrigger::~celTimeoutTrigger ()
{
  DeactivateTrigger ();
}

void celTimeoutTrigger::RegisterCallback (iTriggerCallback* callback)
{
  celTimeoutTrigger::callback = callback;
}

void celTimeoutTrigger::ClearCallback ()
{
  callback = 0;
}

void celTimeoutTrigger::ActivateTrigger ()
{
  fired = false;
  type->pl->CallbackOnce (this, timeout, CEL_EVENT_PRE);
}

bool celTimeoutTrigger::Check ()
{
  if (fired)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iTrigger*)this, 0);
  }
  return fired;
}

void celTimeoutTrigger::DeactivateTrigger ()
{
  type->pl->RemoveCallbackOnce (this, CEL_EVENT_PRE);
}

void celTimeoutTrigger::Activate ()
{
  if (deactivatedTicksLeft == 0) return;
  type->pl->CallbackOnce (this, deactivatedTicksLeft, CEL_EVENT_PRE);
  deactivatedTicksLeft = 0;
}

void celTimeoutTrigger::Deactivate ()
{
  if (deactivatedTicksLeft != 0) return;
  deactivatedTicksLeft = type->pl->GetTicksLeft (this, CEL_EVENT_PRE);
  if (!deactivatedTicksLeft) deactivatedTicksLeft++;	// To prevent problems in rare case it was 0.
  type->pl->RemoveCallbackOnce (this, CEL_EVENT_PRE);
}

void celTimeoutTrigger::TickOnce ()
{
  fired = true;
  if (callback) callback->TriggerFired ((iTrigger*)this, 0);
}

bool celTimeoutTrigger::LoadAndActivateTrigger (iCelDataBuffer* databuf)
{
  uint32 tl = databuf->GetUInt32 ();
  type->pl->RemoveCallbackOnce (this, CEL_EVENT_PRE);
  type->pl->CallbackOnce (this, tl, CEL_EVENT_PRE);
  return true;
}

void celTimeoutTrigger::SaveTriggerState (iCelDataBuffer* databuf)
{
  databuf->Add (type->pl->GetTicksLeft (this, CEL_EVENT_PRE));
}

//---------------------------------------------------------------------------

