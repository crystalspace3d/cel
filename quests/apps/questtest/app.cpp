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

  iChangePropertyQuestRewardFactory* rf_changeprop;
  iChangePropertyRewardFactory* rf_changeprop_NEW;
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
	
  qm->SetTimeoutTrigger_NEW (init_response1, "1000");
  qm->AddDebugPrintReward_NEW (init_response1, "message");
  qm->AddNewStateReward (init_response1, "$ent", "start");
  qm->AddInventoryReward_NEW (init_response1, "player", "badone");

  const celEntityTemplateParams tpl_params;
  qm->AddCreateEntityReward_NEW (init_response1, "BoxTemplate", "templateBox", tpl_params);
  qm->AddDestroyEntityReward_NEW (init_response1, "box1");

  // ---- start ----
  iQuestStateFactory* state_start = fact->CreateState ("start");
  
  iQuestTriggerResponseFactory* start_response1 =
	  state_start->CreateTriggerResponseFactory ();

  qm->SetTimeoutTrigger (start_response1, "3000");
  qm->AddDebugPrintReward_NEW (start_response1, "State Changed!\n");
  rf_changeprop = qm->AddChangePropertyReward (start_response1,
	        "$ent", "counter");
  rf_changeprop->SetLongParameter ("123");
  qm->AddDestroyEntityReward_NEW (init_response1, "non-existant_entity");
  

  iQuestTriggerResponseFactory* start_response2 =
    state_start->CreateTriggerResponseFactory ();
  qm->SetPropertyChangeTrigger (start_response2, "$ent", "counter", "123");
  qm->AddDebugPrintReward_NEW (start_response2, "Property Counter Changed... Sequences Should Have Started\n");

  //Sequence Test
  csRef<iQuestSequenceFactory>dbg_seq = fact->CreateSequence("sequence_debug");
  csRef<iQuestSeqOpFactory> seqopfact = qm->GetSeqOpType("cel.questseqop.debugprint")->CreateSeqOpFactory();
  csRef<iDebugPrintQuestSeqOpFactory> dbg_seqopfact = 
	  scfQueryInterface<iDebugPrintQuestSeqOpFactory> (seqopfact);
  dbg_seqopfact->SetMessageParameter("SEQUENCE RUNNING");
  dbg_seq->AddSeqOpFactory(seqopfact, "10");
  qm->AddSequenceReward(start_response2, "$ent", "sequence_debug", "10");

  iQuestTriggerResponseFactory* start_response3 =
    state_start->CreateTriggerResponseFactory ();
  qm->SetSequenceFinishTrigger(start_response3, "$ent", "sequence_debug");
  qm->AddDebugPrintReward_NEW(start_response3, "SEQUENCE FINISHED, WILL REFACTOR SEQUENCE BEGIN?\n");

  //Refactored Sequence Test
  //csRef<iCelSequenceFactory>dbg_seq_NEW = fact->CreateSequence_NEW("sequence_debug_NEW");
  //csRef<iSeqOpFactory> seqopfact_NEW = qm->GetSeqOpType_NEW("cel.seqops.debugprint")->CreateSeqOpFactory();
  //csRef<iDebugPrintSeqOpFactory> dbg_seqopfact_NEW = 
	 // scfQueryInterface<iDebugPrintSeqOpFactory> (seqopfact_NEW);
  //dbg_seqopfact_NEW->SetMessageParameter("REFACTORED SEQUENCE RUNNING");
  //dbg_seq_NEW->AddSeqOpFactory(seqopfact_NEW, "10");
  //qm->AddSequenceReward_NEW(start_response3, name, "sequence_debug", "10");

  //iQuestTriggerResponseFactory* start_response4 =
  //  state_start->CreateTriggerResponseFactory ();
  //qm->SetSequenceFinishTrigger_NEW(start_response4, name, "sequence_debug_NEW");
  //qm->AddDebugPrintReward_NEW(start_response4, "REFACTORED SEQUENCE FINISHED");

   //-----------------------------------------------------------

  csRef<iPcQuest> pcquest = CEL_QUERY_PROPCLASS_ENT (entity_quest, iPcQuest); 
  celQuestParams params; 
  params.Put ("message", "Hallo Hallo!\n"); 
  params.Put ("ent", name); 

  celParams params_NEW;
  params_NEW.Put("message", "FINALLY: HELLO\n");
  params_NEW.Put ("ent", name); 

  if (!pcquest->NewQuest ("testquest", params, params_NEW)) 
  { 
	ReportError ("Error creating quest '%s'!", "testquest"); 
    return 0; 
  } 

  pcquest->GetQuest ()->SwitchState ("init"); 

  return csPtr<iCelEntity> (entity_quest); 
} 
