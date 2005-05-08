/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2004 by Jorrit Tyberghein
  
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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, 
    MA 02111-1307, USA.
*/
#ifndef PHYSICALLAYER_NUMREG_H
#define PHYSICALLAYER_NUMREG_H

#include "csutil/scf.h"

/**
 *  This interface is used for ID -> iCelEntity* registers in the physical 
 * layer.
 *
 *  Currently, two implementations are available:
 *   - cel.numreg.lists (using arrays)
 *    This version is more effective if you don't care of the IDs used and if
 *   all the ID will be contiguous. This is usually the case of a server or
 *   a single app.
 *   - cel.numreg.hash (using hashs)
 *    This version is more effective if you want to allow any kind of ID used.
 *   This is usually the case of client apps.
 *
 * You can choose between these two implementations with the
 * iCelPlLayer::ChangeNumReg function.
 */
struct iNumReg : public iBase
{
  /**
   *  Registers an object in the registry and returns the new ID, in error
   * case ID 0 is returned
   */
  virtual uint Register (void *obj) = 0;

  /**
   *  Registers an object in the registry with the provided ID. You should call
   * this function only if you are sure the ID isn't allocated yet. It is also
   * advised to use the hash implementation for memory reasons.
   */
  virtual void RegisterWithID (void *obj, uint id) = 0;

  /**
   *  Removes an registered object from the registry
   */
  virtual bool Remove (uint id) = 0;

  /**
   *  Removes an registered object from the registry (Note: this is slow, 
   * whatever the implementation you choose)
   */
  virtual bool Remove (void *obj) = 0;

  /**
   *  Removes all objects from the registry.
   */
  virtual void Clear () = 0;

  /**
   *  Returns the object with ID id from the list.
   */
  virtual void* Get (uint id) = 0;

  /**
   *  Returns the size of the buffer (This is NOT the count of objects in the
   * registry). It is the size of array if you use array implementation, the 
   * size of the hash otherwise.
   */
  virtual size_t Length() = 0;
};

SCF_VERSION(iNumReg, 0, 0, 1);

#endif
