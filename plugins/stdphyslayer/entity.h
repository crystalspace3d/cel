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
#include "physicallayer/entity.h"
#include "plugins/stdphyslayer/pl.h"

class celPropertyClassList;
class celPlLayer;

/**
 * Implementation of iCelEntity.
 */
class celEntity : public csObject
{
private:
  celPropertyClassList* plist;
  csRef<iCelBehaviour> behaviour;
  CS_ID entity_ID;
  celPlLayer *pl;

public:
  celEntity (celPlLayer* pl);
  virtual ~celEntity ();

  CS_ID GetEntityID ()
  { return entity_ID; }
  void SetEntityID (CS_ID ID)
  { entity_ID = ID; }

  virtual void SetName (const char *name);
    
  iCelPropertyClassList* GetPropertyClassList ();
  void SetBehaviour (iCelBehaviour* ent);

  SCF_DECLARE_IBASE_EXT (csObject);

  //--------------------- iCelEntity implementation --------------------//
  struct CelEntity : public iCelEntity
  {
    SCF_DECLARE_EMBEDDED_IBASE (celEntity);
    virtual iObject* QueryObject () { return scfParent; }
    virtual const char* GetName () const { return scfParent->GetName (); }
    virtual void SetName (const char* n) { scfParent->SetName (n); }
    virtual void SetID  (CS_ID n) { scfParent->SetEntityID (n); }
    virtual CS_ID GetID () const
    {
      return scfParent->entity_ID;
    }
    virtual iCelPropertyClassList* GetPropertyClassList ()
    {
      return scfParent->GetPropertyClassList ();
    }
    virtual void SetBehaviour (iCelBehaviour* ent)
    {
      scfParent->SetBehaviour (ent);
    }
    virtual iCelBehaviour* GetBehaviour ()
    {
      return scfParent->behaviour;
    }
  } scfiCelEntity;
  friend struct CelEntity;
};

/**
 * Implementation of iCelEntityList.
 */
class celEntityList : public iCelEntityList
{
private:
  csRefArray<iCelEntity> entities;

public:
  celEntityList ();
  virtual ~celEntityList ();

  SCF_DECLARE_IBASE;

  virtual int GetCount () const;
  virtual iCelEntity* Get (int n) const;
  virtual int Add (iCelEntity* obj);
  virtual bool Remove (iCelEntity* obj);
  virtual bool Remove (int n);
  virtual void RemoveAll ();
  virtual int Find (iCelEntity* obj) const;
  virtual iCelEntity* FindByName (const char *Name) const;
};

#endif // __CEL_PLIMP_ENTITY__

