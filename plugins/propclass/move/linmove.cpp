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
#include <iengine/scenenode.h>
#include <imesh/object.h>
#include <cstool/collider.h>
#include <ivaria/collider.h>
#include <ivaria/reporter.h>
#include "ivaria/mapnode.h"

#include <imesh/thing.h>
#include <csgeom/polymesh.h>
#include <igeom/polymesh.h>
#include <imesh/objmodel.h>
#include <igeom/path.h>
#include <csgeom/path.h>
#include <csgeom/math3d.h>

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

extern bool MoveReport (iObjectRegistry* object_reg, const char* msg, ...);

CEL_IMPLEMENT_FACTORY (LinearMovement, "pclinearmovement")

csStringID celPcLinearMovement::id_body = csInvalidStringID;
csStringID celPcLinearMovement::id_legs = csInvalidStringID;
csStringID celPcLinearMovement::id_offset = csInvalidStringID;
csStringID celPcLinearMovement::id_percentage = csInvalidStringID;
csStringID celPcLinearMovement::id_sector = csInvalidStringID;
csStringID celPcLinearMovement::id_position = csInvalidStringID;
csStringID celPcLinearMovement::id_yrot = csInvalidStringID;
csStringID celPcLinearMovement::id_velocity = csInvalidStringID;

// collision detection variables
#define MAXSECTORSOCCUPIED 20

// velocity = prevVelocity + ACCEL
#define ACCEL 0.5f

#define LEGOFFSET 0

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

PropertyHolder celPcLinearMovement::propinfo;

celPcLinearMovement::celPcLinearMovement (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  if (!vc)
  {
    MoveReport (object_reg, "iVirtualClock Missing!");
    return;
  }

  engine = csQueryRegistry<iEngine> (object_reg);
  if (!engine)
  {
    MoveReport (object_reg, "Engine missing!");
    return;
  }

  velBody = angularVelocity = velWorld = 0;
  angleToReachFlag = false;
  angDelta = 0.0f;
  lastDRUpdate = 0;

  xRot = 0.0f;
  zRot = 0.0f;
  hugGround = false;
  called = false;

  portalDisplaced = 0.0f;
  anchor_needsinit = false;

  path = 0;
  path_speed = 0.0f;
  path_time  = 0.0f;

  offset_err = 0;
  offset_rate = 0;

  /*
   * Speed affects all aspects of movement, including gravity.
   * It's effectively a comparative rate of "time"
   */
  speed = 1.0f;

  deltaLimit = 0.0f;

  if (id_percentage == csInvalidStringID)
  {
    id_percentage = pl->FetchStringID ("cel.parameter.percentage");
    id_body = pl->FetchStringID ("cel.parameter.body");
    id_legs = pl->FetchStringID ("cel.parameter.legs");
    id_offset = pl->FetchStringID ("cel.parameter.offset");
    id_sector = pl->FetchStringID ("cel.parameter.sector");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_yrot = pl->FetchStringID ("cel.parameter.yrot");
    id_velocity = pl->FetchStringID ("cel.parameter.velocity");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_initcd, "cel.action.InitCD");
    AddAction (action_initcdmesh, "cel.action.InitCDMesh");
    AddAction (action_setposition, "cel.action.SetPosition");
    AddAction (action_setvelocity, "cel.action.SetVelocity");
    AddAction (action_addvelocity, "cel.action.AddVelocity");
    AddAction (action_setangularvelocity, "cel.action.SetAngularVelocity");
  }

  // For properties.
  propinfo.SetCount (4);
  AddProperty (propid_anchor, "cel.property.anchor",
  	CEL_DATA_STRING, false, "Mesh Anchor.", 0);
  AddProperty (propid_gravity, "cel.property.gravity",
  	CEL_DATA_FLOAT, false, "Gravity.", &gravity);
  AddProperty (propid_hug, "cel.property.hug",
  	CEL_DATA_BOOL, false, "Hug to ground.", &hugGround);
  AddProperty (propid_speed, "cel.property.speed",
  	CEL_DATA_FLOAT, false, "Movement speed.", &speed);

  ResetGravity ();

  /*
   * Initialize bouding box parameters to detect if they have been
   * loaded or not
   */
  topSize.Set (0, 0, 0);

  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
}

celPcLinearMovement::~celPcLinearMovement ()
{
  SetAnchor (0);
  delete [] propdata;
}

#define LINMOVE_SERIAL 3

