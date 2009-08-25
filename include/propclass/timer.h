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
 * 'cel.action.' to get the ID of the action):
 * - WakeUp: parameters 'time' (long) and 'repeat' (bool).
 *   Optional 'name' (string) parameter for a named timer event.
 * - WakeUpFrame: no parameters.
 * - Clear: Optional 'name' (string) parameter for clearing a named
 *   timer event. Otherwise all timer events are cleared. If you want
 *   to clear only the anonymous timer then you can use "wakeup" as
 *   the name. Note that if no name is given then the frame event is
 *   also cleared.
 *
 * This property class can send out the following messages
 * to the behaviour:
 * - cel.timer.wakeup: wakeup message
 * - cel.timer.wakeup.frame: wakeup every frame (elapsedticks,currentticks)
 */
struct iPcTimer : public virtual iBase
{
  SCF_INTERFACE (iPcTimer, 0, 0, 2);

  /**
   * Wake up the Behaviour in 't' ticks. If the name is not
   * given then it is an anonymous timer (will send out
   * message 'pctimer_wakeup'). There can only be one
   * anonymous timer active. If you want multiple timer events
   * then use a named timer. The message in that case will
   * be 'pctimer_<name>'.
   * If 'repeat' is true then this will be repeated.
   * Otherwise there will only be one signal.
   */
  virtual void WakeUp (csTicks t, bool repeat, const char* name = 0) = 0;

  /**
   * Wake up the Behaviour every frame.
   */
  virtual void WakeUpFrame (int where) = 0;

  /**
   * Clear this timer. Optional 'name' (string) parameter for clearing a named
   * timer event. Otherwise all timer events are cleared. If you want
   * to clear only the anonymous timer then you can use "wakeup" as
   * the name. Note that if no name is given then the frame event is
   * also cleared. If name is equal to 'frame' then only the frame event
   * is cleared.
   */
  virtual void Clear (const char* name = 0) = 0;
};

#endif // __CEL_PF_TIMER__

