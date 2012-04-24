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
#include "propclass/newcamera.h"
#include "propclass/mesh.h"
#include "propclass/sound.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;
struct iMovable;
struct iVirtualClock;
struct iGraphics2D;

/**
 * Factory for actormove and npcmove.
 */
CEL_DECLARE_FACTORY(ActorMove)

#define MOUSEMOVE_HOR_FACTOR 0.4f
#define MOUSEMOVE_VERT_FACTOR 2.4f

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
  csWeakRef<iPcNewCamera> pcnewcamera;

  csWeakRef<iSpriteCal3DState> sprcal3d;

  bool checked_spritestate;

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
  bool jumping;
  bool subscribed;

  // For mouse move.
  csRef<iVirtualClock> vc;
  bool mousemove;
  bool mousemove_inverted;
  bool mousemove_accelerated;
  csTicks mousemove_lastticks;
  float mousemove_totdelta;
  float mousemove_lastx;
  float mousemove_lasty;
  float mousemove_hor_factor;
  float mousemove_vert_factor;
  csWeakRef<iGraphics2D> g2d;

  static csStringID id_movement;
  static csStringID id_running;
  static csStringID id_rotation;
  static csStringID id_jumping;
  static csStringID id_start;
  static csStringID id_yrot;
  static csStringID id_x;
  static csStringID id_y;
  static csStringID id_animation;
  static csStringID id_anicycle;
  static csStringID id_animationid;
  static csStringID id_animationname;

  static csStringID id_input_forward1;
  static csStringID id_input_forward0;
  static csStringID id_input_backward1;
  static csStringID id_input_backward0;
  static csStringID id_input_rotateleft1;
  static csStringID id_input_rotateleft0;
  static csStringID id_input_rotateright1;
  static csStringID id_input_rotateright0;
  static csStringID id_input_strafeleft1;
  static csStringID id_input_strafeleft0;
  static csStringID id_input_straferight1;
  static csStringID id_input_straferight0;
  static csStringID id_input_jump1;
  static csStringID id_input_cammode1;

  csVector3 FindVelocity();
  void HandleMovement (bool jump);
  void FindSiblingPropertyClasses ();
  void GetSpriteStates ();

  // For actions.
  enum actionids
  {
    action_setspeed = 0,
    action_forward,
    action_backward,
    action_strafeleft,
    action_straferight,
    action_rotateleft,
    action_rotateright,
    action_rotateto,
    action_mousemove,
    action_run,
    action_autorun,
    action_clear,
    action_jump,
    action_togglecameramode,
    action_setanimation,
    action_setanimationname,
    action_subscribe
  };

  // For properties.
  enum propids
  {
    propid_mousemove = 0,
    propid_mousemove_inverted,
    propid_mousemove_accelerated,
    propid_mousemove_xfactor,
    propid_mousemove_yfactor
  };
  static PropertyHolder propinfo;

  // For animations.
  csString anim_name_walk;
  csString anim_name_idle;
  csString anim_name_run;
  csString anim_name_jump;

public:
  celPcActorMove (iObjectRegistry* object_reg);
  virtual ~celPcActorMove ();

  virtual void Forward (bool start);
  virtual bool IsMovingForward ();
  virtual void Backward (bool start);
  virtual bool IsMovingBackward ();
  virtual bool IsMoving ();
  virtual void StrafeLeft (bool start);
  virtual bool IsStrafingLeft ();
  virtual void StrafeRight (bool start);
  virtual bool IsStrafingRight ();
  virtual void RotateLeft (bool start);
  virtual bool IsRotatingLeft ();
  virtual void RotateRight (bool start);
  virtual bool IsRotatingRight ();
  virtual void RotateTo (float yrot);
  virtual void Run (bool start);
  virtual bool IsRunning ();
  virtual void AutoRun (bool start);
  virtual bool IsAutoRunning ();
  virtual void Jump ();
  virtual void ToggleCameraMode ();

  virtual void MouseMove (float x, float y);
  virtual void EnableMouseMove (bool en);
  virtual bool IsMouseMoveEnabled () const { return mousemove; }
  virtual void SetMouseMoveAccelerated (bool en) { mousemove_accelerated = en; }
  virtual bool IsMouseMoveAccelerated () const { return mousemove_accelerated; }
  virtual void SetMouseMoveInverted (bool en) { mousemove_inverted = en; }
  virtual bool IsMouseMoveInverted () const { return mousemove_inverted; }
  virtual void SetMouseMoveSpeed (float xs, float ys)
  {
    mousemove_hor_factor = xs;
    mousemove_vert_factor = ys;
  }
  virtual float GetMouseMoveSpeedX () const { return mousemove_hor_factor; }
  virtual float GetMouseMoveSpeedY () const { return mousemove_vert_factor; }

  virtual void SetMovementSpeed (float speed) { movement_speed = speed; }
  virtual float GetMovementSpeed () const { return movement_speed; }

  virtual void SetRotationSpeed (float speed) { rotating_speed = speed; }
  virtual float GetRotationSpeed () const { return rotating_speed; }

  virtual void SetRunningSpeed (float speed) { running_speed = speed; }
  virtual float GetRunningSpeed () const { return running_speed; }

  virtual void SetJumpingVelocity (float speed) { jumping_velocity = speed; }
  virtual float GetJumpingVelocity () const { return jumping_velocity; }

  virtual void SetAnimation (const char *name, bool cycle=true);

  virtual void SetAnimationMapping (celAnimationName idx, const char *name);

  virtual void SubscribeMessages ();

  csPtr<iCelDataBuffer> Save ();
  bool Load (iCelDataBuffer* databuf);
  virtual void TickEveryFrame ();

  virtual bool PerformActionIndexed (int, iCelParameterBlock* params,
  	celData& ret);
  // Override SetProperty from celPcCommon in order to provide support
  // for the 'mousemove' property.
  virtual bool SetPropertyIndexed (int, bool);
  virtual bool GetPropertyIndexed (int, bool&);

  virtual csPtr<iCelDataBuffer> GetPersistentData (
  	celPersistenceType persistence_type);
  virtual celPersistenceResult SetPersistentData (csTicks data_time,
  	iCelDataBuffer* data, celPersistenceType persistence_type);

  // For iMessageReceiver.
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
};

#endif // __CEL_PF_ACTORMOVEFACT__

