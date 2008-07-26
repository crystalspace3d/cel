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
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * - pcbag_addstring: A string is added to the bag (string).
 * - pcbag_removestring: A string is removed from the bag (string).
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - AddString: parameters 'value' (string).
 * - RemoveString: parameters 'value' (string).
 * - Clear: no parameters.
 * - HasString: parameters 'value' (string), returns bool.
 */
struct iPcBag : public virtual iBase
{
  SCF_INTERFACE (iPcBag, 0, 0, 1);

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
  //virtual csSet<csString>::GlobalIterator GetIterator () = 0;
};

#endif // __CEL_PF_BAG

