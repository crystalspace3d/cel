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
#include "plugins/propclass/tools/toolfact.h"
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

CEL_IMPLEMENT_FACTORY_ALT (Tooltip, "pc2d.tooltip", "pctooltip")

//---------------------------------------------------------------------------

celPcTooltip::celPcTooltip (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  visible = false;
  text_r = 0;
  text_g = 0;
  text_b = 0;
  bg_r = 255;
  bg_g = 255;
  bg_b = 0;
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  G3D = csQueryRegistry<iGraphics3D> (object_reg);
  G2D = csQueryRegistry<iGraphics2D> (object_reg);
}

celPcTooltip::~celPcTooltip ()
{
}

void celPcTooltip::SetText (const char* t)
{
  lines.DeleteAll ();
  if (t && strcmp (t, "") != 0)
  {
    csString str (t);
    size_t lastpos = 0, pos;
    do
    {
      pos = str.FindFirst ('\n', lastpos);
      lines.Push (str.Slice (lastpos, pos - lastpos));
      lastpos = pos + 1;
    }
    while (pos != (size_t) - 1);
    CalculateExtents ();
  }
  else
  {
    Hide ();
  }
}

void celPcTooltip::SetFont (iFont* font)
{
  fnt = font;
  CalculateExtents ();
}

void celPcTooltip::CalculateExtents ()
{
  width = 0;
  height = 0;
  int linewidth, numlines = 0;
  csString line;
  csStringArray::Iterator lineit = lines.GetIterator ();
  if (!fnt)
  {
    iFontServer* fntsvr = G2D->GetFontServer ();
    CS_ASSERT (fntsvr != 0);
    fnt = fntsvr->LoadFont (CSFONT_COURIER);
  }
  CS_ASSERT (fnt.IsValid ());
  while (lineit.HasNext ())
  {
    line = lineit.Next ();
    linewidth = fnt->GetLength (line.GetData (), G2D->GetWidth ());
    if (linewidth > width)
      width = linewidth;
    numlines++;
  }
  height = numlines * fnt->GetTextHeight ();
}

void celPcTooltip::Hide ()
{
  if (!visible) return;
  visible = false;
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_POST);
}

void celPcTooltip::Show (int x, int y)
{
  celPcTooltip::x = x;
  celPcTooltip::y = y;
  if (visible) return;
  visible = true;
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_POST);
}

void celPcTooltip::TickEveryFrame ()
{
  if (!visible) return;
  G3D->BeginDraw (CSDRAW_2DGRAPHICS);
  if (!fnt)
  {
    iFontServer* fntsvr = G2D->GetFontServer ();
    CS_ASSERT (fntsvr != 0);
    fnt = fntsvr->LoadFont (CSFONT_COURIER);
  }
  CS_ASSERT (fnt != 0);
  int fgcolor = G2D->FindRGB (text_r, text_g, text_b);
  int bgcolor = bg_r == -1 ? -1 : G2D->FindRGB (bg_r, bg_g, bg_b);
  if (bg_r != -1)
  {
    G2D->DrawBox (x, y, width, height, bgcolor);
  }
  csString line;
  csStringArray::Iterator lineit = lines.GetIterator ();
  int linenum = 0, offset = 0;
  while (lineit.HasNext ())
  {
    line = lineit.Next ();
    fflush (stdout);
    if (justify == CEL_TOOLTIP_CENTER)
      offset = (width - fnt->GetLength (line.GetData (), width)) / 2;
    else if (justify == CEL_TOOLTIP_RIGHT)
      offset = (width - fnt->GetLength (line.GetData (), width));
    G2D->Write (fnt, x + offset, y + (linenum * fnt->GetTextHeight ()),
	fgcolor, bgcolor, line.GetData ());
    linenum++;
  }
}

//---------------------------------------------------------------------------

