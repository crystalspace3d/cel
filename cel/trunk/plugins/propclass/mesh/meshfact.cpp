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
#include "cssysdef.h"
#include "csgeom/vector3.h"
#include "csgeom/vector4.h"
#include "csgeom/math3d.h"
#include "csgeom/quaternion.h"
#include "plugins/propclass/mesh/meshfact.h"
#include "propclass/camera.h"
#include "propclass/move.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "celtool/stdparams.h"
#include "csutil/util.h"
#include "csutil/csobject.h"
#include "csutil/flags.h"
#include "csutil/event.h"
#include "csutil/eventnames.h"
#include "iutil/object.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/objreg.h"
#include "iutil/eventq.h"
#include "iutil/plugin.h"
#include "iutil/vfs.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/engine.h"
#include "iengine/scenenode.h"
#include "imap/loader.h"
#include "ivaria/reporter.h"
#include "ivaria/mapnode.h"
#include "imesh/object.h"
#include "imesh/sprite3d.h"
#include "imesh/gmeshskel2.h"
#include "imesh/animesh.h"
#include "imesh/animnode/skeleton2anim.h"
#include "imesh/nullmesh.h"
#include "imesh/spritecal3d.h"
#include "imesh/skeleton.h"
#include <csgfx/shadervar.h>
#include <csgfx/shadervarcontext.h>
#include "cstool/enginetools.h"

//---------------------------------------------------------------------------

