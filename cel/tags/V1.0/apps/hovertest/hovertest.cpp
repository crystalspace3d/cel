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
#include "hovertest.h"
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
#include "propclass/craft.h"
#include "propclass/defcam.h"
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "propclass/region.h"
#include "propclass/hover.h"
#include "propclass/input.h"
#include "propclass/navgraph.h"
#include "propclass/linmove.h"
#include "propclass/actormove.h"
#include "propclass/quest.h"
#include "propclass/trigger.h"
#include "propclass/zone.h"
#include "propclass/mechsys.h"
#include "propclass/stabiliser_dist.h"

#define PATHFIND_VERBOSE 0

#include "bl.h"

//-----------------------------------------------------------------------------

HoverTest::HoverTest ()
{
  SetApplicationName ("hovertest");
}

HoverTest::~HoverTest ()
{
}

void HoverTest::OnExit ()
{
  if (pl) pl->CleanCache ();
  csDebuggingGraph::Dump (0);
}

void HoverTest::ProcessFrame ()
{
  // We let the entity system do this so there is nothing here.
}

void HoverTest::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (0);
}

bool HoverTest::OnKeyboard (iEvent &ev)
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

bool HoverTest::CreatePlayer (const csVector3 &pos)
{
  // The Real Camera
  player = pl->CreateEntity ("ent_player", behaviour_layer,
        "dynactor",
        "pccommandinput",
        "pcmesh",
        "pcdefaultcamera",
        "pcmechobject",
        "pchover",
        "pccraft",
        (void*)0);
  if (!player) return false;

  csRef<iPcCommandInput> pcinp = CEL_QUERY_PROPCLASS_ENT (player,
  	iPcCommandInput);
  pcinp->Bind ("up", "up");
  pcinp->Bind ("down", "down");
  pcinp->Bind ("left", "left");
  pcinp->Bind ("right", "right");
  pcinp->Bind ("space", "jump");
  pcinp->Bind ("x", "thrust");
  pcinp->Bind ("enter", "center");
  pcinp->Bind ("pgup", "lookup");
  pcinp->Bind ("pgdn", "lookdown");

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (player, iPcMesh);
  pcmesh->SetPath ("/cellib/objects");
  pcmesh->SetMesh ("craft", "orogor");

  csRef<iPcDefaultCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (
  	player, iPcDefaultCamera);
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

  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT(player,
        iPcMechanicsObject);
  csBox3 bbox;
  pcmesh->GetMesh ()->GetMeshObject ()->GetObjectModel ()->
      GetObjectBoundingBox(bbox);
  //pcmechobj->GetBody ()->AttachColliderBox (bbox.GetSize (), csOrthoTransform (), 0.5, 3.0f, 1.0, 0.8);
  pcmechobj->SetFriction (0.05f);
  //pcmechobj->AttachColliderBox (bbox.GetSize (), csOrthoTransform ());
  pcmechobj->AttachColliderSphere (.42f, csVector3 (0, 0, 0));
  pcmechobj->SetSoftness (1.0f);
  pcmechobj->SetMass (1.0f);
  pcmechobj->SetDensity (3.0f);

  //csRef<iPcHover> pchover = CEL_QUERY_PROPCLASS_ENT (player, iPcHover);
  // defaults are fine

  csRef<iPcCraftController> pccraft = CEL_QUERY_PROPCLASS_ENT (player,
        iPcCraftController);
  pccraft->SetAccTurn (0.4f);
  pccraft->SetMaxTurn (1.5);
  pccraft->SetAccPitch (0.4f);
  pccraft->SetMaxPitch (0.5);
  pccraft->SetThrustForce (10.0);
  pccraft->SetTopSpeed (20.0);
  pccraft->SetRedirectVelocityRatio (0.2f);

  return true;
}

bool HoverTest::CreateRoom ()
{
  //===============================
  // Create the room entity.
  //===============================
  level = pl->CreateEntity ("ent_level", 0, 0,
        "pczonemanager",
        "pcinventory",
        "pcmechsys",
	(void*)0);

  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
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
    path = "/cellib/track4";
    file = "level.xml";
    terrain = "TerrainFact";
    terrainfile = "world";
  }

  csRef<iVFS> vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  if (!vfs->ChDirAuto (path, 0, 0, file))
    return ReportError ("Bad file path '%s' at '%s'!", file.GetData (),
    	path.GetData ());

  csRef<iPcZoneManager> pczonemgr = CEL_QUERY_PROPCLASS_ENT (level,
  	iPcZoneManager);
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

  csRef<iPcMechanicsSystem> pcmechsys = CEL_QUERY_PROPCLASS_ENT (level,
  	iPcMechanicsSystem);
  pcmechsys->EnableQuickStep ();
  pcmechsys->SetStepTime (0.02f);

  if (!CreatePlayer (csVector3 (0, 0, 0)))
    return ReportError ("Could not create entity 'ent_player'!");

  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (player, iPcCamera);
  if (!pccamera) return false;
  pccamera->SetZoneManager (pczonemgr, true, regionname, startname);
  if (pczonemgr->PointMesh ("ent_player", regionname, startname) != CEL_ZONEERROR_OK)
    return ReportError ("Error finding start position!");

  csRef<iPcInventory> pcinv_room = CEL_QUERY_PROPCLASS_ENT (level,
  	iPcInventory);
  if (!pcinv_room->AddEntity (player)) return false;
  //if (!pcinv_room->AddEntity (scene)) return false;

    csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT(player,
          iPcMechanicsObject);
    // Get the first start position available.
    // \todo manage multiple start positions.
    iCameraPosition* campos;
    campos = engine->GetCameraPositions ()->Get (0);
    csVector3 up (campos->GetUpwardVector ());
    csVector3 fw (campos->GetForwardVector ());
    csVector3 cr;
    cr.Cross (up, fw);
    csMatrix3 mat (cr.x, cr.y, cr.z, up.x, up.y, up.z, fw.x, fw.y, fw.z);
    pcmechobj->GetBody ()->SetOrientation (mat);
    pcmechobj->GetBody ()->SetPosition (campos->GetPosition ());

  return true;
}

