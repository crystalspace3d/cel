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

extern void MoveReport (iObjectRegistry* object_reg, const char* msg, ...);

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
}

#define COLLDET_SERIAL 27

csPtr<iCelDataBuffer> celPcCollisionDetection::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (COLLDET_SERIAL);
  databuf->SetDataCount (3);
  int j = 0;

  databuf->GetData (j++)->Set (topSize);
  databuf->GetData (j++)->Set (bottomSize);
  databuf->GetData (j++)->Set (shift);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcCollisionDetection::Load (iCelDataBuffer* databuf)
{
  int seriallnr = databuf->GetSerialNumber ();
  if (seriallnr != COLLDET_SERIAL)
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

  if (!Init (topSize, bottomSize, shift))
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


/*
 * Terminal velocity
 * ((120 miles/hour  / 3600 second/hour) * 5280 feet/mile)
 *     / 3.28 feet/meter = 53.65 m/s
 *   *2.0 for "feel" correction = 107.3m/s
 */
#define ABS_MAX_FREEFALL_VELOCITY 107.3f


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
  csMatrix3 identitymatrix;
  csOrthoTransform transform_newpos (identitymatrix, newpos);
  iSector* current_sector=movable->GetSectors ()->Get (0);

  csMatrix3 mat;

  csReversibleTransform rt = movable->GetFullTransform ();
  mat = rt.GetT2O ();

  csOrthoTransform transform_oldpos = csReversibleTransform (csMatrix3(),
    oldpos);

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
    csPlane3 obstacle (cd.a2, cd.b2, cd.c2);
    csVector3 vec = obstacle.Normal().Unit();

    if (vec * localvel > 0) continue;

    csVector3 line[2];
    if (vel.y > 0 && FindIntersection (cd, line))
    {
      // Hit above head?
      if (ABS(newpos.y - MAX(line[0].y,line[1].y)) >
        topSize.y + bottomSize.y)
        // Ouch!
        vel.y = 0;
    }

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

  // Falling unless proven otherwise
  onground = false;
  bool downstairs;

  // Only able to step down if we aren't jumping or falling
  if (hits > 0 || vel.y != 0)
    downstairs = false;
  else
  {
    downstairs = true;

    // Try testing 'stepping' down
    newpos.y -= bottomSize.y / 2;
    transform_newpos = csOrthoTransform (csMatrix3(), newpos);

    num_our_cd = hits = 0;

    cdsys->SetOneHitOnly (false);
    cdsys->ResetCollisionPairs ();	

    hits += CollisionDetect (bottomCollider, current_sector,
      &transform_newpos, &transform_oldpos);
  }

  if (hits > 0)
  {
    float max_y = -1e9;
    for (i = 0; i < num_our_cd; i++ )
    {
      csCollisionPair cd = our_cd_contact[i];
      csVector3 n = ((cd.c2-cd.b2)%(cd.b2-cd.a2)).Unit ();

      // Is it a collision with a ground polygon?
      //  (this tests for the angle between ground and collidet
      //  triangle)
      if (-n.y < 0.7)
        continue;

      // Hit a ground polygon so we are not falling
      onground = true;
      csVector3 line[2];
      if (FindIntersection (cd,line))
        max_y = MAX(MAX(line[0].y, line[1].y),max_y);
    }
    if (onground)
    {
      if (max_y <= newpos.y + bottomSize.y && max_y != -1e9)
        newpos.y = max_y - 0.01f;

      if (vel.y < 0 )
        vel.y = 0;
    }
  }

  if (!onground)
  {
    // Reaction force - Disabled because no distinction made between physics
    // engine predicted velocity and player controlled velocity

    // vel = (mat.GetInverse()*localvel)/delta;

    if (downstairs)
      // No steps here, so readjust position back up
      newpos.y += bottomSize.y / 2;

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


  if ((newpos - oldpos).IsZero(0.0001f))
  {
    // No movement
    newpos = oldpos;
    return false;
  }

  // Part 2.5: check top again and revert move if we're still in a wall.
  num_our_cd = hits = 0;
  cdsys->SetOneHitOnly (false);
  cdsys->ResetCollisionPairs ();
  transform_newpos = csOrthoTransform (csMatrix3(), newpos);

  if (CollisionDetect (topCollider, current_sector,
    &transform_newpos,&transform_oldpos) > 0)
  {
    // No move possible without a collision
    newpos = oldpos;
    revertMove = true;
    return false;
  }
  else
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
  celPcCollisionDetection::shift = _shift;

  radiusCD = 0.1f + sqrt (
  	pow (topSize.x,2.0f)+pow (topSize.z,2.0f)
	+pow (topSize.y + bottomSize.y,2.0f));
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
  {
    MoveReport (object_reg, "Error while setting up CD!");
    return false;
  }

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
  int j;

  csVector3 testpos;
  csVector3 line[2];
  csCollisionPair temppair;


  csCollisionPair* CD_contact;

  csRef<iMeshWrapperIterator> objectIter = engine->GetNearbyMeshes (sector,
        old_transform->GetOrigin (),
        radiusCD,
        true);

  while (objectIter->HasNext ())
  {
    iMeshWrapper* meshWrapper = objectIter->Next ();

    iMovable* meshMovable = meshWrapper->GetMovable();
    // Avoid hitting the mesh from this entity itself.
    if (meshWrapper != pcmesh->GetMesh ())
    {
      cdsys->ResetCollisionPairs ();
      csReversibleTransform tr = meshMovable->GetFullTransform ();
      iCollider* othercollider = FindCollider (meshWrapper->QueryObject ());
      if (othercollider && cdsys->Collide (collider,
                transform, othercollider, &tr))
      {
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
          csReversibleTransform temptrans(*old_transform);

          // Move the triangles from object space into world space
          temppair.a1 = transform->This2Other (CD_contact[j].a1+shift);
          temppair.b1 = transform->This2Other (CD_contact[j].b1+shift);
          temppair.c1 = transform->This2Other (CD_contact[j].c1+shift);
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
          if (FindIntersection (temppair, line))
          {
              // Collided at this line segment. Pick a point in the middle of
              // the segment to test.
              testpos=(line[0]+line[1])/2;
          }
          else
          {
              // No collision found, use the destination of the move.
              testpos=transform->GetO2TTranslation ();
;
          }

          // This follows a line segment from start to finish and returns
          // the sector you are ultimately in.
          CollisionSector = sector->FollowSegment (temptrans,
                        testpos, mirror, CEL_LINMOVE_FOLLOW_ONLY_PORTALS);

          // Iterate through all the sectors of the destination mesh,
          // incase it's in multiple sectors.
          iSectorList * sectors = meshMovable->GetSectors();
          sector_max = sectors->GetCount ();
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

        // We don't increase hits unless a collision really occurred after
        // all tests.
        if (reallycollided)
        {
          hits++;
        }

        if (cdsys->GetOneHitOnly () && hits) return 1;
      }
    }
  }
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


