/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_MESH__
#define __CEL_PF_MESH__

#include "cstypes.h"
#include "csutil/scf.h"

struct iMeshWrapper;
struct iSector;
class csVector3;

/**
 * Property ID used when the mesh changes (for use with
 * property change callbacks).
 */
#define CEL_PCMESH_PROPERTY_MESH 1

SCF_VERSION (iPcMesh, 0, 0, 3);

/**
 * This is a property class holding the representation of a mesh.
 */
struct iPcMesh : public iBase
{
  /**
   * Set the mesh filename to use. Returns false on failure.
   */
  virtual bool SetMesh (const char* factname, const char* filename) = 0;

  /**
   * Set the mesh to use directly. It is normally not recommended to use
   * this function directly. iPcRegion will use it for its dummy entities.
   */
  virtual void SetMesh (iMeshWrapper* mesh) = 0;

  /**
   * Create an empty thing mesh (use instead of SetMesh()).
   * After this use GetMesh() to add polygons to the mesh.
   */
  virtual void CreateEmptyThing () = 0;

  /**
   * Get the mesh.
   */
  virtual iMeshWrapper* GetMesh () const = 0;

  /**
   * Convenience function to move the mesh to some sector and position
   * and also update lighting.  If sector specified is NULL, sector
   * is not updated.
   */
  virtual void MoveMesh (iSector* sector, const csVector3& pos) = 0;

  /**
   * Convenience function to set an 'action' for the mesh.
   * This only works for 3D sprites. For other mesh objects nothing
   * will happen.
   */
  virtual void SetAction (const char* actionName, bool reset = false) = 0;

  /**
   * Return the current action name.
   */
  virtual const char* GetAction () = 0;

  /**
   * Convenience function to set an 'action' to be reversed for the mesh.
   * This only works for 3D sprites. For other mesh objects nothing
   * will happen.
   */
  virtual void SetReverseAction (bool reverse = true) = 0;

  /**
   * Hide this mesh.
   */
  virtual void Hide () = 0;

  /**
   * Show this mesh.
   */
  virtual void Show () = 0;

  /**
   * Return true if mesh is visible.
   */
  virtual bool IsVisible () const = 0;
};

#endif // __CEL_PF_MESH__