csPtr<iCelDataBuffer> celPcLinearMovement::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (LINMOVE_SERIAL);

  csRef<iCelPropertyClass> pc;
  if (pccolldet)
    pc = scfQueryInterface<iCelPropertyClass> (pccolldet);
  else
    pc = 0;
  databuf->Add (pc);

  if (pcmesh)
    pc = scfQueryInterface<iCelPropertyClass> (pcmesh);
  else
    pc = 0;
  databuf->Add (pc);

  if (anchor)
    pc = scfQueryInterface<iCelPropertyClass> (anchor);
  else
    pc = 0;
  databuf->Add (pc);

  databuf->Add (gravity);
  databuf->Add (topSize);
  databuf->Add (bottomSize);
  databuf->Add (shift);
  databuf->Add (velBody);
  databuf->Add (angularVelocity);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcLinearMovement::Load (iCelDataBuffer* databuf)
{
  int seriallnr = databuf->GetSerialNumber ();
  if (seriallnr != LINMOVE_SERIAL)
    return MoveReport (object_reg, "Can't load propertyclass pclinmove!");

  iCelPropertyClass* pc = databuf->GetPC ();
  csRef<iPcCollisionDetection> pccd;
  if (pc)
    pccd = scfQueryInterface<iPcCollisionDetection> (pc);
  pc = databuf->GetPC ();
  pcmesh = 0;
  if (pc) pcmesh = scfQueryInterface<iPcMesh> (pc);

  pc = databuf->GetPC ();
  if (pc)
  {
    csRef<iPcMesh> new_anchor = scfQueryInterface<iPcMesh> (pc);
    LoadAnchor (new_anchor);
  }

  gravity = databuf->GetFloat ();
  databuf->GetVector3 (topSize);
  databuf->GetVector3 (bottomSize);
  databuf->GetVector3 (shift);

  if (!InitCD (topSize, bottomSize, shift, pccd))
    return false;


  databuf->GetVector3 (velBody);
  databuf->GetVector3 (angularVelocity);

  return true;
}

void celPcLinearMovement::LoadAnchor (iPcMesh* a)
{
  anchor_needsinit = false;
  if (!pcmesh) return;

  anchor = a;
  // Set the new anchor if needed.
  if (anchor)
  {
    if (!anchor->GetMesh ())
      anchor_needsinit = true;
    else
    {
      iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
      pcmesh->GetMesh ()->QuerySceneNode ()->SetParent (
      	anchor->GetMesh ()->QuerySceneNode ());
      movable->UpdateMove ();
    }
  }
}

void celPcLinearMovement::SetAnchor (iPcMesh* a)
{
  anchor_needsinit = false;
  if (!pcmesh) return;

  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  csReversibleTransform trans = movable->GetFullTransform ();

  // Clear the previous anchor first if any.
  if (anchor && anchor != a)
  {
    pcmesh->GetMesh ()->QuerySceneNode ()->SetParent (0);
    movable->SetTransform (trans);
    movable->ClearSectors ();
    movable->SetSector (anchor->GetMesh ()->GetMovable ()
    	->GetSectors ()->Get (0));
  }
  anchor = a;
  // Set the new anchor if needed.
  if (anchor)
  {
    pcmesh->GetMesh ()->QuerySceneNode ()->SetParent (
    	anchor->GetMesh ()->QuerySceneNode ());
    csReversibleTransform newtrans = trans / anchor->GetMesh ()
    	->GetMovable ()->GetFullTransform ();
    movable->SetTransform (newtrans);
  }

  movable->UpdateMove ();
}

bool celPcLinearMovement::SetPropertyIndexed (int idx, const char* b)
{
  if (idx == propid_anchor)
  {
    if (b == 0 || *b == 0)
    {
      SetAnchor (0);
      return true;
    }
    iCelEntity* ent = pl->FindEntity (b);
    if (!ent)
      return MoveReport (object_reg,
      	"Can't find entity '%s' for property 'anchor' in pclinmove!", b);
    csRef<iPcMesh> m = CEL_QUERY_PROPCLASS_ENT (ent, iPcMesh);
    if (!m)
      return MoveReport (object_reg,
      	"Entity '%s' doesn't have a pcmesh (property 'anchor' in pclinmove)!",
      	b);
    SetAnchor (m);
    return true;
  }
  return false;
}

bool celPcLinearMovement::GetPropertyIndexed (int idx, const char*& b)
{
  if (idx == propid_anchor)
  {
    if (anchor)
    {
      csRef<iCelPropertyClass> pc = scfQueryInterface<iCelPropertyClass> (anchor);
      b = pc->GetEntity ()->GetName ();
    }
    else
    {
      b = 0;
    }
    return true;
  }
  return false;
}

