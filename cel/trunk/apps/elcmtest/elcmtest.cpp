#include <crystalspace.h>

#include <celtool/initapp.h>
#include <propclass/zone.h>
#include <propclass/camera.h>
#include <propclass/mesh.h>
#include <propclass/linmove.h>
#include <propclass/actormove.h>
#include <propclass/input.h>
#include "propclass/delegcam.h"
#include "propclass/cameras/tracking.h"
#include "propclass/jump.h"
#include "propclass/inv.h"
#include <physicallayer/propclas.h>
#include <behaviourlayer/behave.h>
#include <celtool/stdparams.h>

#include "elcmtest.h"

//-----------------------------------------------------------------------

void CeguiPrinter::TickEveryFrame ()
{
  parent->GetG3D ()->BeginDraw (CSDRAW_2DGRAPHICS);
  parent->GetCEGUI ()->Render ();
}

//-----------------------------------------------------------------------

class ElcmMessageReceiver : public scfImplementation1<ElcmMessageReceiver,
  iMessageReceiver>
{
private:
  ElcmTest* elcmTest;

public:
  ElcmMessageReceiver (ElcmTest* elcmTest) :
    scfImplementationType (this), elcmTest (elcmTest)
  {
  }

  virtual ~ElcmMessageReceiver () { }

  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
  {
    return elcmTest->ReceiveMessage (msg_id, sender, ret, params);
  }
};

//-----------------------------------------------------------------------

ElcmTest::ElcmTest ()
{
  SetApplicationName ("ELCM Test");
}

ElcmTest::~ElcmTest ()
{
}

bool ElcmTest::InitWindowSystem ()
{
  cegui = csQueryRegistry<iCEGUI> (GetObjectRegistry ());
  if (!cegui) return ReportError ("Failed to locate CEGUI plugin!");

  cegui->Initialize ();
  vfs->ChDir ("/cegui/");

  cegui->GetSchemeManagerPtr ()->create("ice.scheme");
  cegui->GetSystemPtr ()->setDefaultMouseCursor("ice", "MouseArrow");
  cegui->GetFontManagerPtr ()->createFreeTypeFont("DejaVuSans", 10, true, "/fonts/ttf/DejaVuSans.ttf");

  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();

  vfs->ChDir ("/cellib/ui/");
  cegui->GetSystemPtr ()->setGUISheet(winMgr->loadWindowLayout("ui.layout"));

  uiInventory = csQueryRegistry<iUIInventory> (GetObjectRegistry ());
  if (!uiInventory) return ReportError ("Failed to locate UI Inventory plugin!");

  return true;
}

bool ElcmTest::InitPhysics ()
{
  dyn = csQueryRegistry<iDynamics> (GetObjectRegistry ());
  if (!dyn) return ReportError ("Error loading bullet plugin!");

  dynSys = dyn->CreateSystem ();
  if (!dynSys) return ReportError ("Error creating dynamic system!");
  //dynSys->SetLinearDampener(.3f);
  dynSys->SetRollingDampener(.995f);
  dynSys->SetGravity (csVector3 (0.0f, -9.81f, 0.0f));

  bullet_dynSys = scfQueryInterface<CS::Physics::Bullet::iDynamicSystem> (dynSys);
  bullet_dynSys->SetInternalScale (1.0f);
  bullet_dynSys->SetStepParameters (0.005f, 2, 10);

  return true;
}

