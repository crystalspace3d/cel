/*
    Crystal Space Entity Layer
    Copyright (C) 2012 by Jorrit Tyberghein

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
#include "csutil/scanstr.h"
#include "iutil/objreg.h"
#include "plugins/propclass/messenger/messengerfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/fontserv.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (Messenger, "pctools.messenger")

//---------------------------------------------------------------------------

class MessageLogIterator : public scfImplementation1<MessageLogIterator,
  iMessageLogIterator>
{
private:
  MessageHash::GlobalIterator it;
  csString id;
  size_t counter;

public:
  MessageLogIterator (MessageHash::GlobalIterator it) :
    scfImplementationType (this), it (it) { }
  virtual ~MessageLogIterator () { }
  virtual bool HasNext () const
  {
    return it.HasNext ();
  }
  virtual const char* Next ()
  {
    MessageCounter msg = it.Next (id);
    counter = msg.counter;
    return msg.message;
  }
  virtual const char* GetID ()
  {
    return id;
  }
  virtual size_t GetCounter ()
  {
    return counter;
  }
};

//---------------------------------------------------------------------------

bool TimedMessage::LayoutWord (int maxwidth, const char* word)
{
  int w, h;
  font->GetDimensions (word, w, h);

  LayoutedLine& line = layoutedLines.Get (layoutedLines.GetSize ()-1);
  int neww = line.w + w;
  if (line.w > 0)
    neww += curspacew;

  if (neww > maxwidth) return false;

  if (line.w > 0)
    line.line += " ";
  line.line += word;
  line.w = neww;

  return true;
}

void TimedMessage::LayoutNewLine ()
{
  int w, h;
  font->GetDimensions ("abcghijklABDGHIJKLM0123456789,!?", w, h);

  LayoutedLine layoutline ("", timeleft + fadetime, font, color);
  layoutline.w = 0;
  layoutline.h = h;
  layoutedLines.Push (layoutline);
}

void TimedMessage::LayoutLine (int maxwidth, const char* line)
{
  LayoutNewLine ();

  csStringArray words (line, " ", csStringArray::delimIgnore);
  for (size_t w = 0 ; w < words.GetSize () ; w++)
  {
    if (!LayoutWord (maxwidth, words.Get (w)))
    {
      // No more room for this line.
      LayoutNewLine ();
      if (!LayoutWord (maxwidth, words.Get (w)))
      {
	// Still doesn't fit. The word may simply be too big. We just
	// ignore it for now (needs to be handled better!)
	// @@@
	printf ("Ignored word '%s'!\n", (const char*)words.Get (w));
	fflush (stdout);
      }
    }
  }
}

void TimedMessage::Layout (int maxwidth)
{
  int h;
  font->GetDimensions (" ", curspacew, h);

  csStringArray lines (message, "\n");
  for (size_t l = 0 ; l < lines.GetSize () ; l++)
    LayoutLine (maxwidth, lines.Get (l));
}

#define LINE_MARGIN 2

int TimedMessage::CalculateFittingVerticalHeight (int maxheight)
{
  int h = 0;
  for (size_t i = 0 ; i < layoutedLines.GetSize () ; i++)
  {
    LayoutedLine& line = layoutedLines.Get (i);
    line.render = false;
    if (line.timeleft > 0.0f)
    {
      int margin = h ? LINE_MARGIN : 0;
      if (line.h + margin + h >= maxheight) return h;
      line.render = true;
      h += line.h + margin;
    }
  }
  return h;
}

int TimedMessage::GetMaxWidth ()
{
  int width = 0;
  for (size_t i = 0 ; i < layoutedLines.GetSize () ; i++)
    if (layoutedLines.Get (i).w > width)
      width = layoutedLines.Get (i).w;
  return width;
}

bool TimedMessage::IsMessageReady ()
{
  for (size_t i = 0 ; i < layoutedLines.GetSize () ; i++)
  {
    LayoutedLine& line = layoutedLines.Get (i);
    if (line.timeleft > 0.0f)
      return false;
  }
  return true;
}

float TimedMessage::HandleElapsed (float elapsed)
{
  updated = false;
  fading = 0.0f;

  int part = 0;		// If this gets to 2 we have unshown lines at the end.
  float newelapsed = elapsed;
  for (size_t i = 0 ; i < layoutedLines.GetSize () ; i++)
  {
    LayoutedLine& line = layoutedLines.Get (i);
    if (line.render)
    {
      part = 1;
      if (line.timeleft > 0)
      {
	line.timeleft -= elapsed;
	if (line.timeleft >= fadetime) fading = 1000000000.0f;
	else if (line.timeleft > fading) fading = line.timeleft;

	if (line.timeleft <= 0)
	{
	  if (-line.timeleft < newelapsed)
	  {
	    newelapsed = -line.timeleft;
	    if (newelapsed < 0) newelapsed = 0;
	  }
	}
	else newelapsed = 0.0f;
      }

      if (line.timeleft <= 0)
      {
	line.render = false;
	updated = true;
      }
    }
    else if (part == 1) part = 2;
  }
  if (part == 2) return 0.0f;
  return newelapsed;
}

//---------------------------------------------------------------------------

MessageSlot::~MessageSlot ()
{
  delete boxPen;
  delete borderPen;
}

void MessageSlot::InitPen ()
{
  delete boxPen; boxPen = 0;
  delete borderPen; borderPen = 0;
  if (boxColor.alpha > 0.01f)
  {
    boxPen = new csPen (g3d->GetDriver2D (), g3d);
    boxPen->SetFlag (CS_PEN_FILL);
    boxPen->SetColor (boxColor);
  }
  if (borderColor.alpha > 0.01f && borderWidth > 0.0f)
  {
    borderPen = new csPen (g3d->GetDriver2D (), g3d);
    borderPen->SetColor (borderColor);
    borderPen->SetPenWidth (borderWidth);
  }
}

void MessageSlot::UpdateScreenSize ()
{
  int sw = g3d->GetWidth ();
  int sh = g3d->GetHeight ();
  if (swidth != sw || sheight != sh)
  {
    swidth = sw;
    if (maxsizex.StartsWith ('%'))
    {
      float percent;
      csScanStr (maxsizex.GetData () + 1, "%f", &percent);
      cachedMaxsizex = sw * percent / 100.0f;
    }
    else if (maxsizex.StartsWith ('-'))
    {
      int pixels;
      csScanStr (maxsizex.GetData () + 1, "%d", &pixels);
      cachedMaxsizex = sw - pixels;
    }
    else
    {
      int pixels;
      csScanStr (maxsizex.GetData (), "%d", &pixels);
      cachedMaxsizex = pixels;
    }

    sheight = sh;
    if (maxsizey.StartsWith ('%'))
    {
      float percent;
      csScanStr (maxsizey.GetData () + 1, "%f", &percent);
      cachedMaxsizey = sh * percent / 100.0f;
    }
    else if (maxsizey.StartsWith ('-'))
    {
      int pixels;
      csScanStr (maxsizey.GetData () + 1, "%d", &pixels);
      cachedMaxsizey = sh - pixels;
    }
    else
    {
      int pixels;
      csScanStr (maxsizey.GetData (), "%d", &pixels);
      cachedMaxsizey = pixels;
    }
  }
}

void MessageSlot::RecalculateCurrentSize ()
{
  UpdateScreenSize ();

  if (sizex > 0) cursizex = sizex - marginx * 2;
  else
  {
    cursizex = 0;
    for (size_t i = 0 ; i < activeMessages.GetSize () ; i++)
    {
      TimedMessage* tm = activeMessages.Get (i);
      int maxwidth = tm->GetMaxWidth ();
      if (maxwidth > cursizex) cursizex = maxwidth;
    }
  }
  int maxheight;
  if (sizey > 0) maxheight = sizey - marginy * 2;
  else maxheight = cachedMaxsizey - marginy * 2;
  cursizey = 0;
  for (size_t i = 0 ; i < activeMessages.GetSize () ; i++)
  {
    TimedMessage* tm = activeMessages.Get (i);
    int margin = cursizey ? LINE_MARGIN : 0;
    int fitheight = tm->CalculateFittingVerticalHeight (maxheight - cursizey - margin);
    // If we could fit no lines and we already have lines on screen then
    // we break. If we could fit no lines and there are no lines on screen we don't
    // break because we can be in the situation that a message has just timed out and
    // so no lines will be visible for that message.
    if (fitheight == 0 && cursizey != 0) break;
    cursizey += margin + fitheight;
  }
  if (sizey > 0) cursizey = sizey - marginy * 2;

  int sx = swidth / 2, sy = sheight / 2;
  switch (screenAnchor)
  {
    case ANCHOR_CENTER:                                            break;
    case ANCHOR_NORTH:                         sy = 0;             break;
    case ANCHOR_NORTHWEST: sx = 0;             sy = 0;             break;
    case ANCHOR_WEST:      sx = 0;                                 break;
    case ANCHOR_SOUTHWEST: sx = 0;             sy = sheight;       break;
    case ANCHOR_SOUTH:                         sy = sheight;       break;
    case ANCHOR_SOUTHEAST: sx = swidth;        sy = sheight;       break;
    case ANCHOR_EAST:      sx = swidth;                            break;
    case ANCHOR_NORTHEAST: sx = swidth;        sy = 0;             break;
    default: break;
  }
  int cx = sx + position.x, cy = sy + position.y;
  switch (boxAnchor)
  {
    case ANCHOR_CENTER:    cx -= cursizex / 2; cy -= cursizey / 2; break;
    case ANCHOR_NORTH:     cx -= cursizex / 2;                     break;
    case ANCHOR_NORTHWEST:                                         break;
    case ANCHOR_WEST:                          cy -= cursizey / 2; break;
    case ANCHOR_SOUTHWEST:                     cy -= cursizey;     break;
    case ANCHOR_SOUTH:     cx -= cursizex / 2; cy -= cursizey;     break;
    case ANCHOR_SOUTHEAST: cx -= cursizex;     cy -= cursizey;     break;
    case ANCHOR_EAST:      cx -= cursizex;     cy -= cursizey / 2; break;
    case ANCHOR_NORTHEAST: cx -= cursizex;                         break;
    default: break;
  }
  finalPosition.Set (cx, cy);
}

void MessageSlot::Message (const char* msg, float timeout, float fadetime,
    iFont* font, int color)
{
  UpdateScreenSize ();

  TimedMessage* tm = new TimedMessage (msg, timeout, fadetime, font, color);
  tm->Layout (cachedMaxsizex - marginx * 2);

  if (activeMessages.GetSize () >= size_t (maxmessages))
  {
    if (queue)
    {
      queuedMessages.Push (tm);
      return;
    }
    else
    {
      while (activeMessages.GetSize () >= size_t (maxmessages)
	  && activeMessages.GetSize () > 0)
      {
	activeMessages.DeleteIndex (0);
      }
    }
  }
  activeMessages.Push (tm);
  RecalculateCurrentSize ();
}

void MessageSlot::CleanReadyMessages ()
{
  size_t i = 0;
  while (i < activeMessages.GetSize ())
  {
    if (activeMessages.Get (i)->IsMessageReady ())
      activeMessages.DeleteIndex (i);
    else
      i++;
  }
}

void MessageSlot::CheckNewMessages ()
{
  while (queuedMessages.GetSize () > 0 && activeMessages.GetSize () < size_t (maxmessages))
  {
    TimedMessage* tm = queuedMessages.Extract (0);
    activeMessages.Push (tm);
  }
}

void MessageSlot::HandleElapsed (float elapsed)
{
  if (activeMessages.GetSize () == 0 && queuedMessages.GetSize () == 0) return;
  CleanReadyMessages ();
  CheckNewMessages ();
  if (activeMessages.GetSize () == 0) return;

  bool needslayout;
  float maxfade = 0.0f;
  float e = elapsed;
  for (size_t i = 0 ; i < activeMessages.GetSize () ; i++)
  {
    TimedMessage* tm = activeMessages.Get (i);
    e = tm->HandleElapsed (e);
    if (tm->LinesAreFinished ()) needslayout = true;
    if (tm->GetMaxFadeTimeLeft () > maxfade) maxfade = tm->GetMaxFadeTimeLeft ();
  }
  if (needslayout)
  {
    CleanReadyMessages ();
    CheckNewMessages ();
    RecalculateCurrentSize ();
  }

  // Control the box fading level.
  if (maxfade < fadetimeleft)
  {
    fadetimeleft -= elapsed;
    if (fadetimeleft < .000001f)
    {
      fadetimeleft = 0.0f;
    }
  }
  else if (maxfade >= 0.00001f)
    fadetimeleft = boxfadetime;
}

bool MessageSlot::Needs3DRender ()
{
  return (activeMessages.GetSize () > 0 || queuedMessages.GetSize () > 0)
    && (boxPen || borderPen);
}

void MessageSlot::Render3D (iGraphics3D* g3d, iGraphics2D* g2d)
{
  if (activeMessages.GetSize () == 0) return;

  float curfade = 1.0f;
  if (boxfadetime > 0.001f) curfade = fadetimeleft / boxfadetime;

  int cx = finalPosition.x-marginx, cy = finalPosition.y-marginy;
  if (boxPen)
  {
    if (curfade > 0.99999f)
      boxPen->SetColor (boxColor);
    else
      boxPen->SetColor (csColor4 (
	    boxColor.red, boxColor.green, boxColor.blue, boxColor.alpha * curfade));
    if (roundness > 0)
      boxPen->DrawRoundedRect (cx, cy, cx+cursizex+marginx*2, cy+cursizey+marginy*2, roundness);
    else
      boxPen->DrawRect (cx, cy, cx+cursizex+marginx*2, cy+cursizey+marginy*2);
  }
  if (borderPen)
  {
    if (curfade > 0.99999f)
      borderPen->SetColor (borderColor);
    else
      borderPen->SetColor (csColor4 (
	    borderColor.red, borderColor.green, borderColor.blue, borderColor.alpha * curfade));
    if (roundness > 0)
      borderPen->DrawRoundedRect (cx, cy, cx+cursizex+marginx*2, cy+cursizey+marginy*2, roundness);
    else
      borderPen->DrawRect (cx, cy, cx+cursizex+marginx*2, cy+cursizey+marginy*2);
  }
}

void MessageSlot::Render2D (iGraphics3D* g3d, iGraphics2D* g2d)
{
  int cx = finalPosition.x, cy = finalPosition.y;
  for (size_t m = 0 ; m < activeMessages.GetSize () ; m++)
  {
    TimedMessage* tm = activeMessages.Get (m);
    float maxfadetime = tm->GetFadeTime ();
    for (size_t i = 0 ; i < tm->GetLayoutedLinesCount () ; i++)
    {
      const LayoutedLine& ll = tm->GetLayoutedLine (i);
      if (!ll.render) continue;
      if (ll.timeleft < maxfadetime)
      {
	float alpha = 1.0f;
	alpha = (1.0f - (maxfadetime - ll.timeleft) / maxfadetime);
	int r, g, b, a;
	g2d->GetRGB (ll.color, r, g, b, a);
	int tc = g2d->FindRGB (r, g, b, int (a * alpha));
	g2d->Write (ll.font, cx, cy, tc, -1, ll.line);
      }
      else
      {
	g2d->Write (ll.font, cx, cy, ll.color, -1, ll.line);
      }
      cy += ll.h + LINE_MARGIN;
    }
  }
}

//---------------------------------------------------------------------------

csStringID celPcMessenger::id_message = csInvalidStringID;
csStringID celPcMessenger::id_name = csInvalidStringID;
csStringID celPcMessenger::id_type = csInvalidStringID;
csStringID celPcMessenger::id_slot = csInvalidStringID;
csStringID celPcMessenger::id_id = csInvalidStringID;
csStringID celPcMessenger::id_position = csInvalidStringID;
csStringID celPcMessenger::id_boxanchor = csInvalidStringID;
csStringID celPcMessenger::id_screenanchor = csInvalidStringID;
csStringID celPcMessenger::id_msg1 = csInvalidStringID;
csStringID celPcMessenger::id_msg2 = csInvalidStringID;
csStringID celPcMessenger::id_msg3 = csInvalidStringID;
csStringID celPcMessenger::id_msg4 = csInvalidStringID;
csStringID celPcMessenger::id_msg5 = csInvalidStringID;
csStringID celPcMessenger::id_msg6 = csInvalidStringID;
csStringID celPcMessenger::id_msg7 = csInvalidStringID;
csStringID celPcMessenger::id_textcolor = csInvalidStringID;
csStringID celPcMessenger::id_boxcolor = csInvalidStringID;
csStringID celPcMessenger::id_bordercolor = csInvalidStringID;
csStringID celPcMessenger::id_border = csInvalidStringID;
csStringID celPcMessenger::id_font = csInvalidStringID;
csStringID celPcMessenger::id_fontsize = csInvalidStringID;
csStringID celPcMessenger::id_timeout = csInvalidStringID;
csStringID celPcMessenger::id_fadetime = csInvalidStringID;
csStringID celPcMessenger::id_click = csInvalidStringID;
csStringID celPcMessenger::id_log = csInvalidStringID;
csStringID celPcMessenger::id_cyclefirst = csInvalidStringID;
csStringID celPcMessenger::id_cyclenext = csInvalidStringID;
csStringID celPcMessenger::id_sizex = csInvalidStringID;
csStringID celPcMessenger::id_sizey = csInvalidStringID;
csStringID celPcMessenger::id_maxsizex = csInvalidStringID;
csStringID celPcMessenger::id_maxsizey = csInvalidStringID;
csStringID celPcMessenger::id_marginx = csInvalidStringID;
csStringID celPcMessenger::id_marginy = csInvalidStringID;
csStringID celPcMessenger::id_maxmessages = csInvalidStringID;
csStringID celPcMessenger::id_queue = csInvalidStringID;
csStringID celPcMessenger::id_boxfadetime = csInvalidStringID;
csStringID celPcMessenger::id_borderwidth = csInvalidStringID;
csStringID celPcMessenger::id_roundness = csInvalidStringID;


PropertyHolder celPcMessenger::propinfo;

celPcMessenger::celPcMessenger (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  if (id_message == csInvalidStringID)
  {
    id_message = pl->FetchStringID ("message");
    id_name = pl->FetchStringID ("name");
    id_type = pl->FetchStringID ("type");
    id_slot = pl->FetchStringID ("slot");
    id_id = pl->FetchStringID ("id");
    id_position = pl->FetchStringID ("position");
    id_boxanchor = pl->FetchStringID ("boxanchor");
    id_screenanchor = pl->FetchStringID ("screenanchor");
    id_msg1 = pl->FetchStringID ("msg1");
    id_msg2 = pl->FetchStringID ("msg2");
    id_msg3 = pl->FetchStringID ("msg3");
    id_msg4 = pl->FetchStringID ("msg4");
    id_msg5 = pl->FetchStringID ("msg5");
    id_msg6 = pl->FetchStringID ("msg6");
    id_msg7 = pl->FetchStringID ("msg7");
    id_textcolor = pl->FetchStringID ("textcolor");
    id_boxcolor = pl->FetchStringID ("boxcolor");
    id_bordercolor = pl->FetchStringID ("bordercolor");
    id_border = pl->FetchStringID ("border");
    id_font = pl->FetchStringID ("font");
    id_fontsize = pl->FetchStringID ("fontsize");
    id_timeout = pl->FetchStringID ("timeout");
    id_fadetime = pl->FetchStringID ("fadetime");
    id_click = pl->FetchStringID ("click");
    id_log = pl->FetchStringID ("log");
    id_cyclefirst = pl->FetchStringID ("cyclefirst");
    id_cyclenext = pl->FetchStringID ("cyclenext");
    id_sizex = pl->FetchStringID ("sizex");
    id_sizey = pl->FetchStringID ("sizey");
    id_maxsizex = pl->FetchStringID ("maxsizex");
    id_maxsizey = pl->FetchStringID ("maxsizey");
    id_marginx = pl->FetchStringID ("marginx");
    id_marginy = pl->FetchStringID ("marginy");
    id_maxmessages = pl->FetchStringID ("maxmessages");
    id_queue = pl->FetchStringID ("queue");
    id_boxfadetime = pl->FetchStringID ("boxfadetime");
    id_borderwidth = pl->FetchStringID ("borderwidth");
    id_roundness = pl->FetchStringID ("roundness");
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.messenger.action.");
    AddAction (action_defineslot, "DefineSlot");
    AddAction (action_definetype, "DefineType");
    AddAction (action_message, "Message");
    AddAction (action_setdefaulttype, "SetDefaultType");
    AddAction (action_clearid, "ClearID");
  }

  vc = csQueryRegistry<iVirtualClock> (object_reg);
  g3d = csQueryRegistry<iGraphics3D> (object_reg);
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_POST);
}

celPcMessenger::~celPcMessenger ()
{
}

static MessageLocationAnchor StringToAnchor (const char* s)
{
  csString str = s;
  str.Downcase ();
  if (str == "" || str == "c" || str == "center") return ANCHOR_CENTER;
  else if (str == "n" || str == "north") return ANCHOR_NORTH;
  else if (str == "nw" || str == "northwest") return ANCHOR_NORTHWEST;
  else if (str == "w" || str == "west") return ANCHOR_WEST;
  else if (str == "sw" || str == "southwest") return ANCHOR_SOUTHWEST;
  else if (str == "s" || str == "south") return ANCHOR_SOUTH;
  else if (str == "se" || str == "southeast") return ANCHOR_SOUTHEAST;
  else if (str == "e" || str == "east") return ANCHOR_EAST;
  else if (str == "ne" || str == "northeast") return ANCHOR_NORTHEAST;
  else return ANCHOR_INVALID;
}

static CycleType StringToCycleType (const char* s)
{
  csString str = s;
  str.Downcase ();
  if (str == "random" || str == "rand" || str == "rnd") return CYCLE_RANDOM;
  else if (str == "sequence" || str == "seq") return CYCLE_SEQUENCE;
  else if (str == "none" || str == "") return CYCLE_NONE;
  else if (str == "undefined") return CYCLE_UNDEFINED;
  int idx;
  csScanStr (s, "%d", &idx);
  if (idx >= 0 && idx <= 1000) return CycleType (int (CYCLE_INDEX) + idx);
  return CYCLE_UNDEFINED;
}


bool celPcMessenger::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_defineslot:
      {
	csString name, boxAnchor, screenAnchor;
	csVector2 position;
	if (!Fetch (name, params, id_name)) return false;

	DefineSlot (name);

	if (!Fetch (position, params, id_position)) return false;
	if (!Fetch (boxAnchor, params, id_boxanchor, true, "c")) return false;
	if (!Fetch (screenAnchor, params, id_screenanchor, true, "c")) return false;
	MessageLocationAnchor boxAnchorV = StringToAnchor (boxAnchor);
	if (boxAnchorV == ANCHOR_INVALID) return false;
	MessageLocationAnchor screenAnchorV = StringToAnchor (screenAnchor);
	if (screenAnchorV == ANCHOR_INVALID) return false;
	SetSlotPosition (name, position, boxAnchorV, screenAnchorV);

	csString maxsizex, maxsizey;
	if (!Fetch (maxsizex, params, id_maxsizex, true, "%90")) return false;
	if (!Fetch (maxsizey, params, id_maxsizey, true, "%90")) return false;
	long sizex, sizey, marginx, marginy;
	if (!Fetch (sizex, params, id_sizex, true, -1)) return false;
	if (!Fetch (sizey, params, id_sizey, true, -1)) return false;
	if (!Fetch (marginx, params, id_marginx, true, 5)) return false;
	if (!Fetch (marginy, params, id_marginy, true, 3)) return false;
	SetSlotDimensions (name, sizex, sizey, maxsizex, maxsizey, marginx, marginy);

	csColor4 boxColor, borderColor;
	if (!Fetch (boxColor, params, id_boxcolor, true)) return false;
	if (!Fetch (borderColor, params, id_bordercolor, true)) return false;
	float borderWidth, boxfadetime;
	if (!Fetch (borderWidth, params, id_borderwidth, true, 0.0f)) return false;
	if (!Fetch (boxfadetime, params, id_boxfadetime, true, 0.0f)) return false;
	long roundness;
	if (!Fetch (roundness, params, id_roundness, true, 0)) return false;
	SetSlotBoxAttributes (name, boxColor, borderColor, borderWidth, roundness, boxfadetime);

	long maxmessages;
	if (!Fetch (maxmessages, params, id_maxmessages, true, 1000000000)) return false;
	bool queue;
	if (!Fetch (queue, params, id_queue, true, true)) return false;
	SetSlotMessageHandling (name, maxmessages, queue);

        return true;
      }
    case action_definetype:
      {
	csString type, slot, font;
	long fontSize;

	if (!Fetch (type, params, id_type)) return false;
	if (!Fetch (slot, params, id_slot, true, "none")) return false;
	if (!Fetch (font, params, id_font, true, "")) return false;
	if (!Fetch (fontSize, params, id_fontsize, true, 10)) return false;

	csColor4 textColor;
	if (!Fetch (textColor, params, id_textcolor, true, csColor4 (1, 1, 1, 1))) return false;

	float timeout, fadetime;
	if (!Fetch (timeout, params, id_timeout, true, 2.0f)) return false;
	if (!Fetch (fadetime, params, id_fadetime, true, 1.0f)) return false;

	bool click, log;
	if (!Fetch (click, params, id_click, true, false)) return false;
	if (!Fetch (log, params, id_log, true, false)) return false;

	csString cf, cn;
	CycleType cyclefirst, cyclenext;
	if (!Fetch (cf, params, id_cyclefirst, true, "random")) return false;
	if (!Fetch (cn, params, id_cyclenext, true, "undefined")) return false;
	cyclefirst = StringToCycleType (cf);
	cyclenext = StringToCycleType (cn);

	DefineType (type, slot, textColor, font, fontSize,
	    timeout, fadetime, click, log,
	    cyclefirst, cyclenext);
        return true;
      }
    case action_message:
      {
	csString type, id;
	if (!Fetch (type, params, id_type, true, defaultType)) return false;
	if (!Fetch (id, params, id_id, true, "")) return false;
	int last = -1;
	csString msg1, msg2, msg3, msg4, msg5, msg6, msg7;
	if (!Fetch (msg1, params, id_msg1, true, "")) return false;
	if (!msg1.IsEmpty ()) last = 0;
	if (!Fetch (msg2, params, id_msg2, true, "")) return false;
	if (!msg2.IsEmpty ()) last = 1;
	if (!Fetch (msg3, params, id_msg3, true, "")) return false;
	if (!msg3.IsEmpty ()) last = 2;
	if (!Fetch (msg4, params, id_msg4, true, "")) return false;
	if (!msg4.IsEmpty ()) last = 3;
	if (!Fetch (msg5, params, id_msg5, true, "")) return false;
	if (!msg5.IsEmpty ()) last = 4;
	if (!Fetch (msg6, params, id_msg6, true, "")) return false;
	if (!msg6.IsEmpty ()) last = 5;
	if (!Fetch (msg7, params, id_msg7, true, "")) return false;
	if (!msg7.IsEmpty ()) last = 6;
	csStringArray msgs;
	if (last >= 0) msgs.Push (msg1);
	if (last >= 1) msgs.Push (msg2);
	if (last >= 2) msgs.Push (msg3);
	if (last >= 3) msgs.Push (msg4);
	if (last >= 4) msgs.Push (msg5);
	if (last >= 5) msgs.Push (msg6);
	if (last >= 6) msgs.Push (msg7);
	Message (type, id, msgs);
        return true;
      }
    case action_setdefaulttype:
      {
	csString type;
	if (!Fetch (type, params, id_type)) return false;
	SetDefaultType (type);
	return true;
      }
    case action_clearid:
      {
	csString id;
	if (!Fetch (id, params, id_id)) return false;
	ClearId (id);
	return true;
      }
    default:
      return false;
  }
  return false;
}

MessageType* celPcMessenger::GetType (const char* type) const
{
  csString st = type;
  for (size_t i = 0 ; i < types.GetSize () ; i++)
  {
    if (st == types[i]->GetType ())
      return types[i];
  }
  return 0;
}

MessageSlot* celPcMessenger::GetSlot (const char* name) const
{
  csString sn = name;
  for (size_t i = 0 ; i < slots.GetSize () ; i++)
    if (sn == slots[i]->GetName ())
      return slots[i];
  return 0;
}

void celPcMessenger::DefineSlot (const char* name)
{
  MessageSlot* slot = new MessageSlot (g3d, name);
  slots.Push (slot);
}

void celPcMessenger::SetSlotPosition (const char* name, const csVector2& position,
      MessageLocationAnchor boxAnchor,
      MessageLocationAnchor screenAnchor)
{
  MessageSlot* slot = GetSlot (name);
  if (!slot) { printf ("Can't find slot '%s'!\n", name); return; }
  slot->SetSlotPosition (position, boxAnchor, screenAnchor);
}

void celPcMessenger::SetSlotDimensions (const char* name,
      int sizex, int sizey, const char* maxsizex, const char* maxsizey,
      int marginx, int marginy)
{
  MessageSlot* slot = GetSlot (name);
  if (!slot) { printf ("Can't find slot '%s'!\n", name); return; }
  slot->SetSlotDimensions (sizex, sizey, maxsizex, maxsizey, marginx, marginy);
}

void celPcMessenger::SetSlotBoxAttributes (const char* name,
      const csColor& boxColor, const csColor4& borderColor,
      float borderWidth, int roundness, float boxfadetime)
{
  MessageSlot* slot = GetSlot (name);
  if (!slot) { printf ("Can't find slot '%s'!\n", name); return; }
  slot->SetSlotBoxAttributes (boxColor, borderColor, borderWidth, roundness, boxfadetime);
}

void celPcMessenger::SetSlotMessageHandling (const char* name, int maxmessages, bool queue)
{
  MessageSlot* slot = GetSlot (name);
  if (!slot) { printf ("Can't find slot '%s'!\n", name); return; }
  slot->SetSlotMessageHandling (maxmessages, queue);
}

void celPcMessenger::DefineType (const char* type, const char* slotName,
      const csColor4& textColor, const char* fontName, int fontSize,
      float timeout, float fadetime, bool click, bool log,
      CycleType cyclefirst, CycleType cyclenext)
{
  iGraphics2D* g2d = g3d->GetDriver2D ();
  iFontServer* fontServ = g2d->GetFontServer ();

  csRef<iFont> font;
  if (fontName && *fontName)
  {
    font = fontServ->LoadFont (fontName, fontSize);
  }
  else
  {
    font = fontServ->LoadFont (CSFONT_COURIER);
  }
  int tc = g2d->FindRGB (int (textColor.red * 255.4),
      int (textColor.green * 255.4),
      int (textColor.blue * 255.4),
      int (textColor.alpha * 255.4));

  MessageSlot* slot = GetSlot (slotName);
  MessageType* mt = new MessageType (type, slot, tc,
      font, timeout, fadetime, click, log,
      cyclefirst, cyclenext);
  types.Push (mt);
}

void celPcMessenger::Message (const char* type, const char* id,
      const csStringArray& msgs)
{
  MessageType* mt = GetType (type);
  if (!mt)
  {
    Error ("Can't find message type '%s'!\n", type);
    return;
  }

  MessageLog& log = mt->GetLog ();
  CycleType cycleType;
  if (!id || !*id) cycleType = mt->GetCycleFirst ();
  else if (log.IDExists (id)) cycleType = mt->GetCycleNext ();
  else cycleType = mt->GetCycleFirst ();

  const char* msg = 0;
  switch (cycleType)
  {
    case CYCLE_RANDOM: msg = msgs[rng.Get (msgs.GetSize ())]; break;
    case CYCLE_SEQUENCE:
      {
	size_t counter = 0;
        if (id && *id) counter = log.GetMessageCount (id);
	msg = msgs[counter % msgs.GetSize ()];
      }
      break;
    case CYCLE_NONE:
      msg = 0;
      break;
    default:
      if (int (cycleType) >= int (CYCLE_INDEX))
      {
	int idx = int (cycleType) - int (CYCLE_INDEX);
	if (size_t (idx) >= msgs.GetSize ())
	{
	  printf ("Warning! Not enough messages!\n");
	  idx = msgs.GetSize ()-1;
	}
	msg = msgs[idx];
      }
      break;
  }

  if (id && *id)
  {
    if (mt->GetDoLog ())
      log.PushMessage (id, msg);
    else
      log.PushMessage (id, "X");	// Also log in case we have an id.
  }
  if (msg && mt->GetSlot ())
  {
    mt->GetSlot ()->Message (msg, mt->GetTimeout(), mt->GetFadetime (),
	mt->GetFont (), mt->GetTextColor ());
  }
}

void celPcMessenger::Message (const char* type, const char* id,
      const char* msg, ...)
{
  csStringArray msgs;
  msgs.Push (msg);
  va_list args;
  va_start (args, msg);
  char const* m = va_arg (args, char*);
  while (m != (const char*)0)
  {
    msgs.Push (m);
    m = va_arg (args, char*);
  }
  va_end (args);

  Message (type, id, msgs);
}

void celPcMessenger::ClearId (const char* id)
{
  // @@@ Not efficient!
  for (size_t i = 0 ; i < types.GetSize () ; i++)
  {
    MessageLog& log = types.Get (i)->GetLog ();
    if (log.IDExists (id))
    {
      log.ClearMessage (id);
      return;
    }
  }
}

void celPcMessenger::ClearType (const char* type)
{
  MessageType* mt = GetType (type);
  if (mt) mt->GetLog ().ClearLog ();
}

void celPcMessenger::ClearAll ()
{
  for (size_t i = 0 ; i < types.GetSize () ; i++)
    types.Get (i)->GetLog ().ClearLog ();
}

csPtr<iMessageLogIterator> celPcMessenger::GetMessages (const char* type)
{
  MessageType* mt = GetType (type);
  if (!mt) return 0;
  return new MessageLogIterator (mt->GetLog ().GetIterator ());
}

MessageLog* celPcMessenger::GetMessageLog (const char* type)
{
  MessageType* mt = GetType (type);
  if (!mt) return 0;
  else return &(mt->GetLog ());
}

void celPcMessenger::TickEveryFrame ()
{
  float elapsed = vc->GetElapsedSeconds ();

  bool needs3d = false;
  for (size_t i = 0 ; i < slots.GetSize () ; i++)
  {
    slots[i]->HandleElapsed (elapsed);
    if (slots[i]->Needs3DRender ())
      needs3d = true;
  }

  if (needs3d)
  {
    g3d->BeginDraw (CSDRAW_3DGRAPHICS);
    for (size_t i = 0 ; i < slots.GetSize () ; i++)
      slots[i]->Render3D (g3d, g3d->GetDriver2D ());
  }

  g3d->BeginDraw (CSDRAW_2DGRAPHICS);
  for (size_t i = 0 ; i < slots.GetSize () ; i++)
    slots[i]->Render2D (g3d, g3d->GetDriver2D ());
}

//---------------------------------------------------------------------------

