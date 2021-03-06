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
#include "ivaria/bullet.h"

#include "plugins/propclass/mechanics/mechanics.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (MechanicsSystem, "pcphysics.system", "pcmechsys")
CEL_IMPLEMENT_FACTORY_ALT (MechanicsObject, "pcphysics.object", "pcmechobject")
CEL_IMPLEMENT_FACTORY_ALT (MechanicsJoint, "pcphysics.joint", "pcmechjoint")

//---------------------------------------------------------------------------

// Parameters.
csStringID celPcMechanicsSystem::param_dynsys = csInvalidStringID;
csStringID celPcMechanicsSystem::param_gravity = csInvalidStringID;
csStringID celPcMechanicsSystem::param_time = csInvalidStringID;
csStringID celPcMechanicsSystem::param_simulationspeed = csInvalidStringID;
csStringID celPcMechanicsSystem::param_plugin = csInvalidStringID;
PropertyHolder celPcMechanicsSystem::propinfo;

celPcMechanicsSystem::celPcMechanicsSystem (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  scfiCelTimerListener = new CelTimerListener (this);
  pl->CallbackEveryFrame (scfiCelTimerListener, CEL_EVENT_PRE);

  delta = 0.01f;
  remaining_delta = 0;
  simulationspeed=1.0f;
  pluginName = "crystalspace.dynamics.ode";
  dynsystem_error_reported = false;

  if (param_dynsys == csInvalidStringID)
  {
    // Parameters.
    param_dynsys = pl->FetchStringID ("dynsys");
    param_gravity = pl->FetchStringID ("gravity");
    param_time = pl->FetchStringID ("time");
    param_simulationspeed = pl->FetchStringID ("simulationspeed");
    param_plugin = pl->FetchStringID ("plugin");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.mechanics.system.action.");
    AddAction (action_setsystem, "SetSystem");
    AddAction (action_setgravity, "SetGravity");
    AddAction (action_quickstep, "QuickStep");
    AddAction (action_enablestepfast, "EnableStepFast");
    AddAction (action_disablestepfast, "DisableStepFast");
    AddAction (action_setsteptime, "SetStepTime");
    AddAction (action_setsimulationspeed, "SetSimulationSpeed");
    AddAction (action_setplugin, "SetPlugin");
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
  if (dynsystem_error_reported) return dynamics;

  dynamics = csQueryRegistryOrLoad<iDynamics> (object_reg, pluginName);
  if (!dynamics)
  {
    if (!dynsystem_error_reported)
    {
      dynsystem_error_reported = true;
      Error ("Can't find dynamic subsystem with name '%s'!", pluginName.GetData ());
    }
    return 0;
  }
  return dynamics;
}

void celPcMechanicsSystem::TickEveryFrame ()
{
  GetDynamicSystem ();
  if (!dynamics) return;
  float elapsed_time = vc->GetElapsedSeconds ();

  if (bullet_dynSys)
  {
    ProcessForces (elapsed_time * simulationspeed);
    dynamics->Step (elapsed_time * simulationspeed);
  }
  else
  {
    // For ODE it is recommended that all steps are done with the
    // same size. So we always will call dynamics->Step(delta) with
    // the constant delta. However, sometimes our elapsed time
    // is not divisible by delta and in that case we have a small
    // time (smaller then delta) left-over. We can't afford to drop
    // that because then speed of physics simulation would differ
    // depending on framerate. So we will put that remainder in
    // remaining_delta and use that here too.
    float delta_modulated = delta*simulationspeed;
    float et = remaining_delta + (elapsed_time / simulationspeed);
    while (et >= delta_modulated)
    {
      ProcessForces (delta_modulated);
      dynamics->Step (delta_modulated);
      et -= delta_modulated;
    }

    // Now we have a small remainder. We remember that in remaining_delta.
    remaining_delta = et;
  }

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

    bullet_dynSys = scfQueryInterface<CS::Physics::Bullet::iDynamicSystem> (dynsystem);
    if (bullet_dynSys)
    {
      dynsystem->SetRollingDampener(.995f);
      bullet_dynSys->SetInternalScale (1.0f);
      bullet_dynSys->SetStepParameters (0.005f, 2, 10);
    }
  }
  return dynsystem;
}

void celPcMechanicsSystem::DisableStepFast ()
{
}

void celPcMechanicsSystem::EnableStepFast ()
{
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
  csRef<iPcMechanicsObject> pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (ent);
  if (!pcmechobj) return 0;
  return pcmechobj->GetBody ();
}

