#include <crystalspace.h>

#include <celtool/initapp.h>
#include <iutil/plugin.h>
#include <propclass/zone.h>
#include <propclass/camera.h>
#include <propclass/mesh.h>
#include <propclass/linmove.h>
#include <propclass/actormove.h>
#include <propclass/input.h>
#include <physicallayer/propclas.h>
#include <celtool/stdparams.h>
#include "celtool/stdpcimp.h"

#include "app.h"
#include "behave.h"

MainApp::MainApp ()
: DemoApplication ("CrystalSpace.BehaviourTreeTest") 
{
}

MainApp::~MainApp ()
{
}

bool MainApp::LoadLevel ()
{
  level_entity = pl->CreateEntity ("level", bl, "level_behave",
    "pcworld.zonemanager",
    CEL_PROPCLASS_END);
  if (!level_entity)
    return ReportError ("Error creating level entity!");

  // Now get the iPcZoneManager interface so we can setup the level.
  csRef<iPcZoneManager> zonemgr = celQueryPropertyClassEntity<iPcZoneManager> (
      level_entity);
  iCelZone* zone = zonemgr->CreateZone ("main");
  iCelRegion* region = zonemgr->CreateRegion ("main");
  zone->LinkRegion (region);

  iCelMapFile* mapfile = region->CreateMapFile ();
  mapfile->SetPath ("/cellib/lev");
  mapfile->SetFile ("walktut_world");

  //iCelMapFile* entitiesfile = region->CreateMapFile ();
 // entitiesfile->SetPath ("/cellib/lev");
 // entitiesfile->SetFile ("walktut_entities");

  return true;
}

bool MainApp::CreatePlayer ()
{
  player_entity = pl->CreateEntity ("player", bl, "player_behave",
    "pccamera.old",
    "pcobject.mesh",
    "pcmove.linear",
    "pcmove.actor.standard",
    "pcinput.standard",
    "pctools.inventory",
    "pclogic.behaviourtree",
    CEL_PROPCLASS_END);
  if (!player_entity)
    return ReportError ("Error creating player entity!");

  // Get the iPcCamera interface so that we can set the camera.
  csRef<iPcCamera> pccamera = celQueryPropertyClassEntity<iPcCamera> (
      player_entity);
  // Get the zone manager from the level entity which should have been created
  // by now.
  csRef<iPcZoneManager> pczonemgr = celQueryPropertyClassEntity<iPcZoneManager> (level_entity);
  pccamera->SetZoneManager (pczonemgr, true, "main", "Camera");

  // Get the iPcMesh interface so we can load the right mesh
  // for our player.
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (player_entity);
  pcmesh->SetPath ("/cellib/objects");
  pcmesh->SetMesh ("test", "cally.cal3d");
  if (!pcmesh->GetMesh ())
    return ReportError ("Error loading model!");

  if (pczonemgr->PointMesh ("player", "main", "Camera"))
    return ReportError ("Can't find region or start position in region!");

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (player_entity);
  pclinmove->InitCD (
  	csVector3 (0.5f,0.8f,0.5f),
  	csVector3 (0.5f,0.4f,0.5f),
  	csVector3 (0,0,0));

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcactormove = celQueryPropertyClassEntity<iPcActorMove> (player_entity);
  pcactormove->SetMovementSpeed (3.0f);
  pcactormove->SetRunningSpeed (5.0f);
  pcactormove->SetRotationSpeed (1.75f);

  // Get iPcCommandInput so we can do key bindings. The behaviour layer
  // will interprete the commands so the actor can move.
  csRef<iPcCommandInput> pcinput = celQueryPropertyClassEntity<iPcCommandInput> (player_entity);
  // We read the key bindings from the standard config file.
  pcinput->Bind ("up", "forward");
  pcinput->Bind ("down", "backward");
  pcinput->Bind ("left", "rotateleft");
  pcinput->Bind ("right", "rotateright");
  pcinput->Bind ("m", "cammode");
  pcinput->Bind ("d", "drop");

  return true;
}

void MainApp::Frame ()
{
}

