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
#include "csgeom/matrix3.h"

/**
 * This is a property class to make and control wheeled vehicles, eg bikes,
 * cars, tanks.
 *
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * - pcwheeled_collision: an entity has collided with this entity.
 *   Parameters are 'otherbody' (string: entity name), 'position' (vector3:
 *   point of collision, 'normal' (vector3: normal of collision),
 *   'depth' (float: penetration depth), and 'index' (int: index of colliding
 *   wheel).
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - SetWheelMesh: parameters 'meshfact' (string), 'meshfile' (string).
 * - SetTankMode: parameter 'tankmode' (bool).
 * - AddWheelAuto: parameters 'position' (vector3), 'rotation' (vector3),
 *                 'meshfact' (string), 'meshfile' (string).
 * - AddWheel: parameters 'position' (vector3), 'turnspeed' (float),
 *                 'returnspeed' (float), 'suspensionsoftness' (float),
 *                 'suspensiondamping' (float), 'brakepower' (float),
 *                 'enginepower' (float), 'leftsteersensitivity' (float),
 *                 'rightsteersensitivity' (float), 'friction' (float),
 *                 'mass' (float), 'handbrakeaffected' (bool),
 *                 'steerinverted' (bool), 'rotation' (vector3),
 *                 'meshfact' (string), 'meshfile' (string).
 * - DeleteWheel: parameter 'wheelnum' (long).
 * - DeleteAllWheels : no parameters.
 * - DestroyWheel: parameter 'wheelnum' (long).
 * - DestroyAllWheels : no parameters.
 * - RestoreWheel: parameter 'wheelnum' (long).
 * - RestoreAllWheels : no parameters.
 * - Accelerate : parameter 'applied' (bool).
 * - Brake : parameter 'applied' (bool).
 * - Handbrake : parameter 'applied' (bool).
 * - SetSteerAmount : parameter 'steeramount' (float).
 * - SteerLeft : parameter 'steeramount' (float).
 * - SteerRight : parameter 'steeramount' (float).
 * - SteerStraight : no parameters.
 * - Reverse : no parameters.
 * - Neutral : no parameters.
 * - SetAutoTransmission : parameter 'autotransmission' (bool)
 * - SetGear : parameter 'gear' (bool)
 * - SetGearSettings : parameters 'force' (float) 'velocity' : float
 *                   'gear' (long).
 * - SetBrakeForce : parameter 'brakeforce' (float).
 * - SetAutoReverse : parameter 'autoreverse' (bool).
 * - SetFrontWheelPreset : parameter 'steersensitivity' (float),
 *                   'enginepower' (float), 'suspensionsoftness' (float),
 *                   'suspensiondamping' (float), 'friction' (float),
 *                   'mass' (float).
 * - SetRearWheelPreset : parameter 'steersensitivity' (float),
 *                   'enginepower' (float), 'suspensionsoftness' (float),
 *                   'suspensiondamping' (float), 'friction' (float),
 *                   'mass' (float).
 * - SetOuterWheelSteerPreset : parameter 'steersensitivity' (float).
 * - SetWheelPosition : parameters 'wheelnum' (long), 'position' (vector3).
 * - SetWheelRotation : parameters 'wheelnum' (long), 'rotation' (vector3).
 * - SetWheelSuspensionSoftness : parameters 'wheelnum' (long),
 *                   'suspensionsoftness' (float).
 * - SetWheelSuspensionDamping : parameters 'wheelnum' (long),
 *                   'suspensiondamping' (float).
 * - SetWheelLeftSteerSensitivity : parameters 'wheelnum' (long),
 *                   'steersensitivity' (float).
 * - SetWheelRightSteerSensitivity : parameters 'wheelnum' (long),
 *                   'suspensionsoftness' (float).
 * - SetWheelFriction : parameters 'wheelnum' (long), 'friction' (float).
 * - SetWheelMass : parameters 'wheelnum' (long), 'mass' (float).
 * - SetWheelTurnSpeed : parameters 'wheelnum' (long), 'turnspeed' (float).
 * - SetWheelReturnSpeed : parameters 'wheelnum' (long), 'returnspeed' (float).
 * - SetWheelEnginePower : parameters 'wheelnum' (long), 'enginepower' (float).
 * - SetWheelBrakePower : parameters 'wheelnum' (long), 'brakepower' (float).
 * - SetWheelSteerInverted : parameters 'wheelnum' (long),
 *                   'steerinverted' (bool).
 * - SetWheelHandbrakeAffected : parameters 'wheelnum' (long),
 *                   'handbrakeaffected' (bool).
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - speed (float, read): the current speed of the vehicle.
 */

