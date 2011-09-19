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

#include "cssysdef.h"

#include "csutil/event.h"
#include "csutil/eventnames.h"
#include "csutil/eventhandlers.h"
#include "csutil/stringarray.h"
#include "csutil/csstring.h"
#include "csutil/xmltiny.h"
#include "csutil/scfstr.h"
#include "csutil/scanstr.h"
#include "cstool/csview.h"
#include "cstool/simplestaticlighter.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/eventh.h"
#include "iutil/plugin.h"
#include "iengine/sector.h"
#include "iengine/camera.h"
#include "iengine/movable.h"
#include "iengine/imposter.h"
#include "csgeom/math3d.h"
#include "ivaria/bullet.h"
#include "imesh/objmodel.h"

#include "physicallayer/entitytpl.h"
#include "propclass/mesh.h"
#include "propclass/mechsys.h"
#include "plugins/propclass/dynworld/dynworld.h"
#include "tools/elcm.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (DynamicWorld, "pcworld.dynamic")


//---------------------------------------------------------------------------

MeshCache::MeshCache ()
{
}

MeshCache::~MeshCache ()
{
  RemoveMeshes ();
}

void MeshCache::RemoveMeshes ()
{
  csHash<MeshCacheFactory*,csString>::GlobalIterator it = meshFactories.GetIterator ();
  while (it.HasNext ())
  {
    csString key;
    MeshCacheFactory* mf = it.Next (key);
    delete mf;
  }
  meshFactories.DeleteAll ();
}

iMeshWrapper* MeshCache::AddMesh (iEngine* engine,
    iMeshFactoryWrapper* factory, iSector* sector,
    const csReversibleTransform& trans)
{
  const char* factname = factory->QueryObject ()->GetName ();
  csRef<iMeshWrapper> mesh;
  MeshCacheFactory* mf = meshFactories.Get (factname, 0);
  if (mf && mf->meshes.GetSize () > 0)
  {
    mesh = mf->meshes.Pop ();
    mesh->GetMovable ()->SetSector (sector);
  }
  else
  {
    mesh = engine->CreateMeshWrapper (factory, "", sector);
  }

  iMovable* movable = mesh->GetMovable ();
  movable->SetTransform (trans);
  movable->UpdateMove ();
  return mesh;
}

void MeshCache::RemoveMesh (iMeshWrapper* mesh)
{
  const char* factname = mesh->GetFactory ()->QueryObject ()->GetName ();
  MeshCacheFactory* mf = meshFactories.Get (factname, 0);
  if (!mf)
  {
    mf = new MeshCacheFactory ();
    meshFactories.Put (factname, mf);
  }
  mf->meshes.Push (mesh);
  mesh->GetMovable ()->ClearSectors ();
  mesh->GetMovable ()->UpdateMove ();
}

//---------------------------------------------------------------------------------------

DynamicFactory::DynamicFactory (celPcDynamicWorld* world, const char* name,
    float maxradiusRelative, float imposterradius) :
  scfImplementationType (this), name (name), world (world),
  maxradiusRelative (maxradiusRelative), imposterradius (imposterradius)
{
  bbox.StartBoundingBox ();
  physBbox.StartBoundingBox ();

  factory = world->engine->FindMeshFactory (name);
  if (factory == 0)
  {
    printf ("Could not find factory '%s'!\n", name);
    fflush (stdout);
    return;
  }

  imposterradius = 0;
  if (imposterradius > 0)
  {
    imposterFactory = scfQueryInterface<iImposterFactory> (factory);
    imposterFactory->SetMinDistance (imposterradius * world->radius);
    //imposterFactory->SetRotationTolerance (45.0);
    //imposterFactory->SetCameraRotationTolerance(45.0);
    imposterFactory->SetShader("base", "lighting_imposter");
    imposterFactory->SetRenderReal(true);
  }

  iObjectModel* model = factory->GetMeshObjectFactory ()->GetObjectModel ();
  if (model)
  {
    float r;
    csVector3 c;
    model->GetRadius (r, c);
    bsphere.SetCenter (c);
    bsphere.SetRadius (r);
    bbox = model->GetObjectBoundingBox ();
  }
  else
  {
    printf ("WARNING! No object model for %s!\n", factory->QueryObject ()->GetName ());
    fflush (stdout);
  }
}

