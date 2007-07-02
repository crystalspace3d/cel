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
#include "csgeom/matrix3.h"

#include "iengine/mesh.h"
#include "iengine/sector.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY_ALT (Steer, "pcmove.steer", "pcsteer")

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
csStringID celPcSteer::id_wander_offset = csInvalidStringID;
csStringID celPcSteer::id_wander_radius = csInvalidStringID;
csStringID celPcSteer::id_wander_rate = csInvalidStringID;
csStringID celPcSteer::id_cohesion_radius = csInvalidStringID;
csStringID celPcSteer::id_separation_radius = csInvalidStringID;
csStringID celPcSteer::id_dm_radius = csInvalidStringID;
csStringID celPcSteer::id_pursue_max_prediction = csInvalidStringID;
csStringID celPcSteer::id_separation_weight = csInvalidStringID;
csStringID celPcSteer::id_cohesion_weight = csInvalidStringID;
csStringID celPcSteer::id_dm_weight = csInvalidStringID;

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
    id_wander_offset = pl->FetchStringID ("cel.parameter.wander_offset");
    id_wander_radius = pl->FetchStringID ("cel.parameter.wander_radius");
    id_wander_rate = pl->FetchStringID ("cel.parameter.wander_rate");
    id_cohesion_radius = pl->FetchStringID ("cel.parameter.cohesion_radius");
    id_separation_radius = pl->FetchStringID ("cel.parameter.separation_radius");
    id_dm_radius = pl->FetchStringID ("cel.parameter.dm_radius");
    id_pursue_max_prediction = pl->FetchStringID ("cel.parameter.pursue_max_prediction");
    id_separation_weight = pl->FetchStringID ("cel.parameter.separation_weight");
    id_cohesion_weight = pl->FetchStringID ("cel.parameter.cohesion_weight");
    id_dm_weight = pl->FetchStringID ("cel.parameter.dm_weight");  
}

  params = new celOneParameterBlock ();
  params->SetParameterDef (id_meshname, "meshname");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_seek, "cel.action.Seek");
    AddAction (action_flee, "cel.action.Flee");
    AddAction (action_wander, "cel.action.Wander");
    AddAction (action_pursue, "cel.action.Pursue");
    AddAction (action_interrupt, "cel.action.Interrupt");
  }

  // For properties.
  propinfo.SetCount (24);
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
  AddProperty (propid_wander_offset, "cel.property.wander_offset",
	       CEL_DATA_FLOAT, false, "Wander Offset", &wander_offset);
  AddProperty (propid_wander_radius, "cel.property.wander_radius",
	       CEL_DATA_FLOAT, false, "Wander Radius", &wander_radius);
  AddProperty (propid_wander_rate, "cel.property.wander_rate",
	       CEL_DATA_FLOAT, false, "Wander_rate", &wander_rate);
  AddProperty (propid_cohesion_radius, "cel.property.cohesion_radius",
	       CEL_DATA_FLOAT, false, "Cohesion Radius.", &cohesion_radius);
  AddProperty (propid_separation_radius, "cel.property.separation_radius",
	       CEL_DATA_FLOAT, false, "Separation Radius.", &separation_radius);
  AddProperty (propid_dm_radius, "cel.property.dm_radius",
	       CEL_DATA_FLOAT, false, "Direction Matching Radius.", &dm_radius);
  AddProperty (propid_pursue_max_prediction, "cel.property.pursue_max_prediction",
	       CEL_DATA_FLOAT, false, "Pursue Max Prediction.", &pursue_max_prediction);
  AddProperty (propid_separation_weight, "cel.property.separation_weight",
	       CEL_DATA_FLOAT, false, "Separation Weight.", &separation_weight);
  AddProperty (propid_cohesion_weight, "cel.property.cohesion_weight",
	       CEL_DATA_FLOAT, false, "Cohesion Wegight.", &cohesion_weight);
  AddProperty (propid_dm_weight, "cel.property.dm_weight",
	       CEL_DATA_FLOAT, false, "Direction Matching Weight.", &dm_weight);
  AddProperty (propid_moving, "cel.property.moving",
	       CEL_DATA_BOOL, true, "Is moving?", &is_moving);
  AddProperty (propid_check_arrival, "cel.property.check_arrival",
	       CEL_DATA_BOOL, true, "Check if arrived at position?", &check_arrival);
  AddProperty (propid_collision_avoidance, "cel.property.collision_avoidance",
	      CEL_DATA_BOOL, true, "Avoid Collisions?", &collision_avoidance);
  AddProperty (propid_check_cohesion, "cel.property.check_cohesion",
	      CEL_DATA_BOOL, true, "Check for Cohesion?", &check_cohesion);
  AddProperty (propid_check_separation, "cel.property.check_separation",
	      CEL_DATA_BOOL, true, "Check for separation", &check_separation);
  AddProperty (propid_check_dm, "cel.property.check_dm",
	      CEL_DATA_BOOL, true, "Check for Direction Matching", &check_dm);
 

  is_moving = false;
  check_arrival = false;
  check_cohesion = false;
  check_separation = false;
  check_dm = false;
  collision_avoidance = false;

  separation_targets = pl->CreateEmptyEntityList();
  cohesion_targets = pl->CreateEmptyEntityList();
  dm_targets = pl->CreateEmptyEntityList();
  
  up = csVector3(0,1,0);
  delay_recheck = 20;
  random.Initialize();
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

  //float ori = - atan2(v1.x, v1.y)*(180/ PI);
  float len = sqrt (csSquaredDist::PointPoint (v1, v2));
  float angle = acos ((v2.x-v1.x) / len);
  if ((v2.z-v1.z) > 0) angle = 2*PI - angle;
  angle += PI / 2.0f;
  if (angle > 2*PI) angle -= 2*PI;
  return angle;
}

