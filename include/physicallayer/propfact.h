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

#ifndef __CEL_PL_PROPFACT__
#define __CEL_PL_PROPFACT__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCelPropertyClass;

SCF_VERSION (iCelPropertyClassFactory, 0, 0, 1);

/**
 * This is a factory for property classes.
 * One factory can support multiple types of property classes.
 */
struct iCelPropertyClassFactory : public iBase
{
  /**
   * Get the name of this property class factory.
   */
  virtual const char* GetName () const = 0;

  /**
   * Create a property class of some type.
   */
  virtual csPtr<iCelPropertyClass> CreatePropertyClass () = 0;
};

#endif // __CEL_PL_PROPFACT__

