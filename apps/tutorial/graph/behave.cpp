#include <crystalspace.h>

#include <physicallayer/entity.h>
#include <physicallayer/propclas.h>
#include <propclass/linmove.h>
#include <propclass/prop.h>
#include <propclass/pathfinder.h>

#include "behave.h"

/*
 * This is a test application which serves as a tutorial
 * for pcpathfinder and celgraph. Main functions to look at 
 * are LoadGraph(), 
 * constructor and SendMessage().
 * 
 */

BehaviourLayer::BehaviourLayer (iCelPlLayer* pl) : scfImplementationType (this)
{
  BehaviourLayer::pl = pl;
}

BehaviourLayer::~BehaviourLayer ()
{
}

iCelBehaviour* BehaviourLayer::CreateBehaviour (iCelEntity* entity,
	const char* name)
{
  iCelBehaviour* behave = 0;
  if (!strcmp (name, "level_behave"))
    behave = new BehaviourLevel (entity, this, pl);
  else if (!strcmp (name, "player_behave"))
    behave = new BehaviourPlayer (entity, this, pl);
  else if (!strcmp (name, "box_behave"))
    behave = new BehaviourBox (entity, this, pl);
  else if (!strcmp (name, "badone_behave"))
    behave = new BehaviourBadOne (entity, this, pl);
  else if(!strcmp(name, "pathfinding_behave"))
    behave = new BehaviourPF (entity, this, pl);


  if (behave)
  {
    entity->SetBehaviour (behave);
    // There is now a reference in the entity. We destroy
    // our own reference here.
    behave->DecRef ();
  }
  return behave;
}

//-----------------------------------------------------------------------------

BehaviourCommon::BehaviourCommon (iCelEntity* entity, BehaviourLayer* bl,
  iCelPlLayer* pl) : scfImplementationType (this)
{
  BehaviourCommon::entity = entity;
  BehaviourCommon::bl = bl;
  BehaviourCommon::pl = pl;
}

BehaviourCommon::~BehaviourCommon ()
{
}

bool BehaviourCommon::SendMessage (const char* msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, pc, ret, params, arg);
  va_end (arg);
  return rc;
}

bool BehaviourCommon::SendMessageV (const char* msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg)
{
  csStringID id = pl->FetchStringID (msg_id);
  return SendMessage (id, pc, ret, params, arg);
}

bool BehaviourCommon::SendMessage (csStringID,
	iCelPropertyClass*,
  	celData&, iCelParameterBlock*, va_list)
{
  return false;
}

//-----------------------------------------------------------------------------

bool BehaviourLevel::SendMessage (csStringID msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg)
{
  return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);
}

//-----------------------------------------------------------------------------

BehaviourPlayer::BehaviourPlayer (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl)
  : BehaviourCommon (entity, bl, pl)
{
  id_pccommandinput_forward1 = pl->FetchStringID ("pccommandinput_forward1");
  id_pccommandinput_forward0 = pl->FetchStringID ("pccommandinput_forward0");
  id_pccommandinput_backward1 = pl->FetchStringID ("pccommandinput_backward1");
  id_pccommandinput_backward0 = pl->FetchStringID ("pccommandinput_backward0");
  id_pccommandinput_rotateleft1 = pl->FetchStringID ("pccommandinput_rotateleft1");
  id_pccommandinput_rotateleft0 = pl->FetchStringID ("pccommandinput_rotateleft0");
  id_pccommandinput_rotateright1 = pl->FetchStringID ("pccommandinput_rotateright1");
  id_pccommandinput_rotateright0 = pl->FetchStringID ("pccommandinput_rotateright0");
  id_pccommandinput_cammode1 = pl->FetchStringID ("pccommandinput_cammode1");
  id_pccommandinput_drop1 = pl->FetchStringID ("pccommandinput_drop1");

  id_pcinventory_addchild = pl->FetchStringID ("pcinventory_addchild");
  id_pcinventory_removechild = pl->FetchStringID ("pcinventory_removechild");

  id_pccommandinput_position1 = pl->FetchStringID ("pccommandinput_position1");
}

void BehaviourPlayer::GetActorMove ()
{
  if (!pcactormove)
  {
    pcactormove = CEL_QUERY_PROPCLASS_ENT (entity, iPcActorMove);
  }
}

void BehaviourPlayer::GetInventory ()
{
  if (!pcinventory)
    {
      pcinventory = CEL_QUERY_PROPCLASS_ENT (entity, iPcInventory);
    }
}

