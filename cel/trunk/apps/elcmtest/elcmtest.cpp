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
#include <physicallayer/entitytpl.h>
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
    scfImplementationType (this), elcmTest (elcmTest) { }
  virtual ~ElcmMessageReceiver () { }

  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
  {
    return elcmTest->ReceiveMessage (msg_id, sender, ret, params);
  }
};

//-----------------------------------------------------------------------

class ElcmSelectionCallback : public scfImplementation2<ElcmSelectionCallback,
  iUIInventory2SelectionCallback, iUIInventorySelectionCallback>
{
private:
  ElcmTest* elcmTest;

public:
  ElcmSelectionCallback (ElcmTest* elcmTest) :
    scfImplementationType (this), elcmTest (elcmTest) { }
  virtual ~ElcmSelectionCallback () { }

  virtual void SelectEntity (iCelEntity* entity, bool left)
  {
    elcmTest->SelectEntity (entity, left);
  }

  virtual void SelectTemplate (iCelEntityTemplate* tpl, bool left)
  {
    elcmTest->SelectTemplate (tpl, left);
  }

  virtual void SelectEntity (iCelEntity* entity)
  {
    elcmTest->SelectEntity (entity);
  }

  virtual void SelectTemplate (iCelEntityTemplate* tpl)
  {
    elcmTest->SelectTemplate (tpl);
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
  uiInventory2 = csQueryRegistry<iUIInventory2> (GetObjectRegistry ());
  if (!uiInventory2) return ReportError ("Failed to locate UI Double Inventory plugin!");

  csRef<ElcmSelectionCallback> cb;
  cb.AttachNew (new ElcmSelectionCallback (this));
  uiInventory->AddSelectionListener (cb);
  uiInventory2->AddSelectionListener (cb);

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
  vfs->ChDir ("/cellib/clutter/");
  if (!loader->LoadLibraryFile ("library"))
    return false;
  return loader->LoadLibraryFile ("/cellib/lev/elcmtest.xml");
}

#define SIZE 250
//#define SIZE 1

bool ElcmTest::FillDynamicWorld ()
{
  csRandomGen rnd;
  rnd.Initialize (1234567);

  csMatrix3 matId;
  iDynamicObject* obj;
  for (int y = -SIZE ; y <= SIZE ; y++)
    for (int x = -SIZE ; x <= SIZE ; x++)
    {
      float r = rnd.Get ();
      if (r < .4)
      {
        obj = dynworld->AddObject ("Barrel", csReversibleTransform (
	    matId, csVector3 (float (x*5), -.4, float (y*5))));
        csString name;
        name.Format ("barrel%d_%d", x+50, y+50);
        csRef<iCelParameterBlock> params;
        params.AttachNew (new celVariableParameterBlock ());
        if (!obj->SetEntity (name, params))
	  return ReportError ("Could not set entity template 'Barrel'!");
      }
      else if (r < .7)
      {
        obj = dynworld->AddObject ("Money", csReversibleTransform (
	    matId, csVector3 (float (x*5), -.95, float (y*5))));
        csString name;
        name.Format ("mon%d_%d", x+50, y+50);
        csRef<iCelParameterBlock> params;
        params.AttachNew (new celVariableParameterBlock ());
        if (!obj->SetEntity (name, params))
	  return ReportError ("Could not set entity template 'Money'!");
      }
      else
      {
        obj = dynworld->AddObject ("Clicker", csReversibleTransform (
	    matId, csVector3 (float (x*5), -.95, float (y*5))));
        csString name;
        name.Format ("click%d_%d", x+50, y+50);
        csRef<iCelParameterBlock> params;
        params.AttachNew (new celVariableParameterBlock ());
        if (!obj->SetEntity (name, params))
	  return ReportError ("Could not set entity template 'Clicker'!");
      }
    }
  dynworld->MarkBaseline ();
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
  pcmesh->SetPath ("/lib/frankie");
  pcmesh->SetMesh ("franky_frankie", "frankie.xml");
  pcmesh->MoveMesh (sector, csVector3 (0, 3, 0));

  csRef<iPcJump> jump = celQueryPropertyClassEntity<iPcJump> (playerEntity);
  jump->SetBoostJump (false);
  jump->SetJumpHeight (1.0f);

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (playerEntity);
  //pclinmove->InitCD (pcmesh->GetMesh (), 0.3f);
  pclinmove->InitCD (
      csVector3 (0.5f,  0.4f, 0.5f),
      csVector3 (0.5f,  0.2f, 0.5f),
      csVector3 (0.0f, -0.0f, 0.0f));

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
  iCelEntityTemplate* moneyTemplate = pl->FindEntityTemplate ("Money");
  inventory->AddEntityTemplate (moneyTemplate, 100);

  elcm->SetPlayer (playerEntity);

  return true;
}

void ElcmTest::SelectEntity (iCelEntity* entity)
{
  iPcInventory* inv = uiInventory->GetInventory ();
  if (!inv) return;
  //inv->RemoveEntity (entity);
}

void ElcmTest::SelectTemplate (iCelEntityTemplate* tpl)
{
  iPcInventory* inv = uiInventory->GetInventory ();
  if (!inv) return;

  if (!dynworld->FindFactory (tpl->GetName ()))
  {
    printf ("This item cannot be dropped!\n");
    fflush (stdout);
    return;
  }

  size_t idx = inv->FindEntityTemplate (tpl);
  if (idx == csArrayItemNotFound)
  {
    printf ("Weird? Where did it go?\n");
    return;
  }
  //int amount = inv->GetEntityTemplateAmount (idx);
  inv->RemoveEntityTemplate (tpl, 1);	// @@@ Temporary!

  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (playerEntity);
  csReversibleTransform trans = pcmesh->GetMesh ()->GetMovable ()->GetFullTransform ();
  trans.SetOrigin (trans.GetOrigin () + trans.GetFront () / 5.0f + trans.GetUp ());
  iDynamicObject* obj = dynworld->AddObject (tpl->GetName (), trans);
  csRef<iCelParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());
  obj->SetEntity (tpl->GetName (), params);
}

