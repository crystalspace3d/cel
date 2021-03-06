/*
    Copyright (C) 2010 by Leonardo Rodrigo Domingues

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include "navmeshtest.h"
#include "imesh/gmeshskel2.h"
#include "imesh/skeleton.h"

MainApp::MainApp () 
  : DemoApplication ("CrystalSpace.NavMeshTest"),
    scfImplementationType (this)
{
  originSet = false;
  destinationSet = false;
  navStructMeshes = 0;
  pathMeshes = 0;
  pathEndMeshes = 0;
  clearMeshes = false;
  updateMeshes = false;
  updatePathMeshes = false;
  updateOriginMeshes = false;
  updateDestinationMeshes = false;
}

MainApp::~MainApp () 
{
}

void MainApp::Frame ()
{

  // First get elapsed time from the virtual clock.
  csTicks elapsed_time = vc->GetElapsedTicks();

  csVector3 obj_move(0);
  csVector3 obj_rotate(0);

  if (kbd->GetKeyState(CSKEY_SHIFT))
  {
    // If the user is holding down shift, the arrow keys will cause
    // the camera to strafe up, down, left or right from it's
    // current position.
    if (kbd->GetKeyState(CSKEY_RIGHT))
    {
      obj_move = CS_VEC_RIGHT * 3.0f;
    }
    if (kbd->GetKeyState(CSKEY_LEFT))
    {
      obj_move = CS_VEC_LEFT * 3.0f;
    }
    if (kbd->GetKeyState(CSKEY_UP))
    {
      obj_move = CS_VEC_UP * 3.0f;
    }
    if (kbd->GetKeyState(CSKEY_DOWN))
    {
      obj_move = CS_VEC_DOWN * 3.0f;
    }
  } 
  else
  {
    // left and right cause the camera to rotate on the global Y
    // axis; page up and page down cause the camera to rotate on the
    // _camera's_ X axis (more on this in a second) and up and down
    // arrows cause the camera to go forwards and backwards.
    if (kbd->GetKeyState(CSKEY_RIGHT))
    {
      obj_rotate.Set(0, 1, 0);
    }
    if (kbd->GetKeyState(CSKEY_LEFT))
    {
      obj_rotate.Set(0, -1, 0);
    }
    if (kbd->GetKeyState(CSKEY_PGUP))
    {
      obj_rotate.Set(1, 0, 0);
    }
    if (kbd->GetKeyState(CSKEY_PGDN))
    {
      obj_rotate.Set(-1, 0, 0);
    }
    if (kbd->GetKeyState(CSKEY_UP))
    {
      obj_move = CS_VEC_FORWARD * 3.0f;
    }
    if (kbd->GetKeyState (CSKEY_DOWN))
    {
      obj_move = CS_VEC_BACKWARD * 3.0f;
    }
  }

  collider_actor.Move(float(elapsed_time) / 1000.0f, 1.0f, obj_move, obj_rotate);

  // Tell 3D driver we're going to display 3D things.
  //if (!g3d->BeginDraw(engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS)) {
  //	return;
  //}

  // Tell the camera to render into the frame buffer.
  rm->RenderView(view);

  if (navStructMeshes)
  {
    csArray<csSimpleRenderMesh*>::Iterator it = navStructMeshes->GetIterator();
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(*it.Next());
    }
    
  }

  if (pathMeshes && path)
  {
    csArray<csSimpleRenderMesh*>::Iterator it = pathMeshes->GetIterator();
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(*it.Next());
    }
  }

  if (pathEndMeshes)
  {
    csArray<csSimpleRenderMesh*>::Iterator it = pathEndMeshes->GetIterator();
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(*it.Next());
    }
  }

  hudManager->GetStateDescriptions ()->Empty ();
  csString msg;
  if (mapNodeOrigin)
  {
    msg.Format ("Origin mapnode: %s (sector %s) index %zu", mapNodeOrigin->QueryObject ()->GetName (),
      mapNodeOrigin->GetSector ()->QueryObject ()->GetName (), mapNodes.Find (mapNodeOrigin));
    hudManager->GetStateDescriptions ()->Push (msg.GetData ());
  }
  if (mapNodeTarget)
  {
    msg.Format ("Target mapnode: %s (sector %s) index %zu", mapNodeTarget->QueryObject ()->GetName (),
      mapNodeTarget->GetSector ()->QueryObject ()->GetName (), mapNodes.Find (mapNodeTarget));
    hudManager->GetStateDescriptions ()->Push (msg.GetData ());
  }

  if (mapNodes.GetSize () < 2) return;

  if (!navStruct)
    hudManager->GetStateDescriptions ()->Push ("Path distance: a navmesh is needed, press 'b' or load one first.");
  else
  {
    float nodesDistance = GetMapNodesDistance ();
    if (nodesDistance < 0)
      hudManager->GetStateDescriptions ()->Push ("Path distance: no path between these two mapnodes");
    else
    {
      csString msg;
      msg.Format ("Path distance: %f", nodesDistance);
      hudManager->GetStateDescriptions ()->Push (msg.GetData ());
    }
  }
}

bool MainApp::OnKeyboard(iEvent& ev) 
{
  csKeyEventType eventtype = csKeyEventHelper::GetEventType(&ev);
  if (eventtype == csKeyEventTypeDown) 
  {
    utf32_char code = csKeyEventHelper::GetCookedCode(&ev);
    if (code == CSKEY_ESC) 
    {
      csRef<iEventQueue> q = csQueryRegistry<iEventQueue>(object_reg);
      if (q.IsValid ()) 
      {
        q->GetEventOutlet()->Broadcast(csevQuit(object_reg));
      }
    }
    else if (code == 'b') // Build navstruct
    {
      CS::MeasureTime measure ("Total time to build the navigation structure");
      navStruct.Invalidate();
      // Get agent settings values from command line
      // or use default values adapted to the castle level
      float AgentHeight;
      if (clp->GetOption ("height"))
        csScanStr (clp->GetOption ("height"), "%f", &AgentHeight);
      else AgentHeight = 1.0f; // Default for castle level
      float AgentRadius;
      if (clp->GetOption ("radius"))
        csScanStr (clp->GetOption ("radius"), "%f", &AgentRadius);
      else AgentRadius = 0.2f;
      float AgentMaxSlope;
      if (clp->GetOption ("maxslope"))
        csScanStr (clp->GetOption ("maxslope"), "%f", &AgentMaxSlope);
      else AgentMaxSlope = 45.0f;
      float AgentMaxClimb;
      if (clp->GetOption ("maxclimb"))
        csScanStr (clp->GetOption ("maxclimb"), "%f", &AgentMaxClimb);
      else AgentMaxClimb = 0.5f;

      if (!params)
      {
        params.AttachNew(navStructBuilder->GetNavMeshParams()->Clone());
        params->SetSuggestedValues(AgentHeight, AgentRadius, AgentMaxSlope);
        params->SetAgentMaxClimb (AgentMaxClimb);
      }
      navStructBuilder->SetNavMeshParams(params);
      csPrintf ("AgentHeight = %f\n", params->GetAgentHeight ());
      csPrintf ("AgentRadius = %f\n", params->GetAgentRadius ());
      csPrintf ("AgentMaxSlope = %f\n", params->GetAgentMaxSlopeAngle ());
      csPrintf ("AgentMaxClimb = %f\n", params->GetAgentMaxClimb ());
     
      csRefArray<iSector> sectorList;
      int size = engine->GetSectors()->GetCount();
      for (int i = 0; i < size; i++)
      {
        sectorList.Push(engine->GetSectors()->Get(i));    
      }
      navStructBuilder->SetSectors(&sectorList);
      navStruct = navStructBuilder->BuildHNavStruct();

      navStructMeshes = navStruct->GetDebugMeshes(currentSector);

      GetMapNodes ();
    }
    else if (code == 's') // Save navstruct
    {
      if (navStruct)
      {
        CS::MeasureTime measure ("Total time to save the navigation structure");
        navStruct->SaveToFile(vfs, "navigationStructure.zip");
      }
    }
    else if (code == 'l') // Load navstruct
    {
      CS::MeasureTime measure ("Total time to load the navigation structure");
      navStruct.Invalidate();
      navStruct = navStructBuilder->LoadHNavStruct(vfs, "navigationStructure.zip");
      if (navStruct)
      {
        navStructMeshes = navStruct->GetDebugMeshes(currentSector);
        GetMapNodes ();
      }
      else csPrintf ("Couldn't load %s!\n", CS::Quote::Single ("navigationStructure.zip"));
    }
    else if (code == 'c') // Clear navstruct, positions and path
    {
      navStruct.Invalidate();
      path.Invalidate();
      originSet = false;
      destinationSet = false;
      pathMeshes = 0;
      pathEndMeshes = 0;
      navStructMeshes = 0;
    }
    else if (code == 'e') // Change the path origin mapnode
    {
      CycleMapNode (mapNodeOrigin);
    }
    else if (code == 'r') // Change the path destination mapnode
    {
      CycleMapNode (mapNodeTarget);
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
    return;

  csVector2 screenPoint;
  screenPoint.x = csMouseEventHelper::GetX(&ev);
  screenPoint.y = csMouseEventHelper::GetY(&ev);

  csScreenTargetResult st = csEngineTools::FindScreenTarget(screenPoint, 10000.0f, view);
  if (!st.mesh)
    return;

  csRef<iSectorList> sectorList = st.mesh->GetMovable()->GetSectors();
  if (sectorList->GetCount() != 1)
  {
    return;
  }
  
  if (kbd->GetKeyState(CSKEY_SHIFT))
  {
    originSet = true;
    origin = st.isect;
    originSector = sectorList->Get(0);

    navStruct->ResetAgentDebugMeshes();
    pathEndMeshes = navStruct->GetAgentDebugMeshes(origin, 70, 140, 255, 150);

    if (destinationSet && navStruct)
    {
      pathEndMeshes = navStruct->GetAgentDebugMeshes(destination, 50, 255, 120, 150);
      path = navStruct->ShortestPath(origin, originSector, destination, destinationSector);
      if (path)
      {
        pathMeshes = path->GetDebugMeshes();
      }
    }
  }
  else
  {
    destinationSet = true;
    destination = st.isect;
    destinationSector = sectorList->Get(0);

    navStruct->ResetAgentDebugMeshes();
    pathEndMeshes = navStruct->GetAgentDebugMeshes(destination, 50, 255, 120, 150);

    if (originSet && navStruct)
    {
      pathEndMeshes = navStruct->GetAgentDebugMeshes(origin, 70, 140, 255, 150);
      path = navStruct->ShortestPath(origin, originSector, destination, destinationSector);
      if (path)
      {
        pathMeshes = path->GetDebugMeshes();
      }
    }
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

void MainApp::PrintHelp ()
{
  csCommandLineHelper commandLineHelper;

  // Printing help
  commandLineHelper.AddCommandLineOption ("height", "Agent height", csVariant (1.0f));
  commandLineHelper.AddCommandLineOption ("radius", "Agent radius", csVariant (0.2f));
  commandLineHelper.AddCommandLineOption ("maxslope", "Maximum slope angle an agent can walk through, in degrees", csVariant (45.0f));
  commandLineHelper.AddCommandLineOption ("maxclimb", "Maximum height an agent can climb", csVariant (1.0f));
  commandLineHelper.AddCommandLineExample ("navmeshtest -height=1.8 -radius=0.4 -maxclimb=1.2 /this");
  commandLineHelper.PrintApplicationHelp
    (GetObjectRegistry (), "navmeshtest", "navmeshtest [OPTIONS] [VFS directory|terrain|castle]", "App to build, save and test NavMeshes.");
}


bool MainApp::OnInitialize (int argc, char* argv[]) 
{
  // Default behavior from DemoApplication
  if (!DemoApplication::OnInitialize (argc, argv))
    return false;

  mapLocation = clp->GetName ();
  if (mapLocation.IsEmpty () || mapLocation == "castle")
    mapLocation = "/lev/castle";
  else if (mapLocation == "terrain")
    mapLocation = "/lev/terrainf";

  if (!csInitializer::RequestPlugins(object_reg,
    CS_REQUEST_PLUGIN("crystalspace.collisiondetection.opcode", iCollideSystem),
    CS_REQUEST_PLUGIN("cel.hnavstructbuilder", iCelHNavStructBuilder),    
    CS_REQUEST_END)) 
  {
      return ReportError("Failed to initialize plugins!");
  }

  return true;
}

void MainApp::OnExit ()
{
  view->GetCamera ()->RemoveCameraListener (this);
}

bool MainApp::Application ()
{
  // Default behavior from DemoApplication
  if (!DemoApplication::Application ())
    return false;

  if (!SetupModules())
  {
    return false;
  }

  // Here we load our world from a map file.
  if (!LoadMap())
  {
    return false;
  }

  // Initialize collision objects for all loaded objects.
  csColliderHelper::InitializeCollisionWrappers(cdsys, engine);

  engine->Prepare();

  // Initialize our collider actor.
  collider_actor.SetCollideSystem(cdsys);
  collider_actor.SetEngine(engine);
  csVector3 legs(.2f, .3f, .2f);
  csVector3 body(.2f, 1.2f, .2f);
  csVector3 shift(0, -1, 0);
  collider_actor.InitializeColliders(view->GetCamera(), legs, body, shift);

  collider_actor.SetGravity(0);

  // Call back NewSector() on sector changes, to filter debug meshes
  view->GetCamera ()->AddCameraListener (this);

  // Define the available keys
  hudManager->GetKeyDescriptions ()->Empty();
  hudManager->GetKeyDescriptions ()->Push ("b: build NavMesh");
  hudManager->GetKeyDescriptions ()->Push ("l: load NavMesh");
  hudManager->GetKeyDescriptions ()->Push ("esc: exit application");
  hudManager->GetKeyDescriptions ()->Push ("-");
  hudManager->GetKeyDescriptions ()->Push ("With NavMesh built/loaded:");
  hudManager->GetKeyDescriptions ()->Push ("s: save NavMesh");
  hudManager->GetKeyDescriptions ()->Push ("c: clear NavMesh");
  hudManager->GetKeyDescriptions ()->Push ("left mouse click: set path origin");
  hudManager->GetKeyDescriptions ()->Push ("shift+left mouse click: set path destination");

  // Find all the map nodes
  if (GetMapNodes () > 1)
  {
    mapNodeOrigin = mapNodes[0];
    mapNodeTarget = mapNodes[1];
    hudManager->GetKeyDescriptions ()->Push ("e: cycle origin mapnode");
    hudManager->GetKeyDescriptions ()->Push ("r: cycle destination mapnode");
  }
  csPrintf ("%zu mapnodes found.\n", mapNodes.GetSize ());

  Run();

  return true;
}

bool MainApp::SetupModules ()
{
  cdsys = csQueryRegistry<iCollideSystem>(object_reg);
  if (!cdsys)
  {
    return ReportError ("Failed to locate CD system!");
  }

  navStructBuilder = csQueryRegistry<iCelHNavStructBuilder>(object_reg);
  if (!navStructBuilder)
  {
    return ReportError("Failed to locate Navigation Structure Builder");
  }

  rm = engine->GetRenderManager();

  return true;
}

bool MainApp::LoadMap () {

  // Set VFS current directory to the level we want to load.
  vfs->ChDir(mapLocation.GetDataSafe());
  // Load the level file which is called 'world'.
  if (!loader->LoadMapFile("world"))
  {
    return ReportError("Error couldn't load level!");
  }

    // Find the starting position in this level.
  if (engine->GetCameraPositions()->GetCount() > 0)
  {
    // There is a valid starting position defined in the level file.
    iCameraPosition* campos = engine->GetCameraPositions()->Get(0);
    currentSector = engine->GetSectors()->FindByName(campos->GetSector());
    agentStartPosition = campos->GetPosition();
  } 
  else
  {
    // We didn't find a valid starting position. So we default
    // to going to room called 'room' at position (0,0,0).
    currentSector = engine->GetSectors()->FindByName("room");
    agentStartPosition = csVector3(0, 0, 0);
  }
  if (!currentSector)
  {
    return ReportError("Can't find a valid starting position!");
  }

  view->GetCamera()->SetSector(currentSector);
  view->GetCamera()->GetTransform().SetOrigin(agentStartPosition);
  agentStartPosition += csVector3(1, 1, 1);

  return true;
}

bool MainApp::CreateAgent ()
{
  // TODO change back dir or use another vfs if this method is used
  vfs->PushDir();
  vfs->ChDir("/lib/kwartz");
  if (!loader->LoadLibraryFile("kwartz.lib"))
  {
    return ReportError ("Error loading kwartz!");
  }
  vfs->PopDir();

  iMeshFactoryWrapper* imeshfact = engine->FindMeshFactory("kwartz_fact");

  //csMatrix3 m; m.Identity ();
  //imeshfact->HardTransform (csReversibleTransform (m, csVector3 (0, -1, 0)));

  // Create the sprite and add it to the engine.
  actor = engine->CreateMeshWrapper(imeshfact, "MySprite", currentSector, agentStartPosition);
  csMatrix3 m;
  m.Identity();
  m /= 5.0;
  csReversibleTransform scaleTransform(m, csVector3(0, 0, 0));
  actor->HardTransform(scaleTransform);
  actor->GetMovable()->UpdateMove();
  csRef<iGeneralMeshState> spstate(scfQueryInterface<iGeneralMeshState>(actor->GetMeshObject()));
  csRef<iGenMeshSkeletonControlState> animcontrol(
      scfQueryInterface<iGenMeshSkeletonControlState>(spstate->GetAnimationControl()));
  csRef<iSkeleton> skel = animcontrol->GetSkeleton();
  skel->StopAll();
  skel->Execute("idle");

  // The following two calls are not needed since CS_ZBUF_USE and
  // Object render priority are the default but they show how you
  // can do this.
  actor->SetZBufMode(CS_ZBUF_USE);
  actor->SetRenderPriority(engine->GetObjectRenderPriority());
  return true;
}

int MainApp::GetMapNodes ()
{
  mapNodes.Empty ();
  // Search in all sectors
  iSectorList* slist = engine->GetSectors ();
  for (int i = 0; i < slist->GetCount (); i++)
  {
    iSector* sector = slist->Get (i);
    csRef<iObjectIterator> it = sector->QueryObject ()->GetIterator ();
    while (it->HasNext ())
    {
      iObject* obj = it->Next ();
      csRef<iMapNode> mapnode = scfQueryInterface<iMapNode> (obj);
      // Ignore mapnodes created by recast plugin :
      // Building a navmesh create mapnodes named "hlg" (high-level graph), and
      // loading create mapnodes named "n".
      if (mapnode && (strcmp(obj->GetName (), "hlg")) && (strcmp(obj->GetName (), "n")))
      { 
        csVector3 pos (mapnode->GetPosition ());
        csPrintf ("Map node %s at %f,%f,%f in sector %s\n", CS::Quote::Single (obj->GetName ()),
          pos.x, pos.y, pos.z, mapnode->GetSector ()->QueryObject ()->GetName ());
        mapNodes.Push (mapnode); 
      }
    }
  }
  return mapNodes.GetSize ();
}

void MainApp::CycleMapNode (csRef<iMapNode>& mapnode)
{
  // Change the selected mapnode to the next one
  size_t idx = mapNodes.Find (mapnode);
  if (idx++ < mapNodes.GetSize () - 1)
    mapnode = mapNodes[idx];
  else
    mapnode = mapNodes[0];
}

float MainApp::GetMapNodesDistance ()
{
  // Get distance between the two map nodes. Set to -1.0 if there's no path.
  if (navStruct && mapNodeOrigin && mapNodeTarget)
  {
    path.Invalidate ();
    path = navStruct->ShortestPath (mapNodeOrigin, mapNodeTarget);
    if (path)
      return path->GetDistance ();
  }
  return -1.0f;
}

void MainApp::NewSector (iCamera* /*camera*/, iSector* sector)
{
  currentSector = sector;
  csPrintf ("Now in sector %s\n", CS::Quote::Single (
    currentSector->QueryObject ()->GetName ()));
  if (navStruct)
    navStructMeshes = navStruct->GetDebugMeshes(currentSector);
}
 
