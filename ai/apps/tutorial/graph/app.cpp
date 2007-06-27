#include <crystalspace.h>

#include <celtool/initapp.h>
#include <tools/celgraph.h>
#include <plugins/tools/celgraph/celgraph.h>
#include <propclass/zone.h>
#include <propclass/camera.h>
#include <propclass/mesh.h>
#include <propclass/steer.h>
#include <propclass/linmove.h>
#include <propclass/actormove.h>
#include <propclass/input.h>
#include <physicallayer/propclas.h>

#include <cssysdef.h>
#include <csutil/scf.h>
#include <csutil/cfgfile.h>
#include <csutil/priorityqueue.h>
#include "app.h"
#include "behave.h"

MainApp::MainApp ()
{
  SetApplicationName ("PathFinding Test");
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
  entitiesfile->SetFile ("walktut_entities");

  return true;
}

bool MainApp::CreatePlayer ()
{
  player_entity = pl->CreateEntity ("player", bl, "player_behave",
				    "pccamera.old",
				    "pcobject.mesh",
				    "pcmove.linear",
				    "pcmove.actorold",
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
  pcinput->Bind ("0", "position");


  return true;
}

bool MainApp::LoadPathFinder ()
{
  pf_entity = pl->CreateEntity ("pf", bl, "pathfinding_behave",
				      "pcmove.linear",
				      "pcmove.actorold",
				      "pcsteer",
				      "pcobject.mesh",
				      "pcinput.standard",
				      CEL_PROPCLASS_END);

  if (!pf_entity)
    return ReportError ("Error creating PathFinding entity!");
  
  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (pf_entity, iPcMesh);
  pcmesh->SetPath ("/cellib/objects");
  pcmesh->SetMesh ("test", "cally.cal3d");
  if (!pcmesh->GetMesh ())
    return ReportError ("Error loading model!");

  csRef<iPcZoneManager> pczonemgr = CEL_QUERY_PROPCLASS_ENT (level_entity,
							     iPcZoneManager);

  if (pczonemgr->PointMesh ("pf", "main", "Camera"))
    return ReportError ("Can't find region or start position in region!");


  //Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (pf_entity,
								iPcLinearMovement);
  pclinmove->InitCD (
		     csVector3 (0.5f,0.8f,0.5f),
		     csVector3 (0.5f,0.4f,0.5f),
		     csVector3 (0,0,0));

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcactormove = CEL_QUERY_PROPCLASS_ENT (pf_entity, iPcActorMove);
  pcactormove->SetMovementSpeed (3.0f);
  pcactormove->SetRunningSpeed (5.0f);
  pcactormove->SetRotationSpeed (2.75f);
  
  

  //csRef<iPcSteer> pcsteer = CEL_QUERY_PROPCLASS_ENT (steering_entity,
  //					     iPcSteer);

 

  // Get iPcCommandInput so we can do key bindings. The behaviour layer
  // will interprete the commands so the steerer can move.
  csRef<iPcCommandInput> pcinput = CEL_QUERY_PROPCLASS_ENT (pf_entity,
							    iPcCommandInput);
  pcinput->Bind ("1", "arrival");
  pcinput->Bind ("2", "ca");
  pcinput->Bind ("3", "cohesion");
  pcinput->Bind ("4", "separation");
  pcinput->Bind ("5", "dm");
  pcinput->Bind ("s", "seek");
  pcinput->Bind ("w", "wander");
  pcinput->Bind ("f", "flee");
  pcinput->Bind ("p", "pursue");
  
  return true;
}

bool MainApp::LoadGraph ()
{
  //csRef<iPluginManager> plugin_mgr = csQueryRegistry<iPluginManager> (object_reg);
  //csRef<iCelGraph> celgraph = csLoadPlugin<iCelGraph> (plugin_mgr, "cel.celgraph");
  
  //  if(celgraph.IsValid())
    

  csRef<iCelGraph> celgraph = scfCreateInstance<iCelGraph> ("cel.celgraph");
    
  if(!celgraph)
    fprintf(stderr, "Error Loading CelGraph!\n");

  csMapNode n1("n1");
  csRef<iCelNode> gn1 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v1(15.858, 0, 11.138);
  n1.SetPosition(v1);
  gn1->SetMapNode(&n1);

  csMapNode n2("n2");
  csRef<iCelNode> gn2 = scfCreateInstance<iCelNode> ("cel.celnode");  
  csVector3 v2(15.665, 0, 2.292);
  n2.SetPosition(v2);
  gn2->SetMapNode(&n2);
  
  csMapNode n3("n3");
  csRef<iCelNode> gn3 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v3(5.641, 0, 2.166);
  n3.SetPosition(v3);
  gn3->SetMapNode(&n3);
  
  csMapNode n4("n4");
  csRef<iCelNode> gn4 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v4(-10.878, 0, 1.195);
  n4.SetPosition(v4);
  gn4->SetMapNode(&n4);  

  csMapNode n5("n5");
  csRef<iCelNode> gn5 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v5(-11.645, 0, -15.996);
  n5.SetPosition(v5);
  gn5->SetMapNode(&n5);  

  csMapNode n6("n6");
  csRef<iCelNode> gn6 = scfCreateInstance<iCelNode> ("cel.celnode");
  csVector3 v6(-3.395, 0, -15.892);
  n6.SetPosition(v6);
  gn6->SetMapNode(&n6);

  csRefArray<iCelNode> prueba;
  prueba.Push(gn1);
  
  celgraph->AddNode(gn1);
  //celgraph->AddNode(gn2);
  //celgraph->AddNode(gn3);
  //celgraph->AddNode(gn4);
  //celgraph->AddNode(gn5);
  //celgraph->AddNode(gn6);

  return true;
}


void MainApp::ProcessFrame ()
{
}

void MainApp::FinishFrame ()
{
  // Just tell the 3D renderer that everything has been rendered.
  g3d->FinishDraw ();
  g3d->Print (0);
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
 if (!LoadPathFinder ())
    return ReportError ("Couldn't create Pathfinding entity!"); 
 if(!LoadGraph())
   return ReportError  ("Error loading graph!"); 
  Run ();

  return true;
}
