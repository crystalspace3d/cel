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
#include "csqsqrt.h"
#include "iutil/plugin.h"
#include "iutil/virtclk.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/object.h"
#include "cstool/initapp.h"
#include "csutil/objreg.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "csgeom/quaternion.h"
#include "iengine/mesh.h"
#include "iengine/light.h"
#include "iengine/camera.h"
#include "iengine/movable.h"
#include "imesh/objmodel.h"
#include "imesh/object.h"
#include "ivaria/reporter.h"
#include "ivaria/dynamics.h"
#include "ivaria/ode.h"

#include "plugins/propclass/mechanics/common.h"
#include "plugins/propclass/mechanics/mechanics.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY_ALT (MechanicsSystem, "pcphysics.system", "pcmechsys")
CEL_IMPLEMENT_FACTORY_ALT (MechanicsObject, "pcphysics.object", "pcmechobject")
CEL_IMPLEMENT_FACTORY_ALT (MechanicsJoint, "pcphysics.joint", "pcmechjoint")

//---------------------------------------------------------------------------

// Parameters.
csStringID celPcMechanicsSystem::param_dynsys = csInvalidStringID;
csStringID celPcMechanicsSystem::param_gravity = csInvalidStringID;
csStringID celPcMechanicsSystem::param_time = csInvalidStringID;
csStringID celPcMechanicsSystem::param_simulationspeed = csInvalidStringID;
PropertyHolder celPcMechanicsSystem::propinfo;

celPcMechanicsSystem::celPcMechanicsSystem (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  scfiCelTimerListener = new CelTimerListener (this);
  pl->CallbackEveryFrame (scfiCelTimerListener, CEL_EVENT_PRE);

  dynsystem_error_reported = false;
  delta = 0.01f;
  remaining_delta = 0;
  simulationspeed=1.0f;

  if (param_dynsys == csInvalidStringID)
  {
    // Parameters.
    param_dynsys = pl->FetchStringID ("cel.parameter.dynsys");
    param_gravity = pl->FetchStringID ("cel.parameter.gravity");
    param_time = pl->FetchStringID ("cel.parameter.time");
    param_simulationspeed = pl->FetchStringID ("cel.parameter.simulationspeed");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_setsystem, "cel.action.SetSystem");
    AddAction (action_setgravity, "cel.action.SetGravity");
    AddAction (action_quickstep, "cel.action.QuickStep");
    AddAction (action_enablestepfast, "cel.action.EnableStepFast");
    AddAction (action_disablestepfast, "cel.action.DisableStepFast");
    AddAction (action_setsteptime, "cel.action.SetStepTime");
    AddAction (action_setsimulationspeed, "cel.action.SetSimulationSpeed");
  }
}

void celPcMechanicsSystem::SetEntity (iCelEntity* entity)
{
  // if we're already set to some entity, unregister.
  if (celPcCommon::entity == entity)
    object_reg->Unregister ((iPcMechanicsSystem*)this, "iPcMechanicsSystem");
  // now call underlying SetEntity.
  celPcCommon::SetEntity(entity);
  // if we're getting set to a new entity, register.
  if (entity)
    object_reg->Register ((iPcMechanicsSystem*)this, "iPcMechanicsSystem");
}

celPcMechanicsSystem::~celPcMechanicsSystem ()
{
  scfiCelTimerListener->DecRef ();
}

void celPcMechanicsSystem::ProcessForces (float dt)
{
// @@@ Note: the code below should really factor in the
// fact that 'dt' may be smaller than delta and also that the
// remaining time for a force may be smaller than the 'dt' value
// given here.
  size_t i;
  for (i = 0 ; i < forces.GetSize () ; i++)
  {
    celForce& f = forces[i];
    if (f.forceid != 0 || f.frame)
    {
      ApplyForce (f);
    }
    else if (f.seconds > 0)
    {
      ApplyForce (f);
      f.seconds -= dt;
      if (f.seconds <= 0)
      {
        forces.DeleteIndex (i);
	i--;
      }
    }
  }
}

void celPcMechanicsSystem::ApplyForce (celForce& f)
{
  if (f.relative)
  {
    if (f.position.IsZero ())
      f.body->GetBody ()->AddRelForce (f.force);
    else
      f.body->GetBody ()->AddRelForceAtRelPos (f.force, f.position);
  }
  else
  {
    if (f.position.IsZero ())
      f.body->GetBody ()->AddForce (f.force);
    else
      f.body->GetBody ()->AddForceAtPos (f.force, f.position);
  }
}

iDynamics* celPcMechanicsSystem::GetDynamics ()
{
  dynamics = csQueryRegistryOrLoad<iDynamics> (object_reg,
  	"crystalspace.dynamics.ode");
  if (!dynamics)
  {
    if (!dynsystem_error_reported)
    {
      dynsystem_error_reported = true;
      CS_REPORT(ERROR,"Can't find dynamic subsystem!");
    }
    return 0;
  }
  return dynamics;
}

void celPcMechanicsSystem::TickEveryFrame ()
{
  GetDynamicSystem ();
  if (!dynamics) return;
  csTicks elapsed_time = vc->GetElapsedTicks ();

  // For ODE it is recommended that all steps are done with the
  // same size. So we always will call dynamics->Step(delta) with
  // the constant delta. However, sometimes our elapsed time
  // is not divisible by delta and in that case we have a small
  // time (smaller then delta) left-over. We can't afford to drop
  // that because then speed of physics simulation would differ
  // depending on framerate. So we will put that remainder in
  // remaining_delta and use that here too.
  float delta_modulated=delta*simulationspeed;
  float et = remaining_delta + (float (elapsed_time) / (1000.0/simulationspeed));
  while (et >= delta_modulated)
  {
    ProcessForces (delta_modulated);
    dynamics->Step (delta_modulated);
    et -= delta_modulated;
  }

  // Now we have a small remainder. We remember that in remaining_delta.
  remaining_delta = et;

  // Delete all expired forces and forces that were only
  // meant to be here for one frame.
  size_t i;
  for (i = 0 ; i < forces.GetSize () ; i++)
  {
    celForce& f = forces[i];
    if (f.forceid == 0 && (f.frame || f.seconds <= 0))
    {
      forces.DeleteIndex (i);
      i--;
    }
  }
}

void celPcMechanicsSystem::SetDynamicSystem (const char* dynsysname)
{
  GetDynamics();
  dynsystem = dynamics->FindSystem(dynsysname);
  if (!dynsystem)
  {
    dynsystem = dynamics->CreateSystem ();
    dynsystem->QueryObject ()->SetName (dynsysname);
    dynsystem->SetGravity (csVector3 (0, -9.8f, 0));
  }
}

void celPcMechanicsSystem::SetDynamicSystem (iDynamicSystem* dynsys)
{
  dynsystem = dynsys;
  dynsystem->SetGravity (csVector3 (0, -9.8f, 0));
  GetDynamics ();
}

iDynamicSystem* celPcMechanicsSystem::GetDynamicSystem ()
{
  if (!dynsystem)
  {
    GetDynamics();
    if (!dynamics)
    {
      return 0;
    }
    dynsystem = dynamics->CreateSystem ();
    dynsystem->QueryObject ()->SetName ("cel.mechanics.system");
    dynsystem->SetGravity (csVector3 (0, -9.8f, 0));

    EnableStepFast ();
  }
  return dynsystem;
}

void celPcMechanicsSystem::DisableStepFast ()
{
  GetDynamicSystem ();
  if (!dynsystem) return;
  csRef<iODEDynamicSystemState> osys= scfQueryInterface<iODEDynamicSystemState> (dynsystem);
  if (osys)
    osys->EnableStepFast (0);
}

