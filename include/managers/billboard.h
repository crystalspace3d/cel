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

#ifndef __CEL_MGR_BILLBOARD__
#define __CEL_MGR_BILLBOARD__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/flags.h"

struct iImage;
struct iBillboard;

SCF_VERSION (iBillboardEventHandler, 0, 0, 1);

/**
 * An event on some billboard (like being selected
 * or something).
 */
struct iBillboardEventHandler : public iBase
{
  /**
   * Billboard is selected.
   */
  virtual void Select (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey) = 0;

  /**
   * Mouse moves while billboard is selected.
   */
  virtual void MouseMove (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey) = 0;

  /**
   * Billboard is unselected.
   */
  virtual void Unselect (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey) = 0;

  /**
   * Billboard is double clicked.
   */
  virtual void DoubleClick (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey) = 0;
};



#define CEL_BILLBOARD_VISIBLE 1
#define CEL_BILLBOARD_MOVABLE 2
#define CEL_BILLBOARD_CLICKABLE 4

SCF_VERSION (iBillboard, 0, 0, 1);

/**
 * A billboard (2D image on screen).
 */
struct iBillboard : public iBase
{
  /**
   * Get the name of this billboard.
   */
  virtual const char* GetName () const = 0;

  /**
   * Get flags for this billboard.
   */
  virtual csFlags& GetFlags () = 0;

  /**
   * Set material for this billboard. Returns false on failure.
   */
  virtual bool SetMaterialName (const char* matname) = 0;

  /**
   * Get material name.
   */
  virtual const char* GetMaterialName () = 0;

  /**
   * Set the size of this billboard in pixels.
   */
  virtual void SetSize (int w, int h) = 0;

  /**
   * Get the size of this billboard in pixels.
   */
  virtual void GetSize (int& w, int& h) const = 0;

  /**
   * Set the position of the top-left corner of this billboard.
   */
  virtual void SetPosition (int x, int y) = 0;

  /**
   * Get the position of the top-left corner of this billboard.
   */
  virtual void GetPosition (int& x, int& y) const = 0;

  /**
   * Relative move.
   */
  virtual void Move (int dx, int dy) = 0;

  /**
   * Add an event handler to this billboard.
   */
  virtual void AddEventHandler (iBillboardEventHandler* evh) = 0;

  /**
   * Remove an event handler from this billboard.
   */
  virtual void RemoveEventHandler (iBillboardEventHandler* evh) = 0;
};

SCF_VERSION (iBillboardManager, 0, 0, 1);

/**
 * This is a manager to handle billboards. This is used in combination
 * with the billboard property class.
 */
struct iBillboardManager : public iBase
{
  /**
   * Create a new billboard with the given name. Returns 0 on failure.
   * By default a new billboard will be visible and not movable.
   */
  virtual iBillboard* CreateBillboard (const char* name) = 0;

  /**
   * Find a billboard with a given name.
   */
  virtual iBillboard* FindBillboard (const char* name) const = 0;

  /**
   * Remove a billboard.
   */
  virtual void RemoveBillboard (iBillboard* billboard) = 0;

  /**
   * Get the number of all billboards.
   */
  virtual int GetBillboardCount () const = 0;

  /**
   * Get a billboard by index.
   */
  virtual iBillboard* GetBillboard (int idx) const = 0;

  /**
   * Remove all billboards.
   */
  virtual void RemoveAll () = 0;

  /**
   * Set flags for all billboards.
   */
  virtual void SetFlags (uint32 flags, uint32 mask) = 0;
};

#endif // __CEL_MGR_BILLBOARD__

