/*
  Crystal Space Entity Layer
  Copyright (C) 2001 PlaneShift Team (info@planeshift.it,
  Copyright (C) 2001-2005 by Jorrit Tyberghein

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
#include <imesh/spritecal3d.h>

#include <csutil/debug.h>
#include <csutil/databuf.h>
#include <csutil/plugmgr.h>
#include <iengine/movable.h>
#include <iengine/mesh.h>
#include <iengine/engine.h>
#include <iengine/sector.h>
#include <imesh/object.h>
#include <cstool/collider.h>
#include <ivaria/collider.h>
#include <ivaria/reporter.h>

#include <imesh/thing.h>
#include <csgeom/polymesh.h>
#include <igeom/polymesh.h>
#include <igeom/objmodel.h>
#include <igeom/path.h>
#include <csgeom/path.h>

//CEL includes
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "propclass/camera.h"
#include "propclass/colldet.h"
#include "propclass/solid.h"
#include "celtool/stdparams.h"

#include "linmove.h"

extern void MoveReport (iObjectRegistry* object_reg, const char* msg, ...);

CEL_IMPLEMENT_FACTORY (LinearMovement, "pclinearmovement")

csStringID celPcLinearMovement::id_body = csInvalidStringID;
csStringID celPcLinearMovement::id_legs = csInvalidStringID;
csStringID celPcLinearMovement::id_offset = csInvalidStringID;
csStringID celPcLinearMovement::id_percentage = csInvalidStringID;
csStringID celPcLinearMovement::action_initcd = csInvalidStringID;
csStringID celPcLinearMovement::action_initcdmesh = csInvalidStringID;
csStringID celPcLinearMovement::id_sector = csInvalidStringID;
csStringID celPcLinearMovement::id_position = csInvalidStringID;
csStringID celPcLinearMovement::id_yrot = csInvalidStringID;
csStringID celPcLinearMovement::action_setposition = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcLinearMovement)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcLinearMovement)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcLinearMovement::PcLinearMovement)
  SCF_IMPLEMENTS_INTERFACE (iPcLinearMovement)
SCF_IMPLEMENT_EMBEDDED_IBASE_END


// collision detection variables
#define MAXSECTORSOCCUPIED  20
csCollisionPair celPcLinearMovement::our_cd_contact[1000];
int celPcLinearMovement::num_our_cd;

// velocity = prevVelocity + ACCEL
#define ACCEL 0.5f

#define LEGOFFSET	  0

// This is the distance the CD will use to look for objects to collide with.

// Set this in order to see what meshes the player is colliding with
//#define SHOW_COLLIDER_MESH_DEBUG

#define LINMOVE_PATH_FLAG (char)0x80

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
 * Terminal velocity
 * ((120 miles/hour  / 3600 second/hour) * 5280 feet/mile)
 *     / 3.28 feet/meter = 53.65 m/s
 *   *2.0 for "feel" correction = 107.3m/s
 */
#define ABS_MAX_FREEFALL_VELOCITY 107.3f


/*
 * This value is a default value to define the part of the mesh used a
 * body and leg in collision detection
 */
#define BODY_LEG_FACTOR 0.6f


//----------------------------------------------------------------------------

