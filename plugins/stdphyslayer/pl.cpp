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
#include "csutil/csendian.h"
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
#include "csutil/memfile.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"
#include "iutil/objreg.h"
#include "iutil/plugin.h"
#include "iutil/virtclk.h"
#include "ivaria/reporter.h"
#include "cstool/enginetools.h"
#include "celtool/stdparams.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celPlLayer)

celPlLayer::celPlLayer (iBase* parent) : scfImplementationType (this, parent)
{
  entities_hash_dirty = false;
  scfiEventHandlerLogic = 0;
  scfiEventHandler3D = 0;
  scfiEventHandler2D = 0;

  compress_delay = 1000;
  allow_entity_addon = true;
}

celPlLayer::~celPlLayer ()
{
  CleanCache ();

  entities.DeleteAll ();
  entities_hash.DeleteAll ();
  entityclasses_hash.DeleteAll ();

  if (scfiEventHandlerLogic)
  {
    csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
    if (q != 0)
      q->RemoveListener (scfiEventHandlerLogic);
    scfiEventHandlerLogic->DecRef ();
  }

  if (scfiEventHandler3D)
  {
    csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
    if (q != 0)
      q->RemoveListener (scfiEventHandler3D);
    scfiEventHandler3D->DecRef ();
  }

  if (scfiEventHandler2D)
  {
    csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
    if (q != 0)
      q->RemoveListener (scfiEventHandler2D);
    scfiEventHandler2D->DecRef ();
  }
}

bool celPlLayer::HandleEvent (iEvent& ev, int where)
{
  if (ev.Name != csevFrame (object_reg))
    return false;

  CallbackInfo* cbinfo = GetCBInfo (where);
  if (!cbinfo) return false;

  cbinfo->handling_every_frame = true;
  // First fire all property classes that must be fired every frame.
  bool compress = false;
  csSet<size_t>::GlobalIterator it = cbinfo->every_frame.GetIterator ();
  while (it.HasNext ())
  {
    size_t pc_idx = it.Next ();
    iCelTimerListener* listener = weak_listeners[pc_idx];
    if (listener)
      listener->TickEveryFrame();
    else
      compress = true;
  }
  cbinfo->handling_every_frame = false;
  while (cbinfo->todo_del_every_frame.GetSize () > 0)
    cbinfo->every_frame.Delete (cbinfo->todo_del_every_frame.Pop ());
  while (cbinfo->todo_add_every_frame.GetSize () > 0)
    cbinfo->every_frame.Add (cbinfo->todo_add_every_frame.Pop ());

  // Then fire all property classes that are interested in receiving
  // events if the alloted time has exceeded. The property classes
  // are added in reverse order so that the top of the array is the
  // one that will fire first.
  csTicks current_time = vc->GetCurrentTicks ();
  csArray<CallbackTiming>& cbs = cbinfo->timed_callbacks;
  while (cbs.GetSize () > 0 && cbs.Top ().time_to_fire <= current_time)
  {
    CallbackTiming pcfire = cbs.Pop ();
    iCelTimerListener* listener = weak_listeners[pcfire.pc_idx];
    if (listener)
      listener->TickOnce();
    else
      compress = true;
  }

  if (compress) CompressCallbackInfo ();

  return true;
}

bool celPlLayer::Initialize (iObjectRegistry* object_reg)
{
  celPlLayer::object_reg = object_reg;
  idlist.Clear ();
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  engine = csQueryRegistry<iEngine> (object_reg);
  if (!engine) return false;	// Engine is required.

  scfiEventHandlerLogic = new EventHandlerLogic (this);
  csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
  csEventID esub[] = { 
    csevFrame (object_reg),
    CS_EVENTLIST_END 
  };
  q->RegisterListener (scfiEventHandlerLogic, esub);

  scfiEventHandler3D = new EventHandler3D (this);
  q->RegisterListener (scfiEventHandler3D, esub);

  scfiEventHandler2D = new EventHandler2D (this);
  q->RegisterListener (scfiEventHandler2D, esub);

  return true;
}

size_t celPlLayer::AddScope (csString version, int size)
{
  return idlist.AddScope (version, size);
}

void celPlLayer::ResetScope (size_t scope_idx)
{
  return idlist.ResetScope (scope_idx);
}

csPtr<iCelEntity> celPlLayer::CreateEntity ()
{
  return CreateEntityInScope ((int)idlist.DefaultScope);
}

