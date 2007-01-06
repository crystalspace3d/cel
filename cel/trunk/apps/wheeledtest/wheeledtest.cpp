/*
    Copyright (C) 2005 by Jorrit Tyberghein

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
#include "wheeledtest.h"
#include "csutil/sysfunc.h"
#include "csgeom/plane3.h"
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
#include "propclass/mechsys.h"
#include "propclass/wheeled.h"

#define PATHFIND_VERBOSE 0

//-----------------------------------------------------------------------------

WheeledTest::WheeledTest ()
{
  SetApplicationName ("wheeledtest");
}

WheeledTest::~WheeledTest ()
{
}

void WheeledTest::OnExit ()
{
  if (pl) pl->CleanCache ();
  csDebuggingGraph::Dump (0);
}

void WheeledTest::ProcessFrame ()
{
  // We let the entity system do this so there is nothing here.
}

void WheeledTest::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (0);
}

bool WheeledTest::OnKeyboard (iEvent &ev)
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
      //First, give the vehicle a chance to remove itself properly.
      csRef<iCelEntity> vehicle=pl->FindEntity("vehicle");
      pl->RemoveEntity(vehicle);
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
        game = pl->FindEntity ("map");
      }
    }
  }
  return false;
}

csPtr<iCelEntity> WheeledTest::CreateVehicle (const char* name,
                                              const char* sectorname, const csVector3& pos)
{
  // The vehicle
  csRef<iCelEntity> entity_cam = pl->CreateEntity (name, bltest, "wheeled",
      "pccommandinput",
      "pcmesh",
      "pcdefaultcamera",
      "pcmechobject",
      "pcwheeled",
     // "pcmeshdeform",
      CEL_PROPCLASS_END);
  if (!entity_cam) return 0;

  csRef<iPcCommandInput> pcinp = CEL_QUERY_PROPCLASS_ENT (entity_cam,
      iPcCommandInput);
  pcinp->Bind ("up", "accelerate");
  pcinp->Bind ("down", "reverse");
  pcinp->Bind ("left", "steerleft");
  pcinp->Bind ("right", "steerright");
  pcinp->Bind ("space", "handbrake");
  pcinp->Bind ("x", "center");
  pcinp->Bind ("pgup", "lookup");
  pcinp->Bind ("pgdn", "lookdown");

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcMesh);
  pcmesh->SetMesh ("celCarBody","/cellib/objects/celcarbody");
  iSector* sector = engine->FindSector (sectorname);
  pcmesh->MoveMesh (sector, pos);

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
  pccamera->SetMinMaxCameraDistance (4.0f, 12.0f);
  pccamera->SetTurnSpeed (1.0f);
  pccamera->SetSwingCoef (0.1f);
  pccamera->SetMode (iPcDefaultCamera::freelook);
  pccamera->SetSpringParameters (3.5f, 0.25f, 0.01f);
  pccamera->SetMinMaxCameraDistance (2.0f, 16.0f);
  pccamera->SetFirstPersonOffset (csVector3 (0, 1.0f, 0));
  pccamera->SetThirdPersonOffset (csVector3 (0, 1.0f, 3.0f));
  pccamera->SetModeName ("thirdperson");

  csRef<iPcMechanicsObject> pcmech=CEL_QUERY_PROPCLASS_ENT(entity_cam,
      iPcMechanicsObject);
  //The mass of the vehicle
  pcmech->SetMass(1000.0);
  pcmech->SetDensity(1.0);
  pcmech->SetFriction(0.04f);
  pcmech->AttachColliderBoundingBox();

  csRef<iPcWheeled> pcwheeled=CEL_QUERY_PROPCLASS_ENT(entity_cam,iPcWheeled);
  pcwheeled->SetWheelMesh("celCarWheel","/cellib/objects/celcarwheel");
  //Activate this, and the vehicle will steer like a tank. Ownage!
  //pcwheeled->SetTankMode(true);

  /*This part demos how to use presets to modify the steer and drive settings
    of groups of wheels. It isn't neccessary, as the vehicle defaults to
    front-wheel steer 4-wheel-drive, but tweaking these provides an easy way
    to modify car handling.
    Settings are: steersensitivity, enginepower, suspensionsoftness,
                  suspensiondamping, friction, mass */
  pcwheeled->SetFrontWheelPreset(1.0f,0.8f, 0.000125, 0.125, 0.07f, 10.0f);

  /*By letting the rear wheels steer a small amount, and giving them most power,
    the vehicle's handling becomes much more twitchy. */
  pcwheeled->SetRearWheelPreset(0.2f, 1.0f, 0.000125, 0.125, 0.09f, 15.0f);

  //Making the outer wheels steer a wider arc also improves traction while turning.
  pcwheeled->SetOuterWheelSteerPreset(0.7f);

  pcwheeled->SetBrakeForce(1000.0f);
  pcwheeled->SetABSEnabled(true);
  pcwheeled->SetSteerAmount(0.7f);

  pcwheeled->AddWheelAuto(csVector3(-0.5,0,-0.7f));
  pcwheeled->AddWheelAuto(csVector3(0.5,0,-0.7f));