void BehaviourPlayer::GetMesh ()
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

void BehaviourPlayer::ShowInventory ()
{
  size_t count = pcinventory->GetEntityCount ();
  size_t i;
  for (i = 0 ; i < count ; i++)
  {
    iCelEntity* child = pcinventory->GetEntity (i);
    printf ("  child %zu is '%s'\n", i, child->GetName ());
  }
}

void BehaviourPlayer::Drop ()
{
  GetInventory ();
  size_t count = pcinventory->GetEntityCount ();
  if (count <= 0)
  {
    printf ("Inventory is empty!\n");
    return;
  }
  iCelEntity* child = pcinventory->GetEntity (0);
  pcinventory->RemoveEntity (child);
  csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (child,
      iPcLinearMovement);
  if (pclinmove)
  {
    GetMesh ();
    // Now we get current position and orientation from player mesh and from
    // that we calculate a spot in front of the player where we will drop down
    // the item.
    csVector3 pos = pcmesh->GetMesh ()->GetMovable ()->GetTransform ()
      .This2Other (csVector3 (0, 2, -2));
    iSector* sector = pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0);
    pclinmove->SetPosition (pos, 0, sector);
    pclinmove->SetBodyVelocity (csVector3 (0, .1f, 0));
    csRef<iPcMesh> pcmesh_child = CEL_QUERY_PROPCLASS_ENT (child, iPcMesh);
    if (pcmesh_child) pcmesh_child->Show ();
  }
}

bool BehaviourPlayer::SendMessage (csStringID msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg)
{
  GetActorMove ();

  if (msg_id == id_pccommandinput_forward1)
    pcactormove->Forward (true);
  else if (msg_id == id_pccommandinput_forward0)
    pcactormove->Forward (false);
  else if (msg_id == id_pccommandinput_backward1)
    pcactormove->Backward (true);
  else if (msg_id == id_pccommandinput_backward0)
    pcactormove->Backward (false);
  else if (msg_id == id_pccommandinput_rotateleft1)
    pcactormove->RotateLeft (true);
  else if (msg_id == id_pccommandinput_rotateleft0)
    pcactormove->RotateLeft (false);
  else if (msg_id == id_pccommandinput_rotateright1)
    pcactormove->RotateRight (true);
  else if (msg_id == id_pccommandinput_rotateright0)
    pcactormove->RotateRight (false);
  else if (msg_id == id_pccommandinput_cammode1)
    pcactormove->ToggleCameraMode ();
  else if (msg_id == id_pccommandinput_drop1)
    Drop ();
  else if (msg_id == id_pcinventory_addchild)
  {
    GetInventory ();
    printf ("Got a new object! Objects in inventory:\n");
    ShowInventory ();
  }
  else if (msg_id == id_pcinventory_removechild)
  {
    GetInventory ();
    printf ("Object removed from inventory! Objects in inventory:\n");
    ShowInventory ();
  }
  else if  (msg_id == id_pccommandinput_position1)
    {
      csRef<iCelEntity> player_entity = pl->FindEntity("player");
      csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (player_entity,
								    iPcLinearMovement);	    
      iSector* sector;
      csVector3 position;
      float rot;

      pclinmove->GetLastFullPosition (position, rot, sector);

      printf("Position .x %f, .y %f, .z%f\n", position.x, position.y, position.z);
    }
  else
    return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);;

  return true;
}

//-----------------------------------------------------------------------------

BehaviourBox::BehaviourBox (iCelEntity* entity, BehaviourLayer* bl,
    iCelPlLayer* pl)
  : BehaviourCommon (entity, bl, pl)
{
  id_pcmeshsel_down = pl->FetchStringID ("pcmeshsel_down");
  GetPlayer ();
}

void BehaviourBox::PickUp ()
{
  if (!player) return;
  csRef<iPcInventory> pcinv = CEL_QUERY_PROPCLASS_ENT (player, iPcInventory);
  if (pcinv)
  {
    pcinv->AddEntity (entity);
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    if (pcmesh) pcmesh->Hide ();
  }
}

void BehaviourBox::GetPlayer ()
{
  if (!pcmeshsel || !player)
  {
    pcmeshsel = CEL_QUERY_PROPCLASS_ENT (entity, iPcMeshSelect);
    player = pl->FindEntity ("player");
    if (!player) return;
    csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (player, iPcCamera);
    if (pccamera)
      pcmeshsel->SetCamera (pccamera);
  }
}

