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
CEL_IMPLEMENT_FACTORY (Properties, "pcproperties")

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcTooltip)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTooltip)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTooltip::PcTooltip)
  SCF_IMPLEMENTS_INTERFACE (iPcTooltip)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcTooltip::celPcTooltip (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTooltip);
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
  G3D = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  G2D = CS_QUERY_REGISTRY (object_reg, iGraphics2D);
}

celPcTooltip::~celPcTooltip ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcTooltip);
}

#define TOOLTIP_SERIAL 1

csPtr<iCelDataBuffer> celPcTooltip::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TOOLTIP_SERIAL);
  databuf->Add (visible);
  databuf->Add ((uint16)x);
  databuf->Add ((uint16)y);
  databuf->Add ((uint8)text_r);
  databuf->Add ((uint8)text_g);
  databuf->Add ((uint8)text_b);
  databuf->Add ((int16)bg_r);	// Room for negative.
  databuf->Add ((int16)bg_g);
  databuf->Add ((int16)bg_b);
  return csPtr<iCelDataBuffer> (databuf);
}


bool celPcTooltip::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TOOLTIP_SERIAL) return false;
  visible = databuf->GetBool ();
  x = databuf->GetUInt16 ();
  y = databuf->GetUInt16 ();
  text_r = databuf->GetUInt8 ();
  text_g = databuf->GetUInt8 ();
  text_b = databuf->GetUInt8 ();
  bg_r = databuf->GetInt16 ();
  bg_g = databuf->GetInt16 ();
  bg_b = databuf->GetInt16 ();

  return true;
}

void celPcTooltip::SetText (const char* t)
{
  lines.DeleteAll ();
  if (strcmp (t, "") != 0)
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
  } else {
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
  pl->RemoveCallbackPCEveryFrame (this, cscmdPostProcess);
}

void celPcTooltip::Show (int x, int y)
{
  celPcTooltip::x = x;
  celPcTooltip::y = y;
  if (visible) return;
  visible = true;
  pl->CallbackPCEveryFrame (this, cscmdPostProcess);
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
  int bgcolor = bg_r == -1 ? 0 : G2D->FindRGB (bg_r, bg_g, bg_b);
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
    G2D->Write (fnt, x + offset, y + (linenum * fnt->GetTextHeight ()), fgcolor, bgcolor, line.GetData ());
    linenum++;
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcTimer)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTimer)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTimer::PcTimer)
  SCF_IMPLEMENTS_INTERFACE (iPcTimer)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

csStringID celPcTimer::action_wakeup = csInvalidStringID;
csStringID celPcTimer::action_wakeupframe = csInvalidStringID;
csStringID celPcTimer::action_clear = csInvalidStringID;

csStringID celPcTimer::id_elapsedticks = csInvalidStringID;
csStringID celPcTimer::id_currentticks = csInvalidStringID;
csStringID celPcTimer::id_time = csInvalidStringID;
csStringID celPcTimer::id_repeat = csInvalidStringID;

celPcTimer::celPcTimer (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTimer);
  enabled = false;
  wakeupframe = false;
  wakeuponce = false;
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != 0);
  DG_TYPE (this, "celPcTimer()");
  if (action_wakeup == csInvalidStringID)
  {
    action_wakeup = pl->FetchStringID ("cel.action.WakeUp");
    action_wakeupframe = pl->FetchStringID ("cel.action.WakeUpFrame");
    action_clear = pl->FetchStringID ("cel.action.Clear");
    id_elapsedticks = pl->FetchStringID ("cel.parameter.elapsedticks");
    id_currentticks = pl->FetchStringID ("cel.parameter.currentticks");
    id_time = pl->FetchStringID ("cel.parameter.time");
    id_repeat = pl->FetchStringID ("cel.parameter.repeat");
  }
  params = new celGenericParameterBlock (2);
  params->SetParameterDef (0, id_elapsedticks, "elapsedticks");
  params->SetParameterDef (1, id_currentticks, "currentticks");
}

celPcTimer::~celPcTimer ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcTimer);
}

bool celPcTimer::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_wakeup)
  {
    CEL_FETCH_LONG_PAR (time,params,id_time);
    if (!p_time) return false;
    CEL_FETCH_BOOL_PAR (repeat,params,id_repeat);
    if (!p_repeat) return false;
    WakeUp ((csTicks)time, repeat);
    return true;
  }
  else if (actionId == action_wakeupframe)
  {
    WakeUpFrame ();
    return true;
  }
  else if (actionId == action_clear)
  {
    Clear ();
    return true;
  }
  return false;
}

#define TIMER_SERIAL 3

