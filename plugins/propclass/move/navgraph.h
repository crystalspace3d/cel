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

#ifndef __CEL_PF_NAVGRAPHFACT__
#define __CEL_PF_NAVGRAPHFACT__

// CS Stuff
#include "cstypes.h"
#include "csutil/scf.h"

// PcCommon defs
#include "plugins/propclass/common/stdpcimp.h"
#include "propclass/region.h"
#include "propclass/navgraph.h"

// Template
#include "physicallayer/facttmpl.h"

// Declare Version info
class celPcNavGraph;

CEL_DECLARE_FACTORY (NavGraph)

/**
 * This is a Navigation Graph property class.
 * It contains the navigation tree (a sorted list of NavLink and NavNode
 * objects).
 */
class celPcNavGraph : public celPcCommon
{
private:
  // Array of Node entities
  csRefArray<iPcNavNode> aNodes;
  // Array of Link entities
  csRefArray<iPcNavLink> aLinks;
  // Array of Path entities  // TODO: paths
  //csRefArray<iPcNavPath> aPaths;
  // CEL region object (there is one graph for each region)
  csRef<iPcRegion> region;

  // Implementation of all the nav graph routines.
  csRef<iPcNavGraphRules> navgraphrules;

  // Internal graph construction routines.
  int LinkNodes();
  void CreatePaths();
  void ResetLinksPaths();

public:
  // Initilisation / file routines
  void ResetGraph();
  void LoadGraph();
  void SaveGraph();
  int LoadNodesFromRegion( char* regionname );
  int Dump(); // create debug file for graph

  // Graph building and query methods
  int BuildNodeGraph (iSector* sector, iCelEntity* defaultent);
  int FindNearestNode (csVector3* point, iSector* sector, iCelEntity* ent);
  int FindShortestPath (int iNodeStart, int iNodeEnd, int* &ipath);

  // Implement PCCommon methods
  celPcNavGraph (iObjectRegistry* object_reg);
  virtual ~celPcNavGraph ();
  virtual const char* GetName () const { return "pclink"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  // Get / Set access
  csRef<iPcRegion> GetRegion() { return region; }
  void SetRegion (csRef<iPcRegion> newregion) { region = newregion; }

  void SetRules (csRef<iPcNavGraphRules> newrules) { navgraphrules = newrules; }

  int AddNode( iPcNavNode* node ) { return aNodes.Push( node );}
  int RemoveNode( int i) {return aNodes.Delete( i ); }
  iPcNavNode* GetNode( int i ) { return aNodes[i];}
  int GetNodeCount() const { return aNodes.Length();}
  int FindNode( iPcNavNode* Node ) { return aNodes.Find( Node ); }

  int AddLink( iPcNavLink* link ) { return aLinks.Push( link );}
  int RemoveLink( int i) {return aLinks.Delete( i ); }
  iPcNavLink* GetLink( int i ) { return aLinks[i];}
  int GetLinkCount() const {return aLinks.Length();}
  int FindLink( iPcNavLink* Link ) { return aLinks.Find( Link ); }

  struct PcNavGraph : public iPcNavGraph
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcNavGraph);

    virtual void* GetPrivateObject ()
    {
      return (void*)scfParent;
    }

    virtual int Dump ()
    {
      return scfParent->Dump();
    }    

    virtual int FindNearestNode (csVector3* point, iSector* sector,
    	iCelEntity* ent )
    {
      return scfParent->FindNearestNode (point, sector, ent);
    }    
    virtual int BuildNodeGraph (iSector* sector, iCelEntity* defaultent)
    {
      return scfParent->BuildNodeGraph (sector, defaultent);
    }    
    virtual int FindShortestPath (int iNodeStart, int iNodeEnd, int* &ipath)
    {
      return scfParent->FindShortestPath (iNodeStart, iNodeEnd, ipath);
    }

