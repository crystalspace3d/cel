/*
    Copyright (C) 2001-2003 by Jorrit Tyberghein

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
#include "cssysdef.h"

#include "csutil/sysfunc.h"
#include "iutil/vfs.h"
#include "iutil/object.h"
#include "csutil/cscolor.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "celtest.h"
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
#include "imap/parser.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "ivaria/collider.h"
#include "csutil/cmdhelp.h"
#include "csutil/debug.h"
#include "csutil/csshlib.h"

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
#include "propclass/camera.h"
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "propclass/region.h"
#include "propclass/input.h"
#include "propclass/navgraph.h"
#include "propclass/linmove.h"
#include "propclass/actormove.h"

// Define MINIMAL for a very minimal celtest.
// Useful for debugging.
#define MINIMAL 0

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
      q->GetEventOutlet ()->Broadcast (cscmdQuit);
    }
  }
  return false;
}

csPtr<iCelEntity> CelTest::CreateBoxEntity (const char* name,
	const char* factName,
	iPcCamera* pccamera,
	float weight, float size,
	float max_indiv_weight, float max_weight,
	float max_indiv_size, float max_size,
	const csVector3& pos)
{
  csRef<iCelEntity> entity_box = pl->CreateEntity (name, bltest, "box",
  	"pcmesh",
  	"pcmeshselect",
	"pctimer",
	"pcsolid",
	"pcgravity",
	"pcmovable",
	"pcmovableconst_cd",
	"pcinventory",
	"pccharacteristics",
	"pctest",
  	(void*)0);
  if (!entity_box) return 0;

  csRef<iPcMeshSelect> pcmeshsel = CEL_QUERY_PROPCLASS_ENT (entity_box,
  	iPcMeshSelect);
  pcmeshsel->SetCamera (pccamera);
  pcmeshsel->SetMouseButtons (CEL_MOUSE_BUTTON2);

  csRef<iPcGravity> pcgravity = CEL_QUERY_PROPCLASS_ENT (entity_box,
  	iPcGravity);
  pcgravity->SetWeight (weight);
  pcgravity->ApplyPermanentForce (csVector3 (0, -9.8f, 0));

  csRef<iPcMovable> pcmovable = CEL_QUERY_PROPCLASS_ENT (entity_box,
  	iPcMovable);
  csRef<iPcMovableConstraint> pcmovableconst = CEL_QUERY_PROPCLASS_ENT (
  	entity_box, iPcMovableConstraint);
  pcmovable->AddConstraint (pcmovableconst);

  csRef<iPcTest> pctest = CEL_QUERY_PROPCLASS_ENT (entity_box, iPcTest);
  pctest->Print ("Hello world!");

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_box, iPcMesh);
  char buf[150];
  sprintf (buf, "/cel/data/%s", factName);
  pcmesh->SetMesh (factName, buf);
  pcmesh->MoveMesh (room, pos);

  csRef<iPcInventory> pcinv = CEL_QUERY_PROPCLASS_ENT (entity_box,
  	iPcInventory);
  pcinv->SetConstraints ("size", 0, max_indiv_size, max_size);
  pcinv->SetConstraints ("weight", 0, max_indiv_weight, max_weight);
  pcinv->SetStrictCharacteristics ("size", true);

  csRef<iPcCharacteristics> pcchars = CEL_QUERY_PROPCLASS_ENT (entity_box,
  	iPcCharacteristics);
  pcchars->SetCharacteristic ("size", size);
  pcchars->SetCharacteristic ("weight", weight);
  pcchars->SetInheritedCharacteristic ("size", 0, 0);
  pcchars->SetInheritedCharacteristic ("weight", .5, 0);

  return csPtr<iCelEntity> (entity_box);
}

csPtr<iCelEntity> CelTest::CreateDummyEntity (const char* name,
	const char* factName,
	float weight, float size, const csVector3& pos,
	const csVector3& force, bool python)
{
  iCelBlLayer* bl;
  if (python && blpython)
    bl = blpython;
  else
    bl = bltest;
  csRef<iCelEntity> entity_dummy = pl->CreateEntity (name, bl, "printer",
  	"pctimer",
	"pccharacteristics",
	"pcsolid",
	"pcgravity",
	"pcmovable",
	"pcmovableconst_cd",
	"pcmesh",
  	(void*)0);

  csRef<iPcTimer> pctimer = CEL_QUERY_PROPCLASS_ENT (entity_dummy, iPcTimer);
  pctimer->WakeUp (1000, true);

  csRef<iPcCharacteristics> pcchars = CEL_QUERY_PROPCLASS_ENT (entity_dummy,
  	iPcCharacteristics);
  pcchars->SetCharacteristic ("size", size);
  pcchars->SetCharacteristic ("weight", weight);

  csRef<iPcGravity> pcgravity = CEL_QUERY_PROPCLASS_ENT (entity_dummy,
  	iPcGravity);
  pcgravity->SetWeight (weight);
  pcgravity->ApplyPermanentForce (csVector3 (0, -9.8f, 0));

  csRef<iPcMovable> pcmovable = CEL_QUERY_PROPCLASS_ENT (entity_dummy,
  	iPcMovable);
  csRef<iPcMovableConstraint> pcmovableconst = CEL_QUERY_PROPCLASS_ENT (
  	entity_dummy, iPcMovableConstraint);
  pcmovable->AddConstraint (pcmovableconst);

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_dummy, iPcMesh);
  char buf[150];
  sprintf (buf, "/cel/data/%s", factName);
  pcmesh->SetMesh (factName, buf);
  pcmesh->MoveMesh (room, pos);

  pcgravity->ApplyForce (force, 1);

  return csPtr<iCelEntity> (entity_dummy);
}

csPtr<iCelEntity> CelTest::CreateActor (const char* name,
	const char* /*factname*/, const csVector3& /*pos*/)
{
  // The Real Camera
  csRef<iCelEntity> entity_cam = pl->CreateEntity (name, bltest, "actor",
  	"pckeyinput",
	"pccamera",
	"pcactormove",
	"pcmesh",
	"pcmeshselect",
	"pclinearmovement",
	"pctooltip",
	(void*)0);
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

  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcCamera);
  pccamera->SetMode (iPcCamera::firstperson);
  pccamera->SetSpringParameters (10.0f, 0.1f, 0.01f);
  pccamera->SetMode (iPcCamera::thirdperson);
  pccamera->SetSpringParameters (3.5f, 0.25f, 0.01f);
  pccamera->SetMode (iPcCamera::m64_thirdperson);
  pccamera->SetSpringParameters (3.5f, 0.25f, 0.01f);
  pccamera->SetMinMaxCameraDistance (2.0f, 6.0f);
  pccamera->SetTurnSpeed (1.0f);
  pccamera->SetMode (iPcCamera::lara_thirdperson);
  pccamera->SetSpringParameters (3.5f, 0.25f, 0.01f);
  pccamera->SetMinMaxCameraDistance (2.0f, 6.0f);
  pccamera->SetTurnSpeed (1.0f);
  pccamera->SetSwingCoef (0.7f);
  pccamera->SetMode (iPcCamera::freelook);
  pccamera->SetSpringParameters (3.5f, 0.25f, 0.01f);
  pccamera->SetMinMaxCameraDistance (2.0f, 16.0f);
  pccamera->SetFirstPersonOffset (csVector3 (0, 1, 0));
  pccamera->SetThirdPersonOffset (csVector3 (0, 1, 3));
  pccamera->SetModeName ("thirdperson");

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcactormove = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcActorMove);
  pcactormove->SetMovementSpeed (2.0f);
  pcactormove->SetRunningSpeed (5.0f);
  pcactormove->SetRotationSpeed (1.75f);
  pcactormove->SetJumpingVelocity (6.31f);

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcMesh);
  char buf[150];
  sprintf (buf, "/cel/data/large");
  pcmesh->SetMesh ("large", buf);
  pcmesh->MoveMesh (room, csVector3(0,0,0));

  csRef<iPcMeshSelect> pcmeshsel = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcMeshSelect);
  pcmeshsel->SetCamera (pccamera);
  pcmeshsel->SetGlobalSelection (true);
  pcmeshsel->SetFollowMode (false);
  pcmeshsel->SetFollowAlwaysMode (true);
  pcmeshsel->SetDragMode (true);
  pcmeshsel->SetDragPlaneNormal (csVector3 (0, 1, 0), false);
  pcmeshsel->SetSendmoveEvent (true);
  pcmeshsel->SetMouseButtons (CEL_MOUSE_BUTTON1);

  csRef<iPcLinearMovement> pclinmove = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcLinearMovement);
  pclinmove->InitCD (
  	csVector3 (0.5, 0.8, 0.5),
  	csVector3 (0.5, 0.4, 0.5),
  	csVector3 (0, -0.4, 0));

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
  	"pcregion",
	"pcinventory",
  	(void*)0);

  //===============================
  // Engine init.
  //===============================
  engine->Prepare ();

  csRef<iPcRegion> pcregion = CEL_QUERY_PROPCLASS_ENT (entity_room, iPcRegion);
