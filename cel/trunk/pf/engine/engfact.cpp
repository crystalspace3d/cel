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
#include "pf/engine/engfact.h"
#include "pl/entity.h"
#include "bl/behave.h"
#include "csutil/util.h"
#include "iutil/objreg.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "cstool/csview.h"
#include "ivaria/view.h"
#include "ivideo/graph3d.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPfEngine)

SCF_EXPORT_CLASS_TABLE (pfengine)
  SCF_EXPORT_CLASS (celPfEngine, "cel.pcfactory.engine",
  	"CEL Engine Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfEngine)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfEngine::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfEngine::celPfEngine (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfEngine::~celPfEngine ()
{
}

bool celPfEngine::Initialize (iObjectRegistry* object_reg)
{
  celPfEngine::object_reg = object_reg;
  return true;
}

iCelPropertyClass* celPfEngine::CreatePropertyClass (const char* type)
{
  if (strcmp (type, "pccamera")) return NULL;
  return new celPcCamera (object_reg);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcCamera)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCamera)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcCamera::PcCamera)
  SCF_IMPLEMENTS_INTERFACE (iPcCamera)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcCamera::celPcCamera (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCamera);
  celPcCamera::object_reg = object_reg;
  iEngine* engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  iGraphics3D* g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  view = new csView (engine, g3d);
  iview = SCF_QUERY_INTERFACE (view, iView);
  view->DecRef ();
  g3d->DecRef ();
  engine->DecRef ();
}

celPcCamera::~celPcCamera ()
{
  if (iview) iview->DecRef ();
}

void celPcCamera::SetEntity (iCelEntity* entity)
{
  celPcCamera::entity = entity;
}

iCamera* celPcCamera::GetCamera () const
{
  return iview->GetCamera ();
}

//---------------------------------------------------------------------------