    // Get / Set Nodes
    virtual int AddNode (iPcNavNode* node)
    {
      return scfParent->AddNode (node);
    }    
    virtual int RemoveNode (int i)
    {
      return scfParent->RemoveNode (i);
    }    
    virtual iPcNavNode* GetNode (int i)
    {
      return scfParent->GetNode (i);
    }    
    virtual int FindNode (iPcNavNode* Node)
    {
      return scfParent->FindNode (Node);
    }    
    virtual int GetNodeCount() const
    {
      return scfParent->GetNodeCount();
    }    

    // Get / Set Links
    virtual int AddLink (iPcNavLink* link)
    {
      return scfParent->AddLink (link);
    }    
    virtual int RemoveLink (int i)
    {
      return scfParent->RemoveLink (i);
    }    
    virtual iPcNavLink* GetLink (int i)
    {
      return scfParent->GetLink (i);
    }    
    virtual int FindLink (iPcNavLink* link)
    {
      return scfParent->FindLink (link);
    }    
    virtual int GetLinkCount() const
    {
      return scfParent->GetLinkCount();
    }    

    virtual void SetRegion (csRef<iPcRegion> newregion)
    {
      scfParent->SetRegion (newregion);
    }    

    virtual csRef<iPcRegion> GetRegion() 
    { 
        return scfParent->GetRegion ();
    }

    virtual void SetRules (csRef<iPcNavGraphRules> newrules)
    {
      scfParent->SetRules (newrules);
    }    

  } scfiPcNavGraph;
};


CEL_DECLARE_FACTORY (NavLink)

/**
 * This is a Navigation Link property class.
 *
 * It is used for links between individual nodes (so each pair of nodes will
 * have a link between them).
 */
class celPcNavLink : public celPcCommon
{
private:
  csRef<iPcNavNode> SourceNode; // Starting node for this link
  csRef<iPcNavNode> DestNode;   // End node for this link
  // Length of the link (strictly speaking this is the cost to traverse)
  float fLength;
  // Indicates whether the link is valid (TODO - allow multiple types of
  // information).
  char iLinkData;

  // TODO - make this so we can override to add terrain weights etc.
  void RecalcLength();

public:
  // Implement factory methods
  celPcNavLink (iObjectRegistry* object_reg);
  virtual ~celPcNavLink ();
  virtual const char* GetName () const { return "pclink"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  csRef<iPcNavNode> GetSource() { return SourceNode; }
  void SetSource (csRef<iPcNavNode> newsource )
  {
    SourceNode = newsource;
    RecalcLength();
  }

  csRef<iPcNavNode> GetDest() { return DestNode; }
  void SetDest (csRef<iPcNavNode> newdest)
  {
    DestNode = newdest;
    RecalcLength();
  }

  int GetLinkInfo() { return iLinkData; }
  void SetLinkInfo (int idata) { iLinkData = idata; }
  
  float GetLength() { return fLength; }

  struct PcNavLink : public iPcNavLink
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcNavLink);

    virtual void SetSource (csRef<iPcNavNode> newsource)
    {
      scfParent->SetSource (newsource);
    }    

    virtual csRef<iPcNavNode> GetSource()
    { 
      return scfParent->GetSource( );
    }

    virtual void SetDest (csRef<iPcNavNode> newdest)
    {
      scfParent->SetDest (newdest);
    }    

    virtual csRef<iPcNavNode> GetDest() 
    { 
      return scfParent->GetDest ();
    }

    virtual void SetLinkInfo (int idata)
    {
      scfParent->SetLinkInfo (idata);
    }    

    virtual int GetLinkInfo() 
    { 
      return scfParent->GetLinkInfo ();
    }

    virtual float GetLength() 
    { 
      return scfParent->GetLength ();
    }
  } scfiPcNavLink;
};

CEL_DECLARE_FACTORY (NavNode)

/**
 * This is a Navigation Node property class.
 *
 * It is assigned to individual node objects
 */
