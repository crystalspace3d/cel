/*
  Crystal Space Entity Layer
  Copyright (C) 2001 PlaneShift Team (info@planeshift.it,
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

#include <csutil/debug.h>
#include <csutil/databuf.h>
#include <csutil/plugmgr.h>
#include <csengine/movable.h>
#include <csengine/meshobj.h>
#include <csengine/engine.h>
#include <cstool/collider.h>
#include <ivaria/collider.h>

#include <imesh/thing/thing.h>
#include <imesh/thing/polygon.h>
#include <igeom/polymesh.h>
#include <igeom/objmodel.h>

//CEL includes
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "propclass/camera.h"
#include "propclass/solid.h"

#include "plugins/propclass/move/linmove.h"

extern void Report (iObjectRegistry* object_reg, const char* msg, ...);

CEL_IMPLEMENT_FACTORY (LinearMovement, "pclinearmovement")


SCF_IMPLEMENT_IBASE_EXT (celPcLinearMovement)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcLinearMovement)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_IBASE (celPcLinearMovement::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcLinearMovement::PcLinearMovement)
  SCF_IMPLEMENTS_INTERFACE (iPcLinearMovement)
SCF_IMPLEMENT_EMBEDDED_IBASE_END


// collision detection variables
#define MAXSECTORSOCCUPIED  20
csCollisionPair celPcLinearMovement::our_cd_contact[1000];
int celPcLinearMovement::num_our_cd;

#define LEGOFFSET	  0

// This is the distance the CD will use to look for objects to collide with.
#define COLLISION_DISTANCE  5

// Set this in order to see what meshes the player is colliding with
//#define SHOW_COLLIDER_MESH_DEBUG

/*
 * This dumps 2 lines for every cross-sector CD that fails a sector-collision
 * test. It's not that bad since this shouldn't happen that often.
 *
 *  The two cases where it should happen are:
 *  1) A mesh crosses a portal.  In this case the mesh should exist in both
 *   sectors or a copy of the mesh exist in each sector or the mesh should
 *   be "cut" into two meshes at the portal boundary. There isn't too much
 *   difference between these three from a performance perspective.
 *  2) A mesh is in a sector, but is entirely on the other side of a portal.
 *   This mesh should probably not be in the sector it's in at all. It will
 *   not be collided with, and probably wont be rendered either!
 *
 * If this message comes up, a collision did NOT occur as far as we're
 * concerned. We found a collision, but we can't reach the "space" that this
 * collision occured in, so it's ignored.
 */
//#define LINMOVE_CD_FOLLOWSEG_DEBUG


/*
 * This value is used in movement limiting/cheat detection.
 * It is the maximum ms of latency that a client is presumed to have.
 */
#define MAX_ACCUMULATED_LAG 5000

//----------------------------------------------------------------------------

celPcLinearMovement::celPcLinearMovement (iObjectRegistry* object_reg)
  :celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcLinearMovement);

  scfiEventHandler = new EventHandler (this);
  csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  q->RegisterListener (scfiEventHandler, ~CSMASK_Broadcast);

  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  if (!vc) Report (object_reg, "iVirtualClock Missing!");

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (!engine)
  {
    Report (object_reg, "Engine missing!");
    return;
  }

  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
  if (!cdsys)
  {
    Report (object_reg, "iCollideSystem missing!");
    return;
  }

  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  if (!pl)
  {
    Report (object_reg, "Physical layer missing!");
    return;
  }

  vel.x = vel.z = 0;
  vel.y = 0;
  angularVelocity.x = angularVelocity.y = angularVelocity.z  = 0;
  camera_pitchspeed = 0;

  topCollider = 0;
  bottomCollider = 0;

  pcmesh = 0;

  DG_TYPE (this, "celPcLinearMovement ()");

  useCD = false;
  onground = false;
  ready = false;
  /*
   * Speed affects all aspects of movement, including gravity.
   * It's effectively a comparative rate of "time"
   */
  speed = 1.0f;

  lastDRUpdate = csGetTicks ();
  accumulatedLag=MAX_ACCUMULATED_LAG;
}

celPcLinearMovement::~celPcLinearMovement ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q != 0)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
}

#define LINMOVE_SERIAL 2

csPtr<iCelDataBuffer> celPcLinearMovement::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (LINMOVE_SERIAL);
  databuf->SetDataCount (3);
  int j = 0;

  databuf->GetData (j++)->Set (topSize);
  databuf->GetData (j++)->Set (bottomSize);
  databuf->GetData (j++)->Set (shift);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcLinearMovement::Load (iCelDataBuffer* databuf)
{
  int seriallnr = databuf->GetSerialNumber ();
  if (seriallnr != LINMOVE_SERIAL)
    return false;
  if (databuf->GetDataCount () != 3)
    return false;

  celData* cd;
  cd = databuf->GetData (0);
  topSize.x = cd->value.v.x;
  topSize.y = cd->value.v.y;
  topSize.z = cd->value.v.z;

  cd = databuf->GetData (1);
  bottomSize.x = cd->value.v.x;
  bottomSize.y = cd->value.v.y;
  bottomSize.z = cd->value.v.z;

  cd = databuf->GetData (2);
  shift.x = cd->value.v.x;
  shift.y = cd->value.v.y;
  shift.z = cd->value.v.z;

  if (!InitCD (topSize, bottomSize, shift))
    return false;

  return true;
}

