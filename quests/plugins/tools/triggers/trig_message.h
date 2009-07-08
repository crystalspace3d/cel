/*
    Crystal Space Entity Layer
    Copyright (C) 2008 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_TRIG_MESSAGE__
#define __CEL_TOOLS_TRIG_MESSAGE__

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
#include "physicallayer/messaging.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever the mesh
 * receives a message.
 * This trigger type listens to the name 'cel.questtrigger.message'.
 */
CEL_DECLARE_TRIGGERTYPE_NEW(Message,"cel.triggers.message")

/**
 * The 'message' trigger factory.
 */
class celMessageTriggerFactory : public scfImplementation2<
	celMessageTriggerFactory, iTriggerFactory,
	iMessageTriggerFactory>
{
private:
  celMessageTriggerType* type;
  csString entity_par;
  csString mask_par;

public:
  celMessageTriggerFactory (celMessageTriggerType* type);
  virtual ~celMessageTriggerFactory ();

  virtual csPtr<iTrigger> CreateTrigger (const celParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iMessageTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity);
  virtual void SetMaskParameter (const char* mask);
};

/**
 * The 'message' trigger.
 */
class celMessageTrigger : public scfImplementation2<
	celMessageTrigger, iTrigger, iMessageReceiver>
{
private:
  celMessageTriggerType* type;
  csRef<iTriggerCallback> callback;
  csString entity;
  csString mask;
  csWeakRef<iCelEntity> ent;

  void FindEntity ();

public:
  celMessageTrigger (celMessageTriggerType* type,
  	const celParams& params,
	const char* entity_par, const char* mask_par);
  virtual ~celMessageTrigger ();

  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  //------------------------- iMessageReceiver ------------------------------
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_TRIG_MESSAGE__

