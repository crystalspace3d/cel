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
 * FPS Graph Implementation
 *
 * This is an example implementation of a Graph ruleset for First Person
 * Shooters. It uses A* to find the shortest path between nodes, using the
 * default link array.
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
#include "iutil/virtclk.h"

#include "plugins/propclass/move/movefact.h"
#include "plugins/propclass/move/navgraph.h"
#include "plugins/propclass/move/navgraphrulesfps.h"

// Use debugging function
extern void Report (iObjectRegistry* object_reg, const char* msg, ...);
extern void Notify (iObjectRegistry* object_reg, const char* msg, ...);


/*
 * Priority Queue Class
 * This is an implementation of the standard priority queue used for A*.
 * The class holds a queue of items which it sorts by a priority weighting.
 * When items are inserted or removed, the queue will place / remove it from
 * the correct position in the queue.
 */

// TODO put this in CS or somewhere generic.. might be useful for other things
class celPriorityQueue
{
public:
  celPriorityQueue( int imaxnodes );
  ~celPriorityQueue();
  inline int Empty ( void ) { return ( iqueuesize == 0 ); }
  void Insert( int, float );
  int Remove( float &);

private:
  int	iqueuesize;
  struct iteminfo
  {
    int   Item;
    float Priority;
  } *items;
  void SortDown();
  void SortUp();
};

celPriorityQueue::celPriorityQueue( int imaxitems )
{
  // Allocate enough memory for the queue
  items = (iteminfo*) malloc (imaxitems * sizeof (iteminfo));
  iqueuesize = 0;
}

celPriorityQueue::~celPriorityQueue()
{
  free (items);
}

void celPriorityQueue::Insert (int iValue, float fPriority)
{
  // Add item to end of Queue
  items[iqueuesize].Priority = fPriority;
  items[iqueuesize].Item = iValue;
  iqueuesize++;

  // Sort up the queue to place item in correct position
  SortUp ();
}

int celPriorityQueue :: Remove( float &fPriority )
{
  // Remove the item with the lowest priority (item 0) from the queue
  int iReturn = items[0].Item;
  fPriority = items[0].Priority;

  // Reshuffle the queue so that everything is back in the correct priority
  // order again
  iqueuesize--;
  items[ 0 ] = items[ iqueuesize ];
  SortDown();

  return iReturn;
}

#define ITEM_PARENT( item )    ((item-1)/2)
#define ITEM_GET_LEFT( item )  ((2*item)+1)
#define ITEM_GET_RIGHT( item ) ((2*item)+2)

void celPriorityQueue::SortDown()
{
  struct iteminfo topitem = items[ 0 ];

  // get first child.
  int parent = 0;
  int child = 1;

  // move it down through the queue until it is in the correct priority order
  while (child < iqueuesize)
  {
    // check the right child 
    int rightchild = ITEM_GET_RIGHT(parent);

    if ( (rightchild < iqueuesize) && ( items[ rightchild ].Priority
    	< items[ child ].Priority ))
    {
      child = rightchild;
    }

    if (topitem.Priority <= items[ child ].Priority)
    {
      break; // Item now in correct position so stop sorting
    }
    else
    {
      items[ parent ] = items[ child ];
      parent = child;
      child = ITEM_GET_LEFT(parent);
    }
  }
  items[ parent ] = topitem;
}

#define SWAP_ITEMS( item1, item2 ) \
    {struct iteminfo Tmp = items[ item1 ];\
	items[ item1 ] = items[ item2 ];\
	items[ item2 ] = Tmp;}

void celPriorityQueue::SortUp()
{
  // get last child
  int child = iqueuesize - 1; 

  // move it up through the queue until it is in the correct priority order
  while (child)
  {
    int parent = ITEM_PARENT(child);

    if ( items[ parent ].Priority > items[ child ].Priority )
    {
      SWAP_ITEMS( child, parent ) // swap parent and child in queue
      child = parent;             // now get the next item in the queue
    }
    else
    {
      // child is now in the right place, so stop sorting
      break;
    }
  }
}

/*
 * Implement NavGraphRulesFPS
 * ==========================
 * Implement the CEL factory methods etc. for flexibility.
 */

CEL_IMPLEMENT_FACTORY (NavGraphRulesFPS, "pcgraphrulesfps");
//----------------------------------------------
SCF_IMPLEMENT_IBASE_EXT (celPcNavGraphRulesFPS)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcNavGraphRulesFPS)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcNavGraphRulesFPS::PcNavGraphRulesFPS)
  SCF_IMPLEMENTS_INTERFACE (iPcNavGraphRulesFPS)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcNavGraphRulesFPS::celPcNavGraphRulesFPS (iObjectRegistry* object_reg)
    : celPcNavGraphRules (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcNavGraphRulesFPS);

  previousent = 0;

  DG_TYPE (this, "celPcNavGraphRulesFPS()");
  Notify (object_reg, "Created a Nav Graph Rules FPS object");
}

