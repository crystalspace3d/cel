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

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
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
    id_entity = pl->FetchStringID ("cel.parameter.entity");
    id_sector = pl->FetchStringID ("cel.parameter.sector");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_radius = pl->FetchStringID ("cel.parameter.radius");
    id_minbox = pl->FetchStringID ("cel.parameter.minbox");
    id_maxbox = pl->FetchStringID ("cel.parameter.maxbox");
    id_start = pl->FetchStringID ("cel.parameter.start");
    id_end = pl->FetchStringID ("cel.parameter.end");
    id_maxdistance = pl->FetchStringID ("cel.parameter.maxdistance");
  }
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_entity, "entity");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_setuptriggersphere, "cel.action.SetupTriggerSphere");
    AddAction (action_setuptriggerbox, "cel.action.SetupTriggerBox");
    AddAction (action_setuptriggerbeam, "cel.action.SetupTriggerBeam");
    AddAction (action_setuptriggerabovemesh, "cel.action.SetupTriggerAboveMesh");
  };

  // For properties.
  propinfo.SetCount (5);
  AddProperty (propid_delay, "cel.property.delay",
	CEL_DATA_LONG, false, "Update delay to check for entities.", 0);
  AddProperty (propid_jitter, "cel.property.jitter",
	CEL_DATA_LONG, false, "Random jitter to add to update delay.", 0);
  AddProperty (propid_monitor, "cel.property.monitor",
	CEL_DATA_STRING, false, "Entity name to monitor.", 0);
  AddProperty (propid_invisible, "cel.property.invisible",
	CEL_DATA_BOOL, false, "Monitor invisible entities.", 0);
  AddProperty (propid_follow, "cel.property.follow",
	CEL_DATA_BOOL, false, "Follow own entity pcmesh.", 0);

  enabled = true;
  send_to_self = true;
  send_to_others = true;
  monitor_entity = 0;
  above_collider = 0;
  SetMonitorDelay (200, 10);
  follow = false;

  sphere_sector = 0;
  box_sector = 0;
  beam_sector = 0;

  monitor_invisible = false;
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
}