void celPcMechanicsSystem::EnableStepFast ()
{
  GetDynamicSystem ();
  if (!dynsystem) return;
  csRef<iODEDynamicSystemState> osys= scfQueryInterface<iODEDynamicSystemState> (dynsystem);
  if (osys)
    osys->EnableStepFast (1);
}

void celPcMechanicsSystem::EnableQuickStep ()
{
  GetDynamicSystem ();
  if (!dynsystem) return;
  csRef<iODEDynamicSystemState> osys= scfQueryInterface<iODEDynamicSystemState> (dynsystem);
  if (osys)
    osys->EnableQuickStep (1);
}


void celPcMechanicsSystem::SetGravity (const csVector3& grav)
{
  GetDynamicSystem ();
  if (!dynsystem) return;
  dynsystem->SetGravity (grav);
}

const csVector3 celPcMechanicsSystem::GetGravity ()
{
  GetDynamicSystem ();
  if (!dynsystem) return csVector3 (0);
  return dynsystem->GetGravity ();
}

iRigidBody* celPcMechanicsSystem::FindBody (const char* entityname)
{
  iCelEntity* ent = pl->FindEntity (entityname);
  if (!ent) return 0;
  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (
  	ent, iPcMechanicsObject);
  if (!pcmechobj) return 0;
  return pcmechobj->GetBody ();
}

bool celPcMechanicsSystem::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setsystem:
      {
        CEL_FETCH_STRING_PAR (dynsys,params,param_dynsys);
        if (p_dynsys)
        {
          SetDynamicSystem(dynsys);
        }
        else
        {
          CS_REPORT(ERROR,"Couldn't get dynamics system name!");
          return false;
        }
        return true;
      }
    case action_quickstep:
      EnableQuickStep ();
      return true;
    case action_enablestepfast:
      EnableStepFast ();
      return true;
    case action_disablestepfast:
      DisableStepFast ();
      return true;
    case action_setsteptime:
      {
        CEL_FETCH_FLOAT_PAR (time,params,param_time);
        if (!p_time)
        {
          CS_REPORT(ERROR,"Couldn't get 'time' parameter for SetStepTime!");
          return false;
        }
        SetStepTime (time);
	return true;
      }
    case action_setsimulationspeed:
      {
        CEL_FETCH_FLOAT_PAR (simulationspeed,params,param_simulationspeed);
        if (!p_simulationspeed)
        {
          CS_REPORT(ERROR,"Couldn't get 'simulationspeed' parameter for SetSimulationSpeed!");
          return false;
        }
        SetSimulationSpeed (simulationspeed);
	return true;
      }
    case action_setgravity:
      {
        CEL_FETCH_VECTOR3_PAR (gravity,params,param_gravity);
        if (p_gravity)
        {
          GetDynamicSystem();
          if (dynsystem)
             dynsystem->SetGravity (gravity);
        }
        else
        {
          CS_REPORT(ERROR,"Couldn't get gravity!");
          return false;
        }
	return true;
      }
    default:
      return false;
  }
}

#define MECHSYS_SERIAL 1

csPtr<iCelDataBuffer> celPcMechanicsSystem::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MECHSYS_SERIAL);
  int j = 0;
  databuf->GetData (j++)->Set (dynsystem->GetGravity ());
  databuf->GetData (j++)->Set (dynsystem->GetLinearDampener ());
  databuf->GetData (j++)->Set (dynsystem->GetRollingDampener ());
  databuf->GetData (j++)->Set (delta);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMechanicsSystem::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MECHSYS_SERIAL)
  {
    CS_REPORT(ERROR,"serialnr != MECHSYS_SERIAL.  Cannot load.");
    return false;
  }

  if (databuf->GetDataCount () != 4)
  {
    CS_REPORT(ERROR,"Msg does not specify the correct data. Cannot load.");
    return false;
  }

  csVector3 gravity;
  float lineardampener;
  float rollingdampener;
  databuf->GetVector3 (gravity);
  lineardampener = databuf->GetFloat ();
  rollingdampener = databuf->GetFloat ();
  delta = databuf->GetFloat ();

  if (!GetDynamicSystem ())
  {
    return false;
  }

  dynsystem->SetGravity (gravity);
  dynsystem->SetLinearDampener (lineardampener);
  dynsystem->SetRollingDampener (rollingdampener);

  return true;
}

void celPcMechanicsSystem::AddForceTagged (iPcMechanicsObject* pcobject,
	const csVector3& force, bool relative, const csVector3& position,
	uint32 forceid)
{
  celForce f;
  f.body = pcobject;
  f.seconds = 0;
  f.frame = false;
  f.forceid = forceid;
  f.force = force;
  f.relative = relative;
  f.position = position;
  forces.Push (f);
}

void celPcMechanicsSystem::RemoveForceTagged (iPcMechanicsObject* pcobject,
	uint32 forceid)
{
  for (size_t i = 0 ; i < forces.GetSize () ; i++)
  {
    celForce& f = forces[i];
    if (f.forceid == forceid)
    {
      forces.DeleteIndex (i);
      return;
    }
  }
}

void celPcMechanicsSystem::AddForceDuration (iPcMechanicsObject* body,
	const csVector3& force, bool relative, const csVector3& position,
	float seconds)
{
  celForce f;
  f.body = body;
  f.seconds = seconds;
  f.frame = false;
  f.forceid = 0;
  f.force = force;
  f.relative = relative;
  f.position = position;
  forces.Push (f);
}

void celPcMechanicsSystem::AddForceFrame (iPcMechanicsObject* body,
	const csVector3& force, bool relative, const csVector3& position)
{
  celForce f;
  f.body = body;
  f.seconds = 0;
  f.frame = true;
  f.forceid = 0;
  f.force = force;
  f.relative = relative;
  f.position = position;
  forces.Push (f);
}

void celPcMechanicsSystem::ClearForces (iPcMechanicsObject* body)
{
  size_t i;
  for (i = 0 ; i < forces.GetSize () ; i++)
  {
    if (forces[i].body == body)
    {
      forces.DeleteIndex (i);
      i--;
    }
  }
}

void celPcMechanicsSystem::ClearAllForces ()
{
  forces.DeleteAll ();
}

csPtr<iRigidBody> celPcMechanicsSystem::CreateBody ()
{
  return dynsystem->CreateBody ();
}

void celPcMechanicsSystem::RemoveBody (iRigidBody* body)
{
  dynsystem->RemoveBody (body);
}

void celPcMechanicsSystem::AddBodyToGroup (iRigidBody* body, const char* group)
{
  csRef<iBodyGroup> grp;
  iBodyGroup** grpptr = groups.GetElementPointer (group);
  if (grpptr != 0)
  {
    grp = *grpptr;
  }
  else
  {
    grp = dynsystem->CreateGroup ();
    groups.PutUnique (group, grp);
  }
  grp->AddBody (body);
}

void celPcMechanicsSystem::RemoveBodyFromGroup (iRigidBody* body,
	const char* group)
{
  iBodyGroup** grpptr = groups.GetElementPointer (group);
  if (grpptr != 0 && (*grpptr)->BodyInGroup (body))
  {
    (*grpptr)->RemoveBody (body);
  }
}

iJoint* celPcMechanicsSystem::CreateJoint (iRigidBody* body1, iRigidBody* body2)
{
  csRef<iJoint> jnt = dynsystem->CreateJoint ();
  jnt->Attach (body1, body2);
  return jnt;
}

void celPcMechanicsSystem::RemoveJoint (iJoint* joint)
{
  dynsystem->RemoveJoint (joint);
}


//---------------------------------------------------------------------------

