/*
    Copyright (C) 2001-2005 by Jorrit Tyberghein

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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA.
*/

//#define CEL_USE_NEW_CAMERA

#include "cssysdef.h"
#include "celtest.h"
#include "csutil/sysfunc.h"
#include "iutil/vfs.h"
#include "iutil/object.h"
#include "iutil/cmdline.h"
#include "csutil/cscolor.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "csutil/event.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/csinput.h"
#include "iutil/virtclk.h"
#include "iutil/plugin.h"
#include "iengine/sector.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/campos.h"
#include "iengine/collectn.h"
#include "iengine/light.h"
#include "iengine/texture.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/material.h"
#include "iengine/region.h"
#include "imesh/thing.h"
#include "imesh/sprite3d.h"
#include "imesh/object.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/txtmgr.h"
#include "ivideo/texture.h"
#include "ivideo/material.h"
#include "ivideo/fontserv.h"
#include "igraphic/imageio.h"
#include "imap/loader.h"
#include "isndsys/ss_loader.h"
#include "isndsys/ss_renderer.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "ivaria/collider.h"
#include "csutil/cmdhelp.h"
#include "csutil/debug.h"
#include "csutil/csshlib.h"

#include "celtool/initapp.h"
#include "celtool/persisthelper.h"
#include "physicallayer/pl.h"
#include "physicallayer/propfact.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/bl.h"
#include "propclass/test.h"
#include "propclass/mesh.h"
#include "propclass/meshsel.h"
#include "propclass/inv.h"
#include "propclass/chars.h"
#include "propclass/move.h"
#include "propclass/tooltip.h"
#include "propclass/defcam.h"
#ifdef CEL_USE_NEW_CAMERA
#include "propclass/newcamera.h"
#endif
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "propclass/region.h"
#include "propclass/input.h"
#include "propclass/navgraph.h"
#include "propclass/linmove.h"
#include "propclass/actormove.h"
#include "propclass/quest.h"
#include "propclass/trigger.h"
#include "propclass/zone.h"
#include "propclass/sound.h"

#define PATHFIND_VERBOSE 0

//-----------------------------------------------------------------------------

CelTest::CelTest ()
{
  SetApplicationName ("celtest");
}

CelTest::~CelTest ()
{
}

void CelTest::OnExit ()
{
  if (pl) pl->CleanCache ();
  csDebuggingGraph::Dump (0);
}

void CelTest::ProcessFrame ()
{
  // We let the entity system do this so there is nothing here.
}

void CelTest::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (0);
}

bool CelTest::OnKeyboard (iEvent &ev)
{
  // We got a keyboard event.
  if (csKeyEventHelper::GetEventType (&ev) == csKeyEventTypeDown)
  {
    // The user pressed a key (as opposed to releasing it).
    utf32_char code = csKeyEventHelper::GetCookedCode (&ev);
    if (code == CSKEY_ESC)
    {
      // The user pressed escape. For now we will simply exit the
      // application. The proper way to quit a Crystal Space application
      // is by broadcasting a cscmdQuit event. That will cause the
      // main runloop to stop. To do that we get the event queue from
      // the object registry and then post the event.
      csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
      q->GetEventOutlet ()->Broadcast (csevQuit (object_reg));
    }
    else if (code == 's')
    {
      csRef<iCelPersistence> p = CS_QUERY_REGISTRY (object_reg,
      	iCelPersistence);
      celStandardLocalEntitySet set (pl);
      size_t i;
      for (i = 0 ; i < pl->GetEntityCount () ; i++)
        set.AddEntity (pl->GetEntityByIndex (i));
      if (!p->Save (&set, "/this/savefile"))
      {
        printf ("Error!\n");
	fflush (stdout);
      }
      else
      {
        printf ("Saved to /this/savefile!\n");
	fflush (stdout);
	game = 0;
	pl->RemoveEntities ();
      }
    }
    else if (code == 'l')
    {
      csRef<iCelPersistence> p = CS_QUERY_REGISTRY (object_reg,
      	iCelPersistence);
      celStandardLocalEntitySet set (pl);
      if (!p->Load (&set, "/this/savefile"))
      {
        printf ("Error!\n");
	fflush (stdout);
      }
      else
      {
        game = pl->FindEntity ("room");
      }
    }
  }
  return false;
}

