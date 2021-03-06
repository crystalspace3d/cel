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

#include "cssysdef.h"

#include <math.h>

#include "plugins/propclass/actormove/actormove.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivaria/reporter.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "imesh/object.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (ActorMove, "pcmove.actor.standard", "pcactormove")

//---------------------------------------------------------------------------

csStringID celPcActorMove::id_movement = csInvalidStringID;
csStringID celPcActorMove::id_running = csInvalidStringID;
csStringID celPcActorMove::id_rotation = csInvalidStringID;
csStringID celPcActorMove::id_jumping = csInvalidStringID;
csStringID celPcActorMove::id_start = csInvalidStringID;
csStringID celPcActorMove::id_yrot = csInvalidStringID;
csStringID celPcActorMove::id_x = csInvalidStringID;
csStringID celPcActorMove::id_y = csInvalidStringID;
csStringID celPcActorMove::id_animation = csInvalidStringID;
csStringID celPcActorMove::id_anicycle = csInvalidStringID;
csStringID celPcActorMove::id_animationid = csInvalidStringID;
csStringID celPcActorMove::id_animationname = csInvalidStringID;
csStringID celPcActorMove::id_newtarget = csInvalidStringID;

csStringID celPcActorMove::id_input_forward1 = csInvalidStringID;
csStringID celPcActorMove::id_input_forward0 = csInvalidStringID;
csStringID celPcActorMove::id_input_backward1 = csInvalidStringID;
csStringID celPcActorMove::id_input_backward0 = csInvalidStringID;
csStringID celPcActorMove::id_input_rotateleft1 = csInvalidStringID;
csStringID celPcActorMove::id_input_rotateleft0 = csInvalidStringID;
csStringID celPcActorMove::id_input_rotateright1 = csInvalidStringID;
csStringID celPcActorMove::id_input_rotateright0 = csInvalidStringID;
csStringID celPcActorMove::id_input_strafeleft1 = csInvalidStringID;
csStringID celPcActorMove::id_input_strafeleft0 = csInvalidStringID;
csStringID celPcActorMove::id_input_straferight1 = csInvalidStringID;
csStringID celPcActorMove::id_input_straferight0 = csInvalidStringID;
csStringID celPcActorMove::id_input_jump1 = csInvalidStringID;
csStringID celPcActorMove::id_input_cammode1 = csInvalidStringID;

PropertyHolder celPcActorMove::propinfo;

