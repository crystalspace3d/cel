/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein

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
#include "cstool/initapp.h"
#include "tools/billboard.h"
#include "plugins/propclass/billboard/billboard.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "iutil/plugin.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (Billboard, "pc2d.billboard", "pcbillboard")

csStringID celPcBillboard::id_materialname = csInvalidStringID;
csStringID celPcBillboard::id_factory = csInvalidStringID;
csStringID celPcBillboard::id_distance = csInvalidStringID;
csStringID celPcBillboard::id_angle = csInvalidStringID;
csStringID celPcBillboard::id_rotate = csInvalidStringID;
csStringID celPcBillboard::id_x = csInvalidStringID;
csStringID celPcBillboard::id_y = csInvalidStringID;
csStringID celPcBillboard::id_button = csInvalidStringID;

PropertyHolder celPcBillboard::propinfo;

celPcBillboard::celPcBillboard (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  billboard_name = csStrNew ("default");
  billboard = 0;
  events_enabled = false;

  scfiBillboardEventHandler = 0;

  if (id_materialname == csInvalidStringID)
  {
    id_materialname = pl->FetchStringID ("materialname");
    id_factory = pl->FetchStringID ("factory");
    id_distance = pl->FetchStringID ("distance");
    id_angle = pl->FetchStringID ("angle");
    id_rotate = pl->FetchStringID ("rotate");
  }

  propholder = &propinfo;

  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.billboard.action.");
    AddAction (action_drawmesh, "DrawMesh");
    AddAction (action_gettextwidth, "GetTextWidth");
    AddAction (action_gettextheight, "GetTextHeight");
  }

  propinfo.SetCount (28);
  AddProperty (propid_billboardname, "name",
	CEL_DATA_STRING, false, "Name of billboard.", &billboard_name);
  AddProperty (propid_materialname, "materialname",
	CEL_DATA_STRING, false, "Name of material.", 0);
  AddProperty (propid_materialnamefast, "materialnamefast",
	CEL_DATA_STRING, false, "Name of material (fast version).", 0);
  AddProperty (propid_clickable, "clickable",
	CEL_DATA_BOOL, false, "Enable mouse events.", 0);
  AddProperty (propid_movable, "movable",
	CEL_DATA_BOOL, false, "Make movable.", 0);
  AddProperty (propid_visible, "visible",
	CEL_DATA_BOOL, false, "Make visible.", 0);
  AddProperty (propid_restack, "restack",
	CEL_DATA_BOOL, false, "Make restackable on selection.", 0);
  AddProperty (propid_sendmove, "sendmove",
	CEL_DATA_BOOL, false, "Send move and moveaway events.", 0);
  AddProperty (propid_color, "color",
	CEL_DATA_COLOR, false, "Color of this billboard.", 0);
  AddProperty (propid_width, "width",
	CEL_DATA_LONG, false, "Width in pixels.", 0);
  AddProperty (propid_height, "height",
	CEL_DATA_LONG, false, "Height in pixels.", 0);
  AddProperty (propid_widthpct, "widthpct",
	CEL_DATA_FLOAT, false,
	"Width in percentage relative to original texture (1==full size).", 0);
  AddProperty (propid_heightpct, "heightpct",
	CEL_DATA_FLOAT, false,
	"Height in percentage relative to original texture (1==full size).", 0);
  AddProperty (propid_x, "x",
	CEL_DATA_LONG, false, "X position of billboard.", 0);
  AddProperty (propid_y, "y",
	CEL_DATA_LONG, false, "Y position of billboard.", 0);
  AddProperty (propid_uv_topleft, "uv_topleft",
	CEL_DATA_VECTOR2, false, "Top-left UV coordinate.", 0);
  AddProperty (propid_uv_botright, "uv_botright",
	CEL_DATA_VECTOR2, false, "Bottom-right UV coordinate.", 0);
  AddProperty (propid_layer, "layer",
	CEL_DATA_STRING, false, "Layer for this billboard.", 0);
  AddProperty (propid_text, "text",
	CEL_DATA_STRING, false, "Text placed on the billboard.", 0);
  AddProperty (propid_text_offset, "text_offset",
	CEL_DATA_VECTOR2, false, "Offset for the text in bb space.", 0);
  AddProperty (propid_text_fg_color, "text_fg_color",
	CEL_DATA_COLOR, false, "Foreground text color", 0);
  AddProperty (propid_text_bg_color, "text_bg_color",
	CEL_DATA_COLOR, false, "Background text color.", 0);
  AddProperty (propid_text_font_size, "text_font_size",
	CEL_DATA_FLOAT, false, "Text font size.", 0);
  AddProperty (propid_text_font, "text_font",
	CEL_DATA_STRING, false, "Text font.", 0);
  AddProperty (propid_text_default_fg_color,
        "text_default_fg_color",
	CEL_DATA_COLOR, false, "Default foreground text color.", 0);
  AddProperty (propid_text_default_bg_color,
        "text_default_bg_color",
	CEL_DATA_COLOR, false, "Default background text color.", 0);
  AddProperty (propid_text_default_font_size,
        "text_default_font_size",
	CEL_DATA_FLOAT, false, "Default text font size.", 0);
  AddProperty (propid_text_default_font, "text_default_font",
	CEL_DATA_STRING, false, "Default text font.", 0);

  font_size = 10.0f;
  default_font_size = 10.0f;

  if (id_x == csInvalidStringID)
  {
    id_x = pl->FetchStringID ("x");
    id_y = pl->FetchStringID ("y");
    id_button = pl->FetchStringID ("button");
  }
  params.AttachNew (new celVariableParameterBlock (3));
  params->AddParameter (id_x);
  params->AddParameter (id_y);
  params->AddParameter (id_button);
}

