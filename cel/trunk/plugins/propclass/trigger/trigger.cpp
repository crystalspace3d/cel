/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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

#include "cssysdef.h"
#include <math.h>
#include "plugins/propclass/trigger/trigger.h"
#include "propclass/camera.h"
#include "propclass/mesh.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "csgeom/math3d.h"
#include "csutil/util.h"
#include "csutil/debug.h"
#include "csutil/flags.h"
#include "csutil/hash.h"
#include "csutil/set.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "iutil/virtclk.h"
#include "iutil/csinput.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/string.h"
#include "iutil/stringarray.h"
#include "imap/loader.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/camera.h"
#include "iengine/region.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/csview.h"
#include "cstool/collider.h"
#include "ivaria/view.h"
#include "ivideo/graph3d.h"
#include "csqsqrt.h"
#include "ivaria/reporter.h"
#include "ivaria/collider.h"
#include "ivaria/mapnode.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Trigger, "pctrigger")

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.propclass.trigger",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

//---------------------------------------------------------------------------

csStringID celPcTrigger::action_setuptriggersphere = csInvalidStringID;
csStringID celPcTrigger::id_sector = csInvalidStringID;
csStringID celPcTrigger::id_position = csInvalidStringID;
csStringID celPcTrigger::id_radius = csInvalidStringID;
csStringID celPcTrigger::id_entity = csInvalidStringID;
csStringID celPcTrigger::action_setuptriggerbox = csInvalidStringID;
csStringID celPcTrigger::id_minbox = csInvalidStringID;
csStringID celPcTrigger::id_maxbox = csInvalidStringID;
csStringID celPcTrigger::action_setuptriggerabovemesh = csInvalidStringID;
csStringID celPcTrigger::id_maxdistance = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcTrigger)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTrigger)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTrigger::PcTrigger)
  SCF_IMPLEMENTS_INTERFACE (iPcTrigger)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcTrigger::celPcTrigger (iObjectRegistry* object_reg)
  : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTrigger);
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);

  if (id_entity == csInvalidStringID)
  {
    id_entity = pl->FetchStringID ("cel.parameter.entity");
    action_setuptriggersphere = pl->FetchStringID (
    	"cel.action.SetupTriggerSphere");
    action_setuptriggerbox = pl->FetchStringID (
    	"cel.action.SetupTriggerBox");
    action_setuptriggerabovemesh = pl->FetchStringID (
    	"cel.action.SetupTriggerAboveMesh");
    id_sector = pl->FetchStringID ("cel.parameter.sector");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_radius = pl->FetchStringID ("cel.parameter.radius");
    id_minbox = pl->FetchStringID ("cel.parameter.minbox");
    id_maxbox = pl->FetchStringID ("cel.parameter.maxbox");
    id_maxdistance = pl->FetchStringID ("cel.parameter.maxdistance");
  }
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_entity, "entity");

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_delay] = 0;		// Handled in this class.
  propdata[propid_jitter] = 0;		// Handled in this class.
  propdata[propid_monitor] = 0;		// Handled in this class.

  enabled = true;
  send_to_self = true;
  send_to_others = true;
  monitor_entity = 0;
  above_collider = 0;
  SetMonitorDelay (200, 10);

  sphere_sector = 0;
  box_sector = 0;
}

celPcTrigger::~celPcTrigger ()
{
  send_to_self = false;	// Prevent sending messages to this entity.
  send_to_others = false;
  LeaveAllEntities ();
  if (pl)
    pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  delete params;
  delete[] monitor_entity;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcTrigger);
}

Property* celPcTrigger::properties = 0;
size_t celPcTrigger::propertycount = 0;

