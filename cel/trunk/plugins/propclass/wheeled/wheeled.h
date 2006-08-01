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
  float LeftSteerSensitivity;
  float RightSteerSensitivity;
  float TurnSpeed;
  float ReturnSpeed;
  float EnginePower;
  float BrakePower;
  float SuspensionSoftness;
  float SuspensionDamping;
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
    static csStringID action_setwheelmesh;
    static csStringID action_settankmode;

    static csStringID action_addwheelauto;
    static csStringID action_addwheel;
    static csStringID action_deletewheel;
    static csStringID action_deleteallwheels;
    static csStringID action_destroywheel;
    static csStringID action_destroyallwheels;
    static csStringID action_restorewheel;
    static csStringID action_restoreallwheels;
    static csStringID action_accelerate;
    static csStringID action_brake;
    static csStringID action_handbrake;
    static csStringID action_setsteeramount;
    static csStringID action_steerleft;
    static csStringID action_steerright;
    static csStringID action_steerstraight;
    static csStringID action_reverse;
    static csStringID action_neutral;
    static csStringID action_setautotransmission;
    static csStringID action_setgear;
    static csStringID action_setgearsettings;
    static csStringID action_setbrakeforce;
    static csStringID action_setnumbergears;
    static csStringID action_setautoreverse;

