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

#ifndef __CEL_PF_COMMON_STDPC__
#define __CEL_PF_COMMON_STDPC__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "csutil/csobjvec.h"
#include "pl/propclas.h"
#include "pl/propfact.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * This is a common implementation for a property class
 * from which all other property classes can inherit.
 * This makes it easier to write a property class.
 */
class celPcCommon : public iCelPropertyClass
{
private:
  csObjVector callbacks;

protected:
  iCelEntity* entity;
  iObjectRegistry* object_reg;

protected:
  void FirePropertyChangeCallback (int propertyId);

public:
  celPcCommon (iObjectRegistry* object_reg);
  virtual ~celPcCommon ();

  SCF_DECLARE_IBASE;

  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
  virtual bool AddPropertyChangeCallback (iCelPropertyChangeCallback* cb);
  virtual bool RemovePropertyChangeCallback (
  	iCelPropertyChangeCallback* cb);

  virtual bool SetProperty (csStringID, long) { return false; }
  virtual bool SetProperty (csStringID, float) { return false; }
  virtual bool SetProperty (csStringID, bool) { return false; }
  virtual bool SetProperty (csStringID, const char*) { return false; }
  virtual bool SetProperty (csStringID, const csVector3&) { return false; }
  virtual celDataType GetPropertyOrActionType (csStringID)
  {
    return CEL_DATA_NONE;
  }
  virtual bool IsPropertyReadOnly (csStringID) { return false; }
  virtual long GetPropertyLong (csStringID) { return 0; }
  virtual float GetPropertyFloat (csStringID) { return 0; }
  virtual bool GetPropertyBool (csStringID) { return false; }
  virtual const char* GetPropertyString (csStringID) { return NULL; }
  virtual bool GetPropertyVector (csStringID, csVector3&) { return false; }
  virtual bool PerformAction (csStringID, const char*) { return false; }
  virtual int GetPropertyAndActionCount () const { return 0; }
  virtual csStringID GetPropertyOrActionID (int) { return csInvalidStringID; }
};

#endif // __CEL_PF_COMMON_STDPC__

