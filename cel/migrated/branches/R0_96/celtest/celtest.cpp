/*
    Copyright (C) 2001 by Jorrit Tyberghein

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

#include "cssys/sysfunc.h"
#include "iutil/vfs.h"
#include "iutil/object.h"
#include "csutil/cscolor.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "celtest.h"
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
#include "iengine/statlght.h"
#include "iengine/texture.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/material.h"
#include "iengine/region.h"
#include "imesh/thing/polygon.h"
#include "imesh/thing/thing.h"
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
#include "cssys/csshlib.h"

#include "pl/pl.h"
#include "pl/propfact.h"
#include "pl/propclas.h"
#include "pl/entity.h"
#include "pl/persist.h"
#include "bl/bl.h"
#include "pf/test.h"
#include "pf/mesh.h"
#include "pf/meshsel.h"
#include "pf/inv.h"
#include "pf/chars.h"
#include "pf/move.h"
#include "pf/tooltip.h"
#include "pf/camera.h"
#include "pf/gravity.h"
#include "pf/timer.h"
#include "pf/region.h"
#include "pf/input.h"

CS_IMPLEMENT_APPLICATION

// Define MINIMAL for a very minimal celtest.
// Useful for debugging.
#define MINIMAL 1

//-----------------------------------------------------------------------------

// The global pointer to celtest
CelTest* celtest;

CelTest::CelTest ()
{
}

CelTest::~CelTest ()
{
  if (pl)
    pl->CleanCache ();
}

void CelTest::SetupFrame ()
{
  // We let the entity system do this so there is nothing here.
}

void CelTest::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (NULL);
}

bool CelTest::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdProcess)
  {
    celtest->SetupFrame ();
    return true;
  }
  else if (ev.Type == csevBroadcast && ev.Command.Code == cscmdFinalProcess)
  {
    celtest->FinishFrame ();
    return true;
  }
  else if (ev.Type == csevKeyDown)
  {
    if (ev.Key.Code == CSKEY_ESC)
    {
      csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
      if (q)
        q->GetEventOutlet()->Broadcast (cscmdQuit);
      return true;
    }
    else if (ev.Key.Code == 's')
    {
      printf ("Saving to '/this/savefile'\n"); fflush (stdout);
      csRef<iCelPersistance> cp (
      	CS_QUERY_REGISTRY (object_reg, iCelPersistance));
      if (!cp)
      {
	printf ("Couldn't find persistance context!\n");
	return true;
      }
      bool rc = cp->SaveEntity (game, "/this/savefile");
      printf ("  success %d\n", rc); fflush (stdout);
    }
    else if (ev.Key.Code == 'l')
    {
      csRef<iCelPersistance> cp (
      	CS_QUERY_REGISTRY (object_reg, iCelPersistance));
      if (!cp)
      {
	printf ("Couldn't find persistance context!\n");
	return true;
      }
      
      printf ("Loading from '/this/savefile\n"); fflush (stdout);
      if (game)
      {
	game = NULL;
	pl->CleanCache ();
      }
      csDebuggingGraph::Dump (NULL);
      csDebuggingGraph::Clear (NULL);

      printf ("  #sectors=%d\n", engine->GetSectors ()->GetCount ());
      printf ("  #meshes=%d\n", engine->GetMeshes ()->GetCount ());
      printf ("  #factories=%d\n", engine->GetMeshFactories ()->GetCount ());
      printf ("  #collections=%d\n", engine->GetCollections ()->GetCount ());
      printf ("  #campos=%d\n", engine->GetCameraPositions ()->GetCount ());
      printf ("  #textures=%d\n", engine->GetTextureList ()->GetCount ());
      printf ("  #materials=%d\n", engine->GetMaterialList ()->GetCount ());
      printf ("  #regions=%d\n", engine->GetRegions ()->GetCount ());

      LoadTextures ();
      game = cp->LoadEntity ("/this/savefile");

      csDebuggingGraph::Dump (NULL);

      // @@@
#if 1
      engine->Prepare ();
#endif
    }
    else if (ev.Key.Code == 'c')
    {
      if (game)
      {
	game = NULL;
	pl->CleanCache ();
      }
      csDebuggingGraph::Dump (NULL);
      printf ("  #sectors=%d\n", engine->GetSectors ()->GetCount ());
      printf ("  #meshes=%d\n", engine->GetMeshes ()->GetCount ());
      int i;
      for (i = 0 ; i < engine->GetMeshes ()->GetCount () ; i++)
      {
        iMeshWrapper* mesh = engine->GetMeshes ()->Get (i);
	printf ("  name=%s\n", mesh->QueryObject ()->GetName ());
      }
      printf ("  #factories=%d\n", engine->GetMeshFactories ()->GetCount ());
      printf ("  #collections=%d\n", engine->GetCollections ()->GetCount ());
      printf ("  #campos=%d\n", engine->GetCameraPositions ()->GetCount ());
      printf ("  #textures=%d\n", engine->GetTextureList ()->GetCount ());
      printf ("  #materials=%d\n", engine->GetMaterialList ()->GetCount ());
      printf ("  #regions=%d\n", engine->GetRegions ()->GetCount ());
      fflush (stdout);
    }
  }
  return false;
}

bool CelTest::CelTestEventHandler (iEvent& ev)
{
  return celtest->HandleEvent (ev);
}

csPtr<iCelEntity> CelTest::CreateBoxEntity (const char* name, const char* factName,
	iPcCamera* pccamera,
	float weight, float size,
	float max_indiv_weight, float max_weight,
	float max_indiv_size, float max_size,
	const csVector3& pos)
{
  iCelPropertyClass* pc;
  csRef<iPcMesh> pcmesh;
  csRef<iPcMeshSelect> pcmeshsel;
  csRef<iPcMovable> pcmovable;
  csRef<iPcMovableConstraint> pcmovableconst;
  csRef<iPcInventory> pcinv;
  csRef<iPcCharacteristics> pcchars;
  csRef<iPcTest> pctest;

  csRef<iCelEntity> entity_box (pl->CreateEntity ());
  entity_box->SetName (name);
  entity_box->SetBehaviour (bltest->CreateBehaviour (entity_box, "box"));

  pc = pl->CreatePropertyClass (entity_box, "pcmeshselect");
  if (!pc) return NULL;
  pcmeshsel = SCF_QUERY_INTERFACE (pc, iPcMeshSelect);
  pcmeshsel->SetCamera (pccamera);
  pcmeshsel->SetMouseButtons (CEL_MOUSE_BUTTON2);

  pc = pl->CreatePropertyClass (entity_box, "pctimer");
  if (!pc) return NULL;

  pc = pl->CreatePropertyClass (entity_box, "pcsolid");
  if (!pc) return NULL;
  pc = pl->CreatePropertyClass (entity_box, "pcgravity");
  if (!pc) return NULL;
  csRef<iPcGravity> pcgravity (SCF_QUERY_INTERFACE (pc, iPcGravity));
  pcgravity->SetWeight (weight);
  pcgravity->ApplyPermanentForce (csVector3 (0, -9.8, 0));
  pc = pl->CreatePropertyClass (entity_box, "pcmovable");
  if (!pc) return NULL;
  //pcmovable = SCF_QUERY_INTERFACE (pc, iPcMovable);
  //pc = pl->CreatePropertyClass (entity_box, "pcmovableconst_cd");
  //if (!pc) return NULL;
  //pcmovableconst = SCF_QUERY_INTERFACE (pc, iPcMovableConstraint);
  //pcmovable->AddConstraint (pcmovableconst);

  pc = pl->CreatePropertyClass (entity_box, "pctest");
  if (!pc) return NULL;
  pctest = SCF_QUERY_INTERFACE (pc, iPcTest);
  pctest->Print ("Hello world!");

  pc = pl->CreatePropertyClass (entity_box, "pcmesh");
  if (!pc) return NULL;
  pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);
  char buf[150];
  sprintf (buf, "/cel/data/%s", factName);
  pcmesh->SetMesh (factName, buf);
  pcmesh->MoveMesh (room, pos);

  pc = pl->CreatePropertyClass (entity_box, "pcinventory");
  if (!pc) return NULL;
  pcinv = SCF_QUERY_INTERFACE (pc, iPcInventory);
  pcinv->SetConstraints ("size", 0, max_indiv_size, max_size);
  pcinv->SetConstraints ("weight", 0, max_indiv_weight, max_weight);
  pcinv->SetStrictCharacteristics ("size", true);

  pc = pl->CreatePropertyClass (entity_box, "pccharacteristics");
  if (!pc) return NULL;
  pcchars = SCF_QUERY_INTERFACE (pc, iPcCharacteristics);
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
  csRef<iCelPropertyClass> pc;
  csRef<iPcMesh> pcmesh;
  csRef<iPcMovable> pcmovable;
  csRef<iPcMovableConstraint> pcmovableconst;
  csRef<iPcCharacteristics> pcchars;
  csRef<iPcGravity> pcgravity;

  csRef<iCelEntity> entity_dummy (pl->CreateEntity ());
  entity_dummy->SetName (name);
  iCelBehaviour* bh;
  if (python && blpython)
  {
    bh = blpython->CreateBehaviour (entity_dummy, "printer");
  }
  else
  {
    bh = bltest->CreateBehaviour (entity_dummy, "printer");
  }

  pc = pl->CreatePropertyClass (entity_dummy, "pctimer");
  if (!pc) return NULL;
  csRef<iPcTimer> pctimer (SCF_QUERY_INTERFACE (pc, iPcTimer));
  pctimer->WakeUp (1000, true);

  entity_dummy->SetBehaviour (bh);
  pc = pl->CreatePropertyClass (entity_dummy, "pccharacteristics");
  if (!pc) return NULL;
  pcchars = SCF_QUERY_INTERFACE (pc, iPcCharacteristics);
  pcchars->SetCharacteristic ("size", size);
  pcchars->SetCharacteristic ("weight", weight);

  pc = pl->CreatePropertyClass (entity_dummy, "pcsolid");
  if (!pc) return NULL;
  pc = pl->CreatePropertyClass (entity_dummy, "pcgravity");
  if (!pc) return NULL;
  pcgravity = SCF_QUERY_INTERFACE (pc, iPcGravity);
  pcgravity->SetWeight (weight);
  pcgravity->ApplyPermanentForce (csVector3 (0, -9.8, 0));
  pc = pl->CreatePropertyClass (entity_dummy, "pcmovable");
  if (!pc) return NULL;
  //pcmovable = SCF_QUERY_INTERFACE (pc, iPcMovable);
  //pc = pl->CreatePropertyClass (entity_dummy, "pcmovableconst_cd");
  //if (!pc) return NULL;
  //pcmovableconst = SCF_QUERY_INTERFACE (pc, iPcMovableConstraint);
  //pcmovable->AddConstraint (pcmovableconst);
  pc = pl->CreatePropertyClass (entity_dummy, "pcmesh");
  if (!pc) return NULL;
  pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);
  char buf[150];
  sprintf (buf, "/cel/data/%s", factName);
  pcmesh->SetMesh (factName, buf);
  pcmesh->MoveMesh (room, pos);

  pcgravity->ApplyForce (force, 1);

  return csPtr<iCelEntity> (entity_dummy);
}

csPtr<iCelEntity> CelTest::CreateActor (const char* name, const char* /*factname*/,
    const csVector3& /*pos*/)
{
  csRef<iCelPropertyClass> pc;
  csRef<iPcMesh> pcmesh;
  csRef<iPcCamera> pccamera;
  csRef<iPcMeshSelect> pcmeshsel;
  csRef<iPcMovable> pcmovable;
  csRef<iPcMovableConstraint> pcmovableconst;
  csRef<iPcGravity> pcgravity;

  // The Real Camera

  csRef<iCelEntity> entity_cam (pl->CreateEntity ());
  entity_cam->SetName (name);
  entity_cam->SetBehaviour (bltest->CreateBehaviour (entity_cam, "actor"));

  pc = pl->CreatePropertyClass (entity_cam, "pckeyinput");
  if (!pc) return NULL;
  csRef<iPcCommandInput> pcinp (SCF_QUERY_INTERFACE (pc, iPcCommandInput));
  if (!pcinp) return NULL;
  pcinp->Bind ("up", "forward");
  pcinp->Bind ("down", "backward");
  pcinp->Bind ("shift", "run");
  pcinp->Bind ("m", "cammode");

  pc = pl->CreatePropertyClass (entity_cam, "pccamera");
  if (!pc) return NULL;
  pccamera = SCF_QUERY_INTERFACE (pc, iPcCamera);

  pc = pl->CreatePropertyClass (entity_cam, "pcmesh");
  if (!pc) return NULL;

  pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);
  char buf[150];
  sprintf (buf, "/cel/data/large");
  pcmesh->SetMesh ("large", buf);
  pcmesh->MoveMesh (room, csVector3(0,0,0));

  pc = pl->CreatePropertyClass (entity_cam, "pcsolid");
  if (!pc) return NULL;
  pc = pl->CreatePropertyClass (entity_cam, "pcgravity");
  if (!pc) return NULL;
  pcgravity = SCF_QUERY_INTERFACE (pc, iPcGravity);
  pcgravity->SetWeight (.3);
  //pcgravity->ApplyPermanentForce (csVector3 (0, -9.8, 0));
  pc = pl->CreatePropertyClass (entity_cam, "pcmovable");
  if (!pc) return NULL;
  pcmovable = SCF_QUERY_INTERFACE (pc, iPcMovable);
  pc = pl->CreatePropertyClass (entity_cam, "pcmovableconst_cd");
  if (!pc) return NULL;
  pcmovableconst = SCF_QUERY_INTERFACE (pc, iPcMovableConstraint);
  pcmovable->AddConstraint (pcmovableconst);

  pc = pl->CreatePropertyClass (entity_cam, "pctooltip");
  if (!pc) return NULL;

  pc = pl->CreatePropertyClass (entity_cam, "pcmeshselect");
  if (!pc) return NULL;
  pcmeshsel = SCF_QUERY_INTERFACE (pc, iPcMeshSelect);
  pcmeshsel->SetCamera (pccamera);
  pcmeshsel->SetGlobalSelection (true);
  pcmeshsel->SetFollowMode (false);
  pcmeshsel->SetFollowAlwaysMode (true);
  pcmeshsel->SetDragMode (true);
  pcmeshsel->SetDragPlaneNormal (csVector3 (0, 1, 0), false);
  pcmeshsel->SetSendmoveEvent (true);
  pcmeshsel->SetMouseButtons (CEL_MOUSE_BUTTON1);

  return csPtr<iCelEntity> (entity_cam);
}

