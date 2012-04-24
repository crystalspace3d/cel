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
  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef __CEL_PF_WHEELEDFACT__
#define __CEL_PF_WHEELEDFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/wheeled.h"
#include "ivaria/dynamics.h"
#include "ivaria/ode.h"
#include "propclass/mechsys.h"
#include "csgeom/matrix3.h"

struct iCelEntity;
struct iObjectRegistry;
class csVector3;
struct iMeshFactoryWrapper;
struct iEngine;
class WheeledCollisionCallback;

struct celWheel
{
  csRef<iODEHinge2Joint> WheelJoint;
  csRef<iRigidBody> RigidBody;
  csRef<iODEAMotorJoint> BrakeMotor;
    //Used by the differentials
  csRef<iODEGeneralJointState> JointState;
  csVector3 Position;
  csMatrix3 Rotation;
  csString Meshfact;
  float LeftSteerSensitivity;
  float RightSteerSensitivity;
  float TurnSpeed;
  float ReturnSpeed;
  float EnginePower;
  float BrakePower;
  float SuspensionSoftness;
  float SuspensionDamping;
  float WheelFriction;
  float WheelMass;
  bool SteerInverted;
  bool HandbrakeAffected;
  //Used for abs braking. Ratio of brake application
  float ABSBrake;
  //Used for steering
  float CurrentStop;
  WheeledCollisionCallback* Callback;
};
/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (Wheeled)

/**
 * This is a property class for making and controlling wheeled vehicles, eg
 * bikes, cars, tanks.
 */
