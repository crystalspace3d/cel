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

#include "plugins/tools/decorators/decorator_executionlimit.h"

//---------------------------------------------------------------------------

//CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celExecutionLimitDecorator)

//---------------------------------------------------------------------------

celExecutionLimitDecorator::celExecutionLimitDecorator (                               
  iBase* parent) : scfImplementationType (this, parent), 
  object_reg(0)                                                                                   
{                                                                                                                       
}

bool celExecutionLimitDecorator::Initialize (                                   
  iObjectRegistry* object_reg)                                                   
{                                                                       
  celExecutionLimitDecorator::object_reg = object_reg;
  status = BT_NOT_STARTED;  
  name = "un-named node";               
  execution_limit = 0;
  execution_count = 0;
  return true;                                                                                         
}

BTStatus celExecutionLimitDecorator::GetStatus ()
{
  return status;
}

void celExecutionLimitDecorator::SetStatus (BTStatus newStatus)
{
  status = newStatus;
}

void celExecutionLimitDecorator::SetName(csString nodeName)
{
  name = nodeName;
}

BTStatus celExecutionLimitDecorator::Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack)
{
  // On first execution
  if (status == BT_NOT_STARTED)
  {	
    if (!child_node.IsValid())
    {
      csReport(object_reg, CS_REPORTER_SEVERITY_NOTIFY,
          "cel.decorators.executionlimit",
          "No child node specified for: %s", name.GetData());

      status = BT_UNEXPECTED_ERROR;
    }
    else
    {
	    if (execution_limit == 0)
      {
        csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
          (object_reg, "cel.parameters.manager");

        const char* s = pm->ResolveParameter(params, execution_limit_param);
        if (s)
        {
          execution_limit = atoi (s);
        }
      }

      if (execution_limit == 0)
      {
        //If execution limit still 0, it has not been set - probably an error
        csReport(object_reg, CS_REPORTER_SEVERITY_NOTIFY,
            "cel.decorators.executionlimit",
            "Execution limit not set for: %s", name.GetData());

        status = BT_UNEXPECTED_ERROR;
      }
      else
      {
        if(execution_count >= execution_limit)
        {
          // child has reached execution limits 
	        status = BT_FAIL_CLEAN;
        }
        else
        {
          // execute child and count execution
          execution_count++;
          BTStack->Push(child_node);
          child_node->SetStatus(BT_NOT_STARTED);  // In case child has been run before
          status = BT_RUNNING;
        }
      }  
    }
  }
  else
  {
    BTStatus child_status = child_node->GetStatus();

    if (child_status == BT_UNEXPECTED_ERROR)
    {
      status = child_status;
    }
    else if (child_status == BT_SUCCESS || child_status == BT_FAIL_CLEAN)
    {
      status = BT_SUCCESS;
    }
  }

  return status;
}

bool celExecutionLimitDecorator::AddChild (iBTNode* child)
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

void celExecutionLimitDecorator::SetExecutionLimit (const char* limit)
{
  execution_limit_param = limit;
}
