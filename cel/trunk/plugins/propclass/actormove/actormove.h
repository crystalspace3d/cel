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
#include "imesh/spritecal3d.h"
#include "imesh/sprite3d.h"

#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/actormove.h"
#include "propclass/npcmove.h"
#include "propclass/linmove.h"
#include "propclass/camera.h"
#include "propclass/defcam.h"
#include "propclass/mesh.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;

/**
 * Factory for actormove and npcmove.
 */
CEL_DECLARE_FACTORY(ActorMove)
CEL_DECLARE_FACTORY(NpcMove)

/**
 * This is a property class that helps with actor movement.
 * It knows about pclinmove and pccamera and can also control
 * animation in sprcal3d and spr3d mesh objects.
 */
class celPcActorMove : public scfImplementationExt1<
	celPcActorMove,celPcCommon,iPcActorMove>
{
private:
  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcMesh> pcmesh;
  csWeakRef<iPcCamera> pccamera;
  csWeakRef<iPcDefaultCamera> pcdefcamera;

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

  static csStringID action_setspeed;
  static csStringID action_forward;
  static csStringID action_backward;
  static csStringID action_strafeleft;
  static csStringID action_straferight;
  static csStringID action_rotateleft;
  static csStringID action_rotateright;
  static csStringID action_rotateto;
  static csStringID action_run;
  static csStringID action_autorun;
  static csStringID action_jump;
  static csStringID action_togglecameramode;
  static csStringID id_movement;
  static csStringID id_running;
  static csStringID id_rotation;
  static csStringID id_jumping;
  static csStringID id_start;
  static csStringID id_yrot;

  void HandleMovement (bool jump);
  void FindSiblingPropertyClasses ();
  void GetSpriteStates ();

public:
  celPcActorMove (iObjectRegistry* object_reg);
  virtual ~celPcActorMove ();

  virtual void Forward (bool start)
  {
    forward = start;
    HandleMovement (false);
  }
  virtual bool IsMovingForward ()
  {
    HandleMovement (false);
    return forward;
  }
  virtual void Backward (bool start)
  {
    backward = start;
    HandleMovement (false);
  }
  virtual bool IsMovingBackward ()
  {
    HandleMovement (false);
    return backward;
  }
  virtual void StrafeLeft (bool start)
  {
    strafeleft = start;
    HandleMovement (false);
  }
  virtual bool IsStrafingLeft ()
  {
    HandleMovement (false);
    return strafeleft;
  }
  virtual void StrafeRight (bool start)
  {
    straferight = start;
    HandleMovement (false);
  }
  virtual bool IsStrafingRight ()
  {
    HandleMovement (false);
    return straferight;
  }
  virtual void RotateLeft (bool start)
  {
    rotateleft = start;
    rotatetoreached = true;
    HandleMovement (false);
  }
  virtual bool IsRotatingLeft ()
  {
    HandleMovement (false);
    return rotateleft;
  }
  virtual void RotateRight (bool start)
  {
    rotateright = start;
    rotatetoreached = true;
    HandleMovement (false);
  }
  virtual bool IsRotatingRight ()
  {
    HandleMovement (false);
    return rotateright;
  }
  virtual void RotateTo (float yrot);
  virtual void Run (bool start)
  {
    if (!autorun) running = start;
    HandleMovement (false);
  }
  virtual bool IsRunning ()
  {
    HandleMovement (false);
    return running;
  }
  virtual void AutoRun (bool start)
  {
    autorun = start;
    HandleMovement (false);
  }
  virtual bool IsAutoRunning ()
  {
    HandleMovement (false);
    return autorun;
  }
  virtual void Jump ()
  {
    HandleMovement (true);
  }
  virtual void ToggleCameraMode ();

  virtual void SetMovementSpeed (float speed) { movement_speed = speed; }
  virtual float GetMovementSpeed () const { return movement_speed; }

  virtual void SetRotationSpeed (float speed) { rotating_speed = speed; }
  virtual float GetRotationSpeed () const { return rotating_speed; }

  virtual void SetRunningSpeed (float speed) { running_speed = speed; }
  virtual float GetRunningSpeed () const { return running_speed; }

  virtual void SetJumpingVelocity (float speed) { jumping_velocity = speed; }
  virtual float GetJumpingVelocity () const { return jumping_velocity; }

  virtual const char* GetName () const { return "pcactormove"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  virtual bool PerformAction (csStringID, iCelParameterBlock* params);
};

/**
 * This is a property class that helps with NPC movement.
 * It knows about pclinmove and can also control
 * animation in sprcal3d and spr3d mesh objects.
 */
class celPcNpcMove : public scfImplementationExt1<
	celPcNpcMove,celPcCommon,iPcNpcMove>
{
private:
  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcMesh> pcmesh;

  bool checked_spritestate;
  csWeakRef<iSpriteCal3DState> sprcal3d;
  csWeakRef<iSprite3DState> spr3d;

  void FindSiblingPropertyClasses ();
  void GetSpriteStates ();

public:
  celPcNpcMove (iObjectRegistry* object_reg);
  virtual ~celPcNpcMove ();

  virtual const char* GetName () const { return "pcnpcmove"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual void TickOnce ();
};

#endif // __CEL_PF_ACTORMOVEFACT__

