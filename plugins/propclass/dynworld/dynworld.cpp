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
#include "csutil/scfstrset.h"
#include "csutil/scanstr.h"
#include "csutil/memfile.h"
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

  atBaseline = false;
  hasMovedSufficiently = false;
  lastUpdateNr = 0;
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

bool DynamicObject::HasMovedSufficiently ()
{
  if (hasMovedSufficiently) return true;
  if (!atBaseline) return false;
  if (!mesh) return false;      // We don't know.
  iMovable* movable = mesh->GetMovable ();
  long updateNr = movable->GetUpdateNumber ();
  if (updateNr != lastUpdateNr)
  {
    const csReversibleTransform& meshTrans = movable->GetTransform ();
    lastUpdateNr = updateNr;
    if (!((trans.GetOrigin ()-meshTrans.GetOrigin ()) < 0.1f))
    {
      printf ("Object '%s' has now moved sufficiently!\n",
          entity ? entity->GetName () : "unknown"); fflush (stdout);
      hasMovedSufficiently = true;
    }
    // @@@ TODO: implement further checks on transform/matrix.
  }
  return hasMovedSufficiently;
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
  world->GetMeshToDynObj ().DeleteAll ((iMeshWrapper*)mesh);
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
  world->GetMeshToDynObj ().PutUnique ((iMeshWrapper*)mesh, this);
  mesh->GetMovable ()->AddListener (this);
  lastUpdateNr = mesh->GetMovable ()->GetUpdateNumber ();
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

  ForceEntity (world);
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
    return mesh->GetMovable ()->GetTransform ();
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
  csReversibleTransform& tr = trans;
  if (mesh)
    tr = mesh->GetMovable ()->GetTransform ();

  node->SetAttribute ("fact", factory->GetName ());
  if (is_static)
    node->SetAttribute ("static", "true");

  csString matrix;
  const csMatrix3& m = tr.GetO2T ();
  matrix.Format ("%g %g %g %g %g %g %g %g %g",
      m.m11, m.m12, m.m13,
      m.m21, m.m22, m.m23,
      m.m31, m.m32, m.m33);
  node->SetAttribute ("m", (const char*)matrix);

  const csVector3& v = tr.GetO2TTranslation ();
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

void DynamicObject::MovableChanged (iMovable* movable)
{
  bsphereValid = false;
  factory->GetWorld ()->tree->MoveObject (child, GetBSphere ());
  factory->GetWorld ()->checkForMovement.Add (this);
}

void DynamicObject::MovableDestroyed (iMovable*)
{
}

const csSphere& DynamicObject::GetBSphere () const
{
  if (bsphereValid) return bsphere;
  if (mesh)
    bsphere = mesh->GetMovable ()->GetTransform ().This2Other (
        factory->GetBSphere ());
  else
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

void DynamicObject::LinkEntity (iCelEntity* entity)
{
  entityTemplate = factory->GetWorld ()->pl->FindEntityTemplate (
      factory->GetName ());
  if (!entityTemplate)
    return;
  DynamicObject::entity = entity;
  DynamicObject::entityName = entity->GetName ();
  SetID (entity->GetID ());
}

void DynamicObject::UnlinkEntity ()
{
  entity = 0;
}

iCelEntity* DynamicObject::ForceEntity (celPcDynamicWorld* world)
{
  if (!entity)
  {
    // First we check if the entity already exists.
    entity = world->pl->GetEntity (id);
    if (entity)
    {
      entity->SetName (entityName);
      if (entityTemplate)
        world->pl->ApplyTemplate (entity, entityTemplate, params);
    }
  }
  if (entityTemplate && !entity)
  {
    // No, create one with the given id.
    entity = world->pl->CreateEntity (entityTemplate, entityName, params);
    entity->SetID (id);
    if (atBaseline)
    {
      // The elcm normally automatically registers new entities as
      // being new after the baseline. But since we know this entity
      // is part of the baseline (since our DynObj is part of the baseline)
      // we must unregister it here.
      if (world->elcm) world->elcm->UnregisterNewEntity (entity);
      entity->MarkBaseline ();
    }
  }
  return entity;
}

csPtr<iString> DynamicObject::GetDescription () const
{
  scfString* statusLine = new scfString ();
  const char* bl, *mo;
  if (atBaseline) bl = " base";
  else bl = "";
  if (hasMovedSufficiently) mo = " moved";
  else mo = "";
  if (!entity)
  {
    statusLine->Format ("%s (%d)%s%s", GetFactory ()->GetName (), id, bl, mo);
  }
  else
  {
    const char* ac;
    if (entity->IsActive ()) ac = " active";
    else ac = "";
    const char* mod;
    if (entity->IsModifiedSinceBaseline ()) mod = " mod";
    else mod = "";
    const char* entbl;
    if (entity->ExistedAtBaseline ()) entbl = " ebase";
    else entbl = "";
    statusLine->Format ("%s (%d:%s)%s%s%s%s%s", GetFactory ()->GetName (), id,
	entity->GetName (), bl, mo, ac, mod, entbl);
  }
  return statusLine;
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
  scopeIdx = pl->AddScope ("cel.numreg.hash", 1000000000);
  tree = new CS::Geometry::KDTree ();
  tree->SetMinimumSplitAmount (50);
  ObjDes* objDes = new ObjDes ();
  tree->SetObjectDescriptor (objDes);
  objDes->DecRef ();
  lastID = 1000000001;
  scopeIdx = csArrayItemNotFound;
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

void celPcDynamicWorld::CheckForMovement ()
{
  csSet<csPtrKey<DynamicObject> >::GlobalIterator it = checkForMovement.GetIterator ();
  while (it.HasNext ())
  {
    DynamicObject* dynobj = it.Next ();
    if (dynobj->ExistedAtBaseline () && dynobj->HasMovedSufficiently ())
    {
      haveMovedFromBaseline.Add (dynobj);
    }
  }
  checkForMovement.DeleteAll ();
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
  if (!fact)
  {
    printf ("Cannot find factory '%s' for AddObject!\n", factory);
    return 0;
  }
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

void celPcDynamicWorld::ForceInvisible (iDynamicObject* dynobj)
{
  DynamicObject* dyn = static_cast<DynamicObject*> (dynobj);
  dyn->RemoveMesh (this);
}

void celPcDynamicWorld::DeleteObjects ()
{
  lastID = 1000000001;
  while (objects.GetSize () > 0)
  {
    csRef<DynamicObject> dynobj = objects.Pop ();
    DeleteObjectInt (dynobj);
  }
  haveMovedFromBaseline.DeleteAll ();
  checkForMovement.DeleteAll ();
  visibleObjects.DeleteAll ();
  fadingOut.DeleteAll ();
  fadingIn.DeleteAll ();
  tree->Clear ();
  meshCache.RemoveMeshes ();
  if (scopeIdx != csArrayItemNotFound)
    pl->ResetScope (scopeIdx);
}

void celPcDynamicWorld::DeleteObjectInt (DynamicObject* dyn)
{
  dyn->RemoveMesh (this);
  if (dyn->GetEntity ())
    pl->RemoveEntity (dyn->GetEntity ());
}

void celPcDynamicWorld::DeleteObject (iDynamicObject* dynobj)
{
  DynamicObject* dyn = static_cast<DynamicObject*> (dynobj);
  if (dyn->GetEntity ())
    pl->RemoveEntity (dyn->GetEntity ());
  checkForMovement.Delete (dyn);
  haveMovedFromBaseline.Delete (dyn);
  visibleObjects.Delete (dyn);
  fadingOut.Delete (dyn);
  fadingIn.Delete (dyn);
  // @@@ This is a pretty slow operation.
  size_t idx = objects.Find (dyn);
  if (idx != csArrayItemNotFound)
  {
    if (dyn->GetChild ())
      tree->RemoveObject (dyn->GetChild ());
    dyn->RemoveMesh (this);
    objects.DeleteIndex (idx);
  }
}


void celPcDynamicWorld::RemoveSafeEntities ()
{
  csSet<csPtrKey<iCelEntity> >::GlobalIterator it = safeToRemove.GetIterator ();
  while (it.HasNext ())
  {
    iCelEntity* entity = it.Next ();
    printf ("Actually remove entity %s\n", entity->GetName ());
    uint id = entity->GetID ();
    bool existedAtBaseline = entity->ExistedAtBaseline ();
    pl->RemoveEntity (entity);
    // The elcm normally automatically registers deleted entities as
    // being deleted if they existed before the baseline. But if this entity
    // existed at the baseline then we don't want to do that because this
    // entity deletion is not 'real'. We're just swapping out the entity.
    if (existedAtBaseline && elcm)
    {
      elcm->UnregisterDeletedEntity (id);
    }
  }
  safeToRemove.DeleteAll ();
  fflush (stdout);
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
  // box intersects with the node. If not then we don't
  // need to continue.
  const csBox3& node_bbox = treenode->GetNodeBBox ();
  if (!csIntersect3::BoxSphere (node_bbox, data->center, data->sqradius))
  {
    return false;
  }

#if 0
  // Special case if our node is entirely in the sphere. Then we don't
  // have to test our objects.
  csBox3 b (node_bbox.Min () - data->center, node_bbox.Max () - data->center);
  printf ("%g,%g,%g - %g,%g,%g: b.SquaredOriginMaxDist=%g data->sqradius=%g\n",
      node_bbox.Min().x,
      node_bbox.Min().y,
      node_bbox.Min().z,
      node_bbox.Max().x,
      node_bbox.Max().y,
      node_bbox.Max().z,
      b.SquaredOriginMaxDist (), data->sqradius);
  if (b.SquaredOriginMaxDist () < data->sqradius)
  {
    printf ("Special case!\n");
  }
#endif

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

  // First we check if some of the dynamic objects moved sufficiently.
  CheckForMovement ();

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

iDynamicObject* celPcDynamicWorld::FindObject (iCelEntity* entity) const
{
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  if (!pcmesh) return 0;	// Impossible to find efficiently. We don't bother.
  iMeshWrapper* mesh = pcmesh->GetMesh ();
  if (!mesh) return 0;		// There will be no dynobj with this mesh.
  return FindObject (mesh);
}

iDynamicObject* celPcDynamicWorld::FindObject (uint id) const
{
  //@@@ Not very efficient!
  for (size_t i = 0 ; i < objects.GetSize () ; i++)
  {
    if (objects[i]->GetID () == id)
      return objects[i];
  }
  return 0;
}

iDynamicObject* celPcDynamicWorld::FindObject (iRigidBody* body) const
{
  //@@@ Not very efficient!
  for (size_t i = 0 ; i < objects.GetSize () ; i++)
  {
    DynamicObject* dyn = objects[i];
    if (dyn->HasBody (body))
      return dyn;
  }
  return 0;
}

iDynamicObject* celPcDynamicWorld::FindObject (iMeshWrapper* mesh) const
{
  iDynamicObject* dynobj = meshToDynObj.Get (mesh, 0);
  return dynobj;
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

void celPcDynamicWorld::MarkBaseline ()
{
  if (elcm)
    elcm->MarkBaseline ();
  for (size_t i = 0 ; i < objects.GetSize () ; i++)
    objects[i]->MarkBaseline ();
}

void celPcDynamicWorld::Dump ()
{
  printf ("### DynWorld ###\n");
  printf ("  Fading in=%d, out=%d\n", fadingIn.GetSize (), fadingOut.GetSize ());
  printf ("  Visible objects=%d\n", visibleObjects.GetSize ());
  printf ("  Safe to remove=%d\n", safeToRemove.GetSize ());
}

// ---------------------------------------------------------------------------
// Saving and loading
// ---------------------------------------------------------------------------

void celPcDynamicWorld::SaveTransform (iCelCompactDataBufferWriter* buf,
      const csReversibleTransform& trans)
{
  buf->AddVector3 (trans.GetOrigin ());
  buf->AddVector3 (trans.GetO2T ().Row1 ());
  buf->AddVector3 (trans.GetO2T ().Row2 ());
  buf->AddVector3 (trans.GetO2T ().Row3 ());
}

void celPcDynamicWorld::LoadTransform (iCelCompactDataBufferReader* buf,
      csReversibleTransform& trans)
{
  csVector3 v, r1, r2, r3;
  buf->GetVector3 (v);
  buf->GetVector3 (r1);
  buf->GetVector3 (r2);
  buf->GetVector3 (r3);
  trans.SetOrigin (v);
  csMatrix3 m;
  m.SetRow1 (r1);
  m.SetRow2 (r2);
  m.SetRow3 (r3);
  trans.SetO2T (m);
}

void celPcDynamicWorld::SaveStrings (iCelCompactDataBufferWriter* buf,
    csScfStringSet* strings)
{
  csStringSet::GlobalIterator it = strings->GetIterator ();
  while (it.HasNext ())
  {
    const char* str;
    csStringID id = it.Next (str);
    buf->AddID (id);
    buf->AddString32 (str);
  }
  buf->AddID (csInvalidStringID);
}

void celPcDynamicWorld::LoadStrings (iCelCompactDataBufferReader* buf,
      csHash<csString,csStringID>& strings)
{
  csStringID id = buf->GetID ();
  while (id != csInvalidStringID)
  {
    const char* str = buf->GetString32 ();
    strings.Put (id, str);
    id = buf->GetID ();
  }
}

#define MARKER_NEW 0xe3
#define MARKER_END 0x3e

csPtr<iDataBuffer> celPcDynamicWorld::SaveModifications ()
{
  csRef<csScfStringSet> strings;
  strings.AttachNew (new csScfStringSet ());
  csRef<iCelCompactDataBufferWriter> buf = pl->CreateCompactDataBufferWriter ();

  if (elcm)
  {
    const csSet<uint>& deletedEntities = elcm->GetDeletedEntities ();
    buf->AddUInt32 (deletedEntities.GetSize ());
    printf ("Save %d deleted items\n", deletedEntities.GetSize ());
    csSet<uint>::GlobalIterator delIt = deletedEntities.GetIterator ();
    while (delIt.HasNext ())
    {
      uint id = delIt.Next ();
      buf->AddUInt32 (id);
    }

    csSet<csPtrKey<iCelEntity> > newEntites = elcm->GetNewEntities ();

    // In this set we keep all dynamic objects that were already saved.
    csSet<csPtrKey<DynamicObject> > alreadySaved;

    csRef<iCelEntityIterator> it = elcm->GetModifiedEntities ();
    while (it->HasNext ())
    {
      iCelEntity* entity = it->Next ();
      buf->AddUInt8 (MARKER_NEW);
      buf->AddUInt32 (entity->GetID ());

      iDynamicObject* dynobj = FindObject (entity);
      if (!dynobj)
      {
        // There are two cases:
        // 1. This is an entity that used to belong to a pre-baseline
        //    dynobj but is now put in an inventory. In this case we
        //    must make sure that at load-time the DynObj is removed
        //    again.
        // 2. This is an entity was not part of a pre-baseline dynobj.
        // Both situation can be solved by saving that a dynobj is not
        // present and then at load time we can delete the pre-baseline
        // DynObj if it exists (will be only for case 1).
        // In case 2 we also need to store how to create the entity.
        buf->AddBool (false);           // Indication that there is no dynobj.
        if (newEntites.Contains (entity))
        {
          printf ("New entity without dynobj '%s'!\n", entity->GetName ());
          // Try to get the factory/template name out of the mesh.
          csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
          if (pcmesh && pcmesh->GetFactoryName () != 0 && *pcmesh->GetFactoryName () != 0)
          {
            buf->AddID (strings->Request (pcmesh->GetFactoryName ()));
            buf->AddID (strings->Request (entity->GetName ()));
          }
          else
          {
            printf ("Can't save '%s'!\n", entity->GetName ());
            return 0;
          }
        }
        else
        {
          printf ("Existing entity without dynobj '%s'!\n", entity->GetName ());
          buf->AddID (csInvalidStringID);	// No template.
        }
      }
      else
      {
        buf->AddBool (true);            // Indication that there is a dynobj.
        alreadySaved.Add (static_cast<DynamicObject*> (dynobj));

        if (newEntites.Contains (entity))
        {
          printf ("Saving new entity '%s'!\n", entity->GetName ());
	  newEntites.Delete (entity);
          iDynamicFactory* dynfact = dynobj->GetFactory ();
          buf->AddID (strings->Request (dynfact->GetName ()));
          buf->AddID (strings->Request (entity->GetName ()));
        }
        else
        {
          printf ("Saving entity '%s'!\n", entity->GetName ());
          buf->AddID (csInvalidStringID);	// No template.
        }
        // @@@ For now we just save the position if the entity is modified.
        // Perhaps this is even a good idea. Have to think about this more.
        SaveTransform (buf, dynobj->GetTransform ());
      }

      entity->SaveModifications (buf, strings);
    }

    // All remaining entities in 'newEntites' also have to be saved.
    csSet<csPtrKey<iCelEntity> >::GlobalIterator newIt = newEntites.GetIterator ();
    while (newIt.HasNext ())
    {
      iCelEntity* entity = newIt.Next ();
      DynamicObject* dynobj = static_cast<DynamicObject*> (FindObject (entity));
      alreadySaved.Add (dynobj);
      buf->AddUInt8 (MARKER_NEW);
      buf->AddUInt32 (entity->GetID ());
      buf->AddBool (true);           // Indication that there is a dynobj.
      iDynamicFactory* dynfact = dynobj->GetFactory ();
      buf->AddID (strings->Request (dynfact->GetName ()));
      buf->AddID (strings->Request (entity->GetName ()));
      // @@@ For now we just save the position if the entity is modified.
      // Perhaps this is even a good idea. Have to think about this more.
      SaveTransform (buf, dynobj->GetTransform ());
      entity->SaveModifications (buf, strings);
      printf ("Saving new pristine entity '%s'!\n", entity->GetName ());
    }
    buf->AddUInt8 (MARKER_END);

    // Now it is possible that we still have dynamic objects for which
    // the entity hasn't changed but the dynobj itself has changed (i.e. moved).
    // We save those here.
    csSet<csPtrKey<DynamicObject> >::GlobalIterator dynIt = haveMovedFromBaseline.GetIterator ();
    while (dynIt.HasNext ())
    {
      DynamicObject* dynobj = dynIt.Next ();
      if (!alreadySaved.Contains (dynobj))
      {
        printf ("Saving moved dynobj '%s'\n", dynobj->GetEntity () ?
            dynobj->GetEntity ()->GetName () : "unknown");
        buf->AddUInt8 (MARKER_NEW);
        buf->AddUInt32 (dynobj->GetID ());
        SaveTransform (buf, dynobj->GetTransform ());
      }
    }
    buf->AddUInt8 (MARKER_END);
  }
  else
  {
    // @@@
    printf ("Saving without ELCM is not yet supported!\n");
  }

  csRef<iCelCompactDataBufferWriter> sbuf = pl->CreateCompactDataBufferWriter ();
  SaveStrings (sbuf, strings);
  csMemFile file;
  file.Write (sbuf->GetData (), sbuf->GetSize ());
  file.Write (buf->GetData (), buf->GetSize ());
  return file.GetAllData ();
}

void celPcDynamicWorld::RestoreModifications (iDataBuffer* dbuf)
{
  csHash<csString,csStringID> strings;
  csRef<iCelCompactDataBufferReader> buf = pl->CreateCompactDataBufferReader (dbuf);
  LoadStrings (buf, strings);

  if (elcm)
  {
    size_t delSize = (size_t)buf->GetUInt32 ();
    printf ("%d to delete\n", delSize);
    for (size_t i = 0 ; i < delSize ; i++)
    {
      uint id = (uint)buf->GetUInt32 ();
      elcm->RegisterDeletedEntity (id);
      DynamicObject* dynobj = static_cast<DynamicObject*> (FindObject (id));
      if (dynobj)
      {
	DeleteObject (dynobj);
      }
      else
      {
	iCelEntity* entity = pl->GetEntity (id);
	printf ("Delete entity %s\n", entity->GetName ());
	pl->RemoveEntity (entity);
      }
    }

    uint8 marker = buf->GetUInt8 ();
    while (marker == MARKER_NEW)
    {
      uint id = buf->GetUInt32 ();
      bool hasDynObj = buf->GetBool ();

      csStringID tmpID = buf->GetID ();
      csStringID entNameID = csInvalidStringID;
      if (tmpID != csInvalidStringID)
      {
        entNameID = buf->GetID ();
      }

      DynamicObject* dynobj = 0;
      iCelEntity* entity;
      if (tmpID != csInvalidStringID)
      {
        // Entity has to be created.
        const char* entName = strings.Get (entNameID, (const char*)0);
        const char* tmpName = strings.Get (tmpID, (const char*)0);
        if (hasDynObj)
        {
	  printf ("Loading new entity/dynobj '%s' from '%s'\n", entName, tmpName);
          csReversibleTransform trans;
          LoadTransform (buf, trans);
          dynobj = static_cast<DynamicObject*> (AddObject (tmpName, trans));
          dynobj->SetID (id);
          dynobj->SetEntity (entName, 0); // @@@ params?
          entity = dynobj->ForceEntity (this);
        }
        else
        {
	  printf ("Loading new entity '%s' from '%s'\n", entName, tmpName);
          iCelEntityTemplate* tmp = pl->FindEntityTemplate (tmpName);
          if (!tmp)
          {
            printf ("Error locating entity template '%s'\n", tmpName);
          }
          // First we see if the entity already exists.
          entity = pl->GetEntity (id);
          if (!entity)
          {
            entity = pl->CreateEntity (tmp, entName, (iCelParameterBlock*)0);      // @@@ params?
            entity->SetID (id);
          }
          else
          {
            entity->SetName (entName);
          }
          csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
          if (pcmesh)
            pcmesh->SetFactoryName (tmpName);
        }
        elcm->RegisterNewEntity (entity);
      }
      else
      {
        dynobj = static_cast<DynamicObject*> (FindObject (id));
	if (dynobj)
	{
          // @@@ Can we avoid this? What if entity is baseline but dynobj is not?
          entity = dynobj->ForceEntity (this);
          if (hasDynObj)
          {
	    printf ("Loading existing entity '%s'\n", entity->GetName ());
            csReversibleTransform trans;
            LoadTransform (buf, trans);
            dynobj->SetTransform (trans);
          }
          else
          {
	    printf ("Loading existing entity but with deleted dynobj '%s'\n", entity->GetName ());
            ForceInvisible (dynobj);
            dynobj->UnlinkEntity ();
            DeleteObject (dynobj);
          }
	}
	else
	{
	  entity = pl->GetEntity (id);
	  if (!entity)
	  {
	    printf ("ERROR! Can't find entity with id %d\n", id);
	    return;
	  }
	  printf ("Loading existing entity without dynobj '%s'\n", entity->GetName ()); fflush (stdout);
          // @@@ The case where hasDynObj = true is not handled here. Is
          // this even possible?
	}
      }

      if (dynobj) dynobj->ClearBaseline ();
      entity->RestoreModifications (buf, strings);

      marker = buf->GetUInt8 ();
    }
    if (marker != MARKER_END)
    {
      printf ("Bad marker (1)!\n");
      return;
    }

    // Now load the remaining moved dynobjects.
    marker = buf->GetUInt8 ();
    while (marker == MARKER_NEW)
    {
      uint id = buf->GetUInt32 ();
      DynamicObject* dynobj = static_cast<DynamicObject*> (FindObject (id));
      printf ("Loading moved dynobj\n"); fflush (stdout);
      csReversibleTransform trans;
      LoadTransform (buf, trans);
      dynobj->SetTransform (trans);
      // Note! We restore the haveMovedFromBaseline set (almost) as it
      // was when we saved it. The reason for the difference is that
      // we didn't save (in this section) the dynamic objects which also
      // had modified entities. These were save dabove.
      haveMovedFromBaseline.Add (dynobj);

      marker = buf->GetUInt8 ();
    }
    if (marker != MARKER_END)
    {
      printf ("Bad marker (2)!\n");
      return;
    }
  }
}

