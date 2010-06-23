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


#include "CelNavMesh.h"

CS_IMPLEMENT_PLUGIN

CS_PLUGIN_NAMESPACE_BEGIN(celNavMesh)
{
  


inline unsigned int nextPow2 (unsigned int v)
{
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

inline unsigned int ilog2 (unsigned int v)
{
  unsigned int r;
  unsigned int shift;
  r = (v > 0xffff) << 4; v >>= r;
  shift = (v > 0xff) << 3; v >>= shift; r |= shift;
  shift = (v > 0xf) << 2; v >>= shift; r |= shift;
  shift = (v > 0x3) << 1; v >>= shift; r |= shift;
  r |= (v >> 1);
  return r;
}



/*
 * DebugDrawGL
 */

void DebugDrawGL::depthMask (bool state)
{
  glDepthMask(state ? GL_TRUE : GL_FALSE);
}

void DebugDrawGL::begin (duDebugDrawPrimitives prim, float size)
{
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  switch (prim)
  {
  case DU_DRAW_POINTS:
    glPointSize(size);
    glBegin(GL_POINTS);
    break;
  case DU_DRAW_LINES:    
    glLineWidth(size);
    glBegin(GL_LINES);
    break;
  case DU_DRAW_TRIS:
    glBegin(GL_TRIANGLES);
    break;
  case DU_DRAW_QUADS:
    glBegin(GL_QUADS);
    break;
  };
}

void DebugDrawGL::vertex (const float* pos, unsigned int color)
{
  glColor4ubv((GLubyte*)&color);
  glVertex3fv(pos);
}

void DebugDrawGL::vertex (const float x, const float y, const float z, unsigned int color)
{
  glColor4ubv((GLubyte*)&color);
  glVertex3f(x,y,z);
}

void DebugDrawGL::end ()
{
  glEnd();
  glLineWidth(1.0f);
  glPointSize(1.0f);
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_POINT_SMOOTH);  
}



/*
 * celNavMeshPath
 */

const int celNavMeshPath::INCREASE_PATH_BY = 10;

celNavMeshPath::celNavMeshPath (float* path, int pathSize, int maxPathSize) 
    : scfImplementationType (this)
{
  this->path = path;
  this->pathSize = pathSize;
  this->maxPathSize = maxPathSize;
  currentPosition = 0;
  increasePosition = 3;
}

celNavMeshPath::~celNavMeshPath ()
{
  delete [] path;
}

// TODO implement
iSector* celNavMeshPath::GetSector () const
{
  return 0;
}

void celNavMeshPath::Current (csVector3& vector) const
{
  vector[0] = path[currentPosition];
  vector[1] = path[currentPosition + 1];
  vector[2] = path[currentPosition + 2];
}

void celNavMeshPath::Next (csVector3& vector)
{
  currentPosition += increasePosition;
  vector[0] = path[currentPosition];
  vector[1] = path[currentPosition + 1];
  vector[2] = path[currentPosition + 2];
}

void celNavMeshPath::Previous (csVector3& vector)
{
  currentPosition -= increasePosition;
  vector[0] = path[currentPosition];
  vector[1] = path[currentPosition + 1];
  vector[2] = path[currentPosition + 2];
}

void celNavMeshPath::GetFirst (csVector3& vector) const
{
  int index = (increasePosition > 0) ? 0 : ((pathSize - 1) * 3);
  vector[0] = path[index];
  vector[1] = path[index + 1];
  vector[2] = path[index + 2];
}

void celNavMeshPath::GetLast (csVector3& vector) const
{
  int index = (increasePosition > 0) ? ((pathSize - 1) * 3) : 0;
  vector[0] = path[index];
  vector[1] = path[index + 1];
  vector[2] = path[index + 2];
}

bool celNavMeshPath::HasNext () const
{
  if (increasePosition > 0)
  {
    if (currentPosition < (pathSize - 1) * 3)
    {
      return true;
    }
  }
  else
  {
    if (currentPosition >= 3)
    {
      return true;
    }
  }
  return false;
}

bool celNavMeshPath::HasPrevious () const
{
  if (increasePosition > 0)
  {
    if (currentPosition >= 3)
    {
      return true;
    }
  }
  else
  {
    if (currentPosition < (pathSize - 1) * 3)
    {
      return true;
    }
  }
  return false;
}

void celNavMeshPath::Invert ()
{
  increasePosition = -increasePosition;
}

void celNavMeshPath::Restart ()
{
  if (increasePosition > 0)
  {
    currentPosition = 0;
  }
  else
  {
    currentPosition = (pathSize - 1) * 3;
  }
}

void celNavMeshPath::AddNode (csVector3 node) 
{
  if (pathSize == maxPathSize)
  {
    float* newPath = new float[(maxPathSize + INCREASE_PATH_BY) * 3];
    memcpy(newPath, path, pathSize * 3 * sizeof(float));
    delete [] path;
    path = newPath;
    maxPathSize += INCREASE_PATH_BY;
  }
  int index = pathSize * 3;
  path[index] = node[0];
  path[index + 1] = node[1];
  path[index + 2] = node[2];
  pathSize++;
}

void celNavMeshPath::InsertNode (int pos, csVector3 node)
{
  int index = pos * 3;
  if (pathSize == maxPathSize)
  {
    float* newPath = new float[(maxPathSize + INCREASE_PATH_BY) * 3];
    memcpy(newPath, path, (pos * 3) * sizeof(float));
    memcpy(newPath + ((pos + 1) * 3), path + (pos * 3), (pathSize - pos) * 3 * sizeof(float));
    delete [] path;
    path = newPath;
    maxPathSize += INCREASE_PATH_BY;
  }
  else
  {
    memmove(path + ((pos + 1) * 3), path + (pos * 3), (pathSize - pos) * 3 * sizeof(float));
  }
  path[index] = node[0];
  path[index + 1] = node[1];
  path[index + 2] = node[2];
  pathSize++;
}

float celNavMeshPath::Length() const
{
  float length = 0;
  int index;
  float f0, f1, f2;
  for (int i = 1; i < pathSize; i++)
  {
    index = i * 3;
    f0 = (path[index] - path[index - 3]);
    f1 = (path[index + 1] - path[index - 2]);
    f2 = (path[index + 2] - path[index - 1]);
    length += csQsqrt(f0 * f0 + f1 * f1 + f2 * f2);
  }

  return length;
}

// Based on Detour NavMeshTesterTool::handleRender()
void celNavMeshPath::DebugRenderPath ()
{
  if (pathSize)
  {
    DebugDrawGL dd;
    dd.depthMask(false);
    const unsigned int pathCol = duRGBA(255, 255, 255, 230);
//    const unsigned int offMeshCol = duRGBA(128, 96, 0, 220);
    dd.begin(DU_DRAW_LINES, 4.0f);
    for (int i = 0; i < pathSize - 1; ++i)
    {
//      unsigned int col = 0;
//      if (m_straightPathFlags[i] & DT_STRAIGHTPATH_OFFMESH_CONNECTION)
//        col = offMeshCol;
//      else
//        col = pathCol;
      unsigned int col = pathCol;

      dd.vertex(path[i * 3], path[i * 3 + 1] + 0.4f, path[i * 3 + 2], col);
      dd.vertex(path[(i + 1) * 3], path[(i + 1) * 3 + 1] + 0.4f, path[(i + 1) * 3 + 2], col);
    }
    dd.end();
    dd.begin(DU_DRAW_POINTS, 10.0f);
    for (int i = 0; i < pathSize; ++i)
    {
      unsigned int col = 0;
/*      if (m_straightPathFlags[i] & DT_STRAIGHTPATH_START)
        col = startCol;
      else if (m_straightPathFlags[i] & DT_STRAIGHTPATH_START)
        col = endCol;
      else if (m_straightPathFlags[i] & DT_STRAIGHTPATH_OFFMESH_CONNECTION)
        col = offMeshCol;
      else
        col = pathCol;
*/
      dd.vertex(path[i*3], path[i * 3 + 1] + 0.4f, path[i * 3 + 2], duRGBA(255, 150, 0, 230));
    }
    dd.end();
    dd.depthMask(true);
  }
}



/*
 * celNavMeshTile
 */
celNavMeshTile::celNavMeshTile () : scfImplementationType (this)
{
  data = 0;
}

celNavMeshTile::~celNavMeshTile ()
{
  delete data;
}

int celNavMeshTile::GetData (const unsigned char* data) const
{
  data = this->data;
  return dataSize;
}

void celNavMeshTile::SetData (unsigned char* data, int dataSize)
{
  delete this->data;
  this->data = data;
  this->dataSize = dataSize;
}



/*
 * celNavMesh
 */

const int celNavMesh::MAX_NODES = 2048;

celNavMesh::celNavMesh () : scfImplementationType (this)
{
  parameters = 0;
  sector = 0;
  detourNavMesh = 0;
  navMeshDrawFlags = DU_DRAWNAVMESH_OFFMESHCONS;
  filter.includeFlags = SAMPLE_POLYFLAGS_ALL;
  filter.excludeFlags = 0;
  polyPickExt[0] = 2;
  polyPickExt[1] = 4;
  polyPickExt[2] = 2;
}

celNavMesh::~celNavMesh ()
{
  delete detourNavMesh;
}

 // Based on Recast Sample_TileMesh::handleBuild() and Sample_TileMesh::handleSettings()
bool celNavMesh::Initialize (const iCelNavMeshParams* parameters, const float* boundingMin, const float* boundingMax)
{
  this->parameters.AttachNew(new celNavMeshParams(parameters));
  rcVcopy(this->boundingMin, boundingMin);
  rcVcopy(this->boundingMax, boundingMax);

  int gw = 0, gh = 0;
  rcCalcGridSize(boundingMin, boundingMax, parameters->GetCellSize(), &gw, &gh);
  const int ts = parameters->GetTileSize();
  const int tw = (gw + ts - 1) / ts;
  const int th = (gh + ts - 1) / ts;
  
  // Max tiles and max polys affect how the tile IDs are caculated.
  // There are 22 bits available for identifying a tile and a polygon.
  int tileBits = rcMin((int)ilog2(nextPow2(tw * th)), 14);
  if (tileBits > 14)
  {
    tileBits = 14;
  }
  int polyBits = 22 - tileBits;
  int maxTiles = 1 << tileBits;
  int maxPolysPerTile = 1 << polyBits;

  dtNavMeshParams params;
  params.orig[0] = boundingMin[0];
  params.orig[1] = boundingMin[1];
  params.orig[2] = boundingMin[2];
  params.tileWidth = parameters->GetTileSize() * parameters->GetCellSize();
  params.tileHeight = params.tileWidth;
  params.maxTiles = maxTiles;
  params.maxPolys = maxPolysPerTile;
  params.maxNodes = MAX_NODES;
  detourNavMesh = new dtNavMesh;
  return !detourNavMesh->init(&params);
}

// Based on Recast NavMeshTesterTool::recalc()
iCelNavMeshPath* celNavMesh::ShortestPath (const csVector3& from, const csVector3& goal, const int maxPathSize) const
{
  celNavMeshPath* path = 0;

  float startPos[3];
  float endPos[3];
  for (int i = 0; i < 3; i++) 
  {
    startPos[i] = from[i];
    endPos[i] = goal[i];
  }

  // Find starting polygons
  dtPolyRef startRef = detourNavMesh->findNearestPoly(startPos, polyPickExt, &filter, 0);
  dtPolyRef endRef = detourNavMesh->findNearestPoly(endPos, polyPickExt, &filter, 0);

  // Find the polygons that compose the path
  dtPolyRef* polys = new dtPolyRef[maxPathSize];
  int npolys = detourNavMesh->findPath(startRef, endRef, startPos, endPos, &filter, polys, maxPathSize);

  // Find the actual path
  float* straightPath = new float[maxPathSize * 3];
  unsigned char* straightPathFlags = new unsigned char[maxPathSize];
  dtPolyRef* straightPathPolys = new dtPolyRef[maxPathSize];
  int nstraightPath = 0;
  if (npolys)
  {
    nstraightPath = detourNavMesh->findStraightPath(startPos, endPos, polys, npolys, straightPath, 
                                                    straightPathFlags, straightPathPolys, maxPathSize);
  }

  if (nstraightPath)
  {
    path = new celNavMeshPath(straightPath, nstraightPath, maxPathSize);
  }
  else
  {
    delete [] straightPath;
  }

  // For now, these are not really used
  delete [] polys;
  delete [] straightPathFlags;
  delete [] straightPathPolys;

  return path;
}

// TODO implement
bool celNavMesh::SetTile (iCelNavMeshTile* tile)
{
  // Remove old tile, if it exists
  //detourNavMesh->removeTile(detourNavMesh->getTileRefAt(x, y), 0, 0);
  return false;
}

bool celNavMesh::AddTile (unsigned char* data, int dataSize)
{
  if (!detourNavMesh->addTile(data, dataSize, true))
  {
    return false;
  }
  return true;
}

iSector* celNavMesh::GetSector () const
{
  return sector;
}

void celNavMesh::SetSector (iSector* sector)
{
  this->sector = sector;
}

iCelNavMeshParams* celNavMesh::GetParameters () const
{
  return parameters;
}

void celNavMesh::DebugRender () const
{
  DebugDrawGL dd;
/*
  // Draw mesh
  duDebugDrawTriMesh(&dd, m_geom->getMesh()->getVerts(), m_geom->getMesh()->getVertCount(),
    m_geom->getMesh()->getTris(), m_geom->getMesh()->getNormals(), m_geom->getMesh()->getTriCount(), 0);
  m_geom->drawOffMeshConnections(&dd);

  glDepthMask(GL_FALSE);

  // Draw bounds
  const float* bmin = m_geom->getMeshBoundsMin();
  const float* bmax = m_geom->getMeshBoundsMax();
  duDebugDrawBoxWire(&dd, bmin[0],bmin[1],bmin[2], bmax[0],bmax[1],bmax[2], duRGBA(255,255,255,128), 1.0f);

  // Tiling grid.
  int gw = 0, gh = 0;
  rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
  const int tw = (gw + (int)m_tileSize-1) / (int)m_tileSize;
  const int th = (gh + (int)m_tileSize-1) / (int)m_tileSize;
  const float s = m_tileSize*m_cellSize;
  duDebugDrawGridXZ(&dd, bmin[0],bmin[1],bmin[2], tw,th, s, duRGBA(0,0,0,64), 1.0f);

  // Draw active tile
  duDebugDrawBoxWire(&dd, m_tileBmin[0],m_tileBmin[1],m_tileBmin[2], m_tileBmax[0],m_tileBmax[1],m_tileBmax[2], m_tileCol, 2.0f);
*/
  //glDepthMask(GL_FALSE);
  glDepthMask(GL_TRUE);
  duDebugDrawNavMesh(&dd, *detourNavMesh, navMeshDrawFlags);
  //glDepthMask(GL_FALSE);
/*
  if (m_tool)
    m_tool->handleRender();

  m_geom->drawConvexVolumes(&dd);

  glDepthMask(GL_TRUE);
  */
}

void celNavMesh::DebugRenderAgent(const csVector3& pos) const
{
  DebugRenderAgent(pos, 51, 102, 0, 129);
}

void celNavMesh::DebugRenderAgent(const csVector3& pos, int red, int green, int blue, int alpha) const
{
  DebugDrawGL dd;
  glDepthMask(GL_FALSE);

  const float r = parameters->GetAgentRadius();
  const float h = parameters->GetAgentHeight();
  const float c = parameters->GetAgentMaxClimb();
  const unsigned int col = duRGBA(red, green, blue, alpha);

  // Agent dimensions.	
  duDebugDrawCylinderWire(&dd, pos[0] - r, pos[1] + 0.02f, pos[2] - r, pos[0] + r, pos[1] + h, pos[2] + r, col, 4.0f);

  duDebugDrawCircle(&dd, pos[0], pos[1] + c, pos[2], r, duRGBA(0, 0, 0, 180), 4.0f);

  unsigned int colb = duRGBA(0, 0, 0, 180);
  dd.begin(DU_DRAW_LINES, 2.0f);
  dd.vertex(pos[0], pos[1] - c, pos[2], colb);
  dd.vertex(pos[0], pos[1] + c, pos[2], colb);
  dd.vertex(pos[0] - r/2, pos[1] + 0.02f, pos[2], colb);
  dd.vertex(pos[0] + r/2, pos[1] + 0.02f, pos[2], colb);
  dd.vertex(pos[0], pos[1] + 0.02f, pos[2] - r/2, colb);
  dd.vertex(pos[0], pos[1] + 0.02f, pos[2] + r/2, colb);
  dd.end();

  glDepthMask(GL_TRUE);
}



/*
 * celNavMeshParams
 */

celNavMeshParams::celNavMeshParams () : scfImplementationType (this)
{
  SetSuggestedValues(2.0f, 0.6f, 45);
}

celNavMeshParams::celNavMeshParams (const iCelNavMeshParams* parameters) : scfImplementationType (this)
{
  agentHeight = parameters->GetAgentHeight();
  agentRadius = parameters->GetAgentRadius();
  agentMaxSlopeAngle = parameters->GetAgentMaxSlopeAngle();
  agentMaxClimb = parameters->GetAgentMaxClimb();
  cellSize = parameters->GetCellSize();
  cellHeight = parameters->GetCellHeight();
  maxSimplificationError = parameters->GetMaxSimplificationError();
  detailSampleDist = parameters->GetDetailSampleDist();
  detailSampleMaxError = parameters->GetDetailSampleMaxError();
  maxEdgeLength = parameters->GetMaxEdgeLength();
  minRegionSize = parameters->GetMinRegionSize();
  mergeRegionSize = parameters->GetMergeRegionSize();
  maxVertsPerPoly = parameters->GetMaxVertsPerPoly();
  tileSize = parameters->GetTileSize();
  borderSize = parameters->GetBorderSize();
}

celNavMeshParams::~celNavMeshParams ()
{
}

iCelNavMeshParams* celNavMeshParams::Clone () const
{
  celNavMeshParams* params = new celNavMeshParams(this);
  return params;
}

void celNavMeshParams::SetSuggestedValues (float agentHeight, float agentRadius, float agentMaxSlopeAngle)
{
  this->agentHeight = agentHeight;
  this->agentRadius = agentRadius;
  this->agentMaxSlopeAngle = agentMaxSlopeAngle;
  agentMaxClimb = agentHeight / 4.0f;
  cellSize = agentRadius / 2.0f;
  cellHeight = cellSize / 2.0f;
  maxSimplificationError = 1.3f;
  detailSampleDist = 6.0f;
  detailSampleMaxError = 1.0f;
  maxEdgeLength = ((int)ceilf(agentRadius / cellSize)) * 8;  
  minRegionSize = 50;
  mergeRegionSize = 20;
  maxVertsPerPoly = 6;
  tileSize = 32;
  borderSize = (int)ceilf(agentRadius / cellSize) + 3;  
}

float celNavMeshParams::GetAgentHeight () const
{
  return agentHeight;
}

void celNavMeshParams::SetAgentHeight (const float height)
{
  agentHeight = height;
}

float celNavMeshParams::GetAgentRadius () const
{
  return agentRadius;
}

void celNavMeshParams::SetAgentRadius (const float radius)
{
  agentRadius = radius;
}

float celNavMeshParams::GetAgentMaxSlopeAngle () const 
{
  return agentMaxSlopeAngle;
}

void celNavMeshParams::SetAgentMaxSlopeAngle (const float angle)
{
  agentMaxSlopeAngle = angle;
}

float celNavMeshParams::GetAgentMaxClimb () const
{
  return agentMaxClimb;
}

void celNavMeshParams::SetAgentMaxClimb (const float climb)
{
  agentMaxClimb = climb;
}

float celNavMeshParams::GetCellSize () const
{
  return cellSize;
}

void celNavMeshParams::SetCellsize (float size)
{
  cellSize = size;
}

float celNavMeshParams::GetCellHeight () const
{
  return cellHeight;
}

void celNavMeshParams::SetCellHeight (float height)
{
  cellHeight = height;
}

float celNavMeshParams::GetMaxSimplificationError () const
{
  return maxSimplificationError;
}

void celNavMeshParams::SetMaxSimplificationError (const float error)
{
  maxSimplificationError = error;
}

float celNavMeshParams::GetDetailSampleDist () const
{
  return detailSampleDist;
}

void celNavMeshParams::SetDetailSampleDist (const float dist)
{
  detailSampleDist = dist;
}

float celNavMeshParams::GetDetailSampleMaxError () const
{
  return detailSampleMaxError;
}

void celNavMeshParams::SetDetailSampleMaxError (const float error)
{
  detailSampleMaxError = error;
}

int celNavMeshParams::GetMaxEdgeLength () const
{
  return maxEdgeLength;
}

void celNavMeshParams::SetMaxEdgeLength (const int length)
{
  maxEdgeLength = length;
}

int celNavMeshParams::GetMinRegionSize () const
{
  return minRegionSize;
}

void celNavMeshParams::SetMinRegionSize (const int size)
{
  minRegionSize = size;
}

int celNavMeshParams::GetMergeRegionSize () const
{
  return mergeRegionSize;
}

void celNavMeshParams::SetMergeRegionSize (const int size)
{
  mergeRegionSize = size;
}

int celNavMeshParams::GetMaxVertsPerPoly () const
{
  return maxVertsPerPoly;
}

void celNavMeshParams::SetMaxVertsPerPoly (const int maxVerts)
{
  maxVertsPerPoly = maxVerts;
}

int celNavMeshParams::GetTileSize () const
{
  return tileSize;
}

void celNavMeshParams::SetTilesize (const int size)
{
  tileSize = size;
}

int celNavMeshParams::GetBorderSize () const
{
  return borderSize;
}

void celNavMeshParams::SetBorderSize (const int size)
{
  borderSize = size;
}



/*
 * celNavMeshBuilder
 */

SCF_IMPLEMENT_FACTORY (celNavMeshBuilder)

celNavMeshBuilder::celNavMeshBuilder (iBase* parent) : scfImplementationType (this, parent)
{
  // Pointers
  triangleVertices = 0;
  triangleIndices = 0;
  chunkyTriMesh = 0;
  triangleFlags = 0;
  solid = 0;
  chf = 0;
  cSet = 0;
  pMesh = 0;
  dMesh = 0;

  numberOfVertices = 0;
  numberOfTriangles = 0;
  numberOfOffMeshCon = 0;
  numberOfVolumes = 0;

  parameters.AttachNew(new celNavMeshParams());
}

celNavMeshBuilder::~celNavMeshBuilder ()
{
  CleanUpSectorData();
  CleanUpTileData();
}

void celNavMeshBuilder::CleanUpSectorData () 
{
  delete triangleVertices;
  triangleVertices = 0;
  delete triangleIndices;
  triangleIndices = 0;
  delete chunkyTriMesh;
  chunkyTriMesh = 0;

  numberOfVertices = 0;
  numberOfTriangles = 0;
  numberOfOffMeshCon = 0;
  numberOfVolumes = 0;
}

bool celNavMeshBuilder::Initialize (iObjectRegistry* objectRegistry) 
{
  this->objectRegistry = objectRegistry;
  strings = csQueryRegistryTagInterface<iStringSet>(objectRegistry, "crystalspace.shared.stringset");
  //strings = csQueryRegistry<iStringSet>(objectRegistry);
  if (!strings)
  {
    return csApplicationFramework::ReportError("Failed to locate the standard stringset!");
  }
  return true;
}

bool celNavMeshBuilder::SetSector (iSector* sector) {
  CleanUpSectorData();
  currentSector = sector;
  return GetSectorData();
}

// Based on Recast InputGeom::loadMesh
bool celNavMeshBuilder::GetSectorData () 
{
  csList<int> indices;
  csList<float> vertices;
  csStringID base = strings->Request("base");
  csStringID collDet = strings->Request("colldet");
  
  csVector3 v;
  csRef<iMeshList> meshList = currentSector->GetMeshes();
  int numberOfMeshes = meshList->GetCount();
  for (int i = 0; i < numberOfMeshes; i++) 
  {
    csRef<iMeshWrapper> meshWrapper = meshList->Get(i);
    csReversibleTransform transform = meshWrapper->GetMovable()->GetTransform();
    csRef<iObjectModel> objectModel = meshWrapper->GetMeshObject()->GetObjectModel();
    csRef<iTriangleMesh> triangleMesh;
    if (objectModel->IsTriangleDataSet(collDet))
    {
      triangleMesh = objectModel->GetTriangleData(collDet);
    }
    else 
    {
      triangleMesh = objectModel->GetTriangleData(base);
    }

    if (triangleMesh) 
    {
      int numberOfMeshTriangles = triangleMesh->GetTriangleCount();
      if (numberOfMeshTriangles > 0) 
      {
        // Copy triangles
        csTriangle* triangles = triangleMesh->GetTriangles();
        for (int k = 0; k < numberOfMeshTriangles; k++) 
        {
          indices.PushBack(triangles[k][0] + numberOfVertices);
          indices.PushBack(triangles[k][1] + numberOfVertices);
          indices.PushBack(triangles[k][2] + numberOfVertices); 
        }
        numberOfTriangles += numberOfMeshTriangles;

        // Copy vertices
        int numberOfMeshVertices = triangleMesh->GetVertexCount();
        csVector3* meshVertices = triangleMesh->GetVertices();
        for (int k = 0; k < numberOfMeshVertices; k++) 
        {
          v = transform.This2Other(meshVertices[k]);
          vertices.PushBack(v[0]);
          vertices.PushBack(v[1]);
          vertices.PushBack(v[2]);
        }
        numberOfVertices += numberOfMeshVertices;
      }
    }
  }

  // Create fake triangles, normal to the portals
  csSet<csPtrKey<iMeshWrapper>>::GlobalIterator portalMeshesIt = 
      currentSector->GetPortalMeshes().GetIterator();
  while (portalMeshesIt.HasNext())
  {
    csRef<iPortalContainer> container = portalMeshesIt.Next()->GetPortalContainer();
    int size = container->GetPortalCount();
    for (int i = 0; i < size; i++)
    {
      csRef<iPortal> portal = container->GetPortal(i);

      // Get portal indices
      int indicesSize = portal->GetVertexIndicesCount();
      const int* indicesPortal = portal->GetVertexIndices();

      // Get portal vertices
      int verticesSize = portal->GetVerticesCount();
      const csVector3* verticesPortal = portal->GetWorldVertices();

      if (indicesSize >= 3)
      {
        int firstVertexIndex = numberOfVertices;

        // Copy portal vertices
        for (int j = 0; j < verticesSize; j++)
        {
          vertices.PushBack(verticesPortal[j][0]);
          vertices.PushBack(verticesPortal[j][1]);
          vertices.PushBack(verticesPortal[j][2]);
        }
        numberOfVertices += verticesSize;

        const csVector3 direction = portal->GetObjectPlane().Normal() * parameters->GetBorderSize() 
            * parameters->GetCellSize();
        const csVector3 v1 = verticesPortal[indicesPortal[0]];
        const csVector3 v2 = verticesPortal[indicesPortal[1]];

        // For the first triangle, add new vertices for the first and second vertices of current triangle
        csVector3 v = v1 + direction;
        vertices.PushBack(v[0]);
        vertices.PushBack(v[1]);
        vertices.PushBack(v[2]);
        v = v2 + direction;
        vertices.PushBack(v[0]);
        vertices.PushBack(v[1]);
        vertices.PushBack(v[2]);
        
        int lastVertexIndex = numberOfVertices;
        int thisVertexIndex = lastVertexIndex + 1;
        numberOfVertices += 2;

        // Create two triangles for edge v1v2, v1-lastVertexIndex-v2 and v2-lastVertexIndex-thisVertexIndex
        indices.PushBack(indicesPortal[0] + firstVertexIndex);
        indices.PushBack(indicesPortal[1] + firstVertexIndex);
        indices.PushBack(lastVertexIndex);
        indices.PushBack(indicesPortal[1] + firstVertexIndex);
        indices.PushBack(thisVertexIndex);
        indices.PushBack(lastVertexIndex);        
        numberOfTriangles += 2;

        // For each triangle on the triangle fan, create new triangles in the direction of the plane normal
        // We are basically extruding the polygon by agentRadius, in the direction of the polygon normal
        int size2 = indicesSize - 2;
        for (int j = 1; j <= size2; j++)
        {
          lastVertexIndex = thisVertexIndex;
          thisVertexIndex++;
          const csVector3 v3 = verticesPortal[indicesPortal[j + 1]];

          // Add new vertex for the third vertex of current triangle          
          v = v3 + direction;
          vertices.PushBack(v[0]);
          vertices.PushBack(v[1]);
          vertices.PushBack(v[2]);
          numberOfVertices++;

          // Create two triangles for edge v2v3, v2-lastVertexIndex-v3 and v3-lastVertexIndex-thisVertexIndex
          indices.PushBack(indicesPortal[j] + firstVertexIndex);
          indices.PushBack(indicesPortal[j + 1] + firstVertexIndex);
          indices.PushBack(lastVertexIndex);          
          indices.PushBack(indicesPortal[j + 1] + firstVertexIndex);
          indices.PushBack(thisVertexIndex);
          indices.PushBack(lastVertexIndex);          
          numberOfTriangles += 2;
        }

        // Create two triangles for edge v3v1 of the last triangle
        // v3-thisVertexIndex-v1 v1-thisVertexIndex-firstVertexIndex
        indices.PushBack(indicesPortal[indicesSize - 1] + firstVertexIndex);
        indices.PushBack(indicesPortal[0] + firstVertexIndex);
        indices.PushBack(thisVertexIndex);
        indices.PushBack(indicesPortal[0] + firstVertexIndex);
        indices.PushBack(firstVertexIndex);
        indices.PushBack(thisVertexIndex);
        numberOfTriangles += 2;
      }
    }    
  }

  // Copy vertices from list to array
  triangleVertices = new float[numberOfVertices * 3];
  if (!triangleVertices) 
  {
    return csApplicationFramework::ReportError("Out of memory while loading triangle data from sector.");
  }
  int i = 0;
  csList<float>::Iterator verticesIt(vertices);
  while (verticesIt.HasNext()) 
  {
    triangleVertices[i++] = verticesIt.Next();
  }

  // Copy indices from list to array
  triangleIndices = new int[numberOfTriangles * 3];
  if (!triangleIndices) 
  {
    return csApplicationFramework::ReportError("Out of memory while loading triangle data from sector.");
  }
  i = 0;
  csList<int>::Iterator indicesIt(indices);
  while (indicesIt.HasNext()) 
  {
    triangleIndices[i++] = indicesIt.Next();
  }

  // Calculate a bounding box for the map triangles
  rcCalcBounds(triangleVertices, numberOfVertices, boundingMin, boundingMax);

  // ChunkyTriMesh is a structure used by Recast
  chunkyTriMesh = new rcChunkyTriMesh;
  if (!chunkyTriMesh) 
  {
    return csApplicationFramework::ReportError("Out of memory while loading triangle data from sector.");
  }
  if (!rcCreateChunkyTriMesh(triangleVertices, triangleIndices, numberOfTriangles, 256, chunkyTriMesh)) 
  {
    return csApplicationFramework::ReportError("Error creating ChunkyTriMesh.");
  }	

  return true;
}

// Based on Recast Sample_TileMesh::buildAllTiles()
iCelNavMesh* celNavMeshBuilder::BuildNavMesh ()
{
  if (!currentSector) 
  {
    return 0;
  }

  celNavMesh* navMesh = new celNavMesh();
  navMesh->Initialize(parameters, boundingMin, boundingMax);

  const float cellSize = parameters->GetCellSize();
  const int tileSize = parameters->GetTileSize();
  int gridWidth = 0, gridHeight = 0;
  rcCalcGridSize(boundingMin, boundingMax, cellSize, &gridWidth, &gridHeight);
  const int tw = (gridWidth + tileSize - 1) / tileSize;
  const int th = (gridHeight + tileSize - 1) / tileSize;
  const float tcs = tileSize * cellSize;

  rcConfig tileConfig;
  memset(&tileConfig, 0, sizeof(tileConfig));
  tileConfig.cs = cellSize;
  tileConfig.ch = parameters->GetCellHeight();  
  tileConfig.walkableHeight = (int)ceilf(parameters->GetAgentHeight() / tileConfig.ch);
  tileConfig.walkableRadius = (int)ceilf(parameters->GetAgentRadius() / cellSize);
  tileConfig.walkableClimb = (int)floorf(parameters->GetAgentMaxClimb() / tileConfig.ch);  
  tileConfig.walkableSlopeAngle = parameters->GetAgentMaxSlopeAngle();
  tileConfig.maxEdgeLen = (int)(parameters->GetMaxEdgeLength() / cellSize);
  tileConfig.maxSimplificationError = parameters->GetMaxSimplificationError();
  tileConfig.minRegionSize = (int)rcSqr(parameters->GetMinRegionSize());
  tileConfig.mergeRegionSize = (int)rcSqr(parameters->GetMergeRegionSize());
  tileConfig.maxVertsPerPoly = parameters->GetMaxVertsPerPoly();
  tileConfig.tileSize = tileSize;
  tileConfig.borderSize = tileConfig.walkableRadius + 3; // Reserve enough padding.
  tileConfig.width = tileConfig.tileSize + tileConfig.borderSize * 2;
  tileConfig.height = tileConfig.tileSize + tileConfig.borderSize * 2;
  tileConfig.detailSampleDist = parameters->GetDetailSampleDist() < 0.9f ? 0 : cellSize * 
                                parameters->GetDetailSampleDist();
  tileConfig.detailSampleMaxError = tileConfig.ch * parameters->GetDetailSampleMaxError();

  float tileBoundingMin[3];
  float tileBoundingMax[3];
  for (int y = 0; y < th; ++y)
  {
    for (int x = 0; x < tw; ++x)
    {
      tileBoundingMin[0] = boundingMin[0] + x * tcs;
      tileBoundingMin[1] = boundingMin[1];
      tileBoundingMin[2] = boundingMin[2] + y * tcs;

      tileBoundingMax[0] = boundingMin[0] + (x + 1) * tcs;
      tileBoundingMax[1] = boundingMax[1];
      tileBoundingMax[2] = boundingMin[2] + (y + 1) * tcs;

      rcVcopy(tileConfig.bmin, tileBoundingMin);
      rcVcopy(tileConfig.bmax, tileBoundingMax);
      tileConfig.bmin[0] -= tileConfig.borderSize * tileConfig.cs;
      tileConfig.bmin[2] -= tileConfig.borderSize * tileConfig.cs;
      tileConfig.bmax[0] += tileConfig.borderSize * tileConfig.cs;
      tileConfig.bmax[2] += tileConfig.borderSize * tileConfig.cs;

      int dataSize = 0;
      unsigned char* data = BuildTile(x, y, tileBoundingMin, tileBoundingMax, tileConfig, dataSize);
      if (data)
      {
        if (!navMesh->AddTile(data, dataSize))
        {
          delete [] data;
          return 0;
        }
      }
    }
  }

  return navMesh;
}

void celNavMeshBuilder::CleanUpTileData()
{
  delete [] triangleFlags;
  triangleFlags = 0;
  delete solid;
  solid = 0;
  delete chf;
  chf = 0;
  delete cSet;
  cSet = 0;
  delete pMesh;
  pMesh = 0;
  delete dMesh;
  dMesh = 0;
}

// Based on Recast Sample_TileMesh::buildTileMesh()
// NOTE I left the original Recast comments
// TODO update navigation mesh areas
// TODO check offmesh connections
unsigned char* celNavMeshBuilder::BuildTile(const int tx, const int ty, const float* bmin, const float* bmax, 
                                            const rcConfig& tileConfig, int& dataSize)
{

  if (!triangleVertices || !triangleIndices || !chunkyTriMesh)
  {
    csApplicationFramework::ReportError("Tried to build a navigation mesh without having set a sector first.");
    return 0;
  }

  // Make sure memory from last run is freed correctly (so there are no memory leaks if BuildTile crashes)
  CleanUpTileData();

  // Allocate voxel heighfield where we rasterize our input data to.
  solid = new rcHeightfield;
  if (!solid)
  {
    csApplicationFramework::ReportError("Out of memory building navigation mesh.");
    return 0;
  }
  if (!rcCreateHeightfield(*solid, tileConfig.width, tileConfig.height, tileConfig.bmin, tileConfig.bmax, 
                           tileConfig.cs, tileConfig.ch))
  {
    return 0;
  }

  // Allocate array that can hold triangle flags.
  // If you have multiple meshes you need to process, allocate
  // and array which can hold the max number of triangles you need to process.
  triangleFlags = new unsigned char[chunkyTriMesh->maxTrisPerChunk];
  if (!triangleFlags)
  {
    csApplicationFramework::ReportError("Out of memory building navigation mesh.");
    return 0;
  }

  float tbmin[2], tbmax[2];
  tbmin[0] = tileConfig.bmin[0];
  tbmin[1] = tileConfig.bmin[2];
  tbmax[0] = tileConfig.bmax[0];
  tbmax[1] = tileConfig.bmax[2];
  int cid[512];// TODO: Make grow when returning too many items.
  const int ncid = rcGetChunksInRect(chunkyTriMesh, tbmin, tbmax, cid, 512);
  if (!ncid)
  {
    return 0;
  }

  int tileTriangleCount = 0;
  for (int i = 0; i < ncid; ++i)
  {
    const rcChunkyTriMeshNode& node = chunkyTriMesh->nodes[cid[i]];
    const int* tris = &chunkyTriMesh->tris[node.i * 3];
    const int ntris = node.n;

    tileTriangleCount += ntris;

    memset(triangleFlags, 0, ntris * sizeof(unsigned char));
    rcMarkWalkableTriangles(tileConfig.walkableSlopeAngle, triangleVertices, numberOfVertices, tris, 
                            ntris, triangleFlags);

    rcRasterizeTriangles(triangleVertices, numberOfVertices, tris, triangleFlags, ntris, *solid, 
                         tileConfig.walkableClimb);
  }

  delete [] triangleFlags;
  triangleFlags = 0;

  // Once all geoemtry is rasterized, we do initial pass of filtering to
  // remove unwanted overhangs caused by the conservative rasterization
  // as well as filter spans where the character cannot possibly stand.
  rcFilterLowHangingWalkableObstacles(tileConfig.walkableClimb, *solid);
  rcFilterLedgeSpans(tileConfig.walkableHeight, tileConfig.walkableClimb, *solid);
  rcFilterWalkableLowHeightSpans(tileConfig.walkableHeight, *solid);

  // Compact the heightfield so that it is faster to handle from now on.
  // This will result more cache coherent data as well as the neighbours
  // between walkable cells will be calculated.
  chf = new rcCompactHeightfield;
  if (!chf)
  {
    csApplicationFramework::ReportError("Out of memory building navigation mesh.");
    return 0;
  }
  if (!rcBuildCompactHeightfield(tileConfig.walkableHeight, tileConfig.walkableClimb, RC_WALKABLE, *solid, *chf))
  {
    return 0;
  }

  delete solid;
  solid = 0;

  // Erode the walkable area by agent radius.
  if (!rcErodeArea(RC_WALKABLE_AREA, tileConfig.walkableRadius, *chf))
  {
    return 0;
  }

  // (Optional) Mark areas.
  for (int i  = 0; i < numberOfVolumes; ++i)
  {
    rcMarkConvexPolyArea(volumes[i].verts, volumes[i].nverts, volumes[i].hmin, volumes[i].hmax, 
                         (unsigned char)volumes[i].area, *chf);
  }

  // Prepare for region partitioning, by calculating distance field along the walkable surface.
  if (!rcBuildDistanceField(*chf))
  {
    return 0;
  }

  // Partition the walkable surface into simple regions without holes.
  if (!rcBuildRegions(*chf, tileConfig.borderSize, tileConfig.minRegionSize, tileConfig.mergeRegionSize))
  {
    return 0;
  }

  // Create contours.
  cSet = new rcContourSet;
  if (!cSet)
  {
    csApplicationFramework::ReportError("Out of memory building navigation mesh.");
    return 0;
  }
  if (!rcBuildContours(*chf, tileConfig.maxSimplificationError, tileConfig.maxEdgeLen, *cSet))
  {
    return 0;
  }
  if (cSet->nconts == 0)
  {
    return 0;
  }

  // Build polygon navmesh from the contours.
  pMesh = new rcPolyMesh;
  if (!pMesh)
  {
    csApplicationFramework::ReportError("Out of memory building navigation mesh.");
    return 0;
  }
  if (!rcBuildPolyMesh(*cSet, tileConfig.maxVertsPerPoly, *pMesh))
  {
    return 0;
  }

  // Build detail mesh.
  dMesh = new rcPolyMeshDetail;
  if (!dMesh)
  {
    csApplicationFramework::ReportError("Out of memory building navigation mesh.");
    return 0;
  }
  if (!rcBuildPolyMeshDetail(*pMesh, *chf, tileConfig.detailSampleDist, tileConfig.detailSampleMaxError, *dMesh))
  {
    return 0;
  }

  delete chf;
  chf = 0;
  delete cSet;
  cSet = 0;

  unsigned char* navData = 0;
  int navDataSize = 0;
  if (tileConfig.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
  {
    // Remove padding from the polymesh data. TODO: Remove this odditity.
    for (int i = 0; i < pMesh->nverts; ++i)
    {
      unsigned short* v = &pMesh->verts[i * 3];
      v[0] -= (unsigned short)tileConfig.borderSize;
      v[2] -= (unsigned short)tileConfig.borderSize;
    }

    if (pMesh->nverts >= 0xffff)
    {
      // The vertex indices are ushorts, and cannot point to more than 0xffff vertices.
      return 0;
    }

    // TODO areas
    // Update poly flags from areas.
    for (int i = 0; i < pMesh->npolys; ++i)
    {
      if (pMesh->areas[i] == RC_WALKABLE_AREA)
        pMesh->areas[i] = SAMPLE_POLYAREA_GROUND;

      if (pMesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
        pMesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
        pMesh->areas[i] == SAMPLE_POLYAREA_ROAD)
      {
        pMesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
      }
      else if (pMesh->areas[i] == SAMPLE_POLYAREA_WATER)
      {
        pMesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
      }
      else if (pMesh->areas[i] == SAMPLE_POLYAREA_DOOR)
      {
        pMesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
      }
    }

    dtNavMeshCreateParams params;
    memset(&params, 0, sizeof(params));
    params.verts = pMesh->verts;
    params.vertCount = pMesh->nverts;
    params.polys = pMesh->polys;
    params.polyAreas = pMesh->areas;
    params.polyFlags = pMesh->flags;
    params.polyCount = pMesh->npolys;
    params.nvp = pMesh->nvp;
    params.detailMeshes = dMesh->meshes;
    params.detailVerts = dMesh->verts;
    params.detailVertsCount = dMesh->nverts;
    params.detailTris = dMesh->tris;
    params.detailTriCount = dMesh->ntris;
    params.offMeshConVerts = offMeshConVerts;
    params.offMeshConRad = offMeshConRads;
    params.offMeshConDir = offMeshConDirs;
    params.offMeshConAreas = offMeshConAreas;
    params.offMeshConFlags = offMeshConFlags;
    params.offMeshConCount = numberOfOffMeshCon;
    params.walkableHeight = parameters->GetAgentHeight();
    params.walkableRadius = parameters->GetAgentRadius();
    params.walkableClimb = parameters->GetAgentMaxClimb();
    params.tileX = tx;
    params.tileY = ty;
    rcVcopy(params.bmin, bmin);
    rcVcopy(params.bmax, bmax);
    params.cs = tileConfig.cs;
    params.ch = tileConfig.ch;
    params.tileSize = tileConfig.tileSize;

    if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
    {
      return 0;
    }
  }

  delete pMesh;
  pMesh = 0;
  delete dMesh;
  dMesh = 0;

  dataSize = navDataSize;
  return navData;
}

// TODO implement
bool celNavMeshBuilder::UpdateNavMesh (iCelNavMesh* navMesh, const csBox3& boundingBox)
{
  return false;
}

// TODO implement
bool celNavMeshBuilder::UpdateNavMesh (iCelNavMesh* navMesh, const csOBB& boundingBox)
{
  return false;
}

const iCelNavMeshParams* celNavMeshBuilder::GetNavMeshParams () const
{
  return parameters;
}

void celNavMeshBuilder::SetNavMeshParams (const iCelNavMeshParams* parameters)
{
  this->parameters.Invalidate();
  this->parameters.AttachNew(new celNavMeshParams(parameters));
}

}
CS_PLUGIN_NAMESPACE_END(celNavMesh)