static inline int FindIntersection (const csCollisionPair& cd,
    			   csVector3 line[2])
{
  csVector3 tri1[3]; tri1[0]=cd.a1; tri1[1]=cd.b1; tri1[2]=cd.c1;
  csVector3 tri2[3]; tri2[0]=cd.a2; tri2[1]=cd.b2; tri2[2]=cd.c2;

  return csMath3::FindIntersection (tri1, tri2, line);
}

void celPcLinearMovement::SetRotation (const csVector3& angleVel)
{
  angularVelocity = angleVel;
}

void celPcLinearMovement::SetVelocity (const csVector3& vel)
{
  /*
   * Y movement is jumping, flight (lift) and gravity effects
   * Take care to check IsOnGround () before calling this for jumping
   */
  celPcLinearMovement::vel.y  = vel.y;
  celPcLinearMovement::vel.x  = vel.x;
  celPcLinearMovement::vel.z  = vel.z;
}

void celPcLinearMovement::GetVelocity (csVector3& v)
{
  v = vel;
}


void celPcLinearMovement::SetSpeed (float speedZ)
{
  speed = speedZ;
}

//Does the actual rotation
bool celPcLinearMovement::RotateV (float delta)
{
  if (camera_pitchspeed)
  {
    csRef<iPcCamera> pccamera;
    pccamera = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcCamera);
    if (pccamera)
    {
      float angle = pccamera->GetPitch ();
      angle += camera_pitchspeed*speed;
      pccamera->SetPitch (MIN (MAX (angle,-3.14159/2),3.14159/2) );
    }
  }

  if (angularVelocity < SMALL_EPSILON)
    return false;

  //delta *= speed;
  csVector3 angle = angularVelocity * delta;
  csMatrix3 rotMat (1,0,0,0,1,0,0,0,1);

  float ca = 0;
  float sa = 0;

  if (angle.y != 0 )
  {
    ca = cos (angle.y);
    sa = sin (angle.y);
  }

  rotMat = rotMat * csMatrix3 (ca, 0, -sa,
    0, 1, 0,
    sa,0, ca);

  pcmesh->GetMesh ()->GetMovable ()->Transform (rotMat);
  return true;
}

/*
 * Terminal velocity
 * ((120 miles/hour  / 3600 second/hour) * 5280 feet/mile)
 *     / 3.28 feet/meter = 53.65 m/s
 *   *2.0 for "feel" correction = 107.3m/s
 */
#define ABS_MAX_FREEFALL_VELOCITY 107.3f

/*
 * MAX_CD_INTERVAL is now the maximum amount of time that should pass in a
 * single step regardless of the current velocity.
 * Since acceleration is factored into the velocity each step, this
 * shouldn't be too large or you'll get unrealistic gravity
 * in some situations.
 */
#define MAX_CD_INTERVAL 1

/*
 * MIN_CD_INTERVAL is the minimum amount of time that can pass in a single
 * step through the collision detection and movement process.
 *
 * This is basically a protection against 0 time (you will never move) and
 * negative time (you will move the opposite direction until you hit an
 * obstruction.
 */
#define MIN_CD_INTERVAL 0.01


bool celPcLinearMovement::MoveSprite (float delta)
{
  float local_max_interval;
  bool rc = false;

  // Make sure time moves forward at least at this rate
  local_max_interval =
    MAX (MIN (MIN ((vel.y==0.0f)
  	? MAX_CD_INTERVAL
	: ABS (topSize.y/vel.y), (vel.x==0.0f)
		? MAX_CD_INTERVAL
		: ABS (topSize.x/vel.x)), (vel.z==0.0f)
			? MAX_CD_INTERVAL
			: ABS (topSize.z/vel.z)), MIN_CD_INTERVAL);
  // Compensate for speed
  local_max_interval /= speed;
  // Err on the side of safety
  local_max_interval -= 0.005f;

  if (useCD)
  {
    while (delta > local_max_interval)
    {
      if (MoveV (local_max_interval))
    	rc = true;
      else // Can't move anymore
    	return rc;
      delta -= local_max_interval;
      local_max_interval = MAX (MIN (MIN ((vel.y==0.0f)
      	? MAX_CD_INTERVAL
	: ABS (topSize.y/vel.y), (vel.x==0.0f)
		? MAX_CD_INTERVAL
		: ABS (topSize.x/vel.x)), (vel.z==0.0f)
			? MAX_CD_INTERVAL
			: ABS (topSize.z/vel.z)), MIN_CD_INTERVAL);
      // Compensate for speed
      local_max_interval /= speed;
      // Err on the side of safety
      local_max_interval -= 0.005f;
    }
  }

  if (!useCD || delta)
  {
    if (MoveV (delta))
      rc = true;
  }
  return rc;
}


