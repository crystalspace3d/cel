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
 * This is a mover property class. It works closely with pclinmove and
 * pcactormove in order to move an object from one position to another
 * while checking collision detection along the way.
 *
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * - pcmover_impossible: don't even start the move: impossible. This
 *   message will have a 'meshname' parameter containing the name of the
 *   mesh that was preventing the move.
 * - pcmover_stuck: can't move further.
 * - pcmover_arrived: arrived at final position.
 * - pcmover_interrupted: movement has been interrupted.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - MoveTo: parameters 'sectorname' (string), 'position' (vector3),
 *     ,'sqradius' (float) and optional 'checklos' (bool, default false).
 * - Start: parameters 'sectorname' (string), 'position' (vector3),
 *     'up' (vector3), and 'sqradius' (float). This action is deprecated,
 *     you should use MoveTo instead.
 * - Interrupt: interrupt the current movement.
 *
 * This property class supports the 


following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - position (vector3, read only): current end position.
 * - sqradius (float, read/write): current squared radius.
 * - moving (bool, read only): returns true if currently moving.
 */
struct iPcSteer : public virtual iBase
{
  SCF_INTERFACE (iPcSteer, 1, 0, 0);

  /**
   * Move to the specified position. When you call this function this property
   * class will attempt to move the linmove to the correct position.
   * If it fails the behaviour will get a 'pcmover_stuck' message. Otherwise
   * it will get a 'pcmover_arrived' message.
   * If checklos parameter is true a line of sight test will be made in
   * advance, and if this function detects that line of sight is blocked
   * then the behaviour will get a 'pcmover_impossible' message and this 
   * function will return false then. 
   * If this property class was already controlling a movement
   * then that movement will be interrupted (possibly giving a
   * pcmover_interrupted message).
   * \param sector is the desired sector to move to.
   * \param position is the desired position to move to.
   * \param sqradius if the linmove ends up within the given squared
   * radius of the desired position the movement will stop and be considered
   * sucessful.
   * \param checklos whether to check line of sight to destination on movement
   * start.
   */
  virtual bool Seek (iSector* sector, const csVector3& position) = 0;

  virtual bool Flee (iSector* sector, const csVector3& position) = 0;

  virtual bool Wander (float offset, float radius, float rate) = 0;
  
  virtual bool Pursue (iCelEntity* target, float max_prediction)=0;
  
  /**
   *Actually performs the movement in cur_direction which has been
   *modified by the previous calls to all other steering functions
   *
   */

  virtual bool Move () = 0;

  virtual void CheckArrivalOn (float) = 0;

  virtual void CheckArrivalOff () = 0;

  virtual bool CheckArrival () = 0;

  virtual void CollisionAvoidanceOn (float, float) = 0;

  virtual void CollisionAvoidanceOff () = 0;

  virtual bool CollisionAvoidance () = 0;

  
  /**
   * Interrupt a movement. The behaviour will get a 'pcmover_interrupted'
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
   * Get the current squared radius.
   */
  //  virtual float GetSqRadius () const = 0;

  virtual float RandomBinomial (float rate) = 0;
  
  virtual void SetDelayRecheck (int delay) = 0;

  /**
   * Return true if currently moving.
   */
  virtual bool IsMoving () const = 0;
};

#endif // __CEL_PF_STEER__

