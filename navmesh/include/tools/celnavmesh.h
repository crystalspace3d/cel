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

#ifndef __CEL_NAVMESHAPI__
#define __CEL_NAVMESHAPI__

#include <csutil/scf.h>

class csVector3;
class csOBB;
class csBox3;
struct iSector;



/**
 * Wrapper for the parameters used to build the navigation mesh
 */
struct iCelNavMeshParams : public virtual iBase
{
  /**
   * Create a new object with the same instance variable's values as this one's
   * \remarks Don't assign the returned value directly to a smart pointer. Use the AttachNew()
   * method instead.
   */  
  virtual iCelNavMeshParams* Clone () const = 0;

  /// Set suggested values, given the agent parameters values
  virtual void SetSuggestedValues (float agentHeight, float agentRadius, float agentMaxSlopeAngle) = 0;

  /// Get agent height
  virtual float GetAgentHeight () const = 0;
  /// Set agent height
  virtual void SetAgentHeight (const float height) = 0;

  /// Get agent radius
  virtual float GetAgentRadius () const = 0;
  /// Set agent radius
  virtual void SetAgentRadius (const float radius) = 0;

  /// Get agent maximum walkable slope angle (in degrees)
  virtual float GetAgentMaxSlopeAngle () const = 0;
  /// Set agent maximum walkable slope angle (in degrees)
  virtual void SetAgentMaxSlopeAngle (const float angle) = 0;

  /// Get agent max climb
  virtual float GetAgentMaxClimb () const = 0;
  /// Set agent max climb
  virtual void SetAgentMaxClimb (const float maxClimb) = 0;

  /// Get cell size
  virtual float GetCellSize () const = 0;
  /// Set cell size
  virtual void SetCellsize (float size) = 0;

  /// Get cell height
  virtual float GetCellHeight () const = 0;
  /// Set cell height
  virtual void SetCellHeight (float height) = 0;

  /// Get maximum distance error from contour to cells
  virtual float GetMaxSimplificationError () const = 0;
  /// Set maximum distance error from contour to cells
  virtual void SetMaxSimplificationError (const float error) = 0;

  /// Get detail mesh sample spacing
  virtual float GetDetailSampleDist () const = 0;
  /// Set detail mesh sample spacing
  virtual void SetDetailSampleDist (const float dist) = 0;

  /// Get detail mesh simplification max sample error
  virtual float GetDetailSampleMaxError () const = 0;
  /// Set detail mesh simplification max sample error
  virtual void SetDetailSampleMaxError (const float error) = 0;
  
  /// Get maximum contour edge length
  virtual int GetMaxEdgeLength () const = 0;
  /// Set maximum contour edge length
  virtual void SetMaxEdgeLength (const int length) = 0;

  /// Get minimum regions size (smaller regions will be deleted)
  virtual int GetMinRegionSize () const = 0;
  /// Set minimum regions size (smaller regions will be deleted)
  virtual void SetMinRegionSize (const int size) = 0;

  /// Get minimum regions size (smaller regions will be merged)
  virtual int GetMergeRegionSize () const = 0;
  /// Set minimum regions size (smaller regions will be merged)
  virtual void SetMergeRegionSize (const int size) = 0;

  /// Get max number of vertices per polygon
  virtual int GetMaxVertsPerPoly () const = 0;
  /// Set max number of vertices per polygon
  virtual void SetMaxVertsPerPoly (const int maxVerts) = 0;

  /// Get tile size (Width and Height of a tile)
  virtual int GetTileSize () const = 0;
  /// Set tile size (Width and Height of a tile)
  virtual void SetTilesize (const int size) = 0;

  /// Get border size
  virtual int GetBorderSize () const = 0;
  /// Set border size
  virtual void SetBorderSize (const int size) = 0;
};

/**
 * Path between two points which lay in the same Sector.
 */
struct iCelNavMeshPath : public virtual iBase
{
  SCF_INTERFACE (iCelNavMeshPath, 1, 0, 0);

  /// Get this path's sector
  virtual iSector* GetSector () const = 0;

  /// Get current node from the navigation path
  virtual void Current (csVector3& vector) const = 0;

  /// Get next node from the navigation path
  virtual void Next (csVector3& vector) = 0;

  /// Get previous node from the navigation path
  virtual void Previous (csVector3& vector) = 0;

  /// Get first node from the navigation path
  virtual void GetFirst (csVector3& vector) const = 0;

  /// Get last node from the navigation path
  virtual void GetLast (csVector3& vector) const = 0;

  /// Check if the navigation mesh has nodes after the current one
  virtual bool HasNext () const = 0;

  /// Check if the navigation mesh has nodes before the current one
  virtual bool HasPrevious () const = 0;

  /// Invert the navigation path
  virtual void Invert () = 0;

