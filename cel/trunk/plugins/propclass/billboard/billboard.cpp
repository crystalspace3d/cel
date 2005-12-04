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
#include "tools/billboard.h"
#include "plugins/propclass/billboard/billboard.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/debug.h"
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

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Billboard, "pcbillboard")

Property* celPcBillboard::properties = 0;
size_t celPcBillboard::propertycount = 0;

void celPcBillboard::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT( pl != 0 );

    propertycount = 27;
    properties = new Property[propertycount];

    properties[propid_billboardname].id = pl->FetchStringID (
    	"cel.property.name");
    properties[propid_billboardname].datatype = CEL_DATA_STRING;
    properties[propid_billboardname].readonly = false;
    properties[propid_billboardname].desc = "Name of billboard.";

    properties[propid_materialname].id = pl->FetchStringID (
    	"cel.property.materialname");
    properties[propid_materialname].datatype = CEL_DATA_STRING;
    properties[propid_materialname].readonly = false;
    properties[propid_materialname].desc = "Name of material.";

    properties[propid_materialnamefast].id = pl->FetchStringID (
    	"cel.property.materialnamefast");
    properties[propid_materialnamefast].datatype = CEL_DATA_STRING;
    properties[propid_materialnamefast].readonly = false;
    properties[propid_materialnamefast].desc = "Name of material (fast version).";

    properties[propid_clickable].id = pl->FetchStringID (
    	"cel.property.clickable");
    properties[propid_clickable].datatype = CEL_DATA_BOOL;
    properties[propid_clickable].readonly = false;
    properties[propid_clickable].desc = "Enable mouse events.";

    properties[propid_movable].id = pl->FetchStringID (
    	"cel.property.movable");
    properties[propid_movable].datatype = CEL_DATA_BOOL;
    properties[propid_movable].readonly = false;
    properties[propid_movable].desc = "Make movable.";

    properties[propid_visible].id = pl->FetchStringID (
    	"cel.property.visible");
    properties[propid_visible].datatype = CEL_DATA_BOOL;
    properties[propid_visible].readonly = false;
    properties[propid_visible].desc = "Make visible.";

    properties[propid_restack].id = pl->FetchStringID (
    	"cel.property.restack");
    properties[propid_restack].datatype = CEL_DATA_BOOL;
    properties[propid_restack].readonly = false;
    properties[propid_restack].desc = "Make restackable on selection.";

    properties[propid_color].id = pl->FetchStringID (
    	"cel.property.color");
    properties[propid_color].datatype = CEL_DATA_COLOR;
    properties[propid_color].readonly = false;
    properties[propid_color].desc = "Color of this billboard.";

    properties[propid_width].id = pl->FetchStringID (
    	"cel.property.width");
    properties[propid_width].datatype = CEL_DATA_LONG;
    properties[propid_width].readonly = false;
    properties[propid_width].desc = "Width in pixels.";

    properties[propid_height].id = pl->FetchStringID (
    	"cel.property.height");
    properties[propid_height].datatype = CEL_DATA_LONG;
    properties[propid_height].readonly = false;
    properties[propid_height].desc = "Height in pixels.";

    properties[propid_widthpct].id = pl->FetchStringID (
    	"cel.property.widthpct");
    properties[propid_widthpct].datatype = CEL_DATA_FLOAT;
    properties[propid_widthpct].readonly = false;
    properties[propid_widthpct].desc =
    	"Width in percentage relative to original texture (1==full size).";

    properties[propid_heightpct].id = pl->FetchStringID (
    	"cel.property.heightpct");
    properties[propid_heightpct].datatype = CEL_DATA_FLOAT;
    properties[propid_heightpct].readonly = false;
    properties[propid_heightpct].desc =
    	"Height in percentage relative to original texture (1==full size).";

    properties[propid_x].id = pl->FetchStringID (
    	"cel.property.x");
    properties[propid_x].datatype = CEL_DATA_LONG;
    properties[propid_x].readonly = false;
    properties[propid_x].desc = "X position of billboard.";

    properties[propid_y].id = pl->FetchStringID (
    	"cel.property.y");
    properties[propid_y].datatype = CEL_DATA_LONG;
    properties[propid_y].readonly = false;
    properties[propid_y].desc = "Y position of billboard.";

    properties[propid_uv_topleft].id = pl->FetchStringID (
    	"cel.property.uv_topleft");
    properties[propid_uv_topleft].datatype = CEL_DATA_VECTOR2;
    properties[propid_uv_topleft].readonly = false;
    properties[propid_uv_topleft].desc = "Top-left UV coordinate.";

    properties[propid_uv_botright].id = pl->FetchStringID (
    	"cel.property.uv_botright");
    properties[propid_uv_botright].datatype = CEL_DATA_VECTOR2;
    properties[propid_uv_botright].readonly = false;
    properties[propid_uv_botright].desc = "Bottom-right UV coordinate.";

    properties[propid_layer].id = pl->FetchStringID (
    	"cel.property.layer");
    properties[propid_layer].datatype = CEL_DATA_STRING;
    properties[propid_layer].readonly = false;
    properties[propid_layer].desc = "Layer for this billboard.";

    properties[propid_text].id = pl->FetchStringID (
	"cel.property.text");
    properties[propid_text].datatype = CEL_DATA_STRING;
    properties[propid_text].readonly = false;
    properties[propid_text].desc = "Text placed on the billboard.";

    properties[propid_text_offset].id = pl->FetchStringID (
	"cel.property.text_offset");
    properties[propid_text_offset].datatype = CEL_DATA_VECTOR2;
    properties[propid_text_offset].readonly = true;
    properties[propid_text_offset].desc = "Offset for the text in bb space.";

    properties[propid_text_fg_color].id = pl->FetchStringID (
	"cel.property.text_fg_color");
    properties[propid_text_fg_color].datatype = CEL_DATA_COLOR;
    properties[propid_text_fg_color].readonly = true;
    properties[propid_text_fg_color].desc = "Foreground text color";

    properties[propid_text_bg_color].id = pl->FetchStringID (
	"cel.property.text_bg_color");
    properties[propid_text_bg_color].datatype = CEL_DATA_COLOR;
    properties[propid_text_bg_color].readonly = true;
    properties[propid_text_bg_color].desc = "Background text color.";

    properties[propid_text_font_size].id = pl->FetchStringID (
	"cel.property.text_font_size");
    properties[propid_text_font_size].datatype = CEL_DATA_FLOAT;
    properties[propid_text_font_size].readonly = true;
    properties[propid_text_font_size].desc = "Text font size.";

    properties[propid_text_font].id = pl->FetchStringID (
	"cel.property.text_font");
    properties[propid_text_font].datatype = CEL_DATA_STRING;
    properties[propid_text_font].readonly = true;
    properties[propid_text_font].desc = "Text font.";

    properties[propid_text_default_fg_color].id = pl->FetchStringID (
	"cel.property.text_default_fg_color");
    properties[propid_text_default_fg_color].datatype = CEL_DATA_COLOR;
    properties[propid_text_default_fg_color].readonly = true;
    properties[propid_text_default_fg_color].desc =
      "Default foreground text color.";

    properties[propid_text_default_bg_color].id = pl->FetchStringID (
	"cel.property.text_default_bg_color");
    properties[propid_text_default_bg_color].datatype = CEL_DATA_COLOR;
    properties[propid_text_default_bg_color].readonly = true;
    properties[propid_text_default_bg_color].desc =
      "Default background text color.";

    properties[propid_text_default_font_size].id = pl->FetchStringID (
	"cel.property.text_default_font_size");
    properties[propid_text_default_font_size].datatype = CEL_DATA_FLOAT;
    properties[propid_text_default_font_size].readonly = true;
    properties[propid_text_default_font_size].desc = "Default text font size.";

    properties[propid_text_default_font].id = pl->FetchStringID (
	"cel.property.text_default_font");
    properties[propid_text_default_font].datatype = CEL_DATA_STRING;
    properties[propid_text_default_font].readonly = true;
    properties[propid_text_default_font].desc = "Default text font.";
  }
}

