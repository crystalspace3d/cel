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

#include "cssysdef.h"
#include "csgeom/vector2.h"
#include "csgeom/vector3.h"
#include "plugins/stdphyslayer/pl.h"
#include "plugins/stdphyslayer/entity.h"
#include "plugins/stdphyslayer/message.h"
#include "physicallayer/propfact.h"
#include "physicallayer/propclas.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/bl.h"
#include "csutil/flags.h"
#include "csutil/csobject.h"
#include "csutil/parray.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"
#include "iutil/objreg.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPlLayer)

SCF_IMPLEMENT_IBASE (celPlLayer)
  SCF_IMPLEMENTS_INTERFACE (iCelPlLayer)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPlLayer::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPlLayer::celPlLayer (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  entities_hash_dirty = false;
}

celPlLayer::~celPlLayer ()
{
  CleanCache ();
 
  entities.DeleteAll ();
  entities_hash.DeleteAll ();

  for (size_t j = 0 ; j < removecallbacks.Length() ; j++)
  {
    iCelEntityRemoveCallback* callback = removecallbacks[j];
    delete callback;
  }
}

bool celPlLayer::Initialize (iObjectRegistry* object_reg)
{
  celPlLayer::object_reg = object_reg;
  idlist.Clear ();
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (!engine) return false;	// Engine is required.
  return true;
}

csPtr<iCelEntity> celPlLayer::CreateEntity ()
{
  CS_ID objid;
  
  csRef<celEntity> entity = csPtr<celEntity> (new celEntity (this));
  iCelEntity* ientity = &entity->scfiCelEntity;
  objid = idlist.Register (ientity);
  if (objid == 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.physicallayer",
	"Failed to register new entity in IDList!");
    delete entity;
    return 0;
  }
  entity->SetEntityID (objid);
  entities.Push (ientity);
  ientity->IncRef ();
  return csPtr<iCelEntity> (ientity);
}

csPtr<iCelEntity> celPlLayer::CreateEntity (const char* entname,
	iCelBlLayer* bl, const char* bhname, ...)
{
  csRef<iCelEntity> ent = CreateEntity ();
  if (!ent)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.physicallayer",
	"Error creating entity '%s'!", entname);
    return 0;
  }
  if (entname) ent->SetName (entname);
  if (bl && bhname)
  {
    iCelBehaviour* behave = bl->CreateBehaviour (ent, bhname);
    if (!behave)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.physicallayer",
	"Error creating behaviour '%s' for entity '%s'!", bhname, entname);
      RemoveEntity (ent);
      return 0;
    }
  }

  va_list args;
  va_start (args, bhname);
  char const* pcname = va_arg (args, char*);
  while (pcname != 0)
  {
    iCelPropertyClass* pc = CreatePropertyClass (ent, pcname);
    if (!pc)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.physicallayer",
	"Error creating property class '%s' for entity '%s'!", pcname, entname);
      RemoveEntity (ent);
      return 0;
    }
    pcname = va_arg (args, char*);
  }
  va_end (args);

  return csPtr<iCelEntity> (ent);
}

void celPlLayer::RemoveEntityName (celEntity* ent)
{
  if (!entities_hash_dirty)
    entities_hash.Delete (ent->GetName (), &(ent->scfiCelEntity));
}

void celPlLayer::AddEntityName (celEntity* ent)
{
  if (!entities_hash_dirty)
    entities_hash.Put (ent->GetName (), &(ent->scfiCelEntity));
}

iCelEntity* celPlLayer::FindEntity (const char* name)
{
  if (entities_hash_dirty)
  {
    entities_hash_dirty = false;
    size_t i;
    entities_hash.DeleteAll ();
    for (i = 0 ; i < entities.Length () ; i++)
    {
      if (entities[i]->GetName ())
        entities_hash.Put (entities[i]->GetName (), entities[i]);
    }
  }
  return entities_hash.Get (name,0);
}

void celPlLayer::RemoveEntity (iCelEntity *entity)
{
  if (!idlist.Remove (entity->GetID ()))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"error while removing Entity with ID %u (%s)",
	(unsigned int)entity->GetID(), entity->GetName());
    return;
  }

  for (size_t i = 0; i < removecallbacks.Length(); i++)
  {
    iCelEntityRemoveCallback* callback = removecallbacks[i];
    callback->RemoveEntity (entity);
  }

  if (!entities_hash_dirty && entity->GetName ())
    entities_hash.Delete (entity->GetName (), entity);
  entities.Delete (entity);
}

iCelEntity* celPlLayer::GetEntity (CS_ID id)
{
  return (iCelEntity*) idlist.Get (id);
}

iCelBehaviour* celPlLayer::GetBehaviour (CS_ID id)
{
  iCelEntity* ent = (iCelEntity*) idlist.Get (id);
  if (ent)
  {
    return ent->GetBehaviour ();
  }
  return 0;
}

