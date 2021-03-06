/*
    Copyright (C) 2006 by Jorrit Tyberghein

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
#include "evolvecraft.h"
#include "csutil/sysfunc.h"
#include "csgeom/plane3.h"
#include "iutil/vfs.h"
#include "iutil/object.h"
#include "iutil/cmdline.h"
#include "csutil/cscolor.h"
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
#include "ivaria/dynamics.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/txtmgr.h"
#include "ivideo/texture.h"
#include "ivideo/material.h"
#include "ivideo/fontserv.h"
#include "igraphic/imageio.h"
#include "imesh/objmodel.h"
#include "imap/loader.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "ivaria/collider.h"
#include "csutil/cmdhelp.h"
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
#include "propclass/craft.h"
#include "propclass/defcam.h"
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "propclass/region.h"
#include "propclass/hover.h"
#include "propclass/input.h"
#include "propclass/linmove.h"
#include "propclass/actormove.h"
#include "propclass/quest.h"
#include "propclass/trigger.h"
#include "propclass/zone.h"
#include "propclass/mechsys.h"
#include "propclass/neuralnet.h"
#include "propclass/evolve.h"
#include "propclass/timer.h"

#define PATHFIND_VERBOSE 0

#include "bl.h"

//-----------------------------------------------------------------------------

EvolveCraft::EvolveCraft ()
{
  SetApplicationName ("evolvecraft");
}

EvolveCraft::~EvolveCraft ()
{
}

void EvolveCraft::OnExit ()
{
  if (pl) pl->CleanCache ();
  
  printer.Invalidate ();
}

void EvolveCraft::Frame ()
{
  // We let the entity system do this so there is nothing here.
}

bool EvolveCraft::OnKeyboard (iEvent &ev)
{
  // We got a keyboard event.
  if (csKeyEventHelper::GetEventType(&ev) == csKeyEventTypeDown)
  {
    // The user pressed a key (as opposed to releasing it).
    utf32_char code = csKeyEventHelper::GetCookedCode(&ev);
    if (code == CSKEY_ESC)
    {
      // We were in menu so terminate the application.  The proper way
      // to terminate a Crystal Space application is by broadcasting a
      // cscmdQuit event.  That will cause the main run loop to stop.  To do
      // so we retrieve the event queue from the object registry and then
      // post the event.
      csRef<iEventQueue> q =
        csQueryRegistry<iEventQueue> (GetObjectRegistry());
      if (q.IsValid())
        q->GetEventOutlet()->Broadcast(csevQuit(GetObjectRegistry ()));
    }
  }

  return false;
}

bool EvolveCraft::CreatePlayer (const csVector3 &pos)
{
  // The Real Camera
  player = pl->CreateEntity ("ent_player", behaviour_layer,
        "dynactor",
	"pctools.timer",
	"pcinput.standard",
	"pcai.neuralnet",
        "pcobject.mesh",
        "pccamera.old",
        "pcphysics.object",
        "pcvehicle.hover",
        "pcvehicle.craft",
	"pcmove.actor.hovercraft",
        (void*)0);
  if (!player) return false;

  csRef<iPcCommandInput> pcinput = celQueryPropertyClassEntity<iPcCommandInput> (player);
  pcinput->EnableKeyboardEvents();
  pcinput->Bind("space", "unfit");

  csRef<iPcNeuralNet> pcnn = celQueryPropertyClassEntity<iPcNeuralNet> (player);
  pcnn->SetSize(6, 1, 3); // 6 inputs, 1 output, 3 hidden layers
  pcnn->SetComplexity("linear");
  pcnn->SetActivationFunc(csRef<celNNActivationFunc> (csPtr<celNNActivationFunc>
	(new celTanhActivationFunc<float>))); // cel.activationFunc.float.tanh
  pcnn->Validate(); // initialize the pcneuralnet
  pcnn->LoadCachedWeights("evolvecraft", ~0); // load genome from cache

  if (doEvolve) // the -evolve commandline option was used
  {
    iCelPropertyClass *pc = pl->CreatePropertyClass(player, "pcai.evolve");
    csRef<iPcEvolve> pcevolve = scfQueryInterface<iPcEvolve>(pc);
    if (!pcevolve) return false;

    pcevolve->SetPopulationSize(50);
    pcevolve->SetSubject(csRef<iCelPropertyClass>
	(scfQueryInterface<iCelPropertyClass> (pcnn)));
    pcevolve->SetProbabilities(0.25, 2.0);
  }

  csRef<iPcTimer> pctimer = celQueryPropertyClassEntity<iPcTimer> (player);
  pctimer->WakeUpFrame(0);
  pctimer->WakeUp(1000, true);

  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (player);
  pcmesh->SetPath ("/cellib/objects");
  pcmesh->SetMesh ("craft", "orogor");

  csRef<iPcDefaultCamera> pccamera = celQueryPropertyClassEntity<iPcDefaultCamera> (player);
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
  pccamera->SetModeName ("lara_thirdperson");

  csRef<iPcMechanicsObject> pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (player);
  //csBox3 bbox = pcmesh->GetMesh ()->GetMeshObject ()->GetObjectModel ()->GetObjectBoundingBox();
  //pcmechobj->GetBody ()->AttachColliderBox (bbox.GetSize (), csOrthoTransform (), 0.5, 3.0f, 1.0, 0.8);
  pcmechobj->SetFriction (0.05f);
  //pcmechobj->AttachColliderBox (bbox.GetSize (), csOrthoTransform ());
  pcmechobj->AttachColliderSphere (.42f, csVector3 (0, 0, 0));
  pcmechobj->SetSoftness (1.0f);
  pcmechobj->SetMass (1.0f);
  pcmechobj->SetDensity (3.0f);

  csRef<iPcCraftController> pccraft = celQueryPropertyClassEntity<iPcCraftController> (player);
  pccraft->SetAccTurn (0.4f);
  pccraft->SetMaxTurn (1.5);
  pccraft->SetAccPitch (0.4f);
  pccraft->SetMaxPitch (0.5);
  pccraft->SetThrustForce (10.0);
  pccraft->SetTopSpeed (20.0);
  pccraft->SetRedirectVelocityRatio (0.2f);

  return true;
}

bool EvolveCraft::CreateRoom ()
{
  //===============================
  // Create the room entity.
  //===============================
  level = pl->CreateEntity ("ent_level", 0, 0,
        "pcworld.zonemanager",
        "pctools.inventory",
        "pcphysics.system",
	(void*)0);

  csRef<iCommandLineParser> cmdline = 
  	csQueryRegistry<iCommandLineParser> (object_reg);
  csString path, file, terrain, terrainfile;
  path = cmdline->GetName (0);
  if (!path.IsEmpty ())
  {
    file = cmdline->GetName (1);
    if (file.IsEmpty ()) file = "level.xml";
    terrain = cmdline->GetName (2);
    if (terrain.IsEmpty ()) terrain = "TerrainFact";
    terrainfile = cmdline->GetName (3);
    if (terrainfile.IsEmpty ()) terrain = "world";
  }
  else
  {
    path = "/cellib/track";
    file = "level.xml";
    terrain = "TerrainFact";
    terrainfile = "world";
  }

  doEvolve = cmdline->GetBoolOption("evolve");
  if (!doEvolve)
    ReportInfo ("Pass the -evolve commandline option to use the pcevolve.");

  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  if (!vfs->ChDirAuto (path, 0, 0, file))
    return ReportError ("Bad file path '%s' at '%s'!", file.GetData (),
    	path.GetData ());

  csRef<iPcZoneManager> pczonemgr = celQueryPropertyClassEntity<iPcZoneManager> (level);
  pczonemgr->SetLoadingMode (CEL_ZONE_NORMAL);
  if (!pczonemgr->Load (0, file.GetData ()))
    return ReportError ("Error loading level '%s' at '%s'!", file.GetData (),
    	path.GetData ());

  scfString regionname, startname;
  pczonemgr->GetLastStartLocation (&regionname, &startname);
  //regionname = "main";
  //startname = "Camera";
  printf("Start position in region '%s', named '%s'\n",
        (const char*)regionname, (const char*)startname);

  csRef<iPcMechanicsSystem> pcmechsys = celQueryPropertyClassEntity<iPcMechanicsSystem> (level);
  pcmechsys->EnableQuickStep ();
  pcmechsys->SetStepTime (0.02f);

  if (!CreatePlayer (csVector3 (0, 0, 0)))
    return ReportError ("Could not create entity 'ent_player'!");

  csRef<iPcCamera> pccamera = celQueryPropertyClassEntity<iPcCamera> (player);
  if (!pccamera) return false;
  pccamera->SetZoneManager (pczonemgr, true, regionname, startname);
  if (pczonemgr->PointMesh ("ent_player", regionname, startname) != CEL_ZONEERROR_OK)
    return ReportError ("Error finding start position!");

  csRef<iPcInventory> pcinv_room = celQueryPropertyClassEntity<iPcInventory> (level);
  if (!pcinv_room->AddEntity (player)) return false;
  //if (!pcinv_room->AddEntity (scene)) return false;

   csRef<iPcMechanicsObject> pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (player);
   // Get the first start position available.
   iCameraPosition* campos;
   campos = engine->GetCameraPositions ()->Get (0);
   csVector3 up (campos->GetUpwardVector ());
   csVector3 fw (campos->GetForwardVector ());
   csVector3 cr;
   cr.Cross (up, fw);
   csMatrix3 mat (cr.x, cr.y, cr.z, up.x, up.y, up.z, fw.x, fw.y, fw.z);
   pcmechobj->GetBody ()->SetOrientation (mat);
   pcmechobj->GetBody ()->SetPosition (campos->GetPosition ()+up);

  return true;
}

bool EvolveCraft::OnInitialize (int argc, char* argv[])
{
  iObjectRegistry* r = GetObjectRegistry();

  // RequestPlugins() will load all plugins we specify.  In addition it will
  // also check if there are plugins that need to be loaded from the
  // configuration system (both the application configuration and CS or global
  // configurations).  It also supports specifying plugins on the command line
  // via the --plugin= option.
  if (!celInitializer::RequestPlugins(r,
      CS_REQUEST_VFS,
      CS_REQUEST_OPENGL3D,
      CS_REQUEST_ENGINE,
      //CS_REQUEST_CONSOLEOUT,
      CS_REQUEST_FONTSERVER,
      CS_REQUEST_IMAGELOADER,
      CS_REQUEST_LEVELLOADER,
      CS_REQUEST_REPORTER,
      CS_REQUEST_REPORTERLISTENER,
      CS_REQUEST_PLUGIN ("cel.physicallayer", iCelPlLayer),
      CS_REQUEST_PLUGIN ("cel.manager.quests", iQuestManager),
      CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.opcode",
          iCollideSystem),
      CS_REQUEST_PLUGIN ("crystalspace.device.joystick", iEventPlug),
      CS_REQUEST_END))
    return ReportError("Failed to initialize plugins!");

  // Attempt to load a joystick plugin.
  /*csRef<iStringArray> joystickClasses =
    iSCF::SCF->QueryClassList ("crystalspace.device.joystick.");
  if (joystickClasses.IsValid())
  {
    csRef<iPluginManager> plugmgr = 
      csQueryRegistry<iPluginManager> (object_reg);
    for (size_t i = 0; i < joystickClasses->GetSize (); i++)
    {
      const char* className = joystickClasses->Get (i);
      iBase* b = plugmgr->LoadPlugin (className);

      csReport (object_reg, CS_REPORTER_SEVERITY_NOTIFY,
	"crystalspace.application.joytest", "Attempt to load plugin '%s' %s",
	className, (b != 0) ? "successful" : "failed");
      if (b != 0) b->DecRef ();
    }
  }*/

  // "Warm up" the event handler so it can interact with the world
  csBaseEventHandler::Initialize (r);

  // Set up an event handler for the application.  Crystal Space is fully
  // event-driven.  Everything (except for this initialization) happens in
  // response to an event.
  if (!RegisterQueue(r, csevAllEvents(r)))
    return ReportError("Failed to set up event handler!");

  return true;
}

