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

CEL_DECLARE_FACTORY (CollisionDetection)

/**
 * Collision Detection (CD) related class.
 * This class handles CD for linearmovement PC.
 */
class celPcCollisionDetection : public celPcCommon
{
protected:
  iPcMesh* pcmesh;

  csColliderActor collider_actor;

  csRef<iCollideSystem> cdsys;
  csWeakRef<iEngine> engine;

  /// Enable collision detection flag.
  bool useCD;

  //Collision vars
  csVector3 shift;
  csVector3 topSize;
  csVector3 bottomSize;

  // Try to find the collider for an object. Can return 0 if there is none.
  // This function uses csColliderWrapper if present. If not it will try
  // if there is a pcsolid to get a collider from.
  iCollider* FindCollider (iObject* object);

public:
  celPcCollisionDetection(iObjectRegistry* object_reg);
  virtual ~celPcCollisionDetection();

  bool Init (const csVector3& body, const csVector3& legs,
  	         const csVector3& shift);
  bool Init ();

  bool IsOnGround () const
  {
    return collider_actor.IsOnGround ();
  }
  void SetOnGround (bool flag)
  {
    collider_actor.SetOnGround (flag);
  }

  void UseCD (bool flag)
  { useCD = flag; }

  bool AdjustForCollisions (csVector3& oldpos,
                            csVector3& newpos,
                            csVector3& vel,
                            float delta,
                            iMovable* movable);

  bool QueryRevert()
  { return collider_actor.CheckRevertMove ();}

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pccollisiondetection"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcCollisionDetection : public iPcCollisionDetection
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCollisionDetection);

    virtual bool Init (const csVector3& top, const csVector3& bottom,
    	const csVector3& shift)
    {
      return scfParent->Init (top, bottom, shift);
    }
    virtual bool Init ()
    {
      return scfParent->Init ();
    }

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

  } scfiPcCollisionDetection;
};

#endif 

