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

#ifndef __CEL_PLIMP_PL__
#define __CEL_PLIMP_PL__

#include "csutil/refarr.h"
#include "csutil/weakrefarr.h"
#include "csutil/hash.h"
#include "csutil/set.h"
#include "iutil/comp.h"
#include "iutil/event.h"
#include "iutil/eventh.h"
#include "physicallayer/pl.h"
#include "plugins/stdphyslayer/numreg.h"

#include "celtool/stdparams.h"

struct iObjectRegistry;
struct iEngine;
struct iVirtualClock;
struct iEvent;
class celEntity;
class celEntityTemplate;
struct ccfPropAct;

struct CallbackTiming
{
  size_t pc_idx;
  csTicks time_to_fire;
};

struct CallbackInfo
{
  csSet<size_t> every_frame;
  // To make sure we don't modify the 'every_frame' array while we
  // are busy traversing it we set the 'handling_every_frame' to true
  // during this loop. The 'todo_add_every_frame' and 'todo_del_every_frame'
  // arrays are used to remember the things to add and remove when
  // the loop is done.
  bool handling_every_frame;
  csArray<size_t> todo_add_every_frame;
  csArray<size_t> todo_del_every_frame;
  csArray<CallbackTiming> timed_callbacks;
  CallbackInfo () : handling_every_frame (false) { }
};

/**
 * Implementation of the physical layer.
 */
class celPlLayer : public iCelPlLayer
{
private:
  csRefArray<iCelPropertyClassFactory> pf_list;
  csHash<iCelPropertyClassFactory*,csStringBase> pf_hash;
  csRefArray<iCelBlLayer> bl_list;

  csRefArray<iCelEntity> entities;
  csHash<iCelEntity*,csStringBase> entities_hash;
  bool entities_hash_dirty;

  csHash<csRef<celEntityTemplate>, csStringBase> entity_templates;

  csRefArray<iCelEntityRemoveCallback> removecallbacks;
  csRefArray<iCelNewEntityCallback> newcallbacks;
  csRefArray<iBase> cache;
  iObjectRegistry* object_reg;
  csStringSet string_registry;
  celIDRegistry idlist;
  csRef<iEngine> engine;
  csRef<iVirtualClock> vc;

  // Allow the creation of entities in entity addons.
  bool allow_entity_addon;

  // For timed callbacks:
  csWeakRefArray<iCelTimerListener> weak_listeners;
  // Where is listener in weak_listeners.
  csHash<size_t, csPtrKey<iCelTimerListener> > weak_listeners_hash;
  CallbackInfo callbacks_pre;
  CallbackInfo callbacks_view;
  CallbackInfo callbacks_post;
  int compress_delay;
  void CompressCallbackInfo ();
  // Register a listener to weak ref table and return index.
  size_t WeakRegListener (iCelTimerListener* listener);
  // 'where' is one of the CEL_EVENT_ flags.
  CallbackInfo* GetCBInfo (int where);

  // List of trackers.
  csRefArray<iCelEntityTracker> trackers;

  csRef<celVariableParameterBlock> ConvertTemplateParams (
    const char* entname,
    iCelParameterBlock* act_params, const celEntityTemplateParams& params);

  // Perform an action from a template on a real property class.
  bool PerformActionTemplate (const ccfPropAct& act, iCelPropertyClass* pc,
  	const celEntityTemplateParams& params,
	iCelEntity* ent, iCelEntityTemplate* factory);

  // Used by CreatePropertyClass*() - makes a guess at propfact id
  iCelPropertyClassFactory* FindOrLoadPropfact (const char *propname);

public:
  celPlLayer (iBase* parent);
  virtual ~celPlLayer ();
  bool Initialize (iObjectRegistry* object_reg);

  bool HandleEvent (iEvent& ev);

  iEngine* GetEngine () const { return engine; }

  SCF_DECLARE_IBASE;

  // For managing the names of entities (to find them faster).
  void RemoveEntityName (celEntity* ent);
  void AddEntityName (celEntity* ent);

  virtual csPtr<iCelEntity> CreateEntity ();
  virtual csPtr<iCelEntity> CreateEntityInScope (int scope);
  virtual csPtr<iCelEntity> CreateEntity (uint id);
  virtual csPtr<iCelEntity> CreateEntity (const char* entname,
  	iCelBlLayer* bl, const char* bhname, ...);
  virtual iCelEntity* FindEntity (const char* name);
  /**
   * Remove an entity from the physical layer ID list.
   */
  virtual void RemoveEntity (iCelEntity* entity);
  void RemoveEntityIndex (size_t idx);
  virtual void RemoveEntities ();
  virtual iCelEntity* GetEntity (uint id);
  virtual size_t GetEntityCount () const { return entities.Length (); }
  virtual iCelEntity* GetEntityByIndex (size_t idx) const
  { return entities[idx]; }
  virtual iCelBehaviour* GetBehaviour (uint id);

