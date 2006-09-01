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
#include "csutil/debug.h"
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
#include "imap/loader.h"
#include "ivaria/reporter.h"
#include "imesh/object.h"
#include "imesh/sprite3d.h"
#include "imesh/gmeshskel2.h"
#include "imesh/spritecal3d.h"
#include "imesh/skeleton.h"
#include <csgfx/shadervar.h>
#include <csgfx/shadervarcontext.h>
#include "cstool/enginetools.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Mesh, "pcmesh")
CEL_IMPLEMENT_FACTORY (MeshSelect, "pcmeshselect")

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.propclass.mesh",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
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

PropertyHolder celPcMesh::propinfo;

celPcMesh::celPcMesh (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  visible = true;
  factory_ptr = 0;
  creation_flag = CEL_CREATE_NONE;
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);

  if (id_name == csInvalidStringID)
  {
    id_name = pl->FetchStringID ("cel.parameter.name");
    id_path = pl->FetchStringID ("cel.parameter.path");
    id_filename = pl->FetchStringID ("cel.parameter.filename");
    id_factoryname = pl->FetchStringID ("cel.parameter.factoryname");
    id_sector = pl->FetchStringID ("cel.parameter.sector");
    id_position = pl->FetchStringID ("cel.parameter.position");
    id_rotation = pl->FetchStringID ("cel.parameter.rotation");
    id_forward = pl->FetchStringID ("cel.parameter.forward");
    id_up = pl->FetchStringID ("cel.parameter.up");
    id_visible = pl->FetchStringID ("cel.parameter.visible");
    id_material = pl->FetchStringID ("cel.parameter.material");

    id_value = pl->FetchStringID ("cel.parameter.value");
    id_type = pl->FetchStringID ("cel.parameter.type");
    id_animation = pl->FetchStringID ("cel.parameter.animation");
    id_cycle = pl->FetchStringID ("cel.parameter.cycle");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_setmesh, "cel.action.SetMesh");
    AddAction (action_loadmesh, "cel.action.LoadMesh");
    AddAction (action_loadmeshpath, "cel.action.LoadMeshPath");
    AddAction (action_movemesh, "cel.action.MoveMesh");
    AddAction (action_rotatemesh, "cel.action.RotateMesh");
    AddAction (action_clearrotation, "cel.action.ClearRotation");
    AddAction (action_lookat, "cel.action.LookAt");
    AddAction (action_setvisible, "cel.action.SetVisible");
    AddAction (action_setmaterial, "cel.action.SetMaterial");
    AddAction (action_setshadervar, "cel.action.SetShaderVar");
    AddAction (action_setanimation, "cel.action.SetAnimation");
  }
 
  // For properties.
  propinfo.SetCount (6);
  AddProperty (propid_position, "cel.property.position",
	CEL_DATA_VECTOR3, true, "Current position of mesh.", 0);
  AddProperty (propid_fullposition, "cel.property.fullposition",
	CEL_DATA_VECTOR3, true, "Current full position of mesh.", 0);
  AddProperty (propid_sector, "cel.property.sector",
	CEL_DATA_STRING, true, "Current sector of mesh.", 0);
  AddProperty (propid_path, "cel.property.path",
	CEL_DATA_STRING, true, "VFS path for model.", 0);
  AddProperty (propid_factory, "cel.property.factory",
	CEL_DATA_STRING, true, "Factory name for the model.", 0);
  AddProperty (propid_filename, "cel.property.filename",
	CEL_DATA_STRING, true, "Filename for the model.", 0);
}

celPcMesh::~celPcMesh ()
{
  Clear ();
  delete [] propdata;
}

bool celPcMesh::GetPropertyIndexed (int idx, csVector3& v)
{
  switch (idx)
  {
    case propid_position:
      if (mesh)
        v = mesh->GetMovable ()->GetTransform ().GetOrigin ();
      else
        v.Set (0, 0, 0);
      return true;
    case propid_fullposition:
      if (mesh)
        v = mesh->GetMovable ()->GetFullPosition ();
      else
        v.Set (0, 0, 0);
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
      s = 0;
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
    default:
      return false;
  }
}

