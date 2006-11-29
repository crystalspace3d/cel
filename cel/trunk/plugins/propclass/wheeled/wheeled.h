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

struct celWheel
{
  csRef<iODEHinge2Joint> WheelJoint;
  csRef<iRigidBody> RigidBody;
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
    action_settankmode,

    action_addwheelauto,
    action_addwheel,
    action_deletewheel,
    action_deleteallwheels,
    action_destroywheel,
    action_destroyallwheels,
    action_restorewheel,
    action_restoreallwheels,
    action_accelerate,
    action_brake,
    action_handbrake,
    action_setsteeramount,
    action_steerleft,
    action_steerright,
    action_steerstraight,
    action_reverse,
    action_neutral,
    action_setautotransmission,
    action_setgear,
    action_setgearsettings,
    action_setbrakeforce,
    action_setautoreverse,

    //Presets.
    action_setfrontwheelpreset,
    action_setrearwheelpreset,
    action_setouterwheelsteerpreset,

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

  // Parameters.
  static csStringID param_meshfile;
  static csStringID param_meshfact;
  static csStringID param_rotation;
  static csStringID param_position;
  static csStringID param_wheelnum;
  static csStringID param_gear;
  static csStringID param_velocity;
  static csStringID param_force;
  static csStringID param_number;
  static csStringID param_tankmode;
  static csStringID param_steeramount;
  static csStringID param_brakeforce;
  static csStringID param_applied;
  static csStringID param_autotransmission;
  static csStringID param_autoreverse;

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

  static PropertyHolder propinfo;

  float brakeforce;
  int gear;
  bool autotransmission;
  bool autoreverse;
  bool tankmode;
  bool handbrakeapplied;
  bool brakeapplied;
  int steerdir;
  float wheelradius;
  //The angle the user wants the wheels to reach
  float steeramount;
  //The amount of preset sensitivity to be applied to front wheels.
  float frontsteer;
  //The amount of preset sensitivity to be applied to rear wheels.
  float rearsteer;
  //The amount of preset sensitivity to be applied to outer wheels.
  float outersteer;
  //The amount of preset power to go to the front wheels.
  float frontpower;
  //The amount of preset power to go to the rear wheels.
  float rearpower;
  //Preset front wheel mass
  float frontmass;
  //Preset rear wheel mass
  float rearmass;
  //Preset front wheel friction
  float frontfriction;
  //Preset rear wheel friction
  float rearfriction;

  float frontss;
  float frontsd;
  float rearss;
  float rearsd;

  csString wheelpath;
  csString wheelfact;
  csRef<iBodyGroup> bodyGroup;
  csRef<iEngine> engine;
  csRef<iDynamicSystem> dyn;
  csRef<iODEDynamicSystemState> osys;
  csArray<csVector2> gears;
  csArray<celWheel> wheels;
  csRef<iPcMechanicsObject> bodyMech;
  celGenericParameterBlock* params;
  int topgear;
  bool accelerating;
  bool cd_enabled;
  // Other fields.
  int counter;
  size_t max;

public:
  celPcWheeled (iObjectRegistry* object_reg);
  virtual ~celPcWheeled ();
  virtual void GetMech();
  void TickOnce();
  void UpdateGear();

  //Setters
  virtual void SetWheelMesh(const char* file, const char* factname);
  virtual void SetTankMode(bool tankmode)
  {celPcWheeled::tankmode=tankmode;};
  virtual void SetSteerAmount(float steeramount)
  {celPcWheeled::steeramount=steeramount;};
  //This one uses presets
  virtual int AddWheelAuto(csVector3 position, const char* wheelfact = 0,
       const char* wheelfile = 0,
       csMatrix3 rotation = csMatrix3(0.0f,0.0f,0.0f,0.0f));
  //Full specification by the user, overrides presets
  virtual int AddWheel(csVector3 position,float turnspeed,
      float returnspeed, float ss, float sd,float brakepower,float enginepower,
      float lss, float rss, float friction, float mass,
      bool hbaffect, bool sinvert, const char* wheelfact = 0,
      const char* wheelfile = 0,
      csMatrix3 rotation = csMatrix3(0.0f,0.0f,0.0f,0.0f));

