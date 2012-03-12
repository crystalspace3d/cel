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
#include "ivaria/reporter.h"
#include "plugins/propclass/pathfinder/pathfinderfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csgeom/matrix3.h"

#include "iengine/mesh.h"
#include "iengine/sector.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (PathFinder, "pcmove.pathfinder", "pcpathfinder")

//---------------------------------------------------------------------------

csStringID celPcPathFinder::id_sectorname = csInvalidStringID;
csStringID celPcPathFinder::id_position = csInvalidStringID;
csStringID celPcPathFinder::id_pursue_max_prediction = csInvalidStringID;
csStringID celPcPathFinder::id_is_active = csInvalidStringID;
csStringID celPcPathFinder::id_min_distance = csInvalidStringID;
csStringID celPcPathFinder::id_meshname = csInvalidStringID;
csStringID celPcPathFinder::id_msg_arrived = csInvalidStringID;
csStringID celPcPathFinder::id_msg_interrupted = csInvalidStringID;

PropertyHolder celPcPathFinder::propinfo;

celPcPathFinder::celPcPathFinder (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  engine = csQueryRegistry<iEngine> (object_reg);

  // For actions.
  if (id_sectorname == csInvalidStringID)
  {
    id_sectorname = pl->FetchStringID ("sectorname");
    id_position = pl->FetchStringID ("position");
    id_pursue_max_prediction = pl->FetchStringID ("pursue_max_prediction");
    id_is_active = pl->FetchStringID ("is_active");
    id_min_distance = pl->FetchStringID ("min_distance");
    id_meshname = pl->FetchStringID ("meshname");
    // for receiving messages
    id_msg_arrived = pl->FetchStringID ("cel.move.arrived");
    id_msg_interrupted = pl->FetchStringID ("cel.move.interrupted");
}

  params = new celOneParameterBlock ();
  params->SetParameterDef (id_meshname);

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.move.pathfinder.action.");
    AddAction (action_seek, "Seek");
    AddAction (action_wander, "Wander");
    AddAction (action_pursue, "Pursue");
    AddAction (action_interrupt, "Interrupt");
  }

  // For properties.
  propinfo.SetCount (4);
  AddProperty (propid_position, "position",
    CEL_DATA_VECTOR3, true, "Desired end position.", &position);
  AddProperty (propid_pursue_max_prediction, "pursue_max_prediction",
    CEL_DATA_FLOAT, false, "Pursue Max Prediction.", &pursue_max_prediction);
  AddProperty (propid_is_active, "is_active",
    CEL_DATA_BOOL, true, "Is pathfinding activated?", &is_active);

  delay_recheck = 20;
  random.Initialize();
  min_distance = 2.0;

  cur_path = 0;
  cur_path = scfCreateInstance<iCelPath> ("cel.celpath");
  if(!cur_path)
    Error ("Loading celPath in PathFinder!");
  //goal = 0;
  /*if(!goal)
    Error ("Loading celNode in PathFinder!");*/
}

celPcPathFinder::~celPcPathFinder ()
{
  if (pl)
    pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  delete params;
}

void celPcPathFinder::SendMessage (const char* msgold, const char* msg,
    csRef<iMessageDispatcher>& dispatcher, const char* meshname)
{
  csRef<iCelEntity> ref = (iCelEntity*)entity;
  if (meshname)
    params->GetParameter (0).Set (meshname);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage (msgold, this, ret, meshname ? params : 0);
  }
  if (!dispatcher)
  {
    dispatcher = entity->QueryMessageChannel ()->
      CreateMessageDispatcher (this, pl->FetchStringID (msg));
    if (!dispatcher) return;
  }
  dispatcher->SendMessage (params);
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
  if (!pclinmove || !pcactormove || !pcsteer)
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

  if (celgraph->ShortestPath(from, goal, cur_path))
  {
    pcsteer->Interrupt();
    FollowPath();
    return true;
  }
  return false;
}

bool celPcPathFinder::Wander (size_t distance)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove || !pcactormove || !pcsteer)
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
  pcsteer->Interrupt();
  FollowPath();

  return true;
}

bool celPcPathFinder :: Pursue (iCelEntity* target, float max_prediction)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove || !pcactormove || !pcsteer)
    return false;

  Interrupt();

  current_action = action_pursue;
  pursue_target = target;
  pursue_max_prediction = max_prediction;
  is_active = true;

  csWeakRef<iPcLinearMovement> targetlinmove;
  float yrot;

  targetlinmove = celQueryPropertyClassEntity<iPcLinearMovement> (target);
  targetlinmove->GetLastFullPosition (position, yrot, sector);

  csVector3 cur_position;
  iSector* cur_sector;

  pclinmove->GetLastFullPosition (cur_position, yrot, cur_sector);

  iCelNode* from = celgraph->GetClosest(cur_position);
  goal = celgraph->GetClosest(position);

  if(celgraph->ShortestPath(from, goal, cur_path))
  {
    pcsteer->Interrupt();
    FollowPath();
    return true;
  }
  return false;
}