bool ElcmTest::CreateLevel ()
{
  worldEntity = pl->CreateEntity ("world", 0, 0,
      "pcworld.dynamic", CEL_PROPCLASS_END);
  if (!worldEntity)
    return ReportError ("Error creating world entity!");

  receiver.AttachNew (new ElcmMessageReceiver (this));
  worldEntity->QueryMessageChannel ()->Subscribe (receiver, "elcm.");

  sector = engine->CreateSector ("room");

  using namespace CS::Geometry;
  TesselatedQuad quad (
      csVector3 (-5000, -1, -5000),
      csVector3 (-5000, -1, 5000),
      csVector3 (5000, -1, -5000));

  if (!loader->LoadTexture ("stone", "/lib/std/stone4.gif"))
    return ReportError ("Error loading %s texture!",
		 CS::Quote::Single ("stone4"));
  iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName ("stone");
  DensityTextureMapper mapper (0.3f);
  quad.SetLevel (3);
  quad.SetMapper (&mapper);

  csRef<iMeshWrapper> floor = GeneralMeshBuilder::CreateFactoryAndMesh (engine,
      sector, "Floor", "Floor", &quad);
  floor->GetMeshObject ()->SetMaterialWrapper (tm);
  csRef<iRigidBody> body = dynSys->CreateBody ();
  csRef<CS::Physics::Bullet::iRigidBody> csBody = scfQueryInterface<CS::Physics::Bullet::iRigidBody> (body);
  csBody->SetLinearDampener (0.0f);
  csBody->SetRollingDampener (0.0f);

  body->AdjustTotalMass (1000.0f);
  //body->SetTransform (trans);
  body->AttachMesh (floor);
  body->AttachColliderBox (csVector3 (10000, 10, 10000),
      csOrthoTransform (csMatrix3 (), csVector3 (0, -6, 0)), 10, 1, 0);
  body->MakeStatic ();

  iLightList* ll = sector->GetLights ();
  csRef<iLight> light;
  light = engine->CreateLight (0, csVector3 (0, 200, 0), 10000, csColor (1, 1, 1));
  ll->Add (light);

  dynworld = celQueryPropertyClassEntity<iPcDynamicWorld> (worldEntity);
  dynworld->SetRadius (30);
  dynworld->Setup (sector, dynSys);
  dynworld->SetELCM (elcm);

  csColliderHelper::InitializeCollisionWrappers (cdsys, engine);
  engine->Prepare ();

  return true;
}

bool ElcmTest::CreateFactories ()
{
  using namespace CS::Geometry;
  TesselatedBox box (csVector3 (-.3, 0, -.3), csVector3 (.3, .6, .3));

  if (!loader->LoadTexture ("wood", "/lib/stdtex/parket.jpg"))
    return ReportError ("Error loading %s texture!",
		 CS::Quote::Single ("parket"));
  iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName ("wood");
  DensityTextureMapper mapper (0.3f);
  box.SetLevel (3);
  box.SetMapper (&mapper);

  csRef<iMeshFactoryWrapper> boxFactory = GeneralMeshBuilder::CreateFactory (
      engine, "Box", &box);
  boxFactory->GetMeshObjectFactory ()->SetMaterialWrapper (tm);

  if (!loader->LoadLibraryFile ("/cellib/lev/elcmtest.xml"))
    return false;

  return true;
}

#define SIZE 250
//#define SIZE 1

bool ElcmTest::FillDynamicWorld ()
{
  iDynamicFactory* boxFact = dynworld->AddFactory ("Box", 1.0, -1.0f);
  boxFact->AddRigidBox (csVector3 (0, .3, 0), csVector3 (.6, .6, .6), 1.0);
  csMatrix3 matId;
  iDynamicObject* obj;
  for (int y = -SIZE ; y <= SIZE ; y++)
    for (int x = -SIZE ; x <= SIZE ; x++)
    {
      obj = dynworld->AddObject ("Box", csReversibleTransform (
	    matId, csVector3 (float (x*5), 0, float (y*5))));
      csString name;
      name.Format ("box%d_%d", x+50, y+50);
      csRef<iCelParameterBlock> params;
      params.AttachNew (new celVariableParameterBlock ());
      if (!obj->SetEntityTemplate ("glowBox", name, params))
	return ReportError ("Could not set entity template 'glowBox'!");
    }
  return true;
}

