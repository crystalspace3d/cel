/*
    Copyright (C) 2012 by Christian Van Brussel

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
#include "iutil/objreg.h"

#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/pl.h"
#include "propclass/actormove.h"
#include "propclass/camera.h"
#include "propclass/linmove.h"
#include "propclass/mesh.h"
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
  if (!strcmp(name, "playerBehaviour"))
    behaviour.AttachNew (new BehaviourPlayer(entity, this, physicalLayer, objectRegistry));

  if (behaviour)
    entity->SetBehaviour(behaviour);

  return behaviour;
}

const char* BehaviourLayer::GetName () const
{
  return "behaviourlayer";
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

bool BehaviourCommon::SendMessage (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                                   iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start(arg, params);
  bool rc = SendMessageV(msg_id, pc, ret, params, arg);
  va_end(arg);
  return rc;
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
}

bool BehaviourPlayer::SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                                   iCelParameterBlock* params, va_list arg)
{
  csRef<iPcActorMove> pcActorMove = celQueryPropertyClassEntity<iPcActorMove> (entity);

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
