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

#include <csutil/weakrefarr.h>

#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/actormove.h"
#include "propclass/camera.h"
#include "propclass/inv.h"
#include "propclass/linmove.h"
#include "propclass/mover.h"
#include "propclass/meshsel.h"
#include "propclass/mesh.h"
#include "propclass/prop.h"
#include <propclass/trigger.h>
#include <propclass/steer.h>
#include <propclass/sound.h>
#include "celtool/stdparams.h"

#include <isndsys/ss_source.h>


#include "frankie.h"
#include "lifesim.h"

#define BTREE_UPDATE_RATE 500

static int entityCount = 0;

bool LifeSimulator::CreateFrankieEntity (const csVector3 position)
{
  csString name = "Frankie ";
  name += entityCount++;

  csRef<iCelEntity> entity =
    physicalLayer->CreateEntity (name, 0, 0,
				 "pcobject.mesh",
				 "pcmove.linear",
				 "pcmove.actor.standard",
				 "pcmove.mover",
				 "pclogic.trigger",
				 "pcmove.steer",
				 "pcsound.source",
                                 "pclogic.behaviourtree",
				 CEL_PROPCLASS_END);
  if (!entity)
    return ReportError ("Error creating player entity!");
	
  // Get the iPcMesh interface so we can load the right mesh for our player.
  csRef<iPcMesh> pcMesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  pcMesh->SetPath("/lib/frankie");
  pcMesh->SetMesh("franky_frankie", "frankie.xml");
  csRef<iMeshWrapper> mesh = pcMesh->GetMesh();
  if (!mesh)
    return false;

  // Get iPcLinearMovement so we can setup the movement system.
  csRef<iPcLinearMovement> pcLinMove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  pcLinMove->InitCD (mesh->GetMeshObject ()->GetMeshWrapper (), 0.3f);
  pcLinMove->SetFullPosition (position, 0.0f, room);

  // Get the iPcActorMove interface so that we can set movement speed.
  csRef<iPcActorMove> pcActorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  pcActorMove->SetMovementSpeed (0.5f);
  pcActorMove->SetRunningSpeed (1.5f);
  pcActorMove->SetRotationSpeed (1.25f);

  csRef<iPcTrigger> pcTrigger = celQueryPropertyClassEntity<iPcTrigger> (entity);
  float radius = 10.0f;
  pcTrigger->SetupTriggerSphere (pcLinMove->GetSector (), pcLinMove->GetPosition (), radius);
  //pcTrigger->EnableMessagesToOthers (true);
  pcTrigger->EnableMessagesToSelf (true);
  //pcTrigger->MonitorClass ("ram");
  pcTrigger->MonitorClass ("sheep");
  pcTrigger->SetFollowEntity (true);
  pcTrigger->EnableTrigger (true);

  csRef<iPcSteer> pcSteer = celQueryPropertyClassEntity<iPcSteer> (entity);
  //pcSteer->CollisionAvoidanceOn(2.0f, 1.0f);
  
  //load sound files and create sound wrappers for each one of them
  //loader->LoadSoundWrapper ("WaterSplash","/lib/YoFrankie_sounds/WaterSplash.ogg");

  // Load behaviour tree from XML
  LoadBehaviourTreeFromXML("/cellib/lev/btFrankie.xml", entity);

  return true;
}
