/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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
#include "csgeom/math3d.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/evdefs.h"
#include "csutil/debug.h"
#include "plugins/propclass/mover/moverfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

#include "iengine/mesh.h"
#include "iengine/sector.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Mover, "pcmover")

//---------------------------------------------------------------------------

csStringID celPcMover::id_sectorname = csInvalidStringID;
csStringID celPcMover::id_position = csInvalidStringID;
csStringID celPcMover::id_up = csInvalidStringID;
csStringID celPcMover::id_sqradius = csInvalidStringID;
csStringID celPcMover::id_meshname = csInvalidStringID;

PropertyHolder celPcMover::propinfo;

celPcMover::celPcMover (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  engine = csQueryRegistry<iEngine> (object_reg);

  // For actions.
  if (id_sectorname == csInvalidStringID)
  {
    id_sectorname = pl->FetchStringID ("cel.parameter.sectorname");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_up = pl->FetchStringID ("cel.parameter.up");
    id_sqradius = pl->FetchStringID ("cel.parameter.sqradius");
    id_meshname = pl->FetchStringID ("cel.parameter.meshname");
  }

  params = new celOneParameterBlock ();
  params->SetParameterDef (id_meshname, "meshname");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_start, "cel.action.Start");
    AddAction (action_interrupt, "cel.action.Interrupt");
  }

  // For properties.
  propinfo.SetCount (4);
  AddProperty (propid_position, "cel.property.position",
	CEL_DATA_VECTOR3, true, "Desired end position.", &position);
  AddProperty (propid_up, "cel.property.up",
	CEL_DATA_VECTOR3, true, "Current up vector.", &up);
  AddProperty (propid_sqradius, "cel.property.sqradius",
	CEL_DATA_FLOAT, false, "Current squared radius.", &sqradius);
  AddProperty (propid_moving, "cel.property.moving",
	CEL_DATA_BOOL, true, "Is moving?", &is_moving);

  is_moving = false;
}

celPcMover::~celPcMover ()
{
  if (pl)
    pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  delete params;
}

#define MOVER_SERIAL 1

csPtr<iCelDataBuffer> celPcMover::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MOVER_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMover::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MOVER_SERIAL) return false;
  return true;
}

void celPcMover::SendMessage (const char* msg, const char* meshname)
{
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    csRef<iCelEntity> ref = (iCelEntity*)entity;
    if (meshname)
      params->GetParameter (0).Set (meshname);
    celData ret;
    bh->SendMessage (msg, this, ret, meshname ? params : 0);
  }
}

static float GetAngle (const csVector3& v1, const csVector3& v2)
{
  float len = sqrt (csSquaredDist::PointPoint (v1, v2));
  float angle = acos ((v2.x-v1.x) / len);
  if ((v2.z-v1.z) > 0) angle = 2*PI - angle;
  angle += PI / 2.0f;
  if (angle > 2*PI) angle -= 2*PI;
  return angle;
}

#define DELAY_RECHECK 20

bool celPcMover::Start (iSector* sector, const csVector3& position,
	const csVector3& up, float sqradius)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;

  Interrupt ();

  celPcMover::sector = sector;
  celPcMover::position = position;
  celPcMover::up = up;
  celPcMover::sqradius = sqradius;

  // First we do a beam between our current position and the desired
  // position. If this beam fails already then we don't even start the move
  // and just send a 'pcmover_impossible' message.
  float cur_yrot;
  csVector3 cur_position;
  iSector* cur_sector;
  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);

  // Use center of linmove CD box to trace beam.
  csVector3 body, legs, shift;
  iPcCollisionDetection* pc_cd;
  pclinmove->GetCDDimensions (body, legs, shift, pc_cd);

  float sqlen = csSquaredDist::PointPoint (cur_position, position);
  if (sqlen < sqradius)
  {
    StopMovement ();
    SendMessage ("pcmover_arrived");
    return true;
  }

  csSectorHitBeamResult rc = cur_sector->HitBeamPortals (cur_position,
      position);
  if (rc.mesh)
    rc = cur_sector->HitBeamPortals (cur_position+csVector3 (0,1,0),
	position+csVector3 (0,1,0));
  if (rc.mesh)
  {
    SendMessage ("pcmover_impossible", rc.mesh->QueryObject ()->GetName ());
    return false;
  }

  csVector3 vec (0,0,1);
  float yrot = GetAngle (position-cur_position, vec);
  pcactormove->RotateTo (yrot);
  pcactormove->Forward (true);

  pl->CallbackOnce ((iCelTimerListener*)this, DELAY_RECHECK, CEL_EVENT_PRE);

  is_moving = true;

  return false;
}

void celPcMover::StopMovement ()
{
  if (!is_moving) return;
  if (pcactormove) pcactormove->Forward (false);
  is_moving = false;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
}

void celPcMover::Interrupt ()
{
  if (is_moving)
  {
    StopMovement ();
    SendMessage ("pcmover_interrupted");
  }
}

bool celPcMover::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_start:
      {
        CEL_FETCH_STRING_PAR (sectorname,params,id_sectorname);
        if (!p_sectorname) return false;
        CEL_FETCH_VECTOR3_PAR (position,params,id_position);
        if (!p_position) return false;
        CEL_FETCH_VECTOR3_PAR (up,params,id_up);
        if (!p_up) return false;
        CEL_FETCH_FLOAT_PAR (sqradius,params,id_sqradius);
        if (!p_sqradius) return false;
        iSector* s = engine->FindSector (sectorname);
        if (!s)
          return false;
        Start (sector, position, up, sqradius);
        // @@@ Return value?
        return true;
      }
    case action_interrupt:
      Interrupt ();
      return true;
    default:
      return false;
  }
}

void celPcMover::TickOnce ()
{
  if (!is_moving) return;

  float cur_yrot;
  csVector3 cur_position;
  iSector* cur_sector;
  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);

  float sqlen = csSquaredDist::PointPoint (cur_position, position);
  if (sqlen < sqradius)
  {
    StopMovement ();
    SendMessage ("pcmover_arrived");
    return;
  }

  csVector3 vec (0,0,1);
  float yrot = GetAngle (position-cur_position, vec);
  pcactormove->RotateTo (yrot);
  pl->CallbackOnce ((iCelTimerListener*)this, DELAY_RECHECK, CEL_EVENT_PRE);
}

void celPcMover::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcactormove = CEL_QUERY_PROPCLASS_ENT (entity, iPcActorMove);
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

//---------------------------------------------------------------------------

