/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_INV__
#define __CEL_PF_INV__

#include "cstypes.h"
#include "csutil/scf.h"
#include "behaviourlayer/behave.h"

struct iCelEntity;
struct iCelEntityTemplate;
struct iPcInventory;
struct iLootGenerator;

/**
 * Listen to inventory changes.
 */
struct iPcInventoryListener : public virtual iBase
{
  SCF_INTERFACE (iPcInventoryListener, 0, 1, 1);

  /**
   * A child is added to this inventory.
   */
  virtual void AddChild (iPcInventory* inventory, iCelEntity* entity) = 0;

  /**
   * A child is removed from this inventory.
   */
  virtual void RemoveChild (iPcInventory* inventory, iCelEntity* entity) = 0;

  /**
   * An entity template is added to this inventory.
   */
  virtual void AddChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount) = 0;

  /**
   * An entity template is removed from this inventory.
   */
  virtual void RemoveChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount) = 0;
};


/**
 * A slot in the inventory system. Applications can implement this
 * to make any kind of slot system (like a 2D based grid).
 */
struct iCelInventorySpaceSlot : public virtual iBase
{
  SCF_INTERFACE (iCelInventorySpaceSlot, 0, 0, 1);

  /**
   * Get an entity on this slot. If there are more entities on this
   * slot then you can use the abstract 'params' to specify the correct one.
   * The exact parameter specification depends on the implementation of
   * this slot. If you don't support stacking then you can ignore this
   * parameter.
   */
  virtual iCelEntity* GetEntity (iCelParameterBlock* params) = 0;

  /**
   * Add an entity in this slot. If you support stacking then you can use
   * 'params' to specify where to stack.
   * /return false if the entity can't be added for some reason.
   */
  virtual bool AddEntity (iCelEntity* entity, iCelParameterBlock* params) = 0;

  /**
   * Remove this entity from the slot.
   * /return false if the entity can't be removed for some reason.
   */
  virtual bool RemoveEntity (iCelEntity* entity) = 0;

  /// Return number of entities inside this slot.
  virtual int GetCount () = 0;

  /// Remove all entities in this slot.
  virtual void RemoveAll () = 0;

  /// Check if there are entities in this slot.
  virtual bool IsEmpty () = 0;
};

/**
 * This interface represents a space system for the inventory property class.
 * You can implement this to implement (for example) a 2D grid based system.
 * Use iPcInventory->SetSpace() to set a space system on the inventory.
 */
struct iCelInventorySpace : public virtual iBase
{
  SCF_INTERFACE (iCelInventorySpace, 0, 1, 1);
  
  /// Return how much space is available in total.
  virtual int GetSpaceTotal () const = 0;
  
  /// Return how much space is left.
  virtual int GetSpaceLeft () const = 0;
  
  // Return how much space is taken.
  virtual int GetSpaceTaken () const = 0;
  
  /**
   * Add an entity on a given slot. The generic 'params' will be used to 
   * indicate the slot. It is up to the implementation of this interface
   * to decide on how to use this.
   * If 'params' is not given then a default slot can be picked (or the
   * add can be rejected in case slot specifications are always required).
   * /return false if there is no room or if this is not allowed.
   */
  virtual bool AddEntity (iCelEntity* entity, iCelParameterBlock* params = 0) = 0;
  /**
   * Remove the entity from its slot.
   * /return false if it couldn't be removed.
   */
  virtual bool RemoveEntity (iCelEntity* entity) = 0;
  
  /**
   * Remove an entity (or entities) from the specified slot (through 'params').
   * If needed the params can be extended with information to specify the
   * right item to be removed (for stacked items).
   */
  virtual bool RemoveEntity (iCelParameterBlock* params) = 0;
  
  //. Get rid of all entities inside space.
  virtual void RemoveAll () = 0;
  
  /**
   * Try to get a certain slot. 'params' is used to specify the slot and
   * is implementation dependent.
   */
  virtual iCelInventorySpaceSlot* GetSlot (iCelParameterBlock* params) = 0;
  
  /**
   * Try to get a certain entity from a slot. 'params' is used to specify
   * the slot and is implementation dependent.
   */
  virtual iCelEntity* GetEntity (iCelParameterBlock* params) = 0;
};


