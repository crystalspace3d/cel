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
#include "csutil/set.h"
#include "csgeom/vector2.h"
#include "csgeom/vector3.h"
#include "csutil/cscolor.h"

#include "physicallayer/datatype.h"
#include "tools/parameters.h"

struct iCelEntity;
struct iCelEntityTemplate;
struct iCelPropertyClassTemplate;
struct iCelPropertyClass;
struct iTemplateCharacteristics;

/**
 * Iterator to iterate over the parameters of a property class
 * template.
 */
struct iCelParameterIterator : public virtual iBase
{
  SCF_INTERFACE (iCelParameterIterator, 0, 0, 1);

  /**
   * Is there another parameter?
   */
  virtual bool HasNext () const = 0;

  /**
   * Get next parameter.
   */
  virtual iParameter* Next (csStringID& id) = 0;
};

/**
 * Iterator to iterate over entity templates.
 */
struct iCelEntityTemplateIterator : public virtual iBase
{
  SCF_INTERFACE (iCelEntityTemplateIterator, 0, 0, 1);

  /**
   * Is there another template?
   */
  virtual bool HasNext () const = 0;

  /**
   * Get next template.
   */
  virtual iCelEntityTemplate* Next () = 0;
};

/**
 * This is an entity template. It can be used to create other entities.
 */
struct iCelEntityTemplate : public virtual iBase
{
  SCF_INTERFACE (iCelEntityTemplate, 0, 2, 1);

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
   * Find a certain property class template.
   */
  virtual iCelPropertyClassTemplate* FindPropertyClassTemplate (const char* name,
      const char* tag) = 0;

  /**
   * Get the amount of property class templates.
   */
  virtual size_t GetPropertyClassTemplateCount () const = 0;

  /**
   * Get a specific property class template.
   */
  virtual iCelPropertyClassTemplate* GetPropertyClassTemplate (size_t idx) const = 0;

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
   * Add a message to be sent to the created behaviour.
   * Note that all messages will be sent after all properties and
   * property classes are created.
   */
  virtual void AddMessage (csStringID msgid,
      csHash<csRef<iParameter>, csStringID>& params) = 0;

  /**
   * Get the number of messages.
   */
  virtual size_t GetMessageCount () const = 0;

  /**
   * Get a message.
   */
  virtual csRef<iCelParameterIterator> GetMessage (size_t idx,
		  csStringID& id) const = 0;

  /**
   * Add a class to this entity. A class is an application defined
   * attribute which indicates to what the entity belongs. It can be something
   * like 'weapon' or 'creature' or whatever the application desires.
   */
  virtual void AddClass (csStringID cls) = 0;

  /**
   * Remove a class from this entity.
   */
  virtual void RemoveClass (csStringID cls) = 0;

  /**
   * Check if this entity belongs to some class.
   */
  virtual bool HasClass (csStringID cls) = 0;

  /**
   * Return the set of classes for this entity.
   */
  virtual const csSet<csStringID>& GetClasses () const = 0;

  /**
   * Merge another template into this one. This physically merges
   * all information from the other template into this one without
   * leaving any trace that this merge has occured (this contrasts
   * with AddParent() below).
   */
  virtual void Merge (iCelEntityTemplate* tpl) = 0;

  /**
   * Add a parent template to this one. When creating an entity
   * from this template the parent templates are applied first in
   * the order that they were added. This is different from Merge() since
   * the parent templates are only used at the time the entities are
   * created so every change that is done to the parent template will
   * still have an effect.
   */
  virtual void AddParent (iCelEntityTemplate* tpl) = 0;

  /**
   * Remove a parent.
   */
  virtual void RemoveParent (iCelEntityTemplate* tpl) = 0;

  /**
   * Remove all parents.
   */
  virtual void RemoveParents () = 0;

  /**
   * Get the parents of this template.
   */
  virtual csPtr<iCelEntityTemplateIterator> GetParents () const = 0;

  /**
   * Get the template characteristics interface for this
   * template.
   */
  virtual iTemplateCharacteristics* GetCharacteristics () = 0;
};

/**
 * This is a property class template for an entity. A property class
 * describe physical attributes of an entity.
 */
struct iCelPropertyClassTemplate : public virtual iBase
{
  SCF_INTERFACE (iCelPropertyClassTemplate, 0, 0, 1);

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
   *
   * This function returns false if the property could
   * not be set for some reason.
   *
   * The physical layer maintains a registry of ID's to use.
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
   * Perform an action with a generic parameter list.
   */
  virtual void PerformAction (csStringID actionID,
  	const csHash<csRef<iParameter>, csStringID>& params) = 0;

  /**
   * Get the number of properties and actions.
   */
  virtual size_t GetPropertyCount () const = 0;

  /**
   * Get a property. If the property is an action (data.type == CEL_DATA_NONE)
   * then the returned iterator will iterate over all parameters of that action.
   */
  virtual csRef<iCelParameterIterator> GetProperty (size_t idx,
		  csStringID& id, celData& data) const = 0;

  /**
   * Find a given property by it's id. Return csArrayItemNotFound if not found.
   */
  virtual size_t FindProperty (csStringID id) const = 0;

  /**
   * Remove all properties.
   */
  virtual void RemoveAllProperties () = 0;
};

#endif // __CEL_PL_ENTITYTEMP__