CS_PLUGIN_NAMESPACE_BEGIN(pfMesh)
{

CEL_IMPLEMENT_FACTORY_ALT (Mesh, "pcobject.mesh", "pcmesh")
CEL_IMPLEMENT_FACTORY_ALT (MeshSelect, "pcobject.mesh.select", "pcmeshselect")

static float FixAngle (float angle)
{
  if (angle >= PI)
    angle -= 2.0f * PI;
  if (angle < -PI)
    angle += 2.0f * PI;
  return angle;
}

//---------------------------------------------------------------------------

csStringID celPcMesh::id_name = csInvalidStringID;
csStringID celPcMesh::id_path = csInvalidStringID;
csStringID celPcMesh::id_filename = csInvalidStringID;
csStringID celPcMesh::id_factoryname = csInvalidStringID;
csStringID celPcMesh::id_sector = csInvalidStringID;
csStringID celPcMesh::id_position = csInvalidStringID;
csStringID celPcMesh::id_rotation = csInvalidStringID;
csStringID celPcMesh::id_forward = csInvalidStringID;
csStringID celPcMesh::id_up = csInvalidStringID;
csStringID celPcMesh::id_visible = csInvalidStringID;
csStringID celPcMesh::id_material = csInvalidStringID;
csStringID celPcMesh::id_type = csInvalidStringID;
csStringID celPcMesh::id_value = csInvalidStringID;
csStringID celPcMesh::id_animation = csInvalidStringID;
csStringID celPcMesh::id_cycle = csInvalidStringID;
csStringID celPcMesh::id_reset = csInvalidStringID;
csStringID celPcMesh::id_min = csInvalidStringID;
csStringID celPcMesh::id_max = csInvalidStringID;
csStringID celPcMesh::id_entity = csInvalidStringID;
csStringID celPcMesh::id_tag = csInvalidStringID;
csStringID celPcMesh::id_socket = csInvalidStringID;
csStringID celPcMesh::id_factory = csInvalidStringID;
csStringID celPcMesh::id_object = csInvalidStringID;

PropertyHolder celPcMesh::propinfo;

celPcMesh::celPcMesh (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  attached_entity = 0;
  visible = true;
  factory_ptr = 0;
  creation_flag = CEL_CREATE_NONE;
  engine = csQueryRegistry<iEngine> (object_reg);
  if (!engine)
  {
    Error ("No iEngine plugin!");
    return;
  }

  if (id_name == csInvalidStringID)
  {
    id_name = pl->FetchStringID ("name");
    id_path = pl->FetchStringID ("path");
    id_filename = pl->FetchStringID ("filename");
    id_factoryname = pl->FetchStringID ("factoryname");
    id_sector = pl->FetchStringID ("sector");
    id_position = pl->FetchStringID ("position");
    id_rotation = pl->FetchStringID ("rotation");
    id_forward = pl->FetchStringID ("forward");
    id_up = pl->FetchStringID ("up");
    id_visible = pl->FetchStringID ("visible");
    id_material = pl->FetchStringID ("material");
    id_value = pl->FetchStringID ("value");
    id_type = pl->FetchStringID ("type");
    id_animation = pl->FetchStringID ("animation");
    id_cycle = pl->FetchStringID ("cycle");
    id_reset = pl->FetchStringID ("reset");
    id_min = pl->FetchStringID ("min");
    id_max = pl->FetchStringID ("max");
    id_entity = pl->FetchStringID ("entity");
    id_tag = pl->FetchStringID ("tag");
    id_socket = pl->FetchStringID ("socket");
    id_factory = pl->FetchStringID ("factory");
    id_object = pl->FetchStringID ("object");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.mesh.action.");
    AddAction (action_setmesh, "SetMesh");
    AddAction (action_loadmesh, "LoadMesh");
    AddAction (action_loadmeshpath, "LoadMeshPath");
    AddAction (action_movemesh, "MoveMesh");
    AddAction (action_rotatemesh, "RotateMesh");
    AddAction (action_clearrotation, "ClearRotation");
    AddAction (action_lookat, "LookAt");
    AddAction (action_setvisible, "SetVisible");
    AddAction (action_setmaterial, "SetMaterial");
    AddAction (action_setshadervar, "SetShaderVar");
    AddAction (action_setanimation, "SetAnimation");
    AddAction (action_createemptything, "CreateEmptyThing");
    AddAction (action_createemptygenmesh, "CreateEmptyGenmesh");
    AddAction (action_createnullmesh, "CreateNullMesh");
    AddAction (action_parentmesh, "ParentMesh");
    AddAction (action_clearparent, "ClearParent");
    AddAction (action_attachsocketmesh, "AttachSocketMesh");
    AddAction (action_detachsocketmesh, "DetachSocketMesh");
  }

  // For properties.
  propinfo.SetCount (10);
  AddProperty (propid_position, "position",
  	CEL_DATA_VECTOR3, true, "Current position of mesh.", 0);
  AddProperty (propid_fullposition, "fullposition",
  	CEL_DATA_VECTOR3, true, "Current full position of mesh.", 0);
  AddProperty (propid_rotation, "rotation",
  	CEL_DATA_VECTOR3, true, "Current rotation of mesh.", 0);
  AddProperty (propid_eulerrotation, "eulerrotation",
  	CEL_DATA_VECTOR3, true, "Current euler rotation of mesh.", 0);
  AddProperty (propid_sector, "sector",
  	CEL_DATA_STRING, true, "Current sector of mesh.", 0);
  AddProperty (propid_path, "path",
  	CEL_DATA_STRING, true, "VFS path for model.", 0);
  AddProperty (propid_factory, "factory",
  	CEL_DATA_STRING, true, "Factory name for the model.", 0);
  AddProperty (propid_filename, "filename",
  	CEL_DATA_STRING, true, "Filename for the model.", 0);
  AddProperty (propid_hitbeam, "hitbeam",
  	CEL_DATA_BOOL, false, "Allow hitbeams for the mesh.", 0);
  AddProperty (propid_meshname, "meshname",
  	CEL_DATA_STRING, true, "Mesh object name for the model.", 0);
}

celPcMesh::~celPcMesh ()
{
  Clear ();
  delete [] propdata;
}

bool celPcMesh::GetPropertyIndexed (int idx, bool& val)
{
  switch (idx)
  {
    case propid_hitbeam:
      if (mesh)
        val = !(mesh->GetFlags ().Check (CS_ENTITY_NOHITBEAM));
      else
        val = false;
      return true;
    default:
      return false;
  }
}

bool celPcMesh::SetPropertyIndexed (int idx, bool val)
{
  switch (idx)
  {
    case propid_hitbeam:
      if (mesh)
        mesh->SetFlagsRecursive (CS_ENTITY_NOHITBEAM,
        	val ? 0: CS_ENTITY_NOHITBEAM);
      return true;
    default:
      return false;
  }
}

bool celPcMesh::GetPropertyIndexed (int idx, csVector3& v)
{
  switch (idx)
  {
    case propid_position:
      if (mesh)
        v = mesh->GetMovable ()->GetTransform ().GetOrigin ();
      else
        v.Set (0.0f, 0.0f, 0.0f);
      return true;
    case propid_fullposition:
      if (mesh)
        v = mesh->GetMovable ()->GetFullPosition ();
      else
        v.Set (0.0f, 0.0f, 0.0f);
      return true;
    case propid_rotation:
      if (mesh)
      {
        csQuaternion quat;
        csVector3 vec;
        float ang;
        quat.SetMatrix (mesh->GetMovable ()->GetTransform ().GetT2O ());
        quat.GetAxisAngle (vec, ang);
        vec.x *= ang;
        vec.x = FixAngle (vec.x);
        vec.y *= ang;
        vec.y = FixAngle (vec.y);
        vec.z *= ang;
        vec.z = FixAngle (vec.z);
        v.Set (-vec.x, -vec.y, -vec.z);
      }
      else
        v.Set (0.0f, 0.0f, 0.0f);
      return true;
    case propid_eulerrotation:
      if (mesh)
      {
        csQuaternion quat;
        quat.SetMatrix (mesh->GetMovable ()->GetTransform ().GetT2O ());
        v.Set (quat.GetEulerAngles ());
      }
      else
        v.Set (0.0f, 0.0f, 0.0f);
      return true;
    default:
      return false;
  }
}

bool celPcMesh::GetPropertyIndexed (int idx, const char*& s)
{
  switch (idx)
  {
    case propid_sector:
      if (mesh)
      {
        iSectorList* sl = mesh->GetMovable ()->GetSectors ();
        if (sl->GetCount () > 0)
          s = sl->Get (0)->QueryObject ()->GetName ();
        else
          s = 0;
      }
      else
      {
        s = 0;
      }
      return true;
    case propid_path:
      s = path.GetData ();
      return true;
    case propid_factory:
      s = factName.GetData ();
      return true;
    case propid_filename:
      s = fileName.GetData ();
      return true;
    case propid_meshname:
      s = meshName.GetData ();
      return true;
    default:
      return false;
  }
}

void celPcMesh::Clear ()
{
  meshName.Empty ();
  fileName.Empty ();
  factName.Empty ();
  path.Empty ();
  RemoveMesh ();
  factory_ptr = 0;
}

void celPcMesh::RemoveMesh ()
{
  if (mesh)
  {
    if (pl && attached_entity)
    {
      pl->UnattachEntity (mesh->QueryObject (), attached_entity);
      attached_entity = 0;
    }
    // CEL_CREATE_MESHREMOVE is also removed here.
    if (creation_flag != CEL_CREATE_MESH)
      engine->RemoveObject (mesh);
    mesh = 0;
    FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
  }
  creation_flag = CEL_CREATE_NONE;
}

bool celPcMesh::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setmesh:
      {
	csString name;
	if (!Fetch (name, params, id_name)) return false;
        iMeshWrapper* m = engine->FindMeshObject (name);
        if (!m)
          return Error ("Can't find mesh '%s' for action SetMesh!",
          	name.GetData ());
        SetMesh (m, false);
        return true;
      }
    case action_setvisible:
      {
        if (mesh)
        {
	  bool visible;
	  if (!Fetch (visible, params, id_visible, true, true)) return false;
          if (visible)
            mesh->SetFlagsRecursive (CS_ENTITY_INVISIBLE, 0);
          else
            mesh->SetFlagsRecursive (CS_ENTITY_INVISIBLE, CS_ENTITY_INVISIBLE);
        }
        return true;
      }
    case action_setmaterial:
      {
	csString material;
	if (!Fetch (material, params, id_material)) return false;
        iMaterialWrapper* mat = engine->FindMaterial (material);
        if (!mat)
          return Error ("Can't find material '%s' for SetMaterial!",
          	material.GetData ());
        if (mesh)
        {
          mesh->GetMeshObject ()->SetMaterialWrapper (mat);
        }
        return true;
      }
    case action_loadmesh:
      {
	csString file, factory;
	if (!Fetch (file, params, id_filename, true, "")) return false;
	if (!Fetch (factory, params, id_factoryname)) return false;
        bool rc = SetMesh (factory, file);
        if (!rc)
          return Error ("Can't load mesh '%s/%s' for action LoadMesh!",
          	factory.GetData (), file.GetData ());
        return true;
      }
    case action_loadmeshpath:
      {
	csString pa, file, factory;
	if (!Fetch (pa, params, id_path)) return false;
	if (!Fetch (file, params, id_filename)) return false;
	if (!Fetch (factory, params, id_factoryname)) return false;
        SetPath (pa);
        bool rc = SetMesh (factory, file);
        if (!rc)
          return Error (
          	"Can't load mesh '%s/%s' (path '%s') for action LoadMeshPath!",
          	(const char*)factory, (const char*)file, (const char*)path);
        return true;
      }
    case action_movemesh:
      {
	csString sector;
	if (!Fetch (sector, params, id_sector, true, "")) return false;
        if (sector.IsEmpty ())
        {
          // Special case. We simply remove the mesh from all sectors.
          if (mesh)
          {
            mesh->GetMovable ()->ClearSectors ();
            mesh->GetMovable ()->UpdateMove ();
          }
        }
        else
        {
          iSector* sect = engine->FindSector (sector);
	  if (!sect)
	    return Error (
	      "Can't find sector '%s' for action MoveMesh!",
	      sector.GetData ());
	  if (ParExists (CEL_DATA_VECTOR3, params, id_position))
	  {
	    csVector3 position;
	    if (!Fetch (position, params, id_position)) return false;
            MoveMesh (sect, position);
	  }
          else
          {
	    csString node;
	    if (!Fetch (node, params, id_position)) return false;
            MoveMesh (sect, node);
          }
        }
	if (mesh && ParExists (CEL_DATA_VECTOR3, params, id_rotation))
	{
	  csVector3 rotation;
	  if (!Fetch (rotation, params, id_rotation)) return false;
          csQuaternion quat;
          quat.SetEulerAngles (rotation);
          mesh->GetMovable ()->SetTransform (quat.GetMatrix ());
          mesh->GetMovable ()->UpdateMove ();
        }
        return true;
      }
    case action_rotatemesh:
      {
	csVector3 rotation;
	if (!Fetch (rotation, params, id_rotation)) return false;
        if (mesh)
        {
          iMovable* mov = mesh->GetMovable ();
          csQuaternion quat;
          quat.SetEulerAngles (rotation);
          csReversibleTransform tr (quat.GetMatrix (), csVector3 (0));
          mov->SetTransform (tr * mov->GetTransform ());
          mov->UpdateMove ();
        }
        return true;
      }
    case action_clearrotation:
      if (mesh)
      {
        mesh->GetMovable ()->SetTransform (csMatrix3 ());
        mesh->GetMovable ()->UpdateMove ();
      }
      return true;
    case action_lookat:
      {
	csVector3 forward, up;
	if (!Fetch (forward, params, id_forward, true, csVector3 (0, 0, 1))) return false;
	if (!Fetch (up, params, id_up, true, csVector3 (0, 1, 0))) return false;
        if (mesh)
        {
          mesh->GetMovable ()->GetTransform ().LookAt (forward, up);
          mesh->GetMovable ()->UpdateMove ();
        }
        return true;
      }
    case action_setshadervar:
      {
	csString par_name, par_type;
	if (!Fetch (par_name, params, id_name)) return false;
	if (!Fetch (par_type, params, id_type)) return false;
	csRef<iShaderVarStringSet> strset = csQueryRegistryTagInterface<iShaderVarStringSet> (
	    object_reg, "crystalspace.shader.variablenameset");
        if (par_type == "float")
        {
	  float par_value;
	  if (!Fetch (par_value, params, id_value)) return false;
          SetShaderVar (strset->Request (par_name), par_value);
        }
        else if (par_type == "long")
        {
	  long par_value;
	  if (!Fetch (par_value, params, id_value)) return false;
          SetShaderVar (strset->Request (par_name), (int)par_value);
        }
        else if (par_type == "vector2")
        {
	  csVector2 par_value;
	  if (!Fetch (par_value, params, id_value)) return false;
          SetShaderVar (strset->Request (par_name), par_value);
        }
        else if (par_type == "vector3")
        {
	  csVector3 par_value;
	  if (!Fetch (par_value, params, id_value)) return false;
          SetShaderVar (strset->Request (par_name), par_value);
        }
        else if (par_type == "vector4")
        {
	  csVector4 par_value;
	  if (!Fetch (par_value, params, id_value)) return false;
          SetShaderVar (strset->Request (par_name), par_value);
        }
        else if ( par_type == "libexpr")
        {
	  csString par_value;
	  if (!Fetch (par_value, params, id_value)) return false;
          return SetShaderVarExpr (strset->Request (par_name), par_value);
        }
        else
          return Error (
          	"Unsupported type %s for action SetShaderVar!",
		par_type.GetData ());
        return true;
      }
    case action_setanimation:
      {
	csString par_animation;
	if (!Fetch (par_animation, params, id_animation)) return false;
	bool par_cycle, par_reset;
	if (!Fetch (par_cycle, params, id_cycle, true, false)) return false;
	if (!Fetch (par_reset, params, id_reset, true, false)) return false;
        SetAnimation (par_animation, par_cycle, 1.0f, 0.1f, 0.1f, par_reset);
        return true;
      }
    case action_createemptything:
      {
	csString par_factoryname;
	if (!Fetch (par_factoryname, params, id_factoryname)) return false;
        CreateEmptyThing (par_factoryname);
        return true;
      }
    case action_createemptygenmesh:
      {
	csString par_factoryname;
	if (!Fetch (par_factoryname, params, id_factoryname)) return false;
        CreateEmptyGenmesh (par_factoryname);
        return true;
      }
    case action_createnullmesh:
      {
	csString par_factoryname;
	if (!Fetch (par_factoryname, params, id_factoryname)) return false;
	csVector3 par_min, par_max;
	if (!Fetch (par_min, params, id_min)) return false;
	if (!Fetch (par_max, params, id_max)) return false;
        CreateNullMesh (par_factoryname, csBox3 (par_min, par_max));
        return true;
      }
    case action_parentmesh:
      {
        if (!mesh) return true;
	csString par_entity;
	if (!Fetch (par_entity, params, id_entity, true, "")) return false;
        iCelEntity* ent;
        if (par_entity.IsEmpty ()) ent = entity;
        else
        {
          ent = pl->FindEntity (par_entity);
          if (!ent)
            return Error ("Can't find entity '%s'!",
            	par_entity.GetData ());
        }
	csString par_tag;
	if (!Fetch (par_tag, params, id_tag, true, "")) return false;
        csRef<iPcMesh> parent_mesh;
        if (par_tag.IsEmpty ())
          parent_mesh = celQueryPropertyClassEntity<iPcMesh> (ent);
        else
          parent_mesh = celQueryPropertyClassTag<iPcMesh> (
          	ent->GetPropertyClassList (), par_tag);
        if (!parent_mesh)
          return Error ("Can't find a mesh!");
        mesh->QuerySceneNode ()->SetParent (parent_mesh->GetMesh ()
        	->QuerySceneNode ());
        mesh->GetMovable ()->UpdateMove ();
        return true;
      }
    case action_clearparent:
      {
        if (mesh)
        {
          mesh->QuerySceneNode ()->SetParent (0);
          mesh->GetMovable ()->UpdateMove ();
        }
        return true;
      }
    case action_attachsocketmesh:
      {
	csString socket, factory;
	if (!Fetch (socket, params, id_socket)) return false;
	if (!Fetch (factory, params, id_factory, true, "")) return false;
        if (!factory.IsEmpty ())
        {
          iMeshFactoryWrapper* meshfact = engine->GetMeshFactories ()
          	->FindByName (factory);
          if (!meshfact)
            return Error (
            	"Can't find factory '%s' for AttachSocketMesh!",
            	(const char*)factory);
          csRef<iMeshWrapper> meshobj = engine->CreateMeshWrapper (meshfact, factory);
          if (!meshobj)
            return Error (
            	"Can't create meshobj from '%s' in AttachSocketMesh!",
            	(const char*)factory);
          AttachSocketMesh (socket, meshobj);
        }
        else
        {
	  csString object;
	  if (!Fetch (object, params, id_object)) return false;
	  csRef<iMeshWrapper> meshobj = engine->FindMeshObject (object);
	  if (!meshobj)
	    return Error (
	      "Can't find meshobj '%s' in AttachSocketMesh!",
	      (const char*)object);
	  AttachSocketMesh (socket, meshobj);
        }
        return true;
      }
    case action_detachsocketmesh:
      {
	csString socket;
	if (!Fetch (socket, params, id_socket)) return false;
        DetachSocketMesh (socket);
        return true;
      }
    default:
      return false;
  }
}