  virtual iCelEntityTemplate* CreateEntityTemplate (const char* factname);
  virtual void RemoveEntityTemplate (iCelEntityTemplate* entfact);
  virtual void RemoveEntityTemplates ();
  virtual iCelEntityTemplate* FindEntityTemplate (const char* factname);
  virtual size_t GetEntityTemplateCount () const;
  virtual iCelEntityTemplate* GetEntityTemplate (size_t idx) const;
  virtual iCelEntity* CreateEntity (iCelEntityTemplate* factory,
  	const char* name, const celEntityTemplateParams& params);
  virtual iCelEntity* CreateEntity (iCelEntityTemplate* factory,
  	const char* name, ...);

  virtual iCelPropertyClass* CreatePropertyClass (iCelEntity *entity,
	  const char* propname);
  virtual iCelPropertyClass* CreateTaggedPropertyClass (iCelEntity *entity,
	  const char* propname, const char* tagname);
  virtual csPtr<iCelDataBuffer> CreateDataBuffer (long serialnr);

  virtual void AttachEntity (iObject* object, iCelEntity* entity);
  virtual void UnattachEntity (iObject* object, iCelEntity* entity);
  virtual iCelEntity* FindAttachedEntity (iObject* object);
  virtual csPtr<iCelEntityList> FindNearbyEntities (iSector* sector,
  	const csVector3& pos, float radius, bool do_invisible = false);
  virtual csPtr<iCelEntityList> FindNearbyEntities (iSector* sector,
  	const csBox3& box, bool do_invisible = false);
  virtual csPtr<iCelEntityList> FindNearbyEntities (iSector* sector,
  	const csVector3& start, const csVector3& end,
	bool do_invisible = false);
  virtual iCelEntity* GetHitEntity (iCamera* camera, int x, int y);
  virtual csPtr<iCelEntityList> CreateEmptyEntityList ();
  virtual iCelEntityTracker* CreateEntityTracker (const char* name);
  virtual iCelEntityTracker* FindEntityTracker (const char* name);
  virtual void RemoveEntityTracker (iCelEntityTracker* tracker);

  virtual void SetEntityAddonAllowed (bool allow)
  { allow_entity_addon = allow; }
  virtual bool IsEntityAddonAllowed () const
  { return allow_entity_addon; }

  virtual bool LoadPropertyClassFactory (const char* plugin_id);
  virtual void RegisterPropertyClassFactory (iCelPropertyClassFactory* pf);
  virtual void UnregisterPropertyClassFactory (
  	iCelPropertyClassFactory* pf);
  virtual size_t GetPropertyClassFactoryCount () const;
  virtual iCelPropertyClassFactory* GetPropertyClassFactory (size_t idx) const;
  virtual iCelPropertyClassFactory* FindPropertyClassFactory (
  	const char* name) const;

  virtual void RegisterBehaviourLayer (iCelBlLayer* bl);
  virtual void UnregisterBehaviourLayer (iCelBlLayer* bl);
  virtual size_t GetBehaviourLayerCount () const;
  virtual iCelBlLayer* GetBehaviourLayer (size_t idx) const;
  virtual iCelBlLayer* FindBehaviourLayer (const char* name) const;

  virtual void AddEntityRemoveCallback (iCelEntityRemoveCallback* callback);
  virtual void RemoveEntityRemoveCallback (iCelEntityRemoveCallback* callback);
  virtual void AddNewEntityCallback (iCelNewEntityCallback* callback);
  virtual void RemoveNewEntityCallback (iCelNewEntityCallback* callback);
  void FireNewEntityCallbacks (iCelEntity* entity);

  virtual void Cache (iBase* object);
  virtual void Uncache (iBase* object);
  virtual void CleanCache ();
  virtual csStringID FetchStringID (const char* str)
  {
    return string_registry.Request (str);
  }
  virtual const char* FetchString (csStringID id)
  {
    return string_registry.Request (id);
  }

  virtual void CallbackEveryFrame (iCelTimerListener* listener, int where);
  virtual void CallbackOnce (iCelTimerListener* listener,
  	csTicks delta, int where);
  virtual void RemoveCallbackEveryFrame (iCelTimerListener* listener,
  	int where);
  virtual void RemoveCallbackOnce (iCelTimerListener* listener, int where);

  virtual int AddScope (csString impl, int size);
  
  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPlLayer);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;

  // Not an embedded interface to avoid circular references!!!
  class EventHandler : public iEventHandler
  {
  private:
    celPlLayer* parent;

  public:
    EventHandler (celPlLayer* parent)
    {
      SCF_CONSTRUCT_IBASE (0);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler ()
    {
      SCF_DESTRUCT_IBASE ();
    }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
    CS_EVENTHANDLER_NAMES("cel.physicallayer")
    CS_EVENTHANDLER_NIL_CONSTRAINTS
  } *scfiEventHandler;
};

#endif // __CEL_PLIMP_PL__