class celPcWheeled : public scfImplementationExt1<
  celPcWheeled, celPcCommon, iPcWheeled>
{
private:
  // Actions
  enum actionids
  {
    action_setwheelmesh = 0,

    action_addwheelauto,
    action_addwheel,
    action_deletewheel,
    action_deleteallwheels,
    action_destroywheel,
    action_destroyallwheels,
    action_restorewheel,
    action_restoreallwheels,
    action_steerleft,
    action_steerright,
    action_steerstraight,
    action_reverse,
    action_neutral,
    action_setgearsettings,

    //Presets.
    action_setfrontwheelpreset,
    action_setrearwheelpreset,

    //Per-wheel actions
    action_setwheelposition,
    action_setwheelrotation,
    action_setwheelsuspensionsoftness,
    action_setwheelsuspensiondamping,
    action_setwheelleftsteersensitivity,
    action_setwheelrightsteersensitivity,
    action_setwheelfriction,
    action_setwheelmass,
    action_setwheelturnspeed,
    action_setwheelreturnspeed,
    action_setwheelenginepower,
    action_setwheelbrakepower,
    action_setwheelsteerinverted,
    action_setwheelhandbrakeaffected
  };

  // For properties.
  enum propids
  {
    propid_speed = 0,
    propid_tankmode,
    propid_steer,
    propid_accelamount,
    propid_brakeamount,
    propid_handbraking,
    propid_steeramount,
    propid_autotransmission,
    propid_gear,
    propid_brakeforce,
    propid_autoreverse,
    propid_outerwheelsteerpreset,
    propid_abs,
    propid_currentgearvelocity,
    propid_currentgearforce,
    propid_averagewheelspin,
    propid_differential,
    propid_antisway,
    propid_antiswayfactor,
    propid_antiswaylimit
  };

  // Parameters.
  static csStringID param_meshfile;
  static csStringID param_meshfact;
  static csStringID param_rotation;
  static csStringID param_position;
  static csStringID param_wheelnum;
  static csStringID param_gear;
  static csStringID param_velocity;
  static csStringID param_force;
  static csStringID param_steeramount;
  static csStringID param_number;
  static csStringID param_applied;

  static csStringID param_otherbody;
  static csStringID param_depth;
  static csStringID param_normal;
  static csStringID param_index;

  static csStringID param_suspensionsoftness;
  static csStringID param_suspensiondamping;
  static csStringID param_leftsteersensitivity;
  static csStringID param_rightsteersensitivity;
  static csStringID param_steersensitivity;
  static csStringID param_turnspeed;
  static csStringID param_returnspeed;
  static csStringID param_enginepower;
  static csStringID param_brakepower;
  static csStringID param_friction;
  static csStringID param_mass;
  static csStringID param_steerinverted;
  static csStringID param_handbrakeaffected;

  csRef<iMessageDispatcher> dispatcher_collision;

  static PropertyHolder propinfo;

  float speed;          //Current speed
  float brakeforce;     //Strength of brakes
  int gear;             //Current gear
  bool autotransmission;//Whether to automatically shift gears
  bool autoreverse;     //Whether to automatically reverse after stopping
  bool tankmode;        //Whether to use tank steering
  bool handbrakeapplied;//Whether handbrake is applied
  bool differential;    //Whether the differentials are used
  bool antisway;        //Whether to apply anti-sway forces to the vehicle's wheels.
  
  bool abs;             //ABS braking
  float abssteer;       //Absolute steering amount
  float steeramount;    //The angle the user wants the wheels to reach
  float frontsteer;     //The Amount of preset sensitivity to be applied to
                        //front wheels.
  float rearsteer;      //The amount of preset sensitivity to be applied to
                        //rear wheels.
  float outersteer;     //The amount of preset sensitivity to be applied to
                        //outer wheels.
  float frontpower;     //The amount of preset power to go to the front wheels.
  float rearpower;      //The amount of preset power to go to the rear wheels.
  float frontmass;      //Preset front wheel mass
  float rearmass;       //Preset rear wheel mass
  float frontfriction;  //Preset front wheel friction
  float rearfriction;   //Preset rear wheel friction

  //Preset suspension settings
  float frontss;
  float frontsd;
  float rearss;
  float rearsd;

  float accelamount;  //Accelerator amount of application
  float brakeamount;  //Brake amount of application

  float antiswayfactor;
  float antiswaylimit;

  csString wheelpath;
  csString wheelfact;
  csRef<iBodyGroup> bodyGroup;
  csRef<iEngine> engine;
  csRef<iDynamicSystem> dyn;
  csRef<iODEDynamicSystemState> osys;
  csArray<csVector2> gears;
  csArray<celWheel> wheels;
  //Add a new z pos for linking differential wheels
  //The csVector2 holds the indexes of left and right wheels in a group.
  csHash<csVector2, float> diffGroups;
  csRef<iPcMechanicsObject> bodyMech;
  csRef<celVariableParameterBlock> params;
  int topgear;
  bool cd_enabled;
  // Other fields.
  int counter;
  size_t max;

public:
  celPcWheeled (iObjectRegistry* object_reg);
  virtual ~celPcWheeled ();
  virtual void GetMech();
  void TickOnce();
  void UpdateAccel(size_t wheelnum);
  void UpdateBrakes(float avgspin, size_t wheelnum);
  void UpdateTankSteer(size_t wheelnum);
  void UpdateGear();
  void UpdateSteer(size_t wheelnum);
  void AddDiffWheel(size_t index);
  void AntiSway();

  //Setters
  virtual void SetWheelMesh(const char* file, const char* factname);
  virtual void SetTankMode(bool tankmode)
  {celPcWheeled::tankmode = tankmode;};
  virtual void SetABS(bool enabled)
  {celPcWheeled::abs = enabled;}
  virtual void SetDifferential(bool enabled)
  {celPcWheeled::differential = enabled;}
  virtual void SetSteerAmount(float steeramount)
  {celPcWheeled::steeramount = steeramount;};
  //This one uses presets
  virtual size_t AddWheelAuto(csVector3 position, const char* wheelfact = 0,
       const char* wheelfile = 0,
       csMatrix3 rotation = csMatrix3(0.0f,0.0f,0.0f,0.0f));
  //Full specification by the user, overrides presets
  virtual size_t AddWheel(csVector3 position,float turnspeed,
      float returnspeed, float ss, float sd,float brakepower,float enginepower,
      float lss, float rss, float friction, float mass,
      bool hbaffect, bool sinvert, const char* wheelfact = 0,
      const char* wheelfile = 0,
      csMatrix3 rotation = csMatrix3(0.0f,0.0f,0.0f,0.0f));

  virtual void DeleteWheel(size_t wheelnum);
  virtual void DeleteAllWheels();
  virtual void DestroyWheel(size_t wheelnum);
  virtual void DestroyAllWheels();
  virtual void RestoreWheel(size_t wheelnum);
  virtual void RestoreAllWheels();

  virtual void Accelerate(float amount = 1.0f)
    {
      if (accelamount >= 0.0f && accelamount <= 1.0f)
        accelamount = amount;
    }
  virtual float GetAcceleratorAmount() {return accelamount;}
  virtual void Brake(float amount = 1.0f)
    {
      if (brakeamount >= 0.0f && brakeamount <= 1.0f)
        brakeamount = amount;
    }
  virtual float GetBrakeAmount() {return brakeamount;}
  virtual void Handbrake(bool on) {handbrakeapplied=on;}
  virtual bool IsHandbraking() {return handbrakeapplied;}

  virtual void SetCollisionCallbackEnabled (bool en) { cd_enabled = en; }
  virtual bool IsCollisionCallbackEnabled () const { return cd_enabled; }

  virtual void SteerLeft(float amount = 1.0f);
  virtual void SteerRight(float amount = 1.0f);
  virtual void Steer(float amount);
  virtual void SteerStraight();
  virtual void Reverse() {gear=-1;}
  virtual void Neutral() {gear=0;}
  virtual void SetAutoTransmission(bool autotransmission)
  {celPcWheeled::autotransmission=autotransmission;}

  virtual void SetGear(int gear);
  virtual void SetGearSettings(int gear,float velocity, float force);
  virtual void SetBrakeForce(float force){brakeforce=force;}
  virtual void SetAutoReverse(bool autoreverse)
  {celPcWheeled::autoreverse=autoreverse;}

  virtual void SetAntiSway(bool enabled){celPcWheeled::antisway = enabled;}
  virtual void SetAntiSwayFactor(float antiswayfactor)
    {celPcWheeled::antiswayfactor = antiswayfactor;}
  virtual void SetAntiSwayLimit(float antiswaylimit)
    {celPcWheeled::antiswaylimit = antiswaylimit;}

  //Some wheel steering presets.
  virtual void SetOuterWheelSteerPreset(float sensitivity);
  virtual void SetFrontWheelPreset(float sensitivity,float enginepower,
   float suspensionsoftness, float suspensiondamping, float friction, float mass);
  virtual void SetRearWheelPreset(float sensitivity,float enginepower,
   float suspensionsoftness, float suspensiondamping, float friction, float mass);
  virtual void ApplyWheelPresets(size_t wheelnum);

  // Stuff independent for each wheel
  //Settings related to the joints which need extra setup--------------
  virtual void SetWheelPosition(size_t wheelnum, csVector3 position);
  virtual void SetWheelRotation(size_t wheelnum, csMatrix3 position);
  virtual void SetWheelSuspensionSoftness(size_t wheelnum, float softness);
  virtual void SetWheelSuspensionDamping(size_t wheelnum, float damping);
  virtual void SetWheelFriction(size_t wheelnum, float friction);
  virtual void SetWheelMass(size_t wheelnum, float mass);
  //--------------

  virtual void SetWheelLeftSteerSensitivity(size_t wheelnum, float
   sensitivity)
  {
    if(sensitivity>=0 && sensitivity <=1)
      wheels[wheelnum].LeftSteerSensitivity=sensitivity;
  }

  virtual void SetWheelRightSteerSensitivity(size_t wheelnum, float
    sensitivity)
  {
    if(sensitivity>=0 && sensitivity <=1)
      wheels[wheelnum].RightSteerSensitivity=sensitivity;
  }

  virtual void SetWheelTurnSpeed(size_t wheelnum, float speed)
  {if(speed>=0) wheels[wheelnum].TurnSpeed=speed;}

  virtual void SetWheelReturnSpeed(size_t wheelnum, float speed)
  {if(speed>=0) wheels[wheelnum].ReturnSpeed=speed;}

  virtual void SetWheelEnginePower(size_t wheelnum, float power)
  {if(power>=0 && power <=1) wheels[wheelnum].EnginePower=power;}

  virtual void SetWheelBrakePower(size_t wheelnum, float power)
  {if(power>=0 && power <=1) wheels[wheelnum].BrakePower=power;}

  virtual void SetWheelSteerInverted(size_t wheelnum, bool inverted)
  {wheels[wheelnum].SteerInverted=inverted;}

  virtual void SetWheelHandbrakeAffected(size_t wheelnum,
      bool handbrakeaffected)
  {wheels[wheelnum].HandbrakeAffected=handbrakeaffected;}

  virtual void SetWheelMesh(size_t wheelnum, const char* file,
     const char* factname);

  //The getter functions
  virtual bool GetDifferential(){return differential;}
  virtual iBodyGroup* GetBodyGroup(){return bodyGroup;}
  virtual int GetTopGear() {return topgear;}
  virtual int GetGear(){return gear;}
  virtual float GetSteerAmount(){return steeramount;}
  virtual float GetSteer() {return abssteer;}
  virtual bool GetTankMode(){return tankmode;}
  virtual bool GetABS(){return abs;}
  virtual float GetGearVelocity(int gear){return gears[gear+1].x;}
  virtual float GetGearForce(int gear){return gears[gear+1].y;}
  virtual float GetSpeed();
  virtual bool GetAntiSway(){return antisway;}
  virtual float GetAntiSwayFactor(){return antiswayfactor;}
  virtual float GetAntiSwayLimit(){return antiswaylimit;}

  // Per-wheel settings
  virtual csVector3 GetWheelPosition(size_t wheelnum)
  {return wheels[wheelnum].Position;}

  virtual csMatrix3 GetWheelRotation(size_t wheelnum)
  {return wheels[wheelnum].Rotation;}

  virtual float GetWheelSuspensionSoftness(size_t wheelnum)
  {return wheels[wheelnum].SuspensionSoftness;}

  virtual float GetWheelSuspensionDamping(size_t wheelnum)
  {return wheels[wheelnum].SuspensionDamping;}

  virtual float GetWheelLeftSteerSensitivity(size_t wheelnum)
  {return wheels[wheelnum].LeftSteerSensitivity;}

  virtual float GetWheelRightSteerSensitivity(size_t wheelnum)
  {return wheels[wheelnum].RightSteerSensitivity;}

  virtual float GetWheelTurnSpeed(size_t wheelnum)
  {return wheels[wheelnum].TurnSpeed;}

  virtual float GetWheelReturnSpeed(size_t wheelnum)
  {return wheels[wheelnum].ReturnSpeed;}

  virtual float GetWheelEnginePower(size_t wheelnum)
  {return wheels[wheelnum].EnginePower;}

  virtual float GetWheelBrakePower(size_t wheelnum)
  {return wheels[wheelnum].BrakePower;}

  virtual float GetWheelFriction(size_t wheelnum)
  {return wheels[wheelnum].WheelMass;}

  virtual float GetWheelMass(size_t wheelnum)
  {return wheels[wheelnum].WheelMass;}

  virtual bool GetWheelSteerInverted(size_t wheelnum)
  {return wheels[wheelnum].SteerInverted;}

  virtual bool GetWheelHandbrakeAffected(size_t wheelnum)
  {return wheels[wheelnum].HandbrakeAffected;}

  virtual iRigidBody* GetWheelBody(size_t wheelnum)
  {return wheels[wheelnum].RigidBody;}

  virtual iODEHinge2Joint* GetWheelJoint(size_t wheelnum)
  {return wheels[wheelnum].WheelJoint;}

  virtual iODEAMotorJoint* GetBrakeMotor(size_t wheelnum)
  {return wheels[wheelnum].BrakeMotor;}

  virtual float GetWheelSpin(size_t wheelnum);
  virtual float GetAverageWheelSpin();

  virtual size_t GetWheelCount() { return wheels.GetSize(); }

  void WheelCollision (iRigidBody *thisbody, iRigidBody *otherbody,
	const csVector3& pos, const csVector3& normal, float depth, size_t index);

  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);
  virtual bool GetPropertyIndexed (int, long&);
  virtual bool SetPropertyIndexed (int, long);
  virtual bool GetPropertyIndexed (int, float&);
  virtual bool SetPropertyIndexed (int, float);

  //---------- Deprecated methods-------------------//
  virtual void SetABSEnabled(bool enabled)
  {celPcWheeled::abs = enabled;}
  virtual bool GetABSEnabled()
  {return celPcWheeled::abs;}

  virtual size_t AddWheel(csVector3 position,float turnspeed,
      float returnspeed, float ss, float sd,float brakepower,float enginepower,
      float lss, float rss, bool hbaffect, bool sinvert,
      const char* wheelfact = 0, const char* wheelfile = 0,
      csMatrix3 rotation = csMatrix3(0.0f,0.0f,0.0f,0.0f))
  {return AddWheel(position, turnspeed, returnspeed, ss, sd, brakepower,
     enginepower, lss, rss, 0.7f, 10.0f, hbaffect, sinvert, wheelfact,
     wheelfile, rotation);}
  virtual void SetFrontWheelPreset(float sensitivity,float enginepower,
   float suspensionsoftness, float suspensiondamping)
  {SetFrontWheelPreset(sensitivity, enginepower, suspensionsoftness,
   suspensiondamping, 0.7f, 10.0f);}
  virtual void SetRearWheelPreset(float sensitivity,float enginepower,
   float suspensionsoftness, float suspensiondamping)
  {SetRearWheelPreset(sensitivity, enginepower, suspensionsoftness,
   suspensiondamping, 0.7f, 10.0f);}
};

#endif // __CEL_PF_WHEELEDFACT__