class celPcNavNode : public celPcCommon
{

private:
  // All links attached to this node (useful for searching)
  csRefArray<iPcNavLink> aLinks;
  // actual position in space of this node
  csVector3 pos;

public:
  // implement factory methods
  celPcNavNode (iObjectRegistry* object_reg);
  virtual ~celPcNavNode ();
  virtual const char* GetName () const { return "pcnode"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  csVector3 GetPos() { return pos; }
  void SetPos( csVector3 newpos ) { pos.Set( newpos ); }

  int AddLink( iPcNavLink* link ) { return aLinks.Push( link );}
  int RemoveLink( int i) {return aLinks.Delete( i ); }
  iPcNavLink* GetLink( int i ) { return aLinks[i];}
  int GetLinkCount() const {return aLinks.Length();}
  int FindLink( iPcNavLink* Link ) { return aLinks.Find( Link ); }

  char cAIHints;

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  struct PcNavNode : public iPcNavNode
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcNavNode);
  
    virtual void SetPos ( csVector3 newpos )
    {
      scfParent->SetPos( newpos );
    }    

    virtual csVector3 GetPos() 
    { 
      return scfParent->GetPos( );
    }

    // Get / Set Links
    virtual int AddLink( iPcNavLink* link )
    {
      return scfParent->AddLink( link );
    }    
    virtual int RemoveLink( int i)
    {
      return scfParent->RemoveLink( i );
    }    
    virtual iPcNavLink* GetLink( int i )
    {
      return scfParent->GetLink( i );
    }    
    virtual int FindLink( iPcNavLink* link )
    {
      return scfParent->FindLink( link );
    }    
    virtual int GetLinkCount() const
    {
      return scfParent->GetLinkCount();
    }    

  } scfiPcNavNode;
};


CEL_DECLARE_FACTORY (NavGraphRules)

/**
 * This is the base class for all Navigation Rules
 * TODO Is this really necessary to implement as a cel factory object ?
 */
class celPcNavGraphRules : public celPcCommon
{
public:
  // NB. All methods take a graph object as the first parameter.
  // this is so that the methods can work on the whole graph, without the
  // rules object having to maintain state
  virtual int TraverseLink (celPcNavGraph* graph, iSector* sector,
    iPcNavLink* plink, iCelEntity* ent);
  virtual void OptimiseGraph (celPcNavGraph* graph);
  virtual int FindShortestPath (celPcNavGraph* graph, int iNodeStart,
  	int iNodeEnd, int* &ipath);
  virtual int FindNearestNode (celPcNavGraph* graph, csVector3* point,
  	iSector* sector, iCelEntity* ent);

  // implement factory methods
  celPcNavGraphRules (iObjectRegistry* object_reg);
  virtual ~celPcNavGraphRules ();
  virtual const char* GetName () const { return "pcnavgraphrules"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  struct PcNavGraphRules : public iPcNavGraphRules
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcNavGraphRules);

    // Various virtual methods that can be used to change the graph behaviour
    virtual int TraverseLink (iPcNavGraph* graph, iSector* sector,
    	iPcNavLink* plink, iCelEntity* ent)
    {
      return scfParent->TraverseLink (
      	(celPcNavGraph*)(graph->GetPrivateObject ()), sector, plink, ent);
    }

    virtual void OptimiseGraph (iPcNavGraph* graph)
    {
      scfParent->OptimiseGraph ((celPcNavGraph*)(graph->GetPrivateObject ()));
    }

    virtual int FindShortestPath (iPcNavGraph* graph, int iNodeStart,
    	int iNodeEnd, int* &ipath)
    {
      return scfParent->FindShortestPath (
      	(celPcNavGraph*)(graph->GetPrivateObject ()),
	iNodeStart, iNodeEnd, ipath);
    }

    virtual int FindNearestNode (iPcNavGraph* graph, csVector3* point,
    	iSector* sector, iCelEntity* ent)
    {
      return scfParent->FindNearestNode (
      	(celPcNavGraph*)(graph->GetPrivateObject ()), point, sector, ent);
    }

  } scfiPcNavGraphRules;
};


#endif
