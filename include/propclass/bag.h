/*
    Crystal Space Entity Layer
    Copyright (C) 2007 by Jorrit Tyberghein

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

#ifndef __CEL_PF_BAG
#define __CEL_PF_BAG

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/set.h"
#include "physicallayer/datatype.h"

struct iCelEntity;
struct iCelPropertyClass;

/**
 * This is a bag of strings property class.
 *
 * This property class supports the following actions (add prefix 'cel.bag.action.'
 * if you want to access this action through a message):
 * - AddString: parameters 'value' (string).
 * - RemoveString: parameters 'value' (string).
 * - Clear: no parameters.
 * - HasString: parameters 'value' (string), returns bool.
 * - SendMessage: parameters 'msgid' (string), ...
 *
 * This property class supports the following properties:
 * - size (long, read only): the size of the bag.
 */
struct iPcBag : public virtual iBase
{
  SCF_INTERFACE (iPcBag, 0, 0, 2);

  /**
   * Add an string to this bag.
   */
  virtual void AddString (const char* str) = 0;

  /**
   * Remove a string from this bag.
   */
  virtual void RemoveString (const char* str) = 0;

  /**
   * Empty the bag.
   */
  virtual void Clear () = 0;

  /**
   * Check if this bag contains the string.
   */
  virtual bool HasString (const char* str) = 0;

  /**
   * Return an iterator iterating over all strings.
   */
  virtual csSet<csString>::GlobalIterator GetIterator () = 0;

  /**
   * Treat the strings in this bag as names of entities and then
   * send this message to all entities in the bag.
   * @return true if at least one of the entities handled the message (returned
   * true).
   */
  virtual bool SendMessage (const char* msgid, iCelParameterBlock* params = 0) = 0;
};

#endif // __CEL_PF_BAG

