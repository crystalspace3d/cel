/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PL_ENTITYTEMP__
#define __CEL_PL_ENTITYTEMP__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/strset.h"
#include "csgeom/vector2.h"
#include "csgeom/vector3.h"
#include "csutil/cscolor.h"

#include "physicallayer/datatype.h"

struct iCelEntity;
struct iCelPropertyClassTemplate;
struct iCelPropertyClass;
struct iCelParameterBlock;

SCF_VERSION (iCelEntityTemplate, 0, 0, 1);

/**
 * This is an entity template. It can be used to create other entities.
 */
struct iCelEntityTemplate : public iBase
{
  /**
   * Get the iObject for this entity template.
   * \return A pointer to the iObject for this entity.
   */
  virtual iObject* QueryObject () = 0;

  /**
   * Get the name of this template.
   * \return A pointer to the name of this template.
   */
  virtual const char* GetName () const = 0;

  /**
   * Set the name of this template.
   * \param n A pointer to the name to set for this template.
   */
  virtual void SetName (const char* n) = 0;

  /**
   * Create a new property class template for this entity.
   */
  virtual iCelPropertyClassTemplate* CreatePropertyClassTemplate () = 0;

  /**
   * Set the behaviour associated with this template.
   * \param layer is the name of the behaviour layer (optional, can be 0
   * for default).
   * \param behaviour is the name of the behaviour.
   */
  virtual void SetBehaviour (const char* layer, const char* behaviour) = 0;

  /**
   * Get the layer for this template.
   */
  virtual const char* GetBehaviourLayer () const = 0;

  /**
   * Get the behaviour for this template.
   */
  virtual const char* GetBehaviour () const = 0;

  /**
   * Add a message to be sent to the created behaviour. The parameters
   * here support CEL_DATA_PARAMETER. Note that all messages will be sent
   * after all properties and property classes are created.
   */
  virtual void AddMessage (const char* msgid, iCelParameterBlock* params) = 0;
};

SCF_VERSION (iCelPropertyClassTemplate, 0, 0, 1);

/**
 * This is a property class template for an entity. A property class
 * describe physical attributes of an entity.
 */
struct iCelPropertyClassTemplate : public iBase
{
  /**
   * Set the name of this property class.
   */
  virtual void SetName (const char* name) = 0;

  /**
   * Get the name of this property class.
   */
  virtual const char* GetName () const = 0;

  /**
   * Set the optional tag of this property class. Multiple property
   * classes of the same type (same name) can be attached to the same
   * entity. To do that you need to add a tag so that you can uniquely
   * identify them.
   */
  virtual void SetTag (const char* tagname) = 0;

  /**
   * Get the tag of this property class or return 0 if there is no
   * tag.
   */
  virtual const char* GetTag () const = 0;

  /**
   * Set a property with variables instead of a predefined value. This uses
   * CEL_DATA_PARAMETER.
   */
  virtual void SetPropertyVariable (csStringID propertyID, celDataType type,
  	const char* varname) = 0;

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
  virtual void SetProperty (csStringID propertyID, long value) = 0;

  /**
   * Set a generic property.
   */
  virtual void SetProperty (csStringID propertyID, float value) = 0;

  /**
   * Set a generic property.
   */
  virtual void SetProperty (csStringID propertyID, bool value) = 0;

  /**
   * Set a generic property.
   */
  virtual void SetProperty (csStringID propertyID, const char* value) = 0;

  /**
   * Set a generic property.
   */
  virtual void SetProperty (csStringID propertyID, const csVector2& value) = 0;

  /**
   * Set a generic property.
   */
  virtual void SetProperty (csStringID propertyID, const csVector3& value) = 0;

  /**
   * Set a generic property.
   */
  virtual void SetProperty (csStringID propertyID, const csColor& value) = 0;

  /**
   * Set a generic property.
   */
  virtual void SetProperty (csStringID propertyID,
  	iCelPropertyClass* value) = 0;

  /**
   * Set a generic property.
   */
  virtual void SetProperty (csStringID propertyID, iCelEntity* entity) = 0;

  /**
   * Perform an action with a generic parameter list. The parameters
   * here support CEL_DATA_PARAMETER.
   */
  virtual void PerformAction (csStringID actionID,
  	iCelParameterBlock* params) = 0;
};

#endif // __CEL_PL_ENTITYTEMP__

