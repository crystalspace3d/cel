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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "pf/tools/toolfact.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/persist.h"
#include "bl/behave.h"
#include "csutil/util.h"
#include "csutil/debug.h"
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

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Tooltip, "pctooltip")
CEL_IMPLEMENT_FACTORY (Timer, "pctimer")

SCF_EXPORT_CLASS_TABLE (pftools)
  SCF_EXPORT_CLASS (celPfTooltip, "cel.pcfactory.tooltip",
  	"CEL Tooltip Property Class Factory")
  SCF_EXPORT_CLASS (celPfTimer, "cel.pcfactory.timer",
	"CEL Timer Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcTooltip)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTooltip)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTooltip::PcTooltip)
  SCF_IMPLEMENTS_INTERFACE (iPcTooltip)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcTooltip::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcTooltip::celPcTooltip (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTooltip);
  scfiEventHandler = NULL;
  visible = false;
  text = NULL;
  celPcTooltip::object_reg = object_reg;
  text_r = 0;
  text_g = 0;
  text_b = 0;
  bg_r = 255;
  bg_g = 255;
  bg_b = 0;
  DG_ADDI (this, "celPcTooltip()");
}

celPcTooltip::~celPcTooltip ()
{
  delete[] text;
  if (scfiEventHandler)
  {
    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q != NULL)
    {
      q->RemoveListener (scfiEventHandler);
      q->DecRef ();
    }
    scfiEventHandler->DecRef ();
  }
  DG_REM (this);
}

void celPcTooltip::SetEntity (iCelEntity* entity)
{
  celPcTooltip::entity = entity;
}

#define TOOLTIP_SERIAL 1

iCelDataBuffer* celPcTooltip::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (TOOLTIP_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (10);
  databuf->GetData (0)->Set (visible);
  databuf->GetData (1)->Set ((uint16)x);
  databuf->GetData (2)->Set ((uint16)y);
  databuf->GetData (3)->Set (text);
  databuf->GetData (4)->Set ((uint8)text_r);
  databuf->GetData (5)->Set ((uint8)text_g);
  databuf->GetData (6)->Set ((uint8)text_b);
  databuf->GetData (7)->Set ((int16)bg_r);	// Room for negative.
  databuf->GetData (8)->Set ((int16)bg_g);
  databuf->GetData (9)->Set ((int16)bg_b);
  return databuf;
}


bool celPcTooltip::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TOOLTIP_SERIAL) return false;
  if (databuf->GetDataCount () != 10) return false;
  celData* cd;
  cd = databuf->GetData (0); if (!cd) return false; visible = cd->value.bo;
  cd = databuf->GetData (1); if (!cd) return false; x = cd->value.uw;
  cd = databuf->GetData (2); if (!cd) return false; y = cd->value.uw;
  delete[] text; text = NULL;
  cd = databuf->GetData (3); if (!cd) return false;
 text = csStrNew (cd->value.s);
  cd = databuf->GetData (4); if (!cd) return false; text_r = cd->value.ub;
  cd = databuf->GetData (5); if (!cd) return false; text_g = cd->value.ub;
  cd = databuf->GetData (6); if (!cd) return false; text_b = cd->value.ub;
  cd = databuf->GetData (7); if (!cd) return false; bg_r = cd->value.w;
  cd = databuf->GetData (8); if (!cd) return false; bg_g = cd->value.w;
  cd = databuf->GetData (9); if (!cd) return false; bg_b = cd->value.w;

  return true;
}

void celPcTooltip::SetText (const char* t)
{
  delete[] text;
  text = csStrNew (t);
}

void celPcTooltip::Hide ()
{
  if (!visible) return;
  visible = false;
  if (scfiEventHandler)
  {
    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    CS_ASSERT (q != NULL);
    q->RemoveListener (scfiEventHandler);
    q->DecRef ();
  }
}

void celPcTooltip::Show (int x, int y)
{
  celPcTooltip::x = x;
  celPcTooltip::y = y;
  if (visible) return;
  visible = true;
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);
  q->DecRef ();
}

