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
#include "csgeom/quaternion.h"
#include "imesh/objmodel.h"
#include "imesh/object.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "iengine/engine.h"
#include "imap/loader.h"

#include "propclass/mesh.h"
#include "propclass/mechsys.h"

#include <cmath>

//--------------------------------------------------------------------------


CS_IMPLEMENT_PLUGIN
CEL_IMPLEMENT_FACTORY (Wheeled, "pcwheeled")


//--------------------------------------------------------------------------

// Parameters.
csStringID celPcWheeled::param_meshfile = csInvalidStringID;
csStringID celPcWheeled::param_meshfact = csInvalidStringID;
csStringID celPcWheeled::param_position = csInvalidStringID;
csStringID celPcWheeled::param_rotation = csInvalidStringID;
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

PropertyHolder celPcWheeled::propinfo;

celPcWheeled::celPcWheeled (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  engine = csQueryRegistry<iEngine> (object_reg);
  dyn=0;
  bodyGroup=0;
  steerdir=0;
  gear=1;
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

  if(param_meshfile==csInvalidStringID)
  {
    // Parameters.
    param_meshfile = pl->FetchStringID("cel.parameter.meshfile");
    param_meshfact = pl->FetchStringID("cel.parameter.meshfact");
    param_position = pl->FetchStringID("cel.parameter.position");
    param_rotation = pl->FetchStringID("cel.parameter.rotation");
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

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_setwheelmesh, "cel.action.SetWheelMesh");
    AddAction (action_settankmode, "cel.action.SetTankMode");
    AddAction (action_addwheelauto, "cel.action.AddWheelAuto");
    AddAction (action_addwheel, "cel.action.AddWheel");
    AddAction (action_deletewheel, "cel.action.DeleteWheel");
    AddAction (action_deleteallwheels, "cel.action.DeleteAllWheels");
    AddAction (action_destroywheel, "cel.action.DestroyWheel");
    AddAction (action_destroyallwheels, "cel.action.DestroyAllWheels");
    AddAction (action_restorewheel, "cel.action.RestoreWheel");
    AddAction (action_restoreallwheels, "cel.action.RestoreAllWheels");

    AddAction (action_accelerate, "cel.action.Accelerate");
    AddAction (action_brake, "cel.action.Brake");
    AddAction (action_handbrake, "cel.action.Handbrake");
    AddAction (action_setsteeramount, "cel.action.SetSteerAmount");
    AddAction (action_steerleft, "cel.action.SteerLeft");
    AddAction (action_steerright, "cel.action.SteerRight");
    AddAction (action_steerstraight, "cel.action.SteerStraight");
    AddAction (action_reverse, "cel.action.Reverse");
    AddAction (action_neutral, "cel.action.Neutral");
    AddAction (action_setautotransmission, "cel.action.SetAutoTransmission");
    AddAction (action_setgear, "cel.action.SetGear");
    AddAction (action_setgearsettings, "cel.action.SetGearSettings");
    AddAction (action_setbrakeforce, "cel.action.SetBrakeForce");
    AddAction (action_setautoreverse, "cel.action.SetAutoReverse");

    //Presets
    AddAction (action_setfrontwheelpreset, "cel.action.SetFrontWheelPreset");
    AddAction (action_setrearwheelpreset, "cel.action.SetRearWheelPreset");
    AddAction (action_setouterwheelsteerpreset, "cel.action.SetOuterWheelSteerPreset");

    //Per-wheel actions
    AddAction (action_setwheelposition, "cel.action.SetWheelPosition");
    AddAction (action_setwheelrotation, "cel.action.SetWheelRotation");
    AddAction (action_setwheelsuspensionsoftness, "cel.action.SetWheelSuspensionSoftness");
    AddAction (action_setwheelsuspensiondamping, "cel.action.SetWheelSuspensionDamping");
    AddAction (action_setwheelleftsteersensitivity, "cel.action.SetWheelLeftSteerSensitivity");
    AddAction (action_setwheelrightsteersensitivity, "cel.action.SetWheelRightSteerSensitivity");
    AddAction (action_setwheelturnspeed, "cel.action.SetWheelTurnSpeed");
    AddAction (action_setwheelreturnspeed, "cel.action.SetWheelReturnSpeed");
    AddAction (action_setwheelenginepower, "cel.action.SetWheelEnginePower");
    AddAction (action_setwheelbrakepower, "cel.action.SetWheelBrakePower");
    AddAction (action_setwheelsteerinverted, "cel.action.SetWheelSteerInverted");
    AddAction (action_setwheelhandbrakeaffected, "cel.action.SetWheelHandbrakeAffected");
  }


  pl->CallbackOnce ((iCelTimerListener*)this, 100, CEL_EVENT_PRE);
}