csPtr<iCelEntity> celPlLayer::CreateEntityInScope (int scope)
{
  uint objid;

  csRef<celEntity> entity = csPtr<celEntity> (new celEntity (this));
  iCelEntity* ientity = entity;
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

void celPlLayer::RegisterID (iCelEntity* entity, uint id)
{
  idlist.RegisterWithID (entity, id);
}

csPtr<iCelEntity> celPlLayer::CreateEntity (uint entity_id)
{
  csRef<celEntity> entity = csPtr<celEntity> (new celEntity (this));
  iCelEntity* ientity = entity;

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
  if (entname && *entname) ent->SetName (entname);
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
    entities_hash.Delete (ent->GetName (), ent);
}

void celPlLayer::AddEntityName (celEntity* ent)
{
  if (!entities_hash_dirty)
    entities_hash.Put (ent->GetName (), ent);
}

iCelEntity* celPlLayer::FindEntity (const char* name)
{
  if (entities_hash_dirty)
  {
    entities_hash_dirty = false;
    size_t i;
    entities_hash.DeleteAll ();
    for (i = 0 ; i < entities.GetSize () ; i++)
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
  return static_cast<iCelEntityTemplate*> (fact);
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
  return static_cast<iCelEntityTemplate*> (f);
}

class celHashEntityTemplateIterator : public scfImplementation1<celHashEntityTemplateIterator, iCelEntityTemplateIterator>
{
  celTemplates::ConstGlobalIterator it;

public:
  celHashEntityTemplateIterator (const celTemplates::ConstGlobalIterator& it) :
    scfImplementationType (this), it (it)
  {
  }
  virtual ~celHashEntityTemplateIterator ()
  {
  }
  virtual bool HasNext () const { return it.HasNext (); }
  virtual iCelEntityTemplate* Next ()
  {
    celEntityTemplate* tpl = it.Next ();
    return static_cast<iCelEntityTemplate*> (tpl);
  }
};

csPtr<iCelEntityTemplateIterator> celPlLayer::GetEntityTemplates () const
{
  celHashEntityTemplateIterator* it = new celHashEntityTemplateIterator (
      entity_templates.GetIterator ());
  return it;
}

csRef<celVariableParameterBlock> celPlLayer::ConvertTemplateParams (
    iCelEntity* entity,
    const csHash<csRef<iParameter>, csStringID>& act_params,
    iCelParameterBlock* params)
{
  csRef<celEntityParameterBlock> thisParam;
  thisParam.AttachNew (new celEntityParameterBlock (this, entity));
  csRef<celCombineParameterBlock> combinedParams;
  if (!params)
  {
    params = thisParam;
  }
  else
  {
    combinedParams.AttachNew (new celCombineParameterBlock (thisParam, params));
    params = combinedParams;
  }

  csRef<celVariableParameterBlock> converted_params;
  converted_params.AttachNew (new celVariableParameterBlock ());
  csHash<csRef<iParameter>, csStringID>::ConstGlobalIterator it = act_params.GetIterator ();
  while (it.HasNext ())
  {
    csStringID key;
    iParameter* par = it.Next (key);
    const celData* data = par->GetData (params);
    if (data)
      converted_params->AddParameter (key, *data);
  }
  return converted_params;
}

bool celPlLayer::PerformActionTemplate (const ccfPropAct& act,
    	iCelPropertyClass* pc,
  	iCelParameterBlock* params,
	iCelEntity* ent, iCelEntityTemplate* factory)
{
  csRef<celVariableParameterBlock> converted_params = ConvertTemplateParams (
    ent, act.params, params);
  celData ret;
  if (!pc->PerformAction (act.id, converted_params, ret))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.physicallayer",
	  "Error performing '%s' in '%s' for entity '%s' from factory '%s'!",
          FetchString(act.id), pc->GetName (), ent->GetName (),
          factory->GetName ());
    RemoveEntity (ent);
    return false;
  }
  return true;
}

iCelEntity* celPlLayer::CreateEntity (iCelEntityTemplate* factory,
  	const char* name, ...)
{
  csRef<celVariableParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());
  va_list args;
  va_start (args, name);
  char const* par = va_arg (args, char*);
  while (par != 0)
  {
    char const* val = va_arg (args, char*);
    params->AddParameter (FetchStringID (par)).Set (val);
    par = va_arg (args, char*);
  }
  va_end (args);
  return CreateEntity (factory, name, params);
}

iCelEntity* celPlLayer::CreateEntity (iCelEntityTemplate* factory,
  	const char* name, iCelParameterBlock* params)
{
  csRef<iCelEntity> ent = CreateEntity (name, 0, 0, CEL_PROPCLASS_END);
  if (!ApplyTemplate (ent, factory, params))
    return 0;
  (static_cast<celEntity*> ((iCelEntity*)ent))->SetTemplateNameID (FetchStringID (factory->GetName ()));
  return ent;
}

