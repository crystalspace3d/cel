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
  float thrust;
  csVector3 position;
  csWeakRef<iPcMechanicsThruster> thruster;
  percentage thrustcoefficient;
  csString tag;

  celThrusterData (const char* tag, iPcMechanicsThruster* thruster, float
	thrust, const csVector3& position, percentage thrustcoefficient)
  {
    celThrusterData::tag = csString (tag);
    celThrusterData::thruster = thruster;
    celThrusterData::thrust = thrust;
    celThrusterData::position = position;
    celThrusterData::thrustcoefficient = thrustcoefficient;
  };

  ~celThrusterData ()
  {
  };
};

/**
 * Factory for the thruster group.
 */
CEL_DECLARE_FACTORY (MechanicsThrusterGroup)

/**
 * This is a property class containing a group of thrusters that work together.
 */
class celPcMechanicsThrusterGroup : public celPcCommon
{
private:
  // Actions
  static csStringID action_addthruster;
  static csStringID action_settype;

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
  float strength;

  const csRefArray<iPcMechanicsThruster> GetThrustersInThisDirection (const csVector3 axis) const;

public:
  celPcMechanicsThrusterGroup (iObjectRegistry* object_reg);
  virtual ~celPcMechanicsThrusterGroup ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmechthrustergroup"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  virtual void SetType (celAxisType type) {
    grouptype = type;
  };

  virtual celAxisType GetType () {
    return grouptype;
  };

  virtual void AddThruster (iPcMechanicsThruster* thruster, float multiplier);
  virtual void RemoveThruster (const char* thrustertag);
  virtual iPcMechanicsThruster* GetThruster (const char* thrustertag);

  virtual float GetStrength (const csVector3 axis);
  virtual bool IsAvailable (const csVector3 axis);
  virtual void ApplyThrust (percentage thrust, const csVector3 axis);
  virtual void UpdateThrust (const char* thrustertag, float thrust);

  struct PcMechanicsThrusterGroup : public iPcMechanicsThrusterGroup
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMechanicsThrusterGroup);
    virtual void SetType (celAxisType type)
    {
      scfParent->SetType (type);
    }
    virtual celAxisType GetType ()
    {
      return scfParent->GetType ();
    }
    virtual void AddThruster (iPcMechanicsThruster* thruster, float multiplier)
    {
      scfParent->AddThruster (thruster, multiplier);
    }
    virtual void RemoveThruster (const char* thrustertag)
    {
      scfParent->RemoveThruster (thrustertag);
    }
    virtual iPcMechanicsThruster* GetThruster (const char* thrustertag)
    {
      return scfParent->GetThruster (thrustertag);
    }
    virtual float GetStrength (const csVector3 axis)
    {
      return scfParent->GetStrength (axis);
    }
    virtual bool IsAvailable (const csVector3 axis)
    {
      return scfParent->IsAvailable (axis);
    }
    virtual void ApplyThrust (percentage thrust, const csVector3 axis)
    {
      scfParent->ApplyThrust (thrust, axis);
    }
    virtual void UpdateThrust (const char* thrustertag, float thrust)
    {
      scfParent->UpdateThrust (thrustertag, thrust);
    }
  } scfiPcMechanicsThrusterGroup;
};


struct celAxisData
{
public:
  csString name;
  csVector3 axis;
  csRefArray<iPcMechanicsThrusterGroup> thrustergroups;
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

/**
 * Factory for the thruster controller.
 */
CEL_DECLARE_FACTORY (MechanicsThrusterController)

/**
 * This is a thruster controller property class.
 */
class celPcMechanicsThrusterController : public celPcCommon
{
private:
  // Actions
  static csStringID action_addaxis;
  static csStringID action_applythrust;
  static csStringID action_addthrustergroup;

  // Parameters for action_addaxis
  static csStringID param_axisname;
  static csStringID param_axistype;
  static csStringID param_axisdir;

  // Parameters for action_applythrust
  static csStringID param_thrust;
  //param_axisname shared

  // Parameters for action_addthrustergroup
  static csStringID param_thrustergroup;
  //param_axisname shared

  // Axis types
  static csStringID type_rotation;
  static csStringID type_translation;

  // Data members
  csWeakRef<iPcMechanicsObject> mechobject;
  csArray<celAxisData*> axes;

public:
  celPcMechanicsThrusterController (iObjectRegistry* object_reg);
  virtual ~celPcMechanicsThrusterController ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmechthrustercontroller"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  virtual void SetMechanicsObject (iPcMechanicsObject* mechobj);
  virtual iPcMechanicsObject* GetMechanicsObject ();
  virtual void AddAxis (const char* name, celAxisType type, const
	csVector3 axis);
  virtual const csVector3 GetAxis (const char* name);
  virtual float GetAxisStrength (const char* name);
  virtual void AddThrusterGroup (iPcMechanicsThrusterGroup* thrustergroup,
	const char* axisname);
  virtual void RemoveThrusterGroup (const char* thrustergrouptag, const char*
	axisname);
  virtual void ApplyThrust (percentage thrust, const char* axisname);

  struct PcMechanicsThrusterController : public iPcMechanicsThrusterController
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMechanicsThrusterController);
    virtual void SetMechanicsObject (iPcMechanicsObject* mechobj)
    {
      scfParent->SetMechanicsObject (mechobj);
    }
    virtual iPcMechanicsObject* GetMechanicsObject ()
    {
      return scfParent->GetMechanicsObject ();
    }
    virtual void AddAxis (const char* name, celAxisType type, const
	csVector3 axis)
    {
      scfParent->AddAxis (name, type, axis);
    }
    virtual const csVector3 GetAxis (const char* name)
    {
      return scfParent->GetAxis (name);
    }
    virtual float GetAxisStrength (const char* name)
    {
      return scfParent->GetAxisStrength (name);
    }
    virtual void AddThrusterGroup (iPcMechanicsThrusterGroup* thrustergroup,
	const char* axisname)
    {
      scfParent->AddThrusterGroup (thrustergroup, axisname);
    }
    virtual void RemoveThrusterGroup (const char* thrustergrouptag,
	const char* axisname)
    {
      scfParent->RemoveThrusterGroup (thrustergrouptag, axisname);
    }
    virtual void ApplyThrust (percentage thrust, const char* axisname)
    {
      scfParent->ApplyThrust (thrust, axisname);
    }
  } scfiPcMechanicsThrusterController;
};

#endif // __CEL_PF_MECHANICS_THRUSTER_CONTROLLER_FACTORY__