struct iCelEntity;
struct iBodyGroup;
struct iMeshFactoryWrapper;
struct iODEHinge2Joint;
struct iRigidBody;

struct iPcWheeled : public virtual iBase
{
  SCF_INTERFACE (iPcWheeled, 0, 0, 1);

  /**
   *  Set the mesh to use for the wheels when not specified. It is expected
   *  to be a left wheel. For right wheels it is flipped around.
   *  \param wheelfact The name of the factory to use for wheel meshes.
   *  \param wheelfile VFS path to the file which contains the wheel's factory.
   *    If 0, it is assumed the factory has already been loaded.
   */
  virtual void SetWheelMesh(const char* wheelfact, const char* wheelfile) = 0;

  /**
   *  Set the mesh to use for a wheel.
   *  \param wheelnum Index of the wheel to set.
   *  \param wheelfact The name of the factory to use for wheel meshes.
   *  \param wheelfile VFS path to the file which contains the wheel's factory.
   *    If 0, it is assumed the factory has already been loaded.
   */
  virtual void SetWheelMesh(size_t wheelnum, const char* wheelfact,
      const char* wheelfile) = 0;

  /**
   * Set if tank steering is enabled vehicle.
   *  \param tankmode Whether tank steering is enabled or not.
   */
  virtual void SetTankMode(bool tankmode) = 0;

  /**
   * Get if tank mode is enabled
   */
  virtual bool GetTankMode() = 0;

  /**
   * Add a wheel to the vehicle's wheel layout. This method uses the
   * vehicle's wheel presets for steering, power and suspension, and
   * automatically applies handbrake and invert steering
   *  to rear wheels.
   *  \param wheelfact The name of the factory to use for wheel meshes.
   *  \param wheelfile VFS path to the file which contains the wheel's factory.
   *    If 0, it is assumed the factory has already been loaded.
   *  \param rotation A rotation matrix to set on the wheel's mesh.
   */
  virtual size_t AddWheelAuto(csVector3 position, const char* wheelfact = 0,
     const char* wheelfile = 0,
     csMatrix3 rotation = csMatrix3(0.0f,0.0f,0.0f,0.0f)) = 0;

  /**
   * Add a wheel to the vehicle's wheel layout.
   * This method allows for total control over the wheel, overriding the
   * vehicle's presets.
   * \param position Where the wheel will be placed.
   * \param turnspeed How fast the wheel should steer away from centre.
   * \param returnspeed How fast the wheel should return to the centre.
   * \param suspensionsoftness How soft the suspension on the wheel should be.
   * \param suspensiondamping How dampened the suspension on the wheel should
   * be.
   * \param brakepower Fraction of vehicle's brake force this wheel recieves.
   * \param leftsteersensitivity How sensitive wheel is when steering left.
   * \param rightsteersensitivity How sensitive wheel is when steering right.
   * \param friction The traction of the wheel.
   * \param mass Mass of the wheel, in kilograms.
   * \param handbrakeaffect Wether the handbrake applies to this wheel.
   * \param steerinvert Wether this wheel steers back-to-front.
   *  \param wheelfact The name of the factory to use for wheel meshes.
   *  \param wheelfile VFS path to the file which contains the wheel's factory.
   *    If 0, it is assumed the factory has already been loaded.
   *  \param rotation A rotation matrix to set on the wheel's mesh.
   */
  virtual size_t AddWheel(csVector3 position,
                       float turnspeed, float returnspeed,
                       float suspensionsoftness, float suspensiondamping,
                       float brakepower, float enginepower,
		       float leftsteersensitivity, float friction, float mass,
                       float rightsteersensitivity, bool handbrakeaffect,
                       bool steerinvert, const char* wheelfact = 0, 
                       const char* wheelfile = 0,
                       csMatrix3 rotation = csMatrix3(0.0f,0.0f,0.0f,0.0f)) = 0;

  /**
   * Destroy a wheel on the car, and delete it from the layout. It
   *  \param wheelnum Index of the wheel to delete.
   */
  virtual void DeleteWheel(size_t wheelnum) = 0;