celPcLinearMovement::celPcLinearMovement (iObjectRegistry* object_reg)
  :celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcLinearMovement);

  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  if (!vc) MoveReport (object_reg, "iVirtualClock Missing!");

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (!engine)
  {
    MoveReport (object_reg, "Engine missing!");
    return;
  }

  vel = 0;
  angularVelocity = 0;
  angleToReachFlag = false;
  angDelta = 0;
  lastDRUpdate = 0;

  portalDisplaced = 0.0f;

  path = 0;
  path_speed = 0;
  path_time  = 0;

  /*
   * Speed affects all aspects of movement, including gravity.
   * It's effectively a comparative rate of "time"
   */
  speed = 1.0f;

  deltaLimit = 0;

  if (action_initcd == csInvalidStringID)
  {
    action_initcd = pl->FetchStringID ("cel.action.InitCD");
    id_percentage = pl->FetchStringID ("cel.parameter.percentage");
    action_initcdmesh = pl->FetchStringID ("cel.action.InitCDMesh");
    id_body = pl->FetchStringID ("cel.parameter.body");
    id_legs = pl->FetchStringID ("cel.parameter.legs");
    id_offset = pl->FetchStringID ("cel.parameter.offset");
    action_setposition = pl->FetchStringID ("cel.action.SetPosition");
    id_sector = pl->FetchStringID ("cel.parameter.sector");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_yrot = pl->FetchStringID ("cel.parameter.yrot");
  }

  /*
   * Initialize bouding box parameters to detect if they have been
   * loaded or not
   */
  topSize.Set(0, 0, 0);

  pl->CallbackEveryFrame ((iCelTimerListener*)this, cscmdPreProcess);
}

celPcLinearMovement::~celPcLinearMovement ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcLinearMovement);
}

#define LINMOVE_SERIAL 2

csPtr<iCelDataBuffer> celPcLinearMovement::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (LINMOVE_SERIAL);

  csRef<iCelPropertyClass> pc;
  if (pccolldet)
    pc = SCF_QUERY_INTERFACE (pccolldet, iCelPropertyClass);
  else
    pc = 0;
  databuf->Add (pc);
  if (pcmesh)
    pc = SCF_QUERY_INTERFACE (pcmesh, iCelPropertyClass);
  else
    pc = 0;
  databuf->Add (pc);

  databuf->Add (topSize);
  databuf->Add (bottomSize);
  databuf->Add (shift);
  databuf->Add (vel);
  databuf->Add (angularVelocity);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcLinearMovement::Load (iCelDataBuffer* databuf)
{
  int seriallnr = databuf->GetSerialNumber ();
  if (seriallnr != LINMOVE_SERIAL)
    return false;

  iCelPropertyClass* pc = databuf->GetPC ();
  csRef<iPcCollisionDetection> pccd;
  if (pc)
    pccd = SCF_QUERY_INTERFACE (pc, iPcCollisionDetection);
  pc = databuf->GetPC ();
  pcmesh = 0;
  if (pc) pcmesh = SCF_QUERY_INTERFACE (pc, iPcMesh);

  databuf->GetVector3 (topSize);
  databuf->GetVector3 (bottomSize);
  databuf->GetVector3 (shift);

  if (!InitCD (topSize, bottomSize, shift, pccd))
    return false;

  databuf->GetVector3 (vel);
  databuf->GetVector3 (angularVelocity);

  return true;
}

bool celPcLinearMovement::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_initcd)
  {
    CEL_FETCH_VECTOR3_PAR (body,params,id_body);
    if (!p_body) return false;
    CEL_FETCH_VECTOR3_PAR (legs,params,id_legs);;
    if (!p_legs) return false;
    CEL_FETCH_VECTOR3_PAR (offset,params,id_offset);
    if (!p_offset) return false;
    bool rc = InitCD (body, legs, offset);
    // @@@ Error report!
    (void)rc;
    return true;
  }
  else if (actionId == action_initcdmesh)
  {
    CEL_FETCH_FLOAT_PAR (percentage,params,id_percentage);
    if (!p_percentage) return false;
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    if (!pcmesh) return true;	// @@@ Error report!
    bool rc = InitCD (pcmesh->GetMesh (), percentage);
    // @@@ Error report!
    (void)rc;
    return true;
  }
  else if (actionId == action_setposition)
  {
    CEL_FETCH_VECTOR3_PAR (position,params,id_position);
    if (!p_position) return false;
    CEL_FETCH_FLOAT_PAR (yrot,params,id_yrot);
    if (!p_yrot) return false;
    CEL_FETCH_STRING_PAR (sector,params,id_sector);
    if (!sector) return false;
    iSector* sect = engine->FindSector (sector);
    if (!sect)
    {
      // @@@ Error
      return false;
    }
    SetPosition (position, yrot, sect);
    return true;
  }
  return false;
}