bool celPcTooltip::HandleEvent (iEvent& ev)
{
  if (!visible) return false;
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdPostProcess)
  {
    iGraphics3D* G3D = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
    iGraphics2D* G2D = CS_QUERY_REGISTRY (object_reg, iGraphics2D);

    G3D->BeginDraw (CSDRAW_2DGRAPHICS);
    iFontServer* fntsvr = G2D->GetFontServer ();
    CS_ASSERT (fntsvr != NULL);
    iFont* fnt = fntsvr->GetFont (0);
    if (fnt == NULL)
    {
      fnt = fntsvr->LoadFont (CSFONT_COURIER);
    }
    CS_ASSERT (fnt != NULL);
    //int fw, fh;
    //fnt->GetMaxSize (fw, fh);
    //int sw = G2D->GetWidth ();
    //int sh = G2D->GetHeight ();
    int fgcolor = G3D->GetTextureManager ()->FindRGB (text_r, text_g, text_b);
    int bgcolor = bg_r == -1 ? 0 : G3D->GetTextureManager ()->
    	FindRGB (bg_r, bg_g, bg_b);
    //G2D->DrawBox (x, y, w, h, bgcolor);
    //int maxlen = fnt->GetLength (text, w-10);
    G2D->Write (fnt, x, y, fgcolor, bgcolor, text);

    G2D->DecRef ();
    G3D->DecRef ();
  }
  return false;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcTimer)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTimer)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTimer::PcTimer)
  SCF_IMPLEMENTS_INTERFACE (iPcTimer)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcTimer::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcTimer::celPcTimer (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTimer);
  scfiEventHandler = NULL;
  enabled = false;
  celPcTimer::object_reg = object_reg;
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != NULL);
  DG_ADDI (this, "celPcTimer()");
}

celPcTimer::~celPcTimer ()
{
  if (vc) vc->DecRef ();
  if (scfiEventHandler)
  {
    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q != NULL)
    {
      q->RemoveListener (scfiEventHandler);
      q->DecRef ();
    }
    scfiEventHandler->DecRef ();
  }
  DG_REM (this);
}

void celPcTimer::SetEntity (iCelEntity* entity)
{
  celPcTimer::entity = entity;
}

#define TIMER_SERIAL 1

iCelDataBuffer* celPcTimer::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (TIMER_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (4);
  databuf->GetData (0)->Set (enabled);
  databuf->GetData (1)->Set ((int32)wakeup);
  databuf->GetData (2)->Set ((int32)wakeup_todo);
  databuf->GetData (3)->Set (repeat);
  return databuf;
}

bool celPcTimer::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TIMER_SERIAL) return false;
  if (databuf->GetDataCount () != 4) return false;
  celData* cd;
  cd = databuf->GetData (0); if (!cd) return false;
  enabled = cd->value.bo;
  cd = databuf->GetData (1); if (!cd) return false;
  wakeup = cd->value.l;
  cd = databuf->GetData (2); if (!cd) return false;
  wakeup_todo = cd->value.l;
  cd = databuf->GetData (3); if (!cd) return false;
  repeat = cd->value.bo;

  return true;
}

void celPcTimer::Clear ()
{
  enabled = false;
  if (scfiEventHandler)
  {
    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    CS_ASSERT (q != NULL);
    q->RemoveListener (scfiEventHandler);
    q->DecRef ();
  }
}

void celPcTimer::WakeUp (csTicks t, bool repeat)
{
  enabled = true;
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);
  q->DecRef ();

  celPcTimer::repeat = repeat;
  wakeup = t;
  wakeup_todo = 0;
}

bool celPcTimer::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdPreProcess)
  {
    csTicks elapsed = vc->GetElapsedTicks ();
    wakeup_todo += elapsed;
    if (wakeup_todo >= wakeup)
    {
      if (repeat)
      {
        wakeup_todo = 0;
      }
      else
      {
        Clear ();
      }
      iCelBehaviour* bh = entity->GetBehaviour ();
      CS_ASSERT (bh != NULL);
      bh->SendMessage ("timer_wakeup", NULL);
    }
  }
  return false;
}

//---------------------------------------------------------------------------

