/*
Crystal Space Entity Layer - Wheeled Vehicle plugin
Copyright (C) 2006 by Christopher Fraser

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
#include "csutil/hash.h"
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

#include "wheeledcb.h"
#include <cmath>

//--------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (Wheeled, "pcvehicle.wheeled", "pcwheeled")

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
csStringID celPcWheeled::param_steeramount = csInvalidStringID;
csStringID celPcWheeled::param_applied = csInvalidStringID;

csStringID celPcWheeled::param_suspensionsoftness = csInvalidStringID;
csStringID celPcWheeled::param_suspensiondamping = csInvalidStringID;
csStringID celPcWheeled::param_leftsteersensitivity = csInvalidStringID;
csStringID celPcWheeled::param_rightsteersensitivity = csInvalidStringID;
csStringID celPcWheeled::param_steersensitivity = csInvalidStringID;
csStringID celPcWheeled::param_turnspeed = csInvalidStringID;
csStringID celPcWheeled::param_returnspeed = csInvalidStringID;
csStringID celPcWheeled::param_enginepower = csInvalidStringID;
csStringID celPcWheeled::param_brakepower = csInvalidStringID;
csStringID celPcWheeled::param_friction = csInvalidStringID;
csStringID celPcWheeled::param_mass = csInvalidStringID;
csStringID celPcWheeled::param_steerinverted = csInvalidStringID;
csStringID celPcWheeled::param_handbrakeaffected = csInvalidStringID;

csStringID celPcWheeled::param_otherbody = csInvalidStringID;
csStringID celPcWheeled::param_depth = csInvalidStringID;
csStringID celPcWheeled::param_normal = csInvalidStringID;
csStringID celPcWheeled::param_index = csInvalidStringID;

PropertyHolder celPcWheeled::propinfo;

celPcWheeled::celPcWheeled (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  engine = csQueryRegistry<iEngine> (object_reg);
  
  dyn = 0;
  bodyGroup = 0;
  abssteer = 0.0f;
  gear = 1;
  topgear = 0;
  frontsteer = 1.0f;
  rearsteer = 0.0f;
  outersteer = 1.0f;
  frontpower = 1.0f;
  rearpower = 1.0f;
  frontss = 0.000125f;
  frontsd = 0.125f;
  rearss = 0.000125f;
  rearsd = 0.125f;
  frontmass = 10.0f;
  rearmass = 10.0f;
  frontfriction = 0.7f;
  rearfriction = 0.7f;
  speed = 0.0f;
  
  accelamount = 0.0f;
  brakeamount = 0.0f;

  antiswayfactor = 1.0f;
  antiswaylimit = 100000.0f;

  abs = true;
  autotransmission = true;
  handbrakeapplied = false;
  autotransmission = true;
  autoreverse = true;
  cd_enabled = true;
  differential = true;
  antisway = false;
  steeramount = 0.7f;
  
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
    param_meshfile = pl->FetchStringID("meshfile");
    param_meshfact = pl->FetchStringID("meshfact");
    param_position = pl->FetchStringID("position");
    param_rotation = pl->FetchStringID("rotation");
    param_wheelnum = pl->FetchStringID("wheelnum");
    param_gear = pl->FetchStringID("gear");
    param_velocity = pl->FetchStringID("velocity");
    param_force = pl->FetchStringID("force");
    param_number = pl->FetchStringID("number");
    param_steeramount = pl->FetchStringID("steeramount");
    param_applied = pl->FetchStringID("applied");
    
    param_mass = pl->FetchStringID("mass");
    param_friction = pl->FetchStringID("friction");
    param_suspensionsoftness = pl->FetchStringID("suspensionsoftness");
    param_suspensiondamping = pl->FetchStringID("suspensiondamping");
    param_steersensitivity = pl->FetchStringID("steersensitivity");
    param_leftsteersensitivity = pl->FetchStringID("leftsteersensitivity");
    param_rightsteersensitivity = pl->FetchStringID("rightsteersensitivity");
    param_steersensitivity = pl->FetchStringID("steersensitivity");
    param_turnspeed = pl->FetchStringID("turnspeed");
    param_returnspeed = pl->FetchStringID("returnspeed");
    param_enginepower = pl->FetchStringID("enginepower");
    param_brakepower = pl->FetchStringID("brakepower");
    param_steerinverted = pl->FetchStringID("steerinverted");
    param_handbrakeaffected = pl->FetchStringID("handbrakeaffected");
    
    param_normal = pl->FetchStringID ("normal");
    param_otherbody = pl->FetchStringID ("otherbody");
    param_depth = pl->FetchStringID ("depth");
    param_index = pl->FetchStringID ("index");
  }
  
  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.mechanics.wheeled.action.");
    AddAction (action_setwheelmesh, "SetWheelMesh");
    AddAction (action_addwheelauto, "AddWheelAuto");
    AddAction (action_addwheel, "AddWheel");
    AddAction (action_deletewheel, "DeleteWheel");
    AddAction (action_deleteallwheels, "DeleteAllWheels");
    AddAction (action_destroywheel, "DestroyWheel");
    AddAction (action_destroyallwheels, "DestroyAllWheels");
    AddAction (action_restorewheel, "RestoreWheel");
    AddAction (action_restoreallwheels, "RestoreAllWheels");
    
    AddAction (action_steerleft, "SteerLeft");
    AddAction (action_steerright, "SteerRight");
    AddAction (action_steerstraight, "SteerStraight");
    AddAction (action_reverse, "Reverse");
    AddAction (action_neutral, "Neutral");
    AddAction (action_setgearsettings, "SetGearSettings");
    
//Presets
    AddAction (action_setfrontwheelpreset, "SetFrontWheelPreset");
    AddAction (action_setrearwheelpreset, "SetRearWheelPreset");
    
//Per-wheel actions
    AddAction (action_setwheelposition, "SetWheelPosition");
    AddAction (action_setwheelrotation, "SetWheelRotation");
    AddAction (action_setwheelsuspensionsoftness, "SetWheelSuspensionSoftness");
    AddAction (action_setwheelsuspensiondamping, "SetWheelSuspensionDamping");
    AddAction (action_setwheelleftsteersensitivity, "SetWheelLeftSteerSensitivity");
    AddAction (action_setwheelrightsteersensitivity, "SetWheelRightSteerSensitivity");
    AddAction (action_setwheelfriction, "SetWheelFriction");
    AddAction (action_setwheelmass, "SetWheelMass");
    AddAction (action_setwheelturnspeed, "SetWheelTurnSpeed");
    AddAction (action_setwheelreturnspeed, "SetWheelReturnSpeed");
    AddAction (action_setwheelenginepower, "SetWheelEnginePower");
    AddAction (action_setwheelbrakepower, "SetWheelBrakePower");
    AddAction (action_setwheelsteerinverted, "SetWheelSteerInverted");
    AddAction (action_setwheelhandbrakeaffected, "SetWheelHandbrakeAffected");
  }
  
  propinfo.SetCount (20);
  AddProperty (propid_speed, "speed",
         CEL_DATA_FLOAT, true, "Vehicle Speed.", &speed);
  AddProperty (propid_tankmode, "tankmode",
         CEL_DATA_BOOL, false, "Tank Steering.", &tankmode);
  AddProperty (propid_steer, "steer",
         CEL_DATA_FLOAT, false, "Absolute steer.", 0);
  AddProperty (propid_accelamount, "accelamount",
         CEL_DATA_FLOAT, false, "Amount of accelerator.", 0);
  AddProperty (propid_brakeamount, "brakeamount",
         CEL_DATA_FLOAT, false, "Amount of brakes applied", 0);
  AddProperty (propid_handbraking, "handbraking",
         CEL_DATA_BOOL, false, "Handbrake is applied.",
         &handbrakeapplied);
  AddProperty (propid_steeramount, "steeramount",
         CEL_DATA_FLOAT, false, "Vehicle Steer Amount.", &steeramount);
  AddProperty (propid_autotransmission, "autotransmission",
         CEL_DATA_BOOL, false, "Automatic Gear Shifts.",
         &autotransmission);
  AddProperty (propid_gear, "gear",
         CEL_DATA_LONG, false, "Current gear", 0);
  AddProperty (propid_brakeforce, "brakeforce",
         CEL_DATA_FLOAT, false, "Force applied to brakes.",
         &brakeforce);
  AddProperty (propid_autoreverse, "autoreverse",
         CEL_DATA_BOOL, false, "Vehicle automatically reverses.",
         &autoreverse);
  AddProperty (propid_outerwheelsteerpreset,
         "outerwheelsteerpreset",
         CEL_DATA_FLOAT, false, "Vehicle outer wheel steer.", 0);
  AddProperty (propid_abs, "abs",
         CEL_DATA_BOOL, false, "Vehicle anti-lock brakes enabled.",
         &abs);
  AddProperty (propid_currentgearvelocity, "currentgearvelocity",
         CEL_DATA_FLOAT, true, "Current gear velocity.", 0);
  AddProperty (propid_currentgearforce, "currentgearforce",
         CEL_DATA_FLOAT, true, "Current gear force.", 0);
  AddProperty (propid_averagewheelspin, "averagewheelspin",
         CEL_DATA_FLOAT, true, "Average wheel spin.", 0);
  AddProperty (propid_differential, "differential",
         CEL_DATA_BOOL, true, "Differential is enabled.",&differential);
  AddProperty (propid_antisway, "antisway",
         CEL_DATA_BOOL, true, "Anti-sway bar is enabled.",&antisway);
  AddProperty (propid_antiswayfactor, "antiswayfactor",
         CEL_DATA_FLOAT, true, "Anti-sway factor.",&antiswayfactor);
  AddProperty (propid_antiswaylimit, "antiswaylimit",
         CEL_DATA_FLOAT, true, "Anti-sway limit.",&antiswaylimit);
  
  params.AttachNew (new celVariableParameterBlock (5));
  params->AddParameter (param_otherbody);
  params->AddParameter (param_position);
  params->AddParameter (param_normal);
  params->AddParameter (param_depth);
  params->AddParameter (param_index);
  
  pl->CallbackOnce ((iCelTimerListener*)this, 25, CEL_EVENT_PRE);
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


bool celPcWheeled::GetPropertyIndexed (int idx, long& l)
{
  if (idx == propid_gear)
  {
    l = gear;
    return true;
  }
  return false;
}

bool celPcWheeled::SetPropertyIndexed (int idx, long l)
{
  if (idx == propid_gear)
  {
    SetGear(l);
    return true;
  }
  return false;
}

bool celPcWheeled::GetPropertyIndexed (int idx, float& f)
{
  if (idx == propid_accelamount)
  {
    f = accelamount;
    return true;
  }
  else if (idx == propid_brakeamount)
  {
    f = brakeamount;
    return true;
  }
  else if (idx == propid_steer)
  {
    f = abssteer;
    return true;
  }
  else if (idx == propid_outerwheelsteerpreset)
  {
    f = outersteer;
    return true;
  }
  else if (idx == propid_currentgearvelocity)
  {
    f = GetGearVelocity(GetGear());
    return true;
  }
  else if (idx == propid_currentgearforce)
  {
    f = GetGearForce(GetGear());
    return true;
  }
  else if (idx == propid_averagewheelspin)
  {
    f = GetAverageWheelSpin();
    return true;
  }
  return false;
}

bool celPcWheeled::SetPropertyIndexed (int idx, float f)
{
  if (idx == propid_accelamount)
  {
    Accelerate(f);
    return true;
  }
  else if (idx == propid_brakeamount)
  {
    Brake(f);
    return true;
  }
  else if (idx == propid_steer)
  {
    Steer(f);
    return true;
  }
  else if (idx == propid_outerwheelsteerpreset)
  {
    SetOuterWheelSteerPreset(f);
    return true;
  }
  return false;
}

bool celPcWheeled::PerformActionIndexed (int idx,
                     iCelParameterBlock* params,
                     celData& ret)
{
  switch (idx)
  {
  case action_setwheelmesh:
    {
      csString factname, filename;
      if (!Fetch (factname, params, param_meshfact, true, "")) return false;
      if (!Fetch (filename, params, param_meshfile, true, "")) return false;
      SetWheelMesh(factname,filename);
      return true;
    }
  case action_addwheelauto:
    {
      csVector3 pos, rotation;
      if (!Fetch (pos, params, param_position)) return false;
      csString factname, filename;
      if (!Fetch (factname, params, param_meshfact, true, "")) return false;
      if (!Fetch (filename, params, param_meshfile, true, "")) return false;
      csQuaternion quat;
      if (ParExists (CEL_DATA_VECTOR3, params, param_rotation))
      {
        if (!Fetch (rotation, params, param_rotation)) return false;
        quat.SetEulerAngles(rotation);
      }
      
      AddWheelAuto(pos, factname, filename, quat.GetMatrix());
      
      return true;
    }
  case action_addwheel:
    {
      csVector3 pos;
      if (!Fetch (pos, params, param_position)) return false;
      
      float turnspeed, returnspeed, ss, sd, brakepower, enginepower, lss, rss, friction, mass;
      if (!Fetch (turnspeed, params, param_turnspeed, true, 2.0f)) return false;
      if (!Fetch (returnspeed, params, param_returnspeed, true, 4.0f)) return false;
      if (!Fetch (ss, params, param_suspensionsoftness, true, 0.000125f)) return false;
      if (!Fetch (sd, params, param_suspensiondamping, true, 0.125f)) return false;
      if (!Fetch (brakepower, params, param_brakepower, true, 1.0f)) return false;
      if (!Fetch (enginepower, params, param_enginepower, true, 1.0f)) return false;
      if (!Fetch (lss, params, param_leftsteersensitivity, true, 1.0f)) return false;
      if (!Fetch (rss, params, param_rightsteersensitivity, true, 1.0f)) return false;
      if (!Fetch (friction, params, param_friction, true, 0.7f)) return false;
      if (!Fetch (mass, params, param_mass, true, 10.0f)) return false;
      
      bool hbaffect, sinvert;
      if (!Fetch (hbaffect, params, param_handbrakeaffected, true, false)) return false;
      if (!Fetch (sinvert, params, param_steerinverted, true, false)) return false;

      csString factname, filename;
      if (!Fetch (factname, params, param_meshfact, true, "")) return false;
      if (!Fetch (filename, params, param_meshfile, true, "")) return false;
      csQuaternion quat;
      if (ParExists (CEL_DATA_VECTOR3, params, param_rotation))
      {
        csVector3 rotation;
        if (!Fetch (rotation, params, param_rotation)) return false;
        quat.SetEulerAngles(rotation);
      }
      
      AddWheel(pos,turnspeed,returnspeed,ss,sd,brakepower,enginepower,
           lss,rss, friction, mass, hbaffect,sinvert, factname,
           filename,
           quat.GetMatrix());
      return true;
    }
  case action_deletewheel:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      DeleteWheel (num);
      return true;
    }
  case action_deleteallwheels:
    DeleteAllWheels();
    return true;
  case action_destroywheel:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      DestroyWheel(num);
      return true;
    }
  case action_destroyallwheels:
    DestroyAllWheels();
    return true;
  case action_restorewheel:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      RestoreWheel(num);
      return true;
    }
  case action_restoreallwheels:
    RestoreAllWheels();
    return true;
  case action_steerleft:
    {
      float amount;
      if (!Fetch (amount, params, param_steeramount, true, 1.0f)) return false;
      SteerLeft(amount);
      return true;
    }
  case action_steerright:
    {
      float amount;
      if (!Fetch (amount, params, param_steeramount, true, 1.0f)) return false;
      SteerRight(amount);
      return true;
    }
  case action_steerstraight:
    SteerStraight();
    return true;
  case action_reverse:
    Reverse();
    return true;
  case action_neutral:
    Neutral();
    return true;
  case action_setgearsettings:
    {
      long g;
      float force, velocity;
      if (!Fetch (force, params, param_force, true, 0.0f)) return false;
      if (!Fetch (velocity, params, param_velocity, true, 0.0f)) return false;
      if (!Fetch (g, params, param_gear, true, 0)) return false;
      SetGearSettings(g,velocity,force);
      return true;
    }
  case action_setfrontwheelpreset:
    {
      float sens, power, ss, sd, fr, ma;
      if (!Fetch (sens, params, param_steersensitivity, true, 0.0f)) return false;
      if (!Fetch (power, params, param_enginepower, true, 0.0f)) return false;
      if (!Fetch (ss, params, param_suspensionsoftness, true, 0.0f)) return false;
      if (!Fetch (sd, params, param_suspensiondamping, true, 0.0f)) return false;
      if (!Fetch (fr, params, param_friction, true, 0.0f)) return false;
      if (!Fetch (ma, params, param_mass, true, 0.0f)) return false;
      SetFrontWheelPreset(sens,power,ss,sd,fr,ma);
      return true;
    }
  case action_setrearwheelpreset:
    {
      float sens, power, ss, sd, fr, ma;
      if (!Fetch (sens, params, param_steersensitivity, true, 0.0f)) return false;
      if (!Fetch (power, params, param_enginepower, true, 0.0f)) return false;
      if (!Fetch (ss, params, param_suspensionsoftness, true, 0.0f)) return false;
      if (!Fetch (sd, params, param_suspensiondamping, true, 0.0f)) return false;
      if (!Fetch (fr, params, param_friction, true, 0.0f)) return false;
      if (!Fetch (ma, params, param_mass, true, 0.0f)) return false;
      SetRearWheelPreset(sens,power,ss,sd,fr,ma);
      return true;
    }
  case action_setwheelposition:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      csVector3 pos;
      if (!Fetch (pos, params, param_position)) return false;
      SetWheelPosition (num,pos);
      return true;
    }
  case action_setwheelrotation:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      csQuaternion quat;
      if (ParExists (CEL_DATA_VECTOR3, params, param_rotation))
      {
        csVector3 rotation;
        if (!Fetch (rotation, params, param_rotation)) return false;
        quat.SetEulerAngles(rotation);
      }
      SetWheelRotation(num, quat.GetMatrix());
      return true;
    }
  case action_setwheelsuspensionsoftness:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float softness;
      if (!Fetch (softness, params, param_suspensionsoftness, true, 0.0f)) return false;
      SetWheelSuspensionSoftness(int(num),softness);
      return true;
    }
  case action_setwheelsuspensiondamping:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float damping;
      if (!Fetch (damping, params, param_suspensiondamping, true, 0.0f)) return false;
      SetWheelSuspensionDamping(num,damping);
      return true;
    }
  case action_setwheelleftsteersensitivity:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float sens;
      if (!Fetch (sens, params, param_steersensitivity, true, 0.0f)) return false;
      SetWheelLeftSteerSensitivity(num,sens);
      return true;
    }
  case action_setwheelrightsteersensitivity:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float sens;
      if (!Fetch (sens, params, param_steersensitivity, true, 0.0f)) return false;
      SetWheelRightSteerSensitivity(num,sens);
      return true;
    }
  case action_setwheelfriction:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float friction;
      if (!Fetch (friction, params, param_friction, true, 0.0f)) return false;
      SetWheelFriction(num,friction);
      return true;
    }
  case action_setwheelmass:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float mass;
      if (!Fetch (mass, params, param_mass, true, 0.0f)) return false;
      SetWheelMass(num,mass);
      return true;
    }
  case action_setwheelturnspeed:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float speed;
      if (!Fetch (speed, params, param_turnspeed, true, 0.0f)) return false;
      SetWheelTurnSpeed(num, speed);
      return true;
    }
  case action_setwheelreturnspeed:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float speed;
      if (!Fetch (speed, params, param_returnspeed, true, 0.0f)) return false;
      SetWheelReturnSpeed(num, speed);
      return true;
    }
  case action_setwheelenginepower:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float power;
      if (!Fetch (power, params, param_enginepower, true, 0.0f)) return false;
      SetWheelEnginePower(num, power);
      return true;
    }
  case action_setwheelbrakepower:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      float power;
      if (!Fetch (power, params, param_brakepower, true, 0.0f)) return false;
      SetWheelBrakePower(num,power);
      return true;
    }
  case action_setwheelsteerinverted:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      bool invert;
      if (!Fetch (invert, params, param_steerinverted, true, false)) return false;
      SetWheelSteerInverted(num, invert);
      return true;
    }
  case action_setwheelhandbrakeaffected:
    {
      long num;
      if (!Fetch (num, params, param_wheelnum)) return false;
      bool affect;
      if (!Fetch (affect, params, param_handbrakeaffected, true, false)) return false;
      SetWheelHandbrakeAffected (num, affect);
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
    loader->Load (file, 0, false, true);
  }
  wheelfact = factname;
}

void celPcWheeled::SetWheelMesh(size_t wheelnum, const char* factname,const
                char* file)
{
  if(file!=0)
  {
    csRef<iLoader> loader = csQueryRegistry<iLoader> (object_reg);
    CS_ASSERT (loader != 0);
    loader->Load (file, 0, false, true);
  }
//Use the global wheel factory if none given
  if(factname==0)
    wheels[wheelnum].Meshfact = wheelfact;
  else
    wheels[wheelnum].Meshfact = factname;
}

void celPcWheeled::AddDiffWheel(size_t index)
{
  float zpos = wheels[index].Position.z;
  float xpos = wheels[index].Position.x;
  csVector2 *diffGroup = diffGroups.GetElementPointer(zpos);
  if (diffGroup)
  {
    if (xpos < 0)
      diffGroup->y = index;
    else
      diffGroup->x = index;
  }
//No differential group exists, make one now
  else
  {
    csVector2 diffGroup ;
    if (xpos < 0)
    {
      diffGroup.y = index;
      diffGroup.x = 0;
    }
    else
    {
      diffGroup.x = index;
      diffGroup.y = 0;
    }
    diffGroups.Put(zpos, diffGroup);
  }
}

//This method uses the vehicle's presets and wheel's position for settings
size_t celPcWheeled::AddWheelAuto(csVector3 position, const char* wheelfact,
                  const char* wheelfile, csMatrix3 rotation)
{
  celWheel wheel;
  wheel.Position=position;
  wheel.TurnSpeed=2.0f;
  wheel.ReturnSpeed=4.0f;
  wheel.BrakePower=1.0f;
  wheel.Rotation = rotation;
  wheel.CurrentStop = 0.0f;
  wheels.Push(wheel);
  size_t index=wheels.GetSize()-1;
  ApplyWheelPresets(index);
  SetWheelMesh(index, wheelfact, wheelfile);
  RestoreWheel(index);
  AddDiffWheel(index);
  return index;
}

//Add a wheel. Every configurable setting is passed in here.
size_t celPcWheeled::AddWheel(csVector3 position,float turnspeed, float
                returnspeed, float ss, float sd,float
                brakepower,float enginepower,
                float lss, float rss, float friction, float mass,
                bool hbaffect,
                bool sinvert, const char* wheelfact, const char*
                wheelfile,
                csMatrix3 rotation)
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
  wheel.WheelFriction=friction;
  wheel.WheelMass=mass;
  wheel.HandbrakeAffected=hbaffect;
  wheel.SteerInverted=sinvert;
  wheel.Rotation = rotation;
  wheel.ABSBrake = 1.0f;
  wheel.CurrentStop = 0.0f;
  wheels.Push(wheel);
  size_t index=wheels.GetSize()-1;
  SetWheelMesh(index, wheelfact, wheelfile);
  RestoreWheel(index);
  AddDiffWheel(index);
  return index;
}

//Physically destroy a wheel, but don't forget it.
void celPcWheeled::DestroyWheel(size_t wheelnum)
{
  GetMech();
  if(!bodyGroup || !bodyMech) return;
//Remove the joint
  if (wheels[wheelnum].WheelJoint != 0)
  {
    osys->RemoveJoint(wheels[wheelnum].WheelJoint);
    wheels[wheelnum].WheelJoint = 0;
  }
//Remove the joint state
  if (wheels[wheelnum].JointState != 0)
  {
    wheels[wheelnum].JointState = 0;
  }
//Remove the brake motor
  if (wheels[wheelnum].BrakeMotor != 0)
  {
    osys->RemoveJoint(wheels[wheelnum].BrakeMotor);
    wheels[wheelnum].BrakeMotor = 0;
  }
  if (wheels[wheelnum].RigidBody != 0)
  {
    csRef<iMeshWrapper> mesh =
      wheels[wheelnum].RigidBody->GetAttachedMesh();
    engine->WantToDie(mesh);
    bodyGroup->RemoveBody(wheels[wheelnum].RigidBody);
    wheels[wheelnum].RigidBody->SetCollisionCallback (0);
    WheeledCollisionCallback* cb = wheels[wheelnum].Callback;
    if (cb)
      cb->DecRef();
    dyn->RemoveBody(wheels[wheelnum].RigidBody);
    wheels[wheelnum].RigidBody = 0;
  }
}

void celPcWheeled::DestroyAllWheels()
{
  for(size_t i=0;i < wheels.GetSize();i++)
  {
    DestroyWheel(i);
  }
}

//Totally remove a wheel
void celPcWheeled::DeleteWheel(size_t wheelnum)
{
  DestroyWheel(wheelnum);
  float zpos = wheels[wheelnum].Position.z;
  diffGroups.DeleteAll(zpos);
  wheels.DeleteIndex(wheelnum);
}

void celPcWheeled::DeleteAllWheels()
{
  DestroyAllWheels();
  diffGroups.DeleteAll();
  wheels.DeleteAll();
}

void celPcWheeled::RestoreWheel(size_t wheelnum)
{
  GetMech();
//Create the mesh
  csRef<iPcMesh> bodyMesh=celQueryPropertyClassEntity<iPcMesh> (GetEntity());
  csOrthoTransform
    bodytransform=bodyMesh->GetMesh()->GetMovable()->GetTransform();
  csVector3 realpos = bodytransform.This2Other(wheels[wheelnum].Position);
  csRef<iMeshWrapper> wheelmesh=0;
  csRef<iSectorList>
    bodySectors=bodyMesh->GetMesh()->GetMovable()->GetSectors();
  csRef<iMeshFactoryWrapper> wmeshfact;
  wmeshfact = engine->FindMeshFactory(wheels[wheelnum].Meshfact);
//Only continue if we have the factory.
  if (wmeshfact)
  {
//Create the mesh of the wheel
    if(bodySectors->GetCount() > 0)
    {
      csRef<iSector> bodySector=bodySectors->Get(0);
      wheelmesh=engine->CreateMeshWrapper(wmeshfact,"wheel",
                        bodySector,
                        realpos);
    }
    else
    {
      wheelmesh=engine->CreateMeshWrapper(wmeshfact,"wheel");
      wheelmesh->GetMovable()->SetPosition(realpos);
      wheelmesh->GetMovable()->UpdateMove();
    }

//Set the wheel rotation and position in the mesh.
//AFAIK the rotation is overridden by the body anyway.
    csMatrix3 bodyrot = bodytransform.GetO2T();
    csMatrix3 wheelrotation = bodyrot * wheels[wheelnum].Rotation;
    csOrthoTransform t = csOrthoTransform(wheelrotation, realpos);
//If it a right wheel, flip it.
    if (wheels[wheelnum].Position.x < 0.0f)
      t.RotateThis(csVector3(0.0f,1.0f,0.0f),3.14159f);
    wheelmesh->GetMovable()->SetTransform(t);
    
//Create the dynamic body
    csRef<iRigidBody> wheelbody=dyn->CreateBody();
    bodyGroup->AddBody(wheelbody);
    WheeledCollisionCallback* wheelcb = new WheeledCollisionCallback (this);
    wheelcb->SetIndex(wheelnum);
    wheelbody->SetCollisionCallback (wheelcb);
    wheels[wheelnum].Callback = wheelcb;
    
    csVector3 wheelcenter(0);
    float wheelradius = 0.0f;
    wheelmesh->GetMeshObject ()->GetObjectModel
      ()->GetRadius(wheelradius,wheelcenter);
     
    wheelbody->SetProperties(wheels[wheelnum].WheelMass,csVector3(0.0f),csMatrix3   
                ());
    wheelbody->SetTransform(t);
    wheelbody->SetPosition(realpos);
    wheelbody->AttachMesh(wheelmesh);
    
    wheelbody->AttachColliderSphere(wheelradius,wheelcenter,
      wheels[wheelnum].WheelFriction,1.0f,0.5f,0.05f);
    wheelbody->AdjustTotalMass(wheels[wheelnum].WheelMass);
    wheelbody->SetTransform(t);
//Create the joint
    csRef<iODEHinge2Joint> joint = osys->CreateHinge2Joint();
    joint->Attach(bodyMech->GetBody(), wheelbody);
    joint->SetHingeAnchor(realpos);     
    joint->
      SetHingeAxis1(bodytransform.This2OtherRelative(csVector3(0,1,0)));
    joint->
      SetHingeAxis2(bodytransform.This2OtherRelative(csVector3(1,0,0)));
    joint->SetSuspensionCFM(wheels[wheelnum].SuspensionSoftness,0);
    joint->SetSuspensionERP(wheels[wheelnum].SuspensionDamping,0);
//     joint->SetLoStop(0.0f,0);
//     joint->SetHiStop(0.0f,0);
//     joint->SetVel(0.0f,0);
//     joint->SetVel(0.0f,1);
//     joint->SetStopERP(1.0f,0);
//     joint->SetFMax(0.0f,0);
//     joint->SetFMax(100.0f,1);
//     
//Create the brakes motor
    csRef<iODEAMotorJoint> bmotor = osys->CreateAMotorJoint();
    bmotor->Attach(bodyMech->GetBody(), wheelbody);
    bmotor->SetAMotorNumAxes(1);
    bmotor->SetAMotorAxis(0, 1,
                bodytransform.This2OtherRelative(csVector3(1, 0,
                  0)));
    bmotor->SetFMax(brakeforce * brakeamount, 0);
    bmotor->SetVel(0.0f, 0);
    
    wheels[wheelnum].RigidBody = wheelbody;
    wheels[wheelnum].WheelJoint = joint;
    csRef<iODEGeneralJointState> js =
      scfQueryInterface<iODEGeneralJointState> (joint);
    wheels[wheelnum].JointState = js;
    wheels[wheelnum].BrakeMotor = bmotor;
  }
}

void celPcWheeled::RestoreAllWheels()
{
  for(size_t i=0; i < wheels.GetSize();i++)
  {
    if(wheels[i].RigidBody==0)
      RestoreWheel(i);
  }
}

//Change position of a wheel
void celPcWheeled::UpdateSteer(size_t i)
{
  float wheelsteer = abssteer * steeramount;
  float ts = wheels[i].TurnSpeed;
  float lss = wheels[i].LeftSteerSensitivity;
  float rss = wheels[i].RightSteerSensitivity;
  float steer_min = lss * wheelsteer;
  float steer_max = rss * wheelsteer;
  float speed = ts / 40.0f;
  float stop = wheels[i].CurrentStop;
  if(wheels[i].WheelJoint!=0)
  {
    if (abssteer < 0.0f)
    {
      
//Not inverted, so turn the wheel left
      if(!wheels[i].SteerInverted)
      {
        if (stop > steer_min)
        {
          stop -= speed;
          if (stop <  steer_min)
            stop = steer_min;
        }
      }
/*Inverted, so turn the wheel right. The car is still steering
      left though,
      so leftsteersensitivity is still used.*/
      else
      {
        if (stop < -steer_min)
        {
          stop += speed;
          if (stop >  -steer_min)
            stop = -steer_min;
        }
      }
    }
    else if (abssteer > 0.0f)
    {
//Not inverted, so Steer it right.
      if(!wheels[i].SteerInverted)
      {
        if (stop < steer_max)
        {
          stop += speed;
          if (stop > steer_max)
            stop = steer_max;
        }
      }
/*Inverted, so turn the wheel left. The car is still steering
      right though,
      so rightsteersensitivity is still used.*/
      else
      {
        if (stop > -steer_max)
        {
          stop -= speed;
          if (stop <  -steer_max)
            stop = -steer_max;
        }
      }
    }