static inline int FindIntersection (const csCollisionPair& cd,
    			   csVector3 line[2])
{
  csVector3 tri1[3]; tri1[0]=cd.a1; tri1[1]=cd.b1; tri1[2]=cd.c1;
  csVector3 tri2[3]; tri2[0]=cd.a2; tri2[1]=cd.b2; tri2[2]=cd.c2;
  csSegment3 isect;
  bool coplanar, ret;

  ret = csIntersect3::TriangleTriangle (tri1, tri2, isect, coplanar);
  line[0]=isect.Start ();
  line[1]=isect.End ();
  return ret;
}

void celPcLinearMovement::SetAngularVelocity (const csVector3& angleVel)
{
  angularVelocity = angleVel;
  angleToReachFlag = false;
}

void celPcLinearMovement::SetAngularVelocity (const csVector3& angleVel,
	const csVector3& angleToReach)
{
  SetAngularVelocity (angleVel);
  angleToReachFlag = true;
  this->angleToReach = angleToReach;
}

void celPcLinearMovement::SetVelocity (const csVector3& vel)
{
  /*
   * Y movement is jumping, flight (lift) and gravity effects
   * Take care to check IsOnGround () before calling this for jumping
   */
    celPcLinearMovement::vel = vel;
}

void celPcLinearMovement::GetVelocity (csVector3& v) const
{
  v = vel;
}


void celPcLinearMovement::SetSpeed (float speedZ)
{
  speed = speedZ;
}

// --------------------------------------------------------------------------

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

// --------------------------------------------------------------------------
//Does the actual rotation
bool celPcLinearMovement::RotateV (float delta)
{
  if (!pcmesh || !pcmesh->GetMesh ())
  {
    MoveReport (object_reg, "Linmove: No Mesh found on entity!");
    return false;
  }      

  // rotation
  if (angularVelocity < SMALL_EPSILON)
    return false;

  //delta *= speed;
  csVector3 angle = angularVelocity * delta;
  if (angleToReachFlag)
  {
    const csMatrix3& transf = pcmesh->GetMesh ()->GetMovable ()->GetTransform ().GetT2O ();
    float current_yrot = Matrix2YRot (transf);
    current_yrot = atan2f (sin (current_yrot), cos (current_yrot) );
    float yrot_delta = fabs (atan2f (sin (angleToReach.y - current_yrot), cos (angleToReach.y - current_yrot)));
    if (fabs(angle.y) > yrot_delta)
      {
  	angle.y = (angle.y / fabs (angle.y)) * yrot_delta;
  	angularVelocity = 0;
 	angleToReachFlag = false;
      }
  }

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
  //float local_max_interval;
  bool rc = false;

  const csMatrix3& transf = pcmesh->GetMesh ()->GetMovable ()
      ->GetTransform ().GetT2O ();
  float yrot = Matrix2YRot (transf);


  // Make sure time moves forward at least at this rate

  //float temp3=(vel.y==0.0f)
	 // ? MAX_CD_INTERVAL
	 // : ABS (topSize.y/vel.y);

  //float temp2=MIN (temp3, (vel.x==0.0f)
	 // ? MAX_CD_INTERVAL
	 // : ABS (topSize.x/vel.x));

  //float temp1=MIN (temp2, (vel.z==0.0f)
	 // ? MAX_CD_INTERVAL
	 // : ABS (topSize.z/vel.z));

  //float local_max_interval =
	 // MAX (temp1, MIN_CD_INTERVAL);

  float local_max_interval =
    MAX (MIN (MIN ((vel.y==0.0f)
  	? MAX_CD_INTERVAL
	: ABS (intervalSize.y/vel.y), (vel.x==0.0f)
		? MAX_CD_INTERVAL
		: ABS (intervalSize.x/vel.x)), (vel.z==0.0f)
			? MAX_CD_INTERVAL
			: ABS (intervalSize.z/vel.z)), MIN_CD_INTERVAL);

  // Compensate for speed
  local_max_interval /= speed;
  // Err on the side of safety
  local_max_interval -= 0.005f;

  if (pccolldet)
  {
    while (delta > local_max_interval)
    {
      rc |= MoveV (local_max_interval);

      if(pccolldet->QueryRevert())
      {
        // Revert Rotation for safety
        csMatrix3 matrix = (csMatrix3) csYRotMatrix3 (yrot);
        pcmesh->GetMesh ()->GetMovable ()->GetTransform ().SetO2T (matrix);
      }
      else
      {
          rc |= RotateV(local_max_interval);
          yrot = Matrix2YRot (transf);
      }

      if (!rc)
          return rc;

      delta -= local_max_interval;
      local_max_interval = MAX (MIN (MIN ((vel.y==0.0f)
      	? MAX_CD_INTERVAL
	: ABS (intervalSize.y/vel.y), (vel.x==0.0f)
		? MAX_CD_INTERVAL
		: ABS (intervalSize.x/vel.x)), (vel.z==0.0f)
			? MAX_CD_INTERVAL
			: ABS (intervalSize.z/vel.z)), MIN_CD_INTERVAL);
      // Compensate for speed
      local_max_interval /= speed;
      // Err on the side of safety
      local_max_interval -= 0.005f;
    }
  }

  if (!pccolldet || delta)
  {
    rc |= MoveV (delta);
    rc |= RotateV(delta);
  }

  return rc;
}


