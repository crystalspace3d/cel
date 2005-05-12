#include "cssysdef.h"
#include "iutil/objreg.h"
#include "iutil/string.h"
#include "csutil/debug.h"
#include "csgeom/math3d.h"

#include "plugins/propclass/mechanics/common.h"
#include "plugins/propclass/mechanics/thruster_controller.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (MechanicsThrusterGroup, "pcmechthrustergroup")

//---------------------------------------------------------------------------

// Actions
csStringID celPcMechanicsThrusterGroup::action_addthruster = csInvalidStringID;
csStringID celPcMechanicsThrusterGroup::action_settype = csInvalidStringID;

// Parameters for action_addthruster
csStringID celPcMechanicsThrusterGroup::param_thruster = csInvalidStringID;
csStringID celPcMechanicsThrusterGroup::param_multiplier = csInvalidStringID;

// Parameters for action_settype
csStringID celPcMechanicsThrusterGroup::param_type = csInvalidStringID;

// Group types
csStringID celPcMechanicsThrusterGroup::type_rotation = csInvalidStringID;
csStringID celPcMechanicsThrusterGroup::type_translation = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcMechanicsThrusterGroup)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMechanicsThrusterGroup)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsThrusterGroup::PcMechanicsThrusterGroup)
  SCF_IMPLEMENTS_INTERFACE (iPcMechanicsThrusterGroup)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMechanicsThrusterGroup::celPcMechanicsThrusterGroup (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMechanicsThrusterGroup);

  // Actions
  if (action_addthruster == csInvalidStringID)
    action_addthruster = pl->FetchStringID ("cel.action.AddThruster");
  if (action_settype == csInvalidStringID)
    action_settype = pl->FetchStringID ("cel.action.SetType");

  // Parameters for action_addthruster
  if (param_thruster == csInvalidStringID)
    param_thruster = pl->FetchStringID ("cel.parameter.thrusterpctag");
  if (param_multiplier == csInvalidStringID)
    param_multiplier = pl->FetchStringID ("cel.parameter.multiplier");

  // Parameters for action_settype
  if (param_type == csInvalidStringID)
    param_type = pl->FetchStringID ("cel.parameter.type");

  // Group types
  if (type_rotation == csInvalidStringID)
    type_rotation = pl->FetchStringID ("rotation");
  if (type_translation == csInvalidStringID)
    type_translation = pl->FetchStringID ("translation");
}

celPcMechanicsThrusterGroup::~celPcMechanicsThrusterGroup ()
{
}

#define THRUSTERGROUP_SERIAL 1

csPtr<iCelDataBuffer> celPcMechanicsThrusterGroup::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (THRUSTERGROUP_SERIAL);
  databuf->Add ((int32) grouptype);
  databuf->Add (strength);
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

bool celPcMechanicsThrusterGroup::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != THRUSTERGROUP_SERIAL) return false;
  grouptype = (celAxisType) databuf->GetInt32 ();
  strength = databuf->GetFloat ();
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

bool celPcMechanicsThrusterGroup::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_addthruster)
  {
    CEL_FETCH_STRING_PAR (thruster,params,param_thruster);
    if (!p_thruster)
    {
      Report (object_reg, "Couldn't get multiplier for thruster!");
      return false;
    }
    CEL_FETCH_FLOAT_PAR (mult,params,param_multiplier);
    if (!p_mult)
    {
      Report (object_reg, "Couldn't get multiplier for thruster!");
      mult = 1.0f;
    }
    csRef<iPcMechanicsThruster> th = CEL_QUERY_PROPCLASS_TAG_ENT
	(GetEntity (), iPcMechanicsThruster, thruster);
    if (!th)
    {
      csString msg = "Couldn't find thruster with given tag: ";
      msg += thruster;
      Report (object_reg, msg);
      return false;
    }
    AddThruster (th, mult);
    return true;
  } else if (actionId == action_settype) {
    CEL_FETCH_STRING_PAR (type,params,param_type);
    if (!p_type)
    {
      Report (object_reg, "Couldn't get thruster group type!");
      return false;
    }
    csStringID type_id = pl->FetchStringID (type);
    celAxisType gtype;
    if (type_id == type_rotation)
      gtype = CEL_TGT_ROTATION;
    else if (type_id == type_translation)
      gtype = CEL_TGT_TRANSLATION;
    else {
      Report (object_reg, "Invalid thruster group type!");
      return false;
    }
    SetType (gtype);
  }
  return false;
}

