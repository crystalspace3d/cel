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
#include "csutil/objreg.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "ivaria/dynamics.h"

#include "plugins/propclass/mechanics/thruster_reactionary.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (MechanicsThrusterReactionary, "pcphysics.thruster.reactionary", "pcmechthrustreactionary")

//---------------------------------------------------------------------------

// Parameters for action_initthruster
csStringID celPcMechanicsThrusterReactionary::param_object = csInvalidStringID;
csStringID celPcMechanicsThrusterReactionary::param_position = csInvalidStringID;
csStringID celPcMechanicsThrusterReactionary::param_orientation = csInvalidStringID;
csStringID celPcMechanicsThrusterReactionary::param_maxthrust = csInvalidStringID;

PropertyHolder celPcMechanicsThrusterReactionary::propinfo;


celPcMechanicsThrusterReactionary::celPcMechanicsThrusterReactionary (
	iObjectRegistry* object_reg) : scfImplementationType (this, object_reg)
{
  lastforceid = 0;
  thrust = 0;
  maxthrust = 0;

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.mechanics.reactionary.action.");
    AddAction (action_initthruster, "InitThruster");
  }

  // Parameters for action_initthruster
  if (param_object == csInvalidStringID)
  {
    param_object = pl->FetchStringID ("objectpctag");
    param_position = pl->FetchStringID ("position");
    param_orientation = pl->FetchStringID ("orientation");
    param_maxthrust = pl->FetchStringID ("maxthrust");
  }
}

celPcMechanicsThrusterReactionary::~celPcMechanicsThrusterReactionary ()
{
}

bool celPcMechanicsThrusterReactionary::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  if (idx == action_initthruster)
  {
    csString objectpctag;
    if (!Fetch (objectpctag, params, param_object)) return false;
    csRef<iPcMechanicsObject> mechobj = 0;
    mechobj = celQueryPropertyClassTagEntity<iPcMechanicsObject> (GetEntity (), objectpctag);
    assert (mechobj);
    SetMechanicsObject (mechobj);
    csVector3 position, orientation;
    if (!Fetch (position, params, param_position)) return false;
    SetPosition (position);
    if (!Fetch (orientation, params, param_orientation)) return false;
    SetOrientation (orientation);
    float maxthrust;
    if (!Fetch (maxthrust, params, param_maxthrust)) return false;
    SetMaxThrust (maxthrust);
    return true;
  }
  return false;
}

void celPcMechanicsThrusterReactionary::ThrustChange (float deltathrust)
{
  if (deltathrust <= AvailableThrust () && deltathrust >= (-thrust))
  {
    if (lastforceid != 0)
      mechanicsobject->RemoveForceTagged (lastforceid);
    thrust = thrust + deltathrust;
    if (thrust > 0)
    {
      lastforceid = mechanicsobject->AddForceTagged (
      	orientation * (thrust <= maxthrust ?
	thrust : maxthrust), true, position);
    }
    else
    {
      thrust = 0;
      lastforceid = 0;
    }
  }
}

float celPcMechanicsThrusterReactionary::AvailableThrust ()
{
  return maxthrust - thrust;
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

float celPcMechanicsThrusterReactionary::GetThrustForce (float thrust)
{
  return thrust;
}