iMeshFactoryWrapper* celPcMesh::LoadMeshFactory ()
{
  csString newpath = path;
  if (newpath.IsEmpty ())
  {
    csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
    newpath = vfs->GetCwd();
  }

  csRef<iThreadedLoader> tloader = csQueryRegistry<iThreadedLoader> (object_reg);
  csRef<iThreadReturn> ret = tloader->LoadFileWait(newpath, fileName, 0);

  if(!ret->WasSuccessful())
  {
    Error ("Error loading mesh object factory or library '%s'!",
      (const char*)fileName);
    return 0;
  }
  csRef<iBase> result = ret->GetResultRefPtr();

  csRef<iMeshFactoryWrapper> imeshfact;
  if (result == 0)
  {
    // We have a library.
    imeshfact = engine->FindMeshFactory (factName);
  }
  else
  {
    imeshfact = scfQueryInterface<iMeshFactoryWrapper> (result);
    if (!imeshfact)
    {
      // Perhaps it is a world file?
      csRef<iEngine> eng = scfQueryInterface<iEngine> (result);
      if (eng)
      {
        if(tloader.IsValid())
        {
          engine->SyncEngineListsNow(tloader);
        }
        imeshfact = engine->FindMeshFactory (factName);
      }
    }
  }

  if (imeshfact == 0)
  {
    Error ("Error loading mesh object factory '%s'!",
    	(const char*)fileName);
    return 0;
  }
  return imeshfact;
}

