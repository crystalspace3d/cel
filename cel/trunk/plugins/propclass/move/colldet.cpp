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

#include <csutil/debug.h>
#include <csutil/databuf.h>
#include <csutil/plugmgr.h>
#include <iengine/movable.h>
#include <iengine/mesh.h>
#include <iengine/engine.h>
#include <iengine/sector.h>
#include <cstool/collider.h>
#include <ivaria/collider.h>

#include <imesh/thing.h>
#include <imesh/terrain.h>
#include <csgeom/polymesh.h>
#include <igeom/polymesh.h>
#include <igeom/objmodel.h>

//CEL includes
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "propclass/camera.h"
#include "propclass/solid.h"

#include "plugins/propclass/move/colldet.h"

extern bool MoveReport (iObjectRegistry* object_reg, const char* msg, ...);

CEL_IMPLEMENT_FACTORY (CollisionDetection, "pccollisiondetection")

SCF_IMPLEMENT_IBASE_EXT (celPcCollisionDetection)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCollisionDetection)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcCollisionDetection::PcCollisionDetection)
  SCF_IMPLEMENTS_INTERFACE (iPcCollisionDetection)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

csCollisionPair celPcCollisionDetection::our_cd_contact[1000];
int celPcCollisionDetection::num_our_cd;

#define LEGOFFSET	  0

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


//----------------------------------------------------------------------------

celPcCollisionDetection::celPcCollisionDetection (iObjectRegistry* object_reg)
  :celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCollisionDetection);

  cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
  if (!cdsys)
  {
    MoveReport (object_reg, "iCollideSystem missing!");
    return;
  }

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);

  topCollider = 0;
  bottomCollider = 0;

  pcmesh = 0;

  onground = false;
  revertMove = false;
}

celPcCollisionDetection::~celPcCollisionDetection ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcCollisionDetection);
}

#define COLLDET_SERIAL 27

csPtr<iCelDataBuffer> celPcCollisionDetection::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (COLLDET_SERIAL);

  databuf->Add (topSize);
  databuf->Add (bottomSize);
  databuf->Add (shift);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcCollisionDetection::Load (iCelDataBuffer* databuf)
{
  int seriallnr = databuf->GetSerialNumber ();
  if (seriallnr != COLLDET_SERIAL)
    return false;

  databuf->GetVector3 (topSize);
  databuf->GetVector3 (bottomSize);
  databuf->GetVector3 (shift);

  if (!Init (topSize, bottomSize, shift))
    return false;

  return true;
}

