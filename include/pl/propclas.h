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

#ifndef __CEL_PL_PROPCLASS__
#define __CEL_PL_PROPCLASS__

#include "cstypes.h"
#include "csutil/scf.h"

#define CEL_QUERY_PROPCLASS(PcList,Interface)		\
  (Interface*)((PcList)->FindByInterface (iSCF::SCF->GetInterfaceID (#Interface), \
  	VERSION_##Interface))

struct iCelEntity;
struct iCelDataBuffer;

SCF_VERSION (iCelPropertyClass, 0, 0, 1);

/**
 * This is a property class for an entity. A property class
 * describes this physical attributes of an entity.
 */
struct iCelPropertyClass : public iBase
{
  /**
   * Get the name of this property class.
   */
  virtual const char* GetName () const = 0;

  /**
   * Get the parent entity associated with this property class.
   */
  virtual iCelEntity* GetEntity () = 0;

  /**
   * Set the entity for this property class.
   */
  virtual void SetEntity (iCelEntity* entity) = 0;

  /**
   * Get a pointer to persistable data.
   */
  virtual iCelDataBuffer* GetDataBuffer () = 0;

  /**
   * Save this object to the data buffer. To use this
   * you first call Save() which will update the data
   * buffer and then you call GetDataBuffer() to fetch
   * all the values.
   */
  virtual void Save () = 0;

  /**
   * Load this object from the data buffer that was
   * updated previously. i.e. to use this you first
   * call GetDataBuffer() which you then fill in from whatever
   * source you want. After that you run Load() which will
   * initialize the object from the filled in data buffer.
   */
  virtual void Load () = 0;
};


SCF_VERSION (iCelPropertyClassList, 0, 0, 1);

/**
 * A list of property classes.
 */
struct iCelPropertyClassList : public iBase
{
  /**
   * Return the number of property classes in this list.
   */
  virtual int GetCount () const = 0;

  /**
   * Return a property class by index.
   */
  virtual iCelPropertyClass* Get (int n) const = 0;

  /**
   * Add a property class.
   */
  virtual int Add (iCelPropertyClass* obj) = 0;

  /**
   * Remove a property class.
   */
  virtual bool Remove (iCelPropertyClass* obj) = 0;

  /**
   * Remove the nth property class.
   */
  virtual bool Remove (int n) = 0;

  /**
   * Remove all property classes.
   */
  virtual void RemoveAll () = 0;

  /**
   * Find a property class and return its index.
   */
  virtual int Find (iCelPropertyClass* obj) const = 0;

  /**
   * Find a property class by name.
   */
  virtual iCelPropertyClass* FindByName (const char *Name) const = 0;

  /**
   * Find a property class by SCF interface. Return that interface
   * with ref-count incremented with one.
   */
  virtual iBase* FindByInterface (scfInterfaceID id, int version) const = 0;
};

#endif // __CEL_PL_PROPCLASS__