void celPcMesh::SetPath (const char* path)
{
  celPcMesh::path = path;
}

bool celPcMesh::SetMesh (const char* factname, const char* filename)
{
  fileName = filename;
  factName = factname;

  RemoveMesh ();
  creation_flag = CEL_CREATE_FACTORY;

  if (!factName.IsEmpty ())
  {
    csRef<iMeshFactoryWrapper> meshfact = engine->GetMeshFactories ()
    	->FindByName (factname);
    if (!meshfact)
    {
      meshfact = LoadMeshFactory ();
      if (meshfact)
      {
        // Cache the factory.
        pl->Cache (meshfact);
      }
    }

    if (factory_ptr)
      factory_ptr = 0;
    if (meshfact)
    {
      factory_ptr = meshfact;
      mesh = engine->CreateMeshWrapper (meshfact, factname/*@@@?*/);
      pl->AttachEntity (mesh->QueryObject (), entity);
      attached_entity = entity;
      FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
      return true;
    }
    return false;
  }
  return true;
}

void celPcMesh::SetMesh (iMeshWrapper* m, bool do_remove)
{
  RemoveMesh ();
  if (do_remove)
    creation_flag = CEL_CREATE_MESHREMOVE;
  else
    creation_flag = CEL_CREATE_MESH;
  mesh = m;
  if (mesh)
  {
    pl->AttachEntity (mesh->QueryObject (), entity);
    attached_entity = entity;
    meshName = mesh->QueryObject ()->GetName ();
    iMeshFactoryWrapper* fact = mesh->GetFactory ();
    if (fact)
      factName = fact->QueryObject ()->GetName ();
  }
  FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
}

void celPcMesh::CreateEmptyThing (const char* factname)
{
  RemoveMesh ();

  csRef<iMeshFactoryWrapper> meshfact = engine->GetMeshFactories ()
  	->FindByName (factname);
  if (meshfact)
  {
    SetMesh (factname, 0);
    return;
  }

  creation_flag = CEL_CREATE_THING;
  meshfact = engine->CreateMeshFactory ("crystalspace.mesh.object.thing",
  	factname);

  mesh = engine->CreateMeshWrapper (meshfact, factname, 0, csVector3 (0));
  pl->AttachEntity (mesh->QueryObject (), entity);
  attached_entity = entity;
  FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
}

void celPcMesh::CreateEmptyGenmesh (const char* factname)
{
  RemoveMesh ();

  csRef<iMeshFactoryWrapper> meshfact = engine->GetMeshFactories ()
  	->FindByName (factname);
  if (meshfact)
  {
    SetMesh (factname, 0);
    return;
  }

  creation_flag = CEL_CREATE_GENMESH;
  meshfact = engine->CreateMeshFactory ("crystalspace.mesh.object.genmesh",
  	factname);
  mesh = engine->CreateMeshWrapper (meshfact, factname, 0, csVector3 (0));
  pl->AttachEntity (mesh->QueryObject (), entity);
  attached_entity = entity;
  FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
}

void celPcMesh::CreateNullMesh (const char* factname,
    const csBox3& box)
{
  RemoveMesh ();

  csRef<iMeshFactoryWrapper> meshfact = engine->GetMeshFactories ()
  	->FindByName (factname);
  if (meshfact)
  {
    SetMesh (factname, 0);
    return;
  }

  creation_flag = CEL_CREATE_NULLMESH;
  meshfact = engine->CreateMeshFactory ("crystalspace.mesh.object.null",
  	factname);
  csRef<iNullFactoryState> nullmesh = scfQueryInterface<iNullFactoryState> (
  	meshfact->GetMeshObjectFactory ());
  nullmesh->SetBoundingBox (box);
  mesh = engine->CreateMeshWrapper (meshfact, factname, 0, csVector3 (0));
  pl->AttachEntity (mesh->QueryObject (), entity);
  attached_entity = entity;
  FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
}

void celPcMesh::MoveMesh (iSector* sector, const csVector3& pos)
{
  if (!mesh) return;
  if (sector)
    mesh->GetMovable ()->SetSector (sector);

  mesh->GetMovable ()->SetPosition (pos);
  mesh->GetMovable ()->UpdateMove ();
}

void celPcMesh::MoveMesh (iSector* sector, const char* node)
{
  if (!mesh) return;
  csRef<iMapNode> mapnode;
  if (sector)
  {
    mesh->GetMovable ()->SetSector (sector);
    mapnode = CS::GetNamedChildObject<iMapNode> (
    	sector->QueryObject (), node);
  }
  else
  {
    iSector* sect = 0;
    if (mesh && mesh->GetMovable ()->GetSectors ()->GetCount () > 0)
    {
      sect = mesh->GetMovable ()->GetSectors ()->Get (0);
      mapnode = CS::GetNamedChildObject<iMapNode> (
      	sect->QueryObject (), node);
    }
    else
    {
      Error ("Can't find current sector for MoveMesh!");
    }
  }
  if (mapnode)
  {
    mesh->GetMovable ()->SetPosition (mapnode->GetPosition ());
    mesh->GetMovable ()->UpdateMove ();
  }
  else
    Error ("Can't find node '%s' for MoveMesh!", (const char*)node);
}

void celPcMesh::MoveMesh (iPcMesh* other_mesh, const csVector3& offset)
{
  if (!mesh) return;
  
  // get movable for efficiency.
  iMovable *other_movable = other_mesh->GetMesh()->GetMovable();

  // position in other mesh's sector.
  if (other_movable->GetSectors()->GetCount())
    mesh->GetMovable ()->SetSector (other_movable->GetSectors()->Get(0));

  // set position
  csVector3 world_offset = other_movable->GetTransform().Other2This(offset);
  mesh->GetMovable ()->SetPosition (other_movable->GetPosition()+offset);

  // update
  mesh->GetMovable ()->UpdateMove ();
}