void celPcMechanicsThrusterGroup::AddThruster (iPcMechanicsThruster* thruster,
	float multiplier)
{
  csRef<iCelPropertyClass> pc = SCF_QUERY_INTERFACE (thruster, iCelPropertyClass);
  celThrusterData* th = new celThrusterData (pc->GetTag (), thruster, thruster->GetEffectiveMaxThrust
	(), thruster->GetPosition (), multiplier);
  thrusters.Push (th);
}

void celPcMechanicsThrusterGroup::RemoveThruster (const char* thrustertag)
{
  csArray<celThrusterData*>::Iterator it = thrusters.GetIterator ();
  while (it.HasNext ()) {
    celThrusterData* td = it.Next ();
    if (strcmp (td->tag.GetData (), thrustertag) == 0)
      thrusters.Delete (td);
  }
}

iPcMechanicsThruster* celPcMechanicsThrusterGroup::GetThruster (const char*
	thrustertag)
{
  csArray<celThrusterData*>::Iterator it = thrusters.GetIterator ();
  while (it.HasNext ()) {
    celThrusterData* td = it.Next ();
    if (strcmp (td->tag.GetData (), thrustertag) == 0)
      return td->thruster;
  }
  return NULL;
}

const csRefArray<iPcMechanicsThruster>
	celPcMechanicsThrusterGroup::GetThrustersInThisDirection
	(const csVector3 axis) const
{
  csRefArray<iPcMechanicsThruster> tarray;
  csArray<celThrusterData*>::Iterator thrustit = thrusters.GetIterator ();
  csRef<iPcMechanicsThruster> thruster;
  csVector3 taxis, axisunit = axis.Unit ();
  while (thrustit.HasNext ())
  {
    printf ("(checking) ");
    fflush (stdout);
    thruster = thrustit.Next ()->thruster;
    if (grouptype == CEL_TGT_ROTATION)
      taxis = (thruster->GetPosition () % thruster->GetOrientation ());
    else
      taxis = thruster->GetOrientation ();
    if ((taxis.Unit () * axisunit) > 0)
      tarray.Push (thruster);
  }
  printf ("\nFound %lu thrusters within 90 degrees of %f,%f,%f.\n", (unsigned long)tarray.GetSize (), axis.x, axis.y, axis.z);
  fflush (stdout);
  return tarray;
}


float celPcMechanicsThrusterGroup::GetStrength (const csVector3 axis)
{
  printf ("Getting strength along axis %f,%f,%f.\n", axis.x,axis.y,axis.z);
  fflush (stdout);
  float strength = 0;
  csRefArray<iPcMechanicsThruster> ta = GetThrustersInThisDirection (axis);
  csRefArray<iPcMechanicsThruster>::Iterator tait = ta.GetIterator ();
  while (tait.HasNext ())
  {
    strength += tait.Next ()->GetEffectiveMaxThrust ();
  }
  return strength;
}

bool celPcMechanicsThrusterGroup::IsAvailable (const csVector3 axis)
{
  printf ("Checking availability along axis %f,%f,%f: ", axis.x,axis.y,axis.z);
  fflush (stdout);
  bool available = true, av = false;
  csRefArray<iPcMechanicsThruster> ta = GetThrustersInThisDirection (axis);
  csRefArray<iPcMechanicsThruster>::Iterator tait = ta.GetIterator ();
  while (tait.HasNext ())
  {
    av = tait.Next()->IsAvailable ();
    available = available && av;
    if (av)
      printf ("thruster is available.\n");
    else
      printf ("thruster is not available.\n");
    fflush (stdout);
  }
  if (available)
    printf ("Available!\n");
  else
    printf ("Not available!\n");
  fflush (stdout);
  return available;
}

