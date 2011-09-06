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
#include "iutil/object.h"
#include "plugins/propclass/projectile/projectilefact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"
#include "iengine/movable.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (Projectile, "pcmove.projectile", "pcprojectile")

//---------------------------------------------------------------------------

csStringID celPcProjectile::id_direction = csInvalidStringID;
csStringID celPcProjectile::id_speed = csInvalidStringID;
csStringID celPcProjectile::id_maxdist = csInvalidStringID;
csStringID celPcProjectile::id_maxhits = csInvalidStringID;
csStringID celPcProjectile::id_entity = csInvalidStringID;
csStringID celPcProjectile::id_intersection = csInvalidStringID;
csStringID celPcProjectile::id_meshname = csInvalidStringID;

PropertyHolder celPcProjectile::propinfo;

celPcProjectile::celPcProjectile (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  if (id_direction == csInvalidStringID)
  {
    id_direction = pl->FetchStringID ("direction");
    id_speed = pl->FetchStringID ("speed");
    id_maxdist = pl->FetchStringID ("maxdist");
    id_maxhits = pl->FetchStringID ("maxhits");
    id_entity = pl->FetchStringID ("entity");
    id_intersection = pl->FetchStringID ("intersection");
    id_meshname = pl->FetchStringID ("meshname");
  }
  params.AttachNew (new celVariableParameterBlock ());
  params->SetParameterDef (0, id_entity);
  params->SetParameterDef (1, id_intersection);
  params->SetParameterDef (2, id_meshname);

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.move.projectile.action.");
    AddAction (action_start, "Start");
    AddAction (action_interrupt, "Interrupt");
  }

  // For properties.
  propinfo.SetCount (1);
  AddProperty (propid_moving, "moving",
  	CEL_DATA_BOOL, true, "Moving.", 0);

  is_moving = false;

  vc = csQueryRegistry<iVirtualClock> (object_reg);
  if (!vc)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pcmove.projectile", "No iVirtualClock!");
    return;
  }
}

celPcProjectile::~celPcProjectile ()
{
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
}

bool celPcProjectile::GetPropertyIndexed (int idx, bool& b)
{
  if (idx == propid_moving)
  {
    b = is_moving;
    return true;
  }
  return false;
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
    pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  }
}

void celPcProjectile::SendMessage (const char* msgold,
    const char* msg, csRef<iMessageDispatcher>& dispatcher)
{
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage (msgold, this, ret, 0);
  }
  if (!dispatcher)
  {
    dispatcher = entity->QueryMessageChannel ()->
      CreateMessageDispatcher (this, msg);
    if (!dispatcher) return;
  }
  dispatcher->SendMessage (0);
}

void celPcProjectile::SendMessage (const char* msgold, const char* msg,
    csRef<iMessageDispatcher>& dispatcher,
    iCelEntity* hitent, const csVector3& isect, const char* meshname)
{
  params->GetParameter (0).Set (hitent);
  params->GetParameter (1).Set (isect);
  params->GetParameter (2).Set (meshname);

  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage (msgold, this, ret, params);
  }
  if (!dispatcher)
  {
    dispatcher = entity->QueryMessageChannel ()->
      CreateMessageDispatcher (this, msg);
    if (!dispatcher) return;
  }
  dispatcher->SendMessage (params);
}

bool celPcProjectile::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_start:
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
    case action_interrupt:
      Interrupt ();
      return true;
    default:
      return false;
  }
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
  // To avoid message problems during destruction.
  csRef<iCelEntity> keepref_this = entity;

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
  bool nohitbeam = pcmesh->GetMesh()->GetFlags().Check(CS_ENTITY_NOHITBEAM);
  pcmesh->GetMesh()->SetFlagsRecursive(CS_ENTITY_NOHITBEAM);
  csSectorHitBeamResult rc = cursector->HitBeamPortals (curpos, newpos);
  if (!nohitbeam)
    pcmesh->GetMesh()->SetFlagsRecursive(CS_ENTITY_NOHITBEAM,0);
  if (rc.mesh)
  {
    curhits++;
    iCelEntity* hitent = pl->FindAttachedEntity (rc.mesh->QueryObject ());
    keepref = entity;
    SendMessage ("pcprojectile_hit", "cel.move.collision",
	dispatcher_collision, hitent, rc.isect,
	rc.mesh->QueryObject ()->GetName ());
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
    SendMessage ("pcprojectile_stopped", "cel.move.interrupted",
	dispatcher_interrupted);
  }
}

//---------------------------------------------------------------------------

