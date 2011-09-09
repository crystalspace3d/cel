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

#include "csgeom/kdtree.h"
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
  celPcDynamicWorld* GetWorld () const { return world; }
  iImposterFactory* GetImposterFactory () const { return imposterFactory; }
};

class DynamicObject : public scfImplementation2<DynamicObject,
  iDynamicObject, iMovableListener>
{
private:
  DynamicFactory* factory;
  csRef<iMeshWrapper> mesh;
  mutable csReversibleTransform trans;
  csRef<iRigidBody> body;
  bool is_static;
  bool is_kinematic;
  bool is_hilight;
  bool hilight_installed;
  float fade;
  csKDTreeChild* child;

  csRef<iCelEntityTemplate> entityTemplate;
  csString entityName;
  celEntityTemplateParams params;
  csWeakRef<iCelEntity> entity;

  void InstallHilight (bool hi);
  void Init ();

  mutable csBox3 bbox;
  mutable bool bboxValid;

  mutable csSphere bsphere;
  mutable bool bsphereValid;

public:
  DynamicObject ();
  DynamicObject (DynamicFactory* factory, const csReversibleTransform& trans);
  virtual ~DynamicObject ();

  csKDTreeChild* GetChild () const { return child; }
  void SetChild (csKDTreeChild* child) { DynamicObject::child = child; }

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
  virtual const csBox3& GetBBox () const;
  const csSphere& GetBSphere () const;
  virtual void RefreshColliders ();
  virtual const csReversibleTransform& GetTransform ();
  virtual void SetTransform (const csReversibleTransform& trans);
  virtual bool SetEntityTemplate (const char* templateName,
      const char* entityName, const celEntityTemplateParams& params);

  virtual void MovableChanged (iMovable* movable);
  virtual void MovableDestroyed (iMovable* movable);

  void PrepareMesh (celPcDynamicWorld* world);
  void RemoveMesh (celPcDynamicWorld* world);
  void Save (iDocumentNode* node, iSyntaxService* syn);
  bool Load (iDocumentNode* node, iSyntaxService* syn, celPcDynamicWorld* world);

  void SetFade (float f);
  float GetFade () const { return fade; }
  iCelEntity* GetEntity () const { return entity; }

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

class celPcDynamicWorld : public scfImplementationExt1<celPcDynamicWorld,
  celPcCommon, iPcDynamicWorld>
{
public:
  csRef<iEngine> engine;
  csRef<iDynamicSystem> dynSys;
  csRef<iCelPlLayer> pl;
  csRef<iVirtualClock> vc;
  csRefArray<DynamicObject> objects;
  csRefArray<DynamicFactory> factories;
  //csRefArray<CurvedMeshDynamicObjectCreator> cmdocs;
  csHash<DynamicFactory*,csString> factory_hash;
  iSector* sector;
  MeshCache meshCache;
  float radius;
  csSet<csPtrKey<DynamicObject> > fadingIn;
  csSet<csPtrKey<DynamicObject> > fadingOut;
  csKDTree tree;
  csRef<iELCM> elcm;

  csSet<csPtrKey<DynamicObject> > visibleObjects;

  // A set of entities which are safe to remove. This information
  // comes from the ELCM.
  csSet<csPtrKey<iCelEntity> > safeToRemove;
  // This function will remove all entities that are still in safeToRemove.
  // The 'visibility' traversal should have removed all entities that
  // are attached to a visible mesh from this set first.
  void RemoveSafeEntities ();

  void ProcessFadingIn (float fade_speed);
  void ProcessFadingOut (float fade_speed);

public:
  celPcDynamicWorld (iObjectRegistry* object_reg);
  virtual ~celPcDynamicWorld ();

  iObjectRegistry* GetObjectRegistry () { return object_reg; }

  virtual void SetELCM (iELCM* elcm);
  void SafeToRemove (iCelEntity* entity);
  virtual void Dump ();

  virtual iDynamicFactory* AddFactory (const char* factory, float maxradius,
      float imposterradius);
  virtual void RemoveFactory (iDynamicFactory* factory);
  virtual void DeleteFactories ();
  virtual size_t GetFactoryCount () const { return factories.GetSize () ; }
  virtual iDynamicFactory* GetFactory (size_t index) const { return factories[index]; }

  virtual iDynamicObject* AddObject (const char* factory,
      const csReversibleTransform& trans);
  virtual void ForceVisible (iDynamicObject* dynobj);
  virtual void DeleteObject (iDynamicObject* dynobj);
  virtual void DeleteObjects ();
  virtual void Setup (iSector* sector, iDynamicSystem* dynSys);
  virtual void SetRadius (float radius);
  virtual float GetRadius () const { return radius; }
  virtual void PrepareView (iCamera* camera, float elapsed_time);
  virtual iDynamicObject* FindObject (iRigidBody* body);
  virtual iDynamicObject* FindObject (iMeshWrapper* mesh);
  virtual void Save (iDocumentNode* node);
  virtual csRef<iString> Load (iDocumentNode* node);
};


#endif // __CEL_PF_DYNWORLDFACT__

