/*
    Crystal Space Entity Layer
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
#include <iutil/comp.h>

#include "plugins/tools/behaviourtree/condition_triggerfired.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celTriggerFiredCondition)

//---------------------------------------------------------------------------

celTriggerFiredCondition::celTriggerFiredCondition (
	iBase* parent) : scfImplementationType (this, parent),
  object_reg(0), triggerFired (false), fireOnce (false)
{
}

bool celTriggerFiredCondition::Initialize (
	iObjectRegistry* object_reg)
{
  celTriggerFiredCondition::object_reg = object_reg;
  return true;
}

BTStatus celTriggerFiredCondition::GetStatus ()
{
  return status;
}

void celTriggerFiredCondition::SetStatus (BTStatus newStatus)
{
  status = newStatus;
}

BTStatus celTriggerFiredCondition::Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack)
{
  if (!triggerFired)
  {
    status = BT_FAIL_CLEAN;
	return status;
  }

  if (fireOnce) 
    triggerFired = false;

  status = BT_SUCCESS;
  return status;
}

bool celTriggerFiredCondition::AddChild (iBTNode* child)
{
  return false;
}

void celTriggerFiredCondition::SetTrigger (iTrigger* trigger)
{
  triggerFired = false;
  trigger->ActivateTrigger ();
  celTriggerFiredCondition::trigger = trigger;
  trigger->RegisterCallback (this);
}

void celTriggerFiredCondition::SetFireOnce (bool once)
{
  fireOnce = once;
}

void celTriggerFiredCondition::TriggerFired (iTrigger* trigger, iCelParameterBlock* params)
{
  triggerFired = true;
}
