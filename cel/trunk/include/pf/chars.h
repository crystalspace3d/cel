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
  virtual bool SetCharProperty (const char* name, float value) = 0;

  /**
   * Get characteristic property.
   */
  virtual float GetCharProperty (const char* name) const = 0;

  /**
   * Clear a property.
   */
  virtual void ClearProperty (const char* name) = 0;

  /**
   * Returns true if a property is present.
   */
  virtual bool HasProperty (const char* name) const = 0;

  /**
   * Clear all properties.
   */
  virtual void ClearAll () = 0;

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
};

#endif // __CEL_PF_CHARS__

