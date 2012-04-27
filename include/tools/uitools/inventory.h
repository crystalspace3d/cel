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

#ifndef __CEL_UITOOLS_INVENTORY__
#define __CEL_UITOOLS_INVENTORY__

struct iCelEntity;
struct iCelEntityTemplate;
struct iPcInventory;

struct iString;
struct iMeshFactoryWrapper;
struct iTextureHandle;
struct iGraphics3D;

/**
 * Get information about an entity/template that the inventory can use to
 * render the entity in the inventory.
 */
struct iUIInventoryInfo : public virtual iBase
{
  SCF_INTERFACE (iUIInventoryInfo, 0, 0, 1);

  /**
   * Get the name for this entity. Can be 0.
   */
  virtual csRef<iString> GetName (iCelEntity* entity) = 0;
  virtual csRef<iString> GetName (iCelEntityTemplate* tpl, int count) = 0;

  /**
   * Get the description to show for this entity. Can be 0.
   * An inventory implementation can show this description directly in the
   * inventory or it can show it as a tooltip.
   */
  virtual csRef<iString> GetDescription (iCelEntity* entity) = 0;
  virtual csRef<iString> GetDescription (iCelEntityTemplate* tpl, int count) = 0;

  /**
   * Get a mesh factory which can be used for showing in the inventory.
   * Can be 0.
   */
  virtual iMeshFactoryWrapper* GetMeshFactory (iCelEntity* entity) = 0;
  virtual iMeshFactoryWrapper* GetMeshFactory (iCelEntityTemplate* tpl, int count) = 0;

  /**
   * Get an image which can be used for showing in the inventory. This
   * can return 0.
   */
  virtual iTextureHandle* GetTexture (iCelEntity* entity) = 0;
  virtual iTextureHandle* GetTexture (iCelEntityTemplate* tpl, int count) = 0;
};


/**
 * Implement this interface if you want to learn when an entity is selected.
 */
struct iUIInventorySelectionCallback : public virtual iBase
{
  SCF_INTERFACE (iUIInventorySelectionCallback, 0, 0, 1);

  virtual void SelectEntity (iCelEntity* entity) = 0;
  virtual void SelectTemplate (iCelEntityTemplate* tpl) = 0;
};

/**
 * The inventory itself.
 */
struct iUIInventory: public virtual iBase
{
  SCF_INTERFACE (iUIInventory, 0, 0, 1);

  /**
   * Setup a class which will be used to get information about the entities
   * to show in the inventory. If this function is not called then there
   * will be a default implementation that just gets the name from the entities.
   */
  virtual void SetInfo (iUIInventoryInfo* info) = 0;

  /**
   * Get the current info. If you want to implement your own info class
   * that only overrides one particular function you can delegate back to
   * this class provided you hold a reference to it.
   */
  virtual iUIInventoryInfo* GetInfo () const = 0;

  /// Open the inventory.
  virtual void Open (const char* title, iPcInventory* inventory) = 0;
  /// Close the inventory.
  virtual void Close () = 0;
  /// While the inventory is open you can query the inventory.
  virtual iPcInventory* GetInventory () const = 0;

  /// Add a new selection listener.
  virtual void AddSelectionListener (iUIInventorySelectionCallback* cb) = 0;
  virtual void RemoveSelectionListener (iUIInventorySelectionCallback* cb) = 0;

  /**
   * Set a style option.
   * Different inventory implementations can support different style options.
   * This function returns false if a certain style option is not supported
   * or if the value is invalid for this style.
   */
  virtual bool SetStyleOption (const char* name, const char* value) = 0;
};

#endif // __CEL_UITOOLS_INVENTORY__