void celPcMechanicsThrusterGroup::ApplyThrust (percentage thrust, const csVector3 axis)
{
  csRef<iPcMechanicsThruster> thruster;
  csRefArray<iPcMechanicsThruster> ta = GetThrustersInThisDirection (axis);
  csRefArray<iPcMechanicsThruster>::Iterator tait = ta.GetIterator ();
  while (tait.HasNext ())
  {
    printf (" [thruster]\n");
    fflush (stdout);
    thruster = tait.Next ();
    thruster->ThrustRequest ((iPcMechanicsThrusterGroup*)
	&scfiPcMechanicsThrusterGroup, thrust);
  }
}

void celPcMechanicsThrusterGroup::UpdateThrust (const char* thrustertag, float thrust)
{
  //Update the effective thrust of this thruster.
  csArray<celThrusterData*>::Iterator it = thrusters.GetIterator ();
  while (it.HasNext ()) {
    celThrusterData* td = it.Next ();
    if (strcmp (td->tag.GetData (), thrustertag) == 0)
      td->thrust = thrust;
  }
}

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (MechanicsThrusterController, "pcmechthrustercontroller")

//---------------------------------------------------------------------------

// Actions
csStringID celPcMechanicsThrusterController::action_addaxis = csInvalidStringID;
csStringID celPcMechanicsThrusterController::action_applythrust = csInvalidStringID;
csStringID celPcMechanicsThrusterController::action_addthrustergroup = csInvalidStringID;

// Parameters for action_addaxis
csStringID celPcMechanicsThrusterController::param_axisname = csInvalidStringID;
csStringID celPcMechanicsThrusterController::param_axistype = csInvalidStringID;
csStringID celPcMechanicsThrusterController::param_axisdir = csInvalidStringID;

// Parameters for action_applythrust
csStringID celPcMechanicsThrusterController::param_thrust = csInvalidStringID;
//param_axisname shared

// Parameters for action_addthrustergroup
csStringID celPcMechanicsThrusterController::param_thrustergroup = csInvalidStringID;
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

celPcMechanicsThrusterController::celPcMechanicsThrusterController (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMechanicsThrusterController);

  // Actions
  if (action_addaxis == csInvalidStringID)
    action_addaxis = pl->FetchStringID ("cel.action.AddAxis");
  if (action_applythrust == csInvalidStringID)
    action_applythrust = pl->FetchStringID ("cel.action.ApplyThrust");
  if (action_addthrustergroup == csInvalidStringID)
    action_addthrustergroup = pl->FetchStringID ("cel.action.AddThrusterGroup");

  // Parameters for action_addaxis
  if (param_axisname == csInvalidStringID)
    param_axisname = pl->FetchStringID ("cel.parameter.axisname");
  if (param_axistype == csInvalidStringID)
    param_axistype = pl->FetchStringID ("cel.parameter.axistype");
  if (param_axisdir == csInvalidStringID)
    param_axisdir = pl->FetchStringID ("cel.parameter.axisdir");

  // Parameters for action_applythrust
  if (param_thrust == csInvalidStringID)
    param_thrust = pl->FetchStringID ("cel.parameter.thrust");
  //param_axisname shared

  // Parameters for action_addthrustergroup
  if (param_thrustergroup == csInvalidStringID)
    param_thrustergroup = pl->FetchStringID ("cel.parameter.thrustergroup");
  //param_axisname shared

  // Axis types
  if (type_rotation == csInvalidStringID)
    type_rotation = pl->FetchStringID ("rotation");
  if (type_translation == csInvalidStringID)
    type_translation = pl->FetchStringID ("translation");
}

celPcMechanicsThrusterController::~celPcMechanicsThrusterController ()
{
}

#define THRUSTERCONTROLLER_SERIAL 1