celPcNavGraphRulesFPS::~celPcNavGraphRulesFPS ()
{
}

csPtr<iCelDataBuffer> celPcNavGraphRulesFPS::Save ()
{
  return 0;
}

bool celPcNavGraphRulesFPS::Load (iCelDataBuffer* databuf)
{
  return true;
}

/*
 * Implementation
 *
 */

int celPcNavGraphRulesFPS::TraverseLink (celPcNavGraph* graph,
	iSector* sector, iPcNavLink* plink, iCelEntity* ent)
{
  /*
   * Check whether the entity can traverse this link
   *
   * Currently uses the celMovableConstraint on the entity to see if it can
   * move along this link or is blocked by another world entity
   */
  int iResult = -1;

  // Check for MovableContraintCD on the entity
  CS_ASSERT( ent != 0);

  // Cache entity and moveableconstraint pointer for speed
  // especially when this is getting called repeatedly from
  // NavGraph->BuildNodeGraph()
  if (previousent == 0 || ent != previousent)
  {
    Notify (object_reg, "TraverseLink: Creating new entity");
    moveconst = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
    	iPcMovableConstraint);
    previousent = ent;
  }

  // TODO Maybe extend the PcMovable class to allow us to test it directly
  // rather than just the movable constraint
  /*
  csRef<iPcMovable> movable = CEL_QUERY_PROPCLASS (ent->GetPropertyClassList (),
  	iPcMovable);

  if (movable)
  {
    // If it exists, run CheckMove() to test
    csVector3 pos;

    //just try to movable->Move() the entity ?

    csVector3 save = movable->GetPosition();
    movable->SetPosition( sector, plink->GetSource()->GetPos()); //Set at start

    iResult = movable->Move( plink->GetDest()->GetPos()
    	- plink->GetSource()->GetPos()); // move along link

    movable->SetPosition( sector, save) // Return to original location

  }
  */

  if (moveconst)
  {
    // If a movableconstraint exists, run CheckMove() to test
    csVector3 pos;

    iResult = moveconst->CheckMove (sector, plink->GetSource()->GetPos(), 
                                    plink->GetDest()->GetPos(), pos);
  }
  else
  {
    Notify (object_reg, ".. NO MOVABLECONSTRAINT on entity!!");
    iResult = CEL_MOVE_SUCCEED;
  }
  return iResult;
}