celPcBillboard::~celPcBillboard ()
{
  if (billboard_mgr && billboard)
  {
    if (scfiBillboardEventHandler)
      billboard->RemoveEventHandler (scfiBillboardEventHandler);
    billboard_mgr->RemoveBillboard (billboard);
  }
  delete[] billboard_name;
  delete scfiBillboardEventHandler;
}

bool celPcBillboard::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_drawmesh:
      {
	csString materialname, factory;
	if (!Fetch (materialname, params, id_materialname)) return false;
	if (!Fetch (factory, params, id_factory)) return false;
	float distance, angle;
	if (!Fetch (distance, params, id_distance, true, -1.0f)) return false;
	if (!Fetch (angle, params, id_angle, true, 0.0f)) return false;
	csVector3 rotate;
	if (!Fetch (rotate, params, id_rotate, true, csVector3 (0, 0, 0))) return false;
        GetBillboard ();
        if (billboard)
        {
          return billboard->DrawMesh (materialname, factory, rotate, angle,
      	    distance);
        }
        return true;
      }
    case action_gettextwidth:
      {
	int w = 0, h = 0;
        GetBillboard ();
        if (billboard) billboard->GetTextDimensions (w, h);
        ret.Set ((uint32)w);
        return true;
      }
    case action_gettextheight:
      {
	int w = 0, h = 0;
        GetBillboard ();
        if (billboard) billboard->GetTextDimensions (w, h);
        ret.Set ((uint32)h);
        return true;
      }
  }
  return false;
}

bool celPcBillboard::SetPropertyIndexed (int idx, float b)
{
  switch (idx)
  {
    case propid_widthpct:
      GetBillboard ();
      if (billboard)
      {
        int iw, ih;
        billboard->GetImageSize (iw, ih);
        int w, h;
        billboard->GetSize (w, h);
        billboard->SetSize (int (iw * b), h);
      }
      return true;
    case propid_heightpct:
      GetBillboard ();
      if (billboard)
      {
        int iw, ih;
        billboard->GetImageSize (iw, ih);
        int w, h;
        billboard->GetSize (w, h);
        billboard->SetSize (w, int (ih * b));
      }
      return true;
    case propid_text_font_size:
      font_size = b;
      if (billboard)
      {
        const char *fontname = billboard->GetTextFont();
        if (fontname)
          billboard->SetTextFont (fontname, b);
      }
      return true;
    case propid_text_default_font_size:
      default_font_size = b;
      return true;
    default:
      return false;
  }
}

bool celPcBillboard::GetPropertyIndexed (int idx, float& f)
{
  GetBillboard ();
  if (!billboard) return false;

  switch (idx)
  {
    case propid_widthpct:
      {
        int iw, ih;
        billboard->GetImageSize (iw, ih);
        int w, h;
        billboard->GetSize (w, h);
        f = float (w) / float (iw);
      }
      return true;
    case propid_heightpct:
      {
        int iw, ih;
        billboard->GetImageSize (iw, ih);
        int w, h;
        billboard->GetSize (w, h);
        f = float (h) / float (ih);
      }
      return true;
    case propid_text_font_size:
      f = billboard->GetTextFontSize ();
      return true;
    case propid_text_default_font_size:
      f = default_font_size;
      return true;
    default:
      return false;
  }
}

