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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "cssysdef.h"
#include "plugins/behaviourlayer/test/bl.h"
#include "plugins/behaviourlayer/test/behave.h"
#include "include/physicallayer/entity.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celBlTest)

celBlTest::celBlTest (iBase* parent) : scfImplementationType (this, parent)
{
}

celBlTest::~celBlTest ()
{
}

bool celBlTest::Initialize (iObjectRegistry* object_reg)
{
  celBlTest::object_reg = object_reg;
  return true;
}

iCelBehaviour* celBlTest::CreateBehaviour (iCelEntity* entity, const char* name)
{
  celBehaviourGeneral* ent = 0;
  if (!strcmp (name, "printer"))
    ent = new celBehaviourPrinter (entity, object_reg);
  else if (!strcmp (name, "room"))
    ent = new celBehaviourRoom (entity, object_reg);
  else if (!strcmp (name, "quest"))
    ent = new celBehaviourQuest (entity, object_reg);
  else if (!strcmp (name, "box"))
    ent = new celBehaviourBox (entity, object_reg);
  else if (!strcmp (name, "actor"))
    ent = new celBehaviourGeneral (entity, object_reg);
  else if (!strcmp (name, "dynactor"))
    ent = new celBehaviourGeneral (entity, object_reg);
  else if (!strcmp (name, "wheeled"))
    ent = new celBehaviourWheeled (entity, object_reg);
  if (ent)
  {
    ent->SetName (name);
    ent->SetBehaviourLayer (this);
  }
  entity->SetBehaviour (ent);
  return ent;
}


