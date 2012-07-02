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

#ifndef __CEL_TOOLS_TRIG_SEQUENCEFINISH__
#define __CEL_TOOLS_TRIG_SEQUENCEFINISH__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/triggers.h"
#include "tools/sequences.h"

#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/sector.h"

// @@@
// Remove Quest Dependency?
#include "tools/questmanager.h"


struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever the camera
 * enters a specific sector.
 * This trigger type listens to the name 'cel.triggers.sequencefinish'.
 */
CEL_DECLARE_TRIGGERTYPE(SequenceFinish,"cel.triggers.sequencefinish")

/**
 * The 'sequencefinish' trigger factory.
 */
class celSequenceFinishTriggerFactory : public scfImplementation2<
	celSequenceFinishTriggerFactory, iTriggerFactory,
	iSequenceFinishTriggerFactory>
{
private:
  csRef<celSequenceFinishTriggerType> type;
  csString entity_par;
  csString tag_par;
  csString sequence_par;
  csWeakRef<iCelSequence> seq;

public:
  celSequenceFinishTriggerFactory (celSequenceFinishTriggerType* type);
  virtual ~celSequenceFinishTriggerFactory ();

  virtual csPtr<iTrigger> CreateTrigger (const celParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iSequenceFinishTriggerFactory ---------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetSequenceParameter (const char* sequence);
  virtual void SetSequence (iCelSequence* sequence);
};

/**
 * The 'sequencefinish' trigger.
 */
class celSequenceFinishTrigger : public scfImplementation2<
	celSequenceFinishTrigger, iTrigger,
	iCelSequenceCallback>
{
private:
  csRef<celSequenceFinishTriggerType> type;
  csRef<iTriggerCallback> callback;
  csString entity;
  csString tag;
  csString sequence_name;
  csWeakRef<iCelSequence> seq;

  void FindSequence ();

  bool finished;

public:
  celSequenceFinishTrigger (celSequenceFinishTriggerType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* sequence_par,
	iCelSequence* sequence = 0);
  virtual ~celSequenceFinishTrigger ();

  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  //----------------------- iSequenceCallback --------------------------
  virtual void SequenceFinished (iCelSequence* sequence);
};

#endif // __CEL_TOOLS_TRIG_SEQUENCEFINISH__

