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

#include "csgeom/kdtreex.h"
#include "csgeom/math3d.h"
#include "csgeom/sphere.h"

#include "ivaria/dynamics.h"
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

/**
 * Factory for DynWorld.
 */
CEL_DECLARE_FACTORY (DynamicWorld)


class celPcDynamicWorld;
class DynamicCell;

class MeshCacheFactory
{
public:
  csArray<iMeshWrapper*> meshes;
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

  virtual csRef<iRigidBody> Create (iDynamicSystem* dynSys, iMeshWrapper* mesh,
      const csReversibleTransform& trans, iRigidBody* sharedBody)
  {
    // Create a body and attach the mesh.
    if (sharedBody)
    {
      return sharedBody;
    }
    else
    {
      csRef<iRigidBody> body = dynSys->CreateBody ();

      csRef<CS::Physics::Bullet::iRigidBody> csBody = scfQueryInterface<CS::Physics::Bullet::iRigidBody> (body);
      csBody->SetLinearDampener (0.0f);
      csBody->SetRollingDampener (0.0f);

      body->AdjustTotalMass (mass);
      body->SetTransform (trans);
      body->AttachMesh (mesh);
      return body;
    }
  }
};

class DOColliderMesh : public DOCollider
{
public:
  DOColliderMesh (const csVector3& offset, float mass) :
    DOCollider (offset, mass) { }
  virtual ~DOColliderMesh () { }
  virtual csRef<iRigidBody> Create (iDynamicSystem* dynSys, iMeshWrapper* mesh,
      const csReversibleTransform& trans, iRigidBody* sharedBody)
  {
    csRef<iRigidBody> body = DOCollider::Create (dynSys, mesh, trans, sharedBody);
    const csMatrix3 tm;
    csOrthoTransform t (tm, offset);
    body->AttachColliderMesh (mesh, t, 10, 1, 0.8f);
    return body;
  }
};

