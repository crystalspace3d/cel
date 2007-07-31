#include <crystalspace.h>

#include <physicallayer/entity.h>
#include <physicallayer/propclas.h>
#include <propclass/linmove.h>
#include <propclass/prop.h>

#include "behave.h"

SCF_IMPLEMENT_IBASE (BehaviourLayer)
  SCF_IMPLEMENTS_INTERFACE (iCelBlLayer)
SCF_IMPLEMENT_IBASE_END


/*
 * This is a test application which serves as a tutorial
 * for pcsteer. Main functions to look at are BehvaiorSteering()
 * constructor and SendMessage().
 * 
 */

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
  else if(!strcmp(name, "steering_behave"))
    behave = new BehaviourSteering (entity, this, pl);


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

BehaviourSteering::BehaviourSteering (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl)
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

  arrival = false;
  ca = false;
  cohesion = false;
  separation = false;
  dm = false;

  entities = pl->CreateEmptyEntityList();
  csRef<iCelEntity> player_entity = pl->FindEntity("player");
  entities->Add(player_entity);
}
 
bool BehaviourSteering::SendMessage (csStringID msg_id,
				     iCelPropertyClass* pc,
				     celData& ret, iCelParameterBlock* params, va_list arg)
{
  /*
   * Here we handle all messages concerning
   * Steering behaviours
   */

  // This will print once we get the arrived message from pcsteer

  if (msg_id == id_pcsteer_arrived)
    printf("Arrived\n");
  else if (msg_id == id_pccommandinput_seek1){
  
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
    float rot;
    
    pclinmove->GetLastFullPosition (position, rot, sector);
 
    /*
     * Once we have targets position and sector
     * we create a reference to the pcsteer instance in our
     * steering entity and call Seek().
     *
     */
   
    csRef<iCelEntity> steering_entity = pl->FindEntity("steer");

    csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
						       iPcSteer);
    
    pcsteer->Seek(sector, position);
    
  }
  else if (msg_id == id_pccommandinput_flee1)
    {
      /*
       * The same proccess of finding targets location
       * is repeated here.
       *
       */

    printf("Flee\n");
    csRef<iCelEntity> player_entity = pl->FindEntity("player");
    csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (player_entity,
								  iPcLinearMovement);
    iSector* sector;
    csVector3 position;
    float rot;
    
    pclinmove->GetLastFullPosition (position, rot, sector);
    

    /*
     * Once we have targets position and sector
     * we create a reference to the pcsteer instance in our
     * steering entity and call Flee().
     *
     */

    csRef<iCelEntity> steering_entity = pl->FindEntity("steer");
    csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
						       iPcSteer);
    pcsteer->Flee(sector, position);

    }
  else if(msg_id == id_pccommandinput_pursue1)
    {
      /*
       * To pursue we just calle Pursue() in pcsteer with
       * the target as parameter.
       * Pcsteer will calculate its position on its own.
       */

      printf("Pursue\n");
      csRef<iCelEntity> player_entity = pl->FindEntity("player");
      csRef<iCelEntity> steering_entity = pl->FindEntity("steer");
      csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
							 iPcSteer);    
      pcsteer->Pursue(player_entity, 0.5f);
    }
  else if(msg_id == id_pccommandinput_arrival1)
    {
      csRef<iCelEntity> steering_entity = pl->FindEntity("steer");
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
      csRef<iCelEntity> steering_entity = pl->FindEntity("steer");
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

	pcsteer->CohesionOn(entities, 10.0f, 3.0f);
	cohesion = true;
	printf("Cohesion On\n");
	
      } else{
	pcsteer->CohesionOff();
	cohesion = false;
	printf("Cohesion Off\n");      
      }
    } else if (msg_id == id_pccommandinput_separation1)
    {
      csRef<iCelEntity> steering_entity = pl->FindEntity("steer");
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
      csRef<iCelEntity> steering_entity = pl->FindEntity("steer");
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