void DynamicFactory::SetAttribute (const char* name, const char* value)
{
  attributes.Put (name, value);
}

const char* DynamicFactory::GetAttribute (const char* name) const
{
  return attributes.Get (name, (const char*)0);
}

void DynamicFactory::AddRigidBox (const csVector3& offset, const csVector3& size,
    float mass)
{
  csBox3 b (-size / 2 + offset, size / 2 + offset);
  for (size_t i = 0 ; i < 8 ; i++)
    physBbox.AddBoundingVertex (b.GetCorner (i));
  colliders.Push (new DOColliderBox (size, offset, mass));
}

void DynamicFactory::AddRigidSphere (float radius, const csVector3& offset,
    float mass)
{
  csBox3 b (offset - csVector3 (radius), offset + csVector3 (radius));
  for (size_t i = 0 ; i < 8 ; i++)
    physBbox.AddBoundingVertex (b.GetCorner (i));
  colliders.Push (new DOColliderSphere (offset, radius, mass));
}

void DynamicFactory::AddRigidCylinder (float radius, float length,
      const csVector3& offset, float mass)
{
  csBox3 b (offset - csVector3 (radius, length/2, radius),
      offset + csVector3 (radius, length/2, radius));
  for (size_t i = 0 ; i < 8 ; i++)
    physBbox.AddBoundingVertex (b.GetCorner (i));
  colliders.Push (new DOColliderCylinder (offset, length, radius, mass));
}

void DynamicFactory::AddRigidMesh (const csVector3& offset, float mass)
{
  for (size_t i = 0 ; i < 8 ; i++)
    physBbox.AddBoundingVertex (bbox.GetCorner (i));
  colliders.Push (new DOColliderMesh (offset, mass));
}

void DynamicFactory::AddRigidConvexMesh (const csVector3& offset, float mass)
{
  for (size_t i = 0 ; i < 8 ; i++)
    physBbox.AddBoundingVertex (bbox.GetCorner (i));
  colliders.Push (new DOColliderConvexMesh (offset, mass));
}

//---------------------------------------------------------------------------------------

void DynamicObject::Init ()
{
  factory = 0;
  is_static = false;
  is_kinematic = false;
  is_hilight = false;
  hilight_installed = false;
  fade = 0;
  bsphereValid = false;
}

DynamicObject::DynamicObject () : scfImplementationType (this)
{
  Init ();
}

DynamicObject::DynamicObject (DynamicFactory* factory, const csReversibleTransform& trans)
  : scfImplementationType (this)
{
  Init ();
  DynamicObject::factory = factory;
  DynamicObject::trans = trans;
  child = 0;
}

DynamicObject::~DynamicObject ()
{
}

void DynamicObject::InstallHilight (bool hi)
{
  csRef<iShaderVarStringSet> strings =
    csQueryRegistryTagInterface<iShaderVarStringSet>
    (factory->GetWorld ()->GetObjectRegistry (),
     "crystalspace.shader.variablenameset");
  CS::ShaderVarStringID ambient = strings->Request ("light ambient");
  csRef<iShaderVariableContext> svc =
    scfQueryInterfaceSafe<iShaderVariableContext> (mesh);
  if (svc)
  {
    if (hi)
    {
      if (!hilight_installed)
      {
        csRef<csShaderVariable> var = svc->GetVariableAdd(ambient);
        var->SetValue(csVector3(1,1,1));
	hilight_installed = true;
      }
    }
    else if (hilight_installed)
    {
      svc->RemoveVariable(ambient);
      hilight_installed = false;
    }
  }
}

void DynamicObject::SetHilight (bool hi)
{
  if (hi == is_hilight) return;
  is_hilight = hi;
  if (!mesh) return;
  InstallHilight (hi);
}

void DynamicObject::MakeStatic ()
{
  is_static = true;
  if (!is_kinematic && body)
    body->MakeStatic ();
}

void DynamicObject::MakeDynamic ()
{
  is_static = false;
  if (!is_kinematic && body)
    body->MakeDynamic ();
}

void DynamicObject::MakeKinematic ()
{
  if (is_kinematic) return;
  is_kinematic = true;
  if (!body) return;
  csRef<CS::Physics::Bullet::iRigidBody> bulletBody = scfQueryInterface<CS::Physics::Bullet::iRigidBody> (body);
  bulletBody->MakeKinematic ();
}

