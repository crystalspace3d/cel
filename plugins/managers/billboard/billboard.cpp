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
  w = h = 40;
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
  celBillboard::w = w;
  celBillboard::h = h;
}

void celBillboard::SetPosition (int x, int y)
{
  celBillboard::x = x;
  celBillboard::y = y;
}

void celBillboard::Move (int dx, int dy)
{
  celBillboard::x += x;
  celBillboard::y += y;
}

void celBillboard::FireMouseUp (int x, int y, int button)
{
  int i;
  for (i = 0 ; i < handlers.Length () ; i++)
    handlers[i]->Unselect (this, x, y, button);
}

void celBillboard::FireMouseDown (int x, int y, int button)
{
  int i;
  for (i = 0 ; i < handlers.Length () ; i++)
    handlers[i]->Select (this, x, y, button);
}

void celBillboard::FireMouseMove (int x, int y, int button)
{
  int i;
  for (i = 0 ; i < handlers.Length () ; i++)
    handlers[i]->MouseMove (this, x, y, button);
}

void celBillboard::FireMouseDoubleClick (int x, int y, int button)
{
  int i;
  for (i = 0 ; i < handlers.Length () ; i++)
    handlers[i]->DoubleClick (this, x, y, button);
}


bool celBillboard::In (int cx, int cy)
{
  if (cx >= x && cx < x+w && cy >= y && cy < y+h)
    return true;
  else
    return false;
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
  int fh = g3d->GetHeight ();
  poly.vertices[0].Set (x, fh-y);
  poly.vertices[1].Set (x+w, fh-y);
  poly.vertices[2].Set (x+w, fh-y-h);
  poly.vertices[3].Set (x, fh-y-h);
  poly.z[0] = 1;
  poly.z[1] = 1;
  poly.z[2] = 1;
  poly.z[3] = 1;
  g3d->SetZMode (CS_ZBUF_FILL);
  g3d->DrawPolygonFX (poly);
}

void celBillboard::AddEventHandler (iBillboardEventHandler* evh)
{
  handlers.Push (evh);
}

void celBillboard::RemoveEventHandler (iBillboardEventHandler* evh)
{
  handlers.Delete (evh);
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
  unsigned int trigger = CSMASK_Nothing | CSMASK_MouseUp | CSMASK_MouseDown |
  	CSMASK_MouseMove | CSMASK_MouseDoubleClick;
  q->RegisterListener (scfiEventHandler, trigger);

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);

  return true;
}

celBillboard* celBillboardManager::FindBillboard (int x, int y)
{
  // @@@ OPTIMIZE WITH SOME KIND OF HIERARCHICAL BBOXES.
  // @@@ KEEP Z-ORDER IN MIND!
  int i;
  for (i = 0 ; i < billboards.Length () ; i++)
  {
    if (billboards[i]->In (x, y))
      return billboards[i];
  }
  return 0;
}

bool celBillboardManager::HandleEvent (iEvent& ev)
{
  switch (ev.Type)
  {
    case csevBroadcast:
      if (ev.Command.Code == cscmdPostProcess)
      {
        g3d->BeginDraw (CSDRAW_3DGRAPHICS);
        int i;
        for (i = 0 ; i < billboards.Length () ; i++)
        {
          billboards[i]->Draw (engine, g3d);
        }
      }
      break;
    case csevMouseUp:
      {
        celBillboard* bb = FindBillboard (ev.Mouse.x, ev.Mouse.y);
	if (bb)
	  bb->FireMouseUp (ev.Mouse.Button, ev.Mouse.x, ev.Mouse.y);
      }
      break;
    case csevMouseDown:
      {
        celBillboard* bb = FindBillboard (ev.Mouse.x, ev.Mouse.y);
	if (bb)
	  bb->FireMouseDown (ev.Mouse.Button, ev.Mouse.x, ev.Mouse.y);
      }
      break;
    case csevMouseMove:
      {
        celBillboard* bb = FindBillboard (ev.Mouse.x, ev.Mouse.y);
	if (bb)
	  bb->FireMouseMove (ev.Mouse.Button, ev.Mouse.x, ev.Mouse.y);
      }
      break;
    case csevMouseDoubleClick:
      {
        celBillboard* bb = FindBillboard (ev.Mouse.x, ev.Mouse.y);
	if (bb)
	  bb->FireMouseDoubleClick (ev.Mouse.Button, ev.Mouse.x, ev.Mouse.y);
      }
      break;
  }
  return false;
}

iBillboard* celBillboardManager::CreateBillboard (const char* name)
{
  celBillboard* bb = new celBillboard ();
  bb->SetName (name);
  billboards.Push (bb);
  billboards_hash.Put (name, bb);
  return bb;
}

iBillboard* celBillboardManager::FindBillboard (const char* name) const
{
  return billboards_hash.Get (name);
}

void celBillboardManager::RemoveBillboard (iBillboard* billboard)
{
  billboards.Delete ((celBillboard*)billboard);
  billboards_hash.Delete (billboard->GetName (), (celBillboard*)billboard);
}

void celBillboardManager::RemoveAll ()
{
  billboards.DeleteAll ();
  billboards_hash.DeleteAll ();
}

void celBillboardManager::SetFlags (uint32 flags, uint32 mask)
{
  int i;
  for (i = 0 ; i < billboards.Length () ; i++)
    billboards[i]->GetFlags ().Set (flags, mask);
}

//---------------------------------------------------------------------------

