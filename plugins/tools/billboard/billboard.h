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

#ifndef __CEL_TOOLS_BILLBOARD__
#define __CEL_TOOLS_BILLBOARD__

#include "csutil/util.h"
#include "csutil/hash.h"
#include "csutil/parray.h"
#include "csutil/refarr.h"
#include "csutil/strhash.h"
#include "csutil/cscolor.h"
#include "cstool/procmesh.h"
#include "csgeom/csrect.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/billboard.h"
#include "ivideo/fontserv.h"

struct iObjectRegistry;
struct iImage;
struct iEngine;
struct iGraphics3D;
struct iEvent;

class celBillboardManager;

// Billboard space definition. This number is divisible
// by 320, 480, 640, 600, 800, 1024, 1280, and 1600 and is
// therefor very suitable for representing screen based locations
// in a resolution independent way.
#define BSX 307200
#define BSY 307200

/**
 * A layer for the billboard manager. Several billboards can
 * be in one layer. You can move layers independently.
 */
class celBillboardLayer : public iBillboardLayer
{
public:
  int bb_layer_x, bb_layer_y;
  char* name;

public:
  celBillboardLayer (const char* name)
  {
    SCF_CONSTRUCT_IBASE (0);
    bb_layer_x = 0;
    bb_layer_y = 0;
    celBillboardLayer::name = csStrNew (name);
  }

  virtual ~celBillboardLayer ()
  {
    delete[] name;
    SCF_DESTRUCT_IBASE ();
  }

  SCF_DECLARE_IBASE;

  virtual void GetOffset (int& x, int& y) const
  {
    x = bb_layer_x;
    y = bb_layer_y;
  }

  virtual void SetOffset (int x, int y)
  {
    bb_layer_x = x;
    bb_layer_y = y;
  }

  virtual void Move (int dx, int dy)
  {
    bb_layer_x += dx;
    bb_layer_y += dy;
  }

  virtual const char* GetName () const { return name; }
};

/**
 * A billboard.
 */
class celBillboard : public iBillboard
{
  friend class celBillboardManager;

private:
  char* name;
  csFlags flags;
  char* materialname;
  csRef<iMaterialWrapper> material;
  int image_w, image_h;
  // Coordinates in billboard space.
  int bb_x, bb_y, bb_w, bb_h;

  //Rotation of the billboard.
  float rotation;
  csRefArray<iBillboardEventHandler> handlers;
  celBillboardManager* mgr;
  celBillboardLayer* layer;
  csColor color;
  bool is_moving;	// Billboard is in the 'moving_billboards' array.

  // If the following flag is true an attempt to delete this billboard
  // we instead set the 'delete_me' flag. This is to make sure that
  // the routine firing messages will interrupt itself properly as
  // soon as the billboard wants to be deleted.
  bool firing_messages;
  bool delete_me;

  csVector2 uv_topleft;
  csVector2 uv_botright;

  csRef<iImage> image;	// Ref to the image. Is cleared when no longer needed.
  bool has_clickmap;	// If true we have a clickmap.
  uint8* clickmap;	// A clickmap. Can be null in case all values are true.
  bool material_ok;

  void SetupMaterial ();
  // Input: texture coordinates.
  bool GetFromClickMap (int tx, int ty);
  // Input: texture coordinates.
  void SetClickMap (int tx, int ty, bool v);
  void TranslateScreenToTexture (int sx, int sy, int& tx, int& ty);

  csString text;
  int text_dx, text_dy;
  csString font_name;
  float font_size;
  csRef<iFont> font;
  csColor fg_color_remember;
  csColor bg_color_remember;
  int fg_color, bg_color;
  bool do_fg_color, do_bg_color;

public:
  celBillboard (celBillboardManager* mgr, celBillboardLayer* layer);
  virtual ~celBillboard ();

  void SetName (const char* n)
  {
    delete[] name;
    name = csStrNew (n);
  }
  // Return rectangle in screen space.
  void GetRect (csRect& r);

  // Draw billboard (requires 3D mode of g3d).
  void Draw (iGraphics3D* g3d, float z);

  // Check if x,y is in billboard (uses screen space).
  bool In (int sx, int sy);
  // Return true if clickmap is completely true.
  bool HasFullClickmap ();

