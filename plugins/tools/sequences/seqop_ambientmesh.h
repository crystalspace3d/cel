/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_SEQOP_AMBIENTMESH__
#define __CEL_TOOLS_SEQOP_AMBIENTMESH__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "iengine/mesh.h"
#include "tools/sequences.h"
#include "csgfx/shadervarcontext.h"

#include "propclass/mesh.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard seqop type to animate the ambient of a mesh.
 * This seqop type listens to the name 'cel.seqops.ambientmesh'.
 */
CEL_DECLARE_SEQOPTYPE(AmbientMesh,"cel.seqops.ambientmesh")

/**
 * The 'ambientmesh' seqop factory.
 */
class celAmbientMeshSeqOpFactory : public scfImplementation2<
	celAmbientMeshSeqOpFactory, iSeqOpFactory,
	iAmbientMeshSeqOpFactory>
{
private:
  csRef<celAmbientMeshSeqOpType> type;
  csString entity_par;
  csString tag_par;
  csString rel_red_par;
  csString rel_green_par;
  csString rel_blue_par;
  csString abs_red_par;
  csString abs_green_par;
  csString abs_blue_par;

public:
  celAmbientMeshSeqOpFactory (celAmbientMeshSeqOpType* type);
  virtual ~celAmbientMeshSeqOpFactory ();

  virtual csPtr<iSeqOp> CreateSeqOp (
      iCelParameterBlock* params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iAmbientMeshSeqOpFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetRelColorParameter (const char* red, const char* green,
  	const char* blue);
  virtual void SetAbsColorParameter (const char* red, const char* green,
  	const char* blue);
};

/**
 * The 'ambientmesh' seqop.
 */
class celAmbientMeshSeqOp : public scfImplementation1<
	celAmbientMeshSeqOp, iSeqOp>
{
private:
  csRef<celAmbientMeshSeqOpType> type;
  csString entity;
  csString tag;
  csColor rel;
  csColor abs;
  bool do_abs;
  bool do_rel;
  CS::ShaderVarStringID ambient;

  csRef<iParameter> entity_param;
  csRef<iParameter> tag_param;
  csRef<iParameter> rel_red_param;
  csRef<iParameter> rel_green_param;
  csRef<iParameter> rel_blue_param;
  csRef<iParameter> abs_red_param;
  csRef<iParameter> abs_green_param;
  csRef<iParameter> abs_blue_param;

  csColor start;
  csWeakRef<iMeshWrapper> mesh;
  csWeakRef<iShaderVariableContext> svc;
  csRef<iParameterManager> pm;

  void SetAmbientColor (const csColor& color);
  csColor GetAmbientColor ();
  void FindMesh (iCelParameterBlock* params);

public:
  celAmbientMeshSeqOp (celAmbientMeshSeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	const char* rel_red_par, const char* rel_green_par,
	const char* rel_blue_par,
	const char* abs_red_par, const char* abs_green_par,
	const char* abs_blue_par);
  virtual ~celAmbientMeshSeqOp ();

  virtual void Init (iCelParameterBlock* params);
  virtual void Do (float time, iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_SEQOP_AMBIENTMESH__

