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

csStringID celPcWheeled::id_message = csInvalidStringID;
csStringID celPcWheeled::action_print = csInvalidStringID;

celPcWheeled::celPcWheeled (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  // For SendMessage parameters.
  if (id_message == csInvalidStringID)
    id_message = pl->FetchStringID ("cel.parameter.message");
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_message, "message");

  // For PerformAction.
  if (action_print == csInvalidStringID)
    action_print = pl->FetchStringID ("cel.action.Print");

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_counter] = &counter;	// Automatically handled.
  propdata[propid_max] = 0;		// Handled in this class.

  counter = 0;
  max = 0;

  numbergears=1;
  gear=1;
  numberwheels=0;
  steering=false;
  autotransmission=true;
  accelerating=false;
  braking=false;
  reversing=false;
  handbrakeapplied=false;
  autotransmission=true;
  wheelradius=0;
  steeramount=0;
  currentsteerangle=0;

//Gear 0 is reverse
  gears.SetSize(2);
  SetGearSettings(0,-25,1000);
  SetGearSettings(1,100,1000);

  SetWheelMesh("/cel/data/celcarwheel","celCarWheel");
  steeringmode=CEL_WHEELED_FRONT_STEER;

  pl->CallbackOnce ((iCelTimerListener*)this, 30, CEL_EVENT_PRE);
}

celPcWheeled::~celPcWheeled ()
{
  delete params;
}

Property* celPcWheeled::properties = 0;
size_t celPcWheeled::propertycount = 0;

void celPcWheeled::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    propertycount = 2;
    properties = new Property[propertycount];

    properties[propid_counter].id = pl->FetchStringID (
        "cel.property.counter");
    properties[propid_counter].datatype = CEL_DATA_LONG;
    properties[propid_counter].readonly = false;
    properties[propid_counter].desc = "Print counter.";

    properties[propid_max].id = pl->FetchStringID (
        "cel.property.max");
    properties[propid_max].datatype = CEL_DATA_LONG;
    properties[propid_max].readonly = false;
    properties[propid_max].desc = "Max length.";
  }
}

bool celPcWheeled::SetProperty (csStringID propertyId, long b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_max].id)
  {
    max = b;
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

long celPcWheeled::GetPropertyLong (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_max].id)
  {
    return (long)max;
  }
  else
  {
    return celPcCommon::GetPropertyLong (propertyId);
  }
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
  if (actionId == action_print)
  {
    CEL_FETCH_STRING_PAR (msg,params,id_message);
    if (!p_msg) return false;
    Print (msg);
    return true;
  }
  return false;
}

void celPcWheeled::Print (const char* msg)
{
  printf ("Print: %s\n", msg);
  fflush (stdout);
  iCelBehaviour* ble = entity->GetBehaviour ();
  if (ble)
  {
    celData ret;
    params->GetParameter (0).Set (msg);
    ble->SendMessage ("pcvehicle_print", this, ret, params);
  }

  counter++;
  size_t l = strlen (msg);
  if (l > max) max = l;
}

//This function calculates the offset of the body's collider based on it's middle
//as the bottom of the body.
void celPcWheeled::Initialise()
{
  csRef<iPcMesh> bodyMesh=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMesh);
  csBox3 boundingbox;
  bodyMesh->GetMesh ()->GetMeshObject ()->GetObjectModel ()->GetObjectBoundingBox(boundingbox);

  bodyMech=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMechanicsObject);
  csRef<iDynamicSystem> dyn=bodyMech->GetMechanicsSystem()->GetDynamicSystem();
  bodyGroup=dyn->CreateGroup();
  csOrthoTransform t;
  t.SetOrigin(boundingbox.GetCenter());
  bodyMech->AttachColliderBox(boundingbox.GetSize(), t);
  bodyGroup->AddBody(bodyMech->GetBody());
}

void celPcWheeled::SetWheelMesh(const char* file, const char* factname)
{
  csRef<iLoader> loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  CS_ASSERT (loader != 0);
  iBase* result;
  loader->Load (file, result, 0, false, true);
  csRef<iVFS> vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  wheelfact=engine->FindMeshFactory(factname);
}

void celPcWheeled::SetSteeringMode(int steeringmode)
{
  celPcWheeled::steeringmode=steeringmode;
}