class DOColliderConvexMesh : public DOCollider
{
public:
  DOColliderConvexMesh (const csVector3& offset, float mass) :
    DOCollider (offset, mass) { }
  virtual ~DOColliderConvexMesh () { }
  virtual csRef<iRigidBody> Create (iDynamicSystem* dynSys, iMeshWrapper* mesh,
      const csReversibleTransform& trans, iRigidBody* sharedBody)
  {
    csRef<iRigidBody> body = DOCollider::Create (dynSys, mesh, trans, sharedBody);
    const csMatrix3 tm;
    csOrthoTransform t (tm, offset);
    body->AttachColliderConvexMesh (mesh, t, 10, 1, 0.8f);
    return body;
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
  virtual csRef<iRigidBody> Create (iDynamicSystem* dynSys, iMeshWrapper* mesh,
      const csReversibleTransform& trans, iRigidBody* sharedBody)
  {
    csRef<iRigidBody> body = DOCollider::Create (dynSys, mesh, trans, sharedBody);
    const csMatrix3 tm;
    csOrthoTransform t (tm, offset);
    body->AttachColliderBox (size, t, 10, 1, 0);
    return body;
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
  virtual csRef<iRigidBody> Create (iDynamicSystem* dynSys, iMeshWrapper* mesh,
      const csReversibleTransform& trans, iRigidBody* sharedBody)
  {
    csRef<iRigidBody> body = DOCollider::Create (dynSys, mesh, trans, sharedBody);
    const csMatrix3 tm;
    csOrthoTransform t (tm, offset);
    t.RotateThis (csVector3 (1.0f, 0.0f, 0.0f), HALF_PI);
    body->AttachColliderCylinder (length, radius, t, 10, 1, 0.8f);
    return body;
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
  virtual csRef<iRigidBody> Create (iDynamicSystem* dynSys, iMeshWrapper* mesh,
      const csReversibleTransform& trans, iRigidBody* sharedBody)
  {
    csRef<iRigidBody> body = DOCollider::Create (dynSys, mesh, trans, sharedBody);
    body->AttachColliderSphere (radius, offset, 100, .5, 0);
    return body;
  }
};

class DynamicFactory : public scfImplementation1<DynamicFactory,
  iDynamicFactory>
{
private:
  csString name;

  celPcDynamicWorld* world;
  iMeshFactoryWrapper* factory;
  csPDelArray<DOCollider> colliders;
  csSphere bsphere;
  csBox3 bbox;
  csBox3 physBbox;
  float maxradiusRelative;

  csRef<iGeometryGenerator> geometryGenerator;
  csRef<iImposterFactory> imposterFactory;
  float imposterradius;
  csHash<csString,csString> attributes;

public:
  DynamicFactory (celPcDynamicWorld* world, const char* name,
      float maxradiusRelative, float imposterradius);
  virtual ~DynamicFactory () { }
  virtual float GetMaximumRadiusRelative () const { return maxradiusRelative; }
  virtual const csBox3& GetBBox () const { return bbox; }
  const csSphere& GetBSphere () const { return bsphere; }

  virtual const csBox3& GetPhysicsBBox () const { return physBbox; }

  virtual void SetGeometryGenerator (iGeometryGenerator* ggen)
  {
    geometryGenerator = ggen;
  }
  virtual iGeometryGenerator* GetGeometryGenerator () const
  {
    return geometryGenerator;
  }
  virtual void SetAttribute (const char* name, const char* value);
  virtual const char* GetAttribute (const char* name) const;

  virtual void AddRigidBox (const csVector3& offset, const csVector3& size,
      float mass);
  virtual void AddRigidSphere (float radius, const csVector3& offset,
      float mass);
  virtual void AddRigidCylinder (float radius, float length,
      const csVector3& offset, float mass);
  virtual void AddRigidMesh (const csVector3& offset, float mass);
  virtual void AddRigidConvexMesh (const csVector3& offset, float mass);

  const csPDelArray<DOCollider>& GetColliders () const { return colliders; }
  iMeshFactoryWrapper* GetMeshFactory () const { return factory; }
  virtual const char* GetName () const { return name; }
  const csString& GetCsName () const { return name; }
  celPcDynamicWorld* GetWorld () const { return world; }
  iImposterFactory* GetImposterFactory () const { return imposterFactory; }
};

class DynamicObject : public scfImplementation2<DynamicObject,
  iDynamicObject, iMovableListener>
{
private:
  DynamicCell* cell;
  DynamicFactory* factory;
  csRef<iMeshWrapper> mesh;
  // If the mesh is not prepared yet this transform is the actual
  // transform of this object. If the mesh is prepared then this
  // represents the transform at the time the mesh is prepared.
  csReversibleTransform trans;
  csRef<iRigidBody> body;
  bool is_static;
  bool is_kinematic;
  bool is_hilight;
  bool hilight_installed;
  float fade;
  CS::Geometry::KDTreeChild* child;
  uint id;

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

  mutable csSphere bsphere;
  mutable bool bsphereValid;

public:
  DynamicObject (DynamicCell* cell);
  DynamicObject (DynamicCell* cell, DynamicFactory* factory,
      const csReversibleTransform& trans);
  virtual ~DynamicObject ();

  void SetID (uint id) { DynamicObject::id = id; }
  uint GetID () const { return id; }

  CS::Geometry::KDTreeChild* GetChild () const { return child; }
  void SetChild (CS::Geometry::KDTreeChild* child)
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
  virtual iRigidBody* GetBody () const { return body; }
  const csSphere& GetBSphere () const;
  virtual void RefreshColliders ();
  virtual const csReversibleTransform& GetTransform ();
  virtual void SetTransform (const csReversibleTransform& trans);
  virtual bool SetEntity (const char* entityName, iCelParameterBlock* params);
  virtual void LinkEntity (iCelEntity* entity);
  virtual void UnlinkEntity ();
  virtual iCelEntity* GetEntity () const { return entity; }
  virtual csPtr<iString> GetDescription () const;

  virtual void MovableChanged (iMovable* movable);
  virtual void MovableDestroyed (iMovable* movable);

  void PrepareMesh (celPcDynamicWorld* world);
  void RemoveMesh (celPcDynamicWorld* world);
  void Save (iDocumentNode* node, iSyntaxService* syn);
  bool Load (iDocumentNode* node, iSyntaxService* syn, celPcDynamicWorld* world);

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
  bool HasBody (iRigidBody* body);
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

class DynamicCell
{
public:
  celPcDynamicWorld* world;
  iCelPlLayer* pl;

  iSector* sector;
  csRefArray<DynamicObject> objects;
  csSet<csPtrKey<DynamicObject> > fadingIn;
  csSet<csPtrKey<DynamicObject> > fadingOut;

  // Mapping of meshes to dynamic objects.
  csHash<iDynamicObject*,csPtrKey<iMeshWrapper> > meshToDynObj;

  // Current visible objects.
  csSet<csPtrKey<DynamicObject> > visibleObjects;

  // A set of all dynamic objects which are pre-baseline and which
  // have moved from their original position. These have to be saved.
  csSet<csPtrKey<DynamicObject> > haveMovedFromBaseline;

  CS::Geometry::KDTree* tree;

  void ProcessFadingIn (float fade_speed);
  void ProcessFadingOut (float fade_speed);

  void DeleteObjectInt (DynamicObject* dynobj);

public:
  DynamicCell (celPcDynamicWorld* world);
  virtual ~DynamicCell ();

  csHash<iDynamicObject*,csPtrKey<iMeshWrapper> >& GetMeshToDynObj ()
  {
    return meshToDynObj;
  }

  void PrepareView (iCamera* camera, float elapsed_time);

  virtual void DeleteObject (iDynamicObject* dynobj);
  virtual void DeleteObjects ();
  virtual iDynamicObject* AddObject (const char* factory,
      const csReversibleTransform& trans);
  virtual size_t GetObjectCount () const { return objects.GetSize (); }
  virtual iDynamicObject* FindObject (iCelEntity* entity) const;
  virtual iDynamicObject* FindObject (iRigidBody* body) const;
  virtual iDynamicObject* FindObject (iMeshWrapper* mesh) const;
  iDynamicObject* FindObject (uint id) const;

  virtual void Save (iDocumentNode* node);
  virtual csRef<iString> Load (iDocumentNode* node);

  virtual void MarkBaseline ();
};

class celPcDynamicWorld : public scfImplementationExt1<celPcDynamicWorld,
  celPcCommon, iPcDynamicWorld>
{
public:
  csRef<iEngine> engine;
  csRef<iDynamicSystem> dynSys;
  csWeakRef<iCelPlLayer> pl;
  csRef<iVirtualClock> vc;
  uint lastID;
  csRefArray<DynamicFactory> factories;
  csHash<DynamicFactory*,csString> factory_hash;
  MeshCache meshCache;
  float radius;
  csRef<iELCM> elcm;
  size_t scopeIdx;

  // Current cell.
  DynamicCell* currentCell;

  // A set of entities which the ELCM thinks are safe to remove.
  csSet<csPtrKey<iCelEntity> > safeToRemove;

  // A set of dynamic objects which we have to check if they moved
  // sufficiently for us to remember its position.
  csSet<csPtrKey<DynamicObject> > checkForMovement;

  // This function will remove all entities that are still in safeToRemove.
  // The 'visibility' traversal should have removed all entities that
  // are attached to a visible mesh from this set first.
  void RemoveSafeEntities ();

  void SaveTransform (iCelCompactDataBufferWriter* buf,
      const csReversibleTransform& trans);
  void LoadTransform (iCelCompactDataBufferReader* buf,
      csReversibleTransform& trans);
  void SaveStrings (iCelCompactDataBufferWriter* buf,
      csScfStringSet* strings);
  void LoadStrings (iCelCompactDataBufferReader* buf,
      csHash<csString,csStringID>& strings);

  // Check all objects which have moved recently to see if some of
  // them have moved enough for them to be considered 'dirty'.
  void CheckForMovement ();

public:
  celPcDynamicWorld (iObjectRegistry* object_reg);
  virtual ~celPcDynamicWorld ();

  iObjectRegistry* GetObjectRegistry () { return object_reg; }

  uint GetLastID ()
  {
    lastID++;
    return lastID-1;
  }

  iCelEntity* CreateSpawnedEntity (iCelEntityTemplate* tpl,
      const char* entityName, iCelParameterBlock* params,
      const csVector3& pos, float yrot, const char* sector,
      const char* node);

  virtual void SetELCM (iELCM* elcm);
  void SafeToRemove (iCelEntity* entity);
  virtual void Dump ();

  virtual iDynamicFactory* AddFactory (const char* factory, float maxradius,
      float imposterradius);
  virtual void RemoveFactory (iDynamicFactory* factory);
  virtual void DeleteFactories ();
  virtual size_t GetFactoryCount () const { return factories.GetSize () ; }
  virtual iDynamicFactory* GetFactory (size_t index) const { return factories[index]; }
  virtual iDynamicFactory* FindFactory (const char* name) const;

  // @@@ Temporary
  virtual iDynamicObject* AddObject (const char* factory,
      const csReversibleTransform& trans)
  {
    return currentCell->AddObject (factory, trans);
  }
  // @@@ Temporary
  virtual size_t GetObjectCount () const { return currentCell->GetObjectCount (); }

  virtual void ForceVisible (iDynamicObject* dynobj);
  virtual void ForceInvisible (iDynamicObject* dynobj);
  // @@@ Temporary
  virtual void DeleteObject (iDynamicObject* dynobj)
  {
    DynamicObject* dyn = static_cast<DynamicObject*> (dynobj);
    checkForMovement.Delete (dyn);
    currentCell->DeleteObject (dynobj);
  }
  // @@@ Temporary
  virtual void DeleteObjects ()
  {
    lastID = 1000000001;
    checkForMovement.DeleteAll ();
    meshCache.RemoveMeshes ();
    currentCell->DeleteObjects ();
    if (scopeIdx != csArrayItemNotFound)
      pl->ResetScope (scopeIdx);
  }
  // @@@ Add DeleteAll instead (to set lastID to 1000000001!!!
  virtual void Setup (iSector* sector, iDynamicSystem* dynSys);
  virtual void SetRadius (float radius);
  virtual float GetRadius () const { return radius; }
  virtual void PrepareView (iCamera* camera, float elapsed_time);
  // @@@ Temporary
  virtual iDynamicObject* FindObject (iCelEntity* entity) const
  {
    return currentCell->FindObject (entity);
  }
  // @@@ Temporary
  virtual iDynamicObject* FindObject (iRigidBody* body) const
  {
    return currentCell->FindObject (body);
  }
  // @@@ Temporary
  virtual iDynamicObject* FindObject (iMeshWrapper* mesh) const
  {
    return currentCell->FindObject (mesh);
  }
  // @@@ Temporary
  iDynamicObject* FindObject (uint id) const
  {
    return currentCell->FindObject (id);
  }

  virtual void Save (iDocumentNode* node);
  virtual csRef<iString> Load (iDocumentNode* node);

  virtual void MarkBaseline ();
  virtual csPtr<iDataBuffer> SaveModifications ();
  virtual void RestoreModifications (iDataBuffer* buf);
};


#endif // __CEL_PF_DYNWORLDFACT__

