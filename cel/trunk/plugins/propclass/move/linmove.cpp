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

#include <imesh/thing/thing.h>
#include <imesh/thing/polygon.h>
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

#include "plugins/propclass/move/linmove.h"

extern void MoveReport (iObjectRegistry* object_reg, const char* msg, ...);

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
  if (!vc) MoveReport (object_reg, "iVirtualClock Missing!");

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  if (!engine)
  {
    MoveReport (object_reg, "Engine missing!");
    return;
  }

  csRef<iCelPlLayer> player = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  pl = player;
  if (!pl)
  {
    MoveReport (object_reg, "Physical layer missing!");
    return;
  }

  vel.x = vel.z = 0;
  vel.y = 0;
  angularVelocity.x = angularVelocity.y = angularVelocity.z  = 0;
  camera_pitchspeed = 0;

  topCollider = 0;
  bottomCollider = 0;

  pcmesh = 0;
  pccolldet = 0;
  path = 0;
  path_speed = 0;
  path_time  = 0;

  DG_TYPE (this, "celPcLinearMovement ()");

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
  databuf->SetDataCount (5);
  int j = 0;

  databuf->GetData (j++)->Set (topSize);
  databuf->GetData (j++)->Set (bottomSize);
  databuf->GetData (j++)->Set (shift);
  databuf->GetData (j++)->Set (vel);
  databuf->GetData (j++)->Set (angularVelocity);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcLinearMovement::Load (iCelDataBuffer* databuf)
{
  int seriallnr = databuf->GetSerialNumber ();
  if (seriallnr != LINMOVE_SERIAL)
    return false;
  if (databuf->GetDataCount () != 5)
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

  if (!InitCD (topSize, bottomSize, shift, NULL))
    return false;

  cd = databuf->GetData (3);
  vel.x = cd->value.v.x;
  vel.y = cd->value.v.y;
  vel.z = cd->value.v.z;

  cd = databuf->GetData (4);
  angularVelocity.x = cd->value.v.x;
  angularVelocity.y = cd->value.v.y;
  angularVelocity.z = cd->value.v.z;

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

  if (pccolldet)
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

  if (!pccolldet || delta)
  {
    if (MoveV (delta))
      rc = true;
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
  {
    pccolldet->AdjustForCollisions(oldpos,newpos,vel,delta,movable);
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

void celPcLinearMovement::UpdateDR (csTicks ticks)
{
  float delta = ticks - lastDRUpdate;
  delta /= 1000;
  ExtrapolatePosition (delta);
  lastDRUpdate = ticks;
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
    pcmesh->GetMesh ()->GetMovable ()->GetTransform().LookAt(look.Unit(),up.Unit());
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
    bool rc = MoveSprite (delta);
    rc = RotateV (delta) || rc;
    if (rc)
      pcmesh->GetMesh ()->GetMovable ()->UpdateMove ();
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
      MoveReport (object_reg, "No Mesh found on entity!");
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

bool celPcLinearMovement::InitCD (iPcCollisionDetection *pc_cd)
{
  return InitCD (topSize, bottomSize, shift, pc_cd);
}

bool celPcLinearMovement::InitCD (const csVector3& body, const csVector3& legs,
	const csVector3& shift,iPcCollisionDetection *pc_cd)
{
  if (!pcmesh)
  {
    csRef<iPcMesh> pcmeshref;
    pcmeshref = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);

    if (!pcmeshref)
    {
      MoveReport (object_reg, "No Mesh found on entity!");
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

  if (!pc_cd)
  {
    csRef<iCelPropertyClass> pc;
    pc = pl->CreatePropertyClass(entity, "pccollisiondetection");
    if ( !pc )
    {
        MoveReport (object_reg, "Could not create property class pccollisiondetection.");
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

  return pccolldet->Init(topSize,bottomSize,shift);
}

bool celPcLinearMovement::IsOnGround () const
{
  if (pccolldet)
    return pccolldet->IsOnGround();

  return false;
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

bool celPcLinearMovement::SetDRData (iDataBuffer* data,bool detectcheat, csStringHash* msgstrings)
{
  if (data->GetSize () <= sizeof (float) * 8)
  {
    MoveReport (object_reg,
    	"Received DR packet with wrong site: %u should be at least %u\n",
	data->GetSize (), sizeof (float) * 11+1);
    return false;
  }	

  char *ptr = (char *) data->GetData ();
  if ((*ptr) & LINMOVE_PATH_FLAG)
    return SetPathDRData (data);
  else
    if (pccolldet)
      pccolldet->SetOnGround(*ptr++);
    else
      ptr++;

  float* tptr = (float*) ptr;

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

  csStringID* sidptr = (csStringID*)tptr;
  csStringID sectorNameStrId = *(sidptr++);

  // Get onground flag
  // Find Action
  char* actionnum = (char*) sidptr;
  int i = *actionnum++;

  const char* sectorName = (sectorNameStrId == csInvalidStringID) ?
      actionnum : msgstrings->Request(sectorNameStrId);

  csRef<iSprite3DFactoryState> factstate = SCF_QUERY_INTERFACE (
    pcmesh->GetMesh ()->GetFactory ()->GetMeshObjectFactory (),
    iSprite3DFactoryState);

  csRef<iSpriteCal3DState> cal3dstate;
  if (!factstate)
  {
    cal3dstate = SCF_QUERY_INTERFACE (
      pcmesh->GetMesh ()->GetMeshObject (),
      iSpriteCal3DState);
  }

  const char* action;
  if (!factstate) // maybe cal3d sprite
  {
    cal3dstate->SetVelocity(-vel.z);
    action = 0;
  }
  else if (i < 0)
    action = factstate->GetAction (-i)->GetName ();
  else
    action = factstate->GetAction (i)->GetName ();

  // The rest of the data contains the sector name, just cast it to char* and
  // compare with current sector name.
  if (strcmp (pcmesh->GetMesh ()->GetMovable ()->GetSectors ()->Get (0)
  	->QueryObject ()->GetName (), sectorName) != 0)
  {
    // Sector changed, find new sector and set it
    iSector* newSector = engine->GetSectors ()->FindByName (sectorName);
    if (!newSector)
      MoveReport (object_reg,
      	"[celPcLinearMovement::SetDRData] Sector %s not found !\n", sectorName);
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

void celPcLinearMovement::SetPathAction (int which, const char *action)
{
  path_actions.Put(which,action);
}

bool celPcLinearMovement::SetPathDRData (iDataBuffer* data)
{
  char *ptr = (char *)data->GetData ();
  ptr++; // skip LINMOVE_PATH_FLAG;
  
  // sector name is next.  path must be all in one sector.
  path_sector = ptr;
  ptr += path_sector.Length()+1;

  int count = *(ptr++); // add number of points to follow

  if (!path)
    path = new csPath(count);

  float *fptr = (float *)ptr;
  path_time = *(fptr++);
  path_speed = *(fptr++);

  int i;
  for (i=0; i<count; i++)
  {
    csVector3 vec;
    vec.x = *(fptr++);
    vec.y = *(fptr++);
    vec.z = *(fptr++);
    path->SetPositionVector (i,vec);

    vec.x = *(fptr++);
    vec.y = *(fptr++);
    vec.z = *(fptr++);
    path->SetUpVector (i,vec);

    vec.x = *(fptr++);
    vec.y = *(fptr++);
    vec.z = *(fptr++);
    path->SetForwardVector (i,vec);

    path->SetTime (i,*(fptr++) );
  }
  // now add action list
  ptr = (char *)fptr;
  for (i=0; i<count; i++)
  {
    path_actions.Put(i,ptr);
    ptr += strlen(ptr)+1;
  }
  return true;
}

csPtr<iDataBuffer> celPcLinearMovement::GetPathDRData()
{
  int len = path->Length();

  // width is pos,up,forward vector + time value = 10 floats
  int width = 10*sizeof(float);

  len *= width;

  len += path_sector.Length () + 1;  // alloc space for sector name
  len += 1;		 	       // alloc space to specify number of points
  len += 1;		 	       // leading flag saying this is path
  len += 2*sizeof(float);	    // alloc for speed and path_time

  for (int j=0; j<path->Length(); j++)
  {
    if (path_actions[j])
    {
      len += strlen(path_actions[j]) + 1;
    }
    else
      len++; // still add NULL term char for blank actions
  }

  csRef<iDataBuffer> databuf = csPtr<iDataBuffer> (new csDataBuffer (len));

  char *ptr = (char *)databuf->GetData ();
  *(ptr++) = LINMOVE_PATH_FLAG;
  strcpy(ptr,path_sector);
  ptr += path_sector.Length()+1;

  *(ptr++) = path->Length();   // add number of points to follow

  float *fptr = (float *)ptr;
  *(fptr++) = path_time;
  *(fptr++) = path_speed;

  int i;
  for (i=0; i<path->Length(); i++)
  {
    csVector3 vec;
    path->GetPositionVector(i,vec);
    *(fptr++) = vec.x;
    *(fptr++) = vec.y;
    *(fptr++) = vec.z;

    path->GetUpVector(i,vec);
    *(fptr++) = vec.x;
    *(fptr++) = vec.y;
    *(fptr++) = vec.z;

    path->GetForwardVector(i,vec);
    *(fptr++) = vec.x;
    *(fptr++) = vec.y;
    *(fptr++) = vec.z;

    *(fptr++) = path->GetTime(i);
  }
  // now add action list
  ptr = (char *)fptr;
  for (i=0; i<path->Length(); i++)
  {
    if (path_actions[i])
    {
      strcpy(ptr,path_actions[i]);
      ptr += strlen(path_actions[i])+1;
    }
    else
    {
      *(ptr++) = 0;
    }
  }
  path_sent = true;

  return csPtr<iDataBuffer> (databuf);
}

csPtr<iDataBuffer> celPcLinearMovement::GetDRData (csStringHash* msgstrings)
{
  if (path)
      return GetPathDRData ();

  // Pack the important stuff into a databuffer.
  const char* sectorName = pcmesh->GetMesh ()->GetMovable ()->GetSectors ()
  	->Get (0)->QueryObject ()->GetName ();
  csStringID sectorNameStrId = msgstrings ?
      msgstrings->Request(sectorName) : csInvalidStringID;
  int sectorNameLen = (sectorNameStrId == csInvalidStringID) ? strlen (sectorName) : 0;
  // It should be (sectorNameLen + 1), but for some reason crashes occur
  // if smaller than (sectorNameLen + 4).
  int bufLen = (sizeof (float) * 8) + sizeof(csStringID)
      + (sizeof (char) * (sectorNameLen + 4)) + 1 + 1;

  csRef<iDataBuffer> databuf = csPtr<iDataBuffer> (new csDataBuffer (bufLen));
  const csMatrix3& transf =
    pcmesh->GetMesh ()->GetMovable ()->GetTransform ().GetT2O ();

  char *ptr = (char *) databuf->GetData ();
  *ptr++ = (pccolldet)?pccolldet->IsOnGround():0;

  float* tptr = (float*)ptr;

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

  // string ID
  csStringID* sidptr = (csStringID*)tptr;
  *(sidptr++) = sectorNameStrId;

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

  char* anum = (char*) sidptr;
  // Persist onground flag too so each client can do
  // its own gravity for all entities.
  *anum++ = action_number;

  // Send sector name.
  if (sectorNameStrId == csInvalidStringID)
  {
      char* dest = anum;
      strncpy (dest, sectorName, sectorNameLen);
      dest[sectorNameLen]=0;
  }

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

  if (path && !path_sent)
  {
    priority = 1;
    return true;
  }

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