void celPcTrigger::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT (pl != 0);

    propertycount = 3;
    properties = new Property[propertycount];

    properties[propid_delay].id = pl->FetchStringID (
    	"cel.property.delay");
    properties[propid_delay].datatype = CEL_DATA_LONG;
    properties[propid_delay].readonly = false;
    properties[propid_delay].desc = "Update delay to check for entities.";

    properties[propid_jitter].id = pl->FetchStringID (
    	"cel.property.jitter");
    properties[propid_jitter].datatype = CEL_DATA_LONG;
    properties[propid_jitter].readonly = false;
    properties[propid_jitter].desc = "Random jitter to add to update delay.";

    properties[propid_monitor].id = pl->FetchStringID (
    	"cel.property.monitor");
    properties[propid_monitor].datatype = CEL_DATA_STRING;
    properties[propid_monitor].readonly = false;
    properties[propid_monitor].desc = "Entity name to monitor.";
  }
}

bool celPcTrigger::SetProperty (csStringID propertyId, long b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_delay].id)
  {
    SetMonitorDelay (b, jitter);
    return true;
  }
  else if (propertyId == properties[propid_jitter].id)
  {
    SetMonitorDelay (delay, b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

long celPcTrigger::GetPropertyLong (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_delay].id)
  {
    return delay;
  }
  else if (propertyId == properties[propid_jitter].id)
  {
    return jitter;
  }
  else
  {
    return celPcCommon::GetPropertyLong (propertyId);
  }
}

bool celPcTrigger::SetProperty (csStringID propertyId, const char* b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_monitor].id)
  {
    MonitorEntity (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

const char* celPcTrigger::GetPropertyString (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_monitor].id)
  {
    return monitor_entity;
  }
  else
  {
    return celPcCommon::GetPropertyString (propertyId);
  }
}

void celPcTrigger::MonitorEntity (const char* entityname)
{
  LeaveAllEntities ();
  monitoring_entity = 0;
  monitoring_entity_pcmesh = 0;
  delete[] monitor_entity;
  monitor_entity = csStrNew (entityname);
}

void celPcTrigger::EnableTrigger (bool en)
{
  enabled = en;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  if (enabled)
    pl->CallbackOnce ((iCelTimerListener*)this,
    	delay+(rand () % (jitter+jitter))-jitter, CEL_EVENT_PRE);
}

void celPcTrigger::SetMonitorDelay (csTicks delay, csTicks jitter)
{
  celPcTrigger::delay = delay;
  celPcTrigger::jitter = jitter;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  if (enabled)
    pl->CallbackOnce ((iCelTimerListener*)this,
    	delay+(rand () % (jitter+jitter))-jitter, CEL_EVENT_PRE);
}

void celPcTrigger::LeaveAllEntities ()
{
  size_t i;
  for (i = 0 ; i < entities_in_trigger.Length () ; i++)
    if (entities_in_trigger[i])
    {
      if (send_to_self)
      {
        FireTriggersEntityLeaves (entities_in_trigger[i]);
        SendTriggerMessage (entity, entities_in_trigger[i],
		"pctrigger_entityleaves");
      }
      if (send_to_others)
      {
        FireTriggersLeaveTrigger (entities_in_trigger[i]);
        SendTriggerMessage (entities_in_trigger[i], entity,
		"pctrigger_leavetrigger");
      }
    }
  entities_in_trigger.SetLength (0);
}

size_t celPcTrigger::EntityInTrigger (iCelEntity* entity)
{
  size_t i;
  for (i = 0 ; i < entities_in_trigger.Length () ; i++)
    if (entities_in_trigger[i] == entity) return i;
  return csArrayItemNotFound;
}
  
void celPcTrigger::SetupTriggerSphere (iSector* sector,
	const csVector3& center, float radius)
{
  LeaveAllEntities ();
  box_sector = 0;
  above_mesh = 0;

  sphere_sector = sector;
  sphere_center = center;
  sphere_radius = radius;
}

void celPcTrigger::SetupTriggerSphere (iSector* sector,
	const char* center_name, float radius)
{
  LeaveAllEntities ();
  box_sector = 0;
  above_mesh = 0;

  csRef<iMapNode> mapnode = CS_GET_NAMED_CHILD_OBJECT (
  	sector->QueryObject (), iMapNode, center_name);
  if (!mapnode)
  {
    Report (object_reg, "Can't find node '%s' for trigger!",
    	(const char*)center_name);
    // @@@ Function should return bool!
    return;
  }

  sphere_sector = sector;
  sphere_center = mapnode->GetPosition ();
  sphere_radius = radius;
}