// Do the actual move
bool celPcLinearMovement::MoveV (float delta)
{
  if (vel < SMALL_EPSILON && (!pccolldet || pccolldet->IsOnGround()))
    return false;  // didn't move anywhere

  delta *= speed;
  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  csMatrix3 mat;

  csReversibleTransform rt = movable->GetFullTransform ();
  mat = rt.GetT2O ();

  csVector3 oldpos = movable->GetPosition ();
  csVector3 newpos;

  newpos = mat*(vel*delta) + oldpos;

  // Check for collisions and adjust position
  if (pccolldet)
    if(!pccolldet->AdjustForCollisions (oldpos, newpos, vel, delta, movable))
      return false;                   // We haven't moved so return early

  csOrthoTransform transform_oldpos = csReversibleTransform (csMatrix3(), oldpos);
  csVector3 origNewpos = newpos;
  bool mirror = false;

  // Update position to account for portals
  iSector* new_sector = movable->GetSectors ()->Get (0);
  iSector* old_sector = new_sector;

  // @@@ Jorrit: had to do this add!
  // We need to measure slightly above the position of the actor or else
  // we won't really cross a portal.
  float height5 = (bottomSize.y + topSize.y) / 20.0;
  newpos.y += height5;

  transform_oldpos.SetOrigin (transform_oldpos.GetOrigin ()
    + csVector3 (0, height5, 0));

  new_sector = new_sector->FollowSegment (transform_oldpos,
      newpos, mirror, CEL_LINMOVE_FOLLOW_ONLY_PORTALS);
  newpos.y -= height5;
  if (new_sector != old_sector)
    movable->SetSector (new_sector);

  portalDisplaced += newpos - origNewpos;

  if(!IsOnGround())
  {
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

  // move to the new position
  movable->GetTransform ().SetOrigin (newpos);

  if (pccolldet)
  {
    // Part 4: Add us to all nearby sectors.
    pcmesh->GetMesh ()->PlaceMesh ();
  }

  movable->UpdateMove ();

  return true;
}

void celPcLinearMovement::UpdateDRDelta (csTicks ticksdelta)
{
  float delta = ticksdelta;
  delta /= 1000;
  ExtrapolatePosition (delta);
  // lastDRUpdate += ticksdelta;  The way this fn is used, it should not update this var
}


void celPcLinearMovement::UpdateDR (csTicks ticks)
{
  if (lastDRUpdate) // first time through gives huge deltas without this
  {
    float delta = ticks - lastDRUpdate;
    delta /= 1000;
    ExtrapolatePosition (delta);
  }
  lastDRUpdate = ticks;
}

void celPcLinearMovement::UpdateDR ()
{
  csTicks time = csGetTicks();
  if (lastDRUpdate)
  {
    float delta = time - lastDRUpdate;
    delta /= 1000;
    ExtrapolatePosition (delta);
  }
  lastDRUpdate = time;
}

csTicks celPcLinearMovement::TimeDiff()
{
  return csGetTicks() - lastDRUpdate;
}

void celPcLinearMovement::ExtrapolatePosition (float delta)
{
  if (path)
  {
    path_time += delta;
    path->CalculateAtTime(path_time);
    csVector3 pos,look,up;
    
    path->GetInterpolatedPosition(pos);
    path->GetInterpolatedUp(up);
    path->GetInterpolatedForward(look);

    pcmesh->GetMesh ()->GetMovable ()->GetTransform().SetOrigin(pos);
    pcmesh->GetMesh ()->GetMovable ()->GetTransform().LookAt(
    	look.Unit(),up.Unit());
    pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();

    csRef<iSprite3DState> spstate =
            SCF_QUERY_INTERFACE (pcmesh->GetMesh ()->GetMeshObject (),
                                 iSprite3DState);

    if (spstate && strcmp(path_actions[path->GetCurrentIndex()],
	                  spstate->GetCurAction()->GetName() ) )
    {
        spstate->SetAction(path_actions[path->GetCurrentIndex()]);
    }
  }
  else
  {
    // bool rc =
    MoveSprite (delta);
    //if (rc)
    //  pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();
  }
}

void celPcLinearMovement::TickEveryFrame ()
{
  FindSiblingPropertyClasses ();
  if (!pcmesh)
  {
    MoveReport (object_reg, "No Mesh found on entity!");
    return;
  }

  csTicks elapsed_time = vc->GetElapsedTicks ();
  if (!elapsed_time)
    return;

  float delta = elapsed_time/1000.0;
  if (deltaLimit != 0)
    delta = MIN(delta, deltaLimit);

  // Adjust the properties.
  ExtrapolatePosition (delta);
}

bool celPcLinearMovement::InitCD (iPcCollisionDetection *pc_cd)
{
  FindSiblingPropertyClasses ();
  if (topSize.IsZero () && pcmesh)
    {
      csRef<iMeshWrapper> meshWrapper = pcmesh->GetMesh ();
      if (meshWrapper)
	{
	  csBox3 worldBoundingBox;
	  meshWrapper->GetWorldBoundingBox (worldBoundingBox);
	  topSize = worldBoundingBox.Max () - worldBoundingBox.Min ();
	  topSize.y = topSize.y * BODY_LEG_FACTOR;
	  bottomSize = worldBoundingBox.Max () - worldBoundingBox.Min ();
	  bottomSize.y = bottomSize.y * (1 - BODY_LEG_FACTOR);
	  shift = 0;
	}
    }
  return InitCD (topSize, bottomSize, shift, pc_cd);
}

void celPcLinearMovement::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  }
}

