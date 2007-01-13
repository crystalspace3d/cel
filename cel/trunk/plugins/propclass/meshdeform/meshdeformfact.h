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

#ifndef __CEL_PF_MESHDEFORMFACT__
#define __CEL_PF_MESHDEFORMFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "imesh/genmesh.h"
#include "propclass/meshdeform.h"
#include "csutil/scf_implementation.h"
#include "plugins/propclass/meshdeform/deformanim.h"

struct iCelEntity;
struct iObjectRegistry;
struct iMeshWrapper;
struct iEngine;

//-----------------------------------------------------------


/**
 * Factory for mesh deform.
 */
CEL_DECLARE_FACTORY (MeshDeform)

/**
 * This is a test property class.
 */
class celPcMeshDeform : public scfImplementationExt1<
	celPcMeshDeform, celPcCommon, iPcMeshDeform>
{
private:
  csRef<iMeshWrapper> mesh;
  csWeakRef<iEngine> engine;

  float deformfactor;
  csRef<iGenMeshAnimationControlType> controltype;
  csRef<iGenMeshAnimationControlFactory> controlfact;
  csRef<iDeformControl> deformcontrol;
public:
  celPcMeshDeform (iObjectRegistry* object_reg);
  virtual ~celPcMeshDeform ();

  static csStringID id_deformfactor;
  static csStringID id_direction;
  static csStringID id_position;
  static csStringID id_amount;

  static PropertyHolder propinfo;

  virtual const char* GetName () const { return "pcmeshdeform"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);


  virtual void GetMesh();

  virtual void SetDeformFactor(float deformfactor)
  {celPcMeshDeform::deformfactor = deformfactor;}
  virtual void SetDeformMesh (iMeshWrapper* mesh);
  virtual iMeshWrapper* GetDeformMesh ()
   {return mesh;}
  virtual void DeformMesh
  (const csVector3& position, const csVector3& direction, float radius,
    bool worldspace = false);
};

#endif // __CEL_PF_MESHDEFORMFACT__

