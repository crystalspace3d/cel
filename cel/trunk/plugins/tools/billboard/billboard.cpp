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
#include "ivideo/material.h"
#include "ivideo/texture.h"
#include "iengine/engine.h"
#include "iengine/material.h"

#include "plugins/tools/billboard/billboard.h"

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
  w = h = -1;
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


bool celBillboard::In (iEngine* engine, int cx, int cy)
{
  if (!material)
  {
    material = engine->FindMaterial (materialname);
    if (!material) return false;
  }
  material->Visit ();
  if (w == -1)
  {
    material->GetMaterialHandle ()->GetTexture ()->GetOriginalDimensions (
    	w, h);
  }
  if (cx >= x && cx < x+w && cy >= y && cy < y+h)
    return true;
  else
    return false;
}

static G3DPolygonDPFX poly;
static bool poly_init = false;

void celBillboard::Draw (iEngine* engine, iGraphics3D* g3d, float z)
{
  if (!flags.Check (CEL_BILLBOARD_VISIBLE)) return;
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
  if (!material)
  {
    material = engine->FindMaterial (materialname);
    if (!material) return;
  }
  material->Visit ();
  if (w == -1)
  {
    material->GetMaterialHandle ()->GetTexture ()->GetMipMapDimensions (
    	0, w, h);
  }
  poly.mat_handle = material->GetMaterialHandle ();
  int fh = g3d->GetHeight ();
  poly.vertices[0].Set (x, fh-y);
  poly.vertices[1].Set (x+w, fh-y);
  poly.vertices[2].Set (x+w, fh-y-h);
  poly.vertices[3].Set (x, fh-y-h);
  float invz = 1.0 / z;
  poly.z[0] = invz;
  poly.z[1] = invz;
  poly.z[2] = invz;
  poly.z[3] = invz;
  g3d->SetRenderState (G3DRENDERSTATE_ZBUFFERMODE, CS_ZBUF_FILL);
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
  moving_billboard = 0;

  z_min = 1;
  z_max = 10;
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

celBillboard* celBillboardManager::FindBillboard (int x, int y,
	uint32 desired_flags)
{
  // @@@ OPTIMIZE WITH SOME KIND OF HIERARCHICAL BBOXES.
  // @@@ KEEP Z-ORDER IN MIND!
  int i;
  for (i = billboards.Length ()-1 ; i >= 0 ; i--)
  {
    csFlags& f = billboards[i]->GetFlags ();
    if (f.Check (CEL_BILLBOARD_CLICKABLE | CEL_BILLBOARD_MOVABLE))
      if (billboards[i]->In (engine, x, y))
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
        if (billboards.Length () > 0)
	{
          g3d->BeginDraw (CSDRAW_3DGRAPHICS);
          int i;
	  float z = z_max;
	  float dz = (z_max-z_min) / float (billboards.Length ());
          for (i = 0 ; i < billboards.Length () ; i++)
          {
            billboards[i]->Draw (engine, g3d, z);
	    z -= dz;
          }
        }
      }
      break;
    case csevMouseUp:
      {
        if (moving_billboard)
	{
	  moving_billboard->SetPosition (ev.Mouse.x + moving_dx,
	  	ev.Mouse.y + moving_dy);
	  moving_billboard = 0;
	}

        celBillboard* bb = FindBillboard (ev.Mouse.x, ev.Mouse.y,
		CEL_BILLBOARD_CLICKABLE);
	if (bb)
	  bb->FireMouseUp (ev.Mouse.Button, ev.Mouse.x, ev.Mouse.y);
      }
      break;
    case csevMouseDown:
      {
        celBillboard* bb = FindBillboard (ev.Mouse.x, ev.Mouse.y,
		CEL_BILLBOARD_CLICKABLE | CEL_BILLBOARD_MOVABLE);
	if (bb)
	{
	  if (bb->GetFlags ().Check (CEL_BILLBOARD_RESTACK))
	  {
	    StackTop (bb);
	  }
	  if (bb->GetFlags ().Check (CEL_BILLBOARD_MOVABLE))
	  {
	    moving_billboard = bb;
	    bb->GetPosition (moving_dx, moving_dy);
	    moving_dx -= ev.Mouse.x;
	    moving_dy -= ev.Mouse.y;
	  }
	  if (bb->GetFlags ().Check (CEL_BILLBOARD_CLICKABLE))
	    bb->FireMouseDown (ev.Mouse.Button, ev.Mouse.x, ev.Mouse.y);
        }
      }
      break;
    case csevMouseMove:
      {
        if (moving_billboard)
	{
	  moving_billboard->SetPosition (ev.Mouse.x + moving_dx,
	  	ev.Mouse.y + moving_dy);
	}

        celBillboard* bb = FindBillboard (ev.Mouse.x, ev.Mouse.y,
		CEL_BILLBOARD_CLICKABLE);
	if (bb)
	  bb->FireMouseMove (ev.Mouse.Button, ev.Mouse.x, ev.Mouse.y);
      }
      break;
    case csevMouseDoubleClick:
      {
        celBillboard* bb = FindBillboard (ev.Mouse.x, ev.Mouse.y,
		CEL_BILLBOARD_CLICKABLE);
	if (bb)
	  bb->FireMouseDoubleClick (ev.Mouse.Button, ev.Mouse.x, ev.Mouse.y);
      }
      break;
  }
  return false;
}