// Parameters.
csStringID celPcMechanicsObject::param_friction = csInvalidStringID;
csStringID celPcMechanicsObject::param_mass = csInvalidStringID;
csStringID celPcMechanicsObject::param_elasticity = csInvalidStringID;
csStringID celPcMechanicsObject::param_density = csInvalidStringID;
csStringID celPcMechanicsObject::param_softness = csInvalidStringID;
csStringID celPcMechanicsObject::param_lift = csInvalidStringID;
csStringID celPcMechanicsObject::param_drag = csInvalidStringID;
csStringID celPcMechanicsObject::param_static = csInvalidStringID;
csStringID celPcMechanicsObject::param_systempcent = csInvalidStringID;
csStringID celPcMechanicsObject::param_systempctag = csInvalidStringID;
csStringID celPcMechanicsObject::param_meshpctag = csInvalidStringID;
csStringID celPcMechanicsObject::param_radius = csInvalidStringID;
csStringID celPcMechanicsObject::param_radiusadjustment = csInvalidStringID;
csStringID celPcMechanicsObject::param_offset = csInvalidStringID;
csStringID celPcMechanicsObject::param_length = csInvalidStringID;
csStringID celPcMechanicsObject::param_axis = csInvalidStringID;
csStringID celPcMechanicsObject::param_angle = csInvalidStringID;
csStringID celPcMechanicsObject::param_size = csInvalidStringID;
csStringID celPcMechanicsObject::param_sizeadjustment = csInvalidStringID;
csStringID celPcMechanicsObject::param_normal = csInvalidStringID;
csStringID celPcMechanicsObject::param_otherbody = csInvalidStringID;
csStringID celPcMechanicsObject::param_force = csInvalidStringID;
csStringID celPcMechanicsObject::param_relative = csInvalidStringID;
csStringID celPcMechanicsObject::param_position = csInvalidStringID;
csStringID celPcMechanicsObject::param_rotation = csInvalidStringID;
csStringID celPcMechanicsObject::param_seconds = csInvalidStringID;
csStringID celPcMechanicsObject::param_velocity = csInvalidStringID;
csStringID celPcMechanicsObject::param_tag = csInvalidStringID;
csStringID celPcMechanicsObject::param_forward = csInvalidStringID;
csStringID celPcMechanicsObject::param_up = csInvalidStringID;
csStringID celPcMechanicsObject::param_depth = csInvalidStringID;
csStringID celPcMechanicsObject::param_group = csInvalidStringID;

PropertyHolder celPcMechanicsObject::propinfo;

celPcMechanicsObject::celPcMechanicsObject (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  scfiDynamicsCollisionCallback = new DynamicsCollisionCallback (this);

  btype = CEL_BODY_INVALID;
  bdata = 0;

  body = 0;
  pcmesh = 0;
  mechsystem = 0;

  forceidseed = 1;
  last_tag = csArrayItemNotFound;

  friction = 1;
  elasticity = 0;
  softness = 0;
  density = 1;
  mass = 1;

  is_static = false;
  cd_enabled = true;

  if (param_friction == csInvalidStringID)
  {
    // Parameters.
    param_friction = pl->FetchStringID ("cel.parameter.friction");
    param_mass = pl->FetchStringID ("cel.parameter.mass");
    param_elasticity = pl->FetchStringID ("cel.parameter.elasticity");
    param_density = pl->FetchStringID ("cel.parameter.density");
    param_softness = pl->FetchStringID ("cel.parameter.softness");
    param_lift = pl->FetchStringID ("cel.parameter.lift");
    param_drag = pl->FetchStringID ("cel.parameter.drag");
    param_static = pl->FetchStringID ("cel.parameter.static");
    param_systempcent = pl->FetchStringID ("cel.parameter.systempcent");
    param_systempctag = pl->FetchStringID ("cel.parameter.systempctag");
    param_meshpctag = pl->FetchStringID ("cel.parameter.meshpctag");
    param_radius = pl->FetchStringID ("cel.parameter.radius");
    param_radiusadjustment = pl->FetchStringID ("cel.parameter.radiusadjustment");
    param_offset = pl->FetchStringID ("cel.parameter.offset");
    param_length = pl->FetchStringID ("cel.parameter.length");
    param_axis = pl->FetchStringID ("cel.parameter.axis");
    param_angle = pl->FetchStringID ("cel.parameter.angle");
    param_size = pl->FetchStringID ("cel.parameter.size");
    param_sizeadjustment = pl->FetchStringID ("cel.parameter.sizeadjustment");
    param_normal = pl->FetchStringID ("cel.parameter.normal");
    param_otherbody = pl->FetchStringID ("cel.parameter.otherbody");
    param_force = pl->FetchStringID ("cel.parameter.force");
    param_relative = pl->FetchStringID ("cel.parameter.relative");
    param_position = pl->FetchStringID ("cel.parameter.position");
    param_seconds = pl->FetchStringID ("cel.parameter.seconds");
    param_velocity = pl->FetchStringID ("cel.parameter.velocity");
    param_tag = pl->FetchStringID ("cel.parameter.tag");
    param_forward = pl->FetchStringID ("cel.parameter.forward");
    param_up = pl->FetchStringID ("cel.parameter.up");
    param_rotation = pl->FetchStringID ("cel.parameter.rotation");
    param_depth = pl->FetchStringID ("cel.parameter.depth");
    param_group = pl->FetchStringID ("cel.parameter.group");
  }

  params = new celGenericParameterBlock (4);
  params->SetParameterDef (0, param_otherbody, "otherbody");
  params->SetParameterDef (1, param_position, "position");
  params->SetParameterDef (2, param_normal, "normal");
  params->SetParameterDef (3, param_depth, "depth");

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_initphys, "cel.action.InitPhys");
    AddAction (action_makestatic, "cel.action.MakeStatic");
    AddAction (action_setsystem, "cel.action.SetSystem");
    AddAction (action_setmesh, "cel.action.SetMesh");
    AddAction (action_setcolliderboundingsphere, "cel.action.SetColliderBoundingSphere");
    AddAction (action_setcollidersphere, "cel.action.SetColliderSphere");
    AddAction (action_setcollidercylinder, "cel.action.SetColliderCylinder");
    AddAction (action_setcolliderboundingbox, "cel.action.SetColliderBoundingBox");
    AddAction (action_setcolliderbox, "cel.action.SetColliderBox");
    AddAction (action_setcolliderplane, "cel.action.SetColliderPlane");
    AddAction (action_setcollidermesh, "cel.action.SetColliderMesh");
    AddAction (action_setcolliderconvexmesh, "cel.action.SetColliderConvexMesh");
    AddAction (action_setlinearvelocity, "cel.action.SetLinearVelocity");
    AddAction (action_setangularvelocity, "cel.action.SetAngularVelocity");
    AddAction (action_addforceonce, "cel.action.AddForceOnce");
    AddAction (action_addforceduration, "cel.action.AddForceDuration");
    AddAction (action_addforceframe, "cel.action.AddForceFrame");
    AddAction (action_addforcetagged, "cel.action.AddForceTagged");
    AddAction (action_removeforcetagged, "cel.action.RemoveForceTagged");
    AddAction (action_clearforces, "cel.action.ClearForces");
    AddAction (action_setposition, "cel.action.SetPosition");
    AddAction (action_clearrotation, "cel.action.ClearRotation");
    AddAction (action_rotate, "cel.action.Rotate");
    AddAction (action_lookat, "cel.action.LookAt");
    AddAction (action_addtogroup, "cel.action.AddToGroup");
  }

  // For properties.
  propinfo.SetCount (6);
  AddProperty (propid_lasttag, "cel.property.lasttag",
	CEL_DATA_LONG, true, "Last tag from AddForceTagged.", &last_tag);
  AddProperty (propid_linearvelocity, "cel.property.linearvelocity",
	CEL_DATA_VECTOR3, false, "Linear velocity.", 0);
  AddProperty (propid_angularvelocity, "cel.property.angularvelocity",
	CEL_DATA_VECTOR3, false, "Angular velocity.", 0);
  AddProperty (propid_static, "cel.property.static",
	CEL_DATA_BOOL, false, "Static yes/no.", 0);
  AddProperty (propid_cdcallback, "cel.property.cdcallback",
	CEL_DATA_BOOL, false, "CD callback enabled yes/no.", &cd_enabled);
  AddProperty (propid_enabled, "cel.property.enabled",
	CEL_DATA_BOOL, false, "CD enabled yes/no.", 0);
}

