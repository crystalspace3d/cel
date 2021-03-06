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

#ifndef __CEL_PF_TRIGGERIMP__
#define __CEL_PF_TRIGGERIMP__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csgeom/box.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/weakrefarr.h"
#include "csutil/refarr.h"
#include "csutil/util.h"
#include "csutil/hash.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "physicallayer/entity.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/trigger.h"
#include "iengine/camera.h"
#include "iengine/movable.h"

class celTriggerMovableListener;
struct iCelEntity;
struct iObjectRegistry;
struct iEngine;
struct iCollideSystem;
struct iCollider;
struct iCelPlLayer;
struct iPcMesh;
class celPcTrigger;

/**
 * Factory for trigger.
 */
CEL_DECLARE_FACTORY(Trigger)

/**
 * This is the trigger property class.
 */
class celPcTrigger : public scfImplementationExt1<
	celPcTrigger, celPcCommon, iPcTrigger>
{
private:
  csWeakRef<iEngine> engine;
  csWeakRef<iCollideSystem> cdsys;

  csWeakRefArray<iCelEntity> entities_in_trigger;
  bool enabled;
  bool send_to_self;
  bool send_to_others;

  csString monitor_entity;
  csString monitor_class;
  csStringID monitor_class_id;
  csWeakRef<iCelEntity> monitoring_entity;
  csWeakRef<iPcMesh> monitoring_entity_pcmesh;
  // Movable listener so we can update trigger center
  csWeakRef<iMovable> movable_for_listener;
  csRef<celTriggerMovableListener> movlistener;

  // Listeners.
  csRefArray<iPcTriggerListener> listeners;

  csTicks delay, jitter;

  static csStringID id_sector;
  static csStringID id_position;
  static csStringID id_radius;
  static csStringID id_entity;
  static csStringID id_minbox;
  static csStringID id_maxbox;
  static csStringID id_start;
  static csStringID id_end;
  static csStringID id_maxdistance;
  celOneParameterBlock* params;

  csRef<iMessageDispatcher> dispatcher_leave;
  csRef<iMessageDispatcher> dispatcher_enter;

  enum actionids
  {
    action_setuptriggersphere = 0,
    action_setuptriggerbox,
    action_setuptriggerbeam,
    action_setuptriggerabovemesh
  };

  // This is a hash of sectors and warped positions
  // that could be relevant for this trigger. It contains at
  // least the source sector of the trigger sphere/box/beam and all
  // sectors near it.
  csHash<csVector3,csPtrKey<iSector> > relevant_sectors;
  void UpdateRelevantSectors ();

  // Sphere to use for checking.
  iSector* sector;
  csVector3 sphere_center;
  float sphere_radius;
  // Box to use for checking.
  csBox3 box_area;
  // Beam to use for checking.
  csVector3 beam_start, beam_end;
  // Test if we're directly above some mesh.
  csWeakRef<iPcMesh> above_mesh;
  iCollider* above_collider;
  float above_maxdist;

  // Whether to follow own entity movable
  bool follow;

  // Monitor invisible entities too.
  bool monitor_invisible;

  TriggerType trigger_type;

  bool strict;

  // For properties.
  enum propids
  {
    propid_delay = 0,
    propid_jitter,
    propid_monitor,
    propid_class,
    propid_invisible,
    propid_follow,
    propid_enabled,
    propid_strict,
    propid_type,
  };
  static PropertyHolder propinfo;

  // Create/Destroy the listener if necessary
  void UpdateListener ();

  // Leave all entities (sends a message to self and others and then
  // clears 'entities_in_trigger').
  void LeaveAllEntities ();

  // Return index in entity array if this entity is known to be in the
  // trigger already. Otherwise returns csArrayItemNotFound.
  size_t EntityInTrigger (iCelEntity* entity);
  // Set callback once with a jitter.
  void SetCallbackOnce();
  // Unset previous callback once, and set it again.
  void ResetCallbackOnce();
public:
  celPcTrigger (iObjectRegistry* object_reg);
  virtual ~celPcTrigger ();

  iEngine* GetEngine () const { return engine; }

  virtual bool PerformActionIndexed (int, iCelParameterBlock* params,
      celData& ret);
  virtual void TickOnce ();
  virtual void Activate ();
  virtual void Deactivate ();

  // celPcTrigger only function to set center.
  void SetCenter (csVector3 &v);

  // Override SetProperty from celPcCommon in order to provide support
  // for the properties.
  virtual bool SetPropertyIndexed (int, long);
  virtual bool GetPropertyIndexed (int, long&);
  virtual bool SetPropertyIndexed (int, bool);
  virtual bool GetPropertyIndexed (int, bool&);
  virtual bool SetPropertyIndexed (int, const char*);
  virtual bool GetPropertyIndexed (int, const char*&);

  virtual void SendTriggerMessage (iCelEntity* destentity,
  	iCelEntity* entity, const char* msgidold, const char* msg,
	csRef<iMessageDispatcher>* dispatcher);

  virtual void SetupTriggerSphere (iSector* sector,
  	const csVector3& center, float radius);
  virtual void SetupTriggerSphere (iSector* sector,
  	const char* center_name, float radius);
  virtual void SetupTriggerBox (iSector* sector, const csBox3& box);
  virtual void SetupTriggerBeam (iSector* sector, const csVector3& start,
  	const csVector3& end);
  virtual void SetupTriggerAboveMesh (iPcMesh* mesh, float maxdistance);
  virtual void MonitorEntity (const char* entityname);
  virtual const char* GetMonitorEntity () const { return monitor_entity; }
  virtual void MonitorClass (const char* classname);
  virtual const char* GetMonitorClass () const { return monitor_class; }
  virtual void SetMonitorDelay (csTicks delay, csTicks jitter);
  virtual void EnableMonitorInvisible (bool en) { monitor_invisible = en; }
  virtual void EnableMessagesToSelf (bool en) { send_to_self = en; }
  virtual void EnableMessagesToOthers (bool en) { send_to_others = en; }
  virtual void EnableTrigger (bool en);
  virtual bool IsEnabled () const { return enabled; }
  virtual const csWeakRefArray<iCelEntity>& GetEntitiesInTrigger () const
  {
    return entities_in_trigger;
  }
  virtual void AddTriggerListener (iPcTriggerListener* listener);
  virtual void RemoveTriggerListener (iPcTriggerListener* listener);

  virtual csTicks GetUpdateDelay () const { return delay; }
  virtual csTicks GetUpdateJitter () const { return jitter; }
  virtual bool HasCheckingOnInvisibleEntities () const { return monitor_invisible; }
  virtual bool HasFollowEntity () const { return follow; }
  virtual void SetFollowEntity (bool c) { follow = c; UpdateListener (); }
  virtual bool HasStrictChecking () const { return strict; }
  virtual void SetStrictChecking (bool c) { strict = c; }
  virtual TriggerType GetTriggerType () const { return trigger_type; }

  void FireTriggersEntityEnters (iCelEntity* entity);
  void FireTriggersEntityLeaves (iCelEntity* entity);
  void FireTriggersEnterTrigger (iCelEntity* entity);
  void FireTriggersLeaveTrigger (iCelEntity* entity);
  virtual bool Check ();
  void SetSector(iSector *sector) { celPcTrigger::sector=sector; };
};

#endif // __CEL_PF_TRIGGERIMP__