void celPcMesh::Clear ()
{
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
    if (pl)
      pl->UnattachEntity (mesh->QueryObject (), entity);
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
        CEL_FETCH_STRING_PAR (name,params,id_name);
        if (!name)
          return Report (object_reg,
      	    "Missing parameter 'name' for action SetMesh!");
        iMeshWrapper* m = engine->FindMeshObject (name);
        if (!m)
          return Report (object_reg, "Can't find mesh '%s' for action SetMesh!",
      	    name);
        SetMesh (m, false);
	return true;
      }
    case action_setvisible:
      {
        CEL_FETCH_BOOL_PAR (visible,params,id_visible);
        if (!p_visible) visible = true;
        if (mesh)
        {
          if (visible)
            mesh->GetFlags ().Reset (CS_ENTITY_INVISIBLE);
          else
            mesh->GetFlags ().Set (CS_ENTITY_INVISIBLE);
        }
	return true;
      }
    case action_setmaterial:
      {
        CEL_FETCH_STRING_PAR (material,params,id_material);
        if (!p_material)
          return Report (object_reg, "'material' parameter missing for SetMaterial!");
        iMaterialWrapper* mat = engine->FindMaterial (material);
        if (!mat)
          return Report (object_reg, "Can't find material '%s' for SetMaterial!",
      	    material);
        if (mesh)
        {
          mesh->GetMeshObject ()->SetMaterialWrapper (mat);
        }
	return true;
      }
    case action_loadmesh:
      {
        CEL_FETCH_STRING_PAR (file,params,id_filename);
        CEL_FETCH_STRING_PAR (factory,params,id_factoryname);
        if (!factory)
          return Report (object_reg,
      	    "Missing parameter 'factoryname' for action LoadMesh!");
        bool rc = SetMesh (factory, file);
        if (!rc)
          return Report (object_reg, "Can't load mesh '%s/%s' for action LoadMesh!",
      	    factory, file);
	return true;
      }
    case action_loadmeshpath:
      {
        CEL_FETCH_STRING_PAR (pa,params,id_path);
        if (!pa)
          return Report (object_reg,
      	    "Missing parameter 'path' for action LoadMeshPath!");
        CEL_FETCH_STRING_PAR (file,params,id_filename);
        if (!file)
          return Report (object_reg,
      	    "Missing parameter 'filename' for action LoadMeshPath!");
        CEL_FETCH_STRING_PAR (factory,params,id_factoryname);
        if (!factory)
          return Report (object_reg,
      	    "Missing parameter 'factoryname' for action LoadMeshPath!");
        SetPath (pa);
        bool rc = SetMesh (factory, file);
        if (!rc)
          return Report (object_reg,
      	    "Can't load mesh '%s/%s' (path '%s') for action LoadMeshPath!",
      	    (const char*)factory, (const char*)file, (const char*)path);
	return true;
      }
    case action_movemesh:
      {
        CEL_FETCH_STRING_PAR (sector,params,id_sector);
        if (!sector)
          return Report (object_reg,
      	    "Missing parameter 'sector' for action MoveMesh!");
        CEL_FETCH_VECTOR3_PAR (position,params,id_position);
        if (!p_position)
          return Report (object_reg,
      	    "Missing parameter 'position' for action MoveMesh!");
        if (*sector == 0)
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
            return Report (object_reg, "Can't find sector '%s' for action MoveMesh!",
      	      sector);
          MoveMesh (sect, position);
        }
        CEL_FETCH_VECTOR3_PAR (rotation,params,id_rotation);
        if (p_rotation && mesh)
        {
          csQuaternion quat;
          quat.SetEulerAngles(rotation);
          mesh->GetMovable()->SetTransform(quat.GetMatrix());
          mesh->GetMovable()->UpdateMove();
        }
	return true;
      }
    case action_rotatemesh:
      {
        CEL_FETCH_VECTOR3_PAR (rotation,params,id_rotation);
        if (!p_rotation)
          return Report (object_reg,
      	    "Missing parameter 'rotation' for action RotateMesh!");
        else if (mesh)
        {
          iMovable* mov = mesh->GetMovable ();
          csQuaternion quat;
          quat.SetEulerAngles(rotation);
          csReversibleTransform tr (quat.GetMatrix (), csVector3 (0));
          mov->SetTransform(tr * mov->GetTransform ());
          mov->UpdateMove();
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
        CEL_FETCH_VECTOR3_PAR (forward,params,id_forward);
        if (!p_forward) forward.Set (0, 0, 1);
        CEL_FETCH_VECTOR3_PAR (up,params,id_up);
        if (!p_up) up.Set (0, 1, 0);
        if (mesh)
        {
          mesh->GetMovable ()->GetTransform ().LookAt (forward, up);
          mesh->GetMovable ()->UpdateMove ();
        }
	return true;
      }
    case action_setshadervar:
      {
        CEL_FETCH_STRING_PAR (par_name,params,id_name);
        if (!p_par_name) return false;
        CEL_FETCH_STRING_PAR (par_type,params,id_type);
        if (!p_par_type) return false;
        csRef<iStringSet> strset = CS_QUERY_REGISTRY_TAG_INTERFACE(object_reg,"crystalspace.shared.stringset", iStringSet);
        if (!strcmp(par_type,"float"))
        {
          CEL_FETCH_FLOAT_PAR (par_value,params,id_value);
          if (!p_par_value) return false;
          SetShaderVar(strset->Request(par_name),par_value);
        }
        else if (!strcmp(par_type,"long"))
        {
          CEL_FETCH_LONG_PAR (par_value,params,id_value);
          if (!p_par_value) return false;
          SetShaderVar(strset->Request(par_name),(int)par_value);
        }
        else if (!strcmp(par_type,"vector3"))
        {
          CEL_FETCH_VECTOR3_PAR (par_value,params,id_value);
          if (!p_par_value) return false;
          SetShaderVar(strset->Request(par_name),par_value);
        }
        /*else if (!strcmp(par_type,"vector2"))
        {
          CEL_FETCH_VECTOR2_PAR (par_value,params,id_value);
          if (!p_par_value) return false;
          SetShaderVar(strset->Request(par_name),par_value);
        }*/
        else
            return Report (object_reg,
      	      "Unsupported type %s for action SetShaderVar!",par_type);
	return true;
      }
    case action_setanimation:
      {
        CEL_FETCH_STRING_PAR (par_animation,params,id_animation);
        if (!p_par_animation) return false;
        CEL_FETCH_BOOL_PAR (par_cycle,params,id_cycle);
        if (!p_par_cycle) par_cycle = false;
        SetAnimation(par_animation,par_cycle);
	return true;
      }
    default:
      return false;
  }
}