bool EvolveCraft::Application()
{
  iObjectRegistry* r = GetObjectRegistry();

  // Open the main system. This will open all the previously loaded plugins
  // (i.e. all windows will be opened).
  if (!OpenApplication(r))
    return ReportError("Error opening system!");

  // Now get the pointer to various modules we need.  We fetch them from the
  // object registry.  The RequestPlugins() call we did earlier registered all
  // loaded plugins with the object registry.  It is also possible to load
  // plugins manually on-demand.

  #define LOAD_PLUGIN(var,plugin)  var = csQueryRegistry<plugin> (r);\
                                     if(!var)  return ReportError\
                                     ("could not find the " #plugin " !");
  LOAD_PLUGIN(vc, iVirtualClock);
  LOAD_PLUGIN(engine, iEngine);
  LOAD_PLUGIN(loader, iLoader);
  LOAD_PLUGIN(g3d, iGraphics3D);
  LOAD_PLUGIN(kbd, iKeyboardDriver);
  LOAD_PLUGIN(pl,  iCelPlLayer);

  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl)
    return ReportError ("Can't find the CEL physical layer!");

  vfs = csQueryRegistry<iVFS> (r);

  behaviour_layer.AttachNew (new htBehaviourLayer (this, pl));
  pl->RegisterBehaviourLayer (behaviour_layer);
  if (!r->Register (behaviour_layer , "iCelBlLayer"))
    return ReportError ("unable to register behaviour layer!");

  if (!CreateRoom ()) return false;

  printer.AttachNew (new FramePrinter (object_reg));

  // Start the default run/event loop.  This will return only when some code,
  // such as OnKeyboard(), has asked the run loop to terminate.
  Run();

  return true;
}