csPtr<iCelEntity> CelTest::CreateActor (const char* name,
	const char* /*factname*/, const csVector3& /*pos*/)
{
  // The Real Camera
  csRef<iCelEntity> entity_cam = pl->CreateEntity (name, bltest, "actor",
  	"pccommandinput",
#ifdef CEL_USE_NEW_CAMERA
	"pcnewcamera",
#else
	"pcdefaultcamera",
#endif
	"pcactormove",
	"pcmesh",
	"pcmeshselect",
	"pclinearmovement",
	"pctooltip",
	"pcinventory",
	"pcsoundlistener",
	CEL_PROPCLASS_END);
  if (!entity_cam) return 0;

  csRef<iPcCommandInput> pcinp = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcCommandInput);
  pcinp->Bind ("up", "forward");
  pcinp->Bind ("down", "backward");
  pcinp->Bind ("shift", "run");
  pcinp->Bind ("m", "cammode");
  pcinp->Bind ("left", "rotateleft");
  pcinp->Bind ("right", "rotateright");
  pcinp->Bind ("a", "strafeleft");
  pcinp->Bind ("d", "straferight");
  pcinp->Bind ("space", "jump");
  pcinp->Bind ("x", "center");
  pcinp->Bind ("pgup", "lookup");
  pcinp->Bind ("pgdn", "lookdown");

#ifdef CEL_USE_NEW_CAMERA
  csRef<iPcNewCamera> newcamera = CEL_QUERY_PROPCLASS_ENT (
	entity_cam, iPcNewCamera);
  //size_t first_idx = 
  //  newcamera->AttachCameraMode(iPcNewCamera::CCM_FIRST_PERSON);
  size_t third_idx =
    newcamera->AttachCameraMode(iPcNewCamera::CCM_THIRD_PERSON);
  //newcamera->SetCurrentCameraMode(third_idx); 
  newcamera->SetPositionOffset(csVector3(0,2,0));
#else
  csRef<iPcDefaultCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (
  	entity_cam, iPcDefaultCamera);
  pccamera->SetMode (iPcDefaultCamera::firstperson);
  pccamera->SetSpringParameters (10.0f, 0.1f, 0.01f);
  pccamera->SetMode (iPcDefaultCamera::thirdperson);
  pccamera->SetSpringParameters (3.5f, 0.25f, 0.01f);
  pccamera->SetMode (iPcDefaultCamera::m64_thirdperson);
  pccamera->SetSpringParameters (3.5f, 0.25f, 0.01f);
  pccamera->SetMinMaxCameraDistance (2.0f, 6.0f);
  pccamera->SetTurnSpeed (1.0f);
  pccamera->SetMode (iPcDefaultCamera::lara_thirdperson);
  pccamera->SetSpringParameters (3.5f, 0.25f, 0.01f);
  pccamera->SetMinMaxCameraDistance (2.0f, 6.0f);
  pccamera->SetTurnSpeed (1.0f);
  pccamera->SetSwingCoef (0.7f);
  pccamera->SetMode (iPcDefaultCamera::freelook);
  pccamera->SetSpringParameters (3.5f, 0.25f, 0.01f);
  pccamera->SetMinMaxCameraDistance (2.0f, 16.0f);
  pccamera->SetFirstPersonOffset (csVector3 (0, 1, 0));
  pccamera->SetThirdPersonOffset (csVector3 (0, 1, 3));
  pccamera->SetModeName ("thirdperson");
#endif

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcactormove = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcActorMove);
  pcactormove->SetMovementSpeed (2.0f);
  pcactormove->SetRunningSpeed (5.0f);
  pcactormove->SetRotationSpeed (1.75f);
  pcactormove->SetJumpingVelocity (6.31f);

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcMesh);
  bool hascal3d = true;
  pcmesh->SetPath ("/cel/data");
  hascal3d = pcmesh->SetMesh ("test", "cally.cal3d");

  csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (entity_cam,
    iPcLinearMovement);
  if (hascal3d)
  {
    pclinmove->InitCD (
      csVector3 (0.5f, 0.8f,  0.5f),
      csVector3 (0.5f, 0.4f,  0.5f),
      csVector3 (0.0f, 0.01f, 0.0f));
  }
  else
  {
    pclinmove->InitCD (
      csVector3 (0.5f,  0.8f, 0.5f),
      csVector3 (0.5f,  0.4f, 0.5f),
      csVector3 (0.0f, -0.4f, 0.0f));
  }

  return csPtr<iCelEntity> (entity_cam);
}

