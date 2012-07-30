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
//#include <propclass/gravity.h>
#include <propclass/trigger.h>
#include "propclass/prop.h"

#include "butterfly.h"
#include "lifesim.h"

#define BTREE_UPDATE_RATE 500

static int entityCount = 0;

bool LifeSimulator::CreateButterFlyEntity (const csVector3 position)
{
  csString name = "Butterfly ";
  name += entityCount++;

  csRef<iCelEntity> entity =
    physicalLayer->CreateEntity (name, behaviourLayer, "butterfly_behaviour",
				 "pcobject.mesh",
				 "pcmove.linear",
				 "pcmove.actor.standard",
				 "pcmove.mover",
				 //"pcmove.gravity",
				 "pclogic.trigger",
				 CEL_PROPCLASS_END);
  if (!entity)
    return ReportError ("Error creating player entity!");

  // Get the iPcMesh interface so we can load the right mesh for our player.
  csRef<iPcMesh> pcMesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  pcMesh->SetPath("/lib/yofrankie_chars/butterfly/");
  pcMesh->SetMesh("butterfly", "library");
  csRef<iMeshWrapper> mesh = pcMesh->GetMesh();
  if (!mesh)
    return false;

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  pcLinMove->InitCD (mesh->GetMeshObject ()->GetMeshWrapper (), 1.0f);
  pcLinMove->SetFullPosition (position, 0.0f, room);
  //pcLinMove->ClearWorldVelocity ();
  
  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcActorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  pcActorMove->SetMovementSpeed (3.5f);
  pcActorMove->SetRunningSpeed (5.5f);
  pcActorMove->SetRotationSpeed (1.25f);
  
  //csRef<iPcGravity> pcGravity = celQueryPropertyClassEntity< (entity, iPcGravity);
  //pcGravity->ClearPermanentForces ();
  
  csRef<iPcTrigger> pcTrigger = celQueryPropertyClassEntity<iPcTrigger> (entity);
  float radius = 5.0f;
  pcTrigger->SetupTriggerSphere (pcLinMove->GetSector (), pcLinMove->GetPosition (), radius);
  pcTrigger->EnableMessagesToOthers (true);
  pcTrigger->EnableMessagesToSelf (true);
  pcTrigger->MonitorClass ("frankie");
  pcTrigger->EnableTrigger (true);

  // Initialize the behaviour tree
  BehaviourCommon* behaviour = static_cast<BehaviourCommon*> (entity->GetBehaviour ());
  behaviour->CreateBehaviourTree ();

  return true;
}

ButterFlyBehaviour::ButterFlyBehaviour (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
				    iCelPlLayer* physicalLayer)
  : BehaviourCommon ("butterfly_behaviour", entity, behaviourLayer, physicalLayer)
{
  // Register for the first wake up
  physicalLayer->CallbackOnce (this, 0, CEL_EVENT_PRE);
  
  this->entity = entity;
  
  className = physicalLayer->FetchStringID( "butterfly");
  entity->AddClass (className);
  
  id_pctrigger_entityenters =  physicalLayer->FetchStringID ("pctrigger_entityenters");
  id_pctrigger_entityleaves =  physicalLayer->FetchStringID ("pctrigger_entityleaves");
  id_pctrigger_entertrigger =  physicalLayer->FetchStringID ("pctrigger_entertrigger");
  id_pctrigger_leavetrigger =  physicalLayer->FetchStringID ("pctrigger_leavetrigger");
  id_pcmover_arrived = physicalLayer->FetchStringID("pcmover_arrived");
  stopBT = false;
}

void ButterFlyBehaviour::CreateBehaviourTree ()
{
  csRef<iPcActorMove> actorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  csRef<iCelPropertyClass> pcActorMove = scfQueryInterface<iCelPropertyClass> (actorMove);

  csRef<iPluginManager> pluginManager = 
    csQueryRegistry<iPluginManager> (behaviourLayer->registry);
 
  // Create a random selector
  csRef<iBTNode> random_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.random"); 
  
  // Load the reward types
  csRef<iRewardType> reward_type = csLoadPlugin<iRewardType> (pluginManager,
    "cel.rewards.action");

  csRef<iReward> reward;
  csRef<iRewardFactory> reward_factory;
  csRef<iActionRewardFactory> explicit_reward_factory;

  csRef<iBTAction> explicit_action_node;

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

  csRef<iCelParameterBlock> params;
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
  random_node->AddChild (run_action_node);
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
  behaviourTree = random_node;
}

bool ButterFlyBehaviour::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
				    iCelParameterBlock* params, va_list arg)
{
  csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  csRef<iPcMover> pcMover = celQueryPropertyClassEntity<iPcMover> (entity);
  csRef<iPcTrigger> pcTrigger = celQueryPropertyClassEntity<iPcTrigger> (entity);

   if (msg_id == id_pctrigger_entertrigger)
    {
	  stopBT = true;
	  pcMover->MoveTo (pcLinMove->GetSector (), pcLinMove->GetPosition () + csVector3 (-5.0f, 10.0f,1.0f), 0.005f);
	  printf("%s run!!\n", entity->GetName ());
	}
	
   else if (msg_id ==   id_pctrigger_entityenters)
	{ 
	  stopBT = true;
	  printf("%s frankie in sight\n", entity->GetName ());
	  pcMover->MoveTo (pcLinMove->GetSector (), pcLinMove->GetPosition () + csVector3 (5.0f, 10.0f,1.0f), 0.005f);
	}
	
   else if (msg_id == id_pcmover_arrived)
   {
    stopBT = false;
    printf ("%s escaped\n", entity->GetName ());
	  float radius = 5.0f;
     pcTrigger->SetupTriggerSphere (pcLinMove->GetSector (), pcLinMove->GetPosition (), radius);
   }
 
   else
        return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);
		
  return false;
}

void ButterFlyBehaviour::TickEveryFrame ()
{
}

void ButterFlyBehaviour::TickOnce ()
{
  // Update the behaviour tree
  if (!stopBT)
  {
   csRef<iCelParameterBlock> params;
   behaviourTree->Execute (params);
  }

  // Register for the next update
  physicalLayer->CallbackOnce (this, BTREE_UPDATE_RATE, CEL_EVENT_PRE);
}
