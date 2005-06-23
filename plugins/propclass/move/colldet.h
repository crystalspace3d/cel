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

#ifndef __CEL_PF_COLL_DET_FACT__
#define __CEL_PF_COLL_DET_FACT__

//CS Includes
#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/strhash.h"
#include "csutil/stringarray.h"
#include "cstool/collider.h"
#include "ivaria/collider.h"
#include "csutil/sysfunc.h"

//CEL includes
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "propclass/mesh.h"
#include "propclass/colldet.h"
#include "celtool/stdpcimp.h"

#define CEL_LINMOVE_FOLLOW_ONLY_PORTALS true

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;
struct iVirtualClock;
struct iCollider;
struct iCollideSystem;
struct iSector;
class csReversibleTransform;
class csObject;
class csColliderWrapper;
struct iPath;

/* Max deviation before we need to send
   a new DR packet */
//#define CELPCLINEARMOVEMENT_DRDELTA 0.2


CEL_DECLARE_FACTORY (CollisionDetection)

/**
 * Collision Detection (CD) related class.
 * This class handles CD for linearmovement PC.
 */
class celPcCollisionDetection : public celPcCommon
{
protected:
  iPcMesh* pcmesh;

  csRef<iCollideSystem> cdsys;
  csRef<iEngine> engine;
//  csRef<iVirtualClock> vc;

  // This is the distance the CD will use to look for objects to collide with.
  csBox3 boundingBox;

  // Linear vars
//  csVector3 angularVelocity;
//  csVector3 vel;
//  float speed;
//  float camera_pitchspeed;
  

  /// Are we on the ground? flag
  bool onground;

  /// Enable collision detection flag.
  bool useCD;
  /// Should we revert the last attempted move/rotation?
  bool revertMove;

//  bool ready;
//  bool stationary;

  // Path vars
//  csRef<iPath> path;
//  float path_time,path_speed;
//  csStringArray path_actions;
//  bool path_sent;
//  csString path_sector;

  //Collision vars
  csVector3 shift;
  csRef<iCollider> topCollider;
  csVector3 topSize;

  csRef<iCollider> bottomCollider;
  csVector3 bottomSize;

//  csVector3     lastKnownPosition;  
//  csVector3     lastKnownVelocity;
//  csMatrix3     lastKnownMatrix;
//  csVector3     lastKnownRotation;
//  csTicks lastDRUpdate;
//  csTicks accumulatedLag;

//  csVector3 lastSentVelocity;
//  csVector3 lastSentRotation;
//  iSpriteAction* lastSentAction;
  
//  bool RotateV (float delta);
  // Move local entity
//  bool MoveV (float delta);
//  bool MoveSprite (float delta);

  // Try to find the collider for an object. Can return 0 if there is none.
  // This function uses csColliderWrapper if present. If not it will try
  // if there is a pcsolid to get a collider from.
  iCollider* FindCollider (iObject* object);

  // Returns a list of sectors near a position.
  int FindSectors (const csVector3& pos, float size, iSector** sectors);