celPcActorMove::celPcActorMove (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_movement == csInvalidStringID)
  {
    id_movement = pl->FetchStringID ("movement");
    id_running = pl->FetchStringID ("running");
    id_rotation = pl->FetchStringID ("rotation");
    id_jumping = pl->FetchStringID ("jumping");
    id_start = pl->FetchStringID ("start");
    id_yrot = pl->FetchStringID ("yrot");
    id_x = pl->FetchStringID ("x");
    id_y = pl->FetchStringID ("y");
    id_animation = pl->FetchStringID ("animation");
    id_anicycle = pl->FetchStringID ("cycle");
    id_animationid = pl->FetchStringID ("mapping");
    id_animationname = pl->FetchStringID ("name");
    id_newtarget = pl->FetchStringID ("newtarget");

    id_input_forward1 = pl->FetchStringID ("cel.input.forward.down");
    id_input_forward0 = pl->FetchStringID ("cel.input.forward.up");
    id_input_backward1 = pl->FetchStringID ("cel.input.backward.down");
    id_input_backward0 = pl->FetchStringID ("cel.input.backward.up");
    id_input_rotateleft1 = pl->FetchStringID ("cel.input.rotateleft.down");
    id_input_rotateleft0 = pl->FetchStringID ("cel.input.rotateleft.up");
    id_input_rotateright1 = pl->FetchStringID ("cel.input.rotateright.down");
    id_input_rotateright0 = pl->FetchStringID ("cel.input.rotateright.up");
    id_input_strafeleft1 = pl->FetchStringID ("cel.input.strafeleft.down");
    id_input_strafeleft0 = pl->FetchStringID ("cel.input.strafeleft.up");
    id_input_straferight1 = pl->FetchStringID ("cel.input.straferight.down");
    id_input_straferight0 = pl->FetchStringID ("cel.input.straferight.up");
    id_input_jump1 = pl->FetchStringID ("cel.input.jump.down");
    id_input_cammode1 = pl->FetchStringID ("cel.input.cammode.down");
  }

  movement_speed = 2.0f;
  running_speed = 5.0f;
  rotating_speed = 1.75f;
  jumping_velocity = 6.31f;
  forward = false;
  backward = false;
  strafeleft = false;
  straferight = false;
  rotateleft = false;
  rotateright = false;
  rotatetoreached = true;
  running = false;
  autorun = false;
  jumping = false;
  checked_spritestate = false;

  // For mouse movement.
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  mousemove = false;
  mousemove_lastticks = (csTicks)~0;
  mousemove_totdelta = 0.0f;
  mousemove_lastx = 0.0f;
  mousemove_lasty = 0.0f;
  mousemove_hor_factor = 1.0f;
  mousemove_vert_factor = 1.0f;
  mousemove_inverted = false;
  mousemove_accelerated = false;
  csRef<iGraphics3D> g3d = csQueryRegistry<iGraphics3D> (object_reg);
  if (!g3d)
  {
    Error ("No iGraphics3D plugin!");
    return;
  }
  g2d = g3d->GetDriver2D ();

  // For camera control
  new_xrot = 0.0f;
  new_yrot = 0.f;
  rotatehor = false;
  rotateup = false;
  rotatedown = false;
  freezemove = false;
  camrotate_lastticks = (csTicks)~0;

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.move.actor.action.");
    AddAction (action_setspeed, "SetSpeed");
    AddAction (action_forward, "Forward");
    AddAction (action_backward, "Backward");
    AddAction (action_strafeleft, "StrafeLeft");
    AddAction (action_straferight, "StrafeRight");
    AddAction (action_rotateleft, "RotateLeft");
    AddAction (action_rotateright, "RotateRight");
    AddAction (action_rotateto, "RotateTo");
    AddAction (action_mousemove, "MouseMove");
    AddAction (action_run, "Run");
    AddAction (action_autorun, "AutoRun");
    AddAction (action_clear, "Clear");
    AddAction (action_jump, "Jump");
    AddAction (action_togglecameramode, "ToggleCameraMode");
    AddAction (action_setanimation, "SetAnimation");
    AddAction (action_setanimationname, "SetAnimationName");
    AddAction (action_subscribe, "Subscribe");
    AddAction (action_changetarget, "ChangeTarget");
  }

  // For properties.
  propinfo.SetCount (5);
  AddProperty (propid_mousemove, "mousemove",
  	CEL_DATA_BOOL, false, "Mouse movement.", 0);
  AddProperty (propid_mousemove_inverted, "mousemove_inverted",
  	CEL_DATA_BOOL, false, "Mouse movement inverted.", &mousemove_inverted);
  AddProperty (propid_mousemove_accelerated,
        "mousemove_accelerated", CEL_DATA_BOOL,
	false, "Mouse movement accelerated.", &mousemove_accelerated);
  AddProperty (propid_mousemove_xfactor, "mousemove_xfactor",
  	CEL_DATA_FLOAT, false, "Mouse movement x speed factor.",
  	&mousemove_hor_factor);
  AddProperty (propid_mousemove_yfactor, "mousemove_yfactor",
  	CEL_DATA_FLOAT, false, "Mouse movement y speed factor.",
  	&mousemove_vert_factor);

  // For animations.
  SetAnimationMapping (CEL_ANIM_IDLE, "stand");
  SetAnimationMapping (CEL_ANIM_WALK, "walk");
  SetAnimationMapping (CEL_ANIM_RUN, "run");
  SetAnimationMapping (CEL_ANIM_JUMP, "jump");

  subscribed = false;
}

celPcActorMove::~celPcActorMove ()
{
  if (mousemove && g2d)
    g2d->SetMouseCursor (csmcArrow);
}

bool celPcActorMove::SetPropertyIndexed (int idx, bool b)
{
  if (idx == propid_mousemove)
  {
    EnableMouseMove (b);
    return true;
  }
  return false;
}

