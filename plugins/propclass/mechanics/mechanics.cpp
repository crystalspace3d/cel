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
#include "csutil/debug.h"
#include "csutil/objreg.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "iengine/mesh.h"
#include "iengine/light.h"
#include "iengine/camera.h"
#include "iengine/movable.h"
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

CEL_IMPLEMENT_FACTORY(MechanicsSystem, "pcmechsys")
CEL_IMPLEMENT_FACTORY(MechanicsObject, "pcmechobject")

//---------------------------------------------------------------------------

// Actions
csStringID celPcMechanicsSystem::action_setsystem = csInvalidStringID;
csStringID celPcMechanicsSystem::action_setgravity = csInvalidStringID;

// Parameters for action_setsystem
csStringID celPcMechanicsSystem::param_dynsys = csInvalidStringID;

// Parameters for action_setgravity
csStringID celPcMechanicsSystem::param_gravity = csInvalidStringID;

celPcMechanicsSystem::celPcMechanicsSystem (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  scfiCelTimerListener = new CelTimerListener (this);
  pl->CallbackEveryFrame (scfiCelTimerListener, cscmdPreProcess);

  dynsystem_error_reported = false;
  delta = 0.01f;
  remaining_delta = 0;
  
  object_reg->Register ((iPcMechanicsSystem*)this, "iPcMechanicsSystem");
  
  if (action_setsystem == csInvalidStringID)
  {
    // Actions
    action_setsystem = pl->FetchStringID ("cel.action.SetSystem");
    action_setgravity  = pl->FetchStringID ("cel.action.SetGravity");
  
    // Parameters for action_setsystem
    param_dynsys = pl->FetchStringID ("cel.parameter.dynsys");
    // Parameters for action_setgravity
    param_gravity = pl->FetchStringID ("cel.parameter.gravity");
  }
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
  for (i = 0 ; i < forces.Length () ; i++)
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
  dynamics = CS_QUERY_REGISTRY (object_reg, iDynamics);
  if (!dynamics && !dynsystem_error_reported)
  {
    csRef<iPluginManager> plugmgr = CS_QUERY_REGISTRY (object_reg,
    	iPluginManager);
    dynamics = CS_LOAD_PLUGIN (plugmgr, "crystalspace.dynamics.ode", iDynamics);
    if (dynamics)
      object_reg->Register (dynamics, "iDynamics");
  }
  if (!dynamics)
  {
    if (!dynsystem_error_reported)
    {
      dynsystem_error_reported = true;
      Report (object_reg, "Can't find dynamic subsystem!");
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
  float et = remaining_delta + (float (elapsed_time) / 1000.0);
  while (et >= delta)
  {
    ProcessForces (delta);
    dynamics->Step (delta);
    et -= delta;
  }

  // Now we have a small remainder. We remember that in remaining_delta.
  remaining_delta = et;

  // Delete all expired forces and forces that were only
  // meant to be here for one frame.
  size_t i;
  for (i = 0 ; i < forces.Length () ; i++)
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
    dynsystem->SetGravity (csVector3 (0, -9.8f, 0));

    EnableStepFast ();
  }
  return dynsystem;
}

void celPcMechanicsSystem::DisableStepFast ()
{
  GetDynamicSystem ();
  if (!dynsystem) return;
  csRef<iODEDynamicSystemState> osys= SCF_QUERY_INTERFACE (dynsystem,
    	iODEDynamicSystemState);
  if (osys)
    osys->EnableStepFast (0);
}

void celPcMechanicsSystem::EnableStepFast ()
{
  GetDynamicSystem ();
  if (!dynsystem) return;
  csRef<iODEDynamicSystemState> osys= SCF_QUERY_INTERFACE (dynsystem,
    	iODEDynamicSystemState);
  if (osys)
    osys->EnableStepFast (1);
}

void celPcMechanicsSystem::EnableQuickStep ()
{
  GetDynamicSystem ();
  if (!dynsystem) return;
  csRef<iODEDynamicSystemState> osys= SCF_QUERY_INTERFACE (dynsystem,
    	iODEDynamicSystemState);
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

bool celPcMechanicsSystem::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_setsystem)
  {
    CEL_FETCH_STRING_PAR (dynsys,params,param_dynsys);
    if (p_dynsys)
    {
      SetDynamicSystem(dynsys);
    }
    else
    {
      Report (object_reg, "Couldn't get dynamics system name!");
      return false;
    }
  }
  if (actionId == action_setgravity)
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
      Report (object_reg, "Couldn't get gravity!");
      return false;
    }
  }
  return true;
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
    Report (object_reg, "serialnr != MECHSYS_SERIAL.  Cannot load.");
    return false;
  }

  if (databuf->GetDataCount () != 4)
  {
    Report (object_reg, "Msg does not specify the correct data. Cannot load.");
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
  for (size_t i = 0 ; i < forces.Length () ; i++)
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
  for (i = 0 ; i < forces.Length () ; i++)
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

// Actions
csStringID celPcMechanicsObject::action_initphys = csInvalidStringID;
csStringID celPcMechanicsObject::action_makestatic = csInvalidStringID;
csStringID celPcMechanicsObject::action_setsystem = csInvalidStringID;
csStringID celPcMechanicsObject::action_setmesh = csInvalidStringID;
csStringID celPcMechanicsObject::action_setcollidersphere = csInvalidStringID;
csStringID celPcMechanicsObject::action_setcollidercylinder = csInvalidStringID;
csStringID celPcMechanicsObject::action_setcolliderbox = csInvalidStringID;
csStringID celPcMechanicsObject::action_setcolliderplane = csInvalidStringID;
csStringID celPcMechanicsObject::action_setcollidermesh = csInvalidStringID;

// Parameters for action_initphys
csStringID celPcMechanicsObject::param_friction = csInvalidStringID;
csStringID celPcMechanicsObject::param_mass = csInvalidStringID;
csStringID celPcMechanicsObject::param_elasticity = csInvalidStringID;
csStringID celPcMechanicsObject::param_density = csInvalidStringID;
csStringID celPcMechanicsObject::param_softness = csInvalidStringID;
csStringID celPcMechanicsObject::param_lift = csInvalidStringID;
csStringID celPcMechanicsObject::param_drag = csInvalidStringID;

// Parameters for action_makestatic
csStringID celPcMechanicsObject::param_static = csInvalidStringID;

// Parameters for action_setsystem
csStringID celPcMechanicsObject::param_systempcent = csInvalidStringID;
csStringID celPcMechanicsObject::param_systempctag = csInvalidStringID;

// Parameters for action_setmesh
csStringID celPcMechanicsObject::param_meshpctag = csInvalidStringID;

// Parameters for action_setcollidersphere
csStringID celPcMechanicsObject::param_radius = csInvalidStringID;
csStringID celPcMechanicsObject::param_offset = csInvalidStringID;

// Parameters for action_setcollidercylinder
csStringID celPcMechanicsObject::param_length = csInvalidStringID;
//param_radius shared
csStringID celPcMechanicsObject::param_axis = csInvalidStringID;
csStringID celPcMechanicsObject::param_angle = csInvalidStringID;
//param_offset shared

// Parameters for action_setcolliderbox
csStringID celPcMechanicsObject::param_size = csInvalidStringID;
//param_axis shared
//param_angle shared
//param_offset shared

// Parameters for action_setcolliderplane
csStringID celPcMechanicsObject::param_normal = csInvalidStringID;
//param_offset shared

// Parameters for message pcdynamicbody_collision
csStringID celPcMechanicsObject::param_otherbody = csInvalidStringID;

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsObject::DynamicsCollisionCallback)
  SCF_IMPLEMENTS_INTERFACE (iDynamicsCollisionCallback)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMechanicsObject::celPcMechanicsObject (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiDynamicsCollisionCallback);
  DG_TYPE (this, "celPcMechanicsObject()");

  btype = CEL_BODY_INVALID;
  bdata = 0;

  body = 0;
  pcmesh = 0;
  mechsystem = 0;

  forceidseed = 0;

  friction = 1;
  elasticity = 0;
  softness = 0;
  density = 1;
  mass = 1;

  is_static = false;

  if (action_initphys == csInvalidStringID)
  {
    // Actions
    action_initphys = pl->FetchStringID ("cel.action.InitPhys");
    action_makestatic = pl->FetchStringID ("cel.action.MakeStatic");
    action_setsystem = pl->FetchStringID ("cel.action.SetSystem");
    action_setmesh = pl->FetchStringID ("cel.action.SetMesh");
    action_setcollidersphere = pl->FetchStringID (
    	"cel.action.SetColliderSphere");
    action_setcollidercylinder = pl->FetchStringID (
    	"cel.action.SetColliderCylinder");
    action_setcolliderbox = pl->FetchStringID ("cel.action.SetColliderBox");
    action_setcolliderplane = pl->FetchStringID ("cel.action.SetColliderPlane");
    action_setcollidermesh = pl->FetchStringID ("cel.action.SetColliderMesh");

    // Parameters for action_initphys
    param_friction = pl->FetchStringID ("cel.parameter.friction");
    param_mass = pl->FetchStringID ("cel.parameter.mass");
    param_elasticity = pl->FetchStringID ("cel.parameter.elasticity");
    param_density = pl->FetchStringID ("cel.parameter.density");
    param_softness = pl->FetchStringID ("cel.parameter.softness");
    param_lift = pl->FetchStringID ("cel.parameter.lift");
    param_drag = pl->FetchStringID ("cel.parameter.drag");

    // Parameters for action_makestatic
    param_static = pl->FetchStringID ("cel.parameter.static");

    // Parameters for action_setsystem
    param_systempcent = pl->FetchStringID ("cel.parameter.systempcent");
    param_systempctag = pl->FetchStringID ("cel.parameter.systempctag");

    // Parameters for action_setmesh
    param_meshpctag = pl->FetchStringID ("cel.parameter.meshpctag");

    // Parameters for action_setcollidersphere
    param_radius = pl->FetchStringID ("cel.parameter.radius");
    param_offset = pl->FetchStringID ("cel.parameter.offset");

    // Parameters for action_setcollidercylinder
    param_length = pl->FetchStringID ("cel.parameter.length");
    //param_radius shared
    param_axis = pl->FetchStringID ("cel.parameter.axis");
    param_angle = pl->FetchStringID ("cel.parameter.angle");
    //param_offset shared

    // Parameters for action_setcolliderbox
    param_size = pl->FetchStringID ("cel.parameter.size");
    //param_axis shared
    //param_angle shared
    //param_offset shared

    // Parameters for action_setcolliderplane
    param_normal = pl->FetchStringID ("cel.parameter.normal");
    //param_offset shared

    // Parameters for message pcdynamicbody_collision
    param_otherbody = pl->FetchStringID ("cel.parameter.otherbody");
  }

  params = new celOneParameterBlock ();
  params->SetParameterDef (param_otherbody, "otherbody");
}

