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

#ifndef __CEL_PLIMP_PL__
#define __CEL_PLIMP_PL__

#include "csutil/csvector.h"
#include "iutil/comp.h"
#include "pl/pl.h"

struct iObjectRegistry;

/**
 * Implementation of the physical layer.
 */
class celPlLayer : public iCelPlLayer
{
private:
  csVector pf_list;

public:
  celPlLayer (iBase* parent);
  virtual ~celPlLayer ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual iCelEntity* CreateEntity ();
  virtual iCelMessage* CreateMessage (const char* msg_string, ...);
  virtual void RegisterPropertyClassFactory (iCelPropertyClassFactory* pf);
  virtual void UnregisterPropertyClassFactory (
  	iCelPropertyClassFactory* pf);
  virtual int GetPropertyClassFactoryCount () const;
  virtual iCelPropertyClassFactory* GetPropertyClassFactory (int idx) const;
  virtual iCelPropertyClassFactory* FindPropertyClassFactory (
  	const char* name) const;

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPlLayer);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_PLIMP_PL__