// Return the wheels to centre
    else
    {
      float rs = wheels[i].ReturnSpeed / 40.0f;
      if (stop > 0.0f)
      {
        stop -= rs;
        if (stop < 0.0f)
          stop = 0.0f;
      }
      else if (stop < 0.0f)
      {
        stop += rs;
        if (stop > 0.0f)
          stop = 0.0f;
      }
    }
    wheels[i].WheelJoint->SetLoStop(-stop,0);
    wheels[i].WheelJoint->SetHiStop(-stop,0);
    wheels[i].CurrentStop = stop;
  }
}

void celPcWheeled::SteerLeft(float amount)
{
  if (amount >= 0.0f && amount <= 1.0f)
    abssteer = -amount;
}

void celPcWheeled::SteerRight(float amount)
{
  if (amount >= 0.0f && amount <= 1.0f)
    abssteer = amount;
}

//Steer the vehicle by amount.
void celPcWheeled::Steer(float amount)
{
  if (amount >= -1.0f && amount <= 1.0f)
  {
    if (amount < 0.0f)
      SteerLeft(-amount);
    else if(amount > 0.0f)
      SteerRight(amount);
    else
      SteerStraight();
  }
}

void celPcWheeled::SteerStraight()
{
  abssteer = 0.0f;
}

void celPcWheeled::GetMech()
{
  if(!bodyMech)
  {
    bodyMech=celQueryPropertyClassEntity<iPcMechanicsObject> (GetEntity());
    if(!bodyMech)
      return;
    dyn=bodyMech->GetMechanicsSystem()->GetDynamicSystem();
    osys=scfQueryInterface<iODEDynamicSystemState> (dyn);
    bodyGroup=dyn->CreateGroup();
    bodyGroup->AddBody(bodyMech->GetBody());
  }
}

