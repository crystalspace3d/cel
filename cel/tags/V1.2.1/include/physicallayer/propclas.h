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
#include "physicallayer/datatype.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

struct iCelPropertyClassList;

/**
 * Find a property class by SCF interface. This function will first
 * try to find a property class that implements the interface but has tag
 * not set (0). If such a property class cannot be found then it will
 * return a random one that implements the given interface.
 */
#define CEL_QUERY_PROPCLASS(PcList,Interface)				    \
  (celQueryPropertyClass<Interface> (PcList))

/**
 * Find a property class by SCF interface and tag. If tag is 0 then
 * it will find the default property class.
 */
#define CEL_QUERY_PROPCLASS_TAG(PcList,Interface,Tag)			    \
  (celQueryPropertyClassTag<Interface> (PcList,Tag))

/**
 * Find a property class by SCF interface. This function will first
 * try to find a property class that implements the interface but has tag
 * not set (0). If such a property class cannot be found then it will
 * return a random one that implements the given interface.
 */
#define CEL_QUERY_PROPCLASS_ENT(Ent,Interface)				    \
  (celQueryPropertyClassEntity<Interface> (Ent))

/**
 * Find a property class by SCF interface and tag. If tag is 0 then
 * it will find the default property class.
 */
#define CEL_QUERY_PROPCLASS_TAG_ENT(Ent,Interface,Tag)			    \
  CEL_QUERY_PROPCLASS_TAG((Ent)->GetPropertyClassList(),Interface,Tag)

/**
 * Remove all property classes with given interface (ignores tag).
 */
#define CEL_REMOVE_PROPCLASS(PcList,Interface)                              \
  ((PcList)->RemoveByInterface (scfInterfaceTraits<Interface>::GetID(),           \
				scfInterfaceTraits<Interface>::GetVersion()))

/**
 * Remove all property classes with given interface and with the given tag.
 * 'tag' can be 0. In that case it will remove property classes with no tag.
 */
#define CEL_REMOVE_PROPCLASS_TAG(PcList,Interface,Tag)                      \
  ((PcList)->RemoveByInterfaceAndTag (					    \
        scfInterfaceTraits<Interface>::GetID(),          			    \
	scfInterfaceTraits<Interface>::GetVersion(), Tag))

/**
 * Remove all property classes with given interface (ignores tag).
 */
#define CEL_REMOVE_PROPCLASS_ENT(Ent, Interface)                            \
  CEL_REMOVE_PROPCLASS((Ent)->GetPropertyClassList(),Interface)

/**
 * Remove all property classes with given interface and with the given tag.
 * 'tag' can be 0. In that case it will remove property classes with no tag.
 */
#define CEL_REMOVE_PROPCLASS_TAG_ENT(Ent,Interface,Tag)                     \
  CEL_REMOVE_PROPCLASS_TAG((Ent)->GetPropertyClassList(),Interface,Tag)

class csVector3;
class csColor;
struct iCelEntity;
struct iCelDataBuffer;
struct iCelPropertyChangeCallback;
struct iCelParameterBlock;

/**
 * This is a property class for an entity. A property class
 * describe physical attributes of an entity.
 */
struct iCelPropertyClass : public virtual iBase
{
  SCF_INTERFACE (iCelPropertyClass, 0, 0, 8);

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
   * Get the parent entity associated with this property class.
   */
  virtual iCelEntity* GetEntity () = 0;

  /**
   * Set the entity for this property class.
   */
  virtual void SetEntity (iCelEntity* entity) = 0;

  /**
   * Save this object to a data buffer.
   * This routine is the first pass saving routine. Few property classes
   * need to implement this. The idea is that here information is saved
   * which is important to make other property classes work. This can
   * be information related to level data that is essential for other
   * property classes. Most property classes will return 0 here.
   */
  virtual csPtr<iCelDataBuffer> SaveFirstPass () = 0;