// Do the actual move
bool celPcLinearMovement::MoveV (float delta)
{
  if (vel < SMALL_EPSILON && onground)
    return false;  // didn't move anywhere

  delta *= speed;
  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  csMatrix3 mat;

  csReversibleTransform rt = movable->GetFullTransform ();
  mat = rt.GetT2O ();

  csVector3 oldpos = movable->GetPosition ();
  csVector3 newpos;

  newpos = mat*(vel*delta) + oldpos;

  //---------- COLLISION DETECTION STARTS HERE-------------------
  if (useCD)
  {
    int hits, i;
    csMatrix3 identitymatrix;
    csOrthoTransform transform_newpos (identitymatrix, newpos);
    iSector* current_sector=movable->GetSectors ()->Get (0);

    // We need to measure slightly above the position of the actor or else
    // we won't really cross a portal.
    csVector3 temppos = oldpos;
    //temppos.y+=0.1f;
    csOrthoTransform transform_oldpos = csReversibleTransform (
    	csMatrix3(), temppos);

    // Part1: find body collisions => movement
    // Find possible colliding sectors.
    csVector3 localvel = mat*(vel * delta);

    num_our_cd = hits = 0;

    // Travel all relevant sectors and do collision detection.
    cdsys->SetOneHitOnly (false);
    cdsys->ResetCollisionPairs ();
    hits += CollisionDetect (topCollider, current_sector,
    	&transform_newpos, &transform_oldpos);

    for (i = 0; i < num_our_cd; i++ )
    {
      csCollisionPair& cd = our_cd_contact[i];
      csVector3 vec = ((cd.c2-cd.b2)%(cd.b2-cd.a2)).Unit ();
      if (vec * localvel < 0 )
    	continue;

      localvel = -(localvel % vec) % vec;
    }
    newpos = oldpos + localvel;

    // Part2: legs
    num_our_cd = hits = 0;
    transform_newpos = csOrthoTransform (csMatrix3(), newpos);

    cdsys->SetOneHitOnly (false);
    cdsys->ResetCollisionPairs ();	
    hits += CollisionDetect (bottomCollider, current_sector,
    	&transform_newpos, &transform_oldpos);

    if (hits == 0)
    {
      // No hits on the bottom bound, we're falling.
      onground = false;
      // gravity! move down!
      vel.y  -= 19.6 * delta;
      /*
       * Terminal velocity
       *   ((120 miles/hour  / 3600 second/hour) * 5280 feet/mile)
       *   / 3.28 feet/meter = 53.65 m/s
       */
      if (vel.y < -(ABS_MAX_FREEFALL_VELOCITY))
    	vel.y = -(ABS_MAX_FREEFALL_VELOCITY);

    }
    else
    {
      float max_y = -1e10;

      for (i = 0; i < num_our_cd; i++ )
      {
    	csCollisionPair cd = our_cd_contact[i];
    	csVector3 n = ((cd.c2-cd.b2)%(cd.b2-cd.a2)).Unit ();

    	// Is it a collision with a ground polygon?
    	//  (this tests for the angle between ground and collidet
    	//  triangle)
    	if (-n.y < 0.7)
    	  continue;

    	csVector3 line[2];
    	cd.a1 += newpos;
    	cd.b1 += newpos;
    	cd.c1 += newpos;
    	if (FindIntersection (cd,line))
    	{
    	  if (line[0].y > max_y)
	    max_y = line[0].y;
    	  if (line[1].y > max_y)
	    max_y = line[1].y;
    	}
      }

      float p = newpos.y - max_y + 0.01;
      if (ABS (p) < bottomSize.y - 0.01)
      {
    	if (max_y != -1e10)
    	  newpos.y = max_y - 0.01;

    	if (vel.y < 0 )
    	  vel.y = 0;
      }

      onground = true;
    }

    // Part 2.5: check top again and revert move if we're still in a wall.
    num_our_cd = hits = 0;
    cdsys->SetOneHitOnly (false);
    cdsys->ResetCollisionPairs ();
    transform_newpos = csOrthoTransform (csMatrix3(), newpos);

    if (CollisionDetect (topCollider, current_sector,
    	&transform_newpos,&transform_oldpos) > 0)
    {
      newpos = oldpos;
    }
    else
    {
      // Part 3: portal traversal.
      iSector* new_sector = movable->GetSectors ()->Get (0);
      iSector* old_sector = new_sector;

      temppos = newpos;
      // @@@ Jorrit: had to do this add!
      float height5 = (bottomSize.y + topSize.y) / 20.0;
      temppos.y += height5;
      transform_oldpos.SetOrigin (transform_oldpos.GetOrigin ()
      	+ csVector3 (0, height5, 0));
      bool mirror = false;

      new_sector = new_sector->FollowSegment (transform_oldpos,
      	temppos, mirror, CEL_LINMOVE_FOLLOW_ONLY_PORTALS);
      if (new_sector != old_sector)
    	movable->SetSector (new_sector);
    }

  }

  //---------- COLLISION END HERE-------------------

  // move to the new position
  movable->GetTransform ().SetOrigin (newpos);

  if (useCD)
  {
    // Part 4: Add us to all nearby sectors.
    pcmesh->GetMesh ()->PlaceMesh ();
  }

  movable->UpdateMove ();

  return true;
}

void celPcLinearMovement::UpdateDR (csTicks ticks)
{
  float delta = ticks - lastDRUpdate;
  delta /= 1000;
  ExtrapolatePosition (delta);
  lastDRUpdate = ticks;
}

void celPcLinearMovement::ExtrapolatePosition (float delta)
{
  bool rc = MoveSprite (delta);
  rc = RotateV (delta) || rc;
  if (rc)
  {
    pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();
    pcmesh->GetMesh ()->DeferUpdateLighting (
    	CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 5);
  }
}

