/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
  
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


#include "CelHPF.h"

CS_PLUGIN_NAMESPACE_BEGIN(celNavMesh)
{



/*
 * celHPath
 */

celHPath::celHPath (csHash<csRef<iCelNavMesh>, csPtrKey<iSector>>& navmeshes)
    : scfImplementationType (this), navMeshes(navmeshes)
{
}

celHPath::~celHPath ()
{
}

void celHPath::Initialize(iCelPath* highLevelPath)
{
  hlPath = highLevelPath;

  // Get first two nodes and last two nodes of the high level graph
  hlPath->Invert();
  lastGoal = hlPath->Current();
  lastFrom = hlPath->Next();
  hlPath->Invert();
  hlPath->Restart();  
  firstFrom = hlPath->Current();
  firstGoal = hlPath->Next();

  // Resize low level path array. High level paths always have two nodes for each segment of the
  // path in a sector (entry and exit point).
  llSize = hlPath->GetNodeCount() / 2;
  llPaths.SetMinimalCapacity(llSize);
  currentllPosition = 0;

  // Construct first part of the low level path
  currentSector = firstFrom->GetSector();
  csRef<iCelNavMesh> navMesh = navMeshes.Get(currentSector, 0);
  csRef<iCelNavMeshPath> path = navMesh->ShortestPath(firstFrom->GetPosition(), firstGoal->GetPosition());
  llPaths[0] = path;

  // Set current node
  csRef<iMapNode> currentNode;
  currentNode.AttachNew(new csMapNode(""));
  currentNode->SetSector(currentSector);
  csVector3 currentPosition;
  path->Current(currentPosition);
  currentNode->SetPosition(currentPosition);
  this->currentNode = currentNode;
}

iObject* celHPath::QueryObject ()
{
  return this;
}

// TODO implement
void celHPath::AddNode (iMapNode* node)
{
  // TODO add node which causes a sector switch?
}

// TODO implement
void celHPath::InsertNode (size_t pos, iMapNode* node)
{
}

iMapNode* celHPath::Next ()
{
  csVector3 position;
  
  if (!llPaths[currentllPosition]->HasNext())
  {
    if (currentllPosition + 1 >= llSize)
    {
      return 0;
    }

    if (!llPaths[currentllPosition + 1].IsValid())
    {
      csRef<iMapNode> from = hlPath->Next();
      csRef<iMapNode> goal = hlPath->Next();
      currentSector = from->GetSector();
      csRef<iCelNavMesh> navMesh = navMeshes.Get(currentSector, 0);
      csRef<iCelNavMeshPath> path = navMesh->ShortestPath(from->GetPosition(), goal->GetPosition());
      currentllPosition++;
      llPaths[currentllPosition] = path;
    }
    else
    {
      currentllPosition++;
    }
  }
  llPaths[currentllPosition]->Next(position); // We don't use the first point of each path (except for the first path)

  csRef<iMapNode> node;
  node.AttachNew(new csMapNode(""));
  node->SetPosition(position);
  node->SetSector(currentSector);
  currentNode = node;

  return node;
}

iMapNode* celHPath::Previous ()
{
  csVector3 position;
  bool changellPath = !llPaths[currentllPosition]->HasPrevious();

  if (!changellPath)
  {
    llPaths[currentllPosition]->Previous(position); 
    if (llPaths[currentllPosition]->HasPrevious() && currentllPosition > 0)
    {
      changellPath = true; // We don't use the first point of each path (except for the first path)
    }
  }

  if (changellPath)
  {
    if (currentllPosition <= 0)
    {
      return 0;
    }

    if (llPaths[currentllPosition - 1].IsValid())
    {
      currentllPosition--;
    }
    else
    {
      // This should only be reached if a user inverts a path after having read a few steps with Next(), and
      // doesn't restart the path. In practice, this will probably never happen.
      csRef<iMapNode> goal = hlPath->Previous();
      csRef<iMapNode> from = hlPath->Previous();
      currentSector = from->GetSector();
      csRef<iCelNavMesh> navMesh = navMeshes.Get(currentSector, 0);
      csRef<iCelNavMeshPath> path = navMesh->ShortestPath(from->GetPosition(), goal->GetPosition());
      while (path->HasNext())
      {
        path->Next(position);
      }
      currentllPosition--;
      llPaths[currentllPosition] = path;
    }
    
    llPaths[currentllPosition]->Current(position); // Always use the last point of a path
  }

  csRef<iMapNode> node;
  node.AttachNew(new csMapNode(""));
  node->SetPosition(position);
  node->SetSector(currentSector);
  currentNode = node;

  return node;
}

iMapNode* celHPath::Current ()
{
  return currentNode;
}

csVector3 celHPath::CurrentPosition ()
{
  return currentNode->GetPosition();
}

iSector* celHPath::CurrentSector ()
{
  return currentNode->GetSector();
}

bool celHPath::HasNext ()
{
  if (!llPaths[currentllPosition]->HasNext())
  {
    if (currentllPosition + 1 >= llSize)
    {
      return false;
    }
  }

  return true;
}

bool celHPath::HasPrevious ()
{
  if (!llPaths[currentllPosition]->HasPrevious())
  {
    if (currentllPosition <= 0)
    {
      return false;
    }
  }

  return true;
}

void celHPath::Restart ()
{
  for (size_t i = 0; i < llSize; i++)
  {
    if (llPaths[i].IsValid())
    {
      llPaths[i]->Restart();
    }
  }
  currentllPosition = 0;
  hlPath->Restart();

  // Set current node
  csVector3 position;
  currentSector = hlPath->Next()->GetSector();
  if (!llPaths[0].IsValid())
  {
    // This should be reached if the user inverts the path and then restarts it, for example
    currentSector = firstFrom->GetSector();
    csRef<iCelNavMesh> navMesh = navMeshes.Get(currentSector, 0);
    csRef<iCelNavMeshPath> path = navMesh->ShortestPath(firstFrom->GetPosition(), firstGoal->GetPosition());
    llPaths[0] = path;    
  }
  llPaths[0]->Current(position);
  csRef<iMapNode> node;
  node.AttachNew(new csMapNode(""));
  node->SetPosition(position);
  node->SetSector(currentSector);
  currentNode = node;
}

void celHPath::Clear ()
{
  for (size_t i = 0; i < llSize; i++)
  {
    llPaths[i].Invalidate();
  }
  llSize = 0;
  hlPath->Clear();
  currentllPosition = 0;
  currentNode = 0;
  currentSector = 0;
}

iMapNode* celHPath::GetFirst ()
{
  csVector3 position;
  if (!llPaths[0].IsValid())
  {
    // This should be reached if the user inverts the path and then asks for the first element, for example
    csPtrKey<iSector> currentSector = firstFrom->GetSector(); // Can't write over celHPath::currentSector
    csRef<iCelNavMesh> navMesh = navMeshes.Get(currentSector, 0);
    csRef<iCelNavMeshPath> path = navMesh->ShortestPath(firstFrom->GetPosition(), firstGoal->GetPosition());
    llPaths[0] = path;    
  }
  llPaths[0]->GetFirst(position);

  csRef<iMapNode> node;
  node.AttachNew(new csMapNode(""));
  node->SetPosition(position);
  node->SetSector(firstFrom->GetSector());
  tmpNode = node; // Can't write over celHPath::currentNode
  
  return node;
}

iMapNode* celHPath::GetLast ()
{
  csVector3 position;
  size_t index = llSize - 1;
  if (!llPaths[index].IsValid())
  {
    csPtrKey<iSector> currentSector = lastFrom->GetSector(); // Can't write over celHPath::currentSector
    csRef<iCelNavMesh> navMesh = navMeshes.Get(currentSector, 0);
    csRef<iCelNavMeshPath> path = navMesh->ShortestPath(lastFrom->GetPosition(), lastGoal->GetPosition());
    llPaths[index] = path;    
  }
  llPaths[index]->GetLast(position);

  csRef<iMapNode> node;
  node.AttachNew(new csMapNode(""));
  node->SetPosition(position);
  node->SetSector(lastFrom->GetSector());
  tmpNode = node; // Can't write over celHPath::currentNode
  
  return node;
}

void celHPath::Invert ()
{
  for (size_t i = 0; i < llSize; i++)
  {
    if (llPaths[i].IsValid())
    {
      llPaths[i]->Invert();
    }
  }
  hlPath->Invert();

  csRef<iMapNode> tmp = lastGoal;
  lastGoal = firstFrom;
  firstFrom = tmp;

  tmp = lastFrom;
  lastFrom = firstGoal;
  firstGoal = tmp;
}

// TODO implement
size_t celHPath::GetNodeCount()
{
  size_t size = 0;

  return size;
}



/*
 * celHNavStruct
 */

celHNavStruct::celHNavStruct (const iCelNavMeshParams* params) : scfImplementationType (this)
{
  parameters.AttachNew(params->Clone());
}

celHNavStruct::~celHNavStruct ()
{
}

void celHNavStruct::AddNavMesh(iCelNavMesh* navMesh)
{
  csPtrKey<iSector> key = navMesh->GetSector();
  navMeshes.Put(key, navMesh);
}

bool celHNavStruct::BuildHighLevelGraph()
{
  hlGraph.Invalidate();
  hlGraph = scfCreateInstance<iCelGraph>("cel.celgraph");
  if (!hlGraph)
  {
    return false;
  }

  csHash<csRef<iCelNode>, csPtrKey<iSector>> nodes;

  int x = 0;
  csHash<csRef<iCelNavMesh>, csPtrKey<iSector>>::GlobalIterator it = navMeshes.GetIterator();
  while (it.HasNext())
  {
    csPtrKey<iSector> sector;
    it.Next(sector);
    csSet<csPtrKey<iMeshWrapper>>::GlobalIterator portalMeshesIt = sector->GetPortalMeshes().GetIterator();
    while (portalMeshesIt.HasNext())
    {
      csRef<iPortalContainer> container = portalMeshesIt.Next()->GetPortalContainer();
      int size = container->GetPortalCount();
      for (int i = 0; i < size; i++)
      {
        csRef<iPortal> portal = container->GetPortal(i);
        portal->CompleteSector(0);
        csFlags flags = portal->GetFlags();

        // Get portal indices
        int indicesSize = portal->GetVertexIndicesCount();

        // Get portal vertices
        int verticesSize = portal->GetVerticesCount();
        const csVector3* verticesPortal = portal->GetWorldVertices();

        if (indicesSize >= 3)
        {
          // Calculate portal's bounding box
          csVector3 boundingMin = verticesPortal[0];
          csVector3 boundingMax = verticesPortal[0];
          for (int j = 1; j < verticesSize; j++)
          {
            for (int k = 0; k < 3; k++)
            {
              if (verticesPortal[j][k] < boundingMin[k])
              {
                boundingMin[k] = verticesPortal[j][k];
              }
              if (verticesPortal[j][k] > boundingMax[k])
              {
                boundingMax[k] = verticesPortal[j][k];
              }
            }
          }
          
          // Add the portal bounding box's center as a node in the current sector to the graph
          csVector3 center(((boundingMin[0] + boundingMax[0]) / 2), ((boundingMin[1] + boundingMax[1]) / 2),
              ((boundingMin[2] + boundingMax[2]) / 2));
          csRef<iMapNode> mapNode;
          mapNode.AttachNew(new csMapNode("n")); // TODO set name?
          mapNode->SetPosition(center);
          mapNode->SetSector(sector);
          size_t nodeIndex;
          csRef<iCelNode> node = hlGraph->CreateEmptyNode(nodeIndex);
          node->SetName(""); // TODO set name?
          node->SetMapNode(mapNode);

          // Add the portal bounding box's center as a node in the destination sector to the graph
          // Warp if necessary
          csVector3 center2 = center;
          if (flags.Check(CS_PORTAL_WARP))
          {
            csReversibleTransform warp = portal->GetWarp();
            center2 = warp.This2Other(center);
          }
          csRef<iMapNode> mapNode2;
          mapNode2.AttachNew(new csMapNode("n")); // TODO set name?
          mapNode2->SetPosition(center2);
          mapNode2->SetSector(portal->GetSector());
          csRef<iCelNode> node2 = hlGraph->CreateEmptyNode(nodeIndex);
          node2->SetName(""); // TODO set name?
          node2->SetMapNode(mapNode2);

          // Add an edge between the two points
          hlGraph->AddEdge(node, node2, true, 0.0f);

          nodes.Put(sector, node);
          nodes.Put(portal->GetSector(), node2);
        }
      }
    }
    x++;
  }

  // Create edges between nodes from the same sector
  csHash<csRef<iCelNavMesh>, csPtrKey<iSector>>::GlobalIterator it2 = navMeshes.GetIterator();
  while (it2.HasNext())
  {
    csPtrKey<iSector> sector;
    csRef<iCelNavMesh> navMesh = it2.Next(sector);
    if (!navMesh)
    {
      return false;
    }
    csArray<csRef<iCelNode>> sectorNodes = nodes.GetAll(sector);
    int size = sectorNodes.GetSize();
    for (int i = 0; i < size - 1; i++)
    {
      csRef<iCelNode> node1 = sectorNodes[i];
      for (int j = i + 1; j < size; j++)
      {
        csRef<iCelNode> node2 = sectorNodes[j];
        csRef<iCelNavMeshPath> path = navMesh->ShortestPath(node1->GetPosition(), node2->GetPosition(), 256);
        float length = path->Length();
        hlGraph->AddEdge(node1, node2, true, length);
        hlGraph->AddEdge(node2, node1, true, length);
      }        
    }
  }

  return true;
}

iCelPath* celHNavStruct::ShortestPath (const csVector3& from, iSector* fromSector, const csVector3& goal,
    iSector* goalSector)
{
  csRef<iMapNode> fromNode;
  fromNode.AttachNew(new csMapNode("n")); // TODO set name?
  fromNode->SetPosition(from);
  fromNode->SetSector(fromSector);

  csRef<iMapNode> goalNode;
  goalNode.AttachNew(new csMapNode("n")); // TODO set name?
  goalNode->SetPosition(goal);
  goalNode->SetSector(goalSector);

  return ShortestPath(fromNode, goalNode);
}

iCelPath* celHNavStruct::ShortestPath (iMapNode* from, iMapNode* goal)
{
  // Add from and goal nodes to the high level graph
  size_t fromNodeIdx, goalNodeIdx;
  csRef<iCelNode> fromNode = hlGraph->CreateEmptyNode(fromNodeIdx); // TODO set name?
  csRef<iCelNode> goalNode = hlGraph->CreateEmptyNode(goalNodeIdx); // TODO set name?
  fromNode->SetMapNode(from);
  goalNode->SetMapNode(goal);

  // Create edges between from and goal nodes and all portals from their respective sectors
  csPtrKey<iSector> fromSector = from->GetSector();
  csPtrKey<iSector> goalSector = goal->GetSector();  
  csRef<iCelNavMesh> fromNavMesh = navMeshes.Get(fromSector, 0);
  csRef<iCelNavMesh> goalNavMesh = navMeshes.Get(goalSector, 0);  
  if (!fromNavMesh || !goalNavMesh)
  {
    hlGraph->RemoveNode(fromNodeIdx);
    hlGraph->RemoveNode(goalNodeIdx);
    return 0;
  }
  csList<size_t> tmpEdges; // Only for edges from some node to goal node
  size_t size = hlGraph->GetNodeCount();
  for (size_t i = 0; i < size; i++)
  {
    if (i != fromNodeIdx && i != goalNodeIdx)
    {
      csRef<iCelNode> node = hlGraph->GetNode(i);
      csRef<iMapNode> mapNode = node->GetMapNode();
      csRef<iSector> sector = mapNode->GetSector();
      // From node
      if (sector->QueryObject()->GetID() == fromSector->QueryObject()->GetID())
      {
        csRef<iCelNavMeshPath> tmpPath;
        tmpPath.AttachNew(fromNavMesh->ShortestPath(from->GetPosition(), mapNode->GetPosition()));
        // TODO check if path is reachable
        hlGraph->AddEdge(fromNode, node, true, tmpPath->Length());
      }
      // Goal node
      if (sector->QueryObject()->GetID() == goalSector->QueryObject()->GetID())
      {
        csRef<iCelNavMeshPath> tmpPath;
        tmpPath.AttachNew(goalNavMesh->ShortestPath(mapNode->GetPosition(), goal->GetPosition()));
        // TODO check if path is reachable
        tmpEdges.PushBack(hlGraph->AddEdge(node, goalNode, true, tmpPath->Length()));
      }
    }
  }

  // Find path in high level graph
  csRef<iCelPath> hlPath = scfCreateInstance<iCelPath>("cel.celpath");
  if (!hlPath)
  {
    csList<size_t>::Iterator it(tmpEdges);
    while (it.HasNext())
    {
      hlGraph->RemoveEdge(goalNode, it.Next());
    }
    hlGraph->RemoveNode(fromNodeIdx);
    hlGraph->RemoveNode(goalNodeIdx);
    return 0;
  }
  hlGraph->ShortestPath2(fromNode, goalNode, hlPath);

  // Remove edges that connect any node to the goal node
  csList<size_t>::Iterator it(tmpEdges);
  while (it.HasNext())
  {
    hlGraph->RemoveEdge(goalNode, it.Next());
  }

  // Remove from and goal nodes from the high level graph (and the edges we added and didn't explicitly remove)
  hlGraph->RemoveNode(fromNodeIdx);
  hlGraph->RemoveNode(goalNodeIdx);
  
  // Initialize path
  csRef<celHPath> path = new celHPath(navMeshes);
  path->Initialize(hlPath);

  return path;
}

// TODO implement
bool celHNavStruct::SaveToFile (const csString& file)
{
  return false;
}

// TODO implement
bool celHNavStruct::LoadFromFile (const csString& file)
{
  return false;
}

const iCelNavMeshParams* celHNavStruct::GetNavMeshParams () const
{
  return parameters;
}

void celHNavStruct::DebugRender ()
{
  csHash<csRef<iCelNavMesh>, csPtrKey<iSector> >::GlobalIterator it = navMeshes.GetIterator();
  while (it.HasNext())
  {
    csRef<iCelNavMesh> navMesh = it.Next();
    navMesh->DebugRender();
  }
}

void celHNavStruct::DebugRenderAgent(const csVector3& pos, int red, int green, int blue, int alpha) const
{
  csHash<csRef<iCelNavMesh>, csPtrKey<iSector> >::ConstGlobalIterator it = navMeshes.GetIterator();
  if (it.HasNext())
  {
    csRef<iCelNavMesh> navMesh = it.Next();
    navMesh->DebugRenderAgent(pos, red, green, blue, alpha);
  }
}



/*
 * celHNavMeshBuilder
 */

SCF_IMPLEMENT_FACTORY (celHNavStructBuilder)

celHNavStructBuilder::celHNavStructBuilder (iBase* parent) : scfImplementationType (this, parent)
{
  sectors = 0;
  parameters.AttachNew(new celNavMeshParams());
}

celHNavStructBuilder::~celHNavStructBuilder ()
{
  delete sectors;
}

bool celHNavStructBuilder::Initialize (iObjectRegistry* objectRegistry) 
{
  this->objectRegistry = objectRegistry;
  return true;
}

void celHNavStructBuilder::SetSectors (csList<iSector*> sectorList)
{
  delete sectors;
  sectors = new csList<iSector*>(sectorList);
  builders.Empty();
  InstantiateNavMeshBuilders();
}

bool celHNavStructBuilder::InstantiateNavMeshBuilders ()
{
  csList<iSector*>::Iterator it(*sectors);
  while (it.HasNext())
  {
    csPtrKey<iSector> key = it.Next();
    csRef<iCelNavMeshBuilder> builder = csLoadPlugin<iCelNavMeshBuilder>(objectRegistry, "cel.navmeshbuilder");
    if (!builder)
    {
      return false;
    }
    builder->SetNavMeshParams(parameters);
    builder->SetSector(key);
    builders.Put(key, builder);
  }
  return true;
}

iCelHNavStruct* celHNavStructBuilder::BuildHNavStruct ()
{
  if (!sectors)
  {
    return 0;
  }

  celHNavStruct* navStruct = new celHNavStruct(parameters);

  csHash<csRef<iCelNavMeshBuilder>, csPtrKey<iSector>>::GlobalIterator it = builders.GetIterator();
  while (it.HasNext())
  {
    csRef<iCelNavMeshBuilder> builder = it.Next();
    iCelNavMesh* navMesh = builder->BuildNavMesh();
    navStruct->AddNavMesh(navMesh);
  }
  navStruct->BuildHighLevelGraph();

  return navStruct;
}

// TODO implement
bool celHNavStructBuilder::UpdateNavMesh (iCelHNavStruct* hNavStruct, const csBox3& boundingBox, iSector* sector)
{
  if (sector) 
  {

  }
  return false;
}

// TODO implement
bool celHNavStructBuilder::UpdateNavMesh (iCelHNavStruct* hNavStruct, const csOBB& boundingBox, iSector* sector)
{
  if (sector)
  {

  }
  return false;
}


const iCelNavMeshParams* celHNavStructBuilder::GetNavMeshParams () const
{
  return parameters;
}

void celHNavStructBuilder::SetNavMeshParams (const iCelNavMeshParams* parameters)
{
  this->parameters.Invalidate();
  this->parameters.AttachNew(new celNavMeshParams(parameters));
}

} CS_PLUGIN_NAMESPACE_END(celNavMesh)
