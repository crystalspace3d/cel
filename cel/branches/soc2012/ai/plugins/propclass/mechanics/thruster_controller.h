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

#ifndef __CEL_PF_MECHANICS_THRUSTER_CONTROLLER_FACTORY__
#define __CEL_PF_MECHANICS_THRUSTER_CONTROLLER_FACTORY__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "csutil/hash.h"

#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/mechsys.h"
#include "propclass/mechthruster.h"

struct iCelEntity;
struct iObjectRegistry;

struct celThrusterData
{
public:
  csWeakRef<iPcMechanicsThruster> thruster;
  float thrustcoefficient;
  csString tag;

  celThrusterData (const char* tag, iPcMechanicsThruster* thruster, float
	thrustcoefficient)
  {
    celThrusterData::tag = csString (tag);
    celThrusterData::thruster = thruster;
    celThrusterData::thrustcoefficient = thrustcoefficient;
  };

  ~celThrusterData ()
  {
  };
};

/**
 * Factory for the thruster group.
 */
CEL_DECLARE_FACTORY (MechanicsBalancedGroup)

/**
 * This is a property class containing a group of thrusters that work together.
 */
class celPcMechanicsBalancedGroup : public scfImplementationExt1<
	celPcMechanicsBalancedGroup, celPcCommon, iPcMechanicsBalancedGroup>
{
private:
  // Actions
  enum actionids
  {
    action_addthruster = 0,
    action_settype
  };

  static PropertyHolder propinfo;

  // Parameters for action_addthruster
  static csStringID param_thruster;
  static csStringID param_multiplier;

  // Parameters for action_settype
  static csStringID param_type;

  // Group types
  static csStringID type_rotation;
  static csStringID type_translation;

  // Data members
  celAxisType grouptype;
  csArray<celThrusterData*> thrusters;

public:
  celPcMechanicsBalancedGroup (iObjectRegistry* object_reg);
  virtual ~celPcMechanicsBalancedGroup ();

  virtual const char* GetName () const { return "pcmechbalancedgroup"; }
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

  // iPcMechanicsBalancedGroup function implementation
  virtual void SetType (celAxisType type)
  {
    grouptype = type;
  };

  virtual celAxisType GetType ()
  {
    return grouptype;
  };

  virtual void AddThruster (iPcMechanicsThruster* thruster, float multiplier);
  virtual void RemoveThruster (const char* thrustertag);
  virtual iPcMechanicsThruster* GetThruster (const char* thrustertag);

  virtual float AvailableThrust ();
  virtual float AvailableThrustForce ();
  virtual void ChangeThrust (float deltathrust);
};


struct celAxisData
{
public:
  csString name;
  csVector3 axis;
  csRefArray<iPcMechanicsBalancedGroup> balancedgroups;
  celAxisType type;

  celAxisData (const char* name, const csVector3& axis, celAxisType type)
  {
    celAxisData::name = csString (name);
    celAxisData::axis = axis;
    celAxisData::type = type;
  }
  ~celAxisData ()
  {
  };
};

struct celThrustRequestData
{
public:
  csRef<iPcMechanicsBalancedGroup> group;
  uint32 id;
  float thrust;

  celThrustRequestData (iPcMechanicsBalancedGroup* group, uint32 id,
	float thrust)
  {
    celThrustRequestData::group = group;
    celThrustRequestData::id = id;
    celThrustRequestData::thrust = thrust;
  };
};

/**
 * Factory for the thruster controller.
 */
CEL_DECLARE_FACTORY (MechanicsThrusterController)

/**
 * This is a thruster controller property class.
 */
class celPcMechanicsThrusterController : public scfImplementationExt1<
	celPcMechanicsThrusterController, celPcCommon,
	iPcMechanicsThrusterController>
{
private:
  // Actions
  enum actionids
  {
    action_addaxis = 0,
    action_applythrust,
    action_addbalancedgroup,
    action_inittc
  };

  static PropertyHolder propinfo;

  // Parameters for action_inittc
  static csStringID param_object;

  // Parameters for action_addaxis
  static csStringID param_axisname;
  static csStringID param_axistype;
  static csStringID param_axisdir;

  // Parameters for action_applythrust
  static csStringID param_thrust;
  //param_axisname shared

  // Parameters for action_addbalancedgroup
  static csStringID param_balancedgroup;
  //param_axisname shared

  // Axis types
  static csStringID type_rotation;
  static csStringID type_translation;

  // Data members
  csWeakRef<iPcMechanicsObject> mechobject;
  csArray<celAxisData*> axes;
  csArray<celThrustRequestData*> requests;
  uint32 lastrequestid;

  virtual void ApplyThrustHelper (float thrust,
  	iPcMechanicsBalancedGroup* group, uint32 id);

public:
  celPcMechanicsThrusterController (iObjectRegistry* object_reg);
  virtual ~celPcMechanicsThrusterController ();

  virtual const char* GetName () const { return "pcmechthrustercontroller"; }
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

  // iPcMechanicsBalancedGroup function implementation
  virtual void SetMechanicsObject (iPcMechanicsObject* mechobj);
  virtual iPcMechanicsObject* GetMechanicsObject ();
  virtual void AddAxis (const char* name, celAxisType type, const
	csVector3 axis);
  virtual const csVector3 GetAxis (const char* name);
  virtual const celAxisType GetAxisType (const char* name);
  virtual const float GetAxisVelocity (const char* name);
  virtual float GetAxisMaxForce (const char* name);
  virtual float GetAxisMaxThrust (const char* name);
  virtual void AddBalancedGroup (iPcMechanicsBalancedGroup* group,
	const char* axisname);
  virtual void RemoveBalancedGroup (const char* grouptag, const char*
	axisname);
  virtual void ApplyThrust (float thrust, const char* axisname, uint32& id);
  virtual void CancelThrust (uint32 id);
};

#endif // __CEL_PF_MECHANICS_THRUSTER_CONTROLLER_FACTORY__