  /**
   * Destroy and delete all wheels in the layout
   */
  virtual void DeleteAllWheels() = 0;

  /**
   * Destroy a wheel on the car. It will remain in the layout, so it can be
   * restored along with all its settings with RestoreWheel(wheelnum).
   */
  virtual void DestroyWheel(size_t wheelnum) = 0;

  /**
   * Destroy all wheels on the car. The layout is still retained,
   * so the wheels can be restored with RestoreWheelLayout().
   */
  virtual void DestroyAllWheels() = 0;

  /**
   * Restore a wheel, along with its settings, from the layout.
   */
  virtual void RestoreWheel(size_t wheelnum) = 0;

  /**
   * Restore all of the vehicle's wheels from the layout.
   */
  virtual void RestoreAllWheels() = 0;

  /**
   * A wheel grouping preset which lowers by the sensitivity of outer wheels,
   * to improve handling.
   * Note that changing this will overwrite per-wheel steer and power
   * settings! You will have to set per-wheel
   * settings after you set all the presets.
   * \param sensitivity factor of sensitivity for outer wheels
   */
  virtual void SetOuterWheelSteerPreset(float sensitivity) = 0;

  /**
   * A wheel grouping preset which sets steering amount and drive power to
   * front wheels. Note that changing this will overwrite per-wheel steer
   * and power settings! You will have to set per-wheel settings after you
   * set all the presets.
   * \param sensitivity sensitivity for front wheels when steering
   * \param power power which front wheels recieve when accelerating (0-1)
   * \param suspensionsoftness How soft the suspension on the wheel should be.
   * \param suspensiondamping How dampened the suspension on the wheel should
   * be.
   * \param friction The traction of the wheel.
   * \param mass Mass of the wheel, in kilograms.
   */
  virtual void SetFrontWheelPreset(float sensitivity,float power,
     float suspensionsoftness, float suspensiondamping, float friction,
     float mass) = 0;

  /**
   * A wheel grouping preset which sets steering amount and drive power to
   * rear wheels. Note that changing this will overwrite per-wheel steer and
   * power settings! You will have to set per-wheel settings after you set
   * all the presets.
   * \param sensitivity sensitivity for rear wheels when steering
   * \param power power which rear wheels recieve when accelerating (0-1)
   * \param suspensionsoftness How soft the suspension on the wheel should be.
   * \param suspensiondamping How dampened the suspension on the wheel
   * should be.
   * \param friction The traction of the wheel.
   * \param mass Mass of the wheel, in kilograms.
   */
  virtual void SetRearWheelPreset(float sensitivity,float power,
     float suspensionsoftness, float suspensiondamping, float friction,
     float mass) = 0;

  /**
   * Set wether the vehicle will accelerate.
   * \param applied Whether the accelerator is applied.
   */
  virtual void Accelerate(bool applied) = 0;

  /**
   * Get whether the vehicle is accelerating.
   */
  virtual bool IsAccelerating() = 0;

  /**
   * Set wether the vehicle will brake.
   * \param applied Wether the brake is applied
   */
  virtual void Brake(bool applied) = 0;

  /**
   * Get whether the vehicle is brakeing
   */
  virtual bool IsBraking() = 0;

  /**
   * Set wether the vehicle will handbrake. (rear wheel lock)
   * \param applied Wether the handbrake is applied
   */
  virtual void Handbrake(bool applied) = 0;

  /**
   * Get whether the vehicle is handbrakeing.
   */
  virtual bool IsHandbraking() = 0;


  /**
   * Set the tightness of the vehicle's turning.
   * \param amount How tight the car will steer.
   */
  virtual void SetSteerAmount(float amount) = 0;

  /**
   * Set the force at which the vehicle brakes.
   * \param force The force with which the vehicle will brake
   */
  virtual void SetBrakeForce(float force) = 0;

  /**
   * Get the tightness of the vehicle's turning.
   */
  virtual float GetSteerAmount() = 0;

  /**
   * Get whether brake is applied
   */
  virtual bool GetBrakeApplied() = 0;

  /**
   * Get whether handbrake is applied
   */
  virtual bool GetHandbrakeApplied() = 0;

  /**
   * Get the forward velocity of the vehicle.
   */
  virtual float GetSpeed() = 0;

  /**
   * Steer the vehicle left.
   * \param amount Tightness of the steering
   */
  virtual void SteerLeft(float amount) = 0;