  virtual void DeleteWheel(int wheelnum);
  virtual void DeleteAllWheels();
  virtual void DestroyWheel(int wheelnum);
  virtual void DestroyAllWheels();
  virtual void RestoreWheel(int wheelnum);
  virtual void RestoreAllWheels();

  virtual void Accelerate(bool on) {accelerating = on;}
  virtual bool IsAccelerating() {return accelerating;}
  virtual void Brake(bool on);
  virtual bool IsBraking() {return brakeapplied;}
  virtual void Handbrake(bool on) {handbrakeapplied=on;}
  virtual bool IsHandbraking() {return handbrakeapplied;}

  virtual void SetCollisionCallbackEnabled (bool en) { cd_enabled = en; }
  virtual bool IsCollisionCallbackEnabled () const { return cd_enabled; }

  virtual void SteerLeft();
  virtual void SteerRight();
  virtual void UpdateTankSteer();
  virtual void SteerStraight();
  virtual void Reverse() {gear=-1; accelerating=true;}
  virtual void Neutral() {gear=0;}
  virtual void SetAutoTransmission(bool autotransmission)
  {celPcWheeled::autotransmission=autotransmission;}

  virtual void SetGear(int gear);
  virtual void SetGearSettings(int gear,float velocity, float force);
  virtual void SetBrakeForce(float force){brakeforce=force;}
  virtual void SetAutoReverse(bool autoreverse)
  {celPcWheeled::autoreverse=autoreverse;}

  //Some wheel steering presets.
  virtual void SetOuterWheelSteerPreset(float sensitivity);
  virtual void SetFrontWheelPreset(float sensitivity,float enginepower,
   float suspensionsoftness, float suspensiondamping, float friction, float mass);
  virtual void SetRearWheelPreset(float sensitivity,float enginepower,
   float suspensionsoftness, float suspensiondamping, float friction, float mass);
  virtual void ApplyWheelPresets(int wheelnum);

  // Stuff independent for each wheel
  //Settings related to the joints which need extra setup--------------
  virtual void SetWheelPosition(int wheelnum, csVector3 position);
  virtual void SetWheelRotation(int wheelnum, csMatrix3 position);
  virtual void SetWheelSuspensionSoftness(int wheelnum, float softness);
  virtual void SetWheelSuspensionDamping(int wheelnum, float damping);
  virtual void SetWheelFriction(int wheelnum, float friction);
  virtual void SetWheelMass(int wheelnum, float mass);
  //--------------

  virtual void SetWheelLeftSteerSensitivity(int wheelnum, float
   sensitivity)
  {
    if(sensitivity>=0 && sensitivity <=1)
      wheels[wheelnum].LeftSteerSensitivity=sensitivity;
  }

  virtual void SetWheelRightSteerSensitivity(int wheelnum, float
    sensitivity)
  {
    if(sensitivity>=0 && sensitivity <=1)
      wheels[wheelnum].RightSteerSensitivity=sensitivity;
  }

  virtual void SetWheelTurnSpeed(int wheelnum, float speed)
  {if(speed>=0) wheels[wheelnum].TurnSpeed=speed;}

  virtual void SetWheelReturnSpeed(int wheelnum, float speed)
  {if(speed>=0) wheels[wheelnum].ReturnSpeed=speed;}

  virtual void SetWheelEnginePower(int wheelnum, float power)
  {if(power>=0 && power <=1) wheels[wheelnum].EnginePower=power;}

  virtual void SetWheelBrakePower(int wheelnum, float power)
  {if(power>=0 && power <=1) wheels[wheelnum].BrakePower=power;}

