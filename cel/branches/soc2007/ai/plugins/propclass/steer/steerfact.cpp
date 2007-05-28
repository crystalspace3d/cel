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
#include "csutil/sysfunc.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/evdefs.h"
#include "plugins/propclass/steer/steerfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

#include "iengine/mesh.h"
#include "iengine/sector.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY_ALT (Steer, "pcsteer.steer", "pcsteer")

//---------------------------------------------------------------------------

csStringID celPcSteer::id_sectorname = csInvalidStringID;
csStringID celPcSteer::id_cur_sector = csInvalidStringID;
csStringID celPcSteer::id_position = csInvalidStringID;
csStringID celPcSteer::id_cur_position = csInvalidStringID;
csStringID celPcSteer::id_cur_direction = csInvalidStringID;
csStringID celPcSteer::id_up = csInvalidStringID;
csStringID celPcSteer::id_meshname = csInvalidStringID;
csStringID celPcSteer::id_cur_yrot = csInvalidStringID;

PropertyHolder celPcSteer::propinfo;

celPcSteer::celPcSteer (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  engine = csQueryRegistry<iEngine> (object_reg);

  // For actions.
  if (id_sectorname == csInvalidStringID)
  {
    id_sectorname = pl->FetchStringID ("cel.parameter.sectorname");
    id_cur_sector = pl->FetchStringID ("cel.parameter.cur_sector");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_cur_position = pl->FetchStringID ("cel.parameter.cur_position");
    id_cur_direction = pl->FetchStringID ("cel.parameter.cur_direction");
    id_up = pl->FetchStringID ("cel.parameter.up");
    //id_sqradius = pl->FetchStringID ("cel.parameter.sqradius");
    id_meshname = pl->FetchStringID ("cel.parameter.meshname");
    id_cur_yrot = pl->FetchStringID ("cel.parameter.cur_yrot");
  }

  params = new celOneParameterBlock ();
  params->SetParameterDef (id_meshname, "meshname");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_steer, "cel.action.Steer");
    AddAction (action_interrupt, "cel.action.Interrupt");
  }

  // For properties.
  propinfo.SetCount (6);
  AddProperty (propid_position, "cel.property.position",
	       CEL_DATA_VECTOR3, true, "Desired end position.", &position);
  AddProperty (propid_cur_position, "cel.property.cur_position",
	       CEL_DATA_VECTOR3, true, "Current position.", &cur_position);
  AddProperty (propid_cur_direction, "cel.property.cur_direction",
	       CEL_DATA_VECTOR3, true, "Current direction.", &cur_direction);
  AddProperty (propid_up, "cel.property.up",
	       CEL_DATA_VECTOR3, true, "Current up vector.", &up);
  AddProperty (propid_cur_yrot, "cel.property.cur_yrot",
	       CEL_DATA_FLOAT, false, "Current rotation.", &cur_yrot);
  AddProperty (propid_moving, "cel.property.moving",
	       CEL_DATA_BOOL, true, "Is moving?", &is_moving);

  is_moving = false;
  up = csVector3(0,1,0);
}

celPcSteer::~celPcSteer ()
{
  if (pl)
    pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  delete params;
}

#define STEER_SERIAL 1

csPtr<iCelDataBuffer> celPcSteer::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (STEER_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcSteer::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != STEER_SERIAL) return false;
  return true;
}

void celPcSteer::SendMessage (const char* msg, const char* meshname)
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


bool celPcSteer::Seek (iSector* sector, const csVector3& position)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;

  celPcSteer::sector = sector;
  celPcSteer::position = position;

  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);

  // set destination y value to the same as current y value so as to
  // check only on x and z axis.
  cur_position.y = position.y;

  csVector3 vec (0,0,1);
  cur_direction = position-cur_position;

  float yrot = GetAngle (cur_direction, vec);
  pcactormove->RotateTo (yrot);
  pcactormove->Forward (true);
  
  return true;
}

void celPcSteer::StopMovement ()
{
  if (!is_moving) return;
  if (pcactormove) pcactormove->Forward (false);
  is_moving = false;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
}

void celPcSteer::Interrupt ()
{
  if (is_moving)
  {
    StopMovement ();
    SendMessage ("pcsteer_interrupted");
  }
}

bool celPcSteer::PerformActionIndexed (int idx,
				       iCelParameterBlock* params,
				       celData& ret)
{
  switch (idx)
  {
  case action_steer:
    {
      CEL_FETCH_STRING_PAR (sectorname,params,id_sectorname);
      if (!p_sectorname) return false;
      CEL_FETCH_VECTOR3_PAR (position,params,id_position);
      if (!p_position) return false;
      iSector* s = engine->FindSector (sectorname);
      if (!s)
	return false;
      Seek (sector, position);
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

void celPcSteer::TickOnce ()
{
  if (!is_moving) return;

  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);
  // set destination y value to the same as current y value so as to
  // check only on x and z axis.
  cur_position.y = position.y;
   
  // check arrival
  /* float sqlen = csSquaredDist::PointPoint (cur_position, position);
  if (sqlen < sqradius)
    {
      StopMovement ();
      SendMessage ("pcsteer_arrived");
      return;
    }
  */
  csVector3 vec (0,0,1);
  float yrot = GetAngle (position-cur_position, vec);
  pcactormove->RotateTo (yrot);
  pl->CallbackOnce ((iCelTimerListener*)this, DELAY_RECHECK, CEL_EVENT_PRE);
}

void celPcSteer::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcactormove = CEL_QUERY_PROPCLASS_ENT (entity, iPcActorMove);
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

//---------------------------------------------------------------------------