bool celPcActorMove::GetPropertyIndexed (int idx, bool& b)
{
  if (idx == propid_mousemove)
  {
    b = mousemove;
    return true;
  }
  return false;
}

void celPcActorMove::MouseMove (float x, float y)
{
  FindSiblingPropertyClasses ();
  mousemove_lastx = x;
  mousemove_lasty = y;
}

void celPcActorMove::EnableMouseMove (bool en)
{
  if (mousemove == en) return;
  mousemove = en;
  if (!mousemove)
  {
    if (!jumping)
      pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    g2d->SetMouseCursor (csmcArrow);
  }
  else
  {
    if (!jumping)
      pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    g2d->SetMouseCursor (csmcNone);
    mousemove_lastticks = (csTicks)~0;
    mousemove_totdelta = 0.0f;
    mousemove_lastx = 0.0f;
    mousemove_lasty = 0.0f;
  }
}

void celPcActorMove::TickEveryFrame ()
{
  FindSiblingPropertyClasses ();

  // handle animation changes
  if (pcmesh)
  {
    if (jumping && pclinmove->IsOnGround())
    {
      jumping = false;
      if (sprcal3d) sprcal3d->SetVelocity (-FindVelocity().z);
      else
      {
        if (forward || backward)
        {
          if (running)
	    SetAnimation (anim_name_run.GetData(),true);
          else
            SetAnimation (anim_name_walk.GetData(),true);
        }
        else
	  SetAnimation (anim_name_idle.GetData(),true);
      }

      if (!mousemove)
        pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    }
  }


  csTicks cur_ticks = vc->GetCurrentTicks ();
  if (camrotate_lastticks == (csTicks)~0)
  {
    camrotate_lastticks = cur_ticks;
  }
  if(rotateup || rotatedown || rotatehor){
    if(rotateup){
      RotateUp( float(cur_ticks - camrotate_lastticks) );
    }
    if(rotatedown){
      RotateDown( float(cur_ticks - camrotate_lastticks) );
    }
    if(rotatehor){
      RotateTo(new_yrot);
      //printf("new_yrot: %f\n", new_yrot);
    }
    camrotate_lastticks = cur_ticks;
    return;
  }else{
    freezemove = false;
  }
  camrotate_lastticks = cur_ticks;


  if (!mousemove) return;
  if (!pcdefcamera) return;

  int frame_width = g2d->GetWidth();
  int frame_height = g2d->GetHeight();

  cur_ticks = vc->GetCurrentTicks ();
  if (mousemove_lastticks == (csTicks)~0)
  {
    // Do nothing for first time.
    mousemove_lastticks = cur_ticks;
    g2d->SetMousePosition (frame_width / 2, frame_height / 2);
    return;
  }
  float delta = float (cur_ticks - mousemove_lastticks);
  mousemove_lastticks = cur_ticks;
  mousemove_totdelta += delta;

#define FRAME_DELAY 20.0f
  if (mousemove_totdelta >= FRAME_DELAY)
  {
    if (mousemove_inverted) mousemove_lasty = -mousemove_lasty;
    float abs_x = fabs (mousemove_lastx);
    float abs_y = fabs (mousemove_lasty);
    // Only rotate if mouse moved noticably.
    if (abs_x > 0.0001 || abs_y > 0.0001)
    {
      if (mousemove_accelerated)
      {
        if (abs_x > .25) abs_x += .25f;
        if (abs_x > .20) abs_x += .20f;
        if (abs_x > .15) abs_x += .15f;
        if (abs_x > .10) abs_x += .10f;
        if (abs_x > .05) abs_x += .05f;

        if (abs_y > .25) abs_y += .25f;
        if (abs_y > .20) abs_y += .20f;
        if (abs_y > .15) abs_y += .15f;
        if (abs_y > .10) abs_y += .10f;
        if (abs_y > .05) abs_y += .05f;
      }

      // Limit the maximum amount of mouse movement.
      if (abs_x > 0.5) abs_x = 0.5f;
      if (abs_y > 0.5) abs_y = 0.5f;

      pcdefcamera->MovePitch ((-mousemove_lasty)
      	* mousemove_vert_factor * MOUSEMOVE_VERT_FACTOR);
      float s = GetRotationSpeed();
      SetRotationSpeed (abs_x * mousemove_hor_factor
      	* MOUSEMOVE_HOR_FACTOR * 100.0f);
      if (abs_x < 0.0001f)
      {
        RotateRight(false);
        RotateLeft(false);
      }
      else if (mousemove_lastx > 0)
      {
        RotateRight(true);
        RotateLeft(false);
      }
      else
      {
        RotateLeft(true);
        RotateRight(false);
      }
      SetRotationSpeed (s); // Reset setting
    }
    else
    {
      RotateRight(false);
      RotateLeft(false);
    }

    mousemove_totdelta -= FRAME_DELAY;
    if (mousemove_totdelta >= FRAME_DELAY) mousemove_totdelta = 0.0f;
    g2d->SetMousePosition (frame_width / 2, frame_height / 2);
    mousemove_lastx = mousemove_lasty = 0.0f;
  }
}

