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
#include "csutil/scanstr.h"
#include "csgeom/vector2.h"
#include "csgeom/vector3.h"
#include "plugins/stdphyslayer/pl.h"
#include "plugins/stdphyslayer/entity.h"
#include "plugins/stdphyslayer/etracker.h"
#include "plugins/stdphyslayer/entitytpl.h"
#include "physicallayer/propfact.h"
#include "physicallayer/propclas.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/bl.h"
#include "csutil/flags.h"
#include "csutil/csobject.h"
#include "csutil/parray.h"
#include "csutil/cseventq.h"
#include "csutil/cfgmgr.h"
#include "csutil/event.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"
#include "iutil/objreg.h"
#include "iutil/plugin.h"
#include "iutil/virtclk.h"
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

SCF_IMPLEMENT_IBASE (celPlLayer::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPlLayer::celPlLayer (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);

  entities_hash_dirty = false;
  scfiEventHandler = 0;

  compress_delay = 1000;
  allow_entity_addon = true;
}

celPlLayer::~celPlLayer ()
{
  CleanCache ();

  entities.DeleteAll ();
  entities_hash.DeleteAll ();

  if (scfiEventHandler)
  {
    csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q != 0)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celPlLayer::HandleEvent (iEvent& ev)
{
  int where;
  if (ev.Name == csevPreProcess (object_reg))
    where = CEL_EVENT_PRE;
  else if (ev.Name == csevProcess (object_reg))
    where = CEL_EVENT_VIEW;
  else if (ev.Name == csevPostProcess (object_reg))
    where = CEL_EVENT_POST;
  else
    return false;

  CallbackInfo* cbinfo = GetCBInfo (where);
  if (!cbinfo) return false;

  // First fire all property classes that must be fired every frame.
  bool compress = false;
  csSet<size_t>::GlobalIterator it = cbinfo->every_frame.GetIterator ();
  csArray<iCelTimerListener*> pcs;
  while (it.HasNext ())
  {
    size_t pc_idx = it.Next ();
    iCelTimerListener* listener = weak_listeners[pc_idx];
    if (listener)
      pcs.Push (listener);
    else
      compress = true;
  }
  size_t i;
  for (i = 0 ; i < pcs.Length () ; i++)
    pcs[i]->TickEveryFrame ();

  // Then fire all property classes that are interested in receiving
  // events if the alloted time has exceeded. The property classes
  // are added in reverse order so that the top of the array is the
  // one that will fire first.
  csTicks current_time = vc->GetCurrentTicks ();
  csArray<CallbackTiming>& cbs = cbinfo->timed_callbacks;
  pcs.SetLength (0);
  while (cbs.Length () > 0 && cbs.Top ().time_to_fire <= current_time)
  {
    CallbackTiming pcfire = cbs.Pop ();
    iCelTimerListener* listener = weak_listeners[pcfire.pc_idx];
    if (listener)
      pcs.Push (listener);
    else
      compress = true;
  }
  for (i = 0 ; i < pcs.Length () ; i++)
    pcs[i]->TickOnce ();

  if (compress) CompressCallbackInfo ();

  return true;
}

bool celPlLayer::Initialize (iObjectRegistry* object_reg)
{
  celPlLayer::object_reg = object_reg;
  idlist.Clear ();
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (!engine) return false;	// Engine is required.

  scfiEventHandler = new EventHandler (this);
  csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  csEventID esub[] = { 
    csevPreProcess (object_reg),   // this goes away...
    csevPostProcess (object_reg),  // this goes away...
    csevProcess (object_reg),      // this goes away...
    csevFrame (object_reg),        // this replaces the above!
    CS_EVENTLIST_END 
  };
  q->RegisterListener (scfiEventHandler, esub);

  return true;
}

int celPlLayer::AddScope (csString version, int size)
{
  return (int)idlist.AddScope (version, size);
}

csPtr<iCelEntity> celPlLayer::CreateEntity ()
{
  return CreateEntityInScope ((int)idlist.DefaultScope);
}

csPtr<iCelEntity> celPlLayer::CreateEntityInScope (int scope)
{
  uint objid;

  csRef<celEntity> entity = csPtr<celEntity> (new celEntity (this));
  iCelEntity* ientity = &entity->scfiCelEntity;
  objid = idlist.Register (ientity, scope);
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

  FireNewEntityCallbacks (ientity);

  return csPtr<iCelEntity> (ientity);
}

csPtr<iCelEntity> celPlLayer::CreateEntity (uint entity_id)
{
  csRef<celEntity> entity = csPtr<celEntity> (new celEntity (this));
  iCelEntity* ientity = &entity->scfiCelEntity;

  entity->SetEntityID (entity_id);
  idlist.RegisterWithID (ientity, entity_id);
  entities.Push(ientity);
  ientity->IncRef();

  FireNewEntityCallbacks (ientity);

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

iCelEntityTemplate* celPlLayer::CreateEntityTemplate (const char* factname)
{
  csRef<celEntityTemplate> fact;
  fact.AttachNew (new celEntityTemplate ());
  fact->SetName (factname);
  entity_templates.Put (factname, fact);
  return &(((celEntityTemplate*)fact)->scfiCelEntityTemplate);
}

void celPlLayer::RemoveEntityTemplate (iCelEntityTemplate* entfact)
{
  entity_templates.DeleteAll (entfact->GetName ());
}

void celPlLayer::RemoveEntityTemplates ()
{
  entity_templates.DeleteAll ();
}

iCelEntityTemplate* celPlLayer::FindEntityTemplate (const char* factname)
{
  csRef<celEntityTemplate> f = entity_templates.Get (factname, 0);
  return f ? &(f->scfiCelEntityTemplate) : 0;
}

csRef<celVariableParameterBlock> celPlLayer::ConvertTemplateParams (
    iCelParameterBlock* act_params, const celEntityTemplateParams& params)
{
  csRef<celVariableParameterBlock> converted_params;
  if (act_params)
  {
    converted_params.AttachNew (new celVariableParameterBlock ());
    size_t k;
    for (k = 0 ; k < act_params->GetParameterCount () ; k++)
    {
      csStringID id;
      celDataType t;
      const char* parname = act_params->GetParameter (k, id, t);
      const celData* par = act_params->GetParameter (id);
      converted_params->SetParameterDef (k, id, parname);
      if (t == CEL_DATA_PARAMETER)
      {
	const char* value = params.Get (par->value.par.parname->GetData (), 0);
	celData& converted_par = converted_params->GetParameter (k);
	switch (par->value.par.partype)
	{
	  case CEL_DATA_LONG:
	    {
	      long l; if (value) sscanf (value, "%ld", &l); else l = 0;
	      converted_par.Set ((int32)l);
	    }
	    break;
	  case CEL_DATA_FLOAT:
	    {
	      float f; if (value) sscanf (value, "%f", &f); else f = 0;
	      converted_par.Set (f);
	    }
	    break;
	  case CEL_DATA_BOOL:
	    {
	      bool b;
	      if (value) csScanStr (value, "%b", &b); else b = false;
	      converted_par.Set (b);
	    }
	    break;
	  case CEL_DATA_STRING:
	    converted_par.Set (value);
	    break;
	  case CEL_DATA_VECTOR2:
	    {
	      csVector2 v;
	      if (value)
	        sscanf (value, "%f,%f", &v.x, &v.y);
	      else
	        v.Set (0, 0);
	      converted_par.Set (v);
	    }
	    break;
	  case CEL_DATA_VECTOR3:
	    {
	      csVector3 v;
	      if (value)
	        sscanf (value, "%f,%f,%f", &v.x, &v.y, &v.z);
	      else
	        v.Set (0, 0, 0);
	      converted_par.Set (v);
	    }
	    break;
	  case CEL_DATA_COLOR:
	    {
	      csColor v;
	      if (value)
	        sscanf (value, "%f,%f,%f", &v.red, &v.green, &v.blue);
	      else
	        v.Set (0, 0, 0);
	      converted_par.Set (v);
	    }
 	    break;
	  case CEL_DATA_ENTITY:
	    {
	      iCelEntity* ent = value ? FindEntity (value) : 0;
	      converted_par.Set (ent);
	    }
	    break;
	  default: break;
	}
      }
      else
      {
	converted_params->GetParameter (k) = *par;
      }
    }
  }
  return converted_params;
}

bool celPlLayer::PerformActionTemplate (const ccfPropAct& act,
    	iCelPropertyClass* pc,
  	const celEntityTemplateParams& params,
	iCelEntity* ent, iCelEntityTemplate* factory)
{
  csRef<celVariableParameterBlock> converted_params = ConvertTemplateParams (
    act.params, params);
  if (!pc->PerformAction (act.id, converted_params))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.physicallayer",
	  "Error performing action in '%s' for entity '%s' from factory '%s'!",
		pc->GetName (), ent->GetName (), factory->GetName ());
    RemoveEntity (ent);
    return false;
  }
  return true;
}

