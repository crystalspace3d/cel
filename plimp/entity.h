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

#ifndef __CEL_PLIMP_ENTITY__
#define __CEL_PLIMP_ENTITY__

#include "pl/entity.h"

class celPropertyClassList;

/**
 * Implementation of iCelEntity.
 */
class celEntity : public iCelEntity
{
private:
  char* name;
  celPropertyClassList* plist;
  iCelBlEntity* bl_entity;

public:
  celEntity ();
  virtual ~celEntity ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual void SetName (const char* n);
  virtual iPropertyClassList* GetPropertyClassList ();
  virtual void SetBlEntity (iCelBlEntity* ent);
  virtual iCelBlEntity* GetBlEntity ()
  {
    return bl_entity;
  }
};

#endif // __CEL_PLIMP_ENTITY__

