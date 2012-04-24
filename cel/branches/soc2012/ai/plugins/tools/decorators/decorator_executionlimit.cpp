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
  execution_limit = 0;
  execution_count = 0;
  return true;												
}

bool celExecutionLimitDecorator::Execute (iCelParameterBlock* params)
{
  //printf("Execution Limit Decorator\n");

  if (execution_limit == 0)
  {
    csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
      (object_reg, "cel.parameters.manager");

    const char* s = pm->ResolveParameter(params, execution_limit_param);
    execution_limit = atoi (s);
  }

  
  if(execution_count >= execution_limit)
  {
    return false;
  }
  execution_count++;
  return child_node->Execute(params);
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