iCelEntity* celPlLayer::CreateEntity (iCelEntityTemplate* factory,
  	const char* name, ...)
{
  celEntityTemplateParams params;
  va_list args;
  va_start (args, name);
  char const* par = va_arg (args, char*);
  while (par != 0)
  {
    char const* val = va_arg (args, char*);
    params.Put (par, val);
    par = va_arg (args, char*);
  }
  va_end (args);
  return CreateEntity (factory, name, params);
}

iCelEntity* celPlLayer::CreateEntity (iCelEntityTemplate* factory,
  	const char* name, const celEntityTemplateParams& params)
{
  celEntityTemplate* cfact = ((celEntityTemplate::CelEntityTemplate*)
  	factory)->GetCelEntityTemplate ();
  csRef<iCelBlLayer> bl;
  if (cfact->GetLayer ())
  {
    bl = FindBehaviourLayer (cfact->GetLayer ());
    if (!bl)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"Can't find behaviour layer '%s' for entity '%s' from factory '%s'!",
		cfact->GetLayer (), name, factory->GetName ());
      return 0;
    }
  }
  else
  {
    bl = CS_QUERY_REGISTRY (object_reg, iCelBlLayer);
    if (!bl && cfact->GetBehaviour ())
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"Can't find default behaviour layer for entity '%s' from factory '%s'!",
		name, factory->GetName ());
      return 0;
    }
  }
  csRef<iCelEntity> ent = CreateEntity (name, 0, 0, CEL_PROPCLASS_END);

  const csRefArray<celPropertyClassTemplate>& pcs = cfact->GetPropClasses ();
  size_t i;
  for (i = 0 ; i < pcs.Length () ; i++)
  {
    celPropertyClassTemplate* pcc = pcs[i];
    const char* pcname = pcc->GetName ();
    iCelPropertyClass* pc = ent->GetPropertyClassList ()->FindByNameAndTag (
    	pcname, 0);
    if (!pc) pc = CreatePropertyClass (ent, pcname);
    if (!pc)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.physicallayer",
	"Error creating property class '%s' for entity '%s from factory '%s''!",
		pcname, name, factory->GetName ());
      RemoveEntity (ent);
      return 0;
    }
    const csArray<ccfPropAct>& props = pcc->GetProperties ();
    size_t j;
    for (j = 0 ; j < props.Length () ; j++)
    {
      const celData& d = props[j].data;
      csStringID id = props[j].id;
      bool rc = false;
      switch (d.type)
      {
        case CEL_DATA_NONE:
	  {
	    // Action.
	    if (!PerformActionTemplate (props[j], pc, params, ent, factory))
	      return 0;
	    rc = true;
	  }
	  break;
        case CEL_DATA_LONG: rc = pc->SetProperty (id, (long)d.value.l); break;
        case CEL_DATA_FLOAT: rc = pc->SetProperty (id, d.value.f); break;
        case CEL_DATA_BOOL: rc = pc->SetProperty (id, d.value.bo); break;
        case CEL_DATA_STRING:
	  rc = pc->SetProperty (id, d.value.s->GetData ());
	  break;
        case CEL_DATA_VECTOR2:
	  {
	    csVector2 v;
	    v.x = d.value.v.x;
	    v.y = d.value.v.y;
	    rc = pc->SetProperty (id, v);
	  }
	  break;
        case CEL_DATA_VECTOR3:
	  {
	    csVector3 v;
	    v.x = d.value.v.x;
	    v.y = d.value.v.y;
	    v.z = d.value.v.z;
	    rc = pc->SetProperty (id, v);
	  }
	  break;
        case CEL_DATA_COLOR:
	  {
	    csColor v;
	    v.red = d.value.col.red;
	    v.green = d.value.col.green;
	    v.blue = d.value.col.blue;
	    rc = pc->SetProperty (id, v);
	  }
	  break;
        case CEL_DATA_PCLASS: rc = pc->SetProperty (id, d.value.pc); break;
        case CEL_DATA_ENTITY: rc = pc->SetProperty (id, d.value.ent); break;
	case CEL_DATA_PARAMETER:
	  {
	    const char* parname = d.value.par.parname->GetData ();
	    const char* value = params.Get (parname, 0);
	    if (value)
	    {
	      switch (d.value.par.partype)
	      {
		case CEL_DATA_LONG:
		  {
		    long l; sscanf (value, "%ld", &l);
		    rc = pc->SetProperty (id, l);
		  }
		  break;
		case CEL_DATA_FLOAT:
		  {
		    float f; sscanf (value, "%f", &f);
		    rc = pc->SetProperty (id, f);
		  }
		  break;
		case CEL_DATA_BOOL:
		  {
		    bool b;
		    csScanStr (value, "%b", &b);
		    rc = pc->SetProperty (id, b);
		  }
		  break;
		case CEL_DATA_STRING:
		  rc = pc->SetProperty (id, value);
		  break;
		case CEL_DATA_VECTOR2:
		  {
		    csVector2 v;
		    sscanf (value, "%f,%f", &v.x, &v.y);
		    rc = pc->SetProperty (id, v);
		  }
		  break;
		case CEL_DATA_VECTOR3:
		  {
		    csVector3 v;
		    sscanf (value, "%f,%f,%f", &v.x, &v.y, &v.z);
		    rc = pc->SetProperty (id, v);
		  }
		  break;
		case CEL_DATA_COLOR:
		  {
		    csColor v;
		    sscanf (value, "%f,%f,%f", &v.red, &v.green, &v.blue);
		    rc = pc->SetProperty (id, v);
		  }
		  break;
		case CEL_DATA_ENTITY:
		  {
		    iCelEntity* ent = FindEntity (value);
		    if (ent)
		      rc = pc->SetProperty (id, ent);
		  }
		  break;
	        default: rc = false; break;
	      }
	    }
	  }
	  break;
	default: rc = false; break;
      }
      if (!rc)
      {
        csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.physicallayer",
	  "Error setting property in '%s' for entity '%s from factory '%s''!",
		pcname, name, factory->GetName ());
        RemoveEntity (ent);
        return 0;
      }
    }
  }

  if (bl && cfact->GetBehaviour ())
  {
    iCelBehaviour* behave = bl->CreateBehaviour (ent, cfact->GetBehaviour ());
    if (!behave)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.physicallayer",
	"Error creating behaviour '%s' for entity '%s'!",
	cfact->GetBehaviour (), name);
      RemoveEntity (ent);
      return 0;
    }
  }
  if (ent->GetBehaviour ())
  {
    const csArray<ccfMessage>& messages = cfact->GetMessages ();
    for (i = 0 ; i < messages.Length () ; i++)
    {
      const ccfMessage& msg = messages[i];
      celData ret;
      csRef<celVariableParameterBlock> converted_params = ConvertTemplateParams
      	(msg.params, params);
      ent->GetBehaviour ()->SendMessage (msg.msgid, 0, ret, converted_params);
    }
  }

  return ent;
}

