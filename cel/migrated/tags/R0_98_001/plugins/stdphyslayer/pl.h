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
#include "csutil/hashhandlers.h"
#include "csutil/hash.h"
#include "iutil/comp.h"
#include "iutil/event.h"
#include "iutil/eventh.h"
#include "physicallayer/pl.h"
#include "plugins/stdphyslayer/numreg.h"

struct iObjectRegistry;
struct iEngine;
struct iVirtualClock;
struct iEvent;
class celEntity;

struct CallbackPCTiming
{
  size_t pc_idx;
  csTicks time_to_fire;
};
struct CallbackPCInfo
{
  csSet<size_t> every_frame;
  csArray<CallbackPCTiming> timed_callbacks;
};

/**
 * Implementation of the physical layer.
 */
class celPlLayer : public iCelPlLayer
{
private:
  csRefArray<iCelPropertyClassFactory> pf_list;
  csHash<iCelPropertyClassFactory*,csStrKey,csConstCharHashKeyHandler> pf_hash;
  csRefArray<iCelBlLayer> bl_list;

  csRefArray<iCelEntity> entities;
  csHash<iCelEntity*,csStrKey,csConstCharHashKeyHandler> entities_hash;
  bool entities_hash_dirty;

  csArray<iCelEntityRemoveCallback*> removecallbacks;
  csRefArray<iBase> cache;
  iObjectRegistry* object_reg;
  csStringSet string_registry;
  NumReg idlist;
  csRef<iEngine> engine;
  csRef<iVirtualClock> vc;

  // For timed callbacks:
  csWeakRefArray<iCelPropertyClass> weak_pcs;	// Weak refs to pcs.
  csHash<size_t, iCelPropertyClass*> weak_pcs_hash;// Where is pc in weak_pcs.
  CallbackPCInfo callbacks_pre;
  CallbackPCInfo callbacks_process;
  CallbackPCInfo callbacks_post;
  CallbackPCInfo callbacks_final;
  int compress_delay;
  void CompressCallbackPCInfo ();
  // Register a PC to weak ref table and return index.
  size_t WeakRegPC (iCelPropertyClass* pc);
  // 'where' is one of the cscmdProcess flags.
  CallbackPCInfo* GetCBInfo (int where);

public:
  celPlLayer (iBase* parent);
  virtual ~celPlLayer ();
  bool Initialize (iObjectRegistry* object_reg);

  bool HandleEvent (iEvent& ev);

  SCF_DECLARE_IBASE;

  // For managing the names of entities (to find them faster).
  void RemoveEntityName (celEntity* ent);
  void AddEntityName (celEntity* ent);

  virtual csPtr<iCelEntity> CreateEntity ();
  virtual csPtr<iCelEntity> CreateEntity (const char* entname,
  	iCelBlLayer* bl, const char* bhname, ...);
  virtual iCelEntity* FindEntity (const char* name);
  /**
   * Remove an entity from the physical layer ID list.
   */
  virtual void RemoveEntity (iCelEntity* entity);
  virtual iCelEntity* GetEntity (CS_ID id);
  virtual iCelBehaviour* GetBehaviour (CS_ID id);
  
  virtual iCelPropertyClass* CreatePropertyClass (iCelEntity *entity,
	  const char* propname);
  virtual csPtr<iCelMessage> CreateMessage (const char* msg_string, ...);
  virtual csPtr<iCelDataBuffer> CreateDataBuffer (long serialnr);

  virtual void AttachEntity (iObject* object, iCelEntity* entity);
  virtual void UnattachEntity (iObject* object, iCelEntity* entity);
  virtual iCelEntity* FindAttachedEntity (iObject* object);
  virtual csPtr<iCelEntityList> FindNearbyEntities (iSector* sector,
  	const csVector3& pos, float radius);
  virtual iCelEntity* GetHitEntity (iCamera* camera, int x, int y);
  virtual csPtr<iCelEntityList> CreateEmptyEntityList ();

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

  virtual void RegisterRemoveCallback (iCelEntityRemoveCallback* callback);
  virtual void UnregisterRemoveCallback (iCelEntityRemoveCallback* callback);

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

  virtual void CallbackPCEveryFrame (iCelPropertyClass* pc, int where);
  virtual void CallbackPCOnce (iCelPropertyClass* pc, csTicks delta, int where);
  virtual void RemoveCallbackPCEveryFrame (iCelPropertyClass* pc, int where);
  virtual void RemoveCallbackPCOnce (iCelPropertyClass* pc, int where);

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
    virtual ~EventHandler () { }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
};

#endif // __CEL_PLIMP_PL__