static inline bool FindIntersection (const csCollisionPair& cd,
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

bool celPcCollisionDetection::AdjustForCollisions (csVector3& oldpos,
                                                   csVector3& newpos,
                                                   csVector3& vel,
                                                   float delta,
                                                   iMovable* movable)
{
  if (!useCD || !topCollider || !bottomCollider)
    return true;

  revertMove = false;

  if (movable->GetSectors()->GetCount() == 0)
    return true;

  int hits, i;
  iSector* current_sector=movable->GetSectors ()->Get (0);

  csMatrix3 id;
  csOrthoTransform transform_oldpos (id, oldpos);
  csOrthoTransform transform_newpos (id, newpos);
  csVector3 maxmove;

  // Part1: find body collisions => movement
  // Find possible colliding sectors.
  csVector3 localvel = (vel * delta);

  // Travel all relevant sectors and do collision detection.

  num_our_cd = 0;
  cdsys->SetOneHitOnly (false);
  cdsys->ResetCollisionPairs ();

  // Perform recursive collision testing to minimise hits and
  // find distance we can travel
  hits = CollisionDetectIterative (topCollider, current_sector,
    &transform_newpos, &transform_oldpos, maxmove);

  // localvel is smaller because we can partly move the object in that direction
  localvel -= maxmove - oldpos;

  for (i = 0; i < num_our_cd; i++ )
  {
    csCollisionPair& cd = our_cd_contact[i];
    csPlane3 obstacle (cd.a2, cd.b2, cd.c2);
    csVector3 vec = obstacle.Normal().Unit();

    if (vec * localvel > 0) continue;

    localvel = -(localvel % vec) % vec;
  }
  newpos = maxmove + localvel;

  transform_newpos = csOrthoTransform (csMatrix3(), newpos);

  // Part2: legs

  num_our_cd = 0;

  transform_newpos = csOrthoTransform (csMatrix3(), newpos);

  cdsys->ResetCollisionPairs ();	

  hits = CollisionDetect (bottomCollider, current_sector,
    &transform_newpos, &transform_oldpos);

  bool stepDown;

  // Only able to step down if we aren't jumping or falling
  if (hits > 0 || vel.y != 0)
    stepDown = false;
  else
  {
    stepDown = true;

    // Try stepping down
    newpos.y -= bottomSize.y/2;
    transform_newpos = csOrthoTransform (csMatrix3(), newpos);

    num_our_cd = 0;

    cdsys->ResetCollisionPairs ();	

    hits = CollisionDetect (bottomCollider, current_sector,
      &transform_newpos, &transform_oldpos);
  }

  // Falling unless proven otherwise
  onground = false;

  float maxJump = newpos.y + bottomSize.y;
  float max_y = -1e9;

  int itercount = 0;

  // Keep moving the model up until it no longer collides
  while (hits > 0 && newpos.y < maxJump)
  {
    bool adjust = false;
    for (i = 0; i < num_our_cd; i++ )
    {
      csCollisionPair cd = our_cd_contact[i];
      csVector3 n = -((cd.c2-cd.b2)%(cd.b2-cd.a2)).Unit ();

      // Is it a collision with a ground polygon?
      //  (this tests for the angle between ground and collidet
      //  triangle)
      if (n.y < 0.7)
        continue;

      // Hit a ground polygon so we are not falling
      onground = adjust = true;
      csVector3 line[2];
      FindIntersection (cd,line);
      max_y = MAX(MAX(line[0].y, line[1].y)+shift.y,max_y);
      if(max_y > maxJump)
      {
        max_y = maxJump;
        break;
      }
    }
    hits = 0;

    if (adjust)
    {
      // Temporarily lift the model up so that it passes the final check
      newpos.y = max_y + 0.01f;
      if(itercount > 4)
        newpos.y = maxJump;
      num_our_cd = 0;

      transform_newpos = csOrthoTransform (csMatrix3(), newpos);

      cdsys->ResetCollisionPairs ();	

      hits = CollisionDetect (bottomCollider, current_sector,
        &transform_newpos, &transform_oldpos);
    }
  }

  if (!onground)
  {
    // Reaction force - Disabled because no distinction made between physics
    // engine predicted velocity and player controlled velocity

    // vel = (mat.GetInverse()*localvel)/delta;

    if (stepDown)
      // No steps here, so readjust position back up
      newpos.y += bottomSize.y / 2;
  }

  // Part 2.5: check top again and revert move if we're still in a wall.
  num_our_cd = 0;
  cdsys->ResetCollisionPairs ();
  transform_newpos = csOrthoTransform (csMatrix3(), newpos);

  // Bring the model back down now that it has passed the final check
  if(onground)
    newpos.y -= 0.02f;

  if (CollisionDetect (topCollider, current_sector,
    &transform_newpos,&transform_oldpos) > 0)
  {
    // No move possible without a collision with the torso
    revertMove = true;
    // If we get 'stuck' on geometry then we should be on some kind of ground
    if(vel.x == 0 && vel.y <= 0 && vel.z == 0)
      onground = true;
    return false;
  }
  //num_our_cd = 0;
  //cdsys->ResetCollisionPairs ();
  //if(CollisionDetect (bottomCollider, current_sector,
  //  &transform_newpos,&transform_oldpos) > 0)
  //{
  //  // No move possible without a collision with the legs
  //  revertMove = true;
  //  return false;
  //}

  return true;
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
    pcmeshref = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);

    if (!pcmeshref)
      return MoveReport (object_reg, "Colldet: No Mesh found on entity!");
    else
      pcmesh = pcmeshref; // Pull ptr out of csRef
  }

  topSize = body;
  bottomSize = legs;
  celPcCollisionDetection::shift = _shift;

  radiusCD = 0.1f + sqrt (
  	topSize.x * topSize.x+topSize.z * topSize.z
	+ (topSize.y + bottomSize.y) * (topSize.y + bottomSize.y));
  csRef<iPolygonMesh> mesh;

  float bX2 = body.x / 2.0f;
  float bZ2 = body.z / 2.0f;
  float bYbottom = legs.y;
  float bYtop = legs.y + body.y;

  mesh = csPtr<iPolygonMesh> (new csPolygonMeshBox (
	csBox3 (csVector3 (-bX2, bYbottom, -bZ2) + shift,
		csVector3 (bX2, bYtop, bZ2) + shift)));
  topCollider = cdsys->CreateCollider (mesh);

  float lX2 = legs.x / 2.0f;
  float lZ2 = legs.z / 2.0f;

  mesh = csPtr<iPolygonMesh> (new csPolygonMeshBox (
	csBox3 (csVector3 (-lX2, LEGOFFSET, -lZ2) + shift,
		csVector3 (lX2, LEGOFFSET + legs.y, lZ2) + shift)));
  bottomCollider = cdsys->CreateCollider (mesh);

  if (!(topCollider && bottomCollider))
    return MoveReport (object_reg, "Error while setting up CD!");

  useCD = true;
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
      csRef<iPcSolid> pcsolid = CEL_QUERY_PROPCLASS (
            ent->GetPropertyClassList (), iPcSolid);
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

