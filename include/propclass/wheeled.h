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


struct iPcWheeled : public virtual iBase

{
  SCF_INTERFACE (iPcWheeled, 0, 0, 1);

 /** 
   *  Set the mesh to use for the wheels.
   *  @param file VFS path to the file which contains the wheel's factory. If 0, it is assumed the factory has already been loaded.
   *  @param factname The name of the factory to use for wheel meshes.
  */
  virtual void SetWheelMesh(const char* file, const char* factname) = 0;

 /**
   * Set if tank steering is enabled vehicle.
   *  @param tankmode Whether tank steering is enabled or not.
  */
  virtual void SetTankMode(bool tankmode) = 0;

 /**
   * Get if tank mode is enabled
  */
  virtual int GetTankMode() = 0;

 /**
   * Add a wheel to the vehicle's wheel layout. This method uses sensible defaults and the vehicle's
     wheel presets for steering and power, and automatically applies handbrake and invert steering
     to rear wheels. Therefore it is enough to just give the wheel's position. 
     The wheel won't be created until SetupWheels() is called.
   *  @param position Where the wheel will be placed.
   *  @param turnspeed How fast the wheel should steer away from centre.
   *  @param returnspeed How fast the wheel should return to the centre.
   *  @param suspensionsoftness How soft the suspension on the wheel should be.
   *  @param suspensiondamping How dampened the suspension on the wheel should be.
   *  @param brakepower Fraction of vehicle's brake force this wheel recieves.
  */
  virtual int AddWheelAuto(csVector3 position,float turnspeed=2.0f, float returnspeed=2.0f,
                       float suspensionsoftness=0.000125f, float suspensiondamping=0.125f,
                       float brakepower=1.0f) = 0;

 /**
   * Add a wheel to the vehicle's wheel layout.
     This method allows for total control over the wheel, overriding the vehicle's presets.
     The wheel won't be created until SetupWheels() is called.
   *  @param position Where the wheel will be placed.
   *  @param turnspeed How fast the wheel should steer away from centre.
   *  @param returnspeed How fast the wheel should return to the centre.
   *  @param suspensionsoftness How soft the suspension on the wheel should be.
   *  @param suspensiondamping How dampened the suspension on the wheel should be.
   *  @param brakepower Fraction of vehicle's brake force this wheel recieves.
   *  @param leftsteersensitivity How sensitive wheel is when steering left.
   *  @param rightsteersensitivity How sensitive wheel is when steering right.
   *  @param handbrakeaffect Wether the handbrake applies to this wheel.
   *  @param steerinvert Wether this wheel steers back-to-front.
  */
  virtual int AddWheel(csVector3 position,float turnspeed, float returnspeed,
                       float suspensionsoftness, float suspensiondamping,
                       float brakepower, float enginepower, float leftsteersensitivity,
                       float rightsteersensitivity, bool handbrakeaffect,
                       bool steerinvert) = 0;

 /**
   * Destroy a wheel on the car, and delete it from the layout. It
   *  @param wheelnum Index of the wheel to delete.
  */
  virtual void DeleteWheel(int wheelnum) = 0;

 /**
   * Destry and delete all wheels in the layout
  */
  virtual void DeleteAllWheels() = 0;

 /**
   * Destroy a wheel on the car. It will remain in the layout, so it can be restored along with all its
     settings with RestoreWheel(wheelnum).
  */
  virtual void DestroyWheel(int wheelnum) = 0;

 /**
   * Destroy all wheels on the car. The layout is still retained, so the wheels can be restored with RestoreWheelLayout().
  */
  virtual void DestroyAllWheels() = 0;

 /**
   * Restore a wheel, along with its settings, from the layout.
  */
  virtual void RestoreWheel(int wheelnum) = 0;

 /**
   * Restore all of the vehicle's wheels from the layout.
  */
  virtual void RestoreAllWheels() = 0;

 /**
   * A wheel grouping preset which lowers by the sensitivity of outer wheels, to improve handling.
   * Note that changing this will overwrite per-wheel steer and power settings! You will have to set per-wheel
   * settings after you set all the presets.
   *  @param sensitivity factor of sensitivity for outer wheels
  */
  virtual void SetOuterWheelSteerPreset(float sensitivity) = 0;

