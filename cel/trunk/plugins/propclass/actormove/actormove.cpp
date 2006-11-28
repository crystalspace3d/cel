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
#include "csutil/debug.h"
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
#include "isndsys/ss_listener.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (ActorMove, "pcactormove")

//---------------------------------------------------------------------------

class celActorMovableListener : public scfImplementation1<
	celActorMovableListener, iMovableListener>
{
private:
  csWeakRef<iSndSysListener> listener;

public:
  celActorMovableListener (iSndSysListener* listener)
  	: scfImplementationType (this), listener (listener)
  {
  }
  virtual ~celActorMovableListener () { }
  virtual void MovableChanged (iMovable* movable)
  {
    if (listener)
    {
      csReversibleTransform tr = movable->GetFullTransform ();
      csVector3 pos = tr.GetOrigin ();
      listener->SetPosition (pos);
      csVector3 up = tr.GetUp ();
      csVector3 front = tr.GetFront ();
      listener->SetDirection (front, up);
    }
  }
  virtual void MovableDestroyed (iMovable*) { }
};

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

PropertyHolder celPcActorMove::propinfo;

celPcActorMove::celPcActorMove (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_movement == csInvalidStringID)
  {
    id_movement = pl->FetchStringID ("cel.parameter.movement");
    id_running = pl->FetchStringID ("cel.parameter.running");
    id_rotation = pl->FetchStringID ("cel.parameter.rotation");
    id_jumping = pl->FetchStringID ("cel.parameter.jumping");
    id_start = pl->FetchStringID ("cel.parameter.start");
    id_yrot = pl->FetchStringID ("cel.parameter.yrot");
    id_x = pl->FetchStringID ("cel.parameter.x");
    id_y = pl->FetchStringID ("cel.parameter.y");
    id_animation = pl->FetchStringID ("cel.parameter.animation");
    id_anicycle = pl->FetchStringID ("cel.parameter.cycle");
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
  csRef<iGraphics3D> g3d = csQueryRegistry<iGraphics3D> (object_reg);
  g2d = g3d->GetDriver2D ();

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_setspeed, "cel.action.SetSpeed");
    AddAction (action_forward, "cel.action.Forward");
    AddAction (action_backward, "cel.action.Backward");
    AddAction (action_strafeleft, "cel.action.StrafeLeft");
    AddAction (action_straferight, "cel.action.StrafeRight");
    AddAction (action_rotateleft, "cel.action.RotateLeft");
    AddAction (action_rotateright, "cel.action.RotateRight");
    AddAction (action_rotateto, "cel.action.RotateTo");
    AddAction (action_mousemove, "cel.action.MouseMove");
    AddAction (action_run, "cel.action.Run");
    AddAction (action_autorun, "cel.action.AutoRun");
    AddAction (action_clear, "cel.action.Clear");
    AddAction (action_jump, "cel.action.Jump");
    AddAction (action_togglecameramode, "cel.action.ToggleCameraMode");
    AddAction (action_setanimation, "cel.action.SetAnimation");
  }

  // For properties.
  propinfo.SetCount (4);
  AddProperty (propid_mousemove, "cel.property.mousemove",
  	CEL_DATA_BOOL, false, "Mouse movement.", 0);
  AddProperty (propid_mousemove_inverted, "cel.property.mousemove_inverted",
  	CEL_DATA_BOOL, false, "Mouse movement inverted.", &mousemove_inverted);
  AddProperty (propid_mousemove_xfactor, "cel.property.mousemove_xfactor",
  	CEL_DATA_FLOAT, false, "Mouse movement x speed factor.",
  	&mousemove_hor_factor);
  AddProperty (propid_mousemove_yfactor, "cel.property.mousemove_yfactor",
  	CEL_DATA_FLOAT, false, "Mouse movement y speed factor.",
  	&mousemove_vert_factor);
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
    pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    g2d->SetMouseCursor (csmcArrow);
  }
  else
  {
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
  if (!mousemove) return;
  if (!pcdefcamera) return;

  int frame_width = g2d->GetWidth();
  int frame_height = g2d->GetHeight();

  csTicks cur_ticks = vc->GetCurrentTicks ();
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

  if (mousemove_totdelta >= 100.0f)
  {
    if (mousemove_inverted) mousemove_lasty = -mousemove_lasty;
    float abs_x = fabs (mousemove_lastx);
    float abs_y = fabs (mousemove_lasty);
    // Only rotate if mouse moved noticably.
    if (abs_x > 0.0001 || abs_y > 0.0001)
    {
      // Limit the maximum amount of mouse movement.
      if (abs_x > 0.4) abs_x = 0.4f;
      if (abs_y > 0.4) abs_y = 0.4f;

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

    mousemove_totdelta -= 100.0f;
    if (mousemove_totdelta >= 100.0f) mousemove_totdelta = 0.0f;
    g2d->SetMousePosition (frame_width / 2, frame_height / 2);
    mousemove_lastx = mousemove_lasty = 0.0f;
  }
}

void celPcActorMove::SetAnimation (const char *name, bool cycle)
{
  if (pcmesh)
    pcmesh->SetAnimation (name, cycle);
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
    case action_setspeed:
      {
        CEL_FETCH_FLOAT_PAR (movement,params,id_movement);
        if (p_movement) SetMovementSpeed (movement);
        CEL_FETCH_FLOAT_PAR (running,params,id_running);
        if (p_running) SetRunningSpeed (running);
        CEL_FETCH_FLOAT_PAR (rotation,params,id_rotation);
        if (p_rotation) SetRotationSpeed (rotation);
        CEL_FETCH_FLOAT_PAR (jumping,params,id_jumping);
        if (p_jumping) SetJumpingVelocity (jumping);
        return true;
      }
    case action_forward:
      {
        CEL_FETCH_BOOL_PAR (start,params,id_start);
        if (!p_start) return false;
        Forward (start);
        return true;
      }
    case action_backward:
      {
        CEL_FETCH_BOOL_PAR (start,params,id_start);
        if (!p_start) return false;
        Backward (start);
        return true;
      }
    case action_strafeleft:
      {
        CEL_FETCH_BOOL_PAR (start,params,id_start);
        if (!p_start) return false;
        StrafeLeft (start);
        return true;
      }
    case action_straferight:
      {
        CEL_FETCH_BOOL_PAR (start,params,id_start);
        if (!p_start) return false;
        StrafeRight (start);
        return true;
      }
    case action_rotateleft:
      {
        CEL_FETCH_BOOL_PAR (start,params,id_start);
        if (!p_start) return false;
        RotateLeft (start);
        return true;
      }
    case action_rotateright:
      {
        CEL_FETCH_BOOL_PAR (start,params,id_start);
        if (!p_start) return false;
        RotateRight (start);
        return true;
      }
    case action_rotateto:
      {
        CEL_FETCH_FLOAT_PAR (yrot,params,id_yrot);
        if (!p_yrot) return false;
        RotateTo (yrot);
        return true;
      }
    case action_mousemove:
      {
        CEL_FETCH_FLOAT_PAR (x,params,id_x);
        if (!p_x) return false;
        CEL_FETCH_FLOAT_PAR (y,params,id_y);
        if (!p_y) return false;
        MouseMove (x, y);
        return true;
      }
    case action_run:
      {
        CEL_FETCH_BOOL_PAR (start,params,id_start);
        if (!p_start) return false;
        Run (start);
        return true;
      }
    case action_autorun:
      {
        CEL_FETCH_BOOL_PAR (start,params,id_start);
        if (!p_start) return false;
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
      CEL_FETCH_STRING_PAR (animation,params,id_animation);
      if (!p_animation) return false;
      CEL_FETCH_BOOL_PAR (anicycle,params,id_anicycle);
      if (!p_anicycle) anicycle = true;
      SetAnimation (animation, anicycle);
      return true;
    }
    default:
      return false;
  }
}

