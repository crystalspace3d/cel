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
#include "csutil/csobject.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "csutil/event.h"
#include "csutil/common_handlers.h"
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
#include "iengine/collection.h"
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
#include "csutil/csshlib.h"

#include "celtool/initapp.h"
#include "celtool/persisthelper.h"
#include "tools/billboard.h"
#include "physicallayer/pl.h"
#include "physicallayer/propfact.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/bl.h"
#include "propclass/test.h"
#include "propclass/delegcam.h"
#include "propclass/cameras/tracking.h"
#include "propclass/mesh.h"
#include "propclass/meshsel.h"
#include "propclass/inv.h"
#include "propclass/jump.h"
#include "propclass/chars.h"
#include "propclass/move.h"
#include "propclass/tooltip.h"
#include "propclass/newcamera.h"
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "propclass/region.h"
#include "propclass/input.h"
#include "propclass/linmove.h"
#include "propclass/analogmotion.h"
#include "propclass/quest.h"
#include "propclass/trigger.h"
#include "propclass/zone.h"
#include "propclass/sound.h"
#include "propclass/wire.h"
#include "propclass/billboard.h"
#include "propclass/prop.h"

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
  printer.Invalidate ();
}

void CelTest::Frame ()
{
  // We let the entity system do this so there is nothing here.
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
    else if (code == CSKEY_F1)
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
    else if (code == CSKEY_F2)
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
  }
  return false;
}

void CelTest::CreateActionIcon ()
{
  csRef<iCelEntity> entity = pl->CreateEntity ("action_icon", 0, 0,
      "pc2d.billboard",
      CEL_PROPCLASS_END);
  if (!entity) return;

  iTextureWrapper* txt = engine->CreateTexture ("action_icon", "/lib/stdtex/tile.png",
      0, CS_TEXTURE_2D);
  txt->Register (g3d->GetTextureManager ());
  engine->CreateMaterial ("action_icon", txt);

  csRef<iPcBillboard> pcbb = celQueryPropertyClassEntity<iPcBillboard> (entity);
  pcbb->SetBillboardName ("action_icon");
  iBillboard* bb = pcbb->GetBillboard ();
  bb->SetMaterialName ("action_icon");
  bb->SetPosition (1000, 1000);
  bb->SetSize (30000, 30000);
  //bb->GetFlags ().SetAll (CEL_BILLBOARD_VISIBLE);
  bb->GetFlags ().SetAll (0);
}

void CelTest::CreateSettingBar ()
{
  csRef<iCelEntity> entity = pl->CreateEntity ("setting_bar", 0, 0,
      "pc2d.billboard",
      CEL_PROPCLASS_END);
  if (!entity) return;

  csRef<iPcBillboard> pcbb = celQueryPropertyClassEntity<iPcBillboard> (entity);
  pcbb->SetBillboardName ("setting_bar");
  iBillboard* bb = pcbb->GetBillboard ();
  bb->SetText ("Setting:");
  bb->SetPosition (100000, 1000);
  bb->SetSize (200000, 30000);
  bb->GetFlags ().SetAll (0);
}