//Work out the straight velocity of the car, which will be useful
//further on.
float celPcWheeled::GetSpeed()
{
//First ensure everything is set and ready to go.
  GetMech();
  csOrthoTransform trans = bodyMech->GetBody()->GetTransform();
  csVector3 linvel = bodyMech->GetBody()->GetLinearVelocity();
  return -trans.Other2ThisRelative(linvel).z;
}

//Get the forward spin velocity of a wheel
float celPcWheeled::GetWheelSpin(size_t wheelnum)
{
//First ensure everything is set and ready to go.
  GetMech();
  iRigidBody* wb = wheels[wheelnum].RigidBody;
  csVector3 avel = wb->GetAngularVelocity();
  csOrthoTransform trans =  wb->GetTransform();
  float vel = -trans.Other2ThisRelative(avel).x;
//Left wheels must be * -1 to return a positive velocity.
  if (wheels[wheelnum].Position.x < 0.0f)
    vel *= -1;
  return vel;
}

//Get average wheel velocity
float celPcWheeled::GetAverageWheelSpin()
{
  size_t numwheels = wheels.GetSize();
  float velsum = 0.0f;
  for(size_t i = 0 ; i < numwheels; i++)
    velsum += GetWheelSpin(i);
  return (velsum /numwheels);
}