static csVector3 GetVector (const float angle)
{
  csYRotMatrix3 rot(angle);
  csVector3 vec(0, 0, 1);

  //csVector3 vec(cos(angle), 0, -sin(angle));
  //return vec;
  return rot*vec;
  
}

float celPcSteer:: RandomBinomial (float rate)
{
  float r = random.Get() -  random.Get();
  return r*rate;
}

void celPcSteer:: SetDelayRecheck(int delay){
  delay_recheck = delay;
}

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

  pl->CallbackOnce ((iCelTimerListener*)this, delay_recheck, CEL_EVENT_PRE);
 
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

  pl->CallbackOnce ((iCelTimerListener*)this, delay_recheck, CEL_EVENT_PRE);

  return true;
}

bool celPcSteer::Wander (float offset, float radius, float rate){
    FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;

  current_action = action_wander;
  wander_offset = offset;
  wander_radius = radius;
  wander_rate = rate;

  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);

  if(is_moving){
    csVector3 vec (0,0,1);

    float yrot = GetAngle (cur_direction, vec);
    pcactormove->RotateTo (yrot+RandomBinomial(wander_rate));
    pcactormove->Forward (true);
    is_moving=true;
  }
  
  Move();   

  pl->CallbackOnce ((iCelTimerListener*)this, delay_recheck, CEL_EVENT_PRE);
  
  return true;
}

bool celPcSteer :: Pursue (iCelEntity* target, float max_prediction)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;

  Interrupt();
  
  current_action = action_pursue;
  pursue_target = target;

  csWeakRef<iPcActorMove> targetactormove;
  csWeakRef<iPcLinearMovement> targetlinmove;
  float target_yrot, cur_speed, prediction;

  targetactormove = CEL_QUERY_PROPCLASS_ENT (target, iPcActorMove);
  targetlinmove = CEL_QUERY_PROPCLASS_ENT (target, iPcLinearMovement);
  
  targetlinmove->GetLastFullPosition (position, target_yrot, sector);
  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);
  cur_speed = pcactormove->GetMovementSpeed();
  
  float distance = csSquaredDist::PointPoint (cur_position, position);
  
  if(cur_speed <= distance/pursue_max_prediction)
    prediction = pursue_max_prediction;
  else
    prediction = distance/cur_speed;
  
  csVector3 target_velocity;  
  targetlinmove->GetVelocity(target_velocity);
  position += target_velocity*prediction;
  
  cur_direction = position-cur_position;
  
  Move();

  pl->CallbackOnce ((iCelTimerListener*)this, delay_recheck, CEL_EVENT_PRE);
 
  return true;
}


void celPcSteer::CheckArrivalOn (float radius)
{
  arrival_radius = radius;
  check_arrival = true;
}


void celPcSteer::CheckArrivalOff ()
{
  check_arrival = false;
}

