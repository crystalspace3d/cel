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

struct iCelEntity;

SCF_VERSION (iCelPlLayer, 0, 0, 1);

/**
 * This is the Physical Layer itself.
 */
struct iCelPlLayer : public iBase
{
  /**
   * Create a new physical layer entity.
   */
  virtual iCelEntity* CreateEntity () = 0;
};

#endif // __CEL_PL_PL__

