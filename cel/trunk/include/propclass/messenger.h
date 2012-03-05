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

enum MessageLocationAnchor
{
  ANCHOR_CENTER = 0,
  ANCHOR_NORTH,
  ANCHOR_NORTHWEST,
  ANCHOR_WEST,
  ANCHOR_SOUTHWEST,
  ANCHOR_SOUTH,
  ANCHOR_SOUTHEAST,
  ANCHOR_EAST,
  ANCHOR_NORTHEAST
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
 * Locations on screen where messages can be put are defined as 'slots'. There
 * is one predefined slot called 'none' which doesn't display anything. A slot
 * has the following properties:
 * - name
 * - position (x,y)
 * - positionAnchor: c, nw, n, ne, e, se, s, sw, w
 * - sizex: if given then this box has a fixed horizontal size.
 * - sizey: if given then this box has a fixed vertical size.
 * - maxsizex: maximum horizontal size (not used if size is fixed).
 * - maxsizey: maximum vertical size (not used if size is fixed).
 * - marginx: horizontal margin.
 * - marginy: vertical margin.
 * - boxcolor: color of the box (specified as 'r,g,b,a' string).
 * - bordercolor: if not given then this is equal to boxcolor.
 * - borderwidth: width of the border in pixels.
 * - maxmessages: maximum number of messages (not lines!) to display
 *   in this slot at the same time.
 * - queue: if there are more then maxmessages messages then when 'queue'
 *   is true the messages will be kept in a queue until all of them
 *   are displayed. If 'queue' is false then new messages will push away
 *   older messages even if these older messages were not visible long
 *   enough.
 * - boxfadetime: if there are no messages left in the box then the
 *   box itself will fade away.
 *
 * Every type of message has the following properties:
 * - type: a string identifier indicating what kind of message this is.
 * - slot: the location where this message is displayed.
 * - timeout: a number of miliseconds that the message is visible on screen before
 *   it starts fading away.
 * - textcolor: color of the text (specified as 'r,g,b,a' string). Defaults to white.
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
 * - DefineSlot: parameters:
 *       'name' (string)
 *       'position' (string, 'x,y')
 *       'positionAnchor' (string, one of c, nw, n, ne, e, se, s, sw, w)
 *       'sizex' (long, default none)
 *       'sizey' (long, default none)
 *       'maxsizex' (long, default none)
 *       'maxsizey' (long, default none)
 *       'marginx' (long, default 5)
 *       'marginy' (long, default 5)
 *       'boxcolor' (color4, optional)
 *       'bordercolor' (color4, optional)
 *       'borderwidth' (long, default 0)
 *       'maxmessages' (long, optional)
 *       'queue' (boolean, default true)
 *       'boxfadetime' (long, default 0)
 * - DefineType: parameters:
 *       'type' (string)
 *       'slot' (string, default 'none')
 *       'textcolor' (color4, default white)
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
   * Define a new message slot.
   * If integer parameters are -1 they are considered as 'not given'.
   */
  virtual void DefineSlot (const char* name,
      const csVector2& position, MessageLocationAnchor positionAnchor,
      int sizex, int sizey, int maxsizex, int maxsizey,
      int marginx, int marginy,
      const csColor4& boxColor, const csColor4& borderColor,
      int borderWidth, int maxmessages, 
      bool queue, csTicks boxfadetime) = 0;

  /**
   * Define a new message type.
   */
  virtual void DefineType (const char* type, const char* slot,
      const csColor4& textColor, const char* font, int fontSize,
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