//   pcwheeled->AddWheelAuto(csVector3(-0.5,0,-0.25f));
//   pcwheeled->AddWheelAuto(csVector3(0.5,0,-0.25f));
//   pcwheeled->AddWheelAuto(csVector3(-0.5,0,0.25f));
//   pcwheeled->AddWheelAuto(csVector3(0.5,0,0.25f));
  pcwheeled->AddWheelAuto(csVector3(-0.5,0,0.7f));
  pcwheeled->AddWheelAuto(csVector3(0.5,0,0.7f));

  //Setup gear speeds. settings are gear, speed, force. note that gear -1
  //is reverse, and the speed is negative.
  pcwheeled->SetGearSettings(-1,-50,700);
  pcwheeled->SetGearSettings(1,10,3000);
  pcwheeled->SetGearSettings(2,20,2000);
  pcwheeled->SetGearSettings(3,50,700);
  pcwheeled->SetGearSettings(4,75,450);
  pcwheeled->SetGearSettings(5,100,200);

 // csRef<iPcMeshDeform> pcmeshdeform
  //  = CEL_QUERY_PROPCLASS_ENT(entity_cam,iPcMeshDeform);
  return csPtr<iCelEntity> (entity_cam);
}

bool WheeledTest::CreateMap ()
{
  csRef<iCelEntity> entity_map;
  csRef<iCelEntity> entity_dummy;

  //===============================
  // Create the map entity.
  //===============================
  entity_map = pl->CreateEntity ("ent_level", 0, 0,
                                 "pczonemanager",
                                 "pcinventory",
                                 "pcmechsys",
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
    path = "/cellib/track4";
    file = "level.xml";
  }

  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  csStringArray paths;
  paths.Push ("/cellib/lev/");
  if (!vfs->ChDirAuto (path, &paths, 0, file))
    return ReportError ("Bad file path '%s' at '%s'!", file.GetData (),
                        path.GetData ());

  csRef<iPcZoneManager> pczonemgr = CEL_QUERY_PROPCLASS_ENT (entity_map,
      iPcZoneManager);
  //I need to set loadall in order to load the map NOW, so the wheels know what sector to go to.
  pczonemgr->SetLoadingMode(CEL_ZONE_LOADALL);
  if (!pczonemgr->Load (0, file))
    return ReportError ("Error loading level '%s' at '%s'!", file.GetData (),
                        path.GetData ());

  scfString regionname, startname;
  pczonemgr->GetLastStartLocation (&regionname, &startname);

  entity_dummy = CreateVehicle ("vehicle", "track4", csVector3 (0,2.0f,1.0f));
  if (!entity_dummy) return false;

  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (entity_dummy, iPcCamera);
  if (!pccamera) return false;
  pccamera->SetZoneManager (pczonemgr, true, regionname, startname);
  if (pczonemgr->PointMesh ("dyn", regionname, startname)
      != CEL_ZONEERROR_OK)
    return ReportError ("Error finding start position!");

  csRef<iPcInventory> pcinv_map = CEL_QUERY_PROPCLASS_ENT (entity_map,
      iPcInventory);
  if (!pcinv_map->AddEntity (entity_dummy)) return false;

  csRef<iPcMechanicsSystem> pcmechsys = CEL_QUERY_PROPCLASS_ENT (entity_map,
      iPcMechanicsSystem);
  pcmechsys->EnableQuickStep ();
  pcmechsys->SetStepTime (0.01f);

  /*csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_map,
      iPcMesh);
  pcmesh->SetMesh(engine->FindMeshObject("Terrain"));

  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity_map,
      iPcMechanicsObject);
  //This is sometimes neccessary to stop the map from shifting lol
  pcmechobj->SetMass(999999999999999999.0f);
  pcmechobj->SetDensity(99999999999999999999.0f);
  pcmechobj->SetFriction(0.6f);
  pcmechobj->AttachColliderMesh ();
  pcmechobj->MakeStatic (true);*/

  game = entity_map;
  return true;
}

bool WheeledTest::OnInitialize (int argc, char* argv[])
{
  csDebuggingGraph::SetupGraph (object_reg);

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

bool WheeledTest::Application ()
{
  // Open the main system. This will open all the previously loaded plug-ins.
  // i.e. all windows will be opened.
  if (!OpenApplication (object_reg))
    return ReportError ("Error opening system!");

  // The virtual clock.
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  if (!vc) return ReportError ("Can't find the virtual clock!");
  //vc->SetClockSpeed(0.3f);
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

  bltest = csQueryRegistryTagInterface<iCelBlLayer> (
                                            object_reg, "iCelBlLayer.Test");
  if (!bltest) return ReportError ("CEL test behaviour layer missing!");
  pl->RegisterBehaviourLayer (bltest);

  if (!CreateMap ()) return false;

  // This calls the default runloop. This will basically just keep
  // broadcasting process events to keep the game going.
  Run ();

  return true;
}

