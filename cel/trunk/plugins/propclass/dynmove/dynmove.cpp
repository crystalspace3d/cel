/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein

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
#include "qsqrt.h"
#include "iutil/plugin.h"
#include "iutil/virtclk.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "csutil/debug.h"
#include "csutil/objreg.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "ivaria/reporter.h"
#include "ivaria/dynamics.h"

#include "plugins/propclass/dynmove/dynmove.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/databhlp.h"
#include "propclass/mesh.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY(DynamicSystem, "pcdynsys")
CEL_IMPLEMENT_FACTORY(DynamicBody, "pcdynbody")

void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistance",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

void Notify (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_NOTIFY, "cel.persistance",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcDynamicSystem)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcDynamicSystem)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcDynamicSystem::PcDynamicSystem)
  SCF_IMPLEMENTS_INTERFACE (iPcDynamicSystem)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcDynamicSystem::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcDynamicSystem::celPcDynamicSystem (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcDynamicSystem);
  DG_TYPE (this, "celPcDynamicSystem()");
  delta = 0.01;
  scfiEventHandler = new EventHandler (this);
  csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
  CS_ASSERT (q != 0);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
}

celPcDynamicSystem::~celPcDynamicSystem ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    if (q != 0)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
}

bool celPcDynamicSystem::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdPreProcess)
  {
    GetDynamicSystem ();
    csTicks elapsed_time = vc->GetElapsedTicks ();
    float et = float (elapsed_time) / 1000.0;
    for (float dt = 0 ; dt < et ; dt += delta)
    {
      dynamics->Step (delta / 10.0);
    }
  }
  return false;
}

iDynamicSystem* celPcDynamicSystem::GetDynamicSystem ()
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
    dynsystem->SetGravity (csVector3 (0, -9.8, 0));
  }
  return dynsystem;
}

#define DYNSYS_SERIAL 1

csPtr<iCelDataBuffer> celPcDynamicSystem::Save ()
{
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (DYNSYS_SERIAL));
  databuf->SetDataCount (4);
  int j = 0;
  databuf->GetData (j++)->Set (dynsystem->GetGravity ());
  databuf->GetData (j++)->Set (dynsystem->GetLinearDampener ());
  databuf->GetData (j++)->Set (dynsystem->GetRollingDampener ());
  databuf->GetData (j++)->Set (delta);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcDynamicSystem::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != DYNSYS_SERIAL)
  {
    Report (object_reg, "serialnr != DYNSYS_SERIAL.  Cannot load.");
    return false;
  }

  if (databuf->GetDataCount () != 4)
  {
    Report (object_reg, "Msg does not specify the correct data. Cannot load.");
    return false;
  }

  celDataBufHelper db (databuf);
  csVector3 gravity;
  float lineardampener;
  float rollingdampener;
  db.Get (gravity);
  db.Get (lineardampener);
  db.Get (rollingdampener);
  db.Get (delta);

  if (!GetDynamicSystem ())
  {
    return false;
  }

  dynsystem->SetGravity (gravity);
  dynsystem->SetLinearDampener (lineardampener);
  dynsystem->SetRollingDampener (rollingdampener);

  return true;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcDynamicBody)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcDynamicBody)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcDynamicBody::PcDynamicBody)
  SCF_IMPLEMENTS_INTERFACE (iPcDynamicBody)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcDynamicBody::celPcDynamicBody (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcDynamicBody);
  DG_TYPE (this, "celPcDynamicBody()");

  btype = CEL_BODY_INVALID;
  bdata = 0;

  friction = 1;
  elasticity = 0;
  softness = 0;
  density = 1;
  mass = 1;

  is_static = false;
}

celPcDynamicBody::~celPcDynamicBody ()
{
  delete bdata;
}

#define DYNBODY_SERIAL 1

