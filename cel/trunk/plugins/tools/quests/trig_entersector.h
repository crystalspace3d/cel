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

#ifndef __CEL_TOOLS_QUESTS_TRIG_ENTERSECTOR__
#define __CEL_TOOLS_QUESTS_TRIG_ENTERSECTOR__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

#include "iengine/camera.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever the camera
 * enters a specific sector. This trigger type listens to the
 * name 'cel.questtrigger.entersector'.
 */
class celEnterSectorTriggerType : public iQuestTriggerType
{
private:
  iObjectRegistry* object_reg;

public:
  celEnterSectorTriggerType (iObjectRegistry* object_reg);
  virtual ~celEnterSectorTriggerType ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const
  {
    return "cel.questtrigger.entersector";
  }
  virtual csPtr<iQuestTriggerFactory> CreateTriggerFactory ();
};

/**
 * The 'entersector' trigger factory.
 */
class celEnterSectorTriggerFactory :
	public iQuestTriggerFactory,
	public iEnterSectorQuestTriggerFactory
{
private:
  char* entity_name;
  char* sector_name;

public:
  celEnterSectorTriggerFactory ();
  virtual ~celEnterSectorTriggerFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestTrigger> CreateTrigger (
      const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iEnterSectorQuestTriggerFactory ----------------------
  virtual void SetEntityName (const char* entity_name);
  virtual void SetSectorName (const char* sector_name);
};

/**
 * The 'entersector' trigger.
 */
class celEnterSectorTrigger :
	public iQuestTrigger,
	public iCameraSectorListener
{
private:
  csRef<iQuestTriggerCallback> callback;

public:
  celEnterSectorTrigger ();
  virtual ~celEnterSectorTrigger ();

  SCF_DECLARE_IBASE;

  virtual void RegisterCallback (iQuestTriggerCallback* callback);
  virtual void ClearCallback ();

  //----------------------- iCameraSectorListener --------------------------
  virtual void NewSector (iCamera* camera, iSector* sector);
};

#endif // __CEL_TOOLS_QUESTS_TRIG_ENTERSECTOR__