  /// Restart the navigation path
  virtual void Restart () = 0;

  /// Add a new node at the end of the path
  virtual void AddNode (csVector3 node) = 0;

  /// Add a new node in position pos
  virtual void InsertNode (int pos, csVector3 node) = 0;

  /// Render path
  virtual void DebugRenderPath () = 0;
};



/**
 * Navigation mesh tile.
 */
// This is a wrapper for recast's internal representation of a tile.
struct iCelNavMeshTile : public virtual iBase
{
  SCF_INTERFACE (iCelNavMeshTile, 1, 0, 0);

  /**
   * Get serialized version of the tile data.
   * \param data Serialized data array.
   * \return Size of the data array.
   */
  virtual int GetData (const unsigned char* data) const = 0;

  /**
   * Set tile data in the serialized form.
   * \param data Serialized data array.
   * \param dataSize Size of the data array.
   * \remarks This method gives ownership of the data array to the iCelNavMeshTile 
   * object, and the object will properly dispose of the array in it's destructor.
   */
  virtual void SetData (unsigned char* data, int dataSize) = 0;
};



/**
 * Polygon mesh representing the navigable areas of a Sector.
 */
struct iCelNavMesh : public virtual iBase
{
  SCF_INTERFACE (iCelNavMesh, 1, 0, 0);

  /**
   * Find the shortest path between two points.
   * \param from Origin of the path.
   * \param goal Destination of the path.
   * \return Pointer to the shortest path between the two points, or 0 in case 
   * something went wrong.
   * \remarks Don't assign the returned value directly to a smart pointer. Use the AttachNew()
   * method instead.
   */
  virtual iCelNavMeshPath* ShortestPath (const csVector3& from, const csVector3& goal, 
                                         int maxPathSize = 32) const = 0;

  /**
   * Replace the old tile with the one described in the parameteres.
   * \param tile Pointer to the navigation mesh tile.
   * \return True in case everything went right and false otherwise.
   * \remarks Normally, this method will be called from iCelNavMeshBuilder::UpdateNavMesh().
   */
  virtual bool SetTile (iCelNavMeshTile* tile) = 0;

  /// Get navigation mesh sector
  virtual iSector* GetSector () const = 0;

  /// Set navigation mesh sector
  virtual void SetSector (iSector* sector) = 0;

  /// Get navigation mesh parameters
  virtual iCelNavMeshParams* GetParameters () const = 0;

  /// Render navigation mesh
  virtual void DebugRender () const = 0;

  /// Render proxy agent
  virtual void DebugRenderAgent (const csVector3& pos) const = 0;

  /// Render proxy agent of the specified color
  virtual void DebugRenderAgent(const csVector3& pos, int red, int green, int blue, int alpha) const = 0;
};



/**
 * Navigation mesh creator.
 */
struct iCelNavMeshBuilder : public virtual iBase
{
  SCF_INTERFACE (iCelNavMeshBuilder, 1, 0, 0);

  /**
   * Set an iSector as the current working sector and loads it's triangles.
   * \param sector Pointer to the new iSector
   * \return True in case everything went right and false otherwise.
   * \remarks Even in case of a false return, the old sector information is lost.
   */
  virtual bool SetSector (iSector* sector) = 0;

  /**
   * Build a navigation mesh from the triangles in a Sector, using current configuration.
   * \return Pointer to the navigation mesh, or 0 if something went wrong.
   * \remarks A valid iSector must have been set using iCelNavMeshBuilder::SetSector()
   * before calling this method.
   * Don't assign the returned value directly to a smart pointer. Use the AttachNew()
   * method instead.
   */
  virtual iCelNavMesh* BuildNavMesh () = 0;

  /**
   * Update the tiles of the navigation mesh that intersect with an axis aligned bounding box.
   * \param navMesh Pointer to the navigation mesh.
   * \param boundingBox Bounding box representing the area to be updated.
   * \return True in case everything went right and false otherwise.
   */
  virtual bool UpdateNavMesh (iCelNavMesh* navMesh, const csBox3& boundingBox) = 0;

  /**
   * Update the tiles of the navigation mesh that intersect with an oriented bounding box.
   * \param navMesh Pointer to the navigation mesh.
   * \param boundingBox Bounding box representing the area to be updated.
   * \return True in case everything went right and false otherwise.
   */
  virtual bool UpdateNavMesh (iCelNavMesh* navMesh, const csOBB& boundingBox) = 0;

  /**
   * Get an object representation of the navigation mesh parameters.
   * \return Pointer to navigation mesh parameters object.
   */
  virtual const iCelNavMeshParams* GetNavMeshParams () const = 0;

  /**
   * Set navigation mesh parameters.
   */
  virtual void SetNavMeshParams (const iCelNavMeshParams* parameters) = 0;
 
};

#endif // __CEL_NAVMESHAPI__