void celPcActorMove::SetAnimation (const char *name, bool cycle)
{
  if (pcmesh)
    pcmesh->SetAnimation (name, cycle);
}

void celPcActorMove::SetAnimationMapping (celAnimationName idx, const char *name)
{
  if (idx == CEL_ANIM_IDLE)
    anim_name_idle = name;
  else if (idx == CEL_ANIM_WALK)
    anim_name_walk = name;
  else if (idx == CEL_ANIM_RUN)
    anim_name_run = name;
  else if (idx == CEL_ANIM_JUMP)
    anim_name_jump = name;
}

#define ACTORMOVE_SERIAL 1

csPtr<iCelDataBuffer> celPcActorMove::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (ACTORMOVE_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}


bool celPcActorMove::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != ACTORMOVE_SERIAL) return false;
  return true;
}

bool celPcActorMove::PerformActionIndexed (int idx,
	iCelParameterBlock* params, celData& ret)
{
  switch (idx)
  {
    case action_subscribe:
      {
        SubscribeMessages ();
        return true;
      }
    case action_setspeed:
      {
	float movement, running, rotation, jumping;
	if (ParExists (CEL_DATA_FLOAT, params, id_movement))
	{
	  if (!Fetch (movement, params, id_movement)) return false;
	  SetMovementSpeed (movement);
	}
	if (ParExists (CEL_DATA_FLOAT, params, id_running))
	{
	  if (!Fetch (running, params, id_running)) return false;
	  SetRunningSpeed (running);
	}
	if (ParExists (CEL_DATA_FLOAT, params, id_rotation))
	{
	  if (!Fetch (rotation, params, id_rotation)) return false;
	  SetRotationSpeed (rotation);
	}
	if (ParExists (CEL_DATA_FLOAT, params, id_jumping))
	{
	  if (!Fetch (jumping, params, id_jumping)) return false;
	  SetJumpingVelocity (jumping);
	}
        return true;
      }
    case action_forward:
      {
        if(freezemove) return false;
	bool start;
	if (!Fetch (start, params, id_start)) return false;
        Forward (start);
        return true;
      }
    case action_backward:
      {
        if(freezemove) return false;
	bool start;
	if (!Fetch (start, params, id_start)) return false;
        Backward (start);
        return true;
      }
    case action_strafeleft:
      {
        if(freezemove) return false;
	bool start;
	if (!Fetch (start, params, id_start)) return false;
        StrafeLeft (start);
        return true;
      }
    case action_straferight:
      {
        if(freezemove) return false;
	bool start;
	if (!Fetch (start, params, id_start)) return false;
        StrafeRight (start);
        return true;
      }
    case action_rotateleft:
      {
	bool start;
	if (!Fetch (start, params, id_start)) return false;
        RotateLeft (start);
        return true;
      }
    case action_rotateright:
      {
	bool start;
	if (!Fetch (start, params, id_start)) return false;
        RotateRight (start);
        return true;
      }
    case action_rotateto:
      {
	float yrot;
	if (!Fetch (yrot, params, id_yrot)) return false;
        RotateTo (yrot);
        return true;
      }
    case action_mousemove:
      {
	float x, y;
	if (!Fetch (x, params, id_x)) return false;
	if (!Fetch (y, params, id_y)) return false;
        MouseMove (x, y);
        return true;
      }
    case action_run:
      {
	bool start;
	if (!Fetch (start, params, id_start)) return false;
        Run (start);
        return true;
      }
    case action_autorun:
      {
	bool start;
	if (!Fetch (start, params, id_start)) return false;
        AutoRun (start);
        return true;
      }
    case action_clear:
      {
	Forward (false);
	Backward (false);
	StrafeLeft (false);
	StrafeRight (false);
	RotateLeft (false);
	RotateRight (false);
        Run (false);
        AutoRun (false);
  	rotatetoreached = true;
        return true;
      }
    case action_jump:
      Jump ();
      return true;
    case action_togglecameramode:
      ToggleCameraMode ();
      return true;
    case action_setanimation:
    {
      csString animation;
      if (!Fetch (animation, params, id_animation)) return false;
      bool anicycle;
      if (!Fetch (anicycle, params, id_anicycle)) return false;
      SetAnimation (animation, anicycle);
      return true;
    }
    case action_setanimationname:
    {
      csString animationid, animationname;
      if (!Fetch (animationid, params, id_animationid)) return false;
      if (!Fetch (animationname, params, id_animationname)) return false;

      celAnimationName animid = static_cast<celAnimationName>(~0);
      if (strcmp(animationid,"idle") == 0)
        animid = CEL_ANIM_IDLE;
      else if (strcmp(animationid,"walk") == 0)
        animid = CEL_ANIM_WALK;
      else if (strcmp(animationid,"run") == 0)
        animid = CEL_ANIM_RUN;
      else if (strcmp(animationid,"jump") == 0)
        animid = CEL_ANIM_JUMP;

      if (animid != static_cast<celAnimationName>(~0))
      {
	SetAnimationMapping (animid, animationname);
	return true;
      }
      return false;
    }
    case action_changetarget:
    {
      rotatehor = true;
      freezemove = true;
      csVector3 actorPos = pclinmove->GetPosition(); // use eye position here will be better

      csString newTarget;
      csVector3 newTargetPos;

      if (!Fetch (newTarget, params, id_newtarget)) return false;

      pl = csQueryRegistry<iCelPlLayer> (object_reg);
      iCelEntity *entity = pl->FindEntity (newTarget);
    
      csRef<iPcMesh> targetPcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
      if (!targetPcmesh)
      {
	Error ("ChangeTarget: target has no mesh!");
	return false;
      }
      newTargetPos = targetPcmesh->GetMesh()->GetMovable()->GetFullPosition();

      csVector3 newLookAt = newTargetPos - actorPos;
      newLookAt.Normalize();

      csReversibleTransform actorTransform = pcmesh->GetMesh ()->GetMovable ()->GetTransform ();

      new_xrot = 3.1415f/2.1f - acos (csClamp (actorTransform.GetT2O() * csVector3(0, 1, 0) * newLookAt, 1.0f, -1.0f));
      float current_xrot = pcdefcamera->GetPitch();
      if (new_xrot > current_xrot)
      {
        rotateup = true;
        rotatedown = false;
      } 
      else
      {
        rotatedown = true;
        rotateup = false;
      }

      csVector3 newLookAt_XZ = newLookAt;
      newLookAt_XZ.y = 0;

      newLookAt_XZ.Normalize();
      if(newLookAt_XZ.z > 1.0f) newLookAt_XZ.z = 1.0f;
      if(newLookAt_XZ.z < -1.0f) newLookAt_XZ.z = -1.0f;

      new_yrot = acos(newLookAt_XZ.z);
      if(newLookAt_XZ.x < 0.0f)
        new_yrot = 2.0*PI - new_yrot;
    }
    default:
      return false;
  }
}