bool celPcLinearMovement::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_initcd:
      {
        CEL_FETCH_VECTOR3_PAR (body,params,id_body);
        if (!p_body)
          return MoveReport (object_reg,
          	"Missing parameter 'body' for action InitCD!");
        CEL_FETCH_VECTOR3_PAR (legs,params,id_legs);;
        if (!p_legs)
          return MoveReport (object_reg,
          	"Missing parameter 'legs' for action InitCD!");
        CEL_FETCH_VECTOR3_PAR (offset,params,id_offset);
        if (!p_offset)
          offset = csVector3 (0.0f, 0.0f, 0.0f);
        bool rc = InitCD (body, legs, offset);
        return rc;
      }
    case action_initcdmesh:
      {
        CEL_FETCH_FLOAT_PAR (percentage,params,id_percentage);
        if (!p_percentage)
          return MoveReport (object_reg,
          	"Missing parameter 'percentage' for action InitCDMesh!");
        csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
        if (!pcmesh)
          return MoveReport (object_reg,
          	"Can't find pcmesh in current entity for action InitCDMesh!");
        bool rc = InitCD (pcmesh->GetMesh (), percentage);
        return rc;
      }
    case action_setposition:
      {
        CEL_FETCH_FLOAT_PAR (yrot,params,id_yrot);
        if (!p_yrot)
          return MoveReport (object_reg,
          	"Missing parameter 'yrot' for action SetPosition!");
        CEL_FETCH_STRING_PAR (sector,params,id_sector);
        if (!sector)
          return MoveReport (object_reg,
          	"Missing parameter 'sector' for action SetPosition!");
        iSector* sect = engine->FindSector (sector);
        if (!sect)
          return MoveReport (object_reg,
          	"Can't find sector '%s' for action SetPosition!", sector);
        const celData* p_position = params->GetParameter (id_position);
        if (!p_position)
          return MoveReport (object_reg,
          	"Missing parameter 'position' for action SetPosition!");
        if (p_position->type == CEL_DATA_VECTOR3)
        {
          csVector3 vpos;
          vpos.x = p_position->value.v.x;
          vpos.y = p_position->value.v.y;
          vpos.z = p_position->value.v.z;
          SetPosition (vpos, yrot, sect);
        }
        else if (p_position->type == CEL_DATA_STRING)
        {
          const char* cpos = p_position->value.s->GetData ();
          SetPosition (cpos, yrot, sect);
        }
        else
          return MoveReport (object_reg,
          	"'position' must be string or vector for SetPosition!");
        return true;
      }
    case action_setvelocity:
      {
        CEL_FETCH_VECTOR3_PAR (velocity,params,id_velocity);
        if (!p_velocity)
          return MoveReport (object_reg,
          	"Missing parameter 'velocity' for action SetVelocity!");
        SetVelocity (velocity);
        return true;
      }
    case action_addvelocity:
      {
        CEL_FETCH_VECTOR3_PAR (velocity,params,id_velocity);
        if (!p_velocity)
          return MoveReport (object_reg,
          	"Missing parameter 'velocity' for action AddVelocity!");
        AddVelocity (velocity);
        return true;
      }
    case action_setangularvelocity:
      {
        CEL_FETCH_VECTOR3_PAR (velocity,params,id_velocity);
        if (!p_velocity)
          return MoveReport (object_reg,
          	"Missing parameter 'velocity' for action SetAngularVelocity!");
        SetAngularVelocity (velocity);
        return true;
      }
    default:
      return false;
  }
}

