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

#ifndef __CEL_TOOLS_QUESTS_TRIG_MESHSELECT__
#define __CEL_TOOLS_QUESTS_TRIG_MESHSELECT__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"
#include "propclass/meshsel.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever a specific object is
 * put in the meshselect.
 * This trigger type listens to the name 'cel.questtrigger.meshselect'.
 */
CEL_DECLARE_TRIGGERTYPE(MeshSelect,"cel.questtrigger.meshselect")

/**
 * The 'meshselect' trigger factory.
 */
class celMeshSelectTriggerFactory :
	public iQuestTriggerFactory,
	public iMeshSelectQuestTriggerFactory
{
private:
  celMeshSelectTriggerType* type;
  char* entity_par;
  char* tag_par;

public:
  celMeshSelectTriggerFactory (celMeshSelectTriggerType* type);
  virtual ~celMeshSelectTriggerFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestTrigger> CreateTrigger (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iMeshSelectQuestTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
};

/**
 * The 'meshselect' trigger.
 */
class celMeshSelectTrigger :
	public iQuestTrigger,
	public iPcMeshSelectListener
{
private:
  celMeshSelectTriggerType* type;
  csRef<iQuestTriggerCallback> callback;
  csWeakRef<iPcMeshSelect> meshselect;
  char* entity;
  char* tag;

  void FindMeshSelect ();

public:
  celMeshSelectTrigger (celMeshSelectTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par);
  virtual ~celMeshSelectTrigger ();

  SCF_DECLARE_IBASE;

  virtual void RegisterCallback (iQuestTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  //----------------------- iPcMeshSelectListener --------------------------
  virtual void MouseDown (iPcMeshSelect* meshsel,
  	int x, int y, int button, iCelEntity* entity);
  virtual void MouseUp (iPcMeshSelect* meshsel,
  	int x, int y, int button, iCelEntity* entity);
  virtual void MouseMove (iPcMeshSelect* meshsel,
  	int x, int y, int button, iCelEntity* entity);
};

#endif // __CEL_TOOLS_QUESTS_TRIG_MESHSELECT__

