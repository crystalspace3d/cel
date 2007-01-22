/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Christian Van Brussel

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
#include "nettest.h"
#include "netmanagers.h"
#include "networktypes.h"
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

NetTest::NetTest ()
{
  SetApplicationName ("Network test");
  gfm = 0;
}

NetTest::~NetTest ()
{
  delete gfm;
}

void NetTest::OnExit ()
{
  if (pl) pl->CleanCache ();
}

void NetTest::ProcessFrame ()
{
  // We let the entity system do this so there is nothing here.
}

void NetTest::PostProcessFrame ()
{
  // We let the entity system do this so there is nothing here.
  if (gfm && g3d->BeginDraw (CSDRAW_2DGRAPHICS))
  {
    // write player data
    if (gfm->IsClientAvailable ())
    {
      celPlayer* player = gfm->game->GetGameClient()->GetPlayer();
      gfm->game->GetGameClient()->GetNetworkStats (stats);    
      celGameInfo* game_info = gfm->game->GetGameInfo ();
      csString player_data;
      player_data.Format("(%d) %s (%s:%d) - Server %d (%s:%d) - Latency: %d ms", 
			 player->player_id, 
			 player->player_name.GetData(), 
			 player->hostname.GetData (), 
			 player->port_nb, 
			 game_info->game_id, 
			 game_info->hostname.GetData (), 
			 game_info->port_nb, 
			 stats.latency);
      WriteShadow( 0, 10, 10, g2d->FindRGB(219, 230, 222), "%s",
      	player_data.GetData());
    }

    // write network state
    WriteShadow( 0, 10, 22, g2d->FindRGB(219, 230, 222), "%s",
    	gfm->GetConnectionState().GetData());

    // write comments
    if (gfm->client_manager)
    {
      int pos_y = 34;
      size_t i = 0;
      for ( ; i < gfm->client_manager->comments.Length(); i++)
      {
	WriteShadow( 0, 10, pos_y, g2d->FindRGB(119, 130, 122), "%s",
		gfm->client_manager->comments[i].GetData());
	pos_y += 12;
      }
    }
  }
}

void NetTest::FinishFrame ()
{
  if (gfm && gfm->IsClientAvailable ())
  {
    g3d->FinishDraw ();
    g3d->Print (0);
  }
}

bool NetTest::OnKeyboard (iEvent &ev)
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
      // is by broadcasting a csevQuit event. That will cause the
      // main runloop to stop. To do that we get the event queue from
      // the object registry and then post the event.
      csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
      q->GetEventOutlet ()->Broadcast (csevQuit (object_reg));
    }

    if (gfm && gfm->IsClientAvailable ())
    {
      if (code == CSKEY_F1)
      {
	printf ("Key F1 pressed: sending two empty events\n");
	fflush (stdout);

	// launch simple client event
	celClientEventData event_data;
	event_data.event_type = CLIENT_EVENT_SIMPLE_TEST;
	gfm->GetClient ()->LaunchClientEvent (event_data);
	gfm->GetClient ()->LaunchClientEvent (event_data);
      }

      if (code == CSKEY_F2)
      {
	printf ("Key F2 pressed: sending two complex data buffers\n");
	fflush (stdout);

	// launch complex client event
	celClientEventData event;
	event.event_type = CLIENT_EVENT_COMPLEX_TEST;
	event.event_time = 100;
	event.event_data = pl->CreateDataBuffer (0);
	event.event_data->Add ((bool) 1);
	event.event_data->Add ((int8) -5);
	event.event_data->Add ((uint8) 5);
	event.event_data->Add ((int16) -50);
	event.event_data->Add ((uint16) 50);
	event.event_data->Add ((int32) -500);
	event.event_data->Add ((uint32) 500);
	event.event_data->Add ((float) 3.5);
	csString txt = "nice text nice text nice text nice text nice text nice text nice text nice text";
	event.event_data->Add (txt.GetData ());
	csVector2 v2;
	v2.x = 10;
	v2.y = 20;
	event.event_data->Add (v2);
	csVector3 v3;
	v3.x = 10;
	v3.y = 20;
	v3.z = 30;
	event.event_data->Add (v3);
	event.event_data->Add (csColor (119, 130, 122));
	gfm->GetClient ()->LaunchClientEvent (event);
	gfm->GetClient ()->LaunchClientEvent (event);
      }

      if (code == CSKEY_F3)
      {
	printf ("Key F3 pressed: sending an event to be forwarded to all players\n");
	fflush (stdout);

	// launch client event
	celClientEventData event;
	event.event_type = CLIENT_EVENT_FORWARD_TEST;
	event.event_time = 1000;
	event.event_data = pl->CreateDataBuffer (0);
	csString txt;
	txt.Format("Message forwarded from %s", gfm->GetClient ()
		->GetPlayer ()->player_name.GetData ());
	event.event_data->Add (txt.GetData ());
	event.event_data->Add (gfm->GetClient ()->GetPlayer ()
		->hostname.GetData ());
	event.event_data->Add (gfm->GetClient ()->GetPlayer ()->port_nb);
	gfm->GetClient ()->LaunchClientEvent (event);
      }

      if (code == CSKEY_F4)
      {
	printf ("Key F4 pressed: asking to be kicked\n");
	fflush (stdout);

	// launch client event
	celClientEventData event_data;
	event_data.event_type = CLIENT_EVENT_KICKING_TEST;
	gfm->GetClient ()->LaunchClientEvent (event_data);
      }

      if (code == CSKEY_F5)
      {
	printf ("Key F5 pressed: asking to kick all players\n");
	fflush (stdout);

	// launch client event
	celClientEventData event_data;
	event_data.event_type = CLIENT_EVENT_KICKING_ALL_TEST;
	gfm->GetClient ()->LaunchClientEvent (event_data);
      }
    }

    if (code == CSKEY_F6)
      gfm->factory->CloseGame ();
  }
  return false;
}

