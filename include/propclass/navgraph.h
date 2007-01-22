/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Steve Cook

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

#ifndef __CEL_PF_NAVGRAPH__
#define __CEL_PF_NAVGRAPH__

// CS Stuff
#include "cstypes.h"
#include "csutil/scf.h"

struct iPcNavGraph;
struct iPcNavLink;
struct iPcNavNode;
struct iPcNavGraphRules;
struct iSector;

SCF_VERSION (iPcNavGraphRules, 0, 0, 1);

/**
 * Interfaces for the navigation classes
 */
struct iPcNavGraphRules : public iBase
{
  /** 
   * Various virtual methods that can be used to change the graph behaviour.
   * Checks visibility and assigns navigation flags.
   */
  virtual int TraverseLink (iPcNavGraph* graph, iSector* sector,
  	iPcNavLink* plink, iCelEntity* ent ) = 0;

  /** 
   * Various virtual methods that can be used to change the graph behaviour.
   * Sorts the final graph.
   */
  virtual void OptimiseGraph (iPcNavGraph* graph) = 0;

  /** 
   * Various virtual methods that can be used to change the graph behaviour.
   * Calculate the shortest path between two nodes
   */
  virtual size_t FindShortestPath (iPcNavGraph* graph, size_t iNodeStart,
  	size_t iNodeEnd, size_t* &ipath) = 0;

  /** 
   * Various virtual methods that can be used to change the graph behaviour.
   * Find the nearest node to this point.
   */
  virtual size_t FindNearestNode (iPcNavGraph* graph, csVector3* point,
  	iSector* sector, iCelEntity* ent) = 0;
};

SCF_VERSION (iPcNavLink, 0, 0, 1);

/// @@@ Needs comment!
struct iPcNavLink : public iBase
{
  virtual void SetSource (iPcNavNode* newsource) = 0;
  virtual csRef<iPcNavNode> GetSource () = 0;
  virtual void SetDest (iPcNavNode* newdest) = 0;
  virtual csRef<iPcNavNode> GetDest () = 0;
  virtual int GetLinkInfo () = 0;
  virtual void SetLinkInfo (int idata) = 0;
  virtual float GetLength () = 0;
};

SCF_VERSION (iPcNavNode, 0, 0, 1);

/// @@@ Needs comment!
struct iPcNavNode : public iBase
{
  virtual void SetPos ( csVector3 newpos ) = 0;
  virtual csVector3 GetPos() = 0;

  virtual size_t AddLink (iPcNavLink* link) = 0;
  virtual bool RemoveLink (size_t i) = 0;
  virtual iPcNavLink* GetLink (size_t i) = 0;
  virtual size_t GetLinkCount () const = 0;
  virtual size_t FindLink (iPcNavLink* link) = 0;
};

SCF_VERSION (iPcNavGraph, 0, 0, 1);

/// @@@ Needs comment!
struct iPcNavGraph : public iBase
{
  // UGLY @@@
  virtual void* GetPrivateObject () = 0;
  virtual int Dump () = 0;
  virtual int BuildNodeGraph (iSector* sector, iCelEntity* defaultent) = 0;
  virtual void SetRegion (iPcRegion* newregion) = 0;
  virtual csRef<iPcRegion> GetRegion() = 0;

  virtual size_t AddNode (iPcNavNode* node) = 0;
  virtual bool RemoveNode (size_t i) = 0;
  virtual iPcNavNode* GetNode (size_t i) = 0;
  virtual size_t GetNodeCount () const = 0;
  virtual size_t FindNode (iPcNavNode* Node) = 0;

  virtual size_t AddLink (iPcNavLink* link) = 0;
  virtual bool RemoveLink (size_t i) = 0;
  virtual iPcNavLink* GetLink (size_t i) = 0;
  virtual size_t GetLinkCount () const = 0;
  virtual size_t FindLink (iPcNavLink* link) = 0;

  virtual void SetRules (iPcNavGraphRules* newrules) =0;

  virtual size_t FindNearestNode (csVector3* point, iSector* sector,
  	iCelEntity* ent) = 0;
  virtual size_t FindShortestPath (size_t iNodeStart, size_t iNodeEnd, size_t*& ipath) = 0;
};

#endif
