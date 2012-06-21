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

#include "plugins/tools/behaviourtree/behaviourtree.h"

//---------------------------------------------------------------------------

//CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celBehaviourTree)
CEL_IMPLEMENT_BTNODE (BehaviourTree)

//---------------------------------------------------------------------------

BTStatus celBehaviourTree::Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack)
{
  celBehaviourTree::params = params;		
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  status = BT_RUNNING;
  return status;
}

bool celBehaviourTree::AddChild (iBTNode* child)
{
  if (stack.IsEmpty())
  {	
	  stack.Push(child);
    return true; 
  }
  else
  {
    //Tree already has a root
    return false;   
  }
}

void celBehaviourTree::TickEveryFrame ()
{
  // Execute current node in tree
  BTStatus top_status = stack.Top()->Execute (params, &stack);
  
  if (top_status == BT_SUCCESS ||
	  top_status == BT_FAIL_CLEAN ||
	  top_status == BT_UNEXPECTED_ERROR)
  {
	  // If node has completed,reset it and remove it from the stack
    stack.Pop();

	  if (stack.IsEmpty())
	  {
      // If tree has completed, stop calling this method
	    pl->RemoveCallbackEveryFrame((iCelTimerListener*)this, CEL_EVENT_PRE);

	    // Note that tree has completed
	    status = top_status;

      if (status == BT_UNEXPECTED_ERROR)
      {
        csReport(object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cel.behaviourtree.root",
            "Behaviour tree has exited due to an unhandled unexpected error");
      }
	  }
  }
}