bool CelTest::CreateRoom ()
{
  csRef<iCelEntity> entity_room;
  csRef<iCelEntity> entity_dummy;
  iCelPropertyClass* pc;
  csRef<iPcInventory> pcinv_room;
  csRef<iPcRegion> pcregion;

  //===============================
  // Create the room entity.
  //===============================
  entity_room = pl->CreateEntity ();
  entity_room->SetName ("room");
  entity_room->SetBehaviour (bltest->CreateBehaviour (entity_room, "room"));

  pc = pl->CreatePropertyClass (entity_room, "pcregion");
  if (!pc) return false;
  pcregion = SCF_QUERY_INTERFACE (pc, iPcRegion);
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

  pc = pl->CreatePropertyClass (entity_room, "pcinventory");
  if (!pc) return false;
  pcinv_room = SCF_QUERY_INTERFACE (pc, iPcInventory);

  entity_dummy = CreateActor ("camera", "", csVector3(0,0,0));
  if (!entity_dummy) return false;
  csRef<iPcCamera> pccamera (CEL_QUERY_PROPCLASS
    (entity_dummy->GetPropertyClassList(), iPcCamera));
  if (!pccamera) return false;
  pccamera->SetRegion (pcregion);
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  //===============================
  // Engine init.
  //===============================
  // @@@ CHECK LATER WHY THIS IS NEEDED!!!
  engine->Prepare ();

  //===============================
  // Create the box entities.
  //===============================
#if !MINIMAL
  iCelEntity* entity_box;
  entity_box = CreateBoxEntity ("box", "box", pccamera, .9, 200,
  	1, 1000000, 60, 180, csVector3 (0, 0, 2));
  if (!entity_box) return false;
  if (!pcinv_room->AddEntity (entity_box)) return false;

  entity_box = CreateBoxEntity ("box_small", "smallbox", pccamera, .3, 50,
  	1, 1000000, 5, 48,
  	csVector3 (-4, 0, 0));
  if (!entity_box) return false;
  if (!pcinv_room->AddEntity (entity_box)) return false;
#endif

  //===============================
  // Create dummy entities.
  //===============================
  entity_dummy = CreateDummyEntity ("dummy1", "large",
  	.3, 8, csVector3 (-2, 0, 1), csVector3 (0, 9, 0), false);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

#if !MINIMAL
  entity_dummy = CreateDummyEntity ("dummy2", "small",
  	.4, 2, csVector3 (2, 0, -1), csVector3 (0, 9, 0), true);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  entity_dummy = CreateDummyEntity ("dummy3", "large",
  	.2, 11, csVector3 (1, 0, 3), csVector3 (0, 9, 0), false);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  entity_dummy = CreateDummyEntity ("dummy4", "medium",
  	.7, 5, csVector3 (0, 0, -1.5), csVector3 (0, 9, 0), false);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  entity_dummy = CreateDummyEntity ("dummy5", "small",
  	.1, 1, csVector3 (-1, 0, -2), csVector3 (0, 9, 0), true);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  entity_dummy = CreateDummyEntity ("dummy6", "medium",
  	.3, 4, csVector3 (2.5, 0, 1.5), csVector3 (0, 9, 0), false);
  if (!entity_dummy) return false;
  if (!pcinv_room->AddEntity (entity_dummy)) return false;
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
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"Error loading texture '%s'!", fileName);
    return false;
  }
  printf ("txt: %p %d\n", txt, txt->GetRefCount());
  pl->Cache (txt);
  iMaterialWrapper* mat = engine->GetMaterialList ()->FindByName (txtName);
  if (mat) pl->Cache (mat);
  return true;
}

