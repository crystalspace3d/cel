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
#include "csutil/strhash.h"
#include "iutil/comp.h"
#include "managers/billboard.h"

struct iObjectRegistry;
struct iImage;

/**
 * A billboard.
 */
class celBillboard : public iBillboard
{
private:
  char* name;
  csFlags flags;
  csRef<iImage> image;
  int x, y, w, h;

public:
  celBillboard ();
  virtual ~celBillboard ();

  void SetName (const char* n)
  {
    delete[] name;
    name = csStrNew (n);
  }

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual csFlags& GetFlags () { return flags; }
  virtual bool SetImage (iImage* image);
  virtual bool SetImage (const char* filename);
  virtual iImage* GetImage () { return image; }
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
};

/**
 * This is a manager for billboards.
 */
class celBillboardManager : public iBillboardManager
{
private:
  iObjectRegistry* object_reg;
  csPDelArray<celBillboard> billboards;
  csHash<celBillboard*,const char*,csConstCharHashKeyHandler> billboards_hash;

public:
  celBillboardManager (iBase* parent);
  virtual ~celBillboardManager ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual iBillboard* CreateBillboard (const char* name);
  virtual iBillboard* FindBillboard (const char* name) const;
  virtual void RemoveBillboard (iBillboard* billboard);
  virtual int GetBillboardCount () const { return billboards.Length (); }
  virtual iBillboard* GetBillboard (int idx) const { return billboards[idx]; }
  virtual void RemoveAll ();
  virtual void SetFlags (uint32 flags, uint32 mask);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celBillboardManager);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_MANAGER_BILLBOARD__