//---------------------------------------------------------------------------

csStringID celPcBillboard::id_x = csInvalidStringID;
csStringID celPcBillboard::id_y = csInvalidStringID;
csStringID celPcBillboard::id_button = csInvalidStringID;

celPcBillboard::celPcBillboard (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  billboard_name = csStrNew ("default");
  billboard = 0;
  events_enabled = false;

  scfiBillboardEventHandler = 0;

  UpdateProperties (object_reg);
  propdata = new void* [propertycount];

  font_size = 10.0f;
  default_font_size = 10.0f;

  props = properties;
  propcount = &propertycount;

  propdata[propid_billboardname] = &billboard_name;
  propdata[propid_materialname] = 0;
  propdata[propid_materialnamefast] = 0;
  propdata[propid_clickable] = 0;
  propdata[propid_movable] = 0;
  propdata[propid_visible] = 0;
  propdata[propid_restack] = 0;
  propdata[propid_color] = 0;
  propdata[propid_width] = 0;
  propdata[propid_height] = 0;
  propdata[propid_widthpct] = 0;
  propdata[propid_heightpct] = 0;
  propdata[propid_x] = 0;
  propdata[propid_y] = 0;
  propdata[propid_uv_topleft] = 0;
  propdata[propid_uv_botright] = 0;
  propdata[propid_layer] = 0;
  propdata[propid_text] = 0;
  propdata[propid_text_offset] = 0;
  propdata[propid_text_fg_color] = 0;
  propdata[propid_text_bg_color] = 0;
  propdata[propid_text_font_size] = 0;
  propdata[propid_text_font] = 0;
  propdata[propid_text_default_fg_color] = 0;
  propdata[propid_text_default_bg_color] = 0;
  propdata[propid_text_default_font_size] = 0;
  propdata[propid_text_default_font] = 0;

  if (id_x == csInvalidStringID)
  {
    id_x = pl->FetchStringID ("cel.parameter.x");
    id_y = pl->FetchStringID ("cel.parameter.y");
    id_button = pl->FetchStringID ("cel.parameter.button");
  }
  params = new celGenericParameterBlock (3);
  params->SetParameterDef (0, id_x, "x");
  params->SetParameterDef (1, id_y, "y");
  params->SetParameterDef (2, id_button, "button");
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
  delete params;
  delete scfiBillboardEventHandler;
}

