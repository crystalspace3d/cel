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
#include <iutil/comp.h>

#include "plugins/tools/behaviourtree/condition_triggerfired.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celTriggerFiredCondition)

//---------------------------------------------------------------------------

celTriggerFiredCondition::celTriggerFiredCondition (				
	iBase* parent) : scfImplementationType (this, parent),	
	object_reg(0)											
{															
}															
bool celTriggerFiredCondition::Initialize (					
	iObjectRegistry* object_reg)							
{									
  celTriggerFiredCondition::object_reg = object_reg;			
  trigger_fired = false;
  return true;												
}

bool celTriggerFiredCondition::Execute (const celParams& params)
{
  //printf("CONDITION: Trigger Fired Check\n");
  return trigger_fired;
}

bool celTriggerFiredCondition::AddChild (iBTNode* child)
{
  return false;
}

void celTriggerFiredCondition::SetTrigger (iTrigger* trigger)
{
  celTriggerFiredCondition::trigger = trigger;
  trigger->RegisterCallback (this);
}

void celTriggerFiredCondition::TriggerFired (iTrigger* trigger, iCelParameterBlock* params)
{
  trigger_fired = true;
}