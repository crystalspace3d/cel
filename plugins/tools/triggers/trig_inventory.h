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

#ifndef __CEL_TOOLS_TRIG_INVENTORY__
#define __CEL_TOOLS_TRIG_INVENTORY__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/triggers.h"
#include "propclass/inv.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard trigger type that triggers whenever a specific object is
 * put in the inventory.
 * This trigger type listens to the name 'cel.triggers.inventory'.
 */
CEL_DECLARE_TRIGGERTYPE(Inventory,"cel.triggers.inventory")

/**
 * The 'inventory' trigger factory.
 */
class celInventoryTriggerFactory : public scfImplementation2<
	celInventoryTriggerFactory, iTriggerFactory,
	iInventoryTriggerFactory>
{
private:
  csRef<celInventoryTriggerType> type;
  csString entity_par;
  csString tag_par;
  csString child_entity_par;
  csString child_template_par;

public:
  celInventoryTriggerFactory (celInventoryTriggerType* type);
  virtual ~celInventoryTriggerFactory ();

  virtual csPtr<iTrigger> CreateTrigger (iQuest* q, iCelParameterBlock* params);
  virtual iTriggerType* GetTriggerType () const { return type; }
  virtual bool Load (iDocumentNode* node);
  virtual bool Save (iDocumentNode* node);

  //----------------- iInventoryTriggerFactory ----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual const char* GetEntity () const { return entity_par; }
  virtual const char* GetTag () const { return tag_par; }
  virtual void SetChildEntityParameter (const char* entity);
  virtual const char* GetChildEntity () const { return child_entity_par; }
  virtual void SetChildTemplateParameter (const char* child_template);
  virtual const char* GetChildTemplate () const { return child_template_par; }
};

/**
 * The 'inventory' trigger.
 */
class celInventoryTrigger : public scfImplementation2<
	celInventoryTrigger, iTrigger,
	iPcInventoryListener>
{
private:
  csRef<celInventoryTriggerType> type;
  csRef<iTriggerCallback> callback;
  csWeakRef<iPcInventory> inventory;
  csString entity;
  uint entityID;
  csString tag;
  csString child_entity;
  csString child_template;
  csRef<celOneParameterBlock> params_child;

  void FindInventory ();
  void FireTrigger (iCelEntity* child);
  void FireTrigger (iCelEntityTemplate* child);

public:
  celInventoryTrigger (celInventoryTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	const char* child_entity_par, const char* child_template_par);
  virtual ~celInventoryTrigger ();

  virtual void RegisterCallback (iTriggerCallback* callback);
  virtual void ClearCallback ();
  virtual void ActivateTrigger ();
  virtual bool Check ();
  virtual void DeactivateTrigger ();

  virtual void Activate () { ActivateTrigger (); }
  virtual void Deactivate () { DeactivateTrigger (); }

  //----------------------- iPcInventoryListener ----------------------------
  virtual void AddChild (iPcInventory* inventory, iCelEntity* entity);
  virtual void RemoveChild (iPcInventory* inventory, iCelEntity* entity);
  virtual void AddChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount);
  virtual void RemoveChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount);
};

#endif // __CEL_TOOLS_TRIG_INVENTORY__

