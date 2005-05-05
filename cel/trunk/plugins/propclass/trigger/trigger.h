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

struct iCelEntity;
struct iObjectRegistry;
struct iEngine;
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
class celPcTrigger : public celPcCommon
{
private:
  csRef<iEngine> engine;

  csWeakRefArray<iCelEntity> entities_in_trigger;
  bool enabled;
  bool send_to_self;
  bool send_to_others;

  char* monitor_entity;
  csWeakRef<iCelEntity> monitoring_entity;
  csWeakRef<iPcMesh> monitoring_entity_pcmesh;

  csTicks delay, jitter;

  static csStringID id_entity;
  celOneParameterBlock* params;

  // Sphere to use for checking.
  iSector* sphere_sector;
  csVector3 sphere_center;
  float sphere_radius;

  // For properties.
  enum propids
  {
    propid_delay = 0,
    propid_jitter,
    propid_monitor
  };
  static Property* properties;
  static size_t propertycount;
  static void UpdateProperties (iObjectRegistry* object_reg);

  // Leave all entities (sends a message to self and others and then
  // clears 'entities_in_trigger').
  void LeaveAllEntities ();

  // Return index in entity array if this entity is known to be in the
  // trigger already. Otherwise returns csArrayItemNotFound.
  size_t EntityInTrigger (iCelEntity* entity);

public:
  celPcTrigger (iObjectRegistry* object_reg);
  virtual ~celPcTrigger ();

  iEngine* GetEngine () const { return engine; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pctrigger"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID, iCelParameterBlock* params);
  virtual void TickOnce ();

  // Override SetProperty from celPcCommon in order to provide support
  // for the properties.
  virtual bool SetProperty (csStringID, long);
  virtual long GetPropertyLong (csStringID);
  virtual bool SetProperty (csStringID, const char*);
  virtual const char* GetPropertyString (csStringID);

  void SendTriggerMessage (iCelEntity* destentity,
  	iCelEntity* entity, const char* msgid);

  void SetupTriggerSphere (iSector* sector,
  	const csVector3& center, float radius);
  void SetupTriggerBox (iSector* sector, const csBox3& box);
  void MonitorEntity (const char* entityname);
  const char* GetMonitorEntity () const { return monitor_entity; }
  void SetMonitorDelay (csTicks delay, csTicks jitter);
  void EnableMessagesToSelf (bool en) { send_to_self = en; }
  void EnableMessagesToOthers (bool en) { send_to_others = en; }
  void EnableTrigger (bool en);
  bool IsEnabled () const { return enabled; }
  const csWeakRefArray<iCelEntity>& GetEntitiesInTrigger () const
  {
    return entities_in_trigger;
  }

  struct PcTrigger : public iPcTrigger
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcTrigger);
    virtual void SetupTriggerSphere (iSector* sector,
    	const csVector3& center, float radius)
    {
      scfParent->SetupTriggerSphere (sector, center, radius);
    }
    virtual void SetupTriggerBox (iSector* sector, const csBox3& box)
    {
      scfParent->SetupTriggerBox (sector, box);
    }
    virtual void MonitorEntity (const char* entityname)
    {
      scfParent->MonitorEntity (entityname);
    }
    virtual const char* GetMonitorEntity () const
    {
      return scfParent->GetMonitorEntity ();
    }
    virtual void SetMonitorDelay (csTicks delay, csTicks jitter)
    {
      scfParent->SetMonitorDelay (delay, jitter);
    }
    virtual void EnableMessagesToSelf (bool en)
    {
      scfParent->EnableMessagesToSelf (en);
    }
    virtual void EnableMessagesToOthers (bool en)
    {
      scfParent->EnableMessagesToOthers (en);
    }
    virtual void EnableTrigger (bool en)
    {
      scfParent->EnableTrigger (en);
    }
    virtual bool IsEnabled () const
    {
      return scfParent->IsEnabled ();
    }
    virtual const csWeakRefArray<iCelEntity>& GetEntitiesInTrigger () const
    {
      return scfParent->GetEntitiesInTrigger ();
    }
  } scfiPcTrigger;
};

#endif // __CEL_PF_TRIGGERIMP__

