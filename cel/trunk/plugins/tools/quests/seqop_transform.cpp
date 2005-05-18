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

SCF_IMPLEMENT_IBASE (celTransformSeqOpFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestSeqOpFactory)
  SCF_IMPLEMENTS_INTERFACE (iTransformQuestSeqOpFactory)
SCF_IMPLEMENT_IBASE_END

celTransformSeqOpFactory::celTransformSeqOpFactory (
	celTransformSeqOpType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celTransformSeqOpFactory::type = type;
  entity_par = 0;
  vector.Set (0, 0, 0);
  rot_axis = -1;
}

celTransformSeqOpFactory::~celTransformSeqOpFactory ()
{
  delete[] entity_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestSeqOp> celTransformSeqOpFactory::CreateSeqOp (
    const csHash<csStrKey,csStrKey>& params)
{
  celTransformSeqOp* seqop = new celTransformSeqOp (type,
  	params, entity_par, vector, rot_axis, rot_angle);
  return seqop;
}

bool celTransformSeqOpFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));

  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.transform",
      "'entity' attribute is missing for the transform seqop!");
    return false;
  }

  csRef<iDocumentNode> v_node = node->GetNode ("v");
  if (v_node)
  {
    vector.x = v_node->GetAttributeValueAsFloat ("x");
    vector.y = v_node->GetAttributeValueAsFloat ("y");
    vector.z = v_node->GetAttributeValueAsFloat ("z");
  }
  csRef<iDocumentNode> rotx_node = node->GetNode ("rotx");
  if (rotx_node)
  {
    rot_axis = CS_AXIS_X;
    rot_angle = rotx_node->GetAttributeValueAsFloat ("angle");
  }
  csRef<iDocumentNode> roty_node = node->GetNode ("roty");
  if (roty_node)
  {
    rot_axis = CS_AXIS_Y;
    rot_angle = roty_node->GetAttributeValueAsFloat ("angle");
  }
  csRef<iDocumentNode> rotz_node = node->GetNode ("rotz");
  if (rotz_node)
  {
    rot_axis = CS_AXIS_Z;
    rot_angle = rotz_node->GetAttributeValueAsFloat ("angle");
  }
  
  return true;
}

void celTransformSeqOpFactory::SetEntityParameter (const char* entity)
{
  if (entity_par == entity) 
    return;

  delete[] entity_par;
  entity_par = csStrNew (entity);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celTransformSeqOp)
  SCF_IMPLEMENTS_INTERFACE (iQuestSeqOp)
SCF_IMPLEMENT_IBASE_END

celTransformSeqOp::celTransformSeqOp (
	celTransformSeqOpType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par,
	const csVector3& vector,
	int axis, float angle)
{
  SCF_CONSTRUCT_IBASE (0);
  celTransformSeqOp::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  celTransformSeqOp::vector = vector;
  do_move = !(vector < .00001f);
  rot_axis = axis;
  rot_angle = angle;
}

celTransformSeqOp::~celTransformSeqOp ()
{
  delete[] entity;
  SCF_DESTRUCT_IBASE ();
}

void celTransformSeqOp::FindMesh ()
{
  if (mesh) return;

  // @@@ To many queries for efficiency?
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  iCelEntity* ent = pl->FindEntity (entity);
  if (ent)
  {
    csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (ent, iPcMesh);
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

