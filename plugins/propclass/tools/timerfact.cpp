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

#include "cssysdef.h"
#include "plugins/propclass/tools/timerfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (Timer, "pctimer")

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcTimer)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTimer)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTimer::PcTimer)
  SCF_IMPLEMENTS_INTERFACE (iPcTimer)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

csStringID celPcTimer::id_elapsedticks = csInvalidStringID;
csStringID celPcTimer::id_currentticks = csInvalidStringID;
csStringID celPcTimer::id_time = csInvalidStringID;
csStringID celPcTimer::id_repeat = csInvalidStringID;

PropertyHolder celPcTimer::propinfo;

celPcTimer::celPcTimer (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTimer);
  enabled = false;
  wakeupframe = false;
  wakeuponce = false;
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  CS_ASSERT (vc != 0);
  if (id_elapsedticks == csInvalidStringID)
  {
    id_elapsedticks = pl->FetchStringID ("cel.parameter.elapsedticks");
    id_currentticks = pl->FetchStringID ("cel.parameter.currentticks");
    id_time = pl->FetchStringID ("cel.parameter.time");
    id_repeat = pl->FetchStringID ("cel.parameter.repeat");
  }
  params = new celGenericParameterBlock (2);
  params->SetParameterDef (0, id_elapsedticks, "elapsedticks");
  params->SetParameterDef (1, id_currentticks, "currentticks");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_wakeup, "cel.action.WakeUp");
    AddAction (action_wakeupframe, "cel.action.WakeUpFrame");
    AddAction (action_clear, "cel.action.Clear");
  }
}

celPcTimer::~celPcTimer ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcTimer);
}

bool celPcTimer::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_wakeup:
      {
        CEL_FETCH_LONG_PAR (time,params,id_time);
        if (!p_time) return false;
        CEL_FETCH_BOOL_PAR (repeat,params,id_repeat);
        if (!p_repeat) return false;
        WakeUp ((csTicks)time, repeat);
        return true;
      }
    case action_wakeupframe:
      WakeUpFrame (CEL_EVENT_PRE);
      return true;
    case action_clear:
      Clear ();
      return true;
    default:
      return false;
  }
}

#define TIMER_SERIAL 3

csPtr<iCelDataBuffer> celPcTimer::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TIMER_SERIAL);
  databuf->Add (enabled);
  databuf->Add ((int32)wakeup);
  databuf->Add (repeat);
  databuf->Add (wakeupframe);
  databuf->Add (wakeuponce);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTimer::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TIMER_SERIAL) return false;
  enabled = databuf->GetBool ();
  wakeup = databuf->GetInt32 ();
  repeat = databuf->GetBool ();
  wakeupframe = databuf->GetBool ();
  wakeuponce = databuf->GetBool ();

  return true;
}

void celPcTimer::Clear ()
{
  enabled = false;
  wakeupframe = false;
  wakeuponce = false;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
}

void celPcTimer::WakeUp (csTicks t, bool repeat)
{
  enabled = true;
  wakeuponce = true;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  pl->CallbackOnce ((iCelTimerListener*)this, t, CEL_EVENT_PRE);

  celPcTimer::repeat = repeat;
  wakeup = t;
}

void celPcTimer::WakeUpFrame (int where)
{
  if (wakeupframe) return;
  enabled = true;
  wakeupframe = true;
  pl->CallbackEveryFrame ((iCelTimerListener*)this, where);
}

void celPcTimer::TickEveryFrame ()
{
  // To prevent the entity from being deleted during
  // the call of pctimer_wakeupframe we keep a temporary reference
  // here.
  csRef<iCelEntity> ref;
  if (wakeupframe)
  {
    ref = entity;
    iCelBehaviour* bh = entity->GetBehaviour ();
    if (bh)
    {
      params->GetParameter (0).Set ((int32)vc->GetElapsedTicks ());
      params->GetParameter (1).Set ((int32)vc->GetCurrentTicks ());
      celData ret;
      bh->SendMessage ("pctimer_wakeupframe", this, ret, params);
    }
  }
}

void celPcTimer::TickOnce ()
{
  // To prevent the entity from being deleted during
  // the call of pctimer_wakeupframe we keep a temporary reference
  // here.
  csRef<iCelEntity> ref;
  if (wakeuponce)
  {
    ref = entity;
    if (repeat)
    {
      pl->CallbackOnce ((iCelTimerListener*)this, wakeup, CEL_EVENT_PRE);
    }
    else
    {
      wakeuponce = false;
    }
    iCelBehaviour* bh = entity->GetBehaviour ();
    if (bh)
    {
      celData ret;
      bh->SendMessage ("pctimer_wakeup", this, ret, 0);
    }
  }
}

//---------------------------------------------------------------------------

