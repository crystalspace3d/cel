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
#include "plugins/stdphyslayer/propclas.h"
#include "plugins/stdphyslayer/entity.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPropertyClassList)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassList)
SCF_IMPLEMENT_IBASE_END

celPropertyClassList::celPropertyClassList (iCelEntity* parent_entity)
{
  SCF_CONSTRUCT_IBASE (0);
  celPropertyClassList::parent_entity = parent_entity;
  DG_ADDI (this, "celPCList()");
}

celPropertyClassList::~celPropertyClassList ()
{
  RemoveAll ();
  DG_REM (this);
}

size_t celPropertyClassList::GetCount () const
{
  return prop_classes.Length ();
}

iCelPropertyClass* celPropertyClassList::Get (size_t n) const
{
  CS_ASSERT ((n != csArrayItemNotFound) && n < (size_t)prop_classes.Length ());
  iCelPropertyClass* pclass = prop_classes[n];
  return pclass;
}

size_t celPropertyClassList::Add (iCelPropertyClass* obj)
{
  size_t idx = prop_classes.Push (obj);
  obj->SetEntity (parent_entity);
  ((celEntity::CelEntity*)parent_entity)->GetCelEntity ()
	  ->NotifySiblingPropertyClasses ();
  DG_LINK (this, obj);
  return idx;
}

bool celPropertyClassList::Remove (iCelPropertyClass* obj)
{
  size_t idx = prop_classes.Find (obj);
  if (idx != csArrayItemNotFound)
  {
    DG_UNLINK (this, obj);
    obj->SetEntity (0);
    prop_classes.DeleteIndex (idx);
    ((celEntity::CelEntity*)parent_entity)->GetCelEntity ()
	  ->NotifySiblingPropertyClasses ();
    return true;
  }
  else return false;
}

bool celPropertyClassList::Remove (size_t n)
{
  CS_ASSERT ((n != csArrayItemNotFound) && n < (size_t)prop_classes.Length ());
  DG_UNLINK (this, prop_classes[n]);
  prop_classes.DeleteIndex (n);
  ((celEntity::CelEntity*)parent_entity)->GetCelEntity ()
	  ->NotifySiblingPropertyClasses ();

  return true;
}

void celPropertyClassList::RemoveAll ()
{
  while (prop_classes.Length () > 0)
    Remove ((size_t)0);
}

size_t celPropertyClassList::Find (iCelPropertyClass* obj) const
{
  return prop_classes.Find (obj);
}

iCelPropertyClass* celPropertyClassList::FindByName (const char* name) const
{
  size_t i;
  for (i = 0 ; i < (size_t)prop_classes.Length () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!strcmp (obj->GetName (), name))
    {
      return obj;
    }
  }
  return 0;
}

iBase* celPropertyClassList::FindByInterface (scfInterfaceID id,
	int version) const
{
  size_t i;
  for (i = 0 ; i < (size_t)prop_classes.Length () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!obj) continue;
    iBase* interf = (iBase*)(obj->QueryInterface (id, version));
    if (interf) return interf;
  }
  return 0;
}