bool celPlLayer::ApplyTemplate (iCelEntity* ent, iCelEntityTemplate* factory,
      iCelParameterBlock* params)
{
  celEntityTemplate* cfact = static_cast<celEntityTemplate*> (factory);

  // First apply all the parents.
  const csRefArray<iCelEntityTemplate>& parents = cfact->GetParentsInt ();
  for (size_t i = 0 ; i < parents.GetSize () ; i++)
  {
    if (!ApplyTemplate (ent, parents[i], params))
      return false;
  }

  csRef<iCelBlLayer> bl;
  if (cfact->GetLayer ())
  {
    bl = FindBehaviourLayer (cfact->GetLayer ());
    if (!bl)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"Can't find behaviour layer '%s' for entity '%s' from factory '%s'!",
		cfact->GetLayer (), ent->GetName (), factory->GetName ());
      return 0;
    }
  }
  else
  {
    bl = csQueryRegistry<iCelBlLayer> (object_reg);
    if (!bl && cfact->GetBehaviour ())
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"Can't find default behaviour layer for entity '%s' from factory '%s'!",
		ent->GetName (), factory->GetName ());
      return 0;
    }
  }

  const csRefArray<celPropertyClassTemplate>& pcs = cfact->GetPropClasses ();
  size_t i;
  for (i = 0 ; i < pcs.GetSize () ; i++)
  {
    celPropertyClassTemplate* pcc = pcs[i];
    const char* pcname = pcc->GetName ();
    const char* pctag = pcc->GetTag ();
    iCelPropertyClass* pc = ent->GetPropertyClassList ()->FindByNameAndTag (
    	pcname, pctag);
    if (!pc) pc = CreateTaggedPropertyClass (ent, pcname, pctag);
    if (!pc)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.physicallayer",
	"Error creating property class '%s' for entity '%s from factory '%s''!",
		pcname, ent->GetName (), factory->GetName ());
      RemoveEntity (ent);
      return false;
    }
    const csArray<ccfPropAct>& props = pcc->GetProperties ();
    size_t j;
    for (j = 0 ; j < props.GetSize () ; j++)
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
	      return false;
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
            csStringID parnameid = FetchStringID (parname);
            const celData* data = params->GetParameter (parnameid);
	    if (data)
	    {
	      switch (d.value.par.partype)
	      {
		case CEL_DATA_LONG:
		  {
		    long l; celParameterTools::ToLong (*data, l);
		    rc = pc->SetProperty (id, l);
		  }
		  break;
		case CEL_DATA_FLOAT:
		  {
		    float f; celParameterTools::ToFloat (*data, f);
		    rc = pc->SetProperty (id, f);
		  }
		  break;
		case CEL_DATA_BOOL:
		  {
		    bool b; celParameterTools::ToBool (*data, b);
		    rc = pc->SetProperty (id, b);
		  }
		  break;
		case CEL_DATA_STRING:
                  {
                    csString value; celParameterTools::ToString (*data, value);
		    rc = pc->SetProperty (id, value.GetData ());
                  }
		  break;
		case CEL_DATA_VECTOR2:
		  {
		    csVector2 v; celParameterTools::ToVector2 (*data, v);
		    rc = pc->SetProperty (id, v);
		  }
		  break;
		case CEL_DATA_VECTOR3:
		  {
		    csVector3 v; celParameterTools::ToVector3 (*data, v);
		    rc = pc->SetProperty (id, v);
		  }
		  break;
		case CEL_DATA_COLOR:
		  {
		    csColor v; celParameterTools::ToColor (*data, v);
		    rc = pc->SetProperty (id, v);
		  }
		  break;
		case CEL_DATA_ENTITY:
		  {
                    csString value; celParameterTools::ToString (*data, value);
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
		pcname, ent->GetName (), factory->GetName ());
        RemoveEntity (ent);
        return false;
      }
    }
  }

  const csSet<csStringID>& classes = cfact->GetClasses ();
  csSet<csStringID>::GlobalIterator it = classes.GetIterator ();
  while (it.HasNext ())
  {
    csStringID id = it.Next ();
    ent->AddClass (id);
  }

  if (bl && cfact->GetBehaviour ())
  {
    iCelBehaviour* behave = bl->CreateBehaviour (ent, cfact->GetBehaviour ());
    if (!behave)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.physicallayer",
	"Error creating behaviour '%s' for entity '%s'!",
	cfact->GetBehaviour (), ent->GetName ());
      RemoveEntity (ent);
      return false;
    }
  }
  const csArray<ccfMessage>& messages = cfact->GetMessages ();
  for (i = 0 ; i < messages.GetSize () ; i++)
  {
    const ccfMessage& msg = messages[i];
    celData ret;
    csRef<celVariableParameterBlock> converted_params = ConvertTemplateParams (
        ent, msg.params, params);
    if (ent->GetBehaviour ())
    {
      csString message = FetchString (msg.msgid);;
      ent->GetBehaviour ()->SendMessage (message, 0, ret, converted_params);
    }
    ent->QueryMessageChannel ()->SendMessage (msg.msgid,
		      static_cast<iMessageSender*> (this), converted_params,
		      0);
  }

  return true;
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
  // First notify the behaviour of the destruction.
  if (entity->GetBehaviour ())
  {
    celData ret;
    entity->GetBehaviour ()->SendMessage ("destruct", 0, ret, 0);
  }
  // First register this entity from all trackers.
  size_t i;
  for (i = 0 ; i < trackers.GetSize () ; i++)
    trackers[i]->RemoveEntity (entity);

  if (!idlist.Remove (entity->GetID ()))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"crystalspace.cel.pllayer",
	"Error while removing Entity with ID %u (%s)",
	(unsigned int)entity->GetID(), entity->GetName());
    return;
  }

  for (i = 0; i < removecallbacks.GetSize(); i++)
  {
    iCelEntityRemoveCallback* callback = removecallbacks[i];
    callback->RemoveEntity (entity);
  }

  // remove entity from class trackers
  csSet<csStringID>::GlobalIterator it = entity->GetClasses().GetIterator();
  while (it.HasNext())
    EntityClassRemoved(entity,it.Next());

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
  for (i = 0 ; i < entities.GetSize () ; i++)
    weakrefs.Push (entities[i]);
