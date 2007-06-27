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

#include "cssysdef.h"
#include "cstool/initapp.h"
#include "csutil/objreg.h"
#include "csutil/event.h"
#include "csutil/cfgacc.h"
#include "csutil/inputdef.h"
#include "csutil/array.h"
#include "csutil/priorityqueue.h"
#include "csgeom/math3d.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "iutil/objreg.h"
#include "ivideo/graph3d.h"
#include "ivaria/reporter.h"
#include "ivaria/conin.h"
#include "ivaria/script.h"

#include "tools/expression.h"
#include "plugins/tools/celgraph/celgraph.h"

CS_IMPLEMENT_PLUGIN

//--------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celEdge)

  celEdge::celEdge (iBase* parent)
  : scfImplementationType (this, parent), object_reg(0)
{
  state = true;
}

celEdge::~celEdge ()
{
}

void celEdge::SetState (bool open)
{
  state = open;
}

void celEdge::SetSuccessor (iCelNode* node)
{
  successor = node;
}

bool celEdge:: GetState ()
{
  return state;
}

iCelNode* celEdge::GetSuccessor ()
{
  return successor;
}

bool celEdge::Initialize(iObjectRegistry* object_reg)
{
  celEdge::object_reg = object_reg;

  return true;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celNode)

celNode::celNode (iBase* parent)
  : scfImplementationType (this, parent)
{
}

celNode::~celNode ()
{
}

void celNode:: AddSuccessor(iCelNode* node, bool state)
{
  csRef<iCelEdge> edge;
  edge->SetState(state);
  edge->SetSuccessor(node);
  edges.Push(edge);
}

void celNode:: SetMapNode (iMapNode* node)
{
  map_node = node;
}

void celNode:: SetParent (iCelNode* par)
{
  parent = par;
}


void celNode:: Heuristic (float cost, iCelNode* goal)
{
  heuristic = csSquaredDist::PointPoint(GetPosition(), goal->GetPosition());
  cost = cost;
}

iMapNode* celNode:: GetMapNode ()
{
  return map_node;
}

csVector3 celNode:: GetPosition ()
{
  return map_node->GetPosition();
}

csArray<iCelNode*> celNode:: GetSuccessors ()
{
  csArray<iCelNode*> nodes;
  size_t n = edges.GetSize();

  
  for(size_t i=0; i<n;i++){
    if(edges[i]->GetState())
      nodes.Push(edges[i]->GetSuccessor());
  }

  return nodes;
}

csArray<iCelNode*> celNode:: GetAllSuccessors ()
{
  csArray<iCelNode*> nodes;
  size_t n = edges.GetSize();

  for(size_t i=0; i<n;i++)
    nodes.Push(edges[i]->GetSuccessor());
  
  return nodes;
}

bool celNode::Initialize(iObjectRegistry* object_reg)
{
  celNode::object_reg = object_reg;

  return true;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celPath)

celPath::celPath (iBase* parent)
  : scfImplementationType (this, parent)
{
  size = 0;
  cur_node = 0;
}

celPath::~celPath ()
{
}

void celPath::AddNode (iMapNode* node)
{
  nodes.Push(node);
}

void celPath::InsertNode (size_t pos, iMapNode* node)
{
  nodes.Insert(pos, node);
}

iMapNode* celPath::Next ()
{
  return nodes[cur_node++];
}

iMapNode* celPath::Previous ()
{
  return nodes[--cur_node];
}

bool celPath::HasNext ()
{
  return cur_node < size;
}

bool celPath::HasPrevious ()
{
  return cur_node > 0;
}

bool celPath::Initialize(iObjectRegistry* object_reg)
{
  celPath::object_reg = object_reg;

  return true;
}


//---------------------------------------------------------------------------

template<typename T1, typename T2>
class Comparator
{
public:
  static int Compare (T1 const &n1, T2 const &n2)
  {
    return n2->GetHeuristic()+n2->GetCost() - n1->GetHeuristic()- n1->GetCost();
  }
};

SCF_IMPLEMENT_FACTORY (celGraph)

celGraph::celGraph (iBase* parent)
  : scfImplementationType (this, parent)
{
}

celGraph::~celGraph ()
{
}

bool celGraph::Initialize(iObjectRegistry* object_reg)
{
  celGraph::object_reg = object_reg;
  
  //nodes = csRefArray<iCelNode>::csRefArray(100, 100);
  
  return true;
}

void celGraph:: AddNode(iCelNode* node)
{
  nodes.Push(node);
}

void celGraph:: AddEdge(iCelNode* from, iCelNode* to, bool state)
{
  from->AddSuccessor(to, state);
}

iCelNode* celGraph:: GetClosest (csVector3 position)
{
  size_t n = nodes.GetSize();
  if(n == 0)
    return NULL;

  iCelNode* closest= nodes[0];
  
  float shortest = csSquaredDist::PointPoint(position, closest->GetPosition());
  float distance;

  for(size_t i=1; i<n; i++){
    distance =  csSquaredDist::PointPoint(position, nodes[i]->GetPosition());
    if(distance < shortest){
      shortest = distance;
      closest = nodes[i];
    }
  }

  return closest;
}

iCelPath* celGraph::ShortestPath (iCelNode* from, iCelNode* goal)
{
  csRef<iCelPath> path;
  CS::Utility::PriorityQueue<iCelNode*, csArray<iCelNode*>, Comparator<iCelNode*, iCelNode*> > queue;
 
  from->Heuristic(0, goal);
  queue.Insert(from);

  //Check if the list is empty
  while(!queue.IsEmpty())
    {
      //Choose the node with less cost+h
      iCelNode* current = queue.Pop();

      //Check if we have arrived to our goal      
      if(current == goal){
	while(true){
	  path->AddNode(current->GetMapNode());

	  if(current == from)
	    return path;

	  current = current->GetParent();
	}
      }
      
      //Get successors
      csArray<iCelNode*> suc = current->GetSuccessors();
      for(size_t i=0; i<suc.GetSize(); i++){
	suc[i]->SetParent(current);
	float cost = csSquaredDist::PointPoint(current->GetPosition(), suc[i]->GetPosition());
	suc[i]->Heuristic(current->GetCost()+cost, goal);
	queue.Insert(suc[i]);
      }
    }

  //goal is unreachable from here
  return path;
}