void celPcMesh::SetAnimation (const char* actionName, bool cycle,
	float weight, float fadein, float fadeout, bool reset)
{
  if (!actionName) return;
  if (!mesh) return;

  // Sprite3D mesh
  csRef<iSprite3DState> spr3dstate (
  	scfQueryInterface<iSprite3DState> (mesh->GetMeshObject ()));
  if (spr3dstate)
  {
    spr3dstate->SetAction (actionName, cycle);
    return;
  }


  // Cal3D mesh
  csRef<iSpriteCal3DState> sprcal3dstate
    (scfQueryInterface<iSpriteCal3DState> (mesh->GetMeshObject ()));
  if (sprcal3dstate)
  {
    if (cycle)
      sprcal3dstate->SetAnimCycle (actionName, weight);
    else
      sprcal3dstate->SetAnimAction (actionName, fadein, fadeout);
    return;
  }

  // Animesh
  csRef<CS::Mesh::iAnimatedMesh> animesh = scfQueryInterface<CS::Mesh::iAnimatedMesh> (mesh->GetMeshObject ());
  if (animesh)
  {
    csRef<CS::Animation::iSkeletonAnimNode> root;
    csRef<CS::Animation::iSkeletonAnimNode> anim;
    csRef<CS::Animation::iSkeletonFSMNode> fsm;
    root = animesh->GetSkeleton()->GetAnimationPacket()->GetAnimationRoot ();
    if (!root) return;

    // Search for an FSM node
    anim = root->FindNode ("fsm");
    if (anim)
      fsm = scfQueryInterface<CS::Animation::iSkeletonFSMNode> (anim);
    if (fsm)
    {
      root->Play ();
      csRef<CS::Animation::iSkeletonFSMNodeFactory> fsmfact =
	scfQueryInterface<CS::Animation::iSkeletonFSMNodeFactory> (root->GetFactory ());
      CS::Animation::StateID wanted_state = fsmfact->FindState (actionName);
      if (wanted_state == CS::Animation::InvalidStateID)
	return;
      if (wanted_state != fsm->GetCurrentState ())
	fsm->SwitchToState (wanted_state);
    }

    else
    {
      // TODO: create an FSM node?
      anim = root->FindNode (actionName);
      if (!anim || anim->IsActive ())
	return;
      root->Stop ();
      csRef<CS::Animation::iSkeletonAnimationNodeFactory> animfact =
	scfQueryInterface<CS::Animation::iSkeletonAnimationNodeFactory> (anim->GetFactory ());
      animfact->SetCyclic (cycle);
      anim->Play();
    }

    return;
  }

  // Genmesh
  csRef<iGeneralMeshState> genstate =
    scfQueryInterface<iGeneralMeshState> (mesh->GetMeshObject ());
  if (genstate)
  {
    csRef<iGenMeshAnimationControl> skelstate = genstate
      ->GetAnimationControl ();
    if (skelstate)
    {
      csRef<iGenMeshSkeletonControlState> ctlstate =
	scfQueryInterface<iGenMeshSkeletonControlState> (skelstate);
      if (ctlstate)
      {
	csRef<iSkeleton> skel = ctlstate->GetSkeleton ();
	if (skel)
	{
	  iSkeletonAnimation* script;
	  if (reset) script = 0;
	  else script  = skel->FindAnimation (actionName);
	  if (script)
          {
	    if (script->GetLoop () != cycle)
	      script->SetLoop (cycle);
	  }
	  else
          {
	    skel->StopAll ();
	    script = skel->Execute (actionName);
	    if (script) script->SetLoop (cycle);
	  }
	}
      }
    }

    return;
  }
}

void celPcMesh::SetAction (const char* actionName, bool resetaction)
{
  if (!actionName) return;
  if (!mesh) return;
  csRef<iSprite3DState> state (
  	scfQueryInterface<iSprite3DState> (mesh->GetMeshObject ()));
  if (state)
  {
    if (resetaction || strcmp (actionName, state->GetCurAction ()
    	->GetName ())) state->SetAction (actionName);
  }
}

void celPcMesh::SetReverseAction (bool reverse)
{
  if (!mesh) return;
  csRef<iSprite3DState> state (scfQueryInterface<iSprite3DState> (
  	mesh->GetMeshObject ()));
  if (state)
  {
    state->SetReverseAction (reverse);
  }
}

const char* celPcMesh::GetAction ()
{
  if (!mesh) return 0;
  csRef<iSprite3DState> state (scfQueryInterface<iSprite3DState> (
  	mesh->GetMeshObject ()));
  if (state)
  {
    const char* act = state->GetCurAction ()->GetName ();
    return act;
  }
  return 0;
}

void celPcMesh::Hide ()
{
  if (!visible) return;
  visible = false;
  if (mesh) mesh->GetFlags ().Set (CS_ENTITY_INVISIBLE);
}

void celPcMesh::Show ()
{
  if (visible) return;
  visible = true;
  if (mesh) mesh->GetFlags ().Reset (CS_ENTITY_INVISIBLE);
}

bool celPcMesh::AttachSocketMesh (const char* socket, iMeshWrapper* meshwrapper)
{
  csRef<iGeneralMeshState> genstate = scfQueryInterface<iGeneralMeshState> (
  	GetMesh ()->GetMeshObject ());
  if (genstate)
  {
    csRef<iGenMeshAnimationControl> skelstate = genstate
    	->GetAnimationControl ();
    if (skelstate)
    {
      csRef<iGenMeshSkeletonControlState> ctlstate =
      	scfQueryInterface<iGenMeshSkeletonControlState> (skelstate);
      if (ctlstate)
      {
        csRef<iSkeleton> skel = ctlstate->GetSkeleton ();
        if (skel)
        {
          iSkeletonSocket* skelsocket = skel->FindSocket (socket);
          if (!skelsocket)
            return Error (
            	"Can't find socket '%s' for AttachSocketMesh!",
            	(const char*)socket);
          meshwrapper->QuerySceneNode ()->SetParent (
          	GetMesh ()->QuerySceneNode ());
          skelsocket->SetSceneNode (meshwrapper->QuerySceneNode ());
          return true;
        }
      }
    }
  }

  csRef<iSprite3DState> spr3dstate = scfQueryInterface<iSprite3DState> (
  	GetMesh ()->GetMeshObject ());
  if (spr3dstate)
  {
    iSpriteSocket* spr3dsocket = spr3dstate->FindSocket (socket);
    if (!spr3dsocket)
      return Error (
      	"Can't find socket '%s' for AttachSocketMesh!",
      	(const char*)socket);
    meshwrapper->QuerySceneNode ()->SetParent (GetMesh ()->QuerySceneNode ());
    spr3dsocket->SetMeshWrapper (meshwrapper);
    return true;
  }

  csRef<iSpriteCal3DState> cal3dstate = scfQueryInterface<iSpriteCal3DState> (
  	GetMesh ()->GetMeshObject ());
  if (cal3dstate)
  {
    iSpriteCal3DSocket* cal3dsocket = cal3dstate->FindSocket (socket);
    if (!cal3dsocket)
      return Error (
      	"Can't find socket '%s' for AttachSocketMesh!",
      	(const char*)socket);
    meshwrapper->QuerySceneNode ()->SetParent (GetMesh ()->QuerySceneNode ());
    cal3dsocket->SetMeshWrapper (meshwrapper);
    return true;
  }
  return false;
}

