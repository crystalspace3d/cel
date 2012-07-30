
#include "cssysdef.h"

#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/actormove.h"
#include "propclass/camera.h"
#include "propclass/inv.h"
#include "propclass/linmove.h"
#include "propclass/mover.h"
#include "propclass/meshsel.h"
#include "propclass/mesh.h"
#include "propclass/prop.h"
#include <propclass/trigger.h>
#include <propclass/steer.h>
#include <propclass/sound.h>

#include <isndsys/ss_source.h>

#include "sheep.h"
#include "lifesim.h"

#define BTREE_UPDATE_RATE 200

static int entityCount = 0;

bool LifeSimulator::CreateSheepEntity (const csVector3 position)
{
  csString name = "Sheep ";
  name += entityCount++;

  csRef<iCelEntity> entity =
    physicalLayer->CreateEntity (name, behaviourLayer, "sheep_behaviour",
				 "pcobject.mesh",
				 "pcmove.linear",
				 "pcmove.actor.standard",
				 "pcmove.mover",
				 "pclogic.trigger",
				 "pcmove.steer",
				 "pcsound.source",
				 CEL_PROPCLASS_END);
  if (!entity)
    return ReportError ("Error creating player entity!");
	
  // Get the iPcMesh interface so we can load the right mesh for our player.
  csRef<iPcMesh> pcMesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  pcMesh->SetPath("/lib/yofrankie_chars/sheep");
  pcMesh->SetMesh("sheep_cute", "library");
  csRef<iMeshWrapper> mesh = pcMesh->GetMesh();
  if (!mesh)
    return false;

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  pcLinMove->InitCD (mesh->GetMeshObject ()->GetMeshWrapper (), 0.3f);
  pcLinMove->SetFullPosition (position, 0.0f, room);

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcActorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  pcActorMove->SetMovementSpeed (0.3f);
  pcActorMove->SetRunningSpeed (1.5f);
  pcActorMove->SetRotationSpeed (0.50f);
  
  csRef<iPcTrigger> pcTrigger = celQueryPropertyClassEntity<iPcTrigger> (entity);
  float radius = 3.0f;
  pcTrigger->SetupTriggerSphere (pcLinMove->GetSector (), pcLinMove->GetPosition (), radius);
  pcTrigger->EnableMessagesToOthers (true);
  pcTrigger->EnableMessagesToSelf (true);
  pcTrigger->MonitorClass ("frankie");
  pcTrigger->SetFollowEntity (true);
  pcTrigger->EnableTrigger (true);
 
 //load sound files and create sound wrappers for each one of them
  loader->LoadSoundWrapper ("Sheep1","/lib/YoFrankie_sounds/Sheep1.ogg");
  loader->LoadSoundWrapper ("Sheep2","/lib/YoFrankie_sounds/Sheep2.ogg"); 
  loader->LoadSoundWrapper ("WaterSplash","/lib/YoFrankie_sounds/WaterSplash.ogg");  
  
  //Get the iPcSoundSource interface so we can set the sound source name
  csRef<iPcSoundSource> pcSound = celQueryPropertyClassEntity<iPcSoundSource> (entity);
  pcSound->SetSoundName ("Sheep2");
  csRef<iSndSysSource> sndsource = pcSound->GetSoundSource ();
  sndsource->GetStream ()->SetLoopState (CS_SNDSYS_STREAM_DONTLOOP);
  sndsource->SetVolume (1.0f);
  
  csRef<iPcSteer> pcSteer = celQueryPropertyClassEntity<iPcSteer> (entity);
  //pcSteer->CollisionAvoidanceOn(2.0f, 1.0f);
 
  // Initialize the behaviour tree
  BehaviourCommon* behaviour = static_cast<BehaviourCommon*> (entity->GetBehaviour ());
  //behaviour->CreateBehaviourTree ();

  return true;
}