void celPcActorMove::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    pclinmove = celQueryPropertyClassEntity<iPcLinearMovement> (entity);
    pccamera = celQueryPropertyClassEntity<iPcCamera> (entity);
    pcdefcamera = celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
    pcnewcamera = celQueryPropertyClassEntity<iPcNewCamera> (entity);
    checked_spritestate = false;
  }
}

void celPcActorMove::GetSpriteStates ()
{
  if (checked_spritestate) return;
  if (!pcmesh) return;
  iMeshWrapper* m = pcmesh->GetMesh ();
  if (!m) return;
  iMeshObject* o = m->GetMeshObject ();
  if (!o) return;
  checked_spritestate = true;
  sprcal3d = scfQueryInterface<iSpriteCal3DState> (o);
}

void celPcActorMove::RotateTo (float yrot)
{
  FindSiblingPropertyClasses ();
  rotatetoreached = false;

  if (!pclinmove)
  {
    Error ("pcmove.linear is missing!");
    return;
  }

  float current_yrot = pclinmove->GetYRotation ();
  //printf("current_yrot: %f\n", current_yrot);
  current_yrot = atan2f (sin (current_yrot), cos (current_yrot));
  rotate_to = atan2f (sin (yrot), cos (yrot));
  float delta_angle = atan2f (sin (rotate_to - current_yrot),
  	cos (rotate_to - current_yrot));

  if (fabs(delta_angle) < SMALL_EPSILON)
  //if (fabs(delta_angle) < rotating_speed * 10.f)
  {
    rotateright = false;
    rotateleft = false;
    rotatehor = false;
    //printf("rotation ends! \n");
    HandleMovement (false);
    return;
  }


  if (current_yrot < 0)
  {
    rotateright = (rotate_to > current_yrot)
    	&& (rotate_to < (current_yrot + PI));
    rotateleft = !rotateright;
  }
  else
  {
    rotateleft = (rotate_to > (current_yrot - PI))
    	&& (rotate_to < current_yrot);
    rotateright = !rotateleft;
  }

  HandleMovement (false);
}

