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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "cssys/sysfunc.h"
#include "iutil/vfs.h"
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
#include "csutil/cmdhelp.h"

#include "pl/pl.h"
#include "pl/propfact.h"
#include "pl/propclas.h"
#include "pl/entity.h"
#include "bl/bl.h"
#include "pf/test.h"
#include "pf/mesh.h"
#include "pf/meshsel.h"
#include "pf/inv.h"
#include "pf/chars.h"

CS_IMPLEMENT_APPLICATION

//-----------------------------------------------------------------------------

// The global pointer to celtest
CelTest* celtest;

CelTest::CelTest ()
{
  engine = NULL;
  loader = NULL;
  g3d = NULL;
  kbd = NULL;
  vc = NULL;
  view = NULL;
  pl = NULL;
  bl = NULL;
}

CelTest::~CelTest ()
{
  if (pl) pl->DecRef ();
  if (bl) bl->DecRef ();
  if (vc) vc->DecRef ();
  if (engine) engine->DecRef ();
  if (loader) loader->DecRef();
  if (g3d) g3d->DecRef ();
  if (kbd) kbd->DecRef ();
  if (view) view->DecRef ();
  csInitializer::DestroyApplication (object_reg);
}

void CelTest::SetupFrame ()
{
  // First get elapsed time from the virtual clock.
  csTicks elapsed_time = vc->GetElapsedTicks ();
  
  // Now rotate the camera according to keyboard state
  float speed = (elapsed_time / 1000.0) * (0.03 * 20);

  iCamera* c = view->GetCamera();
  if (kbd->GetKeyState (CSKEY_RIGHT))
    c->GetTransform ().RotateThis (VEC_ROT_RIGHT, speed);
  if (kbd->GetKeyState (CSKEY_LEFT))
    c->GetTransform ().RotateThis (VEC_ROT_LEFT, speed);
  if (kbd->GetKeyState (CSKEY_PGUP))
    c->GetTransform ().RotateThis (VEC_TILT_UP, speed);
  if (kbd->GetKeyState (CSKEY_PGDN))
    c->GetTransform ().RotateThis (VEC_TILT_DOWN, speed);
  if (kbd->GetKeyState (CSKEY_UP))
    c->Move (VEC_FORWARD * 4 * speed);
  if (kbd->GetKeyState (CSKEY_DOWN))
    c->Move (VEC_BACKWARD * 4 * speed);

  // Tell 3D driver we're going to display 3D things.
  if (!g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS))
    return;

  // Tell the camera to render into the frame buffer.
  view->Draw ();
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
      iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
      if (q)
      {
        q->GetEventOutlet()->Broadcast (cscmdQuit);
        q->DecRef ();
      }
      return true;
    }
    else if (ev.Key.Code == ' ')
    {
      CreateRoom ();
    }
  }
  return false;
}

bool CelTest::CelTestEventHandler (iEvent& ev)
{
  return celtest->HandleEvent (ev);
}

void CelTest::CreateObject (int x, int y, int z, const char* obj)
{
  char name[50];
  sprintf (name, "obj_%dx%dx%d_%s", x, y, z, obj);
  iMeshFactoryWrapper* imeshfact = engine->GetMeshFactories ()
  	->FindByName (obj);
  iMeshWrapper* object = engine->CreateMeshWrapper (
  	imeshfact, name, room, csVector3 (x, y, z));
  object->GetMovable ()->UpdateMove ();
  object->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);
  object->DecRef ();
}