bool HoverTest::OnInitialize (int argc, char* argv[])
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
      CS_REQUEST_PLUGIN ("cel.behaviourlayer.test:iCelBlLayer.Test",
          iCelBlLayer),
      CS_REQUEST_PLUGIN ("cel.persistence.xml", iCelPersistence),
      CS_REQUEST_PLUGIN ("cel.manager.quests", iQuestManager),
      CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.opcode",
          iCollideSystem),
      CS_REQUEST_END))
    return ReportError("Failed to initialize plugins!");

  // "Warm up" the event handler so it can interact with the world
  csBaseEventHandler::Initialize (r);

  // Set up an event handler for the application.  Crystal Space is fully
  // event-driven.  Everything (except for this initialization) happens in
  // response to an event.
  if (!RegisterQueue(r, csevAllEvents(r)))
    return ReportError("Failed to set up event handler!");

  return true;
}

bool HoverTest::Application()
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

  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl)
    return ReportError ("Can't find the CEL physical layer!");

  vfs = CS_QUERY_REGISTRY(r, iVFS);

  #define LOAD_PROPERTY_CLASS(class)  if(!pl->LoadPropertyClassFactory(class))\
                                        return ReportError("error loading property class "\
                                        class "!");
  LOAD_PROPERTY_CLASS("cel.pcfactory.test");
  LOAD_PROPERTY_CLASS("cel.pcfactory.linmove");
  //LOAD_PROPERTY_CLASS("cel.pcfactory.actormove");
  //LOAD_PROPERTY_CLASS("cel.pcfactory.solid");
  LOAD_PROPERTY_CLASS("cel.pcfactory.colldet");
  LOAD_PROPERTY_CLASS("cel.pcfactory.region");
  LOAD_PROPERTY_CLASS("cel.pcfactory.zonemanager");
  LOAD_PROPERTY_CLASS("cel.pcfactory.defaultcamera");
  //LOAD_PROPERTY_CLASS("cel.pcfactory.tooltip");
  LOAD_PROPERTY_CLASS("cel.pcfactory.timer");
  LOAD_PROPERTY_CLASS("cel.pcfactory.inventory");
  //LOAD_PROPERTY_CLASS("cel.pcfactory.characteristics");
  LOAD_PROPERTY_CLASS("cel.pcfactory.mesh");
  LOAD_PROPERTY_CLASS("cel.pcfactory.light");
  LOAD_PROPERTY_CLASS("cel.pcfactory.portal");
  LOAD_PROPERTY_CLASS("cel.pcfactory.meshselect");
  LOAD_PROPERTY_CLASS("cel.pcfactory.pccommandinput");
  LOAD_PROPERTY_CLASS("cel.pcfactory.quest");
  LOAD_PROPERTY_CLASS("cel.pcfactory.properties");
  LOAD_PROPERTY_CLASS("cel.pcfactory.trigger");
  LOAD_PROPERTY_CLASS("cel.pcfactory.billboard");
  //LOAD_PROPERTY_CLASS("cel.pcfactory.graph");
  //LOAD_PROPERTY_CLASS("cel.pcfactory.link");
  //LOAD_PROPERTY_CLASS("cel.pcfactory.node");
  //LOAD_PROPERTY_CLASS("cel.pcfactory.navgraphrules");
  //LOAD_PROPERTY_CLASS("cel.pcfactory.navgraphrulesfps");
  LOAD_PROPERTY_CLASS("cel.pcfactory.mechsys");
  LOAD_PROPERTY_CLASS("cel.pcfactory.mechobject");
  LOAD_PROPERTY_CLASS("cel.pcfactory.hover");
  LOAD_PROPERTY_CLASS("cel.pcfactory.craft");

  behaviour_layer.AttachNew (new htBehaviourLayer (this));
  pl->RegisterBehaviourLayer (behaviour_layer);
  if (!r->Register (behaviour_layer , "iCelBlLayer"))
    return ReportError ("unable to register behaviour layer!");

  if (!CreateRoom ()) return false;

  // Start the default run/event loop.  This will return only when some code,
  // such as OnKeyboard(), has asked the run loop to terminate.
  Run();

  return true;
}
