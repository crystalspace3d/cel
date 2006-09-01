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
#include "ivaria/reporter.h"
#include "iengine/movable.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/persist.h"
#include "propclass/camera.h"

#include "plugins/tools/quests/seqop_transform.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_SEQOPTYPE(Transform)

//---------------------------------------------------------------------------

celTransformSeqOpFactory::celTransformSeqOpFactory (
	celTransformSeqOpType* type) : scfImplementationType (this)
{
  celTransformSeqOpFactory::type = type;
  entity_par = 0;
  tag_par = 0;
  vectorx_par = 0;
  vectory_par = 0;
  vectorz_par = 0;
  rot_axis = -1;
  rot_angle_par = 0;
}

celTransformSeqOpFactory::~celTransformSeqOpFactory ()
{
  delete[] entity_par;
  delete[] tag_par;
  delete[] vectorx_par;
  delete[] vectory_par;
  delete[] vectorz_par;
  delete[] rot_angle_par;
}

csPtr<iQuestSeqOp> celTransformSeqOpFactory::CreateSeqOp (
    const csHash<csStrKey,csStrKey>& params)
{
  celTransformSeqOp* seqop = new celTransformSeqOp (type,
  	params, entity_par, tag_par, vectorx_par, vectory_par, vectorz_par,
	rot_axis, rot_angle_par);
  return seqop;
}

bool celTransformSeqOpFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] vectorx_par; vectorx_par = 0;
  delete[] vectory_par; vectory_par = 0;
  delete[] vectorz_par; vectorz_par = 0;
  delete[] rot_angle_par; rot_angle_par = 0;

  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.transform",
      "'entity' attribute is missing for the transform seqop!");
    return false;
  }
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));

  csRef<iDocumentNode> v_node = node->GetNode ("v");
  if (v_node)
  {
    vectorx_par = csStrNew (v_node->GetAttributeValue ("x"));
    vectory_par = csStrNew (v_node->GetAttributeValue ("y"));
    vectorz_par = csStrNew (v_node->GetAttributeValue ("z"));
  }
  csRef<iDocumentNode> rotx_node = node->GetNode ("rotx");
  if (rotx_node)
  {
    rot_axis = CS_AXIS_X;
    rot_angle_par = csStrNew (rotx_node->GetAttributeValue ("angle"));
  }
  csRef<iDocumentNode> roty_node = node->GetNode ("roty");
  if (roty_node)
  {
    rot_axis = CS_AXIS_Y;
    rot_angle_par = csStrNew (roty_node->GetAttributeValue ("angle"));
  }
  csRef<iDocumentNode> rotz_node = node->GetNode ("rotz");
  if (rotz_node)
  {
    rot_axis = CS_AXIS_Z;
    rot_angle_par = csStrNew (rotz_node->GetAttributeValue ("angle"));
  }
  
  return true;
}

void celTransformSeqOpFactory::SetEntityParameter (const char* entity,
	const char* tag)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
  if (tag_par != tag)
  {
    delete[] tag_par;
    tag_par = csStrNew (tag);
  }
}

void celTransformSeqOpFactory::SetVectorParameter (const char* vectorx,
	const char* vectory, const char* vectorz)
{
  if (vectorx_par != vectorx)
  {
    delete[] vectorx_par;
    vectorx_par = csStrNew (vectorx);
  }
  if (vectory_par != vectory)
  {
    delete[] vectory_par;
    vectory_par = csStrNew (vectory);
  }
  if (vectorz_par != vectorz)
  {
    delete[] vectorz_par;
    vectorz_par = csStrNew (vectorz);
  }
}

void celTransformSeqOpFactory::SetRotationParameter (int axis,
	const char* angle)
{
  rot_axis = axis;
  if (rot_angle_par == angle) return;
  delete[] rot_angle_par;
  rot_angle_par = csStrNew (angle);
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
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par, const char* tag_par,
	const char* vectorx, const char* vectory, const char* vectorz,
	int axis, const char* angle) : scfImplementationType (this)
{
  celTransformSeqOp::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  vector.x = ToFloat (qm->ResolveParameter (params, vectorx));
  vector.y = ToFloat (qm->ResolveParameter (params, vectory));
  vector.z = ToFloat (qm->ResolveParameter (params, vectorz));

  do_move = !(vector < .00001f);
  rot_axis = axis;
  rot_angle = ToFloat (qm->ResolveParameter (params, angle));
}

celTransformSeqOp::~celTransformSeqOp ()
{
  delete[] entity;
  delete[] tag;
}

void celTransformSeqOp::FindMesh ()
{
  if (mesh) return;

  // @@@ To many queries for efficiency?
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
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
    mesh->GetMovable ()->UpdateMove ();
  }
}

//---------------------------------------------------------------------------

