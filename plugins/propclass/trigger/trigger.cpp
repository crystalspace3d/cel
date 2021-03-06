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
#include "iengine/collection.h"
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

CEL_IMPLEMENT_FACTORY_ALT (Trigger, "pclogic.trigger", "pctrigger")

//---------------------------------------------------------------------------

class celTriggerMovableListener : public scfImplementation1<
	celTriggerMovableListener, iMovableListener>
{
private:
  csWeakRef<celPcTrigger> pctrigger;

public:
  celTriggerMovableListener (celPcTrigger* pctrigger)
  	: scfImplementationType (this), pctrigger (pctrigger)
  {
  }
  virtual ~celTriggerMovableListener () { }
  virtual void MovableChanged (iMovable* movable)
  {
    if (pctrigger)
    {

      csReversibleTransform tr = movable->GetFullTransform ();
      csVector3 pos = movable->GetPosition ();
      pctrigger->SetCenter (pos);
      if (movable->GetSectors()->GetCount())
        pctrigger->SetSector (movable->GetSectors()->Get(0));
    }
  }
  virtual void MovableDestroyed (iMovable*) { }
};


//---------------------------------------------------------------------------

csStringID celPcTrigger::id_sector = csInvalidStringID;
csStringID celPcTrigger::id_position = csInvalidStringID;
csStringID celPcTrigger::id_radius = csInvalidStringID;
csStringID celPcTrigger::id_entity = csInvalidStringID;
csStringID celPcTrigger::id_minbox = csInvalidStringID;
csStringID celPcTrigger::id_maxbox = csInvalidStringID;
csStringID celPcTrigger::id_start = csInvalidStringID;
csStringID celPcTrigger::id_end = csInvalidStringID;
csStringID celPcTrigger::id_maxdistance = csInvalidStringID;

PropertyHolder celPcTrigger::propinfo;

celPcTrigger::celPcTrigger (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  engine = csQueryRegistry<iEngine> (object_reg);
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);

  if (id_entity == csInvalidStringID)
  {
    id_entity = pl->FetchStringID ("entity");
    id_sector = pl->FetchStringID ("sector");
    id_position = pl->FetchStringID ("position");
    id_radius = pl->FetchStringID ("radius");
    id_minbox = pl->FetchStringID ("minbox");
    id_maxbox = pl->FetchStringID ("maxbox");
    id_start = pl->FetchStringID ("start");
    id_end = pl->FetchStringID ("end");
    id_maxdistance = pl->FetchStringID ("maxdistance");
  }
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_entity);

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.trigger.action.");
    AddAction (action_setuptriggersphere, "SetupTriggerSphere");
    AddAction (action_setuptriggerbox, "SetupTriggerBox");
    AddAction (action_setuptriggerbeam, "SetupTriggerBeam");
    AddAction (action_setuptriggerabovemesh, "SetupTriggerAboveMesh");
  };

  // For properties.
  propinfo.SetCount (9);
  AddProperty (propid_delay, "delay",
  	CEL_DATA_LONG, false, "Update delay to check for entities.", 0);
  AddProperty (propid_jitter, "jitter",
  	CEL_DATA_LONG, false, "Random jitter to add to update delay.", 0);
  AddProperty (propid_monitor, "monitor",
  	CEL_DATA_STRING, false, "Entity name to monitor.", 0);
  AddProperty (propid_class, "class",
  	CEL_DATA_STRING, false, "Entity class to monitor.", 0);
  AddProperty (propid_invisible, "invisible",
  	CEL_DATA_BOOL, false, "Monitor invisible entities.", 0);
  AddProperty (propid_follow, "follow",
  	CEL_DATA_BOOL, false, "Follow own entity pcmesh.", 0);
  AddProperty (propid_enabled, "enabled",
  	CEL_DATA_BOOL, false, "Enable/Disable trigger.", 0);
  AddProperty (propid_strict, "strict",
  	CEL_DATA_BOOL, false, "Use strict checking for monitored entity.",
			&strict);
  AddProperty (propid_type, "type",
  	CEL_DATA_LONG, true, "Trigger type.", &trigger_type);

  enabled = true;
  strict = true;
  trigger_type = TRIGGER_NONE;
  send_to_self = true;
  send_to_others = true;
  above_collider = 0;
  SetMonitorDelay (200, 10);
  follow = false;

  sector = 0;

  monitor_invisible = false;
  monitor_class_id = csInvalidStringID;
}

