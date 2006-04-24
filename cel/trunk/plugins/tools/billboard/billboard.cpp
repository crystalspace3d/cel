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
#include "csutil/dirtyaccessarray.h"
#include "csutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "igraphic/image.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/material.h"
#include "ivideo/texture.h"
#include "ivideo/txtmgr.h"
#include "iengine/engine.h"
#include "iengine/material.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"

#include "plugins/tools/billboard/billboard.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celBillboardLayer)
  SCF_IMPLEMENTS_INTERFACE (iBillboardLayer)
SCF_IMPLEMENT_IBASE_END

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celBillboard)
  SCF_IMPLEMENTS_INTERFACE (iBillboard)
SCF_IMPLEMENT_IBASE_END

celBillboard::celBillboard (celBillboardManager* mgr, celBillboardLayer* layer)
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
  material_ok = false;
  clickmap = 0;
  color.Set (1, 1, 1);
  uv_topleft.Set (0, 0);
  uv_botright.Set (1, 1);

  firing_messages = false;
  delete_me = false;
  is_moving = false;

  celBillboard::layer = layer;

  SetText (0);
  SetTextOffset (0, 0);
  do_fg_color = false;
  do_bg_color = false;
}

celBillboard::~celBillboard ()
{
  if (is_moving)
    mgr->RemoveMovingBillboard (this);
  delete[] name;
  delete[] materialname;
  delete[] clickmap;
  SCF_DESTRUCT_IBASE ();
}

void celBillboard::GetRect (csRect& r)
{
  int x = bb_x + layer->bb_layer_x;
  int y = bb_y + layer->bb_layer_y;
  mgr->BillboardToScreenspace (x, y);
  int w = bb_w;
  int h = bb_h;
  mgr->BillboardToScreenspace (w, h);
  r.Set (x, y, x+w-1, y+h-1);
}

void celBillboard::TranslateScreenToTexture (int sx, int sy, int& tx, int& ty)
{
  int x = bb_x + layer->bb_layer_x;
  int y = bb_y + layer->bb_layer_y;
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

  tx = (int)(tx * (uv_botright.x - uv_topleft.x) + uv_topleft.x * image_w);
  ty = (int)(ty * (uv_botright.y - uv_topleft.y) + uv_topleft.y * image_h);
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
  if (material_ok) return;

  if (!materialname)
  {
    // There is no material specified. This is ok.
    material_ok = true;
    material = 0;
    return;
  }

  if (!material)
  {
    material = mgr->engine->FindMaterial (materialname);
    if (!material) return;
    material->Visit ();
  }

  if (!has_clickmap)
  {
    // We don't have a clickmap yet so we try to find our image first.
    // Later we might get the correct dimensions and then we can
    // calculate the clickmap.
    if (!image)
    {
      csRef<iMaterialEngine> mateng = SCF_QUERY_INTERFACE (
    	  material->GetMaterial (), iMaterialEngine);
      if (mateng)
      {
        iTextureWrapper* texwrap = mateng->GetTextureWrapper ();
        image = texwrap->GetImageFile ();
      }
    }
  }

  if (image_w == -1)
  {
    material->GetMaterial()->GetTexture ()->GetOriginalDimensions (
      image_w, image_h);
  }

  if (bb_w == -1)
  {
    if (image_w != -1)
    {
      bb_w = image_w * 480;
      bb_h = image_h * 640;
    }
  }

  if (!has_clickmap && image && image_w != -1)
  {
    // Some renderers scale the image for the texture. If that happens
    // then we have to correct for that here in this routine.
    int scaled_image_w = image->GetWidth ();
    int scaled_image_h = image->GetHeight ();
    has_clickmap = true;
    // Only truecolor supported.
    clickmap = new uint8 [image_h * (1 + image_w / 8)];
    memset (clickmap, 0, image_h * (1 + image_w / 8));
    if ((image->GetFormat () & CS_IMGFMT_MASK) == CS_IMGFMT_TRUECOLOR)
    {
      csRef<iMaterialEngine> mateng = SCF_QUERY_INTERFACE (
    	  material->GetMaterial (), iMaterialEngine);
      iTextureWrapper* texwrap = mateng->GetTextureWrapper ();
      int r, g, b;
      texwrap->GetKeyColor (r, g, b);
      csRGBpixel* d;
      bool completely_full = true;
      if (r != -1)
      {
	csRGBpixel* data = (csRGBpixel*)image->GetImageData ();
	int x, y;
	for (y = 0 ; y < image_h ; y++)
	{
	  int sy = y * scaled_image_h / image_h;
	  uint8* c = &clickmap[y*(1 + image_w/8)];
	  d = data + (sy*scaled_image_w);
	  if (scaled_image_w == image_w)
	    for (x = 0 ; x < image_w ; x++)
	    {
	      csRGBpixel* d2 = d+x;
	      if (d2->red != r || d2->green != g || d2->blue != b)
	        c[x/8] |= 1<<(x%8);
	      else
		completely_full = false;
	    }
	  else
	    for (x = 0 ; x < image_w ; x++)
	    {
	      int sx = x * scaled_image_w / image_w;
	      csRGBpixel* d2 = d+sx;
	      if (d2->red != r || d2->green != g || d2->blue != b)
	        c[x/8] |= 1<<(x%8);
	      else
		completely_full = false;
	    }
        }
      }
      else
      {
	csRGBpixel* data = (csRGBpixel*)image->GetImageData ();
	int x, y;
	for (y = 0 ; y < image_h ; y++)
	{
	  int sy = y * scaled_image_h / image_h;
	  uint8* c = &clickmap[y*(1 + image_w/8)];
	  d = data + (sy*scaled_image_w);
	  if (scaled_image_w == image_w)
	    for (x = 0 ; x < image_w ; x++)
	    {
	      if (d[x].alpha != 0) c[x/8] |= 1<<(x%8);
	      else completely_full = false;
	    }
	  else
	    for (x = 0 ; x < image_w ; x++)
	    {
	      int sx = x * scaled_image_w / image_w;
	      if (d[sx].alpha != 0) c[x/8] |= 1<<(x%8);
	      else completely_full = false;
	    }
        }
      }
      if (completely_full)
      {
        // Optimization. The clickmap is completely full. In
	// that case we simply remove the clickmap while keeping
	// has_clickmap true.
	delete[] clickmap;
	clickmap = 0;
      }
    }
    image = 0;	// We no longer need the image.
  }
  if (image_w != -1 && has_clickmap && bb_w != -1 && material)
    material_ok = true;
}

