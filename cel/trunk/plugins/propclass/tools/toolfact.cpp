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

SCF_IMPLEMENT_IBASE (celPcTooltip::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcTooltip::celPcTooltip (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTooltip);
  scfiEventHandler = 0;
  visible = false;
  text = 0;
  text_r = 0;
  text_g = 0;
  text_b = 0;
  bg_r = 255;
  bg_g = 255;
  bg_b = 0;
  DG_TYPE (this, "celPcTooltip()");
}

celPcTooltip::~celPcTooltip ()
{
  delete[] text;
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    if (q != 0)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
}

#define TOOLTIP_SERIAL 1

csPtr<iCelDataBuffer> celPcTooltip::Save ()
{
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (TOOLTIP_SERIAL));
  databuf->SetDataCount (10);
  databuf->GetData (0)->SetBool (visible);
  databuf->GetData (1)->Set ((uint16)x);
  databuf->GetData (2)->Set ((uint16)y);
  databuf->GetData (3)->Set (text);
  databuf->GetData (4)->Set ((uint8)text_r);
  databuf->GetData (5)->Set ((uint8)text_g);
  databuf->GetData (6)->Set ((uint8)text_b);
  databuf->GetData (7)->Set ((int16)bg_r);	// Room for negative.
  databuf->GetData (8)->Set ((int16)bg_g);
  databuf->GetData (9)->Set ((int16)bg_b);
  return csPtr<iCelDataBuffer> (databuf);
}


bool celPcTooltip::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TOOLTIP_SERIAL) return false;
  if (databuf->GetDataCount () != 10) return false;
  celData* cd;
  cd = databuf->GetData (0); if (!cd) return false; visible = cd->value.bo;
  cd = databuf->GetData (1); if (!cd) return false; x = cd->value.uw;
  cd = databuf->GetData (2); if (!cd) return false; y = cd->value.uw;
  delete[] text; text = 0;
  cd = databuf->GetData (3); if (!cd) return false;
  text = csStrNew (*cd->value.s);
  cd = databuf->GetData (4); if (!cd) return false; text_r = cd->value.ub;
  cd = databuf->GetData (5); if (!cd) return false; text_g = cd->value.ub;
  cd = databuf->GetData (6); if (!cd) return false; text_b = cd->value.ub;
  cd = databuf->GetData (7); if (!cd) return false; bg_r = cd->value.w;
  cd = databuf->GetData (8); if (!cd) return false; bg_g = cd->value.w;
  cd = databuf->GetData (9); if (!cd) return false; bg_b = cd->value.w;

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
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    CS_ASSERT (q != 0);
    q->RemoveListener (scfiEventHandler);
  }
}

void celPcTooltip::Show (int x, int y)
{
  celPcTooltip::x = x;
  celPcTooltip::y = y;
  if (visible) return;
  visible = true;
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);
}

bool celPcTooltip::HandleEvent (iEvent& ev)
{
  if (!visible) return false;
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdPostProcess)
  {
    csRef<iGraphics3D> G3D (CS_QUERY_REGISTRY (object_reg, iGraphics3D));
    csRef<iGraphics2D> G2D (CS_QUERY_REGISTRY (object_reg, iGraphics2D));

    G3D->BeginDraw (CSDRAW_2DGRAPHICS);
    iFontServer* fntsvr = G2D->GetFontServer ();
    CS_ASSERT (fntsvr != 0);
    csRef<iFont> fnt = fntsvr->LoadFont (CSFONT_COURIER);
    CS_ASSERT (fnt != 0);
    //int fw, fh;
    //fnt->GetMaxSize (fw, fh);
    //int sw = G2D->GetWidth ();
    //int sh = G2D->GetHeight ();
    int fgcolor = G2D->FindRGB (text_r, text_g, text_b);
    int bgcolor = bg_r == -1 ? 0 : G2D->FindRGB (bg_r, bg_g, bg_b);
    //G2D->DrawBox (x, y, w, h, bgcolor);
    //int maxlen = fnt->GetLength (text, w-10);
    G2D->Write (fnt, x, y, fgcolor, bgcolor, text);
  }
  return false;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcTimer)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTimer)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTimer::PcTimer)
  SCF_IMPLEMENTS_INTERFACE (iPcTimer)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcTimer::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

csStringID celPcTimer::action_wakeup = csInvalidStringID;
csStringID celPcTimer::action_wakeupframe = csInvalidStringID;
csStringID celPcTimer::action_clear = csInvalidStringID;

csStringID celPcTimer::id_elapsedticks = csInvalidStringID;
csStringID celPcTimer::id_currentticks = csInvalidStringID;

