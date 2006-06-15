/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein

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
#include "iutil/objreg.h"
#include "csutil/debug.h"
#include "plugins/propclass/projectile/projectilefact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"
#include "iengine/movable.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Projectile, "pcprojectile")

//---------------------------------------------------------------------------

csStringID celPcProjectile::id_direction = csInvalidStringID;
csStringID celPcProjectile::id_speed = csInvalidStringID;
csStringID celPcProjectile::id_maxdist = csInvalidStringID;
csStringID celPcProjectile::id_maxhits = csInvalidStringID;
csStringID celPcProjectile::id_entity = csInvalidStringID;
csStringID celPcProjectile::id_intersection = csInvalidStringID;
csStringID celPcProjectile::action_start = csInvalidStringID;
csStringID celPcProjectile::action_interrupt = csInvalidStringID;

celPcProjectile::celPcProjectile (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  if (action_start == csInvalidStringID)
  {
    action_start = pl->FetchStringID ("cel.action.Start");
    action_interrupt = pl->FetchStringID ("cel.action.Interrupt");
    id_direction = pl->FetchStringID ("cel.parameter.direction");
    id_speed = pl->FetchStringID ("cel.parameter.speed");
    id_maxdist = pl->FetchStringID ("cel.parameter.maxdist");
    id_maxhits = pl->FetchStringID ("cel.parameter.maxhits");
    id_entity = pl->FetchStringID ("cel.parameter.entity");
    id_intersection = pl->FetchStringID ("cel.parameter.intersection");
  }
  params.AttachNew (new celVariableParameterBlock ());
  params->SetParameterDef (0, id_entity, "entity");
  params->SetParameterDef (1, id_intersection, "intersection");

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;

  propdata[propid_moving] = 0;		// Handled in this class.
  is_moving = false;

  vc = csQueryRegistry<iVirtualClock> (object_reg);
}

celPcProjectile::~celPcProjectile ()
{
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
}

Property* celPcProjectile::properties = 0;
size_t celPcProjectile::propertycount = 0;

void celPcProjectile::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    propertycount = 1;
    properties = new Property[propertycount];

    properties[propid_moving].id = pl->FetchStringID (
    	"cel.property.moving");
    properties[propid_moving].datatype = CEL_DATA_BOOL;
    properties[propid_moving].readonly = true;
    properties[propid_moving].desc = "Moving.";
  }
}

bool celPcProjectile::GetPropertyBool (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_moving].id)
  {
    return is_moving;
  }
  else
  {
    return celPcCommon::GetPropertyBool (propertyId);
  }
}

#define PROJECTILE_SERIAL 1

csPtr<iCelDataBuffer> celPcProjectile::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (PROJECTILE_SERIAL);
  // @@@
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcProjectile::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != PROJECTILE_SERIAL) return false;
  // @@@
  return true;
}

void celPcProjectile::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

void celPcProjectile::SendMessage (const char* msg)
{
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage (msg, this, ret, 0);
  }
}

void celPcProjectile::SendMessage (const char* msg, iCelEntity* hitent,
	const csVector3& isect)
{
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    params->GetParameter (0).Set (hitent);
    params->GetParameter (1).Set (isect);
    bh->SendMessage (msg, this, ret, params);
  }
}

bool celPcProjectile::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_start)
  {
    CEL_FETCH_VECTOR3_PAR (direction,params,id_direction);
    if (!p_direction) return false;	// @@@ Error?
    CEL_FETCH_FLOAT_PAR (speed,params,id_speed);
    if (!p_speed) speed = 1.0f;
    CEL_FETCH_FLOAT_PAR (maxdist,params,id_maxdist);
    if (!p_maxdist) maxdist = 1000000000.0f;
    CEL_FETCH_LONG_PAR (maxhits,params,id_maxhits);
    if (!p_maxhits) maxhits = 1;
    Start (direction, speed, maxdist, maxhits);
    return true;
  }
  else if (actionId == action_interrupt)
  {
    Interrupt ();
    return true;
  }
  return false;
}

bool celPcProjectile::Start (const csVector3& direction,
  	float speed, float maxdist, int maxhits)
{
  if (is_moving) return false;

  FindSiblingPropertyClasses ();
  if (!pcmesh) return false;

  // @@@ Support for anchored/hierarchical objects?
  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  start = movable->GetPosition ();
  celPcProjectile::direction = direction.Unit ();
  celPcProjectile::speed = speed;
  celPcProjectile::maxdist = maxdist;
  celPcProjectile::maxhits = maxhits;
  curhits = 0;
  start_time = vc->GetCurrentTicks ();

  movable->GetTransform ().LookAt (celPcProjectile::direction,
      csVector3 (0, 1, 0));
  movable->UpdateMove ();

  is_moving = true;
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);

  return true;
}

void celPcProjectile::TickEveryFrame ()
{
  FindSiblingPropertyClasses ();
  if (!pcmesh)
  {
    Interrupt ();
    return;
  }

  csTicks now = vc->GetCurrentTicks ();
  float dist = speed * float (now-start_time) / 1000.0f;
  bool stop = false;
  if (dist > maxdist) { dist = maxdist; stop = true; }
  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  csVector3 newpos = start + dist * direction;
  const csVector3& curpos = movable->GetPosition ();
  iSector* cursector = movable->GetSectors ()->Get (0);
  
  // Reference to prevent premature entity removal.
  csRef<iCelEntity> keepref;

  csSectorHitBeamResult rc = cursector->HitBeamPortals (curpos, newpos);
  if (rc.mesh)
  {
    curhits++;
    iCelEntity* hitent = pl->FindAttachedEntity (rc.mesh->QueryObject ());
    keepref = entity;
    SendMessage ("pcprojectile_hit", hitent, rc.isect);
    if (curhits >= maxhits)
    {
      Interrupt ();
      return;
    }
  }
  if (cursector != rc.final_sector)
  {
    printf ("Debug: change sector!\n"); fflush (stdout);
    movable->SetSector (rc.final_sector);
  }
  movable->GetTransform ().SetOrigin (newpos);
  movable->UpdateMove ();

  if (stop) Interrupt ();
}

void celPcProjectile::Interrupt ()
{
  if (is_moving)
  {
    is_moving = false;
    pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    SendMessage ("pcprojectile_stopped");
  }
}

//---------------------------------------------------------------------------

