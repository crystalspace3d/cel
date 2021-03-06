/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin

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
#include "propclass/camera.h"

#include "plugins/tools/sequences/seqop_transform.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celTransformSeqOpType)
CEL_IMPLEMENT_SEQOPTYPE(Transform)

//---------------------------------------------------------------------------

celTransformSeqOpFactory::celTransformSeqOpFactory (
	celTransformSeqOpType* type) : scfImplementationType (this)
{
  celTransformSeqOpFactory::type = type;
  rot_axis = -1;
  reversed = false;
}

celTransformSeqOpFactory::~celTransformSeqOpFactory ()
{
}

csPtr<iSeqOp> celTransformSeqOpFactory::CreateSeqOp (
    iCelParameterBlock* params)
{
  celTransformSeqOp* seqop = new celTransformSeqOp (type,
  	params, entity_par, tag_par, vectorx_par, vectory_par, vectorz_par,
	rot_axis, rot_angle_par, reversed);
  return seqop;
}

bool celTransformSeqOpFactory::Save (iDocumentNode* node)
{
  node->SetAttribute ("entity", entity_par);
  if (!tag_par.IsEmpty ()) node->SetAttribute ("entity_tag", tag_par);
  if (!vectorx_par.IsEmpty ())
  {
    csRef<iDocumentNode> vNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    vNode->SetValue ("v");
    vNode->SetAttribute ("x", vectorx_par);
    vNode->SetAttribute ("y", vectory_par);
    vNode->SetAttribute ("z", vectorz_par);
  }
  if (!rot_angle_par.IsEmpty ())
  {
    csRef<iDocumentNode> rotNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    if (rot_axis == CS_AXIS_X) rotNode->SetValue ("rotx");
    else if (rot_axis == CS_AXIS_Y) rotNode->SetValue ("roty");
    else if (rot_axis == CS_AXIS_Z) rotNode->SetValue ("rotz");
    rotNode->SetAttribute ("angle", rot_angle_par);
  }
  if (reversed)
    node->SetAttribute ("reversed", "true");
  return true;
}

bool celTransformSeqOpFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.seqops.transform",
      "'entity' attribute is missing for the transform seqop!");
    return false;
  }
  tag_par = node->GetAttributeValue ("entity_tag");

  csRef<iDocumentNode> v_node = node->GetNode ("v");
  if (v_node)
  {
    vectorx_par = v_node->GetAttributeValue ("x");
    vectory_par = v_node->GetAttributeValue ("y");
    vectorz_par = v_node->GetAttributeValue ("z");
  }
  rot_axis = -1;
  csRef<iDocumentNode> rotx_node = node->GetNode ("rotx");
  if (rotx_node)
  {
    rot_axis = CS_AXIS_X;
    rot_angle_par = rotx_node->GetAttributeValue ("angle");
  }
  csRef<iDocumentNode> roty_node = node->GetNode ("roty");
  if (roty_node)
  {
    rot_axis = CS_AXIS_Y;
    rot_angle_par = roty_node->GetAttributeValue ("angle");
  }
  csRef<iDocumentNode> rotz_node = node->GetNode ("rotz");
  if (rotz_node)
  {
    rot_axis = CS_AXIS_Z;
    rot_angle_par = rotz_node->GetAttributeValue ("angle");
  }
  reversed = node->GetAttributeValueAsBool ("reversed");
  
  return true;
}

void celTransformSeqOpFactory::SetEntityParameter (const char* entity,
	const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celTransformSeqOpFactory::SetVectorParameter (const char* vectorx,
	const char* vectory, const char* vectorz)
{
  vectorx_par = vectorx;
  vectory_par = vectory;
  vectorz_par = vectorz;
}

void celTransformSeqOpFactory::SetRotationParameter (int axis,
	const char* angle)
{
  rot_axis = axis;
  rot_angle_par = angle;
}

//---------------------------------------------------------------------------

celTransformSeqOp::celTransformSeqOp (
	celTransformSeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	const char* vectorx, const char* vectory, const char* vectorz,
	int axis, const char* angle, bool reversed) : scfImplementationType (this)
{
  celTransformSeqOp::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity_param = pm->GetParameter (params, entity_par);
  tag_param = pm->GetParameter (params, tag_par);
  vectorx_param = pm->GetParameter (params, vectorx);
  vectory_param = pm->GetParameter (params, vectory);
  vectorz_param = pm->GetParameter (params, vectorz);

  rot_axis = axis;
  rot_angle_param = pm->GetParameter (params, angle);

  celTransformSeqOp::reversed = reversed;
}

celTransformSeqOp::~celTransformSeqOp ()
{
}

void celTransformSeqOp::FindMesh (iCelParameterBlock* params)
{
  if (pcmesh) return;

  iCelEntity* ent = pm->ResolveEntityParameter (type->pl, params, entity_param, 0);
  if (!ent) return;
  tag = tag_param->Get (params);

  pcmesh = celQueryPropertyClassTagEntity<iPcMesh> (ent, tag);
}

void celTransformSeqOp::Init (iCelParameterBlock* params)
{
  pcmesh = 0;
  mesh = 0;
  FindMesh (params);
}

void celTransformSeqOp::FindMesh ()
{
  mesh = pcmesh->GetMesh ();
  if (mesh)
  {
    start = mesh->GetMovable ()->GetTransform ().GetOrigin (); 
    start_matrix = mesh->GetMovable ()->GetTransform ().GetO2T ();
  }
}

void celTransformSeqOp::Do (float time, iCelParameterBlock* params)
{
  if (pcmesh)
  {
    if (!mesh)
    {
      FindMesh ();
      if (!mesh) return;
    }
    vector.x = vectorx_param->GetFloat (params);
    vector.y = vectory_param->GetFloat (params);
    vector.z = vectorz_param->GetFloat (params);
    if (reversed) vector = -vector;
    do_move = !(vector < .00001f);

    if (do_move)
    {
      csVector3 v = start + time * vector;
      mesh->GetMovable ()->GetTransform ().SetOrigin (v);
    }
    if (rot_axis >= 0)
    {
      rot_angle = rot_angle_param->GetFloat (params);
      if (reversed) rot_angle = -rot_angle;

      csMatrix3 m = start_matrix;
      switch (rot_axis)
      {
        case CS_AXIS_X: m *= csXRotMatrix3 (rot_angle * time); break;
        case CS_AXIS_Y: m *= csYRotMatrix3 (rot_angle * time); break;
        case CS_AXIS_Z: m *= csZRotMatrix3 (rot_angle * time); break;
      }
      mesh->GetMovable ()->GetTransform ().SetO2T (m);
    }
    mesh->PlaceMesh ();
    mesh->GetMovable ()->UpdateMove ();
  }
}

//---------------------------------------------------------------------------

