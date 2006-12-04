/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_QUESTS_SEQOP_SHADERVAR__
#define __CEL_TOOLS_QUESTS_SEQOP_SHADERVAR__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "csgfx/shadervar.h"
#include "tools/questmanager.h"
#include "iengine/mesh.h"

struct iObjectRegistry;
struct iEvent;
struct iMeshWrapper;

/**
 * A standard seqop type to animate a shadervariable.
 * This seqop type listens to the name 'cel.questseqop.shadervar'.
 */
CEL_DECLARE_SEQOPTYPE(ShaderVar,"cel.questseqop.shadervar")

/**
 * The 'shadervar' seqop factory.
 */
class celShaderVarSeqOpFactory : public scfImplementation2<
	celShaderVarSeqOpFactory, iQuestSeqOpFactory,
	iShaderVarQuestSeqOpFactory>
{
private:
  celShaderVarSeqOpType* type;
  csString varname_par;
  csString mesh_entity_par;
  csString mesh_tag_par;
  csString float_par;
  csString x_par;
  csString y_par;
  csString z_par;
  csString w_par;

public:
  celShaderVarSeqOpFactory (celShaderVarSeqOpType* type);
  virtual ~celShaderVarSeqOpFactory ();

  virtual csPtr<iQuestSeqOp> CreateSeqOp (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iShaderVarQuestSeqOpFactory -----------------------
  virtual void SetVarNameParameter (const char* name);
  virtual void SetMeshEntityParameter (const char* entity,
    const char* tag = 0);
  virtual void SetFloatParameter (const char* f);
  virtual void SetVector2Parameter (const char* x, const char* y);
  virtual void SetVector3Parameter (const char* x, const char* y,
  	const char* z);
  virtual void SetVector4Parameter (const char* x, const char* y,
  	const char* z, const char* w);
};

/**
 * The 'shadervar' seqop.
 */
class celShaderVarSeqOp : public scfImplementation1<
	celShaderVarSeqOp, iQuestSeqOp>
{
private:
  celShaderVarSeqOpType* type;
  csString varname;
  csString mesh_entity;
  csString mesh_tag;

  // Number of relevant components:
  // 1=float
  // 2=vector2
  // 3=vector3
  // 4=vector4
  int numcomp;
  csVector4 start;
  csVector4 rel;

  csWeakRef<iMeshWrapper> mesh;
  csShaderVariable* shadervar;

  void FindMesh ();
  void FindShaderVar ();

public:
  celShaderVarSeqOp (celShaderVarSeqOpType* type,
  	const celQuestParams& params,
	const char* varname_par,
	const char* mesh_entity_par, const char* mesh_tag_par,
	const char* x_par, const char* y_par, const char* z_par,
	const char* w_par);
  virtual ~celShaderVarSeqOp ();

  virtual bool Load (iCelDataBuffer* databuf);
  virtual void Save (iCelDataBuffer* databuf);
  virtual void Init ();
  virtual void Do (float time);
};

#endif // __CEL_TOOLS_QUESTS_SEQOP_SHADERVAR__

