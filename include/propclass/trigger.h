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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_TRIGGER__
#define __CEL_PF_TRIGGER__

#include "cstypes.h"
#include "csgeom/vector3.h"
#include "csgeom/box.h"
#include "csutil/scf.h"
#include "csutil/weakrefarr.h"

struct iCelEntity;
struct iPcTrigger;
struct iPcMesh;

enum TriggerType
{
  TRIGGER_NONE = 0,
  TRIGGER_SPHERE,
  TRIGGER_BOX,
  TRIGGER_ABOVE,
  TRIGGER_BEAM
};

/**
 * Listen to trigger actions.
 */
struct iPcTriggerListener : public virtual iBase
{
  SCF_INTERFACE (iPcTriggerListener, 0, 0, 1);

  /**
   * An entity enters this trigger.
   */
  virtual void EntityEnters (iPcTrigger* trigger, iCelEntity* entity) = 0;

  /**
   * An entity leaves this trigger.
   */
  virtual void EntityLeaves (iPcTrigger* trigger, iCelEntity* entity) = 0;

  /**
   * This entity enters a trigger.
   */
  virtual void EnterTrigger (iPcTrigger* trigger, iCelEntity* entity) = 0;

  /**
   * This entity leaves a trigger.
   */
  virtual void LeaveTrigger (iPcTrigger* trigger, iCelEntity* entity) = 0;
};

/**
 * This is a trigger property class. It is capable of detecting when
 * another entity (that has a pcmesh) enters or leaves an area
 * defined by this trigger. In that case it can send messages to the
 * entity that contains this property class and/or the entities
 * that enter/leave the trigger area.
 *
 * This property class supports the following actions:
 * - SetupTriggerSphere: parameters 'sector' (string),
 *     'position' (vector3 or string (name of mapnode in that case)),
 *     and 'radius' (float).
 * - SetupTriggerBox: parameters 'sector' (string), 'minbox' (vector3),
 *     and 'maxbox' (vector3).
 * - SetupTriggerBeam: parameters 'sector' (string), 'start' (vector3),
 *     and 'end' (vector3).
 * - SetupTriggerAboveMesh: parameters 'entity' (string) and
 *     'maxdistance' (float),
 *
 * This property class can send out the following messages
 * to the behaviour:
 * - pctrigger_entityenters: an entity enters this trigger (entity).
 * - pctrigger_entityleaves: an entity leaves this trigger (entity).
 * - pctrigger_entertrigger: this entity enters a trigger (entity).
 * - pctrigger_leavetrigger: this entity leaves a trigger (entity).
 *
 * This property class supports the following properties:
 * - delay (long, read/write): update delay for checking trigger.
 * - jitter (long, read/write): random jitter added to update delay.
 * - monitor (string, read/write): name of entity to monitor.
 * - class (string, read/write): name of class to monitor.
 * - invisible (bool, read/write): also check on invisible entities
 *     (default off).
 * - follow (bool, read/write): follow own entity pcmesh (default off).
 * - enabled (bool, read/write): enable/disable triger (default on).
 * - strict (bool, read/write): do strict checking for monitored entity. if true
 *   an error will be reported if the entity is not there. (default on)
 * - type (long, read only): trigger type, a value from TriggerType.
 */
struct iPcTrigger : public virtual iBase
{
  SCF_INTERFACE (iPcTrigger, 0, 1, 1);

  /**
   * Add a trigger listener. Trigger listeners are called right before
   * the behaviour is called.
   */
  virtual void AddTriggerListener (iPcTriggerListener* listener) = 0;
  /**
   * Remove a trigger listener.
   */
  virtual void RemoveTriggerListener (iPcTriggerListener* listener) = 0;

  /**
   * Setup a spherical area as trigger zone.
   */
  virtual void SetupTriggerSphere (iSector* sector,
  	const csVector3& center, float radius) = 0;

  /**
   * Setup a spherical area as trigger zone. This version uses a
   * mapnode (\<node\> in XML) in the sector to get the position from.
   */
  virtual void SetupTriggerSphere (iSector* sector,
  	const char* center_name, float radius) = 0;