  /**
   * Performs the collision detection for the provided csColliderWrapper vs
   * all nearby objects.
   * <p>
   * @param cw  	Pointer to a csColliderWrapper that is to be tested for
   *			collisions with nearby objects
   * @param sect	Pointer to the iSector interface of the sector that
   *			this Wrapper is currently in.
   * @param cdt		Pointer to a reversible transform of the "new"
   *			position of the object to test.
   * @param cdstart	Pointer to a reversible transform of the original
   *			position of the object to test (before any movements).
   *<p>
   * This function gets all nearby objects, crossing sector bounds. It compares
   * for collisions. If a collision is found, it follows a line segment from
   * the "old" position of the Mesh (described by cdstart) to the position of
   * one end of a line segment describing the collision. If this results in
   * crossing into the same sector that the mesh we collided with is in, then
   * the collision is valid.
   * <p>
   * This catches the case where a peice of world geometry extends into
   * coordinates of another sector, but does not actually exist in that sector.
   */
  int CollisionDetect (iCollider *cw, iSector* sect,
  	csReversibleTransform *cdt, csReversibleTransform *cdstart);
  /**
   * Performs the collision detection for the provided csColliderWrapper vs
   * all nearby objects and gives the furthest point that will not collide.
   * <p>
   * @param cw  	Pointer to a csColliderWrapper that is to be tested for
   *			collisions with nearby objects
   * @param sect	Pointer to the iSector interface of the sector that
   *			this Wrapper is currently in.
   * @param cdt		Pointer to a reversible transform of the "new"
   *			position of the object to test.
   * @param cdstart	Pointer to a reversible transform of the original
   *			position of the object to test (before any movements).
   * @param maxmove	A vector that holds the maximum point to which the object can move.
   *
   *<p>
   * This function calls CollisionDetect each time splitting the range
   * between the 'new' position and the original position of the object and testing
   * the point in the middle. This finds the point of first contact to an accuracy
   * of EPSILON and then sets maxmove to a point before it.
   * <p>
   */
  int CollisionDetectIterative (iCollider *cw, iSector* sect,
  	csReversibleTransform *cdt, csReversibleTransform *cdstart, csVector3& maxmove);

//  csPtr<iDataBuffer> GetPathDRData ();
//  bool SetPathDRData (iDataBuffer* data);

public:
  celPcCollisionDetection(iObjectRegistry* object_reg);
  virtual ~celPcCollisionDetection();

//  bool HandleEvent (iEvent& ev);

//  void SetRotation (const csVector3& angle);
//  void SetVelocity (const csVector3& vel );
//  void GetVelocity (csVector3& v);
  bool Init (const csVector3& body, const csVector3& legs,
  	         const csVector3& shift);
  bool Init ();
//  void SetSpeed ( float speedz );
//  void SetCameraPitchSpeed (float angle)
//  {
//    camera_pitchspeed = angle;
//  }

//  void GetLastPosition (csVector3& pos,float& yrot, iSector*& sector);
//  void SetPosition (const csVector3& pos,float yrot, const iSector* sector);

  bool IsOnGround () const
  {
    return onground;
  }
  void SetOnGround (bool flag)
  {
    onground = flag;
  }

  void UseCD (bool flag)
  { useCD = flag; }

  bool AdjustForCollisions (csVector3& oldpos,
                            csVector3& newpos,
                            csVector3& vel,
                            float delta,
                            iMovable* movable);

  bool QueryRevert()
  { return revertMove;}

  /// Returns dead reckoning data
//  csPtr<iDataBuffer> GetDRData (csStringHash* msgstrings = 0);

  /**
   * Sets dead reckoning data and moves the entity accordingly
   * If data is 0 then UpdateDR () is called instead.
   */
//  bool SetDRData (iDataBuffer* data,bool detectcheat, csStringHash* msgstrings = 0);
  /**
   * Checks whether there is a need for sending a new DR packet
   * and if so, what the priority of the DR update should be.
   */
//  bool NeedDRData (uint8& priority);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pccollisiondetection"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
//  iSector* GetSector ();

//  void SetReady (bool flag)
//  {
//    ready = flag;
//  }
  
//  bool IsReady () const
//  {
//    return ready;
//  }

  /**
   * This function actually moves and rotates the mesh, relighting
   * if necessary.
   */
//  void ExtrapolatePosition (float delta);

  /**
   * This function calls ExtrapolatePosition with a certain time
   * but calculates the delta relative to the last update, not
   * a fixed delta.  This allows all entities linmoves to be
   * synchronized to the same ticks, even if updates are all
   * happening at different times.
   */
//  void UpdateDR(csTicks ticks);

  /**
   * This function lets linmove store a ref to the supplied
   * iPath.  If this path is present, it will be used for
   * movement instead of linear velocity vector.
   */
//  void SetPath (iPath *newpath)
//  { path = newpath; path_sent = false; }

  /**
   * This function sets the current position on the path
   * for use when time deltas are added later.
   */
//  void SetPathTime (float timeval)
//  { path_time = timeval; }

  /**
   * This relates the movement of the entity
   * along the path to the time values specified
   * by the path.  Speed=0 makes the entity
   * stationary.  Speed=1 is normal traversal
   * of the path.
   */
//  void SetPathSpeed (float speed)
//  { path_speed = speed; }

  /**
   * This relates a particular action name
   * to be used between two points in the path.
   * This allows linmove to automatically
   * switch a bird from "fly" to "glide", for
   * example, during downward segments of the
   * flight path.
   */
//  void SetPathAction (int which, const char *action);

