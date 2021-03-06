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

#ifndef __CEL_PF_DYNWORLDFACT__
#define __CEL_PF_DYNWORLDFACT__

#include "csutil/scf.h"
#include "csutil/scf_implementation.h"
#include "csutil/weakref.h"
#include "csutil/set.h"
#include "csutil/eventhandlers.h"
#include "csutil/refarr.h"
#include "csutil/parray.h"
#include "iutil/comp.h"
#include "iutil/virtclk.h"
#include "iutil/selfdestruct.h"

#include "csgeom/kdtree.h"
#include "csgeom/math3d.h"
#include "csgeom/sphere.h"

#include "imap/services.h"

#include "iengine/engine.h"
#include "iengine/movable.h"
#include "iengine/imposter.h"

#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"

#include "propclass/dynworld.h"

#include "ivaria/convexdecompose.h"
#include "cstool/collisionhelper.h"

/**
 * Factory for DynWorld.
 */
CEL_DECLARE_FACTORY (DynamicWorld)

struct iLightFactory;

#define MARKER_NEW 0xe3
#define MARKER_END 0x3e

class celPcDynamicWorld;
class DynamicCell;
struct iDecalManager;

typedef CS::Geometry::SpatialTreeChild::SphereChild<true> TreeChild;
typedef CS::Geometry::KDTree<TreeChild> Tree;

class MeshCacheFactory
{
public:
  csRefArray<iMeshWrapper> meshes;
};

class MeshCache
{
private:
  csHash<MeshCacheFactory*,csString> meshFactories;

public:
  MeshCache ();
  ~MeshCache ();

  iMeshWrapper* AddMesh (iEngine* engine, iMeshFactoryWrapper* factory,
      iSector* sector, const csReversibleTransform& trans);
  void RemoveMesh (iMeshWrapper* mesh);
  void RemoveMeshes ();
  void RemoveFactory (iEngine* engine, const char* name);
};

class DOCollider
{
protected:
  csVector3 offset;
  float mass;

public:
  DOCollider (const csVector3& offset, float mass) : offset (offset), mass (mass) { }
  virtual ~DOCollider () { }

  float GetMass () const { return mass; }
  const csVector3& GetOffset () const { return offset; }

  virtual csRef<CS::Collisions::iCollider> Create (celPcDynamicWorld* dynworld,
      iMeshFactoryWrapper* factory)
  {
    return 0;
  }

  virtual celBodyInfo GetBodyInfo ()
  {
    celBodyInfo info;
    info.offset = offset;
    info.mass = mass;
    info.radius = 0;
    info.length = 0;
    info.size.Set (0, 0, 0);
    return info;
  }
};

class DOColliderMesh : public DOCollider
{
public:
  DOColliderMesh (const csVector3& offset, float mass) :
    DOCollider (offset, mass) { }
  virtual ~DOColliderMesh () { }
  virtual csRef<CS::Collisions::iCollider> Create (celPcDynamicWorld* dynworld,
      iMeshFactoryWrapper* factory);
  virtual celBodyInfo GetBodyInfo ()
  {
    celBodyInfo info = DOCollider::GetBodyInfo ();
    info.type = TRIMESH_COLLIDER_GEOMETRY;
    return info;
  }
};

class DOColliderConvexMesh : public DOCollider
{
public:
  DOColliderConvexMesh (const csVector3& offset, float mass) :
    DOCollider (offset, mass) { }
  virtual ~DOColliderConvexMesh () { }
  virtual csRef<CS::Collisions::iCollider> Create (celPcDynamicWorld* dynworld,
      iMeshFactoryWrapper* factory);
  virtual celBodyInfo GetBodyInfo ()
  {
    celBodyInfo info = DOCollider::GetBodyInfo ();
    info.type = CONVEXMESH_COLLIDER_GEOMETRY;
    return info;
  }
};

class DOColliderBox : public DOCollider
{
private:
  csVector3 size;

public:
  DOColliderBox (const csVector3& size, const csVector3& offset, float mass) :
    DOCollider (offset, mass), size (size) { }
  virtual ~DOColliderBox () { }
  virtual csRef<CS::Collisions::iCollider> Create (celPcDynamicWorld* dynworld,
      iMeshFactoryWrapper* factory);
  virtual celBodyInfo GetBodyInfo ()
  {
    celBodyInfo info = DOCollider::GetBodyInfo ();
    info.type = BOX_COLLIDER_GEOMETRY;
    info.size = size;
    return info;
  }
};

