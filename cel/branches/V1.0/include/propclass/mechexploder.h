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

#ifndef __CEL_PF_MECHANICS_EXPLODER__
#define __CEL_PF_MECHANICS_EXPLODER__

#include "cstypes.h"
#include "csutil/scf.h"

SCF_VERSION (iPcMechanicsExploder, 0, 0, 1);

/**
 * A property class for handling the physics of an explosion.
 *
 * This property class depends on iPcMechanicsObject and iPcAttractor, and
 * extends their functionality.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - Explode: no parameters.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property):
 * - force (float, read/write): the force of the explosion.
 */
struct iPcMechanicsExploder : public iBase
{
  /**
   * Set the magnitude of the force of the explosion this object will create.
   */
  virtual void SetExplosiveForce (float force) = 0;

  /**
   * Blow up the object!
   */
  virtual void Explode () = 0;
};

#endif // __CEL_PF_MECHANICS_EXPLODER__