bool CelTest::CreateRoom ()
{
  csRef<iCelEntity> entity_room;
  csRef<iCelEntity> entity_dummy;

  //===============================
  // Create the room entity.
  //===============================
  entity_room = pl->CreateEntity ("room", bltest, "room",
  	"pczonemanager",
	"pcinventory",
  	CEL_PROPCLASS_END);

  //===============================
  // Engine init.
  //===============================
  engine->Prepare ();

  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
  csString path, file;
  path = cmdline->GetName (0);
  if (!path.IsEmpty ())
  {
    file = cmdline->GetName (1);
    if (file.IsEmpty ()) file = "level.xml";
  }
  else
  {
    path = "/cellib/lev";
    file = "basic_level.xml";
  }

  csRef<iVFS> vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  csStringArray paths;
  paths.Push ("/cellib/lev/");
  if (!vfs->ChDirAuto (path, &paths, 0, file))
    return ReportError ("Bad file path '%s' at '%s'!", file.GetData (),
    	path.GetData ());

  csRef<iPcZoneManager> pczonemgr = CEL_QUERY_PROPCLASS_ENT (entity_room,
  	iPcZoneManager);
  if (!pczonemgr->Load (0, file))
    return ReportError ("Error loading level '%s' at '%s'!", file.GetData (),
    	path.GetData ());

  scfString regionname, startname;
  pczonemgr->GetLastStartLocation (&regionname, &startname);

  entity_dummy = CreateActor ("camera", "", csVector3 (0,0,0));
  if (!entity_dummy) return false;
  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (entity_dummy, iPcCamera);
  if (!pccamera) return false;
  pccamera->SetZoneManager (pczonemgr, true, regionname, startname);
  if (pczonemgr->PointMesh ("camera", regionname, startname)
  	!= CEL_ZONEERROR_OK)
    return ReportError ("Error finding start position!");

  csRef<iPcInventory> pcinv_room = CEL_QUERY_PROPCLASS_ENT (entity_room,
  	iPcInventory);
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  game = entity_room;

  return true;
}

bool CelTest::OnInitialize (int argc, char* argv[])
{
  csDebuggingGraph::SetupGraph (object_reg);

  if (!csInitializer::SetupConfigManager (object_reg,
  	"/celconfig/celtest.cfg"))
  {
    return ReportError ("Can't setup config file!");
  }


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
	CS_REQUEST_PLUGIN ("cel.behaviourlayer.test:iCelBlLayer.Test",
		iCelBlLayer),
	CS_REQUEST_PLUGIN ("cel.persistence.xml", iCelPersistence),
	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.opcode",
		iCollideSystem),
	CS_REQUEST_PLUGIN ("crystalspace.sndsys.element.loader", iSndSysLoader),
	CS_REQUEST_PLUGIN ("crystalspace.sndsys.renderer.software",
		iSndSysRenderer),
	CS_REQUEST_END))
  {
    return ReportError ("Can't initialize plugins!");
  }

  csBaseEventHandler::Initialize (object_reg);

  // Now we need to setup an event handler for our application.
  // Crystal Space is fully event-driven. Everything (except for this
  // initialization) happens in an event.
  if (!RegisterQueue (object_reg, csevAllEvents (object_reg)))
    return ReportError ("Can't setup event handler!");
  return true;
}

bool CelTest::Application ()
{
  // Open the main system. This will open all the previously loaded plug-ins.
  // i.e. all windows will be opened.
  if (!OpenApplication (object_reg))
    return ReportError ("Error opening system!");

  // The virtual clock.
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  if (!vc) return ReportError ("Can't find the virtual clock!");

  // Find the pointer to engine plugin
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (!engine) return ReportError ("No iEngine plugin!");

  loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  if (!loader) return ReportError ("No iLoader plugin!");

  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  if (!g3d) return ReportError ("No iGraphics3D plugin!");

  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  if (!kbd) return ReportError ("No iKeyboardDriver plugin!");

  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl) return ReportError ("CEL physical layer missing!");

  bltest = CS_QUERY_REGISTRY_TAG_INTERFACE (object_reg,
  	"iCelBlLayer.Test", iCelBlLayer);
  if (!bltest) return ReportError ("CEL test behaviour layer missing!");
  pl->RegisterBehaviourLayer (bltest);

  csRef<iVFS> vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
#if defined(VFS_PKGDATADIR) && defined(VFS_TOPSRCDIR)
  vfs->Mount ("cel", VFS_PKGDATADIR"$/, "VFS_TOPSRCDIR"$/");
#else // VFS_PKGDATADIR
  vfs->Mount ("cel", "$.$/");
