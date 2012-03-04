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
csStringID celPcMessenger::id_type = csInvalidStringID;
csStringID celPcMessenger::id_id = csInvalidStringID;
csStringID celPcMessenger::id_msg1 = csInvalidStringID;
csStringID celPcMessenger::id_msg2 = csInvalidStringID;
csStringID celPcMessenger::id_msg3 = csInvalidStringID;
csStringID celPcMessenger::id_msg4 = csInvalidStringID;
csStringID celPcMessenger::id_msg5 = csInvalidStringID;
csStringID celPcMessenger::id_msg6 = csInvalidStringID;
csStringID celPcMessenger::id_msg7 = csInvalidStringID;
csStringID celPcMessenger::id_location = csInvalidStringID;
csStringID celPcMessenger::id_textcolor = csInvalidStringID;
csStringID celPcMessenger::id_bgcolor = csInvalidStringID;
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


PropertyHolder celPcMessenger::propinfo;

celPcMessenger::celPcMessenger (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  if (id_message == csInvalidStringID)
  {
    id_message = pl->FetchStringID ("message");
    id_type = pl->FetchStringID ("type");
    id_id = pl->FetchStringID ("id");
    id_msg1 = pl->FetchStringID ("msg1");
    id_msg2 = pl->FetchStringID ("msg2");
    id_msg3 = pl->FetchStringID ("msg3");
    id_msg4 = pl->FetchStringID ("msg4");
    id_msg5 = pl->FetchStringID ("msg5");
    id_msg6 = pl->FetchStringID ("msg6");
    id_msg7 = pl->FetchStringID ("msg7");
    id_location = pl->FetchStringID ("location");
    id_textcolor = pl->FetchStringID ("textcolor");
    id_bgcolor = pl->FetchStringID ("bgcolor");
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
  }

  propholder = &propinfo;

  // For actions.
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.messenger.action.");
    AddAction (action_print, "Print");
  }
}

celPcMessenger::~celPcMessenger ()
{
}

bool celPcMessenger::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_print:
      {
        //CEL_FETCH_STRING_PAR (msg,params,id_message);
        //if (!p_msg) return false;
        //Print (msg);
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

void celPcMessenger::Define (const char* type, MessageLocation location,
      const csColor4& textColor, const csColor4& bgColor, const csColor4& borderColor,
      int border, const char* font, int fontSize,
      csTicks timeout, csTicks fadetime, bool click, bool log,
      CycleType cyclefirst, CycleType cyclenext)
{
  MessageType* mt = new MessageType (type, location, textColor, bgColor,
      borderColor, border, font, fontSize, timeout, fadetime, click, log,
      cyclefirst, cyclenext);
  types.Push (mt);
}

void celPcMessenger::Message (const char* type, const char* id,
      const char* msg, ...)
{
  MessageType* mt = GetType (type);
  if (!mt) return;
  if (mt.GetDoLog ())
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