celPcTimer::celPcTimer (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTimer);
  scfiEventHandler = 0;
  enabled = false;
  wakeupframe = false;
  wakeuponce = false;
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != 0);
  DG_TYPE (this, "celPcTimer()");
  csRef<iCelPlLayer> player = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  pl = player;
  if (action_wakeup == csInvalidStringID)
  {
    action_wakeup = pl->FetchStringID ("cel.property.WakeUp");
    action_wakeupframe = pl->FetchStringID ("cel.property.WakeUpFrame");
    action_clear = pl->FetchStringID ("cel.property.Clear");
    id_elapsedticks = pl->FetchStringID ("cel.parameter.elapsedticks");
    id_currentticks = pl->FetchStringID ("cel.parameter.currentticks");
  }
  params = new celGenericParameterBlock (2);
  params->SetParameterDef (0, id_elapsedticks, "elapsedticks");
  params->SetParameterDef (1, id_currentticks, "currentticks");
}

celPcTimer::~celPcTimer ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    if (q != 0)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
}

bool celPcTimer::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_wakeup)
  {
    const celData* p_t = params->GetParameter (pl->FetchStringID (
    	"cel.parameter.time"));
    if (!p_t) return false;
    if (p_t->type != CEL_DATA_LONG) return false;
    const celData* p_repeat = params->GetParameter (pl->FetchStringID (
    	"cel.parameter.repeat"));
    if (!p_repeat) return false;
    if (p_repeat->type != CEL_DATA_BOOL) return false;
    csTicks t = p_t->value.l;
    bool repeat = p_repeat->value.bo;
    WakeUp (t, repeat);
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
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (TIMER_SERIAL));
  databuf->SetDataCount (6);
  databuf->GetData (0)->SetBool (enabled);
  databuf->GetData (1)->Set ((int32)wakeup);
  databuf->GetData (2)->Set ((int32)wakeup_todo);
  databuf->GetData (3)->SetBool (repeat);
  databuf->GetData (4)->SetBool (wakeupframe);
  databuf->GetData (5)->SetBool (wakeuponce);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTimer::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TIMER_SERIAL) return false;
  if (databuf->GetDataCount () != 6) return false;
  celData* cd;
  cd = databuf->GetData (0); if (!cd) return false;
  enabled = cd->value.bo;
  cd = databuf->GetData (1); if (!cd) return false;
  wakeup = cd->value.l;
  cd = databuf->GetData (2); if (!cd) return false;
  wakeup_todo = cd->value.l;
  cd = databuf->GetData (3); if (!cd) return false;
  repeat = cd->value.bo;
  cd = databuf->GetData (4); if (!cd) return false;
  wakeupframe = cd->value.bo;
  cd = databuf->GetData (5); if (!cd) return false;
  wakeuponce = cd->value.bo;

  return true;
}

void celPcTimer::Clear ()
{
  enabled = false;
  wakeupframe = false;
  wakeuponce = false;
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    CS_ASSERT (q != 0);
    q->RemoveListener (scfiEventHandler);
  }
}

void celPcTimer::WakeUp (csTicks t, bool repeat)
{
  enabled = true;
  wakeuponce = true;
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);

  celPcTimer::repeat = repeat;
  wakeup = t;
  wakeup_todo = 0;
}

void celPcTimer::WakeUpFrame ()
{
  if (wakeupframe) return;
  enabled = true;
  wakeupframe = true;
  if (!scfiEventHandler)
  {
    scfiEventHandler = new EventHandler (this);
  }
  csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  unsigned int trigger = CSMASK_Nothing;
  q->RegisterListener (scfiEventHandler, trigger);
}

bool celPcTimer::HandleEvent (iEvent& ev)
{
  if (ev.Type == csevBroadcast && ev.Command.Code == cscmdPreProcess)
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
      bh->SendMessage ("pctimer_wakeupframe", ret, params);
    }
    if (wakeuponce)
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
	  if (wakeupframe)
	  {
	    wakeuponce = false;
	  }
	  else
	  {
            Clear ();
	  }
        }
        iCelBehaviour* bh = entity->GetBehaviour ();
        CS_ASSERT (bh != 0);
	celData ret;
        bh->SendMessage ("pctimer_wakeup", ret, 0);
      }
    }
  }
  return false;
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
  csRef<iCelPlLayer> player = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  pl = player;

  if (id_index == csInvalidStringID)
  {
    id_index = pl->FetchStringID ("cel.parameter.index");
  }
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_index, "index");
  properties_hash_dirty = false;
}

celPcProperties::~celPcProperties ()
{
  Clear ();
  delete params;
}

int celPcProperties::FindProperty (csStringID id)
{
  int i;
  for (i = 0 ; i < properties.Length () ; i++)
  {
    property* p = properties[i];
    if (p->id == csInvalidStringID)
    {
      char* buf = new char[30+strlen (p->propName)];
      strcpy (buf, "cel.property.");
      strcat (buf, p->propName);
      p->id = pl->FetchStringID (buf);
      delete[] buf;
    }
    if (p->id == id) return i;
  }
  return -1;
}