 /**
   * A wheel grouping preset which sets steering amount and drive power to front wheels. 
   * Note that changing this will overwrite per-wheel steer and power settings! You will have to set per-wheel
   * settings after you set all the presets.
   *  @param sensitivity sensitivity for front wheels when steering
   *  @param power power which front wheels recieve when accelerating (0-1)
  */
  virtual void SetFrontWheelPreset(float sensitivity,float power) = 0;

  /** A wheel grouping preset which sets steering amount and drive power to rear wheels.
   * Note that changing this will overwrite per-wheel steer and power settings! You will have to set per-wheel
   * settings after you set all the presets.
   *  @param sensitivity sensitivity for rear wheels when steering
   *  @param power power which rear wheels recieve when accelerating (0-1)
  */
  virtual void SetRearWheelPreset(float sensitivity,float power) = 0;

 /**
   * A wheel preset to reset power and steering of all wheels. Defaults back to front-wheel steer, 4 wheel drive.
  */
  virtual void ClearWheelPresets() = 0;

 /**
   * Set wether the vehicle will accelerate.
   *  @param applied Whether the accelerator is applied.
  */
  virtual void Accelerate(bool applied) = 0;

 /**
   * Get whether the vehicle is accelerating.
  */
  virtual bool IsAccelerating() = 0;

 /**
   * Set wether the vehicle will brake.
   *  @param applied Wether the brake is applied
  */
  virtual void Brake(bool applied) = 0;

 /**
   * Get whether the vehicle is brakeing
  */
  virtual bool IsBraking() = 0;

 /**
   * Set wether the vehicle will handbrake. (rear wheel lock)
   *  @param applied Wether the handbrake is applied
  */
  virtual void Handbrake(bool applied) = 0;

 /**
   * Get whether the vehicle is handbrakeing.
  */
  virtual bool IsHandbraking() = 0;


 /**
   * Set the tightness of the vehicle's turning.
   *  @param amount How tight the car will steer.
  */
  virtual void SetSteerAmount(float amount) = 0;

 /**
   * Set the force at which the vehicle brakes.
   *  @param force The force with which the vehicle will brake
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
   * Straighten the steering wheels
  */
  virtual void SteerStraight() = 0;

 /**
   * Reverse the vehicle. This is the same as setting the gear to -1.
  */
  virtual void Reverse() = 0;

 /**
   * Call this to put the car in neutral gear. This is the same as setting gear to 0.
  */
  virtual void Neutral() = 0;

 /**
   * Set whether the vehicle has an automatic transmission, or gears will be manually controlled.
   *  @param auto Whether automatic transmission is enabled or not.
  */
  virtual void SetAutoTransmission(bool auto) = 0;

 /**
   * Change the vehicle's gear.
   *  @param gear The gear to switch to. -1 is reverse, 0 is neutral, anything above are forwards gears.
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
   * Set the number of forwards gears the vehicle has. This excludes neutral and reverse.
   *  @param number The number of gears the vehicle will have.
  */
  virtual void SetNumberGears(int number) = 0;

 /**
   * Get the number of forwards gears the vehicle has. This excludes neutral and reverse.
  */
  virtual size_t GetNumberGears() = 0;

 /**
   * Set wether the vehicle will automatically begin reversing after braking to a stop. This is enabled by default.
   *  @param autoreverse. Whether the vehicle will automatically reverse.
  */
  virtual void SetAutoReverse(bool autoreverse) = 0;

 /**
   * Get the BodyGroup that the car belongs too.
  */
  virtual iBodyGroup* GetBodyGroup() = 0;

 /**
   * Set the position of a wheel.
   *  @param wheelnum Index of the wheel to set.
   *  @param position The new position of the wheel.
  */
  virtual void SetWheelPosition(int wheelnum, csVector3 position) = 0;

 /**
   * Set the softness of a wheel's suspension. It is initially 0.000125.
   *  @param wheelnum Index of the wheel to set.
   *  @param softness Softness of the suspension, from 0 to 1.
  */
  virtual void SetWheelSuspensionSoftness(int wheelnum, float softness) = 0;

 /**
   * Set the damping of a wheel's suspension. It is initially 0.125.
   *  @param wheelnum Index of the wheel to set.
   *  @param damping Damping of the suspension, from 0 to 1.
  */
  virtual void SetWheelSuspensionDamping(int wheelnum, float damping) = 0;