//Update the vehicle. Update gear, then loop through all wheels, updating
//acceleration, braking, steering
void celPcWheeled::TickOnce()
{
//Dont try to work out the gear in neutral or reverse.
  if(gear > 0 && autotransmission)
    UpdateGear();

  if(antisway)
      AntiSway();

  for(size_t i=0; i < wheels.GetSize();i++)
  {
    if(wheels[i].WheelJoint !=0 && wheels[i].BrakeMotor != 0)
    {
      UpdateAccel(i);
      UpdateSteer(i);
      float avgspin = GetAverageWheelSpin();
      UpdateBrakes(avgspin, i);
      if(tankmode && abssteer != 0.0f)
        UpdateTankSteer(i);
      
    }
  }
  pl->CallbackOnce ((iCelTimerListener*)this, 25, CEL_EVENT_PRE);
}

//Apply the anti-sway forces to paired wheels
void celPcWheeled::AntiSway()
{
    csHash<csVector2, float>::GlobalIterator it = diffGroups.GetIterator();
    while (it.HasNext())
    {
        csVector2 diffGroup = it.Next();
        //Index of the left wheel
        size_t ix = size_t(diffGroup.x);
        //Index of the right wheel
        size_t iy = size_t(diffGroup.y);
        csVector3 anchor2x = wheels[ix].WheelJoint->GetHingeAnchor2();
        csVector3 anchor1x = wheels[ix].WheelJoint->GetHingeAnchor1();
        csVector3 axis = csVector3(0, 1, 0);
        csVector3 anchor2y = wheels[iy].WheelJoint->GetHingeAnchor2();
        csVector3 anchor1y = wheels[iy].WheelJoint->GetHingeAnchor1();
        float displacement = (anchor1x-anchor2x).y - (anchor1y-anchor2y).y;
        csVector3 force = displacement * bodyMech->GetMass() * 30.0f;
        if (force.Norm() > antiswaylimit)
            force = force.Unit() * antiswaylimit;
        wheels[ix].RigidBody->AddForce(-force);
        wheels[iy].RigidBody->AddForce(force);
    }
}

