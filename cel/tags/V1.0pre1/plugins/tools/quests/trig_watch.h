/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_QUESTS_TRIG_WATCH__
#define __CEL_TOOLS_QUESTS_TRIG_WATCH__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"
#include "propclass/mesh.h"

#include "iengine/engine.h"

struct iObjectRegistry;
struct iCollideSystem;
struct iEvent;

/**
 * A standard trigger type that triggers whenever some
 * entity becomes visible.
 * This trigger type listens to the name 'cel.questtrigger.watch'.
 */
CEL_DECLARE_TRIGGERTYPE(Watch,"cel.questtrigger.watch")

/**
 * The 'watch' trigger factory.
 */
class celWatchTriggerFactory : public scfImplementation2<
	celWatchTriggerFactory, iQuestTriggerFactory,
	iWatchQuestTriggerFactory>
{
private:
  celWatchTriggerType* type;
  csString entity_par;
  csString tag_par;
  csString target_entity_par;
  csString target_tag_par;
  csString time_par;
  csString radius_par;
  csString offsetx_par;
  csString offsety_par;
  csString offsetz_par;

public:
  celWatchTriggerFactory (celWatchTriggerType* type);
  virtual ~celWatchTriggerFactory ();

  virtual csPtr<iQuestTrigger> CreateTrigger (iQuest*,
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iWatchQuestTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetTargetEntityParameter (const char* entity,
      const char* tag = 0);
  virtual void SetChecktimeParameter (const char* time);
  virtual void SetRadiusParameter (const char* radius);
  virtual void SetOffsetParameter (const char* offsetx,
      const char* offsety, const char* offsetz);
};

/**
 * The 'trigger' trigger.
 */
class celWatchTrigger : public scfImplementation2<
	celWatchTrigger, iQuestTrigger, iCelTimerListener>
{
private:
  celWatchTriggerType* type;
  csRef<iQuestTriggerCallback> callback;
  csString entity;
  csString tag;
  csString target_entity;
  csString target_tag;
  csTicks time;
  float radius, sqradius;
  csRef<iCelPlLayer> pl;
  csRef<iCollideSystem> cdsys;
  csVector3 offset;

  csWeakRef<iPcMesh> source_mesh;
  csWeakRef<iPcMesh> target_mesh;

  bool FindEntities ();

public:
  celWatchTrigger (celWatchTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* target_entity_par, const char* target_tag_par,
	const char* time_par, const char* radius_par,
	const char* offsetx_par, const char* offsety_par,
	const char* offsetz_par);
  virtual ~celWatchTrigger ();

  virtual void TickEveryFrame () { }
  virtual void TickOnce ();

  virtual void RegisterCallback (iQuestTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);
};

#endif // __CEL_TOOLS_QUESTS_TRIG_WATCH__

