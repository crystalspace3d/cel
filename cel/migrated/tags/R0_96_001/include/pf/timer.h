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

#ifndef __CEL_PF_TIMER__
#define __CEL_PF_TIMER__

#include "cstypes.h"
#include "csutil/scf.h"

SCF_VERSION (iPcTimer, 0, 0, 1);

/**
 * Timer propery class.
 */
struct iPcTimer : public iBase
{
  /**
   * Wake up the Behaviour in 't' ticks.
   * If 'repeat' is true then this will be repeated.
   * Otherwise there will only be one signal.
   */
  virtual void WakeUp (csTicks t, bool repeat) = 0;

  /**
   * Clear this timer.
   */
  virtual void Clear () = 0;
};

#endif // __CEL_PF_TIMER__