  // Fire event handlers (input == screen space).
  void FireMouseUp (int sx, int sy, int button);
  void FireMouseDown (int sx, int sy, int button);
  void FireMouseMove (int sx, int sy, int button);
  void FireMouseMoveAway (int sx, int sy, int button);
  void FireMouseDoubleClick (int sx, int sy, int button);

  // Return the text for this billboard.
  iFont* GetFont () const { return font; }
  int GetTextDX () const { return text_dx; }
  int GetTextDY () const { return text_dy; }
  int GetTextFgColorNum () const { return fg_color; }
  int GetTextBgColorNum () const { return bg_color; }
  bool UseTextFgColor () const { return do_fg_color; }
  bool UseTextBgColor () const { return do_bg_color; }

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual csFlags& GetFlags () { return flags; }
  virtual bool SetMaterialName (const char* matname);
  virtual bool SetMaterialNameFast (const char* matname);
  virtual const char* GetMaterialName () { return materialname; }
  virtual bool DrawMesh (const char* material_name,
  	const char* factory, const csVector3& rotate, float angle,
	float distance);
  virtual void SetSize (int w, int h);
  virtual void GetSize (int& w, int& h);
  virtual void GetImageSize (int& w, int& h);
  virtual void SetPosition (int x, int y);
  virtual void MoveToPosition (csTicks delta, int x, int y);
  virtual void GetPosition (int& x, int& y) const;
  virtual void SetPositionScreen (int x, int y);
  virtual void GetPositionScreen (int& x, int& y) const;
  virtual void Move (int dx, int dy);
  virtual void SetRotation (float angle);
  virtual float GetRotation () const;
  virtual void StackTop ();
  virtual void StackBottom ();
  virtual void StackUp ();
  virtual void StackDown ();
  virtual void SetColor (const csColor& c) { color = c; }
  virtual const csColor& GetColor () const { return color; }
  virtual void SetLayer (iBillboardLayer* layer)
  {
    celBillboard::layer = (celBillboardLayer*)layer;
  }
  virtual iBillboardLayer* GetLayer () const { return layer; }

  virtual void SetUVTopLeft (const csVector2& uv) { uv_topleft = uv; }
  virtual const csVector2& GetUVTopLeft () const { return uv_topleft; }
  virtual void SetUVBottomRight (const csVector2& uv) { uv_botright = uv; }
  virtual const csVector2& GetUVBottomRight () const { return uv_botright; }

  virtual void AddEventHandler (iBillboardEventHandler* evh);
  virtual void RemoveEventHandler (iBillboardEventHandler* evh);
  virtual void SetText (const char* txt);
  virtual const char* GetText () const
  { if (text.IsEmpty ()) return 0; else return text.GetData (); }
  virtual void GetTextDimensions (int& w, int& h);
  virtual void SetTextOffset (int dx, int dy);
  virtual int GetTextOffsetDX () const { return text_dx; }
  virtual int GetTextOffsetDY () const { return text_dy; }
  virtual bool SetTextFont (const char* fontname, float size);
  virtual const char* GetTextFont () const { return font_name.GetData (); }
  virtual float GetTextFontSize () const { return font_size; }
  virtual void SetTextFgColor (const csColor& color);
  virtual const csColor& GetTextFgColor () const { return fg_color_remember; }
  virtual void ClearTextFgColor ();
  virtual void SetTextBgColor (const csColor& color);
  virtual const csColor& GetTextBgColor () const { return bg_color_remember; }
  virtual void SetTextBgTransparent ();
  virtual bool IsTextBgTransparent () const
  {
    return bg_color == -1;
  }
  virtual void ClearTextBgColor ();
};

/**
 * This is a manager for billboards.
 */
class celBillboardManager : public iBillboardManager
{
private:
  iObjectRegistry* object_reg;
  csRef<iGraphics3D> g3d;
  csRef<iVirtualClock> vc;
  csRef<iEventNameRegistry> name_reg;

  // Note: the billboard at the end of the following array is the top of
  // the stack.
  csPDelArray<celBillboard> billboards;
  csHash<celBillboard*,csStrKey> billboards_hash;

  csPDelArray<celBillboardLayer> layers;
  celBillboardLayer* default_layer;

  // Created for rendering meshes on billboards.
  csMeshOnTexture* mesh_on_texture;
  iSector* showroom;

  struct movingBillboard
  {
    celBillboard* bb;
    int delta;
    float tot_delta;
    float srcx, srcy;
    float dstx, dsty;
  };
  csArray<movingBillboard> moving_billboards;