#define MESH_SERIAL 2

csPtr<iCelDataBuffer> celPcMesh::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MESH_SERIAL);
  int i;
  databuf->Add ((uint8)creation_flag);
  if (creation_flag == CEL_CREATE_FACTORY)
  {
    databuf->Add (factName);
    databuf->Add (fileName);
    databuf->Add (path);
  }
  else if (creation_flag == CEL_CREATE_MESH
  	|| creation_flag == CEL_CREATE_MESHREMOVE)
  {
    /// @@@ Note: this requires a unique name for the mesh!
    databuf->Add (mesh->QueryObject ()->GetName ());
  }
  else if (creation_flag == CEL_CREATE_THING)
  {
    // @@@ Loading or saving meshes with this creation option is
    // not going to work properly as we can't easily save the thing data itself.
    // Perhaps we should consider combining this with the thing saver somehow.
  }

  databuf->Add (visible);

  if (mesh)
  {
    databuf->Add (GetAction ());
    iMovable* mov = mesh->GetMovable ();
    iSectorList* sl = mov->GetSectors ();
    databuf->Add ((uint16)(sl->GetCount ()));
    for (i = 0 ; i < sl->GetCount () ; i++)
    {
      databuf->Add (sl->Get (i)->QueryObject ()->GetName ());
    }
    csReversibleTransform& tr = mov->GetTransform ();
    databuf->Add (tr.GetO2TTranslation ());
    databuf->Add (tr.GetO2T ().m11);
    databuf->Add (tr.GetO2T ().m12);
    databuf->Add (tr.GetO2T ().m13);
    databuf->Add (tr.GetO2T ().m21);
    databuf->Add (tr.GetO2T ().m22);
    databuf->Add (tr.GetO2T ().m23);
    databuf->Add (tr.GetO2T ().m31);
    databuf->Add (tr.GetO2T ().m32);
    databuf->Add (tr.GetO2T ().m33);
  }

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMesh::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MESH_SERIAL)
    return Report (object_reg, "Serialnr != MESH_SERIAL.  Cannot load.");

  Clear ();
  visible = true;

  creation_flag = (celPcMeshCreationFlag)(databuf->GetUInt8 ());
  if (creation_flag == CEL_CREATE_FACTORY)
  {
    const char* factn = databuf->GetString ()->GetData ();
    const char* filen = databuf->GetString ()->GetData ();
    const char* pathn = databuf->GetString ()->GetData ();
    SetPath (pathn);
    SetMesh (factn, filen);
  }
  else if (creation_flag == CEL_CREATE_MESH ||
  	   creation_flag == CEL_CREATE_MESHREMOVE)
  {
    const char* meshname = databuf->GetString ()->GetData ();
    iMeshWrapper* m = engine->FindMeshObject (meshname);
    if (!m)
      return Report (object_reg, "Can't find mesh '%s' for loading entity!",
      	meshname);
    SetMesh (m, creation_flag == CEL_CREATE_MESHREMOVE);
  }
  else if (creation_flag == CEL_CREATE_THING)
  {
    CreateEmptyThing ();
  }

  if (databuf->GetBool ()) Show ();
  else Hide ();

  if (mesh)
  {
    SetAction (databuf->GetString ()->GetData (), true);
    uint16 cnt = databuf->GetUInt16 ();
    mesh->GetMovable ()->ClearSectors ();
    int i;
    for (i = 0 ; i < cnt ; i++)
    {
      iSector* s = engine->GetSectors ()->FindByName (
    	  databuf->GetString ()->GetData ());
      CS_ASSERT (s != 0);
      mesh->GetMovable ()->GetSectors ()->Add (s);
    }

    csMatrix3 m_o2t;
    csVector3 v_o2t;
    databuf->GetVector3 (v_o2t);
    m_o2t.m11 = databuf->GetFloat ();
    m_o2t.m12 = databuf->GetFloat ();
    m_o2t.m13 = databuf->GetFloat ();
    m_o2t.m21 = databuf->GetFloat ();
    m_o2t.m22 = databuf->GetFloat ();
    m_o2t.m23 = databuf->GetFloat ();
    m_o2t.m31 = databuf->GetFloat ();
    m_o2t.m32 = databuf->GetFloat ();
    m_o2t.m33 = databuf->GetFloat ();
    csReversibleTransform tr (m_o2t, v_o2t);
    mesh->GetMovable ()->SetTransform (tr);
    mesh->GetMovable ()->UpdateMove ();
  }

  return true;
}