csPtr<iCelDataBuffer> celPcTimer::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TIMER_SERIAL);
  databuf->Add (enabled);
  databuf->Add ((int32)wakeup);
  databuf->Add (repeat);
  databuf->Add (wakeupframe);
  databuf->Add (wakeuponce);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTimer::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TIMER_SERIAL) return false;
  enabled = databuf->GetBool ();
  wakeup = databuf->GetInt32 ();
  repeat = databuf->GetBool ();
  wakeupframe = databuf->GetBool ();
  wakeuponce = databuf->GetBool ();

  return true;
}

void celPcTimer::Clear ()
{
  enabled = false;
  wakeupframe = false;
  wakeuponce = false;
  pl->RemoveCallbackPCOnce (this, cscmdPreProcess);
  pl->RemoveCallbackPCEveryFrame (this, cscmdPreProcess);
}

void celPcTimer::WakeUp (csTicks t, bool repeat)
{
  enabled = true;
  wakeuponce = true;
  pl->RemoveCallbackPCOnce (this, cscmdPreProcess);
  pl->CallbackPCOnce (this, t, cscmdPreProcess);

  celPcTimer::repeat = repeat;
  wakeup = t;
}

void celPcTimer::WakeUpFrame (int where)
{
  if (wakeupframe) return;
  enabled = true;
  wakeupframe = true;
  pl->CallbackPCEveryFrame (this, where);
}

void celPcTimer::TickEveryFrame ()
{
  // To prevent the entity from being deleted during
  // the call of pctimer_wakeupframe we keep a temporary reference
  // here.
  csRef<iCelEntity> ref;
  if (wakeupframe)
  {
    ref = entity;
    iCelBehaviour* bh = entity->GetBehaviour ();
    CS_ASSERT (bh != 0);
    params->GetParameter (0).Set ((int32)vc->GetElapsedTicks ());
    params->GetParameter (1).Set ((int32)vc->GetCurrentTicks ());
    celData ret;
    bh->SendMessage ("pctimer_wakeupframe", this, ret, params);
  }
}

void celPcTimer::TickOnce ()
{
  // To prevent the entity from being deleted during
  // the call of pctimer_wakeupframe we keep a temporary reference
  // here.
  csRef<iCelEntity> ref;
  if (wakeuponce)
  {
    if (repeat)
    {
      pl->CallbackPCOnce (this, wakeup, cscmdPreProcess);
    }
    else
    {
      wakeuponce = false;
    }
    iCelBehaviour* bh = entity->GetBehaviour ();
    CS_ASSERT (bh != 0);
    celData ret;
    bh->SendMessage ("pctimer_wakeup", this, ret, 0);
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcProperties)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcProperties)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcProperties::PcProperties)
  SCF_IMPLEMENTS_INTERFACE (iPcProperties)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

csStringID celPcProperties::id_index = csInvalidStringID;

celPcProperties::celPcProperties (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcProperties);
  DG_TYPE (this, "celPcProperties()");
  if (id_index == csInvalidStringID)
    id_index = pl->FetchStringID ("cel.parameter.index");
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_index, "index");
  properties_hash_dirty = false;
}

celPcProperties::~celPcProperties ()
{
  Clear ();
  delete params;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcProperties);
}

size_t celPcProperties::FindProperty (csStringID id)
{
  size_t i;
  for (i = 0 ; i < properties.Length () ; i++)
  {
    property* p = properties[i];
    if (p->id == csInvalidStringID)
    {
      char* buf = new char[30+strlen (p->propName)];
      if (p->type == CEL_DATA_ACTION)
        strcpy (buf, "cel.action.");
      else
        strcpy (buf, "cel.property.");
      strcat (buf, p->propName);
      p->id = pl->FetchStringID (buf);
      delete[] buf;
    }
    if (p->id == id) return i;
  }
  return csArrayItemNotFound;
}

size_t celPcProperties::FindOrNewProperty (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx != csArrayItemNotFound) return idx;
  const char* propname = pl->FetchString (id);
  return NewProperty (propname+13);
}

celDataType celPcProperties::GetPropertyOrActionType (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return CEL_DATA_NONE;
  return properties[idx]->type;
}

csStringID celPcProperties::GetPropertyOrActionID (int i)
{
  property* p = properties[i];
  if (p->id == csInvalidStringID)
  {
    char* buf = new char[30+strlen (p->propName)];
    if (p->type == CEL_DATA_ACTION)
      strcpy (buf, "cel.action.");
    else
      strcpy (buf, "cel.property.");
    strcat (buf, p->propName);
    p->id = pl->FetchStringID (buf);
    delete[] buf;
  }
  return p->id;
}

const char* celPcProperties::GetPropertyOrActionDescription (csStringID id)
{
  return "none";
}

