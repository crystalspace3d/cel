/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_DYNWORLD__
#define __CEL_PF_DYNWORLD__

#include "csutil/scf.h"

class csBox3;
class csVector3;
class csReversibleTransform;
struct iCamera;
struct iSector;
struct iDynamicSystem;
struct iRigidBody;
struct iString;
struct iMeshWrapper;
struct iELCM;
struct iCelParameterBlock;
struct iCelEntity;
struct iDataBuffer;
struct iDynamicCell;

/**
 * Geometry generator. This is typically attached to factories
 * in case some geometry has to be generated at the moment a mesh
 * is created from the factory.
 */
struct iGeometryGenerator : public virtual iBase
{
  SCF_INTERFACE(iGeometryGenerator,0,0,1);

  virtual void GenerateGeometry (iMeshWrapper* mesh) = 0;
};

/**
 * This interface can be implemented by the application to control
 * how cells are created. If this is not implemented then all cells
 * should be created in advance before the dynworld plugin can use
 * them.
 */
struct iDynamicCellCreator : public virtual iBase
{
  SCF_INTERFACE(iDynamicCellCreator,0,0,1);

  /**
   * The DynWorld plugin needs a cell with the given name.
   * If the application cannot provide a cell with this name
   * then it should return 0 here. This function should only
   * create the cell and fixed geometry but nothing more.
   * The pre-baseline dynamic objects should be created by
   * FillCell().
   */
  virtual iDynamicCell* CreateCell (const char* name) = 0;

  /**
   * Fill a cell with pre-baseline dynamic objects.
   */
  virtual void FillCell (iDynamicCell* cell) = 0;
};

/**
 * A factory object in the dynamic world.
 */
struct iDynamicFactory : public virtual iBase
{
  SCF_INTERFACE(iDynamicFactory,0,0,1);

  virtual const char* GetName () const = 0;

  /**
   * Set the entity template that is linked to this factory.
   * This represents the default entity template to be used when
   * creating entities from this dynamic object.
   */
  virtual void SetDefaultEntityTemplate (const char* tmpName) = 0;

  /**
   * Get the default entity template name or 0 if none.
   */
  virtual const char* GetDefaultEntityTemplate () const = 0;

  /**
   * Set a geometry generator that is going to be used as soon
   * as a mesh is created from this factory.
   */
  virtual void SetGeometryGenerator (iGeometryGenerator* gen) = 0;
  virtual iGeometryGenerator* GetGeometryGenerator () const = 0;

  /**
   * Generic attribute system.
   */
  virtual void SetAttribute (const char* name, const char* value) = 0;

  /**
   * Get an attribute.
   */
  virtual const char* GetAttribute (const char* name) const = 0;

  /**
   * Get the maximum relative radius for this factory.
   */
  virtual float GetMaximumRadiusRelative () const = 0;

  /**
   * Get the bounding box in object space as reported by iObjectModel
   * for the given mesh factory.
   */
  virtual const csBox3& GetBBox () const = 0;

  /**
   * Get the bounding box as calculated from the physical objects that
   * have been attached to this factory.
   */
  virtual const csBox3& GetPhysicsBBox () const = 0;

  /**
   * Create a box rigid body.
   */
  virtual void AddRigidBox (const csVector3& offset, const csVector3& size,
      float mass) = 0;

  /**
   * Create a sphere rigid body.
   */
  virtual void AddRigidSphere (float radius, const csVector3& offset,
      float mass) = 0;

  /**
   * Create a sphere rigid body.
   */
  virtual void AddRigidCylinder (float radius, float length,
      const csVector3& offset, float mass) = 0;

  /**
   * Create a mesh rigid body.
   */
  virtual void AddRigidMesh (const csVector3& offset, float mass) = 0;

  /**
   * Create a convex mesh rigid body.
   */
  virtual void AddRigidConvexMesh (const csVector3& offset, float mass) = 0;
};

/**
 * An object in the dynamic world.
 */
struct iDynamicObject : public virtual iBase
{
  SCF_INTERFACE(iDynamicObject,0,0,1);

  /**
   * Get the factory from which this dynamic object was created.
   */
  virtual iDynamicFactory* GetFactory () const = 0;

  /**
   * Get the cell where this dynamic object lives.
   */
  virtual iDynamicCell* GetCell () const = 0;

