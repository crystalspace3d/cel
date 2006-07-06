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
  steerdir=0;
  gear=0;
  numberwheels=0;
  steering=false;
  autotransmission=true;
  accelerating=false;
  braking=false;
  reversing=false;
  handbrakeapplied=false;
  autotransmission=true;
  wheelradius=0;
  
  steeramount=0.75;

//Gear -1 is reverse, 0 is neutral
  SetNumberGears(1);
  SetGearSettings(-1,-25,3000);
  SetGearSettings(0,0,100);
  SetGearSettings(1,150,2000);
  brakeforce=1000;

 tankmode=false;

  pl->CallbackOnce ((iCelTimerListener*)this, 100, CEL_EVENT_PRE);
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

  //SetWheelMesh("/cel/data/celcarwheel","celCarWheel");
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
     t.RotateThis(csVector3(0,1,0),3.14f);
     wheelbody->SetTransform(t);
  }

  //Create the joint
  csRef<iODEDynamicSystemState> osys=SCF_QUERY_INTERFACE (dyn, iODEDynamicSystemState);
  csRef<iODEHinge2Joint> joint=osys->CreateHinge2Joint();
  joint->Attach(bodyMech->GetBody(),wheelbody);
  joint->SetHingeAnchor(bodytransform.This2Other(position));
  joint->SetHingeAxis1(csVector3(0,1,0));
  joint->SetHingeAxis2(csVector3(1,0,0));
  joint->SetSuspensionCFM(0.000125f,0);
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
  wheel.BrakePower=300;
  wheel.LeftSteerOffset=0;
  wheel.RightSteerOffset=0;
  wheel.TurnSpeed=5;
  wheel.BrakePower=1;
  wheel.EnginePower=1;
  wheel.SteerInverted=false;
  wheels.Push(wheel);
  return 0;
}

void celPcWheeled::RemoveWheel(int wheelnum)
{
}

void celPcWheeled::Accelerate()
{
  gear=1;
}

void celPcWheeled::Brake()
{
  braking=true;
}

void celPcWheeled::HandBrake()
{
  handbrakeapplied=true;
}

void celPcWheeled::SteerLeft()
{
steerdir=-1;
}

void celPcWheeled::SteerRight()
{
steerdir=1;
}
void celPcWheeled::SteerStraight()
{
steerdir=0;
}

void celPcWheeled::UpdateTankSteer()
{
  if(gear!=0)
  {
    for(size_t i =0; i < wheels.Length() ; i++)
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
  else
  {
    for(size_t i =0; i < wheels.Length() ; i++)
    {
      //It's a right wheel
      if (wheels[i].Position.x < 0 )
      {
        wheels[i].WheelJoint->SetVel(-steerdir*steeramount*20,1);
        wheels[i].WheelJoint->SetFMax(gears[2].y, 1);
      }
      //It's a left wheel
      if (wheels[i].Position.x > 0 )
      {
        wheels[i].WheelJoint->SetVel(steerdir*steeramount*20,1);
        wheels[i].WheelJoint->SetFMax(gears[2].y, 1);
      }
    }
  }
}

//Update the vehicle. Order is important here! steering and braking must come after accelerating and reversing.
void celPcWheeled::TickOnce()
{

  if(gear>0)
      UpdateGear();

 for(size_t i=0; i < wheels.Length();i++)
    {
      wheels[i].WheelJoint->SetVel(gears[gear+1].x,1);
      wheels[i].WheelJoint->SetFMax(gears[gear+1].y,1);
    }

  if(braking)
  {
    for(size_t i=0; i < wheels.Length();i++)
    {
      wheels[i].WheelJoint->SetVel(0,1);
      wheels[i].WheelJoint->SetFMax(brakeforce,1);
    }
  }
  if(tankmode && steerdir!=0)
    UpdateTankSteer();
  pl->CallbackOnce ((iCelTimerListener*)this, 30, CEL_EVENT_PRE);
}

void celPcWheeled::Reverse()
{
  gear=-1;
}

void celPcWheeled::Roll()
{
  gear=0;
}

void celPcWheeled::SetAutoTransmission(bool autotransmission)
{
  celPcWheeled::autotransmission=autotransmission;
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

void celPcWheeled::SetNumberGears(int number)
{
  gears.SetSize(number+2);
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

//---------------------------------------------------------------------------