bool CelTest::LoadPcFactory (const char* pcfactname)
{
  csRef<iPluginManager> plugin_mgr (
  	CS_QUERY_REGISTRY (object_reg, iPluginManager));
  csRef<iBase> pf (CS_LOAD_PLUGIN_ALWAYS(plugin_mgr, pcfactname));
  if (!pf)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	    "crystalspace.application.celtest",
	    "CEL '%s' plugin missing!", pcfactname);
    return false;
  }

  return true;
}

bool CelTest::Initialize (int argc, const char* const argv[])
{
  object_reg = csInitializer::CreateEnvironment (argc, argv);
  if (!object_reg) return false;

  csDebuggingGraph::SetupGraph (object_reg);

  // @@@ The code below is temporary until we have a general solution
  // in CS for having plugins outside the CS hierarchy (by an additional
  // .csinfo file for every plugin for example).
  iSCF::SCF->RegisterClass ("cel.physicallayer", "plimp");
  iSCF::SCF->RegisterClass ("cel.behaviourlayer.test", "bltest");
  iSCF::SCF->RegisterClass ("cel.behaviourlayer.python", "blpython");
  iSCF::SCF->RegisterClass ("cel.pcfactory.test", "pftest");
  iSCF::SCF->RegisterClass ("cel.pcfactory.mesh", "pfmesh");
  iSCF::SCF->RegisterClass ("cel.pcfactory.meshselect", "pfmesh");
  iSCF::SCF->RegisterClass ("cel.pcfactory.movable", "pfmove");
  iSCF::SCF->RegisterClass ("cel.pcfactory.solid", "pfmove");
  iSCF::SCF->RegisterClass ("cel.pcfactory.movableconst_cd", "pfmove");
  iSCF::SCF->RegisterClass ("cel.pcfactory.gravity", "pfmove");
  iSCF::SCF->RegisterClass ("cel.pcfactory.timer", "pftools");
  iSCF::SCF->RegisterClass ("cel.pcfactory.tooltip", "pftools");
  iSCF::SCF->RegisterClass ("cel.pcfactory.region", "pfengine");
  iSCF::SCF->RegisterClass ("cel.pcfactory.camera", "pfengine");
  iSCF::SCF->RegisterClass ("cel.pcfactory.inventory", "pfinv");
  iSCF::SCF->RegisterClass ("cel.pcfactory.pckeyinput", "pfinput");
  iSCF::SCF->RegisterClass ("cel.pcfactory.characteristics", "pfinv");
  iSCF::SCF->RegisterClass ("cel.persistance.classic", "cpersist");

  if (!csInitializer::RequestPlugins (object_reg,
  	CS_REQUEST_VFS,
	CS_REQUEST_SOFTWARE3D,
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
	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.rapid",
		iCollideSystem),
	CS_REQUEST_END))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
	"Can't initialize plugins!");
    return false;
  }

  if (!csInitializer::SetupEventHandler (object_reg, CelTestEventHandler))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
	"Can't initialize event handler!");
    return false;
  }

  // Check for commandline help.
  if (csCommandLineHelper::CheckHelp (object_reg))
  {
    csCommandLineHelper::Help (object_reg);
    return false;
  }

  // The virtual clock.
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  if (!vc)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
	"Can't find the virtual clock!");
    return false;
  }

  // Find the pointer to engine plugin
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (!engine)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
	"No iEngine plugin!");
    return false;
  }

  loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  if (!loader)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"No iLoader plugin!");
    return false;
  }

  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  if (!g3d)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"No iGraphics3D plugin!");
    return false;
  }

  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  if (!kbd)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"No iKeyboardDriver plugin!");
    return false;
  }

  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"CEL physical layer missing!");
    return false;
  }
  bltest = CS_QUERY_REGISTRY_TAG_INTERFACE (object_reg,
  	"iCelBlLayer.Test", iCelBlLayer);
  if (!bltest)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"CEL test behaviour layer missing!");
    return false;
  }
  pl->RegisterBehaviourLayer (bltest);
  csRef<iVFS> vfs (CS_QUERY_REGISTRY (object_reg, iVFS));
  CS_ASSERT (vfs != NULL);