//Presets.
    static csStringID action_setfrontwheelpreset;
    static csStringID action_setrearwheelpreset;
    static csStringID action_setouterwheelsteerpreset;

  //Per-wheel actions
    static csStringID action_setwheelposition;
    static csStringID action_setwheelsuspensionsoftness;
    static csStringID action_setwheelsuspensiondamping;
    static csStringID action_setwheelleftsteersensitivity;
    static csStringID action_setwheelrightsteersensitivity;
    static csStringID action_setwheelturnspeed;
    static csStringID action_setwheelreturnspeed;
    static csStringID action_setwheelenginepower;
    static csStringID action_setwheelbrakepower;
    static csStringID action_setwheelsteerinverted;
    static csStringID action_setwheelhandbrakeaffected;

  // Parameters.
    static csStringID param_file;
    static csStringID param_name;
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

    static csStringID param_suspensionsoftness; 
    static csStringID param_suspensiondamping;
    static csStringID param_leftsteersensitivity; 
    static csStringID param_rightsteersensitivity;
    static csStringID param_steersensitivity;
    static csStringID param_turnspeed;
    static csStringID param_returnspeed;
    static csStringID param_enginepower;
    static csStringID param_brakepower;
    static csStringID param_steerinverted;
    static csStringID param_handbrakeaffected;

    float brakeforce;
    int gear;
    int numberwheels;
    bool autotransmission;
    bool autoreverse;
    bool tankmode;
    bool handbrakeapplied;
    bool brakeapplied;
    bool steering;
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

    float frontss;
    float frontsd;
    float rearss;
    float rearsd;
    csString wheelpath;
    csString wheelfactname;
    csRef<iMeshFactoryWrapper> wheelfact;
    csRef<iBodyGroup> bodyGroup;
    csRef<iEngine> engine;
    csRef<iDynamicSystem> dyn;
    csRef<iODEDynamicSystemState> osys;
    csArray<csVector2> gears;
    csArray<celWheel> wheels;
    csRef<iPcMechanicsObject> bodyMech;
    bool accelerating;
  // Other fields.
    int counter;
    size_t max;
    void TickOnce();
    void UpdateGear();

  public:
    celPcWheeled (iObjectRegistry* object_reg);
    virtual ~celPcWheeled ();
    virtual void GetMech();

  //Setters
    virtual void SetWheelMesh(const char* file, const char* factname);
    virtual void SetTankMode(bool tankmode)
    {celPcWheeled::tankmode=tankmode;};
    virtual void SetSteerAmount(float steeramount)
    {celPcWheeled::steeramount=steeramount;};
    //This one uses presets
    virtual int AddWheelAuto(csVector3 position);
    //Full specification by the user, overrides presets
    virtual int AddWheel(csVector3 position,float turnspeed, float
      returnspeed, float ss, float sd,float brakepower,float enginepower,
      float lss, float rss ,bool hbaffect, bool sinvert);

    virtual void DeleteWheel(int wheelnum);
    virtual void DeleteAllWheels();
    virtual void DestroyWheel(int wheelnum);
    virtual void DestroyAllWheels();
    virtual void RestoreWheel(int wheelnum);
    virtual void RestoreAllWheels();
    
    
    virtual void Accelerate(bool on)
    {accelerating = on;};

    virtual bool IsAccelerating()
    {return accelerating;};

    virtual void Brake(bool on);

    virtual bool IsBraking()
    {return brakeapplied;};

    virtual void Handbrake(bool on)
    {handbrakeapplied=on;};

    virtual bool IsHandbraking()
    {return handbrakeapplied;};
  
    
    virtual void SteerLeft();
    virtual void SteerRight();
    virtual void UpdateTankSteer();
    virtual void SteerStraight();
    virtual void Reverse()
    {gear=-1;
     accelerating=true;};
    
    virtual void Neutral()
    {gear=0;};
    
    virtual void SetAutoTransmission(bool autotransmission)
    {celPcWheeled::autotransmission=autotransmission;};
    
    virtual void SetGear(int gear);
    virtual void SetGearSettings(int gear,float velocity, float force);
    virtual void SetBrakeForce(float force){brakeforce=force;};
    
    virtual void SetNumberGears(int number)
    {gears.SetSize(number+2);};
    
    virtual void SetAutoReverse(bool autoreverse)
    {celPcWheeled::autoreverse=autoreverse;};

    //Some wheel steering presets.
    virtual void SetOuterWheelSteerPreset(float sensitivity);
    virtual void SetFrontWheelPreset(float sensitivity,float enginepower,
     float suspensionsoftness, float suspensiondamping);
    virtual void SetRearWheelPreset(float sensitivity,float enginepower,
     float suspensionsoftness, float suspensiondamping);
    virtual void ApplyWheelPresets(int wheelnum);

  // Stuff independent for each wheel
    //Settings related to the joints which need extra setup--------------
    virtual void SetWheelPosition(int wheelnum, csVector3 position);
    virtual void SetWheelSuspensionSoftness(int wheelnum, float softness);
    virtual void SetWheelSuspensionDamping(int wheelnum, float damping);
   //--------------

    virtual void SetWheelLeftSteerSensitivity(int wheelnum, float
     sensitivity)
    {
      if(sensitivity>=0 && sensitivity <=1)
        wheels[wheelnum].LeftSteerSensitivity=sensitivity;
    };

    virtual void SetWheelRightSteerSensitivity(int wheelnum, float
      sensitivity)
    {
      if(sensitivity>=0 && sensitivity <=1)
        wheels[wheelnum].RightSteerSensitivity=sensitivity;
    };

    virtual void SetWheelTurnSpeed(int wheelnum, float speed)
    {if(speed>=0) wheels[wheelnum].TurnSpeed=speed;};

    virtual void SetWheelReturnSpeed(int wheelnum, float speed)
    {if(speed>=0) wheels[wheelnum].ReturnSpeed=speed;};

    virtual void SetWheelEnginePower(int wheelnum, float power)
    {if(power>=0 && power <=1) wheels[wheelnum].EnginePower=power;};

    virtual void SetWheelBrakePower(int wheelnum, float power)
    {if(power>=0 && power <=1) wheels[wheelnum].BrakePower=power;};

    virtual void SetWheelSteerInverted(int wheelnum, bool inverted)
    {wheels[wheelnum].SteerInverted=inverted;};

    virtual void SetWheelHandbrakeAffected(int wheelnum, bool
handbrakeaffected)
    {wheels[wheelnum].HandbrakeAffected=handbrakeaffected;};

  //The getter functions
    virtual iBodyGroup* GetBodyGroup(){return bodyGroup;};
    virtual size_t GetNumberGears() {return gears.Length();};
    virtual int GetGear(){return gear;};
    virtual float GetSteerAmount(){return steeramount;};
    virtual int GetTankMode(){return tankmode;};
    virtual bool GetBrakeApplied(){return brakeapplied;};
    virtual bool GetHandbrakeApplied(){return handbrakeapplied;};

    // Per-wheel settings
    virtual csVector3 GetWheelPosition(int wheelnum)
    {return wheels[wheelnum].Position;};

    virtual float GetWheelSuspensionSoftness(int wheelnum)
    {return wheels[wheelnum].SuspensionSoftness;};

    virtual float GetWheelSuspensionDamping(int wheelnum)
    {return wheels[wheelnum].SuspensionDamping;};

    virtual float GetWheelLeftSteerSensitivity(int wheelnum)
    {return wheels[wheelnum].LeftSteerSensitivity;};

    virtual float GetWheelRightSteerSensitivity(int wheelnum)
    {return wheels[wheelnum].RightSteerSensitivity;};

    virtual float GetWheelTurnSpeed(int wheelnum)
    {return wheels[wheelnum].TurnSpeed;};

    virtual float GetWheelReturnSpeed(int wheelnum)
    {return wheels[wheelnum].ReturnSpeed;};

    virtual float GetWheelEnginePower(int wheelnum)
    {return wheels[wheelnum].EnginePower;};

    virtual float GetWheelBrakePower(int wheelnum)
    {return wheels[wheelnum].BrakePower;};

    virtual bool GetWheelSteerInverted(int wheelnum)
    {return wheels[wheelnum].SteerInverted;};

    virtual bool GetWheelHandbrakeAffected(int wheelnum)
    {return wheels[wheelnum].HandbrakeAffected;};


    virtual const char* GetName () const { return "pcwheeled"; }
    virtual csPtr<iCelDataBuffer> Save ();
    virtual bool Load (iCelDataBuffer* databuf);
    virtual bool PerformAction (csStringID actionId, iCelParameterBlock*
params,
	celData& ret);

};

#endif // __CEL_PF_VEHICLEFACT__