bool celPcMechanicsSystem::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setplugin:
      {
	csString plugin;
	if (!Fetch (plugin, params, param_plugin, true, "")) return false;
        if (!plugin.IsEmpty ())
        {
	  pluginName = plugin;
	  dynsystem_error_reported = false;
        }
        else
        {
          Error ("Couldn't get plugin name!");
          return false;
        }
        return true;
      }
    case action_setsystem:
      {
	csString dynsys;
	if (!Fetch (dynsys, params, param_dynsys, true, "")) return false;
        if (!dynsys.IsEmpty ())
        {
          SetDynamicSystem(dynsys);
        }
        else
        {
          Error ("Couldn't get dynamics system name!");
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
	float time;
	if (!Fetch (time, params, param_time)) return false;
        SetStepTime (time);
	return true;
      }
    case action_setsimulationspeed:
      {
	float simulationspeed;
	if (!Fetch (simulationspeed, params, param_simulationspeed)) return false;
        SetSimulationSpeed (simulationspeed);
	return true;
      }
    case action_setgravity:
      {
	csVector3 gravity;
	if (!Fetch (gravity, params, param_gravity)) return false;
        GetDynamicSystem();
        if (dynsystem)
          dynsystem->SetGravity (gravity);
	return true;
      }
    default:
      return false;
  }
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
  last_tag = 0;

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
    param_friction = pl->FetchStringID ("friction");
    param_mass = pl->FetchStringID ("mass");
    param_elasticity = pl->FetchStringID ("elasticity");
    param_density = pl->FetchStringID ("density");
    param_softness = pl->FetchStringID ("softness");
    param_lift = pl->FetchStringID ("lift");
    param_drag = pl->FetchStringID ("drag");
    param_static = pl->FetchStringID ("static");
    param_systempcent = pl->FetchStringID ("systempcent");
    param_systempctag = pl->FetchStringID ("systempctag");
    param_meshpctag = pl->FetchStringID ("meshpctag");
    param_radius = pl->FetchStringID ("radius");
    param_radiusadjustment = pl->FetchStringID ("radiusadjustment");
    param_offset = pl->FetchStringID ("offset");
    param_length = pl->FetchStringID ("length");
    param_axis = pl->FetchStringID ("axis");
    param_angle = pl->FetchStringID ("angle");
    param_size = pl->FetchStringID ("size");
    param_sizeadjustment = pl->FetchStringID ("sizeadjustment");
    param_normal = pl->FetchStringID ("normal");
    param_otherbody = pl->FetchStringID ("otherbody");
    param_force = pl->FetchStringID ("force");
    param_relative = pl->FetchStringID ("relative");
    param_position = pl->FetchStringID ("position");
    param_seconds = pl->FetchStringID ("seconds");
    param_velocity = pl->FetchStringID ("velocity");
    param_tag = pl->FetchStringID ("tag");
    param_forward = pl->FetchStringID ("forward");
    param_up = pl->FetchStringID ("up");
    param_rotation = pl->FetchStringID ("rotation");
    param_depth = pl->FetchStringID ("depth");
    param_group = pl->FetchStringID ("group");
  }

  params.AttachNew (new celVariableParameterBlock (4));
  params->AddParameter (param_otherbody);
  params->AddParameter (param_position);
  params->AddParameter (param_normal);
  params->AddParameter (param_depth);

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.mechanics.object.action.");
    AddAction (action_initphys, "InitPhys");
    AddAction (action_makestatic, "MakeStatic");
    AddAction (action_setsystem, "SetSystem");
    AddAction (action_setmesh, "SetMesh");
    AddAction (action_setcolliderboundingsphere, "SetColliderBoundingSphere");
    AddAction (action_setcollidersphere, "SetColliderSphere");
    AddAction (action_setcollidercylinder, "SetColliderCylinder");
    AddAction (action_setcolliderboundingbox, "SetColliderBoundingBox");
    AddAction (action_setcolliderbox, "SetColliderBox");
    AddAction (action_setcolliderplane, "SetColliderPlane");
    AddAction (action_setcollidermesh, "SetColliderMesh");
    AddAction (action_setcolliderconvexmesh, "SetColliderConvexMesh");
    AddAction (action_setlinearvelocity, "SetLinearVelocity");
    AddAction (action_setangularvelocity, "SetAngularVelocity");
    AddAction (action_addforceonce, "AddForceOnce");
    AddAction (action_addforceduration, "AddForceDuration");
    AddAction (action_addforceframe, "AddForceFrame");
    AddAction (action_addforcetagged, "AddForceTagged");
    AddAction (action_removeforcetagged, "RemoveForceTagged");
    AddAction (action_clearforces, "ClearForces");
    AddAction (action_setposition, "SetPosition");
    AddAction (action_clearrotation, "ClearRotation");
    AddAction (action_rotate, "Rotate");
    AddAction (action_lookat, "LookAt");
    AddAction (action_addtogroup, "AddToGroup");
  }

  // For properties.
  propinfo.SetCount (6);
  AddProperty (propid_lasttag, "lasttag",
	CEL_DATA_LONG, true, "Last tag from AddForceTagged.", &last_tag);
  AddProperty (propid_linearvelocity, "linearvelocity",
	CEL_DATA_VECTOR3, false, "Linear velocity.", 0);
  AddProperty (propid_angularvelocity, "angularvelocity",
	CEL_DATA_VECTOR3, false, "Angular velocity.", 0);
  AddProperty (propid_static, "static",
	CEL_DATA_BOOL, false, "Static yes/no.", 0);
  AddProperty (propid_cdcallback, "cdcallback",
	CEL_DATA_BOOL, false, "CD callback enabled yes/no.", &cd_enabled);
  AddProperty (propid_enabled, "enabled",
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

  delete bdata;
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
	csVector3 velocity;
	if (!Fetch (velocity, params, param_velocity)) return false;
        SetLinearVelocity (velocity);
	return true;
      }
    case action_setangularvelocity:
      {
	csVector3 velocity;
	if (!Fetch (velocity, params, param_velocity)) return false;
        SetAngularVelocity (velocity);
	return true;
      }
    case action_addforceonce:
      {
	csVector3 force, position;
	if (!Fetch (force, params, param_force)) return false;
	if (!Fetch (position, params, param_position, true, csVector3 (0, 0, 0))) return false;
	bool relative;
	if (!Fetch (relative, params, param_relative, true, false)) return false;
        AddForceOnce (force, relative, position);
	return true;
      }
    case action_addforceduration:
      {
	csVector3 force, position;
	if (!Fetch (force, params, param_force)) return false;
	if (!Fetch (position, params, param_position, true, csVector3 (0, 0, 0))) return false;
	bool relative;
	if (!Fetch (relative, params, param_relative, true, false)) return false;
	float seconds;
	if (!Fetch (seconds, params, param_seconds)) return false;
        AddForceDuration (force, relative, position, seconds);
	return true;
      }
    case action_addforceframe:
      {
	csVector3 force, position;
	if (!Fetch (force, params, param_force)) return false;
	if (!Fetch (position, params, param_position, true, csVector3 (0, 0, 0))) return false;
	bool relative;
	if (!Fetch (relative, params, param_relative, true, false)) return false;
        AddForceFrame (force, relative, position);
	return true;
      }
    case action_addforcetagged:
      {
	csVector3 force, position;
	if (!Fetch (force, params, param_force)) return false;
	if (!Fetch (position, params, param_position, true, csVector3 (0, 0, 0))) return false;
	bool relative;
	if (!Fetch (relative, params, param_relative, true, false)) return false;
        last_tag = AddForceTagged (force, relative, position);
	return true;
      }
    case action_removeforcetagged:
      {
	long tag;
	if (!Fetch (tag, params, param_tag)) return false;
        RemoveForceTagged ((uint32)tag);
	return true;
      }
    case action_clearforces:
      ClearForces ();
      return true;
    case action_setposition:
      {
	csVector3 position;
	if (!Fetch (position, params, param_position)) return false;
        GetBody ()->SetPosition (position);
	return true;
      }
    case action_clearrotation:
      GetBody ()->SetOrientation (csMatrix3 ());
      SetTransform (GetBody ()->GetTransform ());
      return true;
    case action_rotate:
      {
	csVector3 rotation;
	if (!Fetch (rotation, params, param_rotation)) return false;
        csQuaternion quat;
        quat.SetEulerAngles(rotation);
        csReversibleTransform tr (quat.GetMatrix (), csVector3 (0));
        GetBody ()->SetTransform(tr * GetBody ()->GetTransform ());
        SetTransform (GetBody ()->GetTransform ());
	return true;
      }
    case action_lookat:
      {
	csVector3 forward, up;
	if (!Fetch (forward, params, param_forward, true, csVector3 (0, 0, 1))) return false;
	if (!Fetch (up, params, param_up, true, csVector3 (0, 1, 0))) return false;
        csReversibleTransform t;
        t.LookAt (forward, up);
        GetBody ()->SetOrientation (t.GetO2T ());
	return true;
      }
    case action_initphys:
      {
        if (ParExists (CEL_DATA_FLOAT, params, param_friction))
	{
          float friction;
	  if (!Fetch (friction, params, param_friction)) return false;
          SetFriction (friction);
	}
        if (ParExists (CEL_DATA_FLOAT, params, param_mass))
	{
          float mass;
	  if (!Fetch (mass, params, param_mass)) return false;
          SetMass (mass);
	}
        if (ParExists (CEL_DATA_FLOAT, params, param_elasticity))
	{
          float elasticity;
	  if (!Fetch (elasticity, params, param_elasticity)) return false;
          SetElasticity (elasticity);
	}
        if (ParExists (CEL_DATA_FLOAT, params, param_density))
	{
          float density;
	  if (!Fetch (density, params, param_density)) return false;
          SetDensity (density);
	}
        if (ParExists (CEL_DATA_FLOAT, params, param_softness))
	{
          float softness;
	  if (!Fetch (softness, params, param_softness)) return false;
          SetSoftness (softness);
	}
	if (ParExists (CEL_DATA_VECTOR3, params, param_lift))
	{
	  csVector3 lift;
	  if (!Fetch (lift, params, param_lift)) return false;
          SetLift (lift);
	}
        if (ParExists (CEL_DATA_FLOAT, params, param_drag))
	{
          float drag;
	  if (!Fetch (drag, params, param_drag)) return false;
          SetDrag (drag);
	}
	return true;
      }
    case action_makestatic:
      {
	bool makestatic;
	if (!Fetch (makestatic, params, param_static)) return false;
        MakeStatic (makestatic);
	return true;
      }
    case action_setsystem:
      {
	csString syspcent, syspctag;
	if (!Fetch (syspcent, params, param_systempcent)) return false;
	if (!Fetch (syspctag, params, param_systempctag, true, "")) return false;
        csRef<iCelEntity> sysent = pl->FindEntity (syspcent);
        csRef<iPcMechanicsSystem> mechsyss = 0;
	if (!syspctag.IsEmpty ())
          mechsyss = celQueryPropertyClassTagEntity<iPcMechanicsSystem> (sysent, syspctag);
	else
          mechsyss = celQueryPropertyClassEntity<iPcMechanicsSystem> (sysent);
        if (!mechsyss)
        {
          Error ("Couldn't find entity for dynamic system!");
          return false;
        }
        SetMechanicsSystem (mechsyss);
	return true;
      }
    case action_setmesh:
      {
	csString meshpctag;
	if (!Fetch (meshpctag, params, param_meshpctag, true, "")) return false;
        csRef<iPcMesh> pcmesh;
        if (meshpctag.IsEmpty ())
          pcmesh = celQueryPropertyClassEntity<iPcMesh> (GetEntity ());
        else
          pcmesh = celQueryPropertyClassTagEntity<iPcMesh> (GetEntity (), meshpctag);
        SetMesh (pcmesh);
	return true;
      }
    case action_setcollidersphere:
      {
	float radius;
	if (!Fetch (radius, params, param_radius, true, 1.0f)) return false;
	csVector3 offset;
	if (!Fetch (offset, params, param_offset, true, csVector3 (0, 0, 0))) return false;
        AttachColliderSphere (radius, offset);
        return true;
      }
    case action_setcolliderboundingsphere:
      {
	float radiusadjustment;
	if (!Fetch (radiusadjustment, params, param_radiusadjustment, true, 0.0f)) return false;
        AttachColliderBoundingSphere (radiusadjustment);
        return true;
      }
    case action_setcollidercylinder:
      {
	float length, radius, angle;
	if (!Fetch (length, params, param_length)) return false;
	if (!Fetch (radius, params, param_radius, true, 1.0f)) return false;
	if (!Fetch (angle, params, param_angle, true, 0.0f)) return false;
	csVector3 axis, offset;
	if (!Fetch (axis, params, param_axis, true, csVector3 (0, 0, 0))) return false;
	if (!Fetch (offset, params, param_offset, true, csVector3 (0, 0, 0))) return false;
        AttachColliderCylinder (length, radius, csOrthoTransform (csMatrix3
	    (axis.x, axis.y, axis.z, angle), offset));
	return true;
      }
    case action_setcolliderboundingbox:
      {
	csVector3 sizeadjustment;
	if (!Fetch (sizeadjustment, params, param_sizeadjustment, true, csVector3 (0, 0, 0))) return false;
        AttachColliderBoundingBox (sizeadjustment);
        return true;
      }
    case action_setcolliderbox:
      {
	csVector3 size, axis, offset;
	if (!Fetch (size, params, param_size, true, csVector3 (1, 1, 1))) return false;
	if (!Fetch (axis, params, param_axis, true, csVector3 (0, 0, 0))) return false;
	if (!Fetch (offset, params, param_offset, true, csVector3 (0, 0, 0))) return false;
	float angle;
	if (!Fetch (angle, params, param_angle, true, 0.0f)) return false;
        AttachColliderBox (size, csOrthoTransform (csMatrix3 (axis.x, axis.y,
	    axis.z, angle), offset));
	return true;
      }
    case action_setcolliderplane:
      {
	csVector3 normal;
	if (!Fetch (normal, params, param_normal)) return false;
	float offset;
	if (!Fetch (offset, params, param_offset, true, 0.0f)) return false;
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
	csString group;
	if (!Fetch (group, params, param_group, true, "")) return false;
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
	  this, pl->FetchStringID ("cel.mechanics.collision"));
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
      Error ("No mechsys!!!!!!!!!");
      return 0;
    }
  }
  return body;
}