bool BehaviourBox::SendMessage (csStringID msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg)
{
  if (msg_id == id_pcmeshsel_down)
  {
    PickUp ();
    return true;
  }

  return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);
}

//-----------------------------------------------------------------------------

BehaviourBadOne::BehaviourBadOne (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl)
  : BehaviourCommon (entity, bl, pl)
{
  id_pctimer_wakeup = pl->FetchStringID ("pctimer_wakeup");
  id_par_elapsedticks = pl->FetchStringID ("elapsedticks");

  ReadPath ();
} 

static bool GetPropVector (iPcProperties* pcprop, const char* prefix, int i, csVector3& v)
{
  csString propname = prefix;
  propname += i;
  size_t idx = pcprop->GetPropertyIndex (propname);
  if (idx == csArrayItemNotFound) return false;
  if (!pcprop->GetPropertyVector (idx, v)) return false;
  return true;
}

static bool GetPropLong (iPcProperties* pcprop, const char* prefix, int i, long& l)
{
  csString propname = prefix;
  propname += i;
  size_t idx = pcprop->GetPropertyIndex (propname);
  if (idx == csArrayItemNotFound) return false;
  l = pcprop->GetPropertyLong (idx);
  return true;
}

void BehaviourBadOne::ReadPath ()
{
  csRef<iPcProperties> pcprop = CEL_QUERY_PROPCLASS_ENT (entity, iPcProperties);

  // Count the number of points we have.
  int count = 0;
  for (;;)
  {
    csVector3 v;
    if (!GetPropVector (pcprop, "pos", count+1, v)) break;
    count++;
  }

  path.AttachNew (new csPath(count));

  int i = 0;
  long totaltime = 0;
  for (i = 0 ; i < count ; i++)
  {
    csVector3 pos, forward, up;
    GetPropVector (pcprop, "pos", i+1, pos);
    if (!GetPropVector (pcprop, "forward", i+1, forward)) forward.Set (1, 0, 0);
    if (!GetPropVector (pcprop, "up", i+1, up)) up.Set (0, 1, 0);
    long time;
    if (!GetPropLong (pcprop, "time", i+1, time)) time = 1000;
    path->SetPositionVector (i, pos);
    path->SetUpVector (i, up);
    path->SetForwardVector (i, forward);
    path->SetTime (i, float (totaltime) / 1000.0);
    totaltime += time;
  }

  csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcLinearMovement);
  if (pclinmove)
  {
    for (i = 0 ; i < count ; i++)
    {
      pclinmove->SetPathAction (i, "default");
    }
  }
}

void BehaviourBadOne::Restart ()
{
  csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (entity,
      iPcLinearMovement);
  if (pclinmove)
  {
    pclinmove->SetPath (path);
    pclinmove->SetPathTime (0);
  }
}

bool BehaviourBadOne::SendMessage (csStringID msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg)
{
  if (msg_id == id_pctimer_wakeup)
  {
    Restart ();
    return true;
  }

  return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);
}

//-----------------------------------------------------------------------------

BehaviourPF::BehaviourPF (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl)
  : BehaviourCommon (entity, bl, pl)
{
  id_pcsteer_arrived = pl->FetchStringID ("pcsteer_arrived");
  id_pccommandinput_ca1 = pl->FetchStringID ("pccommandinput_ca1");
  id_pccommandinput_cohesion1 = pl->FetchStringID ("pccommandinput_cohesion1");
  id_pccommandinput_separation1 = pl->FetchStringID ("pccommandinput_separation1");
  id_pccommandinput_dm1 = pl->FetchStringID ("pccommandinput_dm1");
  id_pccommandinput_arrival1 = pl->FetchStringID ("pccommandinput_arrival1");
  id_pccommandinput_seek1 = pl->FetchStringID ("pccommandinput_seek1");
  id_pccommandinput_cyclic1 = pl->FetchStringID ("pccommandinput_cyclic1");
  id_pccommandinput_wander1 = pl->FetchStringID ("pccommandinput_wander1");
  id_pccommandinput_pursue1 = pl->FetchStringID ("pccommandinput_pursue1");
  id_pccommandinput_oneway1 = pl->FetchStringID ("pccommandinput_oneway1");
  id_pccommandinput_twoway1 = pl->FetchStringID ("pccommandinput_twoway1");

  arrival = false;
  ca = false;
  cohesion = false;
  separation = false;
  dm = false;

  cur = 0;

  entities = pl->CreateEmptyEntityList();
  csRef<iCelEntity> player_entity = pl->FindEntity("player");
  entities->Add(player_entity);

  LoadGraph();

  csRef<iCelEntity> pf_entity = pl->FindEntity("pf");
  //  csRef<iPcPathFinder> pcpathfinder = CEL_QUERY_PROPCLASS_ENT (pf_entity,
  //						       iPcPathFinder);

}
 
