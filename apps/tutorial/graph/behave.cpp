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
    printf ("  child %d is '%s'\n", i, child->GetName ());
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
    pclinmove->SetVelocity (csVector3 (0, .1f, 0));
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
  id_par_elapsedticks = pl->FetchStringID ("cel.parameter.elapsedticks");

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
	//with radius 10.0 and weight 1.0

	pcsteer->CohesionOn(entities, 10.0f, 1.0f);
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


  csRef<iMapNode> n0;
  n0.AttachNew(new csMapNode("n0"));
  csRef<iCelNode> gn0 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v0(-4.231915, -0.010000, 17.180994);
  n0->SetPosition(v0);
  gn0->SetMapNode(n0);
  celgraph->AddNode(gn0);

  csRef<iMapNode> n1;
  n1.AttachNew(new csMapNode("n1"));
  csRef<iCelNode> gn1 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v1(-4.430651, -0.010000, 14.289759);
  n1->SetPosition(v1);
  gn1->SetMapNode(n1);
  celgraph->AddNode(gn1);

  csRef<iMapNode> n2;
  n2.AttachNew(new csMapNode("n2"));
  csRef<iCelNode> gn2 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v2(1.032714, -0.010000, 14.351811);
  n2->SetPosition(v2);
  gn2->SetMapNode(n2);
  celgraph->AddNode(gn2);

  csRef<iMapNode> n3;
  n3.AttachNew(new csMapNode("n3"));
  csRef<iCelNode> gn3 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v3(7.803181, -0.010000, 14.259256);
  n3->SetPosition(v3);
  gn3->SetMapNode(n3);
  celgraph->AddNode(gn3);

  csRef<iMapNode> n4;
  n4.AttachNew(new csMapNode("n4"));
  csRef<iCelNode> gn4 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v4(14.358644, -0.010000, 14.077885);
  n4->SetPosition(v4);
  gn4->SetMapNode(n4);
  celgraph->AddNode(gn4);

  csRef<iMapNode> n5;
  n5.AttachNew(new csMapNode("n5"));
  csRef<iCelNode> gn5 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v5(14.484720, -0.010000, 10.981868);
  n5->SetPosition(v5);
  gn5->SetMapNode(n5);
  celgraph->AddNode(gn5);

  csRef<iMapNode> n6;
  n6.AttachNew(new csMapNode("n6"));
  csRef<iCelNode> gn6 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v6(8.788480, -0.010000, 10.698524);
  n6->SetPosition(v6);
  gn6->SetMapNode(n6);
  celgraph->AddNode(gn6);

  csRef<iMapNode> n7;
  n7.AttachNew(new csMapNode("n7"));
  csRef<iCelNode> gn7 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v7(8.669841, -0.010000, 6.518142);
  n7->SetPosition(v7);
  gn7->SetMapNode(n7);
  celgraph->AddNode(gn7);

  csRef<iMapNode> n8;
  n8.AttachNew(new csMapNode("n8"));
  csRef<iCelNode> gn8 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v8(14.774999, -0.010000, 6.613670);
  n8->SetPosition(v8);
  gn8->SetMapNode(n8);
  celgraph->AddNode(gn8);
  
  csRef<iMapNode> n9;
  n9.AttachNew(new csMapNode("n9"));
  csRef<iCelNode> gn9 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v9(15.233424, -0.010000, 1.920429);
  n9->SetPosition(v9);
  gn9->SetMapNode(n9);
  celgraph->AddNode(gn9);

  csRef<iMapNode> n10;
  n10.AttachNew(new csMapNode("n10"));
  csRef<iCelNode> gn10 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v10(9.145017, -0.010000, 1.344137);
  n10->SetPosition(v10);
  gn10->SetMapNode(n10);
  celgraph->AddNode(gn10);

  csRef<iMapNode> n11;
  n11.AttachNew(new csMapNode("n11"));
  csRef<iCelNode> gn11 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v11(2.833345, -0.010000, 1.165757);
  n11->SetPosition(v11);
  gn11->SetMapNode(n11);
  celgraph->AddNode(gn11);

  csRef<iMapNode> n12;
  n12.AttachNew(new csMapNode("n12"));
  csRef<iCelNode> gn12 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v12(2.356894, -0.010000, 6.514668);
  n12->SetPosition(v12);
  gn12->SetMapNode(n12);
  celgraph->AddNode(gn12);

  csRef<iMapNode> n13;
  n13.AttachNew(new csMapNode("n13"));
  csRef<iCelNode> gn13 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v13(-2.893554, -0.010000, 6.227006);
  n13->SetPosition(v13);
  gn13->SetMapNode(n13);
  celgraph->AddNode(gn13);

  csRef<iMapNode> n14;
  n14.AttachNew(new csMapNode("n14"));
  csRef<iCelNode> gn14 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v14(-2.903145, -0.010000, 1.717835);
  n14->SetPosition(v14);
  gn14->SetMapNode(n14);
  celgraph->AddNode(gn14);

  csRef<iMapNode> n15;
  n15.AttachNew(new csMapNode("n15"));
  csRef<iCelNode> gn15 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v15(-11.967916, -0.010000, 0.879896);
  n15->SetPosition(v15);
  gn15->SetMapNode(n15);
  celgraph->AddNode(gn15);

  csRef<iMapNode> n16;
  n16.AttachNew(new csMapNode("n16"));
  csRef<iCelNode> gn16 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v16(-15.036117, -0.010000, 1.189961);
  n16->SetPosition(v16);
  gn16->SetMapNode(n16);
  celgraph->AddNode(gn16);

  csRef<iMapNode> n17;
  n17.AttachNew(new csMapNode("n17"));
  csRef<iCelNode> gn17 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v17(-15.543516, -0.010000, 7.151457);
  n17->SetPosition(v17);
  gn17->SetMapNode(n17);
  celgraph->AddNode(gn17);

  csRef<iMapNode> n18;
  n18.AttachNew(new csMapNode("n18"));
  csRef<iCelNode> gn18 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v18(-15.300305, -0.010000, 15.084600);
  n18->SetPosition(v18);
  gn18->SetMapNode(n18);
  celgraph->AddNode(gn18);

  csRef<iMapNode> n19;
  n19.AttachNew(new csMapNode("n19"));
  csRef<iCelNode> gn19 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v19(-12.349187, -0.010000, -3.802047);
  n19->SetPosition(v19);
  gn19->SetMapNode(n19);
  celgraph->AddNode(gn19);

  csRef<iMapNode> n20;
  n20.AttachNew(new csMapNode("n20"));
  csRef<iCelNode> gn20 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v20(-13.103723, -0.010000, -15.752377);
  n20->SetPosition(v20);
  gn20->SetMapNode(n20);
  celgraph->AddNode(gn20);

  csRef<iMapNode> n21;
  n21.AttachNew(new csMapNode("n21"));
  csRef<iCelNode> gn21 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v21(-4.216733, -0.010000, -16.036676);
  n21->SetPosition(v21);
  gn21->SetMapNode(n21);
  celgraph->AddNode(gn21);

  csRef<iMapNode> n22;
  n22.AttachNew(new csMapNode("n22"));
  csRef<iCelNode> gn22 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v22(-3.987110, -0.010000, -7.525320);
  n22->SetPosition(v22);
  gn22->SetMapNode(n22);
  celgraph->AddNode(gn22);

  csRef<iMapNode> n23;
  n23.AttachNew(new csMapNode("n23"));
  csRef<iCelNode> gn23 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v23(11.559246, -0.010000, -8.344143);
  n23->SetPosition(v23);
  gn23->SetMapNode(n23);
  celgraph->AddNode(gn23);

  csRef<iMapNode> n24;
  n24.AttachNew(new csMapNode("n24"));
  csRef<iCelNode> gn24 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v24(2.692560, -0.010000, -15.649880);
  n24->SetPosition(v24);
  gn24->SetMapNode(n24);
  celgraph->AddNode(gn24);

  csRef<iMapNode> n25;
  n25.AttachNew(new csMapNode("n25"));
  csRef<iCelNode> gn25 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v25(9.992839, -0.010000, -15.765086);
  n25->SetPosition(v25);
  gn25->SetMapNode(n25);
  celgraph->AddNode(gn25);

  csRef<iMapNode> n26;
  n26.AttachNew(new csMapNode("n26"));
  csRef<iCelNode> gn26 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v26(10.636127, -0.010000, -20.355223);
  n26->SetPosition(v26);
  gn26->SetMapNode(n26);
  celgraph->AddNode(gn26);

  csRef<iMapNode> n27;
  n27.AttachNew(new csMapNode("n27"));
  csRef<iCelNode> gn27 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v27(-4.475250, -0.010000, -25.567820);
  n27->SetPosition(v27);
  gn27->SetMapNode(n27);
  celgraph->AddNode(gn27);

  csRef<iMapNode> n28;
  n28.AttachNew(new csMapNode("n28"));
  csRef<iCelNode> gn28 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v28(10.864666, -0.010000, -29.258404);
  n28->SetPosition(v28);
  gn28->SetMapNode(n28);
  celgraph->AddNode(gn28);

  csRef<iMapNode> n29;
  n29.AttachNew(new csMapNode("n29"));
  csRef<iCelNode> gn29 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v29(17.533041, -0.010000, -26.618723);
  n29->SetPosition(v29);
  gn29->SetMapNode(n29);
  celgraph->AddNode(gn29);

  csRef<iMapNode> n30;
  n30.AttachNew(new csMapNode("n30"));
  csRef<iCelNode> gn30 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v30(31.384747, -0.010000, -25.544930);
  n30->SetPosition(v30);
  gn30->SetMapNode(n30);
  celgraph->AddNode(gn30);

  csRef<iMapNode> n31;
  n31.AttachNew(new csMapNode("n31"));
  csRef<iCelNode> gn31 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v31(40.078514, -0.010000, -25.190439);
  n31->SetPosition(v31);
  gn31->SetMapNode(n31);
  celgraph->AddNode(gn31);

  csRef<iMapNode> n32;
  n32.AttachNew(new csMapNode("n32"));
  csRef<iCelNode> gn32 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v32(40.769016, -0.010000, -6.522462);
  n32->SetPosition(v32);
  gn32->SetMapNode(n32);
  celgraph->AddNode(gn32);

  csRef<iMapNode> n33;
  n33.AttachNew(new csMapNode("n33"));
  csRef<iCelNode> gn33 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v33(35.519592, -0.010000, -6.484916);
  n33->SetPosition(v33);
  gn33->SetMapNode(n33);
  celgraph->AddNode(gn33);

  csRef<iMapNode> n34;
  n34.AttachNew(new csMapNode("n34"));
  csRef<iCelNode> gn34 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v34(35.171139, -0.010000, 25.075426);
  n34->SetPosition(v34);
  gn34->SetMapNode(n34);
  celgraph->AddNode(gn34);

  csRef<iMapNode> n35;
  n35.AttachNew(new csMapNode("n35"));
  csRef<iCelNode> gn35 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v35(41.059860, -0.010000, 25.295612);
  n35->SetPosition(v35);
  gn35->SetMapNode(n35);
  celgraph->AddNode(gn35);

  csRef<iMapNode> n36;
  n36.AttachNew(new csMapNode("n36"));
  csRef<iCelNode> gn36 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v36(37.357365, -0.010000, 48.972939);
  n36->SetPosition(v36);
  gn36->SetMapNode(n36);
  celgraph->AddNode(gn36);

  csRef<iMapNode> n37;
  n37.AttachNew(new csMapNode("n37"));
  csRef<iCelNode> gn37 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v37(20.817465, -0.010000, 50.163624);
  n37->SetPosition(v37);
  gn37->SetMapNode(n37);
  celgraph->AddNode(gn37);

  csRef<iMapNode> n38;
  n38.AttachNew(new csMapNode("n38"));
  csRef<iCelNode> gn38 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v38(-2.660369, -0.010000, 50.823647);
  n38->SetPosition(v38);
  gn38->SetMapNode(n38);
  celgraph->AddNode(gn38);

  csRef<iMapNode> n39;
  n39.AttachNew(new csMapNode("n39"));
  csRef<iCelNode> gn39 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v39(-4.116637, -0.010000, 22.688683);
  n39->SetPosition(v39);
  gn39->SetMapNode(n39);
  celgraph->AddNode(gn39);


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
  celpath->AddNode(n0);
  celpath->AddNode(n1);
  celpath->AddNode(n2);
  celpath->AddNode(n3);
  celpath->AddNode(n4);
  
  return true;
}