void celPcTrigger::SetupTriggerBox (iSector* sector, const csBox3& box)
{
  LeaveAllEntities ();
  sphere_sector = 0;
  above_mesh = 0;

  box_sector = sector;
  box_area = box;
}

void celPcTrigger::SetupTriggerAboveMesh (iPcMesh* m, float maxdistance)
{
  LeaveAllEntities ();
  sphere_sector = 0;
  box_sector = 0;

  above_mesh = m;
  above_maxdist = maxdistance;
  above_collider = 0;
}

bool celPcTrigger::Check ()
{
  if (monitor_entity)
  {
    // We want to monitor a single entity.
    if (!monitoring_entity)
    {
      // We haven't found the entity yet.
      monitoring_entity = pl->FindEntity (monitor_entity);
      monitoring_entity_pcmesh = CEL_QUERY_PROPCLASS_ENT (monitoring_entity,
      	iPcMesh);
      size_t idx = EntityInTrigger (monitoring_entity);
      return idx != csArrayItemNotFound;
    }
  }
  return false;
}

void celPcTrigger::TickOnce ()
{
  // First find the information for the 'check above' case.
  csReversibleTransform above_trans;
  if (above_mesh)
  {
    iMovable* m = above_mesh->GetMesh ()->GetMovable ();
    above_trans = m->GetFullTransform ();
    if (!above_collider)
    {
      csColliderWrapper* wrap = csColliderWrapper::GetColliderWrapper (
		above_mesh->GetMesh ()->QueryObject ());
      if (!wrap) goto end;
      above_collider = wrap->GetCollider ();
    }
    if (!above_collider) goto end;
  }

  if (monitor_entity)
  {
    // We want to monitor a single entity.
    if (!monitoring_entity)
    {
      // We haven't found the entity yet.
      monitoring_entity = pl->FindEntity (monitor_entity);
      monitoring_entity_pcmesh = CEL_QUERY_PROPCLASS_ENT (monitoring_entity,
      	iPcMesh);
    }
    if (monitoring_entity_pcmesh)
    {
      // We have an entity to monitor.
      size_t idx = EntityInTrigger (monitoring_entity);
      iMovable* movable = monitoring_entity_pcmesh->GetMesh ()->GetMovable ();
      csVector3 mpos = movable->GetFullTransform ().GetOrigin ();
      iSector* sector = movable->GetSectors ()->Get (0);

      bool trigger_fired = false;
      if (sphere_sector)
      {
        // See how far it is from our trigger center.
	if (sphere_sector == sector)
	{
          float sqdistance = csSquaredDist::PointPoint (mpos, sphere_center);
          trigger_fired = sqdistance < sphere_radius * sphere_radius;
        }
      }
      else if (box_sector)
      {
	if (box_sector == sector)
          trigger_fired = box_area.In (mpos);
      }
      else
      {
	csVector3 end (mpos.x, mpos.y-above_maxdist, mpos.z);
	// Small correction to make sure we don't miss the object that we're standing on.
	mpos.y += .01f;
	trigger_fired = cdsys->CollideSegment (above_collider, &above_trans,
		mpos, end);
      }

      if (trigger_fired)
      {
        // Yes!
	if (idx == csArrayItemNotFound)
	{
	  // This entity was previously not in the trigger. Add it now.
	  entities_in_trigger.Push (monitoring_entity);
	  if (send_to_self)
	  {
	    FireTriggersEntityEnters (monitoring_entity);
            SendTriggerMessage (entity, monitoring_entity,
		"pctrigger_entityenters");
	  }
	  if (send_to_others)
	  {
	    FireTriggersEnterTrigger (monitoring_entity);
            SendTriggerMessage (monitoring_entity, entity,
		"pctrigger_entertrigger");
	  }
	}
      }
      else
      {
        // No!
	if (idx != csArrayItemNotFound)
	{
	  // This entity was previously in the trigger. Remove it now.
	  entities_in_trigger.DeleteIndex (idx);
	  if (send_to_self)
	  {
	    FireTriggersEntityLeaves (monitoring_entity);
            SendTriggerMessage (entity, monitoring_entity,
		"pctrigger_entityleaves");
	  }
	  if (send_to_others)
	  {
	    FireTriggersLeaveTrigger (monitoring_entity);
            SendTriggerMessage (monitoring_entity, entity,
		"pctrigger_leavetrigger");
	  }
        }
      }
    }
  }
  else
  {
    csReversibleTransform above_trans;

    // Check all entities that are near our location.
    csRef<iCelEntityList> list;
    if (sphere_sector)
      list = pl->FindNearbyEntities (sphere_sector,
    	sphere_center, sphere_radius);
    else if (box_sector)
      list = pl->FindNearbyEntities (box_sector, box_area);
    else
    {
      csBox3 b;
      above_mesh->GetMesh ()->GetWorldBoundingBox (b);
      iMovable* m = above_mesh->GetMesh ()->GetMovable ();
      list = pl->FindNearbyEntities (m->GetSectors ()->Get (0), b);
    }

    size_t i;

    // Fill a set with all entities that are currently in trigger.
    csSet<csPtrKey<iCelEntity> > previous_entities;
    for (i = 0 ; i < entities_in_trigger.Length () ; i++)
      if (entities_in_trigger[i])
        previous_entities.Add ((iCelEntity*)entities_in_trigger[i]);

    // Now clear our entities_in_trigger table. We will fill it again.
    entities_in_trigger.SetLength (0);

    // Traverse the entities that are near us.
    for (i = 0 ; i < list->GetCount () ; i++)
    {
      iCelEntity* ent = list->Get (i);
      // If we are doing above a mesh then we have to test if the entity
      // is really above the mesh.
      if (above_mesh)
      {
        csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (ent, iPcMesh);
	if (!pcmesh) continue;
	csVector3 origin = pcmesh->GetMesh ()->GetMovable ()
		->GetFullPosition ();
	csVector3 end (origin.x, origin.y-above_maxdist, origin.z);
	// Small correction to make sure we don't miss the object that we're standing on.
	origin.y += .01f;
	if (!cdsys->CollideSegment (above_collider, &above_trans, origin, end))
	  continue;
      }

      entities_in_trigger.Push (ent);
      if (!previous_entities.In (ent))
      {
        // This entity was not in the trigger before. Send a message!
	if (send_to_self)
	{
          FireTriggersEntityEnters (ent);
          SendTriggerMessage (entity, ent, "pctrigger_entityenters");
	}
	if (send_to_others)
	{
          FireTriggersEnterTrigger (ent);
          SendTriggerMessage (ent, entity, "pctrigger_entertrigger");
        }
      }
      // Delete from the set.
      previous_entities.Delete (ent);
    }

    // All entities that are still in the set were in the trigger
    // last time but are not any longer.
    csSet<csPtrKey<iCelEntity> >::GlobalIterator it = previous_entities
    	.GetIterator ();
    while (it.HasNext ())
    {
      iCelEntity* ent = it.Next ();
      if (send_to_self)
      {
        FireTriggersEntityLeaves (ent);
        SendTriggerMessage (entity, ent, "pctrigger_entityleaves");
      }
      if (send_to_others)
      {
        FireTriggersLeaveTrigger (ent);
        SendTriggerMessage (ent, entity, "pctrigger_leavetrigger");
      }
    }
  }
end:
  pl->CallbackOnce ((iCelTimerListener*)this,
  	delay+(rand () % (jitter+jitter))-jitter, CEL_EVENT_PRE);
}

