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
#include "iutil/comp.h"
#include "csutil/randomgen.h"
#include "csutil/sysfunc.h"

#include "plugins/tools/selectors/selector_random.h"

//---------------------------------------------------------------------------


SCF_IMPLEMENT_FACTORY (celRandomSelector)
CEL_IMPLEMENT_BTNODE (RandomSelector)

//---------------------------------------------------------------------------

static csRandomGen rng (csGetTicks ());

bool celRandomSelector::Execute (iCelParameterBlock* params)
{
  int randChildIndex = rng.Get ((int) children.GetSize ());
  return children.Get(randChildIndex)->Execute(params);
}

bool celRandomSelector::AddChild (iBTNode* child)
{
  children.Push(child);
  return true;
}