void celPcActorMove::FindSiblingPropertyClasses ()
{
  if (HavePropertyClassesChanged ())
  {
    if (movlistener)
    {
      if (movable_for_listener)
        movable_for_listener->RemoveListener (movlistener);
      movlistener = 0;
    }

    pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
    pclinmove = CEL_QUERY_PROPCLASS_ENT (entity, iPcLinearMovement);
    pccamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcCamera);
    pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcDefaultCamera);
    pcnewcamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcNewCamera);
    pcsoundlistener = CEL_QUERY_PROPCLASS_ENT (entity, iPcSoundListener);
    checked_spritestate = false;

    if (pcsoundlistener && pcmesh)
    {
      movlistener.AttachNew (new celActorMovableListener (
      	pcsoundlistener->GetSoundListener ()));
      movable_for_listener = pcmesh->GetMesh ()->GetMovable ();
      movable_for_listener->AddListener (movlistener);
    }
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
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pcactormove", "pclinmove is missing!");
    return;
  }
  csVector3 current_position;
  iSector* current_sector;
  float current_yrot;
  pclinmove->GetLastPosition (current_position, current_yrot, current_sector);

  current_yrot = atan2f (sin (current_yrot), cos (current_yrot));
  rotate_to = atan2f (sin (yrot), cos (yrot));
  float delta_angle = atan2f (sin (rotate_to - current_yrot),
  	cos (rotate_to - current_yrot));

  if (fabs(delta_angle) < SMALL_EPSILON)
  {
    rotateright = false;
    rotateleft = false;
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

void celPcActorMove::HandleMovement (bool jump)
{
  FindSiblingPropertyClasses ();
  if (!pclinmove)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pcactormove", "pclinmove is missing!");
    return;
  }
  GetSpriteStates ();
  if (!pcmesh)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pcactormove", "pcmesh is missing!");
    return;
  }

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
  pclinmove->SetVelocity (velocity);
  if (sprcal3d) sprcal3d->SetVelocity (-velocity.z);
  // @@@ do spr3d!

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
    // @@@ do spr3d!
  }
}