bool celPcMesh::DetachSocketMesh (const char* socket)
{
  csRef<iGeneralMeshState> genstate = scfQueryInterface<iGeneralMeshState> (
  	GetMesh ()->GetMeshObject ());
  if (genstate)
  {
    csRef<iGenMeshAnimationControl> skelstate = genstate
    	->GetAnimationControl ();
    if (skelstate)
    {
      csRef<iGenMeshSkeletonControlState> ctlstate =
      	scfQueryInterface<iGenMeshSkeletonControlState> (skelstate);
      if (ctlstate)
      {
        csRef<iSkeleton> skel = ctlstate->GetSkeleton ();
        if (skel)
        {
          iSkeletonSocket* skelsocket = skel->FindSocket (socket);
          if (!skelsocket)
            return Error (
            	"Can't find socket '%s' for DetachSocketMesh!",
            	(const char*)socket);
          iSceneNode* scnode = skelsocket->GetSceneNode ();
          if (!scnode)
            return false;
          iMeshWrapper* skelmeshobj = scnode->QueryMesh ();
          if (!skelmeshobj)
            return false;
          skelmeshobj->QuerySceneNode ()->SetParent (0);
          skelsocket->SetSceneNode (0);
          return true;
        }
      }
    }
  }

  csRef<iSprite3DState> spr3dstate = scfQueryInterface<iSprite3DState> (
  	GetMesh ()->GetMeshObject ());
  if (spr3dstate)
  {
    iSpriteSocket* spr3dsocket = spr3dstate->FindSocket (socket);
    if (!spr3dsocket)
      return Error (
      	"Can't find socket '%s' for DetachMesh!", (const char*)socket);
    iMeshWrapper* spr3dmeshobj = spr3dsocket->GetMeshWrapper ();
    if (!spr3dmeshobj)
      return false;
    spr3dmeshobj->QuerySceneNode ()->SetParent (0);
    spr3dsocket->SetMeshWrapper (0);
    return true;
  }

  csRef<iSpriteCal3DState> cal3dstate = scfQueryInterface<iSpriteCal3DState> (
  	GetMesh ()->GetMeshObject ());
  if (cal3dstate)
  {
    iSpriteCal3DSocket* cal3dsocket = cal3dstate->FindSocket (socket);
    if (!cal3dsocket)
      return Error (
      	"Can't find socket '%s' for DetachMesh!", (const char*)socket);
    iMeshWrapper* cal3dmeshobj = cal3dsocket->GetMeshWrapper ();
    if (!cal3dmeshobj)
      return false;
    cal3dmeshobj->QuerySceneNode ()->SetParent (0);
    cal3dsocket->SetMeshWrapper (0);
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------

csStringID celPcMeshSelect::id_buttons = csInvalidStringID;
csStringID celPcMeshSelect::id_normal = csInvalidStringID;
csStringID celPcMeshSelect::id_camera = csInvalidStringID;
csStringID celPcMeshSelect::id_x = csInvalidStringID;
csStringID celPcMeshSelect::id_y = csInvalidStringID;
csStringID celPcMeshSelect::id_button = csInvalidStringID;
csStringID celPcMeshSelect::id_entity = csInvalidStringID;

PropertyHolder celPcMeshSelect::propinfo;

celPcMeshSelect::celPcMeshSelect (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  pccamera = 0;
  cur_on_top = false;
  mouse_buttons = CEL_MOUSE_BUTTON1;

  drag_normal.Set (0, 0, 1);
  drag_normal_camera = true;

  // Initialize default behaviour.
  do_global = false;
  do_drag = false;
  do_follow = false;
  do_follow_always = false;
  do_sendup = true;
  do_senddown = true;
  do_sendmove = false;

  mousedrv = csQueryRegistry<iMouseDriver> (object_reg);
  name_reg = csEventNameRegistry::GetRegistry (object_reg);

  // Initialize the maximum selection distance to a very large number
  max_distance = 100000.0f;

  if (id_x == csInvalidStringID)
  {
    id_x = pl->FetchStringID ("x");
    id_y = pl->FetchStringID ("y");
    id_button = pl->FetchStringID ("button");
    id_entity = pl->FetchStringID ("entity");
    id_buttons = pl->FetchStringID ("buttons");
    id_normal = pl->FetchStringID ("normal");
    id_camera = pl->FetchStringID ("camera");
  }
  params.AttachNew (new celVariableParameterBlock (4));
  params->AddParameter (id_x);
  params->AddParameter (id_y);
  params->AddParameter (id_button);
  params->AddParameter (id_entity);

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.mesh.select.action.");
    AddAction (action_setcamera, "SetCamera");
    AddAction (action_setmousebuttons, "SetMouseButtons");
    AddAction (action_setdragplanenormal, "SetDragPlaneNormal");
  }

  // For properties.
  propinfo.SetCount (8);
  AddProperty (propid_global, "global",
  	CEL_DATA_BOOL, false, "Global Selection.", 0);
  AddProperty (propid_follow, "follow",
  	CEL_DATA_BOOL, false, "Follow Mode.", 0);
  AddProperty (propid_followalways, "followalways",
  	CEL_DATA_BOOL, false, "Follow Always Mode.", 0);
  AddProperty (propid_drag, "drag",
  	CEL_DATA_BOOL, false, "Drag Mode.", 0);
  AddProperty (propid_sendmove, "sendmove",
  	CEL_DATA_BOOL, false, "Send Move Events.", 0);
  AddProperty (propid_sendup, "sendup",
  	CEL_DATA_BOOL, false, "Send Up Events.", 0);
  AddProperty (propid_senddown, "senddown",
  	CEL_DATA_BOOL, false, "Send Down Events.", 0);
  AddProperty (propid_maxdistance, "maxdistance",
  	CEL_DATA_FLOAT, false, "Maximum Selection Distance.", &max_distance);

  SetupEventHandler ();
}

celPcMeshSelect::~celPcMeshSelect ()
{
  if (handler)
    handler->UnregisterMeshSelect (this);
  SetCamera (0);
}

bool celMeshSelectListener::HandleEvent (iEvent& ev)
{
  CS_ASSERT (!is_iterating);
  is_iterating = true;
  celMeshSelectSet::GlobalIterator it = listeners_with_move.GetIterator ();
  while (it.HasNext ())
  {
    celPcMeshSelect* pcmeshsel = it.Next ();
    if (!todo_rem_listeners.Contains (pcmeshsel))
      pcmeshsel->HandleEvent (ev);
  }
  if (ev.Name != csevMouseMove (name_reg, 0))
  {
    celMeshSelectSet::GlobalIterator it = listeners.GetIterator ();
    while (it.HasNext ())
    {
      celPcMeshSelect* pcmeshsel = it.Next ();
      if (!todo_rem_listeners.Contains (pcmeshsel))
        pcmeshsel->HandleEvent (ev);
    }
  }
  CS_ASSERT (is_iterating);
  is_iterating = false;

  it = todo_add_listeners.GetIterator ();
  while (it.HasNext ()) RegisterMeshSelect (it.Next (), false);
  todo_add_listeners.DeleteAll ();

  it = todo_add_listeners_with_move.GetIterator ();
  while (it.HasNext ()) RegisterMeshSelect (it.Next (), true);
  todo_add_listeners_with_move.DeleteAll ();

  it = todo_rem_listeners.GetIterator ();
  while (it.HasNext ()) UnregisterMeshSelect (it.Next ());
  todo_rem_listeners.DeleteAll ();

  return false;
}

void celMeshSelectListener::RegisterMeshSelect (celPcMeshSelect* meshsel,
	bool withmove)
{
  if (withmove)
  {
    if (is_iterating)
    {
      todo_rem_listeners.Delete (meshsel);
      todo_add_listeners.Delete (meshsel);
      todo_add_listeners_with_move.Add (meshsel);
    }
    else
    {
      listeners.Delete (meshsel);
      listeners_with_move.Add (meshsel);
    }
  }
  else
  {
    if (is_iterating)
    {
      todo_rem_listeners.Delete (meshsel);
      todo_add_listeners_with_move.Delete (meshsel);
      todo_add_listeners.Add (meshsel);
    }
    else
    {
      listeners_with_move.Delete (meshsel);
      listeners.Add (meshsel);
    }
  }
}

void celMeshSelectListener::UnregisterMeshSelect (celPcMeshSelect* meshsel)
{
  if (is_iterating)
  {
    todo_add_listeners.Delete (meshsel);
    todo_add_listeners_with_move.Delete (meshsel);
    todo_rem_listeners.Add (meshsel);
  }
  else
  {
    listeners.Delete (meshsel);
    listeners_with_move.Delete (meshsel);
  }
}

void celPcMeshSelect::SetupEventHandler ()
{
  if (!handler)
  {
    handler = csQueryRegistryTagInterface<celMeshSelectListener> (object_reg,
    	"cel.pcobject.mesh.select.globallistener");
    if (!handler)
    {
      handler.AttachNew (new celMeshSelectListener (name_reg));
      object_reg->Register (handler, "cel.pcobject.mesh.select.globallistener");
      csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
      CS_ASSERT (q != 0);

      csEventID esub[] = {
        csevMouseEvent (object_reg),
        //csevMouseDown (object_reg, 0),
        //csevMouseUp (object_reg, 0),
        //csevMouseMove (object_reg, 0),
        CS_EVENTLIST_END
      };
      q->RegisterListener (handler, esub);
    }
  }
  bool do_move = do_drag || do_follow || do_sendmove;
  handler->RegisterMeshSelect (this, do_move);
}

void celPcMeshSelect::AddMeshSelectListener (iPcMeshSelectListener* listener)
{
  listeners.Push (listener);
}

void celPcMeshSelect::RemoveMeshSelectListener (iPcMeshSelectListener* listener)
{
  listeners.Delete (listener);
}

void celPcMeshSelect::FireListenersDown (int x, int y, int button,
	iCelEntity* entity)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->MouseDown ((iPcMeshSelect*)this, x, y, button, entity);
  }
}

