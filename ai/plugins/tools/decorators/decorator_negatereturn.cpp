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

#include "plugins/tools/decorators/decorator_negatereturn.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celNegateReturnDecorator)
CEL_IMPLEMENT_BTNODE (NegateReturnDecorator)

//---------------------------------------------------------------------------

BTStatus celNegateReturnDecorator::Execute (iCelParameterBlock* params)
{
  //printf("Negate Return Decorator\n");
  BTStatus childStatus = children.Get(0)->Execute(params);
  if (childStatus = BT_SUCCESS) {
    return BT_FAIL_CLEAN;
  } else if (childStatus = BT_FAIL_CLEAN) {
    return BT_SUCCESS;
  } else {
    return childStatus; // BT_UNEXPECTED_ERROR or BT_RUNNING
  }
}

bool celNegateReturnDecorator::AddChild (iBTNode* child)
{
  if (children.IsEmpty())
  {
    children.Push(child);
    return true;
  }
  else
  {
    return false;
  }
}
