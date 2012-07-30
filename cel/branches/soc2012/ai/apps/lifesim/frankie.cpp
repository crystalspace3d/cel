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

#include <csutil/weakrefarr.h>

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
#include "celtool/stdparams.h"

#include <isndsys/ss_source.h>


#include "frankie.h"
#include "lifesim.h"

#define BTREE_UPDATE_RATE 500

static int entityCount = 0;

bool LifeSimulator::CreateFrankieEntity (const csVector3 position)
{
  csString name = "Frankie ";
  name += entityCount++;

  csRef<iCelEntity> entity =
    physicalLayer->CreateEntity (name, behaviourLayer, "frankie_behaviour",
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
  pcMesh->SetPath("/lib/frankie");
  pcMesh->SetMesh("franky_frankie", "frankie.xml");
  csRef<iMeshWrapper> mesh = pcMesh->GetMesh();
  if (!mesh)
    return false;

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  pcLinMove->InitCD (mesh->GetMeshObject ()->GetMeshWrapper (), 0.3f);
  pcLinMove->SetFullPosition (position, 0.0f, room);

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcActorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  pcActorMove->SetMovementSpeed (0.5f);
  pcActorMove->SetRunningSpeed (1.5f);
  pcActorMove->SetRotationSpeed (1.25f);

  csRef<iPcTrigger> pcTrigger = celQueryPropertyClassEntity<iPcTrigger> (entity);
  float radius = 10.0f;
  pcTrigger->SetupTriggerSphere (pcLinMove->GetSector (), pcLinMove->GetPosition (), radius);
  //pcTrigger->EnableMessagesToOthers (true);
  pcTrigger->EnableMessagesToSelf (true);
  //pcTrigger->MonitorClass ("ram");
  pcTrigger->MonitorClass ("sheep");
  pcTrigger->SetFollowEntity (true);
  pcTrigger->EnableTrigger (true);

  csRef<iPcSteer> pcSteer = celQueryPropertyClassEntity<iPcSteer> (entity);
  //pcSteer->CollisionAvoidanceOn(2.0f, 1.0f);
  
   //load sound files and create sound wrappers for each one of them
  loader->LoadSoundWrapper ("WaterSplash","/lib/YoFrankie_sounds/WaterSplash.ogg");

  // Initialize the behaviour tree
  BehaviourCommon* behaviour = static_cast<BehaviourCommon*> (entity->GetBehaviour ());
  behaviour->CreateBehaviourTree ();

  return true;
}

FrankieBehaviour::FrankieBehaviour (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
				    iCelPlLayer* physicalLayer)
  : BehaviourCommon ("frankie_behaviour", entity, behaviourLayer, physicalLayer)
{
  // Register for the first wake up
  //physicalLayer->CallbackEveryFrame (this, CEL_EVENT_PRE); 
  physicalLayer->CallbackOnce (this, 0, CEL_EVENT_PRE);
    
  className = physicalLayer->FetchStringID( "frankie");
  entity->AddClass (className);
  
  sheep_name = "";
  position = "";
  resetTrigger = "false";
  
  id_pctrigger_entityenters =  physicalLayer->FetchStringID ("pctrigger_entityenters");
  id_pctrigger_entityleaves =  physicalLayer->FetchStringID ("pctrigger_entityleaves");
  id_pctrigger_entertrigger =  physicalLayer->FetchStringID ("pctrigger_entertrigger");
  id_pctrigger_leavetrigger =  physicalLayer->FetchStringID ("pctrigger_leavetrigger");
  id_pcsteer_arrived = physicalLayer->FetchStringID("pcsteer_arrived");
}
 
void FrankieBehaviour::CreateBehaviourTree ()
{
  csRef<iPcActorMove> actorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  csRef<iCelPropertyClass> pcActorMove = scfQueryInterface<iCelPropertyClass> (actorMove);
  
  csRef<iPluginManager> pluginManager =  csQueryRegistry<iPluginManager> (behaviourLayer->registry);
	
  csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);


  // Load the reward types
  csRef<iRewardType> reward_type = csLoadPlugin<iRewardType> (pluginManager,
    "cel.rewards.action");	
  csRef<iRewardType> debug_reward_type = csLoadPlugin<iRewardType> (pluginManager,
    "cel.rewards.debugprint");	

  csRef<iReward> reward;
  csRef<iRewardFactory> reward_factory;
  csRef<iActionRewardFactory> explicit_reward_factory;
  csRef<iDebugPrintRewardFactory> debugprint_reward_factory;

  csRef<iBTAction> explicit_action_node;
 
   // Create a default selector
  csRef<iBTNode> root_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.default"); 
	
    // Create a sequential selector
  csRef<iBTNode> monitoring_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.sequential"); 
   root_node->AddChild (monitoring_node);
   
    // Create a sequential selector
  csRef<iBTNode> chasing_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.sequential"); 
  //root_node->AddChild (chasing_node) ;
   
   // Create a random selector
  csRef<iBTNode> random_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.selectors.random"); 
    root_node->AddChild (random_node);
   
     // create a trigger check node
  csRef<iBTNode> trigger_check_node1 =  csLoadPlugin<iBTNode> (pluginManager,
	"cel.behaviourtree.triggerfired"); 
	
  monitoring_node->AddChild (trigger_check_node1);
/* 
  //setup the trigger check nodes
  csRef<iTriggerType> trigger_type = csLoadPlugin<iTriggerType> (pluginManager,
								 "cel.triggers.message");
	
  csRef<iTriggerFactory> trigger_factory = trigger_type->CreateTriggerFactory ();
  csRef<iMessageTriggerFactory> explicit_trigger_factory =
	  scfQueryInterface<iMessageTriggerFactory> (trigger_factory);
	  
  // setup the monitoring trigger check node  
  explicit_trigger_factory->SetEntityParameter (entity->GetName ());
  explicit_trigger_factory->SetMaskParameter ("cel.trigger.entity.enter"); 
  csRef<iTrigger> trigger = trigger_factory->CreateTrigger (celParams ());
  csRef<iTriggerFiredCondition> explicit_trigger_node =
    scfQueryInterface<iTriggerFiredCondition> (trigger_check_node1);
  explicit_trigger_node->SetFireOnce (true);
  explicit_trigger_node->SetTrigger (trigger);
*/
  //setup the trigger check nodes
  csRef<iTriggerType> trigger_type = csLoadPlugin<iTriggerType> (pluginManager,
								 "cel.triggers.trigger");
	
  csRef<iTriggerFactory> trigger_factory = trigger_type->CreateTriggerFactory ();
  csRef<iTriggerTriggerFactory> explicit_trigger_factory =
	  scfQueryInterface<iTriggerTriggerFactory> (trigger_factory);
	  
  // setup the monitoring trigger check node  
  //explicit_trigger_factory->SetEntityParameter (entity->GetName ());
  //explicit_trigger_factory->EnableLeave ();
  //explicit_trigger_factory->SetMaskParameter ("cel.trigger.entity.enter"); 
  csRef<iCelParameterBlock> params;
  csRef<iTrigger> trigger = trigger_factory->CreateTrigger (nullptr, params);
  csRef<iTriggerFiredCondition> explicit_trigger_node =
    scfQueryInterface<iTriggerFiredCondition> (trigger_check_node1);
  explicit_trigger_node->SetFireOnce (true);
  explicit_trigger_node->SetTrigger (trigger);

  // Debug that
  csRef<iBTNode> debug_action_node = csLoadPlugin<iBTNode> (pluginManager,
							    "cel.behaviourtree.action");  
  monitoring_node->AddChild (debug_action_node);
  //trigger_check_node1->AddChild (debug_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (debug_action_node);
  
  reward_factory = debug_reward_type->CreateRewardFactory ();
  debugprint_reward_factory = scfQueryInterface<iDebugPrintRewardFactory> (reward_factory);

  debugprint_reward_factory->SetMessageParameter ("trigger1");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);

  /*
  // create a trigger check node
  csRef<iBTNode> trigger_check_node2 =  csLoadPlugin<iBTNode> (pluginManager,
	"cel.behaviourtree.triggerfired"); 
  chasing_node->AddChild (trigger_check_node2);
	
  // setup the chasing trigger node 
  explicit_trigger_factory->SetEntityParameter (entity->GetName ());
  explicit_trigger_factory->SetMaskParameter ("cel.trigger.entity.enter.this"); 
  trigger = trigger_factory->CreateTrigger (nullptr, params);
  explicit_trigger_node =
    scfQueryInterface<iTriggerFiredCondition> (trigger_check_node2);
  //explicit_trigger_node->SetFireOnce (true);
  explicit_trigger_node->SetTrigger (trigger);
    
  // Debug that
  debug_action_node = csLoadPlugin<iBTNode> (pluginManager,
					     "cel.behaviourtree.action");  
  trigger_check_node2->AddChild (debug_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (debug_action_node);
  
  reward_factory = debug_reward_type->CreateRewardFactory ();
  debugprint_reward_factory = scfQueryInterface<iDebugPrintRewardFactory> (reward_factory);

  debugprint_reward_factory->SetMessageParameter ("trigger2");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);


//--------------------------------------------------------------------------------------------------------------  
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

  // @@@ TODO: fix the quest parameter 0!
  csRef<iCelParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());
  reward = reward_factory->CreateReward (0, params);
  explicit_action_node->AddReward (reward);
  
    // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("Run");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("start"), "true");

  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward (0, params);
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
  */