celPcMechanicsObject::~celPcMechanicsObject ()
{
  delete bdata;
  if (mechsystem)
    mechsystem->ClearForces ((iPcMechanicsObject*)this);
}

#define DYNBODY_SERIAL 1

csPtr<iCelDataBuffer> celPcMechanicsObject::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (DYNBODY_SERIAL);
  csRef<iCelPropertyClass> pc;
  if (pcmesh)
  {
    pc = SCF_QUERY_INTERFACE (pcmesh, iCelPropertyClass);
    databuf->Add (pc);
  }
  else
    databuf->Add ((iCelPropertyClass*) 0);
  GetMechSystem ();
  if (mechsystem)
  {
    pc = SCF_QUERY_INTERFACE (mechsystem, iCelPropertyClass);
    databuf->Add (pc);
  }
  else
    databuf->Add ((iCelPropertyClass*) 0);
  databuf->Add (CS_STATIC_CAST(int32,btype));
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
    Report (object_reg, "serialnr != DYNBODY_SERIAL.  Cannot load.");
    return false;
  }
  csRef<iPcMesh> pcm = SCF_QUERY_INTERFACE (databuf->GetPC (), iPcMesh);
  SetMesh (pcm);
  csRef<iPcMechanicsSystem> pcms = SCF_QUERY_INTERFACE (databuf->GetPC (),
  	iPcMechanicsSystem);
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