csPtr<iCelEntity> NetTest::CreateActor (const char* name,
	const char* /*factname*/, const csVector3& /*pos*/)
{
  // The Real Camera
  csRef<iCelEntity> entity_cam = pl->CreateEntity (name, bltest, "actor",
  	"pccommandinput",
	"pcdefaultcamera",
	"pcactormove",
	"pcmesh",
	"pcmeshselect",
	"pclinearmovement",
	"pctooltip",
	"pcinventory",
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

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcactormove = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcActorMove);
  pcactormove->SetMovementSpeed (2.0f);
  pcactormove->SetRunningSpeed (5.0f);
  pcactormove->SetRotationSpeed (1.75f);
  pcactormove->SetJumpingVelocity (6.31f);

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcMesh);
  bool hascal3d = true;
  pcmesh->SetPath ("/cellib/objects");
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

  scfString regionname, startname;
  pczonemgr->GetLastStartLocation (&regionname, &startname);

  pccamera->SetZoneManager (pczonemgr, true, regionname, startname);
  if (pczonemgr->PointMesh (name, regionname, startname)
  	!= CEL_ZONEERROR_OK)
  {    
    ReportError ("Error finding actor start position!");
    return 0;
  }

  /*
  csRef<iPcInventory> pcinv_room = CEL_QUERY_PROPCLASS_ENT (entity_room,
  	iPcInventory);
  if (!pcinv_room->AddEntity (entity_cam)) return 0;
  */
  return csPtr<iCelEntity> (entity_cam);
}

csPtr<iCelEntity> NetTest::CreateActorNPC (const char* name,
	const char* /*factname*/, const csVector3& /*pos*/)
{
  csRef<iCelEntity> entity_cam = pl->CreateEntity (name, bltest, "actor",
	"pcactormove",
	"pcmesh",
	"pclinearmovement",
	CEL_PROPCLASS_END);
  if (!entity_cam) return 0;

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcactormove = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcActorMove);
  pcactormove->SetMovementSpeed (2.0f);
  pcactormove->SetRunningSpeed (5.0f);
  pcactormove->SetRotationSpeed (1.75f);
  pcactormove->SetJumpingVelocity (6.31f);

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcMesh);
  bool hascal3d = true;
  pcmesh->SetPath ("/cellib/objects");
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

  scfString regionname, startname;
  pczonemgr->GetLastStartLocation (&regionname, &startname);

  if (pczonemgr->PointMesh (name, regionname, startname)
  	!= CEL_ZONEERROR_OK)
  {
    ReportError ("Error finding npc start position!");
    return 0;
  }

  return csPtr<iCelEntity> (entity_cam);
}

csPtr<iCelEntity> NetTest::CreateDefaultCamera (const char *name)
{
  csRef<iCelEntity> default_camera = pl->CreateEntity (name, bltest, "actor",
	"pcdefaultcamera",
	CEL_PROPCLASS_END);
  if (!default_camera) return 0;

  scfString regionname, startname;
  pczonemgr->GetLastStartLocation (&regionname, &startname);
    
  csRef<iPcDefaultCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (
  	default_camera, iPcDefaultCamera);
  pccamera->SetZoneManager (pczonemgr, true, regionname, startname);
  if (pczonemgr->PointMesh (name, regionname, startname)
      != CEL_ZONEERROR_OK)
  {
    ReportError ("Error finding npc start position!");
    return 0;
  }

  return csPtr<iCelEntity> (default_camera);
}