iMeshFactoryWrapper* celPcMesh::LoadMeshFactory ()
{
  csRef<iVFS> vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
  if (!path.IsEmpty ())
  {
    // If we have a path then we first ChDir to that.
    vfs->PushDir ();
    vfs->ChDir (path);
  }

  csRef<iLoader> loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  CS_ASSERT (loader != 0);
  iBase* result;
  bool success = loader->Load (fileName, result, 0, false, true);
  if (!path.IsEmpty ())
  {
    vfs->PopDir ();
  }
  if (!success)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pfmesh.loadmeshfactory",
    	"Error loading mesh object factory or library '%s'!",
	(const char*)fileName);
    return 0;
  }

  csRef<iMeshFactoryWrapper> imeshfact;
  if (result == 0)
  {
    // We have a library.
    imeshfact = engine->FindMeshFactory (factName);
  }
  else
  {
    imeshfact = SCF_QUERY_INTERFACE (result, iMeshFactoryWrapper);
    if (!imeshfact)
    {
      // Perhaps it is a world file?
      csRef<iEngine> eng = SCF_QUERY_INTERFACE (result, iEngine);
      if (eng)
      {
        imeshfact = engine->FindMeshFactory (factName);
      }
    }
  }

  if (imeshfact == 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pfmesh.loadmeshfactory",
    	"Error loading mesh object factory '%s'!",
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
    pl->AttachEntity (mesh->QueryObject (), entity);
  FirePropertyChangeCallback (CEL_PCMESH_PROPERTY_MESH);
}