celPcMechanicsObject::~celPcMechanicsObject ()
{
  if (mechsystem)
    mechsystem->ClearForces ((iPcMechanicsObject*)this);
  if (body)
  {
    if(pl)
      pl->UnattachEntity (body->QueryObject (), entity);
    body->SetCollisionCallback (0);
    if (mechsystem)
      mechsystem->RemoveBody (body);
  }
  if (scfiDynamicsCollisionCallback)
    scfiDynamicsCollisionCallback->DecRef ();

  delete params;
  delete bdata;
}

#define DYNBODY_SERIAL 1

csPtr<iCelDataBuffer> celPcMechanicsObject::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (DYNBODY_SERIAL);
  csRef<iCelPropertyClass> pc;
  if (pcmesh)
  {
    pc = scfQueryInterface<iCelPropertyClass> (pcmesh);
    databuf->Add (pc);
  }
  else
    databuf->Add ((iCelPropertyClass*) 0);
  GetMechSystem ();
  if (mechsystem)
  {
    pc = scfQueryInterface<iCelPropertyClass> (mechsystem);
    databuf->Add (pc);
  }
  else
    databuf->Add ((iCelPropertyClass*) 0);
  databuf->Add (static_cast<int32> (btype));
  switch (btype)
  {
    case CEL_BODY_SPHERE:
      {
        sphere_data* sd = (sphere_data*)bdata;
        databuf->Add (sd->radius);
        databuf->Add (sd->offset);
      }
      break;
    case CEL_BODY_BOX:
      {
        box_data* bd = (box_data*)bdata;
        databuf->Add (bd->size);
        const csMatrix3& m = bd->transform.GetO2T ();
        databuf->Add (m.Row1 ());
        databuf->Add (m.Row2 ());
        databuf->Add (m.Row3 ());
        databuf->Add (bd->transform.GetO2TTranslation ());
      }
      break;
    case CEL_BODY_CYLINDER:
      {
        cylinder_data* cd = (cylinder_data*)bdata;
        databuf->Add (cd->length);
        databuf->Add (cd->radius);
        const csMatrix3& m = cd->transform.GetO2T ();
        databuf->Add (m.Row1 ());
        databuf->Add (m.Row2 ());
        databuf->Add (m.Row3 ());
        databuf->Add (cd->transform.GetO2TTranslation ());
      }
      break;
    case CEL_BODY_PLANE:
      {
        plane_data* pd = (plane_data*)bdata;
        databuf->Add (pd->plane.A ());
        databuf->Add (pd->plane.B ());
        databuf->Add (pd->plane.C ());
        databuf->Add (pd->plane.D ());
      }
      break;
    case CEL_BODY_MESH:
      break;
    case CEL_BODY_CONVEXMESH:
      break;
  }
  databuf->Add (friction);
  databuf->Add (elasticity);
  databuf->Add (softness);
  databuf->Add (density);
  databuf->Add (mass);
  databuf->Add (lift);
  databuf->Add (drag);
  databuf->Add (is_static);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMechanicsObject::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != DYNBODY_SERIAL)
  {
    CS_REPORT(ERROR,"serialnr != DYNBODY_SERIAL.  Cannot load.");
    return false;
  }
  csRef<iPcMesh> pcm = scfQueryInterface<iPcMesh> (databuf->GetPC ());
  SetMesh (pcm);
  csRef<iPcMechanicsSystem> pcms = scfQueryInterface<iPcMechanicsSystem> (databuf->GetPC ());
  SetMechanicsSystem (pcms);
  btype = (int) databuf->GetInt32 ();
  switch (btype)
  {
    case CEL_BODY_SPHERE:
      {
	float radius = databuf->GetFloat ();
	csVector3 vec;
	databuf->GetVector3 (vec);
        delete bdata;
        bdata = new sphere_data (radius, vec);
      }
      break;
    case CEL_BODY_BOX:
      {
        csVector3 size, row1, row2, row3, trans;
	databuf->GetVector3 (size);
	databuf->GetVector3 (row1);
	databuf->GetVector3 (row2);
	databuf->GetVector3 (row3);
	databuf->GetVector3 (trans);
	delete bdata;
	bdata = new box_data (size, csOrthoTransform (
          csMatrix3 (row1.x, row1.y, row1.z,
            row2.x, row2.y, row2.z,
            row3.x, row3.y, row3.z), trans));
      }
      break;
    case CEL_BODY_CYLINDER:
      {
        float length, radius;
        length = databuf->GetFloat ();
        radius = databuf->GetFloat ();
        csVector3 row1, row2, row3, trans;
	databuf->GetVector3 (row1);
	databuf->GetVector3 (row2);
	databuf->GetVector3 (row3);
	databuf->GetVector3 (trans);
	delete bdata;
	bdata = new cylinder_data (length, radius, csOrthoTransform (
          csMatrix3 (row1.x, row1.y, row1.z,
            row2.x, row2.y, row2.z,
            row3.x, row3.y, row3.z), trans));
      }
      break;
    case CEL_BODY_PLANE:
      {
        float a, b, c, d;
        a = databuf->GetFloat ();
        b = databuf->GetFloat ();
        c = databuf->GetFloat ();
        d = databuf->GetFloat ();
	delete bdata;
	bdata = new plane_data (csPlane3 (databuf->GetFloat (),
          databuf->GetFloat (), databuf->GetFloat (), databuf->GetFloat ()));
      }
      break;
    case CEL_BODY_MESH:
      break;
    case CEL_BODY_CONVEXMESH:
      break;
  }
  friction = databuf->GetFloat ();
  elasticity = databuf->GetFloat ();
  softness = databuf->GetFloat ();
  density = databuf->GetFloat ();
  mass = databuf->GetFloat ();
  databuf->GetVector3 (lift);
  drag = databuf->GetFloat ();
  is_static = databuf->GetBool ();
  return true;
}

bool celPcMechanicsObject::SetPropertyIndexed (int idx, bool v)
{
  if (idx == propid_static)
  {
    MakeStatic (v);
    return true;
  }
  if (idx == propid_enabled)
  {
    if (GetBody ())
    {
      if (v)
        GetBody()->Enable();
      else
        GetBody()->Disable();
      return true;
    }
  }
  return false;
}

bool celPcMechanicsObject::GetPropertyIndexed (int idx, bool& v)
{
  if (idx == propid_static)
  {
    v = is_static;
    return true;
  }
  if (idx == propid_enabled)
  {
    if (GetBody ())
    {
      v = GetBody()->IsEnabled();
      return true;
    }
  }
  return false;
}

bool celPcMechanicsObject::SetPropertyIndexed (int idx,
    const csVector3& vec)
{
  switch (idx)
  {
    case propid_linearvelocity:
      SetLinearVelocity (vec);
      return true;
    case propid_angularvelocity:
      SetAngularVelocity (vec);
      return true;
    default:
      return false;
  }
}