#endif
  while (entities.GetSize () > 0)
  {
    RemoveEntityIndex (entities.GetSize ()-1);
  }
#ifdef CS_DEBUG
  for (i = 0 ; i < weakrefs.GetSize () ; i++)
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

iCelPropertyClassFactory* celPlLayer::FindOrLoadPropfact (const char *propname)
{
  if (propname == 0 || !*propname) return 0;

  // if already loaded return existing propfact
  iCelPropertyClassFactory* pf = FindPropertyClassFactory (propname);
  if (pf)
    return pf;

  // use cel.pcfactory.propname if it is able to load
  // and propclass is queried successfully
  csString pfid ("cel.pcfactory.");
  // skip the first 2 characters if they have the 'pc' bit
  // because of historical reasons
  if (propname[0] == 'p' && propname[1] == 'c')
    pfid += &propname[2];
  else
    pfid += propname;
  // try to load property class factory using constructed id
  if (!LoadPropertyClassFactory (pfid))
    return 0;
  return FindPropertyClassFactory (propname);
}

iCelPropertyClass* celPlLayer::CreatePropertyClass (iCelEntity *entity,
  const char *propname, const char* tagname)
{
  iCelPropertyClassFactory* pf = FindOrLoadPropfact (propname);
  if (!pf)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
        "crystalspace.cel.pllayer",
        "No factory for type '%s' registered!", propname);
    return 0;
  }
  // create a new property class
  csRef<iCelPropertyClass> pc (pf->CreatePropertyClass(propname));
  if (!pc)
    return 0;
  if (tagname)
    pc->SetTag (tagname);
  entity->GetPropertyClassList()->Add (pc);
  return pc;
}

iCelPropertyClass* celPlLayer::CreateTaggedPropertyClass (iCelEntity *entity,
  const char *propname, const char* tagname)
{
  return CreatePropertyClass (entity, propname, tagname);
}

