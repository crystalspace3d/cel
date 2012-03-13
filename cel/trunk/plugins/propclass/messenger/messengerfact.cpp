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
  MessageType* mt;
  size_t idx;

public:
  MessageLogIterator (MessageType* mt) :
    scfImplementationType (this), mt (mt), idx (0) { }
  virtual ~MessageLogIterator () { }
  virtual bool HasNext () const
  {
    MessageLog& log = mt->GetLog ();
    return idx < log.GetCount ();
  }
  virtual const char* Next ()
  {
    MessageLog& log = mt->GetLog ();
    idx++;
    return log.GetMessage (idx-1);
  }
  virtual const char* GetID ()
  {
    MessageLog& log = mt->GetLog ();
    return log.GetID (idx);
  }
};

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

bool MessageSlot::LayoutWord (const TimedMessage& tm, const char* word)
{
  int w, h;
  tm.font->GetDimensions (word, w, h);

  LayoutedLine& line = layoutedLines.Get (layoutedLines.GetSize ()-1);
  int neww = line.w + w;
  if (line.w > 0)
    neww += curspacew;

  if (neww > cursizex)
  {
    // Not enough room. Can we extend?
    if (sizex <= 0 && neww <= maxsizex)
    {
      // Yes
      cursizex = neww;
    }
    else
      return false;
  }

  if (line.w > 0)
    line.line += " ";
  line.line += word;
  line.w = neww;

  return true;
}

#define LINE_MARGIN 2

bool MessageSlot::LayoutNewLine (const TimedMessage& tm)
{
  int w, h;
  tm.font->GetDimensions ("abcghijklABDGHIJKLM0123456789,!?", w, h);
  int margin = 0;
  if (layoutedLines.GetSize () > 0)
    margin = LINE_MARGIN;
  int newh = curh + margin + h;

  if (newh > cursizey)
  {
    // Not enough room. Can we extend?
    if (sizey <= 0 && newh <= maxsizey)
    {
      // Yes
      cursizey = newh;
    }
    else
      return false;
  }

  float timeleft = 0.0f;
  for (size_t i = 0 ; i < layoutedLines.GetSize () ; i++)
    timeleft += layoutedLines.Get (i).timeleft;
  timeleft = tm.timeleft - timeleft;
  if (timeleft < 0.0f) timeleft = 0.0f;

  LayoutedLine layoutline ("", timeleft, tm.fadetime, tm.font, tm.color);
  layoutline.y = curh + margin;
  layoutline.w = 0;
  layoutline.h = h;
  layoutedLines.Push (layoutline);
  curh = newh;
  return true;
}

bool MessageSlot::LayoutLine (const TimedMessage& tm, const char* line)
{
  if (!LayoutNewLine (tm))
    return false;

  csStringArray words (line, " ", csStringArray::delimIgnore);
  for (size_t w = 0 ; w < words.GetSize () ; w++)
  {
    if (!LayoutWord (tm, words.Get (w)))
    {
      // No more room for this line.
      if (!LayoutNewLine (tm))
	return false;
      if (!LayoutWord (tm, words.Get (w)))
      {
	// Still doesn't fit. The word may simply be too big. We just
	// ignore it for now (needs to be handled better!)
	// @@@
	printf ("Ignored word '%s'!\n", (const char*)words.Get (w));
	fflush (stdout);
      }
    }
  }


  return true;
}

bool MessageSlot::LayoutMessage (TimedMessage& tm)
{
  int h;
  tm.font->GetDimensions (" ", curspacew, h);

  size_t firstline = layoutedLines.GetSize ();
  csStringArray lines (tm.message, "\n");
  for (size_t l = tm.completedLines ; l < lines.GetSize () ; l++)
  {
    if (!LayoutLine (tm, lines.Get (l)))
    {
      // This line didn't (completely) fit. We mark all lines
      // that we managed to layout as incomplete.
      for (size_t i = firstline ; i < layoutedLines.GetSize () ; i++)
	layoutedLines.Get (i).fadetime = 0.0f;
      tm.completedLines = l+1;
      return false;
    }
  }
  tm.completedLines = ALL_LINES;
  return true;
}

