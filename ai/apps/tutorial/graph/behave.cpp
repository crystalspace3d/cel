#include <crystalspace.h>

#include <physicallayer/entity.h>
#include <physicallayer/propclas.h>
#include <propclass/linmove.h>
#include <propclass/prop.h>
#include <propclass/pathfinder.h>

#include "behave.h"

SCF_IMPLEMENT_IBASE (BehaviourLayer)
  SCF_IMPLEMENTS_INTERFACE (iCelBlLayer)
SCF_IMPLEMENT_IBASE_END

BehaviourLayer::BehaviourLayer (iCelPlLayer* pl)
{
  SCF_CONSTRUCT_IBASE (0);
  BehaviourLayer::pl = pl;
}

BehaviourLayer::~BehaviourLayer ()
{
  SCF_DESTRUCT_IBASE ();
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

SCF_IMPLEMENT_IBASE (BehaviourCommon)
  SCF_IMPLEMENTS_INTERFACE (iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

BehaviourCommon::BehaviourCommon (iCelEntity* entity, BehaviourLayer* bl,
    iCelPlLayer* pl)
{
  SCF_CONSTRUCT_IBASE (0);
  BehaviourCommon::entity = entity;
  BehaviourCommon::bl = bl;
  BehaviourCommon::pl = pl;
}

BehaviourCommon::~BehaviourCommon ()
{
  SCF_DESTRUCT_IBASE ();
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
  id_pccommandinput_flee1 = pl->FetchStringID ("pccommandinput_flee1");
  id_pccommandinput_wander1 = pl->FetchStringID ("pccommandinput_wander1");
  id_pccommandinput_pursue1 = pl->FetchStringID ("pccommandinput_pursue1");
  id_pccommandinput_next1 = pl->FetchStringID ("pccommandinput_next1");

  arrival = false;
  ca = false;
  cohesion = false;
  separation = false;
  dm = false;

  entities = pl->CreateEmptyEntityList();
  csRef<iCelEntity> player_entity = pl->FindEntity("player");
  entities->Add(player_entity);

  LoadGraph();
  
  csRef<iCelEntity> steering_entity = pl->FindEntity("pf");
  csRef<iPcPathFinder> pcpathfinder = CEL_QUERY_PROPCLASS_ENT (steering_entity,
							       iPcPathFinder);
  //  pcpathfinder->SetGraph(celgraph);
}
 
bool BehaviourPF::SendMessage (csStringID msg_id,
				     iCelPropertyClass* pc,
				     celData& ret, iCelParameterBlock* params, va_list arg)
{
  if (msg_id == id_pcsteer_arrived)
    printf("Arrived\n");
  else if (msg_id == id_pccommandinput_seek1){
    printf("Seek\n");
    csRef<iCelEntity> player_entity = pl->FindEntity("player");
    csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (player_entity,
								  iPcLinearMovement);
    iSector* sector;
    csVector3 position;
    csVector3 cur_position;
    float rot;
    
    pclinmove->GetLastFullPosition (position, rot, sector);
    
    csRef<iCelEntity> steering_entity = pl->FindEntity("pf");

    csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
						       iPcSteer);

    csRef<iPcLinearMovement> pclinmove2 = CEL_QUERY_PROPCLASS_ENT (steering_entity,
								   iPcLinearMovement);

    pclinmove2->GetLastFullPosition (cur_position, rot, sector);

    csRef<iPcPathFinder> pcpathfinder = CEL_QUERY_PROPCLASS_ENT (steering_entity,
								 iPcPathFinder);

    pcpathfinder->SetGraph(celgraph);

    pcpathfinder->Seek(sector, position);
    
    /*csRef <iCelPath> cur_path = scfCreateInstance<iCelPath> ("cel.celpath");

    iCelNode* from = celgraph->GetClosest(cur_position);
    iCelNode* goal = celgraph->GetClosest(position);

    celgraph->ShortestPath(from, goal, cur_path);

      /*      while(cur_path->HasNext()){
	csVector3 vec = cur_path->CurrentPosition();
	printf("x: %f y: %f z: %f\n", vec.x, vec.y, vec.z);
	cur_path->Next();
      }
    }
      */
   
      //      pcsteer->Seek(sector, cur_path->CurrentPosition());
      }
  else if (msg_id == id_pccommandinput_flee1)
    {
    printf("Flee\n");
    csRef<iCelEntity> player_entity = pl->FindEntity("player");
    csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (player_entity,
								  iPcLinearMovement);
    iSector* sector;
    csVector3 position;
    float rot;
    
    pclinmove->GetLastFullPosition (position, rot, sector);
    
    csRef<iCelEntity> steering_entity = pl->FindEntity("pf");
    csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
						       iPcSteer);
    pcsteer->Flee(sector, position);

    }
  else if (msg_id == id_pccommandinput_wander1)
    printf("Wander\n");
  else if(msg_id == id_pccommandinput_pursue1)
    {
      csRef<iCelEntity> player_entity = pl->FindEntity("player");
      csRef<iCelEntity> steering_entity = pl->FindEntity("pf");
      csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
							 iPcSteer);    
      pcsteer->Pursue(player_entity, 0.5f);
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
      csRef<iCelEntity> steering_entity = pl->FindEntity("steer");
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
    return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);
  
  return true;
}