//---------------------------------------------------------------------------------------------------------------  
 // Create the 'chase' action node
  csRef<iBTNode> chase_action_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.behaviourtree.action");  
	
  monitoring_node->AddChild (chase_action_node);
  chasing_node->AddChild (chase_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (chase_action_node);
  
  // Setup the reward to cancel the previous animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("Clear");

  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward (0, params);
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
  
 // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.steer");
  explicit_reward_factory->SetIDParameter ("Pursue");
  explicit_reward_factory->AddParameter
    (CEL_DATA_STRING, physicalLayer->FetchStringID ("meshname"), "Sheep 0");
  explicit_reward_factory->AddParameter
    (CEL_DATA_FLOAT, physicalLayer->FetchStringID ("pursue_max_prediction"), "3.0");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);
/*
//-----------------------------------------------------------------------------------------------------------------------------------------
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

  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward (0, params);
  explicit_action_node->AddReward (reward);

  // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("Forward");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("start"), "true");

  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward (0, params);
  explicit_action_node->AddReward (reward);
//-------------------------------------------------------------------------------------------
*/ 
 // Create the 'idle' action node
  csRef<iBTNode> idle_action_node = csLoadPlugin<iBTNode> (pluginManager,
    "cel.behaviourtree.action");  
  random_node->AddChild (idle_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (idle_action_node);

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
  explicit_reward_factory->SetIDParameter ("SetAnimation");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("animation"), "stand");
  //(CEL_DATA_BOOL, physicalLayer->FetchStringID ("start"), "true");

  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward (0, params);
  explicit_action_node->AddReward (reward);