bool celPcProperties::SetProperty (csStringID id, const char* v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, bool v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, float v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, long v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, const csVector2& v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, const csVector3& v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, const csColor& v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, iCelPropertyClass* v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, iCelEntity* v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

const char* celPcProperties::GetPropertyString (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyStringIndex (idx);
}

bool celPcProperties::GetPropertyBool (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyBoolIndex (idx);
}

long celPcProperties::GetPropertyLong (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyLongIndex (idx);
}

float celPcProperties::GetPropertyFloat (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyFloatIndex (idx);
}

bool celPcProperties::GetPropertyVector (csStringID id, csVector2& v)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyVectorIndex (idx, v);
}

bool celPcProperties::GetPropertyVector (csStringID id, csVector3& v)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyVectorIndex (idx, v);
}

bool celPcProperties::GetPropertyColor (csStringID id, csColor& v)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyColorIndex (idx, v);
}

iCelPropertyClass* celPcProperties::GetPropertyPClass (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyPClassIndex (idx);
}

iCelEntity* celPcProperties::GetPropertyEntity (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyEntityIndex (idx);
}

#define PROPERTIES_SERIAL 1

csPtr<iCelDataBuffer> celPcProperties::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (PROPERTIES_SERIAL);
  size_t i;
  databuf->Add ((uint32)properties.Length ());
  for (i = 0 ; i < properties.Length () ; i++)
  {
    property* p = properties[i];
    databuf->Add (p->propName);
    databuf->Add ((uint8)p->type);
    switch (p->type)
    {
      case CEL_DATA_FLOAT:
        databuf->Add (p->v.f);
	break;
      case CEL_DATA_LONG:
        databuf->Add ((int32)p->v.l);
	break;
      case CEL_DATA_BOOL:
        databuf->Add (p->v.b);
	break;
      case CEL_DATA_STRING:
        databuf->Add (p->v.s);
	break;
      case CEL_DATA_VECTOR2:
        databuf->Add (csVector2 (p->v.vec.x, p->v.vec.y));
	break;
      case CEL_DATA_VECTOR3:
        databuf->Add (csVector3 (p->v.vec.x, p->v.vec.y,
		p->v.vec.z));
	break;
      case CEL_DATA_COLOR:
        databuf->Add (csVector3 (p->v.col.red, p->v.col.green,
		p->v.col.blue));
	break;
      case CEL_DATA_PCLASS:
        databuf->Add (p->pclass);
	break;
      case CEL_DATA_ENTITY:
        databuf->Add (p->entity);
	break;
      default:
        // @@@ Impossible!
	break;
    }
  }
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcProperties::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != PROPERTIES_SERIAL) return false;
  properties_hash_dirty = true;
  size_t cnt = databuf->GetUInt32 ();
  Clear ();
  celData* cd;
  size_t i;
  for (i = 0 ; i < cnt ; i++)
  {
    size_t idx = NewProperty (databuf->GetString ()->GetData ());
    cd = databuf->GetData (); if (!cd) return false;
    property* p = properties[idx];
    p->type = (celDataType)cd->value.ub;
    switch (p->type)
    {
      case CEL_DATA_FLOAT:
        p->v.f = cd->value.f;
	break;
      case CEL_DATA_LONG:
        p->v.l = cd->value.l;
	break;
      case CEL_DATA_BOOL:
        p->v.b = cd->value.bo;
	break;
      case CEL_DATA_STRING:
        p->v.s = csStrNew (*cd->value.s);
	break;
      case CEL_DATA_VECTOR2:
        p->v.vec.x = cd->value.v.x;
        p->v.vec.y = cd->value.v.y;
	break;
      case CEL_DATA_VECTOR3:
        p->v.vec.x = cd->value.v.x;
        p->v.vec.y = cd->value.v.y;
        p->v.vec.z = cd->value.v.z;
	break;
      case CEL_DATA_COLOR:
        p->v.col.red = cd->value.col.red;
        p->v.col.green = cd->value.col.green;
        p->v.col.blue = cd->value.col.blue;
	break;
      case CEL_DATA_PCLASS:
        p->pclass = cd->value.pc;
	break;
      case CEL_DATA_ENTITY:
        p->entity = cd->value.ent;
	break;
      default:
        return false;
    }
  }

  return true;
}

size_t celPcProperties::NewProperty (const char* name)
{
  property* p = new property ();
  p->id = csInvalidStringID;	// Fetch later if needed.
  p->propName = csStrNew (name);
  p->type = CEL_DATA_NONE;
  size_t idx = properties.Length ();
  properties.Push (p);
  if (!properties_hash_dirty) properties_hash.Put (name, idx+1);	// +1 so that 0 is an error.
  return idx;
}