  /**
   * Steer the vehicle right.
   * \param amount Tightness of the steering
   */
  virtual void SteerRight(float amount) = 0;

  /**
   * Steer the vehicle left. Uses the amount of steer set in SetSteerAmount.
   */
  virtual void SteerLeft() = 0;

  /**
   * Steer the vehicle right. Uses the amount of steer set in SetSteerAmount.
   */
  virtual void SteerRight() = 0;

  /**
   * Straighten the steering wheels
   */
  virtual void SteerStraight() = 0;

  /**
   * Reverse the vehicle. This is the same as setting the gear to -1.
   */
  virtual void Reverse() = 0;

  /**
   * Call this to put the car in neutral gear. This is the same as setting
   * gear to 0.
   */
  virtual void Neutral() = 0;

  /**
   * Set whether the vehicle has an automatic transmission, or gears will be
   * manually controlled.
   * \param auto Whether automatic transmission is enabled or not.
   */
  virtual void SetAutoTransmission(bool auto) = 0;

  /**
   * Change the vehicle's gear.
   * \param gear The gear to switch to. -1 is reverse, 0 is neutral, anything
   * above are forwards gears.
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
   * Get the highest gear the vehicle has.
   */
  virtual int GetTopGear() = 0;

  /**
   * Set wether the vehicle will automatically begin reversing after braking
   * to a stop. This is enabled by default.
   * \param autoreverse. Whether the vehicle will automatically reverse.
   */
  virtual void SetAutoReverse(bool autoreverse) = 0;

  /**
   * Get the BodyGroup that the car belongs too.
   */
  virtual iBodyGroup* GetBodyGroup() = 0;

  /**
   * Set the position of a wheel.
   * \param wheelnum Index of the wheel to set.
   * \param position The new position of the wheel.
   */
  virtual void SetWheelPosition(size_t wheelnum, csVector3 position) = 0;

  /**
   * Set the rotation of a wheel.
   * \param wheelnum Index of the wheel to set.
   * \param rotation The new rotation of the wheel.
   */
  virtual void SetWheelRotation(size_t wheelnum, csMatrix3 rotation) = 0;

  /**
   * Set the softness of a wheel's suspension. It is initially 0.000125.
   * \param wheelnum Index of the wheel to set.
   * \param softness Softness of the suspension, from 0 to 1.
   */
  virtual void SetWheelSuspensionSoftness(size_t wheelnum, float softness) = 0;

  /**
   * Set the damping of a wheel's suspension. It is initially 0.125.
   * \param wheelnum Index of the wheel to set.
   * \param damping Damping of the suspension, from 0 to 1.
   */
  virtual void SetWheelSuspensionDamping(size_t wheelnum, float damping) = 0;

  /**
   * Set the sensitivity of a wheel when steering left.
   * \param wheelnum Index of the wheel to set.
   * \param sensitivity Sensitivity of the steering, from 0 to 1.
   */
  virtual void SetWheelLeftSteerSensitivity(size_t wheelnum,
      float sensitivity) = 0;

  /**
   * Set the sensitivity of a wheel when steering right.
   * \param wheelnum Index of the wheel to set.
   * \param sensitivity Sensitivity of the steering, from 0 to 1.
   */
  virtual void SetWheelRightSteerSensitivity(size_t wheelnum,
      float sensitivity) = 0;

  /**
   * Set the friction of a wheel. The default is 0.7.
   * \param wheelnum Index of the wheel to set.
   * \param friction Friction of the wheel.
   */
  virtual void SetWheelFriction(size_t wheelnum,
      float friction) = 0;

  /**
   * Set the mass of a wheel. The default is 10.0.
   * \param wheelnum Index of the wheel to set.
   * \param mass Mass of the wheel.
   */
  virtual void SetWheelMass(size_t wheelnum,
      float mass) = 0;

  /**
   * Set the speed at which a wheel turns away from the middle. This is
   * initially 2.
   * \param wheelnum Index of the wheel to set.
   * \param speed Speed at which the wheel turns.
   */
  virtual void SetWheelTurnSpeed(size_t wheelnum, float speed) = 0;

  /**
   * Set the speed at which a wheel returns to the middle. This is initially 2.
   * \param wheelnum Index of the wheel to set.
   * \param speed Speed at which the wheel returns.
   */
  virtual void SetWheelReturnSpeed(size_t wheelnum, float speed) = 0;