void celPcMesh::CreateEmptyThing ()
{
  RemoveMesh ();
  creation_flag = CEL_CREATE_THING;

  mesh = engine->CreateThingMesh (0, 0);
  pl->AttachEntity (mesh->QueryObject (), entity);
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

void celPcMesh::SetAnimation (const char* actionName, bool cycle,
    float weight,float fadein,float fadeout)
{
  printf("set anim %s\n",actionName);
  if (!actionName) return;
  if (!mesh) return;
  csRef<iSprite3DState> spr3dstate (SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
        iSprite3DState));
  if (spr3dstate)
  {
      spr3dstate->SetAction (actionName,cycle);
  }
  else
  {
    csRef<iSpriteCal3DState> sprcal3dstate (SCF_QUERY_INTERFACE (
	  mesh->GetMeshObject (), iSpriteCal3DState));
    if (sprcal3dstate)
    {
      if (cycle)
          sprcal3dstate->SetAnimCycle (actionName,weight);
      else
          sprcal3dstate->SetAnimAction (actionName,fadein,fadeout);
    }
    else
    {
      csRef<iGeneralMeshState> genstate = SCF_QUERY_INTERFACE (
	  mesh->GetMeshObject (),iGeneralMeshState);
      if (genstate)
      {
        csRef<iGenMeshAnimationControl> skelstate = genstate
	  ->GetAnimationControl ();
        if (skelstate)
        {
          csRef<iGenMeshSkeletonControlState> ctlstate = SCF_QUERY_INTERFACE (
	      skelstate,iGenMeshSkeletonControlState);
          if (ctlstate)
          {
            csRef<iSkeleton> skel = ctlstate->GetSkeleton();
            if (skel)
            {
      		  skel->StopAll();
                  skel->Execute (actionName);
            }
          }
        }
      }
    }
  }
}

void celPcMesh::SetAction (const char* actionName, bool resetaction)
{
  if (!actionName) return;
  if (!mesh) return;
  csRef<iSprite3DState> state (SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
  	iSprite3DState));
  if (state)
  {
    if (resetaction || strcmp (actionName, state->GetCurAction()->GetName ()))
	state->SetAction (actionName);
  }
}

void celPcMesh::SetReverseAction (bool reverse)
{
  if (!mesh) return;
  csRef<iSprite3DState> state (SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
  	iSprite3DState));
  if (state)
  {
      state->SetReverseAction(reverse);
  }
}

const char* celPcMesh::GetAction ()
{
  if (!mesh) return 0;
  csRef<iSprite3DState> state (SCF_QUERY_INTERFACE (mesh->GetMeshObject (),
  	iSprite3DState));
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

  sel_entity = 0;
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

  mousedrv = CS_QUERY_REGISTRY (object_reg, iMouseDriver);
  name_reg = csEventNameRegistry::GetRegistry (object_reg);

  // Initialize the maximum selection distance to a very large number
  max_distance = 100000.0f;

  if (id_x == csInvalidStringID)
  {
    id_x = pl->FetchStringID ("cel.parameter.x");
    id_y = pl->FetchStringID ("cel.parameter.y");
    id_button = pl->FetchStringID ("cel.parameter.button");
    id_entity = pl->FetchStringID ("cel.parameter.entity");

    id_buttons = pl->FetchStringID ("cel.parameter.buttons");
    id_normal = pl->FetchStringID ("cel.parameter.normal");
    id_camera = pl->FetchStringID ("cel.parameter.camera");
  }
  params = new celGenericParameterBlock (4);
  params->SetParameterDef (0, id_x, "x");
  params->SetParameterDef (1, id_y, "y");
  params->SetParameterDef (2, id_button, "button");
  params->SetParameterDef (3, id_entity, "entity");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_setcamera, "cel.action.SetCamera");
    AddAction (action_setmousebuttons, "cel.action.SetMouseButtons");
    AddAction (action_setdragplanenormal, "cel.action.SetDragPlaneNormal");
  }

  // For properties.
  propinfo.SetCount (8);
  AddProperty (propid_global, "cel.property.global",
	CEL_DATA_BOOL, false, "Global Selection.", 0);
  AddProperty (propid_follow, "cel.property.follow",
	CEL_DATA_BOOL, false, "Follow Mode.", 0);
  AddProperty (propid_followalways, "cel.property.followalways",
	CEL_DATA_BOOL, false, "Follow Always Mode.", 0);
  AddProperty (propid_drag, "cel.property.drag",
	CEL_DATA_BOOL, false, "Drag Mode.", 0);
  AddProperty (propid_sendmove, "cel.property.sendmove",
	CEL_DATA_BOOL, false, "Send Move Events.", 0);
  AddProperty (propid_sendup, "cel.property.sendup",
	CEL_DATA_BOOL, false, "Send Up Events.", 0);
  AddProperty (propid_senddown, "cel.property.senddown",
	CEL_DATA_BOOL, false, "Send Down Events.", 0);
  AddProperty (propid_maxdistance, "cel.property.maxdistance",
	CEL_DATA_FLOAT, false, "Maximum Selection Distance.", &max_distance);

  SetupEventHandler ();
  DG_TYPE (this, "celPcMeshSelect()");
}