void celPcNavGraphRulesFPS::OptimiseGraph( celPcNavGraph* graph )
{
  /*
   * Optimise the graph data
   * There are two main optimisations here - 
   * 1) to remove all failed links (ie. when traverselink returned
   *    CEL_MOVE_FAIL with the default entity
   * 2) to remove any unnecessary links that were formed from nodes that are
   *    in a straight line.
   *
   * TODO: also investigate whether sorting the nodes or links would
   * improve performance.
   */

  Notify (object_reg, "NavrulesFPS - OptimiseGraph");

  // Remove all failed links from the graph
  int i=0;
  int count=0;
  while (i < graph->GetLinkCount())
  {
    iPcNavLink* link = graph->GetLink (i);

    if (link == 0)
      break;

    if (link->GetLinkInfo() == CEL_MOVE_FAIL) // cant traverse this link
    {
      iPcNavNode* startnode = link->GetSource();
      iPcNavNode* endnode = link->GetDest();

      // Remove the link from the link list on the start and end nodes
      startnode->RemoveLink (startnode->FindLink (link));
      endnode->RemoveLink (endnode->FindLink( link ));

      // Remove the link from the graph
      graph->RemoveLink (i); 
      // dont increment i here, or we'll miss the next link
      count++; // keep track of number of links removed
    }
    else
    {
      i++; // go to next link
    }
  }

  Notify (object_reg, "Optimising graph: removed %d links", count);

  // Remove all aligned links from the graph
  // ie. if two links lie in the same direction, remove the furthest one

  int inode;
  count = 0;
  for (inode=0;inode < graph->GetNodeCount(); inode++)
  {
    // Get the next node
    iPcNavNode* node = graph->GetNode( inode );

    //Notify( object_reg, "Checking node %d", inode);

    // For each node, test all links against all other links
    int ilink1=0;
    while (ilink1 < node->GetLinkCount())
    {
      int ilink2=0;
      bool startagain = false;
      while (ilink2 < node->GetLinkCount() && !startagain)
      {
        int ilinkcount = node->GetLinkCount();

        if (ilink1 != ilink2) // ignore if its the same link
        {
          // Get vector for link
          iPcNavNode* destnode1 = node->GetLink( ilink1 )->GetDest();
          iPcNavNode* destnode2 = node->GetLink( ilink2 )->GetDest();
	  csVector3 vecnode1 = (destnode1->GetPos() - node->GetPos()).Unit();
          csVector3 vecnode2 = (destnode2->GetPos() - node->GetPos()).Unit();

          //Notify( object_reg, "Checking link %d (nd %d to %d [%f,%f,%f]) vs link %d (nd %d to %d [%f,%f,%f]) of %d links - dp %f", 
          //        ilink1, inode, graph->FindNode(destnode1), vecnode1.x, vecnode1.y, vecnode1.z,
          //        ilink2, inode, graph->FindNode(destnode2), vecnode2.x, vecnode2.y, vecnode2.z,
          //        ilinkcount, vecnode1*vecnode2);

          if (vecnode1*vecnode2 >= 0.999) // Get dot product
          {
            // Links are aligned, so delete furthest link
            int iremovelink = (vecnode1.SquaredNorm() > vecnode2.SquaredNorm())
	    	? ilink1:ilink2;

            //Notify (object_reg, "Comparing link %d (dist %f) vs link %d (dist %f) - removing %d - now %d left", 
            //        ilink1, vecnode1.Norm(), ilink2, vecnode2.Norm(), iremovelink,
            //        node->GetLinkCount());

            int ilinkingraph = graph->FindLink (node->GetLink (iremovelink));
            graph->RemoveLink (ilinkingraph); // Remove from graph
            node->RemoveLink (iremovelink);   // Remove link from this node
            count++; // keep track of number of links removed

            if (iremovelink == ilink2) // if link2 removed, keep going
            {
              ilink2--;
            }
            else                       // otherwise restart link1 search again
            {
              ilink1--;
              startagain = true;
            }
          }
        }
        ilink2++;
      }
      ilink1++;
    }
  }
  Notify (object_reg, "Finished optimising graph: removed %d more links",
  	count);
}

int celPcNavGraphRulesFPS::FindShortestPath (celPcNavGraph* graph,
	int iNodeStart, int iNodeEnd, int* &ipath)
{
  /*
   * Find shortest path between two nodes
   * This uses A* to find the shortest path between two nodes, and returns the
   * path as an integer array with the parameter ipathlength as the length of
   * the path.
   *
   * TODO: speed up!
   * currently takes approx:
   * 35ms to search 3k links, 
   * 80ms to search 5k links
   * 150ms to search 11k links
   * 900ms to search 50k links
   * (on a p3-733)
   */

  float* fclosestsofar;
  int* ipreviousnode;

  int inumnodes;
  int iCurrentNode;
  iPcNavNode* TestNode;
  int iVisitNode;

  float flOurDistance;

  //TODO: fix this for timing tests
  //csRef<iVirtualClock> vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  //vc->Advance ();

  inumnodes = graph->GetNodeCount();
  fclosestsofar = (float*) malloc( inumnodes * sizeof( float) );
  ipreviousnode = (int*) malloc( inumnodes * sizeof( int) );

  // Limit queue size to number of links
  celPriorityQueue queue (graph->GetLinkCount());

  int i;
  for (i = 0; i < inumnodes; i++)
  {
    fclosestsofar[i] = -1;
  }

  fclosestsofar[ iNodeStart ] = 0;
  ipreviousnode[ iNodeStart ] = iNodeStart;// tag this as the origin node
  queue.Insert( iNodeStart, 0.0 );// insert start node 

  while ( !queue.Empty() )
  {
    // now pull a node out of the queue
    float flCurrentDistance;
    // returns node which gives shortest total path
    iCurrentNode = queue.Remove (flCurrentDistance);

    // break as soon as we get to the end of the path
    if (iCurrentNode == iNodeEnd) break;

    iPcNavNode *pCurrentNode = graph->GetNode(iCurrentNode);
		
    // run through all of this node's neighbors
    for (i = 0 ; i < pCurrentNode->GetLinkCount() ; i++)
    {
      // get destination node down this link
      TestNode = pCurrentNode->GetLink (i)->GetDest();
      iVisitNode = graph->FindNode( TestNode ); // find the node #
            
      // drop this link if its impassable
      if (pCurrentNode->GetLink( i )->GetLinkInfo() != CEL_MOVE_SUCCEED)
      {
	continue;
      }

      flOurDistance = flCurrentDistance + (pCurrentNode->GetLink (i)
      	->GetLength());

      if ((fclosestsofar[iVisitNode] < -0.5) ||                // new node
	  (flOurDistance < fclosestsofar[iVisitNode] - 0.001)) // shortens dist
      {
	//Notify (object_reg, "fl dist:%f, visit node: %d cur node: %d",
	// flOurDistance, iVisitNode, iCurrentNode);

	fclosestsofar[iVisitNode] = flOurDistance;
	ipreviousnode[iVisitNode] = iCurrentNode;
	queue.Insert ( iVisitNode, flOurDistance );
      }
    }
  }

  if ( fclosestsofar[iNodeEnd] < -0.5 )
  {
    // Destination is unreachable, no path found.
    return 0;
  }

  // now walk backwards through the ipreviousnode array
  iCurrentNode = iNodeEnd;
  int iNumPathNodes = 1; // dont forget to count the destination
	
  // Get number of nodes on the path
  while (iCurrentNode != iNodeStart)
  {
    iNumPathNodes++;
    iCurrentNode = ipreviousnode[iCurrentNode];
  }

#ifdef CS_DEBUG
//    Notify( object_reg, "Number of nodes on the path: %d", iNumPathNodes);
#endif

  // Get the whole path
  iCurrentNode = iNodeEnd;
  for (i = iNumPathNodes - 1 ; i >= 0 ; i--)
  {
    ipath[i] = iCurrentNode;
    iCurrentNode = ipreviousnode[ iCurrentNode ];
  }

  //vc->Advance ();
  //Notify( object_reg, "Search time: %f", vc->GetElapsedTicks ());
/*
  #ifdef _DEBUG
  // print out position and debugging info
  for ( i = 0; i < iNumPathNodes ; i++ )
  {
    csVector3 pos = graph->GetNode( ipath[i] )->GetPos();
    Notify( object_reg, "Path Node %d: %d (%f, %f, %f)", i, ipath[i], pos.x, pos.y, pos.z );
  }
  #endif
*/
  free (fclosestsofar);
  free (ipreviousnode);

  // return path and number of nodes
  return iNumPathNodes;
}

