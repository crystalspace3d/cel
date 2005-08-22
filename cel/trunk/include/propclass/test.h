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

#ifndef __CEL_PF_TEST__
#define __CEL_PF_TEST__

#include "cstypes.h"
#include "csutil/scf.h"

/**
 * This is a test property class.
 * <p>
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * <ul>
 * <li>pctest_print: a message has been printed (message)
 * </ul>
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>Print: parameters 'message' (string).
 * </ul>
 * <p>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * <ul>
 * <li>counter (long, read/write): how many times something has been printed.
 * <li>max (long, read/write): maximum length of what was printed.
 * </ul>
 */
struct iPcTest : public virtual iBase
{
  SCF_INTERFACE (iPcTest, 0, 0, 1);

  /**
   * Print a message.
   */
  virtual void Print (const char* msg) = 0;
};

#endif // __CEL_PF_TEST__

