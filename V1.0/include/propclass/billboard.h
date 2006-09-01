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

#ifndef __CEL_PF_BILLBOARD__
#define __CEL_PF_BILLBOARD__

#include "cstypes.h"
#include "csutil/scf.h"

struct iBillboard;

/**
 * Billboard property class.
 *
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * - pcbillboard_select: billboard is selected (x,y,button)
 * - pcbillboard_move: mouse moves over billboard (x,y,button)
 * - pcbillboard_moveaway: mouse moves away from billboard (x,y,button)
 * - pcbillboard_unselect: billboard is unselected (x,y,button)
 * - pcbillboard_doubleclick: billboard is double clicked (x,y,button)
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - DrawMesh: parameters 'materialname' (string),
 *     'factory' (string), optional: 'distance' (float),
 *     optional: 'rotate' (vector3), optional: 'angle' (float).
 * - GetTextWidth: no parameters, returns an integer with current text width
 *   in billboard coordinates.
 * - GetTextHeight: no parameters, returns an integer with current text height
 *   in billboard coordinates.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - name (string, read/write): name of the billboard.
 * - materialname (string, read/write): name of the material. Warning!
 *     It is important to make sure the materials using the billboard
 *     have the <keepimage/> flag set!
 * - materialnamefast (string, read/write): name of the material (faster
 *     way to set).
 * - clickable (bool, read/write): clickable (default no).
 * - movable (bool, read/write): movable (default no).
 * - visible (bool, read/write): visible (default yes).
 * - restack (bool, read/write): restackable on selection (default no).
 * - sendmove (bool, read/write): send move/moveaway events (default no).
 * - color (color, read/write): color of this billboard.
 * - width (long, read/write): width of this billboard.
 * - height (long, read/write): height of this billboard.
 * - widthpct (float, read/write): width of this billboard in percentage.
 * - heightpct (float, read/write): height of this billboard in percentage.
 * - x (long, read/write): x position of this billboard.
 * - y (long, read/write): y position of this billboard.
 * - uv_topleft (vector2, read/write): topleft uv coordinate in material.
 * - uv_botright (vector2, read/write): botright uv coordinate in material.
 * - layer (string, read/write): layer name.
 * - text (string, read/write): optional text on billboard.
 * - text_offset (vector2, write): offset for text in billboard space.
 * - text_fg_color (color, write): foreground color.
 * - text_bg_color (color, write): background color.
 * - text_font_size (float, write): font size. Set this before setting
 *     text_font. Otherwise default is 10.
 * - text_font (string, write): font.
 * - text_default_fg_color (color, write): default foreground color (for
 *     all billboards).
 * - text_default_bg_color (color, write): default background color (for
 *     all billboards).
 * - text_default_font_size (float, write): font size. Set this before
 *     setting text_default_font. Otherwise default is 10.
 * - text_default_font (string, write): default font (for
 *     all billboards).
 */
struct iPcBillboard : public virtual iBase
{
  SCF_INTERFACE (iPcBillboard, 0, 0, 1);

  /**
   * Set the name of the billboard.
   */
  virtual void SetBillboardName (const char* name) = 0;

  /**
   * Get the name of the billboard.
   */
  virtual const char* GetBillboardName () = 0;

  /**
   * Get the billboard.
   */
  virtual iBillboard* GetBillboard () = 0;

  /**
   * Enable or disable listening for events for this billboard.
   */
  virtual void EnableEvents (bool e) = 0;

  /**
   * Return true if events are enabled.
   */
  virtual bool AreEventsEnabled () const = 0;
};

#endif // __CEL_PF_BILLBOARD__

