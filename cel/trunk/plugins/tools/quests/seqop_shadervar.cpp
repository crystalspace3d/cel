/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein

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
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "ivaria/reporter.h"
#include "iengine/movable.h"
#include "ivideo/shader/shader.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/persist.h"
#include "propclass/mesh.h"

#include "plugins/tools/quests/seqop_shadervar.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_SEQOPTYPE(ShaderVar)

//---------------------------------------------------------------------------

celShaderVarSeqOpFactory::celShaderVarSeqOpFactory (
	celShaderVarSeqOpType* type) : scfImplementationType (this)
{
  celShaderVarSeqOpFactory::type = type;
}

celShaderVarSeqOpFactory::~celShaderVarSeqOpFactory ()
{
}

csPtr<iQuestSeqOp> celShaderVarSeqOpFactory::CreateSeqOp (
    const celQuestParams& params)
{
  celShaderVarSeqOp* seqop = new celShaderVarSeqOp (type,
  	params, varname_par, mesh_entity_par, mesh_tag_par,
	x_par, y_par, z_par, w_par);
  return seqop;
}

bool celShaderVarSeqOpFactory::Load (iDocumentNode* node)
{
  mesh_entity_par.Empty ();
  mesh_tag_par.Empty ();
  x_par.Empty ();
  y_par.Empty ();
  z_par.Empty ();
  w_par.Empty ();

  varname_par = node->GetAttributeValue ("varname");
  if (varname_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
        "cel.questreward.transform",
        "'varname' attribute is missing for the shadervar seqop!");
    return false;
  }

  csRef<iDocumentNode> mesh_node = node->GetNode ("mesh");
  if (mesh_node)
  {
    mesh_entity_par = node->GetAttributeValue ("entity");
    if (mesh_entity_par.IsEmpty ())
    {
      csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
        "cel.questreward.transform",
        "'entity' attribute is missing for the shadervar seqop!");
      return false;
    }
    mesh_tag_par = node->GetAttributeValue ("tag");
  }
  else
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
        "cel.questreward.transform",
        "The shadervar seqop currently only supports meshes!");
    return false;
  }

  csRef<iDocumentNode> vector4_node = node->GetNode ("vector4");
  if (vector4_node)
  {
    x_par = vector4_node->GetAttributeValue ("x");
    y_par = vector4_node->GetAttributeValue ("y");
    z_par = vector4_node->GetAttributeValue ("z");
    w_par = vector4_node->GetAttributeValue ("w");
  }
  csRef<iDocumentNode> vector3_node = node->GetNode ("vector3");
  if (vector3_node)
  {
    x_par = vector3_node->GetAttributeValue ("x");
    y_par = vector3_node->GetAttributeValue ("y");
    z_par = vector3_node->GetAttributeValue ("z");
  }
  csRef<iDocumentNode> vector2_node = node->GetNode ("vector2");
  if (vector2_node)
  {
    x_par = vector2_node->GetAttributeValue ("x");
    y_par = vector2_node->GetAttributeValue ("y");
  }
  csRef<iDocumentNode> float_node = node->GetNode ("float");
  if (float_node)
  {
    x_par = vector2_node->GetAttributeValue ("value");
  }

  if (x_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
        "cel.questreward.transform",
        "Missing variable to animate for the shadervar seqop!");
    return false;
  }

  return true;
}

void celShaderVarSeqOpFactory::SetVarNameParameter (const char* name)
{
  varname_par = name;
}

void celShaderVarSeqOpFactory::SetMeshEntityParameter (const char* entity,
	const char* tag)
{
  mesh_entity_par = entity;
  mesh_tag_par = tag;
}

void celShaderVarSeqOpFactory::SetFloatParameter (const char* f)
{
  x_par = f;
  y_par.Empty ();
  z_par.Empty ();
  w_par.Empty ();
}

void celShaderVarSeqOpFactory::SetVector2Parameter (const char* x,
	const char* y)
{
  x_par = x;
  y_par = y;
  z_par.Empty ();
  w_par.Empty ();
}

