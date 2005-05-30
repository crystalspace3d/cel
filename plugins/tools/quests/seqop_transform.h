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

#ifndef __CEL_TOOLS_QUESTS_SEQOP_TRANSFORM__
#define __CEL_TOOLS_QUESTS_SEQOP_TRANSFORM__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "iengine/mesh.h"
#include "tools/questmanager.h"

#include "propclass/mesh.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard seqop type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This seqop type listens to the name 'cel.questseqop.transform'.
 */
CEL_DECLARE_SEQOPTYPE(Transform,"cel.questseqop.transform")

/**
 * The 'transform' seqop factory.
 */
class celTransformSeqOpFactory :
	public iQuestSeqOpFactory,
	public iTransformQuestSeqOpFactory
{
private:
  celTransformSeqOpType* type;
  char* entity_par;
  char* tag_par;
  char* vectorx_par;
  char* vectory_par;
  char* vectorz_par;
  int rot_axis;
  char* rot_angle_par;

public:
  celTransformSeqOpFactory (celTransformSeqOpType* type);
  virtual ~celTransformSeqOpFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestSeqOp> CreateSeqOp (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iTransformQuestSeqOpFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetVectorParameter (const char* vectorx, const char* vectory,
  	const char* vectorz);
  virtual void SetRotationParameter (int axis, const char* angle);
};

/**
 * The 'printdebug' seqop.
 */
class celTransformSeqOp :
	public iQuestSeqOp
{
private:
  celTransformSeqOpType* type;
  char* entity;
  char* tag;
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
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* vectorx, const char* vectory, const char* vectorz,
	int axis, const char* angle);
  virtual ~celTransformSeqOp ();

  SCF_DECLARE_IBASE;

  virtual bool Load (iCelDataBuffer* databuf);
  virtual void Save (iCelDataBuffer* databuf);
  virtual void Init ();
  virtual void Do (float time);
};

#endif // __CEL_TOOLS_QUESTS_SEQOP_TRANSFORM__

