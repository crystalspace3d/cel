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

#include "ivaria/dynamics.h"

class csBox3;
class csVector3;
class csReversibleTransform;
class csOrthoTransform;
struct iCamera;
struct iSector;
struct iDynamicSystem;
struct iRigidBody;
struct iString;
struct iMeshWrapper;
struct iELCM;
struct iCelParameterBlock;
struct iCelEntity;
struct iCelEntityTemplate;
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
 * Information for a body attached to a factory.
 */
struct celBodyInfo
{
  csColliderGeometryType type;
  csVector3 offset;
  float mass;
  float radius;	// Only for cylinder and sphere.
  float length; // Only for cylinder.
  csVector3 size; // Only for box.
};

/**
 * A definition for a joint. This is used on a dynamic
 * factory.
 * @@@ Note: this class should not be needed if the physics library
 * on the CS side supported the notion of 'factories'.
 */
class DynFactJointDefinition
{
public:
  csOrthoTransform trans;
  bool transX, transY, transZ;
  csVector3 mindist, maxdist;
  bool rotX, rotY, rotZ;
  csVector3 minrot, maxrot;
  csVector3 bounce;
  csVector3 maxforce;

public:
  DynFactJointDefinition ()
  {
    SetTransConstraints (false, false, false);
    SetRotConstraints (false, false, false);
    SetMinMaxDistance (csVector3 (0), csVector3 (1000.0f));
    SetMinMaxAngle (csVector3 (-100.0f), csVector3 (100.0f));
    SetBounce (csVector3 (0));
    SetMaxForce (csVector3 (0.1f));
  }

  /**
   * Set the local transformation of this joint relative to the object.
   */
  void SetTransform (const csOrthoTransform& trans) { this->trans = trans; }
  const csOrthoTransform& GetTransform () const { return trans; }

  /**
   * Set translation constraints.
   */
  void SetTransConstraints (bool x, bool y, bool z)
  {
    transX = x;
    transY = y;
    transZ = z;
  }
  bool IsXTransConstrained () const { return transX; }
  bool IsYTransConstrained () const { return transY; }
  bool IsZTransConstrained () const { return transZ; }

  /**
   * Set minimum and maximum allowed distance between the two bodies.
   */
  void SetMinMaxDistance (const csVector3& min, const csVector3& max)
  {
    mindist = min;
    maxdist = max;
  }
  const csVector3& GetMinimumDistance () const { return mindist; }
  const csVector3& GetMaximumDistance () const { return maxdist; }

  /**
   * Set rotational constraints.
   */
  void SetRotConstraints (bool x, bool y, bool z)
  {
    rotX = x;
    rotY = y;
    rotZ = z;
  }
  bool IsXRotConstrained () const { return rotX; }
  bool IsYRotConstrained () const { return rotY; }
  bool IsZRotConstrained () const { return rotZ; }

  /**
   * Set minimum and maximum allowed angle between the two bodies (radians).
   */
  void SetMinMaxAngle (const csVector3& min, const csVector3& max)
  {
    minrot = min;
    maxrot = max;
  }
  const csVector3& GetMinimumAngle () const { return minrot; }
  const csVector3& GetMaximumAngle () const { return maxrot; }

  /**
   * Set the restitution of the joint's stop point. This is the
   * amount of bounciness when the joint hits a limit.
   */
  void SetBounce (const csVector3& bounce) { this->bounce = bounce; }
  const csVector3& GetBounce () const { return bounce; }

  /**
   * Set the maximum force that can be applied by the joint motor.
   */
  void SetMaxForce (const csVector3& maxforce) { this->maxforce = maxforce; }
  const csVector3& GetMaxForce () const { return maxforce; }
};

/**
 * This iterator is returned by iDynamicFactory->GetAttributes().
 */
struct iAttributeIterator : public virtual iBase
{
  SCF_INTERFACE(iAttributeIterator,0,0,1);

  virtual bool HasNext () const = 0;
  /// Returns a string ID representing the attribute.
  virtual csStringID Next () = 0;
};

/**
 * A factory object in the dynamic world.
 */
struct iDynamicFactory : public virtual iBase
{
  SCF_INTERFACE(iDynamicFactory,0,0,1);

  virtual const char* GetName () const = 0;

  /**
   * Return true if this dynamic factory is a logic factory
   * (created with AddLogicFactory()).
   */
  virtual bool IsLogicFactory () const = 0;

  /**
   * Return true if this dynamic factory is a light factory
   * (created with AddLightFactory()).
   */
  virtual bool IsLightFactory () const = 0;

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
  virtual void SetAttribute (csStringID nameID, const char* value) = 0;
  virtual void SetAttribute (const char* name, const char* value) = 0;

  /**
   * Clear a given attribute.
   */
  virtual void ClearAttribute (csStringID nameID) = 0;
  virtual void ClearAttribute (const char* name) = 0;

