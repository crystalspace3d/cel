/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein

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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "bltest/bl.h"
#include "bltest/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celBlTest)

SCF_EXPORT_CLASS_TABLE (bltest)
  SCF_EXPORT_CLASS (celBlTest, "cel.behaviourlayer.test",
  	"CEL Test Behaviour Layer")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celBlTest)
  SCF_IMPLEMENTS_INTERFACE (iCelBlLayer)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBlTest::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celBlTest::celBlTest (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celBlTest::~celBlTest ()
{
}

bool celBlTest::Initialize (iObjectRegistry* /*object_reg*/)
{
  return true;
}

iCelBehaviour* celBlTest::CreateBehaviour (iCelEntity* entity, const char* name)
{
  if (!strcmp (name, "printer"))
  {
    iCelBehaviour* ent = new celBehaviourPrinter (entity);
    return ent;
  }
  else if (!strcmp (name, "room"))
  {
    iCelBehaviour* ent = new celBehaviourRoom (entity);
    return ent;
  }
  return NULL;
}