/**
 * This is an inventory property class.
 *
 * This property class supports the following actions (add prefix 'cel.inventory.action.'
 * if you want to access this action through a message):
 * - AddTemplate: parameters 'name' (string), 'amount' (long), returns true/false.
 * - RemoveTemplate: parameters 'name' (string), 'amount' (long), returns true/false.
 * - SetLootGenerator: parameters 'name' (string, name of the generator)
 *
 * This property class can send out the following messages
 * (possibly to the containing entity as well as the child entity):
 * - 'cel.entity.add' (old 'pcinventory_addchild'): new child will be added (entity)
 * - 'cel.entity.add.template': new template will be added (entity as string, amount)
 * - 'cel.entity.add.this' (old 'pcinventory_added'): this entity is added as a new child (entity)
 * - 'cel.entity.remove' (old 'pcinventory_removechild'): new child will be removed (entity)
 * - 'cel.entity.remove.template': new template will be removed (entity as string, amount (the amount that is removed))
 * - 'cel.entity.remove.this' (old 'pcinventory_removed'): this entity is removed (entity)
 */
struct iPcInventory : public virtual iBase
{
  SCF_INTERFACE (iPcInventory, 1, 1, 0);

  /**
   * Add an inventory listener. Inventory listeners are called right before
   * the behaviour is called.
   */
  virtual void AddInventoryListener (iPcInventoryListener* listener) = 0;
  /**
   * Remove an inventory listener.
   */
  virtual void RemoveInventoryListener (iPcInventoryListener* listener) = 0;

  /**
   * Add an entity. Returns false if the entity could
   * not be added (capacity exceeded for example).
   * Note that it is safe to add entities that are already in the inventory.
   * Nothing will happen then and true will be returned.
   * If 'params' is given then this will be given to the space system.
   * \param params is a generic slot specification (used by iCelInventorySpace).
   */
  virtual bool AddEntity (iCelEntity* entity, iCelParameterBlock* params = 0) = 0;

  /**
   * Remove an entity. This can fail if removing an entity causes
   * an upstream inventory to fail its constraints.
   */
  virtual bool RemoveEntity (iCelEntity* entity) = 0;
  /**
   * Remove an entity on a slot using the space system. This can fail if
   * removing an entity causes an upstream inventory to fail its constraints.
   * \param params is a generic slot specification (used by iCelInventorySpace).
   * It can also contain additional information to specify which entity to
   * remove from some slot (if stacked items are supported).
   */
  virtual bool RemoveEntity (iCelParameterBlock* params) = 0;

  /**
   * Add an entity template. Returns false if the template could
   * not be added (capacity exceeded for example).
   * If the given template is already in the inventory then the amount
   * will be updated.
   * The space/slot system is currently not yet supported for entity templates.
   */
  virtual bool AddEntityTemplate (iCelEntityTemplate* tpl, int amount) = 0;

  /**
   * Remove an entity template. This can fail if removing a template causes
   * an upstream inventory to fail its constraints.
   * If there are more of a given template in the inventory then the
   * amount will be updated. If there are less then this will fail and
   * nothing will happen.
   */
  virtual bool RemoveEntityTemplate (iCelEntityTemplate* tpl, int amount) = 0;

  /**
   * Remove all entities and templates. This can fail if removing entities
   * causes upstream inventories to fail its constraints.
   * In that case entities are removed until the first failure.
   * To ensure correct removal for this inventory you should
   * first clear all constraints.
   */
  virtual bool RemoveAll () = 0;

  //--------------------------------------------------------------------

  /**
   * Get the number of entities in this inventory (not templates!).
   */
  virtual size_t GetEntityCount () const = 0;

  /**
   * Get some entity.
   */
  virtual iCelEntity* GetEntity (size_t idx) const = 0;

  /**
   * Test if some entity is in the inventory.
   */
  virtual bool In (iCelEntity* entity) const = 0;

  /**
   * Test if some entity or template is in the inventory by name.
   */
  virtual bool In (const char* name) const = 0;

  /**
   * Find the index of some entity in the inventory.
   * Return csArrayItemNotFound if not in inventory.
   */
  virtual size_t FindEntity (iCelEntity* entity) const = 0;

  /**
   * Find the index of some entity in the inventory by name.
   * Return csArrayItemNotFound if not in inventory.
   */
  virtual size_t FindEntity (const char* name) const = 0;

   /**
   * Find the index of some entity in the inventory by class.
   * The first entity with given class will be returned.
   * Return csArrayItemNotFound if not in inventory.
   */
  virtual size_t FindEntity (csStringID classid) const = 0;

  //--------------------------------------------------------------------

  /**
   * Get the number of templates in this inventory. This only counts
   * distinct templates. The number of instances of every template is
   * ignored.
   */
  virtual size_t GetEntityTemplateCount () const = 0;

  /**
   * Get some entity template.
   */
  virtual iCelEntityTemplate* GetEntityTemplate (size_t idx) const = 0;

