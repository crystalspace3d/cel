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

bool MessageSlot::LayoutWord (const TimedMessage& tm, const char* word)
{
  int w, h;
  tm.font->GetDimensions (word, w, h);

  int neww = curw + curspacew + w;

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

  curw = neww;
  LayoutedLine& line = layoutedLines.Get (layoutedLines.GetSize ()-1);
  line.line += " ";
  line.line += word;

  return true;
}

#define LINE_MARGIN 2

bool MessageSlot::LayoutNewLine (const TimedMessage& tm)
{
  int newh = curh + LINE_MARGIN + curh;
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
  layoutline.y = curh + LINE_MARGIN;
  layoutedLines.Push (layoutline);
  curh = newh;
  curw = 0;
  return true;
}

bool MessageSlot::LayoutLine (const TimedMessage& tm, const char* line)
{
  int w;
  tm.font->GetDimensions (line, w, curh);
  if (LayoutNewLine (tm))
    return false;

  curw = 0;
  csStringArray words (line, " ", csStringArray::delimIgnore);
  for (size_t w = 0 ; w < words.GetSize () ; w++)
  {
    if (!LayoutWord (tm, words.Get (w)))
    {
      // No more room for this line.
      if (!LayoutNewLine (tm))
	return false;
      curw = 0;
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
      tm.completedLines = l;
      return false;
    }
  }
  tm.completedLines = ALL_LINES;
  return true;
}

