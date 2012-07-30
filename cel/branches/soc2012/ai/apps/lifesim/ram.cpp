/*
  Copyright (C) 2011 Christian Van Brussel, Eutyche Mukuama, Dodzi de Souza
      Communications and Remote
      Sensing Laboratory of the School of Engineering at the 
      Universite catholique de Louvain, Belgium
      http://www.tele.ucl.ac.be

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
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
#include <propclass/steer.h>
#include <propclass/sound.h>

#include <isndsys/ss_source.h>


#include "ram.h"
#include "lifesim.h"

#define BTREE_UPDATE_RATE 500

static int entityCount = 0;

bool LifeSimulator::CreateRamEntity (const csVector3 position)
{
  csString name = "Ram ";
  name += entityCount++;

  csRef<iCelEntity> entity =
    physicalLayer->CreateEntity (name, behaviourLayer, "ram_behaviour",
				 "pcobject.mesh",
				 "pcmove.linear",
				 "pcmove.actor.standard",
				 "pcmove.mover",
				 "pcmove.steer",
				 "pcsound.source",
				 CEL_PROPCLASS_END);
  if (!entity)
    return ReportError ("Error creating player entity!");

  // Get the iPcMesh interface so we can load the right mesh for our player.
  csRef<iPcMesh> pcMesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  pcMesh->SetPath("/lib/yofrankie_chars/ram/");
  pcMesh->SetMesh("ram", "library");
  csRef<iMeshWrapper> mesh = pcMesh->GetMesh();
  if (!mesh)
    return false;

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  pcLinMove->InitCD (mesh->GetMeshObject ()->GetMeshWrapper (), 1.0f);
  pcLinMove->SetFullPosition (position, 0.0f, room);

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcActorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  pcActorMove->SetMovementSpeed (0.5f);
  pcActorMove->SetRunningSpeed (1.5f);
  pcActorMove->SetRotationSpeed (1.25f);
  
  //load sound files and create sound wrappers for each one of them
  loader->LoadSoundWrapper ("WaterSplash","/lib/YoFrankie_sounds/WaterSplash.ogg"); 

  // Initialize the behaviour tree
  BehaviourCommon* behaviour = static_cast<BehaviourCommon*> (entity->GetBehaviour ());
  behaviour->CreateBehaviourTree ();

  return true;
}

RamBehaviour::RamBehaviour (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
				    iCelPlLayer* physicalLayer)
  : BehaviourCommon ("ram_behaviour", entity, behaviourLayer, physicalLayer)
{
  // Register for the first wake up
  physicalLayer->CallbackEveryFrame (this, CEL_EVENT_PRE); 
  physicalLayer->CallbackOnce (this, 0, CEL_EVENT_PRE);
 
  className = physicalLayer->FetchStringID( "ram"); 
  entity->AddClass (className);
  
  id_pctrigger_entityenters =  physicalLayer->FetchStringID ("pctrigger_entityenters");
  id_pctrigger_entityleaves =  physicalLayer->FetchStringID ("pctrigger_entityleaves");
  id_pctrigger_entertrigger =  physicalLayer->FetchStringID ("pctrigger_entertrigger");
  id_pctrigger_leavetrigger =  physicalLayer->FetchStringID ("pctrigger_leavetrigger");
  id_pcmover_arrived = physicalLayer->FetchStringID("pcmover_arrived");
  
  dead = false;
}

void RamBehaviour::CreateBehaviourTree ()
{
  csRef<iPcActorMove> actorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  csRef<iCelPropertyClass> pcActorMove = scfQueryInterface<iCelPropertyClass> (actorMove);

  csRef<iPluginManager> pluginManager = 
    csQueryRegistry<iPluginManager> (behaviourLayer->registry);
 
    // Create a default selector
  csRef<iBTNode> root_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.default"); 
	
   
    // Create a sequential selector
  csRef<iBTNode> chasing_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.sequential"); 
   root_node->AddChild (chasing_node) ;
   
   // Create a random selector
  csRef<iBTNode> random_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.random"); 
    root_node->AddChild (random_node);
   
  
    // create a negate return node 
  csRef<iBTNode> negatereturn_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.decorators.negatereturn");	
  
    // create a trigger check node
  csRef<iBTNode> trigger_check_node =  csLoadPlugin<iBTNode> (pluginManager,
	"cel.behaviourtree.triggerfired"); 
	
	chasing_node->AddChild (trigger_check_node);
 
  //setup the trigger check nodes
    csRef<iTriggerType> trigger_type = csLoadPlugin<iTriggerType> (pluginManager,
    "cel.triggers.message");
	
  csRef<iTriggerFactory> trigger_factory = trigger_type->CreateTriggerFactory ();
  csRef<iMessageTriggerFactory> explicit_trigger_factory =
	  scfQueryInterface<iMessageTriggerFactory> (trigger_factory); 
   
  // setup the chasing trigger node 
  explicit_trigger_factory->SetEntityParameter (entity->GetName ());
  explicit_trigger_factory->SetMaskParameter ("cel.trigger.entity.enter.this"); 
  csRef<iCelParameterBlock> params;
  csRef<iTrigger> trigger = trigger_factory->CreateTrigger (nullptr, params);
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
   
 // Create the 'chase' action node
  csRef<iBTNode> chase_action_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.behaviourtree.action");  
	
  chasing_node->AddChild (chase_action_node);
  
  explicit_action_node = scfQueryInterface<iBTAction> (chase_action_node);
  
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
 

  // Create the 'walk' action node
  csRef<iBTNode> walk_action_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.behaviourtree.action");  
  random_node->AddChild (walk_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (walk_action_node);

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

bool RamBehaviour::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
				    iCelParameterBlock* params, va_list arg)
{
  /*//printf ("message %s\n", physicalLayer->FetchString (msg_id));
    csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity< (entity, iPcLinearMovement);
	csRef<iPcMover> pcMover = celQueryPropertyClassEntity< (entity, iPcMover);
	csRef<iPcSteer> pcSteer = celQueryPropertyClassEntity< (entity, iPcSteer);
	pcSteer->CheckArrivalOn (1.0f);
    csRef<iPcActorMove> pcActorMove = celQueryPropertyClassEntity< (entity, iPcActorMove);
	
	if (msg_id == id_pctrigger_entertrigger)
    {
	  pcActorMove->SetRunningSpeed (2.0f);
	  entities = physicalLayer->FindNearbyEntities (pcLinMove->GetSector (),
                                  pcLinMove->GetPosition (), 10.0f,false, physicalLayer->FetchStringID ("frankie"));
	  if (entities)
	       pcSteer->Pursue (entities->Get (0), 0.0f);	
	}*/

  return false;
}

