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
  loop_limit = 0;
  return true;												
}

bool celLoopDecorator::Execute (const celParams& params)
{
  if (loop_limit == 0)
  {
    csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (object_reg);
    csRef<iParameterManager> pm = csLoadPlugin<iParameterManager> 
      (plugin_mgr, "cel.parameters.manager");

	loop_limit = atoi (pm->ResolveParameter(params, loop_limit_param));
  }

  for (int i = 0; i < loop_limit; i++)
  {
    //printf("Loop Decorator, iteration = %i\n", i);
    if(!child_node->Execute(params))
	{
		return false;
	}
  }
  return true;
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
