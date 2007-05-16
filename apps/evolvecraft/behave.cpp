/*
    Copyright (C) 2006 by Jorrit Tyberghein

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
#include "behave.h"

// CS Includes
#include "csutil/ref.h"
#include "csgeom/sphere.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"
#include "iengine/camera.h"
#include "ivaria/dynamics.h"
#include "csutil/csstring.h"

// CEL Includes
#include "propclass/mechsys.h"
#include "physicallayer/propclas.h"
#include "physicallayer/pl.h"

SCF_IMPLEMENT_IBASE (htBehaviourActor)
  SCF_IMPLEMENTS_INTERFACE (iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

htBehaviourActor::htBehaviourActor(iCelBlLayer* bl, iCelEntity* entity, iCelPlLayer* pl) : bl (bl), entity (entity), evolving (false), bestFitness (0), targetFitness (100000000.0)
{
  SCF_CONSTRUCT_IBASE (0);
  parid_currentticks = pl->FetchStringID("cel.parameter.currentticks");
  parid_maxfitness = pl->FetchStringID("cel.parameter.max_fitness");
}
htBehaviourActor::~htBehaviourActor()
{
  SCF_DESTRUCT_IBASE ();
}

bool htBehaviourActor::SendMessage (const char *msg_id, iCelPropertyClass *pc, celData &ret, iCelParameterBlock *params,...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, pc, ret, params, arg);
  va_end (arg);
  return rc;
}
bool htBehaviourActor::SendMessageV (const char *msg_id, iCelPropertyClass *pc, celData &ret, iCelParameterBlock *params, va_list arg)
{
  if (csString ("pctimer_wakeupframe") == msg_id)
  {
    // during each frame, we pass the inputs to the neural network and
    // translate the outputs into pccraft actions.

    GetNeuralNet ();
    GetEvolve ();
    GetMechObject ();
    GetCraft ();
    GetMesh ();
    GetCamera ();

    currentTime = params->GetParameter(parid_currentticks)->value.l;

    // here we produce the inputs; these are speed, and distance to obstacle
    // along three hitbeams protruding from the front of the craft.
    csVector3 position (pcmechobj->GetBody()->GetPosition());
    float speed = pcmechobj->GetLinearVelocity().Norm();
    float radius = pcmesh->GetMesh()->GetRadius().GetRadius();
    csVector3 start (pcmechobj->LocalToWorld(csVector3 (0, 0, radius)));
    csVector3 end1 (pcmechobj->LocalToWorld(csVector3 (-500, 0, 1000)));
    csVector3 end2 (pcmechobj->LocalToWorld(csVector3 (-250, 0, 1000)));
    csVector3 end3 (pcmechobj->LocalToWorld(csVector3 (0, 0, 1000)));
    csVector3 end4 (pcmechobj->LocalToWorld(csVector3 (250, 0, 1000)));
    csVector3 end5 (pcmechobj->LocalToWorld(csVector3 (500, 0, 1000)));
    iSector *sector = pcdefcamera->GetCamera()->GetSector();
    float hit1 = (sector->HitBeam(start, end1).isect - position).Norm();
    float hit2 = (sector->HitBeam(start, end2).isect - position).Norm();
    float hit3 = (sector->HitBeam(start, end3).isect - position).Norm();
    float hit4 = (sector->HitBeam(start, end4).isect - position).Norm();
    float hit5 = (sector->HitBeam(start, end5).isect - position).Norm();

    // here we pass the inputs to the neural network and get the output.
    celData data;
    data.Set(speed); pcneuralnet->SetInput(0, data);
    data.Set(hit1); pcneuralnet->SetInput(1, data);
    data.Set(hit2); pcneuralnet->SetInput(2, data);
    data.Set(hit3); pcneuralnet->SetInput(3, data);
    data.Set(hit4); pcneuralnet->SetInput(4, data);
    data.Set(hit5); pcneuralnet->SetInput(5, data);
    pcneuralnet->Process();
    float turn = pcneuralnet->GetOutput(0).value.f;

    // here we translate the output into pccraft actions;
    // if turn < -0.25 then turn left; if turn > 0.25 then turn right.
    pccraft->ThrustOn();
    if (turn < -0.25)
    {
      pccraft->StartTurnLeft();
      pccraft->StopTurnRight();
    }
    else
    {
      pccraft->StopTurnLeft();
      if (turn > 0.25) pccraft->StartTurnRight();
      else pccraft->StopTurnRight();
    }
    return true;
  }
  else if (csString ("pctimer_wakeup") == msg_id)
  {
    // once per second, we do a few checks to make sure it's running smoothly.

    GetNeuralNet ();
    GetEvolve ();
    GetMechObject ();
    GetCraft ();
    GetMesh ();
    GetCamera ();

    if (evolving)
    {
      // if the craft gets stuck, or falls off the edge of the map, then
      // give it a fitness of zero and move on to the next genome.

      csVector3 currentPosition (pcmechobj->GetBody()->GetPosition());
      float altitude = (currentPosition - initPosition).y;
      currentPosition.y = 0;
      float distanceTravelled = (currentPosition - pollPosition).Norm();
      if (pollPosition != initPosition)
      {
	// if the distance travelled since the last time we checked is less
	// than some smallish value, then it's got stuck; if the altitude
	// is too huge (+ve or -ve), then it's fallen out of the level.

	if (distanceTravelled < 1.0 || fabs(altitude) > 500.0)
	{
	  pcevolve->FitnessCallback(0.0);
	}
      }
      pollPosition = currentPosition;
    }
    else
    {
      if (pcevolve.IsValid() && bestFitness < targetFitness)
      {
	// if we're not currently evolving, and we haven't reached our target
	// fitness yet, then start evolving
        evolving = true;
        pcevolve->Generate();
      }
    }
    return true;
  }
  else if (csString ("pcevolve_fitness") == msg_id)
  {
    // the pcevolve is asking us to begin evaluating the fitness of the craft,
    // so we position it at the start point and initialize some values.

    pcmechobj->GetBody ()->SetOrientation (initOrientation);
    pcmechobj->GetBody ()->SetPosition (initPosition);
    pcmechobj->GetBody ()->SetLinearVelocity (csVector3 (0));
    pcmechobj->GetBody ()->SetAngularVelocity (csVector3 (0));
    startTime = currentTime;
    pollPosition = initPosition;
    return true;
  }
  else if (csString ("pcdynamicbody_collision") == msg_id)
  {
    // a collision! we've finished evaluating the fitness of the craft;
    // its fitness is based on how far it got from the start point before
    // the collision.

    if (evolving)
    {
      csTicks timeTaken = currentTime - startTime;
      csVector3 currentPosition (pcmechobj->GetBody()->GetPosition());
      float distanceTravelled = (currentPosition - initPosition).Norm();
      float fitness = distanceTravelled * timeTaken;
      pcevolve->FitnessCallback(fitness);
    }
    return true;
  }
  else if (csString ("pccommandinput_unfit1") == msg_id)
  {
    if (pcevolve)
    {
      pcevolve->FitnessCallback(0.0);
    }
    else
    {
      pcmechobj->GetBody ()->SetOrientation (initOrientation);
      pcmechobj->GetBody ()->SetPosition (initPosition);
      pcmechobj->GetBody ()->SetLinearVelocity (csVector3 (0));
      pcmechobj->GetBody ()->SetAngularVelocity (csVector3 (0));
    }
    return true;
  }
  else if (csString ("pcevolve_result") == msg_id)
  {
    // we've finished evolving one generation.

    evolving = false;
    float fitness = params->GetParameter(parid_maxfitness)->value.f;
    if (fitness > bestFitness)
    {
      bestFitness = fitness;
      pcevolve->SelectGenome(0);
      pcneuralnet->CacheWeights("evolvecraft", ~0);
    }
    printf("fitness: %f; bestFitness: %f\n", fitness, bestFitness);
    return true;
  }

  return false;
}

iPcMechanicsObject* htBehaviourActor::GetMechObject ()
{
  if (!pcmechobj)
  {
    pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity, iPcMechanicsObject);
    CS_ASSERT (pcmechobj != 0);

    // store the craft's starting position and orientation
    initOrientation = pcmechobj->GetBody ()->GetOrientation ();
    initPosition = pcmechobj->GetBody ()->GetPosition ();
    initPosition.y -= 6.0;
  }
  return pcmechobj;
}

iPcCraftController* htBehaviourActor::GetCraft ()
{
  if (!pccraft)
  {
    pccraft = CEL_QUERY_PROPCLASS_ENT (entity, iPcCraftController);
    CS_ASSERT (pccraft != 0);
  }
  return pccraft;
}

iPcDefaultCamera* htBehaviourActor::GetCamera ()
{
  if (!pcdefcamera)
  {
    pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcDefaultCamera);
    CS_ASSERT (pcdefcamera != 0);
  }
  return pcdefcamera;
}

iPcNeuralNet* htBehaviourActor::GetNeuralNet ()
{
  if (!pcneuralnet)
  {
    pcneuralnet = CEL_QUERY_PROPCLASS_ENT (entity, iPcNeuralNet);
    CS_ASSERT (pcneuralnet != 0);
  }
  return pcneuralnet;
}

iPcEvolve* htBehaviourActor::GetEvolve ()
{
  if (!pcevolve)
  {
    pcevolve = CEL_QUERY_PROPCLASS_ENT (entity, iPcEvolve);
  }
  return pcevolve;
}

iPcMesh* htBehaviourActor::GetMesh ()
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    CS_ASSERT (pcmesh != 0);
  }
  return pcmesh;
}