celPcWheeled::~celPcWheeled ()
{
  DestroyAllWheels();
  bodyGroup->RemoveBody(bodyMech->GetBody());
  dyn->RemoveGroup(bodyGroup);
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

bool celPcWheeled::PerformActionIndexed (int idx,
                                    iCelParameterBlock* params,
                                    celData& ret)
{
  switch (idx)
  {
    case action_setwheelmesh:
      {
        CEL_FETCH_STRING_PAR (factname, params, param_meshfact);
        CEL_FETCH_STRING_PAR (filename, params, param_meshfile);
        SetWheelMesh(factname,filename);
        return true;
      }
    case action_settankmode:
      {
        CEL_FETCH_BOOL_PAR (mode, params, param_tankmode);
        SetTankMode(mode);
        return true;
      }
    case action_addwheelauto:
      {
        CEL_FETCH_VECTOR3_PAR (pos, params, param_position);
        CEL_FETCH_VECTOR3_PAR (rotation,params, param_rotation);
        CEL_FETCH_STRING_PAR (factname, params, param_meshfact);
        CEL_FETCH_STRING_PAR (filename, params, param_meshfile);
        csQuaternion quat;
        if(!p_factname)
          factname = 0;
        if(!p_filename)
          filename = 0;
        if (p_rotation)
          quat.SetEulerAngles(rotation);

        AddWheelAuto(pos, factname, filename, quat.GetMatrix());

        return true;
      }
    case action_addwheel:
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

        CEL_FETCH_VECTOR3_PAR (rotation,params, param_rotation);
        CEL_FETCH_STRING_PAR (factname, params, param_meshfact);
        CEL_FETCH_STRING_PAR (filename, params, param_meshfile);
        csQuaternion quat;
        if(!p_factname)
          factname = 0;
        if(!p_filename)
          filename = 0;
        if (p_rotation)
          quat.SetEulerAngles(rotation);

        AddWheel(pos,turnspeed,returnspeed,ss,sd,brakepower,enginepower,
            lss,rss,hbaffect,sinvert, factname, filename, quat.GetMatrix());
        return true;
      }
    case action_deletewheel:
      {
        CEL_FETCH_LONG_PAR (num, params, param_wheelnum);
        DeleteWheel(num);
        return true;
      }
    case action_deleteallwheels:
      DeleteAllWheels();
      return true;
    case action_destroywheel:
      {
        CEL_FETCH_FLOAT_PAR (num, params, param_wheelnum);
        DestroyWheel(int (num));
        return true;
      }
    case action_destroyallwheels:
      DestroyAllWheels();
      return true;
    case action_restorewheel:
      {
        CEL_FETCH_LONG_PAR (num, params, param_wheelnum);
        RestoreWheel(num);
        return true;
      }
    case action_restoreallwheels:
      RestoreAllWheels();
      return true;
    case action_accelerate:
      {
        CEL_FETCH_BOOL_PAR (applied, params, param_applied);
        Accelerate(applied);
        return true;
      }
    case action_brake:
      {
        CEL_FETCH_BOOL_PAR (applied, params, param_applied);
        Brake(applied);
        return true;
      }
    case action_handbrake:
      {
        CEL_FETCH_BOOL_PAR (applied, params, param_applied);
        Handbrake(applied);
        return true;
      }
    case action_setsteeramount:
      {
        CEL_FETCH_FLOAT_PAR (amount, params, param_steeramount);
        SetSteerAmount(amount);
        return true;
      }
    case action_steerleft:
      SteerLeft();
      return true;
    case action_steerright:
      SteerRight();
      return true;
    case action_steerstraight:
      SteerStraight();
      return true;
    case action_reverse:
      Reverse();
      return true;
    case action_neutral:
      Neutral();
      return true;
    case action_setautotransmission:
      {
        CEL_FETCH_BOOL_PAR(trans,params,param_autotransmission);
        SetAutoTransmission(trans);
        return true;
      }
    case action_setgear:
      {
        CEL_FETCH_LONG_PAR(g,params,param_gear);
        SetGear(g);
        return true;
      }
    case action_setgearsettings:
      {
        CEL_FETCH_FLOAT_PAR(force,params,param_force);
        CEL_FETCH_FLOAT_PAR(velocity,params,param_velocity);
        CEL_FETCH_LONG_PAR(g,params,param_gear);
        SetGearSettings(g,velocity,force);
        return true;
      }
    case action_setbrakeforce:
      {
        CEL_FETCH_FLOAT_PAR(force,params,param_brakeforce);
        SetBrakeForce(force);
        return true;
      }
    case action_setautoreverse:
      {
        CEL_FETCH_BOOL_PAR(reverse,params,param_autoreverse);
        SetAutoReverse(reverse);
        return true;
      }
    case action_setfrontwheelpreset:
      {
        CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
        CEL_FETCH_FLOAT_PAR(power,params,param_enginepower);
        CEL_FETCH_FLOAT_PAR(ss,params,param_suspensionsoftness);
        CEL_FETCH_FLOAT_PAR(sd,params,param_suspensiondamping);
        SetFrontWheelPreset(sens,power,ss,sd);
        return true;
      }
    case action_setrearwheelpreset:
      {
        CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
        CEL_FETCH_FLOAT_PAR(power,params,param_enginepower);
        CEL_FETCH_FLOAT_PAR(ss,params,param_suspensionsoftness);
        CEL_FETCH_FLOAT_PAR(sd,params,param_suspensiondamping);
        SetRearWheelPreset(sens,power,ss,sd);
        return true;
      }
    case action_setouterwheelsteerpreset:
      {
        CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
        SetOuterWheelSteerPreset(sens);
        return true;
      }
    case action_setwheelposition:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_VECTOR3_PAR(pos,params,param_position);
        SetWheelPosition(num,pos);
        return true;
      }
    case action_setwheelrotation:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_VECTOR3_PAR(rotation,params,param_rotation);
        csQuaternion quat;
        if (p_rotation)
          quat.SetEulerAngles(rotation);
        SetWheelRotation(num, quat.GetMatrix());
        return true;
      }
    case action_setwheelsuspensionsoftness:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_FLOAT_PAR(softness,params,param_suspensionsoftness);
        SetWheelSuspensionSoftness(int(num),softness);
        return true;
      }
    case action_setwheelsuspensiondamping:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_FLOAT_PAR(damping,params,param_suspensiondamping);
        SetWheelSuspensionDamping(num,damping);
        return true;
      }
    case action_setwheelleftsteersensitivity:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
        SetWheelLeftSteerSensitivity(num,sens);
        return true;
      }
    case action_setwheelrightsteersensitivity:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_FLOAT_PAR(sens,params,param_steersensitivity);
        SetWheelRightSteerSensitivity(num,sens);
        return true;
      }
    case action_setwheelturnspeed:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_FLOAT_PAR(speed,params,param_turnspeed);
        SetWheelTurnSpeed(num, speed);
        return true;
      }
    case action_setwheelreturnspeed:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_FLOAT_PAR(speed,params,param_returnspeed);
        SetWheelReturnSpeed(num, speed);
        return true;
      }
    case action_setwheelenginepower:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_FLOAT_PAR(power,params,param_enginepower);
        SetWheelEnginePower(num, power);
        return true;
      }
    case action_setwheelbrakepower:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_FLOAT_PAR(power,params,param_brakepower);
        SetWheelBrakePower(num,power);
        return true;
      }
    case action_setwheelsteerinverted:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_BOOL_PAR(invert,params,param_steerinverted);
        SetWheelSteerInverted(num, invert);
        return true;
      }
    case action_setwheelhandbrakeaffected:
      {
        CEL_FETCH_LONG_PAR(num,params,param_wheelnum);
        CEL_FETCH_BOOL_PAR(affect,params,param_handbrakeaffected);
        SetWheelHandbrakeAffected(num, affect);
        return true;
      }
    default:
      return false;
  }
}