bool celPcLinearMovement::HandleEvent (iEvent& ev)
{
  if (!pcmesh)
  {
    csRef<iPcMesh> pcmeshref;
    pcmeshref = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
    if (!pcmeshref)
    {
      Report (object_reg, "No Mesh found on entity!");
      return false;
    }
    pcmesh = pcmeshref; // Get out of csRef refcounting and just save ptr
  }

  if (ev.Type != csevBroadcast || ev.Command.Code != cscmdPreProcess)
    return false;

  csTicks elapsed_time = vc->GetElapsedTicks ();
  if (!elapsed_time)
    return false;

  float delta = elapsed_time/1000.0;

  // Adjust the properties.
  ExtrapolatePosition (delta);

  return false;
}

bool celPcLinearMovement::InitCD ()
{
  return InitCD (topSize, bottomSize, shift);
}

bool celPcLinearMovement::InitCD (const csVector3& body, const csVector3& legs,
	const csVector3& shift)
{
  if (!pcmesh)
  {
    csRef<iPcMesh> pcmeshref;
    pcmeshref = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);

    if (!pcmeshref)
    {
      Report (object_reg, "No Mesh found on entity!");
      return false;
    }
    else
    {
      pcmesh = pcmeshref; // Pull ptr out of csRef
    }
  }

  topSize = body;
  bottomSize = legs;
  celPcLinearMovement::shift = shift;

  csRef<iPolygon3DStatic> p;
  csRef<iPolygonMesh> mesh;

  csRef<iPluginManager> plugin_mgr = CS_QUERY_REGISTRY (object_reg,
    iPluginManager);

  csRef<iMeshObjectType> ThingType;
  ThingType = CS_QUERY_PLUGIN_CLASS (plugin_mgr,
    "crystalspace.mesh.object.thing",
    iMeshObjectType);

  if (!ThingType)
    ThingType = CS_LOAD_PLUGIN (plugin_mgr,
      "crystalspace.mesh.object.thing",
      iMeshObjectType);

  csRef<iMeshObjectFactory> thingfact = ThingType->NewFactory ();
  csRef<iMeshObject> meshobject = thingfact->NewInstance ();

  topColliderMesh = engine->CreateMeshWrapper (meshobject,"Top Collider Mesh");
  csRef<iThingState> ws = SCF_QUERY_INTERFACE (meshobject, iThingState);
  csRef<iThingFactoryState> thingState = ws->GetFactory ();

  float bX2 = body.x / 2.0f;
  float bZ2 = body.z / 2.0f;
  float bYbottom = legs.y;
  float bYtop = legs.y + body.y;

  thingState->CreateVertex (csVector3 (-bX2, bYbottom, -bZ2) + shift);
  thingState->CreateVertex (csVector3 (-bX2, bYbottom,  bZ2) + shift);
  thingState->CreateVertex (csVector3 (-bX2, bYtop,   bZ2) + shift);
  thingState->CreateVertex (csVector3 (-bX2, bYtop,  -bZ2) + shift);
  thingState->CreateVertex (csVector3 ( bX2, bYbottom, -bZ2) + shift);
  thingState->CreateVertex (csVector3 ( bX2, bYbottom,  bZ2) + shift);
  thingState->CreateVertex (csVector3 ( bX2, bYtop,   bZ2) + shift);
  thingState->CreateVertex (csVector3 ( bX2, bYtop,  -bZ2) + shift);

  //Left side
  p = thingState->CreatePolygon ();
  p->CreateVertex (0); p->CreateVertex (1);
  p->CreateVertex (2); p->CreateVertex (3);

  //Right side
  p = thingState->CreatePolygon ();
  p->CreateVertex (4); p->CreateVertex (5);
  p->CreateVertex (6); p->CreateVertex (7);

  //Bottom
  p = thingState->CreatePolygon ();
  p->CreateVertex (0); p->CreateVertex (1);
  p->CreateVertex (5); p->CreateVertex (4);

  //Top
  p = thingState->CreatePolygon ();
  p->CreateVertex (3); p->CreateVertex (2);
  p->CreateVertex (6); p->CreateVertex (7);

  //Front
  p = thingState->CreatePolygon ();
  p->CreateVertex (1); p->CreateVertex (5);
  p->CreateVertex (6); p->CreateVertex (2);

  //Back
  p = thingState->CreatePolygon ();
  p->CreateVertex (0); p->CreateVertex (4);
  p->CreateVertex (7); p->CreateVertex (3);

  mesh = meshobject->GetObjectModel ()->GetPolygonMeshColldet ();
  topCollider = csPtr<csColliderWrapper> (
    new csColliderWrapper (topColliderMesh->QueryObject (), cdsys, mesh));

  topCollider->SetName ("Top Collider");

  thingfact = ThingType->NewFactory ();
  meshobject = thingfact->NewInstance ();

  bottomColliderMesh = engine->CreateMeshWrapper (meshobject, "Bottom");
  ws = SCF_QUERY_INTERFACE (meshobject, iThingState);
  thingState = ws->GetFactory ();

  float lX2 = legs.x / 2.0f;
  float lZ2 = legs.z / 2.0f;

  thingState->CreateVertex (csVector3 (-lX2, LEGOFFSET, -lZ2) + shift);
  thingState->CreateVertex (csVector3 (-lX2, LEGOFFSET,  lZ2) + shift);
  thingState->CreateVertex (csVector3 (-lX2, LEGOFFSET + legs.y,   lZ2) + shift);
  thingState->CreateVertex (csVector3 (-lX2, LEGOFFSET + legs.y,  -lZ2) + shift);
  thingState->CreateVertex (csVector3 ( lX2, LEGOFFSET, -lZ2) + shift);
  thingState->CreateVertex (csVector3 ( lX2, LEGOFFSET,  lZ2) + shift);
  thingState->CreateVertex (csVector3 ( lX2, LEGOFFSET + legs.y,   lZ2) + shift);
  thingState->CreateVertex (csVector3 ( lX2, LEGOFFSET + legs.y,  -lZ2) + shift);

  //Left
  p = thingState->CreatePolygon ();
  p->CreateVertex (0); p->CreateVertex (1);
  p->CreateVertex (2); p->CreateVertex (3);

  //Right
  p = thingState->CreatePolygon ();
  p->CreateVertex (4); p->CreateVertex (5);
  p->CreateVertex (6); p->CreateVertex (7);

  //Bottom
  p = thingState->CreatePolygon ();
  p->CreateVertex (0); p->CreateVertex (1);
  p->CreateVertex (5); p->CreateVertex (4);

  //Top
