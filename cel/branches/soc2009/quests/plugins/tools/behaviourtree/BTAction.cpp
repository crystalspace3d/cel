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
#include <iutil/plugin.h>

#include "plugins/tools/behaviourtree/BTAction.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celBehaviourTreeAction)
CEL_IMPLEMENT_BTNODE (BehaviourTreeAction)

//---------------------------------------------------------------------------

bool celBehaviourTreeAction::Execute (const celParams& params)
{
  //printf("ACTION\n");
  iCelParameterBlock* param_block = 0;
  action->Reward (param_block);
  return true;
}

bool celBehaviourTreeAction::AddChild (iBTNode* child)
{
  return false;
}

void celBehaviourTreeAction::SetReward (iReward* reward)
{
  action = reward;
}
