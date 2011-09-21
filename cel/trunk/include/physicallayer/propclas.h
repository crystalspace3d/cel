/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2011 by Jorrit Tyberghein
  
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

class csVector3;
class csColor;
struct iCelEntity;
struct iCelDataBuffer;
struct iCelCompactDataBufferWriter;
struct iCelCompactDataBufferReader;
struct iCelPropertyChangeCallback;
struct iCelParameterBlock;
struct iStringSet;

/**
 * If a property class represents a position in the world then it
 * will also implement this interface.
 */
struct iCelPositionInfo : public virtual iBase
{
  SCF_INTERFACE (iCelPositionInfo, 0, 1, 1);

  /**
   * Get the main sector of this object (if an object spans multiple
   * sectors then you have to get the iMovable information).
   */
  virtual iSector* GetSector () = 0;

  /**
   * Get the world space position of this object.
   */
  virtual const csVector3 GetPosition () = 0;

  /**
   * If the position of this entity is represented with a movable
   * then you can get it with this function. This can return 0 if
   * the entity doesn't support movables.
   */
  virtual iMovable* GetMovable () = 0;

  /**
   * Get a bounding radius for this entity.
   */
  virtual float GetBoundingRadius () = 0;
};

/**
 * This is a property class for an entity. A property class
 * describe physical attributes of an entity.
 */
struct iCelPropertyClass : public virtual iBase
{
  SCF_INTERFACE (iCelPropertyClass, 0, 0, 9);

  /**
   * Set the name of this property class.
   */
  virtual void SetName (const char* name) = 0;

  /**
   * Get the name of this property class.
   */
  virtual const char* GetName () const = 0;

  /**
   * Get the associated iObject from this property class.
   */
  virtual iObject *QueryObject () = 0;

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
   * The physical layer maintains a registry of ID's to use.
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

  /**
   * Get the associated position information. Can be 0 if this
   * property class doesn't support positional information.
   */
  virtual iCelPositionInfo* QueryPositionInfo () = 0;

  /**
   * Activate this property class. This means it will process events again.
   * Property classes are activated by default.
   */
  virtual void Activate () = 0;

  /**
   * Deactivate this property class. This means that events will no longer be
   * processed.
   */
  virtual void Deactivate () = 0;

  /**
   * Mark the baseline for this property class. This means that the status of this
   * property class as it is now doesn't have to be saved. Only changes to the property
   * class that happen after this baseline have to be modified. A property class doesn't
   * actually have to do this in a granular way. It can simply say that it saves itself
   * completely as soon as it has been modified after the baseline.
   */
  virtual void MarkBaseline () = 0;

  /**
   * Return true if this property class was modified after the baseline.
   */
  virtual bool IsModifiedSinceBaseline () const = 0;

  /**
   * Return the data that represents the information that changed after the
   * baseline. If it is too complicated to actually return the information that
   * really changed it is allowed for this function to simply save the complete state.
   * If this function wants to save strings effectively it is best to intern them first
   * using the 'strings' set.
   */
  virtual void SaveModifications (iCelCompactDataBufferWriter* buf, iStringSet* strings) = 0;

  /**
   * Call this function if the property class is in the state as it was at the moment of
   * the baseline. This function will put back the modifications that were made
   * after the baseline.
   * Interned strings can be fetched from the 'strings' set.
   */
  virtual void RestoreModifications (iCelCompactDataBufferReader* buf,
      const csHash<csString,csStringID>& strings) = 0;
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

/**
 * Find a property class by SCF interface and tag. If tag is 0 then
 * it will find the default property class.
 */
template<class Interface>
inline csPtr<Interface> celQueryPropertyClassTagEntity (
  iCelEntity* entity, const char* tag)
{
  return celQueryPropertyClassTag<Interface> (entity->GetPropertyClassList (),
      tag);
}

#endif // __CEL_PL_PROPCLASS__