void celPcLinearMovement::GetCDDimensions (csVector3& body, csVector3& legs,
	csVector3& shift, iPcCollisionDetection*& pc_cd)
{
  body = topSize;
  legs = bottomSize;
  shift = celPcLinearMovement::shift;
  pc_cd = pccolldet;
}

bool celPcLinearMovement::InitCD (iMeshWrapper* mesh, float percentage,
  	iPcCollisionDetection* pc_cd)
{
  csBox3 bbox;
  mesh->GetMeshObject ()->GetObjectModel ()->GetObjectBoundingBox (bbox);
  csVector3 body = bbox.Max () - bbox.Min ();
  csVector3 legs = body;
  csVector3 shift (0); shift.y = bbox.MinY ();
  legs.y = percentage * legs.y / 100.0f;
  body.y = (100.0f-percentage) * body.y / 100.0f;
  return InitCD (body, legs, shift, pc_cd);
}

bool celPcLinearMovement::InitCD (const csVector3& body, const csVector3& legs,
	const csVector3& shift, iPcCollisionDetection *pc_cd)
{
  FindSiblingPropertyClasses ();
  if (!pcmesh)
  {
    MoveReport (object_reg, "No Mesh found on entity!");
    return false;
  }

  topSize = body;
  bottomSize = legs;

  intervalSize.x = MIN(topSize.x, bottomSize.x);
  intervalSize.y = MIN(topSize.y, bottomSize.y);
  intervalSize.z = MIN(topSize.z, bottomSize.z);

  celPcLinearMovement::shift = shift;

  // Temporary fix for ladder climbing
  // This MIGHT not be necessary in most cases
  bottomSize.x = MAX(topSize.x, bottomSize.x);
  bottomSize.z = MAX(topSize.z, bottomSize.z);

  if (!pc_cd)
  {
    csRef<iCelPropertyClass> pc;
    pc = pl->CreatePropertyClass(entity, "pccollisiondetection");
    if ( !pc )
    {
      MoveReport (object_reg,
		"Could not create property class pccollisiondetection.");
      return false;
    }
    csRef<iPcCollisionDetection> pctemp;
    pctemp = SCF_QUERY_INTERFACE(pc, iPcCollisionDetection);

    pccolldet = pctemp;
  }
  else
  {
    pccolldet = pc_cd;
  }

  return pccolldet->Init (topSize, bottomSize, shift);
}