class DOColliderCylinder : public DOCollider
{
private:
  float length, radius;

public:
  DOColliderCylinder (const csVector3& offset, float length, float radius, float mass) :
    DOCollider (offset, mass), length (length), radius (radius) { }
  virtual ~DOColliderCylinder () { }
  virtual csRef<CS::Collisions::iCollider> Create (celPcDynamicWorld* dynworld,
      iMeshFactoryWrapper* factory);
  virtual celBodyInfo GetBodyInfo ()
  {
    celBodyInfo info = DOCollider::GetBodyInfo ();
    info.type = CYLINDER_COLLIDER_GEOMETRY;
    info.radius = radius;
    info.length = length;
    return info;
  }
};

class DOColliderCapsule : public DOCollider
{
private:
  float length, radius;

public:
  DOColliderCapsule (const csVector3& offset, float length, float radius, float mass) :
    DOCollider (offset, mass), length (length), radius (radius) { }
  virtual ~DOColliderCapsule () { }
  virtual csRef<CS::Collisions::iCollider> Create (celPcDynamicWorld* dynworld,
      iMeshFactoryWrapper* factory);
  virtual celBodyInfo GetBodyInfo ()
  {
    celBodyInfo info = DOCollider::GetBodyInfo ();
    info.type = CAPSULE_COLLIDER_GEOMETRY;
    info.radius = radius;
    info.length = length;
    return info;
  }
};

class DOColliderSphere : public DOCollider
{
private:
  float radius;

public:
  DOColliderSphere (const csVector3& offset, float radius, float mass) :
    DOCollider (offset, mass), radius (radius) { }
  virtual ~DOColliderSphere () { }
  virtual csRef<CS::Collisions::iCollider> Create (celPcDynamicWorld* dynworld,
      iMeshFactoryWrapper* factory);
  virtual celBodyInfo GetBodyInfo ()
  {
    celBodyInfo info = DOCollider::GetBodyInfo ();
    info.type = SPHERE_COLLIDER_GEOMETRY;
    info.radius = radius;
    return info;
  }
};

