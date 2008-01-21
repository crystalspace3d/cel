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

#ifndef __CEL_PLIMP_ENTITY__
#define __CEL_PLIMP_ENTITY__

#include "csutil/csobject.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "csutil/weakrefarr.h"
#include "csutil/set.h"
#include "csutil/leakguard.h"
#include "physicallayer/entity.h"
#include "physicallayer/messaging.h"
#include "plugins/stdphyslayer/pl.h"
#include "celtool/stdmsgchannel.h"

class celPropertyClassList;
class celPlLayer;

/**
 * Implementation of iCelEntity.
 */
class celEntity : public scfImplementationExt2<celEntity,csObject,
  iCelEntity, iMessageChannel>
{
private:
  celPropertyClassList* plist;
  csRef<iCelBehaviour> behaviour;
  uint entity_ID;
  celPlLayer* pl;
  csSet<csStringID> classes;

  celMessageChannel channel;

public:
  CS_LEAKGUARD_DECLARE (celEntity);

  celEntity (celPlLayer* pl);
  virtual ~celEntity ();

  uint GetEntityID () { return entity_ID; }
  void SetEntityID (uint ID) { entity_ID = ID; }

  virtual void SetName (const char *name);
  virtual const char* GetName () const { return csObject::GetName (); }

  virtual void AddClass (csStringID cls);
  virtual void RemoveClass (csStringID cls);
  virtual bool HasClass (csStringID cls);
  virtual const csSet<csStringID>& GetClasses () const { return classes; }

  virtual iCelPropertyClassList* GetPropertyClassList ();
  virtual void SetBehaviour (iCelBehaviour* ent);
  virtual iCelBehaviour* GetBehaviour () { return behaviour; }

  // Tell all sibling property classes that a property classes
  // had been added or removed.
  void NotifySiblingPropertyClasses ();

  virtual iObject* QueryObject () { return this; }
  virtual void SetID  (uint n) { SetEntityID (n); }
  virtual uint GetID () const { return entity_ID; }

  //------- For iMessageChannel ---------------------------------------------
  virtual iMessageChannel* QueryMessageChannel ()
  {
    return static_cast<iMessageChannel*> (this);
  }
  virtual csRef<iMessageDispatcher> CreateMessageDispatcher (
      iMessageSender* sender, const char* msg_id)
  {
    return channel.CreateMessageDispatcher (sender, msg_id);
  }
  virtual void RemoveMessageDispatcher (iMessageDispatcher* msgdisp)
  {
    channel.RemoveMessageDispatcher (msgdisp);
  }
  virtual void Subscribe (iMessageReceiver* receiver, const char* mask)
  {
    channel.Subscribe (receiver, mask);
  }
  virtual void Unsubscribe (iMessageReceiver* receiver, const char* mask = 0)
  {
    channel.Unsubscribe (receiver, mask);
  }
};

/**
 * Implementation of iCelEntityList.
 */
class celEntityList : public scfImplementation1<celEntityList,iCelEntityList>
{
private:
  csRefArray<iCelEntity> entities;

public:
  celEntityList ();
  virtual ~celEntityList ();

  virtual size_t GetCount () const;
  virtual iCelEntity* Get (size_t n) const;
  virtual size_t Add (iCelEntity* obj);
  virtual bool Remove (iCelEntity* obj);
  virtual bool Remove (size_t n);
  virtual void RemoveAll ();
  virtual size_t Find (iCelEntity* obj) const;
  virtual iCelEntity* FindByName (const char *Name) const;
  virtual csPtr<iCelEntityIterator> GetIterator () const;

  class Iterator :
    public scfImplementation1<Iterator, iCelEntityIterator>
  {
  private:
    csRefArray<iCelEntity>::ConstIterator it;

  public:
    Iterator (const celEntityList* parent);

    virtual iCelEntity* Next ();
    virtual bool HasNext () const;
  };

  friend class Iterator;
};

#endif // __CEL_PLIMP_ENTITY__

