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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "pf/move/movefact.h"
#include "pf/mesh.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "bl/behave.h"
#include "csutil/util.h"
#include "csutil/csobject.h"
#include "iutil/object.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/objreg.h"
#include "iutil/eventq.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/engine.h"
#include "imap/parser.h"
#include "ivaria/reporter.h"
#include "ivaria/collider.h"
#include "ivaria/polymesh.h"
#include "imesh/object.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPfMove)

SCF_EXPORT_CLASS_TABLE (pfmove)
  SCF_EXPORT_CLASS (celPfMove, "cel.pcfactory.move",
  	"CEL Movable Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfMove)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfMove::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfMove::celPfMove (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfMove::~celPfMove ()
{
}

bool celPfMove::Initialize (iObjectRegistry* object_reg)
{
  celPfMove::object_reg = object_reg;
  return true;
}

iCelPropertyClass* celPfMove::CreatePropertyClass (const char* type)
{
  if (!strcmp (type, "pcmovable"))
    return new celPcMovable (object_reg);
  else if (!strcmp (type, "pcsolid"))
    return new celPcSolid (object_reg);
  else if (!strcmp (type, "pcmovableconst_cd"))
    return new celPcMovableConstraintCD (object_reg);
  else return NULL;
}

const char* celPfMove::GetTypeName (int idx) const
{
  switch (idx)
  {
    case 0: return "pcmovable";
    case 1: return "pcsolid";
    case 2: return "pcmovableconst_cd";
    default: return NULL;
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcMovable)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMovable)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMovable::PcMovable)
  SCF_IMPLEMENTS_INTERFACE (iPcMovable)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMovable::celPcMovable (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMovable);
  pcmesh = NULL;
  celPcMovable::object_reg = object_reg;
}

celPcMovable::~celPcMovable ()
{
  if (pcmesh) pcmesh->DecRef ();
  RemoveAllConstraints ();
}

void celPcMovable::SetEntity (iCelEntity* entity)
{
  celPcMovable::entity = entity;
}

void celPcMovable::SetMesh (iPcMesh* mesh)
{
  if (mesh) mesh->IncRef ();
  if (pcmesh) pcmesh->DecRef ();
  pcmesh = mesh;
}

iPcMesh* celPcMovable::GetMesh ()
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  return pcmesh;
}

int celPcMovable::Move (iSector* sector, const csVector3& pos)
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  CS_ASSERT (pcmesh != NULL);
  csVector3 start = pcmesh->GetMesh ()->GetMovable ()->GetPosition ();
  csVector3 end = pos;
  csVector3 realpos;
  bool partial = false;
  int i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    iPcMovableConstraint* c = (iPcMovableConstraint*)constraints[i];
    int rc = c->CheckMove (sector, start, end, realpos);
    if (rc == CEL_MOVE_FAIL) return rc;
    if (rc == CEL_MOVE_PARTIAL) { end = realpos; partial = true; }
  }
  pcmesh->MoveMesh (sector, realpos);
  return partial ? CEL_MOVE_PARTIAL : CEL_MOVE_SUCCEED;
}

void celPcMovable::AddConstraint (iPcMovableConstraint* constraint)
{
  int idx = constraints.Find (constraint);
  if (idx != -1) return;
  constraints.Push (constraint);
  constraint->IncRef ();
}

void celPcMovable::RemoveConstraint (iPcMovableConstraint* constraint)
{
  int idx = constraints.Find (constraint);
  if (idx == -1) return;
  constraints.Delete (idx);
  constraint->DecRef ();
}

void celPcMovable::RemoveAllConstraints ()
{
  int i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    iPcMovableConstraint* c = (iPcMovableConstraint*)constraints[i];
    c->DecRef ();
  }
  constraints.DeleteAll ();
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcSolid)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcSolid)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcSolid::PcSolid)
  SCF_IMPLEMENTS_INTERFACE (iPcSolid)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcSolid::celPcSolid (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcSolid);
  pcmesh = NULL;
  collider = NULL;
  celPcSolid::object_reg = object_reg;
}

celPcSolid::~celPcSolid ()
{
  if (pcmesh) pcmesh->DecRef ();
  if (collider) collider->DecRef ();
}

void celPcSolid::SetEntity (iCelEntity* entity)
{
  celPcSolid::entity = entity;
}

void celPcSolid::SetMesh (iPcMesh* mesh)
{
  if (mesh) mesh->IncRef ();
  if (pcmesh) pcmesh->DecRef ();
  pcmesh = mesh;
  if (collider) { collider->DecRef (); collider = NULL; }
}

iCollider* celPcSolid::GetCollider ()
{
  if (collider) return collider;
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  CS_ASSERT (pcmesh != NULL);
  iPolygonMesh* pmesh = SCF_QUERY_INTERFACE (pcmesh->GetMesh ()->GetMeshObject (),
		  iPolygonMesh);
  if (pmesh)
  {
    iCollideSystem* cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
    CS_ASSERT (cdsys != NULL);
    collider = cdsys->CreateCollider (pmesh);
    pmesh->DecRef ();
    cdsys->DecRef ();
  }
  return collider;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcMovableConstraintCD)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMovableConstraint)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMovableConstraintCD::PcMovableConstraintCD)
  SCF_IMPLEMENTS_INTERFACE (iPcMovableConstraint)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMovableConstraintCD::celPcMovableConstraintCD (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMovableConstraint);
  celPcMovableConstraintCD::object_reg = object_reg;
}

celPcMovableConstraintCD::~celPcMovableConstraintCD ()
{
}

void celPcMovableConstraintCD::SetEntity (iCelEntity* entity)
{
  celPcMovableConstraintCD::entity = entity;
}

int celPcMovableConstraintCD::CheckMove (iSector* sector, const csVector3& start,
		const csVector3& end, csVector3& pos)
{
  iPcMesh* pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  CS_ASSERT (pcmesh != NULL);
  iPcSolid* pcsolid = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcSolid);
  CS_ASSERT (pcsolid != NULL);
  int rc = CEL_MOVE_SUCCEED;
  pos = end;
  if (pcsolid->GetCollider ())
  {
    iCollideSystem* cdsys = CS_QUERY_REGISTRY (object_reg, iCollideSystem);
    CS_ASSERT (cdsys != NULL);
    //@@@@@@@@@@@@@@@@@@@@@@@@

    cdsys->DecRef ();
  }
  pcsolid->DecRef ();
  pcmesh->DecRef ();
  return rc;
}

//---------------------------------------------------------------------------

