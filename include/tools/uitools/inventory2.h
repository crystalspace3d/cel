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

#ifndef __CEL_UITOOLS_INVENTORY2__
#define __CEL_UITOOLS_INVENTORY2__

struct iCelEntity;
struct iCelEntityTemplate;
struct iPcInventory;

/**
 * Implement this interface if you want to learn when an entity is selected.
 */
struct iUIInventory2SelectionCallback : public virtual iBase
{
  SCF_INTERFACE (iUIInventory2SelectionCallback, 0, 0, 1);

  /// 'left' is true if the selection was on the left side.
  virtual void SelectEntity (iCelEntity* entity, bool left) = 0;
  virtual void SelectTemplate (iCelEntityTemplate* tpl, bool left) = 0;
};

/**
 * The double inventory.
 */
struct iUIInventory2: public virtual iBase
{
  SCF_INTERFACE (iUIInventory2, 0, 0, 1);

  /// Open the inventory.
  virtual void Open (const char* title, iPcInventory* inventoryLeft,
      iPcInventory* inventoryRight) = 0;
  /// Close the inventory.
  virtual void Close () = 0;

  /// Add a new selection listener.
  virtual void AddSelectionListener (iUIInventory2SelectionCallback* cb) = 0;
  virtual void RemoveSelectionListener (iUIInventory2SelectionCallback* cb) = 0;
};

#endif // __CEL_UITOOLS_INVENTORY2__

