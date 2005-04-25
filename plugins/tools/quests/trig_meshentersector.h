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

#ifndef __CEL_TOOLS_QUESTS_TRIG_MESHENTERSECTOR__
#define __CEL_TOOLS_QUESTS_TRIG_MESHENTERSECTOR__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever the mesh
 * enters a specific sector.
 * This trigger type listens to the name 'cel.questtrigger.meshentersector'.
 */
CEL_DECLARE_TRIGGERTYPE(MeshEnterSector,"cel.questtrigger.meshentersector")

/**
 * The 'meshentersector' trigger factory.
 */
class celMeshEnterSectorTriggerFactory :
	public iQuestTriggerFactory,
	public iEnterSectorQuestTriggerFactory
{
private:
  celMeshEnterSectorTriggerType* type;
  char* entity_name_par;
  char* sector_name_par;

public:
  celMeshEnterSectorTriggerFactory (celMeshEnterSectorTriggerType* type);
  virtual ~celMeshEnterSectorTriggerFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestTrigger> CreateTrigger (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iEnterSectorQuestTriggerFactory ----------------------
  virtual void SetEntityNameParameter (const char* entity_name);
  virtual void SetSectorNameParameter (const char* sector_name);
};

/**
 * The 'meshentersector' trigger.
 */
class celMeshEnterSectorTrigger :
	public iQuestTrigger,
	public iMovableListener
{
private:
  celMeshEnterSectorTriggerType* type;
  csRef<iQuestTriggerCallback> callback;
  csWeakRef<iSector> sector;
  csWeakRef<iMeshWrapper> mesh;
  char* entity_name;
  char* sector_name;

  void FindSectorAndMesh ();

public:
  celMeshEnterSectorTrigger (celMeshEnterSectorTriggerType* type,
  	const celQuestParams& params,
	const char* entity_name_par, const char* sector_name_par);
  virtual ~celMeshEnterSectorTrigger ();

  SCF_DECLARE_IBASE;

  virtual void RegisterCallback (iQuestTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual void DeactivateTrigger ();

  //------------------------- iMovableListener ----------------------------
  virtual void MovableChanged (iMovable* movable);
  virtual void MovableDestroyed (iMovable* movable);
};

#endif // __CEL_TOOLS_QUESTS_TRIG_MESHENTERSECTOR__