//------------------------------------------------------------------------------
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
  explicit_reward_factory->SetIDParameter ("SetAnimation");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("animation"), "Frankie_Turn.L");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);

  // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("RotateLeft");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("start"), "true");

  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward (0, params);
  explicit_action_node->AddReward (reward);

  // Debug that
  debug_action_node = csLoadPlugin<iBTNode> (pluginManager,
					     "cel.behaviourtree.action");  
  rotate_left_action_node->AddChild (debug_action_node);
  explicit_action_node = scfQueryInterface<iBTAction> (debug_action_node);
  
  reward_factory = debug_reward_type->CreateRewardFactory ();
  debugprint_reward_factory = scfQueryInterface<iDebugPrintRewardFactory> (reward_factory);

  debugprint_reward_factory->SetMessageParameter ("rotate_left");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);

//------------------------------------------------------------------------------------------------

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
  explicit_reward_factory->SetIDParameter ("SetAnimation");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("animation"), "Frankie_Turn.R");

  reward = reward_factory->CreateReward (nullptr, params);
  explicit_action_node->AddReward (reward);

  // Setup the reward for the new animation
  reward_factory = reward_type->CreateRewardFactory ();
  explicit_reward_factory = scfQueryInterface<iActionRewardFactory> (reward_factory);

  explicit_reward_factory->SetEntityParameter (entity->GetName ());
  explicit_reward_factory->SetPropertyClassParameter ("pcmove.actor.standard");
  explicit_reward_factory->SetIDParameter ("RotateRight");
  explicit_reward_factory->AddParameter
    (CEL_DATA_BOOL, physicalLayer->FetchStringID ("start"), "true");

  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward (0, params);
  explicit_action_node->AddReward (reward);
  
  // Define the root of the behaviour tree
  behaviourTree = root_node;
}

