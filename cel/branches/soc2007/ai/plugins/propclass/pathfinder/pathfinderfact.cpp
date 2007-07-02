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
#include "plugins/propclass/pathfinder/pathfinderfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csgeom/matrix3.h"

#include "iengine/mesh.h"
#include "iengine/sector.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY_ALT (PathFinder, "pcmove.pathfinder", "pcpathfinder")

//---------------------------------------------------------------------------

csStringID celPcPathFinder::id_sectorname = csInvalidStringID;
csStringID celPcPathFinder::id_position = csInvalidStringID;
csStringID celPcPathFinder::id_pursue_max_prediction = csInvalidStringID;
csStringID celPcPathFinder::id_is_active = csInvalidStringID;
csStringID celPcPathFinder::id_min_distance = csInvalidStringID;
csStringID celPcPathFinder::id_meshname = csInvalidStringID;

PropertyHolder celPcPathFinder::propinfo;

celPcPathFinder::celPcPathFinder (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  engine = csQueryRegistry<iEngine> (object_reg);
  //  celgraph = csQueryRegistry<iCelGraph> (object_reg);

  // For actions.
  if (id_sectorname == csInvalidStringID)
  {
    id_sectorname = pl->FetchStringID ("cel.parameter.sectorname");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_pursue_max_prediction = pl->FetchStringID ("cel.parameter.pursue_max_prediction");
    id_is_active = pl->FetchStringID ("cel.parameter.is_active");
    id_min_distance = pl->FetchStringID ("cel.parameter.min_distance");
    id_meshname = pl->FetchStringID ("cel.parameter.meshname");  
}

  params = new celOneParameterBlock ();
  params->SetParameterDef (id_meshname, "meshname");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_seek, "cel.action.Seek");
    AddAction (action_wander, "cel.action.Wander");
    AddAction (action_pursue, "cel.action.Pursue");
    AddAction (action_interrupt, "cel.action.Interrupt");
  }

  // For properties.
  propinfo.SetCount (24);
  AddProperty (propid_position, "cel.property.position",
	       CEL_DATA_VECTOR3, true, "Desired end position.", &position);
  AddProperty (propid_pursue_max_prediction, "cel.property.pursue_max_prediction",
	       CEL_DATA_FLOAT, false, "Pursue Max Prediction.", &pursue_max_prediction);
  AddProperty (propid_is_active, "cel.property.is_active",
	      CEL_DATA_BOOL, true, "Is pathfinding activated?", &is_active);
 
  delay_recheck = 20;
  random.Initialize();
  min_distance = 2.0;

  cur_path = scfCreateInstance<iCelPath> ("cel.celpath");
    if(!cur_path)
      fprintf(stderr, "Error Loading celPath in PathFinder!\n");
}

celPcPathFinder::~celPcPathFinder ()
{
  if (pl)
    pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  delete params;
}

#define GRAPH_SERIAL 1

csPtr<iCelDataBuffer> celPcPathFinder::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (GRAPH_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcPathFinder::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != GRAPH_SERIAL) return false;
  return true;
}

void celPcPathFinder::SendMessage (const char* msg, const char* meshname)
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

void celPcPathFinder:: SetDelayRecheck (int delay)
{
  delay_recheck = delay;
}

void celPcPathFinder:: SetMinDistance (int distance)
{
  min_distance = distance;
}

bool celPcPathFinder::Seek (iSector* sector, const csVector3& position)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;
  if(!pcsteer)
    return false;

  Interrupt();

  is_active = true;
  current_action = action_seek;

  celPcPathFinder::sector = sector;
  celPcPathFinder::position = position;

  float cur_yrot;
  csVector3 cur_position;
  iSector* cur_sector;

  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);

  // set destination y value to the same as current y value so as to
  // check only on x and z axis.
  cur_position.y = position.y;

  iCelNode* from = celgraph->GetClosest(cur_position);
  goal = celgraph->GetClosest(position);

  if(celgraph->ShortestPath(from, goal, cur_path)){
    FollowPath();
    return true;
  }
  return false;
}

bool celPcPathFinder::Wander (size_t distance){
    FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;
  if (!pcsteer)
    return false;

  Interrupt();
  
  float cur_yrot;
  csVector3 cur_position;
  iSector* cur_sector;

  pclinmove->GetLastFullPosition (cur_position, cur_yrot, cur_sector);

  iCelNode* from = celgraph->GetClosest(cur_position);
    
  current_action = action_wander;
  wander_distance = distance;
  is_active = true;

  goal = celgraph->RandomPath(from, wander_distance, cur_path);
  FollowPath();

  return true;
}

