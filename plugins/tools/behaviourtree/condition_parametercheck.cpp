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

//---------------------------------------------------------------------------

celParameterCheckCondition::celParameterCheckCondition (
	iBase* parent) : scfImplementationType (this, parent),
  object_reg(0)
{
}

bool celParameterCheckCondition::Initialize (
	iObjectRegistry* object_reg)
{
  celParameterCheckCondition::object_reg = object_reg;
  status = BT_NOT_STARTED;  
  name = "un-named node";
  parameter = "";
  value = "";
  return true;
}

BTStatus celParameterCheckCondition::GetStatus ()
{
  return status;
}

void celParameterCheckCondition::SetStatus (BTStatus newStatus)
{
  status = newStatus;
}

void celParameterCheckCondition::SetName(csString nodeName)
{
  name = nodeName;
}

BTStatus celParameterCheckCondition::Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack)
{
  if (parameter == "" || value == "")
  {
    csReport(object_reg, CS_REPORTER_SEVERITY_NOTIFY,
        "cel.behaviourtree.parametercheck",
        "Parameter or Value not set for: %s", name.GetData());

    status = BT_UNEXPECTED_ERROR;
  }
  else
  {
    csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
      (object_reg, "cel.parameters.manager");

    if (value == pm->ResolveParameter(params, parameter)){
      status = BT_SUCCESS;
    } else {
      status = BT_FAIL_CLEAN;
    }
  }

  return status;
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