void DynamicObject::UndoKinematic ()
{
  if (!is_kinematic) return;
  is_kinematic = false;
  if (!body) return;
  if (is_static)
    body->MakeStatic ();
  else
    body->MakeDynamic ();
}

void DynamicObject::SetFade (float f)
{
  fade = f;
  if (mesh)
  {
    mesh->GetMeshObject ()->SetMixMode (CS_FX_SETALPHA(fade));
  }
}

void DynamicObject::RemoveMesh (celPcDynamicWorld* world)
{
  if (!mesh) return;
  InstallHilight (false);
  CS_ASSERT (hilight_installed == false);
  world->dynSys->RemoveBody (body);
  body = 0;
  trans = mesh->GetMovable ()->GetTransform ();
  iImposterFactory* imposterFactory = factory->GetImposterFactory ();
  if (imposterFactory)
    imposterFactory->RemoveImposter (mesh);
  mesh->GetMovable ()->RemoveListener (this);
  world->meshCache.RemoveMesh (mesh);
  mesh = 0;
  if (entity)
  {
    // @@@ TEST THIS WELL!
    csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    if (pcmesh)
      pcmesh->SetMesh (0);
    csRef<iPcMechanicsObject> pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (entity);
    if (pcmechobj)
      pcmechobj->SetBody (0);
  }
}

void DynamicObject::PrepareMesh (celPcDynamicWorld* world)
{
  if (mesh) return;
  mesh = world->meshCache.AddMesh (world->engine, factory->GetMeshFactory (),
      world->sector, trans);
  mesh->GetMovable ()->AddListener (this);
  iGeometryGenerator* ggen = factory->GetGeometryGenerator ();
  if (ggen)
    ggen->GenerateGeometry (mesh);
  InstallHilight (is_hilight);
  CS_ASSERT (hilight_installed == is_hilight);
  SetFade (fade);
  const csPDelArray<DOCollider>& colliders = factory->GetColliders ();
  for (size_t i = 0 ; i < colliders.GetSize () ; i++)
  {
    body = colliders[i]->Create (world->dynSys, mesh, trans, body);
  }
  if (is_static)
    body->MakeStatic ();

  if (entityTemplate && !entity)
  {
    entity = world->pl->CreateEntity (entityTemplate, entityName, params);
    entity->SetID (id);
  }
  if (entity)
  {
    csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
    if (pcmesh)
      pcmesh->SetMesh (mesh);
    csRef<iPcMechanicsObject> pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (entity);
    if (pcmechobj)
      pcmechobj->SetBody (body);
  }
}

void DynamicObject::RefreshColliders ()
{
  if (!body) return;
  body->DestroyColliders ();
  const csPDelArray<DOCollider>& colliders = factory->GetColliders ();
  for (size_t i = 0 ; i < colliders.GetSize () ; i++)
  {
    body = colliders[i]->Create (factory->GetWorld ()->dynSys, mesh, trans, body);
  }
  if (is_static)
    body->MakeStatic ();
}

bool DynamicObject::HasBody (iRigidBody* body)
{
  return body == DynamicObject::body;
}

const csReversibleTransform& DynamicObject::GetTransform ()
{
  if (mesh)
    trans = mesh->GetMovable ()->GetTransform ();
  return trans;
}

void DynamicObject::SetTransform (const csReversibleTransform& trans)
{
  if (mesh)
  {
    mesh->GetMovable ()->SetTransform (trans);
    mesh->GetMovable ()->UpdateMove ();
  }
  DynamicObject::trans = trans;
}

void DynamicObject::Save (iDocumentNode* node, iSyntaxService* syn)
{
  if (mesh)
    trans = mesh->GetMovable ()->GetTransform ();

  node->SetAttribute ("fact", factory->GetName ());
  if (is_static)
    node->SetAttribute ("static", "true");

  csString matrix;
  const csMatrix3& m = trans.GetO2T ();
  matrix.Format ("%g %g %g %g %g %g %g %g %g",
      m.m11, m.m12, m.m13,
      m.m21, m.m22, m.m23,
      m.m31, m.m32, m.m33);
  node->SetAttribute ("m", (const char*)matrix);

  const csVector3& v = trans.GetO2TTranslation ();
  csString vector;
  vector.Format ("%g %g %g", v.x, v.y, v.z);
  node->SetAttribute ("v", (const char*)vector);
  node->SetAttributeAsInt ("id", id);
}

