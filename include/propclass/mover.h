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

#ifndef __CEL_PF_MOVER__
#define __CEL_PF_MOVER__

#include "cstypes.h"
#include "csutil/scf.h"

struct iPcLinearMovement;

/**
 * This is a mover property class. It works closely with pclinmove in order
 * to move an object from one position to another while checking collision
 * detection along the way.
 *
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * - pcmover_impossible: don't even start the move: impossible.
 * - pcmover_stuck: can't move further.
 * - pcmover_arrived: arrived at final position.
 * - pcmover_interrupted: movement has been interrupted.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - Start: parameters 'sectorname' (string), 'position' (vector3),
 *     'up' (vector3), 'movespeed' (float), 'rotatespeed' (float),
 *     and 'sqradius' (float).
 * - Interrupt: interrupt the current movement.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - position (vector3, read only): current end position.
 * - up (vector3, read only): current up vector.
 * - movespeed (float, read only): current movement speed.
 * - rotatespeed (float, read only): current rotation speed.
 * - sqradius (float, read/write): current squared radius.
 * - moving (bool, read only): returns true if currently moving.
 */
struct iPcMover : public virtual iBase
{
  SCF_INTERFACE (iPcMover, 0, 0, 1);

  /**
   * Start moving. When you call this function this property class will
   * attempt to move the linmove to the correct position. If it fails the
   * behaviour will get a 'pcmover_stuck' message. Otherwise it will get
   * a 'pcmover_arrived' message. If this function detects that it is
   * completely impossible to move there in advance then the behaviour
   * will get a 'pcmover_impossible' message and this function will return
   * false then. If this property class was already controlling a movement
   * then that movement will be interrupted (possibly giving a
   * pcmover_interrupted message).
   * \param sector is the desired sector to move too.
   * \param position is the desired position to move too.
   * \param up is the up vector (used for rotation).
   * \param movespeed is the speed of movement.
   * \param rotatespeed is the speed of rotation.
   * \param sqradius if the linmove ends up within the given squared
   * radius of the desired position the movement will stop and be considered
   * sucessful.
   */
  virtual bool Start (iSector* sector, const csVector3& position,
	const csVector3& up, float movespeed, float rotatespeed,
	float sqradius) = 0;

  /**
   * Interrupt a movement. The behaviour will get a 'pcmover_interrupted'
   * message if the mover was really moving. Otherwise nothing happens.
   */
  virtual void Interrupt () = 0;

  /**
   * Get the end sector that we want to move too.
   */
  virtual iSector* GetSector () const = 0;

  /**
   * Get the end position that we want to move too.
   */
  virtual const csVector3& GetPosition () const = 0;

  /**
   * Get the current up vector.
   */
  virtual const csVector3& GetUp () const = 0;

  /**
   * Get the current movement speed.
   */
  virtual float GetMoveSpeed () const = 0;

  /**
   * Get the current rotation speed.
   */
  virtual float GetRotateSpeed () const = 0;

  /**
   * Get the current squared radius.
   */
  virtual float GetSqRadius () const = 0;

  /**
   * Return true if currently moving.
   */
  virtual bool IsMoving () const = 0;
};

#endif // __CEL_PF_MOVER__