  virtual void SetWheelSteerInverted(int wheelnum, bool inverted)
  {wheels[wheelnum].SteerInverted=inverted;}

  virtual void SetWheelHandbrakeAffected(int wheelnum,
      bool handbrakeaffected)
  {wheels[wheelnum].HandbrakeAffected=handbrakeaffected;}

  virtual void SetWheelMesh(int wheelnum, const char* file,
     const char* factname);

  //The getter functions
  virtual iBodyGroup* GetBodyGroup(){return bodyGroup;}
  virtual int GetTopGear() {return topgear;}
  virtual int GetGear(){return gear;}
  virtual float GetSteerAmount(){return steeramount;}
  virtual bool GetTankMode(){return tankmode;}
  virtual bool GetBrakeApplied(){return brakeapplied;}
  virtual bool GetHandbrakeApplied(){return handbrakeapplied;}

  // Per-wheel settings
  virtual csVector3 GetWheelPosition(int wheelnum)
  {return wheels[wheelnum].Position;}

  virtual csMatrix3 GetWheelRotation(int wheelnum)
  {return wheels[wheelnum].Rotation;}

  virtual float GetWheelSuspensionSoftness(int wheelnum)
  {return wheels[wheelnum].SuspensionSoftness;}

  virtual float GetWheelSuspensionDamping(int wheelnum)
  {return wheels[wheelnum].SuspensionDamping;}

  virtual float GetWheelLeftSteerSensitivity(int wheelnum)
  {return wheels[wheelnum].LeftSteerSensitivity;}

  virtual float GetWheelRightSteerSensitivity(int wheelnum)
  {return wheels[wheelnum].RightSteerSensitivity;}

  virtual float GetWheelTurnSpeed(int wheelnum)
  {return wheels[wheelnum].TurnSpeed;}

  virtual float GetWheelReturnSpeed(int wheelnum)
  {return wheels[wheelnum].ReturnSpeed;}

  virtual float GetWheelEnginePower(int wheelnum)
  {return wheels[wheelnum].EnginePower;}

  virtual float GetWheelBrakePower(int wheelnum)
  {return wheels[wheelnum].BrakePower;}

  virtual float GetWheelFriction(int wheelnum)
  {return wheels[wheelnum].WheelMass;}

  virtual float GetWheelMass(int wheelnum)
  {return wheels[wheelnum].WheelMass;}

  virtual bool GetWheelSteerInverted(int wheelnum)
  {return wheels[wheelnum].SteerInverted;}

  virtual bool GetWheelHandbrakeAffected(int wheelnum)
  {return wheels[wheelnum].HandbrakeAffected;}

  virtual iRigidBody* GetWheelBody(int wheelnum)
  {return wheels[wheelnum].RigidBody;}

  virtual iODEHinge2Joint* GetWheelJoint(int wheelnum)
  {return wheels[wheelnum].WheelJoint;}

  virtual int GetWheelCount() { return wheels.Length(); }

  struct WheeledCollisionCallback : public iDynamicsCollisionCallback
  {
    celPcWheeled* parent;
    WheeledCollisionCallback (celPcWheeled* parent)
    {
      SCF_CONSTRUCT_IBASE (0);
      WheeledCollisionCallback::parent = parent;
    }
    virtual ~WheeledCollisionCallback ()
    {
      SCF_DESTRUCT_IBASE ();
    }
    SCF_DECLARE_IBASE;

    virtual void Execute (iRigidBody *thisbody, iRigidBody *otherbody,
	const csVector3& pos, const csVector3& normal, float depth)
    {
      parent->Collision (thisbody, otherbody, pos, normal, depth);
    }
  } *scfiWheeledCollisionCallback;

  void Collision (iRigidBody *thisbody, iRigidBody *otherbody,
	const csVector3& pos, const csVector3& normal, float depth);

  virtual const char* GetName () const { return "pcwheeled"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);
};

#endif // __CEL_PF_VEHICLEFACT__

