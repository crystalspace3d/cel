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
struct iPcInventory;

/**
 * Implement this interface if you want to learn when an entity is selected.
 */
struct iUIInventorySelectionCallback : public virtual iBase
{
  SCF_INTERFACE (iUIInventorySelectionCallback, 0, 0, 1);

  virtual void SelectEntity (iCelEntity* entity) = 0;
};

/**
 * The inventory itself.
 */
struct iUIInventory: public virtual iBase
{
  SCF_INTERFACE (iUIInventory, 0, 0, 1);

  /// Open the inventory.
  virtual void Open (const char* title, iPcInventory* inventory) = 0;
  /// Close the inventory.
  virtual void Close () = 0;

  /// Add a new selection listener.
  virtual void AddSelectionListener (iUIInventorySelectionCallback* cb) = 0;
  virtual void RemoveSelectionListener (iUIInventorySelectionCallback* cb) = 0;
};

#endif // __CEL_UITOOLS_INVENTORY__

