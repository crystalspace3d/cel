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
#include "plugins/stdphyslayer/entity.h"
#include "plugins/stdphyslayer/propclas.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_LEAKGUARD_IMPLEMENT (celEntity);

celEntity::celEntity (celPlLayer* pl) : scfImplementationType (this)
{
  plist = new celPropertyClassList (this);
  behaviour = 0;
  celEntity::pl = pl;
  channel.SetPL (pl);
  entity_ID = 0;
  positional = false;
}

celEntity::~celEntity ()
{
  delete plist;
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
  if (csObject::GetName ()) pl->RemoveEntityName (this);
  csObject::SetName (name);
  if (name) pl->AddEntityName (this);
}

void celEntity::NotifySiblingPropertyClasses ()
{
  size_t i;
  positional = false;
  for (i = 0 ; i < plist->GetCount () ; i++)
  {
    iCelPropertyClass* pc = plist->Get (i);
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

