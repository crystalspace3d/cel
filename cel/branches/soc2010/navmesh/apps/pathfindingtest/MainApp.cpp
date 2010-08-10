#include "MainApp.h"

MainApp::MainApp () 
{
  SetApplicationName("Navigation Mesh Test");
  destinationSet = false;
  renderNavMesh = true;
  renderDestination = true;
  renderPath = true;
  renderBox = true;
  updateNavmesh = false;
  updateArea = csBox3(csVector3(24.3f, 1.0f, 13.7f), csVector3(25.3f, 2.0f, 21.3f));
}

MainApp::~MainApp () 
{
}

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
  mapfile->SetPath("/lev/castle");  
  mapfile->SetFile("world");
  vfs->ChDir("/lev/castle");

  return true;
}

bool MainApp::CreatePlayer ()
{
  playerEntity = physicalLayer->CreateEntity("player", behaviourLayer, "playerBehaviour", "pccamera.old", 
                                   "pcobject.mesh", "pcmove.linear", "pcmove.actor.standard", "pcmove.mover",
                                   "pcinput.standard", CEL_PROPCLASS_END);
  if (!playerEntity)
  {
    return ReportError("Error creating player entity!");
  }

  // Get the iPcCamera interface so that we can set the camera.
  pcCamera = CEL_QUERY_PROPCLASS_ENT(playerEntity, iPcCamera);
  camera = pcCamera->GetCamera();

  // Since we want to be able to see the navigation meshes and paths, we have to turn AutoDraw off
  // and Draw the camera manually in the Frame method, along with the navmeshes and in the right
  // order.
  pcCamera->SetAutoDraw(false);

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
  csRef<iObjectModel> objectModel = mesh->GetMeshObject()->GetObjectModel();

  // Get height before scaling the model, and then scale it.
  // At the time this demo application was created, the model's bounding box was not updated
  // after scaling it, so this is the safest way to get the height.  
  float x = objectModel->GetObjectBoundingBox().MaxY() - objectModel->GetObjectBoundingBox().MinY();
  float y = objectModel->GetObjectBoundingBox().MaxY() - objectModel->GetObjectBoundingBox().MinY();
  float z = objectModel->GetObjectBoundingBox().MaxY() - objectModel->GetObjectBoundingBox().MinY();
  
  // Scale model. A tiny model will be used, since her bounding box is big and the map has
  // a lot of closed spaces.
  float scaleFactor = 0.25f;
  csRef<iSpriteCal3DFactoryState> cal3dSprite = scfQueryInterface<iSpriteCal3DFactoryState> 
      (mesh->GetFactory()->GetMeshObjectFactory());
  cal3dSprite->RescaleFactory(scaleFactor);

  // Scale bounding box
  x *= scaleFactor;
  y *= scaleFactor;
  z *= scaleFactor;

  // Calculate approximate height and radius. Height will be the height of the bounding box, and
  // radius will be half of the diagonal lenght of the bounding box, in the xz plane.
  agentHeight = y;
  agentRadius = csQsqrt(csSquare(x) + csSquare(z)) * 0.5f;

  if (pcZoneMgr->PointMesh("player", "main", "Camera"))
  {
    return ReportError("Can't find region or start position in region!");
  }

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pcLinMove = CEL_QUERY_PROPCLASS_ENT(playerEntity, iPcLinearMovement);
  pcLinMove->InitCD(csVector3(0.5f,0.8f,0.5f), csVector3(0.5f,0.4f,0.5f), csVector3(0,0,0));
  pcLinMove->SetFullPosition(pcLinMove->GetPosition(), PI, pcLinMove->GetSector());

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcActorMove = CEL_QUERY_PROPCLASS_ENT (playerEntity, iPcActorMove);
  pcActorMove->SetMovementSpeed(1.5f);
  pcActorMove->SetRunningSpeed(2.5f);
  pcActorMove->SetRotationSpeed(1.75f);
  pcActorMove->ToggleCameraMode();

  // Remove the smooth behaviour from iPcMover (it causes our actor to walk where we
  // don't want it to go).
  csRef<iPcMover> pcMover = CEL_QUERY_PROPCLASS_ENT (playerEntity, iPcMover);
  pcMover->SetSmoothMovement(false);

  // Get iPcCommandInput so we can do key bindings. The behaviour layer will interprete the 
  // commands so the actor can move.
  csRef<iPcCommandInput> pcInput = CEL_QUERY_PROPCLASS_ENT(playerEntity, iPcCommandInput);

  // We read the key bindings from the standard config file.
  pcInput->Bind("up", "forward");
  pcInput->Bind("down", "backward");
  pcInput->Bind("left", "rotateleft");
  pcInput->Bind("right", "rotateright");
  pcInput->Bind("m", "cammode");
  pcInput->Bind("MouseButton0", "setposition");

  return true;
}

