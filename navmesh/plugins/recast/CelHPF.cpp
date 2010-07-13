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

celHPath::celHPath (csHash<csRef<iCelNavMesh>, csPtrKey<iSector> >& navmeshes)
    : scfImplementationType (this), navMeshes(navmeshes)
{
  reverse = false;
}

celHPath::~celHPath ()
{
}

void celHPath::Initialize(iCelPath* highLevelPath)
{
  hlPath = highLevelPath;

  // Get first and last node of the high level graph
  firstNode = hlPath->GetFirst();
  lastNode = hlPath->GetLast();

  // Resize low level path array. High level paths always have two nodes for each segment of the
  // path in a sector (entry and exit point).
  size_t llSize = hlPath->GetNodeCount() / 2;
  llPaths.SetSize(llSize);
  currentllPosition = 0;

  // Construct first part of the low level path
  csRef<iMapNode> goal = hlPath->Next();
  currentSector = firstNode->GetSector();
  csRef<iCelNavMesh> navMesh = navMeshes.Get(currentSector, 0);
  csRef<iCelNavMeshPath> path = navMesh->ShortestPath(firstNode->GetPosition(), goal->GetPosition());
  llPaths[0] = path;

  // Set current node
  currentNode = firstNode;
}

bool celHPath::HasNextInternal ()
{
  if (!llPaths[currentllPosition]->HasNext())
  {
    if (currentllPosition + 1 >= llPaths.GetSize())
    {
      return false;
    }
  }

  return true;
}

bool celHPath::HasPreviousInternal ()
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

bool celHPath::HasNext ()
{
  if (reverse)
  {
    return HasPreviousInternal();
  }
  return HasNextInternal();
}

bool celHPath::HasPrevious ()
{
  if (reverse)
  {
    return HasNextInternal();
  }
  return HasPreviousInternal();
}

