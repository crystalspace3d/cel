/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_TOOLTIP__
#define __CEL_PF_TOOLTIP__

#include "cstypes.h"
#include "csutil/scf.h"

SCF_VERSION (iPcTooltip, 0, 0, 1);

/**
 * Tooltip property class.
 */
struct iPcTooltip : public iBase
{
  /**
   * Set the text to display. Newlines will be properly handled.
   */
  virtual void SetText (const char* text) = 0;

  /**
   * Show tooltip at given location.
   */
  virtual void Show (int x, int y) = 0;

  /**
   * Hide tooltip.
   */
  virtual void Hide () = 0;

  /**
   * Is tooltip visible?
   */
  virtual bool IsVisible () const = 0;

  /**
   * Set text color. Default black.
   */
  virtual void SetTextColor (int r, int g, int b) = 0;

  /**
   * Set background color (use -1, -1, -1 for transparent).
   * Default yellow.
   */
  virtual void SetBackgroundColor (int r, int g, int b) = 0;
};

#endif // __CEL_PF_TOOLTIP__

