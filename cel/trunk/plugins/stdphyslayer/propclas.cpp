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
  SCF_DESTRUCT_IBASE ();
}

size_t celPropertyClassList::GetCount () const
{
  return prop_classes.Length ();
}

iCelPropertyClass* celPropertyClassList::Get (size_t n) const
{
  CS_ASSERT ((n != csArrayItemNotFound) && n < prop_classes.Length ());
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

bool celPropertyClassList::RemoveByInterface (scfInterfaceID scf_id, int ver)
{
  bool res = false;

  for (size_t i = 0; i < prop_classes.Length (); i++)
  {
    iBase* interface = (iBase*)prop_classes[i]->QueryInterface (scf_id, ver);
    if (!interface)
      continue;
    interface->DecRef ();	// Remove our reference.

    Remove(i);
    res = true;
    // We continue, because there may be multiple property classes of the
    // same type.
  }

  return res;
}

bool celPropertyClassList::RemoveByInterfaceAndTag (scfInterfaceID scf_id,
	int ver, const char* tag)
{
  bool res = false;

  for (size_t i = 0; i < prop_classes.Length (); i++)
  {
    const char* pctag = prop_classes[i]->GetTag ();
    if (!((tag == 0 && pctag == 0) || (tag != 0 && strcmp (tag, pctag) == 0)))
      continue;
    iBase* interface = (iBase*)prop_classes[i]->QueryInterface (scf_id, ver);
    if (!interface)
      continue;
    interface->DecRef ();	// Remove our reference.

    Remove(i);
    res = true;
    // We continue, because there may be multiple property classes of the
    // same type and tag.
  }

  return res;
}

bool celPropertyClassList::Remove (size_t n)
{
  CS_ASSERT ((n != csArrayItemNotFound) && n < prop_classes.Length ());
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
  iCelPropertyClass* found_pc = 0;
  for (i = 0 ; i < prop_classes.Length () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!strcmp (name, obj->GetName ()))
    {
      // We prefer to find a property class with no tag. So if we have
      // no tag we can return immediatelly. Otherwise we have to wait
      // until we find one with no tag.
      if (obj->GetTag () == 0)
        return obj;
      else
        found_pc = obj;
    }
  }
  return found_pc;
}

iCelPropertyClass* celPropertyClassList::FindByNameAndTag (const char* name,
	const char* tag) const
{
  size_t i;
  for (i = 0 ; i < prop_classes.Length () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (tag == 0)
    {
      if (!strcmp (name, obj->GetName ()) && obj->GetTag () == 0)
        return obj;
    }
    else
    {
      if (!strcmp (name, obj->GetName ()) && obj->GetTag () != 0 &&
    	  !strcmp (tag, obj->GetTag ()))
        return obj;
    }
  }
  return 0;
}

iBase* celPropertyClassList::FindByInterface (scfInterfaceID id,
	int version) const
{
  size_t i;
  csRef<iBase> found_interf;
  for (i = 0 ; i < prop_classes.Length () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!obj) continue;
    iBase* interf = (iBase*)(obj->QueryInterface (id, version));
    if (interf)
    {
      // We prefer property classes with no tag.
      if (obj->GetTag () == 0)
        return interf;
      else
        found_interf = csPtr<iBase> (interf);
    }
  }
  if (found_interf)
    found_interf->IncRef ();
  return found_interf;
}

iBase* celPropertyClassList::FindByInterfaceAndTag (scfInterfaceID id,
	int version, const char* tag) const
{
  size_t i;
  for (i = 0 ; i < prop_classes.Length () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!obj) continue;
    if (tag == 0 && obj->GetTag () != 0)
      continue;
    if (tag != 0 && obj->GetTag () == 0)
      continue;
    if (tag != 0 || strcmp (obj->GetTag (), tag) != 0)
      continue;
    iBase* interf = (iBase*)(obj->QueryInterface (id, version));
    if (interf)
    {
      return interf;
    }
  }
  return 0;
}

