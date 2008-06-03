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
    {
      currmode = celQueryPropertyClassEntity<iPcCameraMode> (GetEntity ());
    }
  }

  float elapsedsecs = vc->GetElapsedTicks () / 1000.0f;

  CameraDescription desired;
  if (currmode && currmode->DecideState ())
  {
    desired.pos = currmode->GetPosition ();
    desired.tar = currmode->GetTarget ();
    desired.up = currmode->GetUp ();
  }

  // perform a transition to new mode from old position if need be
  if (in_transition)
  {
    // update counter
    currtrans += elapsedsecs / transtime;
    if (currtrans > 1.0f)
    {
      // if going to or from a mode that made player mesh invisible
      //UpdateMeshVisibility ();
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

  csRef<iPcMesh> mesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  if (mesh)
  {
    iMovable* movable = mesh->GetMesh ()->GetMovable ();
    csReversibleTransform trans = movable->GetFullTransform ();
    iSector* sector = movable->GetSectors ()->Get (0);

    csVector3 pos = trans.GetOrigin (),
      dir = trans.This2OtherRelative (csVector3 (0.0f, 0.0f, -1.0f)),
      up  = trans.This2OtherRelative (csVector3 (0.0f, 1.0f, 0.0f));

    csReversibleTransform camtrans;
    camtrans.SetOrigin (pos);
    camtrans.LookAt (dir, up);

    iCamera* c = view->GetCamera ();
    // needs to be in the right sector
    if (c->GetSector () != sector)
      c->SetSector (sector);
    c->SetTransform (camtrans);
    c->OnlyPortals (true);
  }

  csReversibleTransform camtrans;
  camtrans.SetOrigin (desired.pos);
  camtrans.LookAt (desired.tar - desired.pos, desired.up);

  iCamera* c = view->GetCamera ();
  // needs to be in the right sector
  //if (c->GetSector () != sector)
    //c->SetSector (sector);
  c->SetTransform (camtrans);
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
