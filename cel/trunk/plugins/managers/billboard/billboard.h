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

#ifndef __CEL_MANAGER_BILLBOARD__
#define __CEL_MANAGER_BILLBOARD__

#include "csutil/util.h"
#include "csutil/hash.h"
#include "csutil/hashhandlers.h"
#include "csutil/parray.h"
#include "csutil/refarr.h"
#include "csutil/strhash.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "managers/billboard.h"

struct iObjectRegistry;
struct iImage;
struct iEngine;
struct iGraphics3D;
struct iEvent;

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
  int x, y, w, h;
  csRefArray<iBillboardEventHandler> handlers;

public:
  celBillboard ();
  virtual ~celBillboard ();

  void SetName (const char* n)
  {
    delete[] name;
    name = csStrNew (n);
  }

  // Draw billboard (requires 3D mode of g3d).
  void Draw (iEngine* engine, iGraphics3D* g3d, float z);

  // Check if x,y is in billboard.
  bool In (int cx, int cy);

  // Fire event handlers.
  void FireMouseUp (int x, int y, int button);
  void FireMouseDown (int x, int y, int button);
  void FireMouseMove (int x, int y, int button);
  void FireMouseDoubleClick (int x, int y, int button);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual csFlags& GetFlags () { return flags; }
  virtual bool SetMaterialName (const char* matname);
  virtual const char* GetMaterialName () { return materialname; }
  virtual void SetSize (int w, int h);
  virtual void GetSize (int& w, int& h) const
  {
    w = celBillboard::w;
    h = celBillboard::h;
  }
  virtual void SetPosition (int x, int y);
  virtual void GetPosition (int& x, int& y) const
  {
    x = celBillboard::x;
    y = celBillboard::y;
  }
  virtual void Move (int dx, int dy);

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
  csRef<iEngine> engine;

  celBillboard* moving_billboard;
  int moving_dx;
  int moving_dy;

  float z_min;
  float z_max;

  celBillboard* FindBillboard (int x, int y, uint32 desired_flags);

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

#endif // __CEL_MANAGER_BILLBOARD__

