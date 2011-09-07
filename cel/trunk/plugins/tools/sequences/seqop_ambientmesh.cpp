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
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"
#include "iengine/movable.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/persist.h"

#include "plugins/tools/sequences/seqop_ambientmesh.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celAmbientMeshSeqOpType)
CEL_IMPLEMENT_SEQOPTYPE(AmbientMesh)

//---------------------------------------------------------------------------

celAmbientMeshSeqOpFactory::celAmbientMeshSeqOpFactory (
	celAmbientMeshSeqOpType* type) : scfImplementationType (this)
{
  celAmbientMeshSeqOpFactory::type = type;
}

celAmbientMeshSeqOpFactory::~celAmbientMeshSeqOpFactory ()
{
}

csPtr<iSeqOp> celAmbientMeshSeqOpFactory::CreateSeqOp (
    const celParams& params)
{
  celAmbientMeshSeqOp* seqop = new celAmbientMeshSeqOp (type,
  	params, entity_par, tag_par,
	rel_red_par, rel_green_par, rel_blue_par,
	abs_red_par, abs_green_par, abs_blue_par);
  return seqop;
}

bool celAmbientMeshSeqOpFactory::Load (iDocumentNode* node)
{
  rel_red_par.Empty ();
  rel_green_par.Empty ();
  rel_blue_par.Empty ();
  abs_red_par.Empty ();
  abs_green_par.Empty ();
  abs_blue_par.Empty ();

  entity_par = node->GetAttributeValue ("entity");
  if (entity_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.seqops.ambientmesh",
      "'entity' attribute is missing for the ambient mesh seqop!");
    return false;
  }
  tag_par = node->GetAttributeValue ("entity_tag");

  csRef<iDocumentNode> relcolor_node = node->GetNode ("relcolor");
  if (relcolor_node)
  {
    rel_red_par = relcolor_node->GetAttributeValue ("red");
    rel_green_par = relcolor_node->GetAttributeValue ("green");
    rel_blue_par = relcolor_node->GetAttributeValue ("blue");
  }
  csRef<iDocumentNode> abscolor_node = node->GetNode ("abscolor");
  if (abscolor_node)
  {
    abs_red_par = abscolor_node->GetAttributeValue ("red");
    abs_green_par = abscolor_node->GetAttributeValue ("green");
    abs_blue_par = abscolor_node->GetAttributeValue ("blue");
  }
  
  return true;
}

void celAmbientMeshSeqOpFactory::SetEntityParameter (const char* entity,
	const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celAmbientMeshSeqOpFactory::SetRelColorParameter (const char* red,
	const char* green, const char* blue)
{
  rel_red_par = red;
  rel_green_par = green;
  rel_blue_par = blue;
}

void celAmbientMeshSeqOpFactory::SetAbsColorParameter (const char* red,
	const char* green, const char* blue)
{
  abs_red_par = red;
  abs_green_par = green;
  abs_blue_par = blue;
}

//---------------------------------------------------------------------------

celAmbientMeshSeqOp::celAmbientMeshSeqOp (
	celAmbientMeshSeqOpType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* rel_red_par, const char* rel_green_par,
	const char* rel_blue_par,
	const char* abs_red_par, const char* abs_green_par,
	const char* abs_blue_par) : scfImplementationType (this)
{
  celAmbientMeshSeqOp::type = type;

  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity_param = pm->GetParameter (params, entity_par);
  tag_param = pm->GetParameter (params, tag_par);
  rel_red_param = pm->GetParameter (params, rel_red_par);
  rel_green_param = pm->GetParameter (params, rel_green_par);
  rel_blue_param = pm->GetParameter (params, rel_blue_par);
  abs_red_param = pm->GetParameter (params, abs_red_par);
  abs_green_param = pm->GetParameter (params, abs_green_par);
  abs_blue_param = pm->GetParameter (params, abs_blue_par);

  do_abs = abs_red_par != 0 && *abs_red_par != 0;
  do_rel = rel_red_par != 0 && *rel_red_par != 0;

  csRef<iShaderVarStringSet> strings =
    csQueryRegistryTagInterface<iShaderVarStringSet>
    (type->object_reg, "crystalspace.shader.variablenameset");
  ambient = strings->Request ("light ambient");
}

celAmbientMeshSeqOp::~celAmbientMeshSeqOp ()
{
}

void celAmbientMeshSeqOp::SetAmbientColor (const csColor& color)
{
  if (svc)
  {
    csRef<csShaderVariable> var = svc->GetVariableAdd(ambient);
    var->SetValue(csVector3(color.red,color.green,color.blue));
  }
}

csColor celAmbientMeshSeqOp::GetAmbientColor ()
{
  csColor color (0, 0, 0);
  if (svc)
  {
    csRef<csShaderVariable> var = svc->GetVariableAdd(ambient);
    csVector3 v;
    var->GetValue(v);
    color.red = v.x;
    color.green = v.y;
    color.blue = v.z;
  }
  return color;
}


void celAmbientMeshSeqOp::FindMesh (iCelParameterBlock* params)
{
  if (mesh) return;

  entity = entity_param->Get (params);
  tag = tag_param->Get (params);

  // @@@ To many queries for efficiency?
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent = pl->FindEntity (entity);
  if (ent)
  {
    csRef<iPcMesh> pcmesh = celQueryPropertyClassTagEntity<iPcMesh> (ent, tag);
    if (pcmesh)
    {
      mesh = pcmesh->GetMesh ();
      svc = scfQueryInterfaceSafe<iShaderVariableContext> (mesh);
      start = GetAmbientColor ();
    }
  }
}

bool celAmbientMeshSeqOp::Load (iCelDataBuffer* databuf)
{
  mesh = 0;
  svc = 0;
  databuf->GetColor (start);
  return true;
}

void celAmbientMeshSeqOp::Save (iCelDataBuffer* databuf)
{
  databuf->Add (start);
}

void celAmbientMeshSeqOp::Init (iCelParameterBlock* params)
{
  mesh = 0;
  svc = 0;
  FindMesh (params);
}

void celAmbientMeshSeqOp::Do (float time, iCelParameterBlock* params)
{
  if (mesh)
  {
    csColor col;
    if (do_abs)
    {
      abs.red = abs_red_param->GetFloat (params);
      abs.green = abs_green_param->GetFloat (params);
      abs.blue = abs_blue_param->GetFloat (params);
      col = (1.0f-time) * start + time * abs;
    }
    else if (do_rel)
    {
      rel.red = rel_red_param->GetFloat (params);
      rel.green = rel_green_param->GetFloat (params);
      rel.blue = rel_blue_param->GetFloat (params);
      col = start + time * rel;
    }
    else col = start;
    SetAmbientColor (col);
  }
}

//---------------------------------------------------------------------------