bool celPcBillboard::SetPropertyIndexed (int idx, long b)
{
  switch (idx)
  {
    case propid_width:
      GetBillboard ();
      if (billboard)
      {
        int w, h;
        billboard->GetSize (w, h);
        billboard->SetSize (b, h);
      }
      return true;
    case propid_height:
      GetBillboard ();
      if (billboard)
      {
        int w, h;
        billboard->GetSize (w, h);
        billboard->SetSize (w, b);
      }
      return true;
    case propid_x:
      GetBillboard ();
      if (billboard)
      {
        int x, y;
        billboard->GetPosition (x, y);
        billboard->SetPosition (b, y);
      }
      return true;
    case propid_y:
      GetBillboard ();
      if (billboard)
      {
        int x, y;
        billboard->GetPosition (x, y);
        billboard->SetPosition (x, b);
      }
      return true;
    default:
      return false;
  }
}

bool celPcBillboard::GetPropertyIndexed (int idx, long& l)
{
  GetBillboard ();
  if (!billboard) return false;

  switch (idx)
  {
    case propid_width:
      {
        int w, h;
        billboard->GetSize (w, h);
	l = w;
      }
      return true;
    case propid_height:
      {
        int w, h;
        billboard->GetSize (w, h);
	l = h;
      }
      return true;
    case propid_x:
      {
        int x, y;
        billboard->GetPosition (x, y);
	l = x;
      }
      return true;
    case propid_y:
      {
        int x, y;
        billboard->GetPosition (x, y);
	l = y;
      }
      return true;
    default:
      return false;
  }
}

bool celPcBillboard::SetPropertyIndexed (int idx, bool b)
{
  switch (idx)
  {
    case propid_clickable:
      EnableEvents (b);
      return true;
    case propid_movable:
      GetBillboard ();
      if (billboard) billboard->GetFlags ().SetBool (CEL_BILLBOARD_MOVABLE, b);
      return true;
    case propid_visible:
      GetBillboard ();
      if (billboard) billboard->GetFlags ().SetBool (CEL_BILLBOARD_VISIBLE, b);
      return true;
    case propid_restack:
      GetBillboard ();
      if (billboard) billboard->GetFlags ().SetBool (CEL_BILLBOARD_RESTACK, b);
      return true;
    case propid_sendmove:
      GetBillboard ();
      if (billboard) billboard->GetFlags ().SetBool (CEL_BILLBOARD_SENDMOVE, b);
      return true;
    default:
      return false;
  }
}

bool celPcBillboard::GetPropertyIndexed (int idx, bool& b)
{
  GetBillboard ();
  if (!billboard) return false;
  switch (idx)
  {
    case propid_clickable:
      b = AreEventsEnabled ();
      return true;
    case propid_movable:
      b = billboard->GetFlags ().Check (CEL_BILLBOARD_MOVABLE);
      return true;
    case propid_visible:
      b = billboard->GetFlags ().Check (CEL_BILLBOARD_VISIBLE);
      return true;
    case propid_restack:
      b = billboard->GetFlags ().Check (CEL_BILLBOARD_RESTACK);
      return true;
    case propid_sendmove:
      b =  billboard->GetFlags ().Check (CEL_BILLBOARD_SENDMOVE);
      return true;
    default:
      return false;
  }
}

bool celPcBillboard::SetPropertyIndexed (int idx, const char* s)
{
  switch (idx)
  {
    case propid_materialname:
      {
        GetBillboard ();
        if (billboard)
        {
          billboard->SetMaterialName (s);
          return true;
        }
        return false;
      }
    case propid_materialnamefast:
      {
        GetBillboard ();
        if (billboard)
        {
          billboard->SetMaterialNameFast (s);
          return true;
        }
        return false;
      }
    case propid_layer:
      {
        GetBillboard ();
        if (billboard)
        {
          iBillboardLayer* layer = billboard_mgr->FindBillboardLayer (s);
          if (!layer)
            layer = billboard_mgr->CreateBillboardLayer (s);
          billboard->SetLayer (layer);
          return true;
        }
        return false;
      }
    case propid_text:
      {
        GetBillboard ();
        if (billboard)
        {
          billboard->SetText (s);
          return true;
        }
        return false;
      }
    case propid_text_font:
      {
        GetBillboard ();
        if (billboard)
        {
          billboard->SetTextFont (s, font_size);
          return true;
        }
        return false;
      }
    case propid_text_default_font:
      {
        GetBillboard ();
        if (billboard)
        {
          billboard_mgr->SetDefaultTextFont (s, default_font_size);
          return true;
        }
        return false;
      }
    default:
      return false;
  }
}