 /**
   * Set the sensitivity of a wheel when steering left.This is initially automatically determined from the wheels position. If it is an outer wheel when steering left, it is decreased. If it is a back wheel it is decreased further.
   *  @param wheelnum Index of the wheel to set.
   *  @param sensitivity Sensitivity of the steering, from 0 to 1.
  */
  virtual void SetWheelLeftSteerSensitivity(int wheelnum, float sensitivity) = 0;

 /**
   * Set the sensitivity of a wheel when steering right. This is initially automatically determined from the wheels position. If it is an outer wheel when steering right, it is decreased. If it is a back wheel it is decreased further.
   *  @param wheelnum Index of the wheel to set.
   *  @param sensitivity Sensitivity of the steering, from 0 to 1.
  */
  virtual void SetWheelRightSteerSensitivity(int wheelnum, float sensitivity) = 0;

 /**
   * Set the speed at which a wheel turns away from the middle. This is initially 2.
   *  @param wheelnum Index of the wheel to set.
   *  @param speed Speed at which the wheel turns.
  */
  virtual void SetWheelTurnSpeed(int wheelnum, float speed) = 0;

 /**
   * Set the speed at which a wheel returns to the middle. This is initially 2.
   *  @param wheelnum Index of the wheel to set.
   *  @param speed Speed at which the wheel returns.
  */
  virtual void SetWheelReturnSpeed(int wheelnum, float speed) = 0;

 /**
   * Set amount of power from the engine which goes to a wheel when accelerating. This is initially 1.
   *  @param wheelnum Index of the wheel to set.
   *  @param power Amount of power going to the wheel, from 0 to 1.
  */
  virtual void SetWheelEnginePower(int wheelnum, float power) = 0;

 /**
   * Set amount of power from the brakes which goes to a wheel. This is initially 1.
   *  @param wheelnum Index of the wheel to set.
   *  @param power Amount of brake power going to the wheel, from 0 to 1.
  */
  virtual void SetWheelBrakePower(int wheelnum, float power) = 0;

 /**
   * Set whether a wheel steers in the opposite direction. This is initially automatically determined from it's position, where rear wheels are inverted.
   *  @param wheelnum Index of the wheel to set.
   *  @param inverted Wether the wheel is inverted.
  */
  virtual void SetWheelSteerInverted(int wheelnum, bool inverted) = 0;

 /**
   * Set whether the handbrake applies to a wheel. This is initially automatically determined from it's position, where rear wheels are affected by the handbrake
   *  @param wheelnum Index of the wheel to set.
   *  @param handbrakeaffected Whether the handbrake affects the wheel.
  */
  virtual void SetWheelHandbrakeAffected(int wheelnum, bool handbrakeaffected) = 0;

 /**
   * Get the position of a wheel.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual csVector3 GetWheelPosition(int wheelnum) = 0;

 /**
   * Get the softness of a wheel's suspension.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual float GetWheelSuspensionSoftness(int wheelnum) = 0;

 /**
   * Get the damping of a wheel's suspension.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual float GetWheelSuspensionDamping(int wheelnum) = 0;

 /**
   *  Get the sensitivity of a wheel when steering left.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual float GetWheelLeftSteerSensitivity(int wheelnum) = 0;

 /**
     * Get the sensitivity of a wheel when steering right.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual float GetWheelRightSteerSensitivity(int wheelnum) = 0;

 /**
   * Get the speed at which a wheel turns away from the middle.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual float GetWheelTurnSpeed(int wheelnum) = 0;

 /**
   * Get the speed at which a wheel returns to the middle.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual float GetWheelReturnSpeed(int wheelnum) = 0;

 /**
   * Get the amount of power from the engine which goes to a wheel when accelerating.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual float GetWheelEnginePower(int wheelnum) = 0;

 /**
   * Get the amount of power from the brakes which goes to a wheel.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual float GetWheelBrakePower(int wheelnum) = 0;

 /**
   * Get whether a wheel steers in the opposite direction.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual bool GetWheelSteerInverted(int wheelnum) = 0;

 /**
   * Get whether the handbrake applies to a wheel.
   *  @param wheelnum Index of the wheel to get.
  */
  virtual bool GetWheelHandbrakeAffected(int wheelnum) = 0;
};

#endif // __CEL_PF_WHEELED__