static inline bool FindIntersection (const csCollisionPair& cd,
	csVector3 line[2])
{
  csVector3 tri1[3]; tri1[0] = cd.a1; tri1[1] = cd.b1; tri1[2] = cd.c1;
  csVector3 tri2[3]; tri2[0] = cd.a2; tri2[1] = cd.b2; tri2[2] = cd.c2;
  csSegment3 isect;
  bool coplanar, ret;

  ret = csIntersect3::TriangleTriangle (tri1, tri2, isect, coplanar);
  line[0] = isect.Start ();
  line[1] = isect.End ();
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

void celPcLinearMovement::SetSpeed (float speedZ)
{
  speed = speedZ;
}

// --------------------------------------------------------------------------

// Small helper function that returns the angle when given an x and y
// coordinate.
static float GetAngle (float x, float y)
{
  if (x > 1.0f )  x = 1.0f;
  if (x < -1.0f ) x = -1.0f;

  float angle = acos (x);
  if (y < 0.0f)
    angle = 2.0f * PI - angle;

  return angle;
}

static float Matrix2YRot (const csMatrix3& mat)
{
  csVector3 vec (0.0f, 0.0f, 1.0f);
  vec = mat * vec;

  return GetAngle (vec.z, vec.x);
}

// --------------------------------------------------------------------------
//Does the actual rotation
bool celPcLinearMovement::RotateV (float delta)
{
  if (!pcmesh || !pcmesh->GetMesh ())
    return MoveReport (object_reg, "Linmove: No Mesh found on entity!");

  // rotation
  if (angularVelocity < SMALL_EPSILON)
    return false;

  //delta *= speed;
  csVector3 angle = angularVelocity * delta;
  if (angleToReachFlag)
  {
    const csMatrix3& transf = pcmesh->GetMesh ()->GetMovable ()
    	->GetTransform ().GetT2O ();
    float current_yrot = Matrix2YRot (transf);
    current_yrot = atan2f (sin (current_yrot), cos (current_yrot));
    float yrot_delta = fabs (atan2f (sin (angleToReach.y - current_yrot),
    	cos (angleToReach.y - current_yrot)));
    if (fabs(angle.y) > yrot_delta)
    {
      angle.y = (angle.y / fabs (angle.y)) * yrot_delta;
      angularVelocity = 0;
      angleToReachFlag = false;
    }
  }

  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  csYRotMatrix3 rotMat (angle.y);
  movable->SetTransform (movable->GetTransform ().GetT2O () * rotMat);
  movable->UpdateMove ();
  //pcmesh->GetMesh ()->GetMovable ()->Transform (rotMat);
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


int celPcLinearMovement::MoveSprite (float delta)
{
  //float local_max_interval;
  int ret = CEL_MOVE_SUCCEED;

  csReversibleTransform fulltransf = pcmesh->GetMesh ()->GetMovable ()
  	->GetFullTransform ();
  const csMatrix3& transf = fulltransf.GetT2O ();
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

  // Calculate the total velocity (body and world) in OBJECT space.
  csVector3 bodyVel (fulltransf.Other2ThisRelative (velWorld) + velBody);

  float local_max_interval =
  	MAX (MIN (MIN ((bodyVel.y==0.0f)
  	? MAX_CD_INTERVAL
  	: ABS (intervalSize.y/bodyVel.y), (bodyVel.x==0.0f)
  	? MAX_CD_INTERVAL
  	: ABS (intervalSize.x/bodyVel.x)), (bodyVel.z==0.0f)
  	? MAX_CD_INTERVAL
  	: ABS (intervalSize.z/bodyVel.z)), MIN_CD_INTERVAL);

  // Compensate for speed
  local_max_interval /= speed;
  // Err on the side of safety
  local_max_interval -= 0.005f;

  if (pccolldet)
  {
    while (delta > local_max_interval)
    {
      ret = MoveV (local_max_interval);

      if (pccolldet->QueryRevert ())
      {
        // Revert Rotation for safety
        //@@@ This need to be revised! You can't change the full transform!
        //@@@csMatrix3 matrix = (csMatrix3) csYRotMatrix3 (yrot);
        //@@@pcmesh->GetMesh ()->GetMovable ()->GetFullTransform ().SetO2T (matrix);
      }
      else
      {
        RotateV (local_max_interval);
        yrot = Matrix2YRot (transf);
      }

      if (ret == CEL_MOVE_FAIL)
          return ret;

      // The velocity may have changed by now
      bodyVel = fulltransf.Other2ThisRelative(velWorld) + velBody;

      delta -= local_max_interval;
      local_max_interval = MAX (MIN (MIN ((bodyVel.y==0.0f)
      	? MAX_CD_INTERVAL
      	: ABS (intervalSize.y/bodyVel.y), (bodyVel.x==0.0f)
      	? MAX_CD_INTERVAL
      	: ABS (intervalSize.x/bodyVel.x)), (bodyVel.z==0.0f)
      	? MAX_CD_INTERVAL
      	: ABS (intervalSize.z/bodyVel.z)), MIN_CD_INTERVAL);
      // Compensate for speed
      local_max_interval /= speed;
      // Err on the side of safety
      local_max_interval -= 0.005f;
    }
  }

  if (!pccolldet || delta)
  {
    ret = MoveV (delta);
    RotateV(delta);
  }

  return ret;
}

// Apply the gradual offset correction from SetSoftDRUpdate to the mesh position
void celPcLinearMovement::OffsetSprite (float delta)
{
  if (offset_err.IsZero ()) return;  // no offset correction to perform

  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  csVector3 oldpos = movable->GetPosition ();
  csVector3 newpos;

  csVector3 del_offset = offset_rate;
  del_offset *= delta;

  // Check for NaN conditions:
  if (del_offset.x != del_offset.x) del_offset.x = 0.0f;
  if (del_offset.y != del_offset.y) del_offset.y = 0.0f;
  if (del_offset.z != del_offset.z) del_offset.z = 0.0f;

  // Calculate error correction for this time interval
  if ((del_offset.x > offset_err.x && del_offset.x > 0.0f) ||
  	(del_offset.x < offset_err.x && del_offset.x < 0.0f))
  {
    del_offset.x = offset_err.x;
    offset_rate.x = 0.0f;
  }
  if ((del_offset.y > offset_err.y && del_offset.y > 0.0f) ||
  	(del_offset.y < offset_err.y && del_offset.y < 0.0f))
  {
    del_offset.y = offset_err.y;
    offset_rate.y = 0.0f;
  }
  if ((del_offset.z > offset_err.z && del_offset.z > 0.0f) ||
  	(del_offset.z < offset_err.z && del_offset.z < 0.0f))
  {
    del_offset.z = offset_err.z;
    offset_rate.z = 0.0f;
  }
  offset_err -= del_offset;

  newpos = oldpos + del_offset;

  movable->GetTransform ().SetOrigin (newpos);
}

// Do the actual move
int celPcLinearMovement::MoveV (float delta)
{
  if (velBody < SMALL_EPSILON && velWorld < SMALL_EPSILON
  	&& (!pccolldet || pccolldet->IsOnGround ()))
    return CEL_MOVE_DONTMOVE;  // didn't move anywhere

  int ret = CEL_MOVE_SUCCEED;
  iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
  if (movable->GetSectors ()->GetCount () <= 0)
    return CEL_MOVE_DONTMOVE;  // didn't move anywhere

  csMatrix3 mat;

  // To test collision detection we use absolute position and transformation
  // (this is relevant if we are anchored). Later on we will correct that.
  csReversibleTransform fulltransf = movable->GetFullTransform ();
  mat = fulltransf.GetT2O ();
  delta *= speed;

  csVector3 worldVel (fulltransf.This2OtherRelative (velBody) + velWorld);
  csVector3 oldpos (fulltransf.GetOrigin ());
  csVector3 newpos (worldVel*delta + oldpos);
  csVector3 bufpos = newpos;

  // Check for collisions and adjust position
  if (pccolldet)
    if (!pccolldet->AdjustForCollisions (oldpos, newpos, worldVel,
    	delta, movable))
    {
      ret = CEL_MOVE_FAIL;
      newpos = oldpos;
    }
    else
    {
      // check if we collided
      if ((newpos - bufpos).Norm () > 0.000001f)
      {
        ret = CEL_MOVE_PARTIAL;
      }
    }

  csVector3 origNewpos = newpos;
  bool mirror = false;

  // Update position to account for portals
  iSector* new_sector = movable->GetSectors ()->Get (0);
  iSector* old_sector = new_sector;

  // @@@ Jorrit: had to do this add!
  // We need to measure slightly above the position of the actor or else
  // we won't really cross a portal.
  float height5 = (bottomSize.y + topSize.y) / 10.0f;
  newpos.y += height5;
  csMatrix3 id;
  csOrthoTransform transform_oldpos (id, oldpos +
  	csVector3 (0.0f, height5, 0.0f));

  new_sector = new_sector->FollowSegment (transform_oldpos, newpos, mirror,
  	CEL_LINMOVE_FOLLOW_ONLY_PORTALS);
  newpos.y -= height5;
  if (new_sector != old_sector)
    movable->SetSector (new_sector);

  portalDisplaced += newpos - origNewpos;
  if(!IsOnGround ())
  {
    // gravity! move down!
    velWorld.y  -= gravity * delta;
    /*
     * Terminal velocity
     *   ((120 miles/hour  / 3600 second/hour) * 5280 feet/mile)
     *   / 3.28 feet/meter = 53.65 m/s
     */
    // The body velocity is figured in here too.
    if (velWorld.y < 0)
    {
      // Call callbacks
      if (!called)
      {
        size_t i = gravityCallbacks.Length ();
        while (i > 0)
        {
          i--;
          gravityCallbacks[i]->Callback ();
        }
        called = true;
      }

      if (fulltransf.This2OtherRelative (velBody).y
      	+ velWorld.y < -(ABS_MAX_FREEFALL_VELOCITY))
      	velWorld.y = -(ABS_MAX_FREEFALL_VELOCITY)
      	- fulltransf.This2OtherRelative (velBody).y;
      if (velWorld.y > 0)
        velWorld.y = 0;
    }
    else
      called = false;
  }
  else
  {
    if(velWorld.y < 0)
    {
      // Call callbacks
      size_t i = gravityCallbacks.Length ();
      while (i > 0)
      {
        i--;
        gravityCallbacks[i]->Callback ();
      }
      velWorld.y = 0;
    }

    if (hugGround)
      HugGround (newpos, new_sector);
  }

  // Move to the new position. If we have an anchor we have to convert
  // the new position from absolute to relative.
  if (anchor)
  {
    newpos = anchor->GetMesh ()->GetMovable ()->GetFullTransform ()
    	.Other2This (newpos);
  }
  movable->GetTransform ().SetOrigin (newpos);
  movable->GetTransform ().SetT2O(
  	movable->GetTransform ().GetT2O () * transform_oldpos.GetT2O ());

  if (pccolldet)
  {
    // Part 4: Add us to all nearby sectors.
    pcmesh->GetMesh ()->PlaceMesh ();
  }

  movable->UpdateMove ();

  return ret;
}

void celPcLinearMovement::HugGround (const csVector3& pos, iSector* sector)
{
  csVector3 start, end;
  csIntersectingTriangle closest_tri;
  csVector3 isect[4];
  csPlane3 plane;
  bool hit[4];

  // Set minimum base dimensions of 0.5x0.5 for good aesthetics
  float legsXlimit = MAX(bottomSize.x / 2, 0.5);
  float legsZlimit = MAX(bottomSize.z / 2, 0.5);

  start.y = pos.y + shift.y + 0.01;

  // Assuming the bounding box is axis-aligned: (Lower-left point)
  start.x = pos.x - legsXlimit;
  start.z = pos.z - legsZlimit;

  end = start;
  end.y -= 5;

  hit[0] = csColliderHelper::TraceBeam (cdsys, sector, start, end,
  	false, closest_tri, isect[0]) != -1;

  // Assuming the bounding box is axis-aligned: (Upper-left point)
  start.x = pos.x - legsXlimit;
  start.z = pos.z + legsZlimit;

  end = start;
  end.y -= 5;

  hit[1] = csColliderHelper::TraceBeam (cdsys, sector, start, end,
  	false, closest_tri, isect[1]) != -1;

  // Assuming the bounding box is axis-aligned: (Upper-right point)
  start.x = pos.x + legsXlimit;
  start.z = pos.z + legsZlimit;

  end = start;
  end.y -= 5;

  hit[2] = csColliderHelper::TraceBeam (cdsys, sector, start, end,
  	false, closest_tri, isect[2]) != -1;

  // Assuming the bounding box is axis-aligned: (Lower-right point)
  start.x = pos.x + legsXlimit;
  start.z = pos.z - legsZlimit;

  end = start;
  end.y -= 5;

  hit[3] = csColliderHelper::TraceBeam (cdsys, sector, start, end,
  	false, closest_tri, isect[3]) != -1;

  //printf("Isect (%f %f %f %f)\n",hit[0] ? isect[0].y : -999, hit[1] ? isect[1].y : -999, hit[2] ? isect[2].y: -999, hit[3] ? isect[3].y: -999);

  int notHit = 0;
  int lowest = -1;
  for (int i = 0; i < 4 && notHit <= 1; i++)
  {
    if (!hit[i])
    {
      notHit++;
      lowest = i;
      continue;
    }
    if(notHit == 0)
    {
      if(lowest == -1)
        lowest = i;
      else if (isect[lowest].y > isect[i].y)
        lowest = i;
    }
  }
  if (notHit <= 1)
  {
    switch (lowest)
    {
      case 0:
        plane.Set (isect[1], isect[2], isect[3]);
        break;
      case 1:
        plane.Set (isect[0], isect[2], isect[3]);
        break;
      case 2:
        plane.Set (isect[0], isect[1], isect[3]);
        break;
      case 3:
        plane.Set (isect[0], isect[1], isect[2]);
        break;
    }
    csVector3 normal = plane.GetNormal ().Unit ();

    float newxRot = atan2 (normal.z, normal.y );
    float newzRot = -atan2 (normal.x, normal.y );
    csMatrix3 rotMat = csZRotMatrix3 (newzRot) * csXRotMatrix3 (newxRot - xRot)
    	* csZRotMatrix3 (-zRot);
    pcmesh->GetMesh ()->GetMovable ()->Transform (rotMat);
    xRot = newxRot;
    zRot = newzRot;
  }
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

csTicks celPcLinearMovement::TimeDiff ()
{
  return csGetTicks () - lastDRUpdate;
}

void celPcLinearMovement::ExtrapolatePosition (float delta)
{
  if (path)
  {
    path_time += delta;
    path->CalculateAtTime (path_time);
    csVector3 pos, look, up;

    path->GetInterpolatedPosition (pos);
    path->GetInterpolatedUp (up);
    path->GetInterpolatedForward (look);

    pcmesh->GetMesh ()->GetMovable ()->GetTransform().SetOrigin (pos);
    pcmesh->GetMesh ()->GetMovable ()->GetTransform().LookAt(
    	look.Unit (), up.Unit ());
    pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();

    csRef<iSprite3DState> spstate =
    	scfQueryInterface<iSprite3DState> (pcmesh->GetMesh ()->GetMeshObject ());

    if (spstate && strcmp (path_actions[path->GetCurrentIndex ()],
    	spstate->GetCurAction ()->GetName ()))
    {
      spstate->SetAction (path_actions[path->GetCurrentIndex ()]);
    }
    if (path_time>path->GetTime (path->Length () - 1))
    {
      path = 0;
      path_time = 0;
      iCelBehaviour* behaviour = entity->GetBehaviour ();
      if (behaviour)
      {
        celData ret;
        behaviour->SendMessage ("pclinearmovement_arrived", this, ret, 0);
      }
    }
  }
  else
  {
    // bool rc =
    int move_result = MoveSprite (delta);
    if (move_result==CEL_MOVE_FAIL || move_result==CEL_MOVE_PARTIAL)
    {
      iCelBehaviour* behaviour = entity->GetBehaviour ();
      if (behaviour)
      {
        celData ret;
        if (move_result==CEL_MOVE_FAIL)
          behaviour->SendMessage ("pclinearmovement_stuck", this, ret, 0);
        if (move_result==CEL_MOVE_PARTIAL)
          behaviour->SendMessage ("pclinearmovement_collision", this, ret, 0);
      }
    }
    //if (rc)
    //  pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();
  }
}

void celPcLinearMovement::TickEveryFrame ()
{
  FindSiblingPropertyClasses ();

  if (anchor_needsinit) LoadAnchor (anchor);

  if (!pcmesh)
  {
    MoveReport (object_reg, "No Mesh found on entity!");
    return;
  }

  csTicks elapsed_time = vc->GetElapsedTicks ();
  if (!elapsed_time)
    return;

  float delta = elapsed_time / 1000.0f;
  // Compensate for offset
  OffsetSprite (delta);
  if (fabsf (deltaLimit) > SMALL_EPSILON)
    delta = MIN(delta, deltaLimit);

  // Adjust the properties.
  ExtrapolatePosition (delta);
}

bool celPcLinearMovement::InitCD (iPcCollisionDetection *pc_cd)
{
  FindSiblingPropertyClasses ();
  if (topSize.IsZero () && pcmesh)
  {
    iMeshWrapper* meshWrapper = pcmesh->GetMesh ();
    if (meshWrapper)
    {
      csBox3 worldBoundingBox;
      worldBoundingBox = meshWrapper->GetWorldBoundingBox ();
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
  body.y = (100.0f - percentage) * body.y / 100.0f;
  return InitCD (body, legs, shift, pc_cd);
}

bool celPcLinearMovement::InitCD (const csVector3& body, const csVector3& legs,
	const csVector3& shift, iPcCollisionDetection *pc_cd)
{
  FindSiblingPropertyClasses ();
  if (!pcmesh)
    return MoveReport (object_reg, "No Mesh found on entity!");

  topSize = body;
  bottomSize = legs;

  if (bottomSize.z > 1.0f)
    hugGround = true;

  intervalSize.x = MIN(topSize.x, bottomSize.x);
  intervalSize.y = MIN(topSize.y, bottomSize.y);
  intervalSize.z = MIN(topSize.z, bottomSize.z);

  celPcLinearMovement::shift = shift;

  cdsys = csQueryRegistry<iCollideSystem> (object_reg);

  if (!pc_cd)
  {
    csRef<iCelPropertyClass> pc;
    pc = pl->CreatePropertyClass (entity, "pccollisiondetection");
    if (!pc)
      return MoveReport (object_reg,
      	"Could not create property class pccollisiondetection.");
    csRef<iPcCollisionDetection> pctemp;
    pctemp = scfQueryInterface<iPcCollisionDetection> (pc);

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
    return pccolldet->IsOnGround ();

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

void celPcLinearMovement::GetDRData (bool& on_ground, float& speed,
	csVector3& pos, float& yrot, iSector*& sector, csVector3& vel,
	csVector3& worldVel, float& ang_vel)
{
  on_ground = IsOnGround ();
  speed = this->speed;
  GetLastPosition (pos, yrot, sector);
  vel = velBody;
  ang_vel = angularVelocity.y;
  worldVel = this->velWorld;
}


iSector* celPcLinearMovement::GetSector ()
{
  FindSiblingPropertyClasses ();
  return pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0);
}

// --------------------------------------------------------------------------

void celPcLinearMovement::SetPathAction (int which, const char *action)
{
  path_actions.Put (which,action);
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

void celPcLinearMovement::GetLastFullPosition (csVector3& pos, float& yrot,
	iSector*& sector)
{
  if (!pcmesh || !pcmesh->GetMesh ())
  {
    MoveReport (object_reg, "No Mesh found on entity!");
    return;
  }

  // Position
  pos = pcmesh->GetMesh ()->GetMovable ()->GetFullPosition ();

  // rotation
  csMatrix3 transf = pcmesh->GetMesh ()->GetMovable ()
  	->GetFullTransform ().GetT2O ();
  yrot = Matrix2YRot (transf);

  // Sector
  if (pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->GetCount ())
    sector = pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0);
  else
    sector = 0;
}

void celPcLinearMovement::SetFullPosition (const csVector3& pos, float yrot,
	const iSector* sector)
{
  FindSiblingPropertyClasses ();
  // Position
  csVector3 newpos;
  if (anchor)
  {
    newpos = anchor->GetMesh ()->GetMovable ()->GetFullTransform ().Other2This (
    	pos);
  }
  else
  {
    newpos = pos;
  }
  pcmesh->GetMesh ()->GetMovable ()->SetPosition ((iSector *)sector, newpos);

  // Rotation
  csMatrix3 matrix = (csMatrix3) csYRotMatrix3 (yrot);
  // @@@ Not correct if anchor is transformed!!!
  pcmesh->GetMesh ()->GetMovable ()->GetTransform ().SetO2T (matrix);

  // Sector
  pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();
}

void celPcLinearMovement::SetFullPosition (const char* center_name, float yrot,
	iSector* sector)
{
  csRef<iMapNode> mapnode = CS_GET_NAMED_CHILD_OBJECT (
  	sector->QueryObject (), iMapNode, center_name);
  if (mapnode)
  {
    SetFullPosition (mapnode->GetPosition (), yrot, sector);
  }
  else
  {
    MoveReport (object_reg, "Can't find node '%s'!",
    	(const char*)center_name);
  }
}

void celPcLinearMovement::SetPosition (const csVector3& pos, float yrot,
	const iSector* sector)
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

void celPcLinearMovement::SetPosition (const char* center_name, float yrot,
	iSector* sector)
{
  csRef<iMapNode> mapnode = CS_GET_NAMED_CHILD_OBJECT (
  	sector->QueryObject (), iMapNode, center_name);
  if (mapnode)
  {
    SetPosition (mapnode->GetPosition (), yrot, sector);
  }
  else
  {
    MoveReport (object_reg, "Can't find node '%s'!",
    	(const char*)center_name);
  }
}

void celPcLinearMovement::SetDRData (bool on_ground, float speed,
	csVector3& pos, float yrot, iSector *sector, csVector3& vel,
	csVector3& worldVel, float ang_vel)
{
  if (pccolldet)
    pccolldet->SetOnGround (on_ground);

  this->speed = speed;
  SetPosition (pos,yrot,sector);
  SetVelocity (vel);
  ClearWorldVelocity ();
  AddVelocity (worldVel);
  csVector3 rot (0.0f, ang_vel, 0.0f);
  SetAngularVelocity (rot);
  lastDRUpdate = csGetTicks ();
}

void celPcLinearMovement::SetSoftDRData (bool on_ground, float speed,
	csVector3& pos, float yrot, iSector *sector, csVector3& vel,
	csVector3& worldVel, float ang_vel)
{
  if (pccolldet)
    pccolldet->SetOnGround (on_ground);

  csVector3 cur_pos;
  float cur_rot;
  iSector *cur_sect;
  GetLastPosition (cur_pos, cur_rot, cur_sect);
  if (cur_sect == sector)
  {
    offset_err = pos - cur_pos;
    // Check for NaN conditions:
    if (offset_err.x != offset_err.x) offset_err.x = 0.0f;
    if (offset_err.y != offset_err.y) offset_err.y = 0.0f;
    if (offset_err.z != offset_err.z) offset_err.z = 0.0f;
    offset_rate = offset_err;
    SetPosition (cur_pos, yrot, sector);
  }
  else
  {
    offset_rate = offset_err = csVector3 (0.0f, 0.0f ,0.0f);
    SetPosition (pos, yrot, sector);
  }

  this->speed = speed;
  SetVelocity (vel);
  ClearWorldVelocity ();
  AddVelocity (worldVel);
  csVector3 rot (0.0f, ang_vel, 0.0f);
  SetAngularVelocity (rot);
  lastDRUpdate = csGetTicks ();
}

csPtr<iCelDataBuffer> celPcLinearMovement::GetPersistentData (
	celPersistenceType persistence_type)
{
  if (persistence_type == CEL_PERSIST_TYPE_RECORD_FIRST_PASS)
    return SaveFirstPass ();

  if (persistence_type == CEL_PERSIST_TYPE_RECORD)
    return Save ();

  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (LINMOVE_SERIAL);

  bool on_ground;
  float speed;
  csVector3 pos;
  float yrot;
  iSector* sector;
  csVector3 vel;
  csVector3 worldVel;
  float ang_vel;
  GetDRData (on_ground, speed, pos, yrot, sector, vel, worldVel, ang_vel);

  databuf->Add (on_ground);
  databuf->Add (speed);
  databuf->Add (pos);
  databuf->Add (yrot);
  databuf->Add (vel);
  databuf->Add (worldVel);
  databuf->Add (ang_vel);

  return csPtr<iCelDataBuffer> (databuf);
}

celPersistenceResult celPcLinearMovement::SetPersistentData (csTicks data_time,
	iCelDataBuffer* databuf, celPersistenceType persistence_type)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != LINMOVE_SERIAL) return CEL_PERSIST_RESULT_ERROR;

  if (persistence_type == CEL_PERSIST_TYPE_RECORD_FIRST_PASS)
  {
    LoadFirstPass (databuf);
    return CEL_PERSIST_RESULT_OK;
  }

  if (persistence_type == CEL_PERSIST_TYPE_RECORD)
  {
    Load (databuf);
    return CEL_PERSIST_RESULT_OK;
  }

  // TODO: use some smooth update when data forced by the server
  if (persistence_type == CEL_PERSIST_TYPE_SERVER_FORCING)
    return CEL_PERSIST_RESULT_OK;

  bool dr_on_ground = databuf->GetBool ();
  float dr_speed = databuf->GetFloat ();
  csVector3 dr_pos;
  databuf->GetVector3 (dr_pos);
  float dr_yrot = databuf->GetFloat ();
  csVector3 dr_vel;
  databuf->GetVector3 (dr_vel);
  csVector3 dr_worldVel;
  databuf->GetVector3 (dr_worldVel);
  float dr_ang_vel = databuf->GetFloat ();

  // TODO: use data_time for DR data time
  // TODO: send also sector
  SetSoftDRData (dr_on_ground, dr_speed, dr_pos, dr_yrot, GetSector (),
  	dr_vel, dr_worldVel, dr_ang_vel);

  return CEL_PERSIST_RESULT_OK;
}