bool FrankieBehaviour::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
				    iCelParameterBlock* params, va_list arg)
{
  csRef<iPcSteer> pcSteer = celQueryPropertyClassEntity<iPcSteer> (entity);
  
  if (msg_id == id_pctrigger_entertrigger)
  {
    printf ("trigger enter\n");
    pcSteer->CheckArrivalOn (1.0f);
    resetTrigger = "false";
  }
  else if (msg_id == id_pctrigger_leavetrigger) 
  {
    printf ("trigger leave\n");
   // pcSteer->CheckArrivalOff ();
    resetTrigger = "true";
  }
  
  else if (msg_id == id_pctrigger_entityleaves) 
  {
    printf ("trigger entity leave\n");
    //resetTrigger = "true";
  }
  
  else if (msg_id == id_pctrigger_entityenters) 
  {
    printf ("trigger entity enter\n");
    pcSteer->CheckArrivalOn (1.0f);
    resetTrigger = "false";
  }
  
  else if (msg_id == id_pcsteer_arrived)
  {
    printf ("steer arrived\n");
    // pcSteer->CheckArrivalOff ();
    resetTrigger = "true";
  }
  
  return false;
}

void FrankieBehaviour::TickEveryFrame ()
{
  /*
  csRef<iPcLinearMovement> pcLinMove = CEL_QUERY_PROPCLASS_ENT (entity,
								iPcLinearMovement);
  sheep_entities = physicalLayer->FindNearbyEntities (pcLinMove->GetSector (),
						      pcLinMove->GetPosition (), 10.0f, false,
						      physicalLayer->FetchStringID ("sheep"));
  ram_entities = physicalLayer->FindNearbyEntities (pcLinMove->GetSector (),
						    pcLinMove->GetPosition (), 10.0f, false,
						    physicalLayer->FetchStringID ("ram"));
  if (sheep_entities->GetCount () > 0)
    sheep_name = sheep_entities->Get (0)->GetName ();  
  if (ram_entities->GetCount () > 0)
  {
    csRef<iCelEntity> ram_entity = ram_entities->Get(0);
    csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (ram_entity,
								  iPcLinearMovement);
    position = pclinmove->GetPosition ().Description ();
  }	

  // Register for the next update
  physicalLayer->CallbackEveryFrame (this, CEL_EVENT_PRE);
  */
}

void FrankieBehaviour::TickOnce ()
{
  // Update the behaviour tree
  csRef<iCelParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());
  //params->GetParameter (physicalLayer->FetchStringID ("resetTrigger"))->Set (resetTrigger);
  //params->GetParameter (physicalLayer->FetchStringID ("meshname"))->Set (sheep_name);
  //params->GetParameter (physicalLayer->FetchStringID ("position"))->Set (position);
  behaviourTree->Execute (params);

  // Register for the next update
  physicalLayer->CallbackOnce (this, BTREE_UPDATE_RATE, CEL_EVENT_PRE);
}