celPcTrigger::~celPcTrigger ()
{
  send_to_self = false;	// Prevent sending messages to this entity.
  send_to_others = false;
  LeaveAllEntities ();
  if (pl)
    pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  delete params;
}

void celPcTrigger::Activate ()
{
  ResetCallbackOnce ();
}

void celPcTrigger::Deactivate ()
{
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
}

void celPcTrigger::SetCenter (csVector3 &v)
{
  switch(trigger_type)
  {
    case TRIGGER_SPHERE:
      sphere_center = v;
      break;
    case TRIGGER_BOX:
      box_area.SetCenter (v);
      break;
    case TRIGGER_BEAM:
      beam_end = v + (beam_end-beam_start);
      beam_start = v;
      break;
    default:
      break;
  }
}

bool celPcTrigger::SetPropertyIndexed (int idx, long b)
{
  switch (idx)
  {
    case propid_delay:
      SetMonitorDelay (b, jitter);
      return true;
    case propid_jitter:
      SetMonitorDelay (delay, b);
      return true;
    default:
      return false;
  }
}

bool celPcTrigger::GetPropertyIndexed (int idx, long& b)
{
  switch (idx)
  {
    case propid_delay:
      b = delay;
      return true;
    case propid_jitter:
      b = jitter;
      return true;
    default:
      return false;
  }
}

bool celPcTrigger::SetPropertyIndexed (int idx, bool b)
{
  switch (idx)
  {
    case propid_invisible:
      EnableMonitorInvisible (b);
      return true;
    case propid_follow:
      follow = b;
      UpdateListener ();
      return true;
    case propid_enabled:
      EnableTrigger (b);
      return true;
    default:
      return false;
  }
}

bool celPcTrigger::GetPropertyIndexed (int idx, bool& b)
{
  switch (idx)
  {
    case propid_invisible:
      b = monitor_invisible;
      return true;
    case propid_follow:
      b = follow;
      return true;
    case propid_enabled:
      b = IsEnabled ();
      return true;
    default:
      return false;
  }
}

bool celPcTrigger::SetPropertyIndexed (int idx, const char* b)
{
  if (idx == propid_monitor)
  {
    MonitorEntity (b);
    return true;
  }
  else if (idx == propid_class)
  {
    MonitorClass (b);
    return true;
  }
  return false;
}

bool celPcTrigger::GetPropertyIndexed (int idx, const char*& b)
{
  if (idx == propid_monitor)
  {
    b = monitor_entity;
    return true;
  }
  else if (idx == propid_class)
  {
    b = monitor_class;
    return true;
  }
  return false;
}

void celPcTrigger::MonitorEntity (const char* entityname)
{
  LeaveAllEntities ();
  monitoring_entity = 0;
  monitoring_entity_pcmesh = 0;
  monitor_entity = entityname;
}

void celPcTrigger::MonitorClass (const char* classname)
{
  LeaveAllEntities ();
  if (classname)
  {
    monitor_class = classname;
    monitor_class_id = pl->FetchStringID (monitor_class);
  }
  else
  {
    monitor_class.Empty ();
    monitor_class_id = csInvalidStringID;
  }
}

void celPcTrigger::ResetCallbackOnce()
{
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  if (enabled)
    SetCallbackOnce();
}

void celPcTrigger::SetCallbackOnce()
{
  csTicks delay = celPcTrigger::delay;

  if (jitter)
    delay+=(rand () % (jitter+jitter))-jitter;

  pl->CallbackOnce ((iCelTimerListener*)this, delay, CEL_EVENT_PRE);
}


void celPcTrigger::EnableTrigger (bool en)
{
  enabled = en;
  ResetCallbackOnce();
}

void celPcTrigger::SetMonitorDelay (csTicks delay, csTicks jitter)
{
  celPcTrigger::delay = delay;
  celPcTrigger::jitter = jitter;
  ResetCallbackOnce();
}

