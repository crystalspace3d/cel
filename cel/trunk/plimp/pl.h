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

#ifndef __CEL_PLIMP_PL__
#define __CEL_PLIMP_PL__

#include "csutil/csvector.h"
#include "csutil/nobjvec.h"
#include "iutil/comp.h"
#include "pl/pl.h"
#include "plimp/numreg.h"

struct iObjectRegistry;
class celEntity;

CS_DECLARE_OBJECT_VECTOR (csPlLayerCache, iBase);

/**
 * Implementation of the physical layer.
 */
class celPlLayer : public iCelPlLayer
{
private:
  csVector pf_list;
  csVector bl_list;
  csVector removecallbacks;
  csPlLayerCache cache;
  iObjectRegistry* object_reg;
  csStringSet string_registry;
  NumReg idlist;

public:
  celPlLayer (iBase* parent);
  virtual ~celPlLayer ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual iCelEntity* CreateEntity ();
  /**
   * remove an entity from the physical layer ID list.
   * This is called automatically by celEntity destructor.
   */
  void RemoveEntity(celEntity* entity);
  virtual iCelEntity* GetEntity (CS_ID id);
  
  virtual iCelPropertyClass* CreatePropertyClass (iCelEntity *entity,
	  const char* propname);
  virtual iCelMessage* CreateMessage (const char* msg_string, ...);
  virtual iCelDataBuffer* CreateDataBuffer (long serialnr);

  virtual void AttachEntity (iObject* object, iCelEntity* entity);
  virtual void UnattachEntity (iObject* object, iCelEntity* entity);
  virtual iCelEntity* FindAttachedEntity (iObject* object);
  virtual iCelEntityList* FindNearbyEntities (iSector* sector,
  	const csVector3& pos, float radius);
  virtual iCelEntity* GetHitEntity (iCamera* camera, int x, int y);

  virtual void RegisterPropertyClassFactory (iCelPropertyClassFactory* pf);
  virtual void UnregisterPropertyClassFactory (
  	iCelPropertyClassFactory* pf);
  virtual int GetPropertyClassFactoryCount () const;
  virtual iCelPropertyClassFactory* GetPropertyClassFactory (int idx) const;
  virtual iCelPropertyClassFactory* FindPropertyClassFactory (
  	const char* name) const;

  virtual void RegisterBehaviourLayer (iCelBlLayer* bl);
  virtual void UnregisterBehaviourLayer (iCelBlLayer* bl);
  virtual int GetBehaviourLayerCount () const;
  virtual iCelBlLayer* GetBehaviourLayer (int idx) const;
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

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPlLayer);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_PLIMP_PL__

