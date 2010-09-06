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

#ifndef __CEL_PF_STEER__
#define __CEL_PF_STEER__

#include "cstypes.h"
#include "csutil/scf.h"
#include "physicallayer/entity.h"

struct iPcLinearMovement;

/**
 * This is a steering property class. It works closely with pclinmove and
 * pcactormove in order to move an object from one position to another
 * while checking collision detection along the way.
 *
 * This property class can send out the following messages:
 * - 'cel.move.arrived' (old 'pcsteer_arrived'): arrived at final position.
 * - 'cel.move.avoiding_collision' (old 'pcsteer_avoiding_collision'): ?
 * - 'cel.move.interrupted' (old 'pcsteer_interrupted'): movement has been interrupted.
 *
 * This property class supports the following actions (add prefix 'cel.move.steer.action.'
 * if you want to access this action through a message):
 * - Seek: parameters 'sectorname' (string), 'position' (vector3).
 * - Flee: parameters 'sectorname' (string), 'position' (vector3).
 * - Pursue: parameters 'target' (iCelEntity*), 'max_prediction' (float),
 * - Interrupt: interrupt the current movement.
 * This property class supports the following properties:
 * - position (vector3, read only): current end position.
 * - sqradius (float, read/write): current squared radius.
 * - moving (bool, read only): returns true if currently moving.
 * - cur_position (vector3, read only): current position.
 * - cur_direction (vector3, read only): current direction.
 * - check_cohesion (bool, read/write): returns true if currently checking for cohesion.
 * - check_separation (bool, read/write): returns true if currently checking for separation.
 * - check_arrival (bool, read/write): returns true if currently checking for arrival.
 * - check_dm (bool, read/write): returns true if currently checking for direction matching.
 * - arrival_radius (bool, read/write): current arrival radius.
 * - collision_avoidance (bool, read only): returns true if currently checking for collisions.
 * - ca_lookahead (float, read/write): current collision avoidance max lookahead.
 * - separation_radius (float, read/write): current min radius before separating.
 * - cohesion_radius (float, read/write): current max radius before activating cohesion.
 * - separation_weight (float, read/write): current separation weight.
 * - cohesion_weight (float, read/write): current cohesion weight.
 * - dm_weight (float, read/write): current direction matching weight.
 * - pursue_max_prediction (float, read/write): current pursue max prediction.
 * - current_action (int, read only): current action code value.
 * - delay_recheck (int, read/write): current delay before calling TickOnce.
 */
struct iPcSteer : public virtual iBase
{
  SCF_INTERFACE (iPcSteer, 1, 0, 0);

  /**
   * Stay vigilant about changes around.
   * Enables checking for collision and separation even when not moving.
   */
  virtual bool Vigilant () = 0;
  /**
   * Move to the specified position. When you call this function this property
   * class will attempt to move the linmove to the correct position.
   * If arrival checking is activated it will stop at the specified position and
   * it will get a 'pcsteer_arrived' message.
   * If this property class was already controlling a movement
   * then that movement will be interrupted (possibly giving a
   * pcsteer_interrupted message).
   * \param sector is the desired sector to move to.
   * \param position is the desired position to move to.
   */
  virtual bool Seek (iSector* sector, const csVector3& position) = 0;

  /**
   * Move away from the specified position. When you call this function this property
   * class will attempt to move the linmove to oposite of the specified position.
   * If this property class was already controlling a movement
   * then that movement will be interrupted (possibly giving a
   * pcsteer_interrupted message).
   * \param sector is the desired sector to move away from.
   * \param position is the desired position to move away from.
   */
  virtual bool Flee (iSector* sector, const csVector3& position) = 0;

  /**
   * Move to the specified targets position. When you call this function this property
   * class will attempt to move the linmove to the targets current position.
   * The targets position will be updated periodically. Also, if max_prediction > 0,
   * this function will calculate targets future position instead of its actual
   * position in order to have more chance to cacth its target.
   * If arrival checking is activated it will stop at the specified position and
   * it will get a 'pcsteer_arrived' message.
   * If this property class was already controlling a movement
   * then that movement will be interrupted (possibly giving a
   * pcsteer_interrupted message).
   * \param target is the desired target to pursue.
   * \param max_prediction is the maximun prediction that can be used to calculate 
   * target's future position.
   */
  virtual bool Pursue (iCelEntity* target, float max_prediction)=0;

  /**
   * Sets arrival checking on with arrival_radius = radius
   * \param radius sets current arrival_radius
   */
  virtual void CheckArrivalOn (float radius) = 0;

  /**
   * Sets arrival checking off
   */
  virtual void CheckArrivalOff () = 0;

  /**
   * Sets collision avoidance on
   * \param lookahead sets current ca_lookahead
   * \param weight sets current ca_weight
   */
  virtual void CollisionAvoidanceOn (float lookahead, float weight) = 0;

  /**
   * Sets Collision Avoidance off
   */
  virtual void CollisionAvoidanceOff () = 0;


  /**
   * Sets Cohesion on
   * \param targets sets current cohesion_targets
   * \param radius sets current cohesion_radius
   * \param max_radius sets current cohesion_max_radius
   * \param weight sets current cohesion_weight
   */
  virtual void CohesionOn (iCelEntityList* targets, float radius, float max_radius, float weight) = 0;

  /**
   * Sets Cohesion off
   */
  virtual void CohesionOff () = 0;

  /**
   * Sets Separation on
   * \param targets sets current separation_targets
   * \param radius sets current separation_radius
   * \param weight sets current separation_weight
   */
  virtual void SeparationOn (iCelEntityList* targets, float radius, float weight) = 0;

  /**
   * Sets Separation off
   */
  virtual void SeparationOff () = 0;

  /**
   * Sets Direction Matching on
   * \param targets sets current dm_targets
   * \param weight sets current dm_weight
   */
  virtual void DirectionMatchingOn (iCelEntityList* targets, float weight) = 0;

  /**
   * Sets Direction Matching off
   */
  virtual void DirectionMatchingOff () = 0;

  /**
   * Interrupt a movement. The behaviour will get a 'pcsteer_interrupted'
   * message if the mover was really moving. Otherwise nothing happens.
   */
  virtual void Interrupt () = 0;

  /**
   * Get the end sector that we want to move to.
   */
  virtual iSector* GetSector () const = 0;

  /**
   * Get the end position that we want to move to.
   */
  virtual const csVector3& GetPosition () const = 0;

  /**
   * Returns a random number using a random binomial.
   */
  virtual float RandomBinomial (float rate) = 0;

  /**
   * Sets current delay_recheck
   * \param delay sets delay_recheck
   */
  virtual void SetDelayRecheck (int delay) = 0;

  /**
   * Return true if currently moving.
   */
  virtual bool IsMoving () const = 0;
};

#endif // __CEL_PF_STEER__