void MessageSlot::InitEmpty ()
{
  if (sizex > 0) cursizex = sizex - marginx * 2; else cursizex = 0;
  if (sizey > 0) cursizey = sizey - marginy * 2; else cursizey = 0;
  layoutedLines.Empty ();
  curh = 0;
}

void MessageSlot::LayoutText ()
{
  if (layoutedLines.IsEmpty ())
    InitEmpty ();

  for (size_t mi = 0 ; mi < activeMessages.GetSize () ; mi++)
  {
    TimedMessage& tm = activeMessages.Get (mi);
    if (tm.completedLines != ALL_LINES)
      LayoutMessage (tm);
  }

  int swidth = g3d->GetWidth ();
  int sheight = g3d->GetHeight ();
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
  if (activeMessages.GetSize () >= size_t (maxmessages))
  {
    if (queue)
    {
      queuedMessages.Push (TimedMessage (msg, timeout, fadetime, font,
	    color));
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
printf ("activeMessages %s\n", msg); fflush (stdout);
  activeMessages.Push (TimedMessage (msg, timeout, fadetime, font, color));
  LayoutText ();
}

void MessageSlot::CheckMessages ()
{
  if (queuedMessages.GetSize () > 0)
  {
    while (queuedMessages.GetSize () > 0 && activeMessages.GetSize () < size_t (maxmessages))
    {
      activeMessages.Push (queuedMessages.Get (0));
      queuedMessages.DeleteIndex (0);
    }
  }
  size_t i = 0;
  while (i < activeMessages.GetSize ())
  {
    if (activeMessages.Get (i).completedLines == ALL_LINES)
      activeMessages.DeleteIndex (0);
    else
      i++;
  }
}

void MessageSlot::HandleElapsed (float elapsed)
{
  if (layoutedLines.GetSize () == 0) return;
  CheckMessages ();

  LayoutedLine& line = layoutedLines.Get (0);

  bool done = false;
  if (line.timeleft <= 0)
  {
    line.fadetime -= elapsed;
    if (line.fadetime <= 0) done = true;
  }
  else line.timeleft -= elapsed;

  if (done)
  {
    // The other messages will get removed the next frame.
    layoutedLines.DeleteIndex (0);
    curh = 0;
    for (size_t i = 0 ; i < layoutedLines.GetSize () ; i++)
    {
      layoutedLines.Get (i).y = curh;
      curh += layoutedLines.Get (i).h + LINE_MARGIN;
    }
    if (curh > 0) curh -= LINE_MARGIN;
    LayoutText ();
  }
}

bool MessageSlot::Needs3DRender ()
{
  return boxPen || borderPen;
}

void MessageSlot::Render3D (iGraphics3D* g3d, iGraphics2D* g2d)
{
  int cx = finalPosition.x-marginx, cy = finalPosition.y-marginy;
  if (boxPen)
  {
    if (roundness > 0)
      boxPen->DrawRoundedRect (cx, cy, cx+cursizex+marginx*2, cy+cursizey+marginy*2, roundness);
    else
      boxPen->DrawRect (cx, cy, cx+cursizex+marginx*2, cy+cursizey+marginy*2);
  }
  if (borderPen)
  {
    if (roundness > 0)
      borderPen->DrawRoundedRect (cx, cy, cx+cursizex+marginx*2, cy+cursizey+marginy*2, roundness);
    else
      borderPen->DrawRect (cx, cy, cx+cursizex+marginx*2, cy+cursizey+marginy*2);
  }
}

void MessageSlot::Render2D (iGraphics3D* g3d, iGraphics2D* g2d)
{
  int cx = finalPosition.x, cy = finalPosition.y;
  for (size_t i = 0 ; i < layoutedLines.GetSize () ; i++)
  {
    const LayoutedLine& ll = layoutedLines.Get (i);
    if (ll.timeleft <= 0 && ll.maxfadetime > 0.0f)
    {
      float alpha = 1.0f;
      alpha = (1.0f - (ll.maxfadetime - ll.fadetime) / ll.maxfadetime);
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

	if (!Fetch (position, params, id_position)) return false;
	if (!Fetch (boxAnchor, params, id_boxanchor, true, "c")) return false;
	if (!Fetch (screenAnchor, params, id_screenanchor, true, "c")) return false;
	MessageLocationAnchor boxAnchorV = StringToAnchor (boxAnchor);
	if (boxAnchorV == ANCHOR_INVALID) return false;
	MessageLocationAnchor screenAnchorV = StringToAnchor (screenAnchor);
	if (screenAnchorV == ANCHOR_INVALID) return false;

	long sizex, sizey, maxsizex, maxsizey, marginx, marginy;
	if (!Fetch (sizex, params, id_sizex, true, -1)) return false;
	if (!Fetch (sizey, params, id_sizey, true, -1)) return false;
	if (!Fetch (maxsizex, params, id_maxsizex, true, -1)) return false;
	if (!Fetch (maxsizey, params, id_maxsizey, true, -1)) return false;
	if (!Fetch (marginx, params, id_marginx, true, 5)) return false;
	if (!Fetch (marginy, params, id_marginy, true, 3)) return false;

	csColor4 boxColor, borderColor;
	if (!Fetch (boxColor, params, id_boxcolor, true)) return false;
	if (!Fetch (borderColor, params, id_bordercolor, true)) return false;

	float borderWidth;
	if (!Fetch (borderWidth, params, id_borderwidth, true, 0.0f)) return false;
	long maxmessages, roundness;
	if (!Fetch (maxmessages, params, id_maxmessages, true, 1000000000)) return false;
	if (!Fetch (roundness, params, id_roundness, true, 0)) return false;

	bool queue;
	if (!Fetch (queue, params, id_queue, true, true)) return false;
	float boxfadetime;
	if (!Fetch (boxfadetime, params, id_boxfadetime, true, 0.0f)) return false;

	DefineSlot (name, position, boxAnchorV, screenAnchorV,
	    sizex, sizey, maxsizex, maxsizey,
	    marginx, marginy, boxColor, borderColor,
	    borderWidth, roundness, maxmessages, queue, boxfadetime);
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

void celPcMessenger::DefineSlot (const char* name,
      const csVector2& position,
      MessageLocationAnchor boxAnchor,
      MessageLocationAnchor screenAnchor,
      int sizex, int sizey, int maxsizex, int maxsizey,
      int marginx, int marginy,
      const csColor4& boxColor, const csColor4& borderColor,
      float borderWidth, int roundness, int maxmessages, 
      bool queue, float boxfadetime)
{
  MessageSlot* slot = new MessageSlot (g3d, name, position,
      boxAnchor, screenAnchor,
      sizex, sizey, maxsizex, maxsizey, marginx, marginy, boxColor,
      borderColor, borderWidth, roundness, maxmessages, queue, boxfadetime);
  slot->InitPen ();
  slots.Push (slot);
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
printf ("Message %s/%s\n", type, id); fflush (stdout);
  MessageType* mt = GetType (type);
  if (!mt)
  {
    Error ("Can't find message type '%s'!\n", type);
    return;
  }
  if (mt->GetDoLog ())
  {
    // @@@ Keep message.
  }
  if (mt->GetSlot ())
  {
    // @@@ Select the right message.
    mt->GetSlot ()->Message (msgs[0], mt->GetTimeout(), mt->GetFadetime (),
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
    size_t idx = log.GetMessageIndex (id);
    if (idx != csArrayItemNotFound)
    {
      log.ClearMessage (idx);
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
  return new MessageLogIterator (mt);
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

