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
#include "csutil/strset.h"
#include "csutil/ref.h"
#include "pl/datatype.h"
class csVector3;

#define CEL_QUERY_PROPCLASS(PcList,Interface)				    \
  csPtr<Interface> (							    \
  (Interface*)((PcList)->FindByInterface (iSCF::SCF->GetInterfaceID	    \
		    (#Interface), VERSION_##Interface)))

struct iCelEntity;
struct iCelDataBuffer;
struct iCelPropertyChangeCallback;

SCF_VERSION (iCelPropertyClass, 0, 0, 4);

/**
 * This is a property class for an entity. A property class
 * describe physical attributes of an entity.
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
   * Save this object to a data buffer.
   */
  virtual iCelDataBuffer* Save () = 0;

  /**
   * Load this object from a data buffer.
   */
  virtual bool Load (iCelDataBuffer* databuf) = 0;

  /**
   * Add a callback which will be fired when a property changes.
   * Not all property class implementations actually have properties.
   * It is safe to call this function with the same callback (nothing
   * will happen in that case and false will be returned). Otherwise this
   * function will IncRef the callback.
   */
  virtual bool AddPropertyChangeCallback (iCelPropertyChangeCallback* cb) = 0;

  /**
   * Remove a callback. It is safe to call this function for a callback
   * that is not registered. Nothing will happen in that case (except
   * that false is returned). This function will DecRef the callback if
   * it was present.
   */
  virtual bool RemovePropertyChangeCallback (
  	iCelPropertyChangeCallback* cb) = 0;

  /**
   * Set a generic property. In order to make it easier to access
   * properties in a property class from general scripting languages
   * this generic interface to properties is provided. It is always
   * faster and more efficient to set properties directly but this system
   * is usable in general.
   * <p>
   * This function returns false if the property could
   * not be set for some reason.
   * <p>
   * The physical layer maintains a registry of ID's to use. The convention
   * is to use property names like: 'cel.property.num'.
   */
  virtual bool SetProperty (csStringID propertyID, long value) = 0;

  /**
   * Set a generic property.
   */
  virtual bool SetProperty (csStringID propertyID, float value) = 0;

  /**
   * Set a generic property.
   */
  virtual bool SetProperty (csStringID propertyID, bool value) = 0;

  /**
   * Set a generic property.
   */
  virtual bool SetProperty (csStringID propertyID, const char* value) = 0;

  /**
   * Set a generic property.
   */
  virtual bool SetProperty (csStringID propertyID, const csVector3& value) = 0;

  /**
   * Get the type of the given property or action. Returns CEL_DATA_NONE
   * if property is not supported. Only the following types are supported:
   * <ul>
   * <li>CEL_DATA_LONG
   * <li>CEL_DATA_FLOAT
   * <li>CEL_DATA_BOOL
   * <li>CEL_DATA_STRING
   * <li>CEL_DATA_VECTOR3
   * <li>CEL_DATA_ACTION
   * </ul>
   */
  virtual celDataType GetPropertyOrActionType (
  	csStringID propertyID) = 0;

  /**
   * Return true if a property is read-only.
   */
  virtual bool IsPropertyReadOnly (csStringID propertyID) = 0;

  /**
   * Get a property as long.
   */
  virtual long GetPropertyLong (csStringID propertyID) = 0;

  /**
   * Get a property as float.
   */
  virtual float GetPropertyFloat (csStringID propertyID) = 0;

  /**
   * Get a property as bool.
   */
  virtual bool GetPropertyBool (csStringID propertyID) = 0;

  /**
   * Get a property as string.
   */
  virtual const char* GetPropertyString (csStringID propertyID) = 0;

  /**
   * Get a property as a 3D vector.
   */
  virtual bool GetPropertyVector (csStringID propertyID, csVector3& v) = 0;

  /**
   * Perform an action with a generic string parameter.
   */
  virtual bool PerformAction (csStringID actionID, const char* params) = 0;

  /**
   * Return the number of supported properties and actions.
   */
  virtual int GetPropertyAndActionCount () const = 0;

  /**
   * Get the ID of the given property or action.
   */
  virtual csStringID GetPropertyOrActionID (int i) = 0;

  /**
   * Get the description of the given property or action.
   */
  virtual const char* GetPropertyOrActionDescription (csStringID) = 0;
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