class DynamicFactory : public scfImplementationExt2<DynamicFactory,
	csObject, iDynamicFactory, iSelfDestruct>
{
private:
  csString tplName;

  celPcDynamicWorld* world;
  iMeshFactoryWrapper* factory;
  iLightFactory* lightFactory;
  csPDelArray<DOCollider> colliders;
  csSphere bsphere;
  csBox3 bbox;
  csBox3 physBbox;
  float maxradiusRelative;
  bool isLogic;
  bool hasCollider;
  size_t objCounter;	// Count how many objects are in memory for this factory.

  csArray<csVector3> pivotJoints;
  csArray<DynFactJointDefinition> joints;

  csRef<CS::Physics::iRigidBodyFactory> rigidBodyFactory;
  csRef<CS::Collisions::iCollider> collider;

  csRef<iGeometryGenerator> geometryGenerator;
  csRef<iImposterFactory> imposterFactory;
  float imposterradius;
  csHash<csString,csStringID> attributes;

  void UpdatePhysBBox ();

public:
  DynamicFactory (celPcDynamicWorld* world, const char* name, bool usefact,
      float maxradiusRelative, float imposterradius, bool isLogic = false);
  virtual ~DynamicFactory () { }

  CS::Physics::iRigidBodyFactory* GetRigidBodyFactory ();

  virtual iObject* QueryObject () { return this; }
  virtual float GetMaximumRadiusRelative () const { return maxradiusRelative; }
  virtual void SetMaximumRadiusRelative (float r) { maxradiusRelative = r; }
  virtual float GetImposterRadius () const { return imposterradius; }
  virtual void SetImposterRadius (float r);
  virtual const csBox3& GetBBox () const { return bbox; }
  const csSphere& GetBSphere () const { return bsphere; }

  virtual bool IsLogicFactory () const { return isLogic; }
  virtual bool IsLightFactory () const { return lightFactory != 0; }

  virtual void SetDefaultEntityTemplate (const char* tmpName) { tplName = tmpName; }
  virtual const char* GetDefaultEntityTemplate () const { return tplName; }

  virtual const csBox3& GetPhysicsBBox () const;

  virtual void SetGeometryGenerator (iGeometryGenerator* ggen)
  {
    geometryGenerator = ggen;
  }
  virtual iGeometryGenerator* GetGeometryGenerator () const
  {
    return geometryGenerator;
  }

  virtual size_t GetObjectCount () const { return objCounter; }
  void RegisterNewObject () { objCounter++; }
  void RegisterDeletedObject () { objCounter--; }

  virtual void SetAttribute (const char* name, const char* value);
  virtual void SetAttribute (csStringID nameID, const char* value);
  virtual void ClearAttribute (csStringID nameID);
  virtual void ClearAttribute (const char* name);
  virtual const char* GetAttribute (const char* name) const;
  virtual const char* GetAttribute (csStringID nameID) const;
  virtual csPtr<iAttributeIterator> GetAttributes () const;

  virtual void AddRigidBox (const csVector3& offset, const csVector3& size,
      float mass, size_t idx = csArrayItemNotFound);
  virtual void AddRigidSphere (float radius, const csVector3& offset,
      float mass, size_t idx = csArrayItemNotFound);
  virtual void AddRigidCylinder (float radius, float length,
      const csVector3& offset, float mass, size_t idx = csArrayItemNotFound);
  virtual void AddRigidCapsule (float radius, float length,
      const csVector3& offset, float mass, size_t idx = csArrayItemNotFound);
  virtual void AddRigidMesh (const csVector3& offset, float mass,
      size_t idx = csArrayItemNotFound);
  virtual void AddRigidConvexMesh (const csVector3& offset, float mass,
      size_t idx = csArrayItemNotFound);
  virtual size_t GetBodyCount () const { return colliders.GetSize (); }
  virtual celBodyInfo GetBody (size_t idx) const
  {
    return colliders[idx]->GetBodyInfo ();
  }
  virtual void DeleteBody (size_t idx);
  virtual void DeleteBodies ();

  virtual void SetColliderEnabled (bool e);
  virtual bool IsColliderEnabled () const { return hasCollider; }

  virtual DynFactJointDefinition& CreateJoint ();
  virtual size_t GetJointCount () const { return joints.GetSize (); }
  virtual DynFactJointDefinition& GetJoint (size_t idx) { return joints[idx]; }
  virtual void SetJoint (size_t idx, const DynFactJointDefinition& def);
  virtual void RemoveJoint (size_t idx);
  virtual void RemoveJoints ();

  virtual bool CreatePivotJoint (const csVector3& objpos);
  virtual size_t GetPivotJointCount () const { return pivotJoints.GetSize (); }
  virtual csVector3 GetPivotJointPosition (size_t idx);
  virtual void SetPivotJointPosition (size_t idx, const csVector3& objpos);
  virtual void RemovePivotJoint (size_t idx);
  virtual void RemovePivotJoints ();

  const csPDelArray<DOCollider>& GetColliders () const { return colliders; }
  iMeshFactoryWrapper* GetMeshFactory () const { return factory; }
  iLightFactory* GetLightFactory () const { return lightFactory; }
  virtual const char* GetName () const { return Name; }
  celPcDynamicWorld* GetWorld () const { return world; }
  iImposterFactory* GetImposterFactory () const { return imposterFactory; }
  void ChangeFactory (iMeshFactoryWrapper* fact) { factory = fact; }

  //--------------------- iSelfDestruct implementation -------------------//

  virtual void SelfDestruct ();
};

class DODecal
{
public:
  iDecalTemplate* tpl;
  size_t decalId;
  csVector3 pos, up, normal;
  float width, height;
  iDecal* decal;
};