bool ElcmTest::CreatePlayer ()
{
  playerEntity = pl->CreateEntity ("player", 0, 0,
    "pcinput.standard",
    "pctools.inventory",
    "pcmove.analogmotion",
    "pcmove.jump",
    "pcmove.grab",
    "pccamera.delegate",
    "pccamera.mode.tracking",
    "pcobject.mesh",
    "pcmove.linear",
    "pcmove.actor.wasd",
    CEL_PROPCLASS_END);
  if (!playerEntity)
    return ReportError ("Error creating player entity!");

  // Get the iPcCamera interface so that we can set the camera.
  csRef<iPcCamera> pccamera = celQueryPropertyClassEntity<iPcCamera> (playerEntity);
  camera = pccamera->GetCamera ();

  csRef<iPcTrackingCamera> trackcam = celQueryPropertyClassEntity<iPcTrackingCamera> (playerEntity);
  trackcam->SetPanSpeed (2.5f);
  trackcam->SetTiltSpeed (1.2f);

  // Get the iPcMesh interface so we can load the right mesh
  // for our player.
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (playerEntity);
  pcmesh->SetPath ("/lib/kwartz");
  pcmesh->SetMesh ("kwartz_fact", "kwartz.lib");
  pcmesh->MoveMesh (sector, csVector3 (0, 3, 0));

  csRef<iPcJump> jump = celQueryPropertyClassEntity<iPcJump> (playerEntity);
  jump->SetBoostJump (false);
  jump->SetJumpHeight (1.0f);

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (playerEntity);
  pclinmove->InitCD (
      csVector3 (0.5f,  0.8f, 0.5f),
      csVector3 (0.5f,  0.4f, 0.5f),
      csVector3 (0.0f, -0.4f, 0.0f));

  // Get the iPcActorMove interface so that we can set movement speed.
  //csRef<iPcActorMove> pcactormove = celQueryPropertyClassEntity<iPcActorMove> (playerEntity);
  //pcactormove->SetMovementSpeed (3.0f);
  //pcactormove->SetRunningSpeed (5.0f);
  //pcactormove->SetRotationSpeed (1.75f);

  csRef<iPcCommandInput> pcinput = celQueryPropertyClassEntity<iPcCommandInput> (playerEntity);
  pcinput->Bind ("left", "left");
  pcinput->Bind ("right", "right");
  pcinput->Bind ("up", "up");
  pcinput->Bind ("down", "down");
  pcinput->Bind ("a", "left");
  pcinput->Bind ("d", "right");
  pcinput->Bind ("w", "up");
  pcinput->Bind ("s", "down");
  pcinput->Bind ("space", "jump");
  pcinput->Bind ("[", "camleft");
  pcinput->Bind ("]", "camright");
  pcinput->Bind ("pageup", "camup");
  pcinput->Bind ("pagedown", "camdown");

  pcinput->Bind ("x", "lockon");
  pcinput->Bind ("c", "resetcam");

  csRef<iPcInventory> inventory = celQueryPropertyClassEntity<iPcInventory> (
	  playerEntity);

  csRef<iCelEntity> dummy = pl->CreateEntity ("Dummy 1", 0, 0, CEL_PROPCLASS_END);
  inventory->AddEntity (dummy);
  dummy = pl->CreateEntity ("Dummy 2", 0, 0, CEL_PROPCLASS_END);
  inventory->AddEntity (dummy);
  dummy = pl->CreateEntity ("Dummy 3", 0, 0, CEL_PROPCLASS_END);
  inventory->AddEntity (dummy);
  iCelEntityTemplate* goldTemplate = pl->FindEntityTemplate ("gold");
  inventory->AddEntityTemplate (goldTemplate, 100);

  elcm->SetPlayer (playerEntity);

  return true;
}

bool ElcmTest::ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  printf ("ReceiveMessage\n"); fflush (stdout);
  if (msg_id == msgInventory)
  {
    const celData* data = params->GetParameterByIndex (0);
    if (data->type != CEL_DATA_STRING)
      return ReportError ("Invalid parameter for 'inventory' message. Expected string.");
    iCelEntity* ent = pl->FindEntity (data->value.s->GetData ());
    csRef<iPcInventory> inventory = celQueryPropertyClassEntity<iPcInventory> (ent);
    if (!inventory)
    {
      printf ("There is no inventory for this entity!\n");
      fflush (stdout);
    }
    else
    {
      csString title;
      title.Format ("Inventory for %s", ent->QueryObject ()->GetName ());
      uiInventory->Open (title, inventory);
    }
    return true;
  }
  return false;
}

void ElcmTest::Frame ()
{
  float elapsed_time = vc->GetElapsedSeconds ();
  dyn->Step (elapsed_time);
  dynworld->PrepareView (camera, elapsed_time);
}