bool MainApp::OnKeyboard(iEvent& ev)
{
  // We got a keyboard event.
  csKeyEventType eventtype = csKeyEventHelper::GetEventType(&ev);
  if (eventtype == csKeyEventTypeDown)
  {
    // The user pressed a key (as opposed to releasing it).
    utf32_char code = csKeyEventHelper::GetCookedCode(&ev);
    if (code == CSKEY_ESC)
    {
      // The user pressed escape to exit the application.
      // The proper way to quit a Crystal Space application
      // is by broadcasting a cscmdQuit event. That will cause the
      // main runloop to stop. To do that we get the event queue from
      // the object registry and then post the event.
      csRef<iEventQueue> q =
        csQueryRegistry<iEventQueue> (GetObjectRegistry());
      if (q.IsValid()) q->GetEventOutlet()->Broadcast(
	  csevQuit (GetObjectRegistry ()));
    }
  }
  return false;
}

void MainApp::PrintHelp () 
{ 
  csCommandLineHelper commandLineHelper; 
  
  // Printing help 
  commandLineHelper.PrintApplicationHelp 
  (GetObjectRegistry (), "bttest", "bttest", 
    "App to test behaviour tree nodes and load from XML. If working correctly, a welcome message should appear in the console when run."); 
} 

bool MainApp::OnInitialize (int argc, char* argv[])
{
  // Default behavior from DemoApplication 
  if (!DemoApplication::OnInitialize (argc, argv)) 
    return false; 

  if (!celInitializer::RequestPlugins (object_reg,
    	CS_REQUEST_PLUGIN ("cel.physicallayer", iCelPlLayer),
    	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.opcode",
		    iCollideSystem),
      CS_REQUEST_END))
    return ReportError ("Can't initialize plugins!");

  return true;
}

bool MainApp::Application ()
{
  // Default behavior from DemoApplication 
  if (!DemoApplication::Application ()) 
    return false; 

  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  bl.AttachNew (new BehaviourLayer(pl));

  // We also need to register it to the object registry.
  if (!object_reg->Register (bl, "iCelBlLayer"))
    return ReportError ("Can't register our behaviour layer!");

  pl->RegisterBehaviourLayer (bl);

  if (!LoadLevel ())
    return ReportError ("Error loading level!");
  if (!CreatePlayer ())
    return ReportError ("Couldn't create player!");

  LoadBehaviourTreeFromXML();

  hudManager->GetKeyDescriptions ()->Empty(); 
  hudManager->GetStateDescriptions ()->Push(
    "This app tests all behaviour tree nodes and the load from XML feature.");
  hudManager->GetStateDescriptions ()->Push(
    "You should see a welcome message in the console if behaviour trees are working correctly.");
  Run ();

  return true;
}

void MainApp::OnExit ()
{
  printer.Invalidate ();
}

void MainApp::LoadBehaviourTreeFromXML ()
{
  csRef<iDocumentSystem> xml = 
		csQueryRegistry<iDocumentSystem> (object_reg);
  if (!xml)
    xml.AttachNew (new csTinyDocumentSystem ());
  csRef<iDocument> doc = xml->CreateDocument ();

  const char* filename = "/cellib/lev/bttest.xml";
  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  csRef<iDataBuffer> xml_buf = vfs->ReadFile (filename);

  const char* error = doc->Parse (xml_buf, true);
  if (error != 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
                "appbttest",
                "Can't open file '%s': %s!", filename, error);
  }

  csRef<iDocumentNode> xml_node = doc->GetRoot ();
   
  csRef<iBTNode> tree = celQueryPropertyClassEntity<iBTNode> (player_entity);
  csRef<iCelPropertyClass> tree_propclass = scfQueryInterface<iCelPropertyClass> (tree);
  tree_propclass->SetProperty(pl->FetchStringID("xml"), xml_node);   
  
  csRef<iCelParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());

  celData result;  
  tree_propclass->PerformAction(pl->FetchStringID("Load BT From XML"), params, result);
  tree_propclass->PerformAction(pl->FetchStringID("BT Start"), params, result);
}

