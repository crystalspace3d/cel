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

#include "physicallayer/entity.h"

#include "behave.h"
#include "frankie.h"
#include "sheep.h"
#include "ram.h"
#include "rat.h"
#include "butterfly.h"

BehaviourLayer::BehaviourLayer (iObjectRegistry* registry, iCelPlLayer* physicalLayer)
  : scfImplementationType (this), physicalLayer (physicalLayer), registry (registry)
{}

BehaviourLayer::~BehaviourLayer ()
{}

iCelBehaviour* BehaviourLayer::CreateBehaviour (iCelEntity* entity,
	const char* name)
{
  csRef<iCelBehaviour> behaviour;
	
  if (!strcmp (name, "sheep_behaviour"))
    behaviour.AttachNew (new SheepBehaviour (entity, this, physicalLayer));

  if (!strcmp (name, "ram_behaviour"))
    behaviour.AttachNew (new RamBehaviour (entity, this, physicalLayer));

  if (!strcmp (name, "rat_behaviour"))
    behaviour.AttachNew (new RatBehaviour (entity, this, physicalLayer));

  if (!strcmp (name, "butterfly_behaviour"))
    behaviour.AttachNew (new ButterFlyBehaviour (entity, this, physicalLayer));

  if (behaviour)
    entity->SetBehaviour(behaviour);

  return behaviour;
}

/**
 * BehaviourCommon
 */

BehaviourCommon::BehaviourCommon (const char* name, iCelEntity* entity,
				  BehaviourLayer* behaviourLayer, 
                                  iCelPlLayer* physicalLayer) 
  : scfImplementationType (this), name (name), entity (entity),
    behaviourLayer (behaviourLayer), physicalLayer (physicalLayer)
{
}

BehaviourCommon::~BehaviourCommon ()
{
}

bool BehaviourCommon::SendMessage (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                                   iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, pc, ret, params, arg);
  va_end (arg);
  return rc;
}

bool BehaviourCommon::SendMessageV (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                                    iCelParameterBlock* params, va_list arg)
{
  csStringID id = physicalLayer->FetchStringID (msg_id);
  return SendMessage (id, pc, ret, params, arg);
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
