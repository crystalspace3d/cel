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
#include "iengine/movable.h"
#include "ivaria/reporter.h"
#include "ivaria/dynamics.h"

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

SCF_IMPLEMENT_IBASE_EXT (celPcMechanicsSystem)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMechanicsSystem)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iCelTimerListener)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsSystem::PcMechanicsSystem)
  SCF_IMPLEMENTS_INTERFACE (iPcMechanicsSystem)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsSystem::CelTimerListener)
  SCF_IMPLEMENTS_INTERFACE (iCelTimerListener)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMechanicsSystem::celPcMechanicsSystem (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMechanicsSystem);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiCelTimerListener);
  delta = 0.01f;
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  pl->CallbackEveryFrame (&scfiCelTimerListener, cscmdPreProcess);

  dynamics = 0;
  dynsystem = 0;

  object_reg->Register (&scfiPcMechanicsSystem, "iPcMechanicsSystem");
}

celPcMechanicsSystem::~celPcMechanicsSystem ()
{
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

void celPcMechanicsSystem::TickEveryFrame ()
{
  GetDynamicSystem ();
  csTicks elapsed_time = vc->GetElapsedTicks ();
  float et = float (elapsed_time) / 1000.0;
  while (et >= delta)
  {
    ProcessForces (delta);
    dynamics->Step (delta);
    et -= delta;
  }
  // Do the remainder.
  if (et > SMALL_EPSILON)
  {
    ProcessForces (et);
    dynamics->Step (et);
  }

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

void celPcMechanicsSystem::SetDynamicSystem (iDynamicSystem* dynsys)
{
  dynsystem = dynsys;
  dynsystem->SetGravity (csVector3 (0, -9.8f, 0));
  dynamics = CS_QUERY_REGISTRY (object_reg, iDynamics);
  if (!dynamics)
  {
    csRef<iPluginManager> plugmgr = CS_QUERY_REGISTRY (object_reg, iPluginManager);
    dynamics = CS_LOAD_PLUGIN (plugmgr, "crystalspace.dynamics.ode",
	iDynamics);
    if (dynamics)
    {
      object_reg->Register (dynamics, "iDynamics");
    }
  }
  if (!dynamics)
  {
    Report (object_reg, "Can't find dynamic subsystem!");
    return;
  }
}

iDynamicSystem* celPcMechanicsSystem::GetDynamicSystem ()
{
  if (!dynsystem)
  {
    dynamics = CS_QUERY_REGISTRY (object_reg, iDynamics);
    if (!dynamics)
    {
      csRef<iPluginManager> plugmgr = CS_QUERY_REGISTRY (object_reg,
      	iPluginManager);
      dynamics = CS_LOAD_PLUGIN (plugmgr, "crystalspace.dynamics.ode",
      	iDynamics);
      if (dynamics)
      {
        object_reg->Register (dynamics, "iDynamics");
      }
    }
    if (!dynamics)
    {
      Report (object_reg, "Can't find dynamic subsystem!");
      return 0;
    }
    dynsystem = dynamics->CreateSystem ();
    dynsystem->SetGravity (csVector3 (0, -9.8f, 0));
  }
  return dynsystem;
}

void celPcMechanicsSystem::SetGravity (const csVector3& grav)
{
  dynsystem->SetGravity (grav);
}

const csVector3 celPcMechanicsSystem::GetGravity ()
{
  return dynsystem->GetGravity ();
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
  if (grpptr != NULL)
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

void celPcMechanicsSystem::RemoveBodyFromGroup (iRigidBody* body, const char* group)
{
  iBodyGroup** grpptr = groups.GetElementPointer (group);
  if (grpptr != NULL && (*grpptr)->BodyInGroup (body))
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

SCF_IMPLEMENT_IBASE_EXT (celPcMechanicsObject)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMechanicsObject)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsObject::PcMechanicsObject)
  SCF_IMPLEMENTS_INTERFACE (iPcMechanicsObject)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsObject::DynamicsCollisionCallback)
  SCF_IMPLEMENTS_INTERFACE (iDynamicsCollisionCallback)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMechanicsObject::celPcMechanicsObject (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMechanicsObject);
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

  // Actions
  if (action_initphys == csInvalidStringID)
    action_initphys = pl->FetchStringID ("cel.action.InitPhys");
  if (action_makestatic == csInvalidStringID)
    action_makestatic = pl->FetchStringID ("cel.action.MakeStatic");
  if (action_setsystem == csInvalidStringID)
    action_setsystem = pl->FetchStringID ("cel.action.SetSystem");
  if (action_setmesh == csInvalidStringID)
    action_setmesh = pl->FetchStringID ("cel.action.SetMesh");
  if (action_setcollidersphere == csInvalidStringID)
    action_setcollidersphere = pl->FetchStringID ("cel.action.SetColliderSphere");
  if (action_setcollidercylinder == csInvalidStringID)
    action_setcollidercylinder = pl->FetchStringID ("cel.action.SetColliderCylinder");
  if (action_setcolliderbox == csInvalidStringID)
    action_setcolliderbox = pl->FetchStringID ("cel.action.SetColliderBox");
  if (action_setcolliderplane == csInvalidStringID)
    action_setcolliderplane = pl->FetchStringID ("cel.action.SetColliderPlane");
  if (action_setcollidermesh == csInvalidStringID)
    action_setcollidermesh = pl->FetchStringID ("cel.action.SetColliderMesh");

  // Parameters for action_initphys
  if (param_friction == csInvalidStringID)
    param_friction = pl->FetchStringID ("cel.parameter.friction");
  if (param_mass == csInvalidStringID)
    param_mass = pl->FetchStringID ("cel.parameter.mass");
  if (param_elasticity == csInvalidStringID)
    param_elasticity = pl->FetchStringID ("cel.parameter.elasticity");
  if (param_density == csInvalidStringID)
    param_density = pl->FetchStringID ("cel.parameter.density");
  if (param_softness == csInvalidStringID)
    param_softness = pl->FetchStringID ("cel.parameter.softness");
  if (param_lift == csInvalidStringID)
    param_lift = pl->FetchStringID ("cel.parameter.lift");
  if (param_drag == csInvalidStringID)
    param_drag = pl->FetchStringID ("cel.parameter.drag");

  // Parameters for action_makestatic
  if (param_static == csInvalidStringID)
    param_static = pl->FetchStringID ("cel.parameter.static");

  // Parameters for action_setsystem
  if (param_systempcent == csInvalidStringID)
    param_systempcent = pl->FetchStringID ("cel.parameter.systempcent");
  if (param_systempctag == csInvalidStringID)
    param_systempctag = pl->FetchStringID ("cel.parameter.systempctag");

  // Parameters for action_setmesh
  if (param_meshpctag == csInvalidStringID)
    param_meshpctag = pl->FetchStringID ("cel.parameter.meshpctag");

  // Parameters for action_setcollidersphere
  if (param_radius == csInvalidStringID)
    param_radius = pl->FetchStringID ("cel.parameter.radius");
  if (param_offset == csInvalidStringID)
    param_offset = pl->FetchStringID ("cel.parameter.offset");

  // Parameters for action_setcollidercylinder
  if (param_length == csInvalidStringID)
    param_length = pl->FetchStringID ("cel.parameter.length");
  //param_radius shared
  if (param_axis == csInvalidStringID)
    param_axis = pl->FetchStringID ("cel.parameter.axis");
  if (param_angle == csInvalidStringID)
    param_angle = pl->FetchStringID ("cel.parameter.angle");
  //param_offset shared

  // Parameters for action_setcolliderbox
  if (param_size == csInvalidStringID)
    param_size = pl->FetchStringID ("cel.parameter.size");
  //param_axis shared
  //param_angle shared
  //param_offset shared

  // Parameters for action_setcolliderplane
  if (param_normal == csInvalidStringID)
    param_normal = pl->FetchStringID ("cel.parameter.normal");
  //param_offset shared

  // Parameters for message pcdynamicbody_collision
  if (param_otherbody  == csInvalidStringID)
    param_otherbody = pl->FetchStringID ("cel.parameter.otherbody");

  params = new celOneParameterBlock ();
  params->SetParameterDef (param_otherbody, "otherbody");
}

