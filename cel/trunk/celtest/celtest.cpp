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
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"CEL physical layer missing!");
    return false;
  }
  bl = CS_LOAD_PLUGIN (plugin_mgr, "cel.behaviourlayer.test", iCelBlLayer);
  if (!bl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"CEL test behaviour layer missing!");
    return false;
  }
  iCelPropertyClassFactory* pftest = CS_LOAD_PLUGIN (plugin_mgr,
  	"cel.pcfactory.test", iCelPropertyClassFactory);
  if (!pftest)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"CEL test factory missing!");
    return false;
  }
  pl->RegisterPropertyClassFactory (pftest);

  iCelEntity* entity = pl->CreateEntity ();
  iCelPropertyClass* pc = pftest->CreatePropertyClass ("pctest");
  if (!pc)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"'pftest' doesn't seem to support 'pctest' property class!");
    return false;
  }
  entity->GetPropertyClassList ()->Add (pc);

  pftest->DecRef ();
  plugin_mgr->DecRef ();

  // Open the main system. This will open all the previously loaded plug-ins.
  if (!csInitializer::OpenApplication (object_reg))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"crystalspace.application.celtest",
    	"Error opening system!");
    return false;
  }

  // First disable the lighting cache. Our app is simple enough
  // not to need this.
  engine->SetLightingCacheMode (0);

  if (!LoadTexture ("stone", "/lib/std/stone4.gif")) return false;
  if (!LoadTexture ("spark", "/lib/std/spark.png")) return false;
  if (!LoadTexture ("marble", "/lib/stdtex/andrew_marble4.jpg")) return false;

  //if (!LoadMeshFactory ("/this/celtest_data/sparkbox")) return false;
  //if (!LoadMeshFactory ("/this/celtest_data/marblebox")) return false;

  view = new csView (engine, g3d);
  iGraphics2D* g2d = g3d->GetDriver2D ();
  view->SetRectangle (0, 0, g2d->GetWidth (), g2d->GetHeight ());

  CreateRoom ();
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
