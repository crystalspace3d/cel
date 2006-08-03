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

#include "cssysdef.h"
#include "iutil/objreg.h"
#include "csutil/debug.h"
#include "plugins/propclass/wheeled/wheeled.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

#include "iutil/vfs.h"
#include "iengine/mesh.h"
#include "imesh/objmodel.h"
#include "imesh/object.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "iengine/engine.h"
#include "imap/loader.h"

#include "propclass/mesh.h"
#include "propclass/mechsys.h"

#include "iostream"

//--------------------------------------------------------------------------


CS_IMPLEMENT_PLUGIN
CEL_IMPLEMENT_FACTORY (Wheeled, "pcwheeled")


//--------------------------------------------------------------------------


// Actions
csStringID celPcWheeled::action_setwheelmesh = csInvalidStringID;
csStringID celPcWheeled::action_settankmode = csInvalidStringID;
csStringID celPcWheeled::action_addwheelauto = csInvalidStringID;
csStringID celPcWheeled::action_addwheel = csInvalidStringID;
csStringID celPcWheeled::action_deletewheel = csInvalidStringID;
csStringID celPcWheeled::action_deleteallwheels = csInvalidStringID;
csStringID celPcWheeled::action_destroywheel = csInvalidStringID;
csStringID celPcWheeled::action_destroyallwheels = csInvalidStringID;
csStringID celPcWheeled::action_restorewheel = csInvalidStringID;
csStringID celPcWheeled::action_restoreallwheels = csInvalidStringID;

csStringID celPcWheeled::action_accelerate = csInvalidStringID;
csStringID celPcWheeled::action_brake = csInvalidStringID;
csStringID celPcWheeled::action_handbrake = csInvalidStringID;
csStringID celPcWheeled::action_setsteeramount = csInvalidStringID;
csStringID celPcWheeled::action_steerleft = csInvalidStringID;
csStringID celPcWheeled::action_steerright = csInvalidStringID;
csStringID celPcWheeled::action_steerstraight = csInvalidStringID;
csStringID celPcWheeled::action_reverse = csInvalidStringID;
csStringID celPcWheeled::action_neutral = csInvalidStringID;
csStringID celPcWheeled::action_setautotransmission = csInvalidStringID;
csStringID celPcWheeled::action_setgear = csInvalidStringID;
csStringID celPcWheeled::action_setgearsettings = csInvalidStringID;
csStringID celPcWheeled::action_setbrakeforce = csInvalidStringID;
csStringID celPcWheeled::action_setautoreverse = csInvalidStringID;

//Presets
csStringID celPcWheeled::action_setfrontwheelpreset = csInvalidStringID;
csStringID celPcWheeled::action_setrearwheelpreset = csInvalidStringID;
csStringID celPcWheeled::action_setouterwheelsteerpreset = csInvalidStringID;

//Per-wheel actions
csStringID celPcWheeled::action_setwheelposition = csInvalidStringID;
csStringID celPcWheeled::action_setwheelsuspensionsoftness = csInvalidStringID;
csStringID celPcWheeled::action_setwheelsuspensiondamping = csInvalidStringID;
csStringID celPcWheeled::action_setwheelleftsteersensitivity
	= csInvalidStringID;
csStringID celPcWheeled::action_setwheelrightsteersensitivity
	= csInvalidStringID;
csStringID celPcWheeled::action_setwheelturnspeed = csInvalidStringID;
csStringID celPcWheeled::action_setwheelreturnspeed = csInvalidStringID;
csStringID celPcWheeled::action_setwheelenginepower = csInvalidStringID;
csStringID celPcWheeled::action_setwheelbrakepower = csInvalidStringID;
csStringID celPcWheeled::action_setwheelsteerinverted = csInvalidStringID;
csStringID celPcWheeled::action_setwheelhandbrakeaffected = csInvalidStringID;

// Parameters.
csStringID celPcWheeled::param_file = csInvalidStringID;
csStringID celPcWheeled::param_name = csInvalidStringID;
csStringID celPcWheeled::param_position = csInvalidStringID;
csStringID celPcWheeled::param_wheelnum = csInvalidStringID;
csStringID celPcWheeled::param_gear = csInvalidStringID;
csStringID celPcWheeled::param_velocity = csInvalidStringID;
csStringID celPcWheeled::param_force = csInvalidStringID;
csStringID celPcWheeled::param_number = csInvalidStringID;
csStringID celPcWheeled::param_tankmode = csInvalidStringID;
csStringID celPcWheeled::param_steeramount = csInvalidStringID;
csStringID celPcWheeled::param_brakeforce = csInvalidStringID;
csStringID celPcWheeled::param_applied = csInvalidStringID;
csStringID celPcWheeled::param_autotransmission = csInvalidStringID;
csStringID celPcWheeled::param_autoreverse = csInvalidStringID;

