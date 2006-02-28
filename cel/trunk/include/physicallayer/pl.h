/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2005 by Jorrit Tyberghein

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

#ifndef __CEL_PL_PL__
#define __CEL_PL_PL__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/strset.h"
#include "csutil/ref.h"

struct iObject;
struct iCelEntity;
struct iCelEntityList;
struct iCelDataBuffer;
struct iCelMessage;
struct iCelPropertyClass;
struct iCelPropertyClassFactory;
struct iCelBlLayer;
struct iCelBehaviour;
struct iCelEntityTemplate;
struct iSector;
struct iCamera;
class csVector3;
class csBox3;
class csString;

#define CEL_EVENT_PRE 0
#define CEL_EVENT_VIEW 1
#define CEL_EVENT_POST 2

#define CEL_PROPCLASS_END (void*)0
#define CEL_PARAM_END (void*)0

/// Parameters used to create an entity from a template.
typedef csHash<csStrKey, csStrKey> celEntityTemplateParams;

struct iCelEntityTracker;

/**
 * Implement this interface if you want to know when new entities
 * are added.
 */
struct iCelNewEntityCallback : public virtual iBase
{
  SCF_INTERFACE (iCelNewEntityCallback, 0, 0, 2);

  /** this function is called for entities that are added */
  virtual void NewEntity (iCelEntity* entity) = 0;
};

/**
 * Implement this interface if you want to know when entities
 * are removed.
 */
struct iCelEntityRemoveCallback : public virtual iBase
{
  SCF_INTERFACE (iCelEntityRemoveCallback, 0, 0, 2);

  /** this function is called for entities that are removed */
  virtual void RemoveEntity (iCelEntity* entity) = 0;
};


/**
 * Generic timing system in CEL. If you want to get a tick every
 * frame or after a while you can implement this and register.
 */
struct iCelTimerListener : public virtual iBase
{
  SCF_INTERFACE (iCelTimerListener, 0, 0, 1);

  /**
   * This function is called by the physical layer when a broadcast
   * is needed. Use iCelPlLayer->CallbackEveryFrame() to register.
   */
  virtual void TickEveryFrame () = 0;

  /**
   * This function is called by the physical layer when a broadcast
   * is needed. Use iCelPlLayer->CallbackOnce() to register.
   */
  virtual void TickOnce () = 0;
};

SCF_VERSION (iCelPlLayer, 0, 3, 2);

/**
 * This is the Physical Layer itself.
 */
struct iCelPlLayer : public iBase
{
  /**
   * Create a new physical layer entity. The physical layer
   * will keep a reference to this entity. You need to call RemoveEntity()
   * to remove the entity.
   */
  virtual csPtr<iCelEntity> CreateEntity () = 0;

  /**
   * Create a new physical layer entity, with the ID being allocated in the
   * given scope. 
   */
  virtual csPtr<iCelEntity> CreateEntityInScope (int scope) = 0;

  /**
   * Create a new physical layer entity, with an id provided. You should
   * ensure that the id provided isn't already used and that you use the
   * hash implementation of the numreg.
   */
  virtual csPtr<iCelEntity> CreateEntity (uint id) = 0;

  /**
   * Create a new physical layer entity. The physical layer
   * will keep a reference to this entity. You need to call RemoveEntity()
   * to remove the entity. This version accepts a name for the entity
   * and will also create a behaviour from the given behaviour layer.
   * This function will use the reporter to report an error if
   * the entity could not be created for some reason (or the behaviour
   * failed). In that case this function returns 0. This function will
   * also create a number of property classes for this entity. You have
   * to end the list of property class names with CEL_PROPCLASS_END.
   * The variable parameters should be property class strings.
   * \param entname can be 0. In that case no name will be set.
   * \param bl is the behaviour layer and can be 0.
   * \param bhname can be 0.
   */
  virtual csPtr<iCelEntity> CreateEntity (const char* entname,
  	iCelBlLayer* bl, const char* bhname, ...) = 0;

  /**
   * Remove an entity from the physical layer.
   */
  virtual void RemoveEntity (iCelEntity* entity) = 0;

  /**
   * Remove all entities.
   */
  virtual void RemoveEntities () = 0;

  /**
   * Create an entity template.
   * \param factname is the name of the template.
   */
  virtual iCelEntityTemplate* CreateEntityTemplate (const char* factname) = 0;

  /**
   * Remove an entity template.
   */
  virtual void RemoveEntityTemplate (iCelEntityTemplate* entfact) = 0;

  /**
   * Remove all entity templates.
   */
  virtual void RemoveEntityTemplates () = 0;

  /**
   * Find an entity template with a given name.
   */
  virtual iCelEntityTemplate* FindEntityTemplate (const char* factname) = 0;

  /**
   * Get the number of entity templates.
   */
  virtual size_t GetEntityTemplateCount () const = 0;

