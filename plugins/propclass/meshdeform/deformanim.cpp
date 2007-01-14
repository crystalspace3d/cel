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
#include "csgeom/box.h"
#include "iengine/mesh.h"
#include "iengine/engine.h"
#include "propclass/mesh.h"
#include "imesh/object.h"
#include "imesh/objmodel.h"
#include "imesh/genmesh.h"
#include "plugins/propclass/meshdeform/deformanim.h"
#include "csutil/randomgen.h"
//---------------------------------------------------------------------------

csDeformControlType::csDeformControlType(iBase* parent)
  : scfImplementationType(this, parent), object_reg(0)
{
}

csPtr<iGenMeshAnimationControlFactory>
  csDeformControlType::CreateAnimationControlFactory()
{
  csDeformControlFactory* ctrl = new csDeformControlFactory (this);
  return csPtr<iGenMeshAnimationControlFactory> (ctrl);
}
//---------------------------------------------------------------------------

csDeformControlFactory::csDeformControlFactory(iBase* parent)
  : scfImplementationType(this, parent), object_reg(0)
{
}

csPtr<iGenMeshAnimationControl>
  csDeformControlFactory::CreateAnimationControl (iMeshObject *mesh)
{
  csDeformControl* ctrl = new csDeformControl (this);
  csRef<iGeneralMeshState> state = scfQueryInterface<iGeneralMeshState> (mesh);
  state->SetAnimationControl(ctrl);
  return ctrl;
}

//---------------------------------------------------------------------------

csDeformControl::csDeformControl(iBase* parent)
  : scfImplementationType(this, parent)
{
  original_verts = 0;
  deformed_verts = 0;
  total_verts = 0;
  mesh = 0;
  r_gen = csRandomGen();
  v_gen = csRandomGen();
  noise = 0.2f;
  maxdeform = 0.5f;
}

void csDeformControl::Update(csTicks current)
{
}

const csVector3* csDeformControl::UpdateVertices
  (csTicks current, const csVector3* verts, int num_verts, uint32 version_id)
{
  //The mesh has changed, need to (re) initialise
  if(num_verts != total_verts || !original_verts || !deformed_verts)
  {
    total_verts = num_verts;
    delete[] original_verts;
    delete[] deformed_verts;
    original_verts = new csVector3[total_verts];
    deformed_verts = new csVector3[total_verts];

    for (int i = 0; i < num_verts; i++)
    {
      original_verts[i] = csVector3(verts[i].x, verts[i].y, verts[i].z);
      deformed_verts[i] = csVector3(verts[i].x, verts[i].y, verts[i].z);
    }
  }
  
  csVector3* ve = deformed_verts;
  return ve;
} 

void csDeformControl::DeformMesh
(const csVector3& position, const csVector3& direction, float radius)
{
  for (int i = 0; i < total_verts; i++)
  {
    csVector3 cvert = deformed_verts[i];
    float distance = (position - cvert).SquaredNorm();
    //Only perform the deform if the vertice is within the radius of effect.
    if (distance < radius)
    {
       //Seed the random function based on the vertice position.
       //This way two vertices at the same point won't tear.
       v_gen.Initialize(uint32(position.x * 1000.0f));
       float a = v_gen.Get();
       v_gen.Initialize(uint32(position.y * 1000.0f));
       float b = v_gen.Get();
       v_gen.Initialize(uint32(position.z * 1000.0f));
       float c = v_gen.Get();
       //Get a random float unique to the vertice position
       float r_amount = (a + b + c) / 3.0f;
       //Shift the vertice inverse proportional to its distance from point
       //And add the random noise
       float displacement = (1.0f - distance) + (r_amount * noise);
       //Now proportion in the amount that the vertice has already moved.
       float displaced = (cvert - original_verts[i]).SquaredNorm();
       displacement *= (maxdeform - displaced) / maxdeform;
       //Move the vertice
       deformed_verts[i] = cvert + (direction * displacement) ;
    }
  }
}
