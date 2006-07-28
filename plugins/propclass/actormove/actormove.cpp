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

csStringID celPcActorMove::action_setspeed = csInvalidStringID;
csStringID celPcActorMove::action_forward = csInvalidStringID;
csStringID celPcActorMove::action_backward = csInvalidStringID;
csStringID celPcActorMove::action_strafeleft = csInvalidStringID;
csStringID celPcActorMove::action_straferight = csInvalidStringID;
csStringID celPcActorMove::action_rotateleft = csInvalidStringID;
csStringID celPcActorMove::action_rotateright = csInvalidStringID;
csStringID celPcActorMove::action_rotateto = csInvalidStringID;
csStringID celPcActorMove::action_mousemove = csInvalidStringID;
csStringID celPcActorMove::action_run = csInvalidStringID;
csStringID celPcActorMove::action_autorun = csInvalidStringID;
csStringID celPcActorMove::action_jump = csInvalidStringID;
csStringID celPcActorMove::action_togglecameramode = csInvalidStringID;
csStringID celPcActorMove::id_movement = csInvalidStringID;
csStringID celPcActorMove::id_running = csInvalidStringID;
csStringID celPcActorMove::id_rotation = csInvalidStringID;
csStringID celPcActorMove::id_jumping = csInvalidStringID;
csStringID celPcActorMove::id_start = csInvalidStringID;
csStringID celPcActorMove::id_yrot = csInvalidStringID;
csStringID celPcActorMove::id_x = csInvalidStringID;
csStringID celPcActorMove::id_y = csInvalidStringID;

celPcActorMove::celPcActorMove (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (action_setspeed == csInvalidStringID)
  {
    action_setspeed = pl->FetchStringID ("cel.action.SetSpeed");
    action_forward = pl->FetchStringID ("cel.action.Forward");
    action_backward = pl->FetchStringID ("cel.action.Backward");
    action_strafeleft = pl->FetchStringID ("cel.action.StrafeLeft");
    action_straferight = pl->FetchStringID ("cel.action.StrafeRight");
    action_rotateleft = pl->FetchStringID ("cel.action.RotateLeft");
    action_rotateright = pl->FetchStringID ("cel.action.RotateRight");
    action_rotateto = pl->FetchStringID ("cel.action.RotateTo");
    action_mousemove = pl->FetchStringID ("cel.action.MouseMove");
    action_run = pl->FetchStringID ("cel.action.Run");
    action_autorun = pl->FetchStringID ("cel.action.AutoRun");
    action_jump = pl->FetchStringID ("cel.action.Jump");
    action_togglecameramode = pl->FetchStringID (
    	"cel.action.ToggleCameraMode");
    id_movement = pl->FetchStringID ("cel.parameter.movement");
    id_running = pl->FetchStringID ("cel.parameter.running");
    id_rotation = pl->FetchStringID ("cel.parameter.rotation");
    id_jumping = pl->FetchStringID ("cel.parameter.jumping");
    id_start = pl->FetchStringID ("cel.parameter.start");
    id_yrot = pl->FetchStringID ("cel.parameter.yrot");
    id_x = pl->FetchStringID ("cel.parameter.x");
    id_y = pl->FetchStringID ("cel.parameter.y");
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

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_mousemove] = 0;		// Handled in this class.
  propdata[propid_mousemove_inverted] = &mousemove_inverted;
  propdata[propid_mousemove_xfactor] = &mousemove_hor_factor;
  propdata[propid_mousemove_yfactor] = &mousemove_vert_factor;
}

celPcActorMove::~celPcActorMove ()
{
}

Property* celPcActorMove::properties = 0;
size_t celPcActorMove::propertycount = 0;

void celPcActorMove::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    propertycount = 4;
    properties = new Property[propertycount];

    properties[propid_mousemove].id = pl->FetchStringID (
    	"cel.property.mousemove");
    properties[propid_mousemove].datatype = CEL_DATA_BOOL;
    properties[propid_mousemove].readonly = false;
    properties[propid_mousemove].desc = "Mouse movement.";

    properties[propid_mousemove_inverted].id = pl->FetchStringID (
    	"cel.property.mousemove_inverted");
    properties[propid_mousemove_inverted].datatype = CEL_DATA_BOOL;
    properties[propid_mousemove_inverted].readonly = false;
    properties[propid_mousemove_inverted].desc = "Mouse movement inverted.";

    properties[propid_mousemove_xfactor].id = pl->FetchStringID (
    	"cel.property.mousemove_xfactor");
    properties[propid_mousemove_xfactor].datatype = CEL_DATA_FLOAT;
    properties[propid_mousemove_xfactor].readonly = false;
    properties[propid_mousemove_xfactor].desc = "Mouse movement x speed factor.";

    properties[propid_mousemove_yfactor].id = pl->FetchStringID (
    	"cel.property.mousemove_yfactor");
    properties[propid_mousemove_yfactor].datatype = CEL_DATA_FLOAT;
    properties[propid_mousemove_yfactor].readonly = false;
    properties[propid_mousemove_yfactor].desc = "Mouse movement y speed factor.";
  }
}

