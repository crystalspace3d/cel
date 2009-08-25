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
 * Key state for button/key messages.
 */
enum celKeyState
{
  CEL_KEY_STATE_UNUSED = -1,
  CEL_KEY_STATE_UP,
  CEL_KEY_STATE_DOWN,
  CEL_KEY_STATE_REPEAT
};

/**
 * Input propery class.
 *
 * This property class supports the following actions:
 * - Activate: parameter 'activate' (bool default=true).
 * - Bind: parameters 'trigger' (string) and 'command' (string). The 'trigger'
 *   can be equal to 'key' in which cases all keys will be bound.
 *   Also .args can be appended to the command name, to receive key/button
 *   state in message arguments, instead of in message name.
 * - RemoveBind: paramaters 'trigger' (string) and 'command' (string).
 * - RemoveAllBinds.
 * - LoadConfig: parameters 'prefix' (string).
 * - SaveConfig: parameters 'prefix' (string).
 *
 * This property class can send out the following messages
 * to the behaviour:
 * - pccommandinput_<key>: key event. Message receives 'state' parameter 
 *   filled with values from celKeyState enum. Also has optional 'trigger' 
 *   parameter.
 *   This is received instead of the other key messages below depending on 
 *   '.args' suffix on command specification (see above).
 * - pccommandinput_<key>0: key is unpressed. Message has optional
 *   'trigger' parameter.
 * - pccommandinput_<key>1: key is pressed. Message has optional
 *   'trigger' parameter.
 * - pccommandinput_<key>_: key is pressed and auto-repeating. Message
 *   has optional 'trigger' parameter.
 * - pccommandinput_<axis>: movement on the given axis.
 *
 * This property class supports the following properties:
 * - screenspace (bool, read/write): use screenspace instead of -1/1
 *   normalized coordinates (default is -1/1).
 * - cooked (bool, read/write): use cooked mode instead of raw (default
 *   is raw).
 * - sendtrigger (bool, read/write): send out trigger name in the
 *   message to the behaviour. (default is false).
 *
 * Config file format:
 *   <prefix>.Input.Bind.<command> = <trigger> [trigger2] [triggerN..]
 */
struct iPcCommandInput : public virtual iBase
{
  SCF_INTERFACE (iPcCommandInput, 0, 2, 0);

  /**
   * Activates this input property class. When activated, it will catch input
   * events and send command messages.
   */
  virtual void Activate (bool activate=true) = 0;

  /**
   * Enable/disable sending trigger name. When this is enabled
   * this property class will add a 'trigger' parameter to all
   * keyboard messages. That parameter will be the name of the keyboard
   * event that triggered the message. By default this is disabled.
   */
  virtual void SetSendTrigger (bool send) = 0;

  /**
   * Is send trigger enabled or disabled?
   */
  virtual bool IsSendTriggerEnabled () const = 0;

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
   * Get whether or not this property class is in cooked mode. See
   * SetCookedMode for more information.
   */
  virtual bool GetCookedMode () const = 0;

  /**
   * Send screen coordinates instead of coordinates between -1 and 1 for axis
   * movement messages.
   */
  virtual void ScreenCoordinates (bool screen=true) = 0;

  /**
   * Convert a coordinate from screen-space to a value between -1 and 1
   * \param screencoord the coordinate in screen-space
   * \param axis the screen axis to use for the conversion (0 = x, 1 = y)
   */
  virtual float ScreenToCentered (float screencoord, float axis = 0) = 0;

  /**
   * Convert a coordinate from a value between -1 and 1 to screen-space
   * \param centeredcoord a value between -1 and 1
   * \param axis the screen axis to use for the conversion (0 = x, 1 = y)
   */
  virtual float CenteredToScreen (float centeredcoord, float axis = 0) = 0;

  /**
   * Loads from a config file binding triggers(for example keys) to commands
   * \param prefix for the <prefix>.CommandInput.Bind.<trigger>
   */
  virtual bool LoadConfig (const char* prefix) = 0;

  /**
   * Saves to a config file binding triggers(for example keys) to commands
   * \param prefix for the <prefix>.CommandInput.Bind.<trigger>
   */
  virtual void SaveConfig (const char* prefix) = 0;

  /**
   * Binds a trigger to a command. The triggername can be equal to 'key'
   * in which case all keys will be bound.
   */
  virtual bool Bind (const char* triggername, const char* command) = 0;

  /**
   * Returns the command bound to the given key.
   */
  virtual const char* GetBind (const char* triggername) const = 0;

  /**
   * Deletes a bind. If triggername is 0, all binds to the given command are
   * deleted.
   */
  virtual bool RemoveBind (const char* triggername, const char* command) = 0;

  /**
   * Deletes all binds.
   */
  virtual void RemoveAllBinds () = 0;

  /**
   * Enable capturing of mouse events.
   */
  virtual void EnableMouseEvents () = 0;

  /**
   * Disable capturing of mouse events.
   */
  virtual void DisableMouseEvents () = 0;

  /**
   * Enable capturing of keyboard events.
   */
  virtual void EnableKeyboardEvents () = 0;

  /**
   * Disable capturing of keyboard events.
   */
  virtual void DisableKeyboardEvents () = 0;

  /**
   * Enable capturing of joystick events.
   */
  virtual void EnableJoystickEvents () = 0;

  /**
   * Disable capturing of joystick events.
   */
  virtual void DisableJoystickEvents () = 0;
};

#endif // __CEL_PF_INPUT__

