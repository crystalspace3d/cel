#include "MainApp.h"

MainApp::MainApp () 
{
  SetApplicationName("Navigation Mesh Test");
  originSet = false;
  destinationSet = false;
}

MainApp::~MainApp () 
{
}

// TODO ver o que precisa ficar na classe mesmo
bool MainApp::LoadLevel ()
{
  levelEntity = physicalLayer->CreateEntity("level", behaviourLayer, "levelBehaviour", 
                                            "pcworld.zonemanager", CEL_PROPCLASS_END);
  if (!levelEntity)
  {
    return ReportError("Error creating level entity!");
  }

  // Now get the iPcZoneManager interface so we can setup the level.
  csRef<iPcZoneManager> zonemgr = CEL_QUERY_PROPCLASS_ENT(levelEntity, iPcZoneManager);
  zone = zonemgr->CreateZone("main");
  region = zonemgr->CreateRegion("main");
  zone->LinkRegion(region);

  mapfile = region->CreateMapFile();
  //mapfile->SetPath("/cellib/lev");
  //mapfile->SetFile("walktut_world");
  mapfile->SetPath("/lev/castle");  
  mapfile->SetFile("world");
  vfs->ChDir("/lev/castle");

  return true;
}

bool MainApp::CreatePlayer ()
{
  playerEntity = physicalLayer->CreateEntity("player", behaviourLayer, "playerBehaviour", "pccamera.old", 
                                   "pcobject.mesh", "pcmove.linear", "pcmove.actor.standard", "pcmove.mover",
                                   "pcinput.standard", "pctools.inventory", CEL_PROPCLASS_END);
  if (!playerEntity)
  {
    return ReportError("Error creating player entity!");
  }

  // Get the iPcCamera interface so that we can set the camera.
  csRef<iPcCamera> pcCamera = CEL_QUERY_PROPCLASS_ENT(playerEntity, iPcCamera);
  camera = pcCamera->GetCamera();

  // Get the zone manager from the level entity which should have been created by now.
  csRef<iPcZoneManager> pcZoneMgr = CEL_QUERY_PROPCLASS_ENT(levelEntity, iPcZoneManager);
  pcCamera->SetZoneManager(pcZoneMgr, true, "main", "Camera");

  // Get the iPcMesh interface so we can load the right mesh for our player.
  csRef<iPcMesh> pcMesh = CEL_QUERY_PROPCLASS_ENT(playerEntity, iPcMesh);
  pcMesh->SetPath("/cellib/objects");
  pcMesh->SetMesh("test", "cally.cal3d");
  csRef<iMeshWrapper> mesh = pcMesh->GetMesh();
  if (!mesh)
  {
    return ReportError("Error loading model!");
  }
  csMatrix3 m;
  m.Identity();
  m /= 5.0;
  csReversibleTransform scaleTransform(m, csVector3(0, 0, 0));
  mesh->HardTransform(scaleTransform);

  if (pcZoneMgr->PointMesh("player", "main", "Camera"))
  {
    return ReportError("Can't find region or start position in region!");
  }

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pcLinMove = CEL_QUERY_PROPCLASS_ENT(playerEntity, iPcLinearMovement);
  pcLinMove->InitCD(csVector3(0.5f,0.8f,0.5f), csVector3(0.5f,0.4f,0.5f), csVector3(0,0,0));

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcActorMove = CEL_QUERY_PROPCLASS_ENT (playerEntity, iPcActorMove);
  pcActorMove->SetMovementSpeed(3.0f);
  pcActorMove->SetRunningSpeed(5.0f);
  pcActorMove->SetRotationSpeed(1.75f);

  // Get iPcCommandInput so we can do key bindings. The behaviour layer will interprete the 
  // commands so the actor can move.
  csRef<iPcCommandInput> pcInput = CEL_QUERY_PROPCLASS_ENT(playerEntity, iPcCommandInput);

  // We read the key bindings from the standard config file.
  pcInput->Bind("up", "forward");
  pcInput->Bind("down", "backward");
  pcInput->Bind("left", "rotateleft");
  pcInput->Bind("right", "rotateright");
  pcInput->Bind("m", "cammode");
  pcInput->Bind("d", "drop");
  pcInput->Bind("MouseButton0", "setposition");
  pcInput->Bind("b", "buildnavstruct");
  pcInput->Bind("s", "savenavstruct");
  pcInput->Bind("l", "loadnavstruct");
  pcInput->Bind("c", "clearnavstruct");

  return true;
}

// TODO not working. Rendering before the rest?
void MainApp::Frame ()
{
  if (navStruct)
  {
    navStruct->DebugRender();
  }

  if (originSet && navStruct)
  {
    navStruct->DebugRenderAgent(origin, 70, 140, 255, 150);
  }

  if (destinationSet && navStruct)
  {
    navStruct->DebugRenderAgent(destination, 50, 255, 120, 150);
  }

  if (path)
  {
    path->DebugRender();
  }
}

