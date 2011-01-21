/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_TIMERFACT__
#define __CEL_PF_TIMERFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/parray.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/timer.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;
struct iVirtualClock;
struct iGraphics2D;
struct iGraphics3D;
struct iFont;
class csStringArray;

/**
 * Factory for tools.
 */
CEL_DECLARE_FACTORY(Timer)

struct TimeEvent
{
  csTicks firetime;	// When to fire.
  csTicks amount;	// Amount to fire on again (in case of repeat).
  bool repeat;
  csString name;	// Name of this timer (or "wakeup" for anonymous).
  csRef<iMessageDispatcher> dispatcher;
};

/**
 * This is a timer property class.
 */
class celPcTimer : public scfImplementationExt1<
	celPcTimer, celPcCommon, iPcTimer>
{
private:
  csRef<iVirtualClock> vc;
  bool enabled;
  bool wakeupframe;
  int whereframe;
  csArray<TimeEvent> timer_events;

  static PropertyHolder propinfo;

  enum actionids
  {
    action_wakeup = 0,
    action_wakeupframe,
    action_clear
  };

  static csStringID id_elapsedticks;
  static csStringID id_currentticks;
  static csStringID id_time;
  static csStringID id_repeat;
  static csStringID id_name;
  celGenericParameterBlock* params;

  csRef<iMessageDispatcher> dispatcher_wakeupframe;

public:
  celPcTimer (iObjectRegistry* object_reg);
  virtual ~celPcTimer ();

  virtual void WakeUp (csTicks t, bool repeat, const char* name);
  virtual void WakeUpFrame (int where);
  virtual void Clear (const char* name);

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
  virtual void TickOnce ();
  virtual void TickEveryFrame ();
};

#endif // __CEL_PF_TIMERFACT__

