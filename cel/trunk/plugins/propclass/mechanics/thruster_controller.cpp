/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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
#include "iutil/string.h"
#include "csutil/debug.h"
#include "csgeom/math3d.h"
#include "csgeom/transfrm.h"
#include "ivaria/dynamics.h"

#include "plugins/propclass/mechanics/common.h"
#include "plugins/propclass/mechanics/thruster_controller.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (MechanicsBalancedGroup, "pcmechbalancedgroup")

//---------------------------------------------------------------------------

// Actions
csStringID celPcMechanicsBalancedGroup::action_addthruster = csInvalidStringID;
csStringID celPcMechanicsBalancedGroup::action_settype = csInvalidStringID;

// Parameters for action_addthruster
csStringID celPcMechanicsBalancedGroup::param_thruster = csInvalidStringID;
csStringID celPcMechanicsBalancedGroup::param_multiplier = csInvalidStringID;

// Parameters for action_settype
csStringID celPcMechanicsBalancedGroup::param_type = csInvalidStringID;

// Group types
csStringID celPcMechanicsBalancedGroup::type_rotation = csInvalidStringID;
csStringID celPcMechanicsBalancedGroup::type_translation = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcMechanicsBalancedGroup)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMechanicsBalancedGroup)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsBalancedGroup::PcMechanicsBalancedGroup)
  SCF_IMPLEMENTS_INTERFACE (iPcMechanicsBalancedGroup)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMechanicsBalancedGroup::celPcMechanicsBalancedGroup (
	iObjectRegistry* object_reg) : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMechanicsBalancedGroup);

  if (action_addthruster == csInvalidStringID)
  {
    // Actions
    action_addthruster = pl->FetchStringID ("cel.action.AddThruster");
    action_settype = pl->FetchStringID ("cel.action.SetType");

    // Parameters for action_addthruster
    param_thruster = pl->FetchStringID ("cel.parameter.thrusterpctag");
    param_multiplier = pl->FetchStringID ("cel.parameter.multiplier");

    // Parameters for action_settype
    param_type = pl->FetchStringID ("cel.parameter.type");

    // Group types
    type_rotation = pl->FetchStringID ("rotation");
    type_translation = pl->FetchStringID ("translation");
  }
}

celPcMechanicsBalancedGroup::~celPcMechanicsBalancedGroup ()
{
}

#define THRUSTERGROUP_SERIAL 1

csPtr<iCelDataBuffer> celPcMechanicsBalancedGroup::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (THRUSTERGROUP_SERIAL);
  databuf->Add ((int32) grouptype);
  databuf->Add ((int32) thrusters.GetSize ());
  csArray<celThrusterData*>::Iterator thrustit = thrusters.GetIterator ();
  csRef<iPcMechanicsThruster> thruster;
  celThrusterData* td;
  csRef<iCelPropertyClass> pc;
  while (thrustit.HasNext ())
  {
    td = thrustit.Next ();
    pc = SCF_QUERY_INTERFACE (td->thruster, iCelPropertyClass);
    databuf->Add (pc);
    databuf->Add (td->thrustcoefficient);
  }
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMechanicsBalancedGroup::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != THRUSTERGROUP_SERIAL) return false;
  grouptype = (celAxisType) databuf->GetInt32 ();
  int size = (int) databuf->GetInt32 ();
  csRef<iPcMechanicsThruster> thruster;
  csRef<iCelPropertyClass> pc;
  for (int i = 1; i <= size; i++)
  {
    pc = databuf->GetPC ();
    thruster = SCF_QUERY_INTERFACE (pc, iPcMechanicsThruster);
    AddThruster (thruster, databuf->GetFloat ());
  }
  return true;
}

