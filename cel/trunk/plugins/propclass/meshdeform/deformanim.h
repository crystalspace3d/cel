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

#ifndef __CEL_PF_DEFORMANIM__
#define __CEL_PF_DEFORMANIM__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "imesh/genmesh.h"
#include "csutil/randomgen.h"
#include "csutil/scf_implementation.h"

struct iMeshWrapper;
//---- Genmesh Anim support code ------------------------------------

struct iDeformControl : public virtual iBase
{
  SCF_INTERFACE (iDeformControl, 0, 0, 1);
  virtual void SetMesh(iMeshWrapper* mesh) = 0;
  virtual void DeformMesh
    (const csVector3& position, const csVector3& direction, float amount) = 0;
  virtual void SetNoise(float noise) = 0;
  virtual void SetMaxDeform(float maxdeform) = 0;
  virtual float GetNoise() = 0;
  virtual float GetMaxDeform() = 0;
};

class csDeformControlType : public scfImplementation1
  <csDeformControlType, iGenMeshAnimationControlType>
{
  public:
    iObjectRegistry* object_reg;
    csDeformControlType (iBase* parent);
    virtual ~csDeformControlType () {}
    virtual csPtr<iGenMeshAnimationControlFactory>
    CreateAnimationControlFactory();
};

class csDeformControlFactory : public scfImplementation1
  <csDeformControlFactory, iGenMeshAnimationControlFactory>
{
  public:
    iGenMeshAnimationControlType* control;
    iObjectRegistry* object_reg;
    csDeformControlFactory (iBase* parent);
    virtual ~csDeformControlFactory ()
    {}
    virtual csPtr<iGenMeshAnimationControl>
        CreateAnimationControl (iMeshObject *mesh);
    virtual const char* Load (iDocumentNode* node) {return 0;}
    virtual const char* Save (iDocumentNode* parent) {return 0;}
};

class csDeformControl : public scfImplementation2
  <csDeformControl, iGenMeshAnimationControl, iDeformControl>
{
  private:
    csVector3* original_verts;
    csVector3* deformed_verts;
    int total_verts;
    iMeshWrapper* mesh;
    csRandomGen r_gen;
    csRandomGen v_gen;
    float noise;
    float maxdeform;

  public:
    csDeformControl (iBase* parent);
    virtual ~csDeformControl () {}
    virtual bool AnimatesVertices () const {return true;}
    virtual bool AnimatesTexels () const {return false;}
    virtual bool AnimatesNormals () const {return false;}
    virtual bool AnimatesColors () const {return false;}
    virtual const csVector3* UpdateVertices (csTicks current,
    const csVector3* verts, int num_verts, uint32 version_id);

    virtual void Update (csTicks current);
    virtual const csVector2* UpdateTexels (csTicks current,
      const csVector2* texels, int num_texels, uint32 version_id)
    {return texels;}
    virtual const csVector3* UpdateNormals (csTicks current,
      const csVector3* normals, int num_normals, uint32 version_id)
    {return normals;}
    virtual const csColor4* UpdateColors (csTicks current,
      const csColor4* colors, int num_colors, uint32 version_id)
    {return colors;}

    virtual void SetMesh(iMeshWrapper* mesh)
    {csDeformControl::mesh = mesh;}
    virtual void DeformMesh
    (const csVector3& position, const csVector3& direction, float radius);
    virtual void SetNoise(float noise)
    {csDeformControl::noise = noise;}
    virtual void SetMaxDeform(float maxdeform)
    {csDeformControl::maxdeform = maxdeform;}

    virtual float GetNoise()
    {return noise;}
    virtual float GetMaxDeform()
    {return maxdeform;}
}; 

#endif // __CEL_PF_DEFORMANIM__