//Update acceleration of each of a wheel
void celPcWheeled::UpdateAccel(size_t wheelnum)
{
  speed = GetSpeed();
  float appliedaccel = accelamount;
  
//if autoreverse is on, check if the vehicle is slow enough to start
// reversing.
  if (autoreverse && speed < 0.5f && brakeamount >= 0.1f)
  {
    Reverse();
    appliedaccel = brakeamount;
  }
  
//Accelerator pressed with autoreverse, time to switch off reverse.
  if (autoreverse && accelamount >= 0.1 && gear == -1)
    gear = 1;
  
//Update the wheel's speeds to the current gear if accelerating. else
//use the neutral gear settings.
  float vel=gears[1].x;
  float fmax=gears[1].y;
  
  if(appliedaccel > 0.0f)
  {
    vel = gears[gear + 1].x;
    fmax = gears[gear + 1].y * appliedaccel;
  }
  
  
  float powerratio = 0.0f;
  float output = fmax * wheels[wheelnum].EnginePower;
  float wheelpower = 0.0f;
  
  if (differential)
  {
//Try to work out the power of the wheel after the differential has been
//applied.

//Find the differential group that corresponds with the z position of this
//wheel.
    csVector2* diffGroup =
      diffGroups.GetElementPointer(wheels[wheelnum].Position.z);
    if (diffGroup)
    {
//Index of the left wheel
      size_t ix = size_t(diffGroup->x);
//Index of the right wheel
      size_t iy = size_t(diffGroup->y);
      
// x / y
//This is inverted to give the powers that we want, as in 1 / resistance
      powerratio = wheels[iy].JointState->GetFeedbackForce2().Norm() /
        wheels[ix].JointState->GetFeedbackForce2().Norm();
// x + y = 2 * output
      if (ix == wheelnum)
      {
//y = x /powerratio
//Therefore x + x /powerratio = 2 * output
//powerratio *x + x = 2 * output * powerratio
//x = (2 * output * powerratio) / (1 + powerratio)
        wheelpower = (2.0f * output * powerratio) / ( 1.0f +
          powerratio);
      }
      else if (iy == wheelnum)
      {
// x = y * powerratio
//Therefore y + y * powerratio - 2 * output
//y = (2 * output) / (1 + powerratio)
        wheelpower = (2.0f * output) / (1.0f + powerratio);
      }
    }
  }
  
//Apply the throttle
  wheels[wheelnum].WheelJoint->SetVel(vel, 1);
  wheels[wheelnum].WheelJoint->SetFMax(wheelpower, 1);
}