void celBillboard::SetText (const char* txt)
{
  if (txt) text = txt;
  else text.Empty ();
}

void celBillboard::SetTextOffset (int dx, int dy)
{
  mgr->BillboardToScreenspace (dx, dy);
  text_dx = dx;
  text_dy = dy;
}

bool celBillboard::SetTextFont (const char* fontname, float fontsize)
{
  font_name = fontname;
  font_size = fontsize;
  iFontServer* fntsvr = mgr->GetGraphics3D ()
  	->GetDriver2D ()->GetFontServer ();
  font = fntsvr->LoadFont (fontname, fontsize);
  return font != 0;
}

void celBillboard::SetTextFgColor (const csColor& color)
{
  do_fg_color = true;
  fg_color_remember = color;
  fg_color = mgr->GetGraphics3D ()->GetDriver2D ()
    	->FindRGB (int (color.red * 255.0f),
	    int (color.green * 255.0f),
	    int (color.blue * 255.0f));
}

void celBillboard::ClearTextFgColor ()
{
  do_fg_color = false;
}

void celBillboard::SetTextBgColor (const csColor& color)
{
  do_bg_color = true;
  bg_color_remember = color;
  bg_color = mgr->GetGraphics3D ()->GetDriver2D ()
    	->FindRGB (int (color.red * 255.0f),
	    int (color.green * 255.0f),
	    int (color.blue * 255.0f));
}

void celBillboard::SetTextBgTransparent ()
{
  do_bg_color = true;
  bg_color = -1;
}

void celBillboard::ClearTextBgColor ()
{
  do_bg_color = false;
}

bool celBillboard::SetMaterialName (const char* matname)
{
  delete[] materialname;
  materialname = csStrNew (matname);
  material = 0;
  delete[] clickmap;
  clickmap = 0;
  has_clickmap = false;
  material_ok = false;
  SetupMaterial ();
  return true;
}

bool celBillboard::SetMaterialNameFast (const char* matname)
{
  delete[] materialname;
  materialname = csStrNew (matname);
  material = 0;
  material_ok = false;
  SetupMaterial ();
  return true;
}

