/*
    Copyright (C) 2006 by Jorrit Tyberghein

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

// CEL Includes
#include <physicallayer/entity.h>
#include <behaviourlayer/behave.h>

// EvovleCraft Includes
#include "evolvecraft.h"
#include "bl.h"
#include "behave.h"

CS_LEAKGUARD_IMPLEMENT (htBehaviourLayer);

htBehaviourLayer::htBehaviourLayer (HoverTest* app, iCelPlLayer* pl) :
  scfImplementationType (this)
{
  htBehaviourLayer::app = app;
  htBehaviourLayer::pl = pl;
}

htBehaviourLayer::~htBehaviourLayer ()
{
}

iCelBehaviour* htBehaviourLayer::CreateBehaviour (iCelEntity* entity,
	const char* name)
{
  iCelBehaviour* behave = 0;

  // @@@ This is slow! We need to use a string set here!

  if (!strcmp (name, "dynactor"))
    behave = new htBehaviourActor (this, entity, pl);

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

