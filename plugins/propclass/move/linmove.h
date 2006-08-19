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

#define CEL_MOVE_FAIL 0
#define CEL_MOVE_SUCCEED 1
#define CEL_MOVE_PARTIAL 2
#define CEL_MOVE_DONTMOVE 3

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
class celPcLinearMovement : public scfImplementationExt1<
			    celPcLinearMovement, celPcCommon,
			    iPcLinearMovement>
{
protected:
  csWeakRef<iPcMesh> pcmesh;
  csWeakRef<iPcCollisionDetection> pccolldet;
  csRef<iEngine> engine;
  csRef<iVirtualClock> vc;
  csRef<iCollideSystem> cdsys;

  csWeakRef<iPcMesh> anchor;
  bool anchor_needsinit;

  // Linear vars
  float angDelta;
  bool stationary;
  float speed;
  float gravity;

  bool called;
  csRefArray<iPcGravityCallback> gravityCallbacks;

  csVector3 angularVelocity;
  csVector3 angleToReach;
  bool angleToReachFlag;
  csVector3 velBody;

  csVector3 velWorld;

  /// Should the model be tilted so it's aligned with the ground
  bool hugGround;

  float xRot;
  float zRot;

  // Path vars
  csRef<iPath> path;
  float path_time,path_speed;
  csStringArray path_actions;
  bool path_sent;
  csString path_sector;

  //Collision vars
  csVector3 shift;
  csVector3 topSize;

  csVector3 bottomSize;

  /// The total change in displacement caused by space warping portals.
  csVector3 portalDisplaced;

  // Maximum interval at which to perform CD
  csVector3 intervalSize;

  csTicks lastDRUpdate;
  float deltaLimit;

  // Variables for 'Soft Update' of position
  csVector3 offset_err;    // Error in terms of absolute position
  csVector3 offset_rate;   // Speed to bring error to ZERO within 1s

  // Move local entity
  // both MoveV and MoveSprite return CEL_MOVE_* constants defined above
  int MoveV (float delta);
  int MoveSprite (float delta);
  void OffsetSprite (float delta);

  /**
   * Transform pcmesh so that it follows the curvature of the ground at
   * pos (sector)
   */
  void HugGround(const csVector3& pos, iSector* sector);

  // Returns a list of sectors near a position.
  int FindSectors (const csVector3& pos, float size, iSector** sectors);

  void FindSiblingPropertyClasses ();

  static csStringID action_initcdmesh;
  static csStringID id_percentage;
  static csStringID action_initcd;
  static csStringID id_body;
  static csStringID id_legs;
  static csStringID id_offset;
  static csStringID id_sector;
  static csStringID id_position;
  static csStringID id_yrot;
  static csStringID id_velocity;
  static csStringID action_setposition;
  static csStringID action_setvelocity;
  static csStringID action_addvelocity;
  static csStringID action_setangularvelocity;

  // For properties.
  enum propids
  {
    propid_anchor = 0,
    propid_gravity
  };
  static Property* properties;
  static size_t propertycount;
  void UpdateProperties ();

public:
  celPcLinearMovement (iObjectRegistry* object_reg);
  virtual ~celPcLinearMovement ();

  virtual void SetAngularVelocity (const csVector3& angle);
  virtual void SetAngularVelocity (const csVector3& angle,
  	const csVector3& angle_to_reach);

  /// Sets a velocity for this body in body coordinates
  virtual void SetVelocity (const csVector3& vel)
  {
    // Y movement here is NOT lift and gravity effects. It IS for
    // jumping & jetpacks.
    velBody = vel;
  }

  /// Adds on a velocity to this body in world coordinates
  virtual void AddVelocity (const csVector3& vel)
  {
    // Y movement here can be used for lift and gravity effects.
    velWorld += vel;
  }

  /// Resets the velocity of this body in world coordinates.
  virtual void ClearWorldVelocity ()
  {
    // Y movement here can be used for lift and gravity effects.
    velWorld = 0.0f;
  }

  virtual void GetVelocity (csVector3& v) const
  {
    csVector3 worldVel = pcmesh->GetMesh ()->GetMovable ()->GetTransform ()
    	.Other2ThisRelative (velWorld);

    // Return the composite of the object and world velocity
    // in the OBJECT coordinate system.
    v = worldVel + velBody;
  }

  virtual bool RotateV (float delta);

  /**
   * Get the current angular velocity vector.
   */
  virtual void GetAngularVelocity (csVector3& v) const
  {
    v = angularVelocity;
  }

  virtual bool InitCD (iMeshWrapper* mesh, float percentage,
  	iPcCollisionDetection* pc_cd = 0);
  virtual bool InitCD (const csVector3& body, const csVector3& legs,
  	const csVector3& shift,iPcCollisionDetection *pc_cd = 0);
  virtual void GetCDDimensions (csVector3& body, csVector3& legs,
  	csVector3& shift, iPcCollisionDetection*& pc_cd);
  virtual bool InitCD (iPcCollisionDetection *pc_cd=0);
  virtual void SetSpeed (float speedz);

  virtual void GetLastPosition (csVector3& pos, float& yrot, iSector*& sector);
  virtual void GetLastFullPosition (csVector3& pos, float& yrot,
      iSector*& sector);
  virtual void SetPosition (const csVector3& pos, float yrot,
      const iSector* sector);
  virtual void SetFullPosition (const csVector3& pos, float yrot,
      const iSector* sector);
  virtual void SetPosition (const char* center_name, float yrot,
      iSector* sector);
  virtual void SetFullPosition (const char* center_name, float yrot,
      iSector* sector);

  virtual bool IsOnGround () const;

  virtual void SetOnGround (bool onground)
  {
    if (pccolldet)
      pccolldet->SetOnGround (onground);
  }

  /// Set if pcmesh should be transformed to follow the contour of the ground
  virtual void SetHugGround (bool hugGround)
  {
    this->hugGround = hugGround;
  }

  virtual void SetGravity (float grav)
  {
    gravity = grav;
  }

  virtual float GetGravity ()
  {
    return gravity;
  }

  virtual void ResetGravity ()
  {
    gravity = 19.6f;
  }

  virtual void AddGravityCallback (iPcGravityCallback* callback)
  {
    gravityCallbacks.Push (callback);
  }

  virtual void RemoveGravityCallback (iPcGravityCallback* callback)
  {
    gravityCallbacks.Delete (callback);
  }

  virtual bool IsPath () const { return (path != 0); }

  void LoadAnchor (iPcMesh* a);
  virtual void SetAnchor (iPcMesh* a);
  virtual iPcMesh* GetAnchor () const { return pcmesh; }

  /**
   * Returns the difference in time between now and when the last DR update
   * or extrapolation took place
   */
  virtual csTicks TimeDiff (void);


  /// Return all necessary data for Dead Reckoning
  virtual void GetDRData (bool& on_ground, float& speed, csVector3& pos,
      float& yrot, iSector*& sector, csVector3& vel, csVector3& worldVel,
      float& ang_vel);

  /// Sets all relevant dead reckoning data on this entity
  virtual void SetDRData(bool on_ground, float speed, csVector3& pos,
  	float yrot, iSector *sector, csVector3& vel, csVector3& worldVel,
  	float ang_vel);

  /**
   *  Sets dead reckoning data with 'soft' position updating.
   *  Rather than immediately set the new position, the difference
   *  between the current position and the new position is
   *  put into a position error variable. Over a 1-second interval
   *  the MoveV routine will offset the position so that the position
   *  error becomes zero.
   */
  virtual void SetSoftDRData(bool on_ground,float speed, csVector3& pos,
  	float yrot, iSector *sector, csVector3& vel, csVector3& worldVel,
  	float ang_vel);

  virtual const char* GetName () const { return "pclinearmovement"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId,
  	iCelParameterBlock* params,
	celData& ret);
  virtual bool SetProperty (csStringID propertyId, const char* b);
  virtual const char* GetPropertyString (csStringID propertyId);
  virtual void TickEveryFrame ();

  virtual iSector* GetSector ();

  /**
   * This function actually moves and rotates the mesh, relighting
   * if necessary.
   */
  virtual void ExtrapolatePosition (float delta);

  /**
   * This function calls ExtrapolatePosition with a certain time
   * but uses a fixed delta.  This allows all entities linmoves to be
   * synchronized to the same ticks, even if updates are all
   * happening at different times.
   */
  virtual void UpdateDRDelta (csTicks ticksdelta);
  /**
   * This function calls ExtrapolatePosition with a certain time delta
   * or calculates the delta relative to the last update.
   * This allows all entities linmoves to be
   * synchronized to the same ticks, even if updates are all
   * happening at different times.
   */
  virtual void UpdateDR (csTicks delta);
  void UpdateDR ();

  /**
   * This function lets linmove store a ref to the supplied
   * iPath.  If this path is present, it will be used for
   * movement instead of linear velocity vector.
   */
  virtual void SetPath (iPath *newpath)
  { path = newpath; path_sent = false; }

  /**
   * This function sets the current position on the path
   * for use when time deltas are added later.
   */
  virtual void SetPathTime (float timeval)
  { path_time = timeval; }

  /**
   * This relates the movement of the entity
   * along the path to the time values specified
   * by the path.  Speed=0 makes the entity
   * stationary.  Speed=1 is normal traversal
   * of the path.
   */
  virtual void SetPathSpeed (float speed)
  { path_speed = speed; }

  /**
   * This relates a particular action name
   * to be used between two points in the path.
   * This allows linmove to automatically
   * switch a bird from "fly" to "glide", for
   * example, during downward segments of the
   * flight path.
   */
  virtual void SetPathAction (int which, const char *action);

  /**
   * This sets the sector which will be used
   * for the entire path.
   */
  virtual void SetPathSector (const char *sectorname)
  { path_sector = sectorname; }

  virtual void SetDeltaLimit (float deltaLimit)
  { this->deltaLimit = deltaLimit; }

  /// Get the total displacement caused by space warping portals.
  virtual csVector3 GetPortalDisplacement ()
  { return portalDisplaced; }
  /// Clear the total displacement caused by space warping portals.
  virtual void ClearPortalDisplacement ()
  { portalDisplaced = 0.0f; }

  virtual csPtr<iCelDataBuffer> GetPersistentData (
  	celPersistenceType persistence_type);
  virtual celPersistenceResult SetPersistentData (csTicks data_time,
  	iCelDataBuffer* data, celPersistenceType persistence_type);

protected:
  static csCollisionPair our_cd_contact[1000];
  static int num_our_cd;
};

#endif