celPcMechanicsObject::~celPcMechanicsObject ()
{
  delete bdata;
  if (mechsystem)
    mechsystem->ClearForces (&scfiPcMechanicsObject);
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
  csRef<iPcMechanicsSystem> pcms = SCF_QUERY_INTERFACE (databuf->GetPC (), iPcMechanicsSystem);
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
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT(GetEntity (),iPcMesh);
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
  if (obj != NULL)
  {
    csRef<iCelEntity> ent = pl->FindAttachedEntity (obj);
    if (ent == NULL)
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
      assert (dynsys);
      body = dynsys->CreateBody ();
      body->SetCollisionCallback (&scfiDynamicsCollisionCallback);
    }
    else
    {
      Report (object_reg, "No mechsys!!!!!!!!!");
      return NULL;
    }
  }
  return body;
}

iPcMesh* celPcMechanicsObject::GetMesh ()
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  return pcmesh;
}

void celPcMechanicsObject::SetFriction (float friction) {
  celPcMechanicsObject::friction = friction;
}

void celPcMechanicsObject::SetMass (float mass) {
  celPcMechanicsObject::mass = mass;
  if (GetBody () != NULL)
    body->SetProperties (mass, body->GetCenter (), csMatrix3 () * mass);
}

void celPcMechanicsObject::SetElasticity (float elasticity) {
  celPcMechanicsObject::elasticity = elasticity;
}

void celPcMechanicsObject::SetDensity (float density) {
  celPcMechanicsObject::density = density;
}

void celPcMechanicsObject::SetSoftness (float softness) {
  celPcMechanicsObject::softness = softness;
}

void celPcMechanicsObject::SetLift (const csVector3& lift) {
  celPcMechanicsObject::lift = lift;
}