celPcMeshSelect::~celPcMeshSelect ()
{
  if (handler)
    handler->UnregisterMeshSelect (this);
  SetCamera (0);
  delete params;
}

bool celMeshSelectListener::HandleEvent (iEvent& ev)
{
  CS_ASSERT (!is_iterating);
  is_iterating = true;
  celMeshSelectSet::GlobalIterator it = listeners_with_move.GetIterator ();
  while (it.HasNext ())
  {
    celPcMeshSelect* pcmeshsel = it.Next ();
    pcmeshsel->HandleEvent (ev);
  }
  if (ev.Name != csevMouseMove (name_reg, 0))
  {
    celMeshSelectSet::GlobalIterator it = listeners.GetIterator ();
    while (it.HasNext ())
    {
      celPcMeshSelect* pcmeshsel = it.Next ();
      pcmeshsel->HandleEvent (ev);
    }
  }
  CS_ASSERT (is_iterating);
  is_iterating = false;

  it = todo_add_listeners.GetIterator ();
  while (it.HasNext ()) RegisterMeshSelect (it.Next(), false);
  todo_add_listeners.DeleteAll ();

  it = todo_add_listeners_with_move.GetIterator ();
  while (it.HasNext ()) RegisterMeshSelect (it.Next(), true);
  todo_add_listeners_with_move.DeleteAll ();

  it = todo_rem_listeners.GetIterator ();
  while (it.HasNext ()) UnregisterMeshSelect (it.Next());
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
	"cel.pcmeshselect.globallistener");
    if (!handler)
    {
      handler.AttachNew (new celMeshSelectListener (name_reg));
      object_reg->Register (handler, "cel.pcmeshselect.globallistener");
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

#define MESHSEL_SERIAL 1

csPtr<iCelDataBuffer> celPcMeshSelect::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (MESHSEL_SERIAL);
  csRef<iCelPropertyClass> pc;
  if (pccamera) pc = SCF_QUERY_INTERFACE (pccamera, iCelPropertyClass);
  databuf->Add (pc);
  databuf->Add (sel_entity);
  databuf->Add (cur_on_top);
  databuf->Add ((uint32)mouse_buttons);
  databuf->Add (do_global);
  databuf->Add (do_drag);
  databuf->Add (drag_normal);
  databuf->Add (drag_normal_camera);
  databuf->Add (do_follow);
  databuf->Add (do_follow_always);
  databuf->Add (do_sendmove);
  databuf->Add (do_sendup);
  databuf->Add (do_senddown);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcMeshSelect::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != MESHSEL_SERIAL)
    return Report (object_reg, "serialnr != MESHSEL_SERIAL.  Cannot load.");

  csRef<iPcCamera> pcm;
  iCelPropertyClass* pc = databuf->GetPC ();
  if (pc)
  {
    pcm = SCF_QUERY_INTERFACE (pc, iPcCamera);
    SetCamera (pcm);
  }

  sel_entity = databuf->GetEntity ();
  cur_on_top = databuf->GetBool ();
  mouse_buttons = databuf->GetUInt32 ();
  do_global = databuf->GetBool ();
  do_drag = databuf->GetBool ();
  databuf->GetVector3 (drag_normal);
  drag_normal_camera = databuf->GetBool ();
  do_follow = databuf->GetBool ();
  do_follow_always = databuf->GetBool ();
  do_sendmove = databuf->GetBool ();
  do_sendup = databuf->GetBool ();
  do_senddown = databuf->GetBool ();

  SetupEventHandler ();

  return true;
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
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->MouseDown ((iPcMeshSelect*)this, x, y, button, entity);
  }
}