bool MainApp::OnKeyboard(iEvent& ev)
{
  // We got a keyboard event.
  csKeyEventType eventType = csKeyEventHelper::GetEventType(&ev);
  if (eventType == csKeyEventTypeDown)
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
      csRef<iEventQueue> q = csQueryRegistry<iEventQueue>(GetObjectRegistry());
      if (q.IsValid())
      {
        q->GetEventOutlet()->Broadcast(csevQuit(GetObjectRegistry()));
      }
    }
  }
  return false;
}

bool MainApp::OnMouseClick (iEvent& ev) 
{
  switch(csMouseEventHelper::GetButton(&ev))
  {
  case csmbLeft:
    MouseClick1Handler(ev); 
    break;
  case csmbRight:
    MouseClick2Handler(ev); 
    break;
  case csmbMiddle:
    MouseClick3Handler(ev); 
    break;
  }
  return false;
}

// left
void MainApp::MouseClick1Handler (iEvent& ev)
{
  csVector2 screenPoint;
  screenPoint.x = csMouseEventHelper::GetX(&ev);
  screenPoint.y = csMouseEventHelper::GetY(&ev);

  csScreenTargetResult st = csEngineTools::FindScreenTarget(screenPoint, 10000.0f, camera);
  csRef<iSectorList> sectorList = st.mesh->GetMovable()->GetSectors();
  if (sectorList->GetCount() != 1)
  {
    return;
  }
  
  if (kbd->GetKeyState(CSKEY_SHIFT))
  {
    origin = st.isect;
    originSector = sectorList->Get(0);
    originSet = true;
  }
  else
  {
    destination = st.isect;
    destinationSector = sectorList->Get(0);
    destinationSet = true;
  }

  if (originSet && destinationSet && navStruct)
  {
    path = navStruct->ShortestPath(origin, originSector, destination, destinationSector);
  }
}

// right
void MainApp::MouseClick2Handler (iEvent& ev)
{
}

// middle
void MainApp::MouseClick3Handler (iEvent& ev)
{
}

bool MainApp::OnInitialize (int argc, char* argv[])
{
  if (!celInitializer::RequestPlugins(object_reg,
        CS_REQUEST_VFS,
    	CS_REQUEST_OPENGL3D,
    	CS_REQUEST_ENGINE,
    	CS_REQUEST_FONTSERVER,
    	CS_REQUEST_IMAGELOADER,
    	CS_REQUEST_LEVELLOADER,
    	CS_REQUEST_REPORTER,
    	CS_REQUEST_REPORTERLISTENER,
    	CS_REQUEST_PLUGIN("cel.physicallayer", iCelPlLayer),
    	CS_REQUEST_PLUGIN("crystalspace.collisiondetection.opcode", iCollideSystem),
        CS_REQUEST_PLUGIN("cel.hnavstructbuilder", iCelHNavStructBuilder),
        CS_REQUEST_END))
  {
    return ReportError ("Can't initialize plugins!");
  }

  csBaseEventHandler::Initialize(object_reg);

  if (!RegisterQueue(object_reg, csevAllEvents(object_reg)))
  {
    return ReportError ("Can't setup event handler!");
  }

  return true;
}

bool MainApp::Application ()
{
  if (!OpenApplication(object_reg))
  {
    return ReportError("Error opening system!");
  }

  if (!LoadPlugins())
  {
    return ReportError("Error loading plugins!");
  }

  if (!LoadLevel())
  {
    return ReportError("Error loading level!");
  }
  if (!CreatePlayer())
  {
    return ReportError("Couldn't create player!");
  }

  printer.AttachNew(new FramePrinter(object_reg));

  Run();

  return true;
}

bool MainApp::LoadPlugins ()
{
  g3d = csQueryRegistry<iGraphics3D>(object_reg);
  if (!g3d)
  {
    return ReportError("Failed to locate 3D renderer!");
  }

  engine = csQueryRegistry<iEngine>(object_reg);
  if (!engine)
  {
    return ReportError("Failed to locate 3D engine!");
  }

  vc = csQueryRegistry<iVirtualClock>(object_reg);
  if (!vc)
  {
    return ReportError("Failed to locate Virtual Clock!");
  }

  kbd = csQueryRegistry<iKeyboardDriver>(object_reg);
  if (!kbd)
  {
    return ReportError("Failed to locate Keyboard Driver!");
  }

  loader = csQueryRegistry<iLoader>(object_reg);
  if (!loader)
  {
    return ReportError("Failed to locate Loader!");
  }

  vfs = csQueryRegistry<iVFS>(object_reg);
  if (!vfs)
  {
    return ReportError("Failed to locate VFS!");
  }

  physicalLayer = csQueryRegistry<iCelPlLayer>(object_reg);
  if (!physicalLayer)
  {
    return ReportError("Failed to locate Physical Layer!");
  }

  behaviourLayer.AttachNew(new BehaviourLayer(physicalLayer, object_reg));
  if (!object_reg->Register(behaviourLayer, "iCelBlLayer"))
  {
    return ReportError("Can't register our behaviour layer!");
  }
  physicalLayer->RegisterBehaviourLayer(behaviourLayer);

  navStructBuilder = csQueryRegistry<iCelHNavStructBuilder>(object_reg);
  if (!navStructBuilder)
  {
    return ReportError("Failed to locate Navigation Structure Builder");
  }

  return true;
}

void MainApp::OnExit ()
{
  printer.Invalidate();
}
