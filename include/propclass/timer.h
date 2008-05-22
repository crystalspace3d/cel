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
#include "iutil/evdefs.h"

/**
 * Timer propery class.
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - WakeUp: parameters 'time' (long) and 'repeat' (bool).
 * - WakeUpFrame: no parameters.
 * - Clear: no parameters.
 *
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * - pctimer_wakeup: wakeup message
 * - pctimer_wakeupframe: wakeup every frame (elapsedticks,currentticks)
 */
struct iPcTimer : public virtual iBase
{
  SCF_INTERFACE (iPcTimer, 0, 0, 1);

  /**
   * Wake up the Behaviour in 't' ticks.
   * If 'repeat' is true then this will be repeated.
   * Otherwise there will only be one signal.
   */
  virtual void WakeUp (csTicks t, bool repeat) = 0;

  /**
   * Wake up the Behaviour every frame.
   */
  virtual void WakeUpFrame (int where) = 0;

  /**
   * Clear this timer.
   */
  virtual void Clear () = 0;
};

#endif // __CEL_PF_TIMER__

