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
#include "iengine/light.h"
#include "iengine/texture.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/material.h"
#include "iengine/region.h"
#include "imesh/thing.h"
#include "imesh/sprite3d.h"
#include "imesh/object.h"
#include "imesh/nskeleton.h"
#include "imesh/skelanim.h"
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
#include "csutil/csshlib.h"

#include "celtool/initapp.h"
#include "celtool/persisthelper.h"
#include "tools/animsys.h"
#include "tools/expression.h"
#include "physicallayer/pl.h"
#include "physicallayer/propfact.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/bl.h"
#include "propclass/animation.h"
#include "propclass/test.h"
#include "propclass/mesh.h"
#include "propclass/meshsel.h"
#include "propclass/inv.h"
#include "propclass/chars.h"
#include "propclass/move.h"
#include "propclass/tooltip.h"
#include "propclass/newcamera.h"
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "propclass/region.h"
#include "propclass/input.h"
#include "propclass/linmove.h"
#include "propclass/actoranalog.h"
#include "propclass/prop.h"
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
}

void CelTest::ProcessFrame ()
{
  // We let the entity system do this so there is nothing here.
  pcanim->DrawSkeleton (g3d);
  return;

  if (!parser) return;
  celData ret;
  if (!expr->Execute (0, ret))
  {
    puts ("Error running expression!");
    return;
  }
  switch (ret.type)
  {
    case CEL_DATA_LONG:
      printf ("LONG: %d\n", ret.value.l);
      break;
    case CEL_DATA_ULONG:
      printf ("ULONG: %u\n", ret.value.ul);
      break;
    case CEL_DATA_BOOL:
      printf ("BOOL: %d\n", ret.value.b);
      break;
    case CEL_DATA_FLOAT:
      printf ("FLOAT: %g\n", ret.value.f);
      break;
    case CEL_DATA_STRING:
      printf ("STRING: %s\n", ret.value.s->GetData ());
      break;
    case CEL_DATA_PCLASS:
      printf ("PC: %s\n", ret.value.pc ? ret.value.pc->GetName () : "<null>");
      break;
    case CEL_DATA_ENTITY:
      printf ("ENTITY: %s\n",
	  ret.value.ent ? ret.value.ent->GetName () : "<null>");
      break;
    case CEL_DATA_COLOR:
      printf ("COLOR: %g,%g,%g\n", ret.value.col.red,
	  ret.value.col.green, ret.value.col.blue);
      break;
    case CEL_DATA_VECTOR3:
      printf ("VECTOR3: %g,%g,%g\n", ret.value.v.x,
	  ret.value.v.y, ret.value.v.z);
      break;
    case CEL_DATA_VECTOR2:
      printf ("VECTOR2: %g,%g\n", ret.value.v.x,
	  ret.value.v.y);
      break;
    default:
      printf ("UNKNOWN\n");
      break;
  }
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
      csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
      q->GetEventOutlet ()->Broadcast (csevQuit (object_reg));
    }
    else if (code == 's')
    {
      csRef<iCelPersistence> p = 
      	csQueryRegistry<iCelPersistence> (object_reg);
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
      csRef<iCelPersistence> p = 
      	csQueryRegistry<iCelPersistence> (object_reg);
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
    else if (code == 'p')
    {
      pcprop->SetProperty ("curraction", "punch");
    }
  }
  return false;
}

csPtr<iCelEntity> CelTest::CreateActor (const char* name,
	const char* /*factname*/, const csVector3& /*pos*/)
{
  // The Real Camera
  csRef<iCelEntity> entity_cam = pl->CreateEntity (name, bltest, "actor",
  	"pcinput.standard",
	"pcmove.actor.analog",
	"pccamera.standard",
	"pcobject.mesh",
	"pcobject.mesh.select",
	"pcmove.linear",
	"pc2d.tooltip",
	"pctools.inventory",
	"pcsound.listener",
	"pcobject.mesh.animation",
	"pctools.properties",
	CEL_PROPCLASS_END);
  if (!entity_cam) return 0;

  csRef<iPcCommandInput> pcinp = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcCommandInput);
  pcinp->Bind ("JoystickButton4", "ready");
  pcinp->Bind ("JoystickButton6", "lockon");
  pcinp->Bind ("JoystickButton2", "resetcam");
  pcinp->Bind ("JoystickAxis0", "joyaxis0");
  pcinp->Bind ("JoystickAxis1", "joyaxis1");

  pcinp->Bind ("z", "ready");
  pcinp->Bind ("x", "lockon");
  pcinp->Bind ("c", "resetcam");
  pcinp->Bind ("left", "left");
  pcinp->Bind ("right", "right");
  pcinp->Bind ("up", "up");
  pcinp->Bind ("down", "down");

  csRef<iPcNewCamera> newcamera = CEL_QUERY_PROPCLASS_ENT (
    entity_cam, iPcNewCamera);
  newcamera->AttachCameraMode(iPcNewCamera::CCM_TRACKING);

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcMesh);
  bool hascal3d = true;
  pcmesh->SetPath ("/cellib/objects");
  hascal3d = pcmesh->SetMesh ("test", "cally.cal3d");
  //vfs->ChDir ("/lib/kwartz");
  pcmesh->SetPath ("/lib/kwartz");
  pcmesh->SetMesh ("kwartz_fact", "kwartz.lib");

  pcanim = celQueryPropertyClassEntity<iPcAnimation> (entity_cam);
  pcanim->Setup ();
  pcanim->Load ("/cellib/lev/", "animation.xml");

  pcprop = celQueryPropertyClassEntity<iPcProperties> (entity_cam);
  pcprop->SetProperty ("curraction", "<none>");
  pcprop->SetProperty ("speed", 2.0f);

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
  	"pcworld.zonemanager",
	"pctools.inventory",
  	CEL_PROPCLASS_END);

  //===============================
  // Engine init.
  //===============================
  engine->Prepare ();

  csRef<iCommandLineParser> cmdline = 
  	csQueryRegistry<iCommandLineParser> (object_reg);
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

  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
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

