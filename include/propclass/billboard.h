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

SCF_VERSION (iPcBillboard, 0, 0, 1);

/**
 * Billboard property class.
 * This property class sends out messages with the following supported
 * parameters (add prefix 'cel.parameter.' to get the ID):
 * <ul>
 * <li>x
 * <li>y
 * <li>button
 * </ul>
 * This property class can send out the following messages
 * to the behaviour:
 * <ul>
 * <li>pcbillboard_select: billboard is selected (x,y,button)
 * <li>pcbillboard_move: billboard is moved (x,y,button)
 * <li>pcbillboard_unselect: billboard is unselected (x,y,button)
 * <li>pcbillboard_doubleclick: billboard is double clicked (x,y,button)
 * </ul>
 */
struct iPcBillboard : public iBase
{
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