bool celPcBillboard::SetProperty (csStringID propertyId, float b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_widthpct].id)
  {
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
  }
  else if (propertyId == properties[propid_heightpct].id)
  {
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
  }
  else if (propertyId == properties[propid_text_font_size].id)
  {
    font_size = b;
    return true;
  }
  else if (propertyId == properties[propid_text_default_font_size].id)
  {
    default_font_size = b;
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

float celPcBillboard::GetPropertyFloat (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_widthpct].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int iw, ih;
      billboard->GetImageSize (iw, ih);
      int w, h;
      billboard->GetSize (w, h);
      return float (w) / float (iw);
    }
    return 0;
  }
  else if (propertyId == properties[propid_heightpct].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int iw, ih;
      billboard->GetImageSize (iw, ih);
      int w, h;
      billboard->GetSize (w, h);
      return float (h) / float (ih);
    }
    return 0;
  }
  else
  {
    return celPcCommon::GetPropertyFloat (propertyId);
  }
}

bool celPcBillboard::SetProperty (csStringID propertyId, long b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_width].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int w, h;
      billboard->GetSize (w, h);
      billboard->SetSize (b, h);
    }
    return true;
  }
  else if (propertyId == properties[propid_height].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int w, h;
      billboard->GetSize (w, h);
      billboard->SetSize (w, b);
    }
    return true;
  }
  else if (propertyId == properties[propid_x].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int x, y;
      billboard->GetPosition (x, y);
      billboard->SetPosition (b, y);
    }
    return true;
  }
  else if (propertyId == properties[propid_y].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int x, y;
      billboard->GetPosition (x, y);
      billboard->SetPosition (x, b);
    }
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