  /**
   * Set amount of power from the engine which goes to a wheel when
   * accelerating. This is initially 1.
   * \param wheelnum Index of the wheel to set.
   * \param power Amount of power going to the wheel, from 0 to 1.
   */
  virtual void SetWheelEnginePower(size_t wheelnum, float power) = 0;

  /**
   * Set amount of power from the brakes which goes to a wheel. This is
   * initially 1.
   * \param wheelnum Index of the wheel to set.
   * \param power Amount of brake power going to the wheel, from 0 to 1.
   */
  virtual void SetWheelBrakePower(size_t wheelnum, float power) = 0;

  /**
   * Set whether a wheel steers in the opposite direction. This is initially
   * automatically determined from it's position, where rear wheels are
   * inverted.
   * \param wheelnum Index of the wheel to set.
   * \param inverted Wether the wheel is inverted.
   */
  virtual void SetWheelSteerInverted(size_t wheelnum, bool inverted) = 0;

  /**
   * Set whether the handbrake applies to a wheel. This is initially
   * automatically determined from it's position, where rear wheels are
   * affected by the handbrake
   * \param wheelnum Index of the wheel to set.
   * \param handbrakeaffected Whether the handbrake affects the wheel.
   */
  virtual void SetWheelHandbrakeAffected(size_t wheelnum,
      bool handbrakeaffected) = 0;

  /**
   * Get the position of a wheel.
   * \param wheelnum Index of the wheel to get.
   */
  virtual csVector3 GetWheelPosition(size_t wheelnum) = 0;

  /**
   * Get the rotation of a wheel.
   * \param wheelnum Index of the wheel to get.
   */
  virtual csMatrix3 GetWheelRotation(size_t wheelnum) = 0;

  /**
   * Get the softness of a wheel's suspension.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelSuspensionSoftness(size_t wheelnum) = 0;

  /**
   * Get the damping of a wheel's suspension.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelSuspensionDamping(size_t wheelnum) = 0;

  /**
   * Get the sensitivity of a wheel when steering left.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelLeftSteerSensitivity(size_t wheelnum) = 0;

  /**
   * Get the sensitivity of a wheel when steering right.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelRightSteerSensitivity(size_t wheelnum) = 0;

  /**
   * Get the friction of the wheel.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelFriction(size_t wheelnum) = 0;

  /**
   * Get the mass of the wheel.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelMass(size_t wheelnum) = 0;

  /**
   * Get the speed at which a wheel turns away from the middle.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelTurnSpeed(size_t wheelnum) = 0;

  /**
   * Get the speed at which a wheel returns to the middle.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelReturnSpeed(size_t wheelnum) = 0;

  /**
   * Get the amount of power from the engine which goes to a wheel when
   * accelerating.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelEnginePower(size_t wheelnum) = 0;

  /**
   * Get the amount of power from the brakes which goes to a wheel.
   * \param wheelnum Index of the wheel to get.
   */
  virtual float GetWheelBrakePower(size_t wheelnum) = 0;

  /**
   * Get whether a wheel steers in the opposite direction.
   * \param wheelnum Index of the wheel to get.
   */
  virtual bool GetWheelSteerInverted(size_t wheelnum) = 0;

  /**
   * Get whether the handbrake applies to a wheel.
   * \param wheelnum Index of the wheel to get.
   */
  virtual bool GetWheelHandbrakeAffected(size_t wheelnum) = 0;

  /**
   * Get the number of wheels on this vehicle
   */
  virtual size_t GetWheelCount() = 0;

  /**
   * Get whether the rigid body of this wheel
   * \param wheelnum Index of the wheel to get.
   */
  virtual iRigidBody* GetWheelBody(size_t wheelnum) = 0;

  /**
   * Get whether the joint connecting this wheel
   * \param wheelnum Index of the wheel to get.
   */
  virtual iODEHinge2Joint* GetWheelJoint(size_t wheelnum) = 0;

  /**
   * Set whether to send messages back to the entity when wheels collide with
   * other bodies.
   * \param en Wether callback is enabled
   */
  virtual void SetCollisionCallbackEnabled (bool en) = 0;

  /**
   * Get whether to send messages back to the entity when wheels collide with
   * other bodies. Is true by default.
   */
  virtual bool IsCollisionCallbackEnabled () const = 0;
};

#endif // __CEL_PF_WHEELED__

