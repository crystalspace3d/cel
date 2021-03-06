/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin
  
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

#ifndef __CEL_TOOLS_SEQOP_TRANSFORM__
#define __CEL_TOOLS_SEQOP_TRANSFORM__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "iengine/mesh.h"
#include "tools/sequences.h"

#include "propclass/mesh.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A seqop that transforms a mesh.
 * This seqop type listens to the name 'cel.seqops.transform'.
 */
CEL_DECLARE_SEQOPTYPE(Transform,"cel.seqops.transform")

/**
 * The 'transform' seqop factory.
 */
class celTransformSeqOpFactory : public scfImplementation2<
	celTransformSeqOpFactory, iSeqOpFactory,
	iTransformSeqOpFactory>
{
private:
  csRef<celTransformSeqOpType> type;
  csString entity_par;
  csString tag_par;
  csString vectorx_par;
  csString vectory_par;
  csString vectorz_par;
  int rot_axis;
  csString rot_angle_par;
  bool reversed;

public:
  celTransformSeqOpFactory (celTransformSeqOpType* type);
  virtual ~celTransformSeqOpFactory ();

  virtual csPtr<iSeqOp> CreateSeqOp (iCelParameterBlock* params);
  virtual iSeqOpType* GetSeqOpType () const { return type; }
  virtual bool Save (iDocumentNode* node);
  virtual bool Load (iDocumentNode* node);

  //----------------- iTransformQuestSeqOpFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual const char* GetEntity () const { return entity_par; }
  virtual const char* GetTag () const { return tag_par; }
  virtual void SetVectorParameter (const char* vectorx, const char* vectory,
  	const char* vectorz);
  virtual const char* GetVectorX () const { return vectorx_par; }
  virtual const char* GetVectorY () const { return vectory_par; }
  virtual const char* GetVectorZ () const { return vectorz_par; }
  virtual void SetRotationParameter (int axis, const char* angle);
  virtual int GetRotationAxis () const { return rot_axis; }
  virtual const char* GetRotationAngle () const { return rot_angle_par; }
  virtual void SetReversed (bool r) { reversed = r; }
  virtual bool IsReversed () const { return reversed; }
};

/**
 * The 'transform' seqop.
 */
class celTransformSeqOp : public scfImplementation1<
	celTransformSeqOp, iSeqOp>
{
private:
  csRef<celTransformSeqOpType> type;
  csString entity;
  csString tag;
  csVector3 vector;
  bool do_move;
  int rot_axis;
  float rot_angle;
  bool reversed;

  csRef<iParameter> entity_param;
  csRef<iParameter> tag_param;
  csRef<iParameter> vectorx_param;
  csRef<iParameter> vectory_param;
  csRef<iParameter> vectorz_param;
  csRef<iParameter> rot_angle_param;
  csRef<iParameterManager> pm;

  csVector3 start;
  csMatrix3 start_matrix;
  csWeakRef<iMeshWrapper> mesh;
  csWeakRef<iPcMesh> pcmesh;

  void FindMesh (iCelParameterBlock* params);
  void FindMesh ();

public:
  celTransformSeqOp (celTransformSeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	const char* vectorx, const char* vectory, const char* vectorz,
	int axis, const char* angle,
	bool reversed);
  virtual ~celTransformSeqOp ();

  virtual void Init (iCelParameterBlock* params);
  virtual void Do (float time, iCelParameterBlock* params);
};

#endif // __CEL_TOOLS_SEQOP_TRANSFORM__