csVector3 celPcActorMove::FindVelocity()
{

  float speed;
  if (running)
    speed = running_speed;
  else
    speed = movement_speed;

  static csVector3 velocity (0.0f, 0.0f, 0.0f);

  if ((autorun || forward) && straferight)
  {
    velocity.x = -0.75f * speed;
    velocity.z = -0.75f * speed;
  }
  else if ((autorun || forward) && strafeleft)
  {
    velocity.x = 0.75f * speed;
    velocity.z = -0.75f * speed;
  }
  else if (autorun || forward)
  {
    velocity.x = 0.0f;
    velocity.z = -speed;
  }
  else if (backward && straferight)
  {
    velocity.x = -0.75f * speed;
    velocity.z = 0.75f * speed;
  }
  else if (backward && strafeleft)
  {
    velocity.x = 0.75f * speed;
    velocity.z = 0.75f * speed;
  }
  else if (backward)
  {
    velocity.x = 0.0f;
    velocity.z = speed;
  }
  else if (straferight)
  {
    velocity.x = -speed;
    velocity.z = 0.0f;
  }
  else if (strafeleft)
  {
    velocity.x = speed;
    velocity.z = 0.0f;
  }
  else
  {
    velocity.x = 0.0f;
    velocity.z = 0.0f;
  }
  return velocity;
}

void celPcActorMove::HandleMovement (bool jump)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
  {
    Error ("pcmove.linear is missing!");
    return;
  }
  GetSpriteStates ();
  if (!pcmesh)
  {
    Error ("pcobject.mesh is missing!");
    return;
  }
  csVector3 velocity = FindVelocity();
  pclinmove->SetBodyVelocity (velocity);
  if (sprcal3d) sprcal3d->SetVelocity (-velocity.z);

  // Since spr3d and genmesh don't handle animation
  // automatically by velocity, set it here.
  if (pcmesh && !sprcal3d && pclinmove->IsOnGround())
  {
    if (forward || backward)
    {
      if (running)
        SetAnimation (anim_name_run.GetData(),true);
      else
        SetAnimation (anim_name_walk.GetData(),true);
    }
    else
      SetAnimation (anim_name_idle.GetData(),true);
  }

  float actual_rotating_speed = 0.0f;
  if (rotateright)
    actual_rotating_speed = -rotating_speed;
  else if (rotateleft)
    actual_rotating_speed = rotating_speed;
  if (rotatetoreached)
    pclinmove->SetAngularVelocity (csVector3 (0.0f, actual_rotating_speed, 0.0f));
  else
    pclinmove->SetAngularVelocity (csVector3 (0.0f, actual_rotating_speed, 0.0f),
    	csVector3 (0.0f, rotate_to, 0.0f));

  if (jump && pclinmove->IsOnGround ())
  {
    csVector3 velocity (0.0f, jumping_velocity, 0.0f);
    pclinmove->AddVelocity (velocity);
    if (sprcal3d) sprcal3d->SetVelocity (-velocity.z);
    // spr3d and genmesh
    else
      SetAnimation (anim_name_jump.GetData(), false);
  }
}

