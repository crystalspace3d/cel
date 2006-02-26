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

#ifndef __CEL_PF_INPUT__
#define __CEL_PF_INPUT__

#include "cstypes.h"
#include "csutil/scf.h"

/**
 * Input propery class.
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>Bind: parameters 'trigger' (string) and 'command' (string).
 * </ul>
 * <p>
 * This property class can send out the following messages
 * to the behaviour:
 * <ul>
 * <li>pccommandinput_<key>0: key is unpressed
 * <li>pccommandinput_<key>1: key is pressed
 * <li>pccommandinput_<key>_: key is pressed and auto-repeating
 * <li>pccommandinput_<axis>: movement on the given axis
 * </ul>
 * <p>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * <ul>
 * <li>screenspace (bool, read/write): use screenspace instead of -1/1
 *     normalized coordinates (-1/1 is default).
 * <li>cooked (bool, read/write): use cooked mode instead of raw (default
 *     is raw.
 * </ul>
 */
struct iPcCommandInput : public virtual iBase
{
  SCF_INTERFACE (iPcCommandInput, 0, 0, 1);

  /**
   * Activates the input to get Commands
   */
  virtual void Activate (bool activate=true) = 0;

  /**
   * Enable/disable cooked mode. By default raw mode is used
   * which means that keyboard events are parsed in raw mode. That
   * implies the following:
   * - You can't bind on key combinations like Shift-F3
   * - Binding on 'a' is the same as 'A'.
   * In cooked mode you can bind on key combinations and on keys
   * like 'shift'.
   */
  virtual void SetCookedMode (bool cooked) = 0;

  /**
   * Get cooked or raw.
   */
  virtual bool GetCookedMode () const = 0;

  /**
   * Send screen coordinates instead of coordinates between -1 and 1 for axis
   * movement messages.
   */
  virtual void ScreenCoordinates (bool screen=true) = 0;

  /**
   * Convert a coordinate from screen-space to a value between -1 and 1
   * @param screencoord the coordinate in screen-space
   * @param axis the screen axis to use for the conversion (0 = x, 1 = y)
   */
  virtual float ScreenToCentered (float screencoord, float axis = 0) = 0;

  /**
   * Convert a coordinate from a value between -1 and 1 to screen-space
   * @param centeredcoord a value between -1 and 1
   * @param axis the screen axis to use for the conversion (0 = x, 1 = y)
   */
  virtual float CenteredToScreen (float centeredcoord, float axis = 0) = 0;

  /**
   * Loads a Config File binding triggers(for example keys) to commands
   */
  virtual bool LoadConfig (const char* fname) = 0;

  /**
   * Binds a trigger to a command
   */
  virtual bool Bind (const char* triggername, const char* command) = 0;

  /** returns the command bind to a key */
  virtual const char* GetBind (const char* triggername) const = 0;

  /** deletes a bind, if triggername is 0 deletes all binds to the command */
  virtual bool RemoveBind (const char* triggername, const char* command) = 0;
  
  /** deletes all binds */
  virtual void RemoveAllBinds () = 0;
};

#endif // __CEL_PF_INPUT__

