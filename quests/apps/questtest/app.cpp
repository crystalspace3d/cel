#include <crystalspace.h>

#include <celtool/initapp.h>
#include <propclass/zone.h>
#include <propclass/camera.h>
#include <propclass/mesh.h>
#include <propclass/linmove.h>
#include <propclass/actormove.h>
#include <propclass/input.h>
#include <physicallayer/propclas.h>

#include "app.h"
#include "behave.h"
#include "tools/rewards.h"

#include <propclass/quest.h>

MainApp::MainApp ()
{
  SetApplicationName ("Seminar Test");
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
  csRef<iPcZoneManager> zonemgr = CEL_QUERY_PROPCLASS_ENT (level_entity,
  	iPcZoneManager);
  iCelZone* zone = zonemgr->CreateZone ("main");
  iCelRegion* region = zonemgr->CreateRegion ("main");
  zone->LinkRegion (region);

  iCelMapFile* mapfile = region->CreateMapFile ();
  mapfile->SetPath ("/cellib/lev");
  mapfile->SetFile ("walktut_world");

  iCelMapFile* entitiesfile = region->CreateMapFile ();
  entitiesfile->SetPath ("/cellib/lev");
  entitiesfile->SetFile ("questtest_entities");

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
    CEL_PROPCLASS_END);
  if (!player_entity)
    return ReportError ("Error creating player entity!");

  // Get the iPcCamera interface so that we can set the camera.
  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (player_entity, iPcCamera);
  // Get the zone manager from the level entity which should have been created
  // by now.
  csRef<iPcZoneManager> pczonemgr = CEL_QUERY_PROPCLASS_ENT (level_entity,
  	iPcZoneManager);
  pccamera->SetZoneManager (pczonemgr, true, "main", "Camera");

  // Get the iPcMesh interface so we can load the right mesh
  // for our player.
  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (player_entity, iPcMesh);
  pcmesh->SetPath ("/cellib/objects");
  pcmesh->SetMesh ("test", "cally.cal3d");
  if (!pcmesh->GetMesh ())
    return ReportError ("Error loading model!");

  if (pczonemgr->PointMesh ("player", "main", "Camera"))
    return ReportError ("Can't find region or start position in region!");

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (player_entity,
  	iPcLinearMovement);
  pclinmove->InitCD (
  	csVector3 (0.5f,0.8f,0.5f),
  	csVector3 (0.5f,0.4f,0.5f),
  	csVector3 (0,0,0));

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcactormove = CEL_QUERY_PROPCLASS_ENT (player_entity, iPcActorMove);
  pcactormove->SetMovementSpeed (3.0f);
  pcactormove->SetRunningSpeed (5.0f);
  pcactormove->SetRotationSpeed (1.75f);

  // Get iPcCommandInput so we can do key bindings. The behaviour layer
  // will interprete the commands so the actor can move.
  csRef<iPcCommandInput> pcinput = CEL_QUERY_PROPCLASS_ENT (player_entity,
  	iPcCommandInput);
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

bool MainApp::OnInitialize (int argc, char* argv[])
{
  if (!celInitializer::RequestPlugins (object_reg,
        CS_REQUEST_VFS,
    	CS_REQUEST_OPENGL3D,
    	CS_REQUEST_ENGINE,
    	CS_REQUEST_FONTSERVER,
    	CS_REQUEST_IMAGELOADER,
    	CS_REQUEST_LEVELLOADER,
    	CS_REQUEST_REPORTER,
    	CS_REQUEST_REPORTERLISTENER,
    	CS_REQUEST_PLUGIN ("cel.physicallayer", iCelPlLayer),
    	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.opcode",
		    iCollideSystem),
      CS_REQUEST_END))
    return ReportError ("Can't initialize plugins!");

  csBaseEventHandler::Initialize(object_reg);

  if (!RegisterQueue(object_reg, csevAllEvents(object_reg)))
    return ReportError ("Can't setup event handler!");

  return true;
}

