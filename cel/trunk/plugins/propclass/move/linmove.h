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

#ifndef __CEL_PF_LINEAR_MOVE_FACT__
#define __CEL_PF_LINEAR_MOVE_FACT__

//CS Includes
#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
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
#include "propclass/linmove.h"
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
struct iPcCollisionDetection;

/* Max deviation before we need to send
   a new DR packet */
#define CELPCLINEARMOVEMENT_DRDELTA 0.2


CEL_DECLARE_FACTORY (LinearMovement)

/**
 * Movement related class.
 * This class handles everything related to moving, from collision detection to
 * dead reckoning and actual entity movement.
 */
class celPcLinearMovement : public celPcCommon
{
protected:
  csWeakRef<iPcMesh> pcmesh;
  csWeakRef<iPcCollisionDetection> pccolldet;
  csRef<iEngine> engine;
  csRef<iVirtualClock> vc;

  // Linear vars
  // Actual velocity
  csVector3 angularVelocity;
  csVector3 angleToReach;
  bool angleToReachFlag;
  csVector3 vel;
  // Target velocity
  csVector3 targAngularVelocity;
  csVector3 targVel;
  float speed;
  bool stationary;
  float angDelta;

  // Path vars
  csRef<iPath> path;
  float path_time,path_speed;
  csStringArray path_actions;
  bool path_sent;
  csString path_sector;

  //Collision vars
  csVector3 shift;
  csRef<iCollider> topCollider;
  csVector3 topSize;

  csRef<iCollider> bottomCollider;
  csVector3 bottomSize;

  // Maximum interval at which to perform CD
  csVector3 intervalSize;

  csTicks lastDRUpdate;
  float deltaLimit;
  
  bool RotateV (float delta);
  // Move local entity
  bool MoveV (float delta);
  bool MoveSprite (float delta);

  // Returns a list of sectors near a position.
  int FindSectors (const csVector3& pos, float size, iSector** sectors);

  void FindSiblingPropertyClasses ();

  static csStringID id_body;
  static csStringID id_legs;
  static csStringID id_offset;
  static csStringID action_initcd;

public:
  celPcLinearMovement (iObjectRegistry* object_reg);
  virtual ~celPcLinearMovement ();

  void SetAngularVelocity (const csVector3& angle);
  void SetAngularVelocity (const csVector3& angle,
  	const csVector3& angle_to_reach);
  void SetVelocity (const csVector3& vel );
  void GetVelocity (csVector3& v) const;
  /**
   * Get the current angular velocity vector.
   */
  void GetAngularVelocity (csVector3& v) const
  {
    v = angularVelocity;
  }

  bool InitCD (const csVector3& body, const csVector3& legs,
  	const csVector3& shift,iPcCollisionDetection *pc_cd = 0);
  void GetCDDimensions (csVector3& body, csVector3& legs,
	csVector3& shift, iPcCollisionDetection*& pc_cd);
  bool InitCD (iPcCollisionDetection *pc_cd=0);
  void SetSpeed ( float speedz );

  void GetLastPosition (csVector3& pos,float& yrot, iSector*& sector);
  void SetPosition (const csVector3& pos,float yrot, const iSector* sector);

  bool IsOnGround () const;
  bool IsPath() const { return (path != 0); }


  /// Return all necessary data for Dead Reckoning
  void GetDRData(bool& on_ground,
                 float& speed,
                 csVector3& pos,
                 float& yrot,
                 iSector*& sector,
                 csVector3& vel,
                 float& ang_vel);

  /// Sets all relevant dead reckoning data on this entity
  virtual void SetDRData(bool on_ground,float speed,
                         csVector3& pos,float yrot,iSector *sector,
                         csVector3& vel,float ang_vel);


  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pclinearmovement"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);
  virtual void TickEveryFrame ();

  iSector* GetSector ();

  /**
   * This function actually moves and rotates the mesh, relighting
   * if necessary.
   */
  void ExtrapolatePosition (float delta);

  /**
   * This function calls ExtrapolatePosition with a certain time
   * but uses a fixed delta.  This allows all entities linmoves to be
   * synchronized to the same ticks, even if updates are all
   * happening at different times.
   */
  void UpdateDRDelta (csTicks ticksdelta);
  /**
   * This function calls ExtrapolatePosition with a certain time delta
   * or calculates the delta relative to the last update.
   * This allows all entities linmoves to be
   * synchronized to the same ticks, even if updates are all
   * happening at different times.
   */
  void UpdateDR(csTicks delta);
  void UpdateDR();