  /**
   * Load this object from a data buffer.
   * This is the first pass version. Most property classes will
   * return 0 here.
   */
  virtual bool LoadFirstPass (iCelDataBuffer* databuf) = 0;

  /**
   * Save this object to a data buffer.
   */
  virtual csPtr<iCelDataBuffer> Save () = 0;

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
   *
   * This function returns false if the property could
   * not be set for some reason.
   *
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
  virtual bool SetProperty (csStringID propertyID, const csVector2& value) = 0;

  /**
   * Set a generic property.
   */
  virtual bool SetProperty (csStringID propertyID, const csVector3& value) = 0;

  /**
   * Set a generic property.
   */
  virtual bool SetProperty (csStringID propertyID, const csColor& value) = 0;

  /**
   * Set a generic property.
   */
  virtual bool SetProperty (csStringID propertyID,
  	iCelPropertyClass* value) = 0;

  /**
   * Set a generic property.
   */
  virtual bool SetProperty (csStringID propertyID, iCelEntity* entity) = 0;

  /**
   * Set a generic property.
   */
  virtual bool SetProperty (csStringID propertyID, iBase* ibase) = 0;

  /**
   * Get the type of the given property or action. Returns CEL_DATA_NONE
   * if property is not supported. Only the following types are supported:
   * - CEL_DATA_LONG
   * - CEL_DATA_FLOAT
   * - CEL_DATA_BOOL
   * - CEL_DATA_STRING
   * - CEL_DATA_VECTOR2
   * - CEL_DATA_VECTOR3
   * - CEL_DATA_COLOR
   * - CEL_DATA_ACTION
   * - CEL_DATA_PCLASS
   * - CEL_DATA_ENTITY
   */
  virtual celDataType GetPropertyOrActionType (
  	csStringID propertyID) = 0;

  /**
   * Return true if a property is read-only.
   */
  virtual bool IsPropertyReadOnly (csStringID propertyID) = 0;

  /**
   * Get a property as long.
   * \deprecated Use GetPropertyLongByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyLongByID() instead.")
  virtual long GetPropertyLong (csStringID propertyID) = 0;

  /**
   * Get a property as float.
   * \deprecated Use GetPropertyFloatByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyFloatByID() instead.")
  virtual float GetPropertyFloat (csStringID propertyID) = 0;

  /**
   * Get a property as bool.
   * \deprecated Use GetPropertyBoolByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyBoolByID() instead.")
  virtual bool GetPropertyBool (csStringID propertyID) = 0;

  /**
   * Get a property as string.
   * \deprecated Use GetPropertyStringByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyStringByID() instead.")
  virtual const char* GetPropertyString (csStringID propertyID) = 0;

  /**
   * Get a property as a 2D vector.
   * \deprecated Use GetPropertyVectorByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyVectorByID() instead.")
  virtual bool GetPropertyVector (csStringID propertyID, csVector2& v) = 0;

  /**
   * Get a property as a 3D vector.
   * \deprecated Use GetPropertyVectorByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyVectorByID() instead.")
  virtual bool GetPropertyVector (csStringID propertyID, csVector3& v) = 0;

  /**
   * Get a property as a color.
   * \deprecated Use GetPropertyColorByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyColorByID() instead.")
  virtual bool GetPropertyColor (csStringID propertyID, csColor& v) = 0;

  /**
   * Get a property as property class.
   * \deprecated Use GetPropertyPClassByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyPClassByID() instead.")
  virtual iCelPropertyClass* GetPropertyPClass (csStringID propertyID) = 0;

  /**
   * Get a property as entity.
   * \deprecated Use GetPropertyEntityByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyEntityByID() instead.")
  virtual iCelEntity* GetPropertyEntity (csStringID propertyID) = 0;

  /**
   * Get a property as iBase.
   * \deprecated Use GetPropertyIBaseByID() instead.
   */
  CS_DEPRECATED_METHOD_MSG("Use GetPropertyIBaseByID() instead.")
  virtual iBase* GetPropertyIBase (csStringID propertyID) = 0;

