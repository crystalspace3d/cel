/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_DYNMOVE__
#define __CEL_PF_DYNMOVE__

#include "cstypes.h"
#include "csutil/scf.h"

/**
 * This is a property class for dynamic movement.
 *
 * This property class supports the following properties:
 * - speed (float, read/write): default 1.0, movement speed.
 * - jumpspeed (float, read/write): default 1.0, jumping speed.
 * - rotspeed (float, read/write): default 1.0, rotation speed.
 */
struct iPcDynamicMove : public virtual iBase
{
  SCF_INTERFACE (iPcDynamicMove, 0, 0, 1);
};

#endif // __CEL_PF_DYNMOVE__