bool celBillboard::DrawMesh (const char* material_name,
	const char* factory, float distance)
{
  iMaterialWrapper* test = mgr->engine->FindMaterial (material_name);
  if (test) { return SetMaterialName (material_name); }

  iSector* showroom = mgr->GetShowroom ();
  csMeshOnTexture* mesh_on_texture = mgr->GetMeshOnTexture ();
  iMeshFactoryWrapper* factwrap = mgr->engine->FindMeshFactory (factory);
  if (!factwrap) // @@@ Error report
    return false;
  csRef<iMeshWrapper> mesh = mgr->engine->CreateMeshWrapper (factwrap,
  	"__bbmesh__", showroom);
  if (!mesh) // @@@ Error report
    return false;

  iTextureHandle* handle;
  if (!material_ok || material == 0)
  {
    int iw, ih;
    GetImageSize (iw, ih);
    iTextureWrapper* txt = mgr->engine->CreateBlackTexture (material_name,
    	iw, ih, 0, CS_TEXTURE_2D | CS_TEXTURE_3D | CS_TEXTURE_NOMIPMAPS);
    txt->SetKeepImage (true);
    material = mgr->engine->CreateMaterial (material_name, txt);
    iTextureManager *txtmgr = mgr->GetGraphics3D ()->GetTextureManager ();
    if (!txt->GetTextureHandle ()) txt->Register (txtmgr);
    handle = txt->GetTextureHandle ();
  }
  else
  {
    csRef<iMaterialEngine> mateng = scfQueryInterface<iMaterialEngine> (
    	material->GetMaterial ());
    handle = mateng->GetTextureWrapper ()->GetTextureHandle ();
  }

  mesh_on_texture->ScaleCamera (mesh, distance);
  bool rc = mesh_on_texture->Render (mesh, handle, true);

  mgr->engine->RemoveObject (mesh);

  if (!rc) return false;
  if (!material_ok || material == 0)
    return SetMaterialName (material_name);
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
  iw = iw * 480 / mgr->screen_w_fact;
  ih = ih * 640 / mgr->screen_h_fact;
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

void celBillboard::MoveToPosition (csTicks delta, int x, int y)
{
  mgr->MoveToPosition (this, delta, x, y);
}

void celBillboard::SetPositionScreen (int x, int y)
{
  mgr->ScreenToBillboardSpace (x, y);
  bb_x = x - layer->bb_layer_x;
  bb_y = y - layer->bb_layer_y;
}

void celBillboard::GetPositionScreen (int& x, int& y) const
{
  x = bb_x + layer->bb_layer_x;
  y = bb_y + layer->bb_layer_y;
  mgr->BillboardToScreenspace (x, y);
}

void celBillboard::Move (int dx, int dy)
{
  bb_x += dx;
  bb_y += dy;
}

void celBillboard::StackTop ()
{
  mgr->StackTop (this);
}

void celBillboard::StackBottom ()
{
  mgr->StackBottom (this);
}

void celBillboard::StackUp ()
{
  mgr->StackUp (this);
}

void celBillboard::StackDown ()
{
  mgr->StackDown (this);
}

void celBillboard::FireMouseUp (int sx, int sy, int button)
{
  mgr->ScreenToBillboardSpace (sx, sy);
  size_t i;
  firing_messages = true;
  for (i = 0 ; i < handlers.Length () ; i++)
  {
    handlers[i]->Unselect (this, button, sx, sy);
    if (delete_me)
    {
      delete this;
      return;
    }
  }
  firing_messages = false;
}

void celBillboard::FireMouseDown (int sx, int sy, int button)
{
  mgr->ScreenToBillboardSpace (sx, sy);
  size_t i;
  firing_messages = true;
  for (i = 0 ; i < handlers.Length () ; i++)
  {
    handlers[i]->Select (this, button, sx, sy);
    if (delete_me)
    {
      delete this;
      return;
    }
  }
  firing_messages = false;
}

void celBillboard::FireMouseMove (int sx, int sy, int button)
{
  mgr->ScreenToBillboardSpace (sx, sy);
  size_t i;
  firing_messages = true;
  for (i = 0 ; i < handlers.Length () ; i++)
  {
    handlers[i]->MouseMove (this, button, sx, sy);
    if (delete_me)
    {
      delete this;
      return;
    }
  }
  firing_messages = false;
}

void celBillboard::FireMouseDoubleClick (int sx, int sy, int button)
{
  mgr->ScreenToBillboardSpace (sx, sy);
  size_t i;
  firing_messages = true;
  for (i = 0 ; i < handlers.Length () ; i++)
  {
    handlers[i]->DoubleClick (this, button, sx, sy);
    if (delete_me)
    {
      delete this;
      return;
    }
  }
  firing_messages = false;
}

bool celBillboard::HasFullClickmap ()
{
  if (bb_w == -1 || !has_clickmap)
  {
    SetupMaterial ();
    if (bb_w == -1 || !has_clickmap) return false;
  }
  return clickmap == 0;
}

bool celBillboard::In (int sx, int sy)
{
  if (bb_w == -1 || !has_clickmap)
  {
    SetupMaterial ();
    if (bb_w == -1 || !has_clickmap)
      return false;
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
  return false;
}

static csSimpleRenderMesh mesh;
static bool mesh_init = false;

CS_IMPLEMENT_STATIC_VAR (GetMeshIndices, csDirtyAccessArray<uint>, ());
static size_t mesh_indices_count = 0;
CS_IMPLEMENT_STATIC_VAR (GetMeshVertices, csDirtyAccessArray<csVector3>, ());
CS_IMPLEMENT_STATIC_VAR (GetMeshTexels, csDirtyAccessArray<csVector2>, ());
CS_IMPLEMENT_STATIC_VAR (GetMeshColors, csDirtyAccessArray<csVector4>, ());

static void mesh_reset ()
{
  if (!mesh_init)
  {
    mesh_init = true;
    mesh.meshtype = CS_MESHTYPE_QUADS;
    mesh.mixmode = CS_FX_COPY;
    mesh.z_buf_mode = CS_ZBUF_FILL;
  }
  mesh_indices_count = 0;
  GetMeshVertices ()->Empty ();
  GetMeshTexels ()->Empty ();
  GetMeshColors ()->Empty ();
  mesh.texture = 0;
}

static void mesh_draw (iGraphics3D* g3d)
{
  if (mesh_indices_count <= 0) return;
  mesh.indexCount = (uint)mesh_indices_count;
  mesh.indices = GetMeshIndices ()->GetArray ();
  mesh.vertexCount = (uint)GetMeshVertices ()->Length ();
  mesh.vertices = GetMeshVertices ()->GetArray ();
  mesh.texcoords = GetMeshTexels ()->GetArray ();
  mesh.colors = GetMeshColors ()->GetArray ();
  g3d->DrawSimpleMesh (mesh);
  mesh_reset ();
}

void celBillboard::Draw (iGraphics3D* g3d, float z)
{
  if (!flags.Check (CEL_BILLBOARD_VISIBLE)) return;

  int fw = g3d->GetWidth ();
  int fh = g3d->GetHeight ();
  csRect r;
  GetRect (r);
  if (r.xmax <= 0 || r.xmin >= fw-1) return;
  if (r.ymax <= 0 || r.ymin >= fh-1) return;

  SetupMaterial ();
  if (!material) return;
  material->Visit ();
  iTextureHandle* txt = material->GetMaterial()->GetTexture ();
  if (txt != mesh.texture)
  {
    mesh_draw (g3d);
    mesh.texture = txt;
  }

  csVector2 uvtl = uv_topleft;
  csVector2 uvbr = uv_botright;
  if (r.xmin < 0)
  {
    int dx = r.xmax - r.xmin;
    float rr = float (-r.xmin) / float (dx);
    uvtl.x = (1-rr) * uvtl.x + uvbr.x * rr;
    r.xmin = 0;
  }
  else if (r.xmax >= fw)
  {
    int dx = r.xmax - r.xmin;
    float rr = float (r.xmax - fw+1) / float (dx);
    uvbr.x = (1-rr) * uvbr.x + uvtl.x * rr;
    r.xmax = fw-1;
  }
  if (r.ymin < 0)
  {
    int dy = r.ymax - r.ymin;
    float rr = float (-r.ymin) / float (dy);
    uvtl.y = (1-rr) * uvtl.y + uvbr.y * rr;
    r.ymin = 0;
  }
  else if (r.ymax >= fh)
  {
    int dy = r.ymax - r.ymin;
    float rr = float (r.ymax - fh-1) / float (dy);
    uvbr.y = (1-rr) * uvbr.y + uvtl.y * rr;
    r.ymax = fh-1;
  }

  float z_inv_aspect = z / g3d->GetPerspectiveAspect ();

  csDirtyAccessArray<uint>& mesh_indices = *GetMeshIndices ();
  csDirtyAccessArray<csVector3>& mesh_vertices = *GetMeshVertices ();
  csDirtyAccessArray<csVector2>& mesh_texels = *GetMeshTexels ();
  csDirtyAccessArray<csVector4>& mesh_colors = *GetMeshColors ();
  
  mesh_indices_count += 4;
  size_t i;
  for (i = mesh_indices.Length () ; i < mesh_indices_count ; i++)
  {
    mesh_indices.Put (i, i);
  }

  csVector3 v1 (((r.xmin) - fw/2) * z_inv_aspect,
  	        ((fh-r.ymin) - fh/2) * z_inv_aspect, z);
  csVector3 v2 (((r.xmax) - fw/2) * z_inv_aspect,
  	        ((fh-r.ymin) - fh/2) * z_inv_aspect, z);
  csVector3 v3 (((r.xmax) - fw/2) * z_inv_aspect,
  	        ((fh-r.ymax) - fh/2) * z_inv_aspect, z);
  csVector3 v4 (((r.xmin) - fw/2) * z_inv_aspect,
  	        ((fh-r.ymax) - fh/2) * z_inv_aspect, z);
  mesh_vertices.Push (v1);
  mesh_vertices.Push (v2);
  mesh_vertices.Push (v3);
  mesh_vertices.Push (v4);

  mesh_texels.Push (uvtl);
  mesh_texels.Push (csVector2 (uvbr.x, uvtl.y));
  mesh_texels.Push (uvbr);
  mesh_texels.Push (csVector2 (uvtl.x, uvbr.y));

  csVector4 c (color.red, color.green, color.blue, 1.0);
  mesh_colors.Push (c);
  mesh_colors.Push (c);
  mesh_colors.Push (c);
  mesh_colors.Push (c);
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
  default_layer = new celBillboardLayer ("default");
  layers.Push (default_layer);

  mesh_on_texture = 0;
  showroom = 0;
}

celBillboardManager::~celBillboardManager ()
{
  delete mesh_on_texture;

  if (scfiEventHandler)
  {
    csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q != 0)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celBillboardManager::Initialize (iObjectRegistry* object_reg)
{
  celBillboardManager::object_reg = object_reg;

  scfiEventHandler = new EventHandler (this);
  csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  csEventID esub[] = { 
    csevMouseEvent (object_reg),
    csevFrame (object_reg),
    csevPreProcess (object_reg),
    csevPostProcess (object_reg),
    csevProcess (object_reg),
    CS_EVENTLIST_END 
  };
  q->RegisterListener (scfiEventHandler, esub);

  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  name_reg = csEventNameRegistry::GetRegistry (object_reg);

  screen_w_fact = BSX / g3d->GetWidth ();
  screen_h_fact = BSY / g3d->GetHeight ();

  if (!SetDefaultTextFont (CSFONT_COURIER, 10.0f))
    return false;
  default_fg_color_remember.Set (1, 1, 1);
  default_fg_color = g3d->GetDriver2D ()->FindRGB (255, 255, 255);
  default_bg_color = -1;

  return true;
}

size_t celBillboardManager::FindMovingBillboard (celBillboard* bb)
{
  size_t i;
  for (i = 0 ; i < moving_billboards.Length () ; i++)
  {
    if (bb == moving_billboards[i].bb) return i;
  }
  return csArrayItemNotFound;
}

void celBillboardManager::HandleMovingBillboards (csTicks elapsed)
{
  size_t i = moving_billboards.Length ();
  while (i-- > 0)
  {
    movingBillboard& mbb = moving_billboards[i];
    mbb.delta -= elapsed;
    if (mbb.delta <= 0)
    {
      mbb.bb->SetPosition (int (mbb.dstx), int (mbb.dsty));
      mbb.bb->is_moving = false;
      moving_billboards.DeleteIndex (i);
    }
    else
    {
      float d = float (mbb.delta) / mbb.tot_delta;
      mbb.bb->SetPosition (
      	int ((1.0-d) * mbb.dstx + d * mbb.srcx),
      	int ((1.0-d) * mbb.dsty + d * mbb.srcy));
    }
  }
}

void celBillboardManager::RemoveMovingBillboard (celBillboard* bb)
{
  size_t i = FindMovingBillboard (bb);
  moving_billboards.DeleteIndex (i);
  bb->is_moving = false;
}

void celBillboardManager::MoveToPosition (celBillboard* bb, csTicks delta,
	int x, int y)
{
  if (bb->is_moving)
  {
    size_t i = FindMovingBillboard (bb);
    CS_ASSERT (i != csArrayItemNotFound);
    if (delta == 0)
    {
      moving_billboards.DeleteIndex (i);
      bb->is_moving = false;
      bb->SetPosition (x, y);
      return;
    }
    movingBillboard& mbb = moving_billboards[i];
    mbb.delta = delta;
    mbb.tot_delta = float (delta);
    int sx, sy;
    bb->GetPosition (sx, sy);
    mbb.srcx = float (sx);
    mbb.srcy = float (sy);
    mbb.dstx = float (x);
    mbb.dsty = float (y);
    return;
  }
  if (delta == 0)
  {
    bb->SetPosition (x, y);
    return;
  }

  bb->is_moving = true;
  movingBillboard mbb;
  mbb.bb = bb;
  mbb.delta = delta;
  mbb.tot_delta = float (delta);
  int sx, sy;
  bb->GetPosition (sx, sy);
  mbb.srcx = float (sx);
  mbb.srcy = float (sy);
  mbb.dstx = float (x);
  mbb.dsty = float (y);
  moving_billboards.Push (mbb);
}

celBillboard* celBillboardManager::FindBillboard (int x, int y,
	uint32 desired_flags)
{
  // @@@ OPTIMIZE WITH SOME KIND OF HIERARCHICAL BBOXES.
  // @@@ KEEP Z-ORDER IN MIND!
  size_t i = billboards.Length ();
  while (i > 0)
  {
    i--;
    csFlags& f = billboards[i]->GetFlags ();
    if (f.Check (CEL_BILLBOARD_CLICKABLE | CEL_BILLBOARD_MOVABLE))
    {
      if (billboards[i]->In (x, y))
        return billboards[i];
    }
  }
  return 0;
}

bool celBillboardManager::SetDefaultTextFont (const char* fontname,
    float fontsize)
{
  iFontServer* fntsvr = g3d->GetDriver2D ()->GetFontServer ();
  default_font = fntsvr->LoadFont (fontname, fontsize);
  return default_font != 0;
}

void celBillboardManager::SetDefaultTextFgColor (const csColor& color)
{
  default_fg_color_remember = color;
  default_fg_color = g3d->GetDriver2D ()
    	->FindRGB (int (color.red * 255.0f),
	    int (color.green * 255.0f),
	    int (color.blue * 255.0f));
}

void celBillboardManager::SetDefaultTextBgColor (const csColor& color)
{
  default_bg_color_remember = color;
  default_bg_color = g3d->GetDriver2D ()
    	->FindRGB (int (color.red * 255.0f),
	    int (color.green * 255.0f),
	    int (color.blue * 255.0f));
}

void celBillboardManager::SetDefaultTextBgTransparent ()
{
  default_bg_color = -1;
}

bool celBillboardManager::HandleEvent (iEvent& ev)
{
  if (ev.Name == csevPreProcess (object_reg))
  {
    HandleMovingBillboards (vc->GetElapsedTicks ());
  }
  else if (ev.Name == csevPostProcess (object_reg))
  {
    if (billboards.Length () > 0)
    {
      g3d->BeginDraw (CSDRAW_3DGRAPHICS);
      mesh_reset ();
      size_t i;
      float z = z_max;
      float dz = (z_max-z_min) / float (billboards.Length ());
      g3d->SetWorldToCamera (csReversibleTransform ());
      for (i = 0 ; i < billboards.Length () ; i++)
      {
	celBillboard* bb = billboards[i];
        bb->Draw (g3d, z);
	const char* t = bb->GetText ();
	if (t)
	{
	  iFont* font = bb->GetFont ();
	  if (!font) font = default_font;
	  if (font)
	  {
	    mesh_draw (g3d);
	    g3d->BeginDraw (CSDRAW_2DGRAPHICS);
	    csRect r;
	    bb->GetRect (r);
	    int fg = bb->UseTextFgColor () ? bb->GetTextFgColorNum () :
		  	default_fg_color;
	    int bg = bb->UseTextBgColor () ? bb->GetTextBgColorNum () :
		  	default_bg_color;
	    g3d->GetDriver2D ()->Write (font,
			r.xmin+bb->GetTextDX (), r.ymin+bb->GetTextDY (),
			fg, bg, t);
	    g3d->BeginDraw (CSDRAW_3DGRAPHICS);
	  }
	}
	z -= dz;
      }
      mesh_draw (g3d);
    }
  }
  else if (ev.Name == csevMouseUp (name_reg, 0))
  {
    if (moving_billboard)
    {
      moving_billboard->SetPositionScreen (
	  	csMouseEventHelper::GetX(&ev) + moving_dx,
		csMouseEventHelper::GetY(&ev) + moving_dy);
      moving_billboard = 0;
    }

    celBillboard* bb = FindBillboard (csMouseEventHelper::GetX(&ev), 
					  csMouseEventHelper::GetY(&ev),
					  CEL_BILLBOARD_CLICKABLE);
    if (bb)
      bb->FireMouseUp (
	  	csMouseEventHelper::GetX(&ev), csMouseEventHelper::GetY(&ev), 
		csMouseEventHelper::GetButton(&ev));
  }
  else if (ev.Name == csevMouseDown (name_reg, 0))
  {
    celBillboard* bb = FindBillboard (csMouseEventHelper::GetX(&ev), 
					  csMouseEventHelper::GetY(&ev),
					  CEL_BILLBOARD_CLICKABLE |
					  CEL_BILLBOARD_MOVABLE);
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
	moving_dx -= csMouseEventHelper::GetX(&ev);
	moving_dy -= csMouseEventHelper::GetY(&ev);
      }
      if (bb->GetFlags ().Check (CEL_BILLBOARD_CLICKABLE))
	bb->FireMouseDown (csMouseEventHelper::GetX(&ev), 
			       csMouseEventHelper::GetY(&ev), 
			       csMouseEventHelper::GetButton(&ev));
    }
  }
  else if (ev.Name == csevMouseMove (name_reg, 0))
  {
    if (moving_billboard)
    {
      moving_billboard->SetPositionScreen (
	  	csMouseEventHelper::GetX(&ev) + moving_dx,
		csMouseEventHelper::GetY(&ev) + moving_dy);
    }

    celBillboard* bb = FindBillboard (csMouseEventHelper::GetX(&ev), 
					  csMouseEventHelper::GetY(&ev),
					  CEL_BILLBOARD_CLICKABLE);
    if (bb)
      bb->FireMouseMove (csMouseEventHelper::GetX(&ev),
	  		     csMouseEventHelper::GetY(&ev), 
			     csMouseEventHelper::GetButton(&ev));
  }
  else if (ev.Name == csevMouseDoubleClick (name_reg, 0))
  {
    celBillboard* bb = FindBillboard (csMouseEventHelper::GetX(&ev),
					  csMouseEventHelper::GetY(&ev),
					  CEL_BILLBOARD_CLICKABLE);
    if (bb)
      bb->FireMouseDoubleClick (csMouseEventHelper::GetX(&ev), 
				    csMouseEventHelper::GetY(&ev), 
				    csMouseEventHelper::GetButton(&ev));
  }
  return false;
}

void celBillboardManager::StackTop (iBillboard* bb)
{
  size_t idx = billboards.Find ((celBillboard*)bb);
  if (idx == csArrayItemNotFound) return;
  if (idx == billboards.Length ()-1) return;	// Nothing to do.
  celBillboard* cbb = billboards.Extract (idx);
  billboards.Push (cbb);
}

void celBillboardManager::StackBottom (iBillboard* bb)
{
  size_t idx = billboards.Find ((celBillboard*)bb);
  if (idx == csArrayItemNotFound) return;
  if (idx == 0) return; 			// Nothing to do.
  celBillboard* cbb = billboards.Extract (idx);
  billboards.Insert (1, cbb);
}

void celBillboardManager::StackUp (iBillboard* bb)
{
  if (billboards.Length () <= 1) return;	// Nothing to do.
  size_t idx = billboards.Find ((celBillboard*)bb);
  if (idx == csArrayItemNotFound) return;
  if (idx == billboards.Length ()-1) return;	// Nothing to do.
  celBillboard* cbb = billboards.Extract (idx);
  billboards.Insert (idx+1, cbb);
}

void celBillboardManager::StackDown (iBillboard* bb)
{
  if (billboards.Length () <= 1) return;	// Nothing to do.
  size_t idx = billboards.Find ((celBillboard*)bb);
  if (idx == csArrayItemNotFound) return;
  if (idx == 0) return;				// Nothing to do.
  celBillboard* cbb = billboards.Extract (idx);
  billboards.Insert (idx, cbb);
}

void celBillboardManager::StackBefore (iBillboard* bb, iBillboard* other)
{
  if (other == bb) return;
  if (billboards.Length () <= 1) return;	// Nothing to do.
  size_t idx_other = billboards.Find ((celBillboard*)other);
  if (idx_other == csArrayItemNotFound) return;
  size_t idx = billboards.Find ((celBillboard*)bb);
  if (idx == csArrayItemNotFound) return;
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
  size_t idx_other = billboards.Find ((celBillboard*)other);
  if (idx_other == csArrayItemNotFound) return;
  size_t idx = billboards.Find ((celBillboard*)bb);
  if (idx == csArrayItemNotFound) return;
  celBillboard* cbb = billboards.Extract (idx);
  idx_other = billboards.Find ((celBillboard*)other);
  billboards.Insert (idx_other, cbb);
}


iBillboard* celBillboardManager::CreateBillboard (const char* name)
{
  celBillboard* bb = new celBillboard (this, default_layer);
  bb->SetName (name);
  billboards.Push (bb);
  billboards_hash.Put (name, bb);
  return bb;
}

iBillboard* celBillboardManager::FindBillboard (const char* name) const
{
  return billboards_hash.Get (name, 0);
}

void celBillboardManager::RemoveBillboard (iBillboard* billboard)
{
  billboards_hash.Delete (billboard->GetName (), (celBillboard*)billboard);
  celBillboard* celbb = (celBillboard*)billboard;
  if (celbb->firing_messages)
  {
    // We are busy firing messages. Do not immediatelly delete
    // the billboard but just set the 'delete_me' flag.
    billboards.Extract (billboards.Find ((celBillboard*)billboard));
    celbb->delete_me = true;
  }
  else
  {
    billboards.Delete ((celBillboard*)billboard);
  }
  if (billboard == moving_billboard) moving_billboard = 0;
}

iBillboardLayer* celBillboardManager::CreateBillboardLayer (const char* name)
{
  celBillboardLayer* layer = new celBillboardLayer (name);
  layers.Push (layer);
  return layer;
}

iBillboardLayer* celBillboardManager::FindBillboardLayer (const char* name) const
{
  size_t i;
  for (i = 0 ; i < layers.Length () ; i++)
    if (!strcmp (layers[i]->GetName (), name))
      return layers[i];
  return 0;
}

void celBillboardManager::RemoveBillboardLayer (iBillboardLayer* layer)
{
  if (layer == default_layer) return;	// Not allowed!
  size_t i;
  for (i = 0 ; i < billboards.Length () ; i++)
  {
    if (billboards[i]->GetLayer () == layer)
      billboards[i]->SetLayer (default_layer);
  }
  layers.Delete ((celBillboardLayer*)layer);
}

void celBillboardManager::RemoveAll ()
{
  billboards.DeleteAll ();
  billboards_hash.DeleteAll ();
  moving_billboard = 0;
}

void celBillboardManager::SetFlags (uint32 flags, uint32 mask)
{
  size_t i;
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
  if (cbb1->HasFullClickmap () && cbb2->HasFullClickmap ()) return true;
  if (cbb1->HasFullClickmap ())
  {
    int x, y;
    for (x = r1.xmin ; x <= r1.xmax ; x++)
      for (y = r1.ymin ; y <= r1.ymax ; y++)
        if (cbb2->In (x, y))
          return true;
  }
  else if (cbb2->HasFullClickmap ())
  {
    int x, y;
    for (x = r1.xmin ; x <= r1.xmax ; x++)
      for (y = r1.ymin ; y <= r1.ymax ; y++)
        if (cbb1->In (x, y))
          return true;
  }
  else
  {
    int x, y;
    for (x = r1.xmin ; x <= r1.xmax ; x++)
      for (y = r1.ymin ; y <= r1.ymax ; y++)
        if (cbb1->In (x, y) && cbb2->In (x, y))
          return true;
  }

  return false;
}

csMeshOnTexture* celBillboardManager::GetMeshOnTexture ()
{
  GetShowroom ();
  return mesh_on_texture;
}

iSector* celBillboardManager::GetShowroom ()
{
  if (showroom) return showroom;
  delete mesh_on_texture;
  mesh_on_texture = new csMeshOnTexture (object_reg);
  showroom = engine->CreateSector ("cel.billboardmanager.showroom");

  csRef<iLight> light;
  iLightList* ll = showroom->GetLights ();

  light = engine->CreateLight (0, csVector3(-10, 3, 0), 50, csColor(1, 1, 1));
  ll->Add (light);
  light = engine->CreateLight (0, csVector3(10, 3, 0), 50, csColor(1, 1, 1));
  ll->Add (light);
  return showroom;
}

//---------------------------------------------------------------------------
