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

#ifndef __CEL_NAVMESH__
#define __CEL_NAVMESH__

#include <cssysdef.h>
#if defined(CS_OPENGL_PATH)
#include CS_HEADER_GLOBAL(CS_OPENGL_PATH,gl.h)
#else
#include <GL/gl.h>
#endif
#include <csgeom/plane3.h>
#include <csgeom/tri.h>
#include <csgeom/vector3.h>
#include <csqsqrt.h>
#include <cstool/csapplicationframework.h>
#include <csutil/list.h>
#include <csutil/ref.h>
#include <csutil/scf_implementation.h>
#include <iengine/mesh.h>
#include <iengine/movable.h>
#include <iengine/portal.h>
#include <iengine/portalcontainer.h>
#include <iengine/sector.h>
#include <igeom/trimesh.h>
#include <imesh/objmodel.h>
#include <iutil/comp.h>
#include <iutil/objreg.h>
#include <tools/celnavmesh.h>
#include "ChunkyTriMesh.h"
#include "DebugDraw.h"
#include "DetourDebugDraw.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "Recast.h"

CS_PLUGIN_NAMESPACE_BEGIN(celNavMesh)
{

// TODO remove
#include <iutil/object.h>



/*
 * Recast structures
 */
static const int MAX_CONVEXVOL_PTS = 12;
struct ConvexVolume
{
  float verts[MAX_CONVEXVOL_PTS * 3];
  float hmin, hmax;
  int nverts;
  int area;
};

// These are just sample areas to use consistent values across the samples.
// The use should specify these base on his needs.
enum SamplePolyAreas
{
  SAMPLE_POLYAREA_GROUND,
  SAMPLE_POLYAREA_WATER,
  SAMPLE_POLYAREA_ROAD,
  SAMPLE_POLYAREA_DOOR,
  SAMPLE_POLYAREA_GRASS,
  SAMPLE_POLYAREA_JUMP,
};
enum SamplePolyFlags
{
  SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
  SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
  SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
  SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
  SAMPLE_POLYFLAGS_ALL = 0xffff		// All abilities.
};

// OpenGL debug draw implementation.
struct DebugDrawGL : public duDebugDraw
{
  virtual void depthMask (bool state);
  virtual void begin (duDebugDrawPrimitives prim, float size = 1.0f);
  virtual void vertex (const float* pos, unsigned int color);
  virtual void vertex (const float x, const float y, const float z, unsigned int color);
  virtual void end ();
};



/**
 * Wrapper for the parameters used to build the navigation mesh
 */
class celNavMeshParams : public scfImplementation1<celNavMeshParams, iCelNavMeshParams>
{
private:
  float agentHeight;
  float agentRadius;
  float agentMaxSlopeAngle;
  float agentMaxClimb;
  float cellSize;
  float cellHeight;
  float maxSimplificationError;
  float detailSampleDist;
  float detailSampleMaxError;
  int maxEdgeLength;  
  int minRegionSize;
  int mergeRegionSize;
  int maxVertsPerPoly;
  int tileSize;
  int borderSize;  

public:
  celNavMeshParams ();
  celNavMeshParams (const iCelNavMeshParams* parameters);
  virtual ~celNavMeshParams ();

  iCelNavMeshParams* Clone () const;
  virtual void SetSuggestedValues (float agentHeight, float agentRadius, float agentMaxSlopeAngle);

  virtual float GetAgentHeight () const;
  virtual void SetAgentHeight (const float height);
  virtual float GetAgentRadius () const;
  virtual void SetAgentRadius (const float radius);
  virtual float GetAgentMaxSlopeAngle () const;
  virtual void SetAgentMaxSlopeAngle (const float angle);
  virtual float GetAgentMaxClimb () const;
  virtual void SetAgentMaxClimb (const float maxClimb);
  virtual float GetCellSize () const;
  virtual void SetCellsize (float size);
  virtual float GetCellHeight () const;
  virtual void SetCellHeight (float height);
  virtual float GetMaxSimplificationError () const;
  virtual void SetMaxSimplificationError (const float error);
  virtual float GetDetailSampleDist () const;
  virtual void SetDetailSampleDist (const float dist);
  virtual float GetDetailSampleMaxError () const;
  virtual void SetDetailSampleMaxError (const float error);
  virtual int GetMaxEdgeLength () const;
  virtual void SetMaxEdgeLength (const int length);
  virtual int GetMinRegionSize () const;
  virtual void SetMinRegionSize (const int size);
  virtual int GetMergeRegionSize () const;
  virtual void SetMergeRegionSize (const int size);
  virtual int GetMaxVertsPerPoly () const;
  virtual void SetMaxVertsPerPoly (const int maxVerts);
  virtual int GetTileSize () const;
  virtual void SetTilesize (const int size);
  virtual int GetBorderSize () const;
  virtual void SetBorderSize (const int size);
};



/**
 * Path between two points which lay in the same Sector.
 */
// Based on Recast NavMeshTesterTool
class celNavMeshPath : public scfImplementation1<celNavMeshPath, iCelNavMeshPath>
{
private:
  float* path;
  int pathSize; // In nodes
  int maxPathSize;
  int currentPosition; // Path array position, not point index
  int increasePosition; // Value to be added to currentPosition to get next element
  static const int INCREASE_PATH_BY; // Increase path vector by this amount when the it gets full

public:
  celNavMeshPath (float* path, int pathSize, int maxPathSize);
  virtual ~celNavMeshPath ();

  virtual iSector* GetSector () const;
  virtual void Current (csVector3& vector) const;
  virtual void Next (csVector3& vector);
  virtual void Previous (csVector3& vector);
  virtual void GetFirst (csVector3& vector) const;
  virtual void GetLast (csVector3& vector) const;
  virtual bool HasNext () const;
  virtual bool HasPrevious () const;
  virtual void Invert ();
  virtual void Restart ();
  virtual void AddNode (csVector3 node);  
  virtual void InsertNode (int pos, csVector3 node);
  virtual float Length() const;
  virtual void DebugRenderPath ();
};



/**
 * Navigation mesh tile.
 */
class celNavMeshTile : public scfImplementation1<celNavMeshTile, iCelNavMeshTile>
{
private:
  unsigned char* data;
  int dataSize;

public:
  celNavMeshTile ();
  virtual ~celNavMeshTile ();

  virtual int GetData (const unsigned char* data) const;
  virtual void SetData (unsigned char* data, int dataSize);
};



/**
 * Polygon mesh representing the navigable areas of a Sector.
 */
class celNavMesh : public scfImplementation1<celNavMesh, iCelNavMesh>
{
private:
  csRef<iSector> sector;
  float polyPickExt[3];
  dtQueryFilter filter;
  dtNavMesh* detourNavMesh;
  csRef<iCelNavMeshParams> parameters;
  float boundingMin[3];
  float boundingMax[3];
  unsigned char navMeshDrawFlags;
  static const int MAX_NODES;

public:
  celNavMesh ();
  virtual ~celNavMesh ();

  bool Initialize (const iCelNavMeshParams* parameters, iSector* sector, const float* boundingMin, const float* boundingMax);
  bool AddTile (unsigned char* data, int dataSize);

  // API
  virtual iCelNavMeshPath* ShortestPath (const csVector3& from, const csVector3& goal, int maxPathSize = 32) const;
  virtual bool SetTile (iCelNavMeshTile* tile);  
  virtual iSector* GetSector () const;
  virtual void SetSector (iSector* sector);
  virtual iCelNavMeshParams* GetParameters () const;
  virtual void DebugRender () const;
  virtual void DebugRenderAgent (const csVector3& pos) const;
  virtual void DebugRenderAgent (const csVector3& pos, int red, int green, int blue, int alpha) const;
};



/**
 * Navigation mesh creator.
 */
class celNavMeshBuilder : public scfImplementation2<celNavMeshBuilder, iCelNavMeshBuilder, iComponent>
{
private:
  // Crystal space & CEL
  csRef<iObjectRegistry> objectRegistry;
  csRef<iSector> currentSector;
  csRef<iStringSet> strings;

  // Recast & Detour
  rcChunkyTriMesh* chunkyTriMesh;
  
  // Tile specific
  unsigned char* triangleFlags;
  rcHeightfield* solid;
  rcCompactHeightfield* chf;
  rcContourSet* cSet;
  rcPolyMesh* pMesh;
  rcPolyMeshDetail* dMesh;
  
  // Off-Mesh connections.
  static const int MAX_OFFMESH_CONNECTIONS = 256;
  float offMeshConVerts[MAX_OFFMESH_CONNECTIONS*3*2];
  float offMeshConRads[MAX_OFFMESH_CONNECTIONS];
  unsigned char offMeshConDirs[MAX_OFFMESH_CONNECTIONS];
  unsigned char offMeshConAreas[MAX_OFFMESH_CONNECTIONS];
  unsigned short offMeshConFlags[MAX_OFFMESH_CONNECTIONS];
  int numberOfOffMeshCon;

  // Convex Volumes
  static const int MAX_VOLUMES = 256;
  ConvexVolume volumes[MAX_VOLUMES];
  int numberOfVolumes;

  csRef<iCelNavMeshParams> parameters;

  // Others
  int numberOfVertices;
  float* triangleVertices;
  int numberOfTriangles;
  int* triangleIndices;
  float boundingMin[3];
  float boundingMax[3];

  void CleanUpSectorData ();
  void CleanUpTileData ();
  bool GetSectorData ();
  unsigned char* BuildTile(const int tx, const int ty, const float* bmin, const float* bmax, 
                           const rcConfig& tileConfig, int& dataSize);

public:
  celNavMeshBuilder (iBase* parent);
  virtual ~celNavMeshBuilder ();
  virtual bool Initialize (iObjectRegistry*);

  // API
  virtual bool SetSector (iSector* sector);
  virtual iCelNavMesh* BuildNavMesh ();
  virtual bool UpdateNavMesh (iCelNavMesh* navMesh, const csBox3& boundingBox);
  virtual bool UpdateNavMesh (iCelNavMesh* navMesh, const csOBB& boundingBox);
  virtual const iCelNavMeshParams* GetNavMeshParams () const;
  virtual void SetNavMeshParams (const iCelNavMeshParams* parameters);
  virtual iSector* GetSector () const;

};

}
CS_PLUGIN_NAMESPACE_END(celNavMesh)

#endif // __CEL_NAVMESH__