void CelTest::CreateRoom ()
{
  engine->SelectRegion ("room");
  engine->GetCurrentRegion ()->DeleteAll ();

  iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName ("stone");

  float ow_bot = -.5;
  float ow_top = 20;
  float ow_dim = 4.5;

  room = engine->CreateSector ("room");
  iMeshWrapper* walls = engine->CreateSectorWallsMesh (room, "walls");
  iThingState* walls_state = SCF_QUERY_INTERFACE (walls->GetMeshObject (),
  	iThingState);
  iPolygon3D* p;
  p = walls_state->CreatePolygon ();
  p->SetMaterial (tm);
  p->CreateVertex (csVector3 (-ow_dim, ow_bot, ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_bot, ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_bot, -ow_dim));
  p->CreateVertex (csVector3 (-ow_dim, ow_bot, -ow_dim));
  p->SetTextureSpace (p->GetVertex (0), p->GetVertex (1), 3);

  p = walls_state->CreatePolygon ();
  p->SetMaterial (tm);
  p->CreateVertex (csVector3 (-ow_dim, ow_top, -ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_top, -ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_top, ow_dim));
  p->CreateVertex (csVector3 (-ow_dim, ow_top, ow_dim));
  p->SetTextureSpace (p->GetVertex (0), p->GetVertex (1), 3);

  p = walls_state->CreatePolygon ();
  p->SetMaterial (tm);
  p->CreateVertex (csVector3 (-ow_dim, ow_top, ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_top, ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_bot, ow_dim));
  p->CreateVertex (csVector3 (-ow_dim, ow_bot, ow_dim));
  p->SetTextureSpace (p->GetVertex (0), p->GetVertex (1), 3);

  p = walls_state->CreatePolygon ();
  p->SetMaterial (tm);
  p->CreateVertex (csVector3 (ow_dim, ow_top, ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_top, -ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_bot, -ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_bot, ow_dim));
  p->SetTextureSpace (p->GetVertex (0), p->GetVertex (1), 3);

  p = walls_state->CreatePolygon ();
  p->SetMaterial (tm);
  p->CreateVertex (csVector3 (-ow_dim, ow_top, -ow_dim));
  p->CreateVertex (csVector3 (-ow_dim, ow_top, ow_dim));
  p->CreateVertex (csVector3 (-ow_dim, ow_bot, ow_dim));
  p->CreateVertex (csVector3 (-ow_dim, ow_bot, -ow_dim));
  p->SetTextureSpace (p->GetVertex (0), p->GetVertex (1), 3);

  p = walls_state->CreatePolygon ();
  p->SetMaterial (tm);
  p->CreateVertex (csVector3 (ow_dim, ow_top, -ow_dim));
  p->CreateVertex (csVector3 (-ow_dim, ow_top, -ow_dim));
  p->CreateVertex (csVector3 (-ow_dim, ow_bot, -ow_dim));
  p->CreateVertex (csVector3 (ow_dim, ow_bot, -ow_dim));
  p->SetTextureSpace (p->GetVertex (0), p->GetVertex (1), 3);

  walls_state->DecRef ();
  walls->DecRef ();

  iStatLight* light;
  iLightList* ll = room->GetLights ();

  light = engine->CreateLight (NULL, csVector3 (-3, 5, 0), 10,
  	csColor (1, .6, .6), false);
  ll->Add (light->QueryLight ());
  light->DecRef ();

  light = engine->CreateLight (NULL, csVector3 (3, 5,  0), 10,
  	csColor (.3, .3, 1), false);
  ll->Add (light->QueryLight ());
  light->DecRef ();

  light = engine->CreateLight (NULL, csVector3 (0, 5, -3), 10,
  	csColor (.6, 1, .6), false);
  ll->Add (light->QueryLight ());
  light->DecRef ();

  engine->Prepare ();

  //CreateObject (-3, 0, 3, OBJ_SPARKBOX);
  //CreateObject (-2, 0, 3, OBJ_MARBLEBOX);
  //CreateObject (-1, 0, 3, OBJ_SPARKBOX);

  iTextureManager* txtmgr = g3d->GetTextureManager ();
  txtmgr->SetPalette ();

  view->GetCamera ()->SetSector (room);
  view->GetCamera ()->GetTransform ().SetOrigin (csVector3 (0, 4, -5));
  view->GetCamera ()->GetTransform ().LookAt (
  	csVector3 (0, -.6, 1), csVector3 (0, 1, 0));
}

bool CelTest::LoadTexture (const char* txtName, const char* fileName)
{
  if (!loader->LoadTexture (txtName, fileName))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"Error loading texture '%s'!", fileName);
    return false;
  }
  return true;
}

iCelPropertyClassFactory* CelTest::LoadPcFactory (const char* pcfactname)
{
  iPluginManager* plugin_mgr = CS_QUERY_REGISTRY (object_reg, iPluginManager);
  iCelPropertyClassFactory* pf = CS_LOAD_PLUGIN (plugin_mgr,
  	pcfactname, iCelPropertyClassFactory);
  plugin_mgr->DecRef ();
  if (!pf)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"CEL '%s' factory missing!", pcfactname);
    return NULL;
  }
  pl->RegisterPropertyClassFactory (pf);
  return pf;
}

iCelPropertyClass* CelTest::CreatePropertyClass (iCelEntity* entity, iCelPropertyClassFactory* pf,
		const char* name)
{
  iCelPropertyClass* pc = pf->CreatePropertyClass (name);
  if (!pc)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"Factory doesn't seem to support '%s' property class!", name);
    return NULL;
  }
  entity->GetPropertyClassList ()->Add (pc);
  pc->DecRef ();
  return pc;
}

bool CelTest::LoadMeshFactory (const char* fileName)
{
  iMeshFactoryWrapper* imeshfact = loader->LoadMeshObjectFactory (fileName);
  if (imeshfact == NULL)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"Error loading mesh object factory '%s'!", fileName);
    return false;
  }
  imeshfact->DecRef ();
  return true;
}

