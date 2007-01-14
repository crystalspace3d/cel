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

csStringID celPcMeshDeform::id_deformfactor = csInvalidStringID;
csStringID celPcMeshDeform::id_direction = csInvalidStringID;
csStringID celPcMeshDeform::id_position = csInvalidStringID;
csStringID celPcMeshDeform::id_amount = csInvalidStringID;

PropertyHolder celPcMeshDeform::propinfo;


celPcMeshDeform::celPcMeshDeform (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{
  if (id_deformfactor == csInvalidStringID)
  {
    id_deformfactor = pl->FetchStringID ("cel.parameter.deformfactor");
    id_direction = pl->FetchStringID ("cel.parameter.direction");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_amount = pl->FetchStringID ("cel.parameter.amount");
  }

  propholder = &propinfo;

  deformfactor = 1.0f;
  frequency = 100;
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

bool celPcMeshDeform::PerformActionIndexed (int idx,
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

//Change the mesh to deform
void celPcMeshDeform::SetMesh(iMeshWrapper* mesh)
{
  celPcMeshDeform::mesh = mesh;
  csRef<iGenMeshAnimationControl>
    animcontrol = controlfact->CreateAnimationControl(mesh->GetMeshObject());
  deformcontrol =
    scfQueryInterface<iDeformControl> (animcontrol);
  deformcontrol->SetMesh(mesh);
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
(const csVector3& position, const csVector3& direction, float radius,
  bool worldspace)
{
  TryGetMesh();
  csTicks currenttime = clock->GetCurrentTicks();
  csTicks timediff = currenttime - lastdeform;
  if(timediff >= frequency && mesh && deformcontrol)
  {
    lastdeform = currenttime;
    csVector3 deform_vector = direction * deformfactor;
    //If we are given worldspace co-ordinates, we need to convert them into
    // local space
    if (worldspace)
    {
      csVector3 localpos =
        mesh->GetMovable()->GetTransform().Other2This(position);
      deformcontrol->DeformMesh(localpos, deform_vector, radius);
    }
    else
      deformcontrol->DeformMesh(position, deform_vector, radius);
  }
}
