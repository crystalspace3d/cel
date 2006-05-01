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

csStringID celPcMesh::action_setmesh = csInvalidStringID;
csStringID celPcMesh::id_name = csInvalidStringID;
csStringID celPcMesh::action_loadmesh = csInvalidStringID;
csStringID celPcMesh::action_loadmeshpath = csInvalidStringID;
csStringID celPcMesh::id_path = csInvalidStringID;
csStringID celPcMesh::id_filename = csInvalidStringID;
csStringID celPcMesh::id_factoryname = csInvalidStringID;
csStringID celPcMesh::action_movemesh = csInvalidStringID;
csStringID celPcMesh::id_sector = csInvalidStringID;
csStringID celPcMesh::id_position = csInvalidStringID;
csStringID celPcMesh::action_clearrotation = csInvalidStringID;
csStringID celPcMesh::action_lookat = csInvalidStringID;
csStringID celPcMesh::id_forward = csInvalidStringID;
csStringID celPcMesh::id_up = csInvalidStringID;
csStringID celPcMesh::action_setvisible = csInvalidStringID;
csStringID celPcMesh::id_visible = csInvalidStringID;
csStringID celPcMesh::action_setmaterial = csInvalidStringID;
csStringID celPcMesh::id_material = csInvalidStringID;

celPcMesh::celPcMesh (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  visible = true;
  factory_ptr = 0;
  creation_flag = CEL_CREATE_NONE;
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);

  if (action_loadmesh == csInvalidStringID)
  {
    action_setmesh = pl->FetchStringID ("cel.action.SetMesh");
    id_name = pl->FetchStringID ("cel.parameter.name");
    action_loadmesh = pl->FetchStringID ("cel.action.LoadMesh");
    action_loadmeshpath = pl->FetchStringID ("cel.action.LoadMeshPath");
    id_path = pl->FetchStringID ("cel.parameter.path");
    id_filename = pl->FetchStringID ("cel.parameter.filename");
    id_factoryname = pl->FetchStringID ("cel.parameter.factoryname");
    action_movemesh = pl->FetchStringID ("cel.action.MoveMesh");
    id_sector = pl->FetchStringID ("cel.parameter.sector");
    id_position = pl->FetchStringID ("cel.parameter.position");
    action_clearrotation = pl->FetchStringID ("cel.action.ClearRotation");
    action_lookat = pl->FetchStringID ("cel.action.LookAt");
    id_forward = pl->FetchStringID ("cel.parameter.forward");
    id_up = pl->FetchStringID ("cel.parameter.up");
    action_setvisible = pl->FetchStringID ("cel.action.SetVisible");
    id_visible = pl->FetchStringID ("cel.parameter.visible");
    action_setmaterial = pl->FetchStringID ("cel.action.SetMaterial");
    id_material = pl->FetchStringID ("cel.parameter.material");
  }

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_position] = 0;		// Handled in this class.
  propdata[propid_sector] = 0;			// Handled in this class.
  propdata[propid_path] = 0;			// Handled in this class.
  propdata[propid_factory] = 0;			// Handled in this class.
  propdata[propid_filename] = 0;		// Handled in this class.
}

celPcMesh::~celPcMesh ()
{
  Clear ();
  delete [] propdata;
}

Property* celPcMesh::properties = 0;
size_t celPcMesh::propertycount = 0;

