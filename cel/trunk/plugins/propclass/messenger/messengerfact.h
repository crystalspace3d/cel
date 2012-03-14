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
#include "iutil/virtclk.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "csutil/scf.h"
#include "csutil/parray.h"
#include "csutil/randomgen.h"
#include "cstool/pen.h"
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

struct MessageCounter
{
  csString message;
  size_t counter;
  MessageCounter () : counter (0) { }
};

typedef csHash<MessageCounter,csString> MessageHash;

class MessageLog
{
private:
  MessageHash idToMessage;

public:
  size_t GetCount () { return idToMessage.GetSize (); }
  MessageHash::GlobalIterator GetIterator ()
  {
    return idToMessage.GetIterator ();
  }
  void PushMessage (const char* id, const char* msg)
  {
    MessageCounter mc;
    if (idToMessage.Contains (id))
    {
      idToMessage.Get (id, mc).counter++;
    }
    else
    {
      mc.message = msg;
      mc.counter = 1;
      idToMessage.Put (id, mc);
    }
  }
  void ClearLog ()
  {
    idToMessage.DeleteAll ();
  }
  bool IDExists (const char* id)
  {
    return idToMessage.Contains (id);
  }
  size_t GetMessageCount (const char* id)
  {
    MessageCounter mc;
    return idToMessage.Get (id, mc).counter;
  }
  void ClearMessage (const char* id)
  {
    idToMessage.DeleteAll (id);
  }
};

//---------------------------------------------------------------------------

struct LayoutedLine
{
  csString line;
  float timeleft;
  iFont* font;
  int color;

  int w, h;	// Width and height of this line.
  bool render;	// True if this line is currently rendered.

  LayoutedLine (const char* line, float timeleft, iFont* font, int color)
    : line (line), timeleft (timeleft),
      font (font), color (color), render (false) { }
};

/**
 * A message.
 */
class TimedMessage
{
private:
  csString message;
  float timeleft;
  float fadetime;
  iFont* font;
  int color;

  int curspacew;			// Current width of a space.
  bool updated;				// Set to true if a line timed out.
  float fading;				// Time left before all lines have faded away.

  csArray<LayoutedLine> layoutedLines;	// Layouted lines.

  bool LayoutWord (int maxwidth, const char* word);
  void LayoutNewLine ();
  void LayoutLine (int maxwidth, const char* line);

public:
  TimedMessage (const char* msg, float timeleft, float fadetime, iFont* font,
      int color)
    : message (msg), timeleft (timeleft), fadetime (fadetime),
      font (font), color (color) { }

  float GetFadeTime () const { return fadetime; }

  /// Calculate the layout of this message given a certain maximum width.
  void Layout (int maxwidth);

  /// Get the amount of layouted lines.
  size_t GetLayoutedLinesCount () const { return layoutedLines.GetSize (); }
  const LayoutedLine& GetLayoutedLine (size_t idx) const
  { return layoutedLines.Get (idx); }

  /**
   * Calculate the height of the active lines (lines that have been shown
   * and are no longer visible will not be counted) that will fit a given height.
   * This will also mark all lines that have to be rendered.
   */
  int CalculateFittingVerticalHeight (int maxheight);

  /**
   * Return the actual maximum width of all lines. This is only valid
   * after calling Layout().
   */
  int GetMaxWidth ();

  /**
   * Return true if this message is completely ready. i.e. all lines have
   * been displayed and faded away.
   */
  bool IsMessageReady ();

  /**
   * Handle elapsed time for this message. If there are still lines left
   * to be displayed then this will eat up the elapsed time and return 0.0f
   * so that further messages don't get processed. If all lines are displayed
   * and are either fading away or are removed then this will return the
   * remaining elapsed time that can be used for further messages.
   */
  float HandleElapsed (float elapsed);

  /**
   * Call this after HandleElapsed. It will return true if one or more lines
   * have finished displaying and so it is time to recalculate the layout.
   */
  bool LinesAreFinished () const { return updated; }

  /**
   * Call this after HandleElapsed. It will return the number of seconds
   * left before all lines have faded away. It will returns a very big number
   * if some lines are still visible (i.e. not fading).
   */
  float GetMaxFadeTimeLeft () const { return fading; }
};

class MessageSlot
{
private:
  iGraphics3D* g3d;
  csString name;