#define TRIGGER_SERIAL 1

csPtr<iCelDataBuffer> celPcTrigger::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TRIGGER_SERIAL);
  databuf->Add (enabled);
  databuf->Add (send_to_self);
  databuf->Add (send_to_others);
  databuf->Add (monitor_entity);
  databuf->Add ((uint32)delay);
  databuf->Add ((uint32)jitter);
  if (sphere_sector)
  {
    databuf->Add ((uint8)1);
    databuf->Add (sphere_sector->QueryObject ()->GetName ());
    databuf->Add (sphere_center);
    databuf->Add (sphere_radius);
  }
  else if (box_sector)
  {
    databuf->Add ((uint8)2);
    databuf->Add (box_sector->QueryObject ()->GetName ());
    databuf->Add (box_area.Min ());
    databuf->Add (box_area.Max ());
  }
  else if (above_mesh)
  {
    databuf->Add ((uint8)3);
    databuf->Add (above_mesh);
    databuf->Add (above_maxdist);
  }
  else
  {
    databuf->Add ((uint8)0);
  }

  databuf->Add ((uint32)entities_in_trigger.Length ());
  size_t i;
  for (i = 0 ; i < entities_in_trigger.Length () ; i++)
  {
    databuf->Add (entities_in_trigger[i]);
  }
  databuf->Add (monitoring_entity);
  databuf->Add (monitoring_entity_pcmesh);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTrigger::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TRIGGER_SERIAL)
    return Report (object_reg, "serialnr != TRIGGER_SERIAL.  Cannot load.");
  bool en = databuf->GetBool ();
  send_to_self = databuf->GetBool ();
  send_to_others = databuf->GetBool ();
  iString* s = databuf->GetString ();
  if (!s)
    return Report (object_reg, "Problem parsing trigger!");
  MonitorEntity (s->GetData ());
  csTicks d = csTicks (databuf->GetUInt32 ());
  csTicks j = csTicks (databuf->GetUInt32 ());
  SetMonitorDelay (d, j);

  uint8 t = databuf->GetUInt8 ();
  if (t == 1)
  {
    s = databuf->GetString ();
    if (!s)
      return Report (object_reg, "Problem parsing trigger!");
    iSector* sector = engine->FindSector (s->GetData ());
    if (!sector)
      return Report (object_reg, "Can't find sector '%s'!", s->GetData ());
    csVector3 c;
    databuf->GetVector3 (c);
    float r = databuf->GetFloat ();
    SetupTriggerSphere (sector, c, r);
  }
  else if (t == 2)
  {
    s = databuf->GetString ();
    if (!s)
      return Report (object_reg, "Problem parsing trigger!");
    iSector* sector = engine->FindSector (s->GetData ());
    if (!sector)
      return Report (object_reg, "Can't find sector '%s'!", s->GetData ());
    csVector3 mi, ma;
    databuf->GetVector3 (mi);
    databuf->GetVector3 (ma);
    SetupTriggerBox (sector, csBox3 (mi, ma));
  }
  else if (t == 3)
  {
    iCelPropertyClass* pc = databuf->GetPC ();
    csRef<iPcMesh> pcmesh;
    if (pc)
      pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);
    float r = databuf->GetFloat ();
    if (pcmesh)
      SetupTriggerAboveMesh (pcmesh, r);
  }

  EnableTrigger (en);

  iCelEntity* ent;
  entities_in_trigger.DeleteAll ();
  size_t et = databuf->GetUInt32 ();
  while (et > 0)
  {
    et--;
    ent = databuf->GetEntity ();
    if (ent) entities_in_trigger.Push (ent);
  }
  ent = databuf->GetEntity ();
  if (ent) monitoring_entity = ent;
  iCelPropertyClass* pc = databuf->GetPC ();
  if (pc) monitoring_entity_pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);

  return true;
}

