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
#include "pf/mesh/meshfact.h"
#include "pl/entity.h"
#include "bl/entity.h"
#include "csutil/util.h"
#include "iutil/objreg.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/engine.h"
#include "imap/parser.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPfMesh)

SCF_EXPORT_CLASS_TABLE (pfmesh)
  SCF_EXPORT_CLASS (celPfMesh, "cel.pcfactory.mesh",
  	"CEL Mesh Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfMesh)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfMesh::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfMesh::celPfMesh (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfMesh::~celPfMesh ()
{
}

bool celPfMesh::Initialize (iObjectRegistry* object_reg)
{
  celPfMesh::object_reg = object_reg;
  return true;
}

iCelPropertyClass* celPfMesh::CreatePropertyClass (const char* type)
{
  if (strcmp (type, "pcmesh")) return NULL;
  return new celPcMesh (object_reg);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcMesh)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcMesh)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcMesh::PcMesh)
  SCF_IMPLEMENTS_INTERFACE (iPcMesh)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcMesh::celPcMesh (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcMesh);
  mesh = NULL;
  celPcMesh::object_reg = object_reg;
}

celPcMesh::~celPcMesh ()
{
  if (mesh)
  {
    mesh->DecRef ();
    iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
    CS_ASSERT (engine != NULL);
    engine->GetMeshes ()->Remove (mesh);
    engine->DecRef ();
  }
}

void celPcMesh::SetEntity (iCelEntity* entity)
{
  celPcMesh::entity = entity;
}

iMeshFactoryWrapper* celPcMesh::LoadMeshFactory (const char* fileName)
{
  iLoader* loader = CS_QUERY_REGISTRY (object_reg, iLoader);
  CS_ASSERT (loader != NULL);
  iMeshFactoryWrapper* imeshfact = loader->LoadMeshObjectFactory (fileName);
  loader->DecRef ();
  if (imeshfact == NULL)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.pfmesh.loadmeshfactory",
    	"Error loading mesh object factory '%s'!", fileName);
    return NULL;
  }
  imeshfact->DecRef ();
  return imeshfact;
}

void celPcMesh::SetMesh (const char* factname, const char* filename)
{
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != NULL);
  if (mesh)
  {
    mesh->DecRef ();
    engine->GetMeshes ()->Remove (mesh);
    mesh = NULL;
  }

  if (factname && filename)
  {
    iMeshFactoryWrapper* meshfact = engine->GetMeshFactories ()->FindByName (factname);
    if (!meshfact) meshfact = LoadMeshFactory (filename);
    if (meshfact)
    {
      mesh = engine->CreateMeshWrapper (meshfact, factname/*@@@?*/);
    }
  }

  engine->DecRef ();
}

void celPcMesh::MoveMesh (iSector* sector, const csVector3& pos)
{
  CS_ASSERT (mesh != NULL);
  mesh->GetMovable ()->SetSector (sector);
  mesh->GetMovable ()->SetPosition (pos);
  mesh->GetMovable ()->UpdateMove ();
  mesh->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);
}

//---------------------------------------------------------------------------

