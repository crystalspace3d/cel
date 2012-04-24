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

inline void disposeDebugMeshes(csList<csSimpleRenderMesh>* meshes)
{
  if (meshes)
  {
    csList<csSimpleRenderMesh>::Iterator it(*meshes);
    while (it.HasNext())
    {
      csSimpleRenderMesh mesh = it.Next();
      delete [] mesh.vertices;
      delete [] mesh.colors;
    }
    delete meshes;
  }
}

MainApp::MainApp () 
{
  SetApplicationName("Navigation Mesh Test");
  originSet = false;
  destinationSet = false;
  navStructMeshes = 0;
  pathMeshes = 0;
  originMeshes = 0;
  destinationMeshes = 0;
  clearMeshes = false;
  updateMeshes = false;
  updatePathMeshes = false;
  updateOriginMeshes = false;
  updateDestinationMeshes = false;
}

MainApp::~MainApp () 
{
  disposeDebugMeshes(navStructMeshes);
  disposeDebugMeshes(pathMeshes);
  disposeDebugMeshes(originMeshes);
  disposeDebugMeshes(destinationMeshes);
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
    csList<csSimpleRenderMesh>::Iterator it(*navStructMeshes);
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(it.Next());
    }
    
  }

  if (originMeshes)
  {
    csList<csSimpleRenderMesh>::Iterator it(*originMeshes);
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(it.Next());
    }
  }

  if (destinationMeshes)
  {
    csList<csSimpleRenderMesh>::Iterator it(*destinationMeshes);
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(it.Next());
    }
  }

  if (pathMeshes && path)
  {
    csList<csSimpleRenderMesh>::Iterator it(*pathMeshes);
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(it.Next());
    }
  }

  if (!navStruct)
    return;

  // If we just modifying the meshes in the OnKeyboard method, they may be deleted
  // while rendering a frame, causing a crash.
  if (clearMeshes || updateMeshes)
  {
    disposeDebugMeshes(navStructMeshes);
    navStructMeshes = 0;
    if (updateMeshes)
    {
      navStructMeshes = navStruct->GetDebugMeshes();
      updateMeshes = false;
    }
  }
  if (clearMeshes || updatePathMeshes)
  {
    disposeDebugMeshes(pathMeshes);
    pathMeshes = 0;
    if (updatePathMeshes)
    {
      pathMeshes = path->GetDebugMeshes();
      updatePathMeshes = false;
    }
  }
  if (clearMeshes || updateOriginMeshes)
  {
    disposeDebugMeshes(originMeshes);
    originMeshes = 0;
    originSet = false;
    if (updateOriginMeshes)
    {
      originMeshes = navStruct->GetAgentDebugMeshes(origin, 70, 140, 255, 150);
      updateOriginMeshes = false;
      originSet = true;
    }
  }
  if (clearMeshes || updateDestinationMeshes)
  {
    disposeDebugMeshes(destinationMeshes);
    destinationMeshes = 0;
    destinationSet = false;
    if (updateDestinationMeshes)
    {
      destinationMeshes = navStruct->GetAgentDebugMeshes(destination, 50, 255, 120, 150);
      updateDestinationMeshes = false;
      destinationSet = true;
    }
  }
  clearMeshes = false;
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
      navStruct.Invalidate();
      if (!params)
      {
        params.AttachNew(navStructBuilder->GetNavMeshParams()->Clone());
        if (mapLocation == "/lev/castle")
        {
          params->SetSuggestedValues(1.0f, 0.2f, 45.0f);
        }
        else
        {
          params->SetSuggestedValues(10.0f, 2.0f, 45.0f);
        }
        navStructBuilder->SetNavMeshParams(params);
      }
      navStructBuilder->SetNavMeshParams(params);
      csList<iSector*> sectorList;
      int size = engine->GetSectors()->GetCount();
      for (int i = 0; i < size; i++)
      {
        sectorList.PushBack(engine->GetSectors()->Get(i));    
      }
      navStructBuilder->SetSectors(sectorList);
      navStruct = navStructBuilder->BuildHNavStruct();
      updateMeshes = true;
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
      updateMeshes = true;
    }
    else if (code == 'c') // Clear navstruct, positions and path
    {
      navStruct.Invalidate();
      path.Invalidate();
      originSet = false;
      destinationSet = false;
      clearMeshes = true;
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

  csScreenTargetResult st = csEngineTools::FindScreenTarget(screenPoint, 10000.0f, view->GetCamera());
  if (!st.mesh)
    return;

  csRef<iSectorList> sectorList = st.mesh->GetMovable()->GetSectors();
  if (sectorList->GetCount() != 1)
  {
    return;
  }
  
  if (kbd->GetKeyState(CSKEY_SHIFT))
  {
    origin = st.isect;
    originSector = sectorList->Get(0);
    updateOriginMeshes = true;
    if (destinationSet && navStruct)
    {
      path = navStruct->ShortestPath(origin, originSector, destination, destinationSector);
      if (path)
      {
        updatePathMeshes = true;
      }
    }
  }
  else
  {
    destination = st.isect;
    destinationSector = sectorList->Get(0);
    updateDestinationMeshes = true;
    if (originSet && navStruct)
    {
      path = navStruct->ShortestPath(origin, originSector, destination, destinationSector);
      if (path)
      {
        updatePathMeshes = true;
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

bool MainApp::OnInitialize (int argc, char* argv[]) 
{
  if (argc < 2)
  {
    mapLocation = "/lev/castle";
  }
  else
  {
    csString mapName(argv[1]);
    if (mapName == "terrain")
    {
      mapLocation = "/lev/terrainf";
    }
    else if (mapName == "castle")
    {
      mapLocation = "/lev/castle";
    }
    else  
    {
      return ReportError("Map not supported!");
    }
  }

  if (!csInitializer::RequestPlugins(object_reg,
    CS_REQUEST_VFS,
    CS_REQUEST_OPENGL3D,
    CS_REQUEST_ENGINE,
    CS_REQUEST_FONTSERVER,
    CS_REQUEST_IMAGELOADER,
    CS_REQUEST_LEVELLOADER,
    CS_REQUEST_REPORTER,
    CS_REQUEST_REPORTERLISTENER,
    CS_REQUEST_PLUGIN("crystalspace.collisiondetection.opcode", iCollideSystem),
    CS_REQUEST_PLUGIN("cel.hnavstructbuilder", iCelHNavStructBuilder),    
    CS_REQUEST_END)) 
  {
      return ReportError("Failed to initialize plugins!");
  }

  csBaseEventHandler::Initialize(object_reg);
  if (!RegisterQueue(object_reg, csevAllEvents(object_reg))) 
  {
    return ReportError("Failed to set up event handler!");
  }

  return true;
}

void MainApp::OnExit ()
{
}

bool MainApp::Application ()
{
  if (!OpenApplication(object_reg))
  {
    return ReportError("Error opening system!");
  }

  if (!SetupModules())
  {
    return false;
  }

  view.AttachNew(new csView(engine, g3d));
  iGraphics2D* g2d = g3d->GetDriver2D();
  view->SetRectangle(0, 0, g2d->GetWidth(), g2d->GetHeight());

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

  printer.AttachNew(new FramePrinter(object_reg));

  Run();

  return true;
}

bool MainApp::SetupModules ()
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

  cdsys = csQueryRegistry<iCollideSystem>(object_reg);
  if (!cdsys)
  {
    return ReportError ("Failed to locate CD system!");
  }

  vfs = csQueryRegistry<iVFS>(object_reg);
  if (!vfs)
  {
    return ReportError("Failed to locate VFS");
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