csPtr<iCelDataBuffer> celPcMechanicsThrusterController::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (THRUSTERCONTROLLER_SERIAL);
  csRef<iCelPropertyClass> pc = SCF_QUERY_INTERFACE (mechobject, iCelPropertyClass);
  databuf->Add (pc);
  databuf->Add ((int32) axes.GetSize ());
  csArray<celAxisData*>::Iterator axisit = axes.GetIterator ();
  celAxisData* ad;
  csRef<iPcMechanicsThrusterGroup> tg;
  while (axisit.HasNext ())
  {
    ad = axisit.Next ();
    databuf->Add (ad->name.GetData ());
    databuf->Add (ad->axis);
    databuf->Add ((int32) ad->type);
    databuf->Add ((int32) ad->thrustergroups.GetSize ());
    csRefArray<iPcMechanicsThrusterGroup>::Iterator tgit = ad->thrustergroups.GetIterator ();
    while (tgit.HasNext ())
    {
      tg = tgit.Next ();
      pc = SCF_QUERY_INTERFACE (tg, iCelPropertyClass);
      databuf->Add (pc);
    }
  }
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMechanicsThrusterController::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != THRUSTERCONTROLLER_SERIAL) return false;
  csRef<iCelPropertyClass> pc = databuf->GetPC ();
  csRef<iPcMechanicsObject> mechobj = SCF_QUERY_INTERFACE (pc, iPcMechanicsObject);
  int axessize = (int) databuf->GetInt32 ();
  int tgsize, j;
  csRef<iPcMechanicsThrusterGroup> tg;
  for (int i = 1; i <= axessize; i++)
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
      tg = SCF_QUERY_INTERFACE (pc, iPcMechanicsThrusterGroup);
      AddThrusterGroup (tg, name->GetData ());
    }
  }
  return true;
}

bool celPcMechanicsThrusterController::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_addaxis)
  {
    CEL_FETCH_STRING_PAR (axisname,params,param_axisname);
    if (!p_axisname)
    {
      Report (object_reg, "Couldn't get axis name!");
      return false;
    }
    CEL_FETCH_STRING_PAR (axistype,params,param_axistype);
    if (!p_axistype)
    {
      Report (object_reg, "Couldn't get axis type!");
      return false;
    }
    csStringID type_id = pl->FetchStringID (axistype);
    celAxisType atype;
    if (type_id == type_rotation)
      atype = CEL_TGT_ROTATION;
    else if (type_id == type_translation)
      atype = CEL_TGT_TRANSLATION;
    else {
      Report (object_reg, "Invalid axis type!");
      return false;
    }
    CEL_FETCH_VECTOR3_PAR (axisdir,params,param_axisdir);
    if (!p_axisdir)
    {
      Report (object_reg, "Couldn't get axis direction!");
      return false;
    }
    AddAxis (axisname, atype, axisdir);
    return true;
  } else if (actionId == action_applythrust) {
    CEL_FETCH_STRING_PAR (axisname,params,param_axisname);
    if (!p_axisname)
    {
      Report (object_reg, "Couldn't get axis name!");
      return false;
    }
    CEL_FETCH_FLOAT_PAR (thrust,params,param_thrust);
    if (!p_thrust)
    {
      Report (object_reg, "Couldn't get thrust!");
      return false;
    }
    ApplyThrust ((percentage) thrust, axisname);
  } else if (actionId == action_addthrustergroup) {
    CEL_FETCH_STRING_PAR (axisname,params,param_axisname);
    if (!p_axisname)
    {
      Report (object_reg, "Couldn't get axis name!");
      return false;
    }
    CEL_FETCH_STRING_PAR (thrustergrouppctag,params,param_thrustergroup);
    if (!p_thrustergrouppctag)
    {
      Report (object_reg, "Couldn't get thruster group tag!");
      return false;
    }
    csRef<iPcMechanicsThrusterGroup> tg = CEL_QUERY_PROPCLASS_TAG_ENT
	(GetEntity (), iPcMechanicsThrusterGroup, thrustergrouppctag);
    AddThrusterGroup (tg, axisname);
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
  while (it.HasNext ()) {
    ad = it.Next ();
    if (strcmp (ad->name.GetData (), name) == 0)
      break;
  }
  if (ad)
    return ad->axis;
  else
    return csVector3 (0);
}

