/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2001 by Jorrit Tyberghein

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
#include "plugins/stdphyslayer/entity.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_LEAKGUARD_IMPLEMENT (celEntity);

celEntity::celEntity (celPlLayer* pl) : scfImplementationType (this)
{
  behaviour = 0;
  celEntity::pl = pl;
  channel.SetPL (pl);
  entity_ID = 0;
  positional = false;
  active = true;
  entityExistedAtBaseline = false;
}

celEntity::~celEntity ()
{
  RemoveAll ();
}

void celEntity::SetBehaviour (iCelBehaviour* newbehaviour)
{
  behaviour = newbehaviour;
}

iCelPropertyClassList* celEntity::GetPropertyClassList ()
{
  return (iCelPropertyClassList*)this;
}

void celEntity::SetName (const char *name)
{
  if (csObject::GetName ()) pl->RemoveEntityName (this);
  csObject::SetName (name);
  if (name) pl->AddEntityName (this);
}

void celEntity::NotifySiblingPropertyClasses ()
{
  size_t i;
  positional = false;
  for (i = 0 ; i < prop_classes.GetSize () ; i++)
  {
    iCelPropertyClass* pc = prop_classes[i];
    pc->PropertyClassesHaveChanged ();
    if (pc->QueryPositionInfo ())
      positional = true;
  }
}

void celEntity::AddClass (csStringID cls)
{
  if (!classes.Contains(cls))
  {
    classes.AddNoTest (cls);
    pl->EntityClassAdded(this,cls);
  }
}

void celEntity::RemoveClass (csStringID cls)
{
  if (classes.Delete (cls))
  {
    pl->EntityClassRemoved(this,cls);
  }
}

bool celEntity::HasClass (csStringID cls)
{
  return classes.In (cls);
}

class celMessageReceiverFilterWithTag : public scfImplementation1<
	celMessageReceiverFilterWithTag,iMessageReceiverFilter>
{
private:
  csString tag;

public:
  celMessageReceiverFilterWithTag (const char* tag)
    : scfImplementationType (this), tag (tag) { }
  virtual bool IsValidReceiver (iMessageReceiver* receiver)
  {
    csRef<iCelPropertyClass> pc = scfQueryInterface<iCelPropertyClass> (
	receiver);
    if (!pc) return false;
    return tag == pc->GetTag ();
  }
};

csRef<iMessageDispatcher> celEntity::CreateTaggedMessageDispatcher (
      iMessageSender* sender, const char* msg_id,
      const char* tag)
{
  csRef<iMessageReceiverFilter> filter;
  filter.AttachNew (new celMessageReceiverFilterWithTag (tag));
  return channel.CreateMessageDispatcher (sender, msg_id, filter);
}

void celEntity::Activate ()
{
  if (active) return;
  active = true;
  for (size_t i = 0 ; i < prop_classes.GetSize () ; i++)
  {
    prop_classes[i]->Activate ();
  }
}

void celEntity::Deactivate ()
{
  if (!active) return;
  active = false;
  for (size_t i = 0 ; i < prop_classes.GetSize () ; i++)
  {
    prop_classes[i]->Deactivate ();
  }
}

void celEntity::MarkBaseline ()
{
  entityExistedAtBaseline = true;
  for (size_t i = 0 ; i < prop_classes.GetSize () ; i++)
    prop_classes[i]->MarkBaseline ();
}

bool celEntity::IsModifiedSinceBaseline () const
{
  for (size_t i = 0 ; i < prop_classes.GetSize () ; i++)
    if (prop_classes[i]->IsModifiedSinceBaseline ()) return true;
  return false;
}

void celEntity::SaveModifications (iCelCompactDataBufferWriter* buf, iStringSet* strings)
{
  for (size_t i = 0 ; i < prop_classes.GetSize () ; i++)
    if (prop_classes[i]->IsModifiedSinceBaseline ())
    {
      csStringID nameID = strings->Request (prop_classes[i]->GetName ());
      buf->AddUInt32 (nameID);
      if (prop_classes[i]->GetTag () && *prop_classes[i]->GetTag ())
      {
	csStringID tagID = strings->Request (prop_classes[i]->GetTag ());
	buf->AddUInt32 (tagID);
      }
      else
      {
	buf->AddUInt32 ((uint32)csArrayItemNotFound);
      }
      prop_classes[i]->SaveModifications (buf, strings);
    }
  buf->AddUInt32 ((uint32)csArrayItemNotFound);
}

void celEntity::RestoreModifications (iCelCompactDataBufferReader* buf,
    const csHash<csString,csStringID>& strings)
{
  csStringID nameID = buf->GetUInt32 ();
  while (nameID != (csStringID)csArrayItemNotFound)
  {
    csStringID tagID = buf->GetUInt32 ();
    csString tag;
    iCelPropertyClass* pc;
    const char* name = strings.Get (nameID, (const char*)0);
    if (tagID == (csStringID)csArrayItemNotFound)
      pc = FindByName (name);
    else
      pc = FindByNameAndTag (name, strings.Get (tagID, (const char*)0));
    if (!pc)
    {
      printf ("Error finding pc '%s' on entity '%s'\n!", name, GetName ());
      fflush (stdout);
      return;
    }
    pc->RestoreModifications (buf, strings);
    nameID = buf->GetUInt32 ();
  }
}

size_t celEntity::GetCount () const
{
  return prop_classes.GetSize ();
}

iCelPropertyClass* celEntity::Get (size_t n) const
{
  CS_ASSERT ((n != csArrayItemNotFound) && n < prop_classes.GetSize ());
  iCelPropertyClass* pclass = prop_classes[n];
  return pclass;
}

