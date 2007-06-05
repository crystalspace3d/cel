/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNUmy c
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
csStringID celPcSteer::id_arrival_radius = csInvalidStringID;
csStringID celPcSteer::id_ca_lookahead = csInvalidStringID;
csStringID celPcSteer::id_ca_weight = csInvalidStringID;

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
    id_meshname = pl->FetchStringID ("cel.parameter.meshname");
    id_cur_yrot = pl->FetchStringID ("cel.parameter.cur_yrot");
    id_arrival_radius = pl->FetchStringID ("cel.parameter.arrival_radius");
    id_ca_lookahead = pl->FetchStringID ("cel.parameter.ca_lookahead");
    id_ca_weight = pl->FetchStringID ("cel.parameter.ca_weight");

  }

  params = new celOneParameterBlock ();
  params->SetParameterDef (id_meshname, "meshname");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_seek, "cel.action.Seek");
    AddAction (action_flee, "cel.action.Flee");
    AddAction (action_interrupt, "cel.action.Interrupt");
  }

  // For properties.
  propinfo.SetCount (12);
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
  AddProperty (propid_arrival_radius, "cel.property.arrival_radius",
	       CEL_DATA_FLOAT, false, "Arrival Radius.", &arrival_radius);
  AddProperty (propid_ca_lookahead, "cel.property.ca_lookahead",
	       CEL_DATA_FLOAT, false, "CA Lookahead.", &ca_lookahead);
  AddProperty (propid_ca_weight, "cel.property.ca_weight",
	       CEL_DATA_FLOAT, false, "CA weight.", &ca_weight);
  AddProperty (propid_moving, "cel.property.moving",
	       CEL_DATA_BOOL, true, "Is moving?", &is_moving);
  AddProperty (propid_check_arrival, "cel.property.check_arrival",
	       CEL_DATA_BOOL, true, "Check if arrived at position?", &check_arrival);
  AddProperty (propid_collision_avoidance, "cel.property.collision_avoidance",
	      CEL_DATA_BOOL, true, "Avoid Collisions?", &collision_avoidance);


  is_moving = false;
  //check_arrival = false;
  //collision_avoidance = false;
  
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

  //Interrupt();

  current_action = action_seek;

  celPcSteer::sector = sector;
  celPcSteer::position = position;

  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);

  // set destination y value to the same as current y value so as to
  // check only on x and z axis.
  cur_position.y = position.y;

  cur_direction = position-cur_position;

  Move();

  pl->CallbackOnce ((iCelTimerListener*)this, DELAY_RECHECK, CEL_EVENT_PRE);
 
  return true;
}

bool celPcSteer::Flee (iSector* sector, const csVector3& position)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;


  current_action = action_flee;
  
  celPcSteer::sector = sector;
  celPcSteer::position = position;

  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);

  // set destination y value to the same as current y value so as to
  // check only on x and z axis.
  cur_position.y = position.y;

  //Here we substract position from cur_position instead so we get the opposite
  // direction

  cur_direction = cur_position-position;

  Move();   

  pl->CallbackOnce ((iCelTimerListener*)this, DELAY_RECHECK, CEL_EVENT_PRE);

  return true;
}



void celPcSteer::CheckArrivalOn(float radius){
  celPcSteer::arrival_radius = radius;
  check_arrival = true;
}


void celPcSteer::CheckArrivalOff(){
  check_arrival = false;
}

bool celPcSteer::CheckArrival(){
  if(check_arrival){
    float sqlen = csSquaredDist::PointPoint (cur_position, position);
    if (sqlen < arrival_radius)
      {
	StopMovement ();
	SendMessage ("pcsteer_arrived");
	return true;
      }
  }
  return false;
}

void celPcSteer::CollisionAvoidanceOn(float lookahead, float weight){
  ca_lookahead = lookahead;
  ca_weight = weight;
  collision_avoidance = true;
}

void celPcSteer::CollisionAvoidanceOff(){
  collision_avoidance = false;
}

bool celPcSteer::CollisionAvoidance(){
  if(!collision_avoidance)
    return false;
  csSectorHitBeamResult rc = cur_sector->HitBeamPortals (cur_position,
							 position);
  if (rc.mesh){
    float distance = csSquaredDist::PointPoint(cur_position, rc.isect);
    
    /*
     * If the collision is too far away, we don´t bother to avoid it
     */
    if(distance > ca_lookahead)
      return false;
    
    printf("AVOID!\n");
    csVector3 direction = cur_position - rc.isect;
    //direction = direction*ca_weight;
    cur_direction = direction;
    SendMessage ("pcsteer_avoiding_collision", rc.mesh->QueryObject ()->GetName ());
    return true;
  }
  
  return false;
}

bool celPcSteer::Move()
{
  if(!pcactormove)
    return false;

  if(CheckArrival())
    return true;

  CollisionAvoidance();

  csVector3 vec (0,0,1);
  
  float yrot = GetAngle (cur_direction, vec);
  pcactormove->RotateTo (yrot);
  pcactormove->Forward (true);
  is_moving=true;
  
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
  case action_seek:
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
   
  if(current_action == action_seek)
    Seek(sector, position);
  else if (current_action == action_flee)
    Flee(sector, position);
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