void celPcMeshSelect::FireListenersUp (int x, int y, int button,
	iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->MouseUp ((iPcMeshSelect*)this, x, y, button, entity);
  }
}

void celPcMeshSelect::FireListenersMove (int x, int y, int button,
	iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->MouseMove ((iPcMeshSelect*)this, x, y, button, entity);
  }
}

void celPcMeshSelect::SendMessage (int t, iCelEntity* ent,
	int x, int y, int but)
{
  const char* msg = "pcmeshsel_invalid";
  switch (t)
  {
    case MSSM_TYPE_DOWN:
      FireListenersDown (x, y, but, ent);
      msg = "pcmeshsel_down";
      break;
    case MSSM_TYPE_UP:
      FireListenersUp (x, y, but, ent);
      msg = "pcmeshsel_up";
      break;
    case MSSM_TYPE_MOVE:
      FireListenersUp (x, y, but, ent);
      msg = "pcmeshsel_move";
      break;
  }

  iCelBehaviour* bh = entity->GetBehaviour ();
  if (!bh) return;
  params->GetParameter (0).Set ((int32)x);
  params->GetParameter (1).Set ((int32)y);
  params->GetParameter (2).Set ((int32)but);
  params->GetParameter (3).Set (ent);
  celData ret;
  bh->SendMessage (msg, this, ret, params);
}


bool celPcMeshSelect::HandleEvent (iEvent& ev)
{
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
    csRef<iPcMovable> pcmovable (CEL_QUERY_PROPCLASS (
    	sel_entity->GetPropertyClassList (), iPcMovable));
    csRef<iPcMesh> pcmesh;
    if (pcmovable)
      pcmesh = pcmovable->GetMesh ();
    else
      pcmesh = CEL_QUERY_PROPCLASS (
      	sel_entity->GetPropertyClassList (), iPcMesh);
    CS_ASSERT (pcmesh != 0);
    iMeshWrapper* mesh = pcmesh->GetMesh ();
    CS_ASSERT (mesh != 0);

    // We get the position of the point on the mesh where we clicked
    // in world space (that's why drag_offset is added).
    csVector3 mp = mesh->GetMovable ()->GetPosition ();
    mp += drag_offset;

    csVector3 v0, v1;

    csVector2 p (mouse_x, camera->GetShiftY() * 2 - mouse_y);
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
      if (sel_entity) cur_on_top = true;
      else cur_on_top = false;
    }
    else if (sel_entity) // For mouse_up and move.
    {
      bool old_cur_on_top = cur_on_top;
      cur_on_top = (new_sel == sel_entity);
      if (do_senddown && cur_on_top && (cur_on_top != old_cur_on_top))
        SendMessage (MSSM_TYPE_DOWN, sel_entity,
		mouse_x, mouse_y, mouse_but);
      else if (do_sendup && ((mouse_up && cur_on_top) ||
      		!cur_on_top && (cur_on_top != old_cur_on_top)))
        SendMessage (MSSM_TYPE_UP, sel_entity,
		mouse_x, mouse_y, mouse_but);
      else if (do_sendmove)
        SendMessage (MSSM_TYPE_MOVE, sel_entity,
		mouse_x, mouse_y, mouse_but);
      if (mouse_up) sel_entity = 0;
    }
    else if (do_follow_always && do_sendmove && new_sel)
      SendMessage (MSSM_TYPE_MOVE, new_sel,
		mouse_x, mouse_y, mouse_but);
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
        SendMessage (MSSM_TYPE_DOWN, sel_entity,
		mouse_x, mouse_y, mouse_but);
    }
    else if (mouse_up)
    {
      if (do_sendup && sel_entity)
        SendMessage (MSSM_TYPE_UP, sel_entity,
		mouse_x, mouse_y, mouse_but);
      sel_entity = 0;
    }
    else
    {
      if (do_sendmove)
	if (sel_entity)
          SendMessage (MSSM_TYPE_MOVE, sel_entity,
		mouse_x, mouse_y, mouse_but);
        else if (new_sel)
          SendMessage (MSSM_TYPE_MOVE, new_sel,
		mouse_x, mouse_y, mouse_but);
    }
  }

  return false;
}