bool NetTest::CreateRoom (const csString path, const csString file)
{
  printf ("Loading level path: %s file: %s\n", path.GetData (),
  	file.GetData ());
  fflush (stdout);

  // Create the room entity.
  entity_room = pl->CreateEntity ("room", bltest, "room",
  	"pczonemanager",
	"pcinventory",
  	CEL_PROPCLASS_END);

  // Engine init.
  engine->Prepare ();

  // Load the level
  csRef<iVFS> vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  csStringArray paths;
  paths.Push ("/cellib/lev/");
  if (!vfs->ChDirAuto (path, &paths, 0, file))
    return ReportError ("Bad file path '%s' at '%s'!", file.GetData (),
    	path.GetData ());

  pczonemgr = CEL_QUERY_PROPCLASS_ENT (entity_room,
  	iPcZoneManager);
  if (!pczonemgr->Load (0, file))
    return ReportError ("Error loading level '%s' at '%s'!", file.GetData (),
    	path.GetData ());

  printf ("Level loaded\n");
  fflush (stdout);

  return true;
}

bool NetTest::OnInitialize (int argc, char* argv[])
{
  csDebuggingGraph::SetupGraph (object_reg);

  // analyse command line arguments
  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);

  // help
  if (cmdline->GetOption ("nethelp") != 0 || cmdline->GetOption ("help") != 0)
  {
    csPrintf ("\nYou can use the following options to this program:\n");
    csPrintf (" -path=[value]    The path of the level to be loaded (to be used on server side)\n");
    csPrintf (" -file=[value]    The file of the level to be loaded (to be used on server side)\n");
    csPrintf (" -server          If a server must be created\n");
    csPrintf (" -client          If a client must be created\n");
    csPrintf (" -serverhost      The hostname of the server (when there is only a client)\n");
    csPrintf (" -singleplayer    If the game is in single player mode\n");
    csPrintf (" -player=[value]  To specify the name of the player\n");
    csPrintf (" -nethelp         This help message\n");
    csPrintf ("\n");
    csPrintf ("  When used without any options, the default behavior is to create\n");
    csPrintf ("  a client and a server.\n");
    csPrintf ("\n");
    csPrintf ("Examples:\n");
    csPrintf ("  nettest -server -path=cellib/lev -file=basic_world\n");
    csPrintf ("  nettest -server -path=lev/terrainf -file=world\n");
    csPrintf ("  nettest -client -player=Marc -serverhost=192.168.1.15\n");
    csPrintf ("  nettest -client -serverhost=localhost\n");
    csPrintf ("\n");
    csPrintf ("WARNING:\n");
    csPrintf ("  Under MS Windows and if you are using MSYS, when you specify \n");
    csPrintf ("  the level path with the --path option, you must be aware that \n");
    csPrintf ("  MSYS adds the full path to MSYS when an option begins with a ""/"".\n");
    csPrintf ("  You can avoid this problem by removing the first ""/"" of the path.\n");
    exit(0);
  }

  bool do_server = cmdline->GetOption ("server") != 0;
  bool do_client = cmdline->GetOption ("client") != 0;

  if (!celInitializer::RequestPlugins (object_reg,
  	CS_REQUEST_VFS,

	CS_REQUEST_PLUGIN (
	(do_server && !do_client)
		? "crystalspace.graphics3d.null"
		: "crystalspace.graphics3d.opengl",
	iGraphics3D),

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
	CS_REQUEST_PLUGIN ("cel.network.tcp", iCelGameFactory),
	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.opcode",
		iCollideSystem),
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

bool NetTest::Application ()
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

  g2d = g3d->GetDriver2D ();

  csRef<iFontServer> fs = g3d->GetDriver2D()->GetFontServer ();
  if (!fs) return ReportError("Failed to locate font server!");
  font = fs->LoadFont (CSFONT_LARGE);
  if (!font) return ReportError("Failed to locate font!");

  kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
  if (!kbd) return ReportError ("No iKeyboardDriver plugin!");

  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl) return ReportError ("CEL physical layer missing!");

  bltest = CS_QUERY_REGISTRY_TAG_INTERFACE (object_reg,
  	"iCelBlLayer.Test", iCelBlLayer);
  if (!bltest) return ReportError ("CEL test behaviour layer missing!");
  pl->RegisterBehaviourLayer (bltest);

  // initialize game factory
  game_factory = CS_QUERY_REGISTRY (object_reg, iCelGameFactory);
  if (!game_factory) return ReportError ("CEL network layer missing!");
  game_factory->SetGameName ("CEL network test");
  game_factory->SetProtocolVersion ("v0.1");

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

  // analyse command line arguments
  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);

  // read level path and file
  csString level_path;
  csString level_file;
  level_path = cmdline->GetOption ("path");
  if (!level_path.IsEmpty ())
  {
    level_file = cmdline->GetOption ("file");
    if (level_file.IsEmpty ()) level_file = "level.xml";
  }
  else
  {
    level_path = "/cellib/lev";
    level_file = "basic_world";
    //level_file = "basic_level.xml";
  }

  // read client-server type of this application
  // 0 means singleplayer, 1 means server, 2 means client, 3 means client + server + open to other players
