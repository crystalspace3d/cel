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

/*
    DESCRIPTION

    This implements the Navigation Graph property classes, which are used to
    provide pathfinding information to allow entities to travel through the
    Crystal Space world.

    CLASS HIERARCHY
                                     <  NavNode
    The class heirarchy is: NavGraph < (NavPath)
                                |    < (NavLink) 
                                |
                          NavGraphRules

    CLASSES

    NavGraph:   Implementation independent logic (managing data, loading
    		saving etc.)
    NavNode:    Defined point in 3d space that is used as a "stepping stone"
    		to build paths
    NavLink:    Single steps along a path
    NavPath:    A complete stored path between two locations

    IMPLEMENTATION

    The user should create the graph entity and assign nodes and a graph rules
    object. Then use the buildgraph methods to create the path and link
    entities. The graph entity can then be queried to request information such
    as:
     * find the shortest path between nodes 
     * find the nearest node to an arbitrary point

    EXTENDING THE FUNCTIONALITY

    The NavGraphRules class implements the sorting and searching algorithms.
    To implement your own context-specific optimisations, create a new
    subclass of NavGraphRules and assign it to the NavGraph.
*/

#include "cssysdef.h"
#include "iutil/object.h"
#include "iutil/objreg.h"
#include "iengine/engine.h"
#include "iengine/region.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "csutil/debug.h"
#include "ivaria/reporter.h"

#include "plugins/propclass/move/navgraph.h"

// Use debugging function from movefact
extern void MoveReport (iObjectRegistry* object_reg, const char* msg, ...);
extern void MoveNotify (iObjectRegistry* object_reg, const char* msg, ...);

/*
 * Implement NavLink
 * =================
 * Implement the CEL factory methods etc. for flexibility.
 */

CEL_IMPLEMENT_FACTORY (NavLink, "pclink");
//----------------------------------------------
SCF_IMPLEMENT_IBASE_EXT (celPcNavLink)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcNavLink)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcNavLink::PcNavLink)
  SCF_IMPLEMENTS_INTERFACE (iPcNavLink)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcNavLink::celPcNavLink (iObjectRegistry* object_reg)
    : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcNavLink);
  DG_TYPE (this, "celPcNavLink()");

  iLinkData = 0;
}

celPcNavLink::~celPcNavLink ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcNavLink);
}

csPtr<iCelDataBuffer> celPcNavLink::Save ()
{
  return 0;
}

bool celPcNavLink::Load (iCelDataBuffer* databuf)
{
  return true;
}

void celPcNavLink::RecalcLength()
{
  // Recalculate the weighting for this path
  csVector3 vPath;

  if (SourceNode != 0 && DestNode != 0) // These might not both be set yet...
  {
    vPath = DestNode->GetPos() - SourceNode->GetPos();
    fLength = vPath.Norm();
  }
  else fLength = 0;
}


/*
 * Implement NavNode
 * =================
 * Implement the CEL factory methods etc. for flexibility.
 */

CEL_IMPLEMENT_FACTORY (NavNode, "pcnode");
//----------------------------------------------
SCF_IMPLEMENT_IBASE_EXT (celPcNavNode)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcNavNode)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcNavNode::PcNavNode)
  SCF_IMPLEMENTS_INTERFACE (iPcNavNode)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcNavNode::celPcNavNode (iObjectRegistry* object_reg)
    : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcNavNode);
  DG_TYPE (this, "celPcNavNode()");

  pos = csVector3(0,0,0);
}

celPcNavNode::~celPcNavNode ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcNavNode);
}

csPtr<iCelDataBuffer> celPcNavNode::Save ()
{
  return 0;
}

bool celPcNavNode::Load (iCelDataBuffer* databuf)
{
  return true;
}

/* 
 * Implement NavGraphRules
 * =======================
 * This is the parent class for the rules implementations
 */
CEL_IMPLEMENT_FACTORY (NavGraphRules, "pcgraphrules");
//----------------------------------------------
SCF_IMPLEMENT_IBASE_EXT (celPcNavGraphRules)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcNavGraphRules)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcNavGraphRules::PcNavGraphRules)
  SCF_IMPLEMENTS_INTERFACE (iPcNavGraphRules)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcNavGraphRules::celPcNavGraphRules (iObjectRegistry* object_reg)
    : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcNavGraphRules);
  DG_TYPE (this, "celPcNavGraphRules()");
  MoveNotify (object_reg, "Created a Nav Graph Rules object");
}

celPcNavGraphRules::~celPcNavGraphRules ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcNavGraphRules);
}

csPtr<iCelDataBuffer> celPcNavGraphRules::Save ()
{
  return 0;
}