void celPlLayer::RemoveEntityIndex (size_t idx)
{
  if (idx == csArrayItemNotFound) return;

  // To avoid a problem where the destructor of an entity can
  // cause a call to RemoveEntity() itself (and cause entities.Delete()
  // to be nested inside another entities.Delete()) we first add an
  // extra reference to the entity to prevent the entity from being
  // deleted in the call to entities.Delete().
  csRef<iCelEntity> entity = entities[idx];

  // First register this entity from all trackers.
  size_t i;
  for (i = 0 ; i < trackers.Length () ; i++)
    trackers[i]->RemoveEntity (entity);

  if (!idlist.Remove (entity->GetID ()))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"Error while removing Entity with ID %u (%s)",
	(unsigned int)entity->GetID(), entity->GetName());
    return;
  }

  for (i = 0; i < removecallbacks.Length(); i++)
  {
    iCelEntityRemoveCallback* callback = removecallbacks[i];
    callback->RemoveEntity (entity);
  }

  if (!entities_hash_dirty && entity->GetName ())
    entities_hash.Delete (entity->GetName (), entity);
  entities.DeleteIndex (idx);
}

void celPlLayer::RemoveEntity (iCelEntity *entity)
{
  RemoveEntityIndex (entities.Find (entity));
}

void celPlLayer::RemoveEntities ()
{
#ifdef CS_DEBUG
  // We use weakrefs to test if the entities are really gone after
  // calling RemoveEntities(). This assumes that the caller has no refs
  // on his own so this is only a warning.
  size_t i;
  csWeakRefArray<iCelEntity> weakrefs;
  for (i = 0 ; i < entities.Length () ; i++)
    weakrefs.Push (entities[i]);
#endif
  while (entities.Length () > 0)
  {
    RemoveEntityIndex (entities.Length ()-1);
  }
#ifdef CS_DEBUG
  for (i = 0 ; i < weakrefs.Length () ; i++)
    if (weakrefs[i] != 0)
    {
      printf ("Entity '%s' not removed (ref count %d)\n",
      	weakrefs[i]->GetName (), weakrefs[i]->GetRefCount ());
      fflush (stdout);
    }
#endif
}