bool celPcMechanicsObject::GetPropertyIndexed (int idx,
    csVector3& vec)
{
  switch (idx)
  {
    case propid_linearvelocity:
      vec = GetLinearVelocity ();
      return true;
    case propid_angularvelocity:
      vec = GetAngularVelocity ();
      return true;
    default:
      return false;
  }
}

bool celPcMechanicsObject::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setlinearvelocity:
      {
        CEL_FETCH_VECTOR3_PAR (velocity,params,param_velocity);
        if (!p_velocity)
        {
          CS_REPORT(ERROR,"'velocity' missing!");
          return false;
        }
        SetLinearVelocity (velocity);
	return true;
      }
    case action_setangularvelocity:
      {
        CEL_FETCH_VECTOR3_PAR (velocity,params,param_velocity);
        if (!p_velocity)
        {
          CS_REPORT(ERROR,"'velocity' missing!");
          return false;
        }
        SetAngularVelocity (velocity);
	return true;
      }
    case action_addforceonce:
      {
        CEL_FETCH_VECTOR3_PAR (force,params,param_force);
        if (!p_force)
        {
          CS_REPORT(ERROR,"'force' missing!");
          return false;
        }
        CEL_FETCH_BOOL_PAR (relative,params,param_relative);
        if (!p_relative) relative = false;
        CEL_FETCH_VECTOR3_PAR (position,params,param_position);
        if (!p_position) position.Set (0, 0, 0);
        AddForceOnce (force, relative, position);
	return true;
      }
    case action_addforceduration:
      {
        CEL_FETCH_VECTOR3_PAR (force,params,param_force);
        if (!p_force)
        {
          CS_REPORT(ERROR,"'force' missing!");
          return false;
        }
        CEL_FETCH_BOOL_PAR (relative,params,param_relative);
        if (!p_relative) relative = false;
        CEL_FETCH_VECTOR3_PAR (position,params,param_position);
        if (!p_position) position.Set (0, 0, 0);
        CEL_FETCH_FLOAT_PAR (seconds,params,param_seconds);
        if (!p_seconds)
        {
          CS_REPORT(ERROR,"'seconds' missing!");
          return false;
        }
        AddForceDuration (force, relative, position, seconds);
	return true;
      }
    case action_addforceframe:
      {
        CEL_FETCH_VECTOR3_PAR (force,params,param_force);
        if (!p_force)
        {
          CS_REPORT(ERROR,"'force' missing!");
          return false;
        }
        CEL_FETCH_BOOL_PAR (relative,params,param_relative);
        if (!p_relative) relative = false;
        CEL_FETCH_VECTOR3_PAR (position,params,param_position);
        if (!p_position) position.Set (0, 0, 0);
        AddForceFrame (force, relative, position);
	return true;
      }
    case action_addforcetagged:
      {
        CEL_FETCH_VECTOR3_PAR (force,params,param_force);
        if (!p_force)
        {
          CS_REPORT(ERROR,"'force' missing!");
          return false;
        }
        CEL_FETCH_BOOL_PAR (relative,params,param_relative);
        if (!p_relative) relative = false;
        CEL_FETCH_VECTOR3_PAR (position,params,param_position);
        if (!p_position) position.Set (0, 0, 0);
        last_tag = AddForceTagged (force, relative, position);
	return true;
      }
    case action_removeforcetagged:
      {
        CEL_FETCH_LONG_PAR (tag,params,param_tag);
        if (!p_tag)
        {
          CS_REPORT(ERROR,"'tag' missing!");
          return false;
        }
        RemoveForceTagged ((uint32)tag);
	return true;
      }
    case action_clearforces:
      ClearForces ();
      return true;
    case action_setposition:
      {
        CEL_FETCH_VECTOR3_PAR (position,params,param_position);
        if (!p_position)
        {
          CS_REPORT(ERROR,"'position' missing!");
          return false;
        }
        GetBody ()->SetPosition (position);
	return true;
      }
    case action_clearrotation:
      GetBody ()->SetOrientation (csMatrix3 ());
      SetTransform (GetBody ()->GetTransform ());
      return true;
    case action_rotate:
      {
        CEL_FETCH_VECTOR3_PAR (rotation,params,param_rotation);
        if (!p_rotation)
        {
          CS_REPORT(ERROR,"'rotation' missing!");
          return false;
        }
        csQuaternion quat;
        quat.SetEulerAngles(rotation);
        csReversibleTransform tr (quat.GetMatrix (), csVector3 (0));
        GetBody ()->SetTransform(tr * GetBody ()->GetTransform ());
        SetTransform (GetBody ()->GetTransform ());
	return true;
      }
    case action_lookat:
      {
        CEL_FETCH_VECTOR3_PAR (forward,params,param_forward);
        if (!p_forward) forward.Set (0, 0, 1);
        CEL_FETCH_VECTOR3_PAR (up,params,param_up);
        if (!p_up) up.Set (0, 1, 0);
        csReversibleTransform t;
        t.LookAt (forward, up);
        GetBody ()->SetOrientation (t.GetO2T ());
	return true;
      }
    case action_initphys:
      {
        CEL_FETCH_FLOAT_PAR (friction,params,param_friction);
        if (p_friction)
          SetFriction (friction);
        CEL_FETCH_FLOAT_PAR (mass,params,param_mass);
        if (p_mass)
          SetMass (mass);
        CEL_FETCH_FLOAT_PAR (elasticity,params,param_elasticity);
        if (p_elasticity)
          SetElasticity (elasticity);
        CEL_FETCH_FLOAT_PAR (density,params,param_density);
        if (p_density)
          SetDensity (density);
        CEL_FETCH_FLOAT_PAR (softness,params,param_softness);
        if (p_softness)
          SetSoftness (softness);
        CEL_FETCH_VECTOR3_PAR (lift,params,param_lift);
        if (p_lift)
          SetLift (lift);
        CEL_FETCH_FLOAT_PAR (drag,params,param_drag);
        if (p_drag)
          SetDrag (drag);
	return true;
      }
    case action_makestatic:
      {
        CEL_FETCH_BOOL_PAR (makestatic,params,param_static);
        if (!p_makestatic)
        {
          CS_REPORT(ERROR,"Couldn't get static!");
          return false;
        }
        MakeStatic (makestatic);
	return true;
      }
    case action_setsystem:
      {
        CEL_FETCH_STRING_PAR (syspcent,params,param_systempcent);
        if (!p_syspcent)
        {
          CS_REPORT(ERROR,"Couldn't get mechanics system entity!");
          return false;
        }
        CEL_FETCH_STRING_PAR (syspctag,params,param_systempctag);
        if (!p_syspctag)
        {
          CS_REPORT(ERROR,"Couldn't get mechanics system tag!");
          return false;
        }
        csRef<iCelEntity> sysent = pl->FindEntity (syspcent);
        csRef<iPcMechanicsSystem> mechsyss = 0;
        mechsyss = CEL_QUERY_PROPCLASS_TAG_ENT(sysent,iPcMechanicsSystem,syspctag);
        assert (mechsyss);
        SetMechanicsSystem (mechsyss);
	return true;
      }
    case action_setmesh:
      {
        CEL_FETCH_STRING_PAR (meshpctag,params,param_meshpctag);
        csRef<iPcMesh> pcmesh;
        if (!p_meshpctag)
          pcmesh = CEL_QUERY_PROPCLASS_ENT(GetEntity (),iPcMesh);
        else
          pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT(GetEntity (),iPcMesh,
		    meshpctag);
        SetMesh (pcmesh);
	return true;
      }
    case action_setcollidersphere:
      {
        CEL_FETCH_FLOAT_PAR (radius,params,param_radius);
        if (!p_radius) radius = 1.0f;
        CEL_FETCH_VECTOR3_PAR (offset,params,param_offset);
        if (!p_offset) offset.Set (0, 0, 0);
        AttachColliderSphere (radius, offset);
        return true;
      }
    case action_setcolliderboundingsphere:
      {
        CEL_FETCH_FLOAT_PAR (radiusadjustment,params,param_radiusadjustment);
        if (!p_radiusadjustment) radiusadjustment = 0.0f;
        AttachColliderBoundingSphere (radiusadjustment);
        return true;
      }
    case action_setcollidercylinder:
      {
        CEL_FETCH_FLOAT_PAR (length,params,param_length);
        if (!p_length)
        {
          CS_REPORT(ERROR,"Couldn't get length!");
          return false;
        }
        CEL_FETCH_FLOAT_PAR (radius,params,param_radius);
        if (!p_radius) radius = 1.0f;
        CEL_FETCH_VECTOR3_PAR (axis,params,param_axis);
        if (!p_axis) axis.Set (0, 0, 0);
        CEL_FETCH_FLOAT_PAR (angle,params,param_angle);
        if (!p_angle) angle = 0;
        CEL_FETCH_VECTOR3_PAR (offset,params,param_offset);
        if (!p_offset) offset.Set (0, 0, 0);
        AttachColliderCylinder (length, radius, csOrthoTransform (csMatrix3
	    (axis.x, axis.y, axis.z, angle), offset));
	return true;
      }
    case action_setcolliderboundingbox:
      {
        CEL_FETCH_VECTOR3_PAR (sizeadjustment,params,param_sizeadjustment);
        if (!p_sizeadjustment) sizeadjustment.Set (0, 0, 0);
        AttachColliderBoundingBox (sizeadjustment);
        return true;
      }
    case action_setcolliderbox:
      {
        CEL_FETCH_VECTOR3_PAR (size,params,param_size);
        if (!p_size) size.Set (1, 1, 1);
        CEL_FETCH_VECTOR3_PAR (axis,params,param_axis);
        if (!p_axis) axis.Set (0, 0, 0);
        CEL_FETCH_FLOAT_PAR (angle,params,param_angle);
        if (!p_angle) angle = 0;
        CEL_FETCH_VECTOR3_PAR (offset,params,param_offset);
        if (!p_offset) offset.Set (0, 0, 0);
        AttachColliderBox (size, csOrthoTransform (csMatrix3 (axis.x, axis.y,
	    axis.z, angle), offset));
	return true;
      }
    case action_setcolliderplane:
      {
        CEL_FETCH_VECTOR3_PAR (normal,params,param_normal);
        if (!p_normal)
        {
          CS_REPORT(ERROR,"Couldn't get normal!");
          return false;
        }
        CEL_FETCH_FLOAT_PAR (offset,params,param_offset);
        if (!p_offset) offset = 0;
        AttachColliderPlane (csPlane3 (normal, offset));
	return true;
      }
    case action_setcollidermesh:
      {
        AttachColliderMesh ();
        return true;
      }
    case action_setcolliderconvexmesh:
      {
        AttachColliderConvexMesh ();
        return true;
      }
    case action_addtogroup:
      {
        CEL_FETCH_STRING_PAR (group,params,param_group);
        if (!p_group) group = 0;
        AddToGroup (group);
        return true;
      }
    default:
      return false;
  }
}