#if 0
  p = thingState->CreatePolygon ();
  p->CreateVertex (3); p->CreateVertex (2);
  p->CreateVertex (6); p->CreateVertex (7);
#endif

  //Front
  p = thingState->CreatePolygon ();
  p->CreateVertex (1); p->CreateVertex (5);
  p->CreateVertex (6); p->CreateVertex (2);

  //Back
  p = thingState->CreatePolygon ();
  p->CreateVertex (0); p->CreateVertex (4);
  p->CreateVertex (7); p->CreateVertex (3);

  mesh = meshobject->GetObjectModel ()->GetPolygonMeshColldet ();
  bottomCollider = csPtr<csColliderWrapper> (
    new csColliderWrapper (bottomColliderMesh->QueryObject (),
    	cdsys, mesh));

  bottomCollider->SetName ("Bottom Collider");

  bool result;
  if (topCollider && bottomCollider)
    result = true;
  else
    result = false;

  // now we don't need our temporary meshes anymore
  engine->RemoveObject (topColliderMesh);
  engine->RemoveObject (bottomColliderMesh);

  if (!result)
  {
    Report (object_reg, "Error while setting up CD!");
    return result;
  }

  useCD = true;

  return result;
}

/*
 * Iterates over the nearby entities to find collisions
 */