void celPcMesh::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    propertycount = 5;
    properties = new Property[propertycount];

    properties[propid_position].id = pl->FetchStringID (
    	"cel.property.position");
    properties[propid_position].datatype = CEL_DATA_VECTOR3;
    properties[propid_position].readonly = true;
    properties[propid_position].desc = "Current position of mesh.";

    properties[propid_sector].id = pl->FetchStringID (
    	"cel.property.sector");
    properties[propid_sector].datatype = CEL_DATA_STRING;
    properties[propid_sector].readonly = true;
    properties[propid_sector].desc = "Current sector of mesh.";

    properties[propid_path].id = pl->FetchStringID (
    	"cel.property.path");
    properties[propid_path].datatype = CEL_DATA_STRING;
    properties[propid_path].readonly = true;
    properties[propid_path].desc = "VFS path for model.";

    properties[propid_factory].id = pl->FetchStringID (
    	"cel.property.factory");
    properties[propid_factory].datatype = CEL_DATA_STRING;
    properties[propid_factory].readonly = true;
    properties[propid_factory].desc = "Factory name for the model.";

    properties[propid_filename].id = pl->FetchStringID (
    	"cel.property.filename");
    properties[propid_filename].datatype = CEL_DATA_STRING;
    properties[propid_filename].readonly = true;
    properties[propid_filename].desc = "Filename for the model.";
  }
}

bool celPcMesh::GetPropertyVector (csStringID propertyId, csVector3& v)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_position].id)
  {
    if (mesh)
      v = mesh->GetMovable ()->GetTransform ().GetOrigin ();
    else
      v.Set (0, 0, 0);
    return true;
  }
  else
  {
    return celPcCommon::GetPropertyVector (propertyId, v);
  }
}

const char* celPcMesh::GetPropertyString (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_sector].id)
  {
    if (mesh)
    {
      iSectorList* sl = mesh->GetMovable ()->GetSectors ();
      if (sl->GetCount () > 0)
        return sl->Get (0)->QueryObject ()->GetName ();
      else
        return 0;
    }
    else
      return 0;
  }
  else if (propertyId == properties[propid_path].id)
  {
    return path.GetData ();
  }
  else if (propertyId == properties[propid_factory].id)
  {
    return factName.GetData ();
  }
  else if (propertyId == properties[propid_filename].id)
  {
    return fileName.GetData ();
  }
  else
  {
    return celPcCommon::GetPropertyString (propertyId);
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

bool celPcMesh::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_setmesh)
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
  }
  else if (actionId == action_setvisible)
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
  }
  else if (actionId == action_setmaterial)
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
  }
  else if (actionId == action_loadmesh)
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
  }
  else if (actionId == action_loadmeshpath)
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
  }
  else if (actionId == action_movemesh)
  {
    CEL_FETCH_STRING_PAR (sector,params,id_sector);
    if (!sector)
      return Report (object_reg,
      	"Missing parameter 'sector' for action MoveMesh!");
    CEL_FETCH_VECTOR3_PAR (position,params,id_position);
    if (!p_position)
      return Report (object_reg,
      	"Missing parameter 'position' for action MoveMesh!");
    iSector* sect = engine->FindSector (sector);
    if (!sect)
      return Report (object_reg, "Can't find sector '%s' for action MoveMesh!",
      	sector);
    MoveMesh (sect, position);
  }
  else if (actionId == action_clearrotation)
  {
    if (mesh)
    {
      mesh->GetMovable ()->SetTransform (csMatrix3 ());
      mesh->GetMovable ()->UpdateMove ();
    }
  }
  else if (actionId == action_lookat)
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
  }
  else return false;
  return true;
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

