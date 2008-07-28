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

CEL_IMPLEMENT_FACTORY_ALT (Timer, "pctools.timer", "pctimer")

//---------------------------------------------------------------------------

csStringID celPcTimer::id_elapsedticks = csInvalidStringID;
csStringID celPcTimer::id_currentticks = csInvalidStringID;
csStringID celPcTimer::id_time = csInvalidStringID;
csStringID celPcTimer::id_repeat = csInvalidStringID;
csStringID celPcTimer::id_name = csInvalidStringID;

PropertyHolder celPcTimer::propinfo;

celPcTimer::celPcTimer (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  enabled = false;
  wakeupframe = false;
  whereframe = CEL_EVENT_PRE;
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  CS_ASSERT (vc != 0);
  if (id_elapsedticks == csInvalidStringID)
  {
    id_elapsedticks = pl->FetchStringID ("cel.parameter.elapsedticks");
    id_currentticks = pl->FetchStringID ("cel.parameter.currentticks");
    id_time = pl->FetchStringID ("cel.parameter.time");
    id_repeat = pl->FetchStringID ("cel.parameter.repeat");
    id_name = pl->FetchStringID ("cel.parameter.name");
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
        CEL_FETCH_STRING_PAR (name,params,id_name);
        WakeUp ((csTicks)time, repeat, name);
        return true;
      }
    case action_wakeupframe:
      WakeUpFrame (CEL_EVENT_PRE);
      return true;
    case action_clear:
      {
        CEL_FETCH_STRING_PAR (name,params,id_name);
        Clear (name);
      }
      return true;
    default:
      return false;
  }
}

#define TIMER_SERIAL 3

csPtr<iCelDataBuffer> celPcTimer::Save ()
{
  //@@@ BROKEN!
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TIMER_SERIAL);
  databuf->Add (enabled);
  //databuf->Add ((int32)wakeup);
  //databuf->Add (repeat);
  databuf->Add (wakeupframe);
  //databuf->Add (wakeuponce);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTimer::Load (iCelDataBuffer* databuf)
{
  //@@@ BROKEN!
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TIMER_SERIAL) return false;
  enabled = databuf->GetBool ();
  //wakeup = databuf->GetInt32 ();
  //repeat = databuf->GetBool ();
  wakeupframe = databuf->GetBool ();
  //wakeuponce = databuf->GetBool ();

  return true;
}

void celPcTimer::Clear (const char* name)
{
  if (name == 0)
  {
    enabled = false;
    wakeupframe = false;
    timer_events.Empty ();

    pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
    pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, whereframe);
  }
  else if (strcmp (name, "frame") == 0)
  {
    wakeupframe = false;
    pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, whereframe);
  }
  else
  {
    size_t i = 0;
    while (i < timer_events.GetSize ())
    {
      if (timer_events[i].name == name)
	timer_events.DeleteIndex (i);
      else
	i++;
    }
    if (timer_events.GetSize () == 0)
      pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
  }
}

static int CompareTimeEvent (TimeEvent const& e1, TimeEvent const& e2)
{
  csTicks t1 = e1.firetime;
  csTicks t2 = e2.firetime;
  if (t1 < t2) return -1;
  else if (t1 > t2) return 1;
  else return 0;
}

void celPcTimer::WakeUp (csTicks t, bool repeat, const char* name)
{
  enabled = true;
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);

  csTicks current = vc->GetCurrentTicks ();
  TimeEvent te_new;
  te_new.firetime = current + t;
  te_new.amount = t;
  te_new.repeat = repeat;
  te_new.name = name ? name : "wakeup";
  timer_events.InsertSorted (te_new, CompareTimeEvent);

  pl->CallbackOnce ((iCelTimerListener*)this,
      timer_events[0].firetime - current, CEL_EVENT_PRE);
}

void celPcTimer::WakeUpFrame (int where)
{
  if (wakeupframe) return;
  enabled = true;
  wakeupframe = true;
  whereframe = where;
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

    params->GetParameter (0).Set ((int32)vc->GetElapsedTicks ());
    params->GetParameter (1).Set ((int32)vc->GetCurrentTicks ());
    iCelBehaviour* bh = entity->GetBehaviour ();
    if (bh)
    {
      celData ret;
      bh->SendMessage ("pctimer_wakeupframe", this, ret, params);
    }
    if (!dispatcher_wakeupframe)
    {
      dispatcher_wakeupframe = entity->QueryMessageChannel ()
        ->CreateMessageDispatcher (this, "cel.timer.wakeup.frame");
      if (!dispatcher_wakeupframe) return;
    }
    dispatcher_wakeupframe->SendMessage (params);
  }
}

void celPcTimer::TickOnce ()
{
  // To prevent the entity from being deleted during
  // the call of pctimer_wakeupframe we keep a temporary reference
  // here.
  csTicks current = vc->GetCurrentTicks ();
  csRef<iCelEntity> ref;
  while (timer_events.GetSize () > 0 && current >= timer_events[0].firetime)
  {
    TimeEvent te = timer_events[0];
    timer_events.DeleteIndex (0);
    ref = entity;
    if (te.repeat)
    {
      WakeUp (te.amount, te.repeat, te.name);
    }
    iCelBehaviour* bh = entity->GetBehaviour ();
    if (bh)
    {
      celData ret;
      csString msg = "pctimer_";
      msg += te.name;
      bh->SendMessage ((const char*)msg, this, ret, 0);
    }
    if (!te.dispatcher)
    {
      csString msg = "cel.timer.";
      msg += te.name;
      te.dispatcher = entity->QueryMessageChannel ()->CreateMessageDispatcher (
	    this, msg);
      if (!te.dispatcher) return;
    }
    te.dispatcher->SendMessage (0, 0);
  }
  if (timer_events.GetSize () > 0)
    pl->CallbackOnce ((iCelTimerListener*)this,
      timer_events[0].firetime - current, CEL_EVENT_PRE);
}

//---------------------------------------------------------------------------