//--------testing------------------------------------
  return true;
  puts ("--------------TESTING CONSOLE!");
  csRef<iObjectRegistryIterator> it = object_reg->Get (
      scfInterfaceTraits<iCelExpressionParser>::GetID (),
      scfInterfaceTraits<iCelExpressionParser>::GetVersion ());
  iBase* b = it->Next ();

  if (b)
  {
    parser = scfQueryInterface<iCelExpressionParser> (b);
  }
  if (!parser)
  {
    csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (
	object_reg);
    parser = csLoadPlugin<iCelExpressionParser> (plugmgr,
      "cel.behaviourlayer.xml");
    if (!parser)
    {
      puts ("Cannot find the expression parser!\n");
      return false;
    }
    object_reg->Register (parser, "iCelExpressionParser");
  }

  if (!parser) return false;
  expr = parser->Parse ("property(pc(ent(), pcmove.linear), propid(currspeed))");
  if (!expr)
  {
    puts ("Error parsing expression!");
    return false;
  }
  celData ret;
  if (!expr->Execute (entity_dummy, ret))
  {
    puts ("Error running expression!");
    return false;
  }
  switch (ret.type)
  {
    case CEL_DATA_LONG:
      printf ("LONG: %d\n", ret.value.l);
      break;
    case CEL_DATA_ULONG:
      printf ("ULONG: %u\n", ret.value.ul);
      break;
    case CEL_DATA_BOOL:
      printf ("BOOL: %d\n", ret.value.b);
      break;
    case CEL_DATA_FLOAT:
      printf ("FLOAT: %g\n", ret.value.f);
      break;
    case CEL_DATA_STRING:
      printf ("STRING: %s\n", ret.value.s->GetData ());
      break;
    case CEL_DATA_PCLASS:
      printf ("PC: %s\n", ret.value.pc ? ret.value.pc->GetName () : "<null>");
      break;
    case CEL_DATA_ENTITY:
      printf ("ENTITY: %s\n",
	  ret.value.ent ? ret.value.ent->GetName () : "<null>");
      break;
    case CEL_DATA_COLOR:
      printf ("COLOR: %g,%g,%g\n", ret.value.col.red,
	  ret.value.col.green, ret.value.col.blue);
      break;
    case CEL_DATA_VECTOR3:
      printf ("VECTOR3: %g,%g,%g\n", ret.value.v.x,
	  ret.value.v.y, ret.value.v.z);
      break;
    case CEL_DATA_VECTOR2:
      printf ("VECTOR2: %g,%g\n", ret.value.v.x,
	  ret.value.v.y);
      break;
    default:
      printf ("UNKNOWN\n");
      break;
  }

  return true;
}





bool CelTest::OnInitialize (int argc, char* argv[])
{
  if (!celInitializer::SetupConfigManager (object_reg,
  	"/celconfig/celtest.cfg"))
  {
    return ReportError ("Can't setup config file!");
  }

  csRef<iCommandLineParser> cmdline = 
  	csQueryRegistry<iCommandLineParser> (object_reg);
  cmdline->AddOption ("silent", "");

  if (!celInitializer::RequestPlugins (object_reg,
  	CS_REQUEST_VFS,
	CS_REQUEST_OPENGL3D,
	CS_REQUEST_ENGINE,
	CS_REQUEST_PLUGIN("crystalspace.nskeleton.graveyard", Skeleton::iGraveyard),
	CS_REQUEST_PLUGIN("cel.animation.system", CEL::Animation::iAnimationSystem),
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
	CS_REQUEST_PLUGIN ("crystalspace.device.joystick", iEventPlug),
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
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  if (!vc) return ReportError ("Can't find the virtual clock!");

  // Find the pointer to engine plugin
  engine = csQueryRegistry<iEngine> (object_reg);
  if (!engine) return ReportError ("No iEngine plugin!");

  loader = csQueryRegistry<iLoader> (object_reg);
  if (!loader) return ReportError ("No iLoader plugin!");

  g3d = csQueryRegistry<iGraphics3D> (object_reg);
  if (!g3d) return ReportError ("No iGraphics3D plugin!");

  kbd = csQueryRegistry<iKeyboardDriver> (object_reg);
  if (!kbd) return ReportError ("No iKeyboardDriver plugin!");

  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl) return ReportError ("CEL physical layer missing!");

  // we find the scf object already in cel's example behaviour layer
  // in plugins/behaviourlayer/test to use
  bltest = csQueryRegistryTagInterface<iCelBlLayer> (
    object_reg, "iCelBlLayer.Test");
  if (!bltest) return ReportError ("CEL test behaviour layer missing!");
  // after we have a pointer to the behaviourlayer we want to use
  // able to create the behaviours we want, then we register it for usage
  // with the physical layer.
  pl->RegisterBehaviourLayer (bltest);

  if (!CreateRoom ()) return false;

  // This calls the default runloop. This will basically just keep
  // broadcasting process events to keep the game going.
  Run ();

  return true;
}