void celPcWheeled::SetWheelMesh(const char* factname,const char* file)
{
  if(file!=0)
  {
    csRef<iLoader> loader = csQueryRegistry<iLoader> (object_reg);
    CS_ASSERT (loader != 0);
    iBase* result;
    loader->Load (file, result, 0, false, true);
  }
  wheelfact = factname;
}

void celPcWheeled::SetWheelMesh(int wheelnum, const char* factname,const char* file)
{
  if(file!=0)
  {
    csRef<iLoader> loader = csQueryRegistry<iLoader> (object_reg);
    CS_ASSERT (loader != 0);
    iBase* result;
    loader->Load (file, result, 0, false, true);
  }
  //Use the global wheel factory if none given
  if(factname==0)
    wheels[wheelnum].Meshfact = wheelfact;
  else
    wheels[wheelnum].Meshfact = factname;
}

//This method uses the vehicle's presets and wheel's position for settings
int celPcWheeled::AddWheelAuto(csVector3 position, const char* wheelfact,
     const char* wheelfile, csMatrix3 rotation)
{
  celWheel wheel;
  wheel.Position=position;
  wheel.TurnSpeed=2;
  wheel.ReturnSpeed=2;
  wheel.BrakePower=1;
  wheel.Rotation = rotation;
  wheels.Push(wheel);
  int index=wheels.Length()-1;
  ApplyWheelPresets(index);
  SetWheelMesh(index, wheelfact, wheelfile);
  RestoreWheel(index);
  return index;
}

