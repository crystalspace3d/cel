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
#include "plimp/pl.h"
#include "plimp/entity.h"
#include "plimp/message.h"
#include "pl/propfact.h"
#include "csutil/csobject.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPlLayer)

SCF_EXPORT_CLASS_TABLE (plimp)
  SCF_EXPORT_CLASS (celPlLayer, "cel.physicallayer", "CEL Physical Layer")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPlLayer)
  SCF_IMPLEMENTS_INTERFACE (iCelPlLayer)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPlLayer::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPlLayer::celPlLayer (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPlLayer::~celPlLayer ()
{
  int i;
  for (i = 0 ; i < pf_list.Length () ; i++)
  {
    iCelPropertyClassFactory* pf = (iCelPropertyClassFactory*)pf_list[i];
    pf->DecRef ();
  }
}

bool celPlLayer::Initialize (iObjectRegistry* /*object_reg*/)
{
  return true;
}

iCelEntity* celPlLayer::CreateEntity ()
{
  celEntity* entity = new celEntity ();
  return entity;
}

iCelMessage* celPlLayer::CreateMessage (const char* msg_string, ...)
{
  va_list arg;
  va_start (arg, msg_string);
  celMessage* msg = new celMessage (msg_string, arg);
  va_end (arg);
  return msg;
}

// Class which is used to attach to an iObject so that
// we can find the iCelEntity again.

SCF_VERSION (celEntityFinder, 0, 0, 1);
class celEntityFinder : public csObject
{
private:
  iCelEntity* entity;

public:
  celEntityFinder (iCelEntity* entity)
  {
    celEntityFinder::entity = entity;
  }
  virtual ~celEntityFinder ()
  {
    printf ("Remove entity finder\n");
  }
  iCelEntity* GetEntity () const { return entity; }
  SCF_DECLARE_IBASE_EXT (csObject);
};

SCF_IMPLEMENT_IBASE_EXT (celEntityFinder)
  SCF_IMPLEMENTS_INTERFACE (celEntityFinder)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_DECLARE_FAST_INTERFACE (celEntityFinder)

void celPlLayer::AttachEntity (iObject* object, iCelEntity* entity)
{
  iCelEntity* old_entity = FindAttachedEntity (object);
  if (old_entity == entity) return;
  if (old_entity != NULL) UnattachEntity (object, old_entity);
  celEntityFinder* cef = new celEntityFinder (entity);
  iObject* cef_obj = SCF_QUERY_INTERFACE_FAST (cef, iObject);
  object->ObjAdd (cef_obj);
  cef_obj->DecRef ();
  cef->DecRef ();
}

void celPlLayer::UnattachEntity (iObject* object, iCelEntity* entity)
{
  celEntityFinder* cef = CS_GET_CHILD_OBJECT_FAST (object, celEntityFinder);
  if (cef)
  {
    cef->DecRef ();
    if (cef->GetEntity () != entity) return;
    iObject* cef_obj = SCF_QUERY_INTERFACE_FAST (cef, iObject);
    object->ObjRemove (cef_obj);
    cef_obj->DecRef ();
  }
}

iCelEntity* celPlLayer::FindAttachedEntity (iObject* object)
{
  celEntityFinder* cef = CS_GET_CHILD_OBJECT_FAST (object, celEntityFinder);
  if (cef)
  {
    cef->DecRef ();
    return cef->GetEntity ();
  }
  return NULL;
}

void celPlLayer::RegisterPropertyClassFactory (iCelPropertyClassFactory* pf)
{
  if (pf_list.Find (pf) != -1) return;
  pf_list.Push (pf);
  pf->IncRef ();
}

void celPlLayer::UnregisterPropertyClassFactory (
  	iCelPropertyClassFactory* pf)
{
  int idx = pf_list.Find (pf);
  if (idx == -1) return;
  pf_list.Delete (idx);
  pf->DecRef ();
}

int celPlLayer::GetPropertyClassFactoryCount () const
{
  return pf_list.Length ();
}

iCelPropertyClassFactory* celPlLayer::GetPropertyClassFactory (int idx) const
{
  CS_ASSERT (idx >= 0 && idx < pf_list.Length ());
  iCelPropertyClassFactory* pf = (iCelPropertyClassFactory*)pf_list[idx];
  return pf;
}

iCelPropertyClassFactory* celPlLayer::FindPropertyClassFactory (
  	const char* name) const
{
  int i;
  for (i = 0 ; i < pf_list.Length () ; i++)
  {
    iCelPropertyClassFactory* pf = (iCelPropertyClassFactory*)pf_list[i];
    if (!strcmp (pf->GetName (), name))
      return pf;
  }
  return NULL;
}

