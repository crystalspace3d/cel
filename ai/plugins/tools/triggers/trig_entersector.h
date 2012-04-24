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

#ifndef __CEL_TOOLS_TRIG_ENTERSECTOR__
#define __CEL_TOOLS_TRIG_ENTERSECTOR__

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
 * A standard trigger type that triggers whenever the camera
 * enters a specific sector.
 * This trigger type listens to the name 'cel.triggers.entersector'.
 */
CEL_DECLARE_TRIGGERTYPE(EnterSector,"cel.triggers.entersector")

/**
 * The 'entersector' trigger factory.
 */
class celEnterSectorTriggerFactory : public scfImplementation2<
	celEnterSectorTriggerFactory, iTriggerFactory,
	iEnterSectorTriggerFactory>
{
private:
  csRef<celEnterSectorTriggerType> type;
  csString entity_par;
  csString tag_par;
  csString sector_par;

public:
  celEnterSectorTriggerFactory (celEnterSectorTriggerType* type);
  virtual ~celEnterSectorTriggerFactory ();

  virtual iTriggerType* GetTriggerType () const { return type; }
  virtual csPtr<iTrigger> CreateTrigger (iQuest* q, iCelParameterBlock* params);
  virtual bool Load (iDocumentNode* node);
  virtual bool Save (iDocumentNode* node);

  //----------------- iEnterSectorTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual const char* GetEntity () const { return entity_par; }
  virtual const char* GetTag () const { return tag_par; }
  virtual void SetSectorParameter (const char* sector);
  virtual const char* GetSector () const { return sector_par; }
};

/**
 * The 'entersector' trigger.
 */
class celEnterSectorTrigger : public scfImplementation2<
	celEnterSectorTrigger, iTrigger,
	iCameraListener>
{
private:
  csRef<celEnterSectorTriggerType> type;
  csRef<iTriggerCallback> callback;
  csWeakRef<iSector> sect;
  csWeakRef<iCamera> camera;
  csString entity;
  uint entityID;
  csString tag;
  csString sector;

  void FindSectorAndCamera ();

public:
  celEnterSectorTrigger (celEnterSectorTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag,
	const char* sector_par);
  virtual ~celEnterSectorTrigger ();

  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();

  virtual void Activate () { ActivateTrigger (); }
  virtual void Deactivate () { DeactivateTrigger (); }

  //----------------------- iCameraSectorListener --------------------------
  virtual void NewSector (iCamera* camera, iSector* sector);
  virtual void CameraMoved (iCamera* camera) {}
};

#endif // __CEL_TOOLS_TRIG_ENTERSECTOR__