  celBillboard* lastmove_billboard;
  celBillboard* moving_billboard;
  int moving_dx;
  int moving_dy;

  float z_min;
  float z_max;

  celBillboard* FindBillboard (int x, int y, uint32 desired_flags);

  csColor default_fg_color_remember;
  csColor default_bg_color_remember;
  csString default_font_name;
  float default_font_size;
  csRef<iFont> default_font;
  int default_fg_color;
  int default_bg_color;

public:
  csRef<iEngine> engine;

  int screen_w_fact;
  int screen_h_fact;
  void ScreenToBillboardSpace (int& x, int& y)
  {
    x *= screen_w_fact;
    y *= screen_h_fact;
  }
  void BillboardToScreenspace (int& x, int& y)
  {
    x /= screen_w_fact;
    y /= screen_h_fact;
  }

  size_t FindMovingBillboard (celBillboard* bb);
  void RemoveMovingBillboard (celBillboard* bb);
  void MoveToPosition (celBillboard* bb, csTicks delta, int x, int y);
  void HandleMovingBillboards (csTicks elapsed);

public:
  celBillboardManager (iBase* parent);
  virtual ~celBillboardManager ();
  bool Initialize (iObjectRegistry* object_reg);
  bool HandleEvent (iEvent& ev);

  iFont* GetDefaultFont () const { return default_font; }
  iGraphics3D* GetGraphics3D () const { return g3d; }

  SCF_DECLARE_IBASE;

  virtual int ScreenToBillboardX (int x) const { return x * screen_w_fact; }
  virtual int ScreenToBillboardY (int y) const { return y * screen_h_fact; }
  virtual int BillboardToScreenX (int x) const { return x / screen_w_fact; }
  virtual int BillboardToScreenY (int y) const { return y / screen_h_fact; }

  virtual iBillboard* CreateBillboard (const char* name);
  virtual iBillboard* FindBillboard (const char* name) const;
  virtual void RemoveBillboard (iBillboard* billboard);
  virtual size_t GetBillboardCount () const { return billboards.Length (); }
  virtual iBillboard* GetBillboard (size_t idx) const
  { return billboards[idx]; }

  virtual iBillboardLayer* CreateBillboardLayer (const char* name);
  virtual iBillboardLayer* FindBillboardLayer (const char* name) const;
  virtual void RemoveBillboardLayer (iBillboardLayer* layer);
  virtual size_t GetBillboardLayerCount () const { return layers.Length (); }
  virtual iBillboardLayer* GetBillboardLayer (size_t idx) const
  { return layers[idx]; }

  virtual void RemoveAll ();
  virtual void SetFlags (uint32 flags, uint32 mask);

  virtual void StackTop (iBillboard* bb);
  virtual void StackBottom (iBillboard* bb);
  virtual void StackUp (iBillboard* bb);
  virtual void StackDown (iBillboard* bb);
  virtual void StackBefore (iBillboard* bb, iBillboard* other);
  virtual void StackAfter (iBillboard* bb, iBillboard* other);

  virtual bool TestCollision (iBillboard* bb1, iBillboard* bb2);
  virtual bool SetDefaultTextFont (const char* fontname, float size);
  virtual const char* GetDefaultTextFont () const
  { return default_font_name.GetData (); }
  virtual float GetDefaultTextFontSize () const { return default_font_size; }
  virtual void SetDefaultTextFgColor (const csColor& color);
  virtual const csColor& GetDefaultTextFgColor () const
  { return default_fg_color_remember; }
  virtual void SetDefaultTextBgColor (const csColor& color);
  virtual const csColor& GetDefaultTextBgColor () const
  { return default_bg_color_remember; }
  virtual void SetDefaultTextBgTransparent ();
  csMeshOnTexture* GetMeshOnTexture ();
  virtual iSector* GetShowroom ();

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celBillboardManager);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;

  // Not an embedded interface to avoid circular references!!!
  class EventHandler : public iEventHandler
  {
  private:
    celBillboardManager* parent;

  public:
    EventHandler (celBillboardManager* parent)
    {
      SCF_CONSTRUCT_IBASE (0);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler ()
    {
      SCF_DESTRUCT_IBASE ();
    }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
    CS_EVENTHANDLER_NAMES("cel.tools.billboardmgr")
    CS_EVENTHANDLER_NIL_CONSTRAINTS
  } *scfiEventHandler;
};

#endif // __CEL_TOOLS_BILLBOARD__