bool celPcTrigger::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_setuptriggersphere)
  {
    CEL_FETCH_STRING_PAR (sector,params,id_sector);
    if (!p_sector)
      return Report (object_reg,
      	"Missing parameter 'sector' for action SetupTriggerSphere!");

    CEL_FETCH_FLOAT_PAR (radius,params,id_radius);
    if (!p_radius)
      return Report (object_reg,
      	"Missing parameter 'radius' for action SetupTriggerSphere!");
    iSector* sec = engine->FindSector (sector);
    if (!sec)
      return Report (object_reg,
      	"Can't find sector '%s' for action SetupTriggerSphere!", sector);

    const celData* p_position = params->GetParameter (id_position);
    if (!p_position)
      return Report (object_reg,
      	"Missing parameter 'position' for action SetupTriggerSphere!");
    if (p_position->type == CEL_DATA_VECTOR3)
    {
      csVector3 v;
      v.x = p_position->value.v.x;
      v.y = p_position->value.v.y;
      v.z = p_position->value.v.z;
      SetupTriggerSphere (sec, v, radius);
    }
    else if (p_position->type == CEL_DATA_STRING)
    {
      const char* position = p_position->value.s->GetData ();
      SetupTriggerSphere (sec, position, radius);
    }
    else
      return Report (object_reg,
      	"'position' must be string or vector for SetupTriggerSphere!");
    return true;
  }
  else if (actionId == action_setuptriggerbox)
  {
    CEL_FETCH_STRING_PAR (sector,params,id_sector);
    if (!p_sector)
      return Report (object_reg,
      	"Missing parameter 'sector' for action SetupTriggerBox!");
    CEL_FETCH_VECTOR3_PAR (minbox,params,id_minbox);
    if (!p_minbox)
      return Report (object_reg,
      	"Missing parameter 'minbox' for action SetupTriggerBox!");
    CEL_FETCH_VECTOR3_PAR (maxbox,params,id_maxbox);
    if (!p_maxbox)
      return Report (object_reg,
      	"Missing parameter 'maxbox' for action SetupTriggerBox!");
    iSector* sec = engine->FindSector (sector);
    if (!sec)
      return Report (object_reg,
      	"Can't find sector '%s' for action SetupTriggerBox!", sector);
    SetupTriggerBox (sec, csBox3 (minbox, maxbox));
    return true;
  }
  else if (actionId == action_setuptriggerabovemesh)
  {
    CEL_FETCH_STRING_PAR (entity,params,id_entity);
    if (!p_entity)
      return Report (object_reg,
      	"Missing parameter 'entity' for action SetupTriggerAboveMesh!");
    CEL_FETCH_FLOAT_PAR (maxdistance,params,id_maxdistance);
    if (!p_maxdistance)
      return Report (object_reg,
      	"Missing parameter 'maxdistance' for action SetupTriggerAboveMesh!");
    iCelEntity* ent = pl->FindEntity (entity);
    if (!ent)
      return Report (object_reg,
      	"Can't find entity '%s' for action SetupTriggerAboveMesh!", entity);
    csRef<iPcMesh> m = CEL_QUERY_PROPCLASS_ENT (ent, iPcMesh);
    if (!m)
      return Report (object_reg,
      	"Entity '%s' doesn't support pcmesh (action SetupTriggerAboveMesh)!",
	entity);
    SetupTriggerAboveMesh (m, maxdistance);
    return true;
  }
  return false;
}

void celPcTrigger::SendTriggerMessage (iCelEntity* destentity,
	iCelEntity* ent, const char* msgid)
{
  if (ent) params->GetParameter (0).Set (ent);
  iCelBehaviour* bh = destentity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage (msgid, this, ret, params);
  }
}

void celPcTrigger::AddTriggerListener (iPcTriggerListener* listener)
{
  listeners.Push (listener);
}

void celPcTrigger::RemoveTriggerListener (iPcTriggerListener* listener)
{
  listeners.Delete (listener);
}

void celPcTrigger::FireTriggersEntityEnters (iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->EntityEnters (&scfiPcTrigger, entity);
  }
}

void celPcTrigger::FireTriggersEntityLeaves (iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->EntityLeaves (&scfiPcTrigger, entity);
  }
}

void celPcTrigger::FireTriggersEnterTrigger (iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->EnterTrigger (&scfiPcTrigger, entity);
  }
}

void celPcTrigger::FireTriggersLeaveTrigger (iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->LeaveTrigger (&scfiPcTrigger, entity);
  }
}

//---------------------------------------------------------------------------