void celPcTrigger::LeaveAllEntities ()
{
  size_t i;
  for (i = 0 ; i < entities_in_trigger.GetSize () ; i++)
    if (entities_in_trigger[i])
    {
      if (send_to_self)
      {
        FireTriggersEntityLeaves (entities_in_trigger[i]);
        SendTriggerMessage (entity, entities_in_trigger[i],
        	"pctrigger_entityleaves", "cel.trigger.entity.leave",
		&dispatcher_leave);
      }
      if (send_to_others)
      {
        FireTriggersLeaveTrigger (entities_in_trigger[i]);
        SendTriggerMessage (entities_in_trigger[i], entity,
        	"pctrigger_leavetrigger",
		"cel.trigger.entity.leave.this", 0);
      }
    }
  entities_in_trigger.SetSize (0);
}

size_t celPcTrigger::EntityInTrigger (iCelEntity* entity)
{
  size_t i;
  for (i = 0 ; i < entities_in_trigger.GetSize () ; i++)
    if (entities_in_trigger[i] == entity) return i;
  return csArrayItemNotFound;
}

void celPcTrigger::UpdateRelevantSectors ()
{
  if (!relevant_sectors.IsEmpty ()) return;
  if (!sector) return;
  csRef<iSectorIterator> sector_it;
  switch(trigger_type)
  {
    case TRIGGER_BOX:
      sector_it = engine->GetNearbySectors (sector, box_area);
      break;
    case TRIGGER_SPHERE:
      sector_it = engine->GetNearbySectors (sector, sphere_center,
          sphere_radius);
      break;
    case TRIGGER_BEAM:
    {
      float radius = sqrt (csSquaredDist::PointPoint (beam_start, beam_end));
      sector_it = engine->GetNearbySectors (sector,
    	(beam_start + beam_end) / 2.0f, radius);
      break;
    }
    default:
      break;
  }
  while (sector_it->HasNext ())
  {
    iSector* sector = sector_it->Next ();
    const csVector3& pos = sector_it->GetLastPosition ();
    relevant_sectors.Put (sector, pos);
  }
}

void celPcTrigger::SetupTriggerSphere (iSector* sector,
	const csVector3& center, float radius)
{
  LeaveAllEntities ();
  above_mesh = 0;
  trigger_type = TRIGGER_SPHERE;

  celPcTrigger::sector = sector;
  sphere_center = center;
  sphere_radius = radius;
  relevant_sectors.Empty ();
}

void celPcTrigger::SetupTriggerSphere (iSector* sector,
	const char* center_name, float radius)
{
  LeaveAllEntities ();
  above_mesh = 0;
  trigger_type = TRIGGER_SPHERE;

  csRef<iMapNode> mapnode = CS::GetNamedChildObject<iMapNode> (
  	sector->QueryObject (), center_name);
  if (!mapnode)
  {
    Error ("Can't find node '%s' for trigger!",
    	(const char*)center_name);
    // @@@ Function should return bool!
    return;
  }

  celPcTrigger::sector = sector;
  sphere_center = mapnode->GetPosition ();
  sphere_radius = radius;
  relevant_sectors.Empty ();
}

void celPcTrigger::SetupTriggerBox (iSector* sector, const csBox3& box)
{
  LeaveAllEntities ();
  above_mesh = 0;
  trigger_type = TRIGGER_BOX;

  celPcTrigger::sector = sector;
  box_area = box;
  relevant_sectors.Empty ();
}

void celPcTrigger::SetupTriggerBeam (iSector* sector, const csVector3& start,
	const csVector3& end)
{
  LeaveAllEntities ();
  above_mesh = 0;
  trigger_type = TRIGGER_BEAM;

  celPcTrigger::sector = sector;
  beam_start = start;
  beam_end = end;
  relevant_sectors.Empty ();
}

void celPcTrigger::SetupTriggerAboveMesh (iPcMesh* m, float maxdistance)
{
  LeaveAllEntities ();
  sector = 0;
  trigger_type = TRIGGER_ABOVE;

  above_mesh = m;
  above_maxdist = maxdistance;
  above_collider = 0;
  relevant_sectors.Empty ();
}

