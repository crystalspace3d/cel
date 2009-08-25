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
 *
 * This property class can send out the following messages
 * to the behaviour:
 * - cel.misc.test.print: a message has been printed (message)
 *
 * This property class supports the following actions:
 * - Print: parameters 'message' (string).
 *
 * This property class supports the following properties:
 * - counter (long, read/write): how many times something has been printed.
 * - max (long, read/write): maximum length of what was printed.
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

