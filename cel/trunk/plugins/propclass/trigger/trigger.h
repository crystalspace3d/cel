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
struct iLoader;
struct iVFS;
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

  csArray<iCelEntity*> entities_in_trigger;
  bool enabled;
  bool send_to_self;
  bool send_to_others;

  static csStringID id_entity;
  celOneParameterBlock* params;

public:
  celPcTrigger (iObjectRegistry* object_reg);
  virtual ~celPcTrigger ();

  iEngine* GetEngine () const { return engine; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pctrigger"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  virtual bool PerformAction (csStringID, iCelParameterBlock* params);

  void SendTriggerMessage (iCelEntity* entity, const char* msgid);

  void SetupTriggerSphere (const csVector3& center, float radius) { }
  void SetupTriggerBox (const csBox3& box) { }
  void EnableMessagesToSelf (bool en) { send_to_self = en; }
  void EnableMessagesToOthers (bool en) { send_to_others = en; }
  void EnableTrigger (bool en) { enabled = en; }
  bool IsEnabled () const { return enabled; }
  const csArray<iCelEntity*>& GetEntitiesInTrigger () const
  {
    return entities_in_trigger;
  }

  struct PcTrigger : public iPcTrigger
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcTrigger);
    virtual void SetupTriggerSphere (const csVector3& center, float radius)
    {
      scfParent->SetupTriggerSphere (center, radius);
    }
    virtual void SetupTriggerBox (const csBox3& box)
    {
      scfParent->SetupTriggerBox (box);
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
    virtual const csArray<iCelEntity*>& GetEntitiesInTrigger () const
    {
      return scfParent->GetEntitiesInTrigger ();
    }
  } scfiPcTrigger;
};

#endif // __CEL_PF_TRIGGERIMP__

