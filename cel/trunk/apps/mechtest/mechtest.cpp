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
#include "mechtest.h"
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

#define PATHFIND_VERBOSE 0

//-----------------------------------------------------------------------------

MechTest::MechTest ()
{
  SetApplicationName ("mechtest");
}

MechTest::~MechTest ()
{
}

void MechTest::OnExit ()
{
  if (pl) pl->CleanCache ();
  csDebuggingGraph::Dump (0);
}

void MechTest::ProcessFrame ()
{
  // We let the entity system do this so there is nothing here.
}

void MechTest::FinishFrame ()
{
  g3d->FinishDraw ();
  g3d->Print (0);
}

bool MechTest::OnKeyboard (iEvent &ev)
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
    else if (code == 's')
    {
      csRef<iCelPersistence> p = CS_QUERY_REGISTRY (object_reg,
      	iCelPersistence);
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
      csRef<iCelPersistence> p = CS_QUERY_REGISTRY (object_reg,
      	iCelPersistence);
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

csPtr<iCelEntity> MechTest::CreateDynSphere (const char* name,
	const char* roomname, const csVector3& pos)
{
  csRef<iCelEntity> entity_cam = pl->CreateEntity (name, bltest, 0,
	"pcmesh",
	"pcmechobject",
	(void*)0);
  if (!entity_cam) return 0;

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcMesh);
  pcmesh->SetPath ("/cel/data");
  pcmesh->SetMesh ("Sphere", "sphere");
  iSector* room = engine->FindSector (roomname);
  pcmesh->MoveMesh (room, pos);

  csRef<iPcMechanicsObject> pcmechobject = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcMechanicsObject);
  pcmechobject->AttachColliderSphere (.42f, csVector3 (0, 0, 0));

  return csPtr<iCelEntity> (entity_cam);
}

csPtr<iCelEntity> MechTest::CreateDynBox (const char* name,
	const char* roomname, const csVector3& pos)
{
  csRef<iCelEntity> entity_cam = pl->CreateEntity (name, bltest, 0,
	"pcmesh",
	"pcmechobject",
	(void*)0);
  if (!entity_cam) return 0;

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcMesh);
  pcmesh->SetPath ("/cel/data");
  pcmesh->SetMesh ("gmbox", "gmbox");
  iSector* room = engine->FindSector (roomname);
  pcmesh->MoveMesh (room, pos);

  csRef<iPcMechanicsObject> pcmechobject = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcMechanicsObject);
  pcmechobject->AttachColliderBox (csVector3 (.4f, .3f, .3f), csOrthoTransform ());

  return csPtr<iCelEntity> (entity_cam);
}

csPtr<iCelEntity> MechTest::CreateDynActor (const char* name,
	const char* roomname, const csVector3& pos)
{
  // The Real Camera
  csRef<iCelEntity> entity_cam = pl->CreateEntity (name, bltest, "dynactor",
  	"pccommandinput",
	"pcmesh",
	"pcdefaultcamera",
	"pcmechobject",
	(void*)0);
  if (!entity_cam) return 0;

  csRef<iPcCommandInput> pcinp = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcCommandInput);
  pcinp->Bind ("up", "forward");
  pcinp->Bind ("down", "backward");
  pcinp->Bind ("left", "strafeleft");
  pcinp->Bind ("right", "straferight");
  pcinp->Bind ("space", "jump");
  pcinp->Bind ("x", "center");
  pcinp->Bind ("pgup", "lookup");
  pcinp->Bind ("pgdn", "lookdown");

  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity_cam, iPcMesh);
  pcmesh->SetPath ("/cel/data");
  pcmesh->SetMesh ("Sphere", "sphere");
  iSector* room = engine->FindSector (roomname);
  pcmesh->MoveMesh (room, pos);

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
  pccamera->SetModeName ("lara_thirdperson");

  csRef<iPcMechanicsObject> pcmechobject = CEL_QUERY_PROPCLASS_ENT (entity_cam,
  	iPcMechanicsObject);
  pcmechobject->AttachColliderSphere (.42f, csVector3 (0, 0, 0));
  pcmechobject->SetMass (1.0);
  pcmechobject->SetDensity (3.0);

  return csPtr<iCelEntity> (entity_cam);
}