#endif // VFS_PKGDATADIR

  // XXX: This should be in a config file...
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.test"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.linmove"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.actormove"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.solid"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.colldet"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.region"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.zonemanager"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.defaultcamera"))
    return false;
#ifdef CEL_USE_NEW_CAMERA
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.newcamera"))
    return false;
#endif
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.tooltip"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.timer"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.inventory"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.characteristics"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.mesh"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.light"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.portal"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.meshselect"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.pccommandinput"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.quest"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.properties"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.trigger"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.billboard"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.soundlistener"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.soundsource"))
    return false;

  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.graph"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.link"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.node"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.navgraphrules"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.navgraphrulesfps"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.awswindow"))
    return false;

  if (!CreateRoom ()) return false;

  // This calls the default runloop. This will basically just keep
  // broadcasting process events to keep the game going.
  Run ();

  return true;
}

#if 0
  // Create NavGraph Test Entities
  //==============================
  // Create Graph entity
  csRef<iCelEntity> graph = pl->CreateEntity ("navgraph1", 0, 0,
  	"pcgraph",
  	CEL_PROPCLASS_END);
  if (!graph) return 0;
  csRef<iPcNavGraph> pcgraph = CEL_QUERY_PROPCLASS_ENT (graph, iPcNavGraph);

  /* TODO Test Loading Nodes from a file
  // Tie this graph to the current region object
  pcgraph->SetRegion( pcregion );
  // Load all the nodes from this region
  pcgraph->LoadNodesFromRegion( pcregion->GetRegionName() );
  */

  // Create FPS rules for graph navigation
  csRef<iCelEntity> graphrulesfps = pl->CreateEntity ("graphrulesfps", 0, 0,
  	"pcgraphrulesfps",
  	CEL_PROPCLASS_END);
  if (!graphrulesfps) return 0;
  csRef<iPcNavGraphRules> navrules = CEL_QUERY_PROPCLASS_ENT (graphrulesfps,
  	iPcNavGraphRules);
  pcgraph->SetRules (navrules);

  // Create Node entities
  int i, j;
  for (i = -2; i<=2;i++)
  {
    for (j = -2; j<=2;j++)
    {
      csRef<iCelPropertyClass> pc = pl->CreatePropertyClass (graph, "pcnode");
      csRef<iPcNavNode> pcnode = SCF_QUERY_INTERFACE (pc, iPcNavNode);
      pcnode->SetPos (csVector3 (i,0,j));
      pcgraph->AddNode (pcnode); // Add the Node PC to the graph
    }
  }

  // Build Graph
  //============
  iCamera* camera = pccamera->GetCamera ();
  iSector* sector = camera->GetSector();

	  // use this entity for testing the nav graph
  iCelEntity* defaultent = entity_box;

  pcgraph->BuildNodeGraph (sector, defaultent);
#if PATHFIND_VERBOSE
  pcgraph->Dump (); // show all debug info
#endif

  // Test graph
  //===========

  // Test FindNearestNode()
  csVector3 v (-2, 0, 0);
  int inearest = pcgraph->FindNearestNode (&v, sector, defaultent);
  // test FindNearestNode method
#if PATHFIND_VERBOSE
  ReportInfo ("Nearest Node: %d", inearest );
#endif

  // Test FindShortestPath()
  // allocate up to 50 points on path
  int* ipath = (int*) malloc (50 * sizeof( int ));
  for (i = 0; i < 100; i++)
  {
    int itestnode = (int)(((float)rand()
    	* (float) pcgraph->GetNodeCount()) / (float)RAND_MAX);
    int ipathlength = 0;

    // Test FindNearestNode method.
    ipathlength = pcgraph->FindShortestPath (inearest, itestnode, ipath);
#if PATHFIND_VERBOSE
    ReportInfo ("Find Shortest path: from %d to %d: %d steps",
	inearest, itestnode, ipathlength-1 );
#endif

    /* 
      // Print out full path
      for ( int j=0; j < ipathlength;j++)
      {
          csReport(object_reg, CS_REPORTER_SEVERITY_NOTIFY,
	    "crystalspace.application.celtest", "..... %d", ipath[j]);
      }
     */
  }
  free (ipath);

#if PATHFIND_VERBOSE
  // Test FindNearestNode method.
  ReportInfo ("Final Graph Stats: nodes: %d links: %d",
	pcgraph->GetNodeCount(), pcgraph->GetLinkCount());
#endif

  //===============================
#endif

