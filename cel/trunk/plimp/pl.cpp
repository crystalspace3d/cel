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
#include "plimp/pl.h"
#include "plimp/entity.h"
#include "plimp/message.h"
#include "pl/propfact.h"
#include "pl/propclas.h"
#include "pl/datatype.h"
#include "pl/persist.h"
#include "bl/bl.h"
#include "csutil/flags.h"
#include "csutil/csobject.h"
#include "csutil/csvector.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"
#include "iutil/objreg.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPlLayer)

#if 0
SCF_EXPORT_CLASS_TABLE (plimp)
  SCF_EXPORT_CLASS (celPlLayer, "cel.physicallayer", "CEL Physical Layer")
SCF_EXPORT_CLASS_TABLE_END
#endif

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
}

celPlLayer::~celPlLayer ()
{
  CleanCache ();
  
  // print out entities that aren't deleted properly
  for (CS_ID i=1;i<idlist.Length();i++)
  {
    iCelEntity* entity=(iCelEntity*) idlist.Get(i);
    if (entity)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	  "crystalspace.cel.physicallayer",
	  "Entity with ID %lu and Name %s not destructed yet.",
	  entity->GetID(), entity->GetName());
    }
  }
}

bool celPlLayer::Initialize (iObjectRegistry* object_reg)
{
  celPlLayer::object_reg = object_reg;
  idlist.Clear();
  return true;
}

csPtr<iCelEntity> celPlLayer::CreateEntity ()
{
  CS_ID objid;
  
  csRef<celEntity> entity = csPtr<celEntity> (new celEntity (this));
  csRef<iCelEntity> ientity (SCF_QUERY_INTERFACE (entity, iCelEntity));
  objid = idlist.Register(ientity);
  if (objid == 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.physicallayer",
	"Failed to register new entity in IDList!");
    delete entity;
    return NULL;
  }
  entity->SetEntityID(objid);
  return csPtr<iCelEntity> (ientity);
}

void celPlLayer::RemoveEntity(celEntity *entity)
{
  if (!idlist.Remove(entity->GetEntityID()))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"error while removing Entity with ID %lu (%s)", entity->GetEntityID(),
	entity->GetName());
    return;
  }

  for (int i = 0; i < removecallbacks.Length(); i++)
  {
    iCelEntityRemoveCallback* callback = removecallbacks[i];
    callback->RemoveEntity(&entity->scfiCelEntity);
  }
}

iCelEntity* celPlLayer::GetEntity(CS_ID id)
{
  return (iCelEntity*) idlist.Get(id);
}

iCelPropertyClass* celPlLayer::CreatePropertyClass (iCelEntity *entity,
	const char *propname)
{
  iCelPropertyClassFactory* pf = FindPropertyClassFactory(propname);
  if (!pf)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"No factory for type '%s' registered!", propname);
    return NULL;
  }
  csRef<iCelPropertyClass> pc (pf->CreatePropertyClass());
  if (!pc)
    return NULL;
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
  csVector data;
  long serialnr;

