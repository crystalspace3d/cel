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

#include "iengine/mesh.h"
#include "imesh/objmodel.h"
#include "imesh/object.h"
#include "propclass/mesh.h"
#include "propclass/mechsys.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Wheeled, "pcwheeled")

//---------------------------------------------------------------------------

csStringID celPcWheeled::id_message = csInvalidStringID;
csStringID celPcWheeled::action_print = csInvalidStringID;

celPcWheeled::celPcWheeled (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
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
  autotransmission=true;

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
//These are the two exposed initialisation functions. They just call the real
//initialisation with a centre of gravity offset, either user given, or autocalulated.
void celPcWheeled::Initialise(csVector3 centreoffset)
{
 csRef<iPcMesh> bodyMesh=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMesh);
 csBox3 boundingbox;
 bodyMesh->GetMesh ()->GetMeshObject ()->GetObjectModel ()->GetObjectBoundingBox(boundingbox);
 CreateBody(boundingbox.GetSize(), centreoffset);
}

//This function calculates the offset of the body's collider based on it's middle
//as the bottom of the body.
void celPcWheeled::Initialise()
{
 csRef<iPcMesh> bodyMesh=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMesh);
 csBox3 boundingbox;
 bodyMesh->GetMesh ()->GetMeshObject ()->GetObjectModel ()->GetObjectBoundingBox(boundingbox);
 CreateBody(boundingbox.GetSize(), csVector3(0,boundingbox.GetSize().y/2,0));
}

//This is the actual initialisation function.
void celPcWheeled::CreateBody(csVector3 vehiclesize, csVector3 centreoffset)
{
 csRef<iPcMesh> bodyMesh=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMesh);
 csRef<iPcMechanicsObject> bodyMech=CEL_QUERY_PROPCLASS_ENT(GetEntity(),iPcMechanicsObject);
 bodyMech->SetMass(1.0);
 bodyMech->SetDensity(1.0);
 csOrthoTransform t;
 t.SetOrigin(centreoffset);

  csRef<iDynamicSystem> dyn=bodyMech->GetMechanicsSystem()->GetDynamicSystem();
  bodyGroup=dyn->CreateGroup();
  bodyMech->AttachColliderBox(vehiclesize,t);
  bodyGroup->AddBody(bodyMech->GetBody());
}

int celPcWheeled::AddWheel(csRef<iCelEntity> wheel, int steeringmode, bool powered)
{
return 0;
}

void celPcWheeled::RemoveWheel(int wheelnum)
{
}

void celPcWheeled::Accelerate()
{
}

void celPcWheeled::Brake()
{
}

void celPcWheeled::HandBrake()
{
}

void celPcWheeled::SteerLeft()
{
}

void celPcWheeled::SteerRight()
{
}

void celPcWheeled::Reverse()
{
}

void celPcWheeled::SetAutoTransmission(bool auto)
{
}

void celPcWheeled::SetGear(int gear)
{
}

void celPcWheeled::SetGearSettings(int gear, float velocity, float force)
{
}

void celPcWheeled::SetNumberGears(int number)
{
}

//---------------------------------------------------------------------------

