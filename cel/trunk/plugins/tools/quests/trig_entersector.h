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
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/sector.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever the camera
 * enters a specific sector.
 * This trigger type listens to the name 'cel.questtrigger.entersector'.
 */
CEL_DECLARE_TRIGGERTYPE(EnterSector,"cel.questtrigger.entersector")

/**
 * The 'entersector' trigger factory.
 */
class celEnterSectorTriggerFactory :
	public iQuestTriggerFactory,
	public iEnterSectorQuestTriggerFactory
{
private:
  celEnterSectorTriggerType* type;
  char* entity_par;
  char* tag_par;
  char* sector_par;

public:
  celEnterSectorTriggerFactory (celEnterSectorTriggerType* type);
  virtual ~celEnterSectorTriggerFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestTrigger> CreateTrigger (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iEnterSectorQuestTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetSectorParameter (const char* sector);
};

/**
 * The 'entersector' trigger.
 */
class celEnterSectorTrigger :
	public iQuestTrigger,
	public iCameraSectorListener
{
private:
  celEnterSectorTriggerType* type;
  csRef<iQuestTriggerCallback> callback;
  csWeakRef<iSector> sect;
  csWeakRef<iCamera> camera;
  char* entity;
  char* tag;
  char* sector;

  void FindSectorAndCamera ();

public:
  celEnterSectorTrigger (celEnterSectorTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag,
	const char* sector_par);
  virtual ~celEnterSectorTrigger ();

  SCF_DECLARE_IBASE;

  virtual void RegisterCallback (iQuestTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  //----------------------- iCameraSectorListener --------------------------
  virtual void NewSector (iCamera* camera, iSector* sector);
};

#endif // __CEL_TOOLS_QUESTS_TRIG_ENTERSECTOR__