iCelPropertyClass* celPlLayer::CreatePropertyClass (iCelEntity *entity,
	const char *propname)
{
  iCelPropertyClassFactory* pf = FindPropertyClassFactory (propname);
  if (!pf)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"No factory for type '%s' registered!", propname);
    return 0;
  }
  csRef<iCelPropertyClass> pc (pf->CreatePropertyClass());
  if (!pc)
    return 0;
  entity->GetPropertyClassList()->Add (pc);
  return pc;
}

csPtr<iCelMessage> celPlLayer::CreateMessage (const char* msg_string, ...)
{
  va_list arg;
  va_start (arg, msg_string);
  csRef<celMessage> msg =
    csPtr<celMessage> (new celMessage (msg_string, arg));
  va_end (arg);
  return csPtr<iCelMessage> (msg);
}

// Implementation of iCelDataBuffer.
class celDataBuffer : public iCelDataBuffer
{
private:
  csPDelArray<celData> data;
  long serialnr;

public:
  celDataBuffer (long serialnr)
  {
    SCF_CONSTRUCT_IBASE (0);
    celDataBuffer::serialnr = serialnr;
  }
  virtual ~celDataBuffer ()
  {
    SetDataCount (0);
  }

  SCF_DECLARE_IBASE;

  virtual long GetSerialNumber () const
  {
    return serialnr;
  }
  virtual void SetDataCount (size_t cnt)
  {
    size_t old_cnt = data.Length ();
    data.SetLength (cnt);
    size_t i;
    for (i = old_cnt ; i < cnt ; i++)
    {
      celData* cd = new celData ();
      cd->type = CEL_DATA_NONE;
      data.Put (i, cd);
    }
  }
  virtual size_t GetDataCount () const
  {
    return data.Length ();
  }
  virtual celData* GetData (size_t idx) const
  {
    CS_ASSERT ((idx != csArrayItemNotFound) && idx < data.Length ());
    return data[idx];
  }
};

SCF_IMPLEMENT_IBASE (celDataBuffer)
  SCF_IMPLEMENTS_INTERFACE (iCelDataBuffer)
SCF_IMPLEMENT_IBASE_END

csPtr<iCelDataBuffer> celPlLayer::CreateDataBuffer (long serialnr)
{
  return csPtr<iCelDataBuffer> (new celDataBuffer (serialnr));
}

// Class which is used to attach to an iObject so that
// we can find the iCelEntity again.

SCF_VERSION (celEntityFinder, 0, 0, 1);
class celEntityFinder : public csObject
{
private:
  iCelEntity* entity;

public:
  celEntityFinder (iCelEntity* entity)
  {
    celEntityFinder::entity = entity;
  }
  virtual ~celEntityFinder ()
  { }
  iCelEntity* GetEntity () const { return entity; }
  SCF_DECLARE_IBASE_EXT (csObject);
};

SCF_IMPLEMENT_IBASE_EXT (celEntityFinder)
  SCF_IMPLEMENTS_INTERFACE (celEntityFinder)
SCF_IMPLEMENT_IBASE_EXT_END

void celPlLayer::AttachEntity (iObject* object, iCelEntity* entity)
{
  iCelEntity* old_entity = FindAttachedEntity (object);
  if (old_entity == entity) return;
  if (old_entity != 0) UnattachEntity (object, old_entity);
  csRef<celEntityFinder> cef =
    csPtr<celEntityFinder> (new celEntityFinder (entity));
  cef->SetName ("__entfind__");	// @@@ For debugging mostly.
  csRef<iObject> cef_obj (SCF_QUERY_INTERFACE (cef, iObject));
  object->ObjAdd (cef_obj);
}

void celPlLayer::UnattachEntity (iObject* object, iCelEntity* entity)
{
  csRef<celEntityFinder> cef (CS_GET_CHILD_OBJECT (object, celEntityFinder));
  if (cef)
  {
    if (cef->GetEntity () != entity)
    { return; }
    csRef<iObject> cef_obj (SCF_QUERY_INTERFACE (cef, iObject));
    object->ObjRemove (cef_obj);
  }
}

iCelEntity* celPlLayer::FindAttachedEntity (iObject* object)
{
  csRef<celEntityFinder> cef (CS_GET_CHILD_OBJECT (object, celEntityFinder));
  if (cef)
    return cef->GetEntity ();
  return 0;
}

csPtr<iCelEntityList> celPlLayer::FindNearbyEntities (iSector* sector,
	const csVector3& pos, float radius)
{
  // @@@ Some kind of optimization to cache entity lists?
  celEntityList* list = new celEntityList ();
  csRef<iMeshWrapperIterator> objit = engine->GetNearbyMeshes (
  	sector, pos, radius);
  while (objit->HasNext ())
  {
    iMeshWrapper* m = objit->Next ();
    bool invisible = m->GetFlags ().Check (CS_ENTITY_INVISIBLE);
    if (invisible)       
      continue;      
    iCelEntity* ent = FindAttachedEntity (m->QueryObject ());
    if (ent)
    {
      list->Add (ent);
    }
  }
  return list;
}