bool celPcTrigger::Check ()
{
  if (!monitor_entity.IsEmpty ())
  {
    // We want to monitor a single entity.
    if (!monitoring_entity)
    {
      // We haven't found the entity yet.
      monitoring_entity = pl->FindEntity (monitor_entity);
      if (!monitoring_entity)
      {
        if (strict)
          Error ("Can't find monitored entity '%s'!",
		monitor_entity.GetData());
        return false;
      }
      monitoring_entity_pcmesh = celQueryPropertyClassEntity<iPcMesh> (monitoring_entity);
      size_t idx = EntityInTrigger (monitoring_entity);
      return idx != csArrayItemNotFound;
    }
  }
  else
  {
    // @@@ Consider implementing this (using pl->FindNearbyEntities())
    // for the case of monitoring all entities of all entities of
    // a single class.
  }
  return false;
}

void celPcTrigger::UpdateListener ()
{
  // Remove listener if present
  if (movlistener)
  {
    if (movable_for_listener)
      movable_for_listener->RemoveListener (movlistener);
    movlistener = 0;
  }
  // Create a new listener if possible and requested
  if (follow && !above_mesh)
  {
    csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    if (pcmesh && pcmesh->GetMesh ())
    {
      movlistener.AttachNew (new celTriggerMovableListener (
      	this));
      movable_for_listener = pcmesh->GetMesh ()->GetMovable ();
      movable_for_listener->AddListener (movlistener);
      movlistener->MovableChanged (movable_for_listener);
    }
    else
    {
      // We want to try again to get the listener.
      PropertyClassesHaveChanged ();
    }
  }
}