bool BehaviourPF::LoadGraph ()
{
  //csRef<iPluginManager> plugin_mgr = csQueryRegistry<iPluginManager> (object_reg);
  //csRef<iCelGraph> celgraph = csLoadPlugin<iCelGraph> (plugin_mgr, "cel.celgraph");
  
  //  if(celgraph.IsValid())
    
  csRef<iMapNode> n1;
  n1.AttachNew(new csMapNode("n1"));
  csRef<iMapNode> n2;
  n2.AttachNew(new csMapNode("n2"));
  csRef<iMapNode> n3;
  n3.AttachNew(new csMapNode("n3"));
  csRef<iMapNode> n4;
  n4.AttachNew(new csMapNode("n4"));
  csRef<iMapNode> n5;
  n5.AttachNew(new csMapNode("n5"));
  csRef<iMapNode> n6;
  n6.AttachNew(new csMapNode("n6"));
  csRef<iMapNode> n7;
  n7.AttachNew(new csMapNode("n7"));
  
  celgraph = scfCreateInstance<iCelGraph> ("cel.celgraph");
  if(!celgraph)
    fprintf(stderr, "Error Loading CelGraph!\n");

  csRef<iCelNode> gn1 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v1(15.858, 0, 11.138);
  n1->SetPosition(v1);
  gn1->SetMapNode(n1);


  csRef<iCelNode> gn2 = scfCreateInstance<iCelNode> ("cel.celnode");  
  csVector3 v2(15.665, 0, 2.292);
  n2->SetPosition(v2);
  gn2->SetMapNode(n2);
  

  csRef<iCelNode> gn3 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v3(5.641, 0, 2.166);
  n3->SetPosition(v3);
  gn3->SetMapNode(n3);
  

  csRef<iCelNode> gn4 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v4(-10.878, 0, 1.195);
  n4->SetPosition(v4);
  gn4->SetMapNode(n4);  


  csRef<iCelNode> gn5 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v5(-11.645, 0, -15.996);
  n5->SetPosition(v5);
  gn5->SetMapNode(n5);  


  csRef<iCelNode> gn6 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v6(-3.395, 0, -15.892);
  n6->SetPosition(v6);
  gn6->SetMapNode(n6);

  celgraph->AddNode(gn1);
  celgraph->AddNode(gn2);
  celgraph->AddNode(gn3);
  celgraph->AddNode(gn4);
  celgraph->AddNode(gn5);
  celgraph->AddNode(gn6);

  celgraph->AddEdge(gn1, gn2, true);
  celgraph->AddEdge(gn2, gn3, true);
  celgraph->AddEdge(gn3, gn4, true);
  celgraph->AddEdge(gn4, gn5, true);
  celgraph->AddEdge(gn5, gn6, true);  

  csRef<iCelPath> path = scfCreateInstance<iCelPath> ("cel.celpath");
  if(!path)
    fprintf(stderr, "Error Loading celPath!\n");

  if(!celgraph->ShortestPath(gn1, gn2, path))
    fprintf(stderr, "Error Loading celPath!\n");

  return true;
}


