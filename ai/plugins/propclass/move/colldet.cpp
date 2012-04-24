/*
  Crystal Space Entity Layer
  Copyright (C) 2001 PlaneShift Team <info@planeshift.it>
  Copyright (C) 2001-2003 by Jorrit Tyberghein

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

/*
 * This code is heavily based on pslinmove from the PlaneShift project.
 * Thanks a lot for making this!
 */

#include <cssysdef.h>

//CS includes
#include <iutil/objreg.h>
#include <iutil/event.h>
#include <iutil/eventq.h>
#include <iutil/evdefs.h>
#include <iutil/virtclk.h>

#include <imesh/sprite3d.h>

#include <csutil/databuf.h>
#include <csutil/plugmgr.h>
#include <iengine/movable.h>
#include <iengine/mesh.h>
#include <iengine/engine.h>
#include <iengine/sector.h>
#include <cstool/collider.h>
#include <ivaria/collider.h>

#include <imesh/terrain.h>
#include <imesh/objmodel.h>

//CEL includes
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "propclass/camera.h"
#include "propclass/solid.h"

#include "plugins/propclass/move/colldet.h"

CEL_IMPLEMENT_FACTORY_ALT (CollisionDetection, "pcobject.mesh.collisiondetection",
    "pccollisiondetection")

//----------------------------------------------------------------------------

celPcCollisionDetection::celPcCollisionDetection (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  colliderActorReady = false;
  cdsys = csQueryRegistry<iCollideSystem> (object_reg);
  if (!cdsys)
  {
    Error ("iCollideSystem missing!");
    return;
  }
  collider_actor.SetCollideSystem (cdsys);
  collider_actor.SetGravity (19.2f);

  engine = csQueryRegistry<iEngine> (object_reg);
  if (!engine)
  {
    Error ("iEngine missing!");
    return;
  }
  collider_actor.SetEngine (engine);

  pcmesh = 0;
}

celPcCollisionDetection::~celPcCollisionDetection ()
{
}

bool celPcCollisionDetection::AdjustForCollisions (csVector3& oldpos,
                                                   csVector3& newpos,
                                                   csVector3& vel,
                                                   float delta,
                                                   iMovable* /*movable*/)
{
  if (!useCD)
    return true;

  if (!InitActor ()) return true;
  return collider_actor.AdjustForCollisions (oldpos, newpos,
  	vel, delta);
}


bool celPcCollisionDetection::Init ()
{
  return Init (topSize, bottomSize, shift);
}

bool celPcCollisionDetection::Init (const csVector3& body,
	const csVector3& legs, const csVector3& _shift)
{
  if (!pcmesh)
  {
    csRef<iPcMesh> pcmeshref;
    pcmeshref = celQueryPropertyClassEntity<iPcMesh> (entity);

    if (!pcmeshref)
      return Error ("Colldet: No Mesh found on entity!");
    else
      pcmesh = pcmeshref; // Pull ptr out of csRef
  }

  topSize = body;
  bottomSize = legs;
  celPcCollisionDetection::shift = _shift;
  colliderActorReady = false;
  useCD = true;
  return true;
}

bool celPcCollisionDetection::InitActor ()
{
  if (colliderActorReady) return true;
  if (!pcmesh->GetMesh ()) return false;
  collider_actor.InitializeColliders (pcmesh->GetMesh (),
  	bottomSize, topSize, shift);
  colliderActorReady = true;
  return true;
}

iCollider* celPcCollisionDetection::FindCollider (iObject* object)
{
  csColliderWrapper* wrap = csColliderWrapper::GetColliderWrapper (object);
  if (wrap)
  {
    return wrap->GetCollider ();
  }
  else
  {
    // There is no collider wrapper. In this case we test if there
    // is a pcsolid for the entity. If not we'll add a null collider.
    iCelEntity* ent = pl->FindAttachedEntity (object);
    if (ent)
    {
      csRef<iPcSolid> pcsolid = celQueryPropertyClassEntity<iPcSolid> (ent);
      if (pcsolid)
      {
	// Calling pcsolid->GetCollider() will cause a csColliderWrapper
	// to be created.
        return pcsolid->GetCollider ();
      }
    }
    // Add a null collider so we don't check for pcsolid again.
    csRef<csColliderWrapper> cw = csPtr<csColliderWrapper> (
	  	new csColliderWrapper (object, cdsys, (iCollider*)0));
  }
  return 0;
}

