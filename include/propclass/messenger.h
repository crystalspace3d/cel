/*
    Crystal Space Entity Layer
    Copyright (C) 2012 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_MESSENGER__
#define __CEL_PF_MESSENGER__

#include "cstypes.h"
#include "csutil/scf.h"

enum MessageLocation
{
  LOCATION_CENTER = 0,
  LOCATION_NORTH,
  LOCATION_SOUTH
};

enum CycleType
{
  CYCLE_UNDEFINED = 0,
  CYCLE_RANDOM,
  CYCLE_SEQUENCE,
  CYCLE_NONE,
  CYCLE_INDEX = 100
};

/**
 * An iterator for messages.
 */
struct iMessageLogIterator : public virtual iBase
{
  SCF_INTERFACE (iMessageLogIterator, 0, 0, 1);

  virtual bool HasNext () const = 0;
  virtual const char* Next () = 0;

  /**
   * Get the id of the next message as it will be returned by Next()
   * but don't advance the iterator.
   */
  virtual const char* GetID () = 0;
};

/**
 * This is a messenger property class. This property class can display various
 * types of messages to the user. Every type of message is configurable (with
 * regards to timing, how and where it is displayed, if it is remembered or not, ...)
 *
 * Every type of message has the following properties:
 * - type: a string identifier indicating what kind of message this is.
 * - location: 'north', 'south', 'center': the location where this message is displayed.
 * - timeout: a number of miliseconds that the message is visible on screen before
 *   it starts fading away.
 * - textcolor: color of the text (specified as 'r,g,b' string). Defaults to white.
 * - bgcolor: background color (specified as 'r,b,g' string). If not given the box
 *   is transparent.
 * - bordercolor: color of the border (specified as 'r,g,b' string). If not given
 *   then the color of the border is the same as the bgcolor.
 * - border: border around the text for the box. Defaults to 0.
 * - font: the font used for the text.
 * - fontsize: the size of the font.
 * - fadetime: number of miliseconds that the message uses to fade from visible to
 *   invisible after the 'timeout' has passed.
 * - click: if true this message will go away if the user clicks (provided it
 *   hasn't timed out yet).
 * - log: if true this message is logged. The message requires a unique identifier
 *   in that case.
 * - cyclefirst: this string indicates the type of message cycling in case
 *   multiple messages are given to the message. This cycling flag is for the
 *   first message only unless 'cyclenext' is not given or unless the message
 *   has no unique identifier in which case this property class can't know if it
 *   is the first message or not. The following cycling types are possible:
 *       - random: pick a random message from the given list (default).
 *       - <number>: use the specific message from the list (first message is
 *         index 0).
 *       - sequence: cycle through all messages in sequence starting with 0.
 *       - none: no message is given
 * - cyclenext: this string indicates the type of message cycling for all message
 *   invocations except for the first. This flag is only used in case the
 *   message has a unique id.
 *   The same cycling types as for 'cyclefirst' are possible. If 'sequence' is
 *   used for 'cyclenext' then the cycling starts at message 1 (second message).
 *
 * This property class supports the following actions (add prefix 'cel.messenger.action.'
 * if you want to access this action through a message):
 * - Define: parameters:
 *       'type' (string)
 *       'location' (string, default 'center')
 *       'textcolor' (color4, default white)
 *       'bgcolor' (color4, optional)
 *       'bordercolor' (color4, optional)
 *       'border' (long, default 0)
 *       'font' (string, optional)
 *       'fontsize' (long, optional)
 *       'timeout' (long, default 2000)
 *       'fadetime' (long, default 1000)
 *       'click' (bool, default false)
 *       'log' (bool, default false)
 *       'cyclefirst' (string, default 'random')
 *       'cyclenext' (string, optional)
 * - Message: parameters:
 *       'type' (string, optional, if not set the default type is used)
 *       'id' (string, optional)
 *       'msg1' (string).
 *       'msg2' (string, optional).
 *       'msg3' (string, optional).
 *       'msg4' (string, optional).
 *       'msg5' (string, optional).
 *       'msg6' (string, optional).
 *       'msg7' (string, optional).
 * - SetDefaultType: parameters 'type' (string)
 * - ClearID: parameters 'id' (string)
 */
struct iPcMessenger : public virtual iBase
{
  SCF_INTERFACE (iPcMessenger, 0, 0, 1);

  /**
   * Define a new message type.
   */
  virtual void Define (const char* type, MessageLocation location,
      const csColor4& textColor, const csColor4& bgColor, const csColor4& borderColor,
      int border, const char* font, int fontSize,
      csTicks timeout, csTicks fadetime, bool click, bool log,
      CycleType cyclefirst, CycleType cyclenext) = 0;

  /**
   * Set the default message type.
   */
  virtual void SetDefaultType (const char* type) = 0;

  /**
   * Get the default message type.
   */
  virtual const char* GetDefaultType () const = 0;

  /**
   * Give a message to the user.
   * 'type' can be 0 in which case the default type is used.
   * 'id' can be 0 in which case this message has no id.
   */
  virtual void Message (const char* type, const char* id,
      const char* msg, ...) = 0;

  /**
   * Clear a given message id from the log.
   */
  virtual void ClearId (const char* id) = 0;

  /**
   * Clear all messages from the log with a given type.
   */
  virtual void ClearType (const char* type) = 0;

  /**
   * Clear all messages from the log.
   */
  virtual void ClearAll () = 0;

  /**
   * Get an iterator over all messages of a given type.
   */
  virtual csPtr<iMessageLogIterator> GetMessages (const char* type) = 0;
};

#endif // __CEL_PF_MESSENGER__

