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
#include "iutil/objreg.h"
#include "plugins/propclass/messenger/messengerfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"

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

csStringID celPcMessenger::id_message = csInvalidStringID;
csStringID celPcMessenger::id_name = csInvalidStringID;
csStringID celPcMessenger::id_type = csInvalidStringID;
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
  }
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
	CEL_FETCH_LONG_PAR (boxfadetime,params,id_boxfadetime);
	if (!p_boxfadetime) boxfadetime = 0;

	DefineSlot (name, position, anchor, sizex, sizey, maxsizex, maxsizey,
	    marginx, marginy, boxColor, borderColor,
	    borderWidth, maxmessages, queue, boxfadetime);
        return true;
      }
    default:
      return false;
  }
  return false;
}

MessageType* celPcMessenger::GetType (const char* type) const
{
  for (size_t i = 0 ; i < types.GetSize () ; i++)
    if (types[i]->GetType () == type)
      return types[i];
  return 0;
}

MessageSlot* celPcMessenger::GetSlot (const char* name) const
{
  for (size_t i = 0 ; i < slots.GetSize () ; i++)
    if (slots[i]->GetName () == name)
      return slots[i];
  return 0;
}

void celPcMessenger::DefineSlot (const char* name,
      const csVector2& position, MessageLocationAnchor positionAnchor,
      int sizex, int sizey, int maxsizex, int maxsizey,
      int marginx, int marginy,
      const csColor4& boxColor, const csColor4& borderColor,
      int borderWidth, int maxmessages, 
      bool queue, csTicks boxfadetime)
{
  MessageSlot* slot = new MessageSlot (name, position, positionAnchor,
      sizex, sizey, maxsizex, maxsizey, marginx, marginy, boxColor,
      borderColor, borderWidth, maxmessages, queue, boxfadetime);
  slots.Push (slot);
}

void celPcMessenger::DefineType (const char* type, const char* slotName,
      const csColor4& textColor, const char* font, int fontSize,
      csTicks timeout, csTicks fadetime, bool click, bool log,
      CycleType cyclefirst, CycleType cyclenext)
{
  MessageSlot* slot = GetSlot (slotName);
  MessageType* mt = new MessageType (type, slot, textColor,
      font, fontSize, timeout, fadetime, click, log,
      cyclefirst, cyclenext);
  types.Push (mt);
}

void celPcMessenger::Message (const char* type, const char* id,
      const char* msg, ...)
{
  MessageType* mt = GetType (type);
  if (!mt) return;
  if (mt->GetDoLog ())
  {
    // @@@ Keep message.
  }
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

//---------------------------------------------------------------------------