SCF_IMPLEMENT_IBASE (celPcMeshSelect::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

csStringID celPcMeshSelect::action_setcamera = csInvalidStringID;
csStringID celPcMeshSelect::action_setmousebuttons = csInvalidStringID;
csStringID celPcMeshSelect::id_buttons = csInvalidStringID;
csStringID celPcMeshSelect::action_setdragplanenormal = csInvalidStringID;
csStringID celPcMeshSelect::id_normal = csInvalidStringID;
csStringID celPcMeshSelect::id_camera = csInvalidStringID;

csStringID celPcMeshSelect::id_x = csInvalidStringID;
csStringID celPcMeshSelect::id_y = csInvalidStringID;
csStringID celPcMeshSelect::id_button = csInvalidStringID;
csStringID celPcMeshSelect::id_entity = csInvalidStringID;

celPcMeshSelect::celPcMeshSelect (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  scfiEventHandler = 0;
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

    action_setcamera = pl->FetchStringID ("cel.action.SetCamera");
    action_setmousebuttons = pl->FetchStringID ("cel.action.SetMouseButtons");
    id_buttons = pl->FetchStringID ("cel.parameter.buttons");
    action_setdragplanenormal = pl->FetchStringID ("cel.action.SetDragPlaneNormal");
    id_normal = pl->FetchStringID ("cel.parameter.normal");
    id_camera = pl->FetchStringID ("cel.parameter.camera");
  }
  params = new celGenericParameterBlock (4);
  params->SetParameterDef (0, id_x, "x");
  params->SetParameterDef (1, id_y, "y");
  params->SetParameterDef (2, id_button, "button");
  params->SetParameterDef (3, id_entity, "entity");

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_global] = 0;
  propdata[propid_follow] = 0;
  propdata[propid_followalways] = 0;
  propdata[propid_drag] = 0;
  propdata[propid_sendmove] = 0;
  propdata[propid_sendup] = 0;
  propdata[propid_senddown] = 0;
  propdata[propid_maxdistance] = &max_distance;

  SetupEventHandler ();
  DG_TYPE (this, "celPcMeshSelect()");
}

celPcMeshSelect::~celPcMeshSelect ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    if (q)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
  SetCamera (0);
  delete params;
}

Property* celPcMeshSelect::properties = 0;
size_t celPcMeshSelect::propertycount = 0;

void celPcMeshSelect::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT (pl != 0);

    propertycount = 8;
    properties = new Property[propertycount];

    properties[propid_global].id = pl->FetchStringID (
    	"cel.property.global");
    properties[propid_global].datatype = CEL_DATA_BOOL;
    properties[propid_global].readonly = false;
    properties[propid_global].desc = "Global Selection.";

    properties[propid_follow].id = pl->FetchStringID (
    	"cel.property.follow");
    properties[propid_follow].datatype = CEL_DATA_BOOL;
    properties[propid_follow].readonly = false;
    properties[propid_follow].desc = "Follow Mode.";

    properties[propid_followalways].id = pl->FetchStringID (
    	"cel.property.followalways");
    properties[propid_followalways].datatype = CEL_DATA_BOOL;
    properties[propid_followalways].readonly = false;
    properties[propid_followalways].desc = "Follow Always Mode.";

    properties[propid_drag].id = pl->FetchStringID (
    	"cel.property.drag");
    properties[propid_drag].datatype = CEL_DATA_BOOL;
    properties[propid_drag].readonly = false;
    properties[propid_drag].desc = "Drag Mode.";

    properties[propid_sendmove].id = pl->FetchStringID (
    	"cel.property.sendmove");
    properties[propid_sendmove].datatype = CEL_DATA_BOOL;
    properties[propid_sendmove].readonly = false;
    properties[propid_sendmove].desc = "Send Move Events.";

    properties[propid_sendup].id = pl->FetchStringID (
    	"cel.property.sendup");
    properties[propid_sendup].datatype = CEL_DATA_BOOL;
    properties[propid_sendup].readonly = false;
    properties[propid_sendup].desc = "Send Up Events.";

    properties[propid_senddown].id = pl->FetchStringID (
    	"cel.property.senddown");
    properties[propid_senddown].datatype = CEL_DATA_BOOL;
    properties[propid_senddown].readonly = false;
    properties[propid_senddown].desc = "Send Down Events.";

    properties[propid_maxdistance].id = pl->FetchStringID (
    	"cel.property.maxdistance");
    properties[propid_maxdistance].datatype = CEL_DATA_FLOAT;
    properties[propid_maxdistance].readonly = false;
    properties[propid_maxdistance].desc = "Maximum Selection Distance.";
  }
}



