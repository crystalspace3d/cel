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
#include "propclass/camera.h"

#include "plugins/tools/quests/trig_timeout.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(Timeout)

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celTimeoutTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iTimeoutQuestTriggerFactory)
SCF_IMPLEMENT_IBASE_END

celTimeoutTriggerFactory::celTimeoutTriggerFactory (
	celTimeoutTriggerType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celTimeoutTriggerFactory::type = type;
  timeout_par = 0;
}

celTimeoutTriggerFactory::~celTimeoutTriggerFactory ()
{
  delete[] timeout_par;
  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestTrigger> celTimeoutTriggerFactory::CreateTrigger (
    const celQuestParams& params)
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

SCF_IMPLEMENT_IBASE (celTimeoutTrigger)
  SCF_IMPLEMENTS_INTERFACE (iQuestTrigger)
  SCF_IMPLEMENTS_INTERFACE (iTimerEvent)
SCF_IMPLEMENT_IBASE_END

celTimeoutTrigger::celTimeoutTrigger (
	celTimeoutTriggerType* type,
  	const celQuestParams& params,
	const char* timeout_par)
{
  SCF_CONSTRUCT_IBASE (0);
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
  SCF_DESTRUCT_IBASE ();
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

void celTimeoutTrigger::DeactivateTrigger ()
{
  timer->RemoveAllTimerEvents ();
}

bool celTimeoutTrigger::Perform (iTimerEvent* ev)
{
printf ("TIMEOUT FIRE\n"); fflush (stdout);
  if (callback) callback->TriggerFired ((iQuestTrigger*)this);
  return false;
}

//---------------------------------------------------------------------------