void celPcTrigger::SetCenter (csVector3 &v)
{
  if (above_mesh)
    return;
  else if (box_sector)
  {
    box_area.SetCenter(v);
  }
  else if (sphere_sector)
  {
    sphere_center = v;
  }
  else if (beam_sector)
  {
    beam_end = v+(beam_end-beam_start);
    beam_start = v;
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
      UpdateListener();
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
  return false;
}

bool celPcTrigger::GetPropertyIndexed (int idx, const char*& b)
{
  if (idx == propid_monitor)
  {
    b = monitor_entity;
    return true;
  }
  return false;
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

void celPcTrigger::UpdateRelevantSectors ()
{
  if (!relevant_sectors.IsEmpty ()) return;
  csRef<iSectorIterator> sector_it;
  if (box_sector)
    sector_it = engine->GetNearbySectors (box_sector, box_area);
  else if (sphere_sector)
    sector_it = engine->GetNearbySectors (sphere_sector, sphere_center,
        sphere_radius);
  else if (beam_sector)
  {
    float radius = sqrt (csSquaredDist::PointPoint (beam_start, beam_end));
    sector_it = engine->GetNearbySectors (beam_sector,
	(beam_start+beam_end)/2.0f, radius);
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
  box_sector = 0;
  beam_sector = 0;
  above_mesh = 0;

  sphere_sector = sector;
  sphere_center = center;
  sphere_radius = radius;
  relevant_sectors.Empty ();
}

void celPcTrigger::SetupTriggerSphere (iSector* sector,
	const char* center_name, float radius)
{
  LeaveAllEntities ();
  box_sector = 0;
  beam_sector = 0;
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
  relevant_sectors.Empty ();
}

void celPcTrigger::SetupTriggerBox (iSector* sector, const csBox3& box)
{
  LeaveAllEntities ();
  sphere_sector = 0;
  beam_sector = 0;
  above_mesh = 0;

  box_sector = sector;
  box_area = box;
  relevant_sectors.Empty ();
}

void celPcTrigger::SetupTriggerBeam (iSector* sector, const csVector3& start,
	const csVector3& end)
{
  LeaveAllEntities ();
  sphere_sector = 0;
  box_sector = 0;
  above_mesh = 0;

  beam_sector = sector;
  beam_start = start;
  beam_end = end;
  relevant_sectors.Empty ();
}

void celPcTrigger::SetupTriggerAboveMesh (iPcMesh* m, float maxdistance)
{
  LeaveAllEntities ();
  sphere_sector = 0;
  beam_sector = 0;
  box_sector = 0;

  above_mesh = m;
  above_maxdist = maxdistance;
  above_collider = 0;
  relevant_sectors.Empty ();
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
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    if (pcmesh)
    {
      movlistener.AttachNew (new celTriggerMovableListener (
        this));
      movable_for_listener = pcmesh->GetMesh ()->GetMovable ();
      movable_for_listener->AddListener (movlistener);
      movlistener->MovableChanged(movable_for_listener);
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
      iMeshWrapper* monitoring_mesh = monitoring_entity_pcmesh->GetMesh ();
      iMovable* movable = monitoring_mesh->GetMovable ();
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
      }
      else if (box_sector)
      {
        if (box_sector == sector)
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
      }
      else if (beam_sector)
      {
        if (beam_sector == sector)
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
      }
      else
      {
        csVector3 end (mpos.x, mpos.y-above_maxdist, mpos.z);
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
    {
      list = pl->FindNearbyEntities (sphere_sector,
      	sphere_center, sphere_radius, monitor_invisible);
    }
    else if (box_sector)
    {
      list = pl->FindNearbyEntities (box_sector, box_area,
      	monitor_invisible);
    }
    else if (beam_sector)
    {
      list = pl->FindNearbyEntities (beam_sector, beam_start, beam_end,
      	monitor_invisible);
    }
    else
    {
      csBox3 b;
      b = above_mesh->GetMesh ()->GetWorldBoundingBox ();
      iMovable* m = above_mesh->GetMesh ()->GetMovable ();
      list = pl->FindNearbyEntities (m->GetSectors ()->Get (0), b,
      	monitor_invisible);
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
    csSet<csPtrKey<iCelEntity> >::GlobalIterator it =
    	previous_entities.GetIterator ();
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
  databuf->Add (monitor_invisible);
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
    databuf->Add (above_mesh != 0);
    databuf->Add (above_maxdist);
  }
  else if (beam_sector)
  {
    databuf->Add ((uint8)4);
    databuf->Add (beam_sector->QueryObject ()->GetName ());
    databuf->Add (beam_start);
    databuf->Add (beam_end);
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
  databuf->Add (monitoring_entity_pcmesh != 0);

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
  monitor_invisible = databuf->GetBool ();
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
      pcmesh = scfQueryInterface<iPcMesh> (pc);
    float r = databuf->GetFloat ();
    if (pcmesh)
      SetupTriggerAboveMesh (pcmesh, r);
  }
  else if (t == 4)
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
    SetupTriggerBeam (sector, mi, ma);
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
  if (pc) monitoring_entity_pcmesh = scfQueryInterface<iPcMesh> (pc);

  return true;
}

bool celPcTrigger::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setuptriggersphere:
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
    case action_setuptriggerbox:
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
    case action_setuptriggerbeam:
      {
        CEL_FETCH_STRING_PAR (sector,params,id_sector);
        if (!p_sector)
          return Report (object_reg,
      	    "Missing parameter 'sector' for action SetupTriggerBeam!");
        CEL_FETCH_VECTOR3_PAR (start,params,id_start);
        if (!p_start)
          return Report (object_reg,
      	    "Missing parameter 'start' for action SetupTriggerBeam!");
        CEL_FETCH_VECTOR3_PAR (end,params,id_end);
        if (!p_end)
          return Report (object_reg,
      	    "Missing parameter 'end' for action SetupTriggerBeam!");
        iSector* sec = engine->FindSector (sector);
        if (!sec)
          return Report (object_reg,
      	    "Can't find sector '%s' for action SetupTriggerBeam!", sector);
        SetupTriggerBeam (sec, start, end);
        return true;
      }
    case action_setuptriggerabovemesh:
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
    default:
      return false;
  }
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
    listeners[i]->EntityEnters ((iPcTrigger*)this, entity);
  }
}

void celPcTrigger::FireTriggersEntityLeaves (iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->EntityLeaves ((iPcTrigger*)this, entity);
  }
}

void celPcTrigger::FireTriggersEnterTrigger (iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->EnterTrigger ((iPcTrigger*)this, entity);
  }
}

void celPcTrigger::FireTriggersLeaveTrigger (iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->LeaveTrigger ((iPcTrigger*)this, entity);
  }
}

//---------------------------------------------------------------------------