  /**
   * Get an attribute.
   */
  virtual const char* GetAttribute (csStringID nameID) const = 0;
  virtual const char* GetAttribute (const char* name) const = 0;

  /**
   * Get all attributes.
   */
  virtual csPtr<iAttributeIterator> GetAttributes () const = 0;

  /**
   * Get the maximum relative radius for this factory.
   */
  virtual float GetMaximumRadiusRelative () const = 0;
  virtual void SetMaximumRadiusRelative (float r) = 0;

  /**
   * Get the imposter radius. If 0.0 then impostering is
   * disabled (imposters are currently not implemented yet).
   */
  virtual float GetImposterRadius () const = 0;
  virtual void SetImposterRadius (float r) = 0;

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

  // ------------------------------------------------------------------------

  /**
   * Create a box rigid body.
   * If the optional index is given then this body will replace the
   * body that was at that specific index.
   */
  virtual void AddRigidBox (const csVector3& offset, const csVector3& size,
      float mass, size_t idx = csArrayItemNotFound) = 0;

  /**
   * Create a sphere rigid body.
   * If the optional index is given then this body will replace the
   * body that was at that specific index.
   */
  virtual void AddRigidSphere (float radius, const csVector3& offset,
      float mass, size_t idx = csArrayItemNotFound) = 0;

  /**
   * Create a cylinder rigid body.
   * If the optional index is given then this body will replace the
   * body that was at that specific index.
   */
  virtual void AddRigidCylinder (float radius, float length,
      const csVector3& offset, float mass, size_t idx = csArrayItemNotFound) = 0;

  /**
   * Create a capsule rigid body.
   * If the optional index is given then this body will replace the
   * body that was at that specific index.
   */
  virtual void AddRigidCapsule (float radius, float length,
      const csVector3& offset, float mass, size_t idx = csArrayItemNotFound) = 0;

  /**
   * Create a mesh rigid body.
   * If the optional index is given then this body will replace the
   * body that was at that specific index.
   */
  virtual void AddRigidMesh (const csVector3& offset, float mass,
      size_t idx = csArrayItemNotFound) = 0;

  /**
   * Create a convex mesh rigid body.
   * If the optional index is given then this body will replace the
   * body that was at that specific index.
   */
  virtual void AddRigidConvexMesh (const csVector3& offset, float mass,
      size_t idx = csArrayItemNotFound) = 0;

  /**
   * Get the number of bodies.
   */
  virtual size_t GetBodyCount () const = 0;

  /**
   * Get the body.
   */
  virtual celBodyInfo GetBody (size_t idx) const = 0;

  /**
   * Delete a body at a specific index.
   */
  virtual void DeleteBody (size_t idx) = 0;
  
  /**
   * Delete all bodies.
   */
  virtual void DeleteBodies () = 0;

  // ------------------------------------------------------------------------

  /**
   * Create a new joint and return a reference to the data so that it
   * can be set up.
   */
  virtual DynFactJointDefinition& CreateJoint () = 0;

  /**
   * Get the amount of joints.
   */
  virtual size_t GetJointCount () const = 0;

  /**
   * Get a specific joint. The returned reference can be modified freely
   * but objects created using this factory will not be updated automatically.
   */
  virtual DynFactJointDefinition& GetJoint (size_t idx) = 0;

  /**
   * Set joint parameters.
   */
  virtual void SetJoint (size_t idx, const DynFactJointDefinition& def) = 0;

  /**
   * Remove a joint.
   */
  virtual void RemoveJoint (size_t idx) = 0;

  /**
   * Remove all joints.
   */
  virtual void RemoveJoints () = 0;

  // ------------------------------------------------------------------------

  /**
   * Create a pivot joint at a specific object space position (bullet only).
   * Returns false if it was not possible to create a pivot.
   */
  virtual bool CreatePivotJoint (const csVector3& objpos) = 0;

  /**
   * Get the amount of pivot joints.
   */
  virtual size_t GetPivotJointCount () const = 0;

  /**
   * Get the position of a pivot joint.
   */
  virtual csVector3 GetPivotJointPosition (size_t idx) = 0;

  /**
   * Change the starting position of a pivot joint.
   */
  virtual void SetPivotJointPosition (size_t idx, const csVector3& objpos) = 0;

  /**
   * Remove a specific pivot joint.
   */
  virtual void RemovePivotJoint (size_t idx) = 0;

  /**
   * Remove all pivot joints.
   */
  virtual void RemovePivotJoints () = 0;
};

/**
 * An object in the dynamic world.
 */
struct iDynamicObject : public virtual iBase
{
  SCF_INTERFACE(iDynamicObject,0,0,1);