bool celPcMechanicsObject::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_initphys)
  {
    CEL_FETCH_FLOAT_PAR (friction,params,param_friction);
    if (p_friction)
      SetFriction (friction);
    else
      Report (object_reg, "Couldn't get friction!");
    CEL_FETCH_FLOAT_PAR (mass,params,param_mass);
    if (p_mass)
      SetMass (mass);
    else
      Report (object_reg, "Couldn't get mass!");
    CEL_FETCH_FLOAT_PAR (elasticity,params,param_elasticity);
    if (p_elasticity)
      SetElasticity (elasticity);
    else
      Report (object_reg, "Couldn't get elasticity!");
    CEL_FETCH_FLOAT_PAR (density,params,param_density);
    if (p_density)
      SetDensity (density);
    else
      Report (object_reg, "Couldn't get density!");
    CEL_FETCH_FLOAT_PAR (softness,params,param_softness);
    if (p_softness)
      SetSoftness (softness);
    else
      Report (object_reg, "Couldn't get softness!");
    CEL_FETCH_VECTOR3_PAR (lift,params,param_lift);
    if (p_lift)
      SetLift (lift);
    else
      Report (object_reg, "Couldn't get lift!");
    CEL_FETCH_FLOAT_PAR (drag,params,param_drag);
    if (p_drag)
      SetDrag (drag);
    else
      Report (object_reg, "Couldn't get drag!");
    return true;
  }
  else if (actionId == action_makestatic)
  {
    CEL_FETCH_BOOL_PAR (makestatic,params,param_static);
    if (!p_makestatic)
    {
      Report (object_reg, "Couldn't get static!");
      return false;
    }
    MakeStatic (makestatic);
    return true;
  }
  else if (actionId == action_setsystem)
  {
    CEL_FETCH_STRING_PAR (syspcent,params,param_systempcent);
    if (!p_syspcent)
    {
      Report (object_reg, "Couldn't get mechanics system entity!");
      return false;
    }
    CEL_FETCH_STRING_PAR (syspctag,params,param_systempctag);
    if (!p_syspctag)
    {
      Report (object_reg, "Couldn't get mechanics system tag!");
      return false;
    }
    csRef<iCelEntity> sysent = pl->FindEntity (syspcent);
    csRef<iPcMechanicsSystem> mechsyss = 0;
    mechsyss = CEL_QUERY_PROPCLASS_TAG_ENT(sysent,iPcMechanicsSystem,syspctag);
    assert (mechsyss);
    SetMechanicsSystem (mechsyss);
    return true;
  }
  else if (actionId == action_setmesh)
  {
    CEL_FETCH_STRING_PAR (meshpctag,params,param_meshpctag);
    if (!p_meshpctag)
    {
      Report (object_reg, "Couldn't get mesh tag!");
      return false;
    }
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT(GetEntity (),iPcMesh,
		    meshpctag);
    SetMesh (pcmesh);
    return true;
  }
  else if (actionId == action_setcollidersphere)
  {
    CEL_FETCH_FLOAT_PAR (radius,params,param_radius);
    if (!p_radius)
    {
      Report (object_reg, "Couldn't get radius!");
      return false;
    }
    CEL_FETCH_VECTOR3_PAR (offset,params,param_offset);
    if (!p_offset)
    {
      Report (object_reg, "Couldn't get offset!");
      return false;
    }
    AttachColliderSphere (radius, offset);
    return true;
  }
  else if (actionId == action_setcollidercylinder)
  {
    CEL_FETCH_FLOAT_PAR (length,params,param_length);
    if (!p_length)
    {
      Report (object_reg, "Couldn't get length!");
      return false;
    }
    CEL_FETCH_FLOAT_PAR (radius,params,param_radius);
    if (!p_radius)
    {
      Report (object_reg, "Couldn't get radius!");
      return false;
    }
    CEL_FETCH_VECTOR3_PAR (axis,params,param_axis);
    if (!p_axis)
    {
      Report (object_reg, "Couldn't get axis!");
      return false;
    }
    CEL_FETCH_FLOAT_PAR (angle,params,param_angle);
    if (!p_angle)
    {
      Report (object_reg, "Couldn't get angle!");
      return false;
    }
    CEL_FETCH_VECTOR3_PAR (offset,params,param_offset);
    if (!p_offset)
    {
      Report (object_reg, "Couldn't get offset!");
      return false;
    }
    AttachColliderCylinder (length, radius, csOrthoTransform (csMatrix3
	(axis.x, axis.y, axis.z, angle), offset));
    return true;
  }
  else if (actionId == action_setcolliderbox)
  {
    CEL_FETCH_VECTOR3_PAR (size,params,param_size);
    if (!p_size)
    {
      Report (object_reg, "Couldn't get size!");
      return false;
    }
    CEL_FETCH_VECTOR3_PAR (axis,params,param_axis);
    if (!p_axis)
    {
      Report (object_reg, "Couldn't get axis!");
      return false;
    }
    CEL_FETCH_FLOAT_PAR (angle,params,param_angle);
    if (!p_angle)
    {
      Report (object_reg, "Couldn't get angle!");
      return false;
    }
    CEL_FETCH_VECTOR3_PAR (offset,params,param_offset);
    if (!p_offset)
    {
      Report (object_reg, "Couldn't get offset!");
      return false;
    }
    AttachColliderBox (size, csOrthoTransform (csMatrix3 (axis.x, axis.y,
	axis.z, angle), offset));
    return true;
  }
  else if (actionId == action_setcolliderplane)
  {
    CEL_FETCH_VECTOR3_PAR (normal,params,param_normal);
    if (!p_normal)
    {
      Report (object_reg, "Couldn't get normal!");
      return false;
    }
    CEL_FETCH_FLOAT_PAR (offset,params,param_offset);
    if (!p_offset)
    {
      Report (object_reg, "Couldn't get offset!");
      return false;
    }
    AttachColliderPlane (csPlane3 (normal, offset));
    return true;
  }
  else if (actionId == action_setcollidermesh)
  {
    AttachColliderMesh ();
    return true;
  }
  return false;
}