// Implementation of iCelCompactDataBufferWriter.
class celCompactDataBufferWriter : public scfImplementation1<
	celCompactDataBufferWriter, iCelCompactDataBufferWriter>
{
private:
  csMemFile file;

public:
  celCompactDataBufferWriter () : scfImplementationType (this)
  {
  }
  virtual ~celCompactDataBufferWriter ()
  {
  }

  virtual const char* GetData () const { return file.GetData (); }
  virtual size_t GetSize () const { return file.GetDataSize (); }

  virtual void AddBool (bool v) { AddInt8 (int8 (v)); }
  virtual void AddInt8 (int8 v) { file.Write ((char*)&v, 1); }
  virtual void AddInt16 (int16 v)
  {
    v = csLittleEndian::Convert (v);
    file.Write ((char*)&v, sizeof (int16));
  }
  virtual void AddInt32 (int32 v)
  {
    v = csLittleEndian::Convert (v);
    file.Write ((char*)&v, sizeof (int32));
  }
  virtual void AddUInt8 (uint8 v) { AddInt8 (int8 (v)); }
  virtual void AddUInt16 (uint16 v) { AddInt16 (int16 (v)); }
  virtual void AddUInt32 (uint32 v) { AddInt32 (int32 (v)); }
  virtual void AddID (csStringID v) { AddInt32 (int32 (v)); }
  virtual void AddFloat (float v)
  {
    uint32 ieee = csIEEEfloat::FromNative (v);
    ieee = csLittleEndian::Convert (ieee);
    file.Write ((char*)&ieee, sizeof (uint32));
  }
  virtual void AddVector2 (const csVector2& v)
  {
    AddFloat (v.x);
    AddFloat (v.y);
  }
  virtual void AddVector3 (const csVector3& v)
  {
    AddFloat (v.x);
    AddFloat (v.y);
    AddFloat (v.z);
  }
  virtual void AddVector4 (const csVector4& v)
  {
    AddFloat (v.x);
    AddFloat (v.y);
    AddFloat (v.z);
    AddFloat (v.w);
  }
  virtual void AddColor (const csColor& v)
  {
    AddFloat (v.red);
    AddFloat (v.green);
    AddFloat (v.blue);
  }
  virtual void AddColor4 (const csColor4& v)
  {
    AddFloat (v.red);
    AddFloat (v.green);
    AddFloat (v.blue);
    AddFloat (v.alpha);
  }
  virtual void AddString8 (const char* s)
  {
    if (s)
    {
      size_t l = strlen (s);
      AddUInt8 (l);
      file.Write (s, l+1);
    }
    else
    {
      AddUInt8 ((uint8)~0);
    }
  }
  virtual void AddString16 (const char* s)
  {
    if (s)
    {
      size_t l = strlen (s);
      AddUInt16 (l);
      file.Write (s, l+1);
    }
    else
    {
      AddUInt16 ((uint16)~0);
    }
  }
  virtual void AddString32 (const char* s)
  {
    if (s)
    {
      size_t l = strlen (s);
      AddUInt32 (l);
      file.Write (s, l+1);
    }
    else
    {
      AddUInt32 ((uint32)~0);
    }
  }
};

// Implementation of iCelCompactDataBufferReader.
class celCompactDataBufferReader : public scfImplementation1<
	celCompactDataBufferReader, iCelCompactDataBufferReader>
{
private:
  csMemFile file;

public:
  celCompactDataBufferReader (iDataBuffer* buf) :
    scfImplementationType (this), file (buf, true)
  {
  }
  virtual ~celCompactDataBufferReader ()
  {
  }

  virtual bool GetBool () { return bool (GetInt8 ()); }
  virtual int8 GetInt8 () { int8 v; file.Read ((char*)&v, 1); return v; }
  virtual int16 GetInt16 ()
  {
    int16 v;
    file.Read ((char*)&v, sizeof (int16));
    return csLittleEndian::Convert (v);
  }
  virtual int32 GetInt32 ()
  {
    int32 v;
    file.Read ((char*)&v, sizeof (int32));
    return csLittleEndian::Convert (v);
  }
  virtual uint8 GetUInt8 () { return uint8 (GetInt8 ()); }
  virtual uint16 GetUInt16 () { return uint16 (GetInt16 ()); }
  virtual uint32 GetUInt32 () { return uint32 (GetInt32 ()); }
  virtual csStringID GetID () { return csStringID (GetInt32 ()); }
  virtual float GetFloat ()
  {
    uint32 ieee;
    file.Read ((char*)&ieee, sizeof (uint32));
    ieee = csLittleEndian::Convert (ieee);
    return csIEEEfloat::ToNative (ieee);
  }
  virtual void GetVector2 (csVector2& v)
  {
    v.x = GetFloat ();
    v.y = GetFloat ();
  }
  virtual void GetVector3 (csVector3& v)
  {
    v.x = GetFloat ();
    v.y = GetFloat ();
    v.z = GetFloat ();
  }
  virtual void GetVector4 (csVector4& v)
  {
    v.x = GetFloat ();
    v.y = GetFloat ();
    v.z = GetFloat ();
    v.w = GetFloat ();
  }
  virtual void GetColor (csColor& v)
  {
    v.red = GetFloat ();
    v.green = GetFloat ();
    v.blue = GetFloat ();
  }
  virtual void GetColor (csColor4& v)
  {
    v.red = GetFloat ();
    v.green = GetFloat ();
    v.blue = GetFloat ();
    v.alpha = GetFloat ();
  }
  /// The returned pointer is only valid for the lifetime of the buffer.
  virtual const char* GetString8 ()
  {
    uint8 l = GetUInt8 ();
    if (l == (uint8)~0)
      return 0;
    const char* data = file.GetData () + file.GetPos ();
    file.SetPos (file.GetPos () + l+1);
    return data;
  }
  virtual const char* GetString16 ()
  {
    uint16 l = GetUInt16 ();
    if (l == (uint16)~0)
      return 0;
    const char* data = file.GetData () + file.GetPos ();
    file.SetPos (file.GetPos () + l+1);
    return data;
  }
  virtual const char* GetString32 ()
  {
    uint32 l = GetUInt32 ();
    if (l == (uint32)~0)
      return 0;
    const char* data = file.GetData () + file.GetPos ();
    file.SetPos (file.GetPos () + l+1);
    return data;
  }
};