size_t celPcProperties::FindOrNewProperty (const char* name)
{
  size_t idx = GetPropertyIndex (name);
  if (idx != csArrayItemNotFound) return idx;
  return NewProperty (name);
}

void celPcProperties::ClearPropertyValue (property* p)
{
  if (p->type == CEL_DATA_STRING) delete[] p->v.s;
  p->pclass = 0;
  p->entity = 0;
  p->type = CEL_DATA_NONE;
}

void celPcProperties::SetProperty (const char* name, float value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, long value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, bool value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const char* value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const csVector2& value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const csVector3& value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const csColor& value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, iCelPropertyClass* value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, iCelEntity* value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

size_t celPcProperties::GetPropertyIndex (const char* name)
{
  if (properties_hash_dirty)
  {
    properties_hash_dirty = false;
    // Create the hash.
    properties_hash.DeleteAll ();
    size_t i;
    for (i = 0 ; i < properties.Length () ; i++)
    {
      property* p = properties[i];
      properties_hash.Put (p->propName, i+1);	// +1 so that 0 is an error
    }
  }

  return properties_hash.Get (name,0) - 1;
}

void celPcProperties::SetPropertyIndex (size_t index, float value)
{
  CS_ASSERT ((index != csArrayItemNotFound) && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_FLOAT;
  p->v.f = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, long value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_LONG;
  p->v.l = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, bool value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_BOOL;
  p->v.b = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, const csVector2& value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_VECTOR2;
  p->v.vec.x = value.x;
  p->v.vec.y = value.y;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, const csVector3& value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_VECTOR3;
  p->v.vec.x = value.x;
  p->v.vec.y = value.y;
  p->v.vec.z = value.z;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, const csColor& value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_COLOR;
  p->v.col.red = value.red;
  p->v.col.green = value.green;
  p->v.col.blue = value.blue;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, const char* value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_STRING;
  p->v.s = csStrNew (value);
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, iCelPropertyClass* value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_PCLASS;
  p->pclass = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, iCelEntity* value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_ENTITY;
  p->entity = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

celDataType celPcProperties::GetPropertyType (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  return p->type;
}

float celPcProperties::GetPropertyFloatIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_FLOAT)
    return p->v.f;
  else
    return 0;
}

long celPcProperties::GetPropertyLongIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_LONG)
    return p->v.l;
  else
    return 0;
}

bool celPcProperties::GetPropertyBoolIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_BOOL)
    return p->v.b;
  else
    return false;
}

bool celPcProperties::GetPropertyVectorIndex (size_t index, csVector2& v) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_VECTOR2)
  {
    v.x = p->v.vec.x;
    v.y = p->v.vec.y;
    return true;
  }
  else
    return false;
}

bool celPcProperties::GetPropertyVectorIndex (size_t index, csVector3& v) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_VECTOR3)
  {
    v.x = p->v.vec.x;
    v.y = p->v.vec.y;
    v.z = p->v.vec.z;
    return true;
  }
  else
    return false;
}

bool celPcProperties::GetPropertyColorIndex (size_t index, csColor& v) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_COLOR)
  {
    v.red = p->v.col.red;
    v.green = p->v.col.green;
    v.blue = p->v.col.blue;
    return true;
  }
  else
    return false;
}

const char* celPcProperties::GetPropertyStringIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_STRING)
    return p->v.s;
  else
    return 0;
}

iCelPropertyClass* celPcProperties::GetPropertyPClassIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_PCLASS)
    return p->pclass;
  else
    return 0;
}

iCelEntity* celPcProperties::GetPropertyEntityIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_ENTITY)
    return p->entity;
  else
    return 0;
}

void celPcProperties::ClearProperty (size_t index)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_clearproperty", this, ret, params, index);
  }
  property* p = properties[index];
  ClearPropertyValue (p);
  properties.DeleteIndex (index);
  // Properties hash is cleared because it is invalid now.
  properties_hash_dirty = true;
}

void celPcProperties::Clear ()
{
  while (properties.Length () > 0)
  {
    ClearProperty (0);
  }
  properties_hash_dirty = false;
  properties_hash.DeleteAll ();
}

size_t celPcProperties::GetPropertyCount () const
{
  return properties.Length ();
}

const char* celPcProperties::GetPropertyName (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  return p->propName;
}

void celPcProperties::Dump ()
{
}

void celPcProperties::AddPropertyListener (iPcPropertyListener* listener)
{
  listeners.Push (listener);
}

void celPcProperties::RemovePropertyListener (iPcPropertyListener* listener)
{
  listeners.Delete (listener);
}

void celPcProperties::FirePropertyListeners (size_t idx)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->PropertyChanged (&scfiPcProperties, idx);
  }
}

//---------------------------------------------------------------------------