bool celPcMechanicsBalancedGroup::PerformAction (csStringID actionId,
	iCelParameterBlock* params,
	celData& ret)
{
  if (actionId == action_addthruster)
  {
    CEL_FETCH_STRING_PAR (thruster,params,param_thruster);
    if (!p_thruster)
    {
      CS_REPORT(ERROR,"Couldn't get thruster tag!");
      return false;
    }
    CEL_FETCH_FLOAT_PAR (mult,params,param_multiplier);
    if (!p_mult)
    {
      CS_REPORT(ERROR,"Couldn't get multiplier for thruster!");
      mult = 1.0f;
    }
    csRef<iPcMechanicsThruster> th = CEL_QUERY_PROPCLASS_TAG_ENT
	(GetEntity (), iPcMechanicsThruster, thruster);
    if (!th)
    {
      csString msg = "Couldn't find thruster with given tag: ";
      msg += thruster;
      CS_REPORT(ERROR,msg);
      return false;
    }
    AddThruster (th, mult);
    return true;
  }
  else if (actionId == action_settype)
  {
    CEL_FETCH_STRING_PAR (type,params,param_type);
    if (!p_type)
    {
      CS_REPORT(ERROR,"Couldn't get thruster group type!");
      return false;
    }
    csStringID type_id = pl->FetchStringID (type);
    celAxisType gtype;
    if (type_id == type_rotation)
      gtype = CEL_AT_ROTATION;
    else if (type_id == type_translation)
      gtype = CEL_AT_TRANSLATION;
    else
    {
      CS_REPORT(ERROR,"Invalid thruster group type!");
      return false;
    }
    SetType (gtype);
  }
  return false;
}

void celPcMechanicsBalancedGroup::AddThruster (iPcMechanicsThruster* thruster,
	float multiplier)
{
  csRef<iCelPropertyClass> pc = SCF_QUERY_INTERFACE (thruster,
  	iCelPropertyClass);
  celThrusterData* th = new celThrusterData (pc->GetTag (), thruster,
	multiplier);
  thrusters.Push (th);
}

void celPcMechanicsBalancedGroup::RemoveThruster (const char* thrustertag)
{
  csArray<celThrusterData*>::Iterator it = thrusters.GetIterator ();
  while (it.HasNext ())
  {
    celThrusterData* td = it.Next ();
    if (strcmp (td->tag.GetData (), thrustertag) == 0)
      thrusters.Delete (td);
  }
}

iPcMechanicsThruster* celPcMechanicsBalancedGroup::GetThruster (const char*
	thrustertag)
{
  csArray<celThrusterData*>::Iterator it = thrusters.GetIterator ();
  while (it.HasNext ())
  {
    celThrusterData* td = it.Next ();
    if (strcmp (td->tag.GetData (), thrustertag) == 0)
      return td->thruster;
  }
  return 0;
}

float celPcMechanicsBalancedGroup::AvailableThrust ()
{
  float maxinputthrust = thrusters[0]->thruster->AvailableThrust ();
  float tmpavail = 0;
  csArray<celThrusterData*>::Iterator it = thrusters.GetIterator ();
  celThrusterData* thrusterdata;
  while (it.HasNext ())
  {
    thrusterdata = it.Next();
    tmpavail = (thrusterdata->thruster->AvailableThrust ()) /
	thrusterdata->thrustcoefficient;
    if (tmpavail < maxinputthrust)
      maxinputthrust = tmpavail;
  }
  return maxinputthrust;
}

float celPcMechanicsBalancedGroup::AvailableThrustForce ()
{
  float maxinputthrust = AvailableThrust ();
  float outputforce = 0;
  csArray<celThrusterData*>::Iterator it = thrusters.GetIterator ();
  celThrusterData* thrusterdata;
  while (it.HasNext ())
  {
    thrusterdata = it.Next();
    outputforce += thrusterdata->thruster->GetThrustForce (maxinputthrust *
	thrusterdata->thrustcoefficient);
  }
  return outputforce;
}

void celPcMechanicsBalancedGroup::ChangeThrust (float deltathrust)
{
  csArray<celThrusterData*>::Iterator it = thrusters.GetIterator ();
  celThrusterData* thrusterdata;
  while (it.HasNext ())
  {
    thrusterdata = it.Next ();
    thrusterdata->thruster->ThrustChange (deltathrust *
	thrusterdata->thrustcoefficient);
  }
}

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (MechanicsThrusterController, "pcmechthrustercontroller")

//---------------------------------------------------------------------------