csStringID celPcWheeled::param_suspensionsoftness = csInvalidStringID;
csStringID celPcWheeled::param_suspensiondamping = csInvalidStringID;
csStringID celPcWheeled::param_leftsteersensitivity = csInvalidStringID;
csStringID celPcWheeled::param_rightsteersensitivity = csInvalidStringID;
csStringID celPcWheeled::param_steersensitivity = csInvalidStringID;
csStringID celPcWheeled::param_turnspeed = csInvalidStringID;
csStringID celPcWheeled::param_returnspeed = csInvalidStringID;
csStringID celPcWheeled::param_enginepower = csInvalidStringID;
csStringID celPcWheeled::param_brakepower = csInvalidStringID;
csStringID celPcWheeled::param_steerinverted = csInvalidStringID;
csStringID celPcWheeled::param_handbrakeaffected = csInvalidStringID;

celPcWheeled::celPcWheeled (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  dyn=0;
  bodyGroup=0;
  steerdir=0;
  gear=1;
  numberwheels=0;
  topgear=0;
  frontsteer=1.0f;
  rearsteer=0.0f;
  outersteer=1.0f;
  frontpower=1.0f;
  rearpower=1.0f;
  frontss=0.000125;
  frontsd=0.125;
  rearss=0.000125;
  rearsd=0.125;

  autotransmission=true;
  brakeapplied=false;
  handbrakeapplied=false;
  autotransmission=true;
  autoreverse=true;
  accelerating=false;
  wheelradius=0;

  steeramount=0.7;

  gears.SetSize(3);

  //Gear -1 is reverse, 0 is neutral
  SetGearSettings(-1,-25,3000);
  SetGearSettings(0,0,100);
  SetGearSettings(1,150,2000);
  brakeforce=1000;

  tankmode=false;

  if(action_setwheelmesh==csInvalidStringID)
  {
    // Actions
    action_setwheelmesh= pl->FetchStringID("cel.action.SetWheelMesh");
    action_settankmode= pl->FetchStringID("cel.action.SetTankMode");
    action_addwheelauto= pl->FetchStringID("cel.action.AddWheelAuto");
    action_addwheel= pl->FetchStringID("cel.action.AddWheel");
    action_deletewheel= pl->FetchStringID("cel.action.DeleteWheel");
    action_deleteallwheels= pl->FetchStringID("cel.action.DeleteAllWheels");
    action_destroywheel= pl->FetchStringID("cel.action.DestroyWheel");
    action_destroyallwheels=
        pl->FetchStringID("cel.action.DestroyAllWheels");
    action_restorewheel = pl->FetchStringID("cel.action.RestoreWheel");
    action_restoreallwheels=
        pl->FetchStringID("cel.action.RestoreAllWheels");

    action_accelerate= pl->FetchStringID("cel.action.Accelerate");
    action_brake= pl->FetchStringID("cel.action.Brake");
    action_handbrake= pl->FetchStringID("cel.action.Handbrake");
    action_setsteeramount= pl->FetchStringID("cel.action.SetSteerAmount");
    action_steerleft= pl->FetchStringID("cel.action.SteerLeft");
    action_steerright= pl->FetchStringID("cel.action.SteerRight");
    action_steerstraight= pl->FetchStringID("cel.action.SteerStraight");
    action_reverse= pl->FetchStringID("cel.action.Reverse");
    action_neutral= pl->FetchStringID("cel.action.Neutral");
    action_setautotransmission=
        pl->FetchStringID("cel.action.SetAutoTransmission");
    action_setgear= pl->FetchStringID("cel.action.SetGear");
    action_setgearsettings= pl->FetchStringID("cel.action.SetGearSettings");
    action_setbrakeforce= pl->FetchStringID("cel.action.SetBrakeForce");
    action_setautoreverse= pl->FetchStringID("cel.action.SetAutoReverse");

    //Presets
    action_setfrontwheelpreset=
        pl->FetchStringID("cel.action.SetFrontWheelPreset");
    action_setrearwheelpreset=
        pl->FetchStringID("cel.action.SetRearWheelPreset");
    action_setouterwheelsteerpreset=
        pl->FetchStringID("cel.action.SetOuterWheelSteerPreset");

    //Per-wheel actions
    action_setwheelposition=
        pl->FetchStringID("cel.action.SetWheelPosition");
    action_setwheelsuspensionsoftness=
        pl->FetchStringID("cel.action.SetWheelSuspensionSoftness");
    action_setwheelsuspensiondamping=
        pl->FetchStringID("cel.action.SetWheelSuspensionDamping");
    action_setwheelleftsteersensitivity=
        pl->FetchStringID("cel.action.SetWheelLeftSteerSensitivity");
    action_setwheelrightsteersensitivity=
        pl->FetchStringID("cel.action.SetWheelRightSteerSensitivity");
    action_setwheelturnspeed=
        pl->FetchStringID("cel.action.SetWheelTurnSpeed");
    action_setwheelreturnspeed=
        pl->FetchStringID("cel.action.SetWheelReturnSpeed");
    action_setwheelenginepower=
        pl->FetchStringID("cel.action.SetWheelEnginePower");
    action_setwheelbrakepower=
        pl->FetchStringID("cel.action.SetWheelBrakePower");
    action_setwheelsteerinverted=
        pl->FetchStringID("cel.action.SetWheelSteerInverted");
    action_setwheelhandbrakeaffected=
        pl->FetchStringID("cel.action.SetWheelHandbrakeAffected");


    // Parameters.
    param_file = pl->FetchStringID("cel.parameter.file");
    param_name = pl->FetchStringID("cel.parameter.name");
    param_position = pl->FetchStringID("cel.parameter.position");
    param_wheelnum = pl->FetchStringID("cel.parameter.wheelnum");
    param_gear = pl->FetchStringID("cel.parameter.gear");
    param_velocity = pl->FetchStringID("cel.parameter.velocity");
    param_force = pl->FetchStringID("cel.parameter.force");
    param_number = pl->FetchStringID("cel.parameter.number");
    param_tankmode = pl->FetchStringID("cel.parameter.tankmode");
    param_steeramount = pl->FetchStringID("cel.parameter.steeramount");
    param_brakeforce = pl->FetchStringID("cel.parameter.brakeforce");
    param_autotransmission =
        pl->FetchStringID("cel.parameter.autotransmission");
    param_autoreverse = pl->FetchStringID("cel.parameter.autoreverse");
    param_applied = pl->FetchStringID("cel.parameter.applied");

    param_suspensionsoftness =
        pl->FetchStringID("cel.parameter.suspensionsoftness");
    param_suspensiondamping =
        pl->FetchStringID("cel.parameter.suspensiondamping");
    param_steersensitivity =
        pl->FetchStringID("cel.parameter.steersensitivity");
    param_leftsteersensitivity =
        pl->FetchStringID("cel.parameter.leftsteersensitivity");
    param_rightsteersensitivity =
        pl->FetchStringID("cel.parameter.rightsteersensitivity");
    param_steersensitivity =
        pl->FetchStringID("cel.parameter.steersensitivity");
    param_turnspeed = pl->FetchStringID("cel.parameter.turnspeed");
    param_returnspeed = pl->FetchStringID("cel.parameter.returnspeed");
    param_enginepower = pl->FetchStringID("cel.parameter.enginepower");
    param_brakepower = pl->FetchStringID("cel.parameter.brakepower");
    param_steerinverted = pl->FetchStringID("cel.parameter.steerinverted");
    param_handbrakeaffected =
        pl->FetchStringID("cel.parameter.handbrakeaffected");
  }
  pl->CallbackOnce ((iCelTimerListener*)this, 100, CEL_EVENT_PRE);
}

