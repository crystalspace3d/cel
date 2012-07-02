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
 * This is a mover property class. It works closely with pclinmove and
 * pcactormove in order to move an object from one position to another
 * while checking collision detection along the way.
 *
 * This property class can send out the following messages:
 * - 'cel.move.impossible' (old 'pcmover_impossible'):
 *   don't even start the move: impossible. This message will have a
 *   'meshname' parameter containing the name of the mesh that was preventing the move.
 * - 'cel.move.arrived' (old 'pcmover_arrived'): arrived at final position.
 * - 'cel.move.interrupted' (old 'pcmover_interrupted'): movement has been interrupted.
 *
 * This property class supports the following actions (add prefix 'cel.mover.action.'
 * if you want to access this action through a message):
 * - MoveTo: parameters 'sectorname' (string), 'position' (vector3),
 *     ,'sqradius' (float) and optional 'checklos' (bool, default false).
 * - Start: parameters 'sectorname' (string), 'position' (vector3),
 *     'up' (vector3), and 'sqradius' (float). This action is deprecated,
 *     you should use MoveTo instead.
 * - Interrupt: interrupt the current movement.
 *
 * This property class supports the following properties:
 * - position (vector3, read only): current end position.
 * - sqradius (float, read/write): current squared radius.
 * - moving (bool, read only): returns true if currently moving.
 */
struct iPcMover : public virtual iBase
{
  SCF_INTERFACE (iPcMover, 1, 1, 0);

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
   * \param sector is the desired sector to move to.
   * \param position is the desired position to move to.
   * \param up is the up vector (used for rotation).
   * \param sqradius if the linmove ends up within the given squared
   * radius of the desired position the movement will stop and be considered
   * sucessful.
   * \deprecated Use MoveTo instead.
   */
  CS_DEPRECATED_METHOD virtual bool Start (iSector* sector, 
	const csVector3& position, const csVector3& up, float sqradius) = 0;

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
  virtual bool MoveTo (iSector* sector, const csVector3& position,
	float sqradius, bool checklos = false) = 0;

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
   * \deprecated Handling of up vector by pcmover is deprecated.
   */
  CS_DEPRECATED_METHOD virtual const csVector3& GetUp () const = 0;

  /**
   * Get the current squared radius.
   */
  virtual float GetSqRadius () const = 0;

  /**
   * Return true if currently moving.
   */
  virtual bool IsMoving () const = 0;

  /**
   * Return true if the rotations used during the movement will be smooth and
   * false if they will be sudden.
   */
  virtual bool IsSmoothMovement () const = 0;

  /**
   * Define if the movement will have sudden of smooth turns. Having sudden turns will
   * ensure the movement is done in straight lines.
   */
  virtual void SetSmoothMovement (bool smooth) = 0;
};

#endif // __CEL_PF_MOVER__