// Actions
csStringID celPcMechanicsThrusterController::action_addaxis = csInvalidStringID;
csStringID celPcMechanicsThrusterController::action_applythrust = csInvalidStringID;
csStringID celPcMechanicsThrusterController::action_addbalancedgroup = csInvalidStringID;
csStringID celPcMechanicsThrusterController::action_inittc = csInvalidStringID;

// Parameters for action_inittc
csStringID celPcMechanicsThrusterController::param_object = csInvalidStringID;

// Parameters for action_addaxis
csStringID celPcMechanicsThrusterController::param_axisname = csInvalidStringID;
csStringID celPcMechanicsThrusterController::param_axistype = csInvalidStringID;
csStringID celPcMechanicsThrusterController::param_axisdir = csInvalidStringID;

// Parameters for action_applythrust
csStringID celPcMechanicsThrusterController::param_thrust = csInvalidStringID;
//param_axisname shared

// Parameters for action_addbalancedgroup
csStringID celPcMechanicsThrusterController::param_balancedgroup = csInvalidStringID;
//param_axisname shared

// Axis types
csStringID celPcMechanicsThrusterController::type_rotation = csInvalidStringID;
csStringID celPcMechanicsThrusterController::type_translation = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcMechanicsThrusterController)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMechanicsThrusterController)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsThrusterController::PcMechanicsThrusterController)
  SCF_IMPLEMENTS_INTERFACE (iPcMechanicsThrusterController)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMechanicsThrusterController::celPcMechanicsThrusterController (
	iObjectRegistry* object_reg) : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMechanicsThrusterController);

  if (action_addaxis == csInvalidStringID)
  {
    // Actions
    action_addaxis = pl->FetchStringID ("cel.action.AddAxis");
    action_applythrust = pl->FetchStringID ("cel.action.ApplyThrust");
    action_addbalancedgroup = pl->FetchStringID ("cel.action.AddBalancedGroup");
    action_inittc = pl->FetchStringID ("cel.action.InitThrusterController");

    // Parameters for action_inittc
    param_object = pl->FetchStringID ("cel.parameter.objectpctag");

    // Parameters for action_addaxis
    param_axisname = pl->FetchStringID ("cel.parameter.axisname");
    param_axistype = pl->FetchStringID ("cel.parameter.axistype");
    param_axisdir = pl->FetchStringID ("cel.parameter.axisdir");

    // Parameters for action_applythrust
    param_thrust = pl->FetchStringID ("cel.parameter.thrust");

    // Parameters for action_addbalancedgroup
    param_balancedgroup = pl->FetchStringID ("cel.parameter.balancedgroup");

    // Axis types
    type_rotation = pl->FetchStringID ("rotation");
    type_translation = pl->FetchStringID ("translation");
  }
}

celPcMechanicsThrusterController::~celPcMechanicsThrusterController ()
{
}

#define THRUSTERCONTROLLER_SERIAL 1