bool celPcBillboard::GetPropertyIndexed (int idx, const char*& s)
{
  GetBillboard ();
  if (!billboard) return false;
  switch (idx)
  {
    case propid_materialname:
    case propid_materialnamefast:
      s = billboard->GetMaterialName ();
      return true;
    case propid_layer:
      s = billboard->GetLayer ()->GetName ();
      return true;
    case propid_text:
      s = billboard->GetText ();
      return true;
    case propid_text_font:
      s = billboard->GetTextFont ();
      return true;
    case propid_text_default_font:
      s = billboard_mgr->GetDefaultTextFont ();
      return true;
    default:
      return false;
  }
}

bool celPcBillboard::SetPropertyIndexed (int idx, const csVector2& c)
{
  switch (idx)
  {
    case propid_uv_topleft:
      GetBillboard ();
      if (billboard)
      {
        billboard->SetUVTopLeft (c);
        return true;
      }
      return false;
    case propid_uv_botright:
      GetBillboard ();
      if (billboard)
      {
        billboard->SetUVBottomRight (c);
        return true;
      }
      return false;
    case propid_text_offset:
      GetBillboard ();
      if (billboard)
      {
        billboard->SetTextOffset (int (c.x), int (c.y));
        return true;
      }
      return false;
    default:
      return false;
  }
}

bool celPcBillboard::GetPropertyIndexed (int idx, csVector2& c)
{
  GetBillboard ();
  if (!billboard) return false;
  switch (idx)
  {
    case propid_uv_topleft:
      c = billboard->GetUVTopLeft ();
      return true;
    case propid_uv_botright:
      c = billboard->GetUVBottomRight ();
      return true;
    case propid_text_offset:
      c.x = billboard->GetTextOffsetDX ();
      c.y = billboard->GetTextOffsetDY ();
      return true;
    default:
      return false;
  }
}

bool celPcBillboard::SetPropertyIndexed (int idx, const csColor& c)
{
  switch (idx)
  {
    case propid_color:
      GetBillboard ();
      if (billboard)
      {
        billboard->SetColor (c);
        return true;
      }
      return false;
    case propid_text_fg_color:
      GetBillboard ();
      if (billboard)
      {
        billboard->SetTextFgColor (c);
        return true;
      }
      return false;
    case propid_text_bg_color:
      GetBillboard ();
      if (billboard)
      {
        billboard->SetTextBgColor (c);
        return true;
      }
      return false;
    case propid_text_default_fg_color:
      GetBillboard ();
      if (billboard_mgr)
      {
        billboard_mgr->SetDefaultTextFgColor (c);
        return true;
      }
      return false;
    case propid_text_default_bg_color:
      GetBillboard ();
      if (billboard_mgr)
      {
        billboard_mgr->SetDefaultTextBgColor (c);
        return true;
      }
      return false;
    default:
      return false;
  }
}

bool celPcBillboard::GetPropertyIndexed (int idx, csColor& c)
{
  GetBillboard ();
  if (!billboard) return false;
  switch (idx)
  {
    case propid_color:
      c = billboard->GetColor ();
      return true;
    case propid_text_fg_color:
      c = billboard->GetTextFgColor ();
      return true;
    case propid_text_bg_color:
      c = billboard->GetTextBgColor ();
      return true;
    case propid_text_default_fg_color:
      c = billboard_mgr->GetDefaultTextFgColor ();
      return true;
    case propid_text_default_bg_color:
      c = billboard_mgr->GetDefaultTextBgColor ();
      return true;
    default:
      return false;
  }
}

void celPcBillboard::EnableEvents (bool e)
{
  if (events_enabled == e) return;
  events_enabled = e;
  GetBillboard ();
  if (!billboard) return;
  if (events_enabled)
  {
    if (!scfiBillboardEventHandler)
      scfiBillboardEventHandler = new BillboardEventHandler (this);
    billboard->AddEventHandler (scfiBillboardEventHandler);
    billboard->GetFlags ().Set (CEL_BILLBOARD_CLICKABLE);
  }
  else
  {
    billboard->RemoveEventHandler (scfiBillboardEventHandler);
    billboard->GetFlags ().Reset (CEL_BILLBOARD_CLICKABLE);
  }
}