void celPcActorMove::ToggleCameraMode ()
{
  FindSiblingPropertyClasses ();
  if (!pcdefcamera && !pcnewcamera)
  {
    Error ("Must have pccamera.standard or pccamera.old!");
    return;
  }
  if (pcdefcamera)
      pcdefcamera->SetMode (pcdefcamera->GetNextMode ());
  if (pcnewcamera)
      pcnewcamera->NextCameraMode ();
}

void celPcActorMove::Forward (bool start)
{
  forward = start;
  HandleMovement (false);
}

bool celPcActorMove::IsMovingForward ()
{
  HandleMovement (false);
  return forward;
}

void celPcActorMove::Backward (bool start)
{
  backward = start;
  HandleMovement (false);
}

bool celPcActorMove::IsMovingBackward ()
{
  HandleMovement (false);
  return backward;
}

bool celPcActorMove::IsMoving ()
{
  HandleMovement (false);
  return (forward || backward);
}

void celPcActorMove::StrafeLeft (bool start)
{
  strafeleft = start;
  HandleMovement (false);
}

bool celPcActorMove::IsStrafingLeft ()
{
  HandleMovement (false);
  return strafeleft;
}

void celPcActorMove::StrafeRight (bool start)
{
  straferight = start;
  HandleMovement (false);
}

bool celPcActorMove::IsStrafingRight ()
{
  HandleMovement (false);
  return straferight;
}

void celPcActorMove::RotateLeft (bool start)
{
  rotateleft = start;
  rotatetoreached = true;
  HandleMovement (false);
}

bool celPcActorMove::IsRotatingLeft ()
{
  HandleMovement (false);
  return rotateleft;
}

void celPcActorMove::RotateRight (bool start)
{
  rotateright = start;
  rotatetoreached = true;
  HandleMovement (false);
}

bool celPcActorMove::IsRotatingRight ()
{
  HandleMovement (false);
  return rotateright;
}

void celPcActorMove::RotateUp (float delta_tick)
{
  if(pcdefcamera->GetPitch() < new_xrot){
    pcdefcamera->MovePitch(delta_tick * 0.00004f);
  }else{
    rotateup = false;
  }
}

void celPcActorMove::RotateDown (float delta_tick)
{
  if(pcdefcamera->GetPitch() > new_xrot){
    pcdefcamera->MovePitch(-delta_tick * 0.00004f);
  }else{
    rotatedown = false;
  }
}

void celPcActorMove::Run (bool start)
{
  if (!autorun) running = start;
  HandleMovement (false);
}

bool celPcActorMove::IsRunning ()
{
  HandleMovement (false);
  return running;
}

void celPcActorMove::AutoRun (bool start)
{
  autorun = start;
  HandleMovement (false);
}

bool celPcActorMove::IsAutoRunning ()
{
  HandleMovement (false);
  return autorun;
}

void celPcActorMove::Jump ()
{
  if (!jumping && !mousemove)
      pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  jumping = true;
  HandleMovement (true);
}

void celPcActorMove::SubscribeMessages ()
{
  if (!entity)
  {
    Error ("Error in actormove: no entity set!\n");
    return;
  }
  entity->QueryMessageChannel ()->Subscribe (this, "cel.input.");
  subscribed = true;
}