  /**
   * Get a property as long.
   */
  virtual long GetPropertyLongByID (csStringID propertyID) = 0;

  /**
   * Get a property as float.
   */
  virtual float GetPropertyFloatByID (csStringID propertyID) = 0;

  /**
   * Get a property as bool.
   */
  virtual bool GetPropertyBoolByID (csStringID propertyID) = 0;

  /**
   * Get a property as string.
   */
  virtual const char* GetPropertyStringByID (csStringID propertyID) = 0;

  /**
   * Get a property as a 2D vector.
   */
  virtual bool GetPropertyVectorByID (csStringID propertyID, csVector2& v) = 0;

  /**
   * Get a property as a 3D vector.
   */
  virtual bool GetPropertyVectorByID (csStringID propertyID, csVector3& v) = 0;

  /**
   * Get a property as a color.
   */
  virtual bool GetPropertyColorByID (csStringID propertyID, csColor& v) = 0;

  /**
   * Get a property as property class.
   */
  virtual iCelPropertyClass* GetPropertyPClassByID (csStringID propertyID) = 0;

  /**
   * Get a property as entity.
   */
  virtual iCelEntity* GetPropertyEntityByID (csStringID propertyID) = 0;

  /**
   * Get a property as iBase.
   */
  virtual iBase* GetPropertyIBaseByID (csStringID propertyID) = 0;

  /**
   * Perform an action with a generic string parameter.
   * \param actionID the id of the action.
   * \param params an optional parameter block for this action.
   * \param ret is an optional return value. Note! Make sure
   * to clear this variable before calling. PerformAction()
   * implementations can choose not to initialize it.
   */
  virtual bool PerformAction (csStringID actionID,
  	iCelParameterBlock* params, celData& ret) = 0;

  /**
   * Return the number of supported properties and actions.
   */
  virtual size_t GetPropertyAndActionCount () = 0;

  /**
   * Get the ID of the given property or action.
   */
  virtual csStringID GetPropertyOrActionID (size_t i) = 0;

  /**
   * Get the description of the given property or action.
   */
  virtual const char* GetPropertyOrActionDescription (csStringID) = 0;

  /**
   * This method is called by the parent entity when there
   * is a change in property class setting. i.e. another property
   * class has been added or removed from the entity. This
   * allows this property class to make adjustments to the
   * references it keeps internally for other property classes.
   */
  virtual void PropertyClassesHaveChanged () = 0;

  /**
   * Return the persistent data of this property class.
   * <P>Note: this function replaces the Save function.
   */
  virtual csPtr<iCelDataBuffer> GetPersistentData (
        celPersistenceType persistence_type) = 0;

  /**
   * Read the state of this property class from some persistent data. 
   * <P>Note: this function replaces the Load function.
   * \param data_time The timestamp of the persistent data.
   * \param data The persistent data.
   * \param persistence_type The type of the persistence source.
   * \return Whether a problem has been encountered or not. Cheats can happen 
   * only if the type of the persistence source is 
   * CEL_PERSIST_TYPE_CLIENT_CONTROL (ie, when we are on the server side and the 
   * entity is controlled by a client).
   */
  virtual celPersistenceResult SetPersistentData (csTicks data_time, 
        iCelDataBuffer* data, celPersistenceType persistence_type) = 0;
};


/**
 * A list of property classes.
 */
struct iCelPropertyClassList : public virtual iBase
{
  SCF_INTERFACE (iCelPropertyClassList, 0, 0, 2);

  /**
   * Return the number of property classes in this list.
   */
  virtual size_t GetCount () const = 0;

  /**
   * Return a property class by index.
   */
  virtual iCelPropertyClass* Get (size_t n) const = 0;

  /**
   * Add a property class.
   */
  virtual size_t Add (iCelPropertyClass* obj) = 0;

