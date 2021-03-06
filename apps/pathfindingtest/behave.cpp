/*
    Copyright (C) 2010 by Leonardo Rodrigo Domingues

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
#include <crystalspace.h>
#include <physicallayer/entity.h>
#include <physicallayer/propclas.h>
#include <propclass/linmove.h>
#include <propclass/prop.h>
#include "behave.h"

/*
 * BehaviourLayer
 */

BehaviourLayer::BehaviourLayer (iCelPlLayer* physicalLayer, iObjectRegistry* objectRegistry) 
    : scfImplementationType (this)
{
  this->physicalLayer = physicalLayer;
  this->objectRegistry = objectRegistry;
}

BehaviourLayer::~BehaviourLayer ()
{
}

iCelBehaviour* BehaviourLayer::CreateBehaviour (iCelEntity* entity, const char* name)
{
  csRef<iCelBehaviour> behaviour;
  if (!strcmp(name, "levelBehaviour"))
    behaviour.AttachNew (new BehaviourLevel(entity, this, physicalLayer));

  else if (!strcmp(name, "playerBehaviour"))
    behaviour.AttachNew (new BehaviourPlayer(entity, this, physicalLayer, objectRegistry));

  else if (!strcmp(name, "boxBehaviour"))
    behaviour.AttachNew (new BehaviourBox(entity, this, physicalLayer, objectRegistry));

  if (behaviour)
    entity->SetBehaviour(behaviour);

  return behaviour;
}

const char* BehaviourLayer::GetName () const
{
  return "behaviourlayer";
}

void BehaviourLayer::SetNavStruct(iCelHNavStruct* navStruct)
{
  this->navStruct = navStruct;
}

iCelHNavStruct* BehaviourLayer::GetNavStruct() const
{ 
  return navStruct;
}

void BehaviourLayer::SetPath (iCelHPath* path)
{
  this->path = path;
}

iCelHPath* BehaviourLayer::GetPath () const
{
  return path;
}



/*
 * BehaviourCommon
 */

BehaviourCommon::BehaviourCommon (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
                                  iCelPlLayer* physicalLayer) 
    : scfImplementationType (this)
{
  this->entity = entity;
  this->behaviourLayer = behaviourLayer;
  this->physicalLayer = physicalLayer;
}

BehaviourCommon::~BehaviourCommon ()
{
}

bool BehaviourCommon::SendMessageV (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                                    iCelParameterBlock* params, va_list arg)
{
  csStringID id = physicalLayer->FetchStringID(msg_id);
  return SendMessage(id, pc, ret, params, arg);
}

bool BehaviourCommon::SendMessage (csStringID, iCelPropertyClass*, celData&, iCelParameterBlock*, va_list)
{
  return false;
}

void* BehaviourCommon::GetInternalObject ()
{ 
  return 0;
}

iCelBlLayer* BehaviourCommon::GetBehaviourLayer () const
{
  return behaviourLayer;
}



/*
 * BehaviourLevel
 */

bool BehaviourLevel::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                                  iCelParameterBlock* params, va_list arg)
{
  return BehaviourCommon::SendMessage(msg_id, pc, ret, params, arg);
}

const char* BehaviourLevel::GetName () const
{ 
  return "levelBehaviour"; 
}



/*
 * BehaviourPlayer
 */

BehaviourPlayer::BehaviourPlayer (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
                                  iCelPlLayer* physicalLayer, iObjectRegistry* objectRegistry)
    : BehaviourCommon (entity, behaviourLayer, physicalLayer)
{
  this->objectRegistry = objectRegistry;

  id_pccommandinput_forward1 = physicalLayer->FetchStringID("pccommandinput_forward1");
  id_pccommandinput_forward0 = physicalLayer->FetchStringID("pccommandinput_forward0");
  id_pccommandinput_backward1 = physicalLayer->FetchStringID("pccommandinput_backward1");
  id_pccommandinput_backward0 = physicalLayer->FetchStringID("pccommandinput_backward0");
  id_pccommandinput_rotateleft1 = physicalLayer->FetchStringID("pccommandinput_rotateleft1");
  id_pccommandinput_rotateleft0 = physicalLayer->FetchStringID("pccommandinput_rotateleft0");
  id_pccommandinput_rotateright1 = physicalLayer->FetchStringID("pccommandinput_rotateright1");
  id_pccommandinput_rotateright0 = physicalLayer->FetchStringID("pccommandinput_rotateright0");
  id_pccommandinput_cammode1 = physicalLayer->FetchStringID("pccommandinput_cammode1");
  id_pccommandinput_drop1 = physicalLayer->FetchStringID("pccommandinput_drop1");
  id_pccommandinput_setposition = physicalLayer->FetchStringID("pccommandinput_setposition0");

  id_pcmover_arrived = physicalLayer->FetchStringID("pcmover_arrived");
}

void BehaviourPlayer::GetLinearMovement ()
{
  if (!pcLinearMovement)
  {
    pcLinearMovement = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  }
}

void BehaviourPlayer::GetActorMove ()
{
  if (!pcActorMove)
  {
    pcActorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  }
}

void BehaviourPlayer::GetMover ()
{
  if (!pcMover)
  {
    pcMover = celQueryPropertyClassEntity<iPcMover> (entity);
  }
}

