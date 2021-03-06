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

#ifndef __CEL_PF_SOLID__
#define __CEL_PF_SOLID__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCollider;
struct iPcMesh;
class csBox3;

/**
 * A solid representation of an entity. This is used
 * for collision detection.
 *
 * This property class supports the following actions (add prefix 'cel.move.solid.action.'
 * if you want to access this action through a message):
 * - Setup: no parameters. Call this after creation to actually setup
 *   the collider from the mesh that is attached so same entity.
 * - SetupBox: parameters 'min' (vector3) and 'max' (vector3).
 *   Setup a box collider (ignore attached mesh).
 */
struct iPcSolid : public virtual iBase
{
  SCF_INTERFACE (iPcSolid, 0, 0, 2);

  /**
   * Set mesh from which to create the solid
   * representation. If not set a default mesh will be found
   * from the parent entity.
   */
  virtual void SetMesh (iPcMesh* mesh) = 0;

  /**
   * Get the current mesh on which we're working.
   */
  virtual iPcMesh* GetMesh () const = 0;

  /**
   * Get (and create if needed) the collider for this
   * mesh. If this returns 0 it means that the mesh
   * object on which this property class is being used does
   * not support collision detection.
   */
  virtual iCollider* GetCollider () = 0;

  /**
   * Force creation of the collider (equivalent to calling
   * GetCollider() except that it forces a clean up of the previous
   * collider).
   */
  virtual void Setup () = 0;

  /**
   * Initialize a box collider (so ignore the mesh for collider).
   */
  virtual void SetupBox (const csBox3& box) = 0;
};

#endif // __CEL_PF_SOLID__