int celPcLinearMovement::CollisionDetect (csColliderWrapper *collidewrapper,
    				  iSector* sector,
    				  csReversibleTransform* transform,
    				  csReversibleTransform* old_transform)
{
  int hits = 0;
  int j;
  // This never changes during this function, calculate it early
  csVector3 newpos=transform->GetO2TTranslation ();

  csCollisionPair* CD_contact;

  csRef<iMeshWrapperIterator> objectIter = engine->GetNearbyMeshes (sector,
    	transform->GetOrigin (),
    	COLLISION_DISTANCE,
    	true);

  while (objectIter->HasNext ())
  {
    iMeshWrapper* meshWrapper = objectIter->Next ();
    // Avoid hitting the mesh from this entity itself.
    if (meshWrapper != pcmesh->GetMesh ())
    {
      cdsys->ResetCollisionPairs ();
      csReversibleTransform tr = meshWrapper->GetMovable ()
      	->GetFullTransform ();
      csColliderWrapper* collide_wrap = csColliderWrapper::
      	GetColliderWrapper (meshWrapper->QueryObject ());
      iCollider* othercollider = 0;
      if (collide_wrap)
      {
        othercollider = collide_wrap->GetCollider ();
      }
      else
      {
        // There is no collider wrapper. In this case we test if there
	// is a pcsolid for the entity. If not we'll add a null collider.
	iCelEntity* otherentity = pl->FindAttachedEntity (
		meshWrapper->QueryObject ());
	csRef<iPcSolid> pcsolid = CEL_QUERY_PROPCLASS (
		otherentity->GetPropertyClassList (), iPcSolid);
        if (pcsolid)
	{
	  othercollider = pcsolid->GetCollider ();
	}
	else
	{
	  // Add a null collider so we don't check for pcsolid again.
	  csRef<csColliderWrapper> cw = csPtr<csColliderWrapper> (
	  	new csColliderWrapper (meshWrapper->QueryObject (), cdsys,
		(iCollider*)0));
	}
      }

      if (othercollider && cdsys->Collide (collidewrapper->GetCollider (),
      	transform, othercollider, &tr))
      {
    	bool reallycollided = false;

#ifdef SHOW_COLLIDER_MESH_DEBUG
	//@@@
    	Notify2("Player in Sector  : %s",
    	  sector->QueryObject ()->GetName ());
    	Notify2("Collider Mesh Name  : %s",
    	  meshWrapper->QueryObject ()->GetName ());
    	Notify2("Collider Mesh Sector: %s",
    	  meshWrapper->GetMovable ()->GetSectors ()->Get (0)->QueryObject ()
	  	->GetName ());
#endif

    	CD_contact = cdsys->GetCollisionPairs ();
    	for (j = 0; j < cdsys->GetCollisionPairCount (); j++ )
    	{
    	  /*
	   * Here we follow a segment from our current position to the
	   * position of the collision. If the sector the collision occured
	   * in is not the sector of the mesh we collided with,
    	   * this is invalid.
    	   */
    	  int sector_idx, sector_max;
    	  iSector* CollisionSector;
    	  bool mirror=false;
    	  csVector3 testpos;
    	  csVector3 line[2];
    	  csCollisionPair temppair=CD_contact[j];

    	  // Move the triangle from our mesh into world space
    	  temppair.a1 += newpos;
    	  temppair.b1 += newpos;
    	  temppair.c1 += newpos;

    	  if (FindIntersection (temppair, line))
    	  {
	    // Collided at this line segment. Pick a point in the middle of
	    // the segment to test.
	    testpos=(line[0]+line[1])/2;
    	  }
	  else
	  {
	    // No collision found, use the destination of the move.
	    testpos=newpos;
    	  }

    	  // This follows a line segment from start to finish and returns
	  // the sector you are ultimately in.
    	  CollisionSector = sector->FollowSegment (*old_transform,
	  	testpos, mirror, CEL_LINMOVE_FOLLOW_ONLY_PORTALS);

    	  // Iterate through all the sectors of the destination mesh,
	  // incase it's in multiple sectors.
    	  sector_max = meshWrapper->GetMovable ()->GetSectors ()->GetCount ();
    	  for (sector_idx=0 ; sector_idx<sector_max ; sector_idx++)
    	  {
	    // Check to see if this sector is the sector of the collision.
	    if (CollisionSector == meshWrapper->GetMovable ()->GetSectors ()
	    	->Get (sector_idx))
	    {
	      reallycollided = true;
	      our_cd_contact[num_our_cd++] = CD_contact[j];
	      // One valid sector is enough
	      break;
	    }
#ifdef LINMOVE_CD_FOLLOWSEG_DEBUG
	    else
	    {
	      printf ("Failed FollowSegment test on mesh %s in sector %s."
	      	" FollowSegment ended in sector %s\n",
		meshWrapper->QueryObject ()->GetName (),
		meshWrapper->GetMovable ()->GetSectors ()->Get (sector_idx)
			->QueryObject ()->GetName (),
		CollisionSector->QueryObject ()->GetName ());
	      csVector3 cv_oldpos, cv_newpos;
	      cv_oldpos = old_transform->GetO2TTranslation ();
	      cv_newpos = transform->GetO2TTranslation ();
	      printf ("Followed from sector %s position: %f,%f,%f"
	      	"  to sector %s position: %f,%f,%f\n",
		sector->QueryObject ()->GetName (),
		cv_oldpos.x, cv_oldpos.y, cv_oldpos.z,
		CollisionSector->QueryObject ()->GetName (),
		cv_newpos.x, cv_newpos.y, cv_newpos.z);
	    }
#endif
    	  }
    	}

    	// We don't increase hits unless a collision really occurred after
	// all tests.
    	if (reallycollided)
	{
    	  hits++;
	}

    	if (cdsys->GetOneHitOnly () && hits)
    	  return 1;
      }
    }
  }

  return hits;
}

/**
 * WARNING:  At present time this function does not work correctly!
 * <p>
 * The underlying function csEngine::GetNearbySectors () is not implemented.
 * Instead it returns only your current sector.
 */
int celPcLinearMovement::FindSectors (const csVector3& pos, float radius,
    			   iSector** sectors)
{
  int numsector = 0;

  csRef<iSectorIterator> sectorit =
    engine->GetNearbySectors (GetSector (), pos, radius);

  iSector* sector;
  while (sectorit->HasNext ())
  {
    sector = sectorit->Next ();
    sectors[numsector++] = sector;
    if (numsector >= MAXSECTORSOCCUPIED)
      break;
  }

  return numsector;
}


iSector* celPcLinearMovement::GetSector ()
{
  return pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0);
}

// --------------------------------------------------------------------------

/*
 * These #defines embody the maximum allowable speed in the game.
 * Right now they are constant, but later we will store this per player
 * depending on race, spells/buffs for speed, encumberment, etc.
 * 7.07 is 5*sqrt (2), which is the max speed if you strafe while running
 * in a diagonal direction.
 */
#define MAX_NONCHEAT_SPEED  7.07f
#define SPEED_FUDGE_FACTOR  .5f