public:
  celDataBuffer (long serialnr)
  {
    SCF_CONSTRUCT_IBASE (NULL);
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
  virtual void SetDataCount (int cnt)
  {
    int i;
    for (i = cnt ; i < data.Length () ; i++)
    {
      celData* cd = (celData*)data[i];
      delete cd;
    }
    int old_cnt = data.Length ();
    data.SetLength (cnt);
    for (i = old_cnt ; i < cnt ; i++)
    {
      celData* cd = new celData ();
      cd->type = CEL_DATA_NONE;
      data[i] = cd;
    }
  }
  virtual int GetDataCount () const
  {
    return data.Length ();
  }
  virtual celData* GetData (int idx) const
  {
    CS_ASSERT (idx >= 0 && idx < data.Length ());
    return (celData*)data[idx];
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
  if (old_entity != NULL) UnattachEntity (object, old_entity);
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
  return NULL;
}

csPtr<iCelEntityList> celPlLayer::FindNearbyEntities (iSector* sector,
	const csVector3& pos, float radius)
{
  // @@@ Some kind of optimization to cache entity lists?
  celEntityList* list = new celEntityList ();
  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
  CS_ASSERT (engine != NULL);
  csRef<iObjectIterator> objit (engine->GetNearbyObjects (sector, pos, radius));
  while (objit->HasNext ())
  {
    iObject* obj = objit->Next ();
    csRef<iMeshWrapper> m (SCF_QUERY_INTERFACE (obj, iMeshWrapper));
    if (m)
    {
      bool invisible = m->GetFlags ().Check (CS_ENTITY_INVISIBLE);
      if (invisible) 
      {
        objit->Next();
        continue;
      }
    }
    iCelEntity* ent = FindAttachedEntity (obj);
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

  iMeshWrapper* sel = sector->HitBeam (vo, end, isect, NULL);
  if (sel)
  {
    iObject* sel_obj = sel->QueryObject ();
    return FindAttachedEntity (sel_obj);
  }
  return NULL;
}

csPtr<iCelEntityList> celPlLayer::CreateEmptyEntityList ()
{
  return new celEntityList ();
}

void celPlLayer::RegisterPropertyClassFactory (iCelPropertyClassFactory* pf)
{
  if (pf_list.Find (pf) != -1) return;
  pf_list.Push (pf);
}

void celPlLayer::UnregisterPropertyClassFactory (
  	iCelPropertyClassFactory* pf)
{
  pf_list.Delete (pf);
}

int celPlLayer::GetPropertyClassFactoryCount () const
{
  return pf_list.Length ();
}

iCelPropertyClassFactory* celPlLayer::GetPropertyClassFactory (int idx) const
{
  CS_ASSERT (idx >= 0 && idx < pf_list.Length ());
  iCelPropertyClassFactory* pf = pf_list[idx];
  return pf;
}

iCelPropertyClassFactory* celPlLayer::FindPropertyClassFactory (
  	const char* name) const
{
  int i;
  for (i = 0 ; i < pf_list.Length () ; i++)
  {
    iCelPropertyClassFactory* pf = pf_list[i];
    if (!strcmp (pf->GetName (), name))
      return pf;
  }
  return NULL;
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
    csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
    if (engine)
	engine->RemoveObject (object);
    cache.Delete (idx);
  }
}

void celPlLayer::CleanCache ()
{
  while (cache.Length () > 0)
    Uncache ((iBase*)cache[0]);
}

void celPlLayer::RegisterBehaviourLayer (iCelBlLayer* bl)
{
  if (bl_list.Find (bl) != -1) return;
  bl_list.Push (bl);
}

void celPlLayer::UnregisterBehaviourLayer (iCelBlLayer* bl)
{
  bl_list.Delete (bl);
}

int celPlLayer::GetBehaviourLayerCount () const
{
  return bl_list.Length ();
}

iCelBlLayer* celPlLayer::GetBehaviourLayer (int idx) const
{
  CS_ASSERT (idx >= 0 && idx < bl_list.Length ());
  iCelBlLayer* bl = bl_list[idx];
  return bl;
}

iCelBlLayer* celPlLayer::FindBehaviourLayer (const char* name) const
{
  int i;
  for (i = 0 ; i < bl_list.Length () ; i++)
  {
    iCelBlLayer* bl = bl_list[i];
    if (!strcmp (bl->GetName (), name))
      return bl;
  }
  return NULL;
}

void celPlLayer::RegisterRemoveCallback (iCelEntityRemoveCallback* callback)
{
  int idx = removecallbacks.Find (callback);
  if (idx != -1) return;
  removecallbacks.Push (callback);
}

void celPlLayer::UnregisterRemoveCallback (iCelEntityRemoveCallback* callback)
{
  if (removecallbacks.Find (callback) == -1) return;
  removecallbacks.Delete (callback);
}