void celBillboardManager::StackTop (iBillboard* bb)
{
  int idx = billboards.Find ((celBillboard*)bb);
  if (idx == -1) return;
  if (idx == billboards.Length ()-1) return;	// Nothing to do.
  celBillboard* cbb = billboards.Extract (idx);
  billboards.Push (cbb);
}

void celBillboardManager::StackBottom (iBillboard* bb)
{
  int idx = billboards.Find ((celBillboard*)bb);
  if (idx == -1) return;
  if (idx == 0) return; 			// Nothing to do.
  celBillboard* cbb = billboards.Extract (idx);
  billboards.Insert (1, cbb);
}

void celBillboardManager::StackUp (iBillboard* bb)
{
  if (billboards.Length () <= 1) return;	// Nothing to do.
  int idx = billboards.Find ((celBillboard*)bb);
  if (idx == -1) return;
  if (idx == billboards.Length ()-1) return;	// Nothing to do.
  celBillboard* cbb = billboards.Extract (idx);
  billboards.Insert (idx+1, cbb);
}

void celBillboardManager::StackDown (iBillboard* bb)
{
  if (billboards.Length () <= 1) return;	// Nothing to do.
  int idx = billboards.Find ((celBillboard*)bb);
  if (idx == -1) return;
  if (idx == 0) return;				// Nothing to do.
  celBillboard* cbb = billboards.Extract (idx);
  billboards.Insert (idx, cbb);
}

void celBillboardManager::StackBefore (iBillboard* bb, iBillboard* other)
{
  if (other == bb) return;
  if (billboards.Length () <= 1) return;	// Nothing to do.
  int idx_other = billboards.Find ((celBillboard*)other);
  if (idx_other == -1) return;
  int idx = billboards.Find ((celBillboard*)bb);
  if (idx == -1) return;
  celBillboard* cbb = billboards.Extract (idx);
  idx_other = billboards.Find ((celBillboard*)other);
  if (idx_other == billboards.Length ()-1)
    billboards.Push (cbb);
  else
    billboards.Insert (idx_other+1, cbb);
}

void celBillboardManager::StackAfter (iBillboard* bb, iBillboard* other)
{
  if (other == bb) return;
  if (billboards.Length () <= 1) return;	// Nothing to do.
  int idx_other = billboards.Find ((celBillboard*)other);
  if (idx_other == -1) return;
  int idx = billboards.Find ((celBillboard*)bb);
  if (idx == -1) return;
  celBillboard* cbb = billboards.Extract (idx);
  idx_other = billboards.Find ((celBillboard*)other);
  billboards.Insert (idx_other, cbb);
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
  if (billboard == moving_billboard) moving_billboard = 0;
}

void celBillboardManager::RemoveAll ()
{
  billboards.DeleteAll ();
  billboards_hash.DeleteAll ();
  moving_billboard = 0;
}

void celBillboardManager::SetFlags (uint32 flags, uint32 mask)
{
  int i;
  for (i = 0 ; i < billboards.Length () ; i++)
    billboards[i]->GetFlags ().Set (flags, mask);
}

//---------------------------------------------------------------------------

