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
#include "cstool/collider.h"
#include "ivaria/collider.h"
#include "cssys/sysfunc.h"

//CEL includes
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pl/facttmpl.h"
#include "propclass/mesh.h"
#include "propclass/linmove.h"
#include "plugins/propclass/common/stdpcimp.h"

#define CEL_LINMOVE_FOLLOW_ONLY_PORTALS true

struct iCelEntity;
struct iObjectRegistry;
struct iVirtualClock;
struct iCollider;
struct iCollideSystem;
struct iSector;
class csReversibleTransform;
class csObject;
class csColliderWrapper;

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
  iPcMesh* pcmesh;
  csRef<iCollideSystem> cdsys;
  csRef<iEngine> engine;
  csRef<iVirtualClock> vc;

  csVector3 angularVelocity;
  csVector3 vel;
  float speed;
  float camera_pitchspeed;
  bool onground;
  bool useCD;
  bool ready;
  bool stationary;

  //Collision vars
  csRef<csColliderWrapper> topCollider;
  csVector3 topSize;
  csRef<iMeshWrapper> topColliderMesh;
  float	fTopSize;

  csRef<csColliderWrapper> bottomCollider;
  csVector3 bottomSize;
  csRef<iMeshWrapper> bottomColliderMesh;
  float fBottomSize;

//  csVector3     lastKnownPosition;  
//  csVector3     lastKnownVelocity;
//  csMatrix3     lastKnownMatrix;
//  csVector3     lastKnownRotation;
  csTicks lastDRUpdate;
  csTicks accumulatedLag;

  csVector3 lastSentVelocity;
  csVector3 lastSentRotation;
  iSpriteAction* lastSentAction;
  
  bool RotateV (float delta);
  // Move local entity
  bool MoveV (float delta);
  bool MoveSprite (float delta);

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
  int CollisionDetect (csColliderWrapper *cw, iSector* sect,
  	csReversibleTransform *cdt, csReversibleTransform *cdstart);

public:
  celPcLinearMovement (iObjectRegistry* object_reg);
  virtual ~celPcLinearMovement ();

  bool HandleEvent (iEvent& ev);

  void SetRotation (const csVector3& angle);
  void SetVelocity (const csVector3& vel );
  void GetVelocity (csVector3& v);
  bool InitCD (csVector3& top, csVector3& bottom);
  bool InitCD ();
  void SetSpeed ( float speedz );
  void SetCameraPitchSpeed (float angle)
  {
    camera_pitchspeed = angle;
  }

  void GetLastPosition (csVector3& pos,float& yrot, iSector*& sector);
  void SetPosition (const csVector3& pos,float yrot, const iSector* sector);

  bool IsOnGround () const
  {
    return onground;
  }

  /// Returns dead reckoning data
  csPtr<iDataBuffer> GetDRData ();

  /**
   * Sets dead reckoning data and moves the entity accordingly
   * If data is 0 then UpdateDR () is called instead.
   */
  bool SetDRData (iDataBuffer* data,bool detectcheat);
  /**
   * Checks whether there is a need for sending a new DR packet
   * and if so, what the priority of the DR update should be.
   */
  bool NeedDRData (uint8& priority);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pclinearmovement"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  iSector* GetSector ();

  void SetReady (bool flag)
  {
    ready = flag;
  }
  
  bool IsReady () const
  {
    return ready;
  }

  /**
   * This function actually moves and rotates the mesh, relighting
   * if necessary.
   */
  void ExtrapolatePosition (float delta);

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
    virtual void SetCameraPitchSpeed (float angle)
    {
      scfParent->SetCameraPitchSpeed (angle);
    }

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

    virtual bool InitCD (csVector3& top, csVector3& bottom)
    {
      return scfParent->InitCD (top, bottom);
    }
    virtual bool InitCD ()
    {
      return scfParent->InitCD ();
    }

    virtual csPtr<iDataBuffer> GetDRData ()
    {
      return scfParent->GetDRData ();
    }
    virtual bool SetDRData (iDataBuffer* data,bool detectcheat)
    {
      return scfParent->SetDRData (data,detectcheat);
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
    virtual bool IsOnGround () const
    {
      return scfParent->IsOnGround ();
    }
    virtual bool IsReady () const
    {
      return scfParent->IsReady ();
    }
    virtual void ExtrapolatePosition (float delta)
    {
      scfParent->ExtrapolatePosition (delta);
    }
  } scfiPcLinearMovement;

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

    virtual ~EventHandler () {}

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;

protected:
  static csCollisionPair our_cd_contact[1000];
  static int num_our_cd;
};

#endif 

