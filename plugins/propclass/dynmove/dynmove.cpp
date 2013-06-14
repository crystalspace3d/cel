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
  AddProperty (propid_correctup, "correctup",
	CEL_DATA_BOOL, false, "Correct up vector", 0);
  speed = 1.0f;
  jumpspeed = 1.0f;
  rotspeed = 1.0f;
  curspeed.Set (0.0f);
  correctup = false;
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

void celPcDynamicMove::TickEveryFrame ()
{
#if NEW_PHYSICS
  GetPCS ();
#else
  if (correctup && bulletBody)
  {
    iPcMesh* pcmesh = pcmechobj->GetMesh ();
    iMeshWrapper* mesh = pcmesh->GetMesh ();
    csReversibleTransform& trans = mesh->GetMovable ()->GetTransform ();
    csVector3 up = trans.GetUp ();
    if (fabs (up.x) < 0.1f && fabs (up.y - 1.0f) < 0.1f && fabs (up.z) < 0.1f)
      return;

    iRigidBody* body = pcmechobj->GetBody ();
    bulletBody->MakeKinematic ();
    trans.LookAt (trans.GetFront (), csVector3 (0, 1, 0));
    mesh->GetMovable ()->UpdateMove ();
    body->MakeDynamic ();
  }
#endif
}

bool celPcDynamicMove::SetPropertyIndexed (int idx, bool b)
{
  if (idx == propid_correctup)
  {
    correctup = b;
    if (correctup)
      pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    else
      pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    return true;
  }
  return false;
}

bool celPcDynamicMove::GetPropertyIndexed (int idx, bool& b)
{
  if (idx == propid_correctup)
  {
    b = correctup;
    return true;
  }
  return false;
}

void celPcDynamicMove::SetEntity (iCelEntity* entity)
{
  celPcCommon::SetEntity (entity);
  if (entity)
  {
    if (correctup)
      pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
    entity->QueryMessageChannel ()->Subscribe (this, "cel.input.");
  }
}

#if NEW_PHYSICS
void celPcDynamicMove::CreateBody ()
{
  if (!body)
  {
    csRef<CS::Physics::iPhysicalSystem> system = csQueryRegistry<CS::Physics::iPhysicalSystem> (
	object_reg);
    csRef<CS::Collisions::iColliderBox> collider = system->CreateColliderBox (csVector3 (.3, 1.6, .3));
    csRef<CS::Physics::iRigidBodyFactory> factory = system->CreateRigidBodyFactory (collider);
    factory->SetMass (1.0f);
    body = factory->CreateRigidBody ();
    body->SetLinearDamping (0.0f);
    body->SetAngularDamping (0.0f);
    if (pcmesh)
      body->SetAttachedSceneNode (pcmesh->GetMesh ()->QuerySceneNode ());
  }
}
#endif

#if NEW_PHYSICS
void celPcDynamicMove::Move (CS::Physics::iPhysicalSector* sector, const csReversibleTransform& trans)
{
  GetPCS ();
  body->SetTransform (trans);
  sector->AddCollisionObject (body);
}
#else
void celPcDynamicMove::Move (const csReversibleTransform& trans)
{
  GetPCS ();
  iRigidBody* body = pcmechobj->GetBody ();
  body->SetTransform (trans);
}
#endif

bool celPcDynamicMove::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  return false;
}

void celPcDynamicMove::GetPCS ()
{
#if NEW_PHYSICS
  if (!pcmesh)
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  CreateBody ();
#else
  if (!pcmechobj)
  {
    pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (entity);
    if (pcmechobj)
    {
      iRigidBody* body = pcmechobj->GetBody ();
      bulletBody = scfQueryInterface<CS::Physics::Bullet::iRigidBody> (body);
    }
  }
#endif
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
      float yangle = float (sx) / 8.0f;
#if NEW_PHYSICS
	// @@@
#else
      pcmechobj->SetAngularVelocity (csVector3 (0, yangle, 0));
#endif
      if (pcdefcamera)
      {
        float xangle = float (sy) / 10.0f;
        pcdefcamera->SetPitchVelocity (-xangle);
      }
    }
    else
    {
#if NEW_PHYSICS
      // @@@
#else
      pcmechobj->SetAngularVelocity (csVector3 (0, 0, 0));
#endif
      if (pcdefcamera)
        pcdefcamera->SetPitchVelocity (0);
    }
    g2d->SetMousePosition (g2d->GetWidth () / 2, g2d->GetHeight () / 2);
  }
  else if (msgid == id_input_forward_down)
    curspeed = csVector3 (0, 0, -25.0f * speed);
  else if (msgid == id_input_forward_up)
    curspeed = 0.0f;
  else if (msgid == id_input_backward_down)
    curspeed = csVector3 (0, 0, 25.0f * speed);
  else if (msgid == id_input_backward_up)
    curspeed = 0.0f;
  else if (msgid == id_input_strafeleft_down)
    curspeed = csVector3 (25.0f * speed, 0, 0);
  else if (msgid == id_input_strafeleft_up)
    curspeed = 0.0f;
  else if (msgid == id_input_straferight_down)
    curspeed = csVector3 (-25.0f * speed, 0, 0);
  else if (msgid == id_input_straferight_up)
    curspeed = 0.0f;
#if NEW_PHYSICS
  // @@@
#else
  else if (msgid == id_input_rotateleft_down)
    pcmechobj->SetAngularVelocity (csVector3 (0, -25.0f * rotspeed, 0));
  else if (msgid == id_input_rotateleft_up)
    pcmechobj->SetAngularVelocity (csVector3 (0));
  else if (msgid == id_input_rotateright_down)
    pcmechobj->SetAngularVelocity (csVector3 (0, 25.0f * rotspeed, 0));
  else if (msgid == id_input_rotateright_up)
    pcmechobj->SetAngularVelocity (csVector3 (0));
  else if (msgid == id_input_jump_down)
    pcmechobj->AddForceDuration (csVector3 (0, jumpspeed, 0), false, csVector3 (0, 0, 0), .2f);
#endif
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
  else return false;

#if NEW_PHYSICS
  // @@@
#else
  csReversibleTransform trans = pcmechobj->GetMesh ()->GetMesh ()->
    GetMovable ()->GetFullTransform ();
  csVector3 linvel = pcmechobj->GetLinearVelocity ();
  csVector3 newlinvel = trans.This2OtherRelative (curspeed);
  newlinvel.y = linvel.y;
  pcmechobj->SetLinearVelocity (newlinvel);
#endif

  return true;
}

//---------------------------------------------------------------------------