bool celPcLinearMovement::SetDRData (iDataBuffer* data,bool detectcheat)
{
  if (data->GetSize () <= sizeof (float) * 8)
  {
    Report (object_reg,
    	"Received DR packet with wrong site: %u should be at least %u\n",
	data->GetSize (), sizeof (float) * 11+1);
    return false;
  }	

  float* tptr = (float*) data->GetData ();

  /*csXRotMatrix3 matrix (0);*/
  float yangle = *(tptr++);
  speed = *(tptr++);
  csMatrix3 matrix = (csMatrix3) csYRotMatrix3 (yangle);
  pcmesh->GetMesh ()->GetMovable ()->GetTransform ().SetO2T (matrix);

  //angularVelocity.x = *(tptr++);
  angularVelocity.y = *(tptr++);
  //angularVelocity.z = *(tptr++);
  vel.x = *(tptr++);
  //vel.y = *(tptr++);
  vel.z = *(tptr++);
  csVector3 pos;
  pos.x = *(tptr++);
  pos.y = *(tptr++);
  pos.z = *(tptr++);

  // Get onground flag
  // Find Action
  char* actionnum = (char*) tptr++;
  onground = *actionnum++;
  int i = *actionnum++;

  csRef<iSprite3DFactoryState> factstate = SCF_QUERY_INTERFACE (
    pcmesh->GetMesh ()->GetFactory ()->GetMeshObjectFactory (),
    iSprite3DFactoryState);

  const char* action;
  if (!factstate)
    action = 0;
  else if (i < 0)
    action = factstate->GetAction (-i)->GetName ();
  else
    action = factstate->GetAction (i)->GetName ();

  // The rest of the data contains the sector name, just cast it to char* and
  // compare with current sector name.
  if (strcmp (pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0)
  	->QueryObject ()->GetName (), actionnum) != 0)
  {
    // Sector changed, find new sector and set it
    iSector* newSector = engine->GetSectors ()->FindByName (actionnum);
    if (!newSector)
      Report (object_reg,
      	"[celPcLinearMovement::SetDRData] Sector %s not found !\n", actionnum);
    else
      pcmesh->GetMesh ()->GetMovable ()->SetSector (newSector);
  }	

  csVector3 oldpos = pcmesh->GetMesh ()->GetMovable ()->GetPosition ();

  float dist2 = (pos.x - oldpos.x) * (pos.x - oldpos.x) +
    (pos.z - oldpos.z) * (pos.z - oldpos.z);
  float dist = (float)sqrt (dist2);

  float timedelta = csGetTicks () - lastDRUpdate;
  float max_noncheat_distance=MAX_NONCHEAT_SPEED*timedelta/1000;
  //	if (timedelta < 50)
  //	{
  //		timedelta = 1000;
  //	}
  //	float implied_speed = sqrt (dist2)/(timedelta/1000);
  if (!detectcheat || dist<max_noncheat_distance
  	+ MAX_NONCHEAT_SPEED*accumulatedLag/1000)
  {
    if (dist < max_noncheat_distance)
    {
      // Calculate the "unused movement time" here and add it to the
      // accumulated lag.
      accumulatedLag += (csTicks)((max_noncheat_distance-dist)
      	* 1000.0f/MAX_NONCHEAT_SPEED);
      if (accumulatedLag > MAX_ACCUMULATED_LAG)
    	accumulatedLag = MAX_ACCUMULATED_LAG;
    }
    else
    {
      // Subtract from the accumulated lag.
      accumulatedLag-=(csTicks)((dist-max_noncheat_distance)
      	* 1000.0f/MAX_NONCHEAT_SPEED);
      if (accumulatedLag < 0) // This shouldn't happen due to the checks above
    	accumulatedLag = 0;
    }
    pcmesh->GetMesh ()->GetMovable ()->SetPosition (pos);
    pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();
    if (action)
    {
      pcmesh->SetAction (action);
      if (i < 0)
        pcmesh->SetReverseAction (true);
    }

    // Save informations for DR.
    lastDRUpdate = csGetTicks ();
    return true;
  }
  else
  {
    //@@@Warning5(LOG_CHEAT,"Player %s traversed %1.2fm in %1.2f msec with an accumulated lag allowance of %d ms. Cheat detected!\n",
      //@@@entity->GetName (),dist,timedelta,accumulatedLag);
    return true; // THIS SHOULD BE FALSE TO ACTIVATE CHEAT DETECTION!
  }
}

// Small helper function that returns the angle when given an x and y
// coordinate.
static float GetAngle (float x, float y)
{
  if (x > 1.0 )  x = 1.0;
  if (x < -1.0 ) x = -1.0;

  float angle = acos (x);
  if (y < 0)
    angle = 2*PI - angle;

  return angle;
}

static float Matrix2YRot (const csMatrix3& mat)
{
  csVector3 vec (0,0,1);
  vec = mat * vec;

  return GetAngle (vec.z, vec.x);
}