// Implementation of iCelDataBuffer.
class celDataBuffer : public scfImplementation1<
	celDataBuffer, iCelDataBuffer>
{
private:
  csArray<celData> data;
  long serialnr;
  size_t posidx;

public:
  celDataBuffer (long serialnr) : scfImplementationType (this)
  {
    celDataBuffer::serialnr = serialnr;
    posidx = 0;
  }
  virtual ~celDataBuffer ()
  {
  }

  virtual long GetSerialNumber () const
  {
    return serialnr;
  }
  virtual size_t GetDataCount () const
  {
    return data.GetSize ();
  }
  virtual celData* GetData ()
  {
    if (posidx >= data.GetSize ()) return 0;
    posidx++;
    return &data[posidx-1];
  }
  virtual celData* GetData (size_t idx)
  {
    CS_ASSERT ((idx != csArrayItemNotFound) && idx < data.GetSize ());
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

csPtr<iCelDataBuffer> celPlLayer::CreateDataBuffer (long serialnr)
{
  return csPtr<iCelDataBuffer> (new celDataBuffer (serialnr));
}

csPtr<iCelCompactDataBufferWriter> celPlLayer::CreateCompactDataBufferWriter ()
{
  return csPtr<iCelCompactDataBufferWriter> (new celCompactDataBufferWriter ());
}

csPtr<iCelCompactDataBufferReader> celPlLayer::CreateCompactDataBufferReader (
    iDataBuffer* buf)
{
  return csPtr<iCelCompactDataBufferReader> (new celCompactDataBufferReader (buf));
}

// Class which is used to attach to an iObject so that
// we can find the iCelEntity again.

struct celEntityFinder : public scfImplementationExt1<
	celEntityFinder, csObject, scfFakeInterface<celEntityFinder> >
{
  SCF_INTERFACE (celEntityFinder, 0, 0, 1);
private:
  iCelEntity* entity;

public:
  celEntityFinder (iCelEntity* entity) : scfImplementationType (this)
  {
    celEntityFinder::entity = entity;
  }
  virtual ~celEntityFinder ()
  { }
  iCelEntity* GetEntity () const { return entity; }
};

void celPlLayer::AttachEntity (iObject* object, iCelEntity* entity)
{
  iCelEntity* old_entity = FindAttachedEntity (object);
  if (old_entity == entity) return;
  if (old_entity != 0) UnattachEntity (object, old_entity);
  csRef<celEntityFinder> cef =
    csPtr<celEntityFinder> (new celEntityFinder (entity));
  csRef<iObject> cef_obj (scfQueryInterface<iObject> (cef));
  object->ObjAdd (cef_obj);
}

void celPlLayer::UnattachEntity (iObject* object, iCelEntity* entity)
{
  csRef<celEntityFinder> cef (CS::GetChildObject<celEntityFinder> (object));
  if (cef)
  {
    if (cef->GetEntity () != entity)
    { return; }
    csRef<iObject> cef_obj (scfQueryInterface<iObject> (cef));
    object->ObjRemove (cef_obj);
  }
}

iCelEntity* celPlLayer::FindAttachedEntity (iObject* object)
{
  csRef<celEntityFinder> cef (CS::GetChildObject<celEntityFinder> (object));
  if (cef)
    return cef->GetEntity ();
  return 0;
}

csPtr<iCelEntityList> celPlLayer::FindNearbyEntities (iSector* sector,
	const csBox3& box, bool do_invisible, csStringID cls)
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
      if (cls == csInvalidStringID || ent->HasClass (cls))
        list->Add (ent);
    }
  }
  return list;
}

csPtr<iCelEntityList> celPlLayer::FindNearbyEntities (iSector* sector,
	const csVector3& start, const csVector3& end, bool do_invisible,
	csStringID cls)
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
      if (cls == csInvalidStringID || ent->HasClass (cls))
        list->Add (ent);
    }
  }
  return list;
}

csPtr<iCelEntityList> celPlLayer::FindNearbyEntities (iSector* sector,
	const csVector3& pos, float radius, bool do_invisible,
	csStringID cls)
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
      if (cls == csInvalidStringID || ent->HasClass (cls))
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
  for (i = 0 ; i < trackers.GetSize () ; i++)
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
  csScreenTargetResult result = csEngineTools::FindScreenTarget (
  	csVector2 (x, y), 1000000000.0f, camera);
  if (result.mesh)
  {
    iObject* sel_obj = result.mesh->QueryObject ();
    return FindAttachedEntity (sel_obj);
  }
  return 0;
}