void celPcMechanicsObject::SetDrag (float drag) {
  celPcMechanicsObject::drag = drag;
}

void celPcMechanicsObject::SetLinearVelocity (const csVector3& vel)
{
  GetBody ();
  body->SetLinearVelocity (vel);
}

const csVector3 celPcMechanicsObject::GetLinearVelocity ()
{
  return body->GetLinearVelocity ();
}

void celPcMechanicsObject::SetAngularVelocity (const csVector3& vel)
{
  GetBody ();
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
  GetBody ();
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

void celPcMechanicsObject::AttachColliderSphere (float radius,
	const csVector3& offset)
{
  GetBody ();
  GetMesh ();
  body->AttachColliderSphere (radius, offset, friction, density,
  	elasticity, softness);
  body->AdjustTotalMass (mass);
  if (pcmesh)
  {
    body->SetTransform (pcmesh->GetMesh ()->GetMovable ()->GetFullTransform ());
    body->AttachMesh (pcmesh->GetMesh ());
  }

  delete bdata;
  bdata = new sphere_data (radius, offset);
  btype = CEL_BODY_SPHERE;
}

void celPcMechanicsObject::AttachColliderCylinder (float length, float radius,
	const csOrthoTransform& trans)
{
  GetBody ();
  GetMesh ();
  body->AttachColliderCylinder (length, radius, trans, friction, density,
  	elasticity, softness);
  body->AdjustTotalMass (mass);
  if (pcmesh)
  {
    body->SetTransform (pcmesh->GetMesh ()->GetMovable ()->GetFullTransform ());
    body->AttachMesh (pcmesh->GetMesh ());
  }

  delete bdata;
  bdata = new cylinder_data (length, radius, trans);
  btype = CEL_BODY_CYLINDER;
}

void celPcMechanicsObject::AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans)
{
  GetBody ();
  GetMesh ();
  body->AttachColliderBox (size, trans, friction, density, elasticity,
  	softness);
  body->AdjustTotalMass (mass);
  if (pcmesh)
  {
    CS_ASSERT (pcmesh->GetMesh () != 0);
    body->SetTransform (pcmesh->GetMesh ()->GetMovable ()->GetFullTransform ());
    body->AttachMesh (pcmesh->GetMesh ());
  }

  delete bdata;
  bdata = new box_data (size, trans);
  btype = CEL_BODY_BOX;
}

void celPcMechanicsObject::AttachColliderPlane (const csPlane3& plane)
{
  GetBody ();
  GetMesh ();
  body->AttachColliderPlane (plane, friction, density, elasticity,
  	softness);
  body->AdjustTotalMass (mass);
  if (pcmesh)
  {
    body->SetTransform (pcmesh->GetMesh ()->GetMovable ()->GetFullTransform ());
    body->AttachMesh (pcmesh->GetMesh ());
  }

  delete bdata;
  bdata = new plane_data (plane);
  btype = CEL_BODY_PLANE;
}

void celPcMechanicsObject::AttachColliderMesh ()
{
  GetBody ();
  GetMesh ();
  iMeshWrapper* mesh = pcmesh->GetMesh ();
  body->AttachColliderMesh (mesh, mesh->GetMovable ()->GetFullTransform (),
  	friction, density, elasticity, softness);
  body->AdjustTotalMass (mass);
  body->SetTransform (mesh->GetMovable ()->GetFullTransform ());
  body->AttachMesh (mesh);

  delete bdata;
  bdata = 0;
  btype = CEL_BODY_MESH;
}

void celPcMechanicsObject::AddForceTagged (const csVector3& force, bool relative,
	const csVector3& position, uint32& forceid)
{
  forceid = forceidseed++;
  mechsystem->AddForceTagged (&scfiPcMechanicsObject, force, relative,
	position, forceid);
}

void celPcMechanicsObject::RemoveForceTagged (uint32 forceid)
{
  mechsystem->RemoveForceTagged (&scfiPcMechanicsObject, forceid);
}

void celPcMechanicsObject::AddForceOnce (const csVector3& force, bool relative,
	const csVector3& position)
{
  GetBody ();
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
  mechsystem->AddForceDuration (&scfiPcMechanicsObject, force, relative,
	position, seconds);
}

void celPcMechanicsObject::AddForceFrame (const csVector3& force,
	bool relative, const csVector3& position)
{
  mechsystem->AddForceFrame (&scfiPcMechanicsObject, force, relative, position);
}

void celPcMechanicsObject::ClearForces ()
{
  mechsystem->ClearForces (&scfiPcMechanicsObject);
}

void celPcMechanicsObject::AddToGroup (const char* group)
{
  mechsystem->AddBodyToGroup (GetBody (), group);
}

void celPcMechanicsObject::RemoveFromGroup (const char* group)
{
  mechsystem->RemoveBodyFromGroup (GetBody (), group);
}

iJoint* celPcMechanicsObject::CreateJoint (iPcMechanicsObject* other)
{
  return mechsystem->CreateJoint (GetBody (), other->GetBody ());
}

