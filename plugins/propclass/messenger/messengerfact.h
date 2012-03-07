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

#ifndef __CEL_PF_MESSENGERFACT__
#define __CEL_PF_MESSENGERFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "csutil/parray.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/messenger.h"

struct iCelEntity;
struct iObjectRegistry;
struct iFont;

//---------------------------------------------------------------------------

class MessageLog
{
private:
  csStringArray ids;
  csStringArray messages;

public:
  size_t GetCount () { return ids.GetSize (); }
  const char* GetID (size_t idx) const { return ids.Get (idx); }
  const char* GetMessage (size_t idx) const { return messages.Get (idx); }
  void PushMessage (const char* id, const char* msg)
  {
    ids.Push (id);
    messages.Push (msg);
  }
  void ClearLog ()
  {
    ids.DeleteAll ();
    messages.DeleteAll ();
  }
  size_t GetMessageIndex (const char* id)
  {
    // @@@ Not efficient!
    for (size_t i = 0 ; i < ids.GetSize () ; i++)
      if (ids[i] == id)
	return i;
    return csArrayItemNotFound;
  }
  void ClearMessage (size_t idx)
  {
    ids.DeleteIndex (idx);
    messages.DeleteIndex (idx);
  }
};

//---------------------------------------------------------------------------

struct TimedMessage
{
  csString message;
  float timeleft;
  float fadetime;
  iFont* font;
  int color;

  // If true this message fitted completely on screen.
  // Only in that case will we start the countdown (timeleft).
  bool complete;

  TimedMessage (const char* msg, float timeleft, float fadetime, iFont* font,
      int color)
    : message (msg), timeleft (timeleft), fadetime (fadetime),
      font (font), color (color), complete (false) { }
};

class MessageSlot
{
private:
  csString name;
  csVector2 position;
  MessageLocationAnchor positionAnchor;
  csColor4 boxColor;
  csColor4 borderColor;
  int borderWidth;
  int sizex, sizey;
  int maxsizex, maxsizey;
  int marginx, marginy;
  int maxmessages;
  bool queue;
  float boxfadetime;

  csArray<TimedMessage> activeMessages;
  csArray<TimedMessage> queuedMessages;

  int cursizex, cursizey;
  csArray<TimedMessage> layoutedLines;	// Layouted version.
  void LayoutText ();

public:
  MessageSlot (const char* name,
      const csVector2& position, MessageLocationAnchor positionAnchor,
      int sizex, int sizey, int maxsizex, int maxsizey,
      int marginx, int marginy,
      const csColor4& boxColor, const csColor4& borderColor,
      int borderWidth, int maxmessages, 
      bool queue, float boxfadetime) :
    name (name), position (position), positionAnchor (positionAnchor),
    boxColor (boxColor), borderColor (borderColor),
    borderWidth (borderWidth),
    sizex (sizex), sizey (sizey), maxsizex (maxsizex), maxsizey (maxsizey),
    marginx (marginx), marginy (marginy),
    maxmessages (maxmessages),
    queue (queue), boxfadetime (boxfadetime) { }

  const char* GetName () const { return name; }

  void Message (const char* msg, float timeout, float fadetime, iFont* font,
      int color);
};

//---------------------------------------------------------------------------

class MessageType
{
private:
  csString type;
  MessageSlot* slot;
  int textColor;
  csRef<iFont> font;
  float timeout, fadetime;
  bool click, dolog;
  CycleType cyclefirst, cyclenext;

  MessageLog log;

public:
  MessageType (const char* type, MessageSlot* slot,
      int textColor, iFont* font,
      float timeout, float fadetime, bool click, bool dolog,
      CycleType cyclefirst, CycleType cyclenext) :
    type (type), slot (slot),
    textColor (textColor), font (font),
    timeout (timeout), fadetime (fadetime), click (click), dolog (dolog),
    cyclefirst (cyclefirst), cyclenext (cyclenext) { }

  const char* GetType () const { return type; }
  MessageSlot* GetSlot () const { return slot; }
  const int GetTextColor () const { return textColor; }
  iFont* GetFont () const { return font; }
  float GetTimeout () const { return timeout; }
  float GetFadetime () const { return fadetime; }
  bool GetClick () const { return click; }
  bool GetDoLog () const { return dolog; }
  CycleType GetCycleFirst () const { return cyclefirst; }
  CycleType GetCycleNext () const { return cyclenext; }

  MessageLog& GetLog () { return log; }
};

//---------------------------------------------------------------------------

/**
 * Factory for messenger.
 */
CEL_DECLARE_FACTORY (Messenger)

/**
 * This is a messenger property class.
 */
class celPcMessenger : public scfImplementationExt1<
	celPcMessenger, celPcCommon, iPcMessenger>
{
private:
  static csStringID id_message;
  static csStringID id_name;
  static csStringID id_type;
  static csStringID id_slot;
  static csStringID id_id;
  static csStringID id_position;
  static csStringID id_positionanchor;
  static csStringID id_msg1;
  static csStringID id_msg2;
  static csStringID id_msg3;
  static csStringID id_msg4;
  static csStringID id_msg5;
  static csStringID id_msg6;
  static csStringID id_msg7;
  static csStringID id_textcolor;
  static csStringID id_boxcolor;
  static csStringID id_bordercolor;
  static csStringID id_border;
  static csStringID id_font;
  static csStringID id_fontsize;
  static csStringID id_timeout;
  static csStringID id_fadetime;
  static csStringID id_click;
  static csStringID id_log;
  static csStringID id_cyclefirst;
  static csStringID id_cyclenext;
  static csStringID id_sizex;
  static csStringID id_sizey;
  static csStringID id_maxsizex;
  static csStringID id_maxsizey;
  static csStringID id_marginx;
  static csStringID id_marginy;
  static csStringID id_queue;
  static csStringID id_maxmessages;
  static csStringID id_boxfadetime;
  static csStringID id_borderwidth;

  // For actions.
  enum actionids
  {
    action_defineslot = 0,
    action_definetype,
    action_message,
    action_setdefaulttype,
    action_clearid
  };
  static PropertyHolder propinfo;

  // Types.
  csString defaultType;
  csPDelArray<MessageType> types;
  MessageType* GetType (const char* type) const;

  // Slots.
  csPDelArray<MessageSlot> slots;
  MessageSlot* GetSlot (const char* name) const;

public:
  celPcMessenger (iObjectRegistry* object_reg);
  virtual ~celPcMessenger ();

  MessageLog* GetMessageLog (const char* type);

  virtual void DefineSlot (const char* name,
      const csVector2& position, MessageLocationAnchor positionAnchor,
      int sizex, int sizey, int maxsizex, int maxsizey,
      int marginx, int marginy,
      const csColor4& boxColor, const csColor4& borderColor,
      int borderWidth, int maxmessages, 
      bool queue, float boxfadetime);
  virtual void DefineType (const char* type, const char* slot,
      const csColor4& textColor, const char* font, int fontSize,
      float timeout, float fadetime, bool click, bool log,
      CycleType cyclefirst, CycleType cyclenext);
  virtual void SetDefaultType (const char* type) { defaultType = type; }
  virtual const char* GetDefaultType () const { return defaultType; }
  virtual void Message (const char* type, const char* id,
      const csStringArray& msgs);
  virtual void Message (const char* type, const char* id,
      const char* msg, ...);
  virtual void ClearId (const char* id);
  virtual void ClearType (const char* type);
  virtual void ClearAll ();
  virtual csPtr<iMessageLogIterator> GetMessages (const char* type);

  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);
};

#endif // __CEL_PF_MESSENGERFACT__