  /**
   * This function lets linmove store a ref to the supplied
   * iPath.  If this path is present, it will be used for
   * movement instead of linear velocity vector.
   */
  void SetPath (iPath *newpath)
  { path = newpath; path_sent = false; }

  /**
   * This function sets the current position on the path
   * for use when time deltas are added later.
   */
  void SetPathTime (float timeval)
  { path_time = timeval; }

  /**
   * This relates the movement of the entity
   * along the path to the time values specified
   * by the path.  Speed=0 makes the entity
   * stationary.  Speed=1 is normal traversal
   * of the path.
   */
  void SetPathSpeed (float speed)
  { path_speed = speed; }

  /**
   * This relates a particular action name
   * to be used between two points in the path.
   * This allows linmove to automatically
   * switch a bird from "fly" to "glide", for
   * example, during downward segments of the
   * flight path.
   */
  void SetPathAction (int which, const char *action);

  /**
   * This sets the sector which will be used
   * for the entire path.
   */
  void SetPathSector (const char *sectorname)
  { path_sector = sectorname; }

  void SetDeltaLimit(float deltaLimit)
  { this->deltaLimit = deltaLimit; }

  struct PcLinearMovement : public iPcLinearMovement
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcLinearMovement);

    virtual void GetLastPosition (csVector3& pos,float& yrot, iSector*& sector)
    {
      scfParent->GetLastPosition (pos,yrot, sector);
    }

    virtual void SetPosition (const csVector3& pos,float yrot,
    	const iSector* sector)
    {
      scfParent->SetPosition (pos,yrot, sector);
    }

    virtual void SetSpeed (float speedz) { scfParent->SetSpeed (speedz); }

    virtual void SetAngularVelocity (const csVector3& angle)
    {
      scfParent->SetAngularVelocity (angle);
    }	  

    void SetAngularVelocity (const csVector3& angle,
    	const csVector3& angle_to_reach)
    {
      scfParent->SetAngularVelocity (angle, angle_to_reach);
    }

    virtual void SetVelocity (const csVector3& vel)
    {
      scfParent->SetVelocity (vel);
    }

    virtual void GetVelocity (csVector3& v) const
    {
      scfParent->GetVelocity (v);
    }
    virtual void GetAngularVelocity (csVector3& v) const
    {
      scfParent->GetAngularVelocity (v);
    }

    virtual bool InitCD (const csVector3& top, const csVector3& bottom,
    	const csVector3& shift,iPcCollisionDetection *pc_cd=0)
    {
      return scfParent->InitCD (top, bottom, shift,pc_cd);
    }
    virtual bool InitCD (iPcCollisionDetection *pc_cd=0)
    {
      return scfParent->InitCD (pc_cd);
    }
    virtual void GetCDDimensions (csVector3& body, csVector3& legs,
	csVector3& shift, iPcCollisionDetection*& pc_cd)
    {
      scfParent->GetCDDimensions (body, legs, shift, pc_cd);
    }
    virtual bool IsOnGround () const
    {
      return scfParent->IsOnGround ();
    }
    virtual bool IsPath() const
    {
      return scfParent->IsPath ();
    }

    virtual void GetDRData(bool& on_ground,
                           float& speed,
                           csVector3& pos,
                           float& yrot,
                           iSector*& sector,
                           csVector3& vel,
                           float& ang_vel)
    {
      scfParent->GetDRData(on_ground,speed,pos,yrot,sector,vel,ang_vel);
    }
    virtual void SetDRData(bool on_ground,float speed,
                           csVector3& pos,float yrot,iSector *sector,
                           csVector3& vel,float ang_vel)
    {
      scfParent->SetDRData(on_ground,speed,pos,yrot,sector,vel,ang_vel);
    }

    virtual iSector *GetSector ()
    {
      return scfParent->GetSector ();
    }
    virtual void ExtrapolatePosition (float delta)
    {
      scfParent->ExtrapolatePosition (delta);
    }
    virtual void UpdateDR(csTicks ticks)
    {
      scfParent->UpdateDR(ticks);
    }
    virtual void UpdateDRDelta (csTicks ticksdelta)
    {
      scfParent->UpdateDRDelta(ticksdelta);
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

    void SetDeltaLimit(float deltaLimit)
    {
      scfParent->SetDeltaLimit(deltaLimit);
    }
  } scfiPcLinearMovement;

protected:
  static csCollisionPair our_cd_contact[1000];
  static int num_our_cd;
};

#endif 