int celPcWheeled::AddWheel(csVector3 position,float turnspeed, float
      returnspeed, float ss, float sd,float brakepower,float enginepower,
      float lss, float rss,bool hbaffect, bool sinvert, const char* wheelfact,
      const char* wheelfile,  csMatrix3 rotation)
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
  wheel.Rotation = rotation;
  wheels.Push(wheel);
  int index=wheels.Length()-1;
  SetWheelMesh(index, wheelfact, wheelfile);
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
  csRef<iMeshFactoryWrapper> wmeshfact;
  wmeshfact = engine->FindMeshFactory(wheels[wheelnum].Meshfact);
  if(bodySectors->GetCount() > 0)
  {
    csRef<iSector> bodySector=bodySectors->Get(0);
    wheelmesh=engine->CreateMeshWrapper(wmeshfact,"wheel",
                                        bodySector,
                                        wheels[wheelnum].Position);
  }
  else
  {
    wheelmesh=engine->CreateMeshWrapper(wmeshfact,"wheel");
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
 
  csVector3 fullpos = bodytransform.This2Other(wheels[wheelnum].Position);
  //csMatrix3 bodyrot = bodyMesh->GetMesh()->GetMovable()->GetTransform().GetO2T();
  csOrthoTransform t = csOrthoTransform(wheels[wheelnum].Rotation, fullpos);
  wheelbody->SetTransform(t);
  wheelbody->SetPosition(fullpos);
  wheelbody->AttachMesh(wheelmesh);

  wheelbody->AttachColliderSphere (
      wheelradius,wheelcenter,0.8f,1,0.5f,0.05f);
  
   //If it a right wheel, and using the default mesh, flip it.
   if (wheels[wheelnum].Position.x < 0 &&
        wheels[wheelnum].Meshfact == wheelfact)
   {
     csOrthoTransform t = wheelbody->GetTransform();
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
    osys=scfQueryInterface<iODEDynamicSystemState> (dyn);
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

  //Work out the straight velocity of the car, which will be useful
  //further on.
  csOrthoTransform trans = bodyMech->GetBody()->GetTransform();
  csVector3 linvel = bodyMech->GetBody()->GetLinearVelocity();
  float straightvel = trans.Other2ThisRelative(linvel).z;

  //Dont try to work out the gear in neutral or reverse.
  if(gear > 0 && autotransmission)
    UpdateGear();

  //Update the wheel's speeds to the current gear if accelerating. else
  //use the neutral gear settings.
  float vel=gears[1].x;
  float fmax=gears[1].y;
  if(accelerating)
  {
    vel=gears[gear + 1].x;
    fmax=gears[gear + 1].y;
  }

  float steerfactor = 1000.0f + fabs(straightvel) * 100.0f;
  for(size_t i=0; i < wheels.Length();i++)
  {
    if(wheels[i].WheelJoint!=0)
    {
      wheels[i].WheelJoint->SetVel(vel,1);
      wheels[i].WheelJoint->SetFMax(fmax*wheels[i].EnginePower,1);

      // Set the power of steering proportional to the speed of the car.
      // This gives smooth steer and return at low speeds, while
      // preventing wheels from bending at high speeds.
      wheels[i].WheelJoint->SetFMax(steerfactor, 0);
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
    if (autoreverse && straightvel > -2.0)
      Reverse();
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

void celPcWheeled::SetWheelRotation(int wheelnum, csMatrix3 rotation)
{
  wheels[wheelnum].Rotation = rotation;
/*
  csRef<iPcMesh> bodyMesh=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMesh);
  csOrthoTransform
      bodytransform=bodyMesh->GetMesh()->GetMovable()->GetTransform();

    //If the wheel is already created, have to rotate it aswell.
  if(wheels[wheelnum].WheelJoint!=0)
  {
      wheels[wheelnum].RigidBody->DestroyColliders();
      csVector3 fullpos = bodytransform.This2Other(wheels[wheelnum].Position);
      csOrthoTransform t = csOrthoTransform(wheels[wheelnum].Rotation, fullpos);
      wheels[wheelnum].RigidBody->SetTransform(t);

      csVector3 wheelcenter;
      wheels[wheelnum].RigidBody->AttachColliderSphere (
      wheelradius, csVector3(0),0.8f,1,0.5f,0.05f);
  }*/
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
  for(size_t i=0; i < wheels.Length(); i++)
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

  for(size_t i=0 ; i<wheels.Length(); i++)
  {
    ApplyWheelPresets(i);
  }
}

void celPcWheeled::SetOuterWheelSteerPreset(float sensitivity)
{
  if(outersteer>=0 && outersteer<=1)
    celPcWheeled::outersteer=sensitivity;

  for(size_t i=0; i<wheels.Length(); i++)
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

