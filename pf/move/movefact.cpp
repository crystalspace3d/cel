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
  else return NULL;
}

const char* celPfMove::GetTypeName (int idx) const
{
  switch (idx)
  {
    case 0: return "pcmovable";
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

int celPcMovable::Move (iSector* sector, const csVector3& pos)
{
  if (!pcmesh)
  {
    pcmesh = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (), iPcMesh);
  }
  CS_ASSERT (pcmesh != NULL);
  // @@@ Check constraints here!
  pcmesh->MoveMesh (sector, pos);
  return CEL_MOVE_SUCCEED;
}

//---------------------------------------------------------------------------

