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

SCF_IMPLEMENT_FACTORY (celPfTools)

SCF_EXPORT_CLASS_TABLE (pftools)
  SCF_EXPORT_CLASS (celPfTools, "cel.pcfactory.tools",
  	"CEL Tools Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfTools)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfTools::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfTools::celPfTools (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfTools::~celPfTools ()
{
}

bool celPfTools::Initialize (iObjectRegistry* object_reg)
{
  celPfTools::object_reg = object_reg;
  return true;
}

iCelPropertyClass* celPfTools::CreatePropertyClass (const char* type)
{
  if (!strcmp (type, "pctooltip")) return new celPcTooltip (object_reg);
  else if (!strcmp (type, "pctimer")) return new celPcTimer (object_reg);
  else return NULL;
}

const char* celPfTools::GetTypeName (int idx) const
{
  switch (idx)
  {
    case 0: return "pctooltip";
    case 1: return "pctimer";
    default: return NULL;
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcTooltip)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTooltip)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTooltip::PcTooltip)
  SCF_IMPLEMENTS_INTERFACE (iPcTooltip)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTooltip::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcTooltip::celPcTooltip (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTooltip);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiEventHandler);
  visible = false;
  text = NULL;
  celPcTooltip::object_reg = object_reg;
  text_r = 0;
  text_g = 0;
  text_b = 0;
  bg_r = 255;
  bg_g = 255;
  bg_b = 0;
}

celPcTooltip::~celPcTooltip ()
{
  delete[] text;
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  if (q != NULL)
    q->RemoveListener (&scfiEventHandler);
}

void celPcTooltip::SetEntity (iCelEntity* entity)
{
  celPcTooltip::entity = entity;
}

iCelDataBuffer* celPcTooltip::GetDataBuffer ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer ();
  pl->DecRef ();
  return databuf;
}

void celPcTooltip::Save (iCelDataBuffer* databuf)
{
  databuf->SetDataCount (10);
  databuf->GetData (0)->Set ("visible", visible);
  databuf->GetData (1)->Set ("x", (uint16)x);
  databuf->GetData (2)->Set ("y", (uint16)y);
  databuf->GetData (3)->Set ("text", text);
  databuf->GetData (4)->Set ("text_r", (uint8)text_r);
  databuf->GetData (5)->Set ("text_g", (uint8)text_g);
  databuf->GetData (6)->Set ("text_b", (uint8)text_b);
  databuf->GetData (7)->Set ("bg_r", (int16)bg_r);	// Room for negative.
  databuf->GetData (8)->Set ("bg_g", (int16)bg_g);
  databuf->GetData (9)->Set ("bg_b", (int16)bg_b);
}

#define GET_DATA(var,databuf,name,dtype,field) 		\
	{						\
	  celData* cd = databuf->GetData (name);	\
	  if (!cd) return false;			\
	  if (cd->type != dtype) return false;		\
	  var = cd->value.field;			\
	}
#define GET_DATA_STRING(var,databuf,name)		\
	{						\
	  celData* cd = databuf->GetData (name);	\
	  if (!cd) return false;			\
	  if (cd->type != CEL_DATA_STRING) return false;		\
	  delete[] var;					\
	  var = csStrNew (cd->value.s);			\
	}


bool celPcTooltip::Load (iCelDataBuffer* databuf)
{
  GET_DATA (visible, databuf, "visible", CEL_DATA_BOOL, bo);
  GET_DATA (x, databuf, "x", CEL_DATA_UWORD, uw);
  GET_DATA (y, databuf, "y", CEL_DATA_UWORD, uw);
  GET_DATA_STRING (text, databuf, "text");
  GET_DATA (text_r, databuf, "text_r", CEL_DATA_UBYTE, ub);
  GET_DATA (text_g, databuf, "text_g", CEL_DATA_UBYTE, ub);
  GET_DATA (text_b, databuf, "text_b", CEL_DATA_UBYTE, ub);
  GET_DATA (bg_r, databuf, "bg_r", CEL_DATA_WORD, w);
  GET_DATA (bg_g, databuf, "bg_g", CEL_DATA_WORD, w);
  GET_DATA (bg_b, databuf, "bg_b", CEL_DATA_WORD, w);

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
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  q->RemoveListener (&scfiEventHandler);
  q->DecRef ();
}

void celPcTooltip::Show (int x, int y)
{
  celPcTooltip::x = x;
  celPcTooltip::y = y;
  if (visible) return;
  visible = true;
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  q->RemoveListener (&scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (&scfiEventHandler, trigger);
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
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTimer::PcTimer)
  SCF_IMPLEMENTS_INTERFACE (iPcTimer)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTimer::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcTimer::celPcTimer (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTimer);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiEventHandler);
  enabled = false;
  celPcTimer::object_reg = object_reg;
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != NULL);
}

celPcTimer::~celPcTimer ()
{
  if (vc) vc->DecRef ();
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  if (q != NULL)
    q->RemoveListener (&scfiEventHandler);
}

void celPcTimer::SetEntity (iCelEntity* entity)
{
  celPcTimer::entity = entity;
}

iCelDataBuffer* celPcTimer::GetDataBuffer ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer ();
  pl->DecRef ();
  return databuf;
}

void celPcTimer::Save (iCelDataBuffer* databuf)
{
  (void)databuf;
}

bool celPcTimer::Load (iCelDataBuffer* databuf)
{
  (void)databuf;
  return true;
}

void celPcTimer::Clear ()
{
  enabled = false;
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  q->RemoveListener (&scfiEventHandler);
  q->DecRef ();
}

void celPcTimer::WakeUp (csTicks t, bool repeat)
{
  enabled = true;
  iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
  CS_ASSERT (q != NULL);
  q->RemoveListener (&scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (&scfiEventHandler, trigger);
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