size_t celEntity::Add (iCelPropertyClass* obj)
{
  size_t idx = prop_classes.Push (obj);
  obj->SetEntity (this);
  NotifySiblingPropertyClasses ();
  return idx;
}

bool celEntity::Remove (iCelPropertyClass* obj)
{
  size_t idx = prop_classes.Find (obj);
  if (idx != csArrayItemNotFound)
  {
    obj->SetEntity (0);
    prop_classes.DeleteIndex (idx);
    NotifySiblingPropertyClasses ();
    return true;
  }
  else return false;
}

bool celEntity::RemoveByInterface (scfInterfaceID scf_id, int ver)
{
  bool res = false;

  for (size_t i = 0; i < prop_classes.GetSize (); i++)
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

bool celEntity::RemoveByInterfaceAndTag (scfInterfaceID scf_id,
	int ver, const char* tag)
{
  bool res = false;

  for (size_t i = 0; i < prop_classes.GetSize (); i++)
  {
    const char* pctag = prop_classes[i]->GetTag ();
    if (!(((tag == 0 || *tag == 0) && pctag == 0) ||
		    ((tag != 0 && *tag != 0) && strcmp (tag, pctag) == 0)))
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

bool celEntity::Remove (size_t n)
{
  CS_ASSERT ((n != csArrayItemNotFound) && n < prop_classes.GetSize ());
  iCelPropertyClass* obj = prop_classes[n];
  obj->SetEntity (0);
  prop_classes.DeleteIndex (n);
  NotifySiblingPropertyClasses ();

  return true;
}

void celEntity::RemoveAll ()
{
  while (prop_classes.GetSize () > 0)
    Remove ((size_t)0);
}

size_t celEntity::Find (iCelPropertyClass* obj) const
{
  return prop_classes.Find (obj);
}

iCelPropertyClass* celEntity::FindByName (const char* name) const
{
  size_t i;
  iCelPropertyClass* found_pc = 0;
  for (i = 0 ; i < prop_classes.GetSize () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!strcmp (name, obj->GetName ()))
    {
      // We prefer to find a property class with no tag. So if we have
      // no tag we can return immediately. Otherwise we have to wait
      // until we find one with no tag.
      if (obj->GetTag () == 0)
        return obj;
      else
        found_pc = obj;
    }
  }
  return found_pc;
}

iCelPropertyClass* celEntity::FindByNameAndTag (const char* name,
	const char* tag) const
{
  size_t i;
  for (i = 0 ; i < prop_classes.GetSize () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!strcmp (name, obj->GetName ()))
    {
      if (tag == 0 || (*tag == 0 && obj->GetTag () == 0))
      {
        if (obj->GetTag () == 0)
          return obj;
      }
      else if (obj->GetTag () != 0 && !strcmp (tag, obj->GetTag ()))
      {
        return obj;
      }
    }
  }
  return 0;
}

iBase* celEntity::FindByInterface (scfInterfaceID id,
	int version) const
{
  size_t i;
  csRef<iBase> found_interf;
  for (i = 0 ; i < prop_classes.GetSize () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!obj) continue;
    void* interf = obj->QueryInterface (id, version);
    if (interf)
    {
      // We prefer property classes with no tag.
      if (obj->GetTag () == 0)
        return (iBase*)obj;
      else
        found_interf = csPtr<iBase> (obj);
    }
  }
  if (found_interf)
    found_interf->IncRef ();
  return found_interf;
}

iBase* celEntity::FindByInterfaceAndTag (scfInterfaceID id,
	int version, const char* tag) const
{
  bool tag_empty = tag == 0 || *tag == 0;
  size_t i;
  for (i = 0 ; i < prop_classes.GetSize () ; i++)
  {
    iCelPropertyClass* obj = prop_classes[i];
    if (!obj) continue;
    if (tag_empty == true && obj->GetTag () != 0)
      continue;
    if (tag_empty == false && obj->GetTag () == 0)
      continue;
    if (tag_empty == false && strcmp (obj->GetTag (), tag) != 0)
      continue;
    void* interf = obj->QueryInterface (id, version);
    if (interf)
    {
      return (iBase*)obj;
    }
  }
  return 0;
}

//---------------------------------------------------------------------------

celEntityList::celEntityList () : scfImplementationType (this)
{
}

celEntityList::~celEntityList ()
{
  RemoveAll ();
}

size_t celEntityList::GetCount () const
{
  return entities.GetSize ();
}

iCelEntity* celEntityList::Get (size_t n) const
{
  CS_ASSERT ((n != csArrayItemNotFound) && n < entities.GetSize ());
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
  while (entities.GetSize () > 0)
    Remove ((size_t) 0);
}

size_t celEntityList::Find (iCelEntity* obj) const
{
  return entities.Find (obj);
}

iCelEntity* celEntityList::FindByName (const char *Name) const
{
  size_t i;
  for (i = 0 ; i < entities.GetSize () ; i++)
  {
    iCelEntity* ent = entities[i];
    if (!strcmp (ent->GetName (), Name)) return ent;
  }
  return 0;
}

csPtr<iCelEntityIterator> celEntityList::GetIterator () const
{
  return new Iterator (this);
}

//---------------------------------------------------------------------------

celEntityList::Iterator::Iterator (const celEntityList* parent) :
  scfImplementationType (this), it (parent->entities.GetIterator ())
{
}

iCelEntity* celEntityList::Iterator::Next ()
{
  return it.Next ();
}

bool celEntityList::Iterator::HasNext () const
{
  return it.HasNext ();
}