bool celPcNavGraphRules::Load (iCelDataBuffer* databuf)
{
  return true;
}


/* 
 * Implement NavGraph
 * ==================
 * This contains all the basic Graph methods that are implementation independent
 */

CEL_IMPLEMENT_FACTORY (NavGraph, "pcgraph");
//----------------------------------------------
SCF_IMPLEMENT_IBASE_EXT (celPcNavGraph)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcNavGraph)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcNavGraph::PcNavGraph)
  SCF_IMPLEMENTS_INTERFACE (iPcNavGraph)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcNavGraph::celPcNavGraph (iObjectRegistry* object_reg)
    : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcNavGraph);
  DG_TYPE (this, "celPcNavGraph()");

  navgraphrules = 0;
  region = 0;

  MoveNotify (object_reg, "Created a Graph object");
}

celPcNavGraph::~celPcNavGraph ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcNavGraph);
}

csPtr<iCelDataBuffer> celPcNavGraph::Save ()
{
  return 0;
}

bool celPcNavGraph::Load (iCelDataBuffer* databuf)
{
  return true;
}

int celPcNavGraph::FindNearestNode (csVector3* point, iSector* sector,
	iCelEntity* ent)
{ 
  /*
   * Find the nearest node to this point
   * Just calls the FindNearestNode method on the current NavGraphRules object
   * (The reason that the implementation is in the Rules class is to allow the
   * rules to use application specific information such as terrain info to
   * reduce the search time)
   */

  CS_ASSERT( navgraphrules != 0);
  return navgraphrules->FindNearestNode (&scfiPcNavGraph, point, sector, ent);
}

int celPcNavGraph::FindShortestPath (int iNodeStart, int iNodeEnd, int* &ipath)
{ 
  /*
   * Find the shortest path between these two nodes
   * Just calls the FindShortestPath method on the current NavGraphRules object
   */

  CS_ASSERT( navgraphrules != 0);
  return navgraphrules->FindShortestPath (&scfiPcNavGraph,
    	iNodeStart, iNodeEnd, ipath);
}

int celPcNavGraph::Dump()  // Debug info
{
  /*
   * Dump debug info on the graph and link/node structures
   * TODO: add an option to display the node, links and paths inside the game
   * world.
   */

  size_t i;

  MoveNotify (object_reg, "Graph Dump ============================",
    aLinks.Length());

  if (navgraphrules)
    MoveNotify (object_reg, "NavGraphRules set");
  else
    MoveNotify (object_reg, "NavGraphRules not set");

  if (region)
    MoveNotify (object_reg, "Region set");
  else
    MoveNotify (object_reg, "Region not set");

  MoveNotify (object_reg, "Link Information: %d Links----------", aLinks.Length());

    
  for (i=0; i < aLinks.Length() ; i++)
  {
    iPcNavLink* link = aLinks[i];

    MoveNotify (object_reg, "link %d - OK? %d, Length: %f, Data %d", i, link != 0, 
                link->GetLength(), link->GetLinkInfo());
  }
    
    
  MoveNotify (object_reg, "Node Information: %d Nodes---------", aNodes.Length());

  for (i=0; i < aNodes.Length() ; i++)
  {
    iPcNavNode* node = aNodes[i];

    csVector3 pos = node->GetPos();
    MoveNotify (object_reg, "Node %d - OK? %d, Pos: %f, %f, %f Links: %d", i,
      node !=0, pos.x, pos.y, pos.z, node->GetLinkCount());
  }

  MoveNotify (object_reg, "END Graph Dump ========================",
    aLinks.Length());

  return 0;
}

int celPcNavGraph::LoadNodesFromRegion( char* regionname )
{
  /*
   * Load all the node entities in this particular region
   * TODO test this..
   */

  csRef<iEngine> engine (CS_QUERY_REGISTRY (object_reg, iEngine));
  CS_ASSERT (engine != 0);

  // Select the region for this graph
  iRegion* cur_region = engine->CreateRegion ( regionname );

  // Query Engine for list of all objects in the current region
  csRef<iObjectIterator> iter = cur_region->QueryObject()->GetIterator();
  CS_ASSERT( iter != 0);

  while (iter->HasNext ())
  {
    iObject* o = iter->Next ();
    MoveNotify (object_reg, "Found an object in this region: %s", o->GetName());

    // Check if object is a PcNavNode type
    csRef<iPcNavNode> navnode = SCF_QUERY_INTERFACE (o, iPcNavNode);
    if (navnode)
    {
      // Add a reference to the NavNode PC to the graph
      aNodes.Push (navnode);
    }
  }

    return 0;
}