celPcWheeled::~celPcWheeled ()
{
  DestroyAllWheels();
  bodyMech=0;
  wheelfact=0;
  bodyGroup=0;
  engine=0;
  dyn=0;
  osys=0;
  gears=0;
  wheels=0;
}


#define TEST_SERIAL 2

csPtr<iCelDataBuffer> celPcWheeled::Save ()
{
  return 0;
}

bool celPcWheeled::Load (iCelDataBuffer* databuf)
{
  return true;
}

bool celPcWheeled::PerformAction (csStringID actionId,
                                    iCelParameterBlock* params,
                                    celData& ret)
{
  if(actionId==action_setwheelmesh)
  {
    CEL_FETCH_STRING_PAR (factname, params, param_name);
    CEL_FETCH_STRING_PAR (filename, params, param_file);
    SetWheelMesh(factname,filename);
    return true;
  }
  else if(actionId==action_settankmode)
  {
    CEL_FETCH_BOOL_PAR (mode, params, param_tankmode);
    SetTankMode(mode);
    return true;
  }
  else if(actionId==action_addwheelauto)
  {
    CEL_FETCH_VECTOR3_PAR (pos, params, param_position);
    AddWheelAuto(pos);
    return true;
  }
  else if(actionId==action_addwheel)
  {
    CEL_FETCH_VECTOR3_PAR (pos, params, param_position);

    CEL_FETCH_FLOAT_PAR(turnspeed, params, param_turnspeed);
    if(!p_turnspeed)
      turnspeed=2.0f;

    CEL_FETCH_FLOAT_PAR(returnspeed, params, param_returnspeed);
    if(!p_returnspeed)
      returnspeed=2.0f;

    CEL_FETCH_FLOAT_PAR(ss, params, param_suspensionsoftness);
    if(!p_ss)
      ss=0.000125f;

    CEL_FETCH_FLOAT_PAR(sd, params, param_suspensiondamping);
    if(!p_sd)
      sd=0.125f;

    CEL_FETCH_FLOAT_PAR(brakepower, params, param_brakepower);
    if(!p_brakepower)
      brakepower=1.0f;

    CEL_FETCH_FLOAT_PAR(enginepower, params, param_enginepower);
    if(!p_enginepower)
      enginepower=1.0f;

    CEL_FETCH_FLOAT_PAR(lss, params, param_leftsteersensitivity);
    if(!p_lss)
      lss=1.0f;

    CEL_FETCH_FLOAT_PAR(rss, params, param_rightsteersensitivity);
    if(!p_rss)
      rss=1.0f;

    CEL_FETCH_BOOL_PAR(hbaffect, params, param_handbrakeaffected);
    if(!p_hbaffect)
      hbaffect=false;

    CEL_FETCH_BOOL_PAR(sinvert, params, param_steerinverted);
    if(!p_sinvert)
      sinvert=false;

    AddWheel(pos,turnspeed,returnspeed,ss,sd,brakepower,enginepower,
        lss,rss,hbaffect,sinvert);
    return true;
  }
  else if(actionId==action_deletewheel)
  {
    CEL_FETCH_FLOAT_PAR (num, params, param_wheelnum);
    DeleteWheel(int(num));
    return true;
  }
  else if(actionId==action_deleteallwheels)
  {
    DeleteAllWheels();
    return true;
  }
  else if(actionId==action_destroywheel)
  {
    CEL_FETCH_FLOAT_PAR (num, params, param_wheelnum);
    DestroyWheel(int (num));
    return true;
  }
  else if(actionId==action_destroyallwheels)
  {
    DestroyAllWheels();
    return true;
  }
  else if(actionId==action_restorewheel)
  {
    CEL_FETCH_FLOAT_PAR (num, params, param_wheelnum);
    RestoreWheel(int(num));
    return true;
  }
  else if(actionId==action_restoreallwheels)
  {
    RestoreAllWheels();
    return true;
  }

  else if(actionId==action_accelerate)
  {
    CEL_FETCH_BOOL_PAR (applied, params, param_applied);
    Accelerate(applied);
    return true;
  }
  else if(actionId==action_brake)
  {
    CEL_FETCH_BOOL_PAR (applied, params, param_applied);
    Brake(applied);
    return true;
  }
  else if(actionId==action_handbrake)
  {
    CEL_FETCH_BOOL_PAR (applied, params, param_applied);
    Handbrake(applied);
    return true;
  }
  else if(actionId==action_setsteeramount)
  {
    CEL_FETCH_FLOAT_PAR (amount, params, param_steeramount);
    SetSteerAmount(amount);
    return true;
  }
  else if(actionId==action_steerleft)
  {
    SteerLeft();
    return true;
  }
  else if(actionId==action_steerright)
  {
    SteerRight();
    return true;
  }
  else if(actionId==action_steerstraight)
  {
    SteerStraight();
    return true;
  }
  else if(actionId==action_reverse)
  {
    Reverse();
    return true;
  }
  else if(actionId==action_neutral)
  {
    Neutral();
    return true;
  }
  else if(actionId==action_setautotransmission)
  {
    CEL_FETCH_BOOL_PAR(trans,params,param_autotransmission);
    SetAutoTransmission(trans);
    return true;
  }
  else if(actionId==action_setgear)
  {
    CEL_FETCH_FLOAT_PAR(g,params,param_gear);
    SetGear(int(g));
    return true;
  }
  else if(actionId==action_setgearsettings)
  {
    CEL_FETCH_FLOAT_PAR(force,params,param_force);
    CEL_FETCH_FLOAT_PAR(velocity,params,param_velocity);
    CEL_FETCH_FLOAT_PAR(g,params,param_gear);
    SetGearSettings(int(g),velocity,force);
    return true;
  }
  else if(actionId==action_setbrakeforce)
  {
    CEL_FETCH_FLOAT_PAR(force,params,param_brakeforce);
    SetBrakeForce(force);
    return true;
  }
  else if(actionId==action_setautoreverse)
  {
    CEL_FETCH_BOOL_PAR(reverse,params,param_autoreverse);
    SetAutoReverse(reverse);
    return true;
  }
  //Presets
  else if(actionId==action_setfrontwheelpreset)
  {
    CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
    CEL_FETCH_FLOAT_PAR(power,params,param_enginepower);
    CEL_FETCH_FLOAT_PAR(ss,params,param_suspensionsoftness);
    CEL_FETCH_FLOAT_PAR(sd,params,param_suspensiondamping);
    SetFrontWheelPreset(sens,power,ss,sd);
    return true;
  }
  else if(actionId==action_setrearwheelpreset)
  {
    CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
    CEL_FETCH_FLOAT_PAR(power,params,param_enginepower);
    CEL_FETCH_FLOAT_PAR(ss,params,param_suspensionsoftness);
    CEL_FETCH_FLOAT_PAR(sd,params,param_suspensiondamping);
    SetRearWheelPreset(sens,power,ss,sd);
    return true;
  }
  else if(actionId==action_setouterwheelsteerpreset)
  {
    CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
    SetOuterWheelSteerPreset(sens);
    return true;
  }
    //Per-wheel actions
  else if(actionId==action_setwheelposition)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_VECTOR3_PAR(pos,params,param_position);
    SetWheelPosition(int(num),pos);
    return true;
  }
  else if(actionId==action_setwheelsuspensionsoftness)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(softness,params,param_suspensionsoftness);
    SetWheelSuspensionSoftness(int(num),softness);
    return true;
  }
  else if(actionId==action_setwheelsuspensiondamping)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(damping,params,param_suspensiondamping);
    SetWheelSuspensionDamping(int(num),damping);
    return true;
  }
  else if(actionId==action_setwheelleftsteersensitivity)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
    SetWheelLeftSteerSensitivity(int(num),sens);
    return true;
  }
  else if(actionId==action_setwheelrightsteersensitivity)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
    SetWheelRightSteerSensitivity(int(num),sens);
    return true;
  }
  else if(actionId==action_setwheelturnspeed)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(speed,params,param_turnspeed);
    SetWheelTurnSpeed(int(num),speed);
    return true;
  }
  else if(actionId==action_setwheelreturnspeed)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(speed,params,param_returnspeed);
    SetWheelReturnSpeed(int(num),speed);
    return true;
  }
  else if(actionId==action_setwheelenginepower)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(power,params,param_enginepower);
    SetWheelEnginePower(int(num),power);
    return true;
  }
  else if(actionId==action_setwheelbrakepower)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(power,params,param_brakepower);
    SetWheelBrakePower(int(num),power);
    return true;
  }
  else if(actionId==action_setwheelsteerinverted)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_BOOL_PAR(invert,params,param_steerinverted);
    SetWheelSteerInverted(int(num),invert);
    return true;
  }
  else if(actionId==action_setwheelhandbrakeaffected)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_BOOL_PAR(affect,params,param_handbrakeaffected);
    SetWheelHandbrakeAffected(int(num),affect);
    return true;
  }
  return false;
}