void celPcMechanicsObject::Collision (iRigidBody *thisbody,
	iRigidBody *otherbody)
{
  iCelBehaviour* behaviour = entity->GetBehaviour ();
  if (!behaviour) return;
  celData ret;
  //@@@ Find the other body's iPcMechanicsObject.
  /*csRef<iObject> obj = otherbody->QueryObject ();
  if (obj != 0)
  {
    csRef<iCelEntity> ent = pl->FindAttachedEntity (obj);
    if (ent == 0)
    {
      Report (object_reg, "No entity found using given iObject!\n");
    }
    //params->GetParameter (0).Set (othermechobjtag);
  }
  else
  {
    Report (object_reg, "No iObject found!\n");
  }*/
  behaviour->SendMessage ("pcdynamicbody_collision", this, ret, params);
}

void celPcMechanicsObject::GetMechSystem ()
{
  if (!mechsystem)
  {
    mechsystem = CS_QUERY_REGISTRY (object_reg, iPcMechanicsSystem);
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
        body->SetCollisionCallback (&scfiDynamicsCollisionCallback);
      }
    }
    else
    {
      Report (object_reg, "No mechsys!!!!!!!!!");
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

void celPcMechanicsObject::SetDrag (float drag) {
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
  if (stat == is_static) return;
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
  mechsystem->AddForceFrame ((iPcMechanicsObject*)this, force, relative, position);
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