iMapNode* celHPath::NextInternal ()
{
  csVector3 position;
  
  if (!llPaths[currentllPosition]->HasNext())
  {
    if (currentllPosition + 1 >= llPaths.GetSize())
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

iMapNode* celHPath::PreviousInternal ()
{
  csVector3 position;
  bool changellPath = !llPaths[currentllPosition]->HasPrevious();

  if (!changellPath)
  {
    llPaths[currentllPosition]->Previous(position); 
    if (!llPaths[currentllPosition]->HasPrevious() && currentllPosition > 0)
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

iMapNode* celHPath::Next ()
{
  if (reverse)
  {
    return PreviousInternal();
  }
  return NextInternal();
}

iMapNode* celHPath::Previous ()
{
  if (reverse)
  {
    return NextInternal();
  }
  return PreviousInternal();
}

iMapNode* celHPath::Current ()
{
  return currentNode;
}

iMapNode* celHPath::GetFirst ()
{
  if (reverse)
  {
    return lastNode;
  }
  return firstNode;
}

iMapNode* celHPath::GetLast ()
{
  if (reverse)
  {
    return firstNode;
  }
  return lastNode;
}

void celHPath::Invert ()
{
  reverse = reverse ? false : true;
}

void celHPath::Restart ()
{
  if (reverse)
  {
    while (HasNextInternal())
    {
      NextInternal();
    }
  }
  else
  {
    for (size_t i = 0; i < llPaths.GetSize(); i++)
    {
      if (llPaths[i].IsValid())
      {
        llPaths[i]->Restart();
      }
    }
    currentllPosition = 0;
    hlPath->Restart();

    // Set current node
    csRef<iMapNode> goal = hlPath->Next();
    currentSector = firstNode->GetSector();
    if (!llPaths[0].IsValid())
    {
      // This should be reached if the user inverts the path and then restarts it, for example
      csRef<iCelNavMesh> navMesh = navMeshes.Get(currentSector, 0);
      csRef<iCelNavMeshPath> path = navMesh->ShortestPath(firstNode->GetPosition(), goal->GetPosition());
      llPaths[0] = path;    
    }

    currentNode = firstNode;
  }
}

void celHPath::DebugRender ()
{
  int backCount = 0;
  while (HasPrevious())
  {
    Previous();
    backCount++;
  }

  csRef<iMapNode> current = Current();
  csRef<iMapNode> previous = Current();
  csVector3 currentPosition = current->GetPosition();
  csVector3 previousPosition = previous->GetPosition();  

  DebugDrawGL dd;
  dd.depthMask(false);

  const unsigned int lineCol = duRGBA(255, 255, 255, 230);
  const unsigned int vertexCol = duRGBA(255, 150, 0, 230);
  dd.begin(DU_DRAW_LINES, 4.0f);
  while (HasNext())
  {
    previous = current;
    previousPosition = currentPosition;
    current = Next();
    currentPosition = current->GetPosition();
    
    dd.vertex(previousPosition[0], previousPosition[1], previousPosition[2], lineCol);
    dd.vertex(currentPosition[0], currentPosition[1], currentPosition[2], lineCol);
  }
  dd.end();

  current = Current();
  currentPosition = current->GetPosition();
  Restart();
  dd.begin(DU_DRAW_POINTS, 10.0f);
  while (HasNext())
  {
    current = Next();
    currentPosition = current->GetPosition();
    dd.vertex(currentPosition[0], currentPosition[1], currentPosition[2], vertexCol);
  }
  dd.end();

  dd.depthMask(true);
  Restart();
  for (int i = 0; i < backCount; i++)
  {
    Next();
  }
}



/*
 * celHNavStruct
 */

celHNavStruct::celHNavStruct (const iCelNavMeshParams* params, iObjectRegistry* objectRegistry) : scfImplementationType (this)
{
  this->objectRegistry = objectRegistry;
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

  csHash<csRef<iCelNode>, csPtrKey<iSector> > nodes;

  int x = 0;
  csHash<csRef<iCelNavMesh>, csPtrKey<iSector> >::GlobalIterator it = navMeshes.GetIterator();
  while (it.HasNext())
  {
    csPtrKey<iSector> sector;
    it.Next(sector);
    csSet<csPtrKey<iMeshWrapper> >::GlobalIterator portalMeshesIt = sector->GetPortalMeshes().GetIterator();
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
  csHash<csRef<iCelNavMesh>, csPtrKey<iSector> >::GlobalIterator it2 = navMeshes.GetIterator();
  while (it2.HasNext())
  {
    csPtrKey<iSector> sector;
    csRef<iCelNavMesh> navMesh = it2.Next(sector);
    if (!navMesh)
    {
      return false;
    }
    csArray<csRef<iCelNode> > sectorNodes = nodes.GetAll(sector);
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

void celHNavStruct::SetHighLevelGraph(iCelGraph* graph)
{
  hlGraph = graph;
}

iCelHPath* celHNavStruct::ShortestPath (const csVector3& from, iSector* fromSector, const csVector3& goal,
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

iCelHPath* celHNavStruct::ShortestPath (iMapNode* from, iMapNode* goal)
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
        csRef<iCelNavMeshPath> tmpPath = fromNavMesh->ShortestPath(from->GetPosition(), mapNode->GetPosition());
        // TODO check if path is reachable
        hlGraph->AddEdge(fromNode, node, true, tmpPath->Length());
      }
      // Goal node
      if (sector->QueryObject()->GetID() == goalSector->QueryObject()->GetID())
      {
        csRef<iCelNavMeshPath> tmpPath = goalNavMesh->ShortestPath(mapNode->GetPosition(), goal->GetPosition());
        // TODO check if path is reachable
        tmpEdges.PushBack(hlGraph->AddEdge(node, goalNode, true, tmpPath->Length()));
      }
    }
  }
  if (fromSector->QueryObject()->GetID() == goalSector->QueryObject()->GetID())
  {
    csRef<iCelNavMeshPath> tmpPath = goalNavMesh->ShortestPath(from->GetPosition(), goal->GetPosition());
    // TODO check if path is reachable
    hlGraph->AddEdge(fromNode, goalNode, true, tmpPath->Length());
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
  path = new celHPath(navMeshes);
  path->Initialize(hlPath);

  return path;
}

void celHNavStruct::SaveParameters (iDocumentNode* node)
{
  csRef<iDocumentNode> param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("agentheight");
  param->SetAttributeAsFloat("value", parameters->GetAgentHeight());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("agentradius");
  param->SetAttributeAsFloat("value", parameters->GetAgentRadius());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("agentmaxslopeangle");
  param->SetAttributeAsFloat("value", parameters->GetAgentMaxSlopeAngle());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("agentmaxclimb");
  param->SetAttributeAsFloat("value", parameters->GetAgentMaxClimb());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("cellsize");
  param->SetAttributeAsFloat("value", parameters->GetCellSize());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("cellheight");
  param->SetAttributeAsFloat("value", parameters->GetCellHeight());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("maxsimplificationerror");
  param->SetAttributeAsFloat("value", parameters->GetMaxSimplificationError());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("detailsampledist");
  param->SetAttributeAsFloat("value", parameters->GetDetailSampleDist());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("detailsamplemaxerror");
  param->SetAttributeAsFloat("value", parameters->GetDetailSampleMaxError());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("maxedgelength");
  param->SetAttributeAsInt("value", parameters->GetMaxEdgeLength());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("minregionsize");
  param->SetAttributeAsInt("value", parameters->GetMinRegionSize());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("mergeregionsize");
  param->SetAttributeAsInt("value", parameters->GetMergeRegionSize());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("maxvertsperpoly");
  param->SetAttributeAsInt("value", parameters->GetMaxVertsPerPoly());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("tilesize");
  param->SetAttributeAsInt("value", parameters->GetTileSize());

  param = node->CreateNodeBefore(CS_NODE_ELEMENT);
  param->SetValue("bordersize");
  param->SetAttributeAsInt("value", parameters->GetBorderSize());
}

void celHNavStruct::SaveNavMeshes (iDocumentNode* node, iVFS* vfs)
{
  csHash<csRef<iCelNavMesh>, csPtrKey<iSector> >::GlobalIterator navMeshIt = navMeshes.GetIterator();
  csPtrKey<iSector> key;
  int i = 1;
  csString fileName;
  while (navMeshIt.HasNext())
  {
    csRef<iCelNavMesh> navMesh = navMeshIt.Next(key);
    csRef<iDocumentNode> navMeshNode = node->CreateNodeBefore(CS_NODE_ELEMENT);
    navMeshNode->SetValue("navmesh");
    navMeshNode->SetAttributeAsInt("id", i);
    navMeshNode->SetAttribute("sector", key->QueryObject()->GetName());
    fileName = i;
    csRef<iFile> meshFile = vfs->Open(fileName.GetDataSafe(), VFS_FILE_WRITE);
    navMesh->SaveToFile(meshFile);
    i++;
  }
}

void celHNavStruct::SaveHighLevelGraph (iDocumentNode* node1, iDocumentNode* node2)
{
  // Nodes
  size_t size = hlGraph->GetNodeCount();
  for (size_t i = 0; i < size; i++)
  {
    csRef<iCelNode> graphNode = hlGraph->GetNode(i);
    csRef<iDocumentNode> n = node1->CreateNodeBefore(CS_NODE_ELEMENT);
    n->SetValue("node");
    n->SetAttributeAsInt("id", i);
    csVector3 position = graphNode->GetPosition();
    n->SetAttributeAsFloat("x", position[0]);
    n->SetAttributeAsFloat("y", position[1]);
    n->SetAttributeAsFloat("z", position[2]);
    n->SetAttribute("sector", graphNode->GetMapNode()->GetSector()->QueryObject()->GetName());
    csString name;
    name = i;    
    graphNode->SetName(name.GetDataSafe());
  }

  // Edges
  for (size_t i = 0; i < size; i++)
  {
    csRef<iCelNode> graphNode = hlGraph->GetNode(i);
    csRefArray<iCelEdge> edges = graphNode->GetEdges();
    for (size_t j = 0; j < edges.GetSize(); j++)
    {
      csRef<iDocumentNode> e = node2->CreateNodeBefore(CS_NODE_ELEMENT);
      e->SetValue("edge");
      e->SetAttribute("from", graphNode->GetName());
      e->SetAttribute("to", edges[j]->GetSuccessor()->GetName());
      e->SetAttributeAsFloat("weight", edges[j]->GetWeight());
    }    
  }
}

bool celHNavStruct::SaveToFile (iVFS* vfs, const char* file)
{
  csRef<iDocumentSystem> docsys = csLoadPluginCheck<iDocumentSystem>(objectRegistry, "crystalspace.documentsystem.tinyxml");
  if (!docsys)
  {
    return false;
  }

  // Create zip file and mount it
  const char* workingDir = vfs->GetCwd();
  csString workingDirBkp(workingDir);
  csRef<iDataBuffer> wdBuffer = vfs->GetRealPath(workingDir);
  csString realPath(wdBuffer->GetData());
  realPath += file;
  csString virtualPath(workingDir);
  virtualPath += file;
  vfs->Mount(virtualPath.GetDataSafe(), realPath.GetDataSafe());
  vfs->ChDir(virtualPath.GetDataSafe());

  // Create XML file
  csRef<iDocument> doc = docsys->CreateDocument();
  csRef<iDocumentNode> root = doc->CreateRoot();
  csRef<iDocumentNode> mainNode = root->CreateNodeBefore(CS_NODE_ELEMENT);
  mainNode->SetValue("iCelHNavStruct");

  // Create parameters subtree
  csRef<iDocumentNode> params = mainNode->CreateNodeBefore(CS_NODE_ELEMENT);
  params->SetValue("parameters");
  SaveParameters(params);

  // Create navmeshes subtree
  csRef<iDocumentNode> navmeshes = mainNode->CreateNodeBefore(CS_NODE_ELEMENT);
  navmeshes->SetValue("navmeshes");
  SaveNavMeshes(navmeshes, vfs);

  // Create highlevelgraph subtree
  csRef<iDocumentNode> hlgraph = mainNode->CreateNodeBefore(CS_NODE_ELEMENT);
  hlgraph->SetValue("highlevelgraph");
  csRef<iDocumentNode> nodes = hlgraph->CreateNodeBefore(CS_NODE_ELEMENT);
  nodes->SetValue("nodes");
  csRef<iDocumentNode> edges = hlgraph->CreateNodeBefore(CS_NODE_ELEMENT);
  edges->SetValue("edges");
  SaveHighLevelGraph(nodes, edges);

  // Write xml file
  const char* log = doc->Write(vfs, "navstruct.xml");
  vfs->ChDir(workingDirBkp.GetDataSafe());
  vfs->Sync();

  if (log)
  {    
    return false;
  }

  return true;
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
 * celHNavStructBuilder
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

  navStruct.AttachNew(new celHNavStruct(parameters, objectRegistry));

  csHash<csRef<iCelNavMeshBuilder>, csPtrKey<iSector> >::GlobalIterator it = builders.GetIterator();
  while (it.HasNext())
  {
    csRef<iCelNavMeshBuilder> builder = it.Next();
    iCelNavMesh* navMesh = builder->BuildNavMesh();
    navStruct->AddNavMesh(navMesh);
  }
  navStruct->BuildHighLevelGraph();

  return navStruct;
}

bool celHNavStructBuilder::ParseParameters (iDocumentNode* node, iCelNavMeshParams* params)
{
  csRef<iDocumentNode> param = node->GetNode("agentheight");
  float value = param->GetAttributeValueAsFloat("value");
  params->SetAgentHeight(value);

  param = node->GetNode("agentradius");
  value = param->GetAttributeValueAsFloat("value");
  params->SetAgentRadius(value);

  param = node->GetNode("agentmaxslopeangle");
  value = param->GetAttributeValueAsFloat("value");
  params->SetAgentMaxSlopeAngle(value);

  param = node->GetNode("agentmaxclimb");
  value = param->GetAttributeValueAsFloat("value");
  params->SetAgentMaxClimb(value);

  param = node->GetNode("cellsize");
  value = param->GetAttributeValueAsFloat("value");
  params->SetCellSize(value);

  param = node->GetNode("cellheight");
  value = param->GetAttributeValueAsFloat("value");
  params->SetCellHeight(value);

  param = node->GetNode("maxsimplificationerror");
  value = param->GetAttributeValueAsFloat("value");
  params->SetMaxSimplificationError(value);

  param = node->GetNode("detailsampledist");
  value = param->GetAttributeValueAsFloat("value");
  params->SetDetailSampleDist(value);

  param = node->GetNode("detailsamplemaxerror");
  value = param->GetAttributeValueAsFloat("value");
  params->SetDetailSampleMaxError(value);

  param = node->GetNode("maxedgelength");
  int value2 = param->GetAttributeValueAsInt("value");
  params->SetMaxEdgeLength(value2);

  param = node->GetNode("minregionsize");
  value2 = param->GetAttributeValueAsInt("value");
  params->SetMinRegionSize(value2);

  param = node->GetNode("mergeregionsize");
  value2 = param->GetAttributeValueAsInt("value");
  params->SetMergeRegionSize(value2);

  param = node->GetNode("maxvertsperpoly");
  value2 = param->GetAttributeValueAsInt("value");
  params->SetMaxVertsPerPoly(value2);

  param = node->GetNode("tilesize");
  value2 = param->GetAttributeValueAsInt("value");
  params->SetTileSize(value2);

  param = node->GetNode("bordersize");
  value2 = param->GetAttributeValueAsInt("value");
  params->SetBorderSize(value2);

  return true;
}

// TODO only reads sectors for now
bool celHNavStructBuilder::ParseMeshes (iDocumentNode* node, csHash<csRef<iSector>, const char*>& sectors, 
                                        celHNavStruct* navStruct, iVFS* vfs, iCelNavMeshParams* params)
{
  csRef<iEngine> engine = csLoadPluginCheck<iEngine>(objectRegistry, "crystalspace.engine.3d");
  if (!engine)
  {
    return false;
  }
  csList<csRef<iSector> > sectorList;
  size_t size = engine->GetSectors()->GetCount();
  for (size_t i = 0; i < size; i++)
  {
    sectorList.PushBack(engine->GetSectors()->Get(i));    
  }

  csRef<iDocumentNodeIterator> it = node->GetNodes("navmesh");
  while (it->HasNext())
  {
    csRef<iDocumentNode> navMeshNode = it->Next();

    // Get sector
    const char* sectorName = navMeshNode->GetAttributeValue("sector");
    csString sectorNameString(sectorName);
    csList<csRef<iSector> >::Iterator sectorIt(sectorList);
    csRef<iSector> sector;
    while (sectorIt.HasNext())
    {
      sector = sectorIt.Next();
      if (sectorNameString == sector->QueryObject()->GetName())
      {
        sectors.Put(sectorName, sector);
        break;
      }
    }

    // Get navmesh
    int id = navMeshNode->GetAttributeValueAsInt("id");
    csString fileName;
    fileName = id;
    csRef<iFile> file = vfs->Open(fileName.GetDataSafe(), VFS_FILE_READ);
    csRef<iCelNavMeshBuilder> builder = csLoadPluginCheck<iCelNavMeshBuilder>(objectRegistry, "cel.navmeshbuilder");
    csRef<iCelNavMesh> navMesh = builder->LoadNavMesh(file);
    navStruct->AddNavMesh(navMesh);
  }
  return true;
}

bool celHNavStructBuilder::ParseGraph (iDocumentNode* node, iCelGraph* graph, csHash<csRef<iSector>, const char*> sectors)
{
  csRef<iDocumentNode> nodesNode = node->GetNode("nodes");
  csRef<iDocumentNodeIterator> it = nodesNode->GetNodes("node");
  size_t nodeCount = it->GetEndPosition();
  csArray<csRef<iCelNode> > celNodes(nodeCount);
  celNodes.SetSize(nodeCount);
  while (it->HasNext())
  {
    csRef<iDocumentNode> graphNode = it->Next();
    size_t id = graphNode->GetAttributeValueAsInt("id");

    // Get sector
    const char* sectorName = graphNode->GetAttributeValue("sector");
    csRef<iSector> sector = sectors.Get(sectorName, 0);

    // Get position
    csVector3 position;
    float x = graphNode->GetAttributeValueAsFloat("x");
    float y = graphNode->GetAttributeValueAsFloat("y");
    float z = graphNode->GetAttributeValueAsFloat("z");
    position.Set(x, y, z);
    
    // Crete node
    csRef<iCelNode> node = graph->CreateEmptyNode(id);
    celNodes[id] = node;
    csRef<iMapNode> mapNode;
    mapNode.AttachNew(new csMapNode("n"));
    mapNode->SetPosition(position);
    mapNode->SetSector(sector);
    node->SetMapNode(mapNode);
  }

  csRef<iDocumentNode> edgesNode = node->GetNode("edges");
  it = edgesNode->GetNodes("edge");
  while (it->HasNext())
  {
    csRef<iDocumentNode> graphEdge = it->Next();
    int from = graphEdge->GetAttributeValueAsInt("from");
    int to = graphEdge->GetAttributeValueAsInt("to");
    float weight = graphEdge->GetAttributeValueAsFloat("weight");
    celNodes[from]->AddSuccessor(celNodes[to], true, weight);
  }

  return true;
}

iCelHNavStruct* celHNavStructBuilder::LoadHNavStruct (iVFS* vfs, const char* file)
{
  csRef<iDocumentSystem> docsys = csLoadPluginCheck<iDocumentSystem>(objectRegistry, "crystalspace.documentsystem.tinyxml");
  if (!docsys)
  {
    return 0;
  }

  // Mount file
  const char* workingDir = vfs->GetCwd();
  csString workingDirBkp(workingDir);
  csRef<iDataBuffer> wdBuffer = vfs->GetRealPath(workingDir);
  csString realPath(wdBuffer->GetData());
  realPath += file;
  csString virtualPath(workingDir);
  virtualPath += file;
  vfs->Mount(virtualPath.GetDataSafe(), realPath.GetDataSafe());
  vfs->ChDir(virtualPath.GetDataSafe());

  // Read XML file
  csRef<iDocument> doc = docsys->CreateDocument();
  csRef<iFile> xmlFile = vfs->Open("navstruct.xml", VFS_FILE_READ);
  const char* log = doc->Parse(xmlFile);
  if (log)
  {
    return 0;
  }
  csRef<iDocumentNode> root = doc->GetRoot();
  csRef<iDocumentNode> mainNode = root->GetNode("iCelHNavStruct");

  // Read parameters
  csRef<iDocumentNode> paramsNode = mainNode->GetNode("parameters");
  csRef<iCelNavMeshParams> params;
  params.AttachNew(new celNavMeshParams());
  ParseParameters(paramsNode, params);
  
  // Create navigation structure
  csRef<celHNavStruct> navStruct;
  navStruct.AttachNew(new celHNavStruct(params, objectRegistry));

  // Read navigation meshes
  csRef<iDocumentNode> meshesNode = mainNode->GetNode("navmeshes");
  csHash<csRef<iSector>, const char*> sectors;
  ParseMeshes(meshesNode, sectors, navStruct, vfs, params);

  // Read high level graph
  csRef<iDocumentNode> graphNode = mainNode->GetNode("highlevelgraph");
  csRef<iCelGraph> graph = scfCreateInstance<iCelGraph>("cel.celgraph");
  ParseGraph(graphNode, graph, sectors);
  navStruct->SetHighLevelGraph(graph);

  this->navStruct = navStruct;
  vfs->ChDir(workingDirBkp.GetDataSafe());

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
