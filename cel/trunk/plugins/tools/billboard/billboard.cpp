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

celBillboard::celBillboard (celBillboardManager* mgr)
{
  SCF_CONSTRUCT_IBASE (0);
  name = 0;
  flags.SetAll (CEL_BILLBOARD_VISIBLE);
  materialname = 0;
  material = 0;
  image_w = image_h = -1;
  bb_x = bb_y = 0;
  bb_w = bb_h = -1;
  celBillboard::mgr = mgr;
  has_clickmap = false;
  clickmap = 0;
  color.Set (1, 1, 1);
}

celBillboard::~celBillboard ()
{
  delete[] name;
  delete[] materialname;
  delete[] clickmap;
}

void celBillboard::GetRect (csRect& r)
{
  int x = bb_x;
  int y = bb_y;
  mgr->BillboardToScreenspace (x, y);
  int w = bb_w;
  int h = bb_h;
  mgr->BillboardToScreenspace (w, h);
  r.Set (x, y, x+w-1, y+h-1);
}

void celBillboard::TranslateScreenToTexture (int sx, int sy, int& tx, int& ty)
{
  int x = bb_x;
  int y = bb_y;
  mgr->BillboardToScreenspace (x, y);
  int w = bb_w;
  int h = bb_h;
  mgr->BillboardToScreenspace (w, h);
  tx = sx-x;
  ty = sy-y;
  if (bb_w != image_w)
  {
    tx = (tx * image_w) / w;
    if (tx >= image_w) tx = image_w-1;
  }
  if (bb_h != image_h)
  {
    ty = (ty * image_h) / h;
    if (ty >= image_h) ty = image_h-1;
  }
}

bool celBillboard::GetFromClickMap (int tx, int ty)
{
  if (!has_clickmap)
    SetupMaterial ();
  if (!clickmap) return true;
  uint8 c = clickmap[ty*(1 + image_w/8) + tx/8];
  return (c & (1<<(tx%8))) != 0;
}

void celBillboard::SetClickMap (int tx, int ty, bool v)
{
  if (!clickmap) return;
  uint8& c = clickmap[ty*(1 + image_w/8) + tx/8];
  uint8 mask = 1<<(tx%8);
  if (v) c |= mask;
  else c &= ~mask;
}

void celBillboard::SetupMaterial ()
{
  if (!material)
  {
    material = mgr->engine->FindMaterial (materialname);
    if (!material) return;
    material->Visit ();
  }

  if (image_w == -1)
  {
    if (material->GetMaterialHandle ())
    {
      material->GetMaterialHandle ()->GetTexture ()->GetOriginalDimensions (
    	image_w, image_h);
    }
  }

  if (bb_w == -1)
  {
    if (image_w != -1)
    {
      bb_w = image_w * 1600;
      bb_h = image_h * 1280;
    }
  }

  if (!has_clickmap)
  {
    csRef<iMaterialEngine> mateng = SCF_QUERY_INTERFACE (
    	material->GetMaterial (), iMaterialEngine);
    if (mateng)
    {
      iTextureWrapper* texwrap = mateng->GetTextureWrapper ();
      iImage* image = texwrap->GetImageFile ();
      if (image)
      {
        image_w = image->GetWidth ();
        image_h = image->GetHeight ();
        has_clickmap = true;
        // Only truecolor supported.
        clickmap = new uint8 [image_h * (1 + image_w / 8)];
	memset (clickmap, 0, image_h * (1 + image_w / 8));
        if ((image->GetFormat () & CS_IMGFMT_MASK) == CS_IMGFMT_TRUECOLOR)
	{
	  int r, g, b;
	  texwrap->GetKeyColor (r, g, b);
          if (r != -1)
	  {
	    csRGBpixel* data = (csRGBpixel*)image->GetImageData ();
	    int x, y;
	    for (y = 0 ; y < image_h ; y++)
	      for (x = 0 ; x < image_w ; x++)
	      {
	        SetClickMap (x, y, data->red != r || data->green != g ||
			data->blue != b);
	        data++;
	      }
	  }
	  else
	  {
	    csRGBpixel* data = (csRGBpixel*)image->GetImageData ();
	    int x, y;
	    for (y = 0 ; y < image_h ; y++)
	      for (x = 0 ; x < image_w ; x++)
	      {
	        SetClickMap (x, y, data->alpha != 0);
		data++;
	      }
	  }
        }
      }
    }
  }
}

bool celBillboard::SetMaterialName (const char* matname)
{
  delete[] materialname;
  materialname = csStrNew (matname);
  material = 0;
  delete[] clickmap;
  clickmap = 0;
  has_clickmap = false;
  SetupMaterial ();
  return true;
}

void celBillboard::GetSize (int& w, int& h)
{
  if (bb_w == -1) SetupMaterial ();
  w = bb_w;
  h = bb_h;
}

void celBillboard::GetImageSize (int& iw, int& ih)
{
  if (image_w == -1) SetupMaterial ();
  iw = celBillboard::image_w;
  ih = celBillboard::image_h;
  mgr->ScreenToBillboardSpace (iw, ih);
  iw = iw * 1600 / mgr->screen_w_fact;
  ih = ih * 1280 / mgr->screen_h_fact;
}