int celPcNavGraph::LinkNodes ()
{
  /*
   * Creates an array of all link objects from the list of nodes
   * so there will be two link objects for each adjacent pair of nodes (one
   * in each direction)
   */

  int i,j;
  int numnodes;

  // TODO For now assume that no link entities will be manually created, so
  // that there is no need to search and load link entities from the current
  // region This could be implemented later if it makes sense.

  // Iterate through each pair of Nodes
  iCelPropertyClass* pc;

  numnodes = aNodes.Length();
  MoveNotify (object_reg, "Iterating across %d Nodes", numnodes );

  for (i=0; i < numnodes; i++)
  {
    for (j=0; j < numnodes; j++)
    {
      if (i != j)  // Dont attach the node to itself
      {
        // Create Link entity
        csRef<iCelEntity> link = pl->CreateEntity ();
        link->SetName ("");

        // Assign Link PC and set node info
        pc = pl->CreatePropertyClass (link, "pclink");
        csRef<iPcNavLink> pclink = SCF_QUERY_INTERFACE (pc, iPcNavLink);
                
        pclink->SetSource (aNodes[i]);
        pclink->SetDest (aNodes[j]);

        aLinks.Push (pclink);          // Add the link to the graph
        aNodes[i]->AddLink( pclink );  // Add the link to the source node
        //Notify( object_reg, "Created link between %d, %d dist %f", i, j,
	//  pclink->GetLength() );
      }
    }
  }

  MoveNotify (object_reg, "Finished creating links");

  return 0;
}

void celPcNavGraph::CreatePaths()
{
  /*
   * Create the Path cache, by querying FindShortestPath() for a subset of links
   * TODO Implement paths
   */
}

void celPcNavGraph::ResetGraph()
{
  /*
   * Clear all internal data including nodes
   */
  ResetLinksPaths();

  aNodes.DeleteAll();
  navgraphrules = 0;
  region = 0;
}

void celPcNavGraph::ResetLinksPaths()
{
  /*
   * Clear all the link and path data
   */

  // Clear all links
  aLinks.DeleteAll();
  //aPaths.DeleteAll();  //TODO add paths stuff
}
void celPcNavGraph::LoadGraph()
{
  /*
   * Load graph data from disk
   */

  // TODO loading data
}

void celPcNavGraph::SaveGraph()
{
  /*
   * Save graph data to disk
   */

  // TODO saving data
}

int celPcNavGraph::BuildNodeGraph( iSector* sector, iCelEntity* defaultent )
{
  /*
   * Build the Node graph
   * This calls various internal methods to create the list of nodes,
   * create the links and check links for validity, optimise and cache the data.
   *
   * defaultent should be an entity with a hull size that matches the
   * largest entity that will try to move through the sector.
   */

  size_t i;
  int itraversable;

  ResetLinksPaths();    // Clear all the link and path data

  LinkNodes();        // Create the default set of links

  MoveNotify (object_reg, "Traversing %d links", aLinks.Length());

  // for all links
  for (i=0; i < aLinks.Length(); i++)
  {
    // TODO: Add ability to traverse links with multiple entities and save data
    iPcNavLink* link = aLinks[i]; // Get each link
    itraversable = navgraphrules->TraverseLink (&scfiPcNavGraph,
		sector, link, defaultent); // Try to traverse
    link->SetLinkInfo (itraversable); // Set flag if the link is traverseable
  }

  // Sort the graph or otherwise optimise / clear the information
  navgraphrules->OptimiseGraph (&scfiPcNavGraph);

  //CreatePaths();                    // TODO: Create a cache of paths

  return 0;
}

/*
 * NavGraphRules Base class
 * should never be called directly
 */
int celPcNavGraphRules::TraverseLink (celPcNavGraph* graph, iSector* sector,
	iPcNavLink* plink, iCelEntity* ent)
{
  MoveNotify( object_reg, "Navrules - Traverselink");
  return 0; //@@@???
}

void celPcNavGraphRules::OptimiseGraph (celPcNavGraph* graph)
{
  MoveNotify (object_reg, "Navrules - OptimiseGraph");
}

int celPcNavGraphRules::FindShortestPath (celPcNavGraph* graph,
	int iNodeStart, int iNodeEnd, int* &ipath)
{
  MoveNotify( object_reg, "Navrules - FindShortestPath");
  return 0;
}

int celPcNavGraphRules::FindNearestNode (celPcNavGraph* graph,
	csVector3* point, iSector* sector, iCelEntity* ent)
{
  MoveNotify( object_reg, "Navrules - FindNearestNode");
  return 0; //@@@???
}

