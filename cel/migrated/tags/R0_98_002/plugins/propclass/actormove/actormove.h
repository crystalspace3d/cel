/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_ACTORMOVEFACT__
#define __CEL_PF_ACTORMOVEFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/parray.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "csutil/hashhandlers.h"
#include "imesh/spritecal3d.h"
#include "imesh/sprite3d.h"

#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/actormove.h"
#include "propclass/linmove.h"
#include "propclass/camera.h"
#include "propclass/mesh.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;

/**
 * Factory for actormove.
 */
CEL_DECLARE_FACTORY(ActorMove)

/**
 * This is a property class that helps with actor movement.
 * It knows about pclinmove and pccamera and can also control
 * animation in sprcal3d and spr3d mesh objects.
 */
class celPcActorMove : public celPcCommon
{
private:
  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcMesh> pcmesh;
  csWeakRef<iPcCamera> pccamera;

  bool checked_spritestate;
  csWeakRef<iSpriteCal3DState> sprcal3d;
  csWeakRef<iSprite3DState> spr3d;

  float movement_speed;
  float running_speed;
  float rotating_speed;
  float jumping_velocity;
  float rotate_to;
  bool forward;
  bool backward;
  bool strafeleft;
  bool straferight;
  bool rotateleft;
  bool rotateright;
  bool rotatetoreached;
  bool running;
  bool autorun;

  void RotateTo (float yrot);
  void HandleMovement (bool jump);
  void FindSiblingPropertyClasses ();
  void GetSpriteStates ();

public:
  celPcActorMove (iObjectRegistry* object_reg);
  virtual ~celPcActorMove ();

  void Forward (bool start)
  {
    forward = start;
    HandleMovement (false);
  }
  void Backward (bool start)
  {
    backward = start;
    HandleMovement (false);
  }
  void StrafeLeft (bool start)
  {
    strafeleft = start;
    HandleMovement (false);
  }
  void StrafeRight (bool start)
  {
    straferight = start;
    HandleMovement (false);
  }
  void RotateLeft (bool start)
  {
    rotateleft = start;
    rotatetoreached = true;
    HandleMovement (false);
  }
  void RotateRight (bool start)
  {
    rotateright = start;
    rotatetoreached = true;
    HandleMovement (false);
  }
  void Run (bool start)
  {
    if (!autorun) running = start;
    HandleMovement (false);
  }
  void AutoRun (bool start)
  {
    autorun = start;
    HandleMovement (false);
  }
  void Jump ()
  {
    HandleMovement (true);
  }
  void ToggleCameraMode ();

  void SetMovementSpeed (float speed) { movement_speed = speed; }
  float GetMovementSpeed () const { return movement_speed; }

  void SetRotationSpeed (float speed) { rotating_speed = speed; }
  float GetRotationSpeed () const { return rotating_speed; }

  void SetRunningSpeed (float speed) { running_speed = speed; }
  float GetRunningSpeed () const { return running_speed; }

  void SetJumpingVelocity (float speed) { jumping_velocity = speed; }
  float GetJumpingVelocity () const { return jumping_velocity; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcactormove"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcActorMove : public iPcActorMove
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcActorMove);
    virtual void Forward (bool start)
    {
      scfParent->Forward (start);
    }
    virtual void Backward (bool start)
    {
      scfParent->Backward (start);
    }
    virtual void StrafeLeft (bool start)
    {
      scfParent->StrafeLeft (start);
    }
    virtual void StrafeRight (bool start)
    {
      scfParent->StrafeRight (start);
    }
    virtual void RotateLeft (bool start)
    {
      scfParent->RotateLeft (start);
    }
    virtual void RotateRight (bool start)
    {
      scfParent->RotateRight (start);
    }
    virtual void RotateTo (float yrot)
    {
      scfParent->RotateTo (yrot);
    }
    virtual void Run (bool start)
    {
      scfParent->Run (start);
    }
    virtual void AutoRun (bool start)
    {
      scfParent->AutoRun (start);
    }
    virtual void Jump ()
    {
      scfParent->Jump ();
    }
    virtual void ToggleCameraMode ()
    {
      scfParent->ToggleCameraMode ();
    }

    virtual void SetMovementSpeed (float speed)
    {
      scfParent->SetMovementSpeed (speed);
    }
    virtual float GetMovementSpeed () const
    {
      return scfParent->GetMovementSpeed ();
    }

    virtual void SetRotationSpeed (float speed)
    {
      scfParent->SetRotationSpeed (speed);
    }
    virtual float GetRotationSpeed () const
    {
      return scfParent->GetRotationSpeed ();
    }

    virtual void SetRunningSpeed (float speed)
    {
      scfParent->SetRunningSpeed (speed);
    }
    virtual float GetRunningSpeed () const
    {
      return scfParent->GetRunningSpeed ();
    }

    virtual void SetJumpingVelocity (float speed)
    {
      scfParent->SetJumpingVelocity (speed);
    }
    virtual float GetJumpingVelocity () const
    {
      return scfParent->GetJumpingVelocity ();
    }
  } scfiPcActorMove;
  friend struct PcActorMove;
};

#endif // __CEL_PF_ACTORMOVEFACT__