void celPcWheeled::SetWheelMesh(const char* factname,const char* file)
{
  if(file!=0)
  {
    csRef<iLoader> loader = CS_QUERY_REGISTRY (object_reg, iLoader);
    CS_ASSERT (loader != 0);
    iBase* result;
    loader->Load (file, result, 0, false, true);
  }
  wheelfact=engine->FindMeshFactory(factname);
}

//This method uses the vehicle's presets and wheel's position for settings
int celPcWheeled::AddWheelAuto(csVector3 position)
{
  celWheel wheel;
  wheel.Position=position;
  wheel.TurnSpeed=2;
  wheel.ReturnSpeed=2;
  wheel.BrakePower=1;
  wheels.Push(wheel);
  ApplyWheelPresets(wheels.Length()-1);
  int index=wheels.Length()-1;
  RestoreWheel(index);
  return index;
}

int celPcWheeled::AddWheel(csVector3 position,float turnspeed, float
      returnspeed, float ss, float sd,float brakepower,float enginepower,
      float lss, float rss,bool hbaffect, bool sinvert)
{
  celWheel wheel;
  wheel.Position=position;
  wheel.TurnSpeed=turnspeed;
  wheel.ReturnSpeed=returnspeed;
  wheel.SuspensionSoftness=ss;
  wheel.SuspensionDamping=sd;
  wheel.BrakePower=brakepower;
  wheel.EnginePower=enginepower;
  wheel.LeftSteerSensitivity=lss;
  wheel.RightSteerSensitivity=rss;
  wheel.HandbrakeAffected=hbaffect;
  wheel.SteerInverted=sinvert;
  wheels.Push(wheel);
  int index=wheels.Length()-1;
  RestoreWheel(index);
  return index;
}