float celPcMechanicsThrusterController::GetAxisStrength (const char* axisname)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData* ad = 0;
  while (it.HasNext ()) {
    ad = it.Next ();
    if (strcmp (ad->name.GetData (), axisname) == 0) {
      csVector3 axis = ad->axis;
      csRefArray<iPcMechanicsThrusterGroup>::Iterator groupit
	= ad->thrustergroups.GetIterator ();
      csRef<iPcMechanicsThrusterGroup> group;
      float maxstrength = 0;
      while (groupit.HasNext ())
      {
        group = groupit.Next ();
        if (group->IsAvailable (axis)) {
          if (group->GetStrength (axis) > maxstrength)
            maxstrength = group->GetStrength (axis);
        }
      }
      return maxstrength;
    }
  }
  Report (object_reg, (csString ("Invalid axis specified: ") + axisname).GetData ());
  return 0;
}

void celPcMechanicsThrusterController::AddThrusterGroup
	(iPcMechanicsThrusterGroup* thrustergroup, const char* axisname)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData *adi = 0, *ad = 0;
  while (it.HasNext ()) {
    adi = it.Next ();
    if (strcmp (adi->name.GetData (), axisname) == 0) {
      ad = adi;
      break;
    }
  }
  if (ad)
    ad->thrustergroups.Push (thrustergroup);
  else
    Report (object_reg, (csString ("Couldn't find specified axis: ") + axisname).GetData ());
}

void celPcMechanicsThrusterController::RemoveThrusterGroup (const char*
	thrustergrouptag, const char* axisname)
{
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData* ad = 0;
  while (it.HasNext ()) {
    ad = it.Next ();
    if (strcmp (ad->name.GetData (), axisname) == 0) {
      csRefArray<iPcMechanicsThrusterGroup>::Iterator groupit
	= ad->thrustergroups.GetIterator ();
      csRef<iPcMechanicsThrusterGroup> group;
      csRef<iCelPropertyClass> pc;
      while (groupit.HasNext ())
      {
        group = groupit.Next ();
	pc = SCF_QUERY_INTERFACE (group, iCelPropertyClass);
        if (!strcmp (pc->GetTag (), thrustergrouptag))
          ad->thrustergroups.Delete (group);
      }
    }
  }
}

void celPcMechanicsThrusterController::ApplyThrust (percentage thrust, const char* axisname)
{
  printf ((csString ("Applying thrust on axis '") + axisname + "'.\n").GetData ());
  fflush (stdout);
  csArray<celAxisData*>::Iterator it = axes.GetIterator ();
  celAxisData* ad = 0;
  while (it.HasNext ()) {
    ad = it.Next ();
    printf ("Iterating axis: ");
    printf ((ad->name + "\n").GetData ());
    fflush (stdout);
    if (strcmp (ad->name.GetData (), axisname) == 0) {
      printf ("Found match.\n");
      fflush (stdout);
      csVector3 axis;
      if (thrust < 0)
        axis = -ad->axis;
      else
        axis = ad->axis;
      csRefArray<iPcMechanicsThrusterGroup>::Iterator groupit
	= ad->thrustergroups.GetIterator ();
      csRef<iPcMechanicsThrusterGroup> group;
      csRef<iPcMechanicsThrusterGroup> bestgroup = NULL;
      float beststrength = 0;
      while (groupit.HasNext ())
      {
        printf ("[group]\n");
	fflush (stdout);
        group = groupit.Next ();
        if (group->IsAvailable (axis)) {
          if (bestgroup == NULL || group->GetStrength (axis) > beststrength) {
	    beststrength = group->GetStrength (axis);
            bestgroup = group;
	  }
        }
      }
      if (bestgroup != NULL)
      {
        printf ("Found best group. Applying thrust %f.\n", thrust);
	fflush (stdout);
        bestgroup->ApplyThrust (fabs (thrust), axis);
      }
/*      else
      {
        Report (object_reg, (csString ("No available groups in axis '") + axisname + "'!").GetData ());
      }*/
      return;
    }
  }
  Report (object_reg, (csString ("Invalid axis specified: ") + axisname).GetData ());
}

//---------------------------------------------------------------------------