bool MechTest::CreateRoom ()
{
  csRef<iCelEntity> entity_room;
  csRef<iCelEntity> entity_dummy;

  //===============================
  // Create the room entity.
  //===============================
  entity_room = pl->CreateEntity ("room", bltest, "room",
  	"pczonemanager",
	"pcinventory",
	"pcmechsys",
	"pcmechobject",
  	(void*)0);

  //===============================
  // Engine init.
  //===============================
  engine->Prepare ();

  csRef<iCommandLineParser> cmdline = CS_QUERY_REGISTRY (object_reg,
  	iCommandLineParser);
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
    file = "mech_level.xml";
  }

  csRef<iVFS> vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
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

  entity_dummy = CreateDynActor ("dyn", "room", csVector3 (0,2.0f,1.0f));
  if (!entity_dummy) return false;

  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (entity_dummy, iPcCamera);
  if (!pccamera) return false;
  pccamera->SetZoneManager (pczonemgr, true, regionname, startname);
  if (pczonemgr->PointMesh ("dyn", regionname, startname)
  	!= CEL_ZONEERROR_OK)
    return ReportError ("Error finding start position!");

  csRef<iPcInventory> pcinv_room = CEL_QUERY_PROPCLASS_ENT (entity_room,
  	iPcInventory);
  if (!pcinv_room->AddEntity (entity_dummy)) return false;

  csRef<iPcMechanicsSystem> pcmechsys = CEL_QUERY_PROPCLASS_ENT (entity_room,
  	iPcMechanicsSystem);
  pcmechsys->EnableQuickStep ();
  pcmechsys->SetStepTime (0.02f);

  csRef<iPcMechanicsObject> pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity_room,
  	iPcMechanicsObject);
  csPlane3 ground (0, -1, 0, -1);
  pcmechobj->AttachColliderPlane (ground);
  pcmechobj->MakeStatic (true);

  game = entity_room;

  entity_dummy = CreateDynSphere ("sphere1", "room", csVector3 (.5f,0, -8.0f));
  entity_dummy = CreateDynSphere ("sphere2", "room", csVector3 (0,0,-7.0f));
  entity_dummy = CreateDynSphere ("sphere3", "room", csVector3 (-.5f,0,-8.0f));
  entity_dummy = CreateDynSphere ("sphere4", "room", csVector3 (0,.5f,-7.5f));

  entity_dummy = CreateDynBox ("box1", "room", csVector3 (0.0f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box2", "room", csVector3 (0.5f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.0f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.5f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.0f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.5f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (3.0f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box2", "room", csVector3 (3.5f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (4.0f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (4.5f,-.8f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (5.0f,-.8f,-10.0f));

  entity_dummy = CreateDynBox ("box1", "room", csVector3 (0.25f,-.49f,-10.0f));
  entity_dummy = CreateDynBox ("box2", "room", csVector3 (0.75f,-.49f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.25f,-.49f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.75f,-.49f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.25f,-.49f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.75f,-.49f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (3.25f,-.49f,-10.0f));
  entity_dummy = CreateDynBox ("box2", "room", csVector3 (3.75f,-.49f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (4.25f,-.49f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (4.75f,-.49f,-10.0f));

  entity_dummy = CreateDynBox ("box2", "room", csVector3 (0.5f,-.18f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.0f,-.18f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.5f,-.18f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.0f,-.18f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.5f,-.18f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (3.0f,-.18f,-10.0f));
  entity_dummy = CreateDynBox ("box2", "room", csVector3 (3.5f,-.18f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (4.0f,-.18f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (4.5f,-.18f,-10.0f));

  entity_dummy = CreateDynBox ("box2", "room", csVector3 (0.75f,.13f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.25f,.13f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.75f,.13f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.25f,.13f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.75f,.13f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (3.25f,.13f,-10.0f));
  entity_dummy = CreateDynBox ("box2", "room", csVector3 (3.75f,.13f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (4.25f,.13f,-10.0f));

  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.0f,.44f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.5f,.44f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.0f,.44f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.5f,.44f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (3.0f,.44f,-10.0f));
  entity_dummy = CreateDynBox ("box2", "room", csVector3 (3.5f,.44f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (4.0f,.44f,-10.0f));

  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.25f,.75f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.75f,.75f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.25f,.75f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.75f,.75f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (3.25f,.75f,-10.0f));
  entity_dummy = CreateDynBox ("box2", "room", csVector3 (3.75f,.75f,-10.0f));

  entity_dummy = CreateDynBox ("box3", "room", csVector3 (1.5f,1.06f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.0f,1.06f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (2.5f,1.06f,-10.0f));
  entity_dummy = CreateDynBox ("box3", "room", csVector3 (3.0f,1.06f,-10.0f));
  entity_dummy = CreateDynBox ("box2", "room", csVector3 (3.5f,1.06f,-10.0f));

  return true;
}

bool MechTest::OnInitialize (int argc, char* argv[])
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

  // Now we need to setup an event handler for our application.
  // Crystal Space is fully event-driven. Everything (except for this
  // initialization) happens in an event.
  if (!RegisterQueue (object_reg))
    return ReportError ("Can't setup event handler!");
  return true;
}

bool MechTest::Application ()
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
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.portal"))
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

  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.mechsys"))
    return false;
  if (!pl->LoadPropertyClassFactory ("cel.pcfactory.mechobject"))
    return false;

  if (!CreateRoom ()) return false;

  // This calls the default runloop. This will basically just keep
  // broadcasting process events to keep the game going.
  Run ();

  return true;
}

