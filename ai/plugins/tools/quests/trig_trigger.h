/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_QUESTS_TRIG_TRIGGER__
#define __CEL_TOOLS_QUESTS_TRIG_TRIGGER__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

#include "iengine/engine.h"
#include "propclass/trigger.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever the mesh
 * enters a specific sector.
 * This trigger type listens to the name 'cel.questtrigger.trigger'.
 */
CEL_DECLARE_TRIGGERTYPE(Trigger,"cel.questtrigger.trigger")

/**
 * The 'trigger' trigger factory.
 */
class celTriggerTriggerFactory : public scfImplementation2<
	celTriggerTriggerFactory, iQuestTriggerFactory,
	iTriggerQuestTriggerFactory>
{
private:
  celTriggerTriggerType* type;
  char* entity_par;
  char* tag_par;
  bool do_leave;

public:
  celTriggerTriggerFactory (celTriggerTriggerType* type);
  virtual ~celTriggerTriggerFactory ();

  virtual csPtr<iQuestTrigger> CreateTrigger (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iTriggerQuestTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void EnableLeave () { do_leave = true; }
};

/**
 * The 'trigger' trigger.
 */
class celTriggerTrigger : public scfImplementation2<
	celTriggerTrigger, iQuestTrigger, iPcTriggerListener>
{
private:
  celTriggerTriggerType* type;
  csRef<iQuestTriggerCallback> callback;
  char* entity;
  char* tag;
  csWeakRef<iPcTrigger> pctrigger;
  bool do_leave;

  void FindEntities ();

public:
  celTriggerTrigger (celTriggerTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	bool do_leave);
  virtual ~celTriggerTrigger ();

  virtual void RegisterCallback (iQuestTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  //------------------------- iPcTriggerListener ----------------------------
  virtual void EntityEnters (iPcTrigger* trigger, iCelEntity* entity);
  virtual void EntityLeaves (iPcTrigger* trigger, iCelEntity* entity);
  virtual void EnterTrigger (iPcTrigger* trigger, iCelEntity* entity);
  virtual void LeaveTrigger (iPcTrigger* trigger, iCelEntity* entity);
};

#endif // __CEL_TOOLS_QUESTS_TRIG_TRIGGER__