void CelTest::CreateActor ()
{
  // The Real Camera
  entity_cam = pl->CreateEntity ("camera", 0, 0,
    "pcinput.standard",
    "pcmove.analogmotion",
    "pcmove.jump",
    "pcmove.grab",
    "pccamera.delegate",
    "pccamera.mode.tracking",
    "pcobject.mesh",
    "pcobject.mesh.select",
    "pcmove.linear",
    "pcmove.actor.wasd",
    //"pc2d.tooltip",
    "pctools.inventory",
    "pctools.timer",
    "pcsound.listener",
    "pclogic.trigger",
    "pcmisc.test",
    "pctools.properties",
    "pctools.bag",
    CEL_PROPCLASS_END);
  if (!entity_cam) return;

  csRef<iPcCommandInput> pcinp = celQueryPropertyClassEntity<iPcCommandInput> (entity_cam);
  pcinp->Bind ("JoystickButton4", "ready");
  pcinp->Bind ("JoystickButton6", "lockon");
  pcinp->Bind ("JoystickButton2", "resetcam");
  pcinp->Bind ("JoystickAxis0", "joyaxis0");
  pcinp->Bind ("JoystickAxis1", "joyaxis1");
  pcinp->Bind ("MouseAxis0_centered", "mouseaxis0");

  pcinp->Bind ("e", "action");
  pcinp->Bind ("z", "ready");
  pcinp->Bind ("x", "lockon");
  pcinp->Bind ("c", "resetcam");
  pcinp->Bind ("left", "left");
  pcinp->Bind ("right", "right");
  pcinp->Bind ("up", "up");
  pcinp->Bind ("down", "down");
  pcinp->Bind ("a", "left");
  pcinp->Bind ("d", "right");
  pcinp->Bind ("w", "up");
  pcinp->Bind ("s", "down");
  pcinp->Bind ("space", "jump");
  pcinp->Bind ("m", "freeze");
  pcinp->Bind ("shift", "roll");
  pcinp->Bind ("`", "showstates");
  pcinp->Bind ("[", "camleft");
  pcinp->Bind ("]", "camright");
  pcinp->Bind ("pageup", "camup");
  pcinp->Bind ("pagedown", "camdown");

  pcinp->Bind ("pad5", "settings");
  pcinp->Bind ("pad2", "settings_down");
  pcinp->Bind ("pad8", "settings_up");
  pcinp->Bind ("pad4", "settings_left");
  pcinp->Bind ("pad6", "settings_right");
  pcinp->Bind ("pad7", "settings_fastleft");
  pcinp->Bind ("pad9", "settings_fastright");
  pcinp->Bind ("pad1", "settings_slowleft");
  pcinp->Bind ("pad3", "settings_slowright");

  csRef<iPcJump> jump = celQueryPropertyClassEntity<iPcJump> (entity_cam);
  jump->SetBoostJump (false);
  jump->SetJumpHeight (1.0f);
  //jump->SetDoubleJumpSpeed (7.0f);

  csRef<iPcTrackingCamera> trackcam = celQueryPropertyClassEntity<iPcTrackingCamera> (entity_cam);
  trackcam->SetPanSpeed (8);
  trackcam->SetTiltSpeed (4.5);

  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity_cam);
  bool hascal3d = true;
  pcmesh->SetPath ("/cellib/objects");
  hascal3d = pcmesh->SetMesh ("test", "cally.cal3d");
  //pcmesh->SetPath ("/lib/kwartz");
  //hascal3d = pcmesh->SetMesh ("kwartz_fact", "kwartz.lib");

  csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity_cam);
  if (hascal3d)
  {
    pclinmove->InitCD (
      csVector3 (0.8f, 0.8f,  0.8f),
      csVector3 (0.5f, 0.5f,  0.5f),
      csVector3 (0.0f, 0.01f, 0.0f));
  }
  else
  {
    pclinmove->InitCD (
      csVector3 (0.5f,  0.8f, 0.5f),
      csVector3 (0.5f,  0.4f, 0.5f),
      csVector3 (0.0f, -0.4f, 0.0f));
  }

  csRef<iPcTrigger> trigger = celQueryPropertyClassEntity<iPcTrigger> (entity_cam);
  trigger->SetupTriggerSphere (0, csVector3 (0), 1.0);
  trigger->SetFollowEntity (true);
}

size_t CelTest::WireAddOutput (iPcWire* wire, const char* msgid,
		iCelParameterBlock* params)
{
  return wire->AddOutput (pl->FetchStringID (msgid), 0, params);
}