bool BehaviourPF::SendMessage (csStringID msg_id,
				     iCelPropertyClass* pc,
				     celData& ret, iCelParameterBlock* params, va_list arg)
{
  puts ("BehaviourPF");
  /*
   * Here we handle all messages and calls
   * to pcpathfinder
   *
   */

  if (msg_id == id_pcsteer_arrived)
    printf("Arrived\n");
  else if (msg_id == id_pccommandinput_seek1)
    {
      /*
       * In order to seek a target we first have to 
       * calculate its actual position
       *
       */

      printf("Seek\n");
      csRef<iCelEntity> player_entity = pl->FindEntity("player");
      csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (player_entity,
								    iPcLinearMovement);
      iSector* sector;
      csVector3 position;
      csVector3 cur_position;
      float rot;
      
      pclinmove->GetLastFullPosition (position, rot, sector);
      
      csRef<iCelEntity> pf_entity = pl->FindEntity("pf");
      
      csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (pf_entity,
							 iPcSteer);
      
      csRef<iPcLinearMovement> pclinmove2 = CEL_QUERY_PROPCLASS_ENT (pf_entity,
								     iPcLinearMovement);
      
      pclinmove2->GetLastFullPosition (cur_position, rot, sector);

 
    /*
     * Once we have targets position and sector
     * we create a reference to the pcpathfinder instance in our
     * pathfinder entity and call Seek().
     *
     */

      
      csRef<iPcPathFinder> pcpathfinder = CEL_QUERY_PROPCLASS_ENT (pf_entity,
								   iPcPathFinder);

      /*
       * First we call SetGraph in case the reference
       * in pathfinder is not actualized.
       */
     
      pcpathfinder->SetGraph(celgraph);
      pcpathfinder->Seek(sector, position);
      
    }
  else if (msg_id == id_pccommandinput_cyclic1)
    {
      
      printf("Follow Cyclic Path\n");
      csRef<iCelEntity> pf_entity = pl->FindEntity("pf");
    
      csRef<iPcPathFinder> pcpathfinder = CEL_QUERY_PROPCLASS_ENT (pf_entity,
								   iPcPathFinder);
      
      /*
       * First we call SetGraph in case the reference
       * in pathfinder is not actualized.
       */
     
      pcpathfinder->SetGraph(celgraph);
      
      /*
       * We just have to call FollowCyclicPath since celpath was
       * already loaded in LoadGraph().
       */

      pcpathfinder->FollowCyclicPath(celpath);
    
    }
  else if (msg_id == id_pccommandinput_oneway1)
    {
    printf("Follow One Way Path\n");
    csRef<iCelEntity> pf_entity = pl->FindEntity("pf");
    
    csRef<iPcPathFinder> pcpathfinder = CEL_QUERY_PROPCLASS_ENT (pf_entity,
								 iPcPathFinder);
    /*
     * First we call SetGraph in case the reference
     * in pathfinder is not actualized.
     */
     
    pcpathfinder->SetGraph(celgraph);

      /*
       * We just have to call FollowOneWayPath since celpath was
       * already loaded in LoadGraph().
       */

    pcpathfinder->FollowOneWayPath(celpath);

    }
else if (msg_id == id_pccommandinput_twoway1)
    {
    printf("Follow Two Way Path\n");
    csRef<iCelEntity> pf_entity = pl->FindEntity("pf");
    
    csRef<iPcPathFinder> pcpathfinder = CEL_QUERY_PROPCLASS_ENT (pf_entity,
								 iPcPathFinder);
    /*
     * First we call SetGraph in case the reference
     * in pathfinder is not actualized.
     */
    
    pcpathfinder->SetGraph(celgraph);
    
    /*
     * We just have to call FollowOneWayPath since celpath was
     * already loaded in LoadGraph().
     */
    
    celpath->Invert();

    pcpathfinder->FollowTwoWayPath(celpath);

    }
  else if (msg_id == id_pccommandinput_wander1)
    {
      printf("Wander\n");
          
      csRef<iCelEntity> pf_entity = pl->FindEntity("pf");

      csRef<iPcPathFinder> pcpathfinder = CEL_QUERY_PROPCLASS_ENT (pf_entity,
								   iPcPathFinder);

	
    /*
     * First we call SetGraph in case the reference
     * in pathfinder is not actualized.
     */
    
      pcpathfinder->SetGraph(celgraph);
      
      /*
       * We call Wander() which will generate a random path of
       * depth 2.
       */

      pcpathfinder->Wander(2);
}
  else if(msg_id == id_pccommandinput_pursue1)
    {
      csRef<iCelEntity> player_entity = pl->FindEntity("player");
      csRef<iCelEntity> pf_entity = pl->FindEntity("pf");
      csRef<iPcPathFinder> pcpathfinder = CEL_QUERY_PROPCLASS_ENT (pf_entity,
								   iPcPathFinder);    
      
      /*
       * First we call SetGraph in case the reference
       * in pathfinder is not actualized.
       */
      
      pcpathfinder->SetGraph(celgraph);      
      
      /*
       * The pathfinder will continue searching the player
       * until telled otherwise.
       */

      pcpathfinder->Pursue(player_entity, 0.5f);
    }
  else if(msg_id == id_pccommandinput_arrival1)
    {
      csRef<iCelEntity> steering_entity = pl->FindEntity("pf");
      csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
							 iPcSteer);

      if(!arrival){
	//Turns Position Arrival Checking on with a sq radius of 1.0
	pcsteer->CheckArrivalOn(1.0f);
	arrival = true;
	printf("Check Arrival On\n");
      } else {
	pcsteer->CheckArrivalOff();
	arrival = false;
	printf("Check Arrival Off\n");
      }
    }
  else if (msg_id == id_pccommandinput_ca1)
    {
      csRef<iCelEntity> steering_entity = pl->FindEntity("pf");
      csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
							 iPcSteer);

      if(!ca){
	//Turns Collision Avoidance ON
	//with lookahead 3.0 and weight 1.0
	
	pcsteer->CollisionAvoidanceOn(10.0f, 3.0f);
	ca = true;
	printf("Collision Avoidance On\n");
	
      } else{
	pcsteer->CollisionAvoidanceOff();
	ca = false;
	printf("Collision Avoidance Off\n");      
      }
    }
  else if (msg_id == id_pccommandinput_cohesion1)
    {
      csRef<iCelEntity> steering_entity = pl->FindEntity("pf");
      csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
							 iPcSteer);
      if(!cohesion){
	//Turns Cohesion ON
	//with radius 10.0, max radius 100.0 and weight 1.0

	pcsteer->CohesionOn(entities, 10.0f, 100.0, 1.0f);
	cohesion = true;
	printf("Cohesion On\n");
	
      } else{
	pcsteer->CohesionOff();
	cohesion = false;
	printf("Cohesion Off\n");      
      }
    } else if (msg_id == id_pccommandinput_separation1)
    {
      csRef<iCelEntity> steering_entity = pl->FindEntity("pf");
      csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
							 iPcSteer);
      if(!separation){
	//Turns Separation On
	//with radius 3.0 and weight 1.0
	
       	pcsteer->SeparationOn(entities, 1.0f, 3.0f);
	separation = true;
	printf("Separation On\n");
	
      } else{
	pcsteer->SeparationOff();
	separation = false;
	printf("Separation Off\n");      
      }
    }
 else if (msg_id == id_pccommandinput_dm1)
    {
      csRef<iCelEntity> steering_entity = pl->FindEntity("pf");
      csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
							 iPcSteer);

      if(!dm){
	//Turns Direction Matching On
	//With weight 1.0
	
	pcsteer->DirectionMatchingOn(entities, 1.0f);
	dm = true;
	printf("Direction Matching On\n");
	
      } else{
	pcsteer->DirectionMatchingOff();
	dm = false;
	printf("Direction Matching Off\n");      
      }
    }
 else
 {
    return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);
  }
  return true;
}