SheepBehaviour::SheepBehaviour (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
				    iCelPlLayer* physicalLayer)
  : BehaviourCommon ("sheep_behaviour", entity, behaviourLayer, physicalLayer)
{
  // Register for the first wake up
  physicalLayer->CallbackEveryFrame (this, CEL_EVENT_PRE); 
  physicalLayer->CallbackOnce (this, 0, CEL_EVENT_PRE);
  
  className = physicalLayer->FetchStringID( "sheep");
  position = "";
  resetTrigger = "false";
  entity->AddClass (className);
  
  id_pctrigger_entityenters =  physicalLayer->FetchStringID ("pctrigger_entityenters");
  id_pctrigger_entityleaves =  physicalLayer->FetchStringID ("pctrigger_entityleaves");
  id_pctrigger_entertrigger =  physicalLayer->FetchStringID ("pctrigger_entertrigger");
  id_pctrigger_leavetrigger =  physicalLayer->FetchStringID ("pctrigger_leavetrigger");
  id_pcsteer_arrived = physicalLayer->FetchStringID ("pcsteer_arrived");
  id_pclinearmovement_stuck = physicalLayer->FetchStringID ("pclinearmovement_stuck");
}

void SheepBehaviour::CreateBehaviourTree ()
{
  csRef<iPcActorMove> actorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  csRef<iCelPropertyClass> pcActorMove = scfQueryInterface<iCelPropertyClass> (actorMove);
  csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);

  csRef<iPluginManager> pluginManager = 
    csQueryRegistry<iPluginManager> (behaviourLayer->registry);  
  
  // Create a default selector
  csRef<iBTNode> root_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.sequential"); 
	  
    // Create a random selector
  csRef<iBTNode> random_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.random"); 
	
  root_node->AddChild (random_node);
  
  // Create a sequential selector
  csRef<iBTNode> monitoring_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.sequential"); 

  root_node->AddChild (monitoring_node);
  
  // create a trigger check node
  csRef<iBTNode> trigger_check_node =  csLoadPlugin<iBTNode> (pluginManager,
	"cel.behaviourtree.triggerfired"); 
	
  monitoring_node->AddChild (trigger_check_node);
 
  //setup the trigger check node
    csRef<iTriggerType> trigger_type = csLoadPlugin<iTriggerType> (pluginManager,
    "cel.triggers.message");
  csRef<iTriggerFactory> trigger_factory = trigger_type->CreateTriggerFactory ();
  csRef<iMessageTriggerFactory> explicit_trigger_factory =
	  scfQueryInterface<iMessageTriggerFactory> (trigger_factory);
  explicit_trigger_factory->SetEntityParameter (entity->GetName ());
  explicit_trigger_factory->SetMaskParameter ("cel.trigger.entity.enter"); 
  csRef<iCelParameterBlock> params;
  trigger = trigger_factory->CreateTrigger (nullptr, params);
  csRef<iTriggerFiredCondition> explicit_trigger_node =
    scfQueryInterface<iTriggerFiredCondition> (trigger_check_node);
  explicit_trigger_node->SetTrigger (trigger);
  
  // Load the reward types
  csRef<iRewardType> reward_type = csLoadPlugin<iRewardType> (pluginManager,
    "cel.rewards.action");

  csRef<iReward> reward;
  csRef<iRewardFactory> reward_factory;
  csRef<iActionRewardFactory> explicit_reward_factory;

  csRef<iBTAction> explicit_action_node;

   // Create the 'flee' action node
  csRef<iBTNode> flee_action_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.behaviourtree.action");  
	
  monitoring_node->AddChild (flee_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (flee_action_node);

  // Setup the reward to cancel the previous animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("Clear");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);
  
 // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.steer");
  explicit_reward_factory->SetIDParameter ("Flee");
  explicit_reward_factory->AddParameter
    (CEL_DATA_STRING, physicalLayer->FetchStringID ("sectorname"), "TerrainSector");
  explicit_reward_factory->AddParameter
    (CEL_DATA_VECTOR3, physicalLayer->FetchStringID ("position"), pcLinMove->GetPosition ().Description ());

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);
  
  // Create the 'idle' action node
  csRef<iBTNode> idle_action_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.behaviourtree.action");  
   random_node->AddChild (idle_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (idle_action_node);

  // Setup the reward to cancel all previous animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("Clear");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);
  
  /* // play sound
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcsound.source");
  explicit_reward_factory->SetIDParameter ("Play");
  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);*/

  // Create the 'run' action node
  csRef<iBTNode> run_action_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.behaviourtree.action");  
  //random_node->AddChild (run_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (run_action_node);

  // Setup the reward to cancel the previous animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("Clear");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);

  // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("Forward");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("start"), "true");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);

  // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("Run");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("start"), "true");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);

  // Create the 'rotate_left' action node
  csRef<iBTNode> rotate_left_action_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.behaviourtree.action");  
  random_node->AddChild (rotate_left_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (rotate_left_action_node);

  // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("RotateLeft");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("start"), "true");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);

  // Create the 'rotate_right' action node
  csRef<iBTNode> rotate_right_action_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.behaviourtree.action");  
  random_node->AddChild (rotate_right_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (rotate_right_action_node);

  // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("RotateRight");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("start"), "true");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);
  
  // Define the root of the behaviour tree
  behaviourTree = root_node;
}

bool SheepBehaviour::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
				    iCelParameterBlock* params, va_list arg)
{
/*
	csRef<iPcSteer> pcSteer = celQueryPropertyClassEntity< (entity, iPcSteer);
	csRef<iPcSoundSource> pcSound = celQueryPropertyClassEntity< (entity, iPcSoundSource);

   if (msg_id == id_pctrigger_entertrigger)
    {
     pcSteer->CheckArrivalOn (1.0);
	}
	
   else if (msg_id == id_pctrigger_entityenters)
	{ 	   
	}
	
	else if (msg_id == id_pctrigger_entityleaves)
	{ 
	    pcSound->Stop ();
	    pcSteer->CheckArrivalOff ();
	}
	else if (msg_id == id_pctrigger_leavetrigger)
	{
	    pcSound->Stop ();
	    pcSteer->CheckArrivalOff ();
	}
	else if (msg_id == id_pcsteer_arrived)
	{
	  	pcSound->Stop ();
	    pcSteer->CheckArrivalOff ();
	}
*/
  return false;
}

void SheepBehaviour::TickEveryFrame ()
{
/*
  csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity< (entity,
							  iPcLinearMovement);
  frankie_entities = physicalLayer->FindNearbyEntities (pcLinMove->GetSector (),
									  pcLinMove->GetPosition (), 10.0f, false,
									           physicalLayer->FetchStringID ("frankie"));
  ram_entities = physicalLayer->FindNearbyEntities (pcLinMove->GetSector (),
									  pcLinMove->GetPosition (), 10.0f, false,
									               physicalLayer->FetchStringID ("ram"));

  if (ram_entities->GetCount () > 0)
  {
	csRef<iCelEntity> ram_entity = ram_entities->Get(0);
	csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity< (ram_entity,
							  iPcLinearMovement);
	position = pclinmove->GetPosition ().Description ();
  }	
  
  if (frankie_entities->GetCount () > 0)
  {
  	csRef<iCelEntity> frankie_entity = frankie_entities->Get(0);
	csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity< (frankie_entity,
							  iPcLinearMovement);
	position = pclinmove->GetPosition ().Description ();
  }

 // Register for the next update
  physicalLayer->CallbackEveryFrame (this, CEL_EVENT_PRE);
*/
}

void SheepBehaviour::TickOnce ()
{
/*
  // Update the behaviour tree
  celParams params;
  params.PutUnique ("resetTrigger", resetTrigger);
  params.PutUnique ("position", position);
  behaviourTree->Execute (params);

  // Register for the next update
  physicalLayer->CallbackOnce (this, BTREE_UPDATE_RATE, CEL_EVENT_PRE);
*/
}
