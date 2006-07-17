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
  //---------------------------------------------------------------------------
  
  CS_IMPLEMENT_PLUGIN
  
  CEL_IMPLEMENT_FACTORY (Wheeled, "pcwheeled")
  
  //---------------------------------------------------------------------------
  
    // Actions
    csStringID celPcWheeled::action_setwheelmesh= csInvalidStringID;
    csStringID celPcWheeled::action_settankmode= csInvalidStringID;
    csStringID celPcWheeled::action_addwheel= csInvalidStringID;
    csStringID celPcWheeled::action_removewheel= csInvalidStringID;
    csStringID celPcWheeled::action_clearwheels= csInvalidStringID;
    csStringID celPcWheeled::action_setupwheels= csInvalidStringID;
    csStringID celPcWheeled::action_destroywheels= csInvalidStringID;
    csStringID celPcWheeled::action_accelerate= csInvalidStringID;
    csStringID celPcWheeled::action_setbrakeapplied= csInvalidStringID;
    csStringID celPcWheeled::action_sethandbrakeapplied= csInvalidStringID;
    csStringID celPcWheeled::action_setsteeramount= csInvalidStringID;
    csStringID celPcWheeled::action_steerleft= csInvalidStringID;
    csStringID celPcWheeled::action_steerright= csInvalidStringID;
    csStringID celPcWheeled::action_steerstraight= csInvalidStringID;
    csStringID celPcWheeled::action_reverse= csInvalidStringID;
    csStringID celPcWheeled::action_neutral= csInvalidStringID;
    csStringID celPcWheeled::action_setautotransmission= csInvalidStringID;
    csStringID celPcWheeled::action_setgear= csInvalidStringID;
    csStringID celPcWheeled::action_setgearsettings= csInvalidStringID;
    csStringID celPcWheeled::action_setbrakeforce= csInvalidStringID;
    csStringID celPcWheeled::action_setnumbergears= csInvalidStringID;
    csStringID celPcWheeled::action_setautoreverse= csInvalidStringID;
  
  //Presets
    csStringID celPcWheeled::action_setfrontwheelpreset= csInvalidStringID;
    csStringID celPcWheeled::action_setrearwheelpreset= csInvalidStringID;
    csStringID celPcWheeled::action_setouterwheelsteerpreset= csInvalidStringID;
    csStringID celPcWheeled::action_clearwheelpresets= csInvalidStringID;
  
    //Per-wheel actions
    csStringID celPcWheeled::action_setwheelposition= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelsuspensionsoftness= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelsuspensiondamping= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelleftsteersensitivity= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelrightsteersensitivity= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelturnspeed= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelreturnspeed= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelenginepower= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelbrakepower= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelsteerinverted= csInvalidStringID;
    csStringID celPcWheeled::action_setwheelhandbrakeaffected= csInvalidStringID;
  
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
    csStringID celPcWheeled::param_brakeapplied = csInvalidStringID;
    csStringID celPcWheeled::param_handbrakeapplied = csInvalidStringID;
    csStringID celPcWheeled::param_autotransmission = csInvalidStringID;
    csStringID celPcWheeled::param_autoreverse = csInvalidStringID;
  
    csStringID celPcWheeled::param_softness = csInvalidStringID; 
    csStringID celPcWheeled::param_sensitivity = csInvalidStringID; 
    csStringID celPcWheeled::param_damping = csInvalidStringID;
    csStringID celPcWheeled::param_speed = csInvalidStringID;
    csStringID celPcWheeled::param_power = csInvalidStringID;
    csStringID celPcWheeled::param_steerinverted = csInvalidStringID;
    csStringID celPcWheeled::param_handbrakeaffected = csInvalidStringID;
  
    celPcWheeled::celPcWheeled (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  dyn=0;
  bodyGroup=0;
  steerdir=0;
  gear=0;
  numberwheels=0;
  frontsteer=1.0f;
  rearsteer=0.0f;
  outersteer=1.0f;
  frontpower=1.0f;
  rearpower=1.0f;
  steering=false;
  autotransmission=true;
  brakeapplied=false;
  handbrakeapplied=false;
  autotransmission=true;
  autoreverse=true;
  wheelradius=0;
    
  steeramount=0.7;
  
  //Gear -1 is reverse, 0 is neutral
  SetNumberGears(1);
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
    action_addwheel= pl->FetchStringID("cel.action.AddWheel");
    action_removewheel= pl->FetchStringID("cel.action.RemoveWheel");
    action_clearwheels= pl->FetchStringID("cel.action.ClearWheels");
    action_setupwheels= pl->FetchStringID("cel.action.SetupWheels");
    action_destroywheels= pl->FetchStringID("cel.action.DestroyWheels");
    action_accelerate= pl->FetchStringID("cel.action.Accelerate");
    action_setbrakeapplied= pl->FetchStringID("cel.action.SetBrakeApplied");
    action_sethandbrakeapplied= pl->FetchStringID("cel.action.SetHandbrakeApplied");
    action_setsteeramount= pl->FetchStringID("cel.action.SetSteerAmount");
    action_steerleft= pl->FetchStringID("cel.action.SteerLeft");
    action_steerright= pl->FetchStringID("cel.action.SteerRight");
    action_steerstraight= pl->FetchStringID("cel.action.SteerStraight");
    action_reverse= pl->FetchStringID("cel.action.Reverse");
    action_neutral= pl->FetchStringID("cel.action.Neutral");
    action_setautotransmission= pl->FetchStringID("cel.action.SetAutoTransmission");
    action_setgear= pl->FetchStringID("cel.action.SetGear");
    action_setgearsettings= pl->FetchStringID("cel.action.SetGearSettings");
    action_setbrakeforce= pl->FetchStringID("cel.action.SetBrakeForce");
    action_setnumbergears= pl->FetchStringID("cel.action.SetNumberGears");
    action_setautoreverse= pl->FetchStringID("cel.action.SetAutoReverse");
  
    //Presets
    action_setfrontwheelpreset= pl->FetchStringID("cel.action.SetFrontWheelPreset");
    action_setrearwheelpreset= pl->FetchStringID("cel.action.SetRearWheelPreset");
    action_setouterwheelsteerpreset= pl->FetchStringID("cel.action.SetOuterWheelSteerPreset");
    action_clearwheelpresets= pl->FetchStringID("cel.action.ClearWheelPresets");
  
    //Per-wheel actions
    action_setwheelposition= pl->FetchStringID("cel.action.SetWheelPosition");
    action_setwheelsuspensionsoftness= pl->FetchStringID("cel.action.SetWheelSuspensionSoftness");
    action_setwheelsuspensiondamping= pl->FetchStringID("cel.action.SetWheelSuspensionDamping");
    action_setwheelleftsteersensitivity= pl->FetchStringID("cel.action.SetWheelLeftSteerSensitivity");
    action_setwheelrightsteersensitivity= pl->FetchStringID("cel.action.SetWheelRightSteerSensitivity");
    action_setwheelturnspeed= pl->FetchStringID("cel.action.SetWheelTurnSpeed");
    action_setwheelreturnspeed= pl->FetchStringID("cel.action.SetWheelReturnSpeed");
    action_setwheelenginepower= pl->FetchStringID("cel.action.SetWheelEnginePower");
    action_setwheelbrakepower= pl->FetchStringID("cel.action.SetWheelBrakePower");
    action_setwheelsteerinverted= pl->FetchStringID("cel.action.SetWheelSteerInverted");
    action_setwheelhandbrakeaffected= pl->FetchStringID("cel.action.SetWheelHandbrakeAffected");
  
  
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
    param_brakeapplied = pl->FetchStringID("cel.parameter.brakeapplied");
    param_handbrakeapplied = pl->FetchStringID("cel.parameter.handbrakeapplied");
    param_autotransmission = pl->FetchStringID("cel.parameter.autotransmission");
    param_autoreverse = pl->FetchStringID("cel.parameter.autoreverse");
  
    param_softness = pl->FetchStringID("cel.parameter.softness"); 
    param_damping = pl->FetchStringID("cel.parameter.damping");
    param_sensitivity = pl->FetchStringID("cel.parameter.sensitivity");
    param_speed = pl->FetchStringID("cel.parameter.speed");
    param_power = pl->FetchStringID("cel.parameter.power");
    param_steerinverted = pl->FetchStringID("cel.parameter.steerinverted");
    param_handbrakeaffected = pl->FetchStringID("cel.parameter.handbrakeaffected");
  }
  pl->CallbackOnce ((iCelTimerListener*)this, 100, CEL_EVENT_PRE);
}
  
  celPcWheeled::~celPcWheeled ()
{
  DestroyWheels();
  bodyMech=0;
  wheelfact=0;
  bodyGroup=0;
  engine=0;
  dyn=0;
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
                                    iCelParameterBlock* params)
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
  else if(actionId==action_addwheel)
  {
    CEL_FETCH_VECTOR3_PAR (pos, params, param_position);
    AddWheel(pos);
    return true;
  }
  else if(actionId==action_removewheel)
  {
    CEL_FETCH_FLOAT_PAR (num, params, param_wheelnum);
    RemoveWheel(int(num));
    return true;
  }
  else if(actionId==action_clearwheels)
  {
    ClearWheels();
    return true;
  }
  else if(actionId==action_setupwheels)
  {
    SetupWheels();
    return true;
  }
  else if(actionId==action_destroywheels)
  {
    DestroyWheels();
    return true;
  }
  else if(actionId==action_accelerate)
  {
    Accelerate();
    return true;
  }
  else if(actionId==action_setbrakeapplied)
  {
    CEL_FETCH_BOOL_PAR (applied, params, param_brakeapplied);
    SetBrakeApplied(applied);
    return true;
  }
  else if(actionId==action_sethandbrakeapplied)
  {
    CEL_FETCH_BOOL_PAR (applied, params, param_handbrakeapplied);
    SetHandbrakeApplied(applied);
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
    SetGearSettings(gear,velocity,force);
    return true;
  }
  else if(actionId==action_setbrakeforce)
  {
    CEL_FETCH_FLOAT_PAR(force,params,param_brakeforce);
    SetBrakeForce(force);
    return true;
  }
  else if(actionId==action_setnumbergears)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_number);
    SetNumberGears(int(num));
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
    CEL_FETCH_FLOAT_PAR(sens,params,param_sensitivity);
    CEL_FETCH_FLOAT_PAR(power,params,param_power);
    SetFrontWheelPreset(sens,power);
    return true;
  }
  else if(actionId==action_setrearwheelpreset)
  {
    CEL_FETCH_FLOAT_PAR(sens,params,param_sensitivity);
    CEL_FETCH_FLOAT_PAR(power,params,param_power);
    SetRearWheelPreset(sens,power);
    return true;
  }
  else if(actionId==action_setouterwheelsteerpreset)
  {
    CEL_FETCH_FLOAT_PAR(sens,params,param_sensitivity);
    SetOuterWheelSteerPreset(sens);
    return true;
  }
  else if(actionId==action_clearwheelpresets)
  {
    ClearWheelPresets();
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
    CEL_FETCH_FLOAT_PAR(softness,params,param_softness);
    SetWheelSuspensionSoftness(int(num),softness);
    return true;
  }
  else if(actionId==action_setwheelsuspensiondamping)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(damping,params,param_damping);
    SetWheelSuspensionDamping(int(num),damping);
    return true;
  }
  else if(actionId==action_setwheelleftsteersensitivity)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(sens,params,param_sensitivity);
    SetWheelLeftSteerSensitivity(int(num),sens);
    return true;
  }
  else if(actionId==action_setwheelrightsteersensitivity)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(sens,params,param_sensitivity);
    SetWheelRightSteerSensitivity(int(num),sens);
    return true;
  }
  else if(actionId==action_setwheelturnspeed)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(speed,params,param_speed);
    SetWheelTurnSpeed(int(num),speed);
    return true;
  }
  else if(actionId==action_setwheelreturnspeed)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(speed,params,param_speed);
    SetWheelReturnSpeed(int(num),speed);
    return true;
  }
  else if(actionId==action_setwheelenginepower)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(power,params,param_power);
    SetWheelEnginePower(int(num),power);
    return true;
  }
  else if(actionId==action_setwheelbrakepower)
  {
    CEL_FETCH_FLOAT_PAR(num,params,param_wheelnum);
    CEL_FETCH_FLOAT_PAR(power,params,param_power);
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
  
  int celPcWheeled::AddWheel(csVector3 position)
{
  celWheel wheel;
  wheel.RigidBody=0;
  wheel.WheelJoint=0;
  wheel.Position=position;
  wheel.TurnSpeed=2;
  wheel.ReturnSpeed=2;
  wheel.BrakePower=1;
  wheel.EnginePower=1;
  wheel.SuspensionSoftness=0.000125f;
  wheel.SuspensionDamping=0.125f;
  if (position.z>0)
  {
    wheel.SteerInverted=true;
    wheel.HandbrakeAffected=true;
  }
  else
  {
    wheel.SteerInverted=false;
    wheel.HandbrakeAffected=false;
  }
  wheels.Push(wheel);
  ApplyWheelPresets(wheels.Length()-1);
  return wheels.Length()-1;
}
  
  void celPcWheeled::SetupWheels()
{
  
  bodyMech=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMechanicsObject);
  if(bodyMech)
    dyn=bodyMech->GetMechanicsSystem()->GetDynamicSystem();
  if(!bodyGroup)
    bodyGroup=dyn->CreateGroup();
  bodyGroup->AddBody(bodyMech->GetBody());
  DestroyWheels();
  if(!bodyGroup || !bodyMech) return;
  for(size_t i=0; i < wheels.Length();i++)
  {
    //Create the mesh
    csRef<iPcMesh> bodyMesh=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMesh); 
    csOrthoTransform bodytransform=bodyMesh->GetMesh()->GetMovable()->GetTransform();
    csRef<iMeshWrapper> wheelmesh=0;
    csRef<iSectorList> bodySectors=bodyMesh->GetMesh()->GetMovable()->GetSectors();
    if(bodySectors->GetCount() > 0)
    {
      csRef<iSector> bodySector=bodySectors->Get(0);
      wheelmesh=engine->CreateMeshWrapper(wheelfact,"wheel",bodySector,wheels[i].Position);
    }
    else
    {
      wheelmesh=engine->CreateMeshWrapper(wheelfact,"wheel");
      wheelmesh->GetMovable()->SetPosition(wheels[i].Position);
      wheelmesh->GetMovable()->UpdateMove();
    }
    
    //Create the dynamic body
    csRef<iRigidBody> wheelbody=dyn->CreateBody();
    bodyGroup->AddBody(wheelbody);
  
    csVector3 wheelcenter;
    wheelmesh->GetMeshObject ()->GetObjectModel ()->GetRadius(wheelradius,wheelcenter);
    wheelbody->SetProperties (10, csVector3 (0), csMatrix3 ());
    wheelbody->SetPosition(bodytransform.This2Other(wheels[i].Position));
    wheelbody->AttachMesh(wheelmesh);
    wheelbody->AttachColliderSphere(wheelradius,wheelcenter,0.8f,1,0.5f,0.5f);
      //If it a right wheel, flip it.
    if (wheels[i].Position.x<0)
    {
      csOrthoTransform t=wheelbody->GetTransform();
      t.RotateThis(csVector3(0,1,0),3.14f);
      wheelbody->SetTransform(t);
    }
  
    //Create the joint
    csRef<iODEDynamicSystemState> osys=SCF_QUERY_INTERFACE (dyn, iODEDynamicSystemState);
    csRef<iODEHinge2Joint> joint=osys->CreateHinge2Joint();
    joint->Attach(bodyMech->GetBody(),wheelbody);
    joint->SetHingeAnchor(bodytransform.This2Other(wheels[i].Position));
    joint->SetHingeAxis1(csVector3(0,1,0));
    joint->SetHingeAxis2(csVector3(1,0,0));
    joint->SetSuspensionCFM(wheels[i].SuspensionSoftness,0);
    joint->SetSuspensionERP(wheels[i].SuspensionDamping,0);
    joint->SetLoStop(0,0);
    joint->SetHiStop(0,0);
    joint->SetVel(0,0);
    joint->SetVel(0,1);
    joint->SetStopERP(1.0,0);
    joint->SetFMax(1000,0);
    joint->SetFMax(100,1);
  
    wheels[i].RigidBody=wheelbody;
    wheels[i].WheelJoint=joint;
  }
}
  
  void celPcWheeled::RemoveWheel(int wheelnum)
{
  wheels.DeleteIndex(wheelnum);
}
  
  void celPcWheeled::DestroyWheels()
{
  csRef<iODEDynamicSystemState> osys=SCF_QUERY_INTERFACE (dyn, iODEDynamicSystemState);
  for(size_t i=0;i < wheels.Length();i++)
  {
    if (wheels[i].WheelJoint!=0)
    {
      osys->RemoveJoint(wheels[i].WheelJoint);
      wheels[i].WheelJoint=0;
    }
    if (wheels[i].RigidBody!=0)
    {
      csRef<iMeshWrapper> mesh = wheels[i].RigidBody->GetAttachedMesh();
      engine->WantToDie(mesh);
      bodyGroup->RemoveBody(wheels[i].RigidBody);
      dyn->RemoveBody(wheels[i].RigidBody);
      wheels[i].RigidBody=0;
    }
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
          /*Inverted, so turn the wheel right. The car is still steering left though,
        so leftsteersensitivity is still used.*/
        if(wheels[i].SteerInverted)
        {
          float lostop=-steeramount*wheels[i].LeftSteerSensitivity;
          wheels[i].WheelJoint->SetLoStop(lostop,0);
          wheels[i].WheelJoint->SetVel(-wheels[i].TurnSpeed,0);
        }
          //Not inverted, so turn the wheel left
        else
        {
          float histop=steeramount*wheels[i].LeftSteerSensitivity;
          wheels[i].WheelJoint->SetHiStop(histop,0);
          wheels[i].WheelJoint->SetVel(wheels[i].TurnSpeed,0);
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
          /*Inverted, so turn the wheel left. The car is still steering right though,
        so rightsteersensitivity is still used.*/
        if(wheels[i].SteerInverted)
        {
          float histop=steeramount*wheels[i].RightSteerSensitivity;
          wheels[i].WheelJoint->SetHiStop(histop,0);
          wheels[i].WheelJoint->SetVel(wheels[i].TurnSpeed,0);
        }
          //Steer it right
        else
        {
          float lostop=-steeramount*wheels[i].RightSteerSensitivity;
          wheels[i].WheelJoint->SetLoStop(lostop,0);
          wheels[i].WheelJoint->SetVel(-wheels[i].TurnSpeed,0);
        }
      }
    }
  }
}
  
  void celPcWheeled::SteerStraight()
{
  if (steerdir==-1)
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
  if (steerdir==1)
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
          //It's a left wheel,, steering left. slow it down
      if (wheels[i].Position.x > 0  && steerdir < 0)
      {
        wheels[i].WheelJoint->SetVel(0,1);
        wheels[i].WheelJoint->SetFMax(wheels[i].BrakePower*brakeforce,1);
      }
    }
  }
}
  
  //Update the vehicle. Order is important here! first comes acceleration, then braking, then handbrake. tank steering comes last.
  void celPcWheeled::TickOnce()
{
  if(gear>0)
    UpdateGear();
    //Update the wheel's speeds to the current gear
  for(size_t i=0; i < wheels.Length();i++)
  {
    if(wheels[i].WheelJoint!=0)
    {
      wheels[i].WheelJoint->SetVel(gears[gear+1].x,1);
      wheels[i].WheelJoint->SetFMax(gears[gear+1].y*wheels[i].EnginePower,1);
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
      //if autoreverse is on, check if the vehicle is slow enough to start reversing.
    if (autoreverse)
    {
      if(bodyMech->GetBody()->GetLinearVelocity().Norm()<2)
        gear=-1;
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
        wheels[i].WheelJoint->SetFMax(brakeforce*wheels[i].BrakePower*1000,1);
      }
    }
  }
  if(tankmode && steerdir!=0)
    UpdateTankSteer();
    
  pl->CallbackOnce ((iCelTimerListener*)this, 100, CEL_EVENT_PRE);
}
  
    void celPcWheeled::SetGear(int gear)
{
  if (gear>=-1 && gear <= (int)gears.Length()-1)
    celPcWheeled::gear=gear;
}
  
    void celPcWheeled::SetGearSettings(int gear, float velocity, float force)
{
  if (gear >=-1 && gear < (int)gears.Length()-1)
  {
    gears[gear+1].x=velocity;
    gears[gear+1].y=force;
  }
}
  
    void celPcWheeled::UpdateGear()
{
  for(int i=1; i < (int)gears.Length()-2; i++)
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
    wheels[wheelnum].WheelJoint->SetHingeAnchor(bodyMech->LocalToWorld(position));
  }
}
  
    void celPcWheeled::SetWheelSuspensionSoftness(int wheelnum, float softness)
{
  wheels[wheelnum].SuspensionSoftness=softness;
    //If the wheel is already created, have to set it too.
  if(wheels[wheelnum].WheelJoint!=0)
  {
    wheels[wheelnum].WheelJoint->SetSuspensionCFM(softness,0);
  }
}
  
    void celPcWheeled::SetWheelSuspensionDamping(int wheelnum, float damping)
{
  wheels[wheelnum].SuspensionDamping=damping;
    //If the wheel is already created, have to set it too.
  if(wheels[wheelnum].WheelJoint!=0)
  {
    wheels[wheelnum].WheelJoint->SetSuspensionERP(damping,0);
  }
}
  