class DynamicObject : public scfImplementation2<DynamicObject,
  iDynamicObject, iMovableListener>
{
private:
  DynamicCell* cell;
  DynamicFactory* factory;
  csRef<iMeshWrapper> mesh;
  csRef<iLight> light;
  // If the mesh is not prepared yet this transform is the actual
  // transform of this object. If the mesh is prepared then this
  // represents the transform at the time the mesh is prepared.
  csReversibleTransform trans;
  csRef<CS::Physics::iRigidBody> body;
  bool is_static;
  bool is_kinematic;
  bool is_hilight;
  bool hilight_installed;
  float fade;
  TreeChild* child;
  uint id;

  size_t decalId;
  csArray<DODecal> decals;

  // Pivot joints.
#if 0
  // @@@
  csRefArray<CS::Physics::Bullet::iPivotJoint> pivotJoints;
#endif

  // Connected objects (indices are same as the indices of the factory joints).
  csRefArray<iDynamicObject> connectedObjects;
  csRefArray<CS::Physics::iJoint> joints;

  // Update all joints for all bodies that exist.
  void UpdateJoints ();

  bool atBaseline;
  // A dynamic object that is at the baseline (atBaseline == true)
  // will maintain the field below so that we can discover if it has moved
  // sufficiently from the starting baseline. For objects that were not
  // present at the baseline this field is not maintained.
  // Once this field is set to true it will never be set to false again.
  bool hasMovedSufficiently;
  long lastUpdateNr;    // Helper field to see if we moved sufficiently.

  csRef<iCelEntityTemplate> entityTemplate;
  csString entityName;
  csRef<iCelParameterBlock> params;
  csWeakRef<iCelEntity> entity;

  void InstallHilight (bool hi);
  void Init (DynamicCell* cell);
  void SetupPivotJoints ();

  mutable csSphere bsphere;
  mutable bool bsphereValid;

  // Update the mesh and body in the property classes of the entity.
  void MeshBodyToEntity (iMeshWrapper* mesh, CS::Physics::iRigidBody* body);

  void CreateCollider ();
  void CreateBody ();

public:
  DynamicObject (DynamicCell* cell);
  DynamicObject (DynamicCell* cell, DynamicFactory* factory,
      const csReversibleTransform& trans);
  virtual ~DynamicObject ();

  void SetID (uint id);
  virtual uint GetID () const { return id; }

  virtual iDynamicCell* GetCell () const;
  DynamicCell* GetCellInternal () const { return cell; }

  void ClearEntity ();

  TreeChild* GetChild () const { return child; }
  void SetChild (TreeChild* child)
  { DynamicObject::child = child; }

  virtual iDynamicFactory* GetFactory () const { return factory; }
  virtual void MakeStatic ();
  virtual void MakeDynamic ();
  virtual void MakeKinematic ();
  virtual void UndoKinematic ();
  virtual bool IsStatic () const { return is_static; }
  virtual void SetHilight (bool hi);
  virtual bool IsHilight () const { return is_hilight; }
  virtual iMeshWrapper* GetMesh () const { return mesh; }
  virtual iLight* GetLight () const { return light; }

  virtual CS::Physics::iRigidBody* GetBody () const { return body; }
  virtual bool RecreatePivotJoints ();
  virtual bool CreatePivotJoint (const csVector3& worldpos);
  virtual size_t GetPivotJointCount () const
  {
#if 0
    // @@@
    return pivotJoints.GetSize ();
#endif
    return 0;
  }
  virtual csVector3 GetPivotJointPosition (size_t idx);
  virtual void SetPivotJointPosition (size_t idx, const csVector3& worldpos);
  virtual void RemovePivotJoint (size_t idx);
  virtual void RemovePivotJoints ();

  const csSphere& GetBSphere () const;

  virtual bool Connect (size_t jointIdx, iDynamicObject* obj);
  virtual void RecreateJoints ();
  void RemoveJoints ();
  virtual iDynamicObject* GetConnectedObject (size_t jointIdx);

  virtual void RefreshColliders ();
  virtual const csReversibleTransform& GetTransform ();
  virtual void SetTransform (const csReversibleTransform& trans);
  virtual bool SetEntity (const char* entityName, const char* entityTplName, iCelParameterBlock* params);
  virtual void SetEntityName (const char* name);
  virtual const char* GetEntityName () const { return entityName; }
  virtual void LinkEntity (iCelEntity* entity);
  virtual void UnlinkEntity ();
  virtual iCelEntityTemplate* GetEntityTemplate () const { return entityTemplate; }
  virtual iCelParameterBlock* GetEntityParameters () const { return params; }
  virtual iCelEntity* GetEntity () const { return entity; }
  virtual csPtr<iString> GetDescription () const;

  virtual void MovableChanged (iMovable* movable);
  virtual void MovableDestroyed (iMovable* movable);

  void PrepareMesh (celPcDynamicWorld* world);
  void PrepareLight (celPcDynamicWorld* world);
  void PrepareCsObject (celPcDynamicWorld* world);
  void RemoveMesh (celPcDynamicWorld* world);
  void RemoveLight (celPcDynamicWorld* world);
  void RemoveCsObject (celPcDynamicWorld* world);
  void RemoveCollider ();
  void RemoveBody ();
  void Save (iDocumentNode* node, iSyntaxService* syn);
  bool Load (iDocumentNode* node, iSyntaxService* syn, celPcDynamicWorld* world);

  void CreateMissingDecals ();
  virtual size_t AddDecal (const char* decalTplName, const csVector3& position,
      const csVector3& up, const csVector3& normal, float width, float height);
  virtual void RemoveDecal (size_t id);

  void SetFade (float f);
  float GetFade () const { return fade; }
  virtual iCelEntity* ForceEntity ();
  void MarkBaseline ()
  {
    if (entity) entity->MarkBaseline ();
    atBaseline = true;
  }
  void ClearBaseline ()
  {
    atBaseline = false;
  }
  bool ExistedAtBaseline () const { return atBaseline; }

  /**
   * Check if this dynobj has moved sufficiently to warrant a save
   * of its position.
   */
  bool HasMovedSufficiently ();

  // Return true if a body is part of this dynamic object.
  bool HasBody (CS::Physics::iRigidBody* body);
};

