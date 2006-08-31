/*
    Copyright (C) 2004 by Jorrit Tyberghein

    This file is part of Ecksdee.

    Ecksdee is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Ecksdee is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ecksdee; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "cssysdef.h"

// CEL Includes
#include <physicallayer/entity.h>
#include <behaviourlayer/behave.h>

// HoverTest Includes
#include "hovertest.h"
#include "bl.h"
#include "behave.h"

CS_LEAKGUARD_IMPLEMENT (htBehaviourLayer);

SCF_IMPLEMENT_IBASE (htBehaviourLayer)
  SCF_IMPLEMENTS_INTERFACE (iCelBlLayer)
SCF_IMPLEMENT_IBASE_END

htBehaviourLayer::htBehaviourLayer (HoverTest* app)
{
  SCF_CONSTRUCT_IBASE (0);
  htBehaviourLayer::app = app;
}

htBehaviourLayer::~htBehaviourLayer ()
{
  SCF_DESTRUCT_IBASE ();
}

iCelBehaviour* htBehaviourLayer::CreateBehaviour (iCelEntity* entity,
	const char* name)
{
  iCelBehaviour* behave = 0;

  // @@@ This is slow! We need to use a string set here!

  if (!strcmp (name, "dynactor"))
    behave = new htBehaviourActor (this, entity);

  if (behave)
  {
    entity->SetBehaviour (behave);
    // There is now a reference in the entity. We destroy
    // our own reference here.
    behave->DecRef ();
  }
  else
  {
    app->ReportError ("Can't find behaviour '%s'!", name);
  }

  return behave;
}