int celPcNavGraphRulesFPS::FindNearestNode (celPcNavGraph* graph,
	csVector3* point, iSector* sector, iCelEntity* ent)
{
  /*
   * Find nearest node to this point.
   * Run through all the nodes in the graph and find the nearest one, then
   * test that the entity can actually get to this point.
   */

  int iclosestnode, iresult;
  float fclosestdist, fdist;

  csVector3 nodepos;
  csVector3 pointpos;
  csVector3 vpath;
  iPcNavNode* node;

  Notify (object_reg, "NavrulesFPS - FindNearestNode");

  // Get the PL
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY(object_reg, iCelPlLayer);
  if (!pl.IsValid()) return 0; // No Physical Layer. This is bad.
  iCelPropertyClass* pc;

  // Create a new node at the point
  csRef<iCelEntity> tempnode = pl->CreateEntity ();
  pc = pl->CreatePropertyClass (tempnode, "pcnode");
  csRef<iPcNavNode> pcnode = SCF_QUERY_INTERFACE (pc, iPcNavNode);
  pcnode->SetPos (*point);

  // Create a temporary link
  csRef<iCelEntity> templink = pl->CreateEntity ();
  pc = pl->CreatePropertyClass (templink, "pclink");
  csRef<iPcNavLink> pclink = SCF_QUERY_INTERFACE (pc, iPcNavLink);

  iclosestnode = -1;
  fclosestdist = -1;

  int iCount = graph->GetNodeCount();

  // Check all nodes, to find the closest that this entity can traverse
  for (int i=0; i < iCount; i++)
  {
    node = graph->GetNode (i); // Get each node in turn

    // Calculate distance
    nodepos = node->GetPos();
    pointpos = *point;
    vpath = nodepos - pointpos;

    fdist = vpath.SquaredNorm(); // should be bit faster than norm

    // If its the closest "good" node so far
    if ((fclosestdist == -1) || (fdist < fclosestdist))
    {
      // Check to see if theres a direct line of sight

      // Set a link object between the point and this node
      pclink->SetSource (pcnode);
      pclink->SetDest (node);

      // Use traverselink to check whether the given entity can traverse the
      // link.
      iresult = TraverseLink (graph, sector, pclink, ent);
    
      if (iresult == CEL_MOVE_SUCCEED)
      {
        // If OK, set this to the closest node
        iclosestnode = i;
        fclosestdist = fdist;
      }
    }

    //Notify (object_reg, "Node %d - sq. dist: %f, closest sq. dist: %f",
    //i, fdist, fclosestdist);
  }

  return iclosestnode;
}