bool celPcLinearMovement::IsOnGround () const
{
  if (pccolldet)
    return pccolldet->IsOnGround();

  return true;
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

void celPcLinearMovement::GetDRData(bool& on_ground,
                                    float& speed,
                                    csVector3& pos,
                                    float& yrot,
                                    iSector*& sector,
                                    csVector3& vel,
                                    float& ang_vel)
{
  on_ground = IsOnGround();
  speed = this->speed;
  GetLastPosition(pos,yrot,sector);
  vel = this->vel;
  ang_vel = angularVelocity.y;
}


iSector* celPcLinearMovement::GetSector ()
{
  return pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0);
}

// --------------------------------------------------------------------------

void celPcLinearMovement::SetPathAction (int which, const char *action)
{
  path_actions.Put(which,action);
}


//#define DRDBG(X) printf ("DR: [ %f ] : %s\n", delta, X);
#define DRDBG(x)


void celPcLinearMovement::GetLastPosition (csVector3& pos, float& yrot,
	iSector*& sector)
{
  if (!pcmesh || !pcmesh->GetMesh ())
  {
    MoveReport (object_reg, "No Mesh found on entity!");
    return;
  }

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
  FindSiblingPropertyClasses ();
  // Position
  pcmesh->GetMesh ()->GetMovable ()->SetPosition ((iSector *)sector,pos);

  // Rotation
  csMatrix3 matrix = (csMatrix3) csYRotMatrix3 (yrot);
  pcmesh->GetMesh ()->GetMovable ()->GetTransform ().SetO2T (matrix);

  // Sector
  pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();
}

void celPcLinearMovement::SetDRData(bool on_ground,float speed,
                                    csVector3& pos,float yrot,iSector *sector,
                                    csVector3& vel,float ang_vel)
{
  if (pccolldet)
    pccolldet->SetOnGround(on_ground);

  this->speed = speed;
  SetPosition(pos,yrot,sector);
  SetVelocity(vel);
  csVector3 rot(0,ang_vel,0);
  SetAngularVelocity(rot);
  lastDRUpdate = csGetTicks();
}
