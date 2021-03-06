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
struct iSector;
class csColor;
class csVector2;

/**
 * A layer of billboards. You can add several billboards on one
 * layer and then you can move the layer.
 */
struct iBillboardLayer : public virtual iBase
{
  SCF_INTERFACE (iBillboardLayer, 0, 0, 1);

  /**
   * Get current offset of layer in billboard space.
   */
  virtual void GetOffset (int& x, int& y) const = 0;

  /**
   * Set offset of layer in billboard space.
   */
  virtual void SetOffset (int x, int y) = 0;

  /**
   * Relative move of layer.
   */
  virtual void Move (int dx, int dy) = 0;

  /**
   * Get the name of this layer.
   */
  virtual const char* GetName () const = 0;
};

/**
 * An event on some billboard (like being selected
 * or something).
 * Note that all coordinates in this interface are given in billboard
 * coordinates (see iBillboard for more information about what this means).
 */
struct iBillboardEventHandler : public virtual iBase
{
  SCF_INTERFACE (iBillboardEventHandler, 0, 0, 1);

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
   * Mouse moves away from billboard.
   */
  virtual void MouseMoveAway (iBillboard* billboard, int mouse_button,
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
/**
 * If true then billboard will send move and moveaway events.
 * Default is false.
 */
#define CEL_BILLBOARD_SENDMOVE 16

/**
 * A billboard (2D image on screen).
 * Billboards work in a coordinate system independent of the size
 * of the screen. This coordinate system basically works on a virtual
 * screen with size 307200 x 307200. These values were chosen because
 * they can fit most other standard resolutions without requiring fractions
 * (i.e. a 1280x1024 screen is exactly 307200/240 x 307200/300).
 * For convenience we call this coordinate system 'billboard space'.
 */
struct iBillboard : public virtual iBase
{
  SCF_INTERFACE (iBillboard, 1, 0, 0);

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
   * Important! Make sure that the material uses the <keepimage/>
   * flag! The billboard manager depends on this.
   */
  virtual bool SetMaterialName (const char* matname) = 0;

  /**
   * Set material for this billboard. Returns false on failure.
   * This is a faster version which does not recompute the
   * clickmap (unless there is no clickmap yet).
   */
  virtual bool SetMaterialNameFast (const char* matname) = 0;

  /**
   * Get material name.
   */
  virtual const char* GetMaterialName () = 0;

  /**
   * Draw a mesh on this billboard. This mesh will be rendered in
   * the 'showroom' (see the billboard manager).
   * \param material_name is the name of the material that will be
   * used. If this material already exists then a new one will
   * not be created. If you want to force creation of a new material
   * with the same name then you must first remove the material.
   * \param factory is the name of the mesh factory.
   * \param rotate is an optional rotation vector.
   * \param angle is an optional angle around the rotation vector.
   * If not given then there will be no rotation.
   * \param distance is the distance from the mesh where we put our
   * camera. This is optional. If you set this to negative then
   * the mesh will be fit on the billboard as good as is possible.
   * \return false on failure.
   */
  virtual bool DrawMesh (const char* material_name,
  	const char* factory, const csVector3& rotate = csVector3 (0, 0, 0),
	float angle = 0.0f, float distance = -1.0f) = 0;

  /**
   * Set the size of this billboard in billboard space.
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
   * During the given time, slowly move the billboard
   * to the given position. If 'delta' is 0 then this function
   * is equivalent to calling SetPosition().
   * Calling this function while another move is in progress
   * has the effect of clearing that previous move.
   * Uses billboard space.
   */
  virtual void MoveToPosition (csTicks delta, int x, int y) = 0;

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
   * Rotate the billboard.
   */
  virtual void SetRotation (float angle) = 0;

  /**
   * Get rotation of the billboard.
   */
  virtual float GetRotation () const = 0;

  /**
   * Bring this billboard to the top of the stack (first visible).
   */
  virtual void StackTop () = 0;

  /**
   * Bring this billboard to the bottom of the stack.
   */
  virtual void StackBottom () = 0;

  /**
   * Bring this billboard just above the other iBillboard.
   */
  virtual void StackAfter (iBillboard *other) = 0;

  /**
   * Bring this billboard just below the other iBillboard.
   */
  virtual void StackBefore (iBillboard *other) = 0;

  /**
   * Bring this billboard one closer to the top of the stack (first visible).
   */
  virtual void StackUp () = 0;

  /**
   * Bring this billboard one closer to the bottom of the stack.
   */
  virtual void StackDown () = 0;

  /**
   * Set the top-left coordinate of the texture (default 0,0).
   * Range from 0,0 - 1,1.
   */
  virtual void SetUVTopLeft (const csVector2& uv) = 0;

  /**
   * Get the top-left coordinate of the texture (default 0,0).
   * Range from 0,0 - 1,1.
   */
  virtual const csVector2& GetUVTopLeft () const = 0;

  /**
   * Set the bottom-right coordinate of the texture (default 1,1).
   * Range from 0,0 - 1,1.
   */
  virtual void SetUVBottomRight (const csVector2& uv) = 0;

  /**
   * Get the bottom-right coordinate of the texture (default 1,1).
   * Range from 0,0 - 1,1.
   */
  virtual const csVector2& GetUVBottomRight () const = 0;

  /**
   * Set the color for this billboard (default white).
   */
  virtual void SetColor (const csColor& c) = 0;

  /**
   * Get the color for this billboard (default white).
   */
  virtual const csColor& GetColor () const = 0;

  /**
   * Set the layer to use for this billboard.
   */
  virtual void SetLayer (iBillboardLayer* layer) = 0;

  /**
   * Get the layer that is used by this billboard.
   */
  virtual iBillboardLayer* GetLayer () const = 0;

  /**
   * Add an event handler to this billboard.
   */
  virtual void AddEventHandler (iBillboardEventHandler* evh) = 0;

  /**
   * Remove an event handler from this billboard.
   */
  virtual void RemoveEventHandler (iBillboardEventHandler* evh) = 0;

  /**
   * Set the text to use for this billboard.
   * Use 'txt' is 0 to clear the text.
   */
  virtual void SetText (const char* txt) = 0;

  /**
   * Get the text.
   */
  virtual const char* GetText () const = 0;

  /**
   * Get the dimensions of the current text in billboard coordinates.
   */
  virtual void GetTextDimensions (int& w, int& h) = 0;

  /**
   * Set the text offset in billboard space.
   */
  virtual void SetTextOffset (int dx, int dy) = 0;

  /**
   * Get the text offset in billboard space.
   */
  virtual int GetTextOffsetDX () const = 0;

  /**
   * Get the text offset in billboard space.
   */
  virtual int GetTextOffsetDY () const = 0;

  /**
   * Setup the font to use for text.
   */
  virtual bool SetTextFont (const char* fontname, float fontsize) = 0;

  /**
   * Get the font to use for text.
   */
  virtual const char* GetTextFont () const = 0;

  /**
   * Get the font size to use for text.
   */
  virtual float GetTextFontSize () const = 0;

  /**
   * Set the foreground color for the text.
   * If not set then the default foreground color will be used
   */
  virtual void SetTextFgColor (const csColor& color) = 0;

  /**
   * Get the foreground color for the text.
   */
  virtual const csColor& GetTextFgColor () const = 0;

  /**
   * Clear the foreground color for the text so that the
   * default color will be used instead.
   */
  virtual void ClearTextFgColor () = 0;

  /**
   * Set the background color for the text.
   * If not set then the default background color will be used
   */
  virtual void SetTextBgColor (const csColor& color) = 0;

  /**
   * Get the background color for the text.
   */
  virtual const csColor& GetTextBgColor () const = 0;

  /**
   * Set the background color for the text to transparent.
   * If not set then the default background transparency will be used
   */
  virtual void SetTextBgTransparent () = 0;

  /**
   * Get true if the background color for the text is transparent.
   */
  virtual bool IsTextBgTransparent () const = 0;

  /**
   * Clear the background color for the text so that the
   * default color will be used instead.
   */
  virtual void ClearTextBgColor () = 0;
};

/**
 * This is a manager to handle billboards. This is used in combination
 * with the billboard property class.
 */
struct iBillboardManager : public virtual iBase
{
  SCF_INTERFACE (iBillboardManager, 0, 0, 1);

  /**
   * Convert a screen X position to billboard space.
   */
  virtual int ScreenToBillboardX (int x) const = 0;

  /**
   * Convert a screen Y position to billboard space.
   */
  virtual int ScreenToBillboardY (int y) const = 0;

  /**
   * Convert a billboard X position to screen space.
   */
  virtual int BillboardToScreenX (int x) const = 0;

  /**
   * Convert a billboard Y position to screen space.
   */
  virtual int BillboardToScreenY (int y) const = 0;

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
  virtual size_t GetBillboardCount () const = 0;

  /**
   * Get a billboard by index.
   */
  virtual iBillboard* GetBillboard (size_t idx) const = 0;

  /**
   * Create a new billboard layer with the given name. Returns 0 on failure.
   * Note that there is always a layer with name 'default'
   * which is used for new billboards.
   */
  virtual iBillboardLayer* CreateBillboardLayer (const char* name) = 0;

  /**
   * Find a billboard layer with a given name.
   */
  virtual iBillboardLayer* FindBillboardLayer (const char* name) const = 0;

  /**
   * Remove a billboard layer. Note that all billboards that use this
   * layer will be put back on the default layer. Also note that
   * you can't remove the default layer.
   */
  virtual void RemoveBillboardLayer (iBillboardLayer* layer) = 0;

  /**
   * Get the number of all billboard layers.
   */
  virtual size_t GetBillboardLayerCount () const = 0;

  /**
   * Get a billboard layer by index.
   */
  virtual iBillboardLayer* GetBillboardLayer (size_t idx) const = 0;

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

  /**
   * Setup the default font to use for text.
   */
  virtual bool SetDefaultTextFont (const char* fontname, float fontsize) = 0;

  /**
   * Get the default font to use for text.
   */
  virtual const char* GetDefaultTextFont () const = 0;

  /**
   * Get the default font size to use for text.
   */
  virtual float GetDefaultTextFontSize () const = 0;

  /**
   * Setup the default foreground color to use for text.
   */
  virtual void SetDefaultTextFgColor (const csColor& color) = 0;

  /**
   * Get the default foreground color to use for text.
   */
  virtual const csColor& GetDefaultTextFgColor () const = 0;

  /**
   * Setup the default background color to use for text.
   */
  virtual void SetDefaultTextBgColor (const csColor& color) = 0;

  /**
   * Get the default background color to use for text.
   */
  virtual const csColor& GetDefaultTextBgColor () const = 0;

  /**
   * Setup the default text background to use transparency.
   */
  virtual void SetDefaultTextBgTransparent () = 0;

  /**
   * Get the showroom. This is a special sector that is used
   * for rendering meshes in. If you call this function for the
   * first time it will create a sector with two white lights (one
   * at (-10, 3, 0) and one at (10, 3, 0) with radius 50. The object will
   * be placed at (0,0,0). You can modify this sector freely
   * (adding/removing lights, changing renderloop, ...)
   */
  virtual iSector* GetShowroom () = 0;
};

#endif // __CEL_MGR_BILLBOARD__

