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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include <math.h>
#include "propclass/mesh.h"
#include "propclass/solid.h"
#include "propclass/zone.h"
#include "plugins/propclass/simpcam/simpcam.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/flags.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "iutil/virtclk.h"
#include "iutil/csinput.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "imap/loader.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/camera.h"
#include "iengine/collection.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/csview.h"
#include "cstool/collider.h"
#include "ivaria/view.h"
#include "ivaria/collider.h"
#include "ivaria/reporter.h"
#include "ivideo/graph3d.h"
#include "csqsqrt.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (SimpleCamera, "pccamera.simple", "pcsimplecamera")

//---------------------------------------------------------------------------

// Parameters for action_initcam
csStringID celPcSimpleCamera::param_campos = csInvalidStringID;
csStringID celPcSimpleCamera::param_lookat = csInvalidStringID;
csStringID celPcSimpleCamera::param_drawmesh = csInvalidStringID;

// Parameters for action_setmesh
csStringID celPcSimpleCamera::param_mesh = csInvalidStringID;

PropertyHolder celPcSimpleCamera::propinfo;

celPcSimpleCamera::celPcSimpleCamera (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  objectlookat.Set (0, 0, 0);
  objectcampos.Set (0, 0, 0);
  objectcampos_world = false;

  lastActorSector = 0;
  meshExplicitlySet = false;

  propholder = &propinfo;

  // Actions
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.camera.simple.action.");
    AddAction (action_initcam, "InitCamera");
    AddAction (action_setpos, "SetPosition");
    AddAction (action_setmesh, "SetMesh");
  }

  // Parameters for action_initcam
  if (param_campos == csInvalidStringID)
  {
    param_campos = pl->FetchStringID ("campos");
    param_lookat = pl->FetchStringID ("lookat");
    param_drawmesh = pl->FetchStringID ("drawmesh");
    param_mesh = pl->FetchStringID ("meshpctag");
  }
}

celPcSimpleCamera::~celPcSimpleCamera ()
{
}

void celPcSimpleCamera::FindSiblingPropertyClasses ()
{
  if (!meshExplicitlySet)
  {
    if (HavePropertyClassesChanged ())
    {
      pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    }
  }
}

void celPcSimpleCamera::GetActorTransform ()
{
  // Try to get position and sector from the mesh.
  FindSiblingPropertyClasses ();
  if (pcmesh)
  {
    iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
    actor_trans = movable->GetFullTransform ();
    actor_sector = movable->GetSectors ()->Get (0);
  }
  else
  {
    actor_trans.SetT2O (csMatrix3 ());
    actor_sector = 0;
  }
}

bool celPcSimpleCamera::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_initcam:
      {
	csVector3 campos, lookat;
	if (!Fetch (campos, params, param_campos)) return false;
        SetCameraOffset (campos);
	if (!Fetch (lookat, params, param_lookat)) return false;
        SetLookAtOffset (lookat);
	bool drawmesh;
	if (!Fetch (drawmesh, params, param_drawmesh)) return false;
        SetDrawMesh (drawmesh);
        return true;
      }
    case action_setpos:
      {
	csVector3 campos, lookat;
	if (!Fetch (campos, params, param_campos)) return false;
        SetCameraOffset (campos);
	if (!Fetch (lookat, params, param_lookat)) return false;
        SetLookAtOffset (lookat);
        return true;
      }
    case action_setmesh:
      {
	csString mesh;
	if (!Fetch (mesh, params, param_mesh)) return false;
        csRef<iPcMesh> pcmesh = celQueryPropertyClassTagEntity<iPcMesh> (GetEntity (), mesh);
        if (!pcmesh)
        {
          csString msg = "Couldn't find mesh with given tag: ";
          msg += mesh;
          return Error (msg);
        }
        SetMesh (pcmesh);
        return true;
      }
    default:
      return false;
  }
}

void celPcSimpleCamera::SetDrawMesh (bool draw)
{
  drawmesh = draw;
  if (pcmesh)
  {
    if (drawmesh)
    {
      pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE, 0);
    }
    else
    {
      pcmesh->GetMesh ()->SetFlagsRecursive (CS_ENTITY_INVISIBLE,
      	CS_ENTITY_INVISIBLE);
    }
  }
}

void celPcSimpleCamera::UpdateCamera ()
{
  // Try to get position and sector from mesh.
  GetActorTransform ();
  if (actor_sector)
  {
    // Adjust camera transform for relative position and lookat position.
    csReversibleTransform cam_trans;
    if (objectcampos_world)
      cam_trans.SetOrigin (actor_trans.GetOrigin () + objectcampos);
    else
      cam_trans.SetOrigin (actor_trans.This2Other (objectcampos));
    cam_trans.LookAt (cam_trans.Other2This (actor_trans.This2Other (
      objectlookat)), actor_trans.This2OtherRelative (csVector3 (0, 1, 0)));

    iCamera* c = view->GetCamera ();

    // First set the camera back on where the sector is.
    // We assume here that normal camera movement is good.
    if (c->GetSector () != actor_sector)
      c->SetSector (actor_sector);
    c->SetTransform (cam_trans);
    c->OnlyPortals (true);
  }
}

int celPcSimpleCamera::GetDrawFlags ()
{
  return CSDRAW_3DGRAPHICS;
}

void celPcSimpleCamera::Draw ()
{
  UpdateCamera ();
  // Tell 3D driver we're going to display 3D things.
  if (g3d->BeginDraw (CSDRAW_3DGRAPHICS))
    view->Draw ();
}

void celPcSimpleCamera::SetCameraOffset (const csVector3& offset, bool world)
{
  objectcampos = offset;
  objectcampos_world = world;
}

void celPcSimpleCamera::SetLookAtOffset (const csVector3& lookat)
{
  objectlookat = lookat;
}

void celPcSimpleCamera::SetMesh (iPcMesh* mesh)
{
  if (mesh)
  {
    pcmesh = mesh;
    meshExplicitlySet = true;
  }
  else
  {
    pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    meshExplicitlySet = false;
  }
}

//---------------------------------------------------------------------------


