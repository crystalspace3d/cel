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

#include "plugins/tools/selectors/selector_default.h"

//---------------------------------------------------------------------------

//CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celDefaultSelector)
CEL_IMPLEMENT_BTNODE (DefaultSelector)

//---------------------------------------------------------------------------

bool celDefaultSelector::Execute (iCelParameterBlock* params)
{
  //printf("DEFAULT SELECTOR\n");

  int noOfChildren = children.GetSize();
  for (int i = 0; i < noOfChildren; i++)
  {	
    if (children.Get(i)->Execute(params))
      return true;
  }
  return false;
}

bool celDefaultSelector::AddChild (iBTNode* child)
{
	children.Push(child);
	return true;
}