void celPcMechanicsObject::SetBody (iRigidBody* body)
{
  celPcMechanicsObject::body = body;
}

void celPcMechanicsObject::FindMeshLightCamera ()
{
  if (pcmesh || pclight || pccamera) return;
  pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  if (!pcmesh)
  {
    pclight = celQueryPropertyClassEntity<iPcLight> (entity);
    if (!pclight)
    {
      pccamera = celQueryPropertyClassEntity<iPcCamera> (entity);
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
  if (!GetBody ())
  {
    Error ("No body to apply a collider on!");
    return;
  }
  FindMeshLightCamera ();
  if (!pcmesh)
  {
    Error ("Can't attach collider: mesh is missing!");
    return;
  }

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
    param_body = pl->FetchStringID ("body");
    param_position = pl->FetchStringID ("position");
    param_min = pl->FetchStringID ("min");
    param_max = pl->FetchStringID ("max");
    param_x = pl->FetchStringID ("x");
    param_y = pl->FetchStringID ("y");
    param_z = pl->FetchStringID ("z");
  }

  params = new celOneParameterBlock ();
  //params->SetParameterDef (param_otherbody, "otherbody");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.mechanics.joint.action.");
    AddAction (action_setparentbody, "SetParentBody");
    AddAction (action_setposition, "SetPosition");
    AddAction (action_setconstraindist, "SetConstrainDist");
    AddAction (action_setdistances, "SetDistances");
    AddAction (action_setconstrainangle, "SetConstrainAngle");
    AddAction (action_setangles, "SetAngles");
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

void celPcMechanicsJoint::CreateJoint ()
{
  if (joint) return;
  iRigidBody* body1 = 0;
  if (parent_body)
  {
    csRef<iPcMechanicsObject> pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (parent_body);
    if (pcmechobj)
      body1 = pcmechobj->GetBody ();
  }
  csRef<iPcMechanicsObject> pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (entity);
  if (!pcmechobj)
  {
    Error ("Can't find pcmechobject for entity!\n");
    return;
  }
  iRigidBody* body2 = pcmechobj->GetBody ();
  csRef<iPcMechanicsSystem> mechsystem =
  	csQueryRegistry<iPcMechanicsSystem> (object_reg);
  if (!mechsystem)
  {
    Error ("Can't find mechanics system!\n");
    return;
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
	csString body;
	if (!Fetch (body, params, param_body)) return false;
        parent_body = pl->FindEntity (body);
        if (!parent_body)
          return Error ("Can't find entity '%s' for parent body!", body.GetData ());
        CreateJoint ();
	return true;
      }
    case action_setposition:
      {
	csVector3 position;
	if (!Fetch (position, params, param_position)) return false;
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
	bool x, y, z;
	if (!Fetch (x, params, param_x, true, false)) return false;
	if (!Fetch (y, params, param_y, true, false)) return false;
	if (!Fetch (z, params, param_z, true, false)) return false;
        CreateJoint ();
        if (joint) joint->SetTransConstraints (x, y, z);
	return true;
      }
    case action_setdistances:
      {
	csVector3 min, max;
	if (!Fetch (min, params, param_min, true, csVector3 (0, 0, 0))) return false;
	if (!Fetch (max, params, param_max, true, csVector3 (0, 0, 0))) return false;
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
	bool x, y, z;
	if (!Fetch (x, params, param_x, true, false)) return false;
	if (!Fetch (y, params, param_y, true, false)) return false;
	if (!Fetch (z, params, param_z, true, false)) return false;
        CreateJoint ();
        if (joint) joint->SetRotConstraints (x, y, z);
	return true;
      }
    case action_setangles:
      {
	csVector3 min, max;
	if (!Fetch (min, params, param_min, true, csVector3 (0, 0, 0))) return false;
	if (!Fetch (max, params, param_max, true, csVector3 (0, 0, 0))) return false;
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