#if MINIMAL
  pcregion->SetWorldFile ("/cel/data", "world");
  pcregion->SetRegionName ("minimal");
#else
  pcregion->SetWorldFile ("/lev/partsys", "world");
  pcregion->SetRegionName ("partsys");
#endif
  if (!pcregion->Load ())
    return false;
  room = pcregion->GetStartSector ();

  csRef<iPcInventory> pcinv_room = CEL_QUERY_PROPCLASS_ENT (entity_room,
  	iPcInventory);

  entity_dummy = CreateActor ("camera", "", csVector3(0,0,0));
  if (!entity_dummy) return false;
  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (entity_dummy, iPcCamera);
  if (!pccamera) return false;
  pccamera->SetRegion (pcregion);
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  //===============================
  // Create the box entities.
  //===============================
#if !MINIMAL
  csRef<iCelEntity> entity_box;
  entity_box = CreateBoxEntity ("box", "box", pccamera, .9f, 200,
  	1, 1000000, 60, 180, csVector3 (0, 0, 2));
  if (!entity_box) return false;
  if (!pcinv_room->AddEntity (entity_box)) return false;

  entity_box = CreateBoxEntity ("box_small", "smallbox", pccamera, .3f, 50,
  	1, 1000000, 5, 48,
  	csVector3 (-4, 0, 0));
  if (!entity_box) return false;
  if (!pcinv_room->AddEntity (entity_box)) return false;