  /**
   * Remove a property class.
   */
  virtual bool Remove (iCelPropertyClass* obj) = 0;

  /**
   * Remove the nth property class.
   */
  virtual bool Remove (size_t n) = 0;

  /**
   * Remove all property classes with given interface (ignores tag).
   */
  virtual bool RemoveByInterface (scfInterfaceID scf_id, int iVersion) = 0;

  /**
   * Remove all property classes with given interface and with the given tag.
   * 'tag' can be 0. In that case it will remove property classes with no tag.
   */
  virtual bool RemoveByInterfaceAndTag (scfInterfaceID scf_id,
  	int iVersion, const char* tag) = 0;

  /**
   * Remove all property classes.
   */
  virtual void RemoveAll () = 0;

  /**
   * Find a property class and return its index.
   */
  virtual size_t Find (iCelPropertyClass* obj) const = 0;

  /**
   * Find a property class by name. This function will first try to find
   * a property class with the given name that has tag not set (0). If such
   * a property class is not found it will return a random one with the
   * given name.
   */
  virtual iCelPropertyClass* FindByName (const char *name) const = 0;

  /**
   * Find a property class by name and tag. If tag is 0 then it will
   * find the default property class.
   */
  virtual iCelPropertyClass* FindByNameAndTag (const char *name,
  	const char* tag) const = 0;

  /**
   * Find a property class by SCF interface. Return that interface
   * with ref-count incremented with one. This function will first try
   * to find a property class that implements the interface but has tag
   * not set (0). If such a property class cannot be found then it will
   * return a random one that implements the given interface.
   */
  virtual iBase* FindByInterface (
  	scfInterfaceID id, int version) const = 0;

  /**
   * Find a property class by SCF interface and tag. If tag is 0 then
   * it will find the default property class.
   */
  virtual iBase* FindByInterfaceAndTag (
  	scfInterfaceID id, int version, const char* tag) const = 0;
};

/**
 * Find a property class by SCF interface and tag. If tag is 0 then
 * it will find the default property class.
 */
template<class Interface>
inline csPtr<Interface> celQueryPropertyClassTag (
  iCelPropertyClassList* plist, const char* tag)
{
  iBase* base = plist->FindByInterfaceAndTag (
    scfInterfaceTraits<Interface>::GetID (),
    scfInterfaceTraits<Interface>::GetVersion (),
    tag);

  if (base == 0) return csPtr<Interface> (0); 

  Interface* x = (Interface*)base->QueryInterface (
    scfInterfaceTraits<Interface>::GetID (),
    scfInterfaceTraits<Interface>::GetVersion ());

  base->DecRef ();	// Release our base interface.
  return csPtr<Interface> (x);
}

/**
 * Find a property class by SCF interface. This function will first
 * try to find a property class that implements the interface but has tag
 * not set (0). If such a property class cannot be found then it will
 * return a random one that implements the given interface.
 */
template<class Interface>
inline csPtr<Interface> celQueryPropertyClass (
  iCelPropertyClassList* plist)
{
  iBase* base = plist->FindByInterface (
    scfInterfaceTraits<Interface>::GetID (),
    scfInterfaceTraits<Interface>::GetVersion ());

  if (base == 0) return csPtr<Interface> (0); 

  Interface* x = (Interface*)base->QueryInterface (
    scfInterfaceTraits<Interface>::GetID (),
    scfInterfaceTraits<Interface>::GetVersion ());

  base->DecRef ();	// Release our base interface.
  return csPtr<Interface> (x);
}

/**
 * Find a property class by SCF interface. This function will first
 * try to find a property class that implements the interface but has tag
 * not set (0). If such a property class cannot be found then it will
 * return a random one that implements the given interface.
 */
template<class Interface>
inline csPtr<Interface> celQueryPropertyClassEntity (
  iCelEntity* entity)
{
  return celQueryPropertyClass<Interface> (entity->GetPropertyClassList ());
}

#endif // __CEL_PL_PROPCLASS__