# define APP_SINGLEPLAYER 0
# define APP_SERVER 1
# define APP_CLIENT 2
# define APP_CLIENTSERVER 3
  int app_type = APP_CLIENT;

  if (cmdline->GetOption ("server") != 0)
  {
    if (cmdline->GetOption ("client") != 0)
      app_type = APP_CLIENTSERVER;
    else
      app_type = APP_SERVER;
  }
  else if (cmdline->GetOption ("client") != 0)
    app_type = APP_CLIENT;
  else if (cmdline->GetOption ("singleplayer") != 0)
    app_type = APP_SINGLEPLAYER;

  // read player name
  csString player_name = cmdline->GetOption ("player");

  // read server hostname
  csString server_hostname = cmdline->GetOption ("serverhost");

  // register game factory manager
  gfm = new GameFactoryManager (this, game_factory, level_path, level_file);
  game_factory->RegisterGameFactoryManager (gfm);

  // create game info
  celGameInfo game_info;
  game_info.game_name = "Test game";
  game_info.hostname = "127.0.0.1";
  game_info.port_nb = 25000;
  game_info.password = "nice password";
  game_info.max_players = 3;
  game_info.custom_data = pl->CreateDataBuffer (0);
  game_info.custom_data->Add (level_path);
  game_info.custom_data->Add (level_file);

  // create player
  celPlayer player;
  if (!player_name.IsEmpty()) player.player_name = player_name;
  else player.player_name = "Test player";

  // launch networked game
  switch (app_type)
  {
    case APP_SINGLEPLAYER:
      csPrintf ("Creating singleplayer game with player %s\n",
    	  player.player_name.GetData ());
      if (!game_factory->CreateNewGame (CEL_NET_SINGLEPLAYER,
    	  &game_info, &player))
        return false;
      break;
    case APP_SERVER:
      csPrintf ("Creating server\n");
      if (!game_factory->CreateNewGame (CEL_NET_LOCAL, &game_info))
        return false;
      break;
    case APP_CLIENT:
      csPrintf ("Starting client with player %s\n",
    	  player.player_name.GetData ());
      if (server_hostname && !server_hostname.IsEmpty ())
        game_info.hostname = server_hostname;
      if (!game_factory->ConnectGame (&game_info, &player))
        return false;
      break;
    case APP_CLIENTSERVER:
      csPrintf ("Starting client and server with player %s\n",
    	  player.player_name.GetData ());
      if (!game_factory->CreateNewGame (CEL_NET_LOCAL, &game_info, &player))
        return false;
      break;
  }

  // This calls the default runloop. This will basically just keep
  // broadcasting process events to keep the game going.
  Run ();

  return true;
}

void NetTest::WriteShadow (int align,int x,int y,int fg,char *str,...)
{
  char buf[256];

  va_list arg;
  va_start (arg, str);
  vsprintf (buf, str, arg);
  va_end (arg);

  Write (align, x+1, y-1, 0, -1, buf);
  Write (align, x, y, fg, -1, buf);
}

void NetTest::Write(int align,int x,int y,int fg,int bg,char *str,...)
{
  va_list arg;
  char b[256], *buf = b;

  va_start (arg,str);
  int l = vsprintf (buf, str, arg);
  va_end (arg);

  if (align != 0)
  {
    int rb = 0;
    if (align == 1)
    {
      int where;
      sscanf (buf, "%d%n", &rb,&where);
      buf += where + 1;
      l -= where + 1;
    }

    int w, h;
    font->GetDimensions (buf, w, h);

    switch(align)
    {
      case 2: x -= w; break;
      case 1: x = (x + rb - w) / 2; break;
    }
  }

  g2d->Write (font, x, y, fg, bg, buf);
}