  /**
   * Get the amount of instances for a given template.
   */
  virtual int GetEntityTemplateAmount (size_t idx) const = 0;

  /**
   * Test if some entity template is in the inventory.
   */
  virtual bool In (iCelEntityTemplate* tpl) const = 0;

  /**
   * Find the index of some entity template in the inventory.
   * Return csArrayItemNotFound if not in inventory.
   */
  virtual size_t FindEntityTemplate (iCelEntityTemplate* tpl) const = 0;

  /**
   * Find the index of some entity template in the inventory by name.
   * Return csArrayItemNotFound if not in inventory.
   */
  virtual size_t FindEntityTemplate (const char* name) const = 0;

   /**
   * Find the index of some entity template in the inventory by class.
   * The first template with given class will be returned.
   * Return csArrayItemNotFound if not in inventory.
   */
  virtual size_t FindEntityTemplate (csStringID classid) const = 0;

  //--------------------------------------------------------------------

  /**
   * Get an entity from a generic slot (space system).
   */
  virtual iCelEntity* GetEntitySlot (iCelParameterBlock* params) const = 0;

  /**
   * If true reject any entities that don't have this
   * characteristic. By default this is false
   * which means that the inventory will automatically assume
   * valid values when this characteristic is not present.
   *
   * This function can fail if this inventory already has
   * contents and entities in this inventory do not satisfy
   * the strict condition.
   */
  virtual bool SetStrictCharacteristics (const char* charName, bool strict) = 0;

  /**
   * Return the value of 'strict characteristics'.
   */
  virtual bool HasStrictCharacteristics (const char* charName) const = 0;

  /**
   * Set the constraints for some characteristic. There is
   * a min and max value for one entity and also a total max
   * value for the entire inventory.
   *
   * This function can fail if the new constraints cause
   * current contents to be invalidated.
   */
  virtual bool SetConstraints (const char* charName,
  	float minValue, float maxValue, float totalMaxValue) = 0;

  /**
   * Get the constraints for some characteristic.
   * Returns false if there are no constraints defined.
   */
  virtual bool GetConstraints (const char* charName,
  	float& minValue, float& maxValue, float& totalMaxValue) const = 0;

  /**
   * Remove constraints for a characteristic.
   */
  virtual void RemoveConstraints (const char* charName) = 0;

  /**
   * Remove all constraints.
   */
  virtual void RemoveAllConstraints () = 0;

  /**
   * Get the current value for some characteristic.
   */
  virtual float GetCurrentCharacteristic (const char* charName) const = 0;
  
  /**
   * Mark this characteristic as dirty for this inventory and
   * all inventories this entity is in.
   * If 'charName' == 0 then all characteristics
   * will be marked dirty.
   */
  virtual void MarkDirty (const char* charName) = 0;

  /**
   * Test constraints for this characteristic for this inventory
   * and all inventories * this entity is in. If 'charName' == 0
   * then all characteristics will be tested.
   */
  virtual bool TestConstraints (const char* charName) = 0;

  /**
   * This is a debugging function to dump the contents of the inventory and
   * all constraints.
   */
  virtual void Dump () = 0;
  
  /**
   * Set an optional space manager for this inventory. If you don't do this
   * then the inventory system will use no space/slot system.
   */
  virtual void SetSpace (iCelInventorySpace* space) = 0;

  /**
   * Get the used space manager.
   * \warning You should never remove or add entities to the space directly
   * (by using its api), do it from PcInventory RemoveEntity or AddEntity
   * functions instead.
   */
  virtual iCelInventorySpace* GetSpace () = 0;

  /**
   * Add an entity class which is allowed for this inventory.
   * If no entity classes are added then there is no restriction.
   */
  virtual void AddAllowedClass (csStringID cls) = 0;

  /**
   * Clear the allowed entity classes which makes sure that all entities
   * are allowed.
   */
  virtual void ClearAllowedClasses () = 0;

  /**
   * Return true if a given class is allowed. If no allowed classes
   * were given then this will always return true.
   */
  virtual bool IsClassAllowed (csStringID cls) const = 0;

  /**
   * Set a loot generator for this inventory. As soon as this is set
   * it will be used as soon as the inventory is opened (querried).
   * If the inventory already contains items then the loot will be
   * appended. To reactivate a given loot generator you just have
   * to call this function again.
   */
  virtual void SetLootGenerator (iLootGenerator* generator) = 0;

  /**
   * Get the loot generator for this inventory.
   */
  virtual iLootGenerator* GetLootGenerator () const = 0;
};

#endif // __CEL_PF_INV__