bool BehaviourPF::LoadGraph ()
{
  /*
   * To load a celgraph we first create an instance of iCelGraph
   *
   */
    
  celgraph = scfCreateInstance<iCelGraph> ("cel.celgraph");
  if(!celgraph)
    fprintf(stderr, "Error Loading CelGraph!\n");

  /*
   * Then we create and load each Node.
   *
   */


  csVector3 v0(-4.231915f, -0.010000f, 17.180994f);
  csRef<iCelNode> gn0 = celgraph->CreateNode("n0",v0);

  csVector3 v1(-4.430651f, -0.010000f, 14.289759f);
  csRef<iCelNode> gn1 = celgraph->CreateNode("n1",v1);

  csVector3 v2(1.032714f, -0.010000f, 14.351811f);
  csRef<iCelNode> gn2 = celgraph->CreateNode("n2",v2);

  csVector3 v3(7.803181f, -0.010000f, 14.259256f);
  csRef<iCelNode> gn3 = celgraph->CreateNode("n3",v3);

  csVector3 v4(14.358644f, -0.010000f, 14.077885f);
  csRef<iCelNode> gn4 = celgraph->CreateNode("n4",v4);

  csVector3 v5(14.484720f, -0.010000f, 10.981868f);
  csRef<iCelNode> gn5 = celgraph->CreateNode("n5",v5);

  csVector3 v6(8.788480f, -0.010000f, 10.698524f);
  csRef<iCelNode> gn6 = celgraph->CreateNode("n6",v6);

  csVector3 v7(8.669841f, -0.010000f, 6.518142f);
  csRef<iCelNode> gn7 = celgraph->CreateNode("n7",v7);

  csVector3 v8(14.774999f, -0.010000f, 6.613670f);
  csRef<iCelNode> gn8 = celgraph->CreateNode("n8",v8);
  
  csVector3 v9(15.233424f, -0.010000f, 1.920429f);
  csRef<iCelNode> gn9 = celgraph->CreateNode("n9",v9);

  csVector3 v10(9.145017f, -0.010000f, 1.344137f);
  csRef<iCelNode> gn10 = celgraph->CreateNode("n10",v10);

  csVector3 v11(2.833345f, -0.010000f, 1.165757f);
  csRef<iCelNode> gn11 = celgraph->CreateNode("n11",v11);

  csVector3 v12(2.356894f, -0.010000f, 6.514668f);
  csRef<iCelNode> gn12 = celgraph->CreateNode("n12",v12);

  csVector3 v13(-2.893554f, -0.010000f, 6.227006f);
  csRef<iCelNode> gn13 = celgraph->CreateNode("n13",v13);

  csVector3 v14(-2.903145f, -0.010000f, 1.717835f);
  csRef<iCelNode> gn14 = celgraph->CreateNode("n14",v14);

  csVector3 v15(-11.967916f, -0.010000f, 0.879896f);
  csRef<iCelNode> gn15 = celgraph->CreateNode("n15",v15);

  csVector3 v16(-15.036117f, -0.010000f, 1.189961f);
  csRef<iCelNode> gn16 = celgraph->CreateNode("n16",v16);

  csVector3 v17(-15.543516f, -0.010000f, 7.151457f);
  csRef<iCelNode> gn17 = celgraph->CreateNode("n17",v17);

  csVector3 v18(-15.300305f, -0.010000f, 15.084600f);
  csRef<iCelNode> gn18 = celgraph->CreateNode("n18",v18);

  csVector3 v19(-12.349187f, -0.010000f, -3.802047f);
  csRef<iCelNode> gn19 = celgraph->CreateNode("n19",v19);

  csVector3 v20(-13.103723f, -0.010000f, -15.752377f);
  csRef<iCelNode> gn20 = celgraph->CreateNode("n20",v20);

  csVector3 v21(-4.216733f, -0.010000f, -16.036676f);
  csRef<iCelNode> gn21 = celgraph->CreateNode("n21",v21);

  csVector3 v22(-3.987110f, -0.010000f, -7.525320f);
  csRef<iCelNode> gn22 = celgraph->CreateNode("n22",v22);

  csVector3 v23(11.559246f, -0.010000f, -8.344143f);
  csRef<iCelNode> gn23 = celgraph->CreateNode("n23",v23);

  csVector3 v24(2.692560f, -0.010000f, -15.649880f);
  csRef<iCelNode> gn24 = celgraph->CreateNode("n24",v24);

  csVector3 v25(9.992839f, -0.010000f, -15.765086f);
  csRef<iCelNode> gn25 = celgraph->CreateNode("n25",v25);

  csVector3 v26(10.636127f, -0.010000f, -20.355223f);
  csRef<iCelNode> gn26 = celgraph->CreateNode("n26",v26);

  csVector3 v27(-4.475250f, -0.010000f, -25.567820f);
  csRef<iCelNode> gn27 = celgraph->CreateNode("n27",v27);

  csVector3 v28(10.864666f, -0.010000f, -29.258404f);
  csRef<iCelNode> gn28 = celgraph->CreateNode("n28",v28);

  csVector3 v29(17.533041f, -0.010000f, -26.618723f);
  csRef<iCelNode> gn29 = celgraph->CreateNode("n29",v29);

  csVector3 v30(31.384747f, -0.010000f, -25.544930f);
  csRef<iCelNode> gn30 = celgraph->CreateNode("n30",v30);

  csVector3 v31(40.078514f, -0.010000f, -25.190439f);
  csRef<iCelNode> gn31 = celgraph->CreateNode("n31",v31);

  csVector3 v32(40.769016f, -0.010000f, -6.522462f);
  csRef<iCelNode> gn32= celgraph->CreateNode("n32",v32);

  csVector3 v33(35.519592f, -0.010000f, -6.484916f);
  csRef<iCelNode> gn33 = celgraph->CreateNode("n33",v33);

  csVector3 v34(35.171139f, -0.010000f, 25.075426f);
  csRef<iCelNode> gn34 = celgraph->CreateNode("n34",v34);

  csVector3 v35(41.059860f, -0.010000f, 25.295612f);
  csRef<iCelNode> gn35 = celgraph->CreateNode("n35",v35);

  csVector3 v36(37.357365f, -0.010000f, 48.972939f);
  csRef<iCelNode> gn36 = celgraph->CreateNode("n36",v36);

  csVector3 v37(20.817465f, -0.010000f, 50.163624f);
  csRef<iCelNode> gn37 = celgraph->CreateNode("n37",v37);

  csVector3 v38(-2.660369f, -0.010000f, 50.823647f);
  csRef<iCelNode> gn38 = celgraph->CreateNode("n38",v38);

  csVector3 v39(-4.116637f, -0.010000f, 22.688683f);
  csRef<iCelNode> gn39 = celgraph->CreateNode("n39",v39);


  /*
   * Finally, we add all the edges we need between
   * the already added nodes.
   */

  celgraph->AddEdge(gn0, gn1, true);
  celgraph->AddEdge(gn1, gn2, true);
  celgraph->AddEdge(gn2, gn3, true);
  celgraph->AddEdge(gn3, gn4, true);
  celgraph->AddEdge(gn3, gn6, true);
  celgraph->AddEdge(gn4, gn5, true);
  celgraph->AddEdge(gn5, gn6, true);
  celgraph->AddEdge(gn5, gn8, true);
  celgraph->AddEdge(gn6, gn7, true);
  celgraph->AddEdge(gn8, gn7, true);
  celgraph->AddEdge(gn8, gn9, true);
  celgraph->AddEdge(gn7, gn10, true);
  celgraph->AddEdge(gn9, gn10, true);
  celgraph->AddEdge(gn10, gn11, true);
  celgraph->AddEdge(gn7, gn12, true);
  celgraph->AddEdge(gn12, gn11, true);
  celgraph->AddEdge(gn12, gn13, true);
  celgraph->AddEdge(gn11, gn14, true);
  celgraph->AddEdge(gn13, gn14, true);
  celgraph->AddEdge(gn14, gn15, true);
  celgraph->AddEdge(gn15, gn16, true);
  celgraph->AddEdge(gn16, gn17, true);
  celgraph->AddEdge(gn17, gn18, true);
  celgraph->AddEdge(gn15, gn19, true);
  celgraph->AddEdge(gn19, gn20, true);
  celgraph->AddEdge(gn20, gn21, true);
  celgraph->AddEdge(gn21, gn22, true);
  celgraph->AddEdge(gn22, gn23, true);
  celgraph->AddEdge(gn21, gn24, true);
  celgraph->AddEdge(gn24, gn25, true);
  celgraph->AddEdge(gn25, gn26, true);
  celgraph->AddEdge(gn26, gn27, true);
  celgraph->AddEdge(gn26, gn28, true);
  celgraph->AddEdge(gn27, gn28, true);
  celgraph->AddEdge(gn26, gn29, true);
  celgraph->AddEdge(gn28, gn29, true);
  celgraph->AddEdge(gn29, gn30, true);
  celgraph->AddEdge(gn30, gn31, true);
  celgraph->AddEdge(gn30, gn33, true);
  celgraph->AddEdge(gn31, gn32, true);
  celgraph->AddEdge(gn32, gn33, true);
  celgraph->AddEdge(gn32, gn35, true);
  celgraph->AddEdge(gn33, gn34, true);
  celgraph->AddEdge(gn35, gn34, true);
  celgraph->AddEdge(gn34, gn36, true);
  celgraph->AddEdge(gn35, gn36, true);
  celgraph->AddEdge(gn36, gn37, true);
  celgraph->AddEdge(gn37, gn38, true);
  celgraph->AddEdge(gn38, gn39, true);
  celgraph->AddEdge(gn0, gn39, true);

  celgraph->AddEdge(gn1, gn0 , true);
  celgraph->AddEdge(gn2, gn1 , true);
  celgraph->AddEdge(gn3, gn2 , true);
  celgraph->AddEdge(gn4, gn3 , true);
  celgraph->AddEdge(gn6, gn3 , true);
  celgraph->AddEdge(gn5, gn4 , true);
  celgraph->AddEdge(gn6, gn5 , true);
  celgraph->AddEdge(gn8, gn5 , true);
  celgraph->AddEdge(gn7, gn6 , true);
  celgraph->AddEdge(gn7, gn8 , true);
  celgraph->AddEdge(gn9, gn8 , true);
  celgraph->AddEdge(gn10, gn7, true);
  celgraph->AddEdge(gn10, gn9, true);
  celgraph->AddEdge(gn11, gn10, true);
  celgraph->AddEdge(gn12, gn7, true);
  celgraph->AddEdge(gn11, gn12, true);
  celgraph->AddEdge(gn13, gn12, true);
  celgraph->AddEdge(gn14, gn11, true);
  celgraph->AddEdge(gn14, gn13, true);
  celgraph->AddEdge(gn15, gn14, true);
  celgraph->AddEdge(gn16, gn15, true);
  celgraph->AddEdge(gn17, gn16, true);
  celgraph->AddEdge(gn18, gn17, true);
  celgraph->AddEdge(gn19, gn15, true);
  celgraph->AddEdge(gn20, gn19, true);
  celgraph->AddEdge(gn21, gn20, true);
  celgraph->AddEdge(gn22, gn21, true);
  celgraph->AddEdge(gn23, gn22, true);
  celgraph->AddEdge(gn24, gn21, true);
  celgraph->AddEdge(gn25, gn24, true);
  celgraph->AddEdge(gn26, gn25, true);
  celgraph->AddEdge(gn27, gn26, true);
  celgraph->AddEdge(gn28, gn26, true);
  celgraph->AddEdge(gn28, gn27, true);
  celgraph->AddEdge(gn29, gn26, true);
  celgraph->AddEdge(gn29, gn28, true);
  celgraph->AddEdge(gn30, gn29, true);
  celgraph->AddEdge(gn31, gn30, true);
  celgraph->AddEdge(gn33, gn30, true);
  celgraph->AddEdge(gn32, gn31, true);
  celgraph->AddEdge(gn33, gn32, true);
  celgraph->AddEdge(gn35, gn32, true);
  celgraph->AddEdge(gn34, gn33, true);
  celgraph->AddEdge(gn34, gn35, true);
  celgraph->AddEdge(gn36, gn34, true);
  celgraph->AddEdge(gn36, gn35, true);
  celgraph->AddEdge(gn37, gn36, true);
  celgraph->AddEdge(gn38, gn37, true);
  celgraph->AddEdge(gn39, gn38, true);
  celgraph->AddEdge(gn39, gn0, true);

  /*celgraph->AddEdge(gn1, gn2, true);
  celgraph->AddEdge(gn2, gn3, true);
  celgraph->AddEdge(gn3, gn4, true);
  celgraph->AddEdge(gn4, gn5, true);
  celgraph->AddEdge(gn5, gn6, tru(e);  
  */

  /*
   * Now, we load a small path so we can later
   * work with FollowOneWayPath, FollowCyclicPath
   * and FollowTwoWayPath.
   *
   * Notice that celpath consists of a set of 
   * csMapNodes rather than iCelNodes.
   */

  celpath = scfCreateInstance<iCelPath> ("cel.celpath");
  if(!celpath)
    fprintf(stderr, "Error Loading CelPath!\n");
  celpath->AddNode(gn0->GetMapNode());
  celpath->AddNode(gn1->GetMapNode());
  celpath->AddNode(gn2->GetMapNode());
  celpath->AddNode(gn3->GetMapNode());
  celpath->AddNode(gn4->GetMapNode());
  
  return true;
}


