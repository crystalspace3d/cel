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

#ifndef __CEL_PL_ENTITY__
#define __CEL_PL_ENTITY__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCelPropertyClassList;
struct iCelBehaviour;
struct iObject;

SCF_VERSION (iCelEntity, 0, 0, 1);

/**
 * This is an entity in the CEL layer at the PL (physical layer) side.
 */
struct iCelEntity : public iBase
{
  /**
   * Get the iObject for this entity (if supported). Otherwise
   * return NULL.
   */
  virtual iObject* QueryObject () = 0;

  /**
   * Get the name of this entity.
   */
  virtual const char* GetName () const = 0;

  /**
   * Set the name of this entity.
   */
  virtual void SetName (const char* n) = 0;

  /**
   * Return the (unique) ID of the entity.
   */
  virtual CS_ID GetID () const = 0;

  /**
   * Get the list of property classes.
   */
  virtual iCelPropertyClassList* GetPropertyClassList () = 0;

  /**
   * Set the behaviour layer entity associated with this entity.
   */
  virtual void SetBehaviour (iCelBehaviour* behaviour) = 0;

  /**
   * Get the behaviour layer entity associated with this entity.
   */
  virtual iCelBehaviour* GetBehaviour () = 0;
};

SCF_VERSION (iCelEntityList, 0, 0, 1);

/**
 * A list of entities.
 */
struct iCelEntityList : public iBase
{
  /**
   * Return the number of entities in this list.
   */
  virtual int GetCount () const = 0;

  /**
   * Return an entity class by index.
   */
  virtual iCelEntity* Get (int n) const = 0;

  /**
   * Add an entity.
   */
  virtual int Add (iCelEntity* obj) = 0;

  /**
   * Remove an entity.
   */
  virtual bool Remove (iCelEntity* obj) = 0;

  /**
   * Remove the nth entity.
   */
  virtual bool Remove (int n) = 0;

  /**
   * Remove all entities.
   */
  virtual void RemoveAll () = 0;

  /**
   * Find an entity and return its index.
   */
  virtual int Find (iCelEntity* obj) const = 0;

  /**
   * Find an entity by name.
   */
  virtual iCelEntity* FindByName (const char *Name) const = 0;
};

#endif // __CEL_PL_ENTITY__