void celPcMechanicsObject::Collision (iRigidBody *thisbody,
	iRigidBody *otherbody, const csVector3& pos,
	const csVector3& normal, float depth)
{
  if (!cd_enabled) return;
  iCelBehaviour* behaviour = entity->GetBehaviour ();
  celData ret;
  // Find the other body's iPcMechanicsObject.
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
  if (behaviour)
    behaviour->SendMessage ("pcdynamicbody_collision", this, ret, params);

  if (!dispatcher_cd)
  {
    dispatcher_cd = entity->QueryMessageChannel ()->CreateMessageDispatcher (
	  this, "cel.mechanics.collision");
    if (!dispatcher_cd) return;
  }
  dispatcher_cd->SendMessage (params);
}

void celPcMechanicsObject::GetMechSystem ()
{
  if (!mechsystem)
  {
    mechsystem = csQueryRegistry<iPcMechanicsSystem> (object_reg);
  }
}

iRigidBody* celPcMechanicsObject::GetBody ()
{
  GetMechSystem ();
  if (!body)
  {
    if (mechsystem)
    {
      csRef<iDynamicSystem> dynsys = 0;
      dynsys = mechsystem->GetDynamicSystem ();
      if (dynsys)
      {
        body = dynsys->CreateBody ();
	body->QueryObject ()->SetName (entity->GetName ());
	pl->AttachEntity (body->QueryObject (), entity);
        body->SetCollisionCallback (scfiDynamicsCollisionCallback);
      }
    }
    else
    {
      CS_REPORT(ERROR,"No mechsys!!!!!!!!!");
      return 0;
    }
  }
  return body;
}

void celPcMechanicsObject::FindMeshLightCamera ()
{
  if (pcmesh || pclight || pccamera) return;
  pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  if (!pcmesh)
  {
    pclight = CEL_QUERY_PROPCLASS_ENT (entity, iPcLight);
    if (!pclight)
    {
      pccamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcCamera);
    }
    else
    {
      pccamera = 0;
    }
  }
  else
  {
    pclight = 0;
    pccamera = 0;
  }
}

void celPcMechanicsObject::SetTransform (const csReversibleTransform& tr)
{
  if (pcmesh)
  {
    pcmesh->GetMesh ()->GetMovable ()->SetTransform (tr);
    pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();
  }
  else if (pclight)
  {
    pclight->GetLight ()->GetMovable ()->SetTransform (tr);
    pclight->GetLight ()->GetMovable ()->UpdateMove ();
  }
  else if (pccamera)
    pccamera->GetCamera ()->SetTransform (tr);
}

csReversibleTransform celPcMechanicsObject::GetFullTransform ()
{
  if (pcmesh)
    return pcmesh->GetMesh ()->GetMovable ()->GetFullTransform ();
  else if (pclight)
    return pclight->GetLight ()->GetMovable ()->GetFullTransform ();
  else if (pccamera)
    return pccamera->GetCamera ()->GetTransform ();
  else
    return csReversibleTransform ();
}

iPcMesh* celPcMechanicsObject::GetMesh ()
{
  FindMeshLightCamera ();
  return pcmesh;
}

iPcLight* celPcMechanicsObject::GetLight ()
{
  FindMeshLightCamera ();
  return pclight;
}

iPcCamera* celPcMechanicsObject::GetCamera ()
{
  FindMeshLightCamera ();
  return pccamera;
}

void celPcMechanicsObject::SetFriction (float friction)
{
  celPcMechanicsObject::friction = friction;
}

void celPcMechanicsObject::SetMass (float mass)
{
  celPcMechanicsObject::mass = mass;
  if (GetBody () != 0)
    body->SetProperties (mass, body->GetCenter (), csMatrix3 () * mass);
}

void celPcMechanicsObject::SetElasticity (float elasticity)
{
  celPcMechanicsObject::elasticity = elasticity;
}

void celPcMechanicsObject::SetDensity (float density)
{
  celPcMechanicsObject::density = density;
}

void celPcMechanicsObject::SetSoftness (float softness)
{
  celPcMechanicsObject::softness = softness;
}