bool MainApp::Application ()
{
  if (!OpenApplication (object_reg))
    return ReportError ("Error opening system!");

  g3d = csQueryRegistry<iGraphics3D> (object_reg);
  engine = csQueryRegistry<iEngine> (object_reg);
  loader = csQueryRegistry<iLoader> (object_reg);
  vfs = csQueryRegistry<iVFS> (object_reg);
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  kbd = csQueryRegistry<iKeyboardDriver> (object_reg);


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


  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.logic.quest"))
	return false;

 //myQuest
  csRef<iCelEntity> quest = CreateQuest("myquest");
  if (!quest) printf ("CreateQuest(""myquest"") Failed!\n");

  printer.AttachNew (new FramePrinter (object_reg));

  Run ();

  return true;
}

void MainApp::OnExit ()
{
  printer.Invalidate ();
}

csPtr<iCelEntity> MainApp::CreateQuest (const char* name) 
{ 
  // The Quest Entity
  entity_quest = pl->CreateEntity (name, bl, "quest_behave",
    "pclogic.quest", "pctools.properties", CEL_PROPCLASS_END);

  csRef<iQuestManager> qm = csQueryRegistryOrLoad<iQuestManager> (object_reg,
    	"cel.manager.quests"); 

  //iChangePropertyQuestRewardFactory* rf_changeprop;
  iChangePropertyRewardFactory* rf_changeprop;
  //----------------------------------------------------------- 
  // Create 'testquest'. 
  //----------------------------------------------------------- 
  
  iQuestFactory* fact = qm->CreateQuestFactory ("testquest"); 
  if (!fact)
  {
	printf("Factory Create Failed");
	return 0;
  }

  // ---- init ----
  iQuestStateFactory* state_init = fact->CreateState ("init");
  iQuestTriggerResponseFactory* init_response1 =
        state_init->CreateTriggerResponseFactory ();
	
  qm->SetTimeoutTrigger (init_response1, "1");
  qm->AddDebugPrintReward (init_response1, "Timeout Trigger Fired\n Debug Print Functional\n New State Attempted");
  qm->AddNewStateReward (init_response1, name, "start");
  rf_changeprop = qm->AddChangePropertyReward (init_response1,
	        name, "counter");
  rf_changeprop->SetLongParameter ("1");

  // ---- start ----
  iQuestStateFactory* state_start = fact->CreateState ("start");
  
  iQuestTriggerResponseFactory* start_response1 =
	  state_start->CreateTriggerResponseFactory ();

  qm->SetTimeoutTrigger (start_response1, "1");
  qm->AddDebugPrintReward (start_response1, "State Changed\n Change Property Attempted");
  rf_changeprop = qm->AddChangePropertyReward (start_response1,
	        name, "counter");
  rf_changeprop->SetLongParameter ("123");
  

  iQuestTriggerResponseFactory* start_response2 =
    state_start->CreateTriggerResponseFactory ();
  qm->SetPropertyChangeTrigger (start_response2, name, "counter", "123");
  qm->AddDebugPrintReward (start_response2, "Property Change Trigger Fired \n Sequences Should Start");



  //Refactored Sequence Test
  csRef<iCelSequenceFactory> dbg_seq = fact->CreateSequence("sequence_debug");
  csRef<iSeqOpFactory> seqopfact = qm->GetSeqOpType("cel.seqops.debugprint")->CreateSeqOpFactory();
  csRef<iDebugPrintSeqOpFactory> dbg_seqopfact = 
	  scfQueryInterface<iDebugPrintSeqOpFactory> (seqopfact);
  dbg_seqopfact->SetMessageParameter("Refactored Sequence Running");
  dbg_seq->AddSeqOpFactory(seqopfact, "1000");
  qm->AddSequenceReward(start_response2, name, "sequence_debug", "10");

  iQuestTriggerResponseFactory* start_response3 =
    state_start->CreateTriggerResponseFactory ();
  qm->SetTimeoutTrigger(start_response3, "100");
  qm->AddDebugPrintReward(start_response3, "Attempting To Stop Sequence");
  qm->AddSequenceFinishReward(start_response3, name, "sequence_debug");

  iQuestTriggerResponseFactory* start_response4 =
    state_start->CreateTriggerResponseFactory ();
  qm->SetSequenceFinishTrigger(start_response4, name, "sequence_debug");
  qm->AddDebugPrintReward(start_response4, "SequenceFinish Trigger Fired\n Deleting 2 Existing Boxes");
  qm->AddDebugPrintReward(start_response4, " Deleting Non-Existant Entity (Should Raise Error)\n Creating New Box");
  qm->AddDebugPrintReward(start_response4, " Attempting To Add Badone To Inventory \n");
  qm->AddDebugPrintReward(start_response4, " Please check visually that only two boxes remains \n");

  iQuestTriggerResponseFactory* start_response = 
	  state_start->CreateTriggerResponseFactory ();
  qm->SetWatchTrigger(start_response, "player", "box", "5", "100000000000");
  qm->AddDebugPrintReward(start_response, "You are looking at box number 3");

  const celEntityTemplateParams tpl_params;
  qm->AddCreateEntityReward (start_response4, "BoxTemplate", "templateBox", tpl_params);
  qm->AddDestroyEntityReward (start_response4, "box1");
  qm->AddDestroyEntityReward (start_response4, "box2");
 // qm->AddDestroyEntityReward_NEW (start_response4, "box3");
  qm->AddDestroyEntityReward (start_response4, "non-existant_entity");
  qm->AddInventoryReward (start_response4, "player", "badone");

  iQuestTriggerResponseFactory* start_response5 =
    state_start->CreateTriggerResponseFactory ();
  qm->SetInventoryTrigger(start_response5, "player", "badone");
  qm->AddDebugPrintReward(start_response5, "Inventory Trigger Fired => Player received badone");
  qm->AddDebugPrintReward(start_response5, " Attempting to send a message to the player");
  qm->AddMessageReward(start_response5, "player", "Message sent via reward to player");

  iQuestTriggerResponseFactory* start_response6 =
	  state_start->CreateTriggerResponseFactory ();
  qm->SetMessageTrigger(start_response6, "player", "Message sent via reward to player");
  qm->AddDebugPrintReward(start_response6, "Message Trigger Fired => Player received message\n");
  qm->AddDebugPrintReward(start_response6, " Please now attempt to pick up box number 3\n");

  iQuestTriggerResponseFactory* start_response7 = 
	  state_start->CreateTriggerResponseFactory ();
  qm->SetMeshSelectTrigger(start_response7, "box3");
  qm->AddDebugPrintReward(start_response7, "Mesh Select Trigger Fired => You selected the correct box");
  qm->AddDebugPrintReward(start_response7, " Attempting to invoke action Hide in templateBox\n");
  qm->AddDebugPrintReward(start_response7, " Please now check template box is moving\n");

  //csRef<iActionRewardFactory> action_rewfact = 
  //  scfQueryInterface<iActionRewardFactory>
	 // (qm->AddActionReward(start_response7, "templateBox", "SetVelocity", "pcmove.linear"));
  //action_rewfact->AddParameter(CEL_DATA_VECTOR3, , "velocity", "0,1,0");

//  iQuestTriggerResponseFactory* start_response8 =
//	  state_start->CreateTriggerResponseFactory ();


   //-----------------------------------------------------------

  csRef<iPcQuest> pcquest = CEL_QUERY_PROPCLASS_ENT (entity_quest, iPcQuest); 
  celParams params;
  params.Put("message", "FINALLY: HELLO\n");
  params.Put ("ent", name); 

  if (!pcquest->NewQuest ("testquest", params)) 
  { 
	ReportError ("Error creating quest '%s'!", "testquest"); 
    return 0; 
  } 

  pcquest->GetQuest ()->SwitchState ("init"); 

  return csPtr<iCelEntity> (entity_quest); 
} 
