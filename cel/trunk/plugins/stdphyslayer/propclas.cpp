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

int celPropertyClassList::GetCount () const
{
  return prop_classes.Length ();
}

iCelPropertyClass* celPropertyClassList::Get (int n) const
{
  CS_ASSERT (n >= 0 && n < prop_classes.Length ());
  iCelPropertyClass* pclass = prop_classes[n];
  return pclass;
}

int celPropertyClassList::Add (iCelPropertyClass* obj)
{
  int idx = prop_classes.Push (obj);
  obj->SetEntity (parent_entity);
  DG_LINK (this, obj);
  return idx;
}

bool celPropertyClassList::Remove (iCelPropertyClass* obj)
{
  int idx = prop_classes.Find (obj);
  if (idx != -1)
  {
    DG_UNLINK (this, obj);
    obj->SetEntity (0);
    prop_classes.Delete (idx);
    return true;
  }
  else return false;
}

bool celPropertyClassList::Remove (int n)
{
  CS_ASSERT (n >= 0 && n < prop_classes.Length ());
  DG_UNLINK (this, prop_classes[n]);
  prop_classes.Delete (n);

  return true;
}

void celPropertyClassList::RemoveAll ()
{
  while (prop_classes.Length () > 0)
    Remove (0);
}

int celPropertyClassList::Find (iCelPropertyClass* obj) const
{
  return prop_classes.Find (obj);
}

iCelPropertyClass* celPropertyClassList::FindByName (const char* name) const
{
  int i;
  for (i = 0 ; i < prop_classes.Length () ; i++)
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
  int i;
  for (i = 0 ; i < prop_classes.Length () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!obj) continue;
    iBase* interf = (iBase*)(obj->QueryInterface (id, version));
    if (interf) return interf;
  }
  return 0;
}