bool celPcPathFinder :: Pursue (iCelEntity* target, float max_prediction)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;
  if (!pcsteer)
    return false;

  Interrupt();

  current_action = action_pursue;
  pursue_target = target;
  pursue_max_prediction = max_prediction;
  is_active = true;

  csWeakRef<iPcLinearMovement> targetlinmove;
  float yrot;

  targetlinmove = CEL_QUERY_PROPCLASS_ENT (target, iPcLinearMovement);  
  targetlinmove->GetLastFullPosition (position, yrot, sector);
  
  csVector3 cur_position;
  iSector* cur_sector;

  pclinmove->GetLastFullPosition (cur_position, yrot, cur_sector);
  
  iCelNode* from = celgraph->GetClosest(cur_position);
  iCelNode* to = celgraph->GetClosest(position);
  if(celgraph->ShortestPath(from, to, cur_path)){
    FollowPath();
    return true;
  }
  return false;
}

bool celPcPathFinder :: FollowCyclicPath (iCelPath* path)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;
  if (!pcsteer)
    return false;

  Interrupt();

  is_active = true;
  cur_path = path;
  current_action = action_cyclic;
  
  FollowPath();
  return true;
}

bool celPcPathFinder :: FollowOneWayPath (iCelPath* path)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;
  if (!pcsteer)
    return false;

  Interrupt();

  is_active = true;
  cur_path = path;
  current_action = action_one_way;
  
  FollowPath();
  return true;
}

bool celPcPathFinder :: FollowTwoWayPath (iCelPath* path)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
    return false;
  if (!pcactormove)
    return false;
  if (!pcsteer)
    return false;

  Interrupt();

  is_active = true;
  cur_path = path;
  current_action = action_two_way;
  
  FollowPath();
  return true;
}

bool celPcPathFinder::FollowPath ()
{
  if(!pclinmove)
    return false;
  if(!pcactormove)
    return false;
  if(!pcsteer)
    return false;
  
  float yrot;
  csVector3 cur_position;
  iSector* cur_sector;
  
  if(goal->GetMapNode() == cur_path->Current()){
    switch (current_action)
      {
      case action_seek:
	{
	  pcsteer->Seek(sector, position);
	  return true;
	}
      case action_pursue:
	{
	  pcsteer->Pursue(pursue_target, pursue_max_prediction);
	  return true;
	}
      case action_wander:
	{
	  Wander(wander_distance);
	  return true;
	}
      case action_cyclic:
	{
	  cur_path->Restart();
	  break;
	}
      case action_one_way:
	{
	  pcsteer->Interrupt();
	  return true;
	}
      case action_two_way:
	{
	  cur_path->Restart();
	  break;
	}
      case action_interrupt:
	Interrupt ();
	return true;
      default:
	return false;
      }
  } else {
   
    pclinmove->GetLastFullPosition (cur_position, yrot, cur_sector);
    csVector3 target_position = cur_path->CurrentPosition();
    iSector* target_sector = cur_path->CurrentSector();

    if(csSquaredDist::PointPoint(cur_position, target_position) <= min_distance)
      if(cur_path->HasNext()){
	iMapNode* node = cur_path->Next();
	csVector3 target_position = node->GetPosition();
	target_sector = node->GetSector();
      }
      else {
	Interrupt();
	return false;
      }
    pcsteer->Seek(target_sector, target_position);
  }
  pl->CallbackOnce ((iCelTimerListener*)this, delay_recheck, CEL_EVENT_PRE);
   
  return true;
}

void celPcPathFinder::StopTracking ()
{
  if (!is_active) return;
  is_active = false;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  if(pcsteer)
    pcsteer->Interrupt();
}


void celPcPathFinder::Interrupt ()
{
  if (is_active)
  {
    StopTracking ();
    SendMessage ("pcpathfinder_interrupted");
  }
}

bool celPcPathFinder::PerformActionIndexed (int idx,
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

void celPcPathFinder::TickOnce ()
{
  if (!is_active) 
    return;
  
  if(current_action == action_pursue)
    Pursue(pursue_target, pursue_max_prediction);
  else
    FollowPath();
}

void celPcPathFinder::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcactormove = CEL_QUERY_PROPCLASS_ENT (entity, iPcActorMove);
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    pcsteer = CEL_QUERY_PROPCLASS_ENT (entity, iPcSteer);
  }
}

void celPcPathFinder::SetGraph (iCelGraph* graph)
{
  celgraph = graph;
}

//---------------------------------------------------------------------------

