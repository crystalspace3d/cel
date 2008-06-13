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

#ifndef __CEL_TOOLS_QUESTS_SEQOP_MOVEPATH__
#define __CEL_TOOLS_QUESTS_SEQOP_MOVEPATH__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "csutil/stringarray.h"
#include "csgeom/path.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"
#include "ivaria/mapnode.h"

#include "tools/questmanager.h"
#include "propclass/mesh.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard seqop type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This seqop type listens to the name 'cel.questseqop.movepath'.
 */
CEL_DECLARE_SEQOPTYPE(MovePath,"cel.questseqop.movepath")

/**
 * The 'movepath' seqop factory.
 */
class celMovePathSeqOpFactory : public scfImplementation2<
	celMovePathSeqOpFactory, iQuestSeqOpFactory,
	iMovePathQuestSeqOpFactory>
{
private:
  celMovePathSeqOpType* type;
  csString entity_par;
  csString tag_par;
  csStringArray sectors;
  csStringArray nodes;
  csStringArray times;

public:
  celMovePathSeqOpFactory (celMovePathSeqOpType* type);
  virtual ~celMovePathSeqOpFactory ();

  virtual csPtr<iQuestSeqOp> CreateSeqOp (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iMovePathQuestSeqOpFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void AddPathNode (const char* sectorname, const char* node,
  	const char* time);
};

/**
 * The 'movepath' seqop.
 */
class celMovePathSeqOp : public scfImplementation1<
	celMovePathSeqOp, iQuestSeqOp>
{
private:
  celMovePathSeqOpType* type;
  csString entity;
  csString tag;
  csPath* path;
  iSector* sector;
  float maxtime;

  csWeakRef<iMeshWrapper> mesh;

  void FindMesh ();

public:
  celMovePathSeqOp (celMovePathSeqOpType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const csStringArray& sectors, const csStringArray& nodes,
	const csStringArray& times);
  virtual ~celMovePathSeqOp ();

  virtual bool Load (iCelDataBuffer* databuf);
  virtual void Save (iCelDataBuffer* databuf);
  virtual void Init ();
  virtual void Do (float time);
};

#endif // __CEL_TOOLS_QUESTS_SEQOP_MOVEPATH__