void celPcMeshSelect::FireListenersUp (int x, int y, int button,
	iCelEntity* entity)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->MouseUp ((iPcMeshSelect*)this, x, y, button, entity);
  }
}

void celPcMeshSelect::FireListenersMove (int x, int y, int button,
	iCelEntity* entity)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->MouseMove ((iPcMeshSelect*)this, x, y, button, entity);
  }
}

void celPcMeshSelect::SendMessage (int t, iCelEntity* ent,
	int x, int y, int but)
{
  // Prevent this entity from being deleted during message
  // handling.
  csRef<iCelEntity> keepme = entity;

  iMessageDispatcher* dispatcher = 0;
  const char* msg = "pcmeshsel_invalid";
  switch (t)
  {
    case MSSM_TYPE_DOWN:
      FireListenersDown (x, y, but, ent);
      msg = "pcmeshsel_down";
      if (!dispatcher_down)
        dispatcher_down = entity->QueryMessageChannel ()
	  ->CreateMessageDispatcher (this, pl->FetchStringID ("cel.mesh.select.down"));
      dispatcher = dispatcher_down;
      break;
    case MSSM_TYPE_UP:
      FireListenersUp (x, y, but, ent);
      msg = "pcmeshsel_up";
      if (!dispatcher_up)
        dispatcher_up = entity->QueryMessageChannel ()
	  ->CreateMessageDispatcher (this, pl->FetchStringID ("cel.mesh.select.up"));
      dispatcher = dispatcher_up;
      break;
    case MSSM_TYPE_MOVE:
      FireListenersUp (x, y, but, ent);
      msg = "pcmeshsel_move";
      if (!dispatcher_move)
        dispatcher_move = entity->QueryMessageChannel ()
	  ->CreateMessageDispatcher (this, pl->FetchStringID ("cel.mesh.select.move"));
      dispatcher = dispatcher_move;
      break;
  }

  params->GetParameter (0).Set ((int32)x);
  params->GetParameter (1).Set ((int32)y);
  params->GetParameter (2).Set ((int32)but);
  params->GetParameter (3).Set (ent);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage (msg, this, ret, params);
  }
  if (dispatcher)
    dispatcher->SendMessage (params);
}

void celPcMeshSelect::TryGetCamera ()
{
  if (pccamera) return;
  if (camera_entity.IsEmpty ()) return;
  iCelEntity* ent = pl->FindEntity (camera_entity);
  if (!ent) return;
  pccamera = celQueryPropertyClassEntity<iPcCamera> (ent);
}

bool celPcMeshSelect::HandleEvent (iEvent& ev)
{
  TryGetCamera ();
  if (!pccamera) return false;
  iCamera* camera = pccamera->GetCamera ();

  int mouse_but;
  if (ev.Name == csevMouseMove (name_reg, 0))
  {
    if (mousedrv->GetLastButton (csmbLeft)) mouse_but = 1;
    else if (mousedrv->GetLastButton (csmbRight)) mouse_but = 2;
    else if (mousedrv->GetLastButton (csmbMiddle)) mouse_but = 3;
    else mouse_but = 0;
  }
  else
  {
    mouse_but = csMouseEventHelper::GetButton(&ev);
    mouse_but++;	// CS uses 0,1,2.
  }

  int but = 1<<(mouse_but-1);
  if (do_follow || do_drag)
  {
    if (mouse_but != 0 && !(mouse_buttons & but)) return false;
  }
  else
  {
    if (!(mouse_buttons & but)) return false;
  }

  bool mouse_down = ev.Name == csevMouseDown (name_reg, 0);
  bool mouse_up = ev.Name == csevMouseUp (name_reg, 0);
  int mouse_x = csMouseEventHelper::GetX(&ev);
  int mouse_y = csMouseEventHelper::GetY(&ev);

  iCelEntity* new_sel = 0;

  csVector3 dragoffs (0);

  if (mouse_down || do_follow_always || ((do_follow || do_drag) && sel_entity))
  {
    if (camera->GetSector ())
    {
      csScreenTargetResult result = csEngineTools::FindScreenTarget (
      	csVector2 (mouse_x, mouse_y), max_distance, camera);
      if (result.mesh)
      {
        iObject* sel_obj = result.mesh->QueryObject ();
        new_sel = pl->FindAttachedEntity (sel_obj);
        if (new_sel)
        {
          dragoffs = result.isect - result.mesh->GetMovable ()
          	->GetFullPosition ();
        }
      }
    }
  }

  if (do_drag && sel_entity)
  {
    csRef<iPcMovable> pcmovable = celQueryPropertyClassEntity<iPcMovable> (sel_entity);
    csRef<iPcMesh> pcmesh;
    if (pcmovable)
      pcmesh = pcmovable->GetMesh ();
    else
      pcmesh = celQueryPropertyClassEntity<iPcMesh> (sel_entity);
    CS_ASSERT (pcmesh != 0);
    iMeshWrapper* mesh = pcmesh->GetMesh ();
    CS_ASSERT (mesh != 0);

    // We get the position of the point on the mesh where we clicked
    // in world space (that's why drag_offset is added).
    csVector3 mp = mesh->GetMovable ()->GetPosition ();
    mp += drag_offset;

    csVector3 v0, v1;
    float shy;
    csRef<iPerspectiveCamera> pcamera =
      scfQueryInterface<iPerspectiveCamera> (camera);
    if (pcamera)
      shy = pcamera->GetShiftY ();
    else
      shy = 0.0f;
    csVector2 p (mouse_x, shy * 2 - mouse_y);
    // Vector from (0,0,0) to 'vc' in camera space corresponding to
    // the point we clicked on.
    csVector3 vc = camera->InvPerspective (p, 1);

    if (drag_normal_camera)
    {
      v0.Set (0.0f);
      v1 = vc;
      mp = camera->GetTransform ().Other2This (mp);
    }
    else
    {
      // Vector from 'v0' to 'v1' in world space corresponding to
      // same vector.
      v0 = camera->GetTransform ().GetO2TTranslation ();
      v1 = camera->GetTransform ().This2Other (vc);
    }
    csVector3 isect;
    float dist;
    if (csIntersect3::SegmentPlane (v0, v1, drag_normal, mp, isect, dist))
    {
      if (drag_normal_camera)
        isect = camera->GetTransform ().This2Other (isect);
      if (pcmovable)
      {
        pcmovable->Move (isect-mp-drag_offset);
      }
      else
      {
        iSector* sector = mesh->GetMovable ()->GetSectors ()->Get (0);
        // Subtract the drag offset again.
        pcmesh->MoveMesh (sector, isect-drag_offset);
      }
    }
  }

  if (do_follow)
  {
    if (mouse_down)
    {
      if (do_global || new_sel == entity)
      {
        if (sel_entity != new_sel)
          drag_offset = dragoffs;
        sel_entity = new_sel;
      }
      if (do_senddown && sel_entity)
        SendMessage (MSSM_TYPE_DOWN, sel_entity,
        	mouse_x, mouse_y, mouse_but);
      if (sel_entity)
        cur_on_top = true;
      else
        cur_on_top = false;
    }
    else if (sel_entity) // For mouse_up and move.
    {
      bool old_cur_on_top = cur_on_top;
      cur_on_top = (new_sel == sel_entity);
      if (do_senddown && cur_on_top && (cur_on_top != old_cur_on_top))
        SendMessage (MSSM_TYPE_DOWN, sel_entity, mouse_x, mouse_y, mouse_but);
      else if (do_sendup && ((mouse_up && cur_on_top) ||
           	(!cur_on_top && (cur_on_top != old_cur_on_top))))
        SendMessage (MSSM_TYPE_UP, sel_entity, mouse_x, mouse_y, mouse_but);
      else if (do_sendmove)
        SendMessage (MSSM_TYPE_MOVE, sel_entity, mouse_x, mouse_y, mouse_but);
      if (mouse_up) sel_entity = 0;
    }
    else if (do_follow_always && do_sendmove && new_sel)
      SendMessage (MSSM_TYPE_MOVE, new_sel, mouse_x, mouse_y, mouse_but);
  }
  else
  {
    if (mouse_down)
    {
      if (do_global || new_sel == entity)
      {
        if (sel_entity != new_sel)
        {
          drag_offset = dragoffs;
        }
        sel_entity = new_sel;
      }
      if (do_senddown && sel_entity)
        SendMessage (MSSM_TYPE_DOWN, sel_entity, mouse_x, mouse_y, mouse_but);
    }
    else if (mouse_up)
    {
      if (do_sendup && sel_entity)
        SendMessage (MSSM_TYPE_UP, sel_entity, mouse_x, mouse_y, mouse_but);
      sel_entity = 0;
    }
    else
    {
      if (do_sendmove)
      {
        if (sel_entity)
          SendMessage (MSSM_TYPE_MOVE, sel_entity,
          	mouse_x, mouse_y, mouse_but);
        else if (new_sel)
          SendMessage (MSSM_TYPE_MOVE, new_sel, mouse_x, mouse_y, mouse_but);
      }
    }
  }

  return false;
}

