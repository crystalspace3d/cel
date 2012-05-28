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

//---------------------------------------------------------------------------

celBehaviourTree::celBehaviourTree (				
	iBase* parent) : scfImplementationType (this, parent),	
	object_reg(0)											
{															
}															
bool celBehaviourTree::Initialize (					
	iObjectRegistry* object_reg)							
{									
  celBehaviourTree::object_reg = object_reg;			
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  return true;												
}

BTStatus celBehaviourTree::Execute (iCelParameterBlock* params)
{
  celBehaviourTree::params = params;
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  return BT_SUCCESS;
}

bool celBehaviourTree::AddChild (iBTNode* child)
{
  if (root_node.IsValid ())
  {
    //Tree already has a root
    return false;
  }
  else
  {
    root_node = child;
    return true;    
  }
}

void celBehaviourTree::TickEveryFrame ()
{
  root_node->Execute (params);
}