long celPcBillboard::GetPropertyLong (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_width].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int w, h;
      billboard->GetSize (w, h);
      return w;
    }
    return 0;
  }
  else if (propertyId == properties[propid_height].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int w, h;
      billboard->GetSize (w, h);
      return h;
    }
    return 0;
  }
  else if (propertyId == properties[propid_x].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int x, y;
      billboard->GetPosition (x, y);
      return x;
    }
    return 0;
  }
  else if (propertyId == properties[propid_y].id)
  {
    GetBillboard ();
    if (billboard)
    {
      int x, y;
      billboard->GetPosition (x, y);
      return y;
    }
    return 0;
  }
  else
  {
    return celPcCommon::GetPropertyLong (propertyId);
  }
}

bool celPcBillboard::SetProperty (csStringID propertyId, bool b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_clickable].id)
  {
    EnableEvents (b);
    return true;
  }
  else if (propertyId == properties[propid_movable].id)
  {
    GetBillboard ();
    if (billboard) billboard->GetFlags ().SetBool (CEL_BILLBOARD_MOVABLE, b);
    return true;
  }
  else if (propertyId == properties[propid_visible].id)
  {
    GetBillboard ();
    if (billboard) billboard->GetFlags ().SetBool (CEL_BILLBOARD_VISIBLE, b);
    return true;
  }
  else if (propertyId == properties[propid_restack].id)
  {
    GetBillboard ();
    if (billboard) billboard->GetFlags ().SetBool (CEL_BILLBOARD_RESTACK, b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

bool celPcBillboard::GetPropertyBool (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_clickable].id)
  {
    return AreEventsEnabled ();
  }
  else if (propertyId == properties[propid_movable].id)
  {
    GetBillboard ();
    return billboard ?
    	billboard->GetFlags ().Check (CEL_BILLBOARD_MOVABLE) :
	false;
  }
  else if (propertyId == properties[propid_visible].id)
  {
    GetBillboard ();
    return billboard ?
    	billboard->GetFlags ().Check (CEL_BILLBOARD_VISIBLE) :
	false;
  }
  else if (propertyId == properties[propid_restack].id)
  {
    GetBillboard ();
    return billboard ?
    	billboard->GetFlags ().Check (CEL_BILLBOARD_RESTACK) :
	false;
  }
  else
  {
    return celPcCommon::GetPropertyBool (propertyId);
  }
}

bool celPcBillboard::SetProperty (csStringID propertyId, const char* s)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_materialname].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetMaterialName (s);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_materialnamefast].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetMaterialNameFast (s);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_layer].id)
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
  else if (propertyId == properties[propid_text].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetText (s);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_text_font].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetTextFont (s, font_size);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_text_default_font].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard_mgr->SetDefaultTextFont (s, default_font_size);
      return true;
    }
    return false;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, s);
  }
}

const char* celPcBillboard::GetPropertyString (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_materialname].id ||
	propertyId == properties[propid_materialnamefast].id)
  {
    GetBillboard ();
    if (billboard)
      return billboard->GetMaterialName ();
    return 0;
  }
  else if (propertyId == properties[propid_layer].id)
  {
    GetBillboard ();
    if (billboard)
      return billboard->GetLayer ()->GetName ();
    return 0;
  }
  else if (propertyId == properties[propid_text].id)
  {
    GetBillboard ();
    if (billboard)
      return billboard->GetText ();
    return 0;
  }
  else
  {
    return celPcCommon::GetPropertyString (propertyId);
  }
}

bool celPcBillboard::SetProperty (csStringID propertyId, const csVector2& c)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_uv_topleft].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetUVTopLeft (c);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_uv_botright].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetUVBottomRight (c);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_text_offset].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetTextOffset (int (c.x), int (c.y));
      return true;
    }
    return false;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, c);
  }
}

bool celPcBillboard::GetPropertyColor (csStringID propertyId, csVector2& c)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_uv_topleft].id)
  {
    GetBillboard ();
    if (billboard)
    {
      c = billboard->GetUVTopLeft ();
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_uv_botright].id)
  {
    GetBillboard ();
    if (billboard)
    {
      c = billboard->GetUVBottomRight ();
      return true;
    }
    return false;
  }
  else
  {
    return celPcCommon::GetPropertyVector (propertyId, c);
  }
}

