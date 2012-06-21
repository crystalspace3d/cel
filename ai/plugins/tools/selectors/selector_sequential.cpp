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

#include "plugins/tools/selectors/selector_sequential.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celSequentialSelector)
CEL_IMPLEMENT_BTNODE (SequentialSelector)

//---------------------------------------------------------------------------

BTStatus celSequentialSelector::Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack)
{
  if (status == BT_NOT_STARTED)
  {
    noOfChildren = children.GetSize();
	  if (noOfChildren == 0)
	  {
      csReport(object_reg, CS_REPORTER_SEVERITY_NOTIFY,
          "cel.selectors.sequential",
          "No children nodes specified for: %s", name.GetData());

      status = BT_UNEXPECTED_ERROR;
	  }
	  else
	  {
	    // On first execution push first child to top of stack and initialise other local variables
      currentChild = 0;
	    BTStack->Push(children.Get(currentChild));
      children.Get(currentChild)->SetStatus(BT_NOT_STARTED);  // In case child has been run before
	    status = BT_RUNNING;
	  }
   }
  else
  {
    BTStatus child_status = children.Get(currentChild)->GetStatus();

    if (child_status == BT_SUCCESS)
    {
	    // If child has succeeded, select next child
      currentChild++;

      if (currentChild < noOfChildren)
      {
	      // If currentChild is a valid index for a child, push it to the top of the stack
        BTStack->Push(children.Get(currentChild));
        children.Get(currentChild)->SetStatus(BT_NOT_STARTED);  // In case child has been run before
	    }
	    else
	    {
	      // If we have tried all children, then selector has succeeded (as all children must have too)
	      status = BT_SUCCESS;
	    }
    }
    else if (child_status == BT_FAIL_CLEAN || child_status == BT_UNEXPECTED_ERROR) 
    {
      // If child has failed, then so has selector
	    status = child_status;
    }
  }

  return status;
}

bool celSequentialSelector::AddChild (iBTNode* child)
{
	children.Push(child);
	return true;
}
