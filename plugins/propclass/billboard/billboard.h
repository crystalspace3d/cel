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

#ifndef __CEL_PF_BILLFACT__
#define __CEL_PF_BILLFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/parray.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/billboard.h"

struct iCelEntity;
struct iObjectRegistry;
struct iVirtualClock;
struct iBillboardManager;

/**
 * Factory for billboards.
 */
CEL_DECLARE_FACTORY(Billboard)

/**
 * This is a billboard property class.
 */
class celPcBillboard : public scfImplementationExt1<
	celPcBillboard, celPcCommon, iPcBillboard>
{
private:
  char* billboard_name;
  iBillboard* billboard;
  csRef<iBillboardManager> billboard_mgr;
  bool events_enabled;
  static csStringID id_x, id_y, id_button;
  celGenericParameterBlock* params;

  // For PerformAction.
  static csStringID action_drawmesh;
  static csStringID id_materialname;
  static csStringID id_factory;
  static csStringID id_distance;
  static csStringID id_angle;
  static csStringID id_rotate;

  float font_size;
  float default_font_size;

  enum propids
  {
    propid_billboardname = 0,
    propid_materialname,
    propid_materialnamefast,
    propid_clickable,
    propid_movable,
    propid_visible,
    propid_restack,
    propid_sendmove,
    propid_color,
    propid_width,
    propid_height,
    propid_widthpct,
    propid_heightpct,
    propid_x,
    propid_y,
    propid_uv_topleft,
    propid_uv_botright,
    propid_layer,
    propid_text,
    propid_text_offset,
    propid_text_fg_color,
    propid_text_bg_color,
    propid_text_font_size,
    propid_text_font,
    propid_text_default_fg_color,
    propid_text_default_bg_color,
    propid_text_default_font_size,
    propid_text_default_font
  };

  static Property* properties;
  static size_t propertycount;

  static void UpdateProperties (iObjectRegistry* object_reg);

public:
  celPcBillboard (iObjectRegistry* object_reg);
  virtual ~celPcBillboard ();

  virtual void SetBillboardName (const char* name);
  virtual const char* GetBillboardName () { return billboard_name; }
  virtual iBillboard* GetBillboard ();
  virtual void EnableEvents (bool e);
  virtual bool AreEventsEnabled () const { return events_enabled; }

  virtual const char* GetName () const { return "pcbillboard"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  // Override SetProperty from celPcCommon in order to provide support
  // for the materialname property.
  virtual bool SetProperty (csStringID, const char*);
  virtual bool SetProperty (csStringID, bool);
  virtual bool SetProperty (csStringID, long);
  virtual bool SetProperty (csStringID, float);
  virtual bool SetProperty (csStringID, const csColor&);
  virtual bool SetProperty (csStringID, const csVector2&);
  virtual const char* GetPropertyString (csStringID);
  virtual bool GetPropertyBool (csStringID);
  virtual bool GetPropertyColor (csStringID, csColor&);
  virtual bool GetPropertyVector (csStringID, csVector2&);
  virtual long GetPropertyLong (csStringID);
  virtual float GetPropertyFloat (csStringID);

  // For iBillboardEventHandler:
  void Select (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey);
  void MouseMove (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey);
  void MouseMoveAway (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey);
  void Unselect (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey);
  void DoubleClick (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey);

  // Not embedded to avoid problems with circular refs!
  struct BillboardEventHandler : public scfImplementation1<
  	BillboardEventHandler, iBillboardEventHandler>
  {
    celPcBillboard* parent;
    BillboardEventHandler (celPcBillboard* parent) :
    	scfImplementationType (this)
    {
      BillboardEventHandler::parent = parent;
    }
    virtual ~BillboardEventHandler ()
    {
    }
    virtual void Select (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
    {
      parent->Select (billboard, mouse_button, mousex, mousey);
    }
    virtual void MouseMove (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
    {
      parent->MouseMove (billboard, mouse_button, mousex, mousey);
    }
    virtual void MouseMoveAway (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
    {
      parent->MouseMoveAway (billboard, mouse_button, mousex, mousey);
    }
    virtual void Unselect (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
    {
      parent->Unselect (billboard, mouse_button, mousex, mousey);
    }
    virtual void DoubleClick (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey)
    {
      parent->DoubleClick (billboard, mouse_button, mousex, mousey);
    }
  }* scfiBillboardEventHandler;
  friend struct BillboardEventHandler;
};

#endif // __CEL_PF_BILLFACT__

