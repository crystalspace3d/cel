#include <crystalspace.h>

#include <celtool/initapp.h>
#include <propclass/zone.h>
#include <propclass/camera.h>
#include <propclass/mesh.h>
#include<propclass/steer.h>
#include <propclass/linmove.h>
#include <propclass/actormove.h>
#include <propclass/input.h>
#include <physicallayer/propclas.h>

#include "app.h"


/*
 * This is a test application which serves as a tutorial
 * for pcsteer. Main function to look at is LoadSteering().
 * Also check behave.cpp to learn how to call functions in
 * pcsteer.
 */

MainApp::MainApp ()
{
  SetApplicationName ("Steering Test");
  destinationSet = false;
  renderNavMesh = true;
  renderDestination = true;
  renderPath = true;
  updateNavmesh = false;
  updateArea = csBox3(csVector3(24.3f, 1.0f, 13.7f), csVector3(25.3f, 2.0f, 21.3f));
  navStructMeshes = 0;
  pathMeshes = 0;
  destinationMeshes = 0;
  clearMeshes = false;
  updateMeshes = false;
  updatePathMeshes = false;
  updateDestinationMeshes = false;
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

  /*iCelMapFile* entitiesfile = region->CreateMapFile ();
  entitiesfile->SetPath ("/cellib/lev");
  entitiesfile->SetFile ("walktut_entities");*/

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
  pccamera = celQueryPropertyClassEntity<iPcCamera> (player_entity);

  // Since we want to be able to see the navigation meshes and paths, we have to turn AutoDraw off
  // and Draw the camera manually in the Frame method, along with the navmeshes and in the right
  // order.
  pccamera->SetAutoDraw(false);

  // Get the zone manager from the level entity which should have been created
  // by now.
  csRef<iPcZoneManager> pczonemgr = celQueryPropertyClassEntity<iPcZoneManager> (level_entity);
  pccamera->SetZoneManager (pczonemgr, true, "main", "Camera");

  // Get the iPcMesh interface so we can load the right mesh
  // for our player.
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (player_entity);
  pcmesh->SetPath ("/cellib/objects");
  pcmesh->SetMesh ("test", "cally.cal3d");
  csRef<iMeshWrapper> mesh = pcmesh->GetMesh();
  if (!mesh)
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

/*
 * Here we will create the entity which will use
 * the steering property class
 *
 */

bool MainApp::LoadSteering ()
{

  /*
   * We load pcmove.steer
   */

  steering_entity = pl->CreateEntity ("steer", bl, "steering_behave",
				      "pcmove.linear",
				      "pcmove.actor.standard",
				      "pcmove.steer", 
              "pcmove.mover",
				      "pcobject.mesh",
				      "pcinput.standard",
				      CEL_PROPCLASS_END);

  if (!steering_entity)
    return ReportError ("Error creating steering entity!");
  
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (steering_entity);
  pcmesh->SetPath ("/cellib/objects");
  pcmesh->SetMesh ("test", "cally.cal3d");
  csRef<iMeshWrapper> mesh = pcmesh->GetMesh();
  if (!mesh)
    return ReportError ("Error loading model!");

  // Get Cally's dimensions for use by recast & detour  
  csRef<iObjectModel> objectModel = mesh->GetMeshObject()->GetObjectModel(); 
  float x = objectModel->GetObjectBoundingBox().MaxY() - objectModel->GetObjectBoundingBox().MinY();
  float y = objectModel->GetObjectBoundingBox().MaxY() - objectModel->GetObjectBoundingBox().MinY();
  float z = objectModel->GetObjectBoundingBox().MaxY() - objectModel->GetObjectBoundingBox().MinY();
  agentHeight = y;
  agentRadius = csQsqrt(csSquare(x) + csSquare(z)) * 0.5f;

  csRef<iPcZoneManager> pczonemgr = celQueryPropertyClassEntity<iPcZoneManager> (level_entity);

  if (pczonemgr->PointMesh ("steer", "main", "Camera"))
    return ReportError ("Can't find region or start position in region!");


  //Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (steering_entity);
  pclinmove->InitCD (
		     csVector3 (0.5f,0.8f,0.5f),
		     csVector3 (0.5f,0.4f,0.5f),
		     csVector3 (0,0,0));

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcactormove = celQueryPropertyClassEntity<iPcActorMove> (steering_entity);
  pcactormove->SetMovementSpeed (3.0f);
  pcactormove->SetRunningSpeed (5.0f);
  pcactormove->SetRotationSpeed (2.75f);
  
  

  // Get iPcCommandInput so we can do key bindings. The behaviour layer
  // will interprete the commands so the steerer can move.
  csRef<iPcCommandInput> pcinput = celQueryPropertyClassEntity<iPcCommandInput> (steering_entity);
  /*
   * We bind each key needed for the steering behaviours
   *
   */

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


void MainApp::Frame ()
{
  pccamera->Draw();

  // Render navigation structure
  if (navStructMeshes && renderNavMesh)
  {
    csArray<csSimpleRenderMesh*>::Iterator it = navStructMeshes->GetIterator();
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(*it.Next());
    }
  }

  // Render destination agent proxy
  if (destinationMeshes && renderDestination)
  {
    csArray<csSimpleRenderMesh*>::Iterator it = destinationMeshes->GetIterator();
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(*it.Next());
    }
  }

  // Render path
  if (bl->GetPath() && path != bl->GetPath())
  {
    updatePathMeshes = true;
  }
  path = bl->GetPath();
  if (pathMeshes && path && renderPath)
  {
    csArray<csSimpleRenderMesh*>::Iterator it = pathMeshes->GetIterator();
    while (it.HasNext())
    {
      g3d->DrawSimpleMesh(*it.Next());
    }
  }

  if (!navStruct)
    return;

  // Auto-update navigation mesh
  if (updateNavmesh)
  {
    navStruct->Update(updateArea, engine->FindSector("interior"));
    updateMeshes = true;
  }

  // If we just modifying the meshes in the OnKeyboard method, they may be deleted
  // while rendering a frame, causing a crash.
  if (clearMeshes || updateMeshes)
  {
    navStructMeshes = 0;
    if (updateMeshes)
    {
      navStructMeshes = navStruct->GetDebugMeshes();
      updateMeshes = false;
    }
  }
  if (clearMeshes || updatePathMeshes)
  {
    pathMeshes = 0;
    if (updatePathMeshes)
    {
      pathMeshes = path->GetDebugMeshes();
      updatePathMeshes = false;
    }
  }
  if (clearMeshes || updateDestinationMeshes)
  {
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
   else if (code == 'b') // Build navstruct
    {
      navStruct.Invalidate();
      path.Invalidate();
      destinationSet = false;
      bl->SetPath(0);
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
        params->SetMinRegionArea(10);
        // This parameter sets how far from the navmesh I can click and still get an approximated
        // path (for example, clicking on walls and ceilings). It also determines how far the final
        // node of a path can be from it's intended destination (Detour always returns a path, 
        // either to the destination or the closest possible point. We need to see if the path reached
        // the destination and was off by some approximation factor or didn't reach the destination at all).
        params->SetPolygonSearchBox(csVector3(2, 4, 2));
        navStructBuilder->SetNavMeshParams(params);
      }
      csRefArray<iSector> sectorList; 
      int size = engine->GetSectors()->GetCount();
      for (int i = 0; i < size; i++)
      {
        sectorList.Push(engine->GetSectors()->Get(i));    
      }
      navStructBuilder->SetSectors(&sectorList);
      navStruct = navStructBuilder->BuildHNavStruct();
      bl->SetNavStruct(navStruct);
      updateMeshes = true;

      csRef<iPcSteer> pcsteer = celQueryPropertyClassEntity<iPcSteer> (steering_entity);
      pcsteer->SetNavStruct(navStruct);
    }
    else if (code == 'c') // Clear navstruct, positions and path
    {
      navStruct.Invalidate();
      path.Invalidate();
      destinationSet = false;
      bl->SetPath(0);
      clearMeshes = true;
    }
    else if (code == '6') // Switch navmesh rendering
    {
      renderNavMesh = !renderNavMesh;
    }
    else if (code == '7') // Switch path rendering
    {
      renderPath = !renderPath;
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
      CS_REQUEST_PLUGIN("cel.hnavstructbuilder", iCelHNavStructBuilder),
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

  navStructBuilder = csQueryRegistry<iCelHNavStructBuilder>(object_reg);
  if (!navStructBuilder)
  {
    return ReportError("Failed to locate Navigation Structure Builder");
  }

  if (!LoadLevel ())
    return ReportError ("Error loading level!");
 if (!CreatePlayer ())
    return ReportError ("Couldn't create player!"); 
 if (!LoadSteering ())
    return ReportError ("Couldn't create steering entity!");
    
  printer.AttachNew (new FramePrinter (object_reg));

  Run ();

  return true;
}

void MainApp::OnExit ()
{
  printer.Invalidate ();
}
