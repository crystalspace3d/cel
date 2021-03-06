/*
  Copyright (C) 2011 Christian Van Brussel, Eutyche Mukuama, Dodzi de Souza
      Communications and Remote
      Sensing Laboratory of the School of Engineering at the 
      Universite catholique de Louvain, Belgium
      http://www.tele.ucl.ac.be

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "cssysdef.h"

#include "iengine/campos.h"
#include "cstool/collider.h"

#include "celtool/initapp.h"

#include "behave.h"
#include "lifesim.h"

LifeSimulator::LifeSimulator ()
  : DemoApplication ("CrystalSpace.LifeSimulator")
{
}

void LifeSimulator::LoadBehaviourTreeFromXML (const char* filename, iCelEntity* entity)
{
  csRef<iDocumentSystem> xml = 
		csQueryRegistry<iDocumentSystem> (object_reg);
  if (!xml)
    xml.AttachNew (new csTinyDocumentSystem ());
  csRef<iDocument> doc = xml->CreateDocument ();

  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  csRef<iDataBuffer> xml_buf = vfs->ReadFile (filename);

  const char* error = doc->Parse (xml_buf, true);
  if (error != 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
                "cel.questmanager",
                "Can't open file '%s': %s!", filename, error);
  }

  csRef<iDocumentNode> xml_node = doc->GetRoot ();
   
  csRef<iBTNode> tree = celQueryPropertyClassEntity<iBTNode> (entity);
  csRef<iCelPropertyClass> tree_propclass = scfQueryInterface<iCelPropertyClass> (tree);
  tree_propclass->SetProperty(physicalLayer->FetchStringID("xml"), xml_node);   
  
  csRef<iCelParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());

  celData result;  
  tree_propclass->PerformAction(physicalLayer->FetchStringID("Load BT From XML"), params, result);
  tree_propclass->PerformAction(physicalLayer->FetchStringID("BT Start"), params, result);
}

void LifeSimulator::PrintHelp ()
{
  csCommandLineHelper commandLineHelper;

  // Printing help
  commandLineHelper.PrintApplicationHelp
    (GetObjectRegistry (), "cslifesim", "cslifesim", "Crystal Entity Layer's life simulation demonstrator.");
}

bool LifeSimulator::OnInitialize (int argc, char* argv[])
{
  // Default behavior from DemoApplication
  if (!DemoApplication::OnInitialize (argc, argv))
    return false;

  // Load the base plugins
  if (!celInitializer::RequestPlugins (object_reg,
    	CS_REQUEST_PLUGIN ("cel.physicallayer", iCelPlLayer),
    	CS_REQUEST_PLUGIN ("crystalspace.collisiondetection.opcode",
			   iCollideSystem),
        CS_REQUEST_END))
    return ReportError ("Can't initialize plugins!");

  return true;
}

bool LifeSimulator::Application ()
{
  // Default behavior from DemoApplication
  if (!DemoApplication::Application ())
    return false;

  // Create the physical and behavior layers
  physicalLayer = csQueryRegistry<iCelPlLayer> (GetObjectRegistry ());
  behaviourLayer.AttachNew (new BehaviourLayer (GetObjectRegistry (), physicalLayer));
  
  //Create the sound system manager
  sndmgr = csQueryRegistryOrLoad<iSndSysManager> (
    	GetObjectRegistry (), "crystalspace.sndsys.manager");
		
  loader = csQueryRegistry<iLoader> (GetObjectRegistry ());

  // Register the behavior layer
  if (!GetObjectRegistry ()->Register (behaviourLayer, "iCelBlLayer"))
    return ReportError ("Can't register our behaviour layer!");
  physicalLayer->RegisterBehaviourLayer (behaviourLayer);
  
  

  // Create the scene
  if (!CreateScene ())
    return false;

  // Define the available keys
  hudManager->GetKeyDescriptions ()->Push ("f: spawn a Frankie");
  hudManager->GetKeyDescriptions ()->Push ("s: spawn a sheep");
  hudManager->GetKeyDescriptions ()->Push ("r: spawn a ram");
  hudManager->GetKeyDescriptions ()->Push ("t: spawn a rat");
  hudManager->GetKeyDescriptions ()->Push ("u: spawn a butterFly");

  // Create a first Frankie
  CreateFrankieEntity (csVector3 (256.0f, 66.0f, 395.0f));

  // Run the application
  Run();

  return true;
}

void LifeSimulator::Frame ()
{
  // Default behavior from DemoApplication
  DemoApplication::Frame ();
}

bool LifeSimulator::OnKeyboard (iEvent &ev)
{
  // Default behavior from DemoApplication
  if (CS::Utility::DemoApplication::OnKeyboard (ev))
    return true;

  csKeyEventType eventtype = csKeyEventHelper::GetEventType(&ev);
  if (eventtype == csKeyEventTypeDown)
  {
    // Print camera position
    if (csKeyEventHelper::GetCookedCode (&ev) == 'p')
    {
      printf ("camera position: %s\n",
	      view->GetCamera ()->GetTransform ().GetOrigin ().Description ().GetData ());
      return true;
    }

    // Spawn a Frankie
    if (csKeyEventHelper::GetCookedCode (&ev) == 'f')
    {
      csVector3 position;
      if (TraceMouseBeam (position))
	// TODO: this doesn't work on hard cliffs...
	CreateFrankieEntity (position + csVector3 (0.0f, 0.5f, 0.0f));

      return true;
    }
    // Spawn a sheep
    if (csKeyEventHelper::GetCookedCode (&ev) == 's')
    {
      csVector3 position;
      if (TraceMouseBeam (position))
	// TODO: this doesn't work on hard cliffs...
	CreateSheepEntity (position + csVector3 (0.0f, 0.5f, 0.0f));

      return true;
    }
    // Spawn a Ram
    if (csKeyEventHelper::GetCookedCode (&ev) == 'r')
    {
      csVector3 position;
      if (TraceMouseBeam (position))
	// TODO: this doesn't work on hard cliffs...
	CreateRamEntity (position + csVector3 (0.0f, 0.5f, 0.0f));

      return true;
    }
		
    // Spawn a Rat
    if (csKeyEventHelper::GetCookedCode (&ev) == 't')
    {
      csVector3 position;
      if (TraceMouseBeam (position))
	CreateRatEntity (position + csVector3 (0.0f, 0.5f, 0.0f));

      return true;
    }
    // Spawn a ButterFly
    if (csKeyEventHelper::GetCookedCode (&ev) == 'u')
    {
      csVector3 position;
      if (TraceMouseBeam (position))
	CreateButterFlyEntity (position + csVector3 (0.0f, 5.5f, 0.0f));

      return true;
    }
  }

  return false;
}

bool LifeSimulator::CreateScene ()
{
  printf ("Loading level... ");

  // Load the level file named 'world'.
  csRef<iVFS> VFS (csQueryRegistry<iVFS> (GetObjectRegistry ()));
  VFS->ChDir ("/lev/island");
  if (!loader->LoadMapFile ("world"))
    return ReportError(" Error couldn't load level!");

  // Setup the sector and the camera
  room = engine->FindSector ("TerrainSector");
  view->GetCamera ()->SetSector (room);
  cameraManager->SetCamera (view->GetCamera ());
  cameraManager->SetCameraMode (CS::Utility::CAMERA_MOVE_FREE);
  cameraManager->SetMotionSpeed (10.0f);

  printf ("Done\n");
  printf ("Initializing collision system... ");

  // Initialize the collision system
  csRef<iPluginManager> pluginManager = csQueryRegistry<iPluginManager> (GetObjectRegistry ());
  collideSystem = csLoadPlugin<iCollideSystem>
    (pluginManager, "crystalspace.collisiondetection.opcode");
  if (!collideSystem) return ReportError ("No Collision Detection plugin found!");
  GetObjectRegistry ()->Register (collideSystem, "iCollideSystem");

  // Register only the collider of the terrain
  iMeshWrapper* terrain = engine->FindMeshObject ("Terrain");
  if (terrain)
    csColliderHelper::InitializeCollisionWrapper (collideSystem, terrain);
  else
    csColliderHelper::InitializeCollisionWrappers (collideSystem, engine);

  printf ("Done\n");
  printf ("Precaching data... ");
  engine->PrecacheDraw ();
  printf ("Done\n");

  printf ("Ready!\n");

  return true;
}

bool LifeSimulator::TraceMouseBeam (csVector3& position)
{
  csVector2 v2d (mouse->GetLastX (), mouse->GetLastY ());
  iCamera* camera = view->GetCamera ();
  csVector3 v3d = view->InvProject (v2d, 10000);
  csVector3 startBeam = camera->GetTransform ().GetOrigin ();
  csVector3 endBeam = camera->GetTransform ().This2Other (v3d);

  csTraceBeamResult result =
    csColliderHelper::TraceBeam (collideSystem, room, startBeam, endBeam, true);
  if (!result.closest_mesh) return false;

  position = result.closest_isect;

  return true;
}

//---------------------------------------------------------------------------

CS_IMPLEMENT_APPLICATION

int main (int argc, char* argv[])
{
  return LifeSimulator ().Main (argc, argv);
}
