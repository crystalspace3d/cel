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

#ifndef __CEL_PF_INV__
#define __CEL_PF_INV__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCelEntity;

SCF_VERSION (iPcInventory, 0, 0, 1);

/**
 * This is an inventory property class.
 */
struct iPcInventory : public iBase
{
  /**
   * Add an entity. Returns false if the entity could
   * not be added (capacity exceeded for example).
   * Note that it is safe to add entities that are already in the inventory.
   * Nothing will happen then and true will be returned.
   */
  virtual bool AddEntity (iCelEntity* entity) = 0;

  /**
   * Remove an entity.
   */
  virtual void RemoveEntity (iCelEntity* entity) = 0;

  /**
   * Remove all entities.
   */
  virtual void RemoveAll () = 0;

  /**
   * Get the number of entities in this inventory.
   */
  virtual int GetEntityCount () const = 0;

  /**
   * Get some entity.
   */
  virtual iCelEntity* GetEntity (int idx) const = 0;

  /**
   * If true reject any entities that don't have this
   * characteristic. By default this is false
   * which means that the inventory will automatically assume
   * valid values when this characteristic is not present.
   */
  virtual void SetStrictCharacteristics (const char* charName, bool strict) = 0;

  /**
   * Return the value of 'strict characteristics'.
   */
  virtual bool HasStrictCharacteristics (const char* charName) const = 0;

  /**
   * Set the constraints for some characteristic. There is
   * a min and max value for one entity and also a total max
   * value for the entire inventory. WARNING! Changing
   * constraints when the inventory contains items in such a
   * way that the current contents violates the new constraints
   * will have no effect on the contents!
   */
  virtual void SetConstraints (const char* charName, float minValue, float maxValue,
		  float totalMaxValue) = 0;

  /**
   * Get the constraints for some characteristic.
   * Returns false if there are no constraints defined.
   */
  virtual bool GetConstraints (const char* charName, float& minValue, float& maxValue,
		  float& totalMaxValue) const = 0;

  /**
   * Remove constraints for a characteristic.
   */
  virtual void RemoveConstraints (const char* charName) = 0;

  /**
   * Get the current value for some characteristic.
   */
  virtual float GetCurrentCharacteristic (const char* charName) const = 0;
  
  /**
   * Test if adding a given entity would work with the current constraints.
   * If not the name of the violating characteristic is returned. Otherwise
   * NULL is returned.
   */
  virtual const char* TestAddEntity (iCelEntity* entity) = 0;

  /**
   * Test if changing a characteristic of some entity inside this inventory
   * would violate the inventory constraints. Returns true if ok.
   * If 'newLocalValue' points to NULL this means that the entity no longer
   * has that property.<br>
   * Note that 'newLocalValue' is the new 'local' value of the entity. The
   * inherited values are not included.
   */
  virtual bool TestCharacteristicChange (iCelEntity* entity, const char* charName, float* newLocalValue) = 0;

  /**
   * Really update a characteristic for some entity.
   */
  virtual void UpdateCharacteristic (iCelEntity* entity, const char* charName, float newLocalValue) = 0;

  /**
   * This is a debugging function to dump the contents of the inventory and
   * all constraints.
   */
  virtual void Dump () = 0;
};

#endif // __CEL_PF_INV__

