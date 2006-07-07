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
struct iMeshFactoryWrapper;
struct iODEHinge2Joint;
struct iRigidBody;

struct celWheel
{
  csRef<iODEHinge2Joint> WheelJoint;
  csRef<iRigidBody> RigidBody;
  csVector3 Position;
  float LeftSteerOffset;
  float RightSteerOffset;
  float TurnSpeed;
  float ReturnSpeed;
  float EnginePower;
  float BrakePower;
  bool SteerInverted;
};

struct iPcWheeled : public virtual iBase

{
  SCF_INTERFACE (iPcWheeled, 0, 0, 1);

  /**
   * Print a message.
   */
  virtual void Print (const char* msg) = 0;

 /** 
  *  Initialise the vehicle. This should be called after the mesh of the entity has been set.
  */
  virtual void Initialise() = 0;

 /** 
  *  Set the mesh to use for the wheels.
  */
  virtual void SetWheelMesh(const char* file, const char* factname) = 0;

 /**
  * Set if tank steering is enabled vehicle.
  */
  virtual void SetTankMode(bool tankmode) = 0;

 /**
  * Get if tank mode is enabled
  */
  virtual int GetTankMode() = 0;

 /**
  * Add a wheel to the vehicle.
  */
 virtual int AddWheel(csVector3 position) = 0;

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
  * Set the tightness of the vehicle's turning.
  */
  virtual void SetSteerAmount(float amount) = 0;

 /**
  * Set the force at which the vehicle brakes.
  */
  virtual void SetBrakeForce(float force) = 0;

 /**
  * Get the tightness of the vehicle's turning.
  */
  virtual float GetSteerAmount() = 0;

 /**
  * Steer the vehicle left.
  */
  virtual void SteerLeft() = 0;

 /**
  * Steer the vehicle right.
  */
  virtual void SteerRight() = 0;

 /**
  * Straighten the Steering wheels
  */
  virtual void SteerStraight() = 0;

 /**
  * Reverse the vehicle.
  */
  virtual void Reverse() = 0;

 /**
  * Call this to start unpowered rolling.
  */
  virtual void Roll() = 0;

 /**
  * Set whether the vehicle has an automatic transmission, or gears will be manually controlled.
  */
  virtual void SetAutoTransmission(bool auto) = 0;

 /**
  * Change the vehicle's gear.
  */
  virtual void SetGear(int gear) = 0;

 /**
  * Get the vehicle's gear.
  */
  virtual int GetGear() = 0;

 /**
  * Set up a gear with a given velocity and force.
  */
  virtual void SetGearSettings(int gear,float velocity, float force) = 0;

 /**
  * Set the number of gears the vehicle has.
  */
  virtual void SetNumberGears(int number) = 0;

 /**
  * Get the number of gears the vehicle has.
  */
  virtual size_t GetNumberGears() = 0;

 /**
  * Get the BodyGroup that the car belongs too.
  */
  virtual iBodyGroup* GetBodyGroup() = 0;
};

#endif // __CEL_PF_WHEELED__