void celPcTrigger::TickOnce ()
{
  // First find the information for the 'check above' case.
  csReversibleTransform above_trans;
  if (HavePropertyClassesChanged ())
    UpdateListener ();
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

  if (!monitor_entity.IsEmpty ())
  {
    // We want to monitor a single entity.
    if (!monitoring_entity)
    {
      // We haven't found the entity yet.
      monitoring_entity = pl->FindEntity (monitor_entity);
      if (!monitoring_entity)
      {
        if (strict)
          Error ("Can't find monitored entity '%s'!",
			monitor_entity.GetData());
        goto end;
      }
      monitoring_entity_pcmesh = celQueryPropertyClassEntity<iPcMesh> (monitoring_entity);
    }
    if (monitoring_entity_pcmesh)
    {
      // We have an entity to monitor.
      size_t idx = EntityInTrigger (monitoring_entity);
      iMeshWrapper* monitoring_mesh = monitoring_entity_pcmesh->GetMesh ();
      iMovable* movable = monitoring_mesh->GetMovable ();
      csVector3 mpos = movable->GetFullTransform ().GetOrigin ();
      iSector* sector = movable->GetSectors ()->Get (0);

      bool trigger_fired = false;
      switch (trigger_type)
      {
        case TRIGGER_SPHERE:
          // See how far it is from our trigger center.
          if (celPcTrigger::sector == sector)
          {
            float sqdistance = csSquaredDist::PointPoint (mpos, sphere_center);
            trigger_fired = sqdistance < sphere_radius * sphere_radius;
          }
          else
          {
            UpdateRelevantSectors ();
            csVector3* warp_center = relevant_sectors.GetElementPointer (sector);
            if (warp_center)
            {
              float sqdistance = csSquaredDist::PointPoint (mpos, *warp_center);
              trigger_fired = sqdistance < sphere_radius * sphere_radius;
            }
          }
          break;
        case TRIGGER_BOX:
          if (celPcTrigger::sector == sector)
          {
            trigger_fired = box_area.In (mpos);
          }
          else
          {
            UpdateRelevantSectors ();
            csVector3* warp_center = relevant_sectors.GetElementPointer (sector);
            if (warp_center)
            {
              csBox3 warp_box = box_area;
              warp_box.SetCenter (*warp_center);
              trigger_fired = warp_box.In (mpos);
            }
          }
          break;
        case TRIGGER_BEAM:
          if (celPcTrigger::sector == sector)
          {
            csHitBeamResult rc = monitoring_mesh->HitBeam (beam_start, beam_end);
            trigger_fired = rc.hit;
          }
          else
          {
            UpdateRelevantSectors ();
            csVector3* warp_center = relevant_sectors.GetElementPointer (sector);
            if (warp_center)
            {
              // @@@ Warp beam_start and beam_end somehow!!!
              csHitBeamResult rc = monitoring_mesh->HitBeam (
            	beam_start, beam_end);
              trigger_fired = rc.hit;
            }
          }
          break;
        default:
          csVector3 end (mpos.x, mpos.y - above_maxdist, mpos.z);
          // Small correction to make sure we don't miss the object that
          // we're standing on.
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
            	"pctrigger_entityenters",
		"cel.trigger.entity.enter", &dispatcher_enter);
          }
          if (send_to_others)
          {
            FireTriggersEnterTrigger (monitoring_entity);
            SendTriggerMessage (monitoring_entity, entity,
            	"pctrigger_entertrigger",
		"cel.trigger.entity.enter.this", 0);
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
            	"pctrigger_entityleaves",
		"cel.trigger.entity.leave",
		&dispatcher_leave);
          }
          if (send_to_others)
          {
            FireTriggersLeaveTrigger (monitoring_entity);
            SendTriggerMessage (monitoring_entity, entity,
            	"pctrigger_leavetrigger",
		"cel.trigger.entity.leave.this", 0);
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
    switch(trigger_type)
    {
      case TRIGGER_SPHERE:
        list = pl->FindNearbyEntities (sector,
      	  sphere_center, sphere_radius, monitor_invisible,
	  monitor_class_id);
        break;
      case TRIGGER_BOX:
        list = pl->FindNearbyEntities (sector, box_area,
      	  monitor_invisible, monitor_class_id);
        break;
      case TRIGGER_BEAM:
        list = pl->FindNearbyEntities (sector, beam_start, beam_end,
      	  monitor_invisible, monitor_class_id);
        break;
      case TRIGGER_ABOVE:
      {
        csBox3 b;
        b = above_mesh->GetMesh ()->GetWorldBoundingBox ();
        iMovable* m = above_mesh->GetMesh ()->GetMovable ();
        list = pl->FindNearbyEntities (m->GetSectors ()->Get (0), b,
      	  monitor_invisible, monitor_class_id);
        break;
      }
      default:
        return;
    }
    size_t i;

    // Fill a set with all entities that are currently in trigger.
    csSet<csPtrKey<iCelEntity> > previous_entities;
    for (i = 0 ; i < entities_in_trigger.GetSize () ; i++)
      if (entities_in_trigger[i])
        previous_entities.Add ((iCelEntity*)entities_in_trigger[i]);

    // Now clear our entities_in_trigger table. We will fill it again.
    entities_in_trigger.SetSize (0);

    // Keep track of entities which have been processed already,
    // since list may have duplicates
    csSet<csPtrKey<iCelEntity> > processed_entities;

    // Traverse the entities that are near us.
    for (i = 0 ; i < list->GetCount () ; i++)
    {
      iCelEntity* ent = list->Get (i);

      if (processed_entities.In (ent))
        continue; // This entity was already considered, skip it.

      // If we are doing above a mesh then we have to test if the entity
      // is really above the mesh.
      if (above_mesh)
      {
        csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (ent);
        if (!pcmesh) continue;
        csVector3 origin = pcmesh->GetMesh ()->GetMovable ()
        	->GetFullPosition ();
        csVector3 end (origin.x, origin.y-above_maxdist, origin.z);
        // Small correction to make sure we don't miss the object that
	// we're standing on.
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
          SendTriggerMessage (entity, ent, "pctrigger_entityenters",
	      "cel.trigger.entity.enter", &dispatcher_enter);
        }
        if (send_to_others)
        {
          FireTriggersEnterTrigger (ent);
          SendTriggerMessage (ent, entity, "pctrigger_entertrigger",
	      "cel.trigger.entity.enter.this", 0);
        }
      }
      // Delete from the set.
      previous_entities.Delete (ent);
      // We don't need to process that entity in this loop again.
      processed_entities.Add (ent);
    }

    // All entities that are still in the set were in the trigger
    // last time but are not any longer.
    csSet<csPtrKey<iCelEntity> >::GlobalIterator it =
    	previous_entities.GetIterator ();
    while (it.HasNext ())
    {
      iCelEntity* ent = it.Next ();
      if (send_to_self)
      {
        FireTriggersEntityLeaves (ent);
        SendTriggerMessage (entity, ent, "pctrigger_entityleaves",
	    "cel.trigger.entity.leave", &dispatcher_leave);
      }
      if (send_to_others)
      {
        FireTriggersLeaveTrigger (ent);
        SendTriggerMessage (ent, entity, "pctrigger_leavetrigger",
	    "cel.trigger.entity.leave.this", 0);
      }
    }
  }
  end:
  SetCallbackOnce();
}

