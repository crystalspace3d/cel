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

#ifndef __CEL_PF_WHEELED__
#define __CEL_PF_WHEELED__

#include "cstypes.h"
#include "csutil/scf.h"

/**
 * This is a property class to make and control wheeled vehicles, eg bikes, cars, tanks.
 * <p>
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * <ul>
 * <li>pcvehicle_print: a message has been printed (message)
 * </ul>
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>Print: parameters 'message' (string).
 * </ul>
 * <p>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * <ul>
 * <li>counter (long, read/write): how many times something has been printed.
 * <li>max (long, read/write): maximum length of what was printed.
 * </ul>
 */

struct iCelEntity;
struct iBodyGroup;
struct iPcWheeled : public virtual iBase

#define CEL_WHEELED_CAR_FRONT_STEER 1
#define CEL_WHEELED_CAR_REAR_STEER 2
#define CEL_WHEELED_TANK_LEFT_STEER 3
#define CEL_WHEELED_TANK_RIGHT_STEER 4
#define CEL_WHEELED_NO_STEER 5

{
  SCF_INTERFACE (iPcWheeled, 0, 0, 1);

  /**
   * Print a message.
   */
  virtual void Print (const char* msg) = 0;

 /** Initialise the vehicle. This should be called after the mesh of the entity has been set.
  *  Centreoffset is the offset used on the body's collider.
  */
  virtual void Initialise(csVector3 centreoffset) = 0;

 /** If Initialise is called without an offset, it is assumed that the centre of the mesh
  *   is at the bottom of the vehicle. This makes it stable, compared to the middle of
  *   the mesh.
  */
  virtual void Initialise() = 0;
 /**
  * Add a wheel to the vehicle.
  */
 virtual int AddWheel(csRef<iCelEntity> wheelentity, int steeringmode, bool powered) = 0;

 /**
  * Remove a wheel from the vehicle.
  */
 virtual void RemoveWheel(int wheelnum) = 0;

 /**
  * Accelerate the vehicle.
  */
  virtual void Accelerate() = 0;

 /**
  * Brake the vehicle.
  */
  virtual void Brake() = 0;

 /**
  * Apply the handbrake (rear wheel lock).
  */
  virtual void HandBrake() = 0;

 /**
  * Steer the vehicle left.
  */
  virtual void SteerLeft() = 0;

 /**
  * Steer the vehicle right.
  */
  virtual void SteerRight() = 0;

 /**
  * Reverse the vehicle.
  */
  virtual void Reverse() = 0;

 /**
  * Set whether the vehicle has an automatic transmission, or gears will be manually controlled.
  */
  virtual void SetAutoTransmission(bool auto) = 0;

 /**
  * Change the vehicle's gear.
  */
  virtual void SetGear(int gear) = 0;

 /**
  * Set up a gear with a given velocity and force.
  */
  virtual void SetGearSettings(int gear,float velocity, float force) = 0;

 /**
  * Set the number of gears the vehicle has.
  */
  virtual void SetNumberGears(int number) = 0;

 /**
  * Get the BodyGroup that the car belongs too.
  */
  virtual iBodyGroup* GetBodyGroup() = 0;
};

#endif // __CEL_PF_WHEELED__

