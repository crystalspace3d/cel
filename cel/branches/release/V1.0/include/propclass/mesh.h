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
#include "iutil/strset.h"
#include "csutil/scf.h"

struct iMeshWrapper;
struct iSector;
class csVector3;
class csBox3;

/**
 * Property ID used when the mesh changes (for use with
 * property change callbacks).
 */
#define CEL_PCMESH_PROPERTY_MESH 1

/**
 * This is a property class holding the representation of a mesh.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - SetMesh: parameters 'name' (string). This sets an already existing
 *   mesh for this pcmesh.
 * - LoadMesh: parameters 'filename' (string) and 'factoryname' (string).
 *   'filename' is optional. If not given then the factory should already
 *   be in memory.
 * - LoadMeshPath: parameters 'path' (string), 'filename' (string)
 *   and 'factoryname' (string).
 * - MoveMesh: parameters 'sector' (string), 'position' (vector3),
 *   'rotation' (vector3: optional. Represents an absolute rotation).
 * - RotateMesh: 'rotation' (vector3). This is a relative transform.
 * - ClearRotation: no parameters.
 * - LookAt: parameters 'forward' (vector3) and 'up' (vector3).
 * - SetVisible: parameters 'visible' (bool).
 * - SetMaterial: parameters 'material' (string).
 * - SetAnimation: parameters 'animation' (string), 'cycle' (bool:
 *   default false), and 'reset' (bool: default false).
 * - SetShaderVar: parameters 'name' (string), 'type' (bool) and 'value'
 *     (type depending on type parameter).
 * - CreateEmptyThing: parameters 'factoryname' (string)
 * - CreateEmptyGenmesh: parameters 'factoryname' (string)
 * - CreateNullMesh: parameters 'factoryname' (string), 'min' (vector3),
 *   'max' (vector3).
 * - ParentMesh: parameters 'entity' (string) and 'tag' (string).
 *   Parent this mesh to some parent. The parent should be given
 *   as an entity with the optional tag specifying which pcmesh to use.
 *   If entity is not given then the current entity will be used.
 * - ClearParent: no parameters. Remove this mesh from its parent.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - position (vector3, read): current position.
 * - fullposition (vector3, read): current full position.
 * - rotation (vector3, read): current rotation.
 * - eulerrotation (vector3, read): current euler rotation.
 * - sector (string, read): current sector.
 * - path (string, read): path for model.
 * - factory (string, read): factory for model.
 * - filename (string, read): filename for model.
 * - hitbeam (bool, read/write): allow hitbeam for this mesh.
 */
struct iPcMesh : public virtual iBase
{
  SCF_INTERFACE (iPcMesh, 0, 0, 3);

  /**
   * Set the path to use. If this is not done then the filename
   * in SetMesh() will be used alone. Otherwise this path is used to
   * temporarily set the current VFS dir while loading (which will be
   * restored afterwards) and the filename in SetMesh() is used relative
   * to that. Note that this must be called before SetMesh().
   */
  virtual void SetPath (const char* path) = 0;

  /**
   * Set the mesh filename to use. Returns false on failure.
   * Note that this filename can refer to a mesh factory file or
   * a library file containing a mesh factory. If 'filename' is not
   * given (i.e. 0) then it is assumed the factory is already loaded
   * in memory.
   */
  virtual bool SetMesh (const char* factname, const char* filename) = 0;

  /**
   * Set the mesh to use directly. It is normally not recommended to use
   * this function directly. iPcRegion will use it for its dummy entities.
   * \param mesh a pointer to an iMeshWrapper that will be attached to the
   * pcmesh.
   * \param do_remove if true then this pcmesh will remove the mesh
   * on destruction. By default this will not happen if you use SetMesh()
   * on an already existing mesh.
   */
  virtual void SetMesh (iMeshWrapper* mesh, bool do_remove = false) = 0;

  /**
   * Create an empty thing mesh (use instead of SetMesh()).
   * After this use GetMesh() to add polygons to the mesh.
   * Note that if the factory name already exists then the mesh
   * will be created from that factory and this call becomes
   * equivalent to SetMesh(factname,0).
   */
  virtual void CreateEmptyThing (const char* factname) = 0;

  /**
   * Create an empty genmesh (use instead of SetMesh()).
   * Note that if the factory name already exists then the mesh
   * will be created from that factory and this call becomes
   * equivalent to SetMesh(factname,0).
   */
  virtual void CreateEmptyGenmesh (const char* factname) = 0;

  /**
   * Create a nullmesh (use instead of SetMesh()).
   * Note that if the factory name already exists then the mesh
   * will be created from that factory and this call becomes
   * equivalent to SetMesh(factname,0).
   * A nullmesh can be used in mesh hierarchies.
   */
  virtual void CreateNullMesh (const char* factname,
      const csBox3& box) = 0;

  /**
   * Get the mesh.
   */
  virtual iMeshWrapper* GetMesh () const = 0;

  /**
   * Convenience function to move the mesh to some sector and position
   * and also update lighting.  If sector specified is 0, sector
   * is not updated.
   */
  virtual void MoveMesh (iSector* sector, const csVector3& pos) = 0;

  /**
   * Convenience function to set an 'action' for the mesh.
   * This only works for 3D sprites. For other mesh objects nothing
   * will happen.
   */
  CS_DEPRECATED_METHOD virtual void SetAction (const char* actionName,
      bool reset = false) = 0;
  /**
   * Return the current action name.
   */
  CS_DEPRECATED_METHOD virtual const char* GetAction () = 0;

  /**
   * Convenience function to set an 'action' to be reversed for the mesh.
   * This only works for 3D sprites. For other mesh objects nothing
   * will happen.
   */
  CS_DEPRECATED_METHOD virtual void SetReverseAction (bool reverse = true) = 0;

  /**
   * Function to set an 'action' for the mesh.
   * Works for cal3d, spr3d and genmeshskelanim, but note not all mesh objects
   * support the same feature set.
   */
  virtual void SetAnimation (const char* actionName, bool cycle,
      float weight=1.0, float fadein=0.1, float fadeout=0.1,
      bool reset = false) = 0;

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

