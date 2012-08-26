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
#include "csutil/sysfunc.h"
#include "csutil/memfile.h"
#include "cstool/csview.h"
#include "cstool/simplestaticlighter.h"
#include "cstool/genmeshbuilder.h"
#include "cstool/collider.h"
#include "csgfx/imagememory.h"
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
#include "ivideo/graph3d.h"
#include "ivideo/txtmgr.h"

#include "physicallayer/entitytpl.h"
#include "propclass/mesh.h"
#include "propclass/mechsys.h"
#include "propclass/spawn.h"
#include "plugins/propclass/dynworld/dynworld.h"
#include "tools/elcm.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (DynamicWorld, "pcworld.dynamic")


//---------------------------------------------------------------------------

class DynamicCellIterator : public scfImplementation1<DynamicCellIterator,
  iDynamicCellIterator>
{
private:
  csHash<csRef<DynamicCell>,csString>::ConstGlobalIterator it;

public:
  DynamicCellIterator (csHash<csRef<DynamicCell>,csString>::ConstGlobalIterator& it) :
    scfImplementationType (this), it (it) { }
  virtual ~DynamicCellIterator () { }
  virtual bool HasNext () const { return it.HasNext (); }
  virtual iDynamicCell* NextCell ()
  {
    csString name;
    csRef<DynamicCell> cell = it.Next (name);
    return static_cast<iDynamicCell*> (cell);
  }
};


//---------------------------------------------------------------------------

struct dynobjFinder : public scfImplementationExt1<
	dynobjFinder, csObject, scfFakeInterface<dynobjFinder> >
{
public:
  SCF_INTERFACE (dynobjFinder, 0, 0, 1);

private:
  iDynamicObject* dynobj;

public:
  dynobjFinder (iDynamicObject* dynobj) : scfImplementationType (this), dynobj (dynobj) { }
  virtual ~dynobjFinder () { }
  iDynamicObject* GetDynamicObject () const { return dynobj; }

  static void AttachDynObj (iMeshWrapper* mesh, iDynamicObject* dynobj);
  static void UnattachDynObj (iMeshWrapper* mesh, iDynamicObject* dynobj);
  static void AttachDynObj (iLight* light, iDynamicObject* dynobj);
  static void UnattachDynObj (iLight* light, iDynamicObject* dynobj);

  static iDynamicObject* FindAttachedDynObj (iLight* light)
  {
    csRef<dynobjFinder> cef (CS::GetChildObject<dynobjFinder> (light->QueryObject ()));
    if (cef)
      return cef->GetDynamicObject ();
    return 0;
  }
  static iDynamicObject* FindAttachedDynObj (iMeshWrapper* mesh)
  {
    csRef<dynobjFinder> cef (CS::GetChildObject<dynobjFinder> (mesh->QueryObject ()));
    if (cef)
      return cef->GetDynamicObject ();
    return 0;
  }
};

void dynobjFinder::AttachDynObj (iLight* light, iDynamicObject* dynobj)
{
  iDynamicObject* old_dynobj = FindAttachedDynObj (light);
  if (old_dynobj == dynobj) return;
  if (old_dynobj != 0) UnattachDynObj (light, old_dynobj);
  csRef<dynobjFinder> cef =
    csPtr<dynobjFinder> (new dynobjFinder (dynobj));
  csRef<iObject> cef_obj (scfQueryInterface<iObject> (cef));
  light->QueryObject ()->ObjAdd (cef_obj);
}

void dynobjFinder::UnattachDynObj (iLight* light, iDynamicObject* dynobj)
{
  csRef<dynobjFinder> cef (CS::GetChildObject<dynobjFinder> (light->QueryObject ()));
  if (cef)
  {
    if (cef->GetDynamicObject () != dynobj) { return; }
    csRef<iObject> cef_obj (scfQueryInterface<iObject> (cef));
    light->QueryObject ()->ObjRemove (cef_obj);
  }
}

void dynobjFinder::AttachDynObj (iMeshWrapper* mesh, iDynamicObject* dynobj)
{
  iDynamicObject* old_dynobj = FindAttachedDynObj (mesh);
  if (old_dynobj == dynobj) return;
  if (old_dynobj != 0) UnattachDynObj (mesh, old_dynobj);
  csRef<dynobjFinder> cef =
    csPtr<dynobjFinder> (new dynobjFinder (dynobj));
  csRef<iObject> cef_obj (scfQueryInterface<iObject> (cef));
  mesh->QueryObject ()->ObjAdd (cef_obj);
}

void dynobjFinder::UnattachDynObj (iMeshWrapper* mesh, iDynamicObject* dynobj)
{
  csRef<dynobjFinder> cef (CS::GetChildObject<dynobjFinder> (mesh->QueryObject ()));
  if (cef)
  {
    if (cef->GetDynamicObject () != dynobj) { return; }
    csRef<iObject> cef_obj (scfQueryInterface<iObject> (cef));
    mesh->QueryObject ()->ObjRemove (cef_obj);
  }
}

//---------------------------------------------------------------------------

static csString GetEntityNameInt (iCelPlLayer* pl, iCelEntity* entity)
{
  if (!entity) return "unknown";
  else if (entity->GetName ()) return entity->GetName ();
  else
  {
    csStringID tmpID = entity->GetTemplateNameID ();
    return pl->FetchString (tmpID);
  }
}

static void SaveTransform (iCelCompactDataBufferWriter* buf,
      const csReversibleTransform& trans)
{
  buf->AddVector3 (trans.GetOrigin ());
  buf->AddVector3 (trans.GetO2T ().Row1 ());
  buf->AddVector3 (trans.GetO2T ().Row2 ());
  buf->AddVector3 (trans.GetO2T ().Row3 ());
}

