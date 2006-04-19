/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_LIGHT__
#define __CEL_PF_LIGHT__

#include "cstypes.h"
#include "csutil/scf.h"

struct iLight;
struct iSector;
class csVector3;

/**
 * This is a property class holding the representation of a light.
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>SetLight: parameters 'name' (string). This sets an already existing
 *     light for this pclight.
 * <li>MoveLight: parameters 'pos' (vector3). Move a light.
 * </ul>
 */
struct iPcLight : public virtual iBase
{
  SCF_INTERFACE (iPcLight, 0, 0, 1);
  
  /**
   * Set the light by name. Returns false if the light cannot be found.
   */
  virtual bool SetLight (const char* lightname) = 0;
  
  /**
   * Set the light directly.
   */
  virtual void SetLight (iLight* light) = 0;

  /**
   * Get the light.
   */
  virtual iLight* GetLight () const = 0;
};

#endif // __CEL_PF_LIGHT__