struct DynamicObjectExtraData
{
  void LeafAddObject (DynamicObject* object)
  {
  }
  void LeafUpdateObjects (DynamicObject** objects, uint numObjects)
  {
  }
  void NodeUpdate (const DynamicObjectExtraData& child1,
      const DynamicObjectExtraData& child2)
  {
  }
};

#define IDBLOCK_SIZE 10000

class DynamicCell : public scfImplementation1<DynamicCell, iDynamicCell>
{
public:
  csString name;
  celPcDynamicWorld* world;
  iCelPlLayer* pl;

  iSector* sector;
  csRef<CS::Physics::iPhysicalSector> dynSys;
  bool createdDynSys;
  csRefArray<DynamicObject> objects;

  Tree* tree;

  uint lastID;			// Next ID to generate
  size_t idBlockIdx;		// Index into below array
  csArray<uint> allocatedIDBlocks;

  void DeleteObjectInt (DynamicObject* dynobj);

public:
  DynamicCell (const char* name, celPcDynamicWorld* world);
  virtual ~DynamicCell ();

  uint AllocID ();
  // Force allocation of an ID. This function will ensure that the given ID
  // will not be used by future AllocID() calls. Note! This function does not
  // test if the ID is in the cell's ID scope! Use IsAllocatedHere() first.
  void AllocID (uint id);

  // Find out if this id was allocated in this cell.
  bool IsAllocatedHere (uint id);

  void Setup (iSector* sector, CS::Physics::iPhysicalSector* dynSys);

  virtual const char* GetName () const { return name; }

  virtual void DeleteObject (iDynamicObject* dynobj);
  virtual void DeleteObjects ();
  virtual iDynamicObject* AddObject (const char* factory,
      const csReversibleTransform& trans);
  virtual size_t GetObjectCount () const { return objects.GetSize (); }
  virtual iDynamicObject* GetObject (size_t i) const { return objects[i]; }
  DynamicObject* GetObjectInt (size_t i) const { return objects[i]; }

  virtual void Save (iDocumentNode* node);
  virtual csRef<iString> Load (iDocumentNode* node);

  virtual void MarkBaseline ();

  virtual iSector* GetSector () const { return sector; }
  virtual CS::Physics::iPhysicalSector* GetDynamicSector () const { return dynSys; }

