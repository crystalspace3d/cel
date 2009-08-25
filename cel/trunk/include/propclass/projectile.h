/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_PROJECTILE__
#define __CEL_PF_PROJECTILE__

#include "cstypes.h"
#include "csutil/scf.h"

/**
 * This is a projectile property class. This class controls movement
 * of a projectile from one spot to another (or in some direction).
 *
 * This property class can send out the following messages
 * to the behaviour:
 * - pcprojectile_stopped: the projectile movement has stopped but no object
 *     was hit.
 * - pcprojectile_hit: we hit some object (entity, intersection, meshname).
 *     If we hit a mesh that is not an entity then this message will still
 *     be sent but with entity equal to 0 (you can then use the 'meshname'
 *     to see what mesh has been hit).
 *
 * This property class supports the following actions:
 * - Start: start movement in some direction. Parameters 'direction'
 *     (vector3), 'speed' (float), 'maxdist' (float), and 'maxhits' (long).
 * - Interrupt: interrupt the current movement.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - moving (bool, read only): returns true if currently moving.
 */
struct iPcProjectile : public virtual iBase
{
  SCF_INTERFACE (iPcProjectile, 0, 0, 1);

  /**
   * Start a movement of this projectile from its current position.
   * \param direction is the direction to go in.
   * \param speed is the speed in units per second.
   * \param maxdist is the maximum distance to move at which point
   * we will stop and send a pcprojectile_stopped message.
   * \param maxhits is the maximum number of hits at which point
   * we will stop and send a pcprojectile_stopped message.
   * \return false if we couldn't start the movement (usually this
   * means the projectile is already moving).
   */
  virtual bool Start (const csVector3& direction,
  	float speed, float maxdist, int maxhits) = 0;

  /**
   * Interrupt a movement.
   */
  virtual void Interrupt () = 0;

  /**
   * Return true if currently moving.
   */
  virtual bool IsMoving () const = 0;
};

#endif // __CEL_PF_PROJECTILE__