  /**
   * Setup a box trigger zone.
   */
  virtual void SetupTriggerBox (iSector* sector, const csBox3& box) = 0;

  /**
   * Setup a beam trigger zone.
   */
  virtual void SetupTriggerBeam (iSector* sector, const csVector3& start,
  	const csVector3& end) = 0;

  /**
   * Setup a trigger to fire if this mesh ends up above another mesh.
   * This works by tracing a beam down from the center of the mesh.
   */
  virtual void SetupTriggerAboveMesh (iPcMesh* mesh, float maxdistance) = 0;

  /**
   * By default pctrigger will monitor all entities. If you only want
   * pctrigger to monitor one entity then you can give the name of that
   * entity here. pctrigger will try to find the entity with that name
   * and monitor it. To go back to monitoring all entities just call
   * this function with a 0 name.
   */
  virtual void MonitorEntity (const char* entityname) = 0;

  /**
   * Return the entity name that we are currently monitoring or
   * 0 if monitoring all entities.
   */
  virtual const char* GetMonitorEntity () const = 0;

  /**
   * By default pctrigger will monitor all entities. You can also
   * let pctrigger monitor only entities with a specific class.
   * To disable this just use 0 as the class.
   */
  virtual void MonitorClass (const char* classname) = 0;

  /**
   * Return the class name that we are currently monitoring or
   * 0 if monitoring all entities.
   */
  virtual const char* GetMonitorClass () const = 0;

  /**
   * Set the number of milliseconds we delay before monitoring
   * all entities again. There is also a jitter parameter which will
   * add a small random amount to that time to prevent all triggers
   * trying to do the monitoring at once. By default this is set
   * to 200/10 (i.e. roughly five times per second).
   */
  virtual void SetMonitorDelay (csTicks delay, csTicks jitter) = 0;

  /**
   * Enable monitoring on invisible entities. Default off.
   * This option is only useful in case many entities are monitored
   * (not only a single entity).
   */
  virtual void EnableMonitorInvisible (bool en) = 0;

  /**
   * Enable/disable sending messages to the entity containing
   * the trigger about other entities leaving of entering (messages
   * like pctrigger_entityenters and pctrigger_entityleaves).
   * This is enabled by default.
   */
  virtual void EnableMessagesToSelf (bool en) = 0;

  /**
   * Enable/disable sending messages to the entities that
   * enter or leave this trigger (messages
   * like pctrigger_entertrigger and pctrigger_leavetrigger).
   * This is enabled by default.
   */
  virtual void EnableMessagesToOthers (bool en) = 0;

  /**
   * Enable/Disable trigger. Triggers are enabled by default.
   */
  virtual void EnableTrigger (bool en) = 0;

  /**
   * Return true if trigger is enabled.
   */
  virtual bool IsEnabled () const = 0;

  /**
   * Get an array of all entities that are currently
   * in the trigger area.
   */
  virtual const csWeakRefArray<iCelEntity>& GetEntitiesInTrigger () const = 0;

  /**
   * This only works if we are monitoring an individual entity. In that
   * case it will return true if the entity is already in the trigger.
   */
  virtual bool Check () = 0;

  /// Get update delay for checking trigger.
  virtual csTicks GetUpdateDelay () const = 0;
  /// Get random jitter for update delay.
  virtual csTicks GetUpdateJitter () const = 0;
  /// Get flag to control checking on invisible entities.
  virtual bool HasCheckingOnInvisibleEntities () const = 0;
  /// Get flag to let the trigger follow its own mesh.
  virtual bool HasFollowEntity () const = 0;
  /// Set flag to let the trigger follow its own mesh (default off).
  virtual void SetFollowEntity (bool c) = 0;
  /// Get flag to let the trigger do strict checking for monitored entity.
  virtual bool HasStrictChecking () const = 0;
  /**
   * Set flag to let the trigger do strict checking for monitored entity.
   * If true an error will be reported if the entity is not there. Default on.
   */
  virtual void SetStrictChecking (bool c) = 0;
  /// Get the type of this trigger.
  virtual TriggerType GetTriggerType () const = 0;
};

#endif // __CEL_PF_TRIGGER__

