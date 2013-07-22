/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein

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
#include "csgeom/transfrm.h"
#include "iutil/objreg.h"
#include "iutil/csinput.h"
#include "iutil/virtclk.h"
#include "plugins/propclass/dynmove/dynmove.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "propclass/defcam.h"
#include "propclass/mesh.h"

#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "ivaria/dynamics.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (DynamicMove, "pcmove.actor.dynamic")

//---------------------------------------------------------------------------

csStringID celPcDynamicMove::id_input_forward_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_backward_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_forward_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_backward_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_strafeleft_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_straferight_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_strafeleft_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_straferight_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_jump_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_lookup_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_lookup_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_lookdown_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_lookdown_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_center_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_rotateleft_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_rotateleft_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_rotateright_down = csInvalidStringID;
csStringID celPcDynamicMove::id_input_rotateright_up = csInvalidStringID;
csStringID celPcDynamicMove::id_input_mouselook = csInvalidStringID;

PropertyHolder celPcDynamicMove::propinfo;

celPcDynamicMove::celPcDynamicMove (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_input_forward_down == csInvalidStringID)
  {
    id_input_forward_down = pl->FetchStringID ("cel.input.forward.down");
    id_input_backward_down = pl->FetchStringID ("cel.input.backward.down");
    id_input_forward_up = pl->FetchStringID ("cel.input.forward.up");
    id_input_backward_up = pl->FetchStringID ("cel.input.backward.up");
    id_input_strafeleft_down = pl->FetchStringID ("cel.input.strafeleft.down");
    id_input_straferight_down = pl->FetchStringID ("cel.input.straferight.down");
    id_input_strafeleft_up = pl->FetchStringID ("cel.input.strafeleft.up");
    id_input_straferight_up = pl->FetchStringID ("cel.input.straferight.up");
    id_input_jump_down = pl->FetchStringID ("cel.input.jump.down");
    id_input_lookup_down = pl->FetchStringID ("cel.input.lookup.down");
    id_input_lookup_up = pl->FetchStringID ("cel.input.lookup.up");
    id_input_lookdown_down = pl->FetchStringID ("cel.input.lookdown.down");
    id_input_lookdown_up = pl->FetchStringID ("cel.input.lookdown.up");
    id_input_center_down = pl->FetchStringID ("cel.input.center.down");
    id_input_rotateleft_down = pl->FetchStringID ("cel.input.rotateleft.down");
    id_input_rotateleft_up = pl->FetchStringID ("cel.input.rotateleft.up");
    id_input_rotateright_down = pl->FetchStringID ("cel.input.rotateright.down");
    id_input_rotateright_up = pl->FetchStringID ("cel.input.rotateright.up");
    id_input_mouselook = pl->FetchStringID ("cel.input.mouselook");
  }

  propholder = &propinfo;
  propinfo.SetCount (4);
  AddProperty (propid_speed, "speed",
	CEL_DATA_FLOAT, false, "Movement speed", &speed);
  AddProperty (propid_jumpspeed, "jumpspeed",
	CEL_DATA_FLOAT, false, "Jump speed", &jumpspeed);
  AddProperty (propid_rotspeed, "rotspeed",
	CEL_DATA_FLOAT, false, "Rotation speed", &rotspeed);
  speed = 1.0f;
  jumpspeed = 1.0f;
  rotspeed = 1.0f;
  curspeed.Set (0.0f);
  rotate = 0;

  vc = csQueryRegistry<iVirtualClock> (object_reg);
  csRef<iGraphics3D> g3d = csQueryRegistry<iGraphics3D> (object_reg);
  g2d = g3d->GetDriver2D ();
  mouse = csQueryRegistry<iMouseDriver> (object_reg);

  mouselookEnabled = true;
}

celPcDynamicMove::~celPcDynamicMove ()
{
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
}

void celPcDynamicMove::EnableMouselook (bool enable)
{
  mouselookEnabled = enable;
  if (mouselookEnabled)
    g2d->SetMousePosition (g2d->GetWidth () / 2, g2d->GetHeight () / 2);
}

void celPcDynamicMove::SetEntity (iCelEntity* entity)
{
  celPcCommon::SetEntity (entity);
  if (entity)
  {
    pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    entity->QueryMessageChannel ()->Subscribe (this, "cel.input.");
  }
}