int celPcWheeled::AddWheel(csVector3 position)
{
  //Create the mesh
  csRef<iPcMesh> bodyMesh=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMesh); 
  csOrthoTransform bodytransform=bodyMesh->GetMesh()->GetMovable()->GetTransform();
  csRef<iMeshWrapper> wheelmesh=0;
  csRef<iSectorList> bodySectors=bodyMesh->GetMesh()->GetMovable()->GetSectors();
  if(bodySectors->GetCount() > 0)
  {
    csRef<iSector> bodySector=bodySectors->Get(0);
    wheelmesh=engine->CreateMeshWrapper(wheelfact,"wheel",bodySector,position);
  }
  else
  {
    wheelmesh=engine->CreateMeshWrapper(wheelfact,"wheel");
    wheelmesh->GetMovable()->SetPosition(position);
    wheelmesh->GetMovable()->UpdateMove();
  }
  
  //Create the dynamic body
  csRef<iDynamicSystem> dyn=bodyMech->GetMechanicsSystem()->GetDynamicSystem();
  csRef<iRigidBody> wheelbody=dyn->CreateBody();
  bodyGroup->AddBody(wheelbody);

  csVector3 wheelcenter;
  wheelmesh->GetMeshObject ()->GetObjectModel ()->GetRadius(wheelradius,wheelcenter);
  wheelbody->SetProperties (10, csVector3 (0), csMatrix3 ());
  wheelbody->SetPosition(bodytransform.This2Other(position));
  wheelbody->AttachMesh(wheelmesh);
  wheelbody->AttachColliderSphere(wheelradius,wheelcenter,1.0,1,0.5f,0.5f);
    //If it a right wheel, flip it.
  if (position.x<0)
  {
    csOrthoTransform t=wheelbody->GetTransform();
     t.RotateThis(csVector3(0,1,0),3.14);
     wheelbody->SetTransform(t);
  }

  //Create the joint
  csRef<iODEDynamicSystemState> osys=SCF_QUERY_INTERFACE (dyn, iODEDynamicSystemState);
  csRef<iODEHinge2Joint> joint=osys->CreateHinge2Joint();
  joint->Attach(bodyMech->GetBody(),wheelbody);
  joint->SetHingeAnchor(bodytransform.This2Other(position));
  joint->SetHingeAxis1(csVector3(0,1,0));
  joint->SetHingeAxis2(csVector3(1,0,0));
  joint->SetSuspensionCFM(0.000125,0);
  joint->SetSuspensionERP(0.125,0);
  joint->SetLoStop(0,0);
  joint->SetHiStop(0,0);
  joint->SetVel(0,0);
  joint->SetVel(0,1);
  joint->SetStopERP(1.0,0);
  joint->SetFMax(5000,0);
  joint->SetFMax(100,1);

  celWheel wheel;
  wheel.RigidBody=wheelbody;
  wheel.WheelJoint=joint;
  wheel.Position=position;
  wheels.Push(wheel);
  return 0;
}

void celPcWheeled::RemoveWheel(int wheelnum)
{
}

void celPcWheeled::Accelerate()
{
  accelerating=true;
}

void celPcWheeled::Brake()
{
  braking=true;
}

void celPcWheeled::HandBrake()
{
  handbrakeapplied=true;
}

void celPcWheeled::SteerLeft(float amount)
{
  steeramount=amount;
}

void celPcWheeled::SteerRight(float amount)
{
  steeramount=-amount;
}
void celPcWheeled::SteerStraight()
{
  steeramount=0;
}