void celPcMeshSelect::SetCamera (iPcCamera* pccamera)
{
#if defined (CS_DEBUG) && defined (CS_USE_GRAPHDEBUG)
  if (celPcMeshSelect::pccamera)
  {
    csRef<iCelPropertyClass> pc (
       	SCF_QUERY_INTERFACE (celPcMeshSelect::pccamera,
  	  iCelPropertyClass));
    DG_UNLINK (this, pc);
  }
#endif
  celPcMeshSelect::pccamera = pccamera;
#if defined (CS_DEBUG) && defined (CS_USE_GRAPHDEBUG)
  if (pccamera)
  {
    csRef<iCelPropertyClass> pc2 (SCF_QUERY_INTERFACE (pccamera,
      	  iCelPropertyClass));
    DG_LINK (this, pc2);
  }
#endif
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
        CEL_FETCH_STRING_PAR (entity,params,id_entity);
        if (!entity)
          return Report (object_reg,
      	    "Missing parameter 'entity' for action SetCamera!");
        iCelEntity* ent = pl->FindEntity (entity);
        if (!ent)
          return Report (object_reg, "Can't find entity '%s' for action SetCamera!",
      	    entity);
        csRef<iPcCamera> pccam = CEL_QUERY_PROPCLASS_ENT (ent, iPcCamera);
        if (!pccam)
          return Report (object_reg,
      	    "Entity '%s' doesn't have a camera (action SetCamera)!", entity);
        SetCamera (pccam);
        return true;
      }
    case action_setmousebuttons:
      {
        CEL_FETCH_STRING_PAR (buttons_str,params,id_buttons);
        if (p_buttons_str)
        {
          SetMouseButtons (buttons_str);
        }
        else
        {
          CEL_FETCH_LONG_PAR (buttons,params,id_buttons);
          if (!p_buttons)
            return Report (object_reg,
      	      "Missing parameter 'buttons' for action SetMouseButtons!");
          SetMouseButtons (buttons);
        }
        return true;
      }
    case action_setdragplanenormal:
      {
        CEL_FETCH_BOOL_PAR (camera,params,id_camera);
        if (!p_camera)
          return Report (object_reg,
      	    "Missing parameter 'camera' for action SetDragPlaneNormal!");
        CEL_FETCH_VECTOR3_PAR (normal,params,id_normal);
        if (!p_normal)
          return Report (object_reg,
      	    "Missing parameter 'normal' for action SetDragPlaneNormal!");
        SetDragPlaneNormal (normal, camera);
        return true;
      }
    default:
      return false;
  }
}

void celPcMesh::SetShaderVar(csStringID name, float value)
{
  if (mesh)
  {
    iShaderVariableContext* svc = mesh->GetSVContext();
    csShaderVariable *var = svc->GetVariableAdd(name);
    var->SetValue(value);
    svc->AddVariable(var);
  }
}

void celPcMesh::SetShaderVar(csStringID name, int value)
{
  if (mesh)
  {
    iShaderVariableContext* svc = mesh->GetSVContext();
    csShaderVariable *var = svc->GetVariableAdd(name);
    var->SetValue(value);
    svc->AddVariable(var);
  }
}

void celPcMesh::SetShaderVar(csStringID name, csVector3 value)
{
  if (mesh)
  {
    iShaderVariableContext* svc = mesh->GetSVContext();
    csShaderVariable *var = svc->GetVariableAdd(name);
    var->SetValue(value);
    svc->AddVariable(var);
  }
}

void celPcMesh::SetShaderVar(csStringID name, csVector2 value)
{
  if (mesh)
  {
    iShaderVariableContext* svc = mesh->GetSVContext();
    csShaderVariable *var = svc->GetVariableAdd(name);
    var->SetValue(value);
    svc->AddVariable(var);
  }
}

//---------------------------------------------------------------------------