bool celPcSteer::CheckArrival (){
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

void celPcSteer::CollisionAvoidanceOn (float lookahead, float weight)
{
  ca_lookahead = lookahead;
  ca_weight = weight;
  collision_avoidance = true;
}

void celPcSteer::CollisionAvoidanceOff ()
{
  celPcSteer::collision_avoidance = false;
}

bool celPcSteer::CollisionAvoidance ()
{
  if(!collision_avoidance)
    return false;
  csSectorHitBeamResult rc = cur_sector->HitBeamPortals (cur_position,
							 position);

  //If theres a collision we handle it
  if (rc.mesh){
    float distance = csSquaredDist::PointPoint(cur_position, rc.isect);
    
    /*
     * If the collision is too far away, we don´t bother to avoid it
     */
    //if(distance > ca_lookahead)
    //return false;
    
    printf("AVOID!\n");
    csVector3 direction = cur_position - position;
    //direction = direction*ca_weight;
    cur_direction = cur_position - position;
    SendMessage ("pcsteer_avoiding_collision", rc.mesh->QueryObject ()->GetName ());
    return true;
  }

  printf("NO!\n");
  
  return false;
}


void celPcSteer:: CohesionOn (iCelEntityList* targets, float radius, float weight) 
{
  check_cohesion = true;
  cohesion_radius = radius;
  cohesion_weight = weight;
  cohesion_targets->RemoveAll();
  csRef<iCelEntityIterator> it = targets->GetIterator();
  
  while(it->HasNext())
    cohesion_targets->Add(it->Next());
}
  
//void celPcSteer:: CohesionOn (float radius) 
//{
  //check_cohesion = true;
  //}
void celPcSteer:: CohesionOff () 
{
  check_cohesion = false;
}


void celPcSteer :: Cohesion ()
{
  bool changed = false;
  if(check_cohesion){
    csVector3 pos(0, 0, 0);
    csVector3 target_position(0, 0, 0);
    float target_yrot;
    pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);
    
    csRef<iCelEntityIterator> it = cohesion_targets->GetIterator();
    csRef<iCelEntity> target;
    while(it->HasNext()){
      target = it->Next();
      csRef<iPcLinearMovement> targetlinmove = CEL_QUERY_PROPCLASS_ENT (target, iPcLinearMovement);
      targetlinmove->GetLastFullPosition (target_position, target_yrot, sector);
      if(csSquaredDist::PointPoint (cur_position, target_position) >= cohesion_radius){
	pos = pos + target_position;
	changed = true;
      }
    }
    if(changed)
      cur_direction += (pos - cur_position)*cohesion_weight;
  }
}

void celPcSteer:: SeparationOn (iCelEntityList* targets, float radius, float weight) 
{
  check_separation = true;
  separation_radius = radius;
  separation_weight = weight;
  separation_targets->RemoveAll();
  csRef<iCelEntityIterator> it = targets->GetIterator();
  
  while(it->HasNext())
    separation_targets->Add(it->Next());
}

void celPcSteer:: SeparationOff ()
{
  check_separation = false;
}


void celPcSteer :: Separation ()
{
  bool changed = false;
  if(check_separation){
    csVector3 pos(0, 0, 0);
    csVector3 target_position(0, 0, 0);
    float target_yrot;
    pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);
    
    csRef<iCelEntityIterator> it = separation_targets->GetIterator();
    csRef<iCelEntity> target;
    while(it->HasNext()){
      target = it->Next();
      csRef<iPcLinearMovement> targetlinmove = CEL_QUERY_PROPCLASS_ENT (target, iPcLinearMovement);
      targetlinmove->GetLastFullPosition (target_position, target_yrot, sector);
      if(csSquaredDist::PointPoint (cur_position, target_position) <= separation_radius){
	pos += target_position;
	changed = true;
      }
    }
    if(changed)
      cur_direction += (cur_position - pos)*separation_weight;
  }  
}
 
void celPcSteer:: DirectionMatchingOn (iCelEntityList* targets, float weight)
{
  check_dm = true;
  dm_weight = weight;
  dm_targets->RemoveAll();
  csRef<iCelEntityIterator> it = targets->GetIterator();
  
  while(it->HasNext())
    dm_targets->Add(it->Next());
}


//void celPcSteer::DirectionMatchingOn(){
  //check_dm = true;
  //}
  
void celPcSteer:: DirectionMatchingOff ()
{
  check_dm = false;
}



void celPcSteer :: DirectionMatching ()
{
  if(check_dm){
    csVector3 direction(0, 0, 0);
    csVector3 target_velocity(0, 0, 0);
    pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);
    
    csRef<iCelEntityIterator> it = dm_targets->GetIterator();
    csRef<iCelEntity> target;
    while(it->HasNext()){
      target = it->Next();
      csRef<iPcLinearMovement> targetlinmove = CEL_QUERY_PROPCLASS_ENT (target, iPcLinearMovement);
      targetlinmove->GetVelocity(target_velocity);
      
      direction += target_velocity;
    }
    cur_direction += direction;
  }
}

bool celPcSteer::Move ()
{
  if(!pcactormove)
    return false;

  if(CheckArrival())
    return true;

  CollisionAvoidance();
  Separation();
  Cohesion();
  DirectionMatching();

  csVector3 vec (0,0,1);
  

  float yrot = GetAngle (cur_direction, vec);

  /*DEBUG
  csVector3 vec1 (sqrt(3),0,2);
  csVector3 vec2 = GetVector(yrot); 

  printf("Original yrot %f\n", yrot);

  yrot = GetAngle (vec2, vec);

  
  printf("Original Vector .x %f, .y %f, .z%f\n", vec1.x, vec1.y, vec1.z);
  printf("vec2 .x %f, .y %f, .z%f\n", vec2.x, vec2.y, vec2.z);
  printf("yrot %f\n", yrot);
  
  */
  
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
      Seek(sector, position);
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

  //pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);
  // set destination y value to the same as current y value so as to
  // check only on x and z axis.
  cur_position.y = position.y;
   
  if(current_action == action_seek)
    Seek(sector, position);
  else if (current_action == action_flee)
    Flee(sector, position);
  else if (current_action == action_pursue)
    Pursue(pursue_target, pursue_max_prediction);
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