void celPcWheeled::UpdateSteer()
{
  if( currentsteerangle > steeramount-0.1 && currentsteerangle < steeramount+0.1 )
    currentsteerangle=steeramount;
  if( currentsteerangle < steeramount)
    currentsteerangle+=0.1;
  if( currentsteerangle > steeramount)
    currentsteerangle-=0.1;

    //Steer all front wheels that should be steered.
      if(steeringmode==CEL_WHEELED_FRONT_STEER
         || steeringmode==CEL_WHEELED_ALL_STEER)
      {
        for(int i =0; i < wheels.Length() ; i++)
        {
        //It's a front wheel, set it
          if (wheels[i].Position.z < 0 )
          {
            wheels[i].WheelJoint->SetLoStop(currentsteerangle, 0);
            wheels[i].WheelJoint->SetHiStop(currentsteerangle, 0);
          }
        }
      }
    //Steer all rear wheels that should be steered.
      if(steeringmode==CEL_WHEELED_REAR_STEER
         || steeringmode==CEL_WHEELED_ALL_STEER)
      {
        for(int i =0; i < wheels.Length() ; i++)
        {
        //It's a rear wheel, set it
          if (wheels[i].Position.z > 0 )
          {
            wheels[i].WheelJoint->SetLoStop(-currentsteerangle, 0);
            wheels[i].WheelJoint->SetHiStop(-currentsteerangle, 0);
          }
        }
      }

  //Update the tank-style steering
  if(steeringmode==CEL_WHEELED_TANK_STEER)
  {
    if(accelerating || reversing)
    {
      for(int i =0; i < wheels.Length() ; i++)
      {
        //It's a right wheel, steering right. slow it down
        if (wheels[i].Position.x < 0 && steeramount < 0)
        {
          wheels[i].WheelJoint->SetVel(0,1);
          wheels[i].WheelJoint->SetFMax(-steeramount*gears[1].y,1);
        }
        //It's a left wheel,, steering left. slow it down
        if (wheels[i].Position.x > 0  && steeramount > 0)
        {
          wheels[i].WheelJoint->SetVel(0,1);
          wheels[i].WheelJoint->SetFMax(steeramount*gears[1].y,1);
        }
      }
    }
    else
    {
      for(int i =0; i < wheels.Length() ; i++)
      {
        //It's a right wheel
        if (wheels[i].Position.x < 0 )
        {
          wheels[i].WheelJoint->SetVel(steeramount*20,1);
          wheels[i].WheelJoint->SetFMax(gears[1].y, 1);
        }
        //It's a left wheel
        if (wheels[i].Position.x > 0 )
        {
          wheels[i].WheelJoint->SetVel(-steeramount*20,1);
          wheels[i].WheelJoint->SetFMax(gears[1].y, 1);
        }
      }
    }
  }
}

//Update the vehicle. Order is important here! steering and braking must come after accelerating and reversing.
void celPcWheeled::TickOnce()
{
  if (autotransmission)
    UpdateGear();

  if(accelerating)
  {
    for(int i=0; i < wheels.Length();i++)
    {
      wheels[i].WheelJoint->SetVel(gears[gear].x,1);
      wheels[i].WheelJoint->SetFMax(gears[gear].y,1);
    }
  }
  
  if(reversing)
  {
    for(int i=0; i < wheels.Length();i++)
    {
      wheels[i].WheelJoint->SetVel(gears[0].x , 1);
      wheels[i].WheelJoint->SetFMax(gears[0].y , 1);
    }
  }
  if(braking)
  {
    for(int i=0; i < wheels.Length();i++)
    {
      wheels[i].WheelJoint->SetVel(0,1);
      wheels[i].WheelJoint->SetFMax(300,1);
    }
  }
  
  UpdateSteer();
  pl->CallbackOnce ((iCelTimerListener*)this, 30, CEL_EVENT_PRE);
}

void celPcWheeled::Reverse()
{
  reversing=true;
}

void celPcWheeled::Roll()
{
  accelerating=false;
  braking=false;
  reversing=false;
  for(int i=0; i < wheels.Length();i++)
  {
    wheels[i].WheelJoint->SetVel(0 , 1);
    wheels[i].WheelJoint->SetFMax(300 , 1);
  }
}

void celPcWheeled::SetAutoTransmission(bool autotransmission)
{
  celPcWheeled::autotransmission=autotransmission;
}

void celPcWheeled::SetGear(int gear)
{
  celPcWheeled::gear=gear;
}

void celPcWheeled::SetGearSettings(int gear, float velocity, float force)
{
  if (gear < gears.Length())
  {
    gears.Get(gear).x=velocity;
    gears.Get(gear).y=force;
  }
}

void celPcWheeled::SetNumberGears(int number)
{
  gears.SetSize(number+1);
}

void celPcWheeled::UpdateGear()
{
  for(int i=0; i < gears.Length()-1; i++)
  {
    if (bodyMech->GetBody()->GetLinearVelocity().Norm()
        >= (gears[i].x)*wheelradius*wheelradius*3.14-2 )
    {

      gear=i+1;
    }
  }
}

//---------------------------------------------------------------------------

