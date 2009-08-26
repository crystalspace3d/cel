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

#ifndef __CEL_PF_PROP__
#define __CEL_PF_PROP__

#include "cstypes.h"
#include "csutil/scf.h"
#include "physicallayer/datatype.h"

struct iCelEntity;
struct iCelPropertyClass;
struct iPcProperties;
class csVector2;
class csVector3;
class csColor;

/**
 * Listen for property changes.
 */
struct iPcPropertyListener : public virtual iBase
{
  SCF_INTERFACE (iPcPropertyListener, 0, 0, 1);

  /**
   * Property was changed or cleared.
   */
  virtual void PropertyChanged (iPcProperties* pcprop, size_t idx) = 0;
};

/**
 * This is a general property class.
 *
 * This property class can send out the following messages:
 * - 'cel.properties.set' (old 'pcproperties_setproperty'): sets the property (index)
 * - 'cel.properties.clear' (old 'pcproperties_clearproperty'): clears the property (index)
 *
 * This property class supports the following actions:
 * - SetProperty: parameters 'name' (string) and 'value'
 *   (string/bool/float/long/vector).
 */
struct iPcProperties : public virtual iBase
{
  SCF_INTERFACE (iPcProperties, 0, 0, 1);

  /**
   * Add a property listener. Property listeners are called right before
   * the behaviour is called.
   */
  virtual void AddPropertyListener (iPcPropertyListener* listener) = 0;
  /**
   * Remove a property listener.
   */
  virtual void RemovePropertyListener (iPcPropertyListener* listener) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, float value) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, long value) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, bool value) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, const char* value) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, const csVector2& value) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, const csVector3& value) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, const csColor& value) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, iCelPropertyClass* pclass) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, iCelEntity* entity) = 0;

  /**
   * Set property.
   */
  virtual void SetProperty (const char* name, iBase* value) = 0;

  /**
   * Get the specified property index for a named property.
   * Returns csArrayItemNotFound if not found.
   */
  virtual size_t GetPropertyIndex (const char* name) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, float value) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, long value) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, bool value) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, const csVector2& value) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, const csVector3& value) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, const csColor& value) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, const char* value) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, iCelPropertyClass* pc) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, iCelEntity* entity) = 0;

  /**
   * Set property based on index.
   */
  virtual void SetPropertyIndex (size_t index, iBase* value) = 0;

  /**
   * Get the type of a property. Returns CEL_DATA_NONE if the
   * property is undefined. Note that only the following data types
   * are supported:
   * - CEL_DATA_LONG
   * - CEL_DATA_FLOAT
   * - CEL_DATA_BOOL
   * - CEL_DATA_STRING
   * - CEL_DATA_VECTOR2
   * - CEL_DATA_VECTOR3
   * - CEL_DATA_COLOR
   * - CEL_DATA_PCLASS
   * - CEL_DATA_ENTITY
   * - CEL_DATA_IBASE
   */
  virtual celDataType GetPropertyType (size_t index) const = 0;

  /**
   * Get property as float. Returns 0 if wrong type.
   */
  virtual float GetPropertyFloat (size_t index) const = 0;

  /**
   * Get property as long. Returns 0 if wrong type.
   */
  virtual long GetPropertyLong (size_t index) const = 0;

  /**
   * Get property as bool. Returns false if wrong type.
   */
  virtual bool GetPropertyBool (size_t index) const = 0;

  /**
   * Get property as bool. Returns false if wrong type.
   */
  virtual bool GetPropertyVector (size_t index, csVector2& v) const = 0;

  /**
   * Get property as bool. Returns false if wrong type.
   */
  virtual bool GetPropertyVector (size_t index, csVector3& v) const = 0;

  /**
   * Get property as bool. Returns false if wrong type.
   */
  virtual bool GetPropertyColor (size_t index, csColor& v) const = 0;

  /**
   * Get property as string. Returns 0 if wrong type.
   */
  virtual const char* GetPropertyString (size_t index) const = 0;

  /**
   * Get property as property class. Returns 0 if wrong type.
   */
  virtual iCelPropertyClass* GetPropertyPClass (size_t index) const = 0;

  /**
   * Get property as entity. Returns 0 if wrong type.
   */
  virtual iCelEntity* GetPropertyEntity (size_t index) const = 0;

  /**
   * Get property as iBase. Returns 0 if wrong type.
   */
  virtual iBase* GetPropertyIBase (size_t index) const = 0;

  /**
   * Clear a property.
   */
  virtual void ClearProperty (size_t index) = 0;

  /**
   * Clear all properties.
   */
  virtual void Clear () = 0;

  /**
   * Get the number of properties defined in this property class.
   */
  virtual size_t GetPropertyCount () const = 0;

  /**
   * Get the specified property name.
   */
  virtual const char* GetPropertyName (size_t idx) const = 0;

  /**
   * Debug dump information.
   */
  virtual void Dump () = 0;
};

#endif // __CEL_PF_PROP__

