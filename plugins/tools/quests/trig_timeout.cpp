/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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
#include "physicallayer/persist.h"
#include "propclass/camera.h"

#include "plugins/tools/quests/trig_timeout.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(Timeout)

//---------------------------------------------------------------------------

celTimeoutTriggerFactory::celTimeoutTriggerFactory (
	celTimeoutTriggerType* type) : scfImplementationType (this)
{
  celTimeoutTriggerFactory::type = type;
  timeout_par = 0;
}

celTimeoutTriggerFactory::~celTimeoutTriggerFactory ()
{
  delete[] timeout_par;
}

csPtr<iQuestTrigger> celTimeoutTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  celTimeoutTrigger* trig = new celTimeoutTrigger (type,
  	params, timeout_par);
  return trig;
}

bool celTimeoutTriggerFactory::Load (iDocumentNode* node)
{
  delete[] timeout_par; timeout_par = 0;
  timeout_par = csStrNew (node->GetAttributeValue ("timeout"));

  if (!timeout_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.timeout",
      "'timeout' attribute is missing for the timeout trigger!");
    return false;
  }
  return true;
}

void celTimeoutTriggerFactory::SetTimeoutParameter (
	const char* timeout)
{
  if (timeout == timeout_par) 
    return;

  delete[] timeout_par;
  timeout_par = csStrNew (timeout);
}

//---------------------------------------------------------------------------

celTimeoutTrigger::celTimeoutTrigger (
	celTimeoutTriggerType* type,
  	const celQuestParams& params,
	const char* timeout_par) : scfImplementationType (this)
{
  celTimeoutTrigger::type = type;
  timer = new csEventTimer (type->object_reg);
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  const char* to = qm->ResolveParameter (params, timeout_par);
  if (!to)
    timeout = 1;
  else
    sscanf (to, "%d", &timeout);
}

celTimeoutTrigger::~celTimeoutTrigger ()
{
  DeactivateTrigger ();
  delete timer;
}

void celTimeoutTrigger::RegisterCallback (iQuestTriggerCallback* callback)
{
  celTimeoutTrigger::callback = callback;
}

void celTimeoutTrigger::ClearCallback ()
{
  callback = 0;
}

void celTimeoutTrigger::ActivateTrigger ()
{
  timer->RemoveAllTimerEvents ();
  timer->AddTimerEvent ((iTimerEvent*)this, timeout);
}

bool celTimeoutTrigger::Check ()
{
  return false;
}

void celTimeoutTrigger::DeactivateTrigger ()
{
  timer->RemoveAllTimerEvents ();
}

bool celTimeoutTrigger::Perform (iTimerEvent* ev)
{
  if (callback) callback->TriggerFired ((iQuestTrigger*)this);
  return false;
}

bool celTimeoutTrigger::LoadAndActivateTrigger (iCelDataBuffer* databuf)
{
  uint32 tl = databuf->GetUInt32 ();
  timer->RemoveAllTimerEvents ();
  timer->AddTimerEvent ((iTimerEvent*)this, tl);
  return true;
}

void celTimeoutTrigger::SaveTriggerState (iCelDataBuffer* databuf)
{
  databuf->Add ((uint32)timer->GetTimeLeft (0));
}

//---------------------------------------------------------------------------