  csVector2 position;		// Position relative to anchor.
  MessageLocationAnchor boxAnchor;
  MessageLocationAnchor screenAnchor;
  csVector2 finalPosition;	// Final position depending on size.

  csPen* boxPen;
  csPen* borderPen;
  csColor4 boxColor;
  csColor4 borderColor;
  float borderWidth;
  int roundness;
  int sizex, sizey;
  int maxsizex, maxsizey;
  int marginx, marginy;

  int maxmessages;
  bool queue;

  float boxfadetime;
  float fadetimeleft;

  csPDelArray<TimedMessage> activeMessages;
  csPDelArray<TimedMessage> queuedMessages;
  // Activate messages and clean up already displayed
  // previous messages.
  void CheckNewMessages ();
  void CleanReadyMessages ();

  int cursizex, cursizey;		// Current size without margins.
  // Recalculate the current box size based on the present messages.
  void RecalculateCurrentSize ();

  void InitPen ();

public:
  MessageSlot (iGraphics3D* g3d, const char* name) :
    g3d (g3d),
    name (name), position (10, 10),
    boxAnchor (ANCHOR_NORTHWEST), screenAnchor (ANCHOR_NORTHWEST),
    boxColor (0, 0, 0, 0), borderColor (0, 0, 0, 0),
    borderWidth (0.0f), roundness (0),
    sizex (-1), sizey (-1), maxsizex (600), maxsizey (300),
    marginx (5), marginy (3),
    maxmessages (2),
    queue (true), boxfadetime (0.0f)
  {
    boxPen = 0;
    borderPen = 0;
    fadetimeleft = 0.0f;
  }
  ~MessageSlot ();

  void SetSlotPosition (const csVector2& position,
      MessageLocationAnchor boxAnchor,
      MessageLocationAnchor screenAnchor)
  {
    MessageSlot::position = position;
    MessageSlot::boxAnchor = boxAnchor;
    MessageSlot::screenAnchor = screenAnchor;
  }
  void SetSlotDimensions (
      int sizex, int sizey, int maxsizex, int maxsizey,
      int marginx, int marginy)
  {
    MessageSlot::sizex = sizex;
    MessageSlot::sizey = sizey;
    MessageSlot::maxsizex = maxsizex;
    MessageSlot::maxsizey = maxsizey;
    MessageSlot::marginx = marginx;
    MessageSlot::marginy = marginy;
  }
  void SetSlotBoxAttributes (
      const csColor& boxColor, const csColor4& borderColor,
      float borderWidth, int roundness, float boxfadetime)
  {
    MessageSlot::boxColor = boxColor;
    MessageSlot::borderColor = borderColor;
    MessageSlot::borderWidth = borderWidth;
    MessageSlot::roundness = roundness;
    MessageSlot::boxfadetime = boxfadetime;
    InitPen ();
  }
  void SetSlotMessageHandling (int maxmessages, bool queue)
  {
    MessageSlot::maxmessages = maxmessages;
    MessageSlot::queue = queue;
  }

  const char* GetName () const { return name; }

  void Message (const char* msg, float timeout, float fadetime, iFont* font,
      int color);

  void HandleElapsed (float elapsed);
  void Render3D (iGraphics3D* g3d, iGraphics2D* g2d);
  void Render2D (iGraphics3D* g3d, iGraphics2D* g2d);
  bool Needs3DRender ();
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
  static csStringID id_boxanchor;
  static csStringID id_screenanchor;
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
  static csStringID id_roundness;

  csRef<iVirtualClock> vc;
  csRef<iGraphics3D> g3d;
  csRandomGen rng;

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

  virtual void DefineSlot (const char* name);
  virtual void SetSlotPosition (const char* name, const csVector2& position,
      MessageLocationAnchor boxAnchor,
      MessageLocationAnchor screenAnchor);
  virtual void SetSlotDimensions (const char* name,
      int sizex, int sizey, int maxsizex, int maxsizey,
      int marginx, int marginy);
  virtual void SetSlotBoxAttributes (const char* name,
      const csColor& boxColor, const csColor4& borderColor,
      float borderWidth, int roundness, float boxfadetime);
  virtual void SetSlotMessageHandling (const char* name, int maxmessages, bool queue);

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

  virtual void TickEveryFrame ();
};

#endif // __CEL_PF_MESSENGERFACT__