bool celPcTrigger::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setuptriggersphere:
      {
	csString sector;
	Fetch (sector, params, id_sector, true);

	float radius;
	if (!Fetch (radius, params, id_radius)) return false;
        iSector* sec = 0;
        if (!sector.IsEmpty ())
	{
	  sec = engine->FindSector (sector);
          if (!sec)
            return Error (
          	  "Can't find sector '%s' for action SetupTriggerSphere!",
          	  sector.GetData ());
	}
        const celData* p_position = params->GetParameter (id_position);
        if (!p_position)
          return Error (
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
          return Error (
          	"'position' must be string or vector for SetupTriggerSphere!");
        return true;
      }
    case action_setuptriggerbox:
      {
	csString sector;
	Fetch (sector, params, id_sector, true);
	csVector3 minbox, maxbox;
	if (!Fetch (minbox, params, id_minbox)) return false;
	if (!Fetch (maxbox, params, id_maxbox)) return false;
        iSector* sec = 0;
        if (!sector.IsEmpty ())
	{
          sec = engine->FindSector (sector);
          if (!sec)
            return Error (
          	  "Can't find sector '%s' for action SetupTriggerBox!",
          	  sector.GetData ());
	}
        SetupTriggerBox (sec, csBox3 (minbox, maxbox));
        return true;
      }
    case action_setuptriggerbeam:
      {
	csString sector;
	Fetch (sector, params, id_sector, true);
	csVector3 start, end;
	if (!Fetch (start, params, id_start)) return false;
	if (!Fetch (end, params, id_end)) return false;
        iSector* sec = 0;
        if (!sector.IsEmpty ())
	{
          sec = engine->FindSector (sector);
          if (!sec)
            return Error (
          	  "Can't find sector '%s' for action SetupTriggerBeam!",
          	  sector.GetData ());
	}
        SetupTriggerBeam (sec, start, end);
        return true;
      }
    case action_setuptriggerabovemesh:
      {
	csString entity;
	if (!Fetch (entity, params, id_entity)) return false;
	float maxdistance;
	if (!Fetch (maxdistance, params, id_maxdistance)) return false;
        iCelEntity* ent = pl->FindEntity (entity);
        if (!ent)
          return Error (
          	"Can't find entity '%s' for action SetupTriggerAboveMesh!",
          	entity.GetData ());
        csRef<iPcMesh> m = celQueryPropertyClassEntity<iPcMesh> (ent);
        if (!m)
          return Error (
          	"Entity '%s' doesn't support pcmesh (action SetupTriggerAboveMesh)!",
          	entity.GetData ());
        SetupTriggerAboveMesh (m, maxdistance);
        return true;
      }
    default:
      return false;
  }
}

void celPcTrigger::SendTriggerMessage (iCelEntity* destentity,
	iCelEntity* ent, const char* msgidold, const char* msg,
	csRef<iMessageDispatcher>* dispatcher)
{
  if (ent) params->GetParameter (0).Set (ent);
  iCelBehaviour* bh = destentity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage (msgidold, this, ret, params);
  }
  if (!dispatcher)
  {
    // Use direct message.
    destentity->QueryMessageChannel ()->SendMessage (pl->FetchStringID (msg),
		    this, params);
  }
  else
  {
    if (!*dispatcher)
    {
      *dispatcher = destentity->QueryMessageChannel ()->
        CreateMessageDispatcher (this, pl->FetchStringID (msg));
      if (!*dispatcher) return;
    }
    (*dispatcher)->SendMessage (params);
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
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->EntityEnters ((iPcTrigger*)this, entity);
  }
}

void celPcTrigger::FireTriggersEntityLeaves (iCelEntity* entity)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->EntityLeaves ((iPcTrigger*)this, entity);
  }
}

void celPcTrigger::FireTriggersEnterTrigger (iCelEntity* entity)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->EnterTrigger ((iPcTrigger*)this, entity);
  }
}

void celPcTrigger::FireTriggersLeaveTrigger (iCelEntity* entity)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->LeaveTrigger ((iPcTrigger*)this, entity);
  }
}

//---------------------------------------------------------------------------