void celPcBillboard::Select (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
{
  csRef<iCelEntity> ref = entity;
  params->GetParameter (0).Set ((int32)mousex);
  params->GetParameter (1).Set ((int32)mousey);
  params->GetParameter (2).Set ((int32)mouse_button);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage ("pcbillboard_select", this, ret, params,
  	mouse_button, mousex, mousey);
  }
  if (!dispatcher_select)
  {
    dispatcher_select = entity->QueryMessageChannel ()
      ->CreateMessageDispatcher (this, pl->FetchStringID ("cel.billboard.select.down"));
    if (!dispatcher_select) return;
  }
  dispatcher_select->SendMessage (params);
}

void celPcBillboard::MouseMove (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
{
  csRef<iCelEntity> ref = entity;
  params->GetParameter (0).Set ((int32)mousex);
  params->GetParameter (1).Set ((int32)mousey);
  params->GetParameter (2).Set ((int32)mouse_button);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage ("pcbillboard_move", this, ret, params,
  	mouse_button, mousex, mousey);
  }
  if (!dispatcher_move)
  {
    dispatcher_move = entity->QueryMessageChannel ()->CreateMessageDispatcher (
	  this, pl->FetchStringID ("cel.billboard.select.move"));
    if (!dispatcher_move) return;
  }
  dispatcher_move->SendMessage (params);
}

void celPcBillboard::MouseMoveAway (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
{
  csRef<iCelEntity> ref = entity;
  params->GetParameter (0).Set ((int32)mousex);
  params->GetParameter (1).Set ((int32)mousey);
  params->GetParameter (2).Set ((int32)mouse_button);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage ("pcbillboard_moveaway", this, ret, params,
  	mouse_button, mousex, mousey);
  }
  if (!dispatcher_moveaway)
  {
    dispatcher_moveaway = entity->QueryMessageChannel ()
      ->CreateMessageDispatcher (this, pl->FetchStringID ("cel.billboard.select.moveaway"));
    if (!dispatcher_moveaway) return;
  }
  dispatcher_moveaway->SendMessage (params);
}

void celPcBillboard::Unselect (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
{
  csRef<iCelEntity> ref = entity;
  params->GetParameter (0).Set ((int32)mousex);
  params->GetParameter (1).Set ((int32)mousey);
  params->GetParameter (2).Set ((int32)mouse_button);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage ("pcbillboard_unselect", this, ret, params,
  	mouse_button, mousex, mousey);
  }
  if (!dispatcher_selectup)
  {
    dispatcher_selectup = entity->QueryMessageChannel ()
      ->CreateMessageDispatcher (this, pl->FetchStringID ("cel.billboard.select.select.up"));
    if (!dispatcher_selectup) return;
  }
  dispatcher_selectup->SendMessage (params);
}

void celPcBillboard::DoubleClick (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
{
  csRef<iCelEntity> ref = entity;
  params->GetParameter (0).Set ((int32)mousex);
  params->GetParameter (1).Set ((int32)mousey);
  params->GetParameter (2).Set ((int32)mouse_button);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage ("pcbillboard_doubleclick", this, ret, params,
  	mouse_button, mousex, mousey);
  }
  if (!dispatcher_selectdbl)
  {
    dispatcher_selectdbl = entity->QueryMessageChannel ()
      ->CreateMessageDispatcher (this, pl->FetchStringID ("cel.billboard.select.select.double"));
    if (!dispatcher_selectdbl) return;
  }
  dispatcher_selectdbl->SendMessage (params);
}

void celPcBillboard::SetBillboardName (const char* n)
{
  delete[] billboard_name;
  billboard_name = csStrNew (n);
}

iBillboard* celPcBillboard::GetBillboard ()
{
  if (billboard) return billboard;
  if (!billboard_mgr)
  {
    billboard_mgr = csQueryRegistryOrLoad<iBillboardManager> (object_reg,
    	"cel.manager.billboard");
    if (!billboard_mgr)
    {
      Error ("Couldn't load billboard manager plugin!");
      return 0;
    }
  }
  billboard = billboard_mgr->CreateBillboard (billboard_name);
  if (!billboard)
  {
    Error ("Couldn't create billboard '%s'!", billboard_name);
    return 0;
  }
  return billboard;
}


//---------------------------------------------------------------------------