bool celPcBillboard::SetProperty (csStringID propertyId, const csColor& c)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_color].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetColor (c);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_text_fg_color].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetTextFgColor (c);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_text_bg_color].id)
  {
    GetBillboard ();
    if (billboard)
    {
      billboard->SetTextBgColor (c);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_text_default_fg_color].id)
  {
    GetBillboard ();
    if (billboard_mgr)
    {
      billboard_mgr->SetDefaultTextFgColor (c);
      return true;
    }
    return false;
  }
  else if (propertyId == properties[propid_text_default_bg_color].id)
  {
    GetBillboard ();
    if (billboard_mgr)
    {
      billboard_mgr->SetDefaultTextBgColor (c);
      return true;
    }
    return false;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, c);
  }
}

bool celPcBillboard::GetPropertyColor (csStringID propertyId, csColor& c)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_color].id)
  {
    GetBillboard ();
    if (billboard)
    {
      c = billboard->GetColor ();
      return true;
    }
    return false;
  }
  else
  {
    return celPcCommon::GetPropertyColor (propertyId, c);
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
}

void celPcBillboard::MouseMove (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
{
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
}

void celPcBillboard::Unselect (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
{
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
}

void celPcBillboard::DoubleClick (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
{
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
}

#define BILLBOARD_SERIAL 2

csPtr<iCelDataBuffer> celPcBillboard::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (BILLBOARD_SERIAL);
  databuf->Add (billboard_name);
  if (billboard)
  {
    databuf->Add (billboard->GetMaterialName ());
    databuf->Add ((uint32)billboard->GetFlags ().Get ());
    databuf->Add (billboard->GetColor ());
    int x, y, w, h;
    billboard->GetPosition (x, y);
    billboard->GetSize (w, h);
    databuf->Add ((int32)x);
    databuf->Add ((int32)y);
    databuf->Add ((int32)w);
    databuf->Add ((int32)h);
    databuf->Add (billboard->GetLayer ()->GetName ());
  }
  databuf->Add (events_enabled);
  return csPtr<iCelDataBuffer> (databuf);
}


bool celPcBillboard::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != BILLBOARD_SERIAL) return false;

  delete[] billboard_name; billboard_name = 0;
  billboard_name = csStrNew (databuf->GetString ()->GetData ());

  GetBillboard ();
  if (billboard)
  {
    billboard->SetMaterialName (databuf->GetString ()->GetData ());
    billboard->GetFlags ().SetAll (databuf->GetUInt32 ());
    csColor col;
    databuf->GetColor (col);
    billboard->SetColor (col);

    int x = databuf->GetInt32 ();
    int y = databuf->GetInt32 ();
    int w = databuf->GetInt32 ();
    int h = databuf->GetInt32 ();
    billboard->SetPosition (x, y);
    billboard->SetSize (w, h);

    const char* layername = databuf->GetString ()->GetData ();
    iBillboardLayer* layer = billboard_mgr->FindBillboardLayer (layername);
    if (!layer)
      layer = billboard_mgr->CreateBillboardLayer (layername);
    billboard->SetLayer (layer);
  }

  EnableEvents (databuf->GetBool ());

  return true;
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
    billboard_mgr = CS_QUERY_REGISTRY (object_reg, iBillboardManager);
    if (!billboard_mgr)
    {
      csRef<iPluginManager> plugin_mgr = CS_QUERY_REGISTRY (object_reg,
      	iPluginManager);
      billboard_mgr = CS_LOAD_PLUGIN (plugin_mgr, "cel.manager.billboard",
      	iBillboardManager);
      if (!billboard_mgr)
      {
	csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.billboard",
	  "Can't find billboard manager plugin!");
        return 0;
      }
      if (!object_reg->Register (billboard_mgr, "iBillboardManager"))
      {
	csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.billboard",
	  "Couldn't register billboard manager plugin!");
        return 0;
      }
    }
  }
  billboard = billboard_mgr->CreateBillboard (billboard_name);
  if (!billboard)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.billboard",
	  "Couldn't create billboard '%s'!", billboard_name);
    return 0;
  }
  return billboard;
}


//---------------------------------------------------------------------------

