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

#ifndef __CEL_PL_PL__
#define __CEL_PL_PL__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/strset.h"

SCF_DECLARE_FAST_INTERFACE (iCelPlLayer)
SCF_DECLARE_FAST_INTERFACE (iCelBlLayer)
SCF_DECLARE_FAST_INTERFACE (iCelEntity)
SCF_DECLARE_FAST_INTERFACE (iCelEntityList)
SCF_DECLARE_FAST_INTERFACE (iCelPropertyClass)
SCF_DECLARE_FAST_INTERFACE (iCelPropertyClassList)
SCF_DECLARE_FAST_INTERFACE (iCelPropertyClassFactory)
SCF_DECLARE_FAST_INTERFACE (iCelDataBuffer)
SCF_DECLARE_FAST_INTERFACE (iCelBehaviour)

struct iObject;
struct iCelEntity;
struct iCelEntityList;
struct iCelDataBuffer;
struct iCelMessage;
struct iCelPropertyClass;
struct iCelPropertyClassFactory;
struct iCelBlLayer;
struct iSector;
struct iCamera;
class csVector3;

SCF_VERSION (iCelPlLayer, 0, 0, 3);

/**
 * This is the Physical Layer itself.
 */
struct iCelPlLayer : public iBase
{
  /**
   * Create a new physical layer entity.
   */
  virtual iCelEntity* CreateEntity () = 0;

  /**
   * Create a message.
   */
  virtual iCelMessage* CreateMessage (const char* msg_string, ...) = 0;

  /**
   * Create a data buffer. Usually property class implementations
   * will call this to create a data buffer for their own persistant
   * data. The use of this is not required. A property class can just
   * as well make its own implementation of iCelDataBuffer.
   */
  virtual iCelDataBuffer* CreateDataBuffer (long serialnr) = 0;

  /**
   * Attach an entity to some object (usually an object from the engine).
   * This function will check if the entity was already attached and
   * do nothing in that case.
   */
  virtual void AttachEntity (iObject* object, iCelEntity* entity) = 0;

  /**
   * Unattach an entity from some object.
   */
  virtual void UnattachEntity (iObject* object, iCelEntity* entity) = 0;

  /**
   * Returns the entity with ID id
   */
  virtual iCelEntity* GetEntity (CS_ID id) = 0;
  
  /**
   * Given an iObject, find the attached entity (if any).
   */
  virtual iCelEntity* FindAttachedEntity (iObject* object) = 0;

  /**
   * Find all entities that are within a certain radius of
   * a given object. This uses the attached entities from above.
   * Do DecRef() on the returned list when ready.
   */
  virtual iCelEntityList* FindNearbyEntities (iSector* sector,
  	const csVector3& pos, float radius) = 0;

  /**
   * Given a position on screen find the nearest entity to the camera.
   * Returns NULL if no entity found. This function depends on the
   * attached entities.
   */
  virtual iCelEntity* GetHitEntity (iCamera* camera, int x, int y) = 0;

  //-------------------------------------------------------------------------

  /**
   * Register a property class factory with this physical layer.
   */
  virtual void RegisterPropertyClassFactory (iCelPropertyClassFactory* pf) = 0;

  /**
   * Unregister a property class factory.
   */
  virtual void UnregisterPropertyClassFactory (
  	iCelPropertyClassFactory* pf) = 0;

  /**
   * Get the number of registered property class factories.
   */
  virtual int GetPropertyClassFactoryCount () const = 0;

  /**
   * Get the specificied property class factory.
   */
  virtual iCelPropertyClassFactory* GetPropertyClassFactory (int idx) const = 0;

  /**
   * Find a property class factory by name.
   */
  virtual iCelPropertyClassFactory* FindPropertyClassFactory (
  	const char* name) const = 0;

  /**
   * Conveniance function to create a property class from a registered class
   * factory.
   * Note that the reference count of the returned class is not turned up.
   */
  virtual iCelPropertyClass* CreatePropertyClass
      (iCelEntity* entity, const char* propname) = 0;

  //-------------------------------------------------------------------------

  /**
   * Register a behaviour layer.
   */
  virtual void RegisterBehaviourLayer (iCelBlLayer* bl) = 0;

  /**
   * Unregister a behaviour layer.
   */
  virtual void UnregisterBehaviourLayer (iCelBlLayer* bl) = 0;

  /**
   * Get the number of registered behaviour layers.
   */
  virtual int GetBehaviourLayerCount () const = 0;

  /**
   * Get the specificied behaviour layer.
   */
  virtual iCelBlLayer* GetBehaviourLayer (int idx) const = 0;

  /**
   * Find a behaviour layer by name.
   */
  virtual iCelBlLayer* FindBehaviourLayer (const char* name) const = 0;

  //-------------------------------------------------------------------------

  /**
   * Cache an SCF object. This effectively means that the PL will keep
   * a references to this object. This function will not add the object
   * if it is already cached so it is safe to call multiple times.
   */
  virtual void Cache (iBase* object) = 0;

  /**
   * Uncache an SCF object.
   */
  virtual void Uncache (iBase* object) = 0;

  /**
   * Clean up the cache. This will decrease the ref count of all
   * objects in the cache. Possibly this will remove them unless
   * some other object still has a reference.
   */
  virtual void CleanCache () = 0;

  //-------------------------------------------------------------------------

  /**
   * Retreive the unique ID associated with this string or create
   * a new ID if needed. There are various places in CEL where this is
   * used.
   * <p>
   * The convention on string names is usually 'cel.xxx.yyy.zzz'. With 'xxx'
   * equal to the type of string (i.e. 'property'), 'yyy' equal to the
   * scope of the property (i.e. 'pccamera') and 'zzz' the name of the
   * given type (i.e. the property name). An example:
   * 'cel.property.pccamera.startsector'.
   */
  virtual csStringID FetchStringID (const char* str) = 0;

  /**
   * Find the string that corresponds with some ID. Returns NULL
   * if there is no such string.
   */
  virtual const char* FetchString (csStringID id) = 0;
};

#endif // __CEL_PL_PL__