void MessageSlot::LayoutText ()
{
  if (sizex > 0) cursizex = sizex - marginx * 2; else cursizex = 0;
  if (sizey > 0) cursizey = sizey - marginy * 2; else cursizey = 0;

  for (size_t mi = 0 ; mi < activeMessages.GetSize () ; mi++)
  {
    TimedMessage& tm = activeMessages.Get (mi);
    if (tm.completedLines != ALL_LINES)
    {
      if (!LayoutMessage (tm))
      {
        // The message did not fit. tm.completedLines will be updated
        // with the number of lines that we did manage to display.
	// return;
      }
    }
  }
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
printf ("layoutedLines.GetSize()=%d\n", layoutedLines.GetSize()); fflush (stdout);
  if (layoutedLines.GetSize () == 0) return;
  CheckMessages ();

  LayoutedLine& line = layoutedLines.Get (0);
  line.timeleft -= elapsed;
  if (line.timeleft <= 0)
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

void MessageSlot::Render (iGraphics3D* g3d, iGraphics2D* g2d)
{
  int cx = position.x, cy = position.y;
  switch (positionAnchor)
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
  for (size_t i = 0 ; i < layoutedLines.GetSize () ; i++)
  {
    const LayoutedLine& ll = layoutedLines.Get (i);
    printf ("    %d: pos=%d,%d  line=%s\n", i, cx, cy, ll.line.GetData ());
    g2d->Write (ll.font, cx, cy, ll.color, -1, ll.line);
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
csStringID celPcMessenger::id_positionanchor = csInvalidStringID;
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
    id_positionanchor = pl->FetchStringID ("positionanchor");
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
  if (str == "c" || str == "center") return ANCHOR_CENTER;
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
	// @@@ Error reporting.
        CEL_FETCH_STRING_PAR (name,params,id_name);
        if (!p_name) return false;
        CEL_FETCH_VECTOR2_PAR (position,params,id_position);
        if (!p_position) return false;
        CEL_FETCH_STRING_PAR (positionAnchor,params,id_positionanchor);
	MessageLocationAnchor anchor = StringToAnchor (positionAnchor);
	if (anchor == ANCHOR_INVALID) return false;
	CEL_FETCH_LONG_PAR (sizex,params,id_sizex);
	if (!p_sizex) sizex = -1;
	CEL_FETCH_LONG_PAR (sizey,params,id_sizey);
	if (!p_sizey) sizey = -1;
	CEL_FETCH_LONG_PAR (maxsizex,params,id_maxsizex);
	if (!p_maxsizex) maxsizex = -1;
	CEL_FETCH_LONG_PAR (maxsizey,params,id_maxsizey);
	if (!p_maxsizey) maxsizey = -1;
	CEL_FETCH_LONG_PAR (marginx,params,id_marginx);
	if (!p_marginx) marginx = 5;
	CEL_FETCH_LONG_PAR (marginy,params,id_marginy);
	if (!p_marginy) marginy = 3;
	CEL_FETCH_COLOR4_PAR (boxColor,params,id_boxcolor);
	if (!p_boxColor) boxColor.Set (0, 0, 0, 0);
	CEL_FETCH_COLOR4_PAR (borderColor,params,id_bordercolor);
	if (!p_borderColor) borderColor.Set (0, 0, 0, 0);
	CEL_FETCH_LONG_PAR (borderWidth,params,id_borderwidth);
	if (!p_borderWidth) borderWidth = 0;
	CEL_FETCH_LONG_PAR (maxmessages,params,id_maxmessages);
	if (!p_maxmessages) maxmessages = 1000000000;
	CEL_FETCH_BOOL_PAR (queue,params,id_queue);
	if (!p_queue) queue = true;
	CEL_FETCH_FLOAT_PAR (boxfadetime,params,id_boxfadetime);
	if (!p_boxfadetime) boxfadetime = 0.0f;

	DefineSlot (name, position, anchor, sizex, sizey, maxsizex, maxsizey,
	    marginx, marginy, boxColor, borderColor,
	    borderWidth, maxmessages, queue, boxfadetime);
        return true;
      }
    case action_definetype:
      {
	// @@@ Error reporting.
        CEL_FETCH_STRING_PAR (type,params,id_type);
        if (!p_type) return false;
        CEL_FETCH_STRING_PAR (slot,params,id_slot);
        if (!p_slot) slot = "none";
	CEL_FETCH_COLOR4_PAR (textColor,params,id_textcolor);
	if (!p_textColor) textColor.Set (1, 1, 1, 1);
        CEL_FETCH_STRING_PAR (font,params,id_font);
        CEL_FETCH_LONG_PAR (fontSize,params,id_fontsize);
        CEL_FETCH_FLOAT_PAR (timeout,params,id_timeout);
	if (!p_timeout) timeout = 2.0f;
        CEL_FETCH_FLOAT_PAR (fadetime,params,id_fadetime);
	if (!p_fadetime) timeout = 1.0f;

	CEL_FETCH_BOOL_PAR (click,params,id_click);
	if (!p_click) click = false;
	CEL_FETCH_BOOL_PAR (log,params,id_log);
	if (!p_log) log = false;

	CycleType cyclefirst, cyclenext;
        CEL_FETCH_STRING_PAR (cf,params,id_cyclefirst);
	if (!p_cf) cyclefirst = CYCLE_RANDOM;
	else cyclefirst = StringToCycleType (cf);
        CEL_FETCH_STRING_PAR (cn,params,id_cyclenext);
	if (!p_cn) cyclenext = CYCLE_UNDEFINED;
	else cyclenext = StringToCycleType (cn);

	DefineType (type, slot, textColor, font, fontSize,
	    timeout, fadetime, click, log,
	    cyclefirst, cyclenext);
        return true;
      }
    case action_message:
      {
	// @@@ Error reporting.
        CEL_FETCH_STRING_PAR (type,params,id_type);
        if (!p_type) type = defaultType;
        CEL_FETCH_STRING_PAR (id,params,id_id);
	int last = -1;
        CEL_FETCH_STRING_PAR (msg1,params,id_msg1);
	if (p_msg1) last = 0;
        CEL_FETCH_STRING_PAR (msg2,params,id_msg2);
	if (p_msg2) last = 1;
        CEL_FETCH_STRING_PAR (msg3,params,id_msg3);
	if (p_msg3) last = 2;
        CEL_FETCH_STRING_PAR (msg4,params,id_msg4);
	if (p_msg4) last = 3;
        CEL_FETCH_STRING_PAR (msg5,params,id_msg5);
	if (p_msg5) last = 4;
        CEL_FETCH_STRING_PAR (msg6,params,id_msg6);
	if (p_msg6) last = 5;
        CEL_FETCH_STRING_PAR (msg7,params,id_msg7);
	if (p_msg7) last = 6;
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
        CEL_FETCH_STRING_PAR (type,params,id_type);
	SetDefaultType (type);
	return true;
      }
    case action_clearid:
      {
	// @@@ Error reporting.
        CEL_FETCH_STRING_PAR (id,params,id_id);
	if (!p_id) return false;
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
      const csVector2& position, MessageLocationAnchor positionAnchor,
      int sizex, int sizey, int maxsizex, int maxsizey,
      int marginx, int marginy,
      const csColor4& boxColor, const csColor4& borderColor,
      int borderWidth, int maxmessages, 
      bool queue, float boxfadetime)
{
  MessageSlot* slot = new MessageSlot (name, position, positionAnchor,
      sizex, sizey, maxsizex, maxsizey, marginx, marginy, boxColor,
      borderColor, borderWidth, maxmessages, queue, boxfadetime);
  slots.Push (slot);
}

void celPcMessenger::DefineType (const char* type, const char* slotName,
      const csColor4& textColor, const char* fontName, int fontSize,
      float timeout, float fadetime, bool click, bool log,
      CycleType cyclefirst, CycleType cyclenext)
{
  csRef<iGraphics3D> g3d = csQueryRegistry<iGraphics3D> (object_reg);
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
    printf ("Warning! Can't find message type '%s'!\n", type);
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

  g3d->BeginDraw (CSDRAW_2DGRAPHICS);

  for (size_t i = 0 ; i < slots.GetSize () ; i++)
  {
    slots[i]->HandleElapsed (elapsed);
    slots[i]->Render (g3d, g3d->GetDriver2D ());
  }
}

//---------------------------------------------------------------------------