bool celPcActorMove::ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  if (celPcCommon::ReceiveMessage (msg_id, sender, ret, params))
    return true;

  if (msg_id == id_input_forward1)
    Forward (true);
  else if (msg_id == id_input_forward0)
    Forward (false);
  else if (msg_id == id_input_backward1)
    Backward (true);
  else if (msg_id == id_input_backward0)
    Backward (false);
  else if (msg_id == id_input_rotateleft1)
    RotateLeft (true);
  else if (msg_id == id_input_rotateleft0)
    RotateLeft (false);
  else if (msg_id == id_input_rotateright1)
    RotateRight (true);
  else if (msg_id == id_input_rotateright0)
    RotateRight (false);
  else if (msg_id == id_input_strafeleft1)
    StrafeLeft (true);
  else if (msg_id == id_input_strafeleft0)
    StrafeLeft (false);
  else if (msg_id == id_input_straferight1)
    StrafeRight (true);
  else if (msg_id == id_input_straferight0)
    StrafeRight (false);
  else if (msg_id == id_input_cammode1)
    ToggleCameraMode ();
  else if (msg_id == id_input_jump1)
    Jump ();
  else
    return false;
  return true;
}

csPtr<iCelDataBuffer> celPcActorMove::GetPersistentData (
	celPersistenceType persistence_type)
{
  if (persistence_type == CEL_PERSIST_TYPE_RECORD_FIRST_PASS)
    return 0;

  if (persistence_type == CEL_PERSIST_TYPE_RECORD)
    return Save ();

  // @@@ TODO: this is a hack
  FindSiblingPropertyClasses ();
  GetSpriteStates ();

  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (ACTORMOVE_SERIAL);

  if (sprcal3d)
  {
    // @@@ TODO: this doesn't work for idle animations, it seems they are not
    // put in the active anims of the cal3d mesh
    size_t anim_count = sprcal3d->GetActiveAnimCount ();
    databuf->Add ((uint32)anim_count);
    csSpriteCal3DActiveAnim* buffer = new csSpriteCal3DActiveAnim[anim_count];

    sprcal3d->GetActiveAnims (buffer, anim_count);
    uint32 i;
    for (i = 0; i < anim_count; i++)
    {
      databuf->Add ((int32) buffer[i].index);
      databuf->Add ((float) buffer[i].weight);
    }
    delete[] buffer;
  }
  /*
  else if (spr3d)
  {
    databuf->Add ((int32)spr3d->GetCurFrame ());
    databuf->Add (spr3d->GetCurAction ()->GetName ());
  }
  */
  return csPtr<iCelDataBuffer> (databuf);
}

celPersistenceResult celPcActorMove::SetPersistentData (csTicks data_time,
	iCelDataBuffer* databuf,
	celPersistenceType persistence_type)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != ACTORMOVE_SERIAL) return CEL_PERSIST_RESULT_ERROR;

  if (persistence_type == CEL_PERSIST_TYPE_RECORD_FIRST_PASS)
  {
    return CEL_PERSIST_RESULT_OK;
  }

  if (persistence_type == CEL_PERSIST_TYPE_RECORD)
  {
    Load (databuf);
    return CEL_PERSIST_RESULT_OK;
  }

  if (persistence_type == CEL_PERSIST_TYPE_SERVER_FORCING)
    return CEL_PERSIST_RESULT_OK;

  // @@@ TODO: this is a hack
  FindSiblingPropertyClasses ();
  GetSpriteStates ();

  // @@@ TODO: make some smooth updates
  if (sprcal3d)
  {
    int anim_count = databuf->GetUInt32 ();
    csSpriteCal3DActiveAnim* buffer = new csSpriteCal3DActiveAnim[anim_count];
    int i = 0;
    for (i = 0; i < anim_count; i++)
    {
      buffer[i].index = databuf->GetInt32 ();
      buffer[i].weight = databuf->GetFloat ();
    }
    sprcal3d->SetActiveAnims (buffer, anim_count);
    delete[] buffer;
  }
  /*
  else if (spr3d)
  {
    uint32 frame = databuf->GetUInt32 ();
    spr3d->SetFrame (frame);
    iString* action = databuf->GetString ();
    spr3d->SetAction (*action);
  }
  */

  return CEL_PERSIST_RESULT_OK;
}