void celPcMeshSelect::SetupEventHandler ()
{
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  csEventID esub[] = {
    csevMouseDown (object_reg, 0),
    csevMouseUp (object_reg, 0),
    (do_drag || do_follow || do_sendmove)
    	? csevMouseMove (object_reg, 0)
	: CS_EVENTLIST_END,
    CS_EVENTLIST_END
  };
  q->RegisterListener (scfiEventHandler, esub);
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

  // The following vectors are only set if needed.

  // Vector from (0,0,0) to 'vc' in camera space corresponding to
  // the point we clicked on.
  csVector3 vc;
  // Vector from 'vo' to 'vw' in world space corresponding to
  // same vector.
  csVector3 vo, vw;

  csVector3 dragoffs (0);

  if (mouse_down || do_follow_always || ((do_follow || do_drag) && sel_entity))
  {
    // Setup perspective vertex, invert mouse Y axis.
    csVector2 p (mouse_x, camera->GetShiftY() * 2 - mouse_y);

    vc = camera->InvPerspective (p, 1 );
    vw = camera->GetTransform ().This2Other (vc);

    iSector* sector = camera->GetSector ();
    if (sector)
    {
      vo = camera->GetTransform ().GetO2TTranslation ();
      csVector3 isect, end = vo + (vw - vo) * max_distance;

      iMeshWrapper* sel = sector->HitBeam (vo, end, isect, 0, true);
      if (sel)
      {
        iObject* sel_obj = sel->QueryObject ();
        new_sel = pl->FindAttachedEntity (sel_obj);
	if (new_sel)
	{
	  dragoffs = isect - sel->GetMovable ()->GetFullPosition ();
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
    if (drag_normal_camera)
    {
      v0.Set (0.0f);
      v1 = vc;
      mp = camera->GetTransform ().Other2This (mp);
    }
    else
    {
      v0 = vo;
      v1 = vw;
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
	{
	  drag_offset = dragoffs;
	  printf ("drag_offset=%g,%g,%g\n", drag_offset.x, drag_offset.y, drag_offset.z); fflush (stdout);
	}
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
	  printf ("2:drag_offset=%g,%g,%g\n", drag_offset.x, drag_offset.y, drag_offset.z); fflush (stdout);
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

bool celPcMeshSelect::SetProperty (csStringID propertyId, bool b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_global].id)
  {
    SetGlobalSelection (b);
    return true;
  }
  else if (propertyId == properties[propid_follow].id)
  {
    SetFollowMode (b);
    return true;
  }
  else if (propertyId == properties[propid_followalways].id)
  {
    SetFollowAlwaysMode (b);
    return true;
  }
  else if (propertyId == properties[propid_drag].id)
  {
    SetDragMode (b);
    return true;
  }
  else if (propertyId == properties[propid_sendmove].id)
  {
    SetSendmoveEvent (b);
    return true;
  }
  else if (propertyId == properties[propid_sendup].id)
  {
    SetSendupEvent (b);
    return true;
  }
  else if (propertyId == properties[propid_senddown].id)
  {
    SetSenddownEvent (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

bool celPcMeshSelect::GetPropertyBool (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_global].id)
    return HasGlobalSelection ();
  else if (propertyId == properties[propid_follow].id)
    return HasFollowMode ();
  else if (propertyId == properties[propid_followalways].id)
    return HasFollowAlwaysMode ();
  else if (propertyId == properties[propid_drag].id)
    return HasDragMode ();
  else if (propertyId == properties[propid_sendmove].id)
    return HasSendmoveEvent ();
  else if (propertyId == properties[propid_sendup].id)
    return HasSendupEvent ();
  else if (propertyId == properties[propid_senddown].id)
    return HasSenddownEvent ();
  else
    return celPcCommon::GetPropertyBool (propertyId);
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

bool celPcMeshSelect::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_setcamera)
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
  else if (actionId == action_setmousebuttons)
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
  else if (actionId == action_setdragplanenormal)
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
  return false;
}

//---------------------------------------------------------------------------