csPtr<iCelDataBuffer> celPcMechanicsThrusterController::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (
  	THRUSTERCONTROLLER_SERIAL);
  csRef<iCelPropertyClass> pc = SCF_QUERY_INTERFACE (mechobject,
  	iCelPropertyClass);
  databuf->Add (pc);
  databuf->Add ((int32) axes.GetSize ());
  csArray<celAxisData*>::Iterator axisit = axes.GetIterator ();
  celAxisData* ad;
  csRef<iPcMechanicsBalancedGroup> tg;
  while (axisit.HasNext ())
  {
    ad = axisit.Next ();
    databuf->Add (ad->name.GetData ());
    databuf->Add (ad->axis);
    databuf->Add ((int32) ad->type);
    databuf->Add ((int32) ad->balancedgroups.GetSize ());
    csRefArray<iPcMechanicsBalancedGroup>::Iterator tgit =
    	ad->balancedgroups.GetIterator ();
    while (tgit.HasNext ())
    {
      tg = tgit.Next ();
      pc = SCF_QUERY_INTERFACE (tg, iCelPropertyClass);
      databuf->Add (pc);
    }
  }
  databuf->Add ((int32) requests.GetSize ());
  csArray<celThrustRequestData*>::Iterator trit = requests.GetIterator ();
  celThrustRequestData* trd;
  while (trit.HasNext ())
  {
    trd = trit.Next ();
    pc = SCF_QUERY_INTERFACE (trd->group, iCelPropertyClass);
    databuf->Add (pc);
    databuf->Add (trd->id);
    databuf->Add (trd->thrust);
  }
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMechanicsThrusterController::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != THRUSTERCONTROLLER_SERIAL) return false;
  csRef<iCelPropertyClass> pc = databuf->GetPC ();
  csRef<iPcMechanicsObject> mechobj = SCF_QUERY_INTERFACE (pc,
  	iPcMechanicsObject);
  int32 axessize = databuf->GetInt32 ();
  int32 tgsize, i, j;
  csRef<iPcMechanicsBalancedGroup> tg;
  for (i = 1; i <= axessize; i++)
  {
    iString* name = databuf->GetString ();
    csVector3 axis;
    databuf->GetVector3 (axis);
    celAxisType type = (celAxisType) databuf->GetInt32 ();
    AddAxis (name->GetData (), type, axis);
    tgsize = databuf->GetInt32 ();
    for (j = 1; j <= tgsize; j++)
    {
      pc = databuf->GetPC ();
      tg = SCF_QUERY_INTERFACE (pc, iPcMechanicsBalancedGroup);
      AddBalancedGroup (tg, name->GetData ());
    }
  }
  int32 requestsize = databuf->GetInt32 ();
  uint32 id;
  float thrust;
  csRef<iPcMechanicsBalancedGroup> group;
  for (i = 1; j <= requestsize; i++)
  {
    pc = databuf->GetPC ();
    group = SCF_QUERY_INTERFACE (pc, iPcMechanicsBalancedGroup);
    id = databuf->GetUInt32 ();
    thrust = databuf->GetFloat ();
    ApplyThrustHelper (thrust, group, id);
  }
  return true;
}

bool celPcMechanicsThrusterController::PerformAction (csStringID actionId,
	iCelParameterBlock* params,
	celData& ret)
{
  if (actionId == action_addaxis)
  {
    CEL_FETCH_STRING_PAR (axisname,params,param_axisname);
    if (!p_axisname)
    {
      CS_REPORT(ERROR,"Couldn't get axis name!");
      return false;
    }
    CEL_FETCH_STRING_PAR (axistype,params,param_axistype);
    if (!p_axistype)
    {
      CS_REPORT(ERROR,"Couldn't get axis type!");
      return false;
    }
    csStringID type_id = pl->FetchStringID (axistype);
    celAxisType atype;
    if (type_id == type_rotation)
      atype = CEL_AT_ROTATION;
    else if (type_id == type_translation)
      atype = CEL_AT_TRANSLATION;
    else {
      CS_REPORT(ERROR,"Invalid axis type!");
      return false;
    }
    CEL_FETCH_VECTOR3_PAR (axisdir,params,param_axisdir);
    if (!p_axisdir)
    {
      CS_REPORT(ERROR,"Couldn't get axis direction!");
      return false;
    }
    AddAxis (axisname, atype, axisdir);
    return true;
  }
  else if (actionId == action_applythrust)
  {
    CEL_FETCH_STRING_PAR (axisname,params,param_axisname);
    if (!p_axisname)
    {
      CS_REPORT(ERROR,"Couldn't get axis name!");
      return false;
    }
    CEL_FETCH_FLOAT_PAR (thrust,params,param_thrust);
    if (!p_thrust)
    {
      CS_REPORT(ERROR,"Couldn't get thrust!");
      return false;
    }
    uint32 forceid;
    ApplyThrust (thrust, axisname, forceid);
    //TODO: Any way to return forceid to the caller?
  }
  else if (actionId == action_addbalancedgroup)
  {
    CEL_FETCH_STRING_PAR (axisname,params,param_axisname);
    if (!p_axisname)
    {
      CS_REPORT(ERROR,"Couldn't get axis name!");
      return false;
    }
    CEL_FETCH_STRING_PAR (balancedgrouppctag,params,param_balancedgroup);
    if (!p_balancedgrouppctag)
    {
      CS_REPORT(ERROR,"Couldn't get thruster group tag!");
      return false;
    }
    csRef<iPcMechanicsBalancedGroup> tg = CEL_QUERY_PROPCLASS_TAG_ENT
	(GetEntity (), iPcMechanicsBalancedGroup, balancedgrouppctag);
    AddBalancedGroup (tg, axisname);
  }
  else if (actionId == action_inittc)
  {
    CEL_FETCH_STRING_PAR (objectpctag,params,param_object);
    if (p_objectpctag)
    {
      csRef<iPcMechanicsObject> mechobj = 0;
      mechobj = CEL_QUERY_PROPCLASS_TAG_ENT(GetEntity (),
      	iPcMechanicsObject,objectpctag);
      assert (mechobj);
      SetMechanicsObject (mechobj);
    }
    else
    {
      return false;
    }
  }
  return false;
}