bool MainApp::CreateBox ()
{
  boxEntity = physicalLayer->CreateEntity("box", behaviourLayer, "boxBehaviour", "pccamera.old",
                                          "pcobject.mesh", "pcmove.linear", "pcmove.actor.standard",
                                          "pcmove.mover", CEL_PROPCLASS_END);
  if (!boxEntity)
  {
    return ReportError("Error creating box entity!");
  }

  // Get the iPcCamera interface so that we can set the camera.
  csRef<iPcCamera> pcCamera = CEL_QUERY_PROPCLASS_ENT(boxEntity, iPcCamera);
  pcCamera->SetAutoDraw(false);

  // Get the zone manager from the level entity which should have been created by now.
  csRef<iPcZoneManager> pcZoneMgr = CEL_QUERY_PROPCLASS_ENT(levelEntity, iPcZoneManager);
  pcCamera->SetZoneManager(pcZoneMgr, true, "main", "Camera");

  // Create mesh
  using namespace CS::Geometry;
  csBox3 boundingBox;
  boundingBox.SetCenter(csVector3(1.5f, 1.5f, 1.5f));
  boundingBox.SetSize(csVector3(1.0f, 1.0f, 1.0f));
  Box box(boundingBox);
  //Box box(csVector3(-0.5f, -0.5f, -0.5f), csVector3(0.5f, 0.5f, 0.5f));
  csRef<iSector> sector = engine->FindSector("interior");
  csRef<iMeshWrapper> mesh = GeneralMeshBuilder::CreateFactoryAndMesh(engine, sector, "cube", "cubeFact", &box);
  if (!loader->LoadTexture ("stone", "/lib/std/stone4.gif"))
  {
    ReportWarning ("Could not load texture 'stone'");
  }
  iMaterialWrapper* materialWrapper = engine->GetMaterialList()->FindByName("stone");
  mesh->GetMeshObject()->SetMaterialWrapper(materialWrapper);

  // Get the iPcMesh interface so we can load the right mesh for our box.
  csRef<iPcMesh> pcMesh = CEL_QUERY_PROPCLASS_ENT(boxEntity, iPcMesh);
  pcMesh->SetMesh(mesh);
  
  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pcLinMove = CEL_QUERY_PROPCLASS_ENT(boxEntity, iPcLinearMovement);
  pcLinMove->InitCD(mesh, 100.0f);

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcActorMove = CEL_QUERY_PROPCLASS_ENT (boxEntity, iPcActorMove);
  pcActorMove->SetMovementSpeed(1.5f);
  pcActorMove->SetRunningSpeed(2.5f);
  pcActorMove->SetRotationSpeed(1.75f);
  pcActorMove->ToggleCameraMode();

  // Remove the smooth behaviour from iPcMover (it causes our actor to walk where we
  // don't want it to go).
  csRef<iPcMover> pcMover = CEL_QUERY_PROPCLASS_ENT (boxEntity, iPcMover);
  pcMover->SetSmoothMovement(false);
  csVector3 point1(24.8f, 1.0f, 13.7f);
  csVector3 point2(24.8f, 1.0f, 21.3f);
  pcLinMove->SetFullPosition(point2, PI, sector);
  pcMover->MoveTo(sector, point1, 0.005f);

  return true;
}

void MainApp::Frame ()
{
  pcCamera->Draw();

  if (renderNavMesh && navStruct)
  {
    navStruct->DebugRender();
  }

  if (renderDestination && destinationSet && navStruct)
  {
    navStruct->DebugRenderAgent(destination, 50, 255, 120, 150);
  }

  path = behaviourLayer->GetPath();
  if (renderPath && path)
  {
    path->DebugRender();
  }

  if (navStruct && updateNavmesh)
  {
    navStruct->Update(updateArea, engine->FindSector("interior"));
  }
}