  /**
   * Get a unique identifier for this object.
   */
  virtual uint GetID () const = 0;

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
   * object is currently not visible or if the factory
   * is a light factory.
   */
  virtual iMeshWrapper* GetMesh () const = 0;

  /**
   * Get the light for this object. Can be 0 if the light
   * is currently not visible or if the factory is a normal
   * mesh factory.
   */
  virtual iLight* GetLight () const = 0;

  /**
   * Get the body for this object. Can be 0 if the
   * object is currently not visible.
   */
  virtual iRigidBody* GetBody () const = 0;

  /**
   * Recreate the pivot joints from the factory. This
   * will first remove all current pivot joints on the object.
   */
  virtual bool RecreatePivotJoints () = 0;

  /**
   * Create a pivot joint at a specific world space position (bullet only).
   * Returns false if it was not possible to create a pivot.
   */
  virtual bool CreatePivotJoint (const csVector3& worldpos) = 0;

  /**
   * Get the amount of pivot joints.
   */
  virtual size_t GetPivotJointCount () const = 0;

  /**
   * Get the position of a pivot joint.
   */
  virtual csVector3 GetPivotJointPosition (size_t idx) = 0;

  /**
   * Set the position of a pivot joint.
   */
  virtual void SetPivotJointPosition (size_t idx, const csVector3& worldpos) = 0;

  /**
   * Remove a specific pivot joint.
   */
  virtual void RemovePivotJoint (size_t idx) = 0;

  /**
   * Remove all pivot joints.
   */
  virtual void RemovePivotJoints () = 0;

  /**
   * Connect another object to a specific joint in this object.
   * This only works if the factory of this object actually defines
   * a joint with the given index. Otherwise this function returns false.
   * If 'obj' is 0 then the connection will be removed.
   */
  virtual bool Connect (size_t jointIdx, iDynamicObject* obj) = 0;

  /**
   * Recreate the joints. This is useful after the joints have been
   * modified in the dynamic factories so that they have the new information.
   */
  virtual void RecreateJoints () = 0;

  /**
   * Get the object connected at a specific joint. If there is no
   * such object or there is no such joint defined in the factory
   * then this function returns 0.
   */
  virtual iDynamicObject* GetConnectedObject (size_t jointIdx) = 0;

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
   * Set the optional entity name.
   */
  virtual void SetEntityName (const char* name) = 0;

  /**
   * Get the optional entity name (only valid after SetEntity()).
   */
  virtual const char* GetEntityName () const = 0;

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
   * Get the entity template associated with this dynamic object.
   */
  virtual iCelEntityTemplate* GetEntityTemplate () const = 0;

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
   * Get an object.
   */
  virtual iDynamicObject* GetObject (size_t i) const = 0;

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

  /**
   * Inhibit the creation of entities. This is useful in case you have
   * an editor and don't want the entities to be created. Note that changing
   * this flag has no effect on already existing dynamic objects.
   */
  virtual void InhibitEntities (bool e) = 0;

  /**
   * Returns true if entity creation is inhibited.
   */
  virtual bool IsInhibitEntities () const = 0;

  /**
   * Enable/disable game mode. In game mode (default) logic objects are
   * invisible. If game mode is disabled then they are visible but with
   * a transparent texture.
   */
  virtual void EnableGameMode (bool e) = 0;
  virtual bool IsGameMode () const = 0;

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
   * Add a special dynamic factory which is intended for game logic purposes.
   * The objects created by this factory are invisible (in game mode) and
   * can act as special containers for game logic or for connecting joints.
   */
  virtual iDynamicFactory* AddLogicFactory (const char* factory, float maxradius,
      float imposterradius, const csBox3& box) = 0;

  /**
   * Add a new dynamic factory to the world. This version supports a light
   * factory.
   * @param maxradius is a relative maximum radius (0 to 1) at which point
   * the light should become visible. It is relative to the total maximum
   * radius maintained by this world.
   */
  virtual iDynamicFactory* AddLightFactory (const char* factory, float maxradius) = 0;

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
   * Update a dynamic object in case external situations change. This
   * may recreate the physics bodies, joints, recreate the mesh and/or
   * light if needed and so on. This is typically used by editors to
   * be able to refresh the objects currently being edited.
   */
  virtual void UpdateObject (iDynamicObject* dynobj) = 0;

  /**
   * Update all objects in a cell.
   */
  virtual void UpdateObjects (iDynamicCell* cell) = 0;

  /**
   * Update all objects created from a factory.
   */
  virtual void UpdateObjects (iDynamicFactory* factory) = 0;

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
   * Find an object given its entity name. This only works for dynamic
   * objects for which a unique entity name has been given.
   */
  virtual iDynamicObject* FindObject (const char* name) const = 0;
  /**
   * Find an object given it's id.
   */
  virtual iDynamicObject* FindObject (uint id) const = 0;

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

