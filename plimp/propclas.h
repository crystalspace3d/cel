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

#ifndef __CEL_PLIMP_PROPCLASS__
#define __CEL_PLIMP_PROPCLASS__

#include "csutil/csvector.h"
#include "pl/propclas.h"

struct iCelEntity;

/**
 * A list of property classes.
 */
class celPropertyClassList : public iCelPropertyClassList
{
private:
  iCelEntity* parent_entity;
  csVector prop_classes;

public:
  celPropertyClassList (iCelEntity* parent_entity);
  virtual ~celPropertyClassList ();

  SCF_DECLARE_IBASE;

  virtual int GetCount () const;
  virtual iCelPropertyClass* Get (int n) const;
  virtual int Add (iCelPropertyClass* obj);
  virtual bool Remove (iCelPropertyClass* obj);
  virtual bool Remove (int n);
  virtual void RemoveAll ();
  virtual int Find (iCelPropertyClass* obj) const;
  virtual iCelPropertyClass* FindByName (const char* name) const;
  virtual iBase* FindByInterface (scfInterfaceID id, int version) const;
};

#endif // __CEL_PLIMP_PROPCLASS__