int celPcProperties::FindOrNewProperty (csStringID id)
{
  int idx = FindProperty (id);
  if (idx != -1) return idx;
  const char* propname = pl->FetchString (id);
  return NewProperty (propname+26);
}

celDataType celPcProperties::GetPropertyOrActionType (csStringID id)
{
  int idx = FindProperty (id);
  if (idx == -1) return CEL_DATA_NONE;
  return properties[idx]->type;
}

csStringID celPcProperties::GetPropertyOrActionID (int i)
{
  property* p = properties[i];
  if (p->id == csInvalidStringID)
  {
    char* buf = new char[30+strlen (p->propName)];
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
  SetProperty (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, bool v)
{
  SetProperty (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, float v)
{
  SetProperty (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, long v)
{
  SetProperty (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, const csVector2& v)
{
  SetProperty (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, const csVector3& v)
{
  SetProperty (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, const csColor& v)
{
  SetProperty (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, iCelPropertyClass* v)
{
  SetProperty (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, iCelEntity* v)
{
  SetProperty (FindOrNewProperty (id), v);
  return true;
}

const char* celPcProperties::GetPropertyString (csStringID id)
{
  int idx = FindProperty (id);
  if (idx == -1) return 0;
  return GetPropertyStringIndex (idx);
}

bool celPcProperties::GetPropertyBool (csStringID id)
{
  int idx = FindProperty (id);
  if (idx == -1) return false;
  return GetPropertyBoolIndex (idx);
}

long celPcProperties::GetPropertyLong (csStringID id)
{
  int idx = FindProperty (id);
  if (idx == -1) return 0;
  return GetPropertyLongIndex (idx);
}

float celPcProperties::GetPropertyFloat (csStringID id)
{
  int idx = FindProperty (id);
  if (idx == -1) return 0;
  return GetPropertyFloatIndex (idx);
}

bool celPcProperties::GetPropertyVector (csStringID id, csVector2& v)
{
  int idx = FindProperty (id);
  if (idx == -1) return 0;
  return GetPropertyVectorIndex (idx, v);
}

bool celPcProperties::GetPropertyVector (csStringID id, csVector3& v)
{
  int idx = FindProperty (id);
  if (idx == -1) return 0;
  return GetPropertyVectorIndex (idx, v);
}

bool celPcProperties::GetPropertyColor (csStringID id, csColor& v)
{
  int idx = FindProperty (id);
  if (idx == -1) return 0;
  return GetPropertyColorIndex (idx, v);
}

iCelPropertyClass* celPcProperties::GetPropertyPClass (csStringID id)
{
  int idx = FindProperty (id);
  if (idx == -1) return 0;
  return GetPropertyPClassIndex (idx);
}

iCelEntity* celPcProperties::GetPropertyEntity (csStringID id)
{
  int idx = FindProperty (id);
  if (idx == -1) return 0;
  return GetPropertyEntityIndex (idx);
}

#define PROPERTIES_SERIAL 1

csPtr<iCelDataBuffer> celPcProperties::Save ()
{
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (PROPERTIES_SERIAL));
  databuf->SetDataCount (properties.Length ()*3);
  int i, j = 0;
  for (i = 0 ; i < properties.Length () ; i++)
  {
    property* p = properties[i];
    databuf->GetData (j++)->Set (p->propName);
    databuf->GetData (j++)->Set ((uint8)p->type);
    switch (p->type)
    {
      case CEL_DATA_FLOAT:
        databuf->GetData (j++)->Set (p->v.f);
	break;
      case CEL_DATA_LONG:
        databuf->GetData (j++)->Set ((int32)p->v.l);
	break;
      case CEL_DATA_BOOL:
        databuf->GetData (j++)->SetBool (p->v.b);
	break;
      case CEL_DATA_STRING:
        databuf->GetData (j++)->Set (p->v.s);
	break;
      case CEL_DATA_VECTOR2:
        databuf->GetData (j++)->Set (csVector2 (p->v.vec.x, p->v.vec.y));
	break;
      case CEL_DATA_VECTOR3:
        databuf->GetData (j++)->Set (csVector3 (p->v.vec.x, p->v.vec.y,
		p->v.vec.z));
	break;
      case CEL_DATA_COLOR:
        databuf->GetData (j++)->Set (csVector3 (p->v.col.red, p->v.col.green,
		p->v.col.blue));
	break;
      case CEL_DATA_PCLASS:
        databuf->GetData (j++)->Set (p->pclass);
	break;
      case CEL_DATA_ENTITY:
        databuf->GetData (j++)->Set (p->entity);
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
  int cnt = databuf->GetDataCount ();
  Clear ();
  celData* cd;
  int i, j = 0;
  for (i = 0 ; i < cnt / 3 ; i++)
  {
    cd = databuf->GetData (j++); if (!cd) return false;
    int idx = NewProperty (*cd->value.s);
    cd = databuf->GetData (j++); if (!cd) return false;
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

int celPcProperties::NewProperty (const char* name)
{
  property* p = new property ();
  p->id = csInvalidStringID;	// Fetch later if needed.
  p->propName = csStrNew (name);
  p->type = CEL_DATA_NONE;
  int idx = properties.Length ();
  properties.Push (p);
  if (!properties_hash_dirty) properties_hash.Put (name, idx+1);	// +1 so that 0 is an error.
  return idx;
}

int celPcProperties::FindOrNewProperty (const char* name)
{
  int idx = GetPropertyIndex (name);
  if (idx != -1) return idx;
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
  SetProperty (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, long value)
{
  SetProperty (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, bool value)
{
  SetProperty (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const char* value)
{
  SetProperty (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const csVector2& value)
{
  SetProperty (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const csVector3& value)
{
  SetProperty (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const csColor& value)
{
  SetProperty (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, iCelPropertyClass* value)
{
  SetProperty (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, iCelEntity* value)
{
  SetProperty (FindOrNewProperty (name), value);
}

int celPcProperties::GetPropertyIndex (const char* name)
{
  if (properties_hash_dirty)
  {
    properties_hash_dirty = false;
    // Create the hash.
    properties_hash.DeleteAll ();
    int i;
    for (i = 0 ; i < properties.Length () ; i++)
    {
      property* p = properties[i];
      properties_hash.Put (p->propName, i+1);	// +1 so that 0 is an error
    }
  }

  return properties_hash.Get (name,0)-1;
}

void celPcProperties::SetProperty (int index, float value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_FLOAT;
  p->v.f = value;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", ret, params, index);
  }
}

void celPcProperties::SetProperty (int index, long value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_LONG;
  p->v.l = value;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", ret, params, index);
  }
}

void celPcProperties::SetProperty (int index, bool value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_BOOL;
  p->v.b = value;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", ret, params, index);
  }
}

void celPcProperties::SetProperty (int index, const csVector2& value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_VECTOR2;
  p->v.vec.x = value.x;
  p->v.vec.y = value.y;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", ret, params, index);
  }
}

void celPcProperties::SetProperty (int index, const csVector3& value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_VECTOR3;
  p->v.vec.x = value.x;
  p->v.vec.y = value.y;
  p->v.vec.z = value.z;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", ret, params, index);
  }
}

void celPcProperties::SetProperty (int index, const csColor& value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_COLOR;
  p->v.col.red = value.red;
  p->v.col.green = value.green;
  p->v.col.blue = value.blue;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", ret, params, index);
  }
}

void celPcProperties::SetProperty (int index, const char* value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_STRING;
  p->v.s = csStrNew (value);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", ret, params, index);
  }
}

void celPcProperties::SetProperty (int index, iCelPropertyClass* value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_PCLASS;
  p->pclass = value;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", ret, params, index);
  }
}

void celPcProperties::SetProperty (int index, iCelEntity* value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_ENTITY;
  p->entity = value;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", ret, params, index);
  }
}

celDataType celPcProperties::GetPropertyType (int index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  return p->type;
}

float celPcProperties::GetPropertyFloatIndex (int index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_FLOAT)
    return p->v.f;
  else
    return 0;
}

long celPcProperties::GetPropertyLongIndex (int index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_LONG)
    return p->v.l;
  else
    return 0;
}

bool celPcProperties::GetPropertyBoolIndex (int index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_BOOL)
    return p->v.b;
  else
    return false;
}

bool celPcProperties::GetPropertyVectorIndex (int index, csVector2& v) const
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

bool celPcProperties::GetPropertyVectorIndex (int index, csVector3& v) const
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

bool celPcProperties::GetPropertyColorIndex (int index, csColor& v) const
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

const char* celPcProperties::GetPropertyStringIndex (int index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_STRING)
    return p->v.s;
  else
    return 0;
}

iCelPropertyClass* celPcProperties::GetPropertyPClassIndex (int index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_PCLASS)
    return p->pclass;
  else
    return 0;
}

iCelEntity* celPcProperties::GetPropertyEntityIndex (int index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_ENTITY)
    return p->entity;
  else
    return 0;
}

void celPcProperties::ClearProperty (int index)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_clearproperty", ret, params, index);
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

int celPcProperties::GetPropertyCount () const
{
  return properties.Length ();
}

const char* celPcProperties::GetPropertyName (int index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  return p->propName;
}

void celPcProperties::Dump ()
{
}

//---------------------------------------------------------------------------