#if 0
struct terr
{
  csWeakRef<iTerrainObjectState> terrain;
  iMovable* movable;
};

SCF_VERSION (celTerrainInfo, 0, 0, 1);
struct celTerrainInfo : public csObject
{
  // There is a weak ref so we have to use csSafeCopyArray.
  csSafeCopyArray<terr> terrains;
  SCF_DECLARE_IBASE_EXT (csObject);
};

SCF_IMPLEMENT_IBASE_EXT (celTerrainInfo)
  SCF_IMPLEMENTS_INTERFACE (celTerrainInfo)
SCF_IMPLEMENT_IBASE_EXT_END
#endif

/*
 * Iterates over the nearby meshes to find collisions
 */
int celPcCollisionDetection::CollisionDetect (
	iCollider *collider,
	iSector* sector,
	csReversibleTransform* transform,
	csReversibleTransform* old_transform)
{
#if 0
  // First we check if we find a 'celTerrainInfo' in this sector.
  // This structure is used to quickly find available terrains.
  csRef<celTerrainInfo> ti = CS_GET_CHILD_OBJECT (sector, celTerrainInfo);
  if (!ti)
  {
    ti = csPtr<celTerrainInfo> (new celTerrainInfo ());
    iMeshList* ml = sector->GetMeshes ();
    int i;
    for (i = 0 ; i < ml->GetCount () ; i++)
    {
      iMeshWrapper* m = ml->Get (i);
      csRef<iTerrainObjectState> ts = SCF_QUERY_INTERFACE (m->GetMeshObject (),
      	iTerrainObjectState);
      if (ts)
      {
        terr t;
	t.terrain = ts;
	t.movable = m->GetMovable ();
	ti->terrains.Push (t);
      }
    }
    sector->QueryObject ()->ObjAdd ((csObject*)ti);
  }
#endif

  int hits = 0;

  // Do we need to check if a collision has really occurred because of multiple sectors nearby
  bool checkSectors = false;

  csVector3 testpos;
  csVector3 line[2];
  csCollisionPair temppair;
  float radius = radiusCD + (transform->GetOrigin() - old_transform->GetOrigin()).Norm();

  // Get all the portal meshes in this sector
  const csSet <csPtrKey<iMeshWrapper> > meshWrapSet = sector->GetPortalMeshes();
  csSet <csPtrKey<iMeshWrapper> >::GlobalIterator iter = meshWrapSet.GetIterator();

  // Check if any portal mesh is close to the player object
  while (iter.HasNext() && !checkSectors)
  {
    csBox3 bbox;
    iter.Next()->GetWorldBoundingBox(bbox);
    checkSectors = csIntersect3::BoxSphere(bbox, old_transform->GetOrigin(), radius);
  } 

  csCollisionPair* CD_contact;

  csRef<iMeshWrapperIterator> objectIter = engine->GetNearbyMeshes (sector,
        old_transform->GetOrigin (),
        radius,
        true);

  while (objectIter->HasNext ())
  {
    iMeshWrapper* meshWrapper = objectIter->Next ();

    iMovable* meshMovable = meshWrapper->GetMovable();
    // Avoid hitting the mesh from this entity itself.
    if (meshWrapper != pcmesh->GetMesh ())
    {
      bool collided = false;
      cdsys->ResetCollisionPairs ();
      csReversibleTransform tr = meshMovable->GetFullTransform ();
      iCollider* othercollider = FindCollider (meshWrapper->QueryObject ());
      if(!(othercollider && cdsys->Collide (collider,
        transform, othercollider, &tr)))
        continue;

      // Check if we really collided
      bool reallycollided = false;

#ifdef SHOW_COLLIDER_MESH_DEBUG
      //@@@
      printf("Player in Sector  : %s", sector->QueryObject ()->GetName ());
      printf("Collider Mesh Name  : %s",
        meshWrapper->QueryObject ()->GetName ());
      printf("Collider Mesh Sector: %s\n",
        meshMovable->GetSectors ()->Get (0)->QueryObject ()
        ->GetName ());
#endif
      CD_contact = cdsys->GetCollisionPairs ();
      size_t count = cdsys->GetCollisionPairCount();
      iSectorList * sectors = meshMovable->GetSectors();
      int sector_max = sectors->GetCount ();
      csReversibleTransform temptrans(*old_transform);

      for (size_t j = 0; j < count; j++ )
      {
        /*
        * Here we follow a segment from our current position to the
        * position of the collision. If the sector the collision occured
        * in is not the sector of the mesh we collided with,
        * this is invalid.
        */
        int sector_idx;
        iSector* CollisionSector;
        bool mirror=false;

        // Move the triangles from object space into world space
        temppair.a1 = transform->This2Other (CD_contact[j].a1);
        temppair.b1 = transform->This2Other (CD_contact[j].b1);
        temppair.c1 = transform->This2Other (CD_contact[j].c1);
        if (meshWrapper->GetMovable()->IsFullTransformIdentity())
        {
          temppair.a2 = CD_contact[j].a2;
          temppair.b2 = CD_contact[j].b2;
          temppair.c2 = CD_contact[j].c2;
        }
        else
        {
          temppair.a2 = tr.This2Other (CD_contact[j].a2);
          temppair.b2 = tr.This2Other (CD_contact[j].b2);
          temppair.c2 = tr.This2Other (CD_contact[j].c2);
        }
        if(checkSectors)
        {
          FindIntersection (temppair, line);
          // Collided at this line segment. Pick a point in the middle of
          // the segment to test.
          testpos=(line[0]+line[1])/2;

          // This follows a line segment from start to finish and returns
          // the sector you are ultimately in.
          CollisionSector = sector->FollowSegment (temptrans,
            testpos, mirror, CEL_LINMOVE_FOLLOW_ONLY_PORTALS);

          // Iterate through all the sectors of the destination mesh,
          // incase it's in multiple sectors.
          for (sector_idx=0 ; sector_idx<sector_max ; sector_idx++)
          {
            // Check to see if this sector is the sector of the collision.
            if (CollisionSector == sectors->Get (sector_idx))
            {
              reallycollided = true;
              our_cd_contact[num_our_cd++] = temppair;
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
        else
        {
          reallycollided = true;
          our_cd_contact[num_our_cd++] = temppair;
        }
      }

      // We don't increase hits unless a collision really occurred after
      // all tests.
      if (reallycollided)
      {
        hits++;
        if (cdsys->GetOneHitOnly ()) return 1;
      }
    }
  }

  return hits;
}


int celPcCollisionDetection::CollisionDetectIterative (
	iCollider *collider,
	iSector* sector,
	csReversibleTransform* transform,
	csReversibleTransform* old_transform, csVector3& maxmove)
{
    //int iterations = 0;

    
    // The maximum position it's possible for the player to move to

    // If we collide at the start point or don't collide at the end point then there
    // is no need for recursion.
    int hits = CollisionDetect(collider, sector, transform, old_transform);
    if(hits == 0)
    {
        maxmove = transform->GetOrigin();
        return hits;
    }

    cdsys->ResetCollisionPairs ();
    num_our_cd = 0;

    maxmove = old_transform->GetOrigin();
    hits = CollisionDetect(collider, sector, old_transform, old_transform);
    if(hits > 0)
        return hits;

    // The upper and lower bounds
    csVector3 upper = transform->GetOrigin();
    csVector3 lower = old_transform->GetOrigin();

    csMatrix3 id;

    // The last hit that was made, used so that a hit will always be returned
    csVector3 lastHit = upper;

    //cdsys->SetOneHitOnly(true);
    // Repeatedly split the range with which to test the collision against
    while((upper - lower).SquaredNorm() > EPSILON)
    {
        // Test in the middle between upper and lower bounds
        csOrthoTransform current (id, lower + (upper - lower)/2);
        cdsys->ResetCollisionPairs ();
        num_our_cd = 0;

        hits = CollisionDetect(collider, sector, &current, old_transform);
    
        // Adjust bounds
        if(hits > 0)
        {
            lastHit = lower + (upper - lower)/2;
            upper = lastHit;
        }
        else
        {
            maxmove = lower + (upper - lower) / 2;
            lower = maxmove;
        }
        //iterations++;
    }
    if(hits == 0)
    {
        cdsys->SetOneHitOnly(false);
        // Make sure we actually return a hit
        csOrthoTransform current (id, lastHit);
        cdsys->ResetCollisionPairs ();
        num_our_cd = 0;
        hits = CollisionDetect(collider, sector, &current, old_transform) > 0;
    }

    //printf("Iterations %i Percentage %f\n", iterations, (lastHit - old_transform->GetOrigin()).SquaredNorm()/(transform->GetOrigin() - old_transform->GetOrigin()).SquaredNorm());
    return hits;
}




#define MAXSECTORSOCCUPIED  20

/**
 * WARNING:  At present time this function does not work correctly!
 * <p>
 * The underlying function csEngine::GetNearbySectors () is not implemented.
 * Instead it returns only your current sector.
 */
int celPcCollisionDetection::FindSectors (const csVector3& pos, float radius,
    			   iSector** sectors)
{
  int numsector = 0;

  iSector *curr_sector = pcmesh->GetMesh ()->GetMovable ()->GetSectors ()
  	->Get (0);

  csRef<iSectorIterator> sectorit =
    engine->GetNearbySectors (curr_sector, pos, radius);

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