void celShaderVarSeqOpFactory::SetVector3Parameter (const char* x,
	const char* y, const char* z)
{
  x_par = x;
  y_par = y;
  z_par = z;
  w_par.Empty ();
}

void celShaderVarSeqOpFactory::SetVector4Parameter (const char* x,
	const char* y, const char* z, const char* w)
{
  x_par = x;
  y_par = y;
  z_par = z;
  w_par = w;
}

//---------------------------------------------------------------------------

static float ToFloat (const char* s, bool& v)
{
  if (!s) { v = false; return 0.0f; }
  v = true;
  float f = 0.0f;
  sscanf (s, "%f", &f);
  return f;
}

celShaderVarSeqOp::celShaderVarSeqOp (
	celShaderVarSeqOpType* type,
  	const celQuestParams& params,
	const char* varname_par,
	const char* mesh_entity_par, const char* mesh_tag_par,
	const char* x_par, const char* y_par, const char* z_par,
	const char* w_par) : scfImplementationType (this)
{
  celShaderVarSeqOp::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  varname = qm->ResolveParameter (params, varname_par);
  mesh_entity = qm->ResolveParameter (params, mesh_entity_par);
  mesh_tag = qm->ResolveParameter (params, mesh_tag_par);

  bool valid;
  numcomp = 0;
  rel.x = ToFloat (qm->ResolveParameter (params, x_par), valid);
  if (valid) numcomp = 1;
  rel.y = ToFloat (qm->ResolveParameter (params, y_par), valid);
  if (valid) numcomp = 2;
  rel.z = ToFloat (qm->ResolveParameter (params, z_par), valid);
  if (valid) numcomp = 3;
  rel.w = ToFloat (qm->ResolveParameter (params, w_par), valid);
  if (valid) numcomp = 4;
}

celShaderVarSeqOp::~celShaderVarSeqOp ()
{
}

void celShaderVarSeqOp::FindMesh ()
{
  if (mesh) return;
  shadervar = 0;

  // @@@ To many queries for efficiency?
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent = pl->FindEntity (mesh_entity);
  if (ent)
  {
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcMesh,
      mesh_tag);
    if (pcmesh)
    {
      mesh = pcmesh->GetMesh ();
      //start = light->GetColor ();
    }
  }
}

void celShaderVarSeqOp::FindShaderVar ()
{
  if (!mesh) FindMesh ();
  if (!mesh) return;
  if (shadervar) return;
  iShaderVariableContext* svcontext = mesh->GetSVContext ();
  csRef<iStringSet> strset = csQueryRegistryTagInterface<iStringSet> (
  	type->object_reg, "crystalspace.shared.stringset");
  csStringID id = strset->Request (varname);
  shadervar = svcontext->GetVariableAdd (id);
}

bool celShaderVarSeqOp::Load (iCelDataBuffer* databuf)
{
  mesh = 0;
  shadervar = 0;
  //databuf->GetColor (start);
  return true;
}

void celShaderVarSeqOp::Save (iCelDataBuffer* databuf)
{
  //databuf->Add (start);
}

void celShaderVarSeqOp::Init ()
{
  mesh = 0;
  shadervar = 0;
  FindShaderVar ();
}

void celShaderVarSeqOp::Do (float time)
{
  if (shadervar)
  {
    switch (numcomp)
    {
      case 1: shadervar->SetValue (rel.x); break;
      case 2: shadervar->SetValue (csVector2 (rel.x, rel.y)); break;
      case 3: shadervar->SetValue (csVector3 (rel.x, rel.y, rel.z)); break;
      case 4: shadervar->SetValue (rel); break;
    }
#if 0
    csColor col;
    if (do_abs) col = abs;
    else col = start;
    if (do_rel)
      col += time * rel;
    light->SetColor (col);
#endif
  }
}

//---------------------------------------------------------------------------

