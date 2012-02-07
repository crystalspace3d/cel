/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin
  
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

#ifndef __CEL_TOOLS_TRIG_TRIGGER__
#define __CEL_TOOLS_TRIG_TRIGGER__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/triggers.h"

#include "iengine/engine.h"
#include "propclass/trigger.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers when a pcTrigger fires
 * This trigger type listens to the name 'cel.triggers.trigger'.
 */
CEL_DECLARE_TRIGGERTYPE(Trigger,"cel.triggers.trigger")

/**
 * The 'trigger' trigger factory.
 */
class celTriggerTriggerFactory : public scfImplementation2<
	celTriggerTriggerFactory, iTriggerFactory,
	iTriggerTriggerFactory>
{
private:
  csRef<celTriggerTriggerType> type;
  csString entity_par;
  csString tag_par;
  bool do_leave;

public:
  celTriggerTriggerFactory (celTriggerTriggerType* type);
  virtual ~celTriggerTriggerFactory ();

  virtual csPtr<iTrigger> CreateTrigger (iQuest* q, iCelParameterBlock* params);
  virtual iTriggerType* GetTriggerType () const { return type; }
  virtual bool Save (iDocumentNode* node);
  virtual bool Load (iDocumentNode* node);

  //----------------- iTriggerTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual const char* GetEntity () const { return entity_par; }
  virtual const char* GetTag () const { return tag_par; }
  virtual void EnableLeave () { do_leave = true; }
  virtual bool IsLeaveEnabled () const { return do_leave; }
};

/**
 * The 'trigger' trigger.
 */
class celTriggerTrigger : public scfImplementation2<
	celTriggerTrigger, iTrigger, iPcTriggerListener>
{
private:
  csRef<celTriggerTriggerType> type;
  csRef<iTriggerCallback> callback;
  csString entity;
  uint entityID;
  csString tag;
  csWeakRef<iPcTrigger> pctrigger;
  bool do_leave;
  csRef<celOneParameterBlock> params_entity;

  void FindEntities ();
  void FireTrigger (const char* name);

public:
  celTriggerTrigger (celTriggerTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	bool do_leave);
  virtual ~celTriggerTrigger ();

  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();

  virtual void Activate () { ActivateTrigger (); }
  virtual void Deactivate () { DeactivateTrigger (); }

  //------------------------- iPcTriggerListener ----------------------------
  virtual void EntityEnters (iPcTrigger* trigger, iCelEntity* entity);
  virtual void EntityLeaves (iPcTrigger* trigger, iCelEntity* entity);
  virtual void EnterTrigger (iPcTrigger* trigger, iCelEntity* entity);
  virtual void LeaveTrigger (iPcTrigger* trigger, iCelEntity* entity);
};

#endif // __CEL_TOOLS_TRIG_TRIGGER__

