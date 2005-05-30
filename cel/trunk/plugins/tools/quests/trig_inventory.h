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

#ifndef __CEL_TOOLS_QUESTS_TRIG_INVENTORY__
#define __CEL_TOOLS_QUESTS_TRIG_INVENTORY__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"
#include "propclass/inv.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever a specific object is
 * put in the inventory.
 * This trigger type listens to the name 'cel.questtrigger.inventory'.
 */
CEL_DECLARE_TRIGGERTYPE(Inventory,"cel.questtrigger.inventory")

/**
 * The 'inventory' trigger factory.
 */
class celInventoryTriggerFactory :
	public iQuestTriggerFactory,
	public iInventoryQuestTriggerFactory
{
private:
  celInventoryTriggerType* type;
  char* entity_par;
  char* tag_par;
  char* child_entity_par;

public:
  celInventoryTriggerFactory (celInventoryTriggerType* type);
  virtual ~celInventoryTriggerFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestTrigger> CreateTrigger (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iInventoryQuestTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetChildEntityParameter (const char* entity);
};

/**
 * The 'inventory' trigger.
 */
class celInventoryTrigger :
	public iQuestTrigger,
	public iPcInventoryListener
{
private:
  celInventoryTriggerType* type;
  csRef<iQuestTriggerCallback> callback;
  csWeakRef<iPcInventory> inventory;
  char* entity;
  char* tag;
  char* child_entity;

  void FindInventory ();

public:
  celInventoryTrigger (celInventoryTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* child_entity_par);
  virtual ~celInventoryTrigger ();

  SCF_DECLARE_IBASE;

  virtual void RegisterCallback (iQuestTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual void DeactivateTrigger ();
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf);
  virtual void SaveTriggerState (iCelDataBuffer* databuf);

  //----------------------- iPcInventoryListener ----------------------------
  virtual void AddChild (iPcInventory* inventory, iCelEntity* entity);
  virtual void RemoveChild (iPcInventory* inventory, iCelEntity* entity);
};

#endif // __CEL_TOOLS_QUESTS_TRIG_INVENTORY__