//Update the braking of a given wheel
void celPcWheeled::UpdateBrakes(float avgspin, size_t wheelnum)
{
//Final force of the brakes
  float wheelbrake = 0.0f;
  
//Maybe hanbrake is applied. Apply an insane amount of braking then.
  if (handbrakeapplied && wheels[wheelnum].HandbrakeAffected)
    wheelbrake = brakeforce * 1000000.0f;
  
//Don't use brakeamount to brake if we are in autoreverse
  else if(!(autoreverse && gear == -1) && brakeamount > 0.05f)
  {
    wheelbrake = brakeforce * wheels[wheelnum].BrakePower * brakeamount;
//If using abs, test if wheel is locked
    if (abs)
    {
      float wheelspin = GetWheelSpin(wheelnum);
      float spindiff = avgspin - wheelspin;
//Wheel is locked - ease off the brake.
      if (spindiff >= 0.05f || fabs(wheelspin) < 0.001f)
      {
        if (wheels[wheelnum].ABSBrake > 0.0f)
          wheels[wheelnum].ABSBrake -= 0.05f;
      }
//Otherwise increase brake force
      else if (spindiff <= -0.05f)
      {
        if (wheels[wheelnum].ABSBrake < 1.0f)
          wheels[wheelnum].ABSBrake += 0.05f;
      }
      wheelbrake = wheelbrake * wheels[wheelnum].ABSBrake;
    }
  }
//std::cout << wheelbrake << "\n";
  wheels[wheelnum].BrakeMotor->SetFMax(wheelbrake, 0);
}

