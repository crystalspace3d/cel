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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PL_PROPCHANGE__
#define __CEL_PL_PROPCHANGE__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCelPropertyClass;

/**
 * This is a callback that is called whenever a property
 * changes for some property class.
 */
struct iCelPropertyChangeCallback : public virtual iBase
{
  SCF_INTERFACE (iCelPropertyChangeCallback, 0, 0, 1);

  /**
   * This function will be called when a property changes.
   * 'propertyId' is a local id (local to the property class)
   * indicating which property has changed.
   */
  virtual void PropertyChanged (int propertyId,
  	iCelPropertyClass* pclass) = 0;
};

#endif // __CEL_PL_PROPCHANGE__