  /**
   * This sets the sector which will be used
   * for the entire path.
   */
//  void SetPathSector (const char *sectorname)
//  { path_sector = sectorname; }

  struct PcCollisionDetection : public iPcCollisionDetection
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCollisionDetection);

//    virtual void GetLastPosition (csVector3& pos,float& yrot, iSector*& sector)
//    {
//      scfParent->GetLastPosition (pos,yrot, sector);
//    }

//    virtual void SetPosition (const csVector3& pos,float yrot,
//    	const iSector* sector)
//    {
//      scfParent->SetPosition (pos,yrot, sector);
//    }

//    virtual void SetSpeed (float speedz) { scfParent->SetSpeed (speedz); }
//    virtual void SetCameraPitchSpeed (float angle)
//    {
//      scfParent->SetCameraPitchSpeed (angle);
//    }

/*************
    virtual void SetRotation (const csVector3& angle)
    {
      scfParent->SetRotation (angle);
    }	  

    virtual void SetVelocity (const csVector3& vel)
    {
      scfParent->SetVelocity (vel);
    }

    virtual void GetVelocity (csVector3& v)
    {
      scfParent->GetVelocity (v);
    }
**************/

    virtual bool Init (const csVector3& top, const csVector3& bottom,
    	const csVector3& shift)
    {
      return scfParent->Init (top, bottom, shift);
    }
    virtual bool Init ()
    {
      return scfParent->Init ();
    }
/**********************
    virtual csPtr<iDataBuffer> GetDRData (csStringHash* msgstrings = 0)
    {
      return scfParent->GetDRData (msgstrings);
    }
    virtual bool SetDRData (iDataBuffer* data,bool detectcheat, csStringHash* msgstrings = 0)
    {
      return scfParent->SetDRData (data,detectcheat,msgstrings);
    }
    virtual bool NeedDRData (uint8& priority)
    {
      return scfParent->NeedDRData (priority);
    }

    virtual iSector *GetSector ()
    {
      return scfParent->GetSector ();
    }
    virtual void SetReady (bool flag)
    {
      scfParent->SetReady (flag);
    }
*****************/

    virtual bool IsOnGround () const
    {
      return scfParent->IsOnGround ();
    }

    virtual void UseCD(bool flag)
    { scfParent->UseCD(flag); }

    virtual void SetOnGround (bool flag)
    {
      scfParent->SetOnGround (flag);
    }

    /**
     * This function takes a position vector, checks against all known
     * colliders, and returns the adjusted position in the same variable.
     */
    virtual bool AdjustForCollisions (csVector3& oldpos,
                                      csVector3& newpos,
                                      csVector3& vel,
                                      float delta,
                                      iMovable* movable)
    {
        return scfParent->AdjustForCollisions (oldpos,newpos,vel,delta,movable);
    }

    virtual bool QueryRevert()
    {
        return scfParent->QueryRevert();
    }

/*****************
    virtual bool IsReady () const
    {
      return scfParent->IsReady ();
    }
    virtual void ExtrapolatePosition (float delta)
    {
      scfParent->ExtrapolatePosition (delta);
    }
    virtual void UpdateDR(csTicks ticks)
    {
      scfParent->UpdateDR(ticks);
    }
    virtual void SetPath (iPath *newpath)
    {
      scfParent->SetPath (newpath);
    }
    virtual void SetPathTime (float timeval)
    {
      scfParent->SetPathTime (timeval);
    }
    virtual void SetPathSpeed (float speed)
    {
      scfParent->SetPathSpeed (speed);
    }
    virtual void SetPathAction (int which, const char *action)
    {
      scfParent->SetPathAction (which, action);
    }
    virtual void SetPathSector (const char *sectorname)
    {
      scfParent->SetPathSector (sectorname);
    }
********/
  } scfiPcCollisionDetection;

/************
  class EventHandler : public iEventHandler
  {
  private:
    celPcLinearMovement* parent;
  public:
    EventHandler (celPcLinearMovement* parent)
    {
      SCF_CONSTRUCT_IBASE (0);
      EventHandler::parent = parent;
    }

    virtual ~EventHandler ()
    {
      SCF_DESTRUCT_IBASE ();
    }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
************/

protected:
  static csCollisionPair our_cd_contact[1000];
  static int num_our_cd;
};

#endif 

