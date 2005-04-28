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

/**
 * This is a timer property class.
 */
class celPcTimer : public celPcCommon
{
private:
  csRef<iVirtualClock> vc;
  bool enabled;
  csTicks wakeup;
  bool repeat;
  bool wakeupframe;
  bool wakeuponce;

  static csStringID action_wakeup;
  static csStringID action_wakeupframe;
  static csStringID action_clear;

  static csStringID id_elapsedticks;
  static csStringID id_currentticks;
  static csStringID id_time;
  static csStringID id_repeat;
  celGenericParameterBlock* params;

public:
  celPcTimer (iObjectRegistry* object_reg);
  virtual ~celPcTimer ();

  void WakeUp (csTicks t, bool repeat);
  void WakeUpFrame (int where = cscmdPreProcess);
  void Clear ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pctimer"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);
  virtual void TickOnce ();
  virtual void TickEveryFrame ();

  struct PcTimer : public iPcTimer
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcTimer);
    virtual void WakeUp (csTicks t, bool repeat)
    {
      scfParent->WakeUp (t, repeat);
    }
    virtual void WakeUpFrame (int where = cscmdPreProcess)
    {
      scfParent->WakeUpFrame (where);
    }
    virtual void Clear ()
    {
      scfParent->Clear ();
    }
  } scfiPcTimer;
};

#endif // __CEL_PF_TIMERFACT__