#endif

  //===============================
  // Create dummy entities.
  //===============================
  entity_dummy = CreateDummyEntity ("dummy1", "large",
  	.3f, 8, csVector3 (-2, 0, 1), csVector3 (0, 9, 0), false);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

#if !MINIMAL
  entity_dummy = CreateDummyEntity ("dummy2", "small",
  	.4f, 2, csVector3 (2, 0, -1), csVector3 (0, 9, 0), true);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  entity_dummy = CreateDummyEntity ("dummy3", "large",
  	.2f, 11, csVector3 (1, 0, 3), csVector3 (0, 9, 0), false);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  entity_dummy = CreateDummyEntity ("dummy4", "medium",
  	.7f, 5, csVector3 (0, 0, -1.5), csVector3 (0, 9, 0), false);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  entity_dummy = CreateDummyEntity ("dummy5", "small",
  	.1f, 1, csVector3 (-1, 0, -2), csVector3 (0, 9, 0), true);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  entity_dummy = CreateDummyEntity ("dummy6", "medium",
  	.3f, 4, csVector3 (2.5, 0, 1.5), csVector3 (0, 9, 0), false);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  // Create NavGraph Test Entities
  //==============================
  // Create Graph entity
  csRef<iCelEntity> graph = pl->CreateEntity ("navgraph1", 0, 0,
  	"pcgraph",
  	(void*)0);
  if (!graph) return 0;
  csRef<iPcNavGraph> pcgraph = CEL_QUERY_PROPCLASS_ENT (graph, iPcNavGraph);

  /* TODO Test Loading Nodes from a file
  pcgraph->SetRegion( pcregion ); // Tie this graph to the current region object
  pcgraph->LoadNodesFromRegion( pcregion->GetRegionName() ); // Load all the nodes from this region
  */

  // Create FPS rules for graph navigation
  csRef<iCelEntity> graphrulesfps = pl->CreateEntity ("graphrulesfps", 0, 0,
  	"pcgraphrulesfps",
  	(void*)0);
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
  pcgraph->Dump (); // show all debug info

  // Test graph
  //===========

  // Test FindNearestNode()
  csVector3 v (-2, 0, 0);
  int inearest = pcgraph->FindNearestNode (&v, sector, defaultent);
  // test FindNearestNode method
  ReportInfo ("Nearest Node: %d", inearest );

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
    ReportInfo ("Find Shortest path: from %d to %d: %d steps",
	inearest, itestnode, ipathlength-1 );

    /* 
      // Print out full path
      for ( int j=0; j < ipathlength;j++)
      {
          csReport( object_reg, CS_REPORTER_SEVERITY_NOTIFY, "crystalspace.application.celtest",
              "..... %d", ipath[j]);
      }
     */
  }
  free (ipath);

  // Test FindNearestNode method.
  ReportInfo ("Final Graph Stats: nodes: %d links: %d",
	pcgraph->GetNodeCount(), pcgraph->GetLinkCount());
