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
#include "plugins/propclass/common/stdpcimp.h"
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
class celPcBillboard : public celPcCommon, public iBillboardEventHandler
{
private:
  char* billboard_name;
  iBillboard* billboard;
  csRef<iBillboardManager> billboard_mgr;
  bool events_enabled;

  enum propids
  {
    propid_billboardname = 0,
    propid_materialname,
    propid_clickable,
    propid_movable,
    propid_visible
  };

  static Property* properties;
  static int propertycount;

  static void UpdateProperties (iObjectRegistry* object_reg);

public:
  celPcBillboard (iObjectRegistry* object_reg);
  virtual ~celPcBillboard ();

  void SetBillboardName (const char* name);
  iBillboard* GetBillboard ();
  void EnableEvents (bool e);
  bool AreEventsEnabled () const { return events_enabled; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcbillboard"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  // Override SetProperty from celPcCommon in order to provide support
  // for the materialname property.
  virtual bool SetProperty (csStringID, const char*);
  virtual bool SetProperty (csStringID, bool);
  virtual const char* GetPropertyString (csStringID);
  virtual bool GetPropertyBool (csStringID);

  // For iBillboardEventHandler:
  virtual void Select (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey);
  virtual void MouseMove (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey);
  virtual void Unselect (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey);
  virtual void DoubleClick (iBillboard* billboard, int mouse_button,
  	int mousex, int mousey);

  struct PcBillboard : public iPcBillboard
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcBillboard);
    virtual void SetBillboardName (const char* name)
    {
      scfParent->SetBillboardName (name);
    }
    virtual const char* GetBillboardName ()
    {
      return scfParent->billboard_name;
    }
    virtual iBillboard* GetBillboard ()
    {
      return scfParent->GetBillboard ();
    }
    virtual void EnableEvents (bool e)
    {
      scfParent->EnableEvents (e);
    }
    virtual bool AreEventsEnabled () const
    {
      return scfParent->AreEventsEnabled ();
    }
  } scfiPcBillboard;
  friend struct PcBillboard;
};

#endif // __CEL_PF_BILLFACT__