void BehaviourPlayer::GetMesh ()
{
  if (!pcMesh)
  {
    pcMesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  }
}

bool BehaviourPlayer::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                                   iCelParameterBlock* params, va_list arg)
{
  GetActorMove ();

  if (msg_id == id_pccommandinput_forward1)
  {
    pcActorMove->Forward(true);
  }
  else if (msg_id == id_pccommandinput_forward0)
  {
    pcActorMove->Forward(false);
  }
  else if (msg_id == id_pccommandinput_backward1)
  {
    pcActorMove->Backward(true);
  }
  else if (msg_id == id_pccommandinput_backward0)
  {
    pcActorMove->Backward(false);
  }
  else if (msg_id == id_pccommandinput_rotateleft1)
  {
    pcActorMove->RotateLeft(true);
  }
  else if (msg_id == id_pccommandinput_rotateleft0)
  {
    pcActorMove->RotateLeft(false);
  }
  else if (msg_id == id_pccommandinput_rotateright1)
  {
    pcActorMove->RotateRight(true);
  }
  else if (msg_id == id_pccommandinput_rotateright0)
  {
    pcActorMove->RotateRight(false);
  }
  else if (msg_id == id_pccommandinput_cammode1)
  {
    pcActorMove->ToggleCameraMode();
  }
  else if (msg_id == id_pccommandinput_setposition)
  {
    csRef<iCelHNavStruct> navStruct = behaviourLayer->GetNavStruct();
    if (navStruct)
    {
      GetLinearMovement();
      csVector3 origin = pcLinearMovement->GetPosition();
      csRef<iSector> originSector = pcLinearMovement->GetSector();

      csRef<iMouseDriver> mouse = csQueryRegistry<iMouseDriver>(objectRegistry);
      if (!mouse)
      {
        return false;
      }
      csVector2 screenPoint;
      screenPoint.x = mouse->GetLastX();
      screenPoint.y = mouse->GetLastY();
      
      csRef<iPcCamera> pcCamera = celQueryPropertyClassEntity<iPcCamera> (entity);
      csRef<iCamera> camera = pcCamera->GetCamera();
      csScreenTargetResult st = csEngineTools::FindScreenTarget(screenPoint, 10000.0f, pcCamera->GetView ());
      if (!st.mesh)
	return false;

      csRef<iSectorList> sectorList = st.mesh->GetMovable()->GetSectors();
      if (sectorList->GetCount() != 1)
      {
        return false;
      }
      csVector3 destination = st.isect;
      csRef<iSector> destinationSector = sectorList->Get(0);

      path = navStruct->ShortestPath(origin, originSector, destination, destinationSector);
      behaviourLayer->SetPath(path);
      if (path)
      {
        GetMover();
        csRef<iMapNode> node = path->Next();
        pcMover->MoveTo(node->GetSector(), node->GetPosition(), 0.005f);
      }
    }
  }
  else if (msg_id == id_pcmover_arrived)
  {
    if (path)
    {
      if (path->HasNext())
      {
        GetMover();        
        csRef<iMapNode> node = path->Next();
        pcMover->MoveTo(node->GetSector(), node->GetPosition(), 0.005f);
      }
    }
  }
  else
  {
    return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);;
  }

  return true;
}

const char* BehaviourPlayer::GetName () const
{
  return "playerBehaviour";
}



/*
 * BehaviourBox
 */

BehaviourBox::BehaviourBox (iCelEntity* entity, BehaviourLayer* behaviourLayer, 
                                  iCelPlLayer* physicalLayer, iObjectRegistry* objectRegistry)
    : BehaviourCommon (entity, behaviourLayer, physicalLayer), point1(24.8f, 1.0f, 13.7f), 
      point2(24.8f, 1.0f, 21.3f)
{
  id_box_arrived = physicalLayer->FetchStringID("pcmover_arrived");
  oneIsCurrent = true;

  csRef<iEngine> engine = csLoadPluginCheck<iEngine>(objectRegistry, "crystalspace.engine.3d");
  point1Sector = engine->FindSector("interior");
  point2Sector = point1Sector;
}

void BehaviourBox::GetLinearMovement ()
{
  if (!pcLinearMovement)
  {
    pcLinearMovement = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
  }
}

void BehaviourBox::GetActorMove ()
{
  if (!pcActorMove)
  {
    pcActorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);
  }
}

void BehaviourBox::GetMover ()
{
  if (!pcMover)
  {
    pcMover = celQueryPropertyClassEntity<iPcMover> (entity);
  }
}

void BehaviourBox::GetMesh ()
{
  if (!pcMesh)
  {
    pcMesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  }
}

bool BehaviourBox::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                                   iCelParameterBlock* params, va_list arg)
{
 if (msg_id == id_box_arrived)
  {
    GetMover();
    if (oneIsCurrent)
    {
      pcMover->MoveTo(point2Sector, point2, 0.005f);
    }
    else
    {
      pcMover->MoveTo(point1Sector, point1, 0.005f);
    }
    oneIsCurrent = !oneIsCurrent;
  }
  else
  {
    return BehaviourCommon::SendMessage (msg_id, pc, ret, params, arg);;
  }

  return true;
}

const char* BehaviourBox::GetName () const
{
  return "boxBehaviour";
}
