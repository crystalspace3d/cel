/*
    Crystal Space Entity Layer - Mesh Deformation plugin
    Copyright (C) 2007 by Christopher Fraser

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
#include "csutil/debug.h"
#include "plugins/propclass/meshdeform/meshdeformfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csgeom/vector3.h"
//#include "csgeom/box3.h"
#include "csgeom/spline.h"
#include "iengine/mesh.h"
#include "iengine/engine.h"
#include "propclass/mesh.h"
#include "imesh/object.h"
#include "imesh/genmesh.h"
#include "iengine/movable.h"
#include "csgeom/transfrm.h"
#include "iutil/virtclk.h"
#include <iostream>

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (MeshDeform, "pcmeshdeform")

//---------------------------------------------------------------------------

csStringID celPcMeshDeform::param_position = csInvalidStringID;
csStringID celPcMeshDeform::param_direction = csInvalidStringID;
csStringID celPcMeshDeform::param_worldspace = csInvalidStringID;

PropertyHolder celPcMeshDeform::propinfo;


celPcMeshDeform::celPcMeshDeform (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  if (param_position == csInvalidStringID)
  {
    param_position = pl->FetchStringID ("cel.parameter.position");
    param_direction = pl->FetchStringID ("cel.parameter.direction");
    param_worldspace = pl->FetchStringID ("cel.parameter.worldspace");
  }
  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_deformmesh, "cel.action.DeformMesh");
    AddAction (action_resetdeform, "cel.action.ResetDeform");
  }
  propinfo.SetCount (5);
  AddProperty (propid_deformfactor, "cel.property.deformfactor",
        CEL_DATA_FLOAT, false, "Deform Factor.", &deformfactor);
  AddProperty (propid_noise, "cel.property.noise",
        CEL_DATA_FLOAT, false, "Noise.", 0);
  AddProperty (propid_maxfrequency, "cel.property.maxfrequency",
        CEL_DATA_FLOAT, false, "Max frequency.", &frequency);
  AddProperty (propid_maxdeform, "cel.property.maxdeform",
        CEL_DATA_FLOAT, false, "Max deform.", 0);
  AddProperty (propid_radius, "cel.property.radius",
        CEL_DATA_FLOAT, false, "Radius.", 0);

  noise = 0.2f;
  maxdeform = 0.4f;
  radius = 1.0f;
  deformfactor = 1.0f;
  frequency = -1.0f;
  lastdeform = 0;
  mesh = 0;

  clock = csQueryRegistry<iVirtualClock> (object_reg);

  controltype = new csDeformControlType(this);
  controlfact = controltype->CreateAnimationControlFactory();
}

celPcMeshDeform::~celPcMeshDeform ()
{
delete controltype;
delete controlfact;
if (deformcontrol)
  delete deformcontrol;
}

#define TEST_SERIAL 2

csPtr<iCelDataBuffer> celPcMeshDeform::Save ()
{
 return 0;
}

bool celPcMeshDeform::Load (iCelDataBuffer* databuf)
{
 return false;
}

bool celPcMeshDeform::GetPropertyIndexed (int idx, float& f)
{
  if (idx == propid_noise)
  {
    f = GetNoise();
    return true;
  }
  else if (idx == propid_maxdeform)
  {
    f = GetMaxDeform();
    return true;
  }
  else if (idx == propid_radius)
  {
    f = GetRadius();
    return true;
  }
  return false;
}

bool celPcMeshDeform::SetPropertyIndexed (int idx, float f)
{
  if (idx == propid_noise)
  {
    SetNoise(f);
    return true;
  }
  if (idx == propid_maxdeform)
  {
    SetMaxDeform(f);
    return true;
  }
  if (idx == propid_radius)
  {
    SetRadius(f);
    return true;
  }
  return false;
}

bool celPcMeshDeform::PerformActionIndexed (int idx,
                                    iCelParameterBlock* params,
                                    celData& ret)
{
  switch (idx)
  {
    case action_deformmesh:
      {
        CEL_FETCH_VECTOR3_PAR (pos, params, param_position);
        CEL_FETCH_VECTOR3_PAR (dir, params, param_direction);
        CEL_FETCH_BOOL_PAR (world, params, param_worldspace);
        DeformMesh(pos, dir, world);
        return true;
      }
    case action_resetdeform:
      {
        ResetDeform();
        return true;
      }
  }
  return false;
}

//Change the mesh to deform
void celPcMeshDeform::SetMesh(iMeshWrapper* mesh)
{
  celPcMeshDeform::mesh = mesh;
  csRef<iGenMeshAnimationControl>
    animcontrol = controlfact->CreateAnimationControl(mesh->GetMeshObject());
  deformcontrol =
    scfQueryInterface<iDeformControl> (animcontrol);
  deformcontrol->SetMesh(mesh);
  deformcontrol->SetNoise(noise);
  deformcontrol->SetMaxDeform(maxdeform);
  deformcontrol->SetRadius(radius);
}

//This function tries to pull out the mesh from the iPcMesh
void celPcMeshDeform::TryGetMesh()
{
  if (!mesh)
  {
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT(GetEntity(), iPcMesh);
    if (pcmesh)
      SetMesh(pcmesh->GetMesh());
  }
}

//Tell the deform control to deform the mesh
void celPcMeshDeform::DeformMesh
(const csVector3& position, const csVector3& direction, bool worldspace)
{
  TryGetMesh();
  bool ready = true;
  csTicks currenttime = clock->GetCurrentTicks();
  if (frequency > 0.0f)
  {
    //Check that the time since last deform is allowed within the set frequency.
    csTicks timediff = currenttime - lastdeform;
    float mostallowed = 1000.0f / frequency;
    if (timediff <= mostallowed) ready = false;
  }
  if( ready && mesh && deformcontrol)
  {
    lastdeform = currenttime;
    //If we are given worldspace co-ordinates, we need to convert them into
    // local space
    csVector3 realpos;
    csVector3 realdir;
    if (worldspace)
    {
      realpos = mesh->GetMovable()->GetFullTransform().Other2This(position);
      realdir = mesh->GetMovable()->GetFullTransform().
                 Other2ThisRelative(direction);
    }
    else
    {
      realpos = position;
      realdir = direction;
    }
    csVector3 deform_vector = realdir * deformfactor;
    deformcontrol->DeformMesh(realpos, deform_vector);
  }
}
