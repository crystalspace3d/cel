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
class csColor;

SCF_VERSION (iBillboardEventHandler, 0, 0, 1);

/**
 * An event on some billboard (like being selected
 * or something).
 * Note that all coordinates in this interface are given in billboard
 * coordinates (see iBillboard for more information about what this means).
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


/**
 * If true then billboard is visible.
 */
#define CEL_BILLBOARD_VISIBLE 1
/**
 * If true then billboard is movable.
 */
#define CEL_BILLBOARD_MOVABLE 2
/**
 * If true then billboard is clickable (will send select, unselect, mousemove,
 * and double click events).
 */
#define CEL_BILLBOARD_CLICKABLE 4
/**
 * If true then billboard will restack to the top of the stack
 * if it is clicked on.
 */
#define CEL_BILLBOARD_RESTACK 8

SCF_VERSION (iBillboard, 0, 0, 1);

/**
 * A billboard (2D image on screen).
 * Billboards work in a coordinate system independent of the size
 * of the screen. This coordinate system basically works on a virtual
 * screen with size 1024000 x 614400. These values were chosen because
 * they can fit most other standard resolutions without requiring fractions
 * (i.e. a 1280x1024 screen is exactly 1024000/800 x 614400/600).
 * For conveniance we call this coordinate system 'billboard space'.
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
   * Get the size of this billboard in billboard space.
   */
  virtual void GetSize (int& w, int& h) = 0;

  /**
   * Get the size of the image on this billboard in billboard space.
   */
  virtual void GetImageSize (int& w, int& h) = 0;

  /**
   * Set the position of the top-left corner of this billboard.
   * Uses billboard space.
   */
  virtual void SetPosition (int x, int y) = 0;

  /**
   * Get the position of the top-left corner of this billboard.
   * Uses billboard space.
   */
  virtual void GetPosition (int& x, int& y) const = 0;

  /**
   * Relative move.
   * Uses billboard space.
   */
  virtual void Move (int dx, int dy) = 0;

  /**
   * Set the color for this billboard (default white).
   */
  virtual void SetColor (const csColor& c) = 0;

  /**
   * Get the color for this billboard (default white).
   */
  virtual const csColor& GetColor () const = 0;

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

  /**
   * Bring a billboard to the top of the stack (first visible).
   */
  virtual void StackTop (iBillboard* bb) = 0;

  /**
   * Bring a billboard to the bottom of the stack.
   */
  virtual void StackBottom (iBillboard* bb) = 0;

  /**
   * Bring a billboard one closer to the top.
   */
  virtual void StackUp (iBillboard* bb) = 0;

  /**
   * Bring a billboard one closer to the bottom.
   */
  virtual void StackDown (iBillboard* bb) = 0;

  /**
   * Put a billboard before the given other billboard.
   */
  virtual void StackBefore (iBillboard* bb, iBillboard* other) = 0;

  /**
   * Put a billboard after the given other billboard.
   */
  virtual void StackAfter (iBillboard* bb, iBillboard* other) = 0;

  /**
   * Test if two billboards collide with each other and return true
   * if they do.
   */
  virtual bool TestCollision (iBillboard* bb1, iBillboard* bb2) = 0;
};

#endif // __CEL_MGR_BILLBOARD__

