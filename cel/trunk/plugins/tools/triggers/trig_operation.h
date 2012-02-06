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

#ifndef __CEL_TOOLS_TRIG_OPERATION__
#define __CEL_TOOLS_TRIG_OPERATION__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/triggers.h"

#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/sector.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers on logical operations of child triggers.
 * This trigger type listens to the name 'cel.triggers.operation'.
 */
CEL_DECLARE_TRIGGERTYPE(Operation,"cel.triggers.operation")

/**
 * The 'operation' trigger factory.
 */
class celOperationTriggerFactory : public scfImplementation2<
	celOperationTriggerFactory, iTriggerFactory,
	iOperationTriggerFactory>
{
private:
  csRef<celOperationTriggerType> type;
  csString operation_par;
  csRefArray<iTriggerFactory> triggers;
public:
  celOperationTriggerFactory (celOperationTriggerType* type);
  virtual ~celOperationTriggerFactory ();

  //----------------- iTriggerFactory ----------------------
  virtual csPtr<iTrigger> CreateTrigger (iQuest* q, iCelParameterBlock* params);
  virtual iTriggerType* GetTriggerType () const { return type; }
  virtual bool Load (iDocumentNode* node);
  virtual bool Save (iDocumentNode* node);

  //----------------- iOperationTriggerFactory ----------------------
  virtual void SetOperationParameter (const char* operation);
  virtual csRefArray<iTriggerFactory> &GetTriggerFactories () 
  { return triggers; }
};

/**
 * The 'operation' trigger.
 */
class celOperationTrigger : public scfImplementation2<
	celOperationTrigger, iTrigger, iTriggerCallback>
{
protected:
  csRef<celOperationTriggerType> type;
  csRef<iTriggerCallback> callback;
  csRefArray<iTrigger> triggers;
  bool checking;

  csString operation;

public:
  celOperationTrigger (celOperationTriggerType* type,
        iQuest* q,
  	iCelParameterBlock* params,
	const char* operation_par,
        csRefArray<iTriggerFactory> &trigger_factories);

  virtual ~celOperationTrigger ();

  //----------------- iTrigger ----------------------
  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check () = 0;
  virtual void DeactivateTrigger ();

  virtual void Activate () { ActivateTrigger (); }
  virtual void Deactivate () { DeactivateTrigger (); }

  /* iTriggerCallback */
  virtual void TriggerFired (iTrigger* trigger, iCelParameterBlock* params) = 0;
};

class celAndOperationTrigger : public celOperationTrigger
{
public:
  celAndOperationTrigger (celOperationTriggerType* type,
        iQuest* q,
  	iCelParameterBlock* params,
	const char* operation_par,
        csRefArray<iTriggerFactory> &trigger_factories)
	: celOperationTrigger(type,q,params,operation_par,trigger_factories) {};
  virtual void TriggerFired (iTrigger* trigger, iCelParameterBlock* params);
  virtual bool Check ();
};
class celOrOperationTrigger : public celOperationTrigger
{
public:
  celOrOperationTrigger (celOperationTriggerType* type,
        iQuest* q,
  	iCelParameterBlock* params,
	const char* operation_par,
        csRefArray<iTriggerFactory> &trigger_factories)
	: celOperationTrigger(type,q,params,operation_par,trigger_factories) {};
  virtual void TriggerFired (iTrigger* trigger, iCelParameterBlock* params);
  virtual bool Check ();
};
class celXorOperationTrigger : public celOperationTrigger
{
public:
  celXorOperationTrigger (celOperationTriggerType* type,
        iQuest* q,
  	iCelParameterBlock* params,
	const char* operation_par,
        csRefArray<iTriggerFactory> &trigger_factories)
	: celOperationTrigger(type,q,params,operation_par,trigger_factories) {};
  virtual void TriggerFired (iTrigger* trigger, iCelParameterBlock* params);
  virtual bool Check ();
};

#endif // __CEL_TOOLS_TRIG_OPERATION__

