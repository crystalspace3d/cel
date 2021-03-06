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

#ifndef __CEL_TOOLS_TRIG_MESHSELECT__
#define __CEL_TOOLS_TRIG_MESHSELECT__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/triggers.h"
#include "propclass/meshsel.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever a specific object is
 * put in the meshselect.
 * This trigger type listens to the name 'cel.questtrigger.meshselect'.
 */
CEL_DECLARE_TRIGGERTYPE(MeshSelect,"cel.triggers.meshselect")

/**
 * The 'meshselect' trigger factory.
 */
class celMeshSelectTriggerFactory : public scfImplementation2<
	celMeshSelectTriggerFactory, iTriggerFactory,
	iMeshSelectTriggerFactory>
{
private:
  csRef<celMeshSelectTriggerType> type;
  csString entity_par;
  csString tag_par;

public:
  celMeshSelectTriggerFactory (celMeshSelectTriggerType* type);
  virtual ~celMeshSelectTriggerFactory ();

  virtual csPtr<iTrigger> CreateTrigger (iQuest* q, iCelParameterBlock* params);
  virtual iTriggerType* GetTriggerType () const { return type; }
  virtual bool Load (iDocumentNode* node);
  virtual bool Save (iDocumentNode* node);

  //----------------- iMeshSelectTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual const char* GetEntity () const { return entity_par; }
  virtual const char* GetTag () const { return tag_par; }
};

/**
 * The 'meshselect' trigger.
 */
class celMeshSelectTrigger : public scfImplementation2<
	celMeshSelectTrigger, iTrigger,
	iPcMeshSelectListener>
{
private:
  csRef<celMeshSelectTriggerType> type;
  csRef<iTriggerCallback> callback;
  csWeakRef<iPcMeshSelect> meshselect;
  csString entity;
  uint entityID;
  csString tag;
  csRef<celOneParameterBlock> params_entity;

  void FindMeshSelect ();

public:
  celMeshSelectTrigger (celMeshSelectTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par);
  virtual ~celMeshSelectTrigger ();

  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();

  virtual void Activate () { ActivateTrigger (); }
  virtual void Deactivate () { DeactivateTrigger (); }

  //----------------------- iPcMeshSelectListener --------------------------
  virtual void MouseDown (iPcMeshSelect* meshsel,
  	int x, int y, int button, iCelEntity* entity);
  virtual void MouseUp (iPcMeshSelect* meshsel,
  	int x, int y, int button, iCelEntity* entity);
  virtual void MouseMove (iPcMeshSelect* meshsel,
  	int x, int y, int button, iCelEntity* entity);
};

#endif // __CEL_TOOLS_TRIG_MESHSELECT__