void ElcmTest::SelectEntity (iCelEntity* entity, bool left)
{
  iPcInventory* from, * to;
  if (left)
  {
    from = uiInventory2->GetLeftInventory ();
    to = uiInventory2->GetRightInventory ();
  }
  else
  {
    from = uiInventory2->GetRightInventory ();
    to = uiInventory2->GetLeftInventory ();
  }
  if (!from || !to) return;
  from->RemoveEntity (entity);
  to->AddEntity (entity);
}

void ElcmTest::SelectTemplate (iCelEntityTemplate* tpl, bool left)
{
  iPcInventory* from, * to;
  if (left)
  {
    from = uiInventory2->GetLeftInventory ();
    to = uiInventory2->GetRightInventory ();
  }
  else
  {
    from = uiInventory2->GetRightInventory ();
    to = uiInventory2->GetLeftInventory ();
  }
  if (!from || !to) return;
  size_t idx = from->FindEntityTemplate (tpl);
  if (idx == csArrayItemNotFound)
  {
    printf ("Weird? Where did it go?\n");
    return;
  }
  int amount = from->GetEntityTemplateAmount (idx);
  from->RemoveEntityTemplate (tpl, amount);
  to->AddEntityTemplate (tpl, amount);
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
      inventory->GenerateLoot ();
      csRef<iPcInventory> playerInv = celQueryPropertyClassEntity<iPcInventory> (
	  playerEntity);
      csString title;
      title.Format ("Inventory '%s' <-> 'player'", ent->GetName ());
      uiInventory2->Open (title, inventory, playerInv);
    }
    return true;
  }
  else if (msg_id == msgPickupTpl)
  {
    const celData* data = params->GetParameterByIndex (0);
    if (data->type != CEL_DATA_STRING)
      return ReportError ("Invalid parameter for 'pickup' message. Expected string.");
    iCelEntity* ent = pl->FindEntity (data->value.s->GetData ());
    iDynamicObject* dynobj = dynworld->FindDynamicObject (ent);
    if (dynobj)
    {
      csRef<iPcInventory> inventory = celQueryPropertyClassEntity<iPcInventory> (
	      playerEntity);
      if (ent->IsModifiedSinceBaseline ())
      {
	// This entity has state. We cannot convert it to a template in the
	// inventory so we have to add the actual entity.
	inventory->AddEntity (ent);
	dynworld->ForceInvisible (dynobj);
	dynobj->UnlinkEntity ();
        dynworld->DeleteObject (dynobj);
      }
      else
      {
        iCelEntityTemplate* tpl = pl->FindEntityTemplate (
	    dynobj->GetFactory ()->GetName ());
        if (tpl)
        {
	  inventory->AddEntityTemplate (tpl, 1);
          dynworld->DeleteObject (dynobj);
        }
      }
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

bool ElcmTest::OnMouseDown (iEvent& ev)
{
  uint but = csMouseEventHelper::GetButton (&ev);
  int x = csMouseEventHelper::GetX (&ev);
  int y = csMouseEventHelper::GetY (&ev);

  csVector2 v2d (x, g3d->GetDriver2D ()->GetHeight () - y);
  csVector3 v3d = camera->InvPerspective (v2d, 100);
  csVector3 start = camera->GetTransform ().GetOrigin ();
  csVector3 end = camera->GetTransform ().This2Other (v3d);

  if (but == 1)
  {
    CS::Physics::Bullet::HitBeamResult result = bullet_dynSys->HitBeam (start, end);
    if (result.body)
    {
      iRigidBody* hitBody = result.body->QueryRigidBody ();
      csVector3 force = end-start;
      force.Normalize ();
      force *= 2.0 * hitBody->GetMass ();
      hitBody->AddForceAtPos (force, result.isect);
    }
    return true;
  }
  else if (but == 2)
  {
    csSectorHitBeamResult result = camera->GetSector ()->HitBeamPortals (start, end);
    if (result.mesh)
    {
      iDynamicObject* dynobj = dynworld->FindObject (result.mesh);
      if (dynobj)
      {
	dynworld->DeleteObject (dynobj);
      }
    }
    return true;
  }
  return false;
}

bool ElcmTest::OnKeyboard (iEvent& ev)
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
    else if (code == '1')
    {
      csRef<iDataBuffer> buf = dynworld->SaveModifications ();
      csRef<iFile> file = vfs->Open ("/this/savefile", VFS_FILE_WRITE);
      size_t size = file->Write (buf->GetData (), buf->GetSize ());
      printf ("%d bytes written to 'savefile'!\n", size);
      fflush (stdout);
    }
    else if (code == '2')
    {
      dynworld->DeleteObjects ();
      FillDynamicWorld ();
      csRef<iFile> file = vfs->Open ("/this/savefile", VFS_FILE_READ);
      csRef<iDataBuffer> buf = file->GetAllData ();
      dynworld->RestoreModifications (buf);
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
    	CS_REQUEST_PLUGIN ("cel.ui.inventory2", iUIInventory2),
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
  msgPickupTpl = pl->FetchStringID ("elcm.pickup.tpl");

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