void celPcWheeled::DestroyWheel(int wheelnum)
{
  GetMech();
  if(!bodyGroup || !bodyMech) return;

  if (wheels[wheelnum].WheelJoint!=0)
  {
    osys->RemoveJoint(wheels[wheelnum].WheelJoint);
    wheels[wheelnum].WheelJoint=0;
  }
  if (wheels[wheelnum].RigidBody!=0)
  {
    csRef<iMeshWrapper> mesh =
        wheels[wheelnum].RigidBody->GetAttachedMesh();
    engine->WantToDie(mesh);
    bodyGroup->RemoveBody(wheels[wheelnum].RigidBody);
    dyn->RemoveBody(wheels[wheelnum].RigidBody);
    wheels[wheelnum].RigidBody=0;
  }
}

void celPcWheeled::DestroyAllWheels()
{
  for(size_t i=0;i < wheels.Length();i++)
  {
    DestroyWheel(i);
  }
}

void celPcWheeled::DeleteWheel(int wheelnum)
{
  DestroyWheel(wheelnum);
  wheels.DeleteIndex(wheelnum);
}

void celPcWheeled::DeleteAllWheels()
{
  DestroyAllWheels();
  wheels.DeleteAll();
}

void celPcWheeled::RestoreWheel(int wheelnum)
{
  GetMech();
  //Create the mesh
  csRef<iPcMesh> bodyMesh=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMesh);
  csOrthoTransform
      bodytransform=bodyMesh->GetMesh()->GetMovable()->GetTransform();
  csRef<iMeshWrapper> wheelmesh=0;
  csRef<iSectorList>
      bodySectors=bodyMesh->GetMesh()->GetMovable()->GetSectors();
  if(bodySectors->GetCount() > 0)
  {
    csRef<iSector> bodySector=bodySectors->Get(0);

    wheelmesh=engine->CreateMeshWrapper(wheelfact,"wheel",bodySector,
                                        wheels[wheelnum].Position);
  }
  else
  {
    wheelmesh=engine->CreateMeshWrapper(wheelfact,"wheel");
    wheelmesh->GetMovable()->SetPosition(wheels[wheelnum].Position);
    wheelmesh->GetMovable()->UpdateMove();
  }

  //Create the dynamic body
  csRef<iRigidBody> wheelbody=dyn->CreateBody();
  bodyGroup->AddBody(wheelbody);

  csVector3 wheelcenter;
  wheelmesh->GetMeshObject ()->GetObjectModel
      ()->GetRadius(wheelradius,wheelcenter);
  wheelbody->SetProperties (10, csVector3 (0), csMatrix3 ());

  wheelbody->SetPosition(bodytransform.This2Other(
      wheels[wheelnum].Position));
  wheelbody->AttachMesh(wheelmesh);

  wheelbody->AttachColliderSphere (
      wheelradius,wheelcenter,0.8f,1,0.5f,0.05f);
  //If it a right wheel, flip it.
  if (wheels[wheelnum].Position.x<0)
  {
    csOrthoTransform t=wheelbody->GetTransform();
    t.RotateThis(csVector3(0,1,0),3.14f);
    wheelbody->SetTransform(t);
  }

    //Create the joint
  csRef<iODEHinge2Joint> joint=osys->CreateHinge2Joint();
  joint->Attach(bodyMech->GetBody(),wheelbody);

  joint->SetHingeAnchor(bodytransform.This2Other
      (wheels[wheelnum].Position));
  joint->SetHingeAxis1(csVector3(0,1,0));
  joint->SetHingeAxis2(csVector3(1,0,0));
  joint->SetSuspensionCFM(wheels[wheelnum].SuspensionSoftness,0);
  joint->SetSuspensionERP(wheels[wheelnum].SuspensionDamping,0);
  joint->SetLoStop(0,0);
  joint->SetHiStop(0,0);
  joint->SetVel(0,0);
  joint->SetVel(0,1);
  joint->SetStopERP(1.0,0);
  joint->SetFMax(1000,0);
  joint->SetFMax(100,1);

  wheels[wheelnum].RigidBody=wheelbody;
  wheels[wheelnum].WheelJoint=joint;
}

