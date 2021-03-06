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
#include "celtool/stdparams.h"

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
  csRef<iPcZoneManager> zonemgr = celQueryPropertyClassEntity<iPcZoneManager> (level_entity);
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
  csRef<iPcCamera> pccamera = celQueryPropertyClassEntity<iPcCamera> (player_entity);
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

  
  //----------------------------------------------------------- 
  // Create 'testquest'. 
  //----------------------------------------------------------- 
  
  iQuestFactory* fact = qm->CreateQuestFactory ("testquest"); 
  if (!fact)
  {
	printf("Factory Create Failed");
	return 0;
  }

  // ---- init state ----
  iQuestStateFactory* state_init = fact->CreateState ("init");
  iQuestTriggerResponseFactory* init_response =
    state_init->CreateTriggerResponseFactory ();
	
  qm->SetTimeoutTrigger (init_response, "10");
  qm->AddDebugPrintReward (init_response, "\n\nWelcome To The Example Quest");
  
  qm->AddDebugPrintReward (init_response, " -Initalising counter parameter");
  iChangePropertyRewardFactory* rf_changeprop = qm->AddChangePropertyReward 
    (init_response, name, "counter");
  rf_changeprop->SetLongParameter ("0");

  qm->AddDebugPrintReward (init_response, " -Clearing all walktut boxes");
  qm->AddDestroyEntityReward (init_response, "box1");
  qm->AddDestroyEntityReward (init_response, "box2");
  qm->AddDestroyEntityReward (init_response, "box3");

  qm->AddDebugPrintReward (init_response, " -Creating money box");
  csRef<iCelParameterBlock> tpl_params;
  tpl_params.AttachNew (new celVariableParameterBlock ());
  qm->AddCreateEntityReward (init_response, "BoxTemplate", "templateBox", tpl_params);

  qm->AddDebugPrintReward (init_response, " -Entering 1st State\n");
  qm->AddNewStateReward (init_response, name, "start");

  // ---- start state ----
  iQuestStateFactory* state_start = fact->CreateState ("start");

  iRewardType* type = qm->GetRewardType ("cel.rewards.debugprint");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iDebugPrintRewardFactory> dbg_rewfact = scfQueryInterface<iDebugPrintRewardFactory> (rewfact);
  dbg_rewfact->SetMessageParameter ("State Changed => The Game Has Begun");
  state_start->AddInitRewardFactory (rewfact);

  iQuestTriggerResponseFactory* start_response1 =
	state_start->CreateTriggerResponseFactory ();
  qm->SetTimeoutTrigger (start_response1, "10");

  qm->AddDebugPrintReward (start_response1, " -Money Box Self Destuct Sequence Has Begun\n");
  csRef<iCelSequenceFactory> destruct_seq = fact->CreateSequence("sequence_destruct");
  csRef<iSeqOpFactory> seqopfact = qm->GetSeqOpType("cel.seqops.debugprint")->CreateSeqOpFactory();
  csRef<iDebugPrintSeqOpFactory> destruct_seqopfact = 
	scfQueryInterface<iDebugPrintSeqOpFactory> (seqopfact);
  destruct_seqopfact->SetMessageParameter("Self Destruct Progress : ");
  destruct_seq->AddSeqOpFactory(seqopfact, "1000");
  qm->AddSequenceReward(start_response1, name, "sequence_destruct", "10");

  iQuestTriggerResponseFactory* start_response2 =
    state_start->CreateTriggerResponseFactory ();
  qm->SetMeshSelectTrigger(start_response2, "templateBox");
  qm->AddDebugPrintReward(start_response2, "You Picked Up The Money Box & Stopped The Self Destruct\n");
  qm->AddNewStateReward (start_response2, name, "box_pickup");


  iQuestTriggerResponseFactory* start_response3 =
    state_start->CreateTriggerResponseFactory ();
  qm->SetSequenceFinishTrigger(start_response3, name, "sequence_destruct");
  qm->AddDebugPrintReward(start_response3, "SELF-DESTRUCT HAS OCCURRED\n");
  qm->AddNewStateReward (start_response3, name, "box_explode");

  // ---- box_pickup state ----
  iQuestStateFactory* state_box_pickup = fact->CreateState ("box_pickup");
  
  type = qm->GetRewardType ("cel.rewards.sequencefinish");
  csRef<iRewardFactory> seq_fin_rewfact = type->CreateRewardFactory ();
  csRef<iSequenceFinishRewardFactory> explicit_seq_fin_rewfact = 
  	scfQueryInterface<iSequenceFinishRewardFactory> (seq_fin_rewfact);
  explicit_seq_fin_rewfact->SetEntityParameter (name);
  explicit_seq_fin_rewfact->SetSequenceParameter ("sequence_destruct");
  state_box_pickup->AddInitRewardFactory(seq_fin_rewfact);

  iQuestTriggerResponseFactory* box_pickup_response1 =
	state_box_pickup->CreateTriggerResponseFactory ();
  qm->SetTimeoutTrigger (box_pickup_response1, "10");
  rf_changeprop = qm->AddChangePropertyReward 
    (box_pickup_response1, name, "counter");
  rf_changeprop->SetLongParameter ("1");
  

  iQuestTriggerResponseFactory* box_pickup_response2 =
    state_box_pickup->CreateTriggerResponseFactory ();
  qm->SetPropertyChangeTrigger (box_pickup_response2, name, "counter", "1");
  qm->AddDebugPrintReward(box_pickup_response2, "Counter Updated To Reflect Pickup => You Have Won");
  


  // ---- box_explode state ----
  iQuestStateFactory* state_box_explode = fact->CreateState ("box_explode");

  iQuestTriggerResponseFactory* box_explode_response1 =
	state_box_explode->CreateTriggerResponseFactory ();
  qm->SetTimeoutTrigger (box_explode_response1, "10");
  qm->AddInventoryReward (box_explode_response1, "player", "badone");

  iQuestTriggerResponseFactory* box_explode_response2 =
    state_box_explode->CreateTriggerResponseFactory ();
  qm->SetInventoryTrigger(box_explode_response2, "player", "badone");
  qm->AddDebugPrintReward(box_explode_response2, "Badone Hid In Your Cloak From The Explosion");
  
  iQuestTriggerResponseFactory* box_explode_response3 =
	  state_box_explode->CreateTriggerResponseFactory ();
  qm->SetTimeoutTrigger (box_explode_response3, "50");
  qm->AddMessageReward(box_explode_response3, "player", "You Lose");

  iQuestTriggerResponseFactory* box_explode_response4 =
	  state_box_explode->CreateTriggerResponseFactory ();
  qm->SetMessageTrigger(box_explode_response4, "player", "You Lose");
  qm->AddDebugPrintReward(box_explode_response4, "Message Trigger Fired => You Have Lost");
  

   //-----------------------------------------------------------

  csRef<iPcQuest> pcquest = celQueryPropertyClassEntity<iPcQuest> (entity_quest);
  csRef<iCelParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());

  if (!pcquest->NewQuest ("testquest", params)) 
  { 
    ReportError ("Error creating quest '%s'!", "testquest"); 
    return 0; 
  } 

  pcquest->GetQuest ()->SwitchState ("init"); 

  return csPtr<iCelEntity> (entity_quest); 
} 