bool DynamicObject::Load (iDocumentNode* node, iSyntaxService* syn,
    celPcDynamicWorld* world)
{
  csString factname = node->GetAttributeValue ("fact");
  factory = world->factory_hash.Get (factname, 0);
  if (!factory) return false;
  if (!syn->ParseBoolAttribute (node, "static", is_static, false, false))
    return false;

  if (node->GetAttribute ("id"))
    id = node->GetAttributeValueAsInt ("id");
  else
    id = world->GetLastID ();

  csMatrix3 m;
  csVector3 v (0);
  csString matrix = node->GetAttributeValue ("m");
  if (matrix.Length () > 0)
  {
    csScanStr ((const char*)matrix, "%f %f %f %f %f %f %f %f %f",
      &m.m11, &m.m12, &m.m13,
      &m.m21, &m.m22, &m.m23,
      &m.m31, &m.m32, &m.m33);
  }
  csString vector = node->GetAttributeValue ("v");
  if (vector.Length () > 0)
  {
    csScanStr ((const char*)vector, "%f %f %f", &v.x, &v.y, &v.z);
  }
  trans.SetO2T (m);
  trans.SetO2TTranslation (v);

  return true;
}

void DynamicObject::MovableChanged (iMovable*)
{
  bsphereValid = false;
  factory->GetWorld ()->tree->MoveObject (child, GetBSphere ());
}

void DynamicObject::MovableDestroyed (iMovable*)
{
}

const csSphere& DynamicObject::GetBSphere () const
{
  if (bsphereValid) return bsphere;
  if (mesh)
    trans = mesh->GetMovable ()->GetTransform ();

  bsphere = trans.This2Other (factory->GetBSphere ());
  bsphereValid = true;

  return bsphere;
}

bool DynamicObject::SetEntity (const char* entityName, iCelParameterBlock* params)
{
  entityTemplate = factory->GetWorld ()->pl->FindEntityTemplate (
      factory->GetName ());
  if (!entityTemplate)
    return false;
  DynamicObject::entityName = entityName;
  DynamicObject::params = params;
  return true;
}

//---------------------------------------------------------------------------------------

class DynWorldELCMListener : public scfImplementation1<DynWorldELCMListener,
  iELCMListener>
{
private:
  celPcDynamicWorld* dynworld;

public:
  DynWorldELCMListener (celPcDynamicWorld* dynworld) :
    scfImplementationType (this), dynworld (dynworld) { }
  virtual ~DynWorldELCMListener () { }
  virtual void SafeToRemove (iCelEntity* entity)
  {
    dynworld->SafeToRemove (entity);
  }
};

//---------------------------------------------------------------------------------------

class ObjDes : public scfImplementation1<ObjDes, CS::Geometry::iObjectDescriptor>
{
public:
  ObjDes () : scfImplementationType (this) { }
  virtual ~ObjDes () {}
  virtual csPtr<iString> DescribeObject (CS::Geometry::KDTreeChild* child)
  {
    DynamicObject* dynobj = (DynamicObject*)child->GetObject ();
    scfString* str = new scfString ();
    iCelEntity* ent = dynobj->GetEntity ();
    iMeshWrapper* mesh = dynobj->GetMesh ();
    str->Format ("%p (ent %s, mesh %s)", dynobj,
	ent ? ent->QueryObject ()->GetName () : "<none>",
	mesh ? mesh->QueryObject ()->GetName () : "<none>");
    return str;
  }
};