void celPcMechanicsObject::SetLift (const csVector3& lift)
{
  celPcMechanicsObject::lift = lift;
}

void celPcMechanicsObject::SetDrag (float drag)
{
  celPcMechanicsObject::drag = drag;
}

void celPcMechanicsObject::SetLinearVelocity (const csVector3& vel)
{
  if (GetBody ())
    body->SetLinearVelocity (vel);
}

const csVector3 celPcMechanicsObject::GetLinearVelocity ()
{
  return body->GetLinearVelocity ();
}

void celPcMechanicsObject::SetAngularVelocity (const csVector3& vel)
{
  if (GetBody ())
    body->SetAngularVelocity (vel);
}

const csVector3 celPcMechanicsObject::GetAngularVelocity ()
{
  return body->GetAngularVelocity ();
}

void celPcMechanicsObject::MakeStatic (bool stat)
{
  //(vk) commented this test since is_static seems somehow not remembered;
  // ie. make mechobj static at its creation and make it dynamic later,
  // from behaviour or a CEL action from XML...
  //if (stat == is_static) return;
  is_static = stat;
  if (!GetBody ()) return;
  if (is_static)
  {
    body->SetMoveCallback (0);
    body->MakeStatic ();
  }
  else
  {
    body->MakeDynamic ();
  }
}

void celPcMechanicsObject::AttachObject ()
{
  if (pcmesh)
  {
    body->AttachMesh (pcmesh->GetMesh ());
  }
  if (pclight)
  {
    body->AttachLight (pclight->GetLight ());
  }
  if (pccamera)
  {
    body->AttachCamera (pccamera->GetCamera ());
  }
}

void celPcMechanicsObject::AttachColliderSphere (float radius,
	const csVector3& offset)
{
  if (!GetBody ()) return;
  FindMeshLightCamera ();
  body->AttachColliderSphere (radius, offset, friction, density,
  	elasticity, softness);
  body->AdjustTotalMass (mass);
  body->SetTransform (GetFullTransform ());
  AttachObject ();

  delete bdata;
  bdata = new sphere_data (radius, offset);
  btype = CEL_BODY_SPHERE;
}

void celPcMechanicsObject::AttachColliderBoundingSphere (float sizeadjustment)
{
  if (!GetBody ()) return;
  FindMeshLightCamera ();
  if (!pcmesh) return;
  csVector3 offset(0);
  float radius=1.0f;
  csRef<iObjectModel> meshobjmodel;
  meshobjmodel=pcmesh->GetMesh ()->GetMeshObject ()->GetObjectModel ();
  meshobjmodel->GetRadius(radius,offset);
  body->AttachColliderSphere (radius + sizeadjustment, offset, friction, density,
  	elasticity, softness);
  body->AdjustTotalMass (mass);
  body->SetTransform (GetFullTransform ());
  AttachObject ();

  delete bdata;
  bdata = new sphere_data (radius, offset);
  btype = CEL_BODY_SPHERE;
}

void celPcMechanicsObject::AttachColliderCylinder (float length, float radius,
	const csOrthoTransform& trans)
{
  if (!GetBody ()) return;
  FindMeshLightCamera ();
  body->AttachColliderCylinder (length, radius, trans, friction, density,
  	elasticity, softness);
  body->AdjustTotalMass (mass);
  body->SetTransform (GetFullTransform ());
  AttachObject ();

  delete bdata;
  bdata = new cylinder_data (length, radius, trans);
  btype = CEL_BODY_CYLINDER;
}

void celPcMechanicsObject::AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans)
{
  if (!GetBody ()) return;
  FindMeshLightCamera ();
  body->AttachColliderBox (size, trans, friction, density, elasticity,
  	softness);
  body->AdjustTotalMass (mass);
  body->SetTransform (GetFullTransform ());
  AttachObject ();

  delete bdata;
  bdata = new box_data (size, trans);
  btype = CEL_BODY_BOX;
}

void celPcMechanicsObject::AttachColliderBoundingBox (const csVector3& sizeadjustment)
{
  if (!GetBody ()) return;
  FindMeshLightCamera ();
  if(!pcmesh) return;
  csRef<iObjectModel> meshobjmodel;
  meshobjmodel=pcmesh->GetMesh ()->GetMeshObject ()->GetObjectModel ();
  csBox3 boundingbox = meshobjmodel->GetObjectBoundingBox();
  csOrthoTransform t;
  t.SetOrigin(boundingbox.GetCenter());
  csVector3 size = boundingbox.GetSize() + sizeadjustment;
  body->AttachColliderBox(size, t, friction, density, elasticity,
  	softness);
  body->AdjustTotalMass (mass);
  body->SetTransform (GetFullTransform ());
  AttachObject ();

  delete bdata;
  bdata = new box_data (boundingbox.GetSize(), t);
  btype = CEL_BODY_BOX;
}

void celPcMechanicsObject::AttachColliderPlane (const csPlane3& plane)
{
  if (!GetBody ()) return;
  FindMeshLightCamera ();
  body->AttachColliderPlane (plane, friction, density, elasticity,
  	softness);
  body->AdjustTotalMass (mass);
  body->SetTransform (GetFullTransform ());
  AttachObject ();

  delete bdata;
  bdata = new plane_data (plane);
  btype = CEL_BODY_PLANE;
}

void celPcMechanicsObject::AttachColliderMesh ()
{
  if (!GetBody ()) return;
  FindMeshLightCamera ();
  if (!pcmesh) return;
  iMeshWrapper* mesh = pcmesh->GetMesh ();
  csReversibleTransform tr;
  body->AttachColliderMesh (mesh, tr,
  	friction, density, elasticity, softness);
  body->AdjustTotalMass (mass);
  body->SetTransform (mesh->GetMovable ()->GetFullTransform ());
  body->AttachMesh (mesh);

  delete bdata;
  bdata = 0;
  btype = CEL_BODY_MESH;
}

void celPcMechanicsObject::AttachColliderConvexMesh ()
{
  if (!GetBody ()) return;
  FindMeshLightCamera ();
  if (!pcmesh) return;
  iMeshWrapper* mesh = pcmesh->GetMesh ();
  csReversibleTransform tr;
  body->AttachColliderConvexMesh (mesh, tr,
  	friction, density, elasticity, softness);
  body->AdjustTotalMass (mass);
  body->SetTransform (mesh->GetMovable ()->GetFullTransform ());
  body->AttachMesh (mesh);

  delete bdata;
  bdata = 0;
  btype = CEL_BODY_CONVEXMESH;
}

uint32 celPcMechanicsObject::AddForceTagged (const csVector3& force,
	bool relative, const csVector3& position)
{
  uint32 forceid = forceidseed++;
  mechsystem->AddForceTagged ((iPcMechanicsObject*)this, force, relative,
	position, forceid);
  return forceid;
}

void celPcMechanicsObject::RemoveForceTagged (uint32 forceid)
{
  mechsystem->RemoveForceTagged ((iPcMechanicsObject*)this, forceid);
}

void celPcMechanicsObject::AddForceOnce (const csVector3& force, bool relative,
	const csVector3& position)
{
  if (!GetBody ()) return;
  if (relative)
  {
    if (position.IsZero ())
      GetBody ()->AddRelForce (force);
    else
      GetBody ()->AddRelForceAtRelPos (force, position);
  }
  else
  {
    if (position.IsZero ())
      GetBody ()->AddForce (force);
    else
      GetBody ()->AddForceAtPos (force, position);
  }
}

void celPcMechanicsObject::AddForceDuration (const csVector3& force,
	bool relative, const csVector3& position, float seconds)
{
  mechsystem->AddForceDuration ((iPcMechanicsObject*)this, force, relative,
	position, seconds);
}

