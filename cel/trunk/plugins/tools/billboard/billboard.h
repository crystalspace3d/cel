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
#include "csutil/hashhandlers.h"
#include "csutil/parray.h"
#include "csutil/refarr.h"
#include "csutil/strhash.h"
#include "csutil/cscolor.h"
#include "csgeom/csrect.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "tools/billboard.h"

struct iObjectRegistry;
struct iImage;
struct iEngine;
struct iGraphics3D;
struct iEvent;

class celBillboardManager;

/**
 * A billboard.
 */
class celBillboard : public iBillboard
{
private:
  char* name;
  csFlags flags;
  char* materialname;
  iMaterialWrapper* material;
  int image_w, image_h;
  // Coordinates in billboard space.
  int bb_x, bb_y, bb_w, bb_h;
  csRefArray<iBillboardEventHandler> handlers;
  celBillboardManager* mgr;
  csColor color;

  bool has_clickmap;
  uint8* clickmap;

  void SetupMaterial ();
  // Input: texture coordinates.
  bool GetFromClickMap (int tx, int ty);
  // Input: texture coordinates.
  void SetClickMap (int tx, int ty, bool v);
  void TranslateScreenToTexture (int sx, int sy, int& tx, int& ty);

public:
  celBillboard (celBillboardManager* mgr);
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

  // Fire event handlers (input == screen space).
  void FireMouseUp (int sx, int sy, int button);
  void FireMouseDown (int sx, int sy, int button);
  void FireMouseMove (int sx, int sy, int button);
  void FireMouseDoubleClick (int sx, int sy, int button);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual csFlags& GetFlags () { return flags; }
  virtual bool SetMaterialName (const char* matname);
  virtual const char* GetMaterialName () { return materialname; }
  virtual void SetSize (int w, int h);
  virtual void GetSize (int& w, int& h);
  virtual void GetImageSize (int& w, int& h);
  virtual void SetPosition (int x, int y);
  virtual void GetPosition (int& x, int& y) const;
  virtual void SetPositionScreen (int x, int y);
  virtual void GetPositionScreen (int& x, int& y) const;
  virtual void Move (int dx, int dy);
  virtual void SetColor (const csColor& c) { color = c; }
  virtual const csColor& GetColor () const { return color; }

  virtual void AddEventHandler (iBillboardEventHandler* evh);
  virtual void RemoveEventHandler (iBillboardEventHandler* evh);
};

/**
 * This is a manager for billboards.
 */
class celBillboardManager : public iBillboardManager
{
private:
  iObjectRegistry* object_reg;
  // Note: the billboard at the end of the following array is the top of
  // the stack.
  csPDelArray<celBillboard> billboards;
  csHash<celBillboard*,csStrKey,csConstCharHashKeyHandler> billboards_hash;
  csRef<iGraphics3D> g3d;

  celBillboard* moving_billboard;
  int moving_dx;
  int moving_dy;

  float z_min;
  float z_max;

  celBillboard* FindBillboard (int x, int y, uint32 desired_flags);

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

public:
  celBillboardManager (iBase* parent);
  virtual ~celBillboardManager ();
  bool Initialize (iObjectRegistry* object_reg);
  bool HandleEvent (iEvent& ev);

  SCF_DECLARE_IBASE;

  virtual iBillboard* CreateBillboard (const char* name);
  virtual iBillboard* FindBillboard (const char* name) const;
  virtual void RemoveBillboard (iBillboard* billboard);
  virtual int GetBillboardCount () const { return billboards.Length (); }
  virtual iBillboard* GetBillboard (int idx) const { return billboards[idx]; }
  virtual void RemoveAll ();
  virtual void SetFlags (uint32 flags, uint32 mask);

  virtual void StackTop (iBillboard* bb);
  virtual void StackBottom (iBillboard* bb);
  virtual void StackUp (iBillboard* bb);
  virtual void StackDown (iBillboard* bb);
  virtual void StackBefore (iBillboard* bb, iBillboard* other);
  virtual void StackAfter (iBillboard* bb, iBillboard* other);

  virtual bool TestCollision (iBillboard* bb1, iBillboard* bb2);

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
    virtual ~EventHandler () { }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
};

#endif // __CEL_TOOLS_BILLBOARD__