void celPcActorMove::ToggleCameraMode ()
{
  FindSiblingPropertyClasses ();
  if (!pcdefcamera && !pcnewcamera)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pcactormove",
    	"Must have either a pcdefaultcamera or pcnewcamera!");
    return;
  }
  if (pcdefcamera)
      pcdefcamera->SetMode (pcdefcamera->GetNextMode ());
  if (pcnewcamera)
      pcnewcamera->NextCameraMode ();
}

csPtr<iCelDataBuffer> celPcActorMove::GetPersistentData (
	celPersistenceType persistence_type)
{
  if (persistence_type == CEL_PERSIST_TYPE_RECORD_FIRST_PASS)
    return SaveFirstPass ();

  if (persistence_type == CEL_PERSIST_TYPE_RECORD)
    return Save ();

  // TODO: this is a hack
  FindSiblingPropertyClasses ();
  GetSpriteStates ();

  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (ACTORMOVE_SERIAL);

  /*if (sprcal3d)
  {
    // TODO: use GetAnimCount () instead of GetActiveAnimCount ();
    size_t anim_count = sprcal3d->GetActiveAnimCount ();
    databuf->Add ((uint32)anim_count);
    csSpriteCal3DActiveAnim* buffer = new csSpriteCal3DActiveAnim[anim_count];

    sprcal3d->GetActiveAnims (buffer, anim_count);
    // TODO: use instead a new celData type: RAW_DATA?
    uint32 i;
    for (i = 0; i < anim_count; i++)
    {
      databuf->Add ((int32) buffer[i].index);
      databuf->Add ((float) buffer[i].weight);
    }
    delete[] buffer;
  }
  else if (spr3d)
  {
    databuf->Add ((int32)spr3d->GetCurFrame ());
    databuf->Add (spr3d->GetCurAction ()->GetName ());
  }*/

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
    LoadFirstPass (databuf);
    return CEL_PERSIST_RESULT_OK;
  }

  if (persistence_type == CEL_PERSIST_TYPE_RECORD)
  {
    Load (databuf);
    return CEL_PERSIST_RESULT_OK;
  }

  if (persistence_type == CEL_PERSIST_TYPE_SERVER_FORCING)
    return CEL_PERSIST_RESULT_OK;

  // TODO: this is a hack
  FindSiblingPropertyClasses ();
  GetSpriteStates ();

  // TODO: make some smooth update
  /*if (sprcal3d)
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
  else if (spr3d)
  {
    uint32 frame = databuf->GetUInt32 ();
    spr3d->SetFrame (frame);
    iString* action = databuf->GetString ();
    spr3d->SetAction (*action);
  }*/

  return CEL_PERSIST_RESULT_OK;
}

