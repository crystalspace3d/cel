/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein

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
#include "iutil/objreg.h"
#include "plugins/propclass/delegcam/delegate.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

// CS Includes
#include "iengine/camera.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "iutil/virtclk.h"
#include "ivaria/view.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"

// CEL Includes
#include "propclass/mesh.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (DelegateCamera, "pccamera.delegate")

csStringID celPcDelegateCamera::id_pclass = csInvalidStringID;

PropertyHolder celPcDelegateCamera::propinfo;

//---------------------------------------------------------------------------

csVector3 InterpolateVector (float i, const csVector3 &curr, const csVector3 &next)
{
  // a bit of a clamp [0, 1]
  i = (i > 1.0f) ? 1.0f : ((i < 0.0f) ? 0.0f : i);
  // a classic
  return i * (next - curr) + curr;
}

celPcDelegateCamera::celPcDelegateCamera (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  in_transition = false;
  currtrans = 0.0f;
  transtime = 2.0f;

  prev.pos.Set (0.0f);
  prev.tar.Set (0.0f);
  prev.up.Set (0.0f);

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    AddAction (action_setcurrmode, "cel.action.SetCurrentMode");
  }

  // For properties.
  propinfo.SetCount (9);
  AddProperty (propid_trans_in, "cel.property.trans",
    CEL_DATA_BOOL, true, "Whether in a transition.", &in_transition);
  AddProperty (propid_trans_time, "cel.property.trans_time",
    CEL_DATA_FLOAT, false, "Time to transition to a new mode.", &transtime);
  AddProperty (propid_trans_step, "cel.property.trans_curr",
    CEL_DATA_FLOAT, true, "0 -> 1 value indicating stage in the transition.", &currtrans);
  AddProperty (propid_prev_pos, "cel.property.prev_position",
    CEL_DATA_VECTOR3, true, "Previous mode's position.", &prev.pos);
  AddProperty (propid_prev_tar, "cel.property.prev_target",
    CEL_DATA_VECTOR3, true, "Previous mode's target.", &prev.tar);
  AddProperty (propid_prev_up, "cel.property.prev_up",
    CEL_DATA_VECTOR3, true, "Previous mode's up vector.", &prev.up);
  AddProperty (propid_pos, "cel.property.position",
    CEL_DATA_VECTOR3, true, "Current position.", &curr.pos);
  AddProperty (propid_tar, "cel.property.target",
    CEL_DATA_VECTOR3, true, "Current target.", &curr.tar);
  AddProperty (propid_up, "cel.property.up",
    CEL_DATA_VECTOR3, true, "Current up vector.", &curr.up);
}

celPcDelegateCamera::~celPcDelegateCamera ()
{
}

#define TEST_SERIAL 0

csPtr<iCelDataBuffer> celPcDelegateCamera::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TEST_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcDelegateCamera::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TEST_SERIAL) return false;
  return true;
}

bool celPcDelegateCamera::PerformActionIndexed (int idx,
  iCelParameterBlock* params,
  celData& ret)
{
  switch (idx)
  {
    case action_setcurrmode:
    {
      CEL_FETCH_PCLASS_PAR (pc, params, id_pclass);
      csRef<iPcCameraMode> camera = scfQueryInterface<iPcCameraMode> (pc);
      if (camera)
        currmode = camera;
      return true;
    }
    default:
      return false;
  }
  return false;
}

//---------------------------------------------------------------------------

void celPcDelegateCamera::SetCurrentMode (iPcCameraMode* mode)
{
  if (currmode)
  {
    prev.pos = currmode->GetPosition ();
    prev.tar = currmode->GetTarget ();
    prev.up = currmode->GetUp ();
    // start the transition!!
    in_transition = true;
    currtrans = 0.0f;
  }
  currmode = mode;
}
iPcCameraMode* celPcDelegateCamera::GetCurrentMode () const
{
  return currmode;
}

void celPcDelegateCamera::SetTransitionTime (float t)
{
  transtime = t;
}
float celPcDelegateCamera::GetTransitionTime () const
{
  return transtime;
}

void celPcDelegateCamera::UpdateCamera ()
{
  if (HavePropertyClassesChanged ())
  {
    if (!currmode)
      currmode = celQueryPropertyClassEntity<iPcCameraMode> (GetEntity ());
    if (!player)
    {
      csRef<iPcMesh> mesh = celQueryPropertyClassEntity<iPcMesh> (entity);
      if (mesh)
        player = mesh->GetMesh ()->GetMovable ();
    }
  }

  CameraDescription desired;
  if (currmode && currmode->DecideState ())
  {
    desired.pos = currmode->GetPosition ();
    desired.tar = currmode->GetTarget ();
    desired.up = currmode->GetUp ();
  }
  else
  {
    // if no mode is set or it failed deciding the state, then we improvise! :)
    if (player)
    {
      csReversibleTransform trans = player->GetFullTransform ();
      desired.pos = trans.GetOrigin () + trans.This2OtherRelative (csVector3 (0, 2, 5));
      desired.tar = trans.This2OtherRelative (csVector3 (0, -0.2, -1));;
      desired.up = trans.This2OtherRelative (csVector3 (0, 1, 0));
    }
    else  // uh-oh, now we're really guessing :P
    {
      desired.pos.Set (0.0f);
      desired.tar.Set (0, 0, 1);
      desired.up.Set (0, 1, 0);
    }
  }

  // perform a transition to new mode from old position if need be
  if (in_transition)
  {
    float elapsedsecs = vc->GetElapsedTicks () / 1000.0f;
    // update counter
    currtrans += elapsedsecs / transtime;
    if (currtrans > 1.0f)
    {
      // switch transition off
      in_transition = false;
      // and clip this for usage below
      currtrans = 1.0f;
    }
    // actually do the interpolation
    curr.pos = InterpolateVector (currtrans, prev.pos, desired.pos);
    curr.tar = InterpolateVector (currtrans, prev.tar, desired.tar);
    curr.up =  InterpolateVector (currtrans, prev.up, desired.up);
  }

  // read currently active sector
  iSector* sector = 0;
  if (player)
    sector = player->GetSectors ()->Get (0);

  // now build the transform
  csReversibleTransform camtrans;
  camtrans.SetOrigin (desired.pos);
  camtrans.LookAt (desired.tar - desired.pos, desired.up);

  iCamera* c = view->GetCamera ();
  // needs to be in the right sector
  if (sector and c->GetSector () != sector)
    c->SetSector (sector);
  c->SetTransform (camtrans);
  // only do collision detection on portals
  c->OnlyPortals (true);
}

int celPcDelegateCamera::GetDrawFlags ()
{
  return engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS
    | CSDRAW_CLEARZBUFFER;
}
void celPcDelegateCamera::Draw ()
{
  UpdateCamera ();
  // Tell 3D driver we're going to display 3D things.
  if (g3d->BeginDraw (GetDrawFlags ()))
    view->Draw ();
}