void celPcMechanicsObject::AddForceFrame (const csVector3& force,
	bool relative, const csVector3& position)
{
  mechsystem->AddForceFrame ((iPcMechanicsObject*)this, force, relative,
  	position);
}

void celPcMechanicsObject::ClearForces ()
{
  mechsystem->ClearForces ((iPcMechanicsObject*)this);
}

void celPcMechanicsObject::AddToGroup (const char* group)
{
  if (!GetBody ()) return;
  mechsystem->AddBodyToGroup (GetBody (), group);
}

void celPcMechanicsObject::RemoveFromGroup (const char* group)
{
  if (!GetBody ()) return;
  mechsystem->RemoveBodyFromGroup (GetBody (), group);
}

iJoint* celPcMechanicsObject::CreateJoint (iPcMechanicsObject* other)
{
  if (!GetBody ()) return 0;
  return mechsystem->CreateJoint (GetBody (), other->GetBody ());
}

csVector3 celPcMechanicsObject::LocalToWorld(csVector3 local)
{
  iRigidBody *rb = GetBody();
  csReversibleTransform rt(rb->GetOrientation() , rb->GetPosition());
  return rt.This2OtherRelative(local);
}
csVector3 celPcMechanicsObject::WorldToLocal(csVector3 world)
{
  iRigidBody *rb = GetBody();
  csReversibleTransform rt(rb->GetOrientation() , rb->GetPosition());
  return rt.Other2ThisRelative(world);
}

//---------------------------------------------------------------------------

// Parameters.
csStringID celPcMechanicsJoint::param_body = csInvalidStringID;
csStringID celPcMechanicsJoint::param_position = csInvalidStringID;
csStringID celPcMechanicsJoint::param_min = csInvalidStringID;
csStringID celPcMechanicsJoint::param_max = csInvalidStringID;
csStringID celPcMechanicsJoint::param_x = csInvalidStringID;
csStringID celPcMechanicsJoint::param_y = csInvalidStringID;
csStringID celPcMechanicsJoint::param_z = csInvalidStringID;
PropertyHolder celPcMechanicsJoint::propinfo;


celPcMechanicsJoint::celPcMechanicsJoint (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (param_body == csInvalidStringID)
  {
    // Parameters.
    param_body = pl->FetchStringID ("cel.parameter.body");
    param_position = pl->FetchStringID ("cel.parameter.position");
    param_min = pl->FetchStringID ("cel.parameter.min");
    param_max = pl->FetchStringID ("cel.parameter.max");
    param_x = pl->FetchStringID ("cel.parameter.x");
    param_y = pl->FetchStringID ("cel.parameter.y");
    param_z = pl->FetchStringID ("cel.parameter.z");
  }

  params = new celOneParameterBlock ();
  //params->SetParameterDef (param_otherbody, "otherbody");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_setparentbody, "cel.action.SetParentBody");
    AddAction (action_setposition, "cel.action.SetPosition");
    AddAction (action_setconstraindist, "cel.action.SetConstrainDist");
    AddAction (action_setdistances, "cel.action.SetDistances");
    AddAction (action_setconstrainangle, "cel.action.SetConstrainAngle");
    AddAction (action_setangles, "cel.action.SetAngles");
  }
}

celPcMechanicsJoint::~celPcMechanicsJoint ()
{
  if (joint)
  {
    csRef<iPcMechanicsSystem> mechsystem = csQueryRegistry<iPcMechanicsSystem> (object_reg);
    if (mechsystem)
      mechsystem->RemoveJoint (joint);
  }
  delete params;
}

#define DYNJOINT_SERIAL 1

csPtr<iCelDataBuffer> celPcMechanicsJoint::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (DYNJOINT_SERIAL);
  // @@@ TODO
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMechanicsJoint::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != DYNJOINT_SERIAL)
  {
    CS_REPORT(ERROR,"serialnr != DYNJOINT_SERIAL.  Cannot load.");
    return false;
  }
  // @@@ TODO
  return true;
}

void celPcMechanicsJoint::CreateJoint ()
{
  if (joint) return;
  iRigidBody* body1 = 0;
  if (parent_body)
  {
    csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (
    	parent_body, iPcMechanicsObject);
    if (pcmechobj)
      body1 = pcmechobj->GetBody ();
  }
  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (
  	entity, iPcMechanicsObject);
  if (!pcmechobj)
  {
    fprintf (stderr, "Can't find pcmechobject for entity!\n"); fflush (stderr);
    return;	// @@@ Error?
  }
  iRigidBody* body2 = pcmechobj->GetBody ();
  csRef<iPcMechanicsSystem> mechsystem =
  	csQueryRegistry<iPcMechanicsSystem> (object_reg);
  if (!mechsystem)
  {
    fprintf (stderr, "Can't find mechanics system!\n"); fflush (stderr);
    return;	// @@@ Error?
  }
  joint = mechsystem->CreateJoint (body1, body2);
}

bool celPcMechanicsJoint::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setparentbody:
      {
        CEL_FETCH_STRING_PAR (body,params,param_body);
        if (!p_body)
        {
          CS_REPORT(ERROR,"'body' missing!");
          return false;
        }
        parent_body = pl->FindEntity (body);
        if (!parent_body)
        {
          csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
      	    "cel.propclass.mechanics", "Can't find entity '%s' for parent body!",
	    body);
          return false;
        }
        CreateJoint ();
	return true;
      }
    case action_setposition:
      {
        CEL_FETCH_VECTOR3_PAR (position,params,param_position);
        if (!p_position)
        {
          CS_REPORT(ERROR,"'position' missing!");
          return false;
        }
        CreateJoint ();
        if (joint)
        {
          csOrthoTransform trans;
          trans.SetOrigin (position);
          joint->SetTransform (trans);
        }
	return true;
      }
    case action_setconstraindist:
      {
        CEL_FETCH_BOOL_PAR (x,params,param_x);
        if (!p_x) x = false;
        CEL_FETCH_BOOL_PAR (y,params,param_y);
        if (!p_y) y = false;
        CEL_FETCH_BOOL_PAR (z,params,param_z);
        if (!p_z) z = false;
        CreateJoint ();
        if (joint) joint->SetTransConstraints (x, y, z);
	return true;
      }
    case action_setdistances:
      {
        CEL_FETCH_VECTOR3_PAR (min,params,param_min);
        if (!p_min) min.Set (0, 0, 0);
        CEL_FETCH_VECTOR3_PAR (max,params,param_max);
        if (!p_max) max.Set (0, 0, 0);
        CreateJoint ();
        if (joint)
        {
          joint->SetMinimumDistance (min);
          joint->SetMaximumDistance (max);
        }
	return true;
      }
    case action_setconstrainangle:
      {
        CEL_FETCH_BOOL_PAR (x,params,param_x);
        if (!p_x) x = false;
        CEL_FETCH_BOOL_PAR (y,params,param_y);
        if (!p_y) y = false;
        CEL_FETCH_BOOL_PAR (z,params,param_z);
        if (!p_z) z = false;
        CreateJoint ();
        if (joint) joint->SetRotConstraints (x, y, z);
	return true;
      }
    case action_setangles:
      {
        CEL_FETCH_VECTOR3_PAR (min,params,param_min);
        if (!p_min) min.Set (0, 0, 0);
        CEL_FETCH_VECTOR3_PAR (max,params,param_max);
        if (!p_max) max.Set (0, 0, 0);
        CreateJoint ();
        if (joint)
        {
          joint->SetMinimumAngle (min);
          joint->SetMaximumAngle (max);
        }
	return true;
      }
    default:
      return false;
  }
}

