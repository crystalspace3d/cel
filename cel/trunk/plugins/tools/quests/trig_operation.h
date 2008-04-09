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

#ifndef __CEL_TOOLS_QUESTS_TRIG_OPERATION__
#define __CEL_TOOLS_QUESTS_TRIG_OPERATION__

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
CEL_DECLARE_TRIGGERTYPE(Operation,"cel.questtrigger.operation")

/**
 * The 'entersector' trigger factory.
 */
class celOperationTriggerFactory : public scfImplementation2<
	celOperationTriggerFactory, iQuestTriggerFactory,
	iOperationQuestTriggerFactory>
{
private:
  celOperationTriggerType* type;
  char* operation_par;
  char* entity_par;
  char* tag_par;
  char* sector_par;
  csRefArray<iQuestTriggerFactory> triggers;
public:
  celOperationTriggerFactory (celOperationTriggerType* type);
  virtual ~celOperationTriggerFactory ();

  //----------------- iQuestTriggerFactory ----------------------
  virtual csPtr<iQuestTrigger> CreateTrigger (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iOperationQuestTriggerFactory ----------------------
  virtual void SetOperationParameter (const char* operation);
  virtual csRefArray<iQuestTriggerFactory> &GetTriggerFactories () 
  { return triggers; }
};

/**
 * The 'entersector' trigger.
 */
class celOperationTrigger : public scfImplementation2<
	celOperationTrigger, iQuestTrigger, iQuestTriggerCallback>
{
protected:
  celOperationTriggerType* type;
  csRef<iQuestTriggerCallback> callback;
  csRefArray<iQuestTrigger> triggers;

  char* operation;

public:
  celOperationTrigger (celOperationTriggerType* type,
  	const celQuestParams& params,
	const char* operation_par,
        csRefArray<iQuestTriggerFactory> &trigger_factories);

  virtual ~celOperationTrigger ();

  //----------------- iQuestTrigger ----------------------
  virtual void RegisterCallback (iQuestTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check () = 0;
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  /* iQuestTriggerCallback */
  virtual void TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params) = 0;
};

class celAndOperationTrigger : public celOperationTrigger
{
public:
  celAndOperationTrigger (celOperationTriggerType* type,
  	const celQuestParams& params,
	const char* operation_par,
        csRefArray<iQuestTriggerFactory> &trigger_factories)
	: celOperationTrigger(type,params,operation_par,trigger_factories) {};
  virtual void TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params);
  virtual bool Check ();
};
class celOrOperationTrigger : public celOperationTrigger
{
public:
  celOrOperationTrigger (celOperationTriggerType* type,
  	const celQuestParams& params,
	const char* operation_par,
        csRefArray<iQuestTriggerFactory> &trigger_factories)
	: celOperationTrigger(type,params,operation_par,trigger_factories) {};
  virtual void TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params);
  virtual bool Check ();
};
class celXorOperationTrigger : public celOperationTrigger
{
public:
  celXorOperationTrigger (celOperationTriggerType* type,
  	const celQuestParams& params,
	const char* operation_par,
        csRefArray<iQuestTriggerFactory> &trigger_factories)
	: celOperationTrigger(type,params,operation_par,trigger_factories) {};
  virtual void TriggerFired (iQuestTrigger* trigger, iCelParameterBlock* params);
  virtual bool Check ();
};

#endif // __CEL_TOOLS_QUESTS_TRIG_OPERATION__