csPtr<iDataBuffer> celPcLinearMovement::GetDRData ()
{
  // Pack the important stuff into a databuffer.
  const char* sectorName = pcmesh->GetMesh ()->GetMovable ()->GetSectors ()
  	->Get (0)->QueryObject ()->GetName ();
  int sectorNameLen = strlen (sectorName);
  // It should be (sectorNameLen + 1), but for some reason crashes occur
  // if smaller than (sectorNameLen + 4).
  int bufLen = (sizeof (float) * 8) + (sizeof (char) * (sectorNameLen + 4))
  	+ 1 + 1;
  csRef<iDataBuffer> databuf = csPtr<iDataBuffer> (new csDataBuffer (bufLen));
  const csMatrix3& transf =
    pcmesh->GetMesh ()->GetMovable ()->GetTransform ().GetT2O ();

  float* tptr = (float*) databuf->GetData ();

  // rotation
  float yangle = Matrix2YRot (transf);
  *(tptr++) = yangle;
  // speed
  *(tptr++) = speed;

  // *(tptr++) = angularVelocity.x;
  *(tptr++) = angularVelocity.y;
  // *(tptr++) = angularVelocity.z;
  *(tptr++) = vel.x;
  //*(tptr++) = vel.y;
  *(tptr++) = vel.z;

  const csVector3& pos = pcmesh->GetMesh ()->GetMovable ()->GetPosition ();
  *(tptr++) = pos.x;
  *(tptr++) = pos.y;
  *(tptr++) = pos.z;

  // Append the current anim action to the message. Can optimize with 1 byte
  // later.
  iSpriteAction* action = 0;
  int action_number = 0;
  csRef<iSprite3DState> spstate =
    SCF_QUERY_INTERFACE (pcmesh->GetMesh ()->GetMeshObject (),
    iSprite3DState);
  if (spstate)
  {
    action = spstate->GetCurAction ();

    // Find Action
    csRef<iSprite3DFactoryState> factstate = SCF_QUERY_INTERFACE (
      pcmesh->GetMesh ()->GetFactory ()->GetMeshObjectFactory (),
      iSprite3DFactoryState);
    CS_ASSERT (factstate);
    for (action_number = 0; action_number < factstate->GetActionCount ();
    	action_number++)
    {
      if (factstate->GetAction (action_number) == action)
        break;
    }
    if (action_number == factstate->GetActionCount ())
      action_number = 0;
    if (spstate->GetReverseAction () )
      action_number = -action_number;
  }

  char* anum = (char*) tptr;
  // Persist onground flag too so each client can do
  // its own gravity for all entities.
  *anum++ = onground;
  *anum++ = action_number;

  // Send sector name.
  char* dest = anum;
  strncpy (dest, sectorName, sectorNameLen);
  dest[sectorNameLen]=0;

  // Save informations for DR.
  lastDRUpdate = csGetTicks ();
  lastSentAction = action;

  return csPtr<iDataBuffer> (databuf);
}

//#define DRDBG (X) printf ("DR: [ %f ] : %s\n", delta, X);
#define DRDBG(x)

bool celPcLinearMovement::NeedDRData (uint8& priority)
{
  // Never send DR messages until player is set to "ready"
  if (!ready)
    return false;

  float delta = csGetTicks () - lastDRUpdate;
  //@@@priority = PRIORITY_LOW;
  priority = 0;

  if (vel.IsZero () && angularVelocity.IsZero () &&
    lastSentVelocity.IsZero () && lastSentRotation.IsZero () &&
    delta>3000 && !stationary)
  {
    lastSentRotation = angularVelocity;
    lastSentVelocity = vel;
    DRDBG ("Stop\n");
    //@@@priority = PRIORITY_HIGH;
    priority = 1;
    stationary = true;
    return true;
  }

  // has the velocity rotation changed since last DR msg sent?
  if ((angularVelocity != lastSentRotation || vel.x != lastSentVelocity.x
  	|| vel.z != lastSentVelocity.z) &&
	((delta>250) || angularVelocity.IsZero ()
		|| lastSentRotation.IsZero () || vel.IsZero ()
		|| lastSentVelocity.IsZero ()))
  {
    lastSentRotation = angularVelocity;
    lastSentVelocity = vel;
    DRDBG ("Vel/Rot changed");
    stationary = false;
    return true;
  }

  if ((vel.y  && !lastSentVelocity.y) || (!vel.y && lastSentVelocity.y))
  {
    lastSentRotation = angularVelocity;
    lastSentVelocity = vel;
    DRDBG ("DR:Vert Vel Change rule\n");
    stationary = false;
    return true;
  }

  // if in motion, send every second instead of every 3 secs.
  if ((!lastSentVelocity.IsZero () || !lastSentRotation.IsZero () ) &&
    (delta > 1000))
  {
    lastSentRotation = angularVelocity;
    lastSentVelocity = vel;
    DRDBG ("1 sec motion");
    stationary = false;
    return true;
  }

  // Now check if stationary action has changed
  csRef<iSprite3DState> spstate =
    SCF_QUERY_INTERFACE (pcmesh->GetMesh ()->GetMeshObject (),
    iSprite3DState);
  if (spstate && spstate->GetCurAction () != lastSentAction)
  {
    stationary = false;
    return true;
  }

  // Real position/rotation/action is as calculated, no need for update.
  return false;
};

void celPcLinearMovement::GetLastPosition (csVector3& pos, float& yrot,
	iSector*& sector)
{
  // Position
  pos = pcmesh->GetMesh ()->GetMovable ()->GetPosition ();

  // rotation
  const csMatrix3& transf = pcmesh->GetMesh ()->GetMovable ()
  	->GetTransform ().GetT2O ();
  yrot = Matrix2YRot (transf);

  // Sector
  if (pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->GetCount ())
    sector = pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0);
  else
    sector = 0;
}

void celPcLinearMovement::SetPosition (const csVector3& pos, float yrot,
	const  iSector* sector)
{
  // Position
  pcmesh->GetMesh ()->GetMovable ()->SetPosition ((iSector *)sector,pos);

  // Rotation
  csMatrix3 matrix = (csMatrix3) csYRotMatrix3 (yrot);
  pcmesh->GetMesh ()->GetMovable ()->GetTransform ().SetO2T (matrix);

  // Sector
  pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();
}