void celPcMechanicsThrusterController::SetMechanicsObject (iPcMechanicsObject*
	mechobj)
{
  mechobject = mechobj;
}

iPcMechanicsObject* celPcMechanicsThrusterController::GetMechanicsObject ()
{
  return mechobject;
}

void celPcMechanicsThrusterController::AddAxis (const char* name,
	celAxisType type, const csVector3 axis)
{
  celAxisData* newdat = new celAxisData (name, axis, type);
  axes.Push (newdat);
}

const csVector3 celPcMechanicsThrusterController::GetAxis (const char* name)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData* ad = 0;
  while (it.HasNext ())
  {
    ad = it.Next ();
    if (strcmp (ad->name.GetData (), name) == 0)
      break;
  }
  if (ad)
    return ad->axis;
  else
    return csVector3 (0);
}

const celAxisType celPcMechanicsThrusterController::GetAxisType (
	const char* name)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData* ad = 0;
  while (it.HasNext ())
  {
    ad = it.Next ();
    if (strcmp (ad->name.GetData (), name) == 0)
      break;
  }
  if (ad)
    return ad->type;
  else
    return CEL_AT_NONE;
}

const float celPcMechanicsThrusterController::GetAxisVelocity (
	const char* name)
{
  if (mechobject != 0) {
    csArray<celAxisData*>::Iterator it = axes.GetIterator ();
    celAxisData* ad = 0;
    while (it.HasNext ())
    {
      ad = it.Next ();
      if (strcmp (ad->name.GetData (), name) == 0)
        break;
    }
    if (ad)
    {
      csRef<iRigidBody> tehbody = mechobject->GetBody ();
      csOrthoTransform trans = tehbody->GetTransform ();
      csVector3 vvel;
      if (ad->type == CEL_AT_ROTATION)
        vvel = tehbody->GetAngularVelocity ();
      else
        vvel = -tehbody->GetLinearVelocity ();
      return trans.Other2ThisRelative (vvel) * ad->axis.Unit ();;
    }
  }
  return 0;
}

float celPcMechanicsThrusterController::GetAxisMaxForce (const char* axisname)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData* ad = 0;
  while (it.HasNext ())
  {
    ad = it.Next ();
    if (strcmp (ad->name.GetData (), axisname) == 0)
    {
      csVector3 axis = ad->axis;
      csRefArray<iPcMechanicsBalancedGroup>::Iterator groupit
	= ad->balancedgroups.GetIterator ();
      csRef<iPcMechanicsBalancedGroup> group;
      float maxstrength = 0;
      while (groupit.HasNext ())
      {
        group = groupit.Next ();
        if (group->AvailableThrustForce () > maxstrength)
          maxstrength = group->AvailableThrustForce ();
      }
      return maxstrength;
    }
  }
  CS_REPORT(ERROR,(csString ("Invalid axis specified: ") + axisname)
  	.GetData ());
  return 0;
}

