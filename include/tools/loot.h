/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein
  
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

#ifndef __CEL_LOOT__
#define __CEL_LOOT__

struct iParameter;
struct iCelParameterBlock;
struct iPcInventory;

/**
 * Loot generator. Both loot packages as loot selectors (see below) are loot
 * generators.
 */
struct iLootGenerator : public virtual iBase
{
  SCF_INTERFACE (iLootGenerator, 0, 0, 1);

  /**
   * Generate loot in an inventory. The inventory doesn't have to be empty for this to work.
   * If there are already items in the inventory then the loot is simply added.
   * If the constraints are set too strict then it is possible that no loot can
   * ever be generated.
   * The given parameter block should at the very least contain 'this' as the current entity.
   * This function returns false in case the inventory rejected (part) of the generated loot.
   * In that case it is possible that part of the loot was added.
   */
  virtual bool GenerateLoot (iPcInventory* inventory, iCelParameterBlock* params) = 0;
};

/**
 * A loot package. This represents a set of items that an inventory can
 * obtain as soon as it is opened. A loot package is also a loot generator.
 */
struct iLootPackage : public virtual iBase
{
  SCF_INTERFACE (iLootPackage, 0, 0, 1);

  /**
   * Get the name of this loot package.
   */
  virtual const char* GetName () const = 0;

  /**
   * Set and get the range of items that are allowed. Default range is 1
   * to 10 which means that this loot package will generate at least 1
   * but maximum 10 distinct items (ignoring the individual count for each
   * item).
   */
  virtual void SetMinLoot (int m) = 0;
  virtual int GetMinLoot () const = 0;
  virtual void SetMaxLoot (int m) = 0;
  virtual int GetMaxLoot () const = 0;

  /**
   * If random generation is disabled (default) then at the time loot items are generated,
   * the items are considered in the order with which they were added to this package. So
   * the first item that was added is considered first and then the second, and so on. This
   * means that items that come first in the list usually have more chance of ending up in
   * a loot package. If random generation is enabled then the loot items are shuffled first.
   */
  virtual void SetRandomGeneration (bool r) = 0;
  virtual bool IsRandomGenerationEnabled () const = 0;

  /**
   * Add a loot item.
   * @param templateName is the name of the entity template that will be used for this item.
   * If this template doesn't exist in the CEL system then this is not an error. The item
   * will simply never get generated (unless it comes into live at some point in the future).
   * @param chance is the chance (between 0 and 1) that this item will be generated.
   * @param minamount is the minimum amount of instances of this item.
   * @param maxamount is the maximum amount of instances of this item.
   * @param validatePar is an optional validation parameter that will be evaluated
   * at the time this item is considered for addition (so after 'chance' is considered).
   * If this parameter evaluates to true then the item will be used (this parameter is
   * typically implemented with an expression). This expression can use the dynamic
   * parameter @this to get the current entity containing the inventory that we're generating for.
   */
  virtual void AddLootItem (const char* templateName, float chance, int minamount, int maxamount,
      iParameter* validatePar = 0) = 0;
  virtual size_t GetLootItemCount () const = 0;
  virtual const char* GetLootItemTemplate (size_t idx) const = 0;
  virtual float GetLootItemChance (size_t idx) const = 0;
  virtual int GetLootItemMinAmount (size_t idx) const = 0;
  virtual int GetLootItemMaxAmount (size_t idx) const = 0;
  virtual iParameter* GetLootItemValidate (size_t idx) const = 0;

  /**
   * Get the loot generator that this object represents.
   */
  virtual iLootGenerator* QueryLootGenerator () = 0;
};

/**
 * A loot package selector. This selector is responsible for selecting the right loot package
 * based on a number of criteria and rules. A loot selector is also a loot generator.
 */
struct iLootSelector : public virtual iBase
{
  SCF_INTERFACE (iLootSelector, 0, 0, 1);

  /**
   * Get the name of this loot selector.
   */
  virtual const char* GetName () const = 0;

  /**
   * Add a loot selection rule. Loot selection rules are considered in order that they are added
   * to this selector.
   * @param generator is the loot generator to select (can be a loot package or even another
   * loot selector).
   * @param chance is a number between 0 and 1 indicating the chance with which this generator
   * will be chosen.
   * @param validatePar is an optional validation parameter. After the 'chance' is evaluated this
   * validation parameter can be used to finally decide if the generator should be considered.
   * Usually an expression parameter is used for this. This expression can use the dynamic
   * parameter @this to get the current entity containing the inventory that we're generating for.
   */
  virtual void AddLootGenerator (iLootGenerator* generator, float chance, iParameter* validatePar = 0) = 0;
  virtual size_t GetLootGeneratorCount () const = 0;
  virtual iLootGenerator* GetLootGenerator (size_t idx) const = 0;
  virtual float GetLootGeneratorChance (size_t idx) const = 0;
  virtual iParameter* GetLootGeneratorValidate (size_t idx) const = 0;

  /**
   * Get the loot generator that this object represents.
   */
  virtual iLootGenerator* QueryLootGenerator () = 0;
};

/**
 * The Loot Manager
 * This plugin is responsible for maintaining loot generation.
 */
struct iLootManager: public virtual iBase
{
  SCF_INTERFACE (iLootManager, 0, 0, 1);

  /**
   * Create a new loot package.
   */
  virtual iLootPackage* CreateLootPackage (const char* name) = 0;
  virtual size_t GetLootPackageCount () const = 0;
  virtual iLootPackage* GetLootPackage (size_t idx) const = 0;
  virtual iLootPackage* FindLootPackage (const char* name) const = 0;

  /**
   * Remove a loot package. Note that if this loot package is still in
   * use by other loot selectors or in inventories then the package will
   * not be deleted but it will no longer be available for later.
   */
  virtual void RemoveLootPackage (iLootPackage* package) = 0;

  /**
   * Create a new loot selector.
   */
  virtual iLootSelector* CreateLootSelector (const char* name) = 0;
  virtual size_t GetLootSelectorCount () const = 0;
  virtual iLootSelector* GetLootSelector (size_t idx) const = 0;
  virtual iLootSelector* FindLootSelector (const char* name) const = 0;

  /**
   * Remove a loot selector. Note that if this loot selector is still in
   * use by other loot selectors or in inventories then the selector will
   * not be deleted but it will no longer be available for later.
   */
  virtual void RemoveLootSelector (iLootSelector* selector) = 0;

  /**
   * Find a loot generator by name. This will look in both the loot
   * packages as in the loot selectors so it is preferable that the
   * names of packages and selectors are unique.
   */
  virtual iLootGenerator* FindLootGenerator (const char* name) const = 0;
};

#endif // __CEL_LOOT__

