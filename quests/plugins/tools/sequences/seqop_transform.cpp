/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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
}

celTransformSeqOpFactory::~celTransformSeqOpFactory ()
{
}

csPtr<iSeqOp> celTransformSeqOpFactory::CreateSeqOp (
    const celParams& params)
{
  celTransformSeqOp* seqop = new celTransformSeqOp (type,
  	params, entity_par, tag_par, vectorx_par, vectory_par, vectorz_par,
	rot_axis, rot_angle_par);
  return seqop;
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

static float ToFloat (const char* s)
{
  if (!s) return 0.0f;
  float f;
  sscanf (s, "%f", &f);
  return f;
}

celTransformSeqOp::celTransformSeqOp (
	celTransformSeqOpType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* vectorx, const char* vectory, const char* vectorz,
	int axis, const char* angle) : scfImplementationType (this)
{
  celTransformSeqOp::type = type;
  //csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  csRef<iPluginManager> plugin_mgr = 
   csQueryRegistry<iPluginManager> (type->object_reg);
  csRef<iParameterManager> pm = csLoadPlugin<iParameterManager> 
    (plugin_mgr, "cel.parameters.manager");

  entity = pm->ResolveParameter (params, entity_par);
  tag = pm->ResolveParameter (params, tag_par);
  vector.x = ToFloat (pm->ResolveParameter (params, vectorx));
  vector.y = ToFloat (pm->ResolveParameter (params, vectory));
  vector.z = ToFloat (pm->ResolveParameter (params, vectorz));

  do_move = !(vector < .00001f);
  rot_axis = axis;
  rot_angle = ToFloat (pm->ResolveParameter (params, angle));
}

celTransformSeqOp::~celTransformSeqOp ()
{
}

void celTransformSeqOp::FindMesh ()
{
  if (mesh) return;

  // @@@ To many queries for efficiency?
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent = pl->FindEntity (entity);
  if (ent)
  {
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcMesh, tag);
    if (pcmesh)
    {
      mesh = pcmesh->GetMesh ();
      start = mesh->GetMovable ()->GetTransform ().GetOrigin ();
      start_matrix = mesh->GetMovable ()->GetTransform ().GetO2T ();
    }
  }
}

bool celTransformSeqOp::Load (iCelDataBuffer* databuf)
{
  mesh = 0;
  databuf->GetVector3 (start);
  csVector3 row1, row2, row3;
  databuf->GetVector3 (row1);
  databuf->GetVector3 (row2);
  databuf->GetVector3 (row3);
  start_matrix.Set (
  	row1.x, row1.y, row1.z,
  	row2.x, row2.y, row2.z,
  	row3.x, row3.y, row3.z);
  return true;
}

void celTransformSeqOp::Save (iCelDataBuffer* databuf)
{
  databuf->Add (start);
  databuf->Add (start_matrix.Row1 ());
  databuf->Add (start_matrix.Row2 ());
  databuf->Add (start_matrix.Row3 ());
}

void celTransformSeqOp::Init ()
{
  mesh = 0;
  FindMesh ();
}

void celTransformSeqOp::Do (float time)
{
  if (mesh)
  {
    if (do_move)
    {
      csVector3 v = start + time * vector;
      mesh->GetMovable ()->GetTransform ().SetOrigin (v);
    }
    if (rot_axis >= 0)
    {
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