float celPcMechanicsThrusterController::GetAxisMaxThrust (const char* axisname)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData* ad = 0;
  while (it.HasNext ())
  {
    ad = it.Next ();
    if (strcmp (ad->name.GetData (), axisname) == 0)
    {
      csVector3 axis = ad->axis;
      csRefArray<iPcMechanicsBalancedGroup>::Iterator groupit
	= ad->balancedgroups.GetIterator ();
      csRef<iPcMechanicsBalancedGroup> group, strongestgroup;
      float maxstrength = 0;
      while (groupit.HasNext ())
      {
        group = groupit.Next ();
        if (group->AvailableThrustForce () > maxstrength)
        {
          maxstrength = group->AvailableThrustForce ();
          strongestgroup = group;
        }
      }
      if (strongestgroup)
        return strongestgroup->AvailableThrust ();
      return 0;
    }
  }
  CS_REPORT(ERROR,(csString ("Invalid axis specified: ") + axisname)
  	.GetData ());
  return 0;
}

void celPcMechanicsThrusterController::AddBalancedGroup
	(iPcMechanicsBalancedGroup* balancedgroup, const char* axisname)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData *adi = 0, *ad = 0;
  while (it.HasNext ())
  {
    adi = it.Next ();
    if (strcmp (adi->name.GetData (), axisname) == 0)
    {
      ad = adi;
      break;
    }
  }
  if (ad)
    ad->balancedgroups.Push (balancedgroup);
  else
    CS_REPORT(ERROR,(csString ("Couldn't find specified axis: ")
    	+ axisname).GetData ());
}

void celPcMechanicsThrusterController::RemoveBalancedGroup (const char*
	balancedgrouptag, const char* axisname)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData* ad = 0;
  while (it.HasNext ())
  {
    ad = it.Next ();
    if (strcmp (ad->name.GetData (), axisname) == 0)
    {
      csRefArray<iPcMechanicsBalancedGroup>::Iterator groupit
	= ad->balancedgroups.GetIterator ();
      csRef<iPcMechanicsBalancedGroup> group;
      csRef<iCelPropertyClass> pc;
      while (groupit.HasNext ())
      {
        group = groupit.Next ();
	pc = SCF_QUERY_INTERFACE (group, iCelPropertyClass);
        if (!strcmp (pc->GetTag (), balancedgrouptag))
          ad->balancedgroups.Delete (group);
      }
    }
  }
}

void celPcMechanicsThrusterController::ApplyThrustHelper (float thrust,
	iPcMechanicsBalancedGroup* group, uint32 id)
{
  celThrustRequestData* trd = new celThrustRequestData (group, id, thrust);
  requests.Push (trd);
  group->ChangeThrust (thrust);
}

void celPcMechanicsThrusterController::ApplyThrust (float thrust,
	const char* axisname, uint32& id)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData* ad = 0;
  while (it.HasNext ())
  {
    ad = it.Next ();
    if (strcmp (ad->name.GetData (), axisname) == 0)
    {
      if (ad->balancedgroups.IsEmpty ())
      {
        CS_REPORT(ERROR,"No groups in this axis!");
	return;
      }
      csRefArray<iPcMechanicsBalancedGroup>::Iterator groupit
	= ad->balancedgroups.GetIterator ();
      csRef<iPcMechanicsBalancedGroup> group;
      csRef<iPcMechanicsBalancedGroup> bestgroup = 0;
      float beststrength = 0;
      while (groupit.HasNext ())
      {
        group = groupit.Next ();
        if (group != 0 && (bestgroup == 0
		|| group->AvailableThrust () > beststrength))
        {
          beststrength = group->AvailableThrust ();
          bestgroup = group;
        }
      }
      if (bestgroup != 0)
      {
	lastrequestid++;
	id = lastrequestid;
        ApplyThrustHelper (thrust, bestgroup, id);
      }
      else
      {
        CS_REPORT(ERROR,"No best group found. Something's wrong!");
      }
      return;
    }
  }
  CS_REPORT(ERROR,(csString ("Invalid axis specified: ") +
	axisname).GetData ());
}

void celPcMechanicsThrusterController::CancelThrust (uint32 id)
{
  csArray<celThrustRequestData*>::Iterator trit = requests.GetIterator ();
  celThrustRequestData* trd = 0;
  while (trit.HasNext ())
  {
    trd = trit.Next ();
    if (trd->id == id)
    {
      trd->group->ChangeThrust (-trd->thrust);
      requests.Delete (trd);
      delete trd;
    }
  }
}

//---------------------------------------------------------------------------