static void LoadTransform (iCelCompactDataBufferReader* buf,
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

static void SaveStrings (iCelCompactDataBufferWriter* buf,
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

static void LoadStrings (iCelCompactDataBufferReader* buf,
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

//---------------------------------------------------------------------------

// This spawner implementation will be used by spawn property classes
// to control the creation of dynamic objects when new entities are spawned.
class celDynworldSpawner : public scfImplementation1<celDynworldSpawner,
  iCelSpawner>
{
private:
  csWeakRef<celPcDynamicWorld> dynworld;

public:
  celDynworldSpawner (celPcDynamicWorld* dynworld) : scfImplementationType (this),
  	dynworld (dynworld)
  {
  }
  virtual ~celDynworldSpawner () { }

  virtual iCelEntity* CreateEntity (iCelEntityTemplate* tpl,
      const char* entityName, iCelParameterBlock* params,
      const csVector3& pos, float yrot, const char* sector,
      const char* node)
  {
    if (!dynworld) return 0;
    return dynworld->CreateSpawnedEntity (tpl, entityName, params,
        pos, yrot, sector, node);
  }
};

//---------------------------------------------------------------------------

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

//---------------------------------------------------------------------------

struct DynWorldKDData
{
  // Previously visible objects.
  csSet<csPtrKey<DynamicObject> >& prevObjects;
  // New visible objects.
  csSet<csPtrKey<DynamicObject> >& objects;
  // A set of objects that are safe to remove. If we find one of them is visible
  // then we can remove that entity from this set.
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
    // Use timestamps to detect if we already checked this object before
    // (note that objects can be in multiple nodes at once if the split
    // planes happen to intersect objects).
    if (objects[i]->timestamp != cur_timestamp)
    {
      objects[i]->timestamp = cur_timestamp;

      // Test the bounding box of the object.
      DynamicObject* dynobj = (DynamicObject*)objects[i]->GetObject ();
      float maxradiusRelative = dynobj->GetFactory ()->GetMaximumRadiusRelative ();
      float sqrad = data->sqradius * maxradiusRelative * maxradiusRelative;
      if (dynobj->IsStatic ())
        sqrad *= 1.1f;

      const csSphere& obj_bsphere = dynobj->GetBSphere ();
      float sqdist = csSquaredDist::PointPoint (data->center, obj_bsphere.GetCenter ());
      if ((sqdist-csSquare (obj_bsphere.GetRadius () + data->radius)) < 0)
      {
	// Object is visible so we can remove it from 'prevObjects' and add it to 'objects'.
        data->prevObjects.Delete (dynobj);
        data->objects.Add (dynobj);
	iCelEntity* entity = dynobj->GetEntity ();
	// This object is visible so it is no longer safe to remove it.
	if (entity)
	  data->safeToRemove.Delete (entity);
      }
    }
  }

  return true;
}

//---------------------------------------------------------------------------

DynamicCell::DynamicCell (const char* name, celPcDynamicWorld* world) :
  scfImplementationType (this), name (name), world (world)
{
  pl = world->pl;
  tree = new CS::Geometry::KDTree ();
  tree->SetMinimumSplitAmount (50);
  ObjDes* objDes = new ObjDes ();
  tree->SetObjectDescriptor (objDes);
  objDes->DecRef ();
  createdDynSys = false;
  lastID = 0;
  idBlockIdx = ~0;
}

DynamicCell::~DynamicCell ()
{
  DeleteObjects ();
  delete tree;
  if (createdDynSys && dynSys)
  {
    csRef<iDynamics> dyn = csQueryRegistry<iDynamics> (world->GetObjectRegistry ());
    if (dyn) dyn->RemoveSystem (dynSys);
  }
}

CS::Physics::Bullet::iDynamicSystem* DynamicCell::GetBulletDynamicSystem ()
{
  if (!bullet_dynSys)
    bullet_dynSys = scfQueryInterface<CS::Physics::Bullet::iDynamicSystem> (dynSys);
  return bullet_dynSys;
}


// We use an ID allocation scheme were every cell allocates blocks
// of ID's (with size IDBLOCK_SIZE) and then generates ID from that block
// until it is full at which point another block is requested.
// These blocks are remembered when a cell is persisted so that objects
// created in the cell get the same ID's next time the cell is loaded
// again.
uint DynamicCell::AllocID ()
{
  if (allocatedIDBlocks.GetSize () == 0 || (lastID % IDBLOCK_SIZE == 0))
  {
    // After restoring a cell it is possible that the allocatedIDBlocks is preset
    // to a number of ID blocks. In that case we need to allocate from these pre-allocated
    // blocks. We test for this case here.
    idBlockIdx++;
    if (idBlockIdx >= allocatedIDBlocks.GetSize ())
    {
      // We need a new block.
      uint block = world->AllocIDBlock ();
      allocatedIDBlocks.Push (block);
      lastID = block;
    }
    else
    {
      // Use the next available pre-allocated block.
      lastID = allocatedIDBlocks[idBlockIdx];
    }
  }
  lastID++;
  return lastID-1;
}

// Mark that this ID has been allocated. This is typically used
// when persisted objects are being restored. Note that this function
// assumes that the ID is actually part of a block that has been allocated
// by this cell. Don't use this otherwise!
void DynamicCell::AllocID (uint id)
{
  if (id >= lastID)
  {
    lastID = id+1;
    while (lastID >= allocatedIDBlocks[idBlockIdx]+IDBLOCK_SIZE)
      idBlockIdx++;
  }
}

bool DynamicCell::IsAllocatedHere (uint id)
{
  for (size_t i = 0 ; i < allocatedIDBlocks.GetSize () ; i++)
  {
    if (id >= allocatedIDBlocks[i] && id < allocatedIDBlocks[i]+IDBLOCK_SIZE)
      return true;
  }
  return false;

}

void DynamicCell::Setup (iSector* sector, iDynamicSystem* ds)
{
  DynamicCell::sector = sector;

  if (createdDynSys && dynSys)
  {
    csRef<iDynamics> dyn = csQueryRegistry<iDynamics> (world->GetObjectRegistry ());
    if (dyn) dyn->RemoveSystem (dynSys);
  }

  dynSys = ds;
  createdDynSys = false;

  if (!dynSys)
  {
    csRef<iDynamics> dyn = csQueryRegistry<iDynamics> (world->GetObjectRegistry ());
    dynSys = dyn->CreateSystem ();
    if (!dynSys)
    {
      world->Error ("Could not create dynamics system!\n");
      return;
    }
    createdDynSys = true;
    //dynSys->SetLinearDampener(.3f);
    dynSys->SetRollingDampener(.995f);
    dynSys->SetGravity (csVector3 (0.0f, -9.81f, 0.0f));

    csRef<CS::Physics::Bullet::iDynamicSystem> bullet_dynSys = GetBulletDynamicSystem ();
    if (bullet_dynSys)
    {
      bullet_dynSys->SetInternalScale (1.0f);
      bullet_dynSys->SetStepParameters (0.005f, 2, 10);
    }
  }
}


void DynamicCell::DeleteObjectInt (DynamicObject* dyn)
{
  dyn->RemoveCsObject (world);
  if (dyn->GetEntity ())
    pl->RemoveEntity (dyn->GetEntity ());
  // @@@ Efficient?
  world->hasMovedFromBaseline.Delete (dyn);
  world->visibleObjects.Delete (dyn);
  world->fadingOut.Delete (dyn);
  world->fadingIn.Delete (dyn);
}

void DynamicCell::DeleteObject (iDynamicObject* dynobj)
{
  DynamicObject* dyn = static_cast<DynamicObject*> (dynobj);
  if (dyn->GetEntity ())
    pl->RemoveEntity (dyn->GetEntity ());
  world->hasMovedFromBaseline.Delete (dyn);
  world->visibleObjects.Delete (dyn);
  world->fadingOut.Delete (dyn);
  world->fadingIn.Delete (dyn);
  // @@@ This is a pretty slow operation.
  size_t idx = objects.Find (dyn);
  if (idx != csArrayItemNotFound)
  {
    if (dyn->GetChild ())
      tree->RemoveObject (dyn->GetChild ());
    dyn->RemoveCsObject (world);
    objects.DeleteIndex (idx);
  }
}

void DynamicCell::DeleteObjects ()
{
  for (size_t i = 0 ; i < objects.GetSize () ; i++)
  {
    DynamicObject* obj = static_cast<DynamicObject*> (objects.Get (i));
    obj->RemoveJoints ();
    obj->RemovePivotJoints ();
  }
  while (objects.GetSize () > 0)
  {
    csRef<DynamicObject> dynobj = objects.Pop ();
    DeleteObjectInt (dynobj);
  }
  tree->Clear ();
}

iDynamicObject* DynamicCell::AddObject (const char* factory,
    const csReversibleTransform& trans)
{
  csRef<DynamicObject> obj;
  iDynamicFactory* ifact = world->FindFactory (factory);
  if (!ifact)
  {
    world->Error ("Cannot find factory '%s' for AddObject!\n", factory);
    return 0;
  }
  DynamicFactory* fact = static_cast<DynamicFactory*> (ifact);

  obj.AttachNew (new DynamicObject (this, fact, trans));
  objects.Push (obj);

  obj->SetID (AllocID ());

  CS::Geometry::KDTreeChild* child = tree->AddObject (obj->GetBSphere (), obj);
  obj->SetChild (child);
  return obj;
}

void DynamicCell::Save (iDocumentNode* node)
{
  csRef<iSyntaxService> syn = csQueryRegistryOrLoad<iSyntaxService> (
      world->GetObjectRegistry (), "crystalspace.syntax.loader.service.text");

  for (size_t i = 0 ; i < objects.GetSize () ; i++)
  {
    csRef<iDocumentNode> el = node->CreateNodeBefore (CS_NODE_ELEMENT);
    el->SetValue ("obj");
    objects[i]->Save (el, syn);
  }
}

csRef<iString> DynamicCell::Load (iDocumentNode* node)
{
  csRef<iSyntaxService> syn = csQueryRegistryOrLoad<iSyntaxService> (
      world->GetObjectRegistry (), "crystalspace.syntax.loader.service.text");

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    csString value = child->GetValue ();
    if (value == "obj")
    {
      csRef<DynamicObject> dynobj;
      dynobj.AttachNew (new DynamicObject (this));
      if (!dynobj->Load (child, syn, world))
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

void DynamicCell::MarkBaseline ()
{
  for (size_t i = 0 ; i < objects.GetSize () ; i++)
    objects[i]->MarkBaseline ();
}

void DynamicCell::SaveIDAllocations (iCelCompactDataBufferWriter* buf)
{
  // Save the ID information and allocations for this cell.
  buf->AddUInt32 (allocatedIDBlocks.GetSize ());
  for (size_t i = 0 ; i < allocatedIDBlocks.GetSize () ; i++)
    buf->AddUInt32 (allocatedIDBlocks[i]);
}

void DynamicCell::SaveModifications (iCelCompactDataBufferWriter* buf,
      iStringSet* strings, csSet<csPtrKey<DynamicObject> >& alreadySaved)
{
  iELCM* elcm = world->elcm;
  csSet<csPtrKey<iCelEntity> > newEntites = elcm->GetNewEntities ();

  printf ("##### Saving cell %p/%s #####\n", this, GetName ());

  // First we save all modified dynamic objects that belong to this cell.
  csRef<iCelEntityIterator> it = elcm->GetModifiedEntities ();
  while (it->HasNext ())
  {
    iCelEntity* entity = it->Next ();

    iDynamicObject* dynobj = world->FindObject (entity);
    if (dynobj && (dynobj->GetCell () == this))
    {
      buf->AddUInt8 (MARKER_NEW);
      buf->AddUInt32 (entity->GetID ());
      alreadySaved.Add (static_cast<DynamicObject*> (dynobj));

      if (newEntites.Contains (entity))
      {
        // This dynamic object represents a new entity (one that wasn't
        // present at the baseline).
        printf ("Saving new entity '%s'!\n", GetEntityNameInt (pl, entity).GetData ());
        newEntites.Delete (entity);
        iDynamicFactory* dynfact = dynobj->GetFactory ();
        buf->AddID (strings->Request (dynfact->GetName ()));
	buf->AddID (entity->GetTemplateNameID ());
        if (entity->GetName ())
          buf->AddID (strings->Request (entity->GetName ()));
        else
          buf->AddID (csInvalidStringID);
      }
      else
      {
        // This dynamic object represents a previously existing entity
        // (belongs to a dynamic object that existed at the baseline).
        printf ("Saving entity '%s'!\n", GetEntityNameInt (pl, entity).GetData ());
        buf->AddID (csInvalidStringID);	// No template.
      }
      // @@@ For now we just save the position if the entity is modified.
      // Perhaps this is even a good idea. Have to think about this more.
      SaveTransform (buf, dynobj->GetTransform ());
      entity->SaveModifications (buf, strings);
    }
  }

  // All remaining entities in 'newEntites' (that belong to this cell)
  // also have to be saved. These are entities that haven't been modified
  // yet but they are new.
  csSet<csPtrKey<iCelEntity> >::GlobalIterator newIt = newEntites.GetIterator ();
  while (newIt.HasNext ())
  {
    iCelEntity* entity = newIt.Next ();
    DynamicObject* dynobj = static_cast<DynamicObject*> (world->FindObject (entity));
    if (dynobj && dynobj->GetCell () == this)
    {
      alreadySaved.Add (dynobj);
      buf->AddUInt8 (MARKER_NEW);
      buf->AddUInt32 (entity->GetID ());
      iDynamicFactory* dynfact = dynobj->GetFactory ();
      buf->AddID (strings->Request (dynfact->GetName ()));
      buf->AddID (entity->GetTemplateNameID ());
      if (entity->GetName ())
        buf->AddID (strings->Request (entity->GetName ()));
      else
        buf->AddID (csInvalidStringID);
      // @@@ For now we just save the position if the entity is modified.
      // Perhaps this is even a good idea. Have to think about this more.
      SaveTransform (buf, dynobj->GetTransform ());
      entity->SaveModifications (buf, strings);
      printf ("Saving new pristine entity '%s'!\n", GetEntityNameInt (pl, entity).GetData ());
    }
  }
  buf->AddUInt8 (MARKER_END);

  // Now it is possible that we still have dynamic objects for which
  // the entity hasn't changed but the dynobj itself has changed (i.e. moved).
  // We save those here.
  csSet<csPtrKey<DynamicObject> >::GlobalIterator dynIt = world->hasMovedFromBaseline.GetIterator ();
  while (dynIt.HasNext ())
  {
    DynamicObject* dynobj = dynIt.Next ();
    if (!alreadySaved.Contains (dynobj) && dynobj->GetCell () == this)
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

void DynamicCell::RestoreIDAllocations (iCelCompactDataBufferReader* buf)
{
  // First restore the ID information and allocation.
  size_t s = (size_t)buf->GetUInt32 ();
  allocatedIDBlocks.DeleteAll ();
  for (size_t i = 0 ; i < s ; i++)
    allocatedIDBlocks.Push ((uint)buf->GetUInt32 ());
  lastID = 0;
  idBlockIdx = ~0;
}

void DynamicCell::RestoreModifications (iCelCompactDataBufferReader* buf,
    const csHash<csString,csStringID>& strings)
{
  iELCM* elcm = world->elcm;
  printf ("##### Loading cell %s #####\n", GetName ());

  // Read all dynobj for this cell.
  uint8 marker = buf->GetUInt8 ();
  while (marker == MARKER_NEW)
  {
    uint id = buf->GetUInt32 ();

    csStringID dynNameID = buf->GetID ();
    csStringID entNameID = csInvalidStringID;
    csStringID tmpNameID = csInvalidStringID;
    if (dynNameID != csInvalidStringID)
    {
      tmpNameID = buf->GetID ();
      entNameID = buf->GetID ();
    }

    DynamicObject* dynobj = 0;
    iCelEntity* entity;
    if (dynNameID != csInvalidStringID)
    {
      // Entity has to be created.
      const char* dynName = strings.Get (dynNameID, (const char*)0);
      const char* tmpName = strings.Get (tmpNameID, (const char*)0);
      const char* entName = strings.Get (entNameID, (const char*)0);
      printf ("Loading new entity/dynobj '%s' from '%s' (template '%s')\n", entName, dynName, tmpName);
      csReversibleTransform trans;
      LoadTransform (buf, trans);
      dynobj = static_cast<DynamicObject*> (AddObject (dynName, trans));
      dynobj->SetID (id);
      dynobj->SetEntity (entName, tmpName, 0);	// Parameters?@@@@@@
      entity = dynobj->ForceEntity ();
      elcm->RegisterNewEntity (entity);
    }
    else
    {
      dynobj = static_cast<DynamicObject*> (world->FindObject (id));
      // @@@ Can we avoid this? What if entity is baseline but dynobj is not?
      entity = dynobj->ForceEntity ();
      printf ("Loading existing entity '%s'\n", GetEntityNameInt (pl, entity).GetData ());
      csReversibleTransform trans;
      LoadTransform (buf, trans);
      dynobj->SetTransform (trans);
#if 0
      // @@@ What about this case?
      else
      {
        printf ("Loading existing entity but with deleted dynobj '%s'\n", GetEntityNameInt (pl, entity).GetData ());
        ForceInvisible (dynobj);
        dynobj->UnlinkEntity ();
        DeleteObject (dynobj);
      }
#endif
    }

    dynobj->ClearBaseline ();
    entity->MarkBaseline ();
    entity->RestoreModifications (buf, strings);

    marker = buf->GetUInt8 ();
  }
  if (marker != MARKER_END)
  {
    world->Error ("Savefile corrupt! Bad marker (1)!\n");
    return;
  }

  // Now load the remaining moved dynobjects.
  marker = buf->GetUInt8 ();
  while (marker == MARKER_NEW)
  {
    uint id = buf->GetUInt32 ();
    DynamicObject* dynobj = static_cast<DynamicObject*> (world->FindObject (id));
    printf ("Loading moved dynobj\n"); fflush (stdout);
    csReversibleTransform trans;
    LoadTransform (buf, trans);
    dynobj->SetTransform (trans);
    // Note! We restore the hasMovedFromBaseline set (almost) as it
    // was when we saved it. The reason for the difference is that
    // we didn't save (in this section) the dynamic objects which also
    // had modified entities. These were saved above.
    world->hasMovedFromBaseline.Add (dynobj);

    marker = buf->GetUInt8 ();
  }
  if (marker != MARKER_END)
  {
    world->Error ("Safefile corrupt! Bad marker (2)!\n");
    return;
  }
}

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

void MeshCache::RemoveFactory (iEngine* engine, const char* name)
{
  MeshCacheFactory* mcf = meshFactories.Get (name, 0);
  if (mcf)
  {
    while (mcf->meshes.GetSize () > 0)
    {
      engine->RemoveObject (mcf->meshes.Pop ());
    }
  }
}

iMeshWrapper* MeshCache::AddMesh (iEngine* engine,
    iMeshFactoryWrapper* factory, iSector* sector,
    const csReversibleTransform& trans)
{
  const char* factname = factory->QueryObject ()->GetName ();
  csRef<iMeshWrapper> mesh;
  MeshCacheFactory* mf = meshFactories.Get (factname, 0);
  if (mf && mf->meshes.GetSize () > 0)
    mesh = mf->meshes.Pop ();
  else
    mesh = engine->CreateMeshWrapper (factory, 0, sector, csVector3 (0), false);

  iMovable* movable = mesh->GetMovable ();
  movable->SetSector (sector);
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
    bool usefact,
    float maxradiusRelative, float imposterradius, bool isLogic) :
  scfImplementationType (this), name (name), world (world),
  maxradiusRelative (maxradiusRelative), isLogic (isLogic)
{
  bbox.StartBoundingBox ();
  physBbox.StartBoundingBox ();

  factory = 0;
  lightFactory = 0;
  objCounter = 0;

  hasCollider = false;

  factory = world->engine->FindMeshFactory (name);
  if (factory == 0)
  {
    world->Error ("Could not find factory '%s'!\n", name);
    return;
  }

  if (!usefact)
  {
    lightFactory = world->engine->FindLightFactory (name);
    if (lightFactory == 0)
    {
      world->Error ("Could not find light factory '%s'!\n", name);
      return;
    }
  }

  SetImposterRadius (imposterradius);

  if (factory)
  {
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
  //else if (lightFactory)
  //{
  //}
}

void DynamicFactory::SetImposterRadius (float r)
{
  imposterradius = 0;	// @@@ TODO: reenable when imposters appear to be working properly.
  if (imposterradius > 0)
  {
    imposterFactory = scfQueryInterface<iImposterFactory> (factory);
    imposterFactory->SetMinDistance (imposterradius * world->radius);
    //imposterFactory->SetRotationTolerance (45.0);
    //imposterFactory->SetCameraRotationTolerance(45.0);
    imposterFactory->SetShader ("base", "lighting_imposter");
    imposterFactory->SetRenderReal (true);
  }
  else
  {
    imposterFactory = 0;
  }
}

void DynamicFactory::SetAttribute (csStringID nameID, const char* value)
{
  attributes.PutUnique (nameID, value);
}

void DynamicFactory::SetAttribute (const char* name, const char* value)
{
  SetAttribute (world->pl->FetchStringID (name), value);
}

void DynamicFactory::ClearAttribute (csStringID nameID)
{
  attributes.DeleteAll (nameID);
}

void DynamicFactory::ClearAttribute (const char* name)
{
  ClearAttribute (world->pl->FetchStringID (name));
}

const char* DynamicFactory::GetAttribute (csStringID nameID) const
{
  return attributes.Get (nameID, (const char*)0);
}

const char* DynamicFactory::GetAttribute (const char* name) const
{
  return GetAttribute (world->pl->FetchStringID (name));
}

class AttributeIterator : public scfImplementation1<AttributeIterator,
	iAttributeIterator>
{
private:
  csHash<csString, csStringID>::ConstGlobalIterator it;

public:
  AttributeIterator (const csHash<csString, csStringID>::ConstGlobalIterator it) :
	  scfImplementationType (this), it (it)
  {
  }
  virtual ~AttributeIterator () { }
  virtual bool HasNext () const { return it.HasNext (); }
  virtual csStringID Next ()
  {
    csStringID key;
    it.Next (key);
    return key;
  }
};

csPtr<iAttributeIterator> DynamicFactory::GetAttributes () const
{
  return new AttributeIterator (attributes.GetIterator ());
}

void DynamicFactory::SetColliderEnabled (bool e)
{
  hasCollider = e;
}

void DynamicFactory::AddRigidBox (const csVector3& offset, const csVector3& size,
    float mass, size_t idx)
{
  if (idx == csArrayItemNotFound)
  {
    csBox3 b (-size / 2 + offset, size / 2 + offset);
    for (size_t i = 0 ; i < 8 ; i++)
      physBbox.AddBoundingVertex (b.GetCorner (i));
    colliders.Push (new DOColliderBox (size, offset, mass));
  }
  else
  {
    colliders.Put (idx, new DOColliderBox (size, offset, mass));
    UpdatePhysBBox ();
  }
}

void DynamicFactory::AddRigidSphere (float radius, const csVector3& offset,
    float mass, size_t idx)
{
  if (idx == csArrayItemNotFound)
  {
    csBox3 b (offset - csVector3 (radius), offset + csVector3 (radius));
    for (size_t i = 0 ; i < 8 ; i++)
      physBbox.AddBoundingVertex (b.GetCorner (i));
    colliders.Push (new DOColliderSphere (offset, radius, mass));
  }
  else
  {
    colliders.Put (idx, new DOColliderSphere (offset, radius, mass));
    UpdatePhysBBox ();
  }
}

void DynamicFactory::AddRigidCylinder (float radius, float length,
      const csVector3& offset, float mass, size_t idx)
{
  if (idx == csArrayItemNotFound)
  {
    csBox3 b (offset - csVector3 (radius, length/2, radius),
        offset + csVector3 (radius, length/2, radius));
    for (size_t i = 0 ; i < 8 ; i++)
      physBbox.AddBoundingVertex (b.GetCorner (i));
    colliders.Push (new DOColliderCylinder (offset, length, radius, mass));
  }
  else
  {
    colliders.Put (idx, new DOColliderCylinder (offset, length, radius, mass));
    UpdatePhysBBox ();
  }
}

void DynamicFactory::AddRigidCapsule (float radius, float length,
      const csVector3& offset, float mass, size_t idx)
{
  if (idx == csArrayItemNotFound)
  {
    csBox3 b (offset - csVector3 (radius, length/2, radius),
        offset + csVector3 (radius, length/2, radius));
    for (size_t i = 0 ; i < 8 ; i++)
      physBbox.AddBoundingVertex (b.GetCorner (i));
    colliders.Push (new DOColliderCapsule (offset, length, radius, mass));
  }
  else
  {
    colliders.Put (idx, new DOColliderCapsule (offset, length, radius, mass));
    UpdatePhysBBox ();
  }
}

void DynamicFactory::AddRigidMesh (const csVector3& offset, float mass,
    size_t idx)
{
  if (idx == csArrayItemNotFound)
  {
    for (size_t i = 0 ; i < 8 ; i++)
      physBbox.AddBoundingVertex (bbox.GetCorner (i));
    colliders.Push (new DOColliderMesh (offset, mass));
  }
  else
  {
    colliders.Put (idx, new DOColliderMesh (offset, mass));
    UpdatePhysBBox ();
  }
}

void DynamicFactory::AddRigidConvexMesh (const csVector3& offset, float mass,
    size_t idx)
{
  if (idx == csArrayItemNotFound)
  {
    for (size_t i = 0 ; i < 8 ; i++)
      physBbox.AddBoundingVertex (bbox.GetCorner (i));
    colliders.Push (new DOColliderConvexMesh (offset, mass));
  }
  else
  {
    colliders.Put (idx, new DOColliderConvexMesh (offset, mass));
    UpdatePhysBBox ();
  }
}

void DynamicFactory::UpdatePhysBBox ()
{
  physBbox.StartBoundingBox ();
  for (size_t i = 0 ; i < colliders.GetSize () ; i++)
  {
    celBodyInfo info = GetBody (i);
    switch (info.type)
    {
      case BOX_COLLIDER_GEOMETRY:
	{
	  csBox3 b (-info.size / 2 + info.offset, info.size / 2 + info.offset);
	  for (size_t i = 0 ; i < 8 ; i++)
	    physBbox.AddBoundingVertex (b.GetCorner (i));
	  break;
	}
      case SPHERE_COLLIDER_GEOMETRY:
	{
	  csBox3 b (info.offset - csVector3 (info.radius),
	      info.offset + csVector3 (info.radius));
	  for (size_t i = 0 ; i < 8 ; i++)
	    physBbox.AddBoundingVertex (b.GetCorner (i));
	  break;
	}
      case CAPSULE_COLLIDER_GEOMETRY:
      case CYLINDER_COLLIDER_GEOMETRY:
	{
	  csBox3 b (info.offset - csVector3 (info.radius, info.length/2, info.radius),
	      info.offset + csVector3 (info.radius, info.length/2, info.radius));
	  for (size_t i = 0 ; i < 8 ; i++)
	    physBbox.AddBoundingVertex (b.GetCorner (i));
	  break;
	}
      case TRIMESH_COLLIDER_GEOMETRY:
      case CONVEXMESH_COLLIDER_GEOMETRY:
	for (size_t i = 0 ; i < 8 ; i++)
	  physBbox.AddBoundingVertex (bbox.GetCorner (i));
	break;
      default:
	break;
    }
  }
}

void DynamicFactory::DeleteBodies ()
{
  colliders.DeleteAll ();
  physBbox.StartBoundingBox ();
}

void DynamicFactory::DeleteBody (size_t idx)
{
  colliders.DeleteIndex (idx);
  UpdatePhysBBox ();
}

DynFactJointDefinition& DynamicFactory::CreateJoint ()
{
  size_t idx = joints.Push (DynFactJointDefinition ());
  return joints[idx];
}

void DynamicFactory::RemoveJoint (size_t idx)
{
  joints.DeleteIndex (idx);
}

void DynamicFactory::SetJoint (size_t idx, const DynFactJointDefinition& def)
{
  joints[idx] = def;
}

void DynamicFactory::RemoveJoints ()
{
  joints.DeleteAll ();
}

bool DynamicFactory::CreatePivotJoint (const csVector3& objpos)
{
  pivotJoints.Push (objpos);
  return true;
}

csVector3 DynamicFactory::GetPivotJointPosition (size_t idx)
{
  return pivotJoints[idx];
}

void DynamicFactory::SetPivotJointPosition (size_t idx, const csVector3& objpos)
{
  pivotJoints[idx] = objpos;
}

void DynamicFactory::RemovePivotJoint (size_t idx)
{
  pivotJoints.DeleteIndex (idx);
}

void DynamicFactory::RemovePivotJoints ()
{
  pivotJoints.DeleteAll ();
}

//---------------------------------------------------------------------------------------

void DynamicObject::Init (DynamicCell* cell)
{
  DynamicObject::cell = cell;
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
  id = 0;
}

void DynamicObject::SetupPivotJoints ()
{
  for (size_t i = 0 ; i < factory->GetPivotJointCount () ; i++)
  {
    csVector3 worldpos = GetTransform ().This2Other (factory->GetPivotJointPosition (i));
    CreatePivotJoint (worldpos);
  }
}

DynamicObject::DynamicObject (DynamicCell* cell) : scfImplementationType (this)
{
  Init (cell);
}

DynamicObject::DynamicObject (DynamicCell* cell, DynamicFactory* factory,
    const csReversibleTransform& trans) : scfImplementationType (this)
{
  Init (cell);
  DynamicObject::factory = factory;
  DynamicObject::trans = trans;
  child = 0;
  if (factory) factory->RegisterNewObject ();
}

DynamicObject::~DynamicObject ()
{
  if (factory) factory->RegisterDeletedObject ();
  RemovePivotJoints ();
  RemoveJoints ();
  if (entityName)
  {
    celPcDynamicWorld* world = factory->GetWorld ();
    world->GetNameToDynObj ().Delete (entityName, this);
  }
}

void DynamicObject::SetID (uint id)
{
  celPcDynamicWorld* world = factory->GetWorld ();

  // Unregister the previous id of this object (if any).
  if (id)
    world->GetIdToDynObj ().Delete (id, this);

  DynamicObject::id = id;

  // We first try to find out in which cell this ID was allocated
  // before. Then we can mark this ID as being used in the correct cell.
  DynamicCell* idCell = world->FindCellForID (id);
  CS_ASSERT (idCell != 0);
  idCell->AllocID (id);	// Force this Id to be allocated.

  // Now register the new id.
  world->GetIdToDynObj ().Put (id, this);
}

iDynamicCell* DynamicObject::GetCell () const
{
  return static_cast<iDynamicCell*> (cell);
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
	  GetEntityNameInt (factory->GetWorld ()->pl, entity).GetData ()); fflush (stdout);
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
    if (fade < .001)
      mesh->GetMeshObject ()->SetMixMode (CS_FX_COPY);
    else
      mesh->GetMeshObject ()->SetMixMode (CS_FX_SETALPHA(fade));
  }
}

void DynamicObject::MeshBodyToEntity (iMeshWrapper* mesh, iRigidBody* body)
{
  if (!entity) return;
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  if (pcmesh)
  {
    pcmesh->SetFactoryName (factory->GetName ());
    pcmesh->SetMesh (mesh);
  }
  csRef<iPcMechanicsObject> pcmechobj = celQueryPropertyClassEntity<iPcMechanicsObject> (entity);
  if (pcmechobj)
    pcmechobj->SetBody (body);
}

void DynamicObject::RemoveLight (celPcDynamicWorld* world)
{
  trans = light->GetMovable ()->GetTransform ();
  world->engine->RemoveObject (light);
  dynobjFinder::UnattachDynObj (light, this);
  light = 0;
}

void DynamicObject::RemoveMesh (celPcDynamicWorld* world)
{
  InstallHilight (false);
  CS_ASSERT (hilight_installed == false);
  trans = mesh->GetMovable ()->GetTransform ();
  iImposterFactory* imposterFactory = factory->GetImposterFactory ();
  if (imposterFactory)
    imposterFactory->RemoveImposter (mesh);
  mesh->GetMovable ()->RemoveListener (this);
  world->meshCache.RemoveMesh (mesh);
  dynobjFinder::UnattachDynObj (mesh, this);
  mesh = 0;
  MeshBodyToEntity (0, 0);
}

void DynamicObject::RemoveCsObject (celPcDynamicWorld* world)
{
  if (mesh)
    RemoveMesh (world);
  if (light)
    RemoveLight (world);

  if (body) cell->dynSys->RemoveBody (body);
  body = 0;
  factory->GetWorld ()->GetIdToDynObj ().Delete (id, this);
}

void DynamicObject::PrepareLight (celPcDynamicWorld* world)
{
  light = world->engine->CreateLight (factory->GetName (),
      trans.GetOrigin (), factory->GetLightFactory ());
  iLightList* ll = cell->sector->GetLights ();
  ll->Add (light);
  light->GetMovable ()->SetTransform (trans);
  light->GetMovable ()->UpdateMove ();
}

void DynamicObject::PrepareMesh (celPcDynamicWorld* world)
{
  mesh = world->meshCache.AddMesh (world->engine, factory->GetMeshFactory (),
      cell->sector, trans);
  bool invis;
  if (!world->IsGameMode ()) invis = false;
  else invis = factory->IsLogicFactory () || factory->IsLogicFactory ();
  mesh->GetFlags ().Set (CS_ENTITY_INVISIBLE, invis ? CS_ENTITY_INVISIBLE : 0);
  if (!invis && (factory->IsLogicFactory () || factory->IsLightFactory ()))
  {
    mesh->SetRenderPriority (world->engine->GetRenderPriority ("alpha"));
    mesh->SetZBufMode (CS_ZBUF_TEST);
  }
  else
  {
    mesh->SetRenderPriority (world->engine->GetRenderPriority ("object"));
    mesh->SetZBufMode (CS_ZBUF_USE);
  }

  mesh->GetMovable ()->AddListener (this);
  lastUpdateNr = mesh->GetMovable ()->GetUpdateNumber ();
  iGeometryGenerator* ggen = factory->GetGeometryGenerator ();
  if (ggen)
    ggen->GenerateGeometry (mesh);
  InstallHilight (is_hilight);
  CS_ASSERT (hilight_installed == is_hilight);
  SetFade (fade);
}

void DynamicObject::PrepareCsObject (celPcDynamicWorld* world)
{
  if (mesh || light) return;
  if (factory->GetMeshFactory ())
    PrepareMesh (world);
  if (factory->GetLightFactory ())
    PrepareLight (world);

  factory->GetWorld ()->GetIdToDynObj ().Put (id, this);

  ForceEntity ();
  if (!entity)
  {
    // There is no entity so we need another way to find the dynobj from
    // a mesh or light.
    if (mesh)
      dynobjFinder::AttachDynObj (mesh, this);
    if (light)
      dynobjFinder::AttachDynObj (light, this);
  }

  CreateCollider ();
  CreateBody ();
}

void DynamicObject::RemoveJoints ()
{
  for (size_t i = 0 ; i < joints.GetSize () ; i++)
  {
    if (joints[i])
    {
      cell->dynSys->RemoveJoint (joints[i]);
      joints.Put (i, 0);
    }
  }
}

void DynamicObject::UpdateJoints ()
{
  connectedObjects.SetSize (factory->GetJointCount (), 0);
  joints.SetSize (factory->GetJointCount (), 0);
  if (!GetBody ()) { RemoveJoints (); return; }
  for (size_t i = 0 ; i < joints.GetSize () ; i++)
  {
    if (connectedObjects[i])
    {
      if (!connectedObjects[i]->GetBody ())
      {
	DynamicObject* dy = static_cast<DynamicObject*> (
			(iDynamicObject*) connectedObjects[i]);
        dy->PrepareCsObject (factory->GetWorld ());
        if (!connectedObjects[i]->GetBody ())
          continue;
      }
      csRef<iJoint> j = joints[i];
      if (!j)
      {
        j = cell->dynSys->CreateJoint ();
        joints.Put (i, j);
        DynFactJointDefinition& def = factory->GetJoint (i);
        j->SetTransform (def.GetTransform ());
        j->SetTransConstraints (def.IsXTransConstrained (), def.IsYTransConstrained (),
	    def.IsZTransConstrained ());
        j->SetMinimumDistance (def.GetMinimumDistance ());
        j->SetMaximumDistance (def.GetMaximumDistance ());
        j->SetRotConstraints (def.IsXRotConstrained (), def.IsYRotConstrained (),
	    def.IsZRotConstrained ());
        j->SetMinimumAngle (def.GetMinimumAngle ());
        j->SetMaximumAngle (def.GetMaximumAngle ());
        j->SetBounce (def.GetBounce ());
        j->SetMaxForce (def.GetMaxForce ());
        j->Attach (connectedObjects[i]->GetBody (), GetBody ());
      }
      j->RebuildJoint ();
    }
    else if (joints[i])
    {
      cell->dynSys->RemoveJoint (joints[i]);
      joints.Put (i, 0);
    }
  }
}

bool DynamicObject::Connect (size_t jointIdx, iDynamicObject* obj)
{
  if (jointIdx >= factory->GetJointCount ()) return false;
  connectedObjects.SetSize (factory->GetJointCount (), 0);
  joints.SetSize (factory->GetJointCount (), 0);
  if (joints[jointIdx])
  {
    cell->dynSys->RemoveJoint (joints[jointIdx]);
    joints.Put (jointIdx, 0);
  }
  connectedObjects.Put (jointIdx, obj);
  UpdateJoints ();
  return true;
}

void DynamicObject::RecreateJoints ()
{
  RemoveJoints ();
  UpdateJoints ();
}

iDynamicObject* DynamicObject::GetConnectedObject (size_t jointIdx)
{
  if (jointIdx >= factory->GetJointCount ()) return 0;
  return connectedObjects[jointIdx];
}

void DynamicObject::CreateCollider ()
{
  if (!mesh) return;
  if (factory->IsColliderEnabled ())
  {
    csColliderWrapper* wrap = csColliderWrapper::GetColliderWrapper (mesh->QueryObject ());
    if (wrap) return;
    celPcDynamicWorld* world = factory->GetWorld ();
    iCollideSystem* cdsys = world->GetCollideSystem ();
    if (!cdsys) return;
    csColliderHelper::InitializeCollisionWrapper (cdsys, mesh);
  }
}

void DynamicObject::CreateBody ()
{
  bsphereValid = false;
  RemoveBody ();

  if (!factory->GetWorld ()->IsPhysicsEnabled ())
    return;	// No physics, no body.

  if (mesh)
    trans = mesh->GetMovable ()->GetTransform ();
  if (light)
    trans = light->GetMovable ()->GetTransform ();
  const csPDelArray<DOCollider>& colliders = factory->GetColliders ();
  for (size_t i = 0 ; i < colliders.GetSize () ; i++)
  {
    body = colliders[i]->Create (cell->dynSys, mesh, light, trans, body);
  }
  if (is_static && body)
    body->MakeStatic ();

  SetupPivotJoints ();
  UpdateJoints ();

  MeshBodyToEntity (mesh, body);
}

void DynamicObject::RemoveCollider ()
{
  if (!mesh) return;
  csColliderWrapper* wrap = csColliderWrapper::GetColliderWrapper (mesh->QueryObject ());
  if (!wrap) return;
  mesh->QueryObject ()->ObjRemove (static_cast<iObject*> (wrap));
}

void DynamicObject::RemoveBody ()
{
  if (!body) return;
  body->DestroyColliders ();
  body = 0;
  MeshBodyToEntity (mesh, 0);
}

void DynamicObject::RefreshColliders ()
{
  bool remember_static = is_static;
  if (!remember_static) MakeStatic ();

  if (body)
  {
    if (factory->GetWorld ()->IsPhysicsEnabled ())
      CreateBody ();
    else
      RemoveBody ();
  }
  CreateCollider ();

  if (!remember_static) MakeDynamic ();
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
  // @@@ Error checking???
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

  if (params)
  {
    csRef<iDocumentNode> paramsNode = node->CreateNodeBefore (CS_NODE_ELEMENT);
    paramsNode->SetValue ("params");
    if (!celParameterTools::WriteParams (factory->GetWorld ()->GetObjectRegistry (),
	  paramsNode, params))
      ; // @@@ TODO
  }

  if (entity)
  {
    csStringID tmpID = entity->GetTemplateNameID ();
    if (tmpID != csInvalidStringID)
    {
      const char* tmpName = factory->GetWorld ()->pl->FetchString (tmpID);
      csString defaultTmpName = factory->GetDefaultEntityTemplate ();
      // The entity differs from the default factory template.
      if (defaultTmpName != tmpName)
        node->SetAttribute ("ent", tmpName);
    }
  }
  else if (entityTemplate)
  {
    csString defaultTmpName = factory->GetDefaultEntityTemplate ();
    // The entity differs from the default factory template.
    if (defaultTmpName != entityTemplate->GetName ())
      node->SetAttribute ("ent", entityTemplate->GetName ());
  }
}

bool DynamicObject::Load (iDocumentNode* node, iSyntaxService* syn,
    celPcDynamicWorld* world)
{
  csString factname = node->GetAttributeValue ("fact");
  factory = world->factory_hash.Get (factname, 0);
  // @@@ TODO Light factory
  if (!factory)
    return world->Error ("Can't find factory '%s'!\n", factname.GetData ());
  factory->RegisterNewObject ();
  if (!syn->ParseBoolAttribute (node, "static", is_static, false, false))
    return false;

  if (node->GetAttribute ("id"))
    id = node->GetAttributeValueAsInt ("id");
  else
    id = cell->AllocID ();

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

  csRef<iCelParameterBlock> params;
  csRef<iDocumentNode> paramsNode = node->GetNode ("params");
  if (paramsNode)
    params = celParameterTools::ParseParams (factory->GetWorld ()->GetObjectRegistry (),
	paramsNode, CEL_PARAM_END);

  csString tmpName = node->GetAttributeValue ("ent");
  if (tmpName)
    SetEntity (0, tmpName, params);
  else if (factory->GetDefaultEntityTemplate ())
    SetEntity (0, factory->GetDefaultEntityTemplate (), params);
  else
    SetEntity (0, factory->GetName (), params);

  return true;
}

void DynamicObject::MovableChanged (iMovable* movable)
{
  bsphereValid = false;
  cell->tree->MoveObject (child, GetBSphere ());
  factory->GetWorld ()->checkForMovement.Add (this);
}

void DynamicObject::MovableDestroyed (iMovable*)
{
}

bool DynamicObject::RecreatePivotJoints ()
{
  RemovePivotJoints ();
  SetupPivotJoints ();
  return true;
}

bool DynamicObject::CreatePivotJoint (const csVector3& worldpos)
{
  csRef<CS::Physics::Bullet::iDynamicSystem> bullet_dynSys =
    cell->GetBulletDynamicSystem ();
  if (!bullet_dynSys) return false;
  csRef<CS::Physics::Bullet::iPivotJoint> joint = bullet_dynSys->CreatePivotJoint ();
  pivotJoints.Push (joint);
  joint->Attach (GetBody (), worldpos);
  return true;
}

csVector3 DynamicObject::GetPivotJointPosition (size_t idx)
{
  return pivotJoints[idx]->GetPosition ();
}

void DynamicObject::SetPivotJointPosition (size_t idx, const csVector3& worldpos)
{
  pivotJoints[idx]->SetPosition (worldpos);
}

void DynamicObject::RemovePivotJoint (size_t idx)
{
  csRef<CS::Physics::Bullet::iDynamicSystem> bullet_dynSys =
    cell->GetBulletDynamicSystem ();
  if (!bullet_dynSys) return;
  bullet_dynSys->RemovePivotJoint (pivotJoints[idx]);
  pivotJoints.DeleteIndex (idx);
}

void DynamicObject::RemovePivotJoints ()
{
  csRef<CS::Physics::Bullet::iDynamicSystem> bullet_dynSys =
    cell->GetBulletDynamicSystem ();
  if (!bullet_dynSys) return;
  for (size_t i = 0 ; i < pivotJoints.GetSize () ; i++)
  {
    bullet_dynSys->RemovePivotJoint (pivotJoints[i]);
  }
  pivotJoints.DeleteAll ();
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

bool DynamicObject::SetEntity (const char* entityName, const char* entityTplName,
    iCelParameterBlock* params)
{
  DynamicObject::params = params;
  if (!entityTplName)
  {
    entityTplName = factory->GetDefaultEntityTemplate ();
    if (!entityTplName) return true;	// This is not an error. There is just no entity.
  }
  celPcDynamicWorld* world = factory->GetWorld ();
  entityTemplate = world->pl->FindEntityTemplate (entityTplName);
  if (!entityTemplate)
  {
    printf ("Can't find entity template '%s'!\n", entityTplName);
  }
  SetEntityName (entityName);
  return true;
}

void DynamicObject::SetEntityName (const char* name)
{
  celPcDynamicWorld* world = factory->GetWorld ();
  if (DynamicObject::entityName)
    world->GetNameToDynObj ().Delete (DynamicObject::entityName, this);
  DynamicObject::entityName = name;
  if (DynamicObject::entityName)
    world->GetNameToDynObj ().Put (DynamicObject::entityName, this);
}

void DynamicObject::LinkEntity (iCelEntity* entity)
{
  csStringID tmpID = entity->GetTemplateNameID ();
  const char* tmpName = factory->GetWorld ()->pl->FetchString (tmpID);
  entityTemplate = factory->GetWorld ()->pl->FindEntityTemplate (tmpName);
  if (!entityTemplate)
    return;
  DynamicObject::entity = entity;
  DynamicObject::entityName = entity->GetName ();
  SetID (entity->GetID ());
  MeshBodyToEntity (mesh, body);
}

void DynamicObject::UnlinkEntity ()
{
  MeshBodyToEntity (0, 0);
  entity = 0;
}

void DynamicObject::ClearEntity ()
{
  if (entity)
  {
    factory->GetWorld ()->pl->RemoveEntity (entity);
    UnlinkEntity ();
  }
}

iCelEntity* DynamicObject::ForceEntity ()
{
  celPcDynamicWorld* world = factory->GetWorld ();
  if (world->IsInhibitEntities ()) return 0;

  if (!entity)
  {
    // First we check if the entity already exists.
    entity = world->pl->GetEntity (id);
    if (entity)
    {
      if (!entityName.IsEmpty ())
        entity->SetName (entityName);
      if (entityTemplate)
      {
        world->pl->ApplyTemplate (entity, entityTemplate, params);
	entity->SetTemplateNameID (world->pl->FetchStringID (entityTemplate->GetName ()));
      }
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
  MeshBodyToEntity (mesh, body);
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
    if (entity->GetName ())
      statusLine->Format ("%s (%d:%s)%s%s%s%s%s", GetFactory ()->GetName (), id,
	  entity->GetName (), bl, mo, ac, mod, entbl);
    else
      statusLine->Format ("%s (%d)%s%s%s%s%s", GetFactory ()->GetName (), id,
	  bl, mo, ac, mod, entbl);
  }
  return statusLine;
}

//---------------------------------------------------------------------------------------

// Listen to the ELCM and see when it says an entity can be safely removed.
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

PropertyHolder celPcDynamicWorld::propinfo;

celPcDynamicWorld::celPcDynamicWorld (iObjectRegistry* object_reg)
  : scfImplementationType (this, object_reg)
{  
  radius = 80;
  engine = csQueryRegistry<iEngine> (object_reg);
  g3d = csQueryRegistry<iGraphics3D> (object_reg);
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  pl = csQueryRegistry<iCelPlLayer> (object_reg);

  scopeIdx = pl->AddScope ("cel.numreg.hash", 1000000000);
  lastIDBlock = 1000000000;
  restoringIDBlocks = false;

  csRef<celDynworldSpawner> spawner;
  spawner.AttachNew (new celDynworldSpawner (this));
  object_reg->Register (spawner, "cel.spawner");

  currentCell = 0;
  inhibitEntities = false;
  gameMode = true;
  doPhysics = true;

  propholder = &propinfo;
  propinfo.SetCount (1);
  AddProperty (propid_physics, "physics",
	CEL_DATA_BOOL, false, "Enable physics.", 0);
}

celPcDynamicWorld::~celPcDynamicWorld ()
{
  DeleteAll ();
  if (scopeIdx != csArrayItemNotFound && pl)
    pl->ResetScope (scopeIdx);
}

iCollideSystem* celPcDynamicWorld::GetCollideSystem ()
{
  if (!cdsys)
  {
    cdsys = csQueryRegistry<iCollideSystem> (object_reg);
    if (!cdsys)
      Error ("Can't find collide system!");
  }
  return cdsys;
}

bool celPcDynamicWorld::SetPropertyIndexed (int idx, bool b)
{
  if (idx == propid_physics)
  {
    EnablePhysics (b);
    return true;
  }
  return false;
}

bool celPcDynamicWorld::GetPropertyIndexed (int idx, bool& l)
{
  if (idx == propid_physics)
  {
    l = doPhysics;
    return true;
  }
  return false;
}


DynamicCell* celPcDynamicWorld::FindCellForID (uint id)
{
  csHash<csRef<DynamicCell>,csString>::GlobalIterator it = cells.GetIterator ();
  while (it.HasNext ())
  {
    csString name;
    csRef<DynamicCell> cell = it.Next (name);
    if (cell->IsAllocatedHere (id))
      return cell;
  }
  return 0;
}

void celPcDynamicWorld::DeleteAll ()
{
  lastIDBlock = 1000000000;
  checkForMovement.DeleteAll ();
  cells.DeleteAll ();
  currentCell = 0;
  idToDynObj.DeleteAll ();
  nameToDynObj.DeleteAll ();
  safeToRemove.DeleteAll ();
  hasMovedFromBaseline.DeleteAll ();
  visibleObjects.DeleteAll ();
  fadingOut.DeleteAll ();
  fadingIn.DeleteAll ();
  safeToRemove.DeleteAll ();
  meshCache.RemoveMeshes ();
}

void celPcDynamicWorld::SetELCM (iELCM* elcm)
{
  celPcDynamicWorld::elcm = elcm;
  DynWorldELCMListener* listener = new DynWorldELCMListener (this);
  elcm->AddELCMListener (listener);
  listener->DecRef ();
}

iDynamicCell* celPcDynamicWorld::AddCell (const char* name, iSector* sector, iDynamicSystem* dynSys)
{
  csRef<DynamicCell> cell;
  cell.AttachNew (new DynamicCell (name, this));
  cell->Setup (sector, dynSys);
  cells.Put (name, cell);
  return cell;
}

iDynamicCell* celPcDynamicWorld::FindCell (const char* name)
{
  return cells.Get (name, 0);
}

void celPcDynamicWorld::RemoveCell (iDynamicCell* cell)
{
  cells.DeleteAll (cell->GetName ());
}

csPtr<iDynamicCellIterator> celPcDynamicWorld::GetCells () const
{
  csHash<csRef<DynamicCell>,csString>::ConstGlobalIterator it = cells.GetIterator ();
  return new DynamicCellIterator (it);
}


void celPcDynamicWorld::CheckForMovement ()
{
  csSet<csPtrKey<DynamicObject> >::GlobalIterator it = checkForMovement.GetIterator ();
  while (it.HasNext ())
  {
    DynamicObject* dynobj = it.Next ();
    if (dynobj->ExistedAtBaseline () && dynobj->HasMovedSufficiently ())
    {
      hasMovedFromBaseline.Add (dynobj);
    }
  }
  checkForMovement.DeleteAll ();
}

void celPcDynamicWorld::SafeToRemove (iCelEntity* entity)
{
  safeToRemove.Add (entity);
}

void celPcDynamicWorld::UpdateObject (iDynamicObject* dynobj)
{
  dynobj->RefreshColliders ();
  dynobj->RecreateJoints ();
  ForceInvisible (dynobj);
}

void celPcDynamicWorld::UpdateObjects (iDynamicCell* cell)
{
  for (size_t i = 0 ; i < cell->GetObjectCount () ; i++)
  {
    iDynamicObject* obj = cell->GetObject (i);
    UpdateObject (obj);
  }
}

void celPcDynamicWorld::UpdateObjects (iDynamicFactory* factory)
{
  csRef<iDynamicCellIterator> it = GetCells ();
  while (it->HasNext ())
  {
    iDynamicCell* cell = it->NextCell ();
    for (size_t i = 0 ; i < cell->GetObjectCount () ; i++)
    {
      iDynamicObject* obj = cell->GetObject (i);
      if (obj->GetFactory () == factory)
        UpdateObject (obj);
    }
  }

  if (factory->IsLightFactory ())
  {
    csString name = factory->GetName ();
    meshCache.RemoveFactory (engine, name);
    iMeshFactoryWrapper* dummyfactory = engine->FindMeshFactory (name);
    if (dummyfactory)
      engine->RemoveObject (dummyfactory);

    using namespace CS::Geometry;
    Sphere primitive (csEllipsoid (csVector3 (0, 0, 0), csVector3 (.2, .2, .2)), 8);
    iLightFactory* lf = engine->FindLightFactory (name);
    const csColor& color = lf->GetColor ();
    int r = int (color.red * 255.1);
    int g = int (color.green * 255.1);
    int b = int (color.blue * 255.1);
    iMeshFactoryWrapper* mf = CreateDummyFactory (name, primitive, r, g, b, 50);
    (static_cast<DynamicFactory*> (factory))->ChangeFactory (mf);
  }
}

iDynamicFactory* celPcDynamicWorld::FindFactory (const char* factory) const
{
  return factory_hash.Get (factory, 0);
}

iMeshFactoryWrapper* celPcDynamicWorld::CreateDummyFactory (
    const char* factoryName, CS::Geometry::Primitive& primitive,
    int r, int g, int b, int a)
{
  using namespace CS::Geometry;
  csRef<iMeshFactoryWrapper> mf = GeneralMeshBuilder::CreateFactory (engine, factoryName, &primitive);
  // Create a single color transparent material if it doesn't already exist.
  csString materialName;
  // We divide colors by 16 to limit the amount of different materials that
  // will be created in case a light only changes slightly in an editor.
  materialName.Format ("__dynworld_%d_%d_%d_%d__", r/16, g/16, b/16, a);
  iMaterialWrapper* mat = engine->FindMaterial (materialName);
  if (!mat)
  {
    csRGBpixel singlePixel (r, g, b, a);
    iTextureManager* texman = g3d->GetTextureManager();
    int Format = texman->GetTextureFormat ();
    csRef<iImage> image;
    image.AttachNew (new csImageMemory (1, 1, (const void*)&singlePixel, Format));
    iTextureWrapper* tex = engine->GetTextureList ()->NewTexture (image);
    tex->Register (texman);
    tex->QueryObject ()->SetName (materialName);
    mat = engine->CreateMaterial (materialName, tex);
  }
  mf->GetMeshObjectFactory ()->SetMaterialWrapper (mat);
  return mf;
}

iDynamicFactory* celPcDynamicWorld::AddLogicFactory (const char* factory, float maxradius,
    float imposterradius, const csBox3& bbox)
{
  // We are creating a new invisible mesh.
  csRef<iMeshFactoryWrapper> mf;
  mf = engine->FindMeshFactory (factory);
  if (!mf)
  {
    using namespace CS::Geometry;
    Box primitive (bbox);
    CreateDummyFactory (factory, primitive, 0, 255, 255, 50);
  }

  csRef<DynamicFactory> obj;
  obj.AttachNew (new DynamicFactory (this, factory, true, maxradius, imposterradius, true));
  factories.Push (obj);
  factory_hash.Put (factory, obj);
  return obj;
}

iDynamicFactory* celPcDynamicWorld::AddFactory (const char* factory, float maxradius,
    float imposterradius)
{
  csRef<DynamicFactory> obj;
  obj.AttachNew (new DynamicFactory (this, factory, true, maxradius, imposterradius));
  factories.Push (obj);
  factory_hash.Put (factory, obj);
  return obj;
}

iDynamicFactory* celPcDynamicWorld::AddLightFactory (const char* factory,
    float maxradius)
{
  // We are creating a new invisible mesh.
  csRef<iMeshFactoryWrapper> mf;
  mf = engine->FindMeshFactory (factory);
  if (!mf)
  {
    using namespace CS::Geometry;
    Sphere primitive (csEllipsoid (csVector3 (0, 0, 0), csVector3 (.2, .2, .2)), 8);
    iLightFactory* lf = engine->FindLightFactory (factory);
    const csColor& color = lf->GetColor ();
    int r = int (color.red * 255.1);
    int g = int (color.green * 255.1);
    int b = int (color.blue * 255.1);
    CreateDummyFactory (factory, primitive, r, g, b, 50);
  }

  csRef<DynamicFactory> obj;
  obj.AttachNew (new DynamicFactory (this, factory, false, maxradius, -1));
  factories.Push (obj);
  factory_hash.Put (factory, obj);
  return obj;
}

void celPcDynamicWorld::RemoveFactory (iDynamicFactory* factory)
{
  factory_hash.DeleteAll (factory->GetName ());
  factories.Delete (static_cast<DynamicFactory*> (factory));
}

void celPcDynamicWorld::DeleteFactories ()
{
  factory_hash.DeleteAll ();
  factories.DeleteAll ();
}

void celPcDynamicWorld::ForceVisible (iDynamicObject* dynobj)
{
  static_cast<DynamicObject*> (dynobj)->PrepareCsObject (this);
}

void celPcDynamicWorld::ForceInvisible (iDynamicObject* dynobj)
{
  DynamicObject* dyn = static_cast<DynamicObject*> (dynobj);
  dyn->RemoveCsObject (this);
}

void celPcDynamicWorld::RemoveSafeEntities ()
{
  csSet<csPtrKey<iCelEntity> >::GlobalIterator it = safeToRemove.GetIterator ();
  while (it.HasNext ())
  {
    iCelEntity* entity = it.Next ();
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
      dyn->RemoveCsObject (this);
    }
    else
    {
      newset.Add (dyn);
    }
    dyn->SetFade (f);
  }
  fadingOut = newset;
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
  currentCell->tree->Front2Back (data.center, DynWorld_Front2Back, (void*)&data, 0);

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
      dyn->PrepareCsObject (this);
      fadingOut.Delete (dyn);
      fadingIn.Add (dyn);
    }
  }
}

void celPcDynamicWorld::SetRadius (float radius)
{
  celPcDynamicWorld::radius = radius;
}

void celPcDynamicWorld::Save (iDocumentNode* node)
{
  node->SetAttributeAsInt ("lastid", lastIDBlock);
  csHash<csRef<DynamicCell>,csString>::GlobalIterator it = cells.GetIterator ();
  while (it.HasNext ())
  {
    csString name;
    csRef<DynamicCell> cell = it.Next (name);
    csRef<iDocumentNode> el = node->CreateNodeBefore (CS_NODE_ELEMENT);
    el->SetValue ("cell");
    el->SetAttribute ("name", cell->GetName ());
    cell->Save (el);
  }
}

csRef<iString> celPcDynamicWorld::Load (iDocumentNode* node)
{
  if (node->GetAttribute ("lastid"))
    lastIDBlock = node->GetAttributeValueAsInt ("lastid");
  else
    lastIDBlock = 1000000001;        // @@@ Is this right?

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    csString value = child->GetValue ();
    if (value == "cell")
    {
      csString cellName = child->GetAttributeValue ("name");
      DynamicCell* cell = cells.Get (cellName, 0);
      if (!cell && cellCreator)
        cell = static_cast<DynamicCell*> (cellCreator->CreateCell (cellName));
      if (!cell)
      {
        csRef<scfString> str;
        str.AttachNew (new scfString ());
        str->Format ("Can't find cell %s!", cellName.GetData ());
        return str;
      }
      csRef<iString> error = cell->Load (child);
      if (error) return error;
    }
  }
  return 0;
}

void celPcDynamicWorld::MarkBaseline ()
{
  if (elcm)
    elcm->MarkBaseline ();
  currentCell->MarkBaseline ();
}

iDynamicObject* celPcDynamicWorld::FindObject (iCelEntity* entity) const
{
  return FindObject (entity->GetID ());
}

iDynamicObject* celPcDynamicWorld::FindObject (uint id) const
{
  return idToDynObj.Get (id, 0);
}

iDynamicObject* celPcDynamicWorld::FindObject (iRigidBody* body) const
{
  return FindObject (body->GetAttachedMesh ());
}

iDynamicObject* celPcDynamicWorld::FindObject (const char* name) const
{
  return nameToDynObj.Get (name, 0);
}

iDynamicObject* celPcDynamicWorld::FindObject (iMeshWrapper* mesh) const
{
  if (!mesh) return 0;
  iCelEntity* entity = pl->FindAttachedEntity (mesh->QueryObject ());
  if (!entity)
  {
    // We couldn't find an entity but it is possible that there is a dynamic
    // object associated with the mesh through a dynobjFinder object.
    return dynobjFinder::FindAttachedDynObj (mesh);
  }
  return FindObject (entity->GetID ());
}

void celPcDynamicWorld::Dump ()
{
  csPrintf ("### DynWorld ###\n");
  csPrintf ("  Fading in=%zu, out=%zu\n", fadingIn.GetSize (), fadingOut.GetSize ());
  csPrintf ("  Visible objects=%zu\n", visibleObjects.GetSize ());
  csPrintf ("  Safe to remove=%zu\n", safeToRemove.GetSize ());
}

// ---------------------------------------------------------------------------
// Saving and loading
// ---------------------------------------------------------------------------

csPtr<iDataBuffer> celPcDynamicWorld::SaveModifications ()
{
  csRef<csScfStringSet> strings;
  strings.AttachNew (new csScfStringSet ());
  csRef<iCelCompactDataBufferWriter> buf = pl->CreateCompactDataBufferWriter ();

  // Save the last ID block.
  buf->AddUInt32 (lastIDBlock);
  // For all cells, save the ID allocation.
  {
    csHash<csRef<DynamicCell>,csString>::GlobalIterator it = cells.GetIterator ();
    while (it.HasNext ())
    {
      csString name;
      csRef<DynamicCell> cell = it.Next (name);
      buf->AddID (strings->Request (cell->GetName ()));
      cell->SaveIDAllocations (buf);
    }
  }
  buf->AddID (csInvalidStringID);

  // Save all deleted entities.
  const csSet<uint>& deletedEntities = elcm->GetDeletedEntities ();
  buf->AddUInt32 (deletedEntities.GetSize ());
  csPrintf ("Save %zu deleted items\n", deletedEntities.GetSize ());
  csSet<uint>::GlobalIterator delIt = deletedEntities.GetIterator ();
  while (delIt.HasNext ())
  {
    uint id = delIt.Next ();
    buf->AddUInt32 (id);
  }

  // In this set we keep all dynamic objects that were already saved.
  csSet<csPtrKey<DynamicObject> > alreadySaved;

  csSet<csPtrKey<iCelEntity> > newEntites = elcm->GetNewEntities ();

  // First we save entities that have no dynobj. They are not part of
  // a single cell so we do that here.
  csRef<iCelEntityIterator> it = elcm->GetModifiedEntities ();
  while (it->HasNext ())
  {
    iCelEntity* entity = it->Next ();

    iDynamicObject* dynobj = FindObject (entity);
    if (!dynobj)
    {
      buf->AddUInt8 (MARKER_NEW);
      buf->AddUInt32 (entity->GetID ());

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
      if (newEntites.Contains (entity))
      {
        printf ("New entity without dynobj '%s'!\n", GetEntityNameInt (pl, entity).GetData ());
        csStringID tmpID = entity->GetTemplateNameID ();
        if (tmpID != csInvalidStringID)
        {
          csString tmpName = pl->FetchString (tmpID);
          buf->AddID (strings->Request (tmpName));
          if (entity->GetName ())
            buf->AddID (strings->Request (entity->GetName ()));
          else
            buf->AddID (csInvalidStringID);
	  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
	  if (pcmesh && pcmesh->GetFactoryName ())
	    buf->AddID (strings->Request (pcmesh->GetFactoryName ()));
	  else
	    buf->AddID (csInvalidStringID);
        }
        else
        {
          Error ("Can't save '%s'!\n", GetEntityNameInt (pl, entity).GetData ());
          return 0;
        }
      }
      else
      {
        printf ("Existing entity without dynobj '%s'!\n", GetEntityNameInt (pl, entity).GetData ());
        buf->AddID (csInvalidStringID);	// No template.
      }

      entity->SaveModifications (buf, strings);
    }
  }

  buf->AddUInt8 (MARKER_END);

  // Now save the dynamic objects in the cells.
  {
    csHash<csRef<DynamicCell>,csString>::GlobalIterator it = cells.GetIterator ();
    while (it.HasNext ())
    {
      csString name;
      csRef<DynamicCell> cell = it.Next (name);
      buf->AddID (strings->Request (cell->GetName ()));
      cell->SaveModifications (buf, strings, alreadySaved);
    }
  }
  buf->AddID (csInvalidStringID);

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

  lastIDBlock = (uint)buf->GetUInt32 ();
  restoringIDBlocks = true;

  // Load all cells and their ID allocation table. Don't load the dynamic
  // objects yet.
  csStringID cellID = buf->GetID ();
  while (cellID != csInvalidStringID)
  {
    const char* cellName = strings.Get (cellID, (const char*)0);
    DynamicCell* cell = cells.Get (cellName, 0);
    bool created = false;
    if (!cell && cellCreator)
    {
      cell = static_cast<DynamicCell*> (cellCreator->CreateCell (cellName));
      created = true;
    }
    if (!cell)
    {
      Error ("Failed to find/create the cell '%s'!\n", cellName);
      restoringIDBlocks = false;
      return;
    }
    printf ("Restoring ID allocations for %s\n", cellName); fflush (stdout);
    cell->RestoreIDAllocations (buf);
    if (created)
      cellCreator->FillCell (cell);
    cellID = buf->GetID ();
  }

  restoringIDBlocks = false;

  // Restore all deleted entities.
  size_t delSize = (size_t)buf->GetUInt32 ();
  csPrintf ("%zu to delete\n", delSize);
  for (size_t i = 0 ; i < delSize ; i++)
  {
    uint id = (uint)buf->GetUInt32 ();
    elcm->RegisterDeletedEntity (id);
    DynamicObject* dynobj = static_cast<DynamicObject*> (FindObject (id));
    if (dynobj)
    {
      dynobj->GetCell ()->DeleteObject (dynobj);
    }
    else
    {
      iCelEntity* entity = pl->GetEntity (id);
      printf ("Delete entity %s\n", GetEntityNameInt (pl, entity).GetData ());
      pl->RemoveEntity (entity);
    }
  }

  // First we read all entities that have no dynobj.
  uint8 marker = buf->GetUInt8 ();
  while (marker == MARKER_NEW)
  {
    uint id = buf->GetUInt32 ();

    csStringID tmpID = buf->GetID ();
    csStringID entNameID = csInvalidStringID;
    if (tmpID != csInvalidStringID)
    {
      entNameID = buf->GetID ();
    }

    iCelEntity* entity;
    if (tmpID != csInvalidStringID)
    {
      // Entity has to be created.
      const char* entName = strings.Get (entNameID, (const char*)0);
      const char* tmpName = strings.Get (tmpID, (const char*)0);
      csStringID factID = buf->GetID ();
      const char* factName = strings.Get (factID, (const char*)0);
      printf ("Loading new entity '%s' from '%s' (factory name '%s')\n", entName, tmpName, factName);
      iCelEntityTemplate* tmp = pl->FindEntityTemplate (tmpName);
      if (!tmp)
      {
        Error ("Error locating entity template '%s'\n", tmpName);
	return;
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
      if (factName)
      {
	csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
	pcmesh->SetFactoryName (factName);
      }
      elcm->RegisterNewEntity (entity);
    }
    else
    {
      entity = pl->GetEntity (id);
      if (!entity)
      {
        Error ("ERROR! Can't find entity with id %d\n", id);
        return;
      }
      printf ("Loading existing entity without dynobj '%s'\n", GetEntityNameInt (pl, entity).GetData ()); fflush (stdout);
      // @@@ The case where hasDynObj = true is not handled here. Is
      // this even possible?
    }

    entity->MarkBaseline ();
    entity->RestoreModifications (buf, strings);

    marker = buf->GetUInt8 ();
  }
  if (marker != MARKER_END)
  {
    Error ("Savefile corrupt! Bad marker (1)!\n");
    return;
  }

  // Now load the actual dynamic objects in the cells.
  cellID = buf->GetID ();
  while (cellID != csInvalidStringID)
  {
    const char* cellName = strings.Get (cellID, (const char*)0);
    DynamicCell* cell = cells.Get (cellName, 0);
    CS_ASSERT (cell != 0);
    cell->RestoreModifications (buf, strings);
    cellID = buf->GetID ();
  }
}

void celPcDynamicWorld::InhibitEntities (bool e)
{
  inhibitEntities = e;
  if (!inhibitEntities) return;
  csHash<csRef<DynamicCell>,csString>::GlobalIterator it = cells.GetIterator ();
  while (it.HasNext ())
  {
    csString name;
    csRef<DynamicCell> cell = it.Next (name);
    for (size_t i = 0 ; i < cell->GetObjectCount () ; i++)
    {
      DynamicObject* obj = cell->GetObjectInt (i);
      obj->ClearEntity ();
    }
  }
}

void celPcDynamicWorld::EnableGameMode (bool e)
{
  gameMode = e;
  csHash<csRef<DynamicCell>,csString>::GlobalIterator it = cells.GetIterator ();
  while (it.HasNext ())
  {
    csString name;
    csRef<DynamicCell> cell = it.Next (name);
    for (size_t i = 0 ; i < cell->GetObjectCount () ; i++)
    {
      iDynamicObject* obj = cell->GetObject (i);
      if (obj->GetMesh ())
      {
	bool invis;
	if (!gameMode) invis = false;
	else invis = (obj->GetFactory ()->IsLogicFactory () || obj->GetFactory ()->IsLightFactory ());
	iMeshWrapper* mesh = obj->GetMesh ();
	mesh->GetFlags ().Set (CS_ENTITY_INVISIBLE, invis ? CS_ENTITY_INVISIBLE : 0);
        if (!invis && (obj->GetFactory ()->IsLogicFactory () || obj->GetFactory ()->IsLightFactory ()))
	{
	  mesh->SetRenderPriority (engine->GetRenderPriority ("alpha"));
	  mesh->SetZBufMode (CS_ZBUF_TEST);
	}
	else
	{
	  mesh->SetRenderPriority (engine->GetRenderPriority ("object"));
	  mesh->SetZBufMode (CS_ZBUF_USE);
	}
      }
    }
  }
}

void celPcDynamicWorld::EnablePhysics (bool e)
{
  if (doPhysics == e) return;
  doPhysics = e;

  csRef<iDynamicCellIterator> it = GetCells ();
  while (it->HasNext ())
  {
    iDynamicCell* cell = it->NextCell ();
    UpdateObjects (cell);
  }
}

iCelEntity* celPcDynamicWorld::CreateSpawnedEntity (iCelEntityTemplate* tpl,
      const char* entityName, iCelParameterBlock* params,
      const csVector3& p, float yrot, const char* sector,
      const char* node)

{
  csVector3 pos = p;

  iSector* sect = engine->FindSector (sector);
  if (!sect)
  {
    Error ("Can't find sector %s!", sector);
    return 0;
  }

  if (node && *node)
  {
    Error ("Nodes not yet supported by dynworld plugin!\n");
    return 0;
  }

  csReversibleTransform trans (csYRotMatrix3 (yrot), pos);

  iDynamicObject* obj = currentCell->AddObject (tpl->GetName (), trans);
  obj->SetEntity (entityName, tpl->GetName (), params);
  iCelEntity* newent = obj->ForceEntity ();
  newent->MarkBaseline ();
  newent->Activate ();

  return newent;
}

