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
#include "cstool/mapnode.h"

CS_PLUGIN_NAMESPACE_BEGIN(celNavMesh)
{



/*
 * celHPath
 */

celHPath::celHPath () : scfImplementationType (this)
{
}

celHPath::~celHPath ()
{
}
/*
// TODO implement
iObject* celHPath::QueryObject ()
{
  return 0;
}
*/
// TODO implement
void celHPath::AddNode (iMapNode* node)
{
}

// TODO implement
void celHPath::InsertNode (size_t pos, iMapNode* node)
{
}

// TODO implement
iMapNode* celHPath::Next ()
{
  return 0;
}

// TODO implement
iMapNode* celHPath::Previous ()
{
  return 0;
}

// TODO implement
iMapNode* celHPath::Current ()
{
  return 0;
}

// TODO implement
csVector3 celHPath::CurrentPosition ()
{
  return csVector3();
}

// TODO implement
iSector* celHPath::CurrentSector ()
{
  return 0;
}

// TODO implement
bool celHPath::HasNext ()
{
  return false;
}

// TODO implement
bool celHPath::HasPrevious ()
{
  return false;
}

// TODO implement
void celHPath::Restart ()
{
}

// TODO implement
void celHPath::Clear ()
{
}

// TODO implement
iMapNode* celHPath::GetFirst ()
{
  return 0;
}

// TODO implement
iMapNode* celHPath::GetLast ()
{
  return 0;
}

// TODO implement
void celHPath::Invert ()
{
}

// TODO implement
size_t celHPath::GetNodeCount()
{
  return 0;
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

// TODO finish
bool celHNavStruct::BuildHighLevelGraph()
{
  hlGraph.Invalidate();
  hlGraph = scfCreateInstance<iCelGraph>("cel.celgraph");
  if (!hlGraph)
  {
    return false;
  }

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
          
          // Add the portal bounding box's center as a node to the graph
          csVector3 center(boundingMin[0] + (boundingMax[0] / 2), boundingMin[1] + (boundingMax[1] / 2),
              boundingMin[2] + (boundingMax[2] / 2));
          csRef<iMapNode> mapNode;
          mapNode.AttachNew(new csMapNode("n")); // TODO set name?
          mapNode->SetPosition(center);
          mapNode->SetSector(sector);
          csRef<iCelNode> node = scfCreateInstance<iCelNode>("cel.celnode");
          if (!node)
          {
            return false;
          }
          node->SetName(""); // TODO set name?
          node->SetMapNode(mapNode);
          hlGraph->AddNode(node);

          // TODO add point at the "same" coordinate (warped?) on destination sector, and create edge 
          // between it and the original node (zero cost).
          csReversibleTransform warp = portal->GetWarp();
          csVector3 warped = warp.This2Other(center);
          csRef<iMapNode> mapNode2;
          mapNode2.AttachNew(new csMapNode("n")); // TODO set name?
          mapNode2->SetPosition(warped);
          mapNode2->SetSector(portal->GetSector());
          csRef<iCelNode> node2 = scfCreateInstance<iCelNode>("cel.celnode");
          if (!node2)
          {
            return false;
          }
          node2->SetName(""); // TODO set name?
          node2->SetMapNode(mapNode2);
          hlGraph->AddNode(node2);

          hlGraph->AddEdge(node, node2, true); // TODO state?
          // TODO edge weight???
        }
      }
    }
  }

  return true;
}

// TODO implement
iCelPath* celHNavStruct::ShortestPath (const csVector3& from, iSector* fromSector, const csVector3& goal,
    iSector* goalSector) const
{
  return 0;
}

// TODO implement
iCelPath* celHNavStruct::ShortestPath (iMapNode* from, iMapNode* goal) const
{
  return 0;
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
    iSector* sector = it.Next();    
    csRef<iCelNavMeshBuilder> builder = scfCreateInstance<iCelNavMeshBuilder>("cel.navmeshbuilder");
    if (!builder)
    {
      return false;
    }
    builder->SetNavMeshParams(parameters);
    builder->SetSector(sector);
    csPtrKey<iSector> key = sector;
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