//When using tank steering, brake a given wheel
void celPcWheeled::UpdateTankSteer(size_t wheelnum)
{
  float brakeapply = wheels[wheelnum].BrakePower * brakeforce;
  brakeapply *= fabs(abssteer);
//The tank steers by braking one side of the wheels
//It's a right wheel, steering right. slow it down
  if (wheels[wheelnum].Position.x < 0.0f && abssteer > 0.0f)
  {
    wheels[wheelnum].BrakeMotor->SetFMax(brakeapply, 0);
    wheels[wheelnum].WheelJoint->SetFMax(0.0f, 1);
  }
//It's a left wheel, steering left. slow it down
  if (wheels[wheelnum].Position.x > 0.0f  && abssteer < 0.0f)
  {
    wheels[wheelnum].BrakeMotor->SetFMax(brakeapply, 0);
    wheels[wheelnum].WheelJoint->SetFMax(0.0f, 1);
  }
}

void celPcWheeled::UpdateGear()
{
  float wheelspin = GetAverageWheelSpin();
  for(int i = 1; i <= topgear; i++)
  {
    if (wheelspin  >= gears[i].x - 2.5f)
    {
      gear = i;
    }
  }
}

void celPcWheeled::SetGear(int gear)
{
  if (gear>=-1 && gear <= topgear)
  {
    celPcWheeled::gear=gear;
  }
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

void celPcWheeled::SetWheelPosition(size_t wheelnum, csVector3 position)
{
  wheels[wheelnum].Position=position;
//If the wheel is already created, have to move it's body aswell.
  if(wheels[wheelnum].WheelJoint!=0)
  {
    wheels[wheelnum].WheelJoint->
      SetHingeAnchor(bodyMech->LocalToWorld(position));
  }
}

//A wheel collision. Send a message back to wheeled entity
void celPcWheeled::WheelCollision (iRigidBody *thisbody,
                iRigidBody *otherbody, const csVector3& pos,
                const csVector3& normal, float depth, size_t index)
{
  if (cd_enabled)
  {
    iCelBehaviour* behaviour = entity->GetBehaviour ();
    celData ret;
    // Find the other body's entity
    params->GetParameter (0).Set (0);
    if (otherbody)
    {
      iObject* obj = otherbody->QueryObject ();
      if (obj)
      {
        iCelEntity* ent = pl->FindAttachedEntity (obj);
        if (ent)
          params->GetParameter (0).Set (ent->GetName ());
      }
    }
    params->GetParameter (1).Set (pos);
    params->GetParameter (2).Set (normal);
    params->GetParameter (3).Set (depth);
    params->GetParameter (4).Set ((int)index);
    if (behaviour)
      behaviour->SendMessage ("pcwheeled_collision", this, ret, params);
    if (!dispatcher_collision)
    {
      dispatcher_collision = entity->QueryMessageChannel ()->
        CreateMessageDispatcher (this, pl->FetchStringID ("cel.mechanics.collision"));
      if (!dispatcher_collision) return;
    }
    dispatcher_collision->SendMessage (params);
  }
}

void celPcWheeled::SetWheelFriction(size_t wheelnum, float friction)
{
  wheels[wheelnum].WheelFriction = friction;
  DestroyWheel(wheelnum);
  RestoreWheel(wheelnum);
}

void celPcWheeled::SetWheelMass(size_t wheelnum, float mass)
{
  wheels[wheelnum].WheelMass = mass;
  DestroyWheel(wheelnum);
  RestoreWheel(wheelnum);
}

void celPcWheeled::SetWheelRotation(size_t wheelnum, csMatrix3 rotation)
{
  wheels[wheelnum].Rotation = rotation;
/*
  csRef<iPcMesh> bodyMesh=celQueryPropertyClassEntity<iPcMesh> (GetEntity());
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

void celPcWheeled::SetWheelSuspensionSoftness(size_t wheelnum, float
                        softness)
{
  wheels[wheelnum].SuspensionSoftness=softness;
//If the wheel is already created, have to set its joint too.
  if(wheels[wheelnum].WheelJoint!=0)
  {
    wheels[wheelnum].WheelJoint->SetSuspensionCFM(softness,0);
  }
}

void celPcWheeled::SetWheelSuspensionDamping(size_t wheelnum, float
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
                     float suspensiondamping,
                     float friction, float mass)
{
  if (rearsteer>=0 && rearsteer<=1)
    celPcWheeled::frontsteer=sensitivity;
  if(enginepower>=0 && enginepower <=1)
    celPcWheeled::frontpower=enginepower;
  celPcWheeled::frontss=suspensionsoftness;
  celPcWheeled::frontsd=suspensiondamping;
  celPcWheeled::frontfriction=friction;
  celPcWheeled::frontmass=mass;
  for(size_t i=0; i < wheels.GetSize(); i++)
  {
    ApplyWheelPresets(i);
  }
}

void celPcWheeled::SetRearWheelPreset(float sensitivity,float enginepower,
                    float suspensionsoftness,
                    float suspensiondamping,
                    float friction, float mass)
{
  if (rearsteer>=0 && rearsteer<=1)
    celPcWheeled::rearsteer=sensitivity;
  if(enginepower>=0 && enginepower <=1)
    celPcWheeled::rearpower=enginepower;
  celPcWheeled::rearss=suspensionsoftness;
  celPcWheeled::rearsd=suspensiondamping;
  celPcWheeled::rearfriction=friction;
  celPcWheeled::rearmass=mass;
  for(size_t i=0 ; i<wheels.GetSize(); i++)
  {
    ApplyWheelPresets(i);
  }
}

void celPcWheeled::SetOuterWheelSteerPreset(float sensitivity)
{
  if(outersteer>=0 && outersteer<=1)
    celPcWheeled::outersteer=sensitivity;
  
  for(size_t i=0; i<wheels.GetSize(); i++)
  {
    ApplyWheelPresets(i);
  }
}

void celPcWheeled::ApplyWheelPresets(size_t wheelnum)
{
//Apply front and rear steer and power settings
  if(wheels[wheelnum].Position.z<0)
  {
    wheels[wheelnum].LeftSteerSensitivity=frontsteer;
    wheels[wheelnum].RightSteerSensitivity=frontsteer;
    wheels[wheelnum].EnginePower=frontpower;
    wheels[wheelnum].SuspensionSoftness=frontss;
    wheels[wheelnum].SuspensionDamping=frontsd;
    wheels[wheelnum].WheelFriction=frontfriction;
    wheels[wheelnum].WheelMass=frontmass;
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
    wheels[wheelnum].WheelFriction=rearfriction;
    wheels[wheelnum].WheelMass=rearmass;
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

