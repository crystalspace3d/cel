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

#ifndef __CEL_TOOLS_TRIG_MESHENTERSECTOR__
#define __CEL_TOOLS_TRIG_MESHENTERSECTOR__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/triggers.h"

#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever the mesh
 * enters a specific sector.
 * This trigger type listens to the name 'cel.triggers.meshentersector'.
 */
CEL_DECLARE_TRIGGERTYPE(MeshEnterSector,"cel.triggers.meshentersector")

/**
 * The 'meshentersector' trigger factory.
 */
class celMeshEnterSectorTriggerFactory : public scfImplementation2<
	celMeshEnterSectorTriggerFactory, iTriggerFactory,
	iEnterSectorTriggerFactory>
{
private:
  csRef<celMeshEnterSectorTriggerType> type;
  csString entity_par;
  csString tag_par;
  csString sector_par;

public:
  celMeshEnterSectorTriggerFactory (celMeshEnterSectorTriggerType* type);
  virtual ~celMeshEnterSectorTriggerFactory ();

  virtual csPtr<iTrigger> CreateTrigger (const celParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iEnterSectorTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetSectorParameter (const char* sector);
};

/**
 * The 'meshentersector' trigger.
 */
class celMeshEnterSectorTrigger : public scfImplementation2<
	celMeshEnterSectorTrigger, iTrigger,
	iMovableListener>
{
private:
  csRef<celMeshEnterSectorTriggerType> type;
  csRef<iTriggerCallback> callback;
  csWeakRef<iSector> sect;
  csWeakRef<iMeshWrapper> mesh;
  csString entity;
  csString tag;
  csString sector;

  void FindSectorAndMesh ();

public:
  celMeshEnterSectorTrigger (celMeshEnterSectorTriggerType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* sector_par);
  virtual ~celMeshEnterSectorTrigger ();

  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  //------------------------- iMovableListener ----------------------------
  virtual void MovableChanged (iMovable* movable);
  virtual void MovableDestroyed (iMovable* movable);
};

#endif // __CEL_TOOLS_TRIG_MESHENTERSECTOR__

