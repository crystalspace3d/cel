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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PF_CHARS__
#define __CEL_PF_CHARS__

#include "cstypes.h"
#include "csutil/scf.h"

SCF_DECLARE_FAST_INTERFACE (iPcCharacteristics)

struct iCelEntity;
struct iPcInventory;

SCF_VERSION (iPcCharacteristics, 0, 0, 1);

/**
 * This is a characteristics property class.
 */
struct iPcCharacteristics : public iBase
{
  /**
   * Set characteristic property.
   * Returns false if this is impossible because the entity
   * is currently in an inventory that does not allow this value.
   */
  virtual bool SetCharacteristic (const char* name, float value) = 0;

  /**
   * Indicate how this characteristic is inherited if this object
   * contains an inventory. The default is to ignore contents
   * of inventory for a characteristic ('factor' and 'add' equal to 0).
   * The value of this characteristic of the inventory will
   * be multiplied with 'factor' and added to 'add'.
   * <p>
   * This function can fail if the new properties cause the
   * current contents to fail.
   */
  virtual bool SetInheritedCharacteristic (const char* name,
		  float factor, float add) = 0;

  /**
   * Get characteristic property. This includes inherited
   * characteristics.
   */
  virtual float GetCharacteristic (const char* name) const = 0;

  /**
   * Get local characteristic property. This ignores inherited
   * characteristics.
   */
  virtual float GetLocalCharacteristic (const char* name) const = 0;

  /**
   * Get inherited characteristic property. This ignores local
   * characteristic value.
   */
  virtual float GetInheritedCharacteristic (const char* name) const = 0;

  /**
   * Clear a characteristic. This can fail if the object is currently
   * in an inventory which requires strict presence of this
   * characteristic.
   */
  virtual bool ClearCharacteristic (const char* name) = 0;

  /**
   * Returns true if a property is present.
   */
  virtual bool HasCharacteristic (const char* name) const = 0;

  /**
   * Clear all properties. This function can fail if
   * clearing a property causes a violation of parent
   * inventories. In that case part of the properties
   * may be cleared already.
   */
  virtual bool ClearAll () = 0;

  /**
   * Indicate that this object is added to some inventory.
   * This method is meant to be called ONLY by iPcInventory
   * implementations!
   */
  virtual void AddToInventory (iPcInventory* inv) = 0;

  /**
   * Indicate that this object is removed from some inventory.
   * This method is meant to be called ONLY by iPcInventory
   * implementations!
   */
  virtual void RemoveFromInventory (iPcInventory* inv) = 0;

  /**
   * Mark this characteristic as dirty for all inventories
   * this entity is in. If 'charName' == NULL then all characteristics
   * will be marked dirty.
   */
  virtual void MarkDirty (const char* charName) = 0;

  /**
   * Test constraints for this characteristic for all inventories
   * this entity is in. If 'charName' == NULL then all characteristics
   * will be tested.
   */
  virtual bool TestConstraints (const char* charName) = 0;

  /**
   * Debug dump information.
   */
  virtual void Dump () = 0;
};

#endif // __CEL_PF_CHARS__