celPcDynamicWorld::celPcDynamicWorld (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{  
  radius = 80;
  engine = csQueryRegistry<iEngine> (object_reg);
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  pl->AddScope ("cel.numreg.hash", 1000000000);
  tree = new CS::Geometry::KDTree ();
  ObjDes* objDes = new ObjDes ();
  tree->SetObjectDescriptor (objDes);
  objDes->DecRef ();
  lastID = 1000000001;
}

celPcDynamicWorld::~celPcDynamicWorld ()
{
  DeleteObjects ();
  delete tree;
}

void celPcDynamicWorld::SetELCM (iELCM* elcm)
{
  celPcDynamicWorld::elcm = elcm;
  DynWorldELCMListener* listener = new DynWorldELCMListener (this);
  elcm->AddELCMListener (listener);
  listener->DecRef ();
}

void celPcDynamicWorld::SafeToRemove (iCelEntity* entity)
{
  safeToRemove.Add (entity);
}

iDynamicFactory* celPcDynamicWorld::AddFactory (const char* factory, float maxradius,
    float imposterradius)
{
  csRef<DynamicFactory> obj;
  obj.AttachNew (new DynamicFactory (this, factory, maxradius, imposterradius));
  factories.Push (obj);
  factory_hash.Put (factory, obj);
  return obj;
}

void celPcDynamicWorld::RemoveFactory (iDynamicFactory* factory)
{
  factory_hash.DeleteAll (factory->GetName ());
  factories.Delete (static_cast<DynamicFactory*> (factory));
}

iDynamicFactory* celPcDynamicWorld::FindFactory (const char* name) const
{
  for (size_t i = 0 ; i < factories.GetSize () ; i++)
    if (factories[i]->GetCsName () == name)
      return factories[i];
  return 0;
}

void celPcDynamicWorld::DeleteFactories ()
{
  factory_hash.DeleteAll ();
  factories.DeleteAll ();
}

iDynamicObject* celPcDynamicWorld::AddObject (const char* factory,
    const csReversibleTransform& trans)
{
  csRef<DynamicObject> obj;
  DynamicFactory* fact = factory_hash.Get (factory, 0);
  if (!fact) return 0;
  obj.AttachNew (new DynamicObject (fact, trans));
  objects.Push (obj);

  obj->SetID (GetLastID ());

  CS::Geometry::KDTreeChild* child = tree->AddObject (obj->GetBSphere (), obj);
  obj->SetChild (child);
  return obj;
}

void celPcDynamicWorld::ForceVisible (iDynamicObject* dynobj)
{
  static_cast<DynamicObject*> (dynobj)->PrepareMesh (this);
}

void celPcDynamicWorld::DeleteObjects ()
{
  lastID = 1000000001;
  while (objects.GetSize () > 0)
  {
    csRef<DynamicObject> dynobj = objects.Pop ();
    DeleteObjectInt (dynobj);
  }
  visibleObjects.DeleteAll ();
  fadingOut.DeleteAll ();
  fadingIn.DeleteAll ();
  tree->Clear ();
  meshCache.RemoveMeshes ();
}

void celPcDynamicWorld::DeleteObjectInt (DynamicObject* dyn)
{
  dyn->RemoveMesh (this);
}

void celPcDynamicWorld::DeleteObject (iDynamicObject* dynobj)
{
  DynamicObject* dyn = static_cast<DynamicObject*> (dynobj);
  visibleObjects.Delete (dyn);
  fadingOut.Delete (dyn);
  fadingIn.Delete (dyn);
  size_t idx = objects.Find (dyn);
  if (idx != csArrayItemNotFound)
  {
    if (dyn->GetChild ())
      tree->RemoveObject (dyn->GetChild ());
    dyn->RemoveMesh (this);
    objects.DeleteIndex (idx);
  }
}

iDynamicObject* celPcDynamicWorld::FindDynamicObject (iCelEntity* entity) const
{
  //@@@ Not very efficient!
  for (size_t i = 0 ; i < objects.GetSize () ; i++)
  {
    if (objects[i]->GetEntity () == entity)
      return objects[i];
  }
  return 0;
}

void celPcDynamicWorld::RemoveSafeEntities ()
{
  csSet<csPtrKey<iCelEntity> >::GlobalIterator it = safeToRemove.GetIterator ();
  while (it.HasNext ())
  {
    iCelEntity* entity = it.Next ();
    printf ("Actually remove entity %s\n", entity->GetName ()); fflush (stdout);
    pl->RemoveEntity (entity);
  }
  safeToRemove.DeleteAll ();
}

void celPcDynamicWorld::ProcessFadingIn (float fade_speed)
{
  csSet<csPtrKey<DynamicObject> > newset;
  csSet<csPtrKey<DynamicObject> >::GlobalIterator it = fadingIn.GetIterator ();
  while (it.HasNext ())
  {
    DynamicObject* dyn = it.Next ();
    float f = dyn->GetFade ();
    f -= fade_speed;
    if (f <= 0)
    {
      f = 0;
    }
    else
    {
      newset.Add (dyn);
    }
    dyn->SetFade (f);
  }
  fadingIn = newset;
}

void celPcDynamicWorld::ProcessFadingOut (float fade_speed)
{
  csSet<csPtrKey<DynamicObject> > newset;
  csSet<csPtrKey<DynamicObject> >::GlobalIterator it = fadingOut.GetIterator ();
  while (it.HasNext ())
  {
    DynamicObject* dyn = it.Next ();
    float f = dyn->GetFade ();
    f += fade_speed;
    if (f >= 1)
    {
      f = 1;
      dyn->RemoveMesh (this);
    }
    else
    {
      newset.Add (dyn);
    }
    dyn->SetFade (f);
  }
  fadingOut = newset;
}

struct DynWorldKDData
{
  csSet<csPtrKey<DynamicObject> >& prevObjects;
  csSet<csPtrKey<DynamicObject> >& objects;
  csSet<csPtrKey<iCelEntity> >& safeToRemove;
  csVector3 center;
  float radius;
  float sqradius;

  DynWorldKDData (
      csSet<csPtrKey<DynamicObject> >& prevObjects,
      csSet<csPtrKey<DynamicObject> >& objects,
      csSet<csPtrKey<iCelEntity> >& safeToRemove,
      const csVector3& center, float radius) :
    prevObjects (prevObjects),
    objects (objects),
    safeToRemove (safeToRemove),
    center (center), radius (radius), sqradius (radius * radius) { }
};

static bool DynWorld_Front2Back (CS::Geometry::KDTree* treenode,
	void* userdata, uint32 cur_timestamp, uint32&)
{
  DynWorldKDData* data = (DynWorldKDData*)userdata;

  // In the first part of this test we are going to test if the
  // box vector intersects with the node. If not then we don't
  // need to continue.
  const csBox3& node_bbox = treenode->GetNodeBBox ();
  if (!csIntersect3::BoxSphere (node_bbox, data->center, data->sqradius))
  {
    return false;
  }

  treenode->Distribute ();

  int num_objects;
  CS::Geometry::KDTreeChild** objects;
  num_objects = treenode->GetObjectCount ();
  objects = treenode->GetObjects ();

  int i;
  for (i = 0 ; i < num_objects ; i++)
  {
    if (objects[i]->timestamp != cur_timestamp)
    {
      objects[i]->timestamp = cur_timestamp;

      // Test the bounding box of the object.
      DynamicObject* dynobj = (DynamicObject*)objects[i]->GetObject ();
      float maxradiusRelative = dynobj->GetFactory ()->GetMaximumRadiusRelative ();
      float sqrad = data->sqradius * maxradiusRelative * maxradiusRelative;
      if (dynobj->IsStatic ())
        sqrad *= 1.1f;

#if 1
      const csSphere& obj_bsphere = dynobj->GetBSphere ();
      float sqdist = csSquaredDist::PointPoint (data->center, obj_bsphere.GetCenter ());
      if ((sqdist-csSquare (obj_bsphere.GetRadius () + data->radius)) < 0)
      {
        data->prevObjects.Delete (dynobj);
        data->objects.Add (dynobj);
	iCelEntity* entity = dynobj->GetEntity ();
	if (entity)
	  data->safeToRemove.Delete (entity);
      }
#else
      const csBox3& obj_bbox = dynobj->GetBBox ();
      if (csIntersect3::BoxSphere (obj_bbox, data->center, sqrad))
      {
        data->prevObjects.Delete (dynobj);
        data->objects.Add (dynobj);
	iCelEntity* entity = dynobj->GetEntity ();
	if (entity)
	  data->safeToRemove.Delete (entity);
      }
#endif
    }
  }

  return true;
}

void celPcDynamicWorld::PrepareView (iCamera* camera, float elapsed_time)
{
  float fade_speed = elapsed_time / 3.0;
  ProcessFadingIn (fade_speed);
  ProcessFadingOut (fade_speed);

  const csVector3& campos = camera->GetTransform ().GetOrigin ();

  // Traverse the tree. This will discover all visible objects and
  // put them in the 'visibleObjects' set. All visible entities
  // are removed from the 'safeToRemove' set.
  csSet<csPtrKey<DynamicObject> > prevVisible = visibleObjects;
  visibleObjects.Empty ();
  DynWorldKDData data (prevVisible, visibleObjects, safeToRemove,
      campos, radius);
  tree->Front2Back (data.center, DynWorld_Front2Back, (void*)&data, 0);

  // All entities remaining in 'safeToRemove' can really be removed.
  RemoveSafeEntities ();

  csSet<csPtrKey<DynamicObject> >::GlobalIterator it = prevVisible.GetIterator ();
  while (it.HasNext ())
  {
    DynamicObject* dyn = it.Next ();
    if (dyn->GetMesh ())
    {
      fadingIn.Delete (dyn);
      fadingOut.Add (dyn);
    }
  }

  csSet<csPtrKey<DynamicObject> >::GlobalIterator it2 = visibleObjects.GetIterator ();
  while (it2.HasNext ())
  {
    DynamicObject* dyn = it2.Next ();
    if (!dyn->GetMesh ())
    {
      dyn->PrepareMesh (this);
      fadingOut.Delete (dyn);
      fadingIn.Add (dyn);
    }
  }
}

iDynamicObject* celPcDynamicWorld::FindObject (iRigidBody* body)
{
  size_t i;
  for (i = 0 ; i < objects.GetSize () ; i++)
  {
    DynamicObject* dyn = objects[i];
    if (dyn->HasBody (body))
      return dyn;
  }
  return 0;
}

iDynamicObject* celPcDynamicWorld::FindObject (iMeshWrapper* mesh)
{
  size_t i;
  for (i = 0 ; i < objects.GetSize () ; i++)
  {
    DynamicObject* dyn = objects[i];
    if (dyn->GetMesh () == mesh)
      return dyn;
  }
  return 0;
}

void celPcDynamicWorld::Setup (iSector* sector, iDynamicSystem* dynSys)
{
  celPcDynamicWorld::sector = sector;
  celPcDynamicWorld::dynSys = dynSys;
}

void celPcDynamicWorld::SetRadius (float radius)
{
  celPcDynamicWorld::radius = radius;
}

void celPcDynamicWorld::Save (iDocumentNode* node)
{
  csRef<iSyntaxService> syn = csQueryRegistryOrLoad<iSyntaxService> (object_reg,
      "crystalspace.syntax.loader.service.text");

  node->SetAttributeAsInt ("lastid", lastID);

  for (size_t i = 0 ; i < objects.GetSize () ; i++)
  {
    csRef<iDocumentNode> el = node->CreateNodeBefore (CS_NODE_ELEMENT);
    el->SetValue ("obj");
    objects[i]->Save (el, syn);
  }
}

csRef<iString> celPcDynamicWorld::Load (iDocumentNode* node)
{
  csRef<iSyntaxService> syn = csQueryRegistryOrLoad<iSyntaxService> (object_reg,
      "crystalspace.syntax.loader.service.text");

  if (node->GetAttribute ("lastid"))
    lastID = node->GetAttributeValueAsInt ("lastid");
  else
    lastID = 1000000001;        // @@@ Is this right?

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    csString value = child->GetValue ();
    if (value == "obj")
    {
      csRef<DynamicObject> dynobj;
      dynobj.AttachNew (new DynamicObject ());
      if (!dynobj->Load (child, syn, this))
      {
        csRef<iString> str;
        str.AttachNew (new scfString ("Error loading object!"));
        return str;
      }
      objects.Push (dynobj);
      CS::Geometry::KDTreeChild* child = tree->AddObject (dynobj->GetBSphere (),
	  dynobj);
      dynobj->SetChild (child);
    }
  }

  return 0;
}

void celPcDynamicWorld::Dump ()
{
  printf ("### DynWorld ###\n");
  printf ("  Fading in=%d, out=%d\n", fadingIn.GetSize (), fadingOut.GetSize ());
  printf ("  Visible objects=%d\n", visibleObjects.GetSize ());
  printf ("  Safe to remove=%d\n", safeToRemove.GetSize ());
}