void celBillboard::SetSize (int w, int h)
{
  bb_w = w;
  bb_h = h;
}

void celBillboard::SetPosition (int x, int y)
{
  bb_x = x;
  bb_y = y;
}

void celBillboard::GetPosition (int& x, int& y) const
{
  x = bb_x;
  y = bb_y;
}

void celBillboard::SetPositionScreen (int x, int y)
{
  mgr->ScreenToBillboardSpace (x, y);
  bb_x = x;
  bb_y = y;
}

void celBillboard::GetPositionScreen (int& x, int& y) const
{
  x = bb_x;
  y = bb_y;
  mgr->BillboardToScreenspace (x, y);
}

void celBillboard::Move (int dx, int dy)
{
  bb_x += dx;
  bb_y += dy;
}

void celBillboard::FireMouseUp (int sx, int sy, int button)
{
  mgr->ScreenToBillboardSpace (sx, sy);
  int i;
  for (i = 0 ; i < handlers.Length () ; i++)
    handlers[i]->Unselect (this, sx, sy, button);
}

void celBillboard::FireMouseDown (int sx, int sy, int button)
{
  mgr->ScreenToBillboardSpace (sx, sy);
  int i;
  for (i = 0 ; i < handlers.Length () ; i++)
    handlers[i]->Select (this, sx, sy, button);
}

void celBillboard::FireMouseMove (int sx, int sy, int button)
{
  mgr->ScreenToBillboardSpace (sx, sy);
  int i;
  for (i = 0 ; i < handlers.Length () ; i++)
    handlers[i]->MouseMove (this, sx, sy, button);
}

void celBillboard::FireMouseDoubleClick (int sx, int sy, int button)
{
  mgr->ScreenToBillboardSpace (sx, sy);
  int i;
  for (i = 0 ; i < handlers.Length () ; i++)
    handlers[i]->DoubleClick (this, sx, sy, button);
}


bool celBillboard::In (int sx, int sy)
{
  if (bb_w == -1 || !has_clickmap)
  {
    SetupMaterial ();
    if (bb_w == -1 || !has_clickmap) return false;
  }
  csRect r;
  GetRect (r);
  if (sx >= r.xmin && sx <= r.xmax && sy >= r.ymin && sy <= r.ymax)
  {
    int tx, ty;
    TranslateScreenToTexture (sx, sy, tx, ty);
    if (tx < 0 || tx >= image_w || ty < 0 || ty >= image_h)
      return false;
    return GetFromClickMap (tx, ty);
  }
  else
    return false;
}

static G3DPolygonDPFX poly;
static bool poly_init = false;

void celBillboard::Draw (iGraphics3D* g3d, float z)
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
    poly.use_fog = false;
    poly.mixmode = CS_FX_COPY | CS_FX_GOURAUD;
  }
  SetupMaterial ();
  if (!material) return;
  material->Visit ();
  poly.mat_handle = material->GetMaterialHandle ();
  int fh = g3d->GetHeight ();
  csRect r;
  GetRect (r);
  poly.vertices[0].Set (r.xmin, fh-r.ymin);
  poly.vertices[1].Set (r.xmax, fh-r.ymin);
  poly.vertices[2].Set (r.xmax, fh-r.ymax);
  poly.vertices[3].Set (r.xmin, fh-r.ymax);
  poly.colors[0] = color;
  poly.colors[1] = color;
  poly.colors[2] = color;
  poly.colors[3] = color;
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

  screen_w_fact = 1024000 / g3d->GetWidth ();
  screen_h_fact = 614400 / g3d->GetHeight ();

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
    {
      if (billboards[i]->In (x, y))
        return billboards[i];
    }
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
            billboards[i]->Draw (g3d, z);
	    z -= dz;
          }
        }
      }
      break;
    case csevMouseUp:
      {
        if (moving_billboard)
	{
	  moving_billboard->SetPositionScreen (ev.Mouse.x + moving_dx,
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
	    bb->GetPositionScreen (moving_dx, moving_dy);
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
	  moving_billboard->SetPositionScreen (ev.Mouse.x + moving_dx,
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
  celBillboard* bb = new celBillboard (this);
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

bool celBillboardManager::TestCollision (iBillboard* bb1, iBillboard* bb2)
{
  csRect r1, r2;
  celBillboard* cbb1 = (celBillboard*)bb1;
  celBillboard* cbb2 = (celBillboard*)bb2;
  cbb1->GetRect (r1);
  cbb2->GetRect (r2);
  r1.Intersect (r2);
  if (r1.IsEmpty ()) return false;

  // Test transparent bits...
  // @@@ Not very optimal. In future we should use some kind of
  // quadtree to help this.
  int x, y;
  for (x = r1.xmin ; x <= r1.xmax ; x++)
    for (y = r1.ymin ; y <= r1.ymax ; y++)
      if (cbb1->In (x, y) && cbb2->In (x, y))
        return true;

  return false;
}

//---------------------------------------------------------------------------

