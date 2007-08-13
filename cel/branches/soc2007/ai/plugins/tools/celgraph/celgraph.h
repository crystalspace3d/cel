/*    Crystal Space Entity Layer
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

#ifndef __CEL_TOOLS_CELGRAPH__
#define __CEL_TOOLS_CELGRAPH__

#include "csutil/util.h"
#include "csutil/hash.h"
#include "csutil/redblacktree.h"
#include "csutil/weakrefarr.h"
#include "csutil/stringarray.h"
#include "csutil/eventhandlers.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "ivaria/conin.h"
#include "ivaria/conout.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
#include "physicallayer/propclas.h"
#include "behaviourlayer/behave.h"
#include "behaviourlayer/bl.h"
#include "tools/celgraph.h"
#include "propclass/prop.h"
#include "propclass/quest.h"

#include <stdio.h>
#include <string.h>

/**
 * This is a mapnode Graph for CEL.
 */
class celEdge : public scfImplementation2<
		   celEdge, iCelEdge, iComponent>
{
 private:
  iObjectRegistry* object_reg;
  csRef<iCelNode> successor;
  //csRef<iCelNode> predecessor;
  bool state;

 public:
  celEdge (iBase* parent);
  virtual ~celEdge ();
  virtual void SetState (bool open);
  virtual void SetSuccessor (iCelNode* node);
  virtual bool GetState ();
  virtual iCelNode* GetSuccessor ();
  //virtual iCelNode* GetPredecessor ();
  virtual bool Initialize (iObjectRegistry* object_reg);
};

class celNode : public scfImplementation2<
		   celNode, iCelNode, iComponent>
{
 private:
  iObjectRegistry* object_reg;
  csRefArray<iCelEdge> edges;
  csRef<iMapNode> map_node;
  csRef<iCelNode> parent;
  float heuristic;
  float cost;
  char* name;
  
 public:
  
  celNode (iBase* parent);
  virtual ~celNode ();
  virtual void AddSuccessor (iCelNode* node, bool state);
  virtual void SetMapNode (iMapNode* node);
  virtual void SetParent (iCelNode* par);
  virtual void SetName (const char* n);
  virtual void Heuristic(float cost, iCelNode* goal);
  virtual iMapNode* GetMapNode ();
  virtual csVector3 GetPosition ();
  virtual char* GetName ();
  virtual iCelNode* GetParent () {return parent;}
  virtual csArray<iCelNode*> GetSuccessors ();
  virtual csArray<iCelNode*> GetAllSuccessors ();
  virtual float GetHeuristic () {return heuristic;};
  virtual float GetCost () {return cost;};
  virtual bool Initialize (iObjectRegistry* object_reg);
};

/**
 * This is a mapnode Path for CEL.
 */ 

class celPath : public scfImplementation2<
celPath, iCelPath, iComponent>
{
 private:
  iObjectRegistry* object_reg;
  size_t size;
  size_t cur_node;
  csRefArray<iMapNode> nodes;
  
 public:
  celPath (iBase* parent);
  virtual ~celPath ();
  virtual void AddNode (iMapNode* node);
  virtual void InsertNode (size_t pos, iMapNode* node);
  virtual iMapNode* Next ();
  virtual iMapNode* Previous ();
  virtual iMapNode* Current ();
  virtual csVector3 CurrentPosition ();
  virtual iSector* CurrentSector ();
  virtual bool HasNext ();
  virtual bool HasPrevious ();
  virtual void Restart ();
  virtual void Clear ();
  virtual bool Initialize (iObjectRegistry* object_reg);
  virtual iMapNode* GetFirst ();
  virtual iMapNode* GetLast ();
  virtual void Invert ();
};

/**
 * This is a mapnode Graph for CEL.
 */
class celGraph : public scfImplementation2<
  celGraph, iCelGraph, iComponent>
{
private:
  iObjectRegistry* object_reg;
   csRefArray <iCelNode> nodes;
  //csHash<csRef<iCelNode>, csStringBase> nodes;

public: 
  celGraph (iBase* parent);
  virtual ~celGraph ();
  virtual bool Initialize (iObjectRegistry* object_reg);
  virtual void AddNode(iCelNode* node);
  virtual void AddEdge(iCelNode* from, iCelNode* to, bool state);
  virtual bool AddEdgeByNames(const char* from, const char* to, bool state);
  virtual iCelNode* GetClosest(csVector3 position);
  virtual bool ShortestPath(iCelNode* from, iCelNode* goal, iCelPath* path);
  virtual iCelNode* RandomPath(iCelNode* from, int distance, iCelPath* path);
};

#endif //__CEL_TOOLS_CELGRAPH__ 