bool CelTest::Initialize (int argc, const char* const argv[])
{
  object_reg = csInitializer::CreateEnvironment ();
  if (!object_reg) return false;

  // @@@ The code below is temporary until we have a general solution
  // in CS for having plugins outside the CS hierarchy (by an additional
  // .csinfo file for every plugin for example).
  iSCF::SCF->RegisterClass ("cel.physicallayer", "plimp", NULL);
  iSCF::SCF->RegisterClass ("cel.behaviourlayer.test", "bltest", NULL);
  iSCF::SCF->RegisterClass ("cel.pcfactory.test", "pftest", NULL);
  iSCF::SCF->RegisterClass ("cel.pcfactory.mesh", "pfmesh", NULL);
  iSCF::SCF->RegisterClass ("cel.pcfactory.inventory", "pfinv", NULL);

  csInitializer::SetupCommandLineParser (object_reg, argc, argv);
  if (!csInitializer::RequestPlugins (object_reg,
  	CS_REQUEST_VFS,
	CS_REQUEST_SOFTWARE3D,
	CS_REQUEST_ENGINE,
	CS_REQUEST_FONTSERVER,
	CS_REQUEST_IMAGELOADER,
	CS_REQUEST_LEVELLOADER,
	CS_REQUEST_REPORTER,
	CS_REQUEST_REPORTERLISTENER,
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

  iPluginManager* plugin_mgr = CS_QUERY_REGISTRY (object_reg, iPluginManager);
  pl = CS_LOAD_PLUGIN (plugin_mgr, "cel.physicallayer", iCelPlLayer);
  bl = CS_LOAD_PLUGIN (plugin_mgr, "cel.behaviourlayer.test", iCelBlLayer);
  plugin_mgr->DecRef (); plugin_mgr = NULL;
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"CEL physical layer missing!");
    return false;
  }
  if (!bl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"CEL test behaviour layer missing!");
    return false;
  }

  iCelPropertyClassFactory* pftest = LoadPcFactory ("cel.pcfactory.test");
  if (!pftest) return false;
  iCelPropertyClassFactory* pfmesh = LoadPcFactory ("cel.pcfactory.mesh");
  if (!pfmesh) return false;
  iCelPropertyClassFactory* pfinv = LoadPcFactory ("cel.pcfactory.inventory");
  if (!pfinv) return false;

  // Open the main system. This will open all the previously loaded plug-ins.
  if (!csInitializer::OpenApplication (object_reg))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"Error opening system!");
    return false;
  }

  if (!LoadTexture ("stone", "/lib/std/stone4.gif")) return false;
  if (!LoadTexture ("spark", "/lib/std/spark.png")) return false;
  if (!LoadTexture ("marble", "/lib/stdtex/andrew_marble4.jpg")) return false;

  // First disable the lighting cache. Our app is simple enough
  // not to need this.
  engine->SetLightingCacheMode (0);

  //if (!LoadMeshFactory ("/this/celtest_data/sparkbox")) return false;
  //if (!LoadMeshFactory ("/this/celtest_data/marblebox")) return false;

  view = new csView (engine, g3d);
  iGraphics2D* g2d = g3d->GetDriver2D ();
  view->SetRectangle (0, 0, g2d->GetWidth (), g2d->GetHeight ());

  CreateRoom ();

  iCelEntity* entity_box, * entity_dummy1, * entity_dummy2, * entity_dummy3, * entity_dummy4;
  iCelPropertyClass* pc;
  iPcCharacteristics* pcchars;
  
  //===============================
  // Create the box entity.
  //===============================
  entity_box = pl->CreateEntity (); entity_box->SetName ("box");
  entity_box->SetBlEntity (bl->CreateBlEntity ("printer"));

  pc = CreatePropertyClass (entity_box, pftest, "pctest");
  if (!pc) return false;
  iPcTest* pctest = SCF_QUERY_INTERFACE (pc, iPcTest);
  pctest->Print ("Hello world!");
  pctest->DecRef ();

  pc = CreatePropertyClass (entity_box, pfmesh, "pcmesh");
  if (!pc) return false;
  iPcMesh* pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);
  pcmesh->SetMesh ("sparkbox", "/this/celtest/data/sparkbox");
  pcmesh->MoveMesh (room, csVector3 (0));
  pcmesh->DecRef ();

  pc = CreatePropertyClass (entity_box, pfmesh, "pcmeshselect");
  if (!pc) return false;
  iPcMeshSelect* pcmeshsel = SCF_QUERY_INTERFACE (pc, iPcMeshSelect);
  pcmeshsel->SetCamera (view->GetCamera ());
  pcmeshsel->DecRef ();

  pc = CreatePropertyClass (entity_box, pfinv, "pcinventory");
  if (!pc) return false;
  iPcInventory* pcinv = SCF_QUERY_INTERFACE (pc, iPcInventory);
  pcinv->SetConstraints ("size", 0, 10, 100);
  pcinv->SetConstraints ("weight", 0, .5, 1000000);
  pcinv->SetStrictCharacteristics ("size", true);
  // pcinv is decreffed later.

  pc = CreatePropertyClass (entity_box, pfinv, "pccharacteristics");
  if (!pc) return false;
  pcchars = SCF_QUERY_INTERFACE (pc, iPcCharacteristics);
  pcchars->SetCharacteristic ("size", 3);
  pcchars->SetCharacteristic ("weight", 4);
  pcchars->SetInheritedCharacteristic ("size", 0, 0);
  pcchars->SetInheritedCharacteristic ("weight", .5, 0);
  pcchars->DecRef ();

  //===============================
  // Create four dummy entities.
  //===============================
  entity_dummy1 = pl->CreateEntity (); entity_dummy1->SetName ("dummy1");
  entity_dummy1->SetBlEntity (bl->CreateBlEntity ("printer"));
  pc = CreatePropertyClass (entity_dummy1, pfinv, "pccharacteristics");
  if (!pc) return false;
  pcchars = SCF_QUERY_INTERFACE (pc, iPcCharacteristics);
  pcchars->SetCharacteristic ("size", 3);
  pcchars->SetCharacteristic ("weight", .3);
  pcchars->DecRef ();

  entity_dummy2 = pl->CreateEntity (); entity_dummy2->SetName ("dummy2");
  entity_dummy2->SetBlEntity (bl->CreateBlEntity ("printer"));
  pc = CreatePropertyClass (entity_dummy2, pfinv, "pccharacteristics");
  if (!pc) return false;
  pcchars = SCF_QUERY_INTERFACE (pc, iPcCharacteristics);
  pcchars->SetCharacteristic ("size", 3);
  pcchars->SetCharacteristic ("weight", .8);
  pcchars->DecRef ();
 
  entity_dummy3 = pl->CreateEntity (); entity_dummy3->SetName ("dummy3");
  entity_dummy3->SetBlEntity (bl->CreateBlEntity ("printer"));
  pc = CreatePropertyClass (entity_dummy3, pfinv, "pccharacteristics");
  if (!pc) return false;
  pcchars = SCF_QUERY_INTERFACE (pc, iPcCharacteristics);
  pcchars->SetCharacteristic ("weight", .2);
  pcchars->DecRef ();
 
  entity_dummy4 = pl->CreateEntity (); entity_dummy4->SetName ("dummy4");
  entity_dummy4->SetBlEntity (bl->CreateBlEntity ("printer"));
  pc = CreatePropertyClass (entity_dummy4, pfinv, "pccharacteristics");
  if (!pc) return false;
  pcchars = SCF_QUERY_INTERFACE (pc, iPcCharacteristics);
  pcchars->SetCharacteristic ("size", 5);
  pcchars->SetCharacteristic ("weight", .3);
  pcchars->DecRef ();
 
  //===============================
  // Try adding them to the inventory.
  //===============================
  pcinv->Dump ();
  if (pcinv->AddEntity (entity_dummy1)) printf ("Entity 1 added!\n");
  else printf ("Entity 1 NOT added!\n");
  pcinv->Dump ();
  if (pcinv->AddEntity (entity_dummy2)) printf ("Entity 2 added!\n");
  else printf ("Entity 2 NOT added!\n");
  pcinv->Dump ();
  if (pcinv->AddEntity (entity_dummy3)) printf ("Entity 3 added!\n");
  else printf ("Entity 3 NOT added!\n");
  pcinv->Dump ();
  if (pcinv->AddEntity (entity_dummy4)) printf ("Entity 4 added!\n");
  else printf ("Entity 4 NOT added!\n");
  pcinv->Dump ();

  pcchars = CEL_QUERY_PROPCLASS (entity_dummy1->GetPropertyClassList (), iPcCharacteristics);
  if (pcchars->SetCharacteristic ("size", 110)) printf ("Entity 1 resized to 110!\n");
  else printf ("Entity 1 NOT resized to 110!\n");
  pcinv->Dump ();
  if (pcchars->SetCharacteristic ("size", 5)) printf ("Entity 1 resized to 5!\n");
  else printf ("Entity 1 NOT resized to 5!\n");
  pcinv->Dump ();
  pcchars->DecRef ();

  pcchars = CEL_QUERY_PROPCLASS (entity_box->GetPropertyClassList (), iPcCharacteristics);
  pcchars->Dump ();
  pcchars->DecRef ();

  pcinv->DecRef ();

  //===============================
  pftest->DecRef ();
  pfmesh->DecRef ();
  pfinv->DecRef ();

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
  celtest = new CelTest ();

  if (celtest->Initialize (argc, argv))
    celtest->Start ();

  delete celtest;
  return 0;
}
