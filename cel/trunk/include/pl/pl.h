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

#ifndef __CEL_PL_PL__
#define __CEL_PL_PL__

#include "cstypes.h"
#include "csutil/scf.h"

SCF_DECLARE_FAST_INTERFACE (iCelPlLayer)
SCF_DECLARE_FAST_INTERFACE (iCelBlLayer)
SCF_DECLARE_FAST_INTERFACE (iCelEntity)
SCF_DECLARE_FAST_INTERFACE (iCelPropertyClass)
SCF_DECLARE_FAST_INTERFACE (iCelPropertyClassList)
SCF_DECLARE_FAST_INTERFACE (iCelPropertyClassFactory)
SCF_DECLARE_FAST_INTERFACE (iCelBehaviour)

struct iCelEntity;
struct iCelMessage;
struct iCelPropertyClassFactory;

SCF_VERSION (iCelPlLayer, 0, 0, 2);

/**
 * This is the Physical Layer itself.
 */
struct iCelPlLayer : public iBase
{
  /**
   * Create a new physical layer entity.
   */
  virtual iCelEntity* CreateEntity () = 0;

  /**
   * Create a message.
   */
  virtual iCelMessage* CreateMessage (const char* msg_string, ...) = 0;

  /**
   * Register a property class factory with this physical layer.
   */
  virtual void RegisterPropertyClassFactory (iCelPropertyClassFactory* pf) = 0;

  /**
   * Unregister a property class factory.
   */
  virtual void UnregisterPropertyClassFactory (
  	iCelPropertyClassFactory* pf) = 0;

  /**
   * Get the number of registered property class factories.
   */
  virtual int GetPropertyClassFactoryCount () const = 0;

  /**
   * Get the specificied property class factory.
   */
  virtual iCelPropertyClassFactory* GetPropertyClassFactory (int idx) const = 0;

  /**
   * Find a property class factory by name.
   */
  virtual iCelPropertyClassFactory* FindPropertyClassFactory (
  	const char* name) const = 0;
};

#endif // __CEL_PL_PL__