iCelEntity* celPlLayer::GetHitEntity (iCamera* camera, int x, int y)
{
  // Vector from (0,0,0) to 'vc' in camera space corresponding to 
  // the point we clicked on.
  csVector3 vc;
  // Vector from 'vo' to 'vw' in world space corresponding to
  // same vector.
  csVector3 vo, vw;

  // Setup perspective vertex, invert mouse Y axis.
  csVector2 p (x, camera->GetShiftY() * 2 - y);

  camera->InvPerspective (p, 1, vc);
  vw = camera->GetTransform ().This2Other (vc);

  iSector* sector = camera->GetSector ();
  vo = camera->GetTransform ().GetO2TTranslation ();
  csVector3 isect, end = vo + (vw - vo) * 60;

  iMeshWrapper* sel = sector->HitBeam (vo, end, isect, 0);
  if (sel)
  {
    iObject* sel_obj = sel->QueryObject ();
    return FindAttachedEntity (sel_obj);
  }
  return 0;
}

csPtr<iCelEntityList> celPlLayer::CreateEmptyEntityList ()
{
  return new celEntityList ();
}

bool celPlLayer::LoadPropertyClassFactory (const char* plugin_id)
{
  csRef<iPluginManager> plugin_mgr =
  	CS_QUERY_REGISTRY (object_reg, iPluginManager);
  csRef<iBase> pf;
  pf = CS_QUERY_PLUGIN_CLASS (plugin_mgr, plugin_id, iBase);
  if (!pf)
  {
    pf = CS_LOAD_PLUGIN_ALWAYS (plugin_mgr, plugin_id);
  }
  if (!pf)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	  "crystalspace.cel.physicallayer",
	  "CEL '%s' property class factory plugin missing!", plugin_id);
    return false;
  }

  return true;
}

void celPlLayer::RegisterPropertyClassFactory (iCelPropertyClassFactory* pf)
{
  if (pf_list.Find (pf) != csArrayItemNotFound) return;
  pf_list.Push (pf);
  pf_hash.Put (pf->GetName (), pf);
}

void celPlLayer::UnregisterPropertyClassFactory (
  	iCelPropertyClassFactory* pf)
{
  pf_hash.Delete (pf->GetName (), pf);
  pf_list.Delete (pf);
}

size_t celPlLayer::GetPropertyClassFactoryCount () const
{
  return pf_list.Length ();
}

iCelPropertyClassFactory* celPlLayer::GetPropertyClassFactory (size_t idx) const
{
  CS_ASSERT ((idx != csArrayItemNotFound) && idx < pf_list.Length ());
  iCelPropertyClassFactory* pf = pf_list[idx];
  return pf;
}

iCelPropertyClassFactory* celPlLayer::FindPropertyClassFactory (
  	const char* name) const
{
  return pf_hash.Get (name,0);
}

void celPlLayer::Cache (iBase* object)
{
  int idx = cache.Find (object);
  if (idx == -1)
    cache.Push (object);
}

void celPlLayer::Uncache (iBase* object)
{
  int idx = cache.Find (object);
  if (idx != -1)
  {
    engine->RemoveObject (object);
    cache.DeleteIndex (idx);
  }
}

void celPlLayer::CleanCache ()
{
  while (cache.Length () > 0)
    Uncache ((iBase*)cache[0]);
}

void celPlLayer::RegisterBehaviourLayer (iCelBlLayer* bl)
{
  if (bl_list.Find (bl) != csArrayItemNotFound) return;
  bl_list.Push (bl);
}

void celPlLayer::UnregisterBehaviourLayer (iCelBlLayer* bl)
{
  bl_list.Delete (bl);
}

size_t celPlLayer::GetBehaviourLayerCount () const
{
  return bl_list.Length ();
}

iCelBlLayer* celPlLayer::GetBehaviourLayer (size_t idx) const
{
  CS_ASSERT ((idx != csArrayItemNotFound) && idx < bl_list.Length ());
  iCelBlLayer* bl = bl_list[idx];
  return bl;
}

iCelBlLayer* celPlLayer::FindBehaviourLayer (const char* name) const
{
  size_t i;
  for (i = 0 ; i < bl_list.Length () ; i++)
  {
    iCelBlLayer* bl = bl_list[i];
    if (!strcmp (bl->GetName (), name))
      return bl;
  }
  return 0;
}

void celPlLayer::RegisterRemoveCallback (iCelEntityRemoveCallback* callback)
{
  int idx = removecallbacks.Find (callback);
  if (idx != -1) return;
  removecallbacks.Push (callback);
}

void celPlLayer::UnregisterRemoveCallback (iCelEntityRemoveCallback* callback)
{
  if (removecallbacks.Find (callback) == csArrayItemNotFound) return;
  removecallbacks.Delete (callback);
}
