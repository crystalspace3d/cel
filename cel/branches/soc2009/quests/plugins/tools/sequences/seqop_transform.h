/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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
  celTransformSeqOpType* type;
  csString entity_par;
  csString tag_par;
  csString vectorx_par;
  csString vectory_par;
  csString vectorz_par;
  int rot_axis;
  csString rot_angle_par;

public:
  celTransformSeqOpFactory (celTransformSeqOpType* type);
  virtual ~celTransformSeqOpFactory ();

  virtual csPtr<iSeqOp> CreateSeqOp (
      const celParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iTransformQuestSeqOpFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetVectorParameter (const char* vectorx, const char* vectory,
  	const char* vectorz);
  virtual void SetRotationParameter (int axis, const char* angle);
};

/**
 * The 'transform' seqop.
 */
class celTransformSeqOp : public scfImplementation1<
	celTransformSeqOp, iSeqOp>
{
private:
  celTransformSeqOpType* type;
  csString entity;
  csString tag;
  csVector3 vector;
  bool do_move;
  int rot_axis;
  float rot_angle;

  csVector3 start;
  csMatrix3 start_matrix;
  csWeakRef<iMeshWrapper> mesh;

  void FindMesh ();

public:
  celTransformSeqOp (celTransformSeqOpType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* vectorx, const char* vectory, const char* vectorz,
	int axis, const char* angle);
  virtual ~celTransformSeqOp ();

  virtual bool Load (iCelDataBuffer* databuf);
  virtual void Save (iCelDataBuffer* databuf);
  virtual void Init ();
  virtual void Do (float time);
};

#endif // __CEL_TOOLS_SEQOP_TRANSFORM__

