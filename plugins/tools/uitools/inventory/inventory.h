/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein
  
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

#ifndef __CEL_UITOOLS_INVENTORY_IMP__
#define __CEL_UITOOLS_INVENTORY_IMP__

#include <CEGUI.h>
#include "csutil/util.h"
#include "csutil/stringarray.h"
#include "ivaria/icegui.h"

#include "physicallayer/pl.h"
#include "propclass/inv.h"
#include "tools/uitools/inventory.h"

struct iEngine;

CS_PLUGIN_NAMESPACE_BEGIN(UIInventory)
{

class InvListener;

class DefaultInfo : public scfImplementation1<DefaultInfo,iUIInventoryInfo>
{
private:
  iCelPlLayer* pl;
public:
  DefaultInfo (iCelPlLayer* pl) : scfImplementationType (this), pl (pl) { }
  virtual ~DefaultInfo () { }

  virtual csRef<iString> GetName (iCelEntity* entity);
  virtual csRef<iString> GetName (iCelEntityTemplate* tpl, int count);

  virtual csRef<iString> GetDescription (iCelEntity* entity) { return 0; }
  virtual csRef<iString> GetDescription (iCelEntityTemplate* tpl, int count) { return 0; }

  virtual iMeshFactoryWrapper* GetMeshFactory (iCelEntity* entity) { return 0; }
  virtual iMeshFactoryWrapper* GetMeshFactory (iCelEntityTemplate* tpl, int count) { return 0; }

  virtual iTextureHandle* GetTexture (iCelEntity* entity) { return 0; }
  virtual iTextureHandle* GetTexture (iCelEntityTemplate* tpl, int count) { return 0; }
};


class celUIInventory : public scfImplementation2<celUIInventory,
  iUIInventory, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iCEGUI> cegui;
  csRef<iCelPlLayer> pl;
  csRef<iPcInventory> inventory;

  CEGUI::Window* window;
  csRef<InvListener> listener;

  csRef<iUIInventoryInfo> info;
  csRefArray<iUIInventorySelectionCallback> callbacks;

  void FireSelectionListeners (iCelEntity* entity);
  void FireSelectionListeners (iCelEntityTemplate* tpl);

  void UpdateLists (iPcInventory* inventory);
  bool OkButton (const CEGUI::EventArgs& e);
  bool CancelButton (const CEGUI::EventArgs& e);
  bool Select (const CEGUI::EventArgs& e);

  void Setup ();

public:
  celUIInventory (iBase* parent);
  virtual ~celUIInventory ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  void Refresh ();

  virtual void SetInfo (iUIInventoryInfo* info) { celUIInventory::info = info; }
  virtual iUIInventoryInfo* GetInfo () const { return info; }
  virtual void Open (const char* title, iPcInventory* inventory);
  virtual void Close ();
  virtual iPcInventory* GetInventory () const { return inventory; }

  virtual void AddSelectionListener (iUIInventorySelectionCallback* cb);
  virtual void RemoveSelectionListener (iUIInventorySelectionCallback* cb);

  virtual bool SetStyleOption (const char* name, const char* value) { return false; }
  virtual bool Bind (const char* eventname, const char* command, const char* args)
  {
    return false;
  }
};

}
CS_PLUGIN_NAMESPACE_END(UIInventory)

#endif // __CEL_UITOOLS_INVENTORY_IMP__

