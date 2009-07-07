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
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/persist.h"
#include "propclass/camera.h"

#include "plugins/tools/triggers/trig_timeout.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celTimeoutTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE_NEW(Timeout)

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
    const celParams& params)
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
  	const celParams& params,
	const char* timeout_par) : scfImplementationType (this)
{
  celTimeoutTrigger::type = type;
  timer = new csEventTimer (type->object_reg);
  //csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  csRef<iPluginManager> plugin_mgr = 
   csQueryRegistry<iPluginManager> (type->object_reg);
  pm = csLoadPlugin<iParameterManager> (plugin_mgr,
    "cel.parameters.manager");


  const char* to = pm->ResolveParameter (params, timeout_par);
  fired = false;
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
  timer->RemoveAllTimerEvents ();
  timer->AddTimerEvent ((iTimerEvent*)this, timeout);
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
  timer->RemoveAllTimerEvents ();
}

bool celTimeoutTrigger::Perform (iTimerEvent* ev)
{
  fired = true;
  if (callback) callback->TriggerFired ((iTrigger*)this, 0);
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