iCelEntity* celPlLayer::GetEntity (uint id)
{
  return (iCelEntity*) idlist.Get (id);
}

iCelBehaviour* celPlLayer::GetBehaviour (uint id)
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

iCelPropertyClass* celPlLayer::CreateTaggedPropertyClass (iCelEntity *entity,
	const char *propname, const char* tagname)
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
  pc->SetTag (tagname);
  entity->GetPropertyClassList()->Add (pc);
  return pc;
}

// Implementation of iCelDataBuffer.
class celDataBuffer : public iCelDataBuffer
{
private:
  csArray<celData> data;
  long serialnr;
  size_t posidx;

public:
  celDataBuffer (long serialnr)
  {
    SCF_CONSTRUCT_IBASE (0);
    celDataBuffer::serialnr = serialnr;
    posidx = 0;
  }
  virtual ~celDataBuffer ()
  {
    SCF_DESTRUCT_IBASE ();
  }

  SCF_DECLARE_IBASE;

  virtual long GetSerialNumber () const
  {
    return serialnr;
  }
  virtual size_t GetDataCount () const
  {
    return data.Length ();
  }
  virtual celData* GetData ()
  {
    if (posidx >= data.Length ()) return 0;
    posidx++;
    return &data[posidx-1];
  }
  virtual celData* GetData (size_t idx)
  {
    CS_ASSERT ((idx != csArrayItemNotFound) && idx < data.Length ());
    return &data[idx];
  }
  virtual void Reset ()
  {
    posidx = 0;
  }
  virtual celData* AddData ()
  {
    size_t idx = data.Push (celData ());
    return &data[idx];
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
struct celEntityFinder : public csObject
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
	const csBox3& box, bool do_invisible)
{
  // @@@ Some kind of optimization to cache entity lists?
  celEntityList* list = new celEntityList ();
  csRef<iMeshWrapperIterator> objit = engine->GetNearbyMeshes (
  	sector, box);
  while (objit->HasNext ())
  {
    iMeshWrapper* m = objit->Next ();
    if (!do_invisible)
    {
      bool invisible = m->GetFlags ().Check (CS_ENTITY_INVISIBLE);
      if (invisible)
        continue;
    }
    iCelEntity* ent = FindAttachedEntity (m->QueryObject ());
    if (ent)
    {
      list->Add (ent);
    }
  }
  return list;
}

csPtr<iCelEntityList> celPlLayer::FindNearbyEntities (iSector* sector,
	const csVector3& start, const csVector3& end, bool do_invisible)
{
  // @@@ Some kind of optimization to cache entity lists?
  celEntityList* list = new celEntityList ();
  csRef<iMeshWrapperIterator> objit = engine->GetNearbyMeshes (
  	sector, start, end);
  while (objit->HasNext ())
  {
    iMeshWrapper* m = objit->Next ();
    if (!do_invisible)
    {
      bool invisible = m->GetFlags ().Check (CS_ENTITY_INVISIBLE);
      if (invisible)
        continue;
    }
    iCelEntity* ent = FindAttachedEntity (m->QueryObject ());
    if (ent)
    {
      list->Add (ent);
    }
  }
  return list;
}

csPtr<iCelEntityList> celPlLayer::FindNearbyEntities (iSector* sector,
	const csVector3& pos, float radius, bool do_invisible)
{
  // @@@ Some kind of optimization to cache entity lists?
  celEntityList* list = new celEntityList ();
  csRef<iMeshWrapperIterator> objit = engine->GetNearbyMeshes (
  	sector, pos, radius);
  while (objit->HasNext ())
  {
    iMeshWrapper* m = objit->Next ();
    if (!do_invisible)
    {
      bool invisible = m->GetFlags ().Check (CS_ENTITY_INVISIBLE);
      if (invisible)
        continue;
    }
    iCelEntity* ent = FindAttachedEntity (m->QueryObject ());
    if (ent)
    {
      list->Add (ent);
    }
  }
  return list;
}

iCelEntityTracker* celPlLayer::CreateEntityTracker (const char* name)
{
  celEntityTracker* tr = new celEntityTracker (this, name);
  trackers.Push (tr);
  return tr;
}

iCelEntityTracker* celPlLayer::FindEntityTracker (const char* name)
{
  size_t i;
  for (i = 0 ; i < trackers.Length () ; i++)
    if (strcmp (name, trackers[i]->GetName ()) == 0)
      return trackers[i];
  return 0;
}

void celPlLayer::RemoveEntityTracker (iCelEntityTracker* tracker)
{
  trackers.Delete (tracker);
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
  size_t idx = cache.Find (object);
  if (idx == csArrayItemNotFound)
    cache.Push (object);
}

void celPlLayer::Uncache (iBase* object)
{
  size_t idx = cache.Find (object);
  if (idx != csArrayItemNotFound)
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

void celPlLayer::AddEntityRemoveCallback (iCelEntityRemoveCallback* callback)
{
  size_t idx = removecallbacks.Find (callback);
  if (idx != csArrayItemNotFound) return;
  removecallbacks.Push (callback);
}

void celPlLayer::RemoveEntityRemoveCallback (iCelEntityRemoveCallback* callback)
{
  if (removecallbacks.Find (callback) == csArrayItemNotFound) return;
  removecallbacks.Delete (callback);
}

void celPlLayer::AddNewEntityCallback (iCelNewEntityCallback* callback)
{
  size_t idx = newcallbacks.Find (callback);
  if (idx != csArrayItemNotFound) return;
  newcallbacks.Push (callback);
}

void celPlLayer::RemoveNewEntityCallback (iCelNewEntityCallback* callback)
{
  if (newcallbacks.Find (callback) == csArrayItemNotFound) return;
  newcallbacks.Delete (callback);
}

void celPlLayer::FireNewEntityCallbacks (iCelEntity* entity)
{
  size_t i = newcallbacks.Length ();
  while (i > 0)
  {
    i--;
    iCelNewEntityCallback* callback = newcallbacks[i];
    callback->NewEntity (entity);
  }
}

CallbackInfo* celPlLayer::GetCBInfo (int where)
{
  switch (where)
  {
    case CEL_EVENT_PRE: return &callbacks_pre;
    case CEL_EVENT_VIEW: return &callbacks_view;
    case CEL_EVENT_POST: return &callbacks_post;
    default: return 0;
  }
}

struct pc_idx
{
  iCelTimerListener* listener;
  size_t idx;
  pc_idx (iCelTimerListener* p_listener, size_t p_idx)
  	: listener (p_listener), idx (p_idx) { }
};

void celPlLayer::CompressCallbackInfo ()
{
  compress_delay--;
  if (compress_delay > 0) return;
  compress_delay = 1000;

  // First copy all weak ref PC's that are still relevant to 'store' and
  // remember original index.
  size_t orig_pcs_length = weak_listeners.Length ();
  csArray<pc_idx> store;
  size_t i;
  for (i = 0 ; i < orig_pcs_length ; i++)
    if (weak_listeners[i])
      store.Push (pc_idx (weak_listeners[i], i));

  // Delete the weak array and build it again.
  weak_listeners.DeleteAll ();
  weak_listeners_hash.DeleteAll ();
  for (i = 0 ; i < store.Length () ; i++)
  {
    weak_listeners.Push (store[i].listener);
    weak_listeners_hash.Put (store[i].listener, i);
  }

  // Now create a reverse table to map from original index to new index.
  size_t* map = new size_t[orig_pcs_length];
  memset (map, 0xffffffff, sizeof (size_t) * orig_pcs_length);
  for (i = 0 ; i < store.Length () ; i++)
    map[store[i].idx] = i;

  // Now change the indices in all lists.
  int p[3] = { CEL_EVENT_PRE, CEL_EVENT_VIEW, CEL_EVENT_POST };
  int where;
  for (where = 0 ; where < 3 ; where++)
  {
    CallbackInfo* cbinfo = GetCBInfo (p[where]);
    csSet<size_t> newset;
    csSet<size_t>::GlobalIterator it = cbinfo->every_frame.GetIterator ();
    while (it.HasNext ())
    {
      size_t oldidx = it.Next ();
      size_t newidx = map[oldidx];
      if (newidx != (size_t)~0)
        newset.Add (newidx);
    }
    cbinfo->every_frame = newset;

    size_t i;
    for (i = 0 ; i < cbinfo->timed_callbacks.Length () ; )
    {
      size_t newidx = map[cbinfo->timed_callbacks[i].pc_idx];
      if (newidx == (size_t)~0)
      {
        cbinfo->timed_callbacks.DeleteIndex (i);
      }
      else
      {
        cbinfo->timed_callbacks[i].pc_idx = newidx;
	i++;
      }
    }
  }

  delete[] map;
}

size_t celPlLayer::WeakRegListener (iCelTimerListener* listener)
{
  size_t pc_idx = weak_listeners_hash.Get (listener, (size_t)~0);
  if (pc_idx == (size_t)~0)
  {
    // Not found yet. Add it.
    pc_idx = weak_listeners.Push (listener);
    weak_listeners_hash.Put (listener, pc_idx);
  }
  else if (weak_listeners[pc_idx] == 0)
  {
    // The pc_idx is there but the listener has been deleted in the mean time.
    // In that case we will update the listener in the table.
    weak_listeners[pc_idx] = listener;
  }
  return pc_idx;
}

void celPlLayer::CallbackEveryFrame (iCelTimerListener* listener, int where)
{
  CallbackInfo* cbinfo = GetCBInfo (where);
  if (!cbinfo) return;
  size_t pc_idx = WeakRegListener (listener);
  cbinfo->every_frame.Add (pc_idx);
}

static int CompareTimedCallback (CallbackTiming const& r1,
	CallbackTiming const& r2)
{
  // Reverse sort!
  if (r1.time_to_fire < r2.time_to_fire) return 1;
  else if (r2.time_to_fire < r1.time_to_fire) return -1;
  else return 0;
}

void celPlLayer::CallbackOnce (iCelTimerListener* listener, csTicks delta,
	int where)
{
  CallbackInfo* cbinfo = GetCBInfo (where);
  if (!cbinfo) return;
  CallbackTiming cbtime;
  size_t pc_idx = WeakRegListener (listener);
  cbtime.pc_idx = pc_idx;
  cbtime.time_to_fire = vc->GetCurrentTicks () + delta;

  // We insert the lowest times last so that we can easily Pop them
  // from there.
  cbinfo->timed_callbacks.InsertSorted (cbtime, CompareTimedCallback);
}

void celPlLayer::RemoveCallbackEveryFrame (iCelTimerListener* listener,
	int where)
{
  size_t pc_idx = weak_listeners_hash.Get (listener, (size_t)~0);
  // If our pc is not yet in the weak_listeners table then it can't possibly
  // be in the every_frame table so we can return here already.
  if (pc_idx == (size_t)~0) return;

  CallbackInfo* cbinfo = GetCBInfo (where);
  cbinfo->every_frame.Delete (pc_idx);
}

void celPlLayer::RemoveCallbackOnce (iCelTimerListener* listener, int where)
{
  size_t pc_idx = weak_listeners_hash.Get (listener, (size_t)~0);
  // If our pc is not yet in the weak_listeners table then it can't possibly
  // be in the timed_callbacks table so we can return here already.
  if (pc_idx == (size_t)~0) return;

  CallbackInfo* cbinfo = GetCBInfo (where);
  size_t i;
  for (i = 0 ; i < cbinfo->timed_callbacks.Length () ; )
    if (cbinfo->timed_callbacks[i].pc_idx == pc_idx)
      cbinfo->timed_callbacks.DeleteIndex (i);
    else
      i++;
}