csPtr<iCelEntityList> celPlLayer::CreateEmptyEntityList ()
{
  return new celEntityList ();
}

static const char* deprecated_plugin_ids[] =
{
  "colldet",			"object.mesh.collisiondetection",
  "collisiondetection",		"object.mesh.collisiondetection",
  "mesh",			"object.mesh",
  "meshselect",			"object.mesh.select",
  "meshdeform",			"object.mesh.deform",
  "light",			"object.light",
  "portal",			"object.portal",
  "linmove",			"move.linear",
  "linearmovement",		"move.linear",
  "movable",			"move.movable",
  "solid",			"move.solid",
  "movableconst_cd",		"move.movableconst_cd",
  "gravity",			"move.gravity",
  "actormove",			"move.actor.standard",
  "npcmove",			"move.npc",
  "mover",			"move.mover",
  "projectile",			"move.projectile",
  "billboard",			"2d.billboard",
  "tooltip",			"2d.tooltip",
  "damage",			"logic.damage",
  "quest",			"logic.quest",
  "rules",			"logic.rules",
  "spawn",			"logic.spawn",
  "trigger",			"logic.trigger",
  "defaultcamera",		"camera.old",
  "newcamera",			"camera.standard",
  "simplecamera",		"camera.simple",
  "region",			"world.region",
  "zonemanager",		"world.zonemanager",
  "hover",			"vehicle.hover",
  "craft",			"vehicle.craft",
  "wheeled",			"vehicle.wheeled",
  "commandinput",		"input.standard",
  "pccommandinput",		"input.standard",
  "inventory",			"tools.inventory",
  "characteristics",		"tools.inventory.characteristics",
  "timer",			"tools.timer",
  "properties",			"tools.properties",
  "mechsys",			"physics.system",
  "mechobject",			"physics.object",
  "mechjoint",			"physics.joint",
  "mechbalancedgroup",		"physics.thruster.group",
  "mechthrustercontroller",	"physics.thruster.controller",
  "mechthrusterreactionary",	"physics.thruster.reactionary",
  "soundlistener",		"sound.listener",
  "soundsource",		"sound.source",
  "test",			"misc.test",
  0
};