void CelTest::ConnectWires ()
{
  iCelEntity* action_icon = pl->FindEntity ("action_icon");

  iCelPropertyClass* pc;
  csRef<iPcWire> wire;
  csRef<celOneParameterBlock> params;
  size_t idx;

  pc = pl->CreatePropertyClass (entity_cam, "pclogic.wire");
  // 1: For debugging, print out when we are near an entity.
  wire = scfQueryInterface<iPcWire> (pc);
  wire->AddInput ("cel.trigger.entity.enter");
  idx = WireAddOutput (wire, "cel.test.action.Print");
  wire->MapParameterExpression (idx, "message", "'We found '+@entity");
  // 2: Add the name of the entity to the bag.
  idx = WireAddOutput (wire, "cel.bag.action.AddString");
  wire->MapParameterExpression (idx, "value", "entname(@entity)");
  // 3: Set the visibility state of the billboard.
  params.AttachNew (new celOneParameterBlock (pl->FetchStringID ("name"), "visible"));
  idx = wire->AddOutput (pl->FetchStringID ("cel.billboard.action.SetProperty"),
      action_icon->QueryMessageChannel (), params);
  // Use > 1 because there is also the 'camera' entity itself.
  wire->MapParameterExpression (idx, "value", "property(pc(pctools.bag),id(size))>1");

  // 1: For debugging, print out when we are far from an entity.
  pc = pl->CreatePropertyClass (entity_cam, "pclogic.wire");
  wire = scfQueryInterface<iPcWire> (pc);
  wire->AddInput ("cel.trigger.entity.leave");
  idx = WireAddOutput (wire, "cel.test.action.Print");
  wire->MapParameterExpression (idx, "message", "'We leave '+@entity");
  // 2: Remove the name of the entity from the bag.
  idx = WireAddOutput (wire, "cel.bag.action.RemoveString");
  wire->MapParameterExpression (idx, "value", "entname(@entity)");
  // 3: Set the visibility state of the billboard.
  params.AttachNew (new celOneParameterBlock (pl->FetchStringID ("name"), "visible"));
  idx = wire->AddOutput (pl->FetchStringID ("cel.billboard.action.SetProperty"),
      action_icon->QueryMessageChannel (), params);
  // Use > 1 because there is also the 'camera' entity itself.
  wire->MapParameterExpression (idx, "value", "property(pc(pctools.bag),id(size))>1");

  // If the action key is pressed we send out a 'cel.game.action' to the current entities.
  pc = pl->CreatePropertyClass (entity_cam, "pclogic.wire");
  wire = scfQueryInterface<iPcWire> (pc);
  wire->AddInput ("cel.input.action.up");
  params.AttachNew (new celOneParameterBlock (pl->FetchStringID ("msgid"), "cel.game.action"));
  WireAddOutput (wire, "cel.bag.action.SendMessage", params);
}

bool CelTest::CreateRoom ()
{
  csRef<iCelEntity> entity_room;

  //===============================
  // Create the room entity.
  //===============================
  entity_room = pl->CreateEntity ("room", 0, 0,
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
    //path = "/cellib/lev";
    //file = "basic_level.xml";
    path = "/cellib/smallhouse";
    file = "level.xml";
  }

  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  csStringArray paths;
  paths.Push ("/cellib/lev/");
  if (!vfs->ChDirAuto (path, &paths, 0, file))
    return ReportError ("Bad file path '%s' at '%s'!", file.GetData (),
    	path.GetData ());

  csRef<iPcZoneManager> pczonemgr = celQueryPropertyClassEntity<iPcZoneManager> (entity_room);
  if (!pczonemgr->Load (0, file))
    return ReportError ("Error loading level '%s' at '%s'!", file.GetData (),
    	path.GetData ());

  scfString regionname, startname;
  pczonemgr->GetLastStartLocation (&regionname, &startname);

  CreateActor ();
  if (!entity_cam) return false;
  csRef<iPcCamera> pccamera = celQueryPropertyClassEntity<iPcCamera> (entity_cam);
  if (!pccamera) return false;
  pccamera->SetZoneManager (pczonemgr, true, regionname, startname);
  if (pczonemgr->PointMesh ("camera", regionname, startname)
  	!= CEL_ZONEERROR_OK)
    return ReportError ("Error finding start position!");

  csRef<iPcInventory> pcinv_room = celQueryPropertyClassEntity<iPcInventory> (entity_room);
  if (!pcinv_room->AddEntity (entity_cam)) return false;

  game = entity_room;

  CreateActionIcon ();
  CreateSettingBar ();

  ConnectWires ();

  actorsettings.Initialize (pl);

  return true;
}

bool CelTest::OnInitialize (int argc, char* argv[])
{
  if (!celInitializer::SetupConfigManager (object_reg,
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

  if (!CreateRoom ()) return false;

  printer.AttachNew (new FramePrinter (object_reg));

  // This calls the default runloop. This will basically just keep
  // broadcasting process events to keep the game going.
  Run ();

  return true;
}
