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

#ifndef __CEL_BL_BL__
#define __CEL_BL_BL__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCelBehaviour;
struct iCelEntity;

SCF_VERSION (iCelBlLayer, 0, 0, 1);

/**
 * This is the Behaviour Layer itself.
 */
struct iCelBlLayer : public iBase
{
  /**
   * The name of this behaviour layer.
   */
  virtual const char* GetName () const = 0;

  /**
   * Create a new behaviour layer entity.
   * The given name is specific to the BL implementation. It can
   * be the name of a script for example.
   */
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity,
  	const char* name) = 0;
};

#endif // __CEL_BL_BL__

