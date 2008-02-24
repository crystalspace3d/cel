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
#include "iutil/objreg.h"
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
csStringID celPcMover::id_movespeed = csInvalidStringID;
csStringID celPcMover::id_rotatespeed = csInvalidStringID;
csStringID celPcMover::id_sqradius = csInvalidStringID;
csStringID celPcMover::action_start = csInvalidStringID;
csStringID celPcMover::action_interrupt = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcMover)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMover)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMover::PcMover)
  SCF_IMPLEMENTS_INTERFACE (iPcMover)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMover::celPcMover (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMover);

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);

  // For PerformAction.
  if (id_sectorname == csInvalidStringID)
  {
    id_sectorname = pl->FetchStringID ("cel.parameter.sectorname");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_up = pl->FetchStringID ("cel.parameter.up");
    id_movespeed = pl->FetchStringID ("cel.parameter.movespeed");
    id_rotatespeed = pl->FetchStringID ("cel.parameter.rotatespeed");
    id_sqradius = pl->FetchStringID ("cel.parameter.sqradius");
    action_start = pl->FetchStringID ("cel.action.Start");
    action_interrupt = pl->FetchStringID ("cel.action.Interrupt");
  }

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_position] = &position;// Automatically handled.
  propdata[propid_up] = &up;
  propdata[propid_movespeed] = &movespeed;
  propdata[propid_rotatespeed] = &rotatespeed;
  propdata[propid_sqradius] = &sqradius;
  propdata[propid_moving] = &is_moving;
}

celPcMover::~celPcMover ()
{
  pl->RemoveCallbackPCEveryFrame (this, cscmdPreProcess);
}

Property* celPcMover::properties = 0;
int celPcMover::propertycount = 0;

void celPcMover::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT (pl != 0);

    propertycount = 6;
    properties = new Property[propertycount];

    properties[propid_position].id = pl->FetchStringID (
    	"cel.property.position");
    properties[propid_position].datatype = CEL_DATA_VECTOR3;
    properties[propid_position].readonly = true;
    properties[propid_position].desc = "Desired end position.";

    properties[propid_up].id = pl->FetchStringID (
    	"cel.property.up");
    properties[propid_up].datatype = CEL_DATA_VECTOR3;
    properties[propid_up].readonly = true;
    properties[propid_up].desc = "Current up vector.";

    properties[propid_movespeed].id = pl->FetchStringID (
    	"cel.property.movespeed");
    properties[propid_movespeed].datatype = CEL_DATA_FLOAT;
    properties[propid_movespeed].readonly = true;
    properties[propid_movespeed].desc = "Current movement speed.";

    properties[propid_rotatespeed].id = pl->FetchStringID (
    	"cel.property.rotatespeed");
    properties[propid_rotatespeed].datatype = CEL_DATA_FLOAT;
    properties[propid_rotatespeed].readonly = true;
    properties[propid_rotatespeed].desc = "Current rotation speed.";

    properties[propid_sqradius].id = pl->FetchStringID (
    	"cel.property.sqradius");
    properties[propid_sqradius].datatype = CEL_DATA_FLOAT;
    properties[propid_sqradius].readonly = false;
    properties[propid_sqradius].desc = "Current squared radius.";

    properties[propid_moving].id = pl->FetchStringID (
    	"cel.property.moving");
    properties[propid_moving].datatype = CEL_DATA_BOOL;
    properties[propid_moving].readonly = true;
    properties[propid_moving].desc = "Is moving?";
  }
}

#define MOVER_SERIAL 1

csPtr<iCelDataBuffer> celPcMover::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MOVER_SERIAL);
  databuf->SetDataCount (0);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMover::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MOVER_SERIAL) return false;
  if (databuf->GetDataCount () != 0) return false;
  return true;
}

void celPcMover::SendMessage (const char* msg)
{
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    csRef<iCelEntity> ref = entity;
    celData ret;
    bh->SendMessage (msg, ret, 0);
  }
}

bool celPcMover::Start (iSector* sector, const csVector3& position, const csVector3& up,
  	float movespeed, float rotatespeed, float sqradius)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;

  Interrupt ();

  celPcMover::sector = sector;
  celPcMover::position = position;
  celPcMover::up = up;
  celPcMover::movespeed = movespeed;
  celPcMover::rotatespeed = rotatespeed;
  celPcMover::sqradius = sqradius;

  // First we do a beam between our current position and the desired
  // position. If this beam fails already then we don't even start the move
  // and just send a 'pcmover_impossible' message.
  csVector3 cur_pos;
  float cur_yrot;
  iSector* cur_sector;
  pclinmove->GetLastPosition (cur_pos, cur_yrot, cur_sector);

  // Use center of linmove CD box to trace beam.
  csVector3 body, legs, shift;
  iPcCollisionDetection* pc_cd;
  pclinmove->GetCDDimensions (body, legs, shift, pc_cd);
  //@@@

  csVector3 isect;
  iMeshWrapper* hit_mesh = cur_sector->HitBeamPortals (cur_pos,
  	position, isect, 0);
  if (hit_mesh)
  {
    SendMessage ("pcmover_impossible");
    return false;
  }

  // @@@ TODO: Continue here...

  is_moving = true;

  return false;
}

void celPcMover::Interrupt ()
{
  if (is_moving)
  {
    is_moving = false;
    pl->RemoveCallbackPCEveryFrame (this, cscmdPreProcess);
    SendMessage ("pcmover_interrupted");
  }
}

bool celPcMover::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_start)
  {
    CEL_FETCH_STRING_PAR (sectorname,params,id_sectorname);
    if (!p_sectorname) return false;
    CEL_FETCH_VECTOR3_PAR (position,params,id_position);
    if (!p_position) return false;
    CEL_FETCH_VECTOR3_PAR (up,params,id_up);
    if (!p_up) return false;
    CEL_FETCH_FLOAT_PAR (movespeed,params,id_movespeed);
    if (!p_movespeed) return false;
    CEL_FETCH_FLOAT_PAR (rotatespeed,params,id_rotatespeed);
    if (!p_rotatespeed) return false;
    CEL_FETCH_FLOAT_PAR (sqradius,params,id_sqradius);
    if (!p_sqradius) return false;
    iSector* s = engine->FindSector (sectorname);
    if (!s)
      return false;
    Start (sector, position, up, movespeed, rotatespeed, sqradius);
    // @@@ Return value?
    return true;
  }
  else if (actionId == action_interrupt)
  {
    Interrupt ();
    return true;
  }
  return false;
}

void celPcMover::TickEveryFrame ()
{
  // @@@ TODO. Implement actual movement stuff here.
}

void celPcMover::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

//---------------------------------------------------------------------------