bool celPcActorMove::SetProperty (csStringID propertyId, bool b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_mousemove].id)
  {
    EnableMouseMove (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

bool celPcActorMove::GetPropertyBool (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_mousemove].id)
  {
    return mousemove;
  }
  else
  {
    return celPcCommon::GetPropertyBool (propertyId);
  }
}

void celPcActorMove::MouseMove (float x, float y)
{
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
    if ((abs_x > 0.0001 || abs_y > 0.0001) && abs_x < .4 && abs_y < .4)
    {
      pcdefcamera->MovePitch ((-mousemove_lasty)
	  * mousemove_vert_factor * MOUSEMOVE_VERT_FACTOR);
      float s = GetRotationSpeed();
      SetRotationSpeed (fabs (mousemove_lastx)
	  * mousemove_hor_factor * MOUSEMOVE_HOR_FACTOR * 100.0f);
      if (fabs (mousemove_lastx) < 0.0001f)
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
    pcmesh->SetAnimation(name,cycle);
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

bool celPcActorMove::PerformAction (csStringID actionId,
	iCelParameterBlock* params, celData& ret)
{
  if (actionId == action_setspeed)
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
  else if (actionId == action_forward)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    Forward (start);
    return true;
  }
  else if (actionId == action_backward)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    Backward (start);
    return true;
  }
  else if (actionId == action_strafeleft)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    StrafeLeft (start);
    return true;
  }
  else if (actionId == action_straferight)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    StrafeRight (start);
    return true;
  }
  else if (actionId == action_rotateleft)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    RotateLeft (start);
    return true;
  }
  else if (actionId == action_rotateright)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    RotateRight (start);
    return true;
  }
  else if (actionId == action_rotateto)
  {
    CEL_FETCH_FLOAT_PAR (yrot,params,id_yrot);
    if (!p_yrot) return false;
    RotateTo (yrot);
    return true;
  }
  else if (actionId == action_mousemove)
  {
    CEL_FETCH_FLOAT_PAR (x,params,id_x);
    if (!p_x) return false;
    CEL_FETCH_FLOAT_PAR (y,params,id_y);
    if (!p_y) return false;
    MouseMove (x, y);
    return true;
  }
  else if (actionId == action_run)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    Run (start);
    return true;
  }
  else if (actionId == action_autorun)
  {
    CEL_FETCH_BOOL_PAR (start,params,id_start);
    if (!p_start) return false;
    AutoRun (start);
    return true;
  }
  else if (actionId == action_jump)
  {
    Jump ();
    return true;
  }
  else if (actionId == action_togglecameramode)
  {
    ToggleCameraMode ();
    return true;
  }
  return false;
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
  sprcal3d = SCF_QUERY_INTERFACE (o, iSpriteCal3DState);
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

  static csVector3 velocity(0,0,0);

  if ((autorun || forward) && straferight)
  {
    velocity.x = -0.75 * speed;
    velocity.z = -0.75 * speed;
  }
  else if ((autorun || forward) && strafeleft)
  {
    velocity.x = 0.75 * speed;
    velocity.z = -0.75 * speed;
  }
  else if (autorun || forward)
  {
    velocity.x = 0;
    velocity.z = -speed;
  }
  else if (backward && straferight)
  {
    velocity.x = -0.75 * speed;
    velocity.z = 0.75 * speed;
  }
  else if (backward && strafeleft)
  {
    velocity.x = 0.75 * speed;
    velocity.z = 0.75 * speed;
  }
  else if (backward)
  {
    velocity.x = 0;
    velocity.z = speed;
  }
  else if (straferight)
  {
    velocity.x = -speed;
    velocity.z = 0;
  }
  else if (strafeleft)
  {
    velocity.x = speed;
    velocity.z = 0;
  }
  else
  {
    velocity.x = 0;
    velocity.z = 0;
  }
  pclinmove->SetVelocity (velocity);
  if (sprcal3d) sprcal3d->SetVelocity (-velocity.z);
  // @@@ do spr3d!

  float actual_rotating_speed = 0;
  if (rotateright)
    actual_rotating_speed = -rotating_speed;
  else if (rotateleft)
    actual_rotating_speed = rotating_speed;
  if (rotatetoreached)
  {
    pclinmove->SetAngularVelocity (csVector3 (0, actual_rotating_speed, 0));
  }
  else
  {
    pclinmove->SetAngularVelocity (csVector3 (0, actual_rotating_speed, 0),
				   csVector3 (0, rotate_to, 0));
  }

  if (jump && pclinmove->IsOnGround ())
  {
    csVector3 velocity (0,jumping_velocity, 0);
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
		    "cel.pcactormove", "Must have either a pcdefaultcamera or pcnewcamera!");
    return;
  }
  if (pcdefcamera)
      pcdefcamera->SetMode (pcdefcamera->GetNextMode ());
  if (pcnewcamera)
      pcnewcamera->NextCameraMode();
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