bool celPcPathFinder :: FollowCyclicPath (iCelPath* path)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove || !pcactormove || !pcsteer)
    return false;

  Interrupt();

  is_active = true;
  cur_path = path;
  current_action = action_cyclic;
  goal->SetMapNode(cur_path->GetLast());

  FollowPath();
  return true;
}

bool celPcPathFinder :: FollowOneWayPath (iCelPath* path)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove || !pcactormove || !pcsteer)
    return false;

  Interrupt();

  is_active = true;
  cur_path = path;
  current_action = action_one_way;
  goal->SetMapNode(cur_path->GetLast());
  FollowPath();
  return true;
}

bool celPcPathFinder :: FollowTwoWayPath (iCelPath* path)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove || !pcactormove || !pcsteer)
    return false;

  Interrupt();

  is_active = true;
  cur_path = path;
  current_action = action_two_way;
  goal->SetMapNode(cur_path->GetLast());
  FollowPath();
  return true;
}

bool celPcPathFinder::FollowPath ()
{
  if (!pclinmove || !pcactormove || !pcsteer)
    return false;

  csVector3 cur_position;

  if(goal->GetMapNode() == cur_path->Current())
  {
    switch (current_action)
    {
      case action_seek:
      {
        pcsteer->Seek(sector, position);
        Interrupt();
        return true;
      }
      case action_pursue:
      {
        pcsteer->Pursue(pursue_target, pursue_max_prediction);
        Interrupt();
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
        pcsteer->CheckArrivalOn(min_distance);
        Interrupt();
        return true;
      }
      case action_two_way:
      {
        cur_path->Invert();
        cur_path->Restart();
        goal->SetMapNode(cur_path->GetLast());
        break;
      }
      case action_interrupt:
        Interrupt ();
        return true;
      default:
        return false;
    }
  }
  else
  {
    csVector3 target_position = cur_path->CurrentPosition();
    iSector* target_sector = cur_path->CurrentSector();

    pcsteer->Seek(target_sector, target_position);
    entity->QueryMessageChannel()->Subscribe(this,"cel.move.arrived");
    entity->QueryMessageChannel()->Subscribe(this,"cel.move.interrupted");
  }

  //pl->CallbackOnce ((iCelTimerListener*)this, delay_recheck, CEL_EVENT_PRE);

  return true;
}

bool celPcPathFinder::ReceiveMessage (csStringID msg_id, iMessageSender* sender,
		celData& ret, iCelParameterBlock* params)
{
  if (celPcCommon::ReceiveMessage (msg_id, sender, ret, params))
    return true;

  if (msg_id == id_msg_arrived)
  {
    if(cur_path->HasNext())
    {
      iMapNode* node = cur_path->Next();
      csVector3 target_position = node->GetPosition();
      iSector* target_sector = node->GetSector();
      pcsteer->Seek(target_sector, target_position);
    }
    else
    {
      Interrupt();
    }
    return true;
  }
  else if (msg_id == id_msg_interrupted)
  {
    Interrupt();
    return true;
  }
  return false;
}

void celPcPathFinder::StopTracking ()
{
  if (!is_active)
    return;
  is_active = false;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
}


void celPcPathFinder::Interrupt ()
{
  if (is_active)
  {
    StopTracking ();
    // XXX only send the following if not interrupted from steering?
    // SendMessage ("pcpathfinder_interrupted", "cel.move.interrupted",
    // dispatcher_interrupted);
  }
}

bool celPcPathFinder::PerformActionIndexed (int idx, iCelParameterBlock* params,
  celData& ret)
{
  switch (idx)
  {
  case action_seek:
    {
      csString sectorname;
      if (!Fetch (sectorname, params, id_sectorname)) return false;
      csVector3 position;
      if (!Fetch (position, params, id_position)) return false;
      iSector* s = engine->FindSector (sectorname);
      if (!s)
        return Error ("Can't find sector '%s' for Seek!", sectorname.GetData ());
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
    pcactormove = celQueryPropertyClassEntity<iPcActorMove> (entity);
    pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    pcsteer = celQueryPropertyClassEntity<iPcSteer> (entity);
  }
}

void celPcPathFinder::SetGraph (iCelGraph* graph)
{
  celgraph = graph;
}

//---------------------------------------------------------------------------

