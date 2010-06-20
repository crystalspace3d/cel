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

#ifndef __CEL_HPFAPI__
#define __CEL_HPFAPI__

#include <csutil/scf.h>
#include <csutil/list.h>

class csBox3;
class csOBB;
class csString;
class csVector3;
struct iCelNavMeshParams;
struct iCelPath;
struct iSector;
struct iSectorList;




/**
 * Hierarchical navigation structure representing the navigable areas of a Map.
 */
struct iCelHNavStruct : public virtual iBase
{
  SCF_INTERFACE (iCelHNavStruct, 1, 0, 0);

  /**
   * Find the shortest path between two points.
   * \param from Origin coordinates.
   * \param fromSector Origin sector.
   * \param goal Destination coordinates.
   * \param goalSector Destination sector.
   * \return Pointer to the shortest path between the two points, or 0 in case something went wrong.
   */
  virtual iCelPath* ShortestPath (const csVector3& from, iSector* fromSector, const csVector3& goal,
    iSector* goalSector) const = 0;

  /**
   * Find the shortest path between two points.
   * \param from Origin of the path.
   * \param goal Destination of the path.
   * \return Pointer to the shortest path between the two points, or 0 in case something went wrong.
   */
  virtual iCelPath* ShortestPath (iMapNode* from, iMapNode* goal) const = 0;

  /**
   * Save this structure to a file.
   * \param file File location.
   * \return True in case everything went right and false otherwise.
   */
  virtual bool SaveToFile (const csString& file) = 0;

  /**
   * Load this structure from a file.
   * \param file File location.
   * \return True in case everything went right and false otherwise.
   */
  virtual bool LoadFromFile (const csString& file) = 0;

    /**
   * Get an object representation of the navigation mesh parameters.
   * \return Pointer to navigation mesh parameters object.
   */
  virtual iCelNavMeshParams* GetNavMeshParams () = 0;

  /**
   * Set navigation mesh parameters.
   * \remarks If you wish to set a specific parameter, use iCelHNavStructBuilder::GetNavMeshParams()
   * instead and call the appropriate setter method.
   */
  virtual void SetNavMeshParams (iCelNavMeshParams* parameters) = 0;
};



/**
 * Hierarchical navigation struct creator.
 */
struct iCelHNavStructBuilder : public virtual iBase
{
  SCF_INTERFACE (iCelHNavStructBuilder, 1, 0, 0);

  /**
   * Set the Sectors used to build the navigation structure.
   * \param sectorList List containing the sectors for which navmeshes will be built.
   * \return True in case everything went right and false otherwise.
   * \remarks Even in case of a false return, the old sector information is lost.
   */
  virtual void SetSectors(csList<iSector*> sectorList) = 0;

  /**
   * Build a hierarchical navigation structure using the current configurations.
   * \return Pointer to the navigation mesh, or 0 if something went wrong.
   */
  virtual iCelHNavStruct* BuildHNavStruct () = 0;

  /**
   * Update the tiles of the hierarchical navigation structure that intersect with an axis aligned bounding box.
   * \param hNavStruct Pointer to the hierarchical navigation structure.
   * \param boundingBox Bounding box representing the area to be updated.
   * \param sector Only update tiles from this sector, if specified.
   * \return True in case everything went right and false otherwise.
   * \remarks A sector should be specified whenever possible.
   */
  virtual bool UpdateNavMesh (iCelHNavStruct* hNavStruct, const csBox3& boundingBox, iSector* sector = 0) = 0;

  /**
   * Update the tiles of the hierarchical navigation structure that intersect with an oriented bounding box.
   * \param hNavStruct Pointer to the hierarchical navigation structure.
   * \param boundingBox Bounding box representing the area to be updated.
   * \param sector Only update tiles from this sector, if specified.
   * \return True in case everything went right and false otherwise.
   * \remarks A sector should be specified whenever possible.
   */
  virtual bool UpdateNavMesh (iCelHNavStruct* hNavStruct, const csOBB& boundingBox, iSector* sector = 0) = 0;

  /**
   * Get an object representation of the navigation mesh parameters.
   * \return Pointer to navigation mesh parameters object.
   */
  virtual iCelNavMeshParams* GetNavMeshParams () = 0;

  /**
   * Set navigation mesh parameters.
   * \remarks If you wish to set a specific parameter, use iCelHNavStructBuilder::GetNavMeshParams()
   * instead and call the appropriate setter method.
   */
  virtual void SetNavMeshParams (iCelNavMeshParams* parameters) = 0;
};

#endif // __CEL_HPFAPI__