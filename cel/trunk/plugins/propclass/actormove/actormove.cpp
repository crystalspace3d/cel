/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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
#include "plugins/propclass/actormove/actormove.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "csutil/debug.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivaria/reporter.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"
#include "iengine/mesh.h"
#include "imesh/object.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (ActorMove, "pcactormove")

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcActorMove)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcActorMove)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcActorMove::PcActorMove)
  SCF_IMPLEMENTS_INTERFACE (iPcActorMove)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcActorMove::celPcActorMove (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcActorMove);
  movement_speed = 2.0f;
  running_speed = 5.0f;
  rotating_speed = 1.75f;
  jumping_velocity = 6.31f;
  forward = false;
  backward = false;
  strafeleft = false;
  straferight = false;
  rotateleft = false;
  rotateright = false;
  running = false;
  autorun = false;
  checked_spritestate = false;
}

celPcActorMove::~celPcActorMove ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcActorMove);
}

#define ACTORMOVE_SERIAL 1

csPtr<iCelDataBuffer> celPcActorMove::Save ()
{
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (ACTORMOVE_SERIAL));
  databuf->SetDataCount (0);
  return csPtr<iCelDataBuffer> (databuf);
}


bool celPcActorMove::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != ACTORMOVE_SERIAL) return false;
  if (databuf->GetDataCount () != 0) return false;
  return true;
}

void celPcActorMove::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pccamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcCamera);
    checked_spritestate = false;
  }
}

void celPcActorMove::GetSpriteStates ()
{
  if (checked_spritestate) return;
  if (!pcmesh) return;
  iMeshWrapper* m = pcmesh->GetMesh ();
  if (!m) return;
  iMeshObject* o = m->GetMeshObject ();
  if (!o) return;
  checked_spritestate = true;
  sprcal3d = SCF_QUERY_INTERFACE (o, iSpriteCal3DState);
  if (sprcal3d) return;
  spr3d = SCF_QUERY_INTERFACE (o, iSprite3DState);
}

void celPcActorMove::HandleMovement (bool jump)
{
  FindSiblingPropertyClasses ();
  if (!pccamera)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		    "cel.pcactormove", "pccamera is missing!");
    return;
  }
  if (!pclinmove)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		    "cel.pcactormove", "pclinmove is missing!");
    return;
  }
  GetSpriteStates ();
  if (!pcmesh)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		    "cel.pcactormove", "pcmesh is missing!");
    return;
  }

  float speed;
  if (running)
    speed = running_speed;
  else
    speed = movement_speed;

  csVector3 velocity;
  pclinmove->GetVelocity (velocity);

  if ((autorun || forward) && straferight)
  {
    velocity.x = -0.75 * speed;
    velocity.z = -0.75 * speed;
  }
  else if ((autorun || forward) && strafeleft)
  {
    velocity.x = 0.75 * speed;
    velocity.z = -0.75 * speed;
  }
  else if (autorun || forward)
  {
    velocity.x = 0;
    velocity.z = -speed;
  }
  else if (backward && straferight)
  {
    velocity.x = -0.75 * speed;
    velocity.z = 0.75 * speed;
  }
  else if (backward && strafeleft)
  {
    velocity.x = 0.75 * speed;
    velocity.z = 0.75 * speed;
  }
  else if (backward)
  {
    velocity.x = 0;
    velocity.z = speed;
  }
  else if (straferight)
  {
    velocity.x = -speed;
    velocity.z = 0;
  }
  else if (strafeleft)
  {
    velocity.x = speed;
    velocity.z = 0;
  }
  else
  {
    velocity.x = 0;
    velocity.z = 0;
  }
  pclinmove->SetVelocity (velocity);
  if (sprcal3d) sprcal3d->SetVelocity (velocity.z);
  // @@@ do spr3d!

  if (rotateright)
    pclinmove->SetRotation (csVector3 (0, -rotating_speed, 0));
  else if (rotateleft)
    pclinmove->SetRotation (csVector3 (0, rotating_speed, 0));
  else
    pclinmove->SetRotation (csVector3 (0));

  if (jump && pclinmove->IsOnGround ())
  {
    velocity.y = jumping_velocity;
    pclinmove->SetVelocity (velocity);
    if (sprcal3d) sprcal3d->SetVelocity (velocity.z);
    // @@@ do spr3d!
  }
}

void celPcActorMove::ToggleCameraMode ()
{
  FindSiblingPropertyClasses ();
  if (!pccamera)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		    "cel.pcactormove", "pccamera is missing!");
    return;
  }
  pccamera->SetMode (pccamera->GetNextMode ());
}

//---------------------------------------------------------------------------