void RamBehaviour::TickEveryFrame ()
{/*
 {
	 csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity< (entity, iPcLinearMovement);
	 csVector3 pos = pcLinMove->GetFullPosition ();
	 printf ("%s  y = %f\n", entity->GetName (), pos.y);
	 if (pos.y <= WATER_LEVEL) 
	 {
	  // Remove call back once to stop the BTNode execution
	  physicalLayer->RemoveCallbackOnce (this, CEL_EVENT_PRE);
	 //Get the iPcSoundSource interface so we can set the sound source name
      csRef<iPcSoundSource> pcSound = celQueryPropertyClassEntity< (entity, iPcSoundSource);
      pcSound->SetSoundName ("WaterSplash");
      csRef<iSndSysSource> sndsource = pcSound->GetSoundSource ();
      sndsource->GetStream ()->SetLoopState (CS_SNDSYS_STREAM_DONTLOOP);
      sndsource->SetVolume (1.0f);

	  //Clear all previous animations
	  csRef<iPcActorMove> pcActorMove = celQueryPropertyClassEntity< (entity, iPcActorMove);
	  //pcActorMove->Forward (false);
	  pcActorMove->SetAnimation ("drawn");
	 // pcActorMove->SetAnimation ("die", false);
      	  
	  csRef<iPcSteer> pcSteer = celQueryPropertyClassEntity< (entity, iPcSteer);
	  pcSteer->CheckArrivalOff ();
	  
	  //Disable any trigger for this mesh
	  //csRef<iPcTrigger> pcTrigger = celQueryPropertyClassEntity< (entity, iPcTrigger);
	  //pcTrigger->EnableTrigger (false);  
	  
	  physicalLayer->RemoveEntity (entity);
	  
	  //Hide this mesh
	 // csRef<iPcMesh> pcMesh = celQueryPropertyClassEntity<(entity, iPcMesh);
     // pcMesh->Hide();
	  physicalLayer->RemoveCallbackEveryFrame (this, CEL_EVENT_PRE);
	  dead = true;
	 }
 }*/
}

void RamBehaviour::TickOnce ()
{
  // Update the behaviour tree
  csRef<iCelParameterBlock> params;
  behaviourTree->Execute (params);

  // Register for the next update
  physicalLayer->CallbackOnce (this, BTREE_UPDATE_RATE, CEL_EVENT_PRE);
}
