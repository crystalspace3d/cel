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
  ANCHOR_INVALID = -1,
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
   * Get the id of the previous message as it was returned by Next()
   * but don't advance the iterator.
   */
  virtual const char* GetID () = 0;

  /**
   * Get the amount of times this message has been given to the user.
   */
  virtual size_t GetCounter () = 0;
};

/**
 * This is a messenger property class. This property class can display various
 * types of messages to the user. Every type of message is configurable (with
 * regards to timing, how and where it is displayed, if it is remembered or not, ...)
 *
 * Locations on screen where messages can be put are defined as 'slots'. There
 * is one predefined slot called 'none' which doesn't display anything.
 *
 * Messages are grouped in types. A slot can display messages of different types
 * but a single type can only go to one slot.
 *
 * This property class supports the following actions (add prefix 'cel.messenger.action.'
 * if you want to access this action through a message):
 * - DefineSlot: parameters:
 *       'name' (string)
 *       'position' (vector2, a position which represents a position of the message box
 *                   as specified by 'boxanchor'. This position is itself relative to
 *                   the 'screenanchor').
 *       'boxanchor' (string, one of c, nw, n, ne, e, se, s, sw, w)
 *       'screenanchor' (string, one of c, nw, n, ne, e, se, s, sw, w)
 *       'sizex' (long, default none): fixed horizontal size.
 *       'sizey' (long, default none): fixed vertical size.
 *       'maxsizex' (string, default '%90'): maximum horizontal size (only if sizex is not given)
 *                  See SetSlotDimensions() for more information about this format.
 *       'maxsizey' (string, default '%90'): maximum vertical size (only if sizey is not given)
 *                  See SetSlotDimensions() for more information about this format.
 *       'marginx' (long, default 5): horizontal margin
 *       'marginy' (long, default 3): vertical margin
 *       'boxcolor' (color4, optional): default is no color (no box)
 *       'bordercolor' (color4, optional): default is no border
 *       'borderwidth' (float, default 0): the width of the border
 *       'roundness' (long, default 0): the roundness of the box corners
 *       'maxmessages' (long, optional): maximum number of active messages that can be
 *                     displayed at the same time in this slot. The size of the slot can
 *                     further restrict the actual number of visible messages).
 *       'queue' (boolean, default true): if there are more then maxmessages messages then
 *                     the messages are kept in a queue until they can be displayed. Otherwise
 *                     messages push away the oldest messages.
 *       'boxfadetime' (long, default 0)
 * - DefineType: parameters:
 *       'type' (string): the name of this type.
 *       'slot' (string, default 'none')
 *       'textcolor' (color4, default white)
 *       'font' (string, optional)
 *       'fontsize' (long, optional)
 *       'timeout' (float, default 2.0): time before the message starts fading away
 *       'fadetime' (float, default 1.0): time the message needs to fade away
 *       'click' (bool, default false): first active messages starts fading away faster
 *               if the player clicks.
 *       'log' (bool, default false): log messages. If this is set the messages need a unique id.
 *       'cyclefirst' (string, default 'random'): the message selected for the first message.
 *             - random: pick a random message from the message list
 *             - <number>: use a specific message from the list (first is index 0)
 *             - sequence: cycle through all messages in sequence starting with 0. The
 *               message will need a unique ID.
 *             - none: no message is given
 *       'cyclenext' (string, optional): this is used if the message has a unique ID and
 *              for all messages except the first.
 * - Message: parameters:
 *       'type' (string, optional, if not set the default type is used)
 *       'id' (string, optional): required in some cases depending on the type.
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
   */
  virtual void DefineSlot (const char* name) = 0;

  /**
   * Set the slot position.
   */
  virtual void SetSlotPosition (const char* name, const csVector2& position,
      MessageLocationAnchor boxAnchor,
      MessageLocationAnchor screenAnchor) = 0;

  /**
   * Set the slot dimensions.
   * maxsizex and maxsizey are strings with the following format:
   *   - '': empty string or 0 means no maximum given. In that case '%90' is
   *         assumed.
   *   - 'number': a specific dimension
   *   - '%number': a relative dimension (to the maximum screen size). 100 is max.
   *   - '-number': number of pixels less then maximum screen size.
   * If sizex and/or sizey are -1 then they are considered not giving and
   * the box can grow upto the maximum size.
   */
  virtual void SetSlotDimensions (const char* name,
      int sizex, int sizey, const char* maxsizex, const char* maxsizey,
      int marginx, int marginy) = 0;

  /**
   * Set the slot box visual attributes.
   */
  virtual void SetSlotBoxAttributes (const char* name,
      const csColor& boxColor, const csColor4& borderColor,
      float borderWidth, int roundness, float boxfadetime) = 0;

  /**
   * Set the slot message handling.
   */
  virtual void SetSlotMessageHandling (const char* name, int maxmessages, bool queue) = 0;

  /**
   * Define a new message type.
   */
  virtual void DefineType (const char* type, const char* slot,
      const csColor4& textColor, const char* font, int fontSize,
      float timeout, float fadetime, bool click, bool log,
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
      const csStringArray& msgs) = 0;
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

