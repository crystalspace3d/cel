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
#include <iutil/plugin.h>

#include "plugins/tools/behaviourtree/btaction.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celBehaviourTreeAction)
CEL_IMPLEMENT_BTNODE (BehaviourTreeAction)

//---------------------------------------------------------------------------

BTStatus celBehaviourTreeAction::Execute (iCelParameterBlock* params)
{
  for (size_t i = 0; i < actions.GetSize (); i++)
    actions[i]->Reward (nullptr);
  return BT_SUCCESS;
}

bool celBehaviourTreeAction::AddChild (iBTNode* child)
{
  return false;
}

void celBehaviourTreeAction::AddReward (iReward* reward)
{
  actions.Push (reward);
}