void celPcMeshSelect::SetCamera (iPcCamera* pccamera)
{
  celPcMeshSelect::pccamera = pccamera;
}

bool celPcMeshSelect::SetPropertyIndexed (int idx, bool b)
{
  switch (idx)
  {
    case propid_global:
      SetGlobalSelection (b);
      return true;
    case propid_follow:
      SetFollowMode (b);
      return true;
    case propid_followalways:
      SetFollowAlwaysMode (b);
      return true;
    case propid_drag:
      SetDragMode (b);
      return true;
    case propid_sendmove:
      SetSendmoveEvent (b);
      return true;
    case propid_sendup:
      SetSendupEvent (b);
      return true;
    case propid_senddown:
      SetSenddownEvent (b);
      return true;
    default:
      return false;
  }
}

bool celPcMeshSelect::GetPropertyIndexed (int idx, bool& b)
{
  switch (idx)
  {
    case propid_global:
      b = HasGlobalSelection ();
      return true;
    case propid_follow:
      b = HasFollowMode ();
      return true;
    case propid_followalways:
      b = HasFollowAlwaysMode ();
      return true;
    case propid_drag:
      b = HasDragMode ();
      return true;
    case propid_sendmove:
      b = HasSendmoveEvent ();
      return true;
    case propid_sendup:
      b = HasSendupEvent ();
      return true;
    case propid_senddown:
      b = HasSenddownEvent ();
      return true;
    default:
      return false;
  }
}

void celPcMeshSelect::SetMouseButtons (const char* buttons)
{
  int but = 0;
  const char* b = buttons;
  while (*b)
  {
    if (*b == 'l' || *b == 'L') but |= CEL_MOUSE_BUTTON1;
    else if (*b == 'r' || *b == 'R') but |= CEL_MOUSE_BUTTON2;
    else if (*b == 'm' || *b == 'M') but |= CEL_MOUSE_BUTTON3;
    b++;
  }
  SetMouseButtons (but);
}

bool celPcMeshSelect::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setcamera:
      {
	pccamera = 0;
	csString entity;
	if (!Fetch (entity, params, id_entity)) return false;
        iCelEntity* ent = pl->FindEntity (entity);
        if (!ent)
	{
	  // We'll try to get it later.
	  camera_entity = entity;
	  return true;
	}
        csRef<iPcCamera> pccam = celQueryPropertyClassEntity<iPcCamera> (ent);
        if (!pccam)
          return Error (
          	"Entity '%s' doesn't have a camera (action SetCamera)!",
          	entity.GetData ());
        SetCamera (pccam);
        return true;
      }
    case action_setmousebuttons:
      {
	csString buttons_str;
	if (!Fetch (buttons_str, params, id_buttons, true, "")) return false;
        if (!buttons_str.IsEmpty ())
        {
          SetMouseButtons (buttons_str);
        }
        else
        {
	  long buttons;
	  if (!Fetch (buttons, params, id_buttons)) return false;
          SetMouseButtons (buttons);
        }
        return true;
      }
    case action_setdragplanenormal:
      {
	bool camera;
	if (!Fetch (camera, params, id_camera)) return false;
	csVector3 normal;
	if (!Fetch (normal, params, id_normal)) return false;
        SetDragPlaneNormal (normal, camera);
        return true;
      }
    default:
      return false;
  }
}

bool celPcMesh::SetShaderVarExpr (CS::ShaderVarStringID name, const char* exprname)
{
  if (mesh)
  {
    csRef<iShaderManager> shmgr = csQueryRegistry<iShaderManager> (
    	object_reg);
    iShaderVariableAccessor* acc = shmgr->GetShaderVariableAccessor (
    	exprname);
    if (!acc) return Error ("Can't find shader expression '%s'!", exprname);

    iShaderVariableContext* svc = mesh->GetSVContext ();
    csShaderVariable *var = svc->GetVariableAdd (name);
    var->SetAccessor (acc);
  }
  return true;
}

void celPcMesh::SetShaderVar (CS::ShaderVarStringID name, float value)
{
  if (mesh)
  {
    iShaderVariableContext* svc = mesh->GetSVContext ();
    csShaderVariable *var = svc->GetVariableAdd (name);
    var->SetValue (value);
  }
}

void celPcMesh::SetShaderVar (CS::ShaderVarStringID name, int value)
{
  if (mesh)
  {
    iShaderVariableContext* svc = mesh->GetSVContext ();
    csShaderVariable *var = svc->GetVariableAdd (name);
    var->SetValue (value);
  }
}

void celPcMesh::SetShaderVar (CS::ShaderVarStringID name, csVector2 value)
{
  if (mesh)
  {
    iShaderVariableContext* svc = mesh->GetSVContext ();
    csShaderVariable *var = svc->GetVariableAdd (name);
    var->SetValue (value);
  }
}

void celPcMesh::SetShaderVar (CS::ShaderVarStringID name, csVector3 value)
{
  if (mesh)
  {
    iShaderVariableContext* svc = mesh->GetSVContext ();
    csShaderVariable *var = svc->GetVariableAdd (name);
    var->SetValue (value);
  }
}

void celPcMesh::SetShaderVar (CS::ShaderVarStringID name, csVector4 value)
{
  if (mesh)
  {
    iShaderVariableContext* svc = mesh->GetSVContext ();
    csShaderVariable *var = svc->GetVariableAdd (name);
    var->SetValue (value);
  }
}

//---------------------------------------------------------------------------

}
CS_PLUGIN_NAMESPACE_END(pfMesh)
