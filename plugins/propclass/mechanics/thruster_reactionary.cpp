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
#include "csutil/debug.h"
#include "csutil/objreg.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "ivaria/dynamics.h"

#include "plugins/propclass/mechanics/common.h"
#include "plugins/propclass/mechanics/thruster_reactionary.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (MechanicsThrusterReactionary, "pcmechthrustreactionary")

static float clipPercent (float val)
{
  if (val < 0)
    return 0;
  if (val > 1)
    return 1;
  return val;
}

//---------------------------------------------------------------------------

// Actions
csStringID celPcMechanicsThrusterReactionary::action_initthruster = csInvalidStringID;

// Parameters for action_initthruster
csStringID celPcMechanicsThrusterReactionary::param_object = csInvalidStringID;
csStringID celPcMechanicsThrusterReactionary::param_position = csInvalidStringID;
csStringID celPcMechanicsThrusterReactionary::param_orientation = csInvalidStringID;
csStringID celPcMechanicsThrusterReactionary::param_maxthrust = csInvalidStringID;


SCF_IMPLEMENT_IBASE_EXT (celPcMechanicsThrusterReactionary)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMechanicsThruster)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iCelTimerListener)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsThrusterReactionary::PcMechanicsThruster)
  SCF_IMPLEMENTS_INTERFACE (iPcMechanicsThruster)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMechanicsThrusterReactionary::CelTimerListener)
  SCF_IMPLEMENTS_INTERFACE (iCelTimerListener)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMechanicsThrusterReactionary::celPcMechanicsThrusterReactionary (
	iObjectRegistry* object_reg) : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMechanicsThruster);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiCelTimerListener);
  pl->CallbackEveryFrame (&scfiCelTimerListener, cscmdPreProcess);

  // Actions
  if (action_initthruster == csInvalidStringID)
    action_initthruster = pl->FetchStringID ("cel.action.InitThruster");

  // Parameters for action_initthruster
  if (param_object == csInvalidStringID)
    param_object = pl->FetchStringID ("cel.parameter.objectpctag");
  if (param_position == csInvalidStringID)
    param_position = pl->FetchStringID ("cel.parameter.position");
  if (param_orientation == csInvalidStringID)
    param_orientation = pl->FetchStringID ("cel.parameter.orientation");
  if (param_maxthrust == csInvalidStringID)
    param_maxthrust = pl->FetchStringID ("cel.parameter.maxthrust");
}

celPcMechanicsThrusterReactionary::~celPcMechanicsThrusterReactionary ()
{
}

#define MECHSYS_SERIAL 1

csPtr<iCelDataBuffer> celPcMechanicsThrusterReactionary::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MECHSYS_SERIAL);
  csRef<iCelPropertyClass> pc = SCF_QUERY_INTERFACE (mechanicsobject,
  	iCelPropertyClass);
  databuf->Add (pc);
  databuf->Add (position);
  databuf->Add (orientation);
  databuf->Add (maxthrust);
  databuf->Add (request.thrust);
  pc = SCF_QUERY_INTERFACE (request.group, iCelPropertyClass);
  databuf->Add (pc);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMechanicsThrusterReactionary::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MECHSYS_SERIAL)
  {
    Report (object_reg, "serialnr != MECHSYS_SERIAL.  Cannot load.");
    return false;
  }
  csRef<iCelPropertyClass> pc = databuf->GetPC ();
  mechanicsobject = SCF_QUERY_INTERFACE (pc, iPcMechanicsObject);
  databuf->GetVector3 (position);
  databuf->GetVector3 (orientation);
  maxthrust = databuf->GetFloat ();
  request.thrust = databuf->GetFloat ();
  pc = databuf->GetPC ();
  request.group = SCF_QUERY_INTERFACE (pc, iPcMechanicsThrusterGroup);
  return true;
}

bool celPcMechanicsThrusterReactionary::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_initthruster)
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
    CEL_FETCH_VECTOR3_PAR (position,params,param_position);
    if (p_position)
      SetPosition (position);
    else
      Report (object_reg, "Couldn't get position for thruster!");
    CEL_FETCH_VECTOR3_PAR (orientation,params,param_orientation);
    if (p_orientation)
    {
      fflush (stdout);
      SetOrientation (orientation);
    }
    else
    {
      Report (object_reg, "Couldn't get orientation for thruster!");
    }
    CEL_FETCH_FLOAT_PAR (maxthrust,params,param_maxthrust);
    if (p_maxthrust)
      SetMaxThrust (maxthrust);
    else
      Report (object_reg, "Couldn't get maxthrust for thruster!");
    return true;
  }
  return false;
}

void celPcMechanicsThrusterReactionary::TickEveryFrame ()
{
  if (request.thrust != 0.0)
  {
    //Thruster is active.
    mechanicsobject->AddForceFrame (orientation * ((request.thrust <= 1.0 ?
	request.thrust : 1.0) * maxthrust), true, position);
  }
}

void celPcMechanicsThrusterReactionary::ThrustRequest (
	iPcMechanicsThrusterGroup* group, float thrust)
{
  printf ("received thrust request.\n\n");
  fflush (stdout);
  if (request.thrust == 0.0 || request.group == group)
  {
    request.group = group;
    request.thrust = clipPercent (thrust);
  }
}

void celPcMechanicsThrusterReactionary::CancelThrustRequest
	(iPcMechanicsThrusterGroup* group)
{
  if (request.group = group)
  {
    request.group = 0;
    request.thrust = 0.0;
  }
}

void celPcMechanicsThrusterReactionary::SetPosition (const csVector3& pos)
{
  position = pos;
}

const csVector3& celPcMechanicsThrusterReactionary::GetPosition ()
{
  return position;
}

void celPcMechanicsThrusterReactionary::SetOrientation (
	const csVector3& orientation)
{
  this->orientation = orientation.Unit();
}

const csVector3& celPcMechanicsThrusterReactionary::GetOrientation ()
{
  return orientation;
}

void celPcMechanicsThrusterReactionary::SetMaxThrust (float maxthrust)
{
  this->maxthrust = maxthrust;
}

float celPcMechanicsThrusterReactionary::GetMaxThrust ()
{
  return maxthrust;
}

float celPcMechanicsThrusterReactionary::GetEffectiveMaxThrust ()
{
  return maxthrust;
}

