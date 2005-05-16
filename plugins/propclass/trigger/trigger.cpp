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
#include "ivaria/view.h"
#include "ivideo/graph3d.h"
#include "csqsqrt.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Trigger, "pctrigger")

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistence",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//---------------------------------------------------------------------------

csStringID celPcTrigger::id_entity = csInvalidStringID;

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

  if (id_entity == csInvalidStringID)
    id_entity = pl->FetchStringID ("cel.parameter.entity");
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
  SetMonitorDelay (200, 10);
}

celPcTrigger::~celPcTrigger ()
{
  send_to_self = false;	// Prevent sending messages to this entity.
  LeaveAllEntities ();
  if (pl)
    pl->RemoveCallbackOnce ((iCelTimerListener*)this, cscmdPreProcess);
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
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, cscmdPreProcess);
  if (enabled)
    pl->CallbackOnce ((iCelTimerListener*)this,
    	delay+(rand () % (jitter+jitter))-jitter, cscmdPreProcess);
}

void celPcTrigger::SetMonitorDelay (csTicks delay, csTicks jitter)
{
  celPcTrigger::delay = delay;
  celPcTrigger::jitter = jitter;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, cscmdPreProcess);
  if (enabled)
    pl->CallbackOnce ((iCelTimerListener*)this,
    	delay+(rand () % (jitter+jitter))-jitter, cscmdPreProcess);
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
  sphere_sector = sector;
  sphere_center = center;
  sphere_radius = radius;
}

void celPcTrigger::SetupTriggerBox (iSector* sector, const csBox3& box)
{
  LeaveAllEntities ();
  // Not implemented yet!
  printf ("SetupTriggerBox is not implemented yet!\n"); fflush (stdout);
  CS_ASSERT (false);
}

void celPcTrigger::TickOnce ()
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
    }
    if (monitoring_entity_pcmesh)
    {
      // We have an entity to monitor. See how far it is from
      // our trigger center.
      float sqdistance = csSquaredDist::PointPoint (
      	monitoring_entity_pcmesh->GetMesh ()->GetMovable ()
		->GetFullTransform ().GetOrigin (),
	sphere_center);
      size_t idx = EntityInTrigger (monitoring_entity);
      if (sqdistance < sphere_radius * sphere_radius)
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
    // Check all entities that are near our location.
    csRef<iCelEntityList> list = pl->FindNearbyEntities (sphere_sector,
    	sphere_center, sphere_radius);
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
  pl->CallbackOnce ((iCelTimerListener*)this,
  	delay+(rand () % (jitter+jitter))-jitter,
  	cscmdPreProcess);
}

#define TRIGGER_SERIAL 1

csPtr<iCelDataBuffer> celPcTrigger::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TRIGGER_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTrigger::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TRIGGER_SERIAL)
  {
    Report (object_reg, "serialnr != TRIGGER_SERIAL.  Cannot load.");
    return false;
  }

  return true;
}

bool celPcTrigger::PerformAction (csStringID /*actionId*/,
	iCelParameterBlock* /*params*/)
{
  return false;
}

void celPcTrigger::SendTriggerMessage (iCelEntity* destentity,
	iCelEntity* ent, const char* msgid)
{
  if (ent) params->GetParameter (0).SetIBase (ent);
  celData ret;
  destentity->GetBehaviour ()->SendMessage (msgid, this, ret, params);
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
  size_t i;
  for (i = 0 ; i < listeners.Length () ; i++)
    listeners[i]->EntityEnters (&scfiPcTrigger, entity);
}

void celPcTrigger::FireTriggersEntityLeaves (iCelEntity* entity)
{
  size_t i;
  for (i = 0 ; i < listeners.Length () ; i++)
    listeners[i]->EntityLeaves (&scfiPcTrigger, entity);
}

void celPcTrigger::FireTriggersEnterTrigger (iCelEntity* entity)
{
  size_t i;
  for (i = 0 ; i < listeners.Length () ; i++)
    listeners[i]->EnterTrigger (&scfiPcTrigger, entity);
}

void celPcTrigger::FireTriggersLeaveTrigger (iCelEntity* entity)
{
  size_t i;
  for (i = 0 ; i < listeners.Length () ; i++)
    listeners[i]->LeaveTrigger (&scfiPcTrigger, entity);
}

//---------------------------------------------------------------------------