  /**
   * Make static.
   */
  virtual void MakeStatic () = 0;

  /**
   * Make dynamic.
   */
  virtual void MakeDynamic () = 0;

  /**
   * Make kinematic.
   */
  virtual void MakeKinematic () = 0;

  /**
   * Undo kinematic and restore previous static or dynamic state.
   */
  virtual void UndoKinematic () = 0;

  /**
   * Is static?
   */
  virtual bool IsStatic () const = 0;

  /**
   * Set hilight.
   */
  virtual void SetHilight (bool hi) = 0;

  /**
   * Check hilight.
   */
  virtual bool IsHilight () const = 0;

  /**
   * Get the mesh for this object. Can be 0 if the
   * object is currently not visible.
   */
  virtual iMeshWrapper* GetMesh () const = 0;

  /**
   * Get the body for this object. Can be 0 if the
   * object is currently not visible.
   */
  virtual iRigidBody* GetBody () const = 0;

  /**
   * Refresh the colliders for this object.
   */
  virtual void RefreshColliders () = 0;

  /**
   * Get the transform of this object.
   */
  virtual const csReversibleTransform& GetTransform () = 0;

  /**
   * Set the transform of this object. It is usually recommended
   * to make the object kinematic before you do this. This function
   * will not automatically do that.
   */
  virtual void SetTransform (const csReversibleTransform& trans) = 0;

  /**
   * Set the entity name to use for this object.
   * Returns false if this fails (for example, there is no entity
   * template with the given name).
   */
  virtual bool SetEntity (const char* entityName, const char* entityTplName, iCelParameterBlock* params) = 0;

  /**
   * Link this dynamic object with the given entity.
   */
  virtual void LinkEntity (iCelEntity* entity) = 0;

  /**
   * Unlink the entity from this object. This is useful if you want to
   * put the entity in some inventory so you have to delete the dynobj afterwards.
   */
  virtual void UnlinkEntity () = 0;

  /**
   * Get the (optional) entity for this dynamic object. If the dynamic object
   * is out of reach it is possible that the entity is not created yet.
   */
  virtual iCelEntity* GetEntity () const = 0;

  /**
   * Force creation of the entity.
   */
  virtual iCelEntity* ForceEntity () = 0;

  /**
   * Return a one-line string briefly describing this dynamic object.
   */
  virtual csPtr<iString> GetDescription () const = 0;
};

/**
 * This iterator is returned by iPcDynamicWorld->GetCells().
 */
struct iDynamicCellIterator : public virtual iBase
{
  SCF_INTERFACE(iDynamicCellIterator,0,0,1);

  virtual bool HasNext () const = 0;
  virtual iDynamicCell* NextCell () = 0;
};

/**
 * A Cell.
 */
struct iDynamicCell : public virtual iBase
{
  SCF_INTERFACE(iDynamicCell,0,0,1);

  virtual const char* GetName () const = 0;

  /**
   * Add a new dynamic object to the cell. This object will have no physics
   * properties yet.
   */
  virtual iDynamicObject* AddObject (const char* factory,
      const csReversibleTransform& trans) = 0;

  /**
   * Get the number of objects.
   */
  virtual size_t GetObjectCount () const = 0;

  /**
   * Remove an object.
   */
  virtual void DeleteObject (iDynamicObject* dynobj) = 0;
  /**
   * Delete all objects in this cell.
   */
  virtual void DeleteObjects () = 0;

  /**
   * Save the cell to XML.
   */
  virtual void Save (iDocumentNode* node) = 0;
  /**
   * Load the cell from XML.
   */
  virtual csRef<iString> Load (iDocumentNode* node) = 0;

  /**
   * Mark the baseline.
   */
  virtual void MarkBaseline () = 0;

  /**
   * Get the sector for this cell.
   */
  virtual iSector* GetSector () const = 0;

  /**
   * Get the dynamic system for this cell.
   */
  virtual iDynamicSystem* GetDynamicSystem () const = 0;
};

/**
 * Interface to the dynamic world plugin.
 */
struct iPcDynamicWorld : public virtual iBase
{
  SCF_INTERFACE(iPcDynamicWorld,0,0,1);

  /**
   * Set an optional ELCM plugin to use for managing life time of entities.
   */
  virtual void SetELCM (iELCM* elcm) = 0;

