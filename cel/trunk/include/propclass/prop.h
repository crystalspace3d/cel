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
class csVector2;
class csVector3;
class csColor;

SCF_VERSION (iPcProperties, 0, 0, 1);

/**
 * This is a general property class.
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>SetProperty: parameters 'name' (string) and 'value' (any).
 * </ul>
 * <p>
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * <ul>
 * <li>pcproperties_setproperty: sets the property (index)
 * <li>pcproperties_clearproperty: clears the property (index)
 * </ul>
 */
struct iPcProperties : public iBase
{
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
   * Get the specified property index for a named property.
   * Returns -1 if not found.
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
   * Get the type of a property. Returns CEL_DATA_NONE if the
   * property is undefined. Note that only the following data types
   * are supported:
   * <ul>
   * <li>CEL_DATA_LONG
   * <li>CEL_DATA_FLOAT
   * <li>CEL_DATA_BOOL
   * <li>CEL_DATA_STRING
   * <li>CEL_DATA_VECTOR2
   * <li>CEL_DATA_VECTOR3
   * <li>CEL_DATA_COLOR
   * <li>CEL_DATA_PCLASS
   * <li>CEL_DATA_ENTITY
   * </ul>
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