void celPcWheeled::RestoreAllWheels()
{
  for(size_t i=0; i < wheels.Length();i++)
  {
    if(wheels[i].RigidBody==0)
      RestoreWheel(i);
  }
}



void celPcWheeled::SteerLeft()
{
  steerdir=-1;
  if(!tankmode)
  {
    for(size_t i=0;i < wheels.Length();i++)
    {
      if(wheels[i].WheelJoint!=0)
      {
         //Not inverted, so turn the wheel left
        if(!wheels[i].SteerInverted)
        {
          float histop=steeramount*wheels[i].RightSteerSensitivity;
          wheels[i].WheelJoint->SetLoStop(0,0);
          wheels[i].WheelJoint->SetHiStop(histop,0);
          wheels[i].WheelJoint->SetVel(wheels[i].TurnSpeed,0);
        }
        /*Inverted, so turn the wheel right. The car is still steering
        left though,
        so leftsteersensitivity is still used.*/
        else
        {
          float lostop=-steeramount*wheels[i].RightSteerSensitivity;
          wheels[i].WheelJoint->SetLoStop(lostop,0);
          wheels[i].WheelJoint->SetHiStop(0,0);
          wheels[i].WheelJoint->SetVel(-wheels[i].TurnSpeed,0);
        }
      }
    }
  }
}

void celPcWheeled::SteerRight()
{
  steerdir=1;
  if(!tankmode)
  {
    for(size_t i=0;i < wheels.Length();i++)
    {
      if(wheels[i].WheelJoint!=0)
      {
         //Not inverted, so Steer it right.
        if(!wheels[i].SteerInverted)
        {
          float lostop=-steeramount*wheels[i].RightSteerSensitivity;
          wheels[i].WheelJoint->SetLoStop(lostop,0);
          wheels[i].WheelJoint->SetHiStop(0,0);
          wheels[i].WheelJoint->SetVel(-wheels[i].TurnSpeed,0);
        }

        /*Inverted, so turn the wheel left. The car is still steering
        right though,
        so rightsteersensitivity is still used.*/
        else
        {
          float histop=steeramount*wheels[i].RightSteerSensitivity;
          wheels[i].WheelJoint->SetLoStop(0,0);
          wheels[i].WheelJoint->SetHiStop(histop,0);
          wheels[i].WheelJoint->SetVel(wheels[i].TurnSpeed,0);
        }
      }
    }
  }
}