#if defined(VFS_PKGDATADIR) && defined(VFS_TOPSRCDIR)
  vfs->Mount ("cel", VFS_PKGDATADIR"$/, "VFS_TOPSRCDIR"$/celtest$/");
#else // VFS_PKGDATADIR
  vfs->Mount ("cel", "$.$/");
  vfs->Mount ("cel", "$.$/celtest$/");
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

  // Open the main system. This will open all the previously loaded plug-ins.
  if (!csInitializer::OpenApplication (object_reg))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"Error opening system!");
    return false;
  }

  // XXX: This should be in a config file...
  if (!LoadPcFactory ("cel.pcfactory.test"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.movable"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.solid"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.movableconst_cd"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.gravity"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.region"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.camera"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.tooltip"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.timer"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.inventory"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.characteristics"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.mesh"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.meshselect"))
    return false;
  if (!LoadPcFactory ("cel.pcfactory.pckeyinput"))
    return false;

  if (!LoadTextures ()) return false;
  if (!CreateRoom ()) return false;

  return true;
}

void CelTest::Start ()
{
  csDefaultRunLoop (object_reg);
}

/*---------------------------------------------------------------------*
 * Main function
 *---------------------------------------------------------------------*/
int main (int argc, char* argv[])
{
#ifdef LIBDIR
  csAddLibraryPath(LIBDIR);
  csAddLibraryPath("../blpython/.libs/");
  csAddLibraryPath("../bltest/.libs/");
  csAddLibraryPath("../celtest/.libs/");
  csAddLibraryPath("../persist/classic/.libs/");
  csAddLibraryPath("../pf/common/.libs/");
  csAddLibraryPath("../pf/engine/.libs/");
  csAddLibraryPath("../pf/input/.libs/");
  csAddLibraryPath("../pf/inv/.libs/");
  csAddLibraryPath("../pf/mesh/.libs/");
  csAddLibraryPath("../pf/move/.libs/");
  csAddLibraryPath("../pf/test/.libs/");
  csAddLibraryPath("../pf/tools/.libs/");
  csAddLibraryPath("../plimp/.libs/");
#endif // LIBDIR
  
  celtest = new CelTest ();

  if (celtest->Initialize (argc, argv))
    celtest->Start ();

  iObjectRegistry* object_reg = celtest->object_reg;
  delete celtest;

  csDebuggingGraph::Dump (NULL);
  csInitializer::DestroyApplication (object_reg);
  return 0;
}