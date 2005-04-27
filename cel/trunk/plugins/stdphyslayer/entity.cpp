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
#include "csutil/util.h"
#include "csutil/debug.h"
#include "plugins/stdphyslayer/entity.h"
#include "plugins/stdphyslayer/propclas.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celEntity)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iCelEntity)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celEntity::CelEntity)
  SCF_IMPLEMENTS_INTERFACE (iCelEntity)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

CS_LEAKGUARD_IMPLEMENT (celEntity);

celEntity::celEntity (celPlLayer* pl)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiCelEntity);
  plist = new celPropertyClassList (&scfiCelEntity);
  behaviour = 0;
  celEntity::pl = pl;
  entity_ID = 0;
}

celEntity::~celEntity ()
{
  delete plist;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiCelEntity);
}

void celEntity::SetBehaviour (iCelBehaviour* newbehaviour)
{
  behaviour = newbehaviour;
}

iCelPropertyClassList* celEntity::GetPropertyClassList ()
{
  return (iCelPropertyClassList*)plist;
}

void celEntity::SetName (const char *name)
{
  if (GetName ()) pl->RemoveEntityName (this);
  csObject::SetName (name);
  if (name) pl->AddEntityName (this);
}

void celEntity::NotifySiblingPropertyClasses ()
{
  size_t i;
  for (i = 0 ; i < plist->GetCount () ; i++)
  {
    iCelPropertyClass* pc = plist->Get (i);
    pc->PropertyClassesHaveChanged ();
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celEntityList)
  SCF_IMPLEMENTS_INTERFACE (iCelEntityList)
SCF_IMPLEMENT_IBASE_END

celEntityList::celEntityList ()
{
  SCF_CONSTRUCT_IBASE (0);
}

celEntityList::~celEntityList ()
{
  RemoveAll ();
  SCF_DESTRUCT_IBASE ();
}

size_t celEntityList::GetCount () const
{
  return entities.Length ();
}

iCelEntity* celEntityList::Get (size_t n) const
{
  CS_ASSERT ((n != csArrayItemNotFound) && n < entities.Length ());
  return entities[n];
}

size_t celEntityList::Add (iCelEntity* obj)
{
  return entities.Push (obj);
}

bool celEntityList::Remove (iCelEntity* obj)
{
  size_t idx = entities.Find (obj);
  if (idx != csArrayItemNotFound)
  {
    entities.DeleteIndex (idx);
    return true;
  }
  return false;
}

bool celEntityList::Remove (size_t n)
{
  iCelEntity* ent = Get (n);
  (void)ent;
  entities.DeleteIndex (n);
  return true;
}

void celEntityList::RemoveAll ()
{
  while (entities.Length () > 0)
    Remove ((size_t) 0);
}

size_t celEntityList::Find (iCelEntity* obj) const
{
  return entities.Find (obj);
}

iCelEntity* celEntityList::FindByName (const char *Name) const
{
  size_t i;
  for (i = 0 ; i < entities.Length () ; i++)
  {
    iCelEntity* ent = entities[i];
    if (!strcmp (ent->GetName (), Name)) return ent;
  }
  return 0;
}

//---------------------------------------------------------------------------