void celPcWheeled::SteerStraight()
{
  //It was steering right, bring it left.
  if (steerdir==1)
  {
    for(size_t i=0;i < wheels.Length();i++)
    {
      if(wheels[i].WheelJoint!=0)
      {
        if(!wheels[i].SteerInverted)
        {
          wheels[i].WheelJoint->SetHiStop(0,0);
          wheels[i].WheelJoint->SetVel(wheels[i].ReturnSpeed,0);
        }
        else
        {
          wheels[i].WheelJoint->SetLoStop(0,0);
          wheels[i].WheelJoint->SetVel(-wheels[i].ReturnSpeed,0);
        }
      }
    }
  }
  //It was steering left, bring it right.
  if (steerdir==-1)
  {
    for(size_t i=0;i < wheels.Length();i++)
    {
      if(wheels[i].WheelJoint!=0)
      {
        if(!wheels[i].SteerInverted)
        {
          wheels[i].WheelJoint->SetLoStop(0,0);
          wheels[i].WheelJoint->SetVel(-wheels[i].ReturnSpeed,0);
        }
        else
        {
          wheels[i].WheelJoint->SetHiStop(0,0);
          wheels[i].WheelJoint->SetVel(wheels[i].ReturnSpeed,0);
        }
      }
    }
  }
  steerdir=0;
}

void celPcWheeled::UpdateTankSteer()
{
  //The tank steers by braking one side of the wheels
  //  if(gear!=0)
  for(size_t i =0; i < wheels.Length() ; i++)
  {
    if(wheels[i].WheelJoint!=0)
    {
          //It's a right wheel, steering right. slow it down
      if (wheels[i].Position.x < 0 && steerdir > 0)
      {
        wheels[i].WheelJoint->SetVel(0,1);
        wheels[i].WheelJoint->SetFMax(wheels[i].BrakePower*brakeforce,1);
      }
          //It's a left wheel, steering left. slow it down
      if (wheels[i].Position.x > 0  && steerdir < 0)
      {
        wheels[i].WheelJoint->SetVel(0,1);
        wheels[i].WheelJoint->SetFMax(wheels[i].BrakePower*brakeforce,1);
      }
    }
  }
}

void celPcWheeled::Brake (bool on)
{
  brakeapplied=on;
  if (gear == -1)
  {
    accelerating = false;
    gear = 1;
  }
}

void celPcWheeled::GetMech()
{
  if(!bodyMech)
  {
    bodyMech=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMechanicsObject);
    if(!bodyMech)
      return;
    dyn=bodyMech->GetMechanicsSystem()->GetDynamicSystem();
    osys=SCF_QUERY_INTERFACE (dyn, iODEDynamicSystemState);
    bodyGroup=dyn->CreateGroup();
    bodyGroup->AddBody(bodyMech->GetBody());
  }
}
//
//Update the vehicle. Order is important here! first comes acceleration,
//then braking, then handbrake. tank steering comes last.
void celPcWheeled::TickOnce()
{
  //First ensure everything is set and ready to go.
  GetMech();

  //Dont try to work out the gear in neutral or reverse.
  if(gear > 0 && autotransmission)
    UpdateGear();

  //Update the wheel's speeds to the current gear if accelerating. else
  //use the neutral gear settings.
  float vel=gears[1].x;
  float fmax=gears[1].y;
  if(accelerating)
  {
    vel=gears[gear+1].x;
    fmax=gears[gear+1].y;
  }
  for(size_t i=0; i < wheels.Length();i++)
  {
    if(wheels[i].WheelJoint!=0)
    {
      wheels[i].WheelJoint->SetVel(vel,1);
      wheels[i].WheelJoint->SetFMax(fmax*wheels[i].EnginePower,1);
    }
  }
    //Apply the brakes
  if(brakeapplied)
  {
      //Don't brake if in reverse
    if(gear!=-1)
    {
      for(size_t i=0; i < wheels.Length();i++)
      {
        if(wheels[i].WheelJoint!=0)
        {
          wheels[i].WheelJoint->SetVel(0,1);
          wheels[i].WheelJoint->SetFMax(brakeforce*wheels[i].BrakePower,1);
        }
      }
    }
      //if autoreverse is on, check if the vehicle is slow enough to start
      // reversing.
    if (autoreverse)
    {
      if(bodyMech->GetBody()->GetLinearVelocity().Norm()<2)
        Reverse();
    }
  }
    //Apply the handbrake
  if(handbrakeapplied)
  {
    for(size_t i=0; i < wheels.Length();i++)
    {
      if(wheels[i].WheelJoint!=0 && wheels[i].HandbrakeAffected)
      {
        wheels[i].WheelJoint->SetVel(0,1);
        wheels[i].WheelJoint->SetFMax
            (brakeforce*wheels[i].BrakePower*1000,1);
      }
    }
  }

  if(tankmode && steerdir!=0)
    UpdateTankSteer();

  pl->CallbackOnce ((iCelTimerListener*)this, 100, CEL_EVENT_PRE);
}

void celPcWheeled::SetGear(int gear)
{
  if (gear>=-1 && gear <= topgear)
    celPcWheeled::gear=gear;
}

void celPcWheeled::SetGearSettings(int gear, float velocity, float
        force)
{
  //Set the number of gears to the top + 2, to make way for neutral and
  //reverse.
  if(gear > topgear)
  {
    gears.SetSize(gear+2);
    topgear = gear;
  }
  if (gear >=-1)
  {
    gears[gear+1].x=velocity;
    gears[gear+1].y=force;
  }
}