  /**
   * Get a specific entity template.
   */
  virtual iCelEntityTemplate* GetEntityTemplate (size_t idx) const = 0;

  /**
   * Create an entity from a template.
   * \param factory is the template to create from.
   * \param name is the name of the new entity.
   * \param params is a hash with parameters that will be used
   * during entity creation.
   */
  virtual iCelEntity* CreateEntity (iCelEntityTemplate* factory,
  	const char* name, const celEntityTemplateParams& params) = 0;

  /**
   * Create an entity from a template.
   * \param factory is the template to create from.
   * \param name is the name of the new entity.
   * After that comes a list of parameter pairs (two strings) with the
   * last one being equal to CEL_PARAM_END. This will construct
   * a celEntityTemplateParams instance from these parameters and
   * then call the other CreateEntity() variant.
   */
  virtual iCelEntity* CreateEntity (iCelEntityTemplate* factory,
  	const char* name, ...) = 0;

  /**
   * Create a data buffer. Usually property class implementations
   * will call this to create a data buffer for their own persistent
   * data. The use of this is not required. A property class can just
   * as well make its own implementation of iCelDataBuffer.
   */
  virtual csPtr<iCelDataBuffer> CreateDataBuffer (long serialnr) = 0;

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
  virtual iCelEntity* GetEntity (uint id) = 0;

  /**
   * Get the total number of entities.
   */
  virtual size_t GetEntityCount () const = 0;

  /**
   * Get the specific entity by index (not by ID).
   */
  virtual iCelEntity* GetEntityByIndex (size_t idx) const = 0;

  /**
   * Find an entity with a given name. This function will scan all entities
   * (at maximum) the first time it is called for a given entity. Later on
   * it will use a hash for faster finding.
   */
  virtual iCelEntity* FindEntity (const char* name) = 0;

  /**
   * This is a convenience function to get the behaviour that belongs
   * to the entity for a given ID.
   */
  virtual iCelBehaviour* GetBehaviour (uint id) = 0;

  /**
   * Given an iObject, find the attached entity (if any).
   */
  virtual iCelEntity* FindAttachedEntity (iObject* object) = 0;

  /**
   * Find all entities that are within a certain radius of
   * a given position. This uses the attached entities from above.
   * \param do_invisible if true then also return invisible entities.
   */
  virtual csPtr<iCelEntityList> FindNearbyEntities (iSector* sector,
  	const csVector3& pos, float radius, bool do_invisible = false) = 0;

  /**
   * Find all entities that are in a given box.
   * This uses the attached entities from above.
   * \param do_invisible if true then also return invisible entities.
   */
  virtual csPtr<iCelEntityList> FindNearbyEntities (iSector* sector,
  	const csBox3& box, bool do_invisible = false) = 0;

  /**
   * Find all entities that are intersected by a beam.
   * This uses the attached entities from above.
   * \param do_invisible if true then also return invisible entities.
   */
  virtual csPtr<iCelEntityList> FindNearbyEntities (iSector* sector,
  	const csVector3& start, const csVector3& end,
	bool do_invisible = false) = 0;

  /**
   * Given a position on screen find the nearest entity to the camera.
   * Returns 0 if no entity found. This function depends on the
   * attached entities.
   */
  virtual iCelEntity* GetHitEntity (iCamera* camera, int x, int y) = 0;

  /**
   * Create an empty entity list for personal use.
   */
  virtual csPtr<iCelEntityList> CreateEmptyEntityList () = 0;

  /**
   * Create an entity tracker that you can use to keep track of
   * specific sets of entities. Call RemoveEntityTracker() to remove it.
   */
  virtual iCelEntityTracker* CreateEntityTracker (const char* name) = 0;

  /**
   * Find a tracker by name.
   */
  virtual iCelEntityTracker* FindEntityTracker (const char* name) = 0;

  /**
   * Remove a tracker.
   */
  virtual void RemoveEntityTracker (iCelEntityTracker* tracker) = 0;

  /**
   * Enable or disable the operations of the cel entity addon. This addon
   * will enable users to add entities from a CS world file. This addon
   * will not do that if this flag is disabled (enabled by default). This is
   * mostly used by the persistence layer to prevent the addons from working
   * then since the entities will be loaded otherwise in that situation.
   */
  virtual void SetEntityAddonAllowed (bool allow) = 0;

  /**
   * Query if entity addons are allowed.
   */
  virtual bool IsEntityAddonAllowed () const = 0;

  //-------------------------------------------------------------------------

  /**
   * Load a property class plugin by class id if it doesn't already
   * exist. Returns false on failure. This must be called before you can
   * use property classes from this factory.
   */
  virtual bool LoadPropertyClassFactory (const char* plugin_id) = 0;

