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

#include "plugins/tools/decorators/decorator_loop.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celLoopDecorator)

//---------------------------------------------------------------------------

celLoopDecorator::celLoopDecorator (                               
  iBase* parent) : scfImplementationType (this, parent), 
  object_reg(0)                                                                                   
{                                                                                                                       
}

bool celLoopDecorator::Initialize (                                   
  iObjectRegistry* object_reg)                                                   
{                                                                       
  celLoopDecorator::object_reg = object_reg;                 
  status = BT_NOT_STARTED;
  loop_limit = 0;
  return true;                                                                                         
}

BTStatus celLoopDecorator::GetStatus ()
{
  return status;
}

void celLoopDecorator::SetStatus (BTStatus newStatus)
{
  status = newStatus;
}

BTStatus celLoopDecorator::Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack)
{
  if (status == BT_NOT_STARTED)
  {
    loop_count = 0;

    if (loop_limit == 0)
    {
      csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
        (object_reg, "cel.parameters.manager");
      const char* s = pm->ResolveParameter (params, loop_limit_param);
      loop_limit = atoi (s);
    }

    BTStack->Push(child_node);
    child_node->SetStatus(BT_NOT_STARTED);  // In case child has been run before

    status = BT_RUNNING;
  }
  
  BTStatus child_status = child_node->GetStatus();

  if (child_status == BT_SUCCESS)
  {
    loop_count++;
	if (loop_count < loop_limit)
    {
      BTStack->Push(child_node);
      child_node->SetStatus(BT_NOT_STARTED);  // In case child has been run before
    }
    else
    {
      // If the loop has executed more times than the limit 
      // without failing, it has succeeded
      status = BT_SUCCESS;
    } 
  }
  else if (child_status == BT_FAIL_CLEAN ||
	  child_status == BT_UNEXPECTED_ERROR)
  {
    // If a child fails, the loop node fails
    status = child_status;
  }
  return status;
}

bool celLoopDecorator::AddChild (iBTNode* child)
{
  if (child_node.IsValid())
  {
    //Decorator already has child
    return false;
  }
  else
  {
    child_node = child;
    return true;    
  }
}

void celLoopDecorator::SetLoopLimit (const char* limit)
{
  loop_limit_param = limit;
}