void MainApp::CreateBehaviourTree ()
{
  csRef<iPluginManager> plugin_mgr = 
    csQueryRegistry<iPluginManager> (object_reg);
 
  //Build Nodes
  csRef<iBTNode> root_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.selectors.default"); 

  csRef<iBTNode> initial_sequence_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.selectors.sequential"); 
  csRef<iBTNode> negatereturn_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.decorators.negatereturn");
  csRef<iBTNode> trigger_check_node =  csLoadPlugin<iBTNode> (plugin_mgr,
	  "cel.behaviourtree.triggerfired");  
  csRef<iBTNode> looking_action_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.behaviourtree.action");  
  

  csRef<iBTNode> execution_limit_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.decorators.executionlimit");
  csRef<iBTNode> angry_sequence_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.selectors.sequential"); 
  csRef<iBTNode> angry_action_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.behaviourtree.action"); 
  csRef<iBTNode> loop_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.decorators.loop");
  csRef<iBTNode> calming_action_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.behaviourtree.action"); 


  csRef<iBTNode> lottery_sequence_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.selectors.sequential"); 
  csRef<iBTNode> parameter_check_node =  csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.behaviourtree.parametercheck");
  csRef<iBTNode> lottery_action_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.behaviourtree.action");


  csRef<iBTNode> random_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.selectors.random"); 
  csRef<iBTNode> irritable_action_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.behaviourtree.action");  
  csRef<iBTNode> loving_action_node = csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.behaviourtree.action");

  //Set Up Decorator Nodes
  csRef<iExecutionLimitDecorator> explicit_execution_limit_node = 
    scfQueryInterface<iExecutionLimitDecorator> (execution_limit_node);
  explicit_execution_limit_node->SetExecutionLimit("2");

  csRef<iLoopDecorator> explicit_loop_node = 
    scfQueryInterface<iLoopDecorator> (loop_node);
  explicit_loop_node->SetLoopLimit("3");

  csRef<iParameterCheckCondition> explicit_parameter_check_node =
    scfQueryInterface<iParameterCheckCondition> (parameter_check_node);
  explicit_parameter_check_node->SetParameter("1");
  explicit_parameter_check_node->SetValue("2");

  //Set Up Trigger Check Node
  //Create an inventory trigger and set to trigger node
  csRef<iTriggerType> trigger_type = csLoadPlugin<iTriggerType> (plugin_mgr,
    "cel.triggers.inventory");
  csRef<iTriggerFactory> trigger_factory = trigger_type->CreateTriggerFactory ();
  csRef<iInventoryTriggerFactory> explicit_trigger_factory =
    scfQueryInterface<iInventoryTriggerFactory> (trigger_factory);
  explicit_trigger_factory->SetEntityParameter ("player");
  explicit_trigger_factory->SetChildEntityParameter ("box3");
  // @@@ TODO: fix the quest parameter 0!
  csRef<iCelParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());
  csRef<iTrigger> trigger = trigger_factory->CreateTrigger (0, params);
  csRef<iTriggerFiredCondition> explicit_trigger_node =
    scfQueryInterface<iTriggerFiredCondition> (trigger_check_node);
  explicit_trigger_node->SetTrigger (trigger);


  //Set Up Actions
  //Create a debug print reward factory
  csRef<iRewardType> reward_type = csLoadPlugin<iRewardType> (plugin_mgr,
    "cel.rewards.debugprint");
  csRef<iRewardFactory> reward_factory = reward_type->CreateRewardFactory ();
  csRef<iDebugPrintRewardFactory> explicit_reward_factory = 
    scfQueryInterface<iDebugPrintRewardFactory> (reward_factory);

  //Create rewards for action nodes
  csRef<iBTAction> explicit_action_node =
    scfQueryInterface<iBTAction> (looking_action_node);
  explicit_reward_factory->SetMessageParameter ("Looking For Money Box :s");
  // @@@ TODO: fix the quest parameter 0!
  csRef<iReward> reward = reward_factory->CreateReward(0, params);
  looking_action_node->SetName("Looking Node");
  explicit_action_node->AddReward (reward);

  explicit_action_node = scfQueryInterface<iBTAction> (angry_action_node);
  explicit_reward_factory->SetMessageParameter ("ANGRY! >:");
  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward(0, params);
  explicit_action_node->AddReward (reward);

  explicit_action_node = scfQueryInterface<iBTAction> (calming_action_node);
  explicit_reward_factory->SetMessageParameter ("Calming down, calm... calm :|");
  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward(0, params);
  explicit_action_node->AddReward (reward);

  explicit_action_node = scfQueryInterface<iBTAction> (lottery_action_node);
  explicit_reward_factory->SetMessageParameter ("Won the Lottery! :D");
  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward(0, params);
  explicit_action_node->AddReward (reward);
	  
  explicit_action_node = scfQueryInterface<iBTAction> (irritable_action_node);
  explicit_reward_factory->SetMessageParameter ("Irritable :(");
  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward(0, params);
  explicit_action_node->AddReward (reward);

  explicit_action_node = scfQueryInterface<iBTAction> (loving_action_node);
  explicit_reward_factory->SetMessageParameter ("Loving :)");
  // @@@ TODO: fix the quest parameter 0!
  reward = reward_factory->CreateReward(0, params);
  explicit_action_node->AddReward (reward);


  //Connect Tree
  //root_node->AddChild (initial_sequence_node);
  root_node->AddChild (execution_limit_node);
  root_node->AddChild (lottery_sequence_node);
  root_node->AddChild (random_node);

  initial_sequence_node->AddChild (negatereturn_node);
  initial_sequence_node->AddChild (looking_action_node);

  negatereturn_node->AddChild (trigger_check_node);

  execution_limit_node->AddChild (angry_sequence_node);

  angry_sequence_node->AddChild (angry_action_node);
  angry_sequence_node->AddChild (loop_node);

  loop_node->AddChild (calming_action_node);

  lottery_sequence_node->AddChild (parameter_check_node);
  lottery_sequence_node->AddChild (lottery_action_node);

  random_node->AddChild (irritable_action_node);
  random_node->AddChild (loving_action_node);


  if (player_entity)
  { 
    csRef<iBTNode> tree = celQueryPropertyClassEntity<iBTNode> (player_entity);
    csRef<iCelPropertyClass> tree_propclass = scfQueryInterface<iCelPropertyClass> (tree);
    tree_propclass->SetProperty(pl->FetchStringID("update rate"), long(3));   
    tree_propclass->SetProperty(pl->FetchStringID("root node"), root_node);  
    tree_propclass->SetProperty(pl->FetchStringID("tree name"), "Example Tree");

    celData result;  
    tree_propclass->PerformAction(pl->FetchStringID("BT Start"), params, result);

    long update_rate = 
      tree_propclass->GetPropertyLongByID(pl->FetchStringID("update rate")); 
    long tree_status = 
      tree_propclass->GetPropertyLongByID(pl->FetchStringID("tree status"));
    const char* tree_name = 
      tree_propclass->GetPropertyStringByID(pl->FetchStringID("tree name"));
    csRef<iBase> root_node_base = 
      tree_propclass->GetPropertyIBaseByID(pl->FetchStringID("root node"));
    csRef<iBTNode> root_node = scfQueryInterface<iBTNode> (root_node_base);
    int root_node_status = root_node->GetStatus(); 

    csReport(object_reg, CS_REPORTER_SEVERITY_NOTIFY,
        "appbttest",
        "Tree %s: Status=%li, Update Rate=%li \n Root Node: Status=%i",
        tree_name, tree_status, update_rate, root_node_status);  

    tree_propclass->PerformAction(pl->FetchStringID("BT Interrupt"), params, result);
    tree_status = tree_propclass->GetPropertyLongByID(pl->FetchStringID("tree status"));   
    if (tree_status == BT_NOT_STARTED)
    {
      csReport(object_reg, CS_REPORTER_SEVERITY_NOTIFY,
          "appbttest",
          "Tree Interrupted Succesfully");  
    }
  }
  else
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
              "MainApp::CreateBehaviourTree ()",
              "Unable to make behaviour tree!");
  }
}