void celPcWheeled::SetFrontWheelPreset(float sensitivity,float power)
{
  frontsteer=sensitivity;
  frontpower=power;
  for(size_t i=0; i++; i < wheels.Length())
  {
    ApplyWheelPresets(i);
  }
}

void celPcWheeled::SetRearWheelPreset(float sensitivity,float power)
{
  if (rearsteer>=0 && rearsteer<=1)
    rearsteer=sensitivity;
  if(power>=0 && power <=1)
    rearpower=power;

  for(size_t i=0 ;i++; i<wheels.Length())
  {
    ApplyWheelPresets(i);
  }
}

void celPcWheeled::SetOuterWheelSteerPreset(float sensitivity)
{
  if(outersteer>=0 && outersteer<=1)
    outersteer=sensitivity;

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
  }
  else if(wheels[wheelnum].Position.z>0)
  {
    wheels[wheelnum].LeftSteerSensitivity=rearsteer;
    wheels[wheelnum].RightSteerSensitivity=rearsteer;
    wheels[wheelnum].EnginePower=rearpower;
  }
  else
  {
    wheels[wheelnum].LeftSteerSensitivity=0.0f;
    wheels[wheelnum].RightSteerSensitivity=0.0f;
    wheels[wheelnum].EnginePower=1.0f;
  }
  //Apply outer steer settings
      //A right wheels[wheelnum]. So its sensitivity when steering left is reduced.
  if (wheels[wheelnum].Position.x<0)
    wheels[wheelnum].LeftSteerSensitivity*=outersteer;
      //A left wheels[wheelnum]. So its sensitivity when steering right is reduced.
  if (wheels[wheelnum].Position.x>0)
    wheels[wheelnum].RightSteerSensitivity*=outersteer;
}

void celPcWheeled::ClearWheelPresets()
{
  for(size_t i=0; i<wheels.Length();i++)
  {
    wheels[i].LeftSteerSensitivity=0;
    wheels[i].RightSteerSensitivity=0;
  }
}
  //---------------------------------------------------------------------------
  