csPtr<iCelDataBuffer> celPcDynamicBody::Save ()
{
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (DYNBODY_SERIAL));
  int body_info_cnt = 0;
  switch (btype)
  {
    case CEL_BODY_SPHERE: body_info_cnt = 2; break;
    case CEL_BODY_BOX: body_info_cnt = 5; break;
    case CEL_BODY_CYLINDER: body_info_cnt = 2; break;
    case CEL_BODY_PLANE: body_info_cnt = 4; break;
    case CEL_BODY_MESH: body_info_cnt = 0; break;
  }
  databuf->SetDataCount (2+1+body_info_cnt+5+1);
  csRef<iCelPropertyClass> pc;
  int j = 0;
  if (pcmesh) pc = SCF_QUERY_INTERFACE (pcmesh, iCelPropertyClass);
  databuf->GetData (j++)->Set (pc);
  if (dynsystem) pc = SCF_QUERY_INTERFACE (dynsystem, iCelPropertyClass);
  databuf->GetData (j++)->Set (pc);
  databuf->GetData (j++)->Set (btype);
  switch (btype)
  {
    case CEL_BODY_SPHERE:
      {
        sphere_data* sd = (sphere_data*)bdata;
        databuf->GetData (j++)->Set (sd->radius);
        databuf->GetData (j++)->Set (sd->offset);
      }
      break;
    case CEL_BODY_BOX:
      {
        box_data* bd = (box_data*)bdata;
        databuf->GetData (j++)->Set (bd->size);
        const csMatrix3& m = bd->transform.GetO2T ();
        databuf->GetData (j++)->Set (m.Row1 ());
        databuf->GetData (j++)->Set (m.Row2 ());
        databuf->GetData (j++)->Set (m.Row3 ());
        databuf->GetData (j++)->Set (bd->transform.GetO2TTranslation ());
      }
      break;
    case CEL_BODY_CYLINDER:
      {
        cylinder_data* cd = (cylinder_data*)bdata;
        databuf->GetData (j++)->Set (cd->length);
        databuf->GetData (j++)->Set (cd->radius);
        const csMatrix3& m = cd->transform.GetO2T ();
        databuf->GetData (j++)->Set (m.Row1 ());
        databuf->GetData (j++)->Set (m.Row2 ());
        databuf->GetData (j++)->Set (m.Row3 ());
        databuf->GetData (j++)->Set (cd->transform.GetO2TTranslation ());
      }
      break;
    case CEL_BODY_PLANE:
      {
        plane_data* pd = (plane_data*)bdata;
        databuf->GetData (j++)->Set (pd->plane.A ());
        databuf->GetData (j++)->Set (pd->plane.B ());
        databuf->GetData (j++)->Set (pd->plane.C ());
        databuf->GetData (j++)->Set (pd->plane.D ());
      }
      break;
    case CEL_BODY_MESH:
      break;
  }
  databuf->GetData (j++)->Set (friction);
  databuf->GetData (j++)->Set (elasticity);
  databuf->GetData (j++)->Set (softness);
  databuf->GetData (j++)->Set (density);
  databuf->GetData (j++)->Set (mass);
  databuf->GetData (j++)->Set (is_static);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcDynamicBody::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != DYNBODY_SERIAL)
  {
    Report (object_reg, "serialnr != DYNBODY_SERIAL.  Cannot load.");
    return false;
  }
  if (databuf->GetDataCount () < 9)
  {
    Report (object_reg, "Msg does not specify the correct data.  Cannot load.");
    return false;
  }
// @@@ TODO
#if 0
  celData* cd;
  collider_wrap = 0;
  no_collider = false;
  cd = databuf->GetData (0);
  if (!cd)
  {
    Report (object_reg,"iPcMesh prop class not specified.  Cannot load.");
    return false;
  }
  csRef<iPcMesh> pcm;
  if (cd->value.pc)
    pcm = SCF_QUERY_INTERFACE (cd->value.pc, iPcMesh);
  SetMesh (pcm);
#endif
  return true;
}

iRigidBody* celPcDynamicBody::GetBody ()
{
  if (!body)
  {
    body = dynsystem->GetDynamicSystem ()->CreateBody ();
  }
  return body;
}

iPcMesh* celPcDynamicBody::GetMesh ()
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  return pcmesh;
}

void celPcDynamicBody::MakeStatic (bool stat)
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
    // Restore move callback???@@@
    body->MakeDynamic ();
  }
}

void celPcDynamicBody::AttachColliderSphere (float radius,
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

void celPcDynamicBody::AttachColliderCylinder (float length, float radius,
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

void celPcDynamicBody::AttachColliderBox (const csVector3& size,
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

void celPcDynamicBody::AttachColliderPlane (const csPlane3& plane)
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

void celPcDynamicBody::AttachColliderMesh ()
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

