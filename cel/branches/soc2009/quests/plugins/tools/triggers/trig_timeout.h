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

#ifndef __CEL_TOOLS_TRIG_TIMEOUT__
#define __CEL_TOOLS_TRIG_TIMEOUT__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "csutil/timer.h"
#include "iutil/timer.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/triggers.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers after a specific time.
 * This trigger type listens to the name 'cel.triggers.timeout'.
 */
CEL_DECLARE_TRIGGERTYPE_NEW(Timeout,"cel.triggers.timeout")

/**
 * The 'timeout' trigger factory.
 */
class celTimeoutTriggerFactory : public scfImplementation2<
	celTimeoutTriggerFactory, iTriggerFactory,
	iTimeoutTriggerFactory>
{
private:
  celTimeoutTriggerType* type;
  csString timeout_par;

public:
  celTimeoutTriggerFactory (celTimeoutTriggerType* type);
  virtual ~celTimeoutTriggerFactory ();

  virtual csPtr<iTrigger> CreateTrigger (const celParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iTimeoutTriggerFactory ----------------------
  virtual void SetTimeoutParameter (const char* timeout_par);
};

/**
 * The 'timeout' trigger.
 */
class celTimeoutTrigger : public scfImplementation2<
	celTimeoutTrigger, iTrigger,
	iTimerEvent>
{
private:
  celTimeoutTriggerType* type;
  csRef<iParameterManager> pm;
  csRef<iTriggerCallback> callback;
  csTicks timeout;
  csEventTimer* timer;
  bool fired;

public:
  celTimeoutTrigger (celTimeoutTriggerType* type,
  	const celParams& params, const char* timeout_par);
  virtual ~celTimeoutTrigger ();

  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  //------------------------- iTimerEvent -------------------------------
  virtual bool Perform (iTimerEvent* ev);
};

#endif // __CEL_TOOLS_TRIG_TIMEOUT__