void celPcWheeled::UpdateGear()
{
  for(int i=0; i < topgear; i++)
  {
    if (bodyMech->GetBody()->GetLinearVelocity().Norm()
        >= (gears[i+1].x)*wheelradius*wheelradius*3.14-2 )
    {
      gear=i+1;
    }
  }
}

void celPcWheeled::SetWheelPosition(int wheelnum, csVector3 position)
{
  wheels[wheelnum].Position=position;
    //If the wheel is already created, have to move it's body aswell.
  if(wheels[wheelnum].WheelJoint!=0)
  {

    wheels[wheelnum].WheelJoint->
        SetHingeAnchor(bodyMech->LocalToWorld(position));
  }
}

void celPcWheeled::SetWheelSuspensionSoftness(int wheelnum, float
        softness)
{
  wheels[wheelnum].SuspensionSoftness=softness;
    //If the wheel is already created, have to set its joint too.
  if(wheels[wheelnum].WheelJoint!=0)
  {
    wheels[wheelnum].WheelJoint->SetSuspensionCFM(softness,0);
  }
}

void celPcWheeled::SetWheelSuspensionDamping(int wheelnum, float
        damping)
{
  wheels[wheelnum].SuspensionDamping=damping;
    //If the wheel is already created, have to set its joint too.
  if(wheels[wheelnum].WheelJoint!=0)
  {
    wheels[wheelnum].WheelJoint->SetSuspensionERP(damping,0);
  }
}

void celPcWheeled::SetFrontWheelPreset(float sensitivity,float enginepower,
                                       float suspensionsoftness,
                                       float suspensiondamping)
{
  if (rearsteer>=0 && rearsteer<=1)
    celPcWheeled::frontsteer=sensitivity;
  if(enginepower>=0 && enginepower <=1)
    celPcWheeled::frontpower=enginepower;
  celPcWheeled::frontss=suspensionsoftness;
  celPcWheeled::frontsd=suspensiondamping;
  for(size_t i=0; i++; i < wheels.Length())
  {
    ApplyWheelPresets(i);
  }
}

void celPcWheeled::SetRearWheelPreset(float sensitivity,float enginepower,
                                      float suspensionsoftness,
                                      float suspensiondamping)
{
  if (rearsteer>=0 && rearsteer<=1)
    celPcWheeled::rearsteer=sensitivity;
  if(enginepower>=0 && enginepower <=1)
    celPcWheeled::rearpower=enginepower;
  celPcWheeled::rearss=suspensionsoftness;
  celPcWheeled::rearsd=suspensiondamping;

  for(size_t i=0 ;i++; i<wheels.Length())
  {
    ApplyWheelPresets(i);
  }
}

void celPcWheeled::SetOuterWheelSteerPreset(float sensitivity)
{
  if(outersteer>=0 && outersteer<=1)
    celPcWheeled::outersteer=sensitivity;

  for(size_t i=0; i++; i<wheels.Length())
  {
    ApplyWheelPresets(i);
  }
}

void celPcWheeled::ApplyWheelPresets(int wheelnum)
{
  //Apply front and rear steer and power settings
  if(wheels[wheelnum].Position.z<0)
  {
    wheels[wheelnum].LeftSteerSensitivity=frontsteer;
    wheels[wheelnum].RightSteerSensitivity=frontsteer;
    wheels[wheelnum].EnginePower=frontpower;
    wheels[wheelnum].SuspensionSoftness=frontss;
    wheels[wheelnum].SuspensionDamping=frontsd;
    wheels[wheelnum].SteerInverted=false;
    wheels[wheelnum].HandbrakeAffected=false;
  }
  else if(wheels[wheelnum].Position.z>0)
  {
    wheels[wheelnum].LeftSteerSensitivity=rearsteer;
    wheels[wheelnum].RightSteerSensitivity=rearsteer;
    wheels[wheelnum].EnginePower=rearpower;
    wheels[wheelnum].SuspensionSoftness=rearss;
    wheels[wheelnum].SuspensionDamping=rearsd;
    wheels[wheelnum].SteerInverted=true;
    wheels[wheelnum].HandbrakeAffected=true;
  }
  else
  {
    wheels[wheelnum].LeftSteerSensitivity=0.0f;
    wheels[wheelnum].RightSteerSensitivity=0.0f;
    wheels[wheelnum].EnginePower=1.0f;
    wheels[wheelnum].SuspensionSoftness=frontss;
    wheels[wheelnum].SuspensionDamping=frontsd;
    wheels[wheelnum].SteerInverted=false;
    wheels[wheelnum].HandbrakeAffected=false;
  }
  //Apply outer steer settings
      //A right wheel. So its sensitivity when steering left is
  //reduced.
  if (wheels[wheelnum].Position.x<0)
    wheels[wheelnum].LeftSteerSensitivity*=outersteer;
      //A left wheel. So its sensitivity when steering right is
      //reduced.
  if (wheels[wheelnum].Position.x>0)
    wheels[wheelnum].RightSteerSensitivity*=outersteer;
}

//-------------------------------------------------------------------------