void celPcDynamicMove::CreateBody ()
{
  if (!actor)
  {
    csRef<CS::Physics::iPhysicalSystem> system = csQueryRegistry<CS::Physics::iPhysicalSystem> (
	object_reg);
    csRef<CS::Collisions::iColliderBox> collider = system->CreateColliderBox (csVector3 (.3, 1.6, .3));
    csRef<CS::Collisions::iCollisionActorFactory> factory = system->CreateCollisionActorFactory (collider);
    //factory->SetMass (1.0f);
    actor = factory->CreateCollisionActor ();
    //body->SetLinearDamping (0.0f);
    //body->SetAngularDamping (0.0f);
    if (pcmesh)
      actor->SetAttachedSceneNode (pcmesh->GetMesh ()->QuerySceneNode ());
  }
}

void celPcDynamicMove::Move (CS::Physics::iPhysicalSector* sector, const csReversibleTransform& trans)
{
  GetPCS ();
  actor->SetTransform (trans);
  sector->AddCollisionObject (actor);
}

bool celPcDynamicMove::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  return false;
}

void celPcDynamicMove::GetPCS ()
{
  if (!pcmesh)
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  CreateBody ();
}

void celPcDynamicMove::GetCam ()
{
  if (pcdefcamera) return;
  pcdefcamera = celQueryPropertyClassEntity<iPcDefaultCamera> (entity);
}

bool celPcDynamicMove::ReceiveMessage (csStringID msgid, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  if (celPcCommon::ReceiveMessage (msgid, sender, ret, params))
    return true;

  GetPCS ();

  bool rc = true;

  if (msgid == id_input_mouselook)
  {
    GetCam ();
    if (!mouselookEnabled) return false;
    g2d->SetMouseCursor (csmcNone);
    int x = mouse->GetLastX ();
    int y = mouse->GetLastY ();
    int sx = x - g2d->GetWidth () / 2;
    int sy = y - g2d->GetHeight () / 2;
    if (sx != 0 || sy != 0)
    {
      rotate = float (sx) / 8.0f;
      if (pcdefcamera)
      {
        float xangle = float (sy) / 10.0f;
        pcdefcamera->SetPitchVelocity (-xangle);
      }
    }
    else
    {
      rotate = 0.0f;
      if (pcdefcamera)
        pcdefcamera->SetPitchVelocity (0);
    }
    g2d->SetMousePosition (g2d->GetWidth () / 2, g2d->GetHeight () / 2);
  }

  else if (msgid == id_input_forward_down)
    curspeed.z = -25.0f * speed;
  else if (msgid == id_input_forward_up)
    curspeed.z = 0.0f;

  else if (msgid == id_input_backward_down)
    curspeed.z = 25.0f * speed;
  else if (msgid == id_input_backward_up)
    curspeed.z = 0.0f;

  else if (msgid == id_input_strafeleft_down)
    curspeed.x = 25.0f * speed;
  else if (msgid == id_input_strafeleft_up)
    curspeed.x = 0.0f;

  else if (msgid == id_input_straferight_down)
    curspeed.x = -25.0f * speed;
  else if (msgid == id_input_straferight_up)
    curspeed.x = 0.0f;

  else if (msgid == id_input_rotateleft_down)
    rotate = -1.0f;
  else if (msgid == id_input_rotateleft_up)
    rotate = 0.0f;

  else if (msgid == id_input_rotateright_down)
    rotate = 1.0f;
  else if (msgid == id_input_rotateright_up)
    rotate = 0.0f;

  else if (msgid == id_input_jump_down)
    actor->Jump ();	// @@@ Test if on ground!

  else if (msgid == id_input_lookup_down)
  {
    GetCam ();
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (1.0f);
  }
  else if (msgid == id_input_lookup_up)
  {
    GetCam ();
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
  }
  else if (msgid == id_input_lookdown_down)
  {
    GetCam ();
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (-1.0f);
  }
  else if (msgid == id_input_lookdown_up)
  {
    GetCam ();
    if (pcdefcamera)
      pcdefcamera->SetPitchVelocity (0.0f);
  }
  else if (msgid == id_input_center_down)
  {
    GetCam ();
    if (pcdefcamera)
      pcdefcamera->CenterCamera ();
  }
  else rc = false;

  actor->SetSpeed (curspeed);

  return rc;
}


void celPcDynamicMove::TickEveryFrame ()
{
  if (fabs (rotate) > 0.001f)
  {
    float yaw = rotate * vc->GetElapsedSeconds () * rotspeed;
    actor->Rotate (yaw);
  }
}

//---------------------------------------------------------------------------

