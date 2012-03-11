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

CEL_IMPLEMENT_FACTORY_ALT (MechanicsBalancedGroup, "pcphysics.thruster.group", "pcmechbalancedgroup")

//---------------------------------------------------------------------------

// Parameters for action_addthruster
csStringID celPcMechanicsBalancedGroup::param_thruster = csInvalidStringID;
csStringID celPcMechanicsBalancedGroup::param_multiplier = csInvalidStringID;

// Parameters for action_settype
csStringID celPcMechanicsBalancedGroup::param_type = csInvalidStringID;

// Group types
csStringID celPcMechanicsBalancedGroup::type_rotation = csInvalidStringID;
csStringID celPcMechanicsBalancedGroup::type_translation = csInvalidStringID;

PropertyHolder celPcMechanicsBalancedGroup::propinfo;

celPcMechanicsBalancedGroup::celPcMechanicsBalancedGroup (
	iObjectRegistry* object_reg) : scfImplementationType (this, object_reg)
{
  if (param_thruster == csInvalidStringID)
  {
    // Parameters for action_addthruster
    param_thruster = pl->FetchStringID ("thrusterpctag");
    param_multiplier = pl->FetchStringID ("multiplier");

    // Parameters for action_settype
    param_type = pl->FetchStringID ("type");

    // Group types
    type_rotation = pl->FetchStringID ("rotation");
    type_translation = pl->FetchStringID ("translation");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.mechanics.group.action.");
    AddAction (action_addthruster, "AddThruster");
    AddAction (action_settype, "SetType");
  }
}

celPcMechanicsBalancedGroup::~celPcMechanicsBalancedGroup ()
{
}

bool celPcMechanicsBalancedGroup::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_addthruster:
      {
	csString thruster;
	if (!Fetch (thruster, params, param_thruster)) return false;
        CEL_FETCH_FLOAT_PAR (mult,params,param_multiplier);
        if (!p_mult)
        {
          CS_REPORT(ERROR,"Couldn't get multiplier for thruster!");
          mult = 1.0f;
        }
        csRef<iPcMechanicsThruster> th = celQueryPropertyClassTagEntity<iPcMechanicsThruster>
	    (GetEntity (), thruster);
        if (!th)
        {
          csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
              "cel.propclass.mechanics",
              "Couldn't find thruster with given tag: %s", thruster.GetData ());
          return false;
        }
        AddThruster (th, mult);
        return true;
      }
    case action_settype:
      {
	csString type;
	if (!Fetch (type, params, param_type)) return false;
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
	return true;
      }
    default:
      return false;
  }
}

void celPcMechanicsBalancedGroup::AddThruster (iPcMechanicsThruster* thruster,
	float multiplier)
{
  csRef<iCelPropertyClass> pc = scfQueryInterface<iCelPropertyClass> (thruster);
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

CEL_IMPLEMENT_FACTORY_ALT (MechanicsThrusterController, "pcphysics.thruster.controller", "pcmechthrustercontroller")

//---------------------------------------------------------------------------

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

PropertyHolder celPcMechanicsThrusterController::propinfo;

celPcMechanicsThrusterController::celPcMechanicsThrusterController (
	iObjectRegistry* object_reg) : scfImplementationType (this, object_reg)
{
  if (param_object == csInvalidStringID)
  {
    // Parameters for action_inittc
    param_object = pl->FetchStringID ("objectpctag");

    // Parameters for action_addaxis
    param_axisname = pl->FetchStringID ("axisname");
    param_axistype = pl->FetchStringID ("axistype");
    param_axisdir = pl->FetchStringID ("axisdir");

    // Parameters for action_applythrust
    param_thrust = pl->FetchStringID ("thrust");

    // Parameters for action_addbalancedgroup
    param_balancedgroup = pl->FetchStringID ("balancedgroup");

    // Axis types
    type_rotation = pl->FetchStringID ("rotation");
    type_translation = pl->FetchStringID ("translation");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.mechanics.thruster.action.");
    AddAction (action_addaxis, "AddAxis");
    AddAction (action_applythrust, "ApplyThrust");
    AddAction (action_addbalancedgroup, "AddBalancedGroup");
    AddAction (action_inittc, "InitThrusterController");
  }
}

celPcMechanicsThrusterController::~celPcMechanicsThrusterController ()
{
}

bool celPcMechanicsThrusterController::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_addaxis:
      {
	csString axisname, axistype;
	if (!Fetch (axisname, params, param_axisname)) return false;
	if (!Fetch (axistype, params, param_axistype)) return false;
        csStringID type_id = pl->FetchStringID (axistype);
        celAxisType atype;
        if (type_id == type_rotation)
          atype = CEL_AT_ROTATION;
        else if (type_id == type_translation)
          atype = CEL_AT_TRANSLATION;
        else
	{
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
    case action_applythrust:
      {
	csString axisname;
	if (!Fetch (axisname, params, param_axisname)) return false;
        CEL_FETCH_FLOAT_PAR (thrust,params,param_thrust);
        if (!p_thrust)
        {
          CS_REPORT(ERROR,"Couldn't get thrust!");
          return false;
        }
        uint32 forceid;
        ApplyThrust (thrust, axisname, forceid);
        //TODO: Any way to return forceid to the caller?
	return true;
      }
    case action_addbalancedgroup:
      {
	csString axisname, balancedgrouppctag;
	if (!Fetch (axisname, params, param_axisname)) return false;
	if (!Fetch (balancedgrouppctag, params, param_balancedgroup)) return false;
        csRef<iPcMechanicsBalancedGroup> tg = celQueryPropertyClassTagEntity<iPcMechanicsBalancedGroup>
	    (GetEntity (), balancedgrouppctag);
        AddBalancedGroup (tg, axisname);
	return true;
      }
    case action_inittc:
      {
	csString objectpctag;
	if (!Fetch (objectpctag, params, param_object)) return false;
	csRef<iPcMechanicsObject> mechobj = 0;
	mechobj = celQueryPropertyClassTagEntity<iPcMechanicsObject> (GetEntity (),
	  objectpctag);
	assert (mechobj);
	SetMechanicsObject (mechobj);
	return true;
      }
    default:
      return false;
  }
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
  csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.propclass.mechanics",
      "Invalid axis specified: %s", axisname);
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
  csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.propclass.mechanics",
      "Invalid axis specified:  %s", axisname);
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
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
        "cel.propclass.mechanics",
        "Couldn't find specified axis: %s", axisname);
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
	pc = scfQueryInterface<iCelPropertyClass> (group);
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
  csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.propclass.mechanics",
      "Invalid axis specified: %s", axisname);
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

