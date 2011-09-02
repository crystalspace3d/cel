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
 * A factory object in the dynamic world.
 */
struct iDynamicFactory : public virtual iBase
{
  SCF_INTERFACE(iDynamicFactory,0,0,1);

  virtual const char* GetName () const = 0;

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
   * Get the bounding box in world space.
   */
  virtual const csBox3& GetBBox () const = 0;

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
};

/**
 * Interface to the dynamic world plugin.
 */
struct iPcDynamicWorld : public virtual iBase
{
  SCF_INTERFACE(iPcDynamicWorld,0,0,1);

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

  //------------------------------------------------------------------------------

  /**
   * Add a new dynamic object to the world. This object will have no physics
   * properties yet.
   */
  virtual iDynamicObject* AddObject (const char* factory,
      const csReversibleTransform& trans) = 0;

  /**
   * Force a dynamic object to be visible. This will force the
   * creation of the mesh.
   */
  virtual void ForceVisible (iDynamicObject* dynobj) = 0;

  /**
   * Remove an object.
   */
  virtual void DeleteObject (iDynamicObject* dynobj) = 0;

  /**
   * Delete all objects.
   */
  virtual void DeleteObjects () = 0;

  /**
   * Set the sector for this dynamic world.
   */
  virtual void Setup (iSector* sector, iDynamicSystem* dynSys) = 0;

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
   * Find an object given its rigid body.
   */
  virtual iDynamicObject* FindObject (iRigidBody* body) = 0;

  /**
   * Find an object given its mesh.
   */
  virtual iDynamicObject* FindObject (iMeshWrapper* mesh) = 0;

  /**
   * Save the world to XML.
   */
  virtual void Save (iDocumentNode* node) = 0;

  /**
   * Load the world from XML.
   * Return 0 on success or otherwise a string with the error.
   */
  virtual csRef<iString> Load (iDocumentNode* node) = 0;
};

#endif // __CEL_PF_DYNWORLD__