  void SaveIDAllocations (iCelCompactDataBufferWriter* buf);
  void SaveModifications (iCelCompactDataBufferWriter* buf,
      iStringSet* strings, csSet<csPtrKey<DynamicObject> >& alreadySaved);
  void RestoreIDAllocations (iCelCompactDataBufferReader* buf);
  void RestoreModifications (iCelCompactDataBufferReader* buf,
      const csHash<csString,csStringID>& strings);
};

class celPcDynamicWorld : public scfImplementationExt1<celPcDynamicWorld,
  celPcCommon, iPcDynamicWorld>
{
public:
  csRef<iEngine> engine;
  csRef<iGraphics3D> g3d;
  csWeakRef<iCelPlLayer> pl;
  csRef<iVirtualClock> vc;
  CS::Collisions::CollisionHelper collisionHelper;
  csRef<CS::Physics::iPhysicalSystem> physicalSystem;
  csRef<iCollideSystem> cdsys;
  csRef<iDecalManager> decalMgr;

  csRefArray<DynamicFactory> factories;
  csHash<DynamicFactory*,csString> factory_hash;

  csRefArray<iDecalTemplate> decalTemplates;
  csHash<iDecalTemplate*,csString> template_hash;

  MeshCache meshCache;
  float radius;
  csRef<iELCM> elcm;
  size_t scopeIdx;

  // For properties.
  enum propids
  {
    propid_physics = 0
  };
  static PropertyHolder propinfo;

  // Don't create entities if this is true.
  bool inhibitEntities;
  // Game mode.
  bool gameMode;
  // Physics.
  bool doPhysics;

  uint lastIDBlock;
  // The following flag is set to true while we are restoring ID blocks.
  // During that time it is illegal to allocate new ID blocks.
  bool restoringIDBlocks;

  csSet<csPtrKey<DynamicObject> > fadingIn;
  csSet<csPtrKey<DynamicObject> > fadingOut;

  // Mapping of id to dynamic objects.
  csHash<iDynamicObject*,uint> idToDynObj;
  // Mapping of names to dynamic objects (for dynobjects having a name).
  csHash<iDynamicObject*,csString> nameToDynObj;

  // Current visible objects.
  csSet<csPtrKey<DynamicObject> > visibleObjects;

  // A set of all dynamic objects which are pre-baseline and which
  // have moved from their original position. These have to be saved.
  csSet<csPtrKey<DynamicObject> > hasMovedFromBaseline;

  csRef<iDynamicCellCreator> cellCreator;

  // Current cell.
  DynamicCell* currentCell;

  // All cells.
  csHash<csRef<DynamicCell>,csString> cells;

  // A set of entities which the ELCM thinks are safe to remove.
  csSet<csPtrKey<iCelEntity> > safeToRemove;

  // A set of dynamic objects which we have to check if they moved
  // sufficiently for us to remember its position.
  csSet<csPtrKey<DynamicObject> > checkForMovement;

  // This function will remove all entities that are still in safeToRemove.
  // The 'visibility' traversal should have removed all entities that
  // are attached to a visible mesh from this set first.
  void RemoveSafeEntities ();

  // Check all objects which have moved recently to see if some of
  // them have moved enough for them to be considered 'dirty'.
  void CheckForMovement ();

  void ProcessFadingIn (float fade_speed);
  void ProcessFadingOut (float fade_speed);

  // Create dummy mesh factory.
  iMeshFactoryWrapper* CreateDummyFactory (const char* factoryName,
      CS::Geometry::Primitive& primitive,
      int r, int g, int b, int a);

public:
  celPcDynamicWorld (iObjectRegistry* object_reg);
  virtual ~celPcDynamicWorld ();

  CS::Collisions::CollisionHelper& GetCollisionHelper () { return collisionHelper; }
  CS::Collisions::iCollisionSystem* GetCollisionSystem () const { return physicalSystem; }
  CS::Physics::iPhysicalSystem* GetPhysicalSystem () const { return physicalSystem; }
  iCollideSystem* GetCollideSystem ();

  uint AllocIDBlock ()
  {
    CS_ASSERT (!restoringIDBlocks);
    lastIDBlock += IDBLOCK_SIZE;
    return lastIDBlock - IDBLOCK_SIZE;
  }

  csHash<iDynamicObject*,uint>& GetIdToDynObj ()
  {
    return idToDynObj;
  }
  csHash<iDynamicObject*,csString>& GetNameToDynObj ()
  {
    return nameToDynObj;
  }
  // Given an ID, find the cell which allocated this ID.
  DynamicCell* FindCellForID (uint id);

  iObjectRegistry* GetObjectRegistry () { return object_reg; }
  iDecalManager* GetDecalManager () const { return decalMgr; }

  iCelEntity* CreateSpawnedEntity (iCelEntityTemplate* tpl,
      const char* entityName, iCelParameterBlock* params,
      const csVector3& pos, float yrot, const char* sector,
      const char* node);

  virtual void SetELCM (iELCM* elcm);

  virtual void InhibitEntities (bool e);
  virtual bool IsInhibitEntities () const { return inhibitEntities; }
  virtual void EnableGameMode (bool e);
  virtual bool IsGameMode () const { return gameMode; }
  virtual void EnablePhysics (bool e);
  virtual bool IsPhysicsEnabled () const { return doPhysics; }

  void SafeToRemove (iCelEntity* entity);
  virtual void Dump ();

  virtual iDynamicCell* AddCell (const char* name, iSector* sector,
      CS::Physics::iPhysicalSector* dynSys = 0);
  virtual iDynamicCell* FindCell (const char* name);
  virtual void RemoveCell (iDynamicCell* cell);
  virtual void SetCurrentCell (iDynamicCell* cell) { currentCell = static_cast<DynamicCell*> (cell); }
  virtual iDynamicCell* GetCurrentCell () const { return currentCell; }
  virtual csPtr<iDynamicCellIterator> GetCells () const;
  virtual void SetDynamicCellCreator (iDynamicCellCreator* creator)
  {
    cellCreator = creator;
  }

  virtual void UpdateObject (iDynamicObject* dynobj);
  virtual void UpdateObjects (iDynamicCell* cell);
  virtual void UpdateObjects (iDynamicFactory* factory);

  virtual void RegisterDecalTemplate (const char* name, iDecalTemplate* tpl);
  virtual size_t GetDecalTemplateCount () const;
  virtual iDecalTemplate* GetDecalTemplate (size_t index) const;
  virtual iDecalTemplate* FindDecalTemplate (const char* name);
  virtual void RemoveDecalTemplate (const char* name);
  virtual void RemoveDecalTemplates ();

  virtual iDynamicFactory* AddFactory (const char* factory, float maxradius,
      float imposterradius);
  virtual iDynamicFactory* AddLogicFactory (const char* factory, float maxradius,
      float imposterradius, const csBox3& bbox);
  virtual iDynamicFactory* AddLightFactory (const char* factory, float maxradius);
  virtual void RemoveFactory (iDynamicFactory* factory);
  virtual void DeleteFactories ();
  virtual size_t GetFactoryCount () const { return factories.GetSize () ; }
  virtual iDynamicFactory* GetFactory (size_t index) const { return factories[index]; }
  virtual iDynamicFactory* FindFactory (const char* name) const;

  virtual void ForceVisible (iDynamicObject* dynobj);
  virtual void ForceInvisible (iDynamicObject* dynobj);
  virtual void DeleteAll ();
  virtual void SetRadius (float radius);
  virtual float GetRadius () const { return radius; }
  virtual void PrepareView (iCamera* camera, float elapsed_time);
  virtual void ForceView (iCamera* camera);
  virtual iDynamicObject* FindObject (iCelEntity* entity) const;
  virtual iDynamicObject* FindObject (CS::Physics::iRigidBody* body) const;
  virtual iDynamicObject* FindObject (iMeshWrapper* mesh) const;
  virtual iDynamicObject* FindObject (const char* name) const;
  virtual iDynamicObject* FindObject (uint id) const;

  virtual void Save (iDocumentNode* node);
  virtual csRef<iString> Load (iDocumentNode* node);

  virtual void MarkBaseline ();
  virtual csPtr<iDataBuffer> SaveModifications ();
  virtual void RestoreModifications (iDataBuffer* buf);

  // Override SetProperty from celPcCommon in order to provide support
  // for the 'physics' property.
  virtual bool SetPropertyIndexed (int idx, bool b);
  virtual bool GetPropertyIndexed (int, bool&);
};


#endif // __CEL_PF_DYNWORLDFACT__

