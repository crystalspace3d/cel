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
#include "propclass/prop.h"
#include <physicallayer/propclas.h>
#include <physicallayer/entitytpl.h>
#include <behaviourlayer/behave.h>
#include <celtool/stdparams.h>

#include "elcmtest.h"

//-----------------------------------------------------------------------

void CeguiPrinter::TickEveryFrame ()
{
  parent->GetG3D ()->BeginDraw (CSDRAW_2DGRAPHICS);
  parent->WriteStatusLine ();
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

class ElcmCellCreator : public scfImplementation1<ElcmCellCreator,
  iDynamicCellCreator>
{
private:
  ElcmTest* elcmTest;

public:
  ElcmCellCreator (ElcmTest* elcmTest) :
    scfImplementationType (this), elcmTest (elcmTest) { }
  virtual ~ElcmCellCreator () { }

  virtual iDynamicCell* CreateCell (const char* name)
  {
    return elcmTest->CreateCell (name);
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

bool ElcmTest::CreateSky ()
{
  using namespace CS::Geometry;

  csRef<iMeshWrapper> clouds = GeneralMeshBuilder::CreateMesh (engine,
      sector, "Clouds", "clouddome");
  CS::Graphics::RenderPriority transp = engine->GetRenderPriority ("transp");
  clouds->SetRenderPriority (transp);

  csRef<iMeshWrapper> sky = GeneralMeshBuilder::CreateMesh (engine,
      sector, "Sky", "skydome");
  CS::Graphics::RenderPriority object = engine->GetRenderPriority ("object");
  sky->SetRenderPriority (object);

  return true;
}

void ElcmTest::MakeFloor (iSector* sect)
{
  using namespace CS::Geometry;

  iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName ("stone");

  csRef<iMeshFactoryWrapper> floorFact;
  floorFact = engine->FindMeshFactory ("Floor");
  if (!floorFact)
  {
    TesselatedQuad quad (
      csVector3 (-5000, -1, -5000),
      csVector3 (-5000, -1, 5000),
      csVector3 (5000, -1, -5000));
    DensityTextureMapper mapper (0.3f);
    quad.SetLevel (3);
    quad.SetMapper (&mapper);
    floorFact = GeneralMeshBuilder::CreateFactory (engine, "Floor", &quad);
  }

  csRef<iMeshWrapper> floor = GeneralMeshBuilder::CreateMesh (engine,
      sect, "Floor", floorFact);
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
}

void ElcmTest::AddLight (iSector* sect, const csVector3& pos, float radius,
    const csColor& color)
{
  iLightList* ll = sect->GetLights ();
  csRef<iLight> light;
  light = engine->CreateLight (0, csVector3 (0, 200, 0), 10000, csColor (1, 1, 1));
  ll->Add (light);
}

bool ElcmTest::CreateLevel ()
{
  worldEntity = pl->CreateEntity ("world", 0, 0,
      "pcworld.dynamic", CEL_PROPCLASS_END);
  if (!worldEntity)
    return ReportError ("Error creating world entity!");

  receiver.AttachNew (new ElcmMessageReceiver (this));
  worldEntity->QueryMessageChannel ()->Subscribe (receiver, "elcm.");

  if (!loader->LoadTexture ("stone", "/lib/std/stone4.gif"))
    return ReportError ("Error loading %s texture!",
		 CS::Quote::Single ("stone4"));

  sector = engine->CreateSector ("outside");
  MakeFloor (sector);
  AddLight (sector, csVector3 (0, 200, 0), 10000, csColor (1, 1, 1));

  dynworld = celQueryPropertyClassEntity<iPcDynamicWorld> (worldEntity);
  dynworld->SetRadius (50);
  dynworld->SetELCM (elcm);
  outsideCell = dynworld->AddCell ("outside", sector, dynSys);
  dynworld->SetCurrentCell (outsideCell);

  csColliderHelper::InitializeCollisionWrappers (cdsys, engine);
  engine->Prepare ();

  return true;
}

bool ElcmTest::CreateFactories ()
{
  vfs->ChDir ("/lib/sky/");
  if (!loader->LoadLibraryFile ("library"))
    return false;
  vfs->ChDir ("/cellib/clutter/");
  if (!loader->LoadLibraryFile ("library"))
    return false;
  return loader->LoadLibraryFile ("/cellib/lev/elcmtest.xml");
}

void ElcmTest::FillTreasureCell (iDynamicCell* cell, int seed)
{
  csRandomGen rnd;
  rnd.Initialize (1234567);
  for (size_t i = 0 ; i < rnd.Get (400) + 200 ; i++)
  {
    csString objName = "Money";
    float ox = rnd.Get () * 100.0f - 50.0f;
    float oy = rnd.Get () * 100.0f + 1.0f;
    float yoffset = 0.02;
    csMatrix3 mat = csYRotMatrix3 (rnd.Get () * 3.1415926535);
    iDynamicObject* obj = cell->AddObject (objName, csReversibleTransform (
	mat, csVector3 (ox, yoffset-1.0f, oy)));
    csRef<celVariableParameterBlock> params;
    obj->SetEntity (0, params);
  }
}

void ElcmTest::FillClickerCell (iDynamicCell* cell, int seed)
{
  csRandomGen rnd;
  rnd.Initialize (1234567);
  for (size_t i = 0 ; i < rnd.Get (400) + 200 ; i++)
  {
    csString objName = "Clicker";
    float ox = rnd.Get () * 100.0f - 50.0f;
    float oy = rnd.Get () * 100.0f + 1.0f;
    float yoffset = 0.05;
    csMatrix3 mat = csYRotMatrix3 (rnd.Get () * 3.1415926535);
    iDynamicObject* obj = cell->AddObject (objName, csReversibleTransform (
	mat, csVector3 (ox, yoffset-1.0f, oy)));
    csRef<celVariableParameterBlock> params;
    obj->SetEntity (0, params);
  }
}

void ElcmTest::FillBarrelCell (iDynamicCell* cell, int seed)
{
  csRandomGen rnd;
  rnd.Initialize (1234567);
  for (size_t i = 0 ; i < rnd.Get (400) + 200 ; i++)
  {
    csString objName = "Barrel";
    float ox = rnd.Get () * 100.0f - 50.0f;
    float oy = rnd.Get () * 100.0f + 1.0f;
    float yoffset = -.4;
    csMatrix3 mat = csYRotMatrix3 (rnd.Get () * 3.1415926535);
    iDynamicObject* obj = cell->AddObject (objName, csReversibleTransform (
	mat, csVector3 (ox, yoffset-1.0f, oy)));
    csRef<celVariableParameterBlock> params;
    obj->SetEntity (0, params);
  }
}

void ElcmTest::FillClutterCell (iDynamicCell* cell, int seed)
{
  csRandomGen rnd;
  rnd.Initialize (1234567);
  for (size_t i = 0 ; i < rnd.Get (400) + 200 ; i++)
  {
    csString objName;
    float ox = rnd.Get () * 100.0f - 50.0f;
    float oy = rnd.Get () * 100.0f + 1.0f;
    float yoffset = 0;
    switch (rnd.Get (5))
    {
      case 0: objName = "Money"; yoffset = 0.02; break;
      case 1: objName = "Steak"; yoffset = 0.02; break;
      case 2: objName = "Can"; yoffset = 0.047; break;
      case 3: objName = "Milk"; yoffset = 0.106; break;
      case 4: objName = "Cup"; break;
    }
    csMatrix3 mat = csYRotMatrix3 (rnd.Get () * 3.1415926535);
    iDynamicObject* obj = cell->AddObject (objName, csReversibleTransform (
	mat, csVector3 (ox, yoffset-1.0f, oy)));
    csRef<celVariableParameterBlock> params;
    obj->SetEntity (0, params);
  }
}

iDynamicCell* ElcmTest::CreateCell (const char* name)
{
  csVector3 pos;
  int random;
  csScanStr (name, "cell,%f,%f,%f,%d", &pos.x, &pos.y, &pos.z, &random);

  csColor color;
  switch (random % 7)
  {
    case 0: color.Set (1, .5, .5); break;
    case 1: color.Set (.5, 1, .5); break;
    case 2: color.Set (.5, .5, 1); break;
    case 3: color.Set (1, .5, 1); break;
    case 4: color.Set (.5, 1, 1); break;
    case 5: color.Set (1, 1, .5); break;
    case 6: color.Set (.5, .5, .5); break;
  }

  iSector* sect = engine->CreateSector (name);
  MakeFloor (sect);
  AddLight (sect, csVector3 (0, 200, 0), 10000, color);
  csColliderHelper::InitializeCollisionWrappers (cdsys, sect);

  iDynamicCell* cell = dynworld->AddCell (name, sect, dynSys);

  switch (random % 4)
  {
    case 0: FillTreasureCell (cell, random); break;
    case 1: FillClickerCell (cell, random); break;
    case 2: FillBarrelCell (cell, random); break;
    case 3: FillClutterCell (cell, random); break;
  }

  iDynamicObject* obj;
  obj = cell->AddObject ("Door", csReversibleTransform (csMatrix3 (),
	csVector3 (0, .4, 0)));

  csRef<celVariableParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());
  params->AddParameter (pl->FetchStringID ("cell")).Set ("outside");
  params->AddParameter (pl->FetchStringID ("pos")).Set (pos);
  if (!obj->SetEntity (0, params))
  {
    ReportError ("Could not set entity template 'Door'!");
    return 0;
  }

  obj->MakeStatic ();
  cell->MarkBaseline ();
  return cell;
}

#define SIZE 250
//#define SIZE 1
#define MULT 7.0f

bool ElcmTest::FillDynamicWorld ()
{
  csRandomGen rnd;
  rnd.Initialize (1234567);

  int cntBarrel = 0;
  int cntTable = 0;
  int cntClicker = 0;
  int cntDoor = 0;
  csMatrix3 mat;
  iDynamicObject* obj;
  for (int y = -SIZE ; y <= SIZE ; y++)
    for (int x = -SIZE ; x <= SIZE ; x++)
    {
      float r = rnd.Get ();
      mat = csYRotMatrix3 (rnd.Get () * 3.1415926535);
      float ox = float (x*MULT) + rnd.Get () * 4.0f - 2.0f;
      float oy = float (y*MULT) + rnd.Get () * 4.0f - 2.0f;
      if (r < .4)
      {
	cntBarrel++;
        obj = outsideCell->AddObject ("Barrel", csReversibleTransform (mat, csVector3 (ox, -.4, oy)));
        csRef<iCelParameterBlock> params;
        params.AttachNew (new celVariableParameterBlock ());
        if (!obj->SetEntity (0, params))
	  return ReportError ("Could not set entity template 'Barrel'!");
      }
      else if (r < .65)
      {
	cntTable++;
        obj = outsideCell->AddObject ("Table", csReversibleTransform (mat, csVector3 (ox, -1, oy)));
        csRef<iCelParameterBlock> params;
        params.AttachNew (new celVariableParameterBlock ());
        if (!obj->SetEntity (0, params))
	  return ReportError ("Could not set entity template 'Table'!");

	csString objName;
	float yoffset = 0;
	switch (rnd.Get (5))
	{
	  case 0: objName = "Money"; yoffset = 0.02; break;
	  case 1: objName = "Steak"; yoffset = 0.02; break;
	  case 2: objName = "Can"; yoffset = 0.047; break;
	  case 3: objName = "Milk"; yoffset = 0.106; break;
	  case 4: objName = "Cup"; break;
	}
        obj = outsideCell->AddObject (objName, csReversibleTransform (
	    mat, csVector3 (ox, yoffset, oy)));
        if (!obj->SetEntity (0, params))
	  return ReportError ("Could not set entity template '%s'!",
	      objName.GetData ());
      }
      else if (r < .9)
      {
	cntClicker++;
        obj = outsideCell->AddObject ("Clicker", csReversibleTransform (mat, csVector3 (ox, -.95, oy)));
        csRef<iCelParameterBlock> params;
        params.AttachNew (new celVariableParameterBlock ());
        if (!obj->SetEntity (0, params))
	  return ReportError ("Could not set entity template 'Clicker'!");
      }
      else
      {
	cntDoor++;
        obj = outsideCell->AddObject ("Door", csReversibleTransform (mat, csVector3 (ox, .4, oy)));
	obj->MakeStatic ();
	csString cellName;
	cellName.Format ("cell,%g,%g,%g,%d", ox, .4, oy, rnd.Get (1000000));

        csRef<celVariableParameterBlock> params;
        params.AttachNew (new celVariableParameterBlock ());
	params->AddParameter (pl->FetchStringID ("cell")).Set (cellName);
	params->AddParameter (pl->FetchStringID ("pos")).Set (csVector3 (0, 0, 0));
        if (!obj->SetEntity (0, params))
	  return ReportError ("Could not set entity template 'Door'!");
      }
      r = rnd.Get ();
      if (r < .1)
      {
        float ox = float (x*MULT) + MULT/2.0f;
        float oy = float (y*MULT) + MULT/2.0f;
	csVector3 pos (ox, -1, oy);
        obj = outsideCell->AddObject ("MoneySpawn", csReversibleTransform (
	      csMatrix3 (), pos));
        csRef<celVariableParameterBlock> params;
        params.AttachNew (new celVariableParameterBlock ());
	params->AddParameter (pl->FetchStringID ("sector")).Set ("outside");
	params->AddParameter (pl->FetchStringID ("pos")).Set (pos);
        if (!obj->SetEntity (0, params))
	  return ReportError ("Could not set entity template 'MoneyTemplate'!");
      }
    }
  dynworld->MarkBaseline ();
  printf ("Created %d objects (%d barrels, %d tables, %d clickers, %d doors)!\n",
      outsideCell->GetObjectCount (), cntBarrel, cntTable, cntClicker, cntDoor);
  return true;
}

bool ElcmTest::CreatePlayer ()
{
  playerEntity = pl->CreateEntity ("player", 0, 0,
    "pcinput.standard",
    "pctools.inventory",
    "pccamera.delegate",
    "pccamera.mode.tracking",
    "pcobject.mesh",
    "pcmove.linear",
    "pcmove.actor.standard",
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
  //pcmesh->SetPath ("/lib/krystal");
  //pcmesh->SetMesh ("krystal", "krystal.xml");
  pcmesh->SetPath ("/lib/frankie");
  pcmesh->SetMesh ("franky_frankie", "frankie.xml");
  pcmesh->MoveMesh (sector, csVector3 (0, 3, 0));

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (playerEntity);
  //pclinmove->InitCD (pcmesh->GetMesh (), 0.3f);
  pclinmove->InitCD (
      csVector3 (0.5f,  0.4f, 0.5f),
      csVector3 (0.5f,  0.2f, 0.5f),
      csVector3 (0.0f,  0.0f, 0.0f));

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcactormove = celQueryPropertyClassEntity<iPcActorMove> (playerEntity);
  pcactormove->SetMovementSpeed (5.0f);
  pcactormove->SetRunningSpeed (8.0f);
  pcactormove->SetRotationSpeed (2.0f);
  pcactormove->SubscribeMessages ();

  //csRef<iPcJump> jump = celQueryPropertyClassEntity<iPcJump> (playerEntity);
  //jump->SetBoostJump (false);
  //jump->SetJumpHeight (1.0f);

  csRef<iPcCommandInput> pcinput = celQueryPropertyClassEntity<iPcCommandInput> (playerEntity);
  pcinput->Bind ("left", "rotateleft");
  pcinput->Bind ("right", "rotateright");
  pcinput->Bind ("up", "forward");
  pcinput->Bind ("down", "backward");
  pcinput->Bind ("a", "rotateleft");
  pcinput->Bind ("d", "rotateright");
  pcinput->Bind ("w", "forward");
  pcinput->Bind ("s", "backward");
  pcinput->Bind ("q", "strafeleft");
  pcinput->Bind ("e", "straferight");
  pcinput->Bind ("space", "jump");
  pcinput->Bind ("[", "camleft");
  pcinput->Bind ("]", "camright");
  pcinput->Bind ("pageup", "camup");
  pcinput->Bind ("pagedown", "camdown");

  pcinput->Bind ("x", "lockon");
  pcinput->Bind ("c", "resetcam");

  csRef<iPcInventory> inventory = celQueryPropertyClassEntity<iPcInventory> (
	  playerEntity);

  iCelEntityTemplate* moneyTemplate = pl->FindEntityTemplate ("Money");
  inventory->AddEntityTemplate (moneyTemplate, 100);

  elcm->SetPlayer (playerEntity);

  return true;
}

void ElcmTest::SelectEntity (iCelEntity* entity)
{
  iPcInventory* inv = uiInventory->GetInventory ();
  if (!inv) return;
  csStringID tmpID = entity->GetTemplateNameID ();
  if (tmpID != csInvalidStringID)
  {
    csString factName = pl->FetchString (tmpID);
    csRef<iPcMesh> playerpcmesh = celQueryPropertyClassEntity<iPcMesh> (
	playerEntity);
    csReversibleTransform trans = playerpcmesh->GetMesh ()->GetMovable ()->GetFullTransform ();
    trans.SetOrigin (trans.GetOrigin () + trans.GetFront () / 2.0f + trans.GetUp ());
    iDynamicObject* obj = dynworld->GetCurrentCell ()->AddObject (factName, trans);
    obj->LinkEntity (entity);
    inv->RemoveEntity (entity);
    entity->Activate ();
  }
  else
  {
    printf ("This item cannot be removed from the inventory!\n");
    fflush (stdout);
  }
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
  trans.SetOrigin (trans.GetOrigin () + trans.GetFront () / 2.0f + trans.GetUp ());
  iDynamicObject* obj = dynworld->GetCurrentCell ()->AddObject (
      tpl->GetName (), trans);
  csRef<iCelParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());
  obj->SetEntity (0, params);
  iCelEntity* entity = obj->ForceEntity ();
  entity->MarkBaseline ();
  entity->Activate ();
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

void ElcmTest::PickUpDynObj (iDynamicObject* dynobj)
{
  iCelEntity* ent = dynobj->GetEntity ();
  csRef<iPcInventory> inventory = celQueryPropertyClassEntity<iPcInventory> (
	  playerEntity);
  if (ent->IsModifiedSinceBaseline ())
  {
    // This entity has state. We cannot convert it to a template in the
    // inventory so we have to add the actual entity.
    inventory->AddEntity (ent);
    dynworld->ForceInvisible (dynobj);
    dynobj->UnlinkEntity ();
    dynworld->GetCurrentCell ()->DeleteObject (dynobj);
  }
  else
  {
    iCelEntityTemplate* tpl = pl->FindEntityTemplate (
	dynobj->GetFactory ()->GetName ());
    if (tpl)
    {
      inventory->AddEntityTemplate (tpl, 1);
      dynworld->GetCurrentCell ()->DeleteObject (dynobj);
    }
  }
}

bool ElcmTest::ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  printf ("ReceiveMessage\n"); fflush (stdout);
  if (msg_id == msgInventory)
  {
    const celData* data = params->GetParameter (pl->FetchStringID ("inventoryEntity"));
    if (!data || data->type != CEL_DATA_ENTITY)
      return ReportError ("Invalid parameter for 'inventory' message. Expected long.");
    iCelEntity* ent = data->value.ent;
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
  else if (msg_id == msgTeleport)
  {
    const celData* data = params->GetParameter (pl->FetchStringID ("cell"));
    if (!data || data->type != CEL_DATA_STRING)
      return ReportError ("Invalid parameter for 'teleport' message. Expected string for 'cell' parameter.");
    csString cellName = data->value.s->GetData ();

    data = params->GetParameter (pl->FetchStringID ("pos"));
    if (!data || data->type != CEL_DATA_VECTOR3)
      return ReportError ("Invalid parameter for 'teleport' message. Expected vector3 for 'pos' parameter.");
    csVector3 pos (data->value.v.x, data->value.v.y, data->value.v.z);
    Teleport (cellName, pos);
    return true;
  }
  return false;
}

void ElcmTest::Teleport (const char* cellName, const csVector3& pos)
{
  iDynamicCell* cell = dynworld->FindCell (cellName);
  if (!cell)
    cell = CreateCell (cellName);
  if (!cell)
  {
    printf ("Can't teleport to cell '%s'!\n", cellName);
    fflush (stdout);
    return;
  }
  dynworld->SetCurrentCell (cell);

  iSector* sect = engine->FindSector (cellName);
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (playerEntity);
  pcmesh->MoveMesh (sect, pos);
}

void ElcmTest::Frame ()
{
  float elapsed_time = vc->GetElapsedSeconds ();
  dyn->Step (elapsed_time);
  dynworld->PrepareView (camera, elapsed_time);
}

iCelEntity* ElcmTest::FindHitEntity (int x, int y)
{
  iDynamicObject* dynobj = FindHitDynObj (x, y);
  if (!dynobj) return 0;
  return dynobj->GetEntity ();
}

iDynamicObject* ElcmTest::FindHitDynObj (int x, int y)
{
  csVector2 v2d (x, g3d->GetDriver2D ()->GetHeight () - y);
  csVector3 v3d = camera->InvPerspective (v2d, 100);
  csVector3 start = camera->GetTransform ().GetOrigin ();
  csVector3 end = camera->GetTransform ().This2Other (v3d);

  csSectorHitBeamResult result = camera->GetSector ()->HitBeamPortals (start, end);
  if (result.mesh)
  {
    iDynamicObject* dynobj = dynworld->GetCurrentCell ()->FindObject (result.mesh);
    if (dynobj) return dynobj;
  }
  return 0;
}

iRigidBody* ElcmTest::FindHitBody (int x, int y, csVector3& start,
    csVector3& end, csVector3& isect)
{
  csVector2 v2d (x, g3d->GetDriver2D ()->GetHeight () - y);
  csVector3 v3d = camera->InvPerspective (v2d, 100);
  start = camera->GetTransform ().GetOrigin ();
  end = camera->GetTransform ().This2Other (v3d);
  CS::Physics::Bullet::HitBeamResult result = bullet_dynSys->HitBeam (start, end);
  if (result.body)
  {
    iRigidBody* hitBody = result.body->QueryRigidBody ();
    isect = result.isect;
    return hitBody;
  }
  return 0;
}

void ElcmTest::WriteStatusLine ()
{
  iGraphics2D* g2d = g3d->GetDriver2D ();
  g2d->Write (font, 9, 23, colorBlack, -1, statusLine.GetData ());
  g2d->Write (font, 10, 24, colorRed, -1, statusLine.GetData ());
  csString line2;
  const csVector3& pos = camera->GetTransform ().GetOrigin ();
  line2.Format ("Pos (%g,%g,%g) #obj=%d #ent=%d #mesh=%d", pos.x, pos.y, pos.z,
      dynworld->GetCurrentCell ()->GetObjectCount (), pl->GetEntityCount (),
      engine->GetMeshes ()->GetCount ());
  g2d->Write (font, 9, 9, colorBlack, -1, line2.GetData ());
  g2d->Write (font, 10, 10, colorRed, -1, line2.GetData ());
}

void ElcmTest::UpdateStatusLine (iDynamicObject* dynobj)
{
  if (dynobj == 0)
  {
    statusLine = "...";
    return;
  }
  csRef<iString> desc = dynobj->GetDescription ();

  iCelEntity* ent = dynobj->GetEntity ();
  if (ent)
  {
    csRef<iPcProperties> pcprop = celQueryPropertyClassEntity<iPcProperties> (ent);
    if (pcprop)
    {
      size_t idx = pcprop->GetPropertyIndex ("counter");
      if (idx != csArrayItemNotFound)
      {
	long counter = pcprop->GetPropertyLong (idx);
        statusLine.Format ("%s counter=%ld", desc->GetData (), counter);
	return;
      }
    }
  }
  statusLine = desc->GetData ();
}

bool ElcmTest::OnMouseMove (iEvent& ev)
{
  int x = csMouseEventHelper::GetX (&ev);
  int y = csMouseEventHelper::GetY (&ev);
  iDynamicObject* dynobj = FindHitDynObj (x, y);
  UpdateStatusLine (dynobj);
  return false;
}

bool ElcmTest::OnMouseDown (iEvent& ev)
{
  uint but = csMouseEventHelper::GetButton (&ev);
  int x = csMouseEventHelper::GetX (&ev);
  int y = csMouseEventHelper::GetY (&ev);

  if (but == 0)
  {
    iDynamicObject* dynobj = FindHitDynObj (x, y);
    if (!dynobj) return false;
    uint32 mod = csMouseEventHelper::GetModifiers (&ev);
    bool ctrl = (mod & CSMASK_CTRL) != 0;
    if (ctrl)
    {
      dynworld->GetCurrentCell ()->DeleteObject (dynobj);
      UpdateStatusLine (0);
    }
    else
    {
      if (!dynobj->GetEntity ()) return false;
      iMessageChannel* channel = dynobj->GetEntity ()->QueryMessageChannel ();
      channel->SendMessage ("elcm.activate", 0, 0);
      UpdateStatusLine (dynobj);
    }
    return true;
  }
  else if (but == 1)
  {
    csVector3 start, end, isect;
    iRigidBody* hitBody = FindHitBody (x, y, start, end, isect);
    if (!hitBody) return false;
    csVector3 force = end-start;
    force.Normalize ();
    force *= 2.0 * hitBody->GetMass ();
    hitBody->AddForceAtPos (force, isect);
    return true;
  }
  else if (but == 2)
  {
    iDynamicObject* dynobj = FindHitDynObj (x, y);
    if (!dynobj) return false;
    PickUpDynObj (dynobj);
    UpdateStatusLine (0);
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
      dynworld->DeleteAll ();
      outsideCell = dynworld->AddCell ("outside", sector, dynSys);
      dynworld->SetCurrentCell (outsideCell);
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

  // @@@ Should be temporary!
  csRef<iThreadManager> threadmgr = csQueryRegistry<iThreadManager> (object_reg);
  threadmgr->SetAlwaysRunNow (true);

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

  elcm->SetActivityRadius (16.0f);
  elcm->SetDistanceThresshold (1.0f);
  elcm->SetCheckTime (100);
  elcm->SetUnloadCheckFrequency (30);

  colorWhite = g3d->GetDriver2D ()->FindRGB (255, 255, 255);
  colorBlack = g3d->GetDriver2D ()->FindRGB (0, 0, 0);
  colorRed = g3d->GetDriver2D ()->FindRGB (255, 0, 0);
  font = g3d->GetDriver2D ()->GetFontServer ()->LoadFont (CSFONT_COURIER);

  msgInventory = pl->FetchStringID ("elcm.inventory");
  msgTeleport = pl->FetchStringID ("elcm.teleport");

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
  if (!CreateSky ())
    return ReportError ("Error creating sky!");
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
