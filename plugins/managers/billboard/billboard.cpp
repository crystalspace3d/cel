/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein

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
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "igraphic/image.h"
#include "ivideo/graph3d.h"
#include "iengine/engine.h"
#include "iengine/material.h"

#include "plugins/managers/billboard/billboard.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celBillboard)
  SCF_IMPLEMENTS_INTERFACE (iBillboard)
SCF_IMPLEMENT_IBASE_END

celBillboard::celBillboard ()
{
  SCF_CONSTRUCT_IBASE (0);
  name = 0;
  flags.SetAll (CEL_BILLBOARD_VISIBLE);
  materialname = 0;
  material = 0;
  x = y = 0;
  w = h = 10;
}

celBillboard::~celBillboard ()
{
  delete[] name;
  delete[] materialname;
}


bool celBillboard::SetMaterialName (const char* matname)
{
  delete[] materialname;
  materialname = csStrNew (matname);
  material = 0;
  return true;
}

void celBillboard::SetSize (int w, int h)
{
}

void celBillboard::SetPosition (int x, int y)
{
}

void celBillboard::Move (int dx, int dy)
{
}

static G3DPolygonDPFX poly;
static bool poly_init = false;

void celBillboard::Draw (iEngine* engine, iGraphics3D* g3d)
{
  if (!flags.Check (CEL_BILLBOARD_VISIBLE)) return;
  if (!material)
  {
    material = engine->FindMaterial (materialname);
    if (!material) return;
  }
  if (!poly_init)
  {
    poly_init = true;
    poly.num = 4;
    poly.texels[0].Set (0, 0);
    poly.texels[1].Set (1, 0);
    poly.texels[2].Set (1, 1);
    poly.texels[3].Set (0, 1);
    poly.colors[0].Set (1, 1, 1);
    poly.colors[1].Set (1, 1, 1);
    poly.colors[2].Set (1, 1, 1);
    poly.colors[3].Set (1, 1, 1);
    poly.use_fog = false;
    poly.mixmode = CS_FX_COPY;
  }
  poly.mat_handle = material->GetMaterialHandle ();
  poly.vertices[0].Set (x, y);
  poly.vertices[1].Set (x+w, y);
  poly.vertices[2].Set (x+w, y+h);
  poly.vertices[3].Set (x, y+h);
  poly.z[0] = 1;
  poly.z[1] = 1;
  poly.z[2] = 1;
  poly.z[3] = 1;
  g3d->SetZMode (CS_ZBUF_USE);
  g3d->DrawPolygonFX (poly);
}

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celBillboardManager)

SCF_IMPLEMENT_IBASE (celBillboardManager)
  SCF_IMPLEMENTS_INTERFACE (iBillboardManager)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBillboardManager::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celBillboardManager::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celBillboardManager::celBillboardManager (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  scfiEventHandler = 0;
}

celBillboardManager::~celBillboardManager ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q != 0)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
}

bool celBillboardManager::Initialize (iObjectRegistry* object_reg)
{
  celBillboardManager::object_reg = object_reg;

  scfiEventHandler = new EventHandler (this);
  csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);

  return true;
}

bool celBillboardManager::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdPostProcess)
  {
    g3d->BeginDraw (CSDRAW_3DGRAPHICS);
    int i;
    for (i = 0 ; i < billboards.Length () ; i++)
    {
      billboards[i]->Draw (engine, g3d);
    }
  }
  return false;
}

iBillboard* celBillboardManager::CreateBillboard (const char* name)
{
  return 0;
}

iBillboard* celBillboardManager::FindBillboard (const char* name) const
{
  return 0;
}

void celBillboardManager::RemoveBillboard (iBillboard* billboard)
{
}

void celBillboardManager::RemoveAll ()
{
}

void celBillboardManager::SetFlags (uint32 flags, uint32 mask)
{
}

//---------------------------------------------------------------------------