#endif

  //===============================
  game = entity_room;

  return true;
}

bool CelTest::LoadTextures ()
{
#if MINIMAL
  if (!LoadTexture ("stone", "/lib/std/stone4.gif")) return false;
#else
  if (!LoadTexture ("stone", "/lib/std/stone4.gif")) return false;
  if (!LoadTexture ("spark", "/lib/std/spark.png")) return false;
  if (!LoadTexture ("wood", "/lib/stdtex/andrew_wood.jpg")) return false;
  if (!LoadTexture ("marble", "/lib/stdtex/marble_08_ao___128.jpg")) return false;
#endif
  return true;
}

bool CelTest::LoadTexture (const char* txtName, const char* fileName)
{
  iTextureWrapper* txt = loader->LoadTexture (txtName, fileName);
  if (!txt)
    return ReportError ("Error loading texture '%s'!", fileName);
  printf ("txt: %p %d\n", txt, txt->GetRefCount());
  pl->Cache (txt);
  iMaterialWrapper* mat = engine->GetMaterialList ()->FindByName (txtName);
  if (mat) pl->Cache (mat);
  return true;
}

bool CelTest::OnInitialize (int argc, char* argv[])
{
  csDebuggingGraph::SetupGraph (object_reg);

  if (!csInitializer::RequestPlugins (object_reg,
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
	CS_REQUEST_PLUGIN ("cel.behaviourlayer.python:iCelBlLayer.Python",
		iCelBlLayer),
	CS_REQUEST_PLUGIN ("cel.persistance.classic", iCelPersistance),
	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.opcode",
		iCollideSystem),
	CS_REQUEST_END))
  {
    return ReportError ("Can't initialize plugins!");
  }

  // Now we need to setup an event handler for our application.
  // Crystal Space is fully event-driven. Everything (except for this
  // initialization) happens in an event.
  if (!RegisterQueue (object_reg))
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
  
  blpython = CS_QUERY_REGISTRY_TAG_INTERFACE (object_reg,
  	"iCelBlLayer.Python", iCelBlLayer);
  if (!blpython)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
    	"crystalspace.application.celtest",
    	"CEL python behaviour layer missing.");
  }
  else
    pl->RegisterBehaviourLayer (blpython);

  // XXX: This should be in a config file...
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.test"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.movable"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.linmove"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.actormove"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.solid"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.colldet"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.movableconst_cd"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.gravity"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.region"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.camera"))
    return false;
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
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.meshselect"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.pckeyinput"))
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

  if (!LoadTextures ()) return false;
  if (!CreateRoom ()) return false;

  // This calls the default runloop. This will basically just keep
  // broadcasting process events to keep the game going.
  Run ();

  return true;
}