  //------------------------------------------------------------------------------

  /**
   * Add a new cell to the world.
   * If dynSys is 0 then this function will create its own dynamic system and
   * it will also destroy it in case the cell is removed.
   */
  virtual iDynamicCell* AddCell (const char* name, iSector* sector,
      iDynamicSystem* dynSys = 0) = 0;

  /**
   * Find a cell by name.
   */
  virtual iDynamicCell* FindCell (const char* name) = 0;

  /**
   * Remove a cell.
   */
  virtual void RemoveCell (iDynamicCell* cell) = 0;

  /**
   * Set a cell as current.
   */
  virtual void SetCurrentCell (iDynamicCell* cell) = 0;

  /**
   * Get the current cell.
   */
  virtual iDynamicCell* GetCurrentCell () const = 0;

  /**
   * Get an iterator to iterate over all cells.
   */
  virtual csPtr<iDynamicCellIterator> GetCells () const = 0;

  /**
   * Set a dynamic cell creator to be used when the dynamic world plugin
   * needs to create a cell.
   */
  virtual void SetDynamicCellCreator (iDynamicCellCreator* creator) = 0;

  //------------------------------------------------------------------------------

  /**
   * Add a new dynamic factory to the world.
   * @param maxradius is a relative maximum radius (0 to 1) at which point
   * the object should become visible. It is relative to the total maximum
   * radius maintained by this world.
   * @param imposterradius is the relative radius (0 to 1) at which the object
   * should be impostered. Set to negative if you don't want to use imposters at all.
   * It is also relative to the total maximum radius maintained by this world.
   * Note that imposterradius should always be less then maxradius.
   */
  virtual iDynamicFactory* AddFactory (const char* factory, float maxradius,
      float imposterradius) = 0;

  /**
   * Remove a factory from the world.
   */
  virtual void RemoveFactory (iDynamicFactory* factory) = 0;

  /// Remove all factories.
  virtual void DeleteFactories () = 0;

  /**
   * Get the number of factories.
   */
  virtual size_t GetFactoryCount () const = 0;

  /**
   * Get a factory.
   */
  virtual iDynamicFactory* GetFactory (size_t index) const = 0;

  /**
   * Find a factory.
   */
  virtual iDynamicFactory* FindFactory (const char* name) const = 0;

  //------------------------------------------------------------------------------

  /**
   * Force a dynamic object to be visible. This will force the
   * creation of the mesh.
   */
  virtual void ForceVisible (iDynamicObject* dynobj) = 0;

  /**
   * Force a dynamic object to be invisible. This will force the
   * removal of the mesh.
   */
  virtual void ForceInvisible (iDynamicObject* dynobj) = 0;

  /**
   * Delete everything.
   */
  virtual void DeleteAll () = 0;

  /**
   * Set the view radius. Default radius is 20.
   */
  virtual void SetRadius (float radius) = 0;

  /**
   * Get the view radius.
   */
  virtual float GetRadius () const = 0;

  /**
   * Prepare the sector for viewing at a certain location.
   */
  virtual void PrepareView (iCamera* camera, float elapsed_time) = 0;

  /**
   * Find a dynamic object from the entity.
   */
  virtual iDynamicObject* FindObject (iCelEntity* entity) const = 0;
  /**
   * Find an object given its rigid body.
   */
  virtual iDynamicObject* FindObject (iRigidBody* body) const = 0;
  /**
   * Find an object given its mesh.
   */
  virtual iDynamicObject* FindObject (iMeshWrapper* mesh) const = 0;

  /**
   * Save the world to XML.
   */
  virtual void Save (iDocumentNode* node) = 0;

  /**
   * Load the world from XML.
   * Return 0 on success or otherwise a string with the error.
   */
  virtual csRef<iString> Load (iDocumentNode* node) = 0;

  /**
   * Mark the baseline.
   */
  virtual void MarkBaseline () = 0;

  /**
   * Save the modifications since the baseline.
   */
  virtual csPtr<iDataBuffer> SaveModifications () = 0;

  /**
   * Restore a save file from the baseline.
   */
  virtual void RestoreModifications (iDataBuffer* buf) = 0;

  /**
   * For debugging: dump information about the DynWorld status.
   */
  virtual void Dump () = 0;
};

#endif // __CEL_PF_DYNWORLD__