bool MainApp::OnKeyboard(iEvent& ev)
{
  // We got a keyboard event.
  csKeyEventType eventType = csKeyEventHelper::GetEventType(&ev);
  if (eventType == csKeyEventTypeUp)
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
    else if (code == 'b') // Build navstruct
    {
      navStruct.Invalidate();
      path.Invalidate();
      destinationSet = false;
      behaviourLayer->SetPath(0);
      if (!params)
      {
        params.AttachNew(navStructBuilder->GetNavMeshParams()->Clone());
        params->SetSuggestedValues(agentHeight, agentRadius, 45.0f);
        // Our agent is tiny and the map has stairs, so lets change agentMaxClimb so she can
        // go everywhere.
        params->SetAgentMaxClimb(3.0f * agentHeight / 4.0f);
        // Reduce minium region size so we can see the navmeshes inside the cells in the dungeon
        // sector. The cells remain inaccessible however, since the model's bounding box is too
        // big.
        params->SetMinRegionSize(10);
        // This parameter sets how far from the navmesh I can click and still get an approximated
        // path (for example, clicking on walls and ceilings). It also determines how far the final
        // node of a path can be from it's intended destination (Detour always returns a path, 
        // either to the destination or the closest possible point. We need to see if the path reached
        // the destination and was off by some approximation factor or didn't reach the destination at all).
        params->SetPolygonSearchBox(csVector3(2, 4, 2));
        navStructBuilder->SetNavMeshParams(params);
      }
      csList<iSector*> sectorList;
      int size = engine->GetSectors()->GetCount();
      for (int i = 0; i < size; i++)
      {
        sectorList.PushBack(engine->GetSectors()->Get(i));    
      }
      navStructBuilder->SetSectors(sectorList);
      navStruct = navStructBuilder->BuildHNavStruct();
      behaviourLayer->SetNavStruct(navStruct);
    }
    else if (code == 's') // Save navstruct
    {
      if (navStruct)
      {
        navStruct->SaveToFile(vfs, "navigationStructure.zip");
      }
    }
    else if (code == 'l') // Load navstruct
    {
      navStruct.Invalidate();
      navStruct = navStructBuilder->LoadHNavStruct(vfs, "navigationStructure.zip");
      behaviourLayer->SetNavStruct(navStruct);
    }
    else if (code == 'c') // Clear navstruct, positions and path
    {
      navStruct.Invalidate();
      path.Invalidate();
      destinationSet = false;
      behaviourLayer->SetPath(0);
    }
    else if (code == '1') // Switch navmesh rendering
    {
      renderNavMesh = !renderNavMesh;
    }
    else if (code == '2') // Switch destination rendering
    {
      renderDestination = !renderDestination;
    }
    else if (code == '3') // Switch path rendering
    {
      renderPath = !renderPath;
    }
    else if (code == '4') // Switch stone block rendering
    {
      if (renderBox)
      {
        csRef<iPcMesh> pcMesh = CEL_QUERY_PROPCLASS_ENT(boxEntity, iPcMesh);
        pcMesh->Hide();
        renderBox = false;
      }
      else
      {
        csRef<iPcMesh> pcMesh = CEL_QUERY_PROPCLASS_ENT(boxEntity, iPcMesh);
        pcMesh->Show();
        renderBox = true;
      }

    }
    else if (code == 'u') // Update navmesh now
    {
      navStruct->Update(updateArea, engine->FindSector("interior"));
      
    }
    else if (code == 'y') // Update navmesh every time the big stone block moves
    {
      updateNavmesh = !updateNavmesh;
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
  if (!navStruct)
  {
    return;
  }

  csVector2 screenPoint;
  screenPoint.x = csMouseEventHelper::GetX(&ev);
  screenPoint.y = csMouseEventHelper::GetY(&ev);

  csScreenTargetResult st = csEngineTools::FindScreenTarget(screenPoint, 10000.0f, camera);
  csRef<iSectorList> sectorList = st.mesh->GetMovable()->GetSectors();
  if (sectorList->GetCount() != 1)
  {
    return;
  }
  
  destination = st.isect;
  destinationSector = sectorList->Get(0);
  destinationSet = true;

  if (destinationSet && navStruct)
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
  if (!CreateBox())
  {
    return ReportError("Couldn't create box!");
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