bool celPlLayer::LoadPropertyClassFactory (const char* plugin_id)
{
  // Sanity check to avoid crashes with bad plugin_id's.
  if (strlen (plugin_id) >= 15)
  {
    int i = 0;
    while (deprecated_plugin_ids[i])
    {
      if (!strcmp (plugin_id+14, deprecated_plugin_ids[i]))
      {
        csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "crystalspace.cel.physicallayer",
	    "Property class factory name '%s' is deprecated! Use 'cel.pcfactory.%s' instead.",
	    plugin_id, deprecated_plugin_ids[i+1]);
        break;
      }
      i += 2;
    }
  }

  csRef<iPluginManager> plugin_mgr =
  	csQueryRegistry<iPluginManager> (object_reg);
  csRef<iComponent> pf;
  pf = csQueryPluginClass<iComponent> (plugin_mgr, plugin_id);
  if (!pf)
  {
    pf = csLoadPluginAlways (plugin_mgr, plugin_id);
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

void celPlLayer::RegisterPropertyClassFactory (iCelPropertyClassFactory* pf,
    const char* altname)
{
  if (pf_list.Find (pf) != csArrayItemNotFound) return;
  pf_list.Push (pf);
  pf_hash.Put (pf->GetName (), pf);
  if (altname)
    pf_hash.Put (altname, pf);
}

void celPlLayer::UnregisterPropertyClassFactory (
  	iCelPropertyClassFactory* pf)
{
  pf_hash.Delete (pf->GetName (), pf);
  pf_list.Delete (pf);
}

size_t celPlLayer::GetPropertyClassFactoryCount () const
{
  return pf_list.GetSize ();
}

iCelPropertyClassFactory* celPlLayer::GetPropertyClassFactory (size_t idx) const
{
  CS_ASSERT ((idx != csArrayItemNotFound) && idx < pf_list.GetSize ());
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
  while (cache.GetSize () > 0)
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
  return bl_list.GetSize ();
}

iCelBlLayer* celPlLayer::GetBehaviourLayer (size_t idx) const
{
  CS_ASSERT ((idx != csArrayItemNotFound) && idx < bl_list.GetSize ());
  iCelBlLayer* bl = bl_list[idx];
  return bl;
}

iCelBlLayer* celPlLayer::FindBehaviourLayer (const char* name) const
{
  size_t i;
  for (i = 0 ; i < bl_list.GetSize () ; i++)
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
  size_t i = newcallbacks.GetSize ();
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
  size_t orig_pcs_length = weak_listeners.GetSize ();
  csArray<pc_idx> store;
  size_t i;
  for (i = 0 ; i < orig_pcs_length ; i++)
    if (weak_listeners[i])
      store.Push (pc_idx (weak_listeners[i], i));

  // Delete the weak array and build it again.
  weak_listeners.DeleteAll ();
  weak_listeners_hash.DeleteAll ();
  for (i = 0 ; i < store.GetSize () ; i++)
  {
    weak_listeners.Push (store[i].listener);
    weak_listeners_hash.Put (store[i].listener, i);
  }

  // Now create a reverse table to map from original index to new index.
  size_t* map = new size_t[orig_pcs_length];
  memset (map, 0xffffffff, sizeof (size_t) * orig_pcs_length);
  for (i = 0 ; i < store.GetSize () ; i++)
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
    for (i = 0 ; i < cbinfo->timed_callbacks.GetSize () ; )
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
  if (cbinfo->handling_every_frame)
  {
    // First remove it from the delete todo.
    cbinfo->todo_del_every_frame.Delete (pc_idx);
    cbinfo->todo_add_every_frame.Push (pc_idx);
  }
  else
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
  if (cbinfo->handling_every_frame)
  {
    // First remove it from the add todo.
    cbinfo->todo_add_every_frame.Delete (pc_idx);
    cbinfo->todo_del_every_frame.Push (pc_idx);
  }
  else
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
  for (i = 0 ; i < cbinfo->timed_callbacks.GetSize () ; )
    if (cbinfo->timed_callbacks[i].pc_idx == pc_idx)
      cbinfo->timed_callbacks.DeleteIndex (i);
    else
      i++;
}

csTicks celPlLayer::GetTicksLeft (iCelTimerListener* listener, int where)
{
  size_t pc_idx = weak_listeners_hash.Get (listener, (size_t)~0);
  // If our pc is not yet in the weak_listeners table then it can't possibly
  // be in the timed_callbacks table so we can return here already.
  if (pc_idx == (size_t)~0) return csArrayItemNotFound;

  CallbackInfo* cbinfo = GetCBInfo (where);
  size_t i;
  for (i = 0 ; i < cbinfo->timed_callbacks.GetSize () ; )
    if (cbinfo->timed_callbacks[i].pc_idx == pc_idx)
    {
      csTicks time_to_fire = cbinfo->timed_callbacks[i].time_to_fire;
      return time_to_fire - vc->GetCurrentTicks ();
    }
    else
      i++;
  return csArrayItemNotFound;
}

// Handling of class-entities lists
void celPlLayer::EntityClassAdded(iCelEntity *ent,csStringID entclass)
{
  csRef<iCelEntityList> list = entityclasses_hash.Get(entclass,0);
  if (!list)
  {
    list.AttachNew(new celEntityList ());
    entityclasses_hash.Put(entclass,list);
  }
  list->Add(ent);
}

void celPlLayer::EntityClassRemoved(iCelEntity *ent,csStringID entclass)
{
  csRef<iCelEntityList> list = entityclasses_hash.Get(entclass,0);
  list->Remove(ent);
}

const csRef<iCelEntityList> celPlLayer::GetClassEntitiesList (
	csStringID classid)
{
  csRef<iCelEntityList> list = entityclasses_hash.Get(classid,0);
  if (!list)
  {
    list.AttachNew(new celEntityList ());
    entityclasses_hash.Put(classid,list);
  }
  return list;
}

// Send messages to entity lists.
int celPlLayer::SendMessage (iCelEntityList *entlist, const char* msgname,
		iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start (arg, params);
  int responses = SendMessageV (entlist, msgname, params, arg);
  va_end (arg);
  return responses;
}

int celPlLayer::SendMessageV (iCelEntityList *entlist, const char* msgname, 
		iCelParameterBlock* params, va_list arg)
{
  csRef<iCelEntityIterator> it = entlist->GetIterator();
  celData ret;
  int responses = 0;
  while (it->HasNext())
  {
    iCelEntity *ent = it->Next();
    iCelBehaviour *beh = ent->GetBehaviour ();
    if (beh)
    {
      responses += beh->SendMessageV (msgname, 0, ret, params, arg);
    }
  }
  return responses;
}

int celPlLayer::SendMessage (csStringID msgid, iMessageSender* sender,
      iCelEntityList *entlist, iCelParameterBlock* params,
      iCelDataArray* ret)
{
  csRef<iCelEntityIterator> it = entlist->GetIterator();
  int responses = 0;
  while (it->HasNext())
  {
    iCelEntity* ent = it->Next();
    responses += ent->QueryMessageChannel ()->SendMessage (msgid, sender,
	params, ret);
  }
  return responses;
}

