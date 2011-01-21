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

#include "plugins/tools/behaviourtree/condition_parametercheck.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celParameterCheckCondition)
CEL_IMPLEMENT_BTNODE (ParameterCheckCondition)

//---------------------------------------------------------------------------

bool celParameterCheckCondition::Execute (const celParams& params)
{
  //printf("CONDITION: Parameter Check\n");

  csRef<iPluginManager> plugin_mgr = 
    csQueryRegistry<iPluginManager> (object_reg);
  csRef<iParameterManager> pm = csLoadPlugin<iParameterManager> 
    (plugin_mgr, "cel.parameters.manager");

  return (value == pm->ResolveParameter(params, parameter));
}

bool celParameterCheckCondition::AddChild (iBTNode* child)
{
  return false;
}

void celParameterCheckCondition::SetParameter (const char* param)
{
  parameter = param;
}

void celParameterCheckCondition::SetValue (const char* value)
{
  celParameterCheckCondition::value = value;
}