bool ElcmTest::OnKeyboard(iEvent& ev)
{
  // We got a keyboard event.
  csKeyEventType eventtype = csKeyEventHelper::GetEventType(&ev);
  if (eventtype == csKeyEventTypeDown)
  {
    // The user pressed a key (as opposed to releasing it).
    utf32_char code = csKeyEventHelper::GetCookedCode(&ev);
    if (code == CSKEY_ESC)
    {
      // The user pressed escape to exit the application.
      // The proper way to quit a Crystal Space application
      // is by broadcasting a cscmdQuit event. That will cause the
      // main runloop to stop. To do that we get the event queue from
      // the object registry and then post the event.
      csRef<iEventQueue> q =
        csQueryRegistry<iEventQueue> (GetObjectRegistry());
      if (q.IsValid()) q->GetEventOutlet()->Broadcast(
	  csevQuit (GetObjectRegistry ()));
    }
    else if (code == 'p')
    {
      elcm->Dump ();
      dynworld->Dump ();
      printf ("### Physical Layer ###\n");
      printf ("  Total entities=%d\n", pl->GetEntityCount ());
      printf ("  Total meshes=%d\n", engine->GetMeshes ()->GetCount ());
      fflush (stdout);
    }
    else if (code == 'i')
    {
      csRef<iPcInventory> inventory = celQueryPropertyClassEntity<iPcInventory> (
	  playerEntity);
      uiInventory->Open ("Inventory for player", inventory);
    }
  }
  return false;
}

bool ElcmTest::OnInitialize (int argc, char* argv[])
{
  if (!celInitializer::RequestPlugins (object_reg,
        CS_REQUEST_VFS,
    	CS_REQUEST_OPENGL3D,
    	CS_REQUEST_ENGINE,
    	CS_REQUEST_FONTSERVER,
    	CS_REQUEST_IMAGELOADER,
    	CS_REQUEST_LEVELLOADER,
    	CS_REQUEST_REPORTER,
    	CS_REQUEST_REPORTERLISTENER,
    	CS_REQUEST_PLUGIN ("crystalspace.cegui.wrapper", iCEGUI),
    	CS_REQUEST_PLUGIN ("cel.ui.inventory", iUIInventory),
	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.opcode",
		iCollideSystem),
    	CS_REQUEST_PLUGIN ("cel.physicallayer", iCelPlLayer),
    	CS_REQUEST_PLUGIN ("cel.tools.elcm", iELCM),
	CS_REQUEST_PLUGIN("crystalspace.dynamics.bullet", iDynamics),
        CS_REQUEST_END))
    return ReportError ("Can't initialize plugins!");

  csBaseEventHandler::Initialize(object_reg);

  if (!RegisterQueue(object_reg, csevAllEvents(object_reg)))
    return ReportError ("Can't setup event handler!");

  return true;
}

bool ElcmTest::Application ()
{
  if (!OpenApplication (object_reg))
    return ReportError ("Error opening system!");

  g3d = csQueryRegistry<iGraphics3D> (object_reg);
  engine = csQueryRegistry<iEngine> (object_reg);
  loader = csQueryRegistry<iLoader> (object_reg);
  vfs = csQueryRegistry<iVFS> (object_reg);
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  kbd = csQueryRegistry<iKeyboardDriver> (object_reg);
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);

  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  elcm = csQueryRegistry<iELCM> (object_reg);

  elcm->SetActivityRadius (12.0f);
  elcm->SetDistanceThresshold (1.0f);
  elcm->SetCheckTime (100);
  elcm->SetUnloadCheckFrequency (30);

  msgInventory = pl->FetchStringID ("elcm.inventory");

  ceguiPrinter.AttachNew (new CeguiPrinter (this));
  pl->CallbackEveryFrame (ceguiPrinter, CEL_EVENT_POST);

  if (!InitWindowSystem ())
    return ReportError ("Error initializing windowing!");
  if (!InitPhysics ())
    return ReportError ("Error initializing physics!");
  if (!CreateLevel ())
    return ReportError ("Error creating level!");
  if (!CreateFactories ())
    return ReportError ("Couldn't create factories!");
  if (!CreatePlayer ())
    return ReportError ("Couldn't create player!");
  if (!FillDynamicWorld ())
    return ReportError ("Couldn't fill the dynamic world!");

  printer.AttachNew (new FramePrinter (object_reg));

  Run ();

  return true;
}

void ElcmTest::OnExit ()
{
  printer.Invalidate ();
}