  /**
   * Register a property class factory with this physical layer.
   * It is not needed for apps to call this function as the property class
   * factory itself will do it when it is loaded. To load a property class
   * factory plugin you can use LoadPropertyClassFactory().
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
  virtual size_t GetPropertyClassFactoryCount () const = 0;

  /**
   * Get the specificied property class factory.
   */
  virtual iCelPropertyClassFactory* GetPropertyClassFactory (size_t idx)
  	const = 0;

  /**
   * Find a property class factory by name.
   */
  virtual iCelPropertyClassFactory* FindPropertyClassFactory (
  	const char* name) const = 0;

  /**
   * Convenience function to create a property class from a registered class
   * factory.
   * Note that the reference count of the returned class is not turned up.
   */
  virtual iCelPropertyClass* CreatePropertyClass
      (iCelEntity* entity, const char* propname) = 0;

  /**
   * Convenience function to create a property class with a tag from a
   * registered class factory.
   * Note that the reference count of the returned class is not turned up.
   */
  virtual iCelPropertyClass* CreateTaggedPropertyClass
      (iCelEntity* entity, const char* propname, const char* tagname) = 0;

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
  virtual size_t GetBehaviourLayerCount () const = 0;

  /**
   * Get the specificied behaviour layer.
   */
  virtual iCelBlLayer* GetBehaviourLayer (size_t idx) const = 0;

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
   * The convention on string names is usually 'cel.xxx.yyy'. With 'xxx'
   * equal to the type of string (i.e. 'property'), and 'yyy' the name of the
   * given type (i.e. the property name). An example:
   * 'cel.property.startsector'.
   */
  virtual csStringID FetchStringID (const char* str) = 0;

  /**
   * Find the string that corresponds with some ID. Returns 0
   * if there is no such string.
   */
  virtual const char* FetchString (csStringID id) = 0;

  /**
   * This registers a callback that is called each time when an entity is
   * removed.
   */
  virtual void AddEntityRemoveCallback (iCelEntityRemoveCallback* clback) = 0;

  /**
   * Removes an registered RemoveCallback.
   */
  virtual void RemoveEntityRemoveCallback (
  	iCelEntityRemoveCallback* clback) = 0;

  /**
   * This registers a callback that is called each time when an entity is
   * added.
   */
  virtual void AddNewEntityCallback (iCelNewEntityCallback* clback) = 0;

  /**
   * Removes an registered NewCallback.
   */
  virtual void RemoveNewEntityCallback (iCelNewEntityCallback* clback) = 0;

  //-------------------------------------------------------------------------

  /**
   * Register this listener as one that is interested in getting
   * an event every frame. This will call TickEveryFrame().
   * \param listener is the iCelTimerListener listener.
   * \param where should be CEL_EVENT_PRE, CEL_EVENT_VIEW,
   * or CEL_EVENT_POST.
   */
  virtual void CallbackEveryFrame (iCelTimerListener* listener, int where) = 0;

  /**
   * Register this listener as one that is interested in getting
   * an event in 'delta' milliseconds. This will call Tick().
   * \param listener is the iCelTimerListener listener.
   * \param delta is the time to wait before firing in milliseconds.
   * \param where should be CEL_EVENT_PRE, CEL_EVENT_VIEW,
   * or CEL_EVENT_POST.
   */
  virtual void CallbackOnce (iCelTimerListener* listener, csTicks delta,
  	int where) = 0;

  /**
   * Remove all 'every-frame' callbacks to a specific listener.
   */
  virtual void RemoveCallbackEveryFrame (iCelTimerListener* listener,
  	int where) = 0;

  /**
   * Remove all 'once' callbacks to a specific listener.
   */
  virtual void RemoveCallbackOnce (iCelTimerListener* listener, int where) = 0;

  /**
   * Add an ID scope to the physical layer.
   *
   * Entities are created by propclasses only in the default scope so you can
   * completely manage the ID allocation of your own scopes. 
   */
  virtual int AddScope (csString version, int size) = 0;
};

SCF_VERSION (iCelEntityTracker, 0, 0, 1);

/**
 * This structure maintains a tracker for entities. You can use this
 * to find all entities near some location. Note that this only works
 * on entities with a pcmesh property class.
 */
struct iCelEntityTracker : public iBase
{
  /**
   * Get the name of this tracker.
   */
  virtual const char* GetName () const = 0;

  /**
   * Add an entity to this tracker. This only works on entities that
   * have a pcmesh property class. It will return false if that property
   * class is missing.
   */
  virtual bool AddEntity (iCelEntity* entity) = 0;

  /**
   * Remove an entity from this tracker.
   */
  virtual void RemoveEntity (iCelEntity* entity) = 0;

  /**
   * Remove all entities from this tracker.
   */
  virtual void RemoveEntities () = 0;

  /**
   * Find all entities that are within a certain radius of
   * a given object.
   */
  virtual csPtr<iCelEntityList> FindNearbyEntities (iSector* sector,
  	const csVector3& pos, float radius) = 0;
};

#endif // __CEL_PL_PL__

