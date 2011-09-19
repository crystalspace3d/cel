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

#ifndef __CEL_UITOOLS_INVENTORY2_IMP__
#define __CEL_UITOOLS_INVENTORY2_IMP__

#include <CEGUI.h>
#include "csutil/util.h"
#include "ivaria/icegui.h"

#include "propclass/inv.h"
#include "tools/uitools/inventory2.h"

struct iEngine;

class celUIInventory2 : public scfImplementation2<celUIInventory2,
  iUIInventory2, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iCEGUI> cegui;
  csRef<iPcInventory> inventoryLeft;
  csRef<iPcInventory> inventoryRight;

  CEGUI::Window* window;

  csRefArray<iUIInventory2SelectionCallback> callbacks;

  void FireSelectionListeners (iCelEntity* entity, bool left);
  void FireSelectionListeners (iCelEntityTemplate* tpl, bool left);

  void FillList (CEGUI::Listbox* list, iPcInventory* inventory);
  void UpdateLists ();
  bool OkButton (const CEGUI::EventArgs& e);
  bool CancelButton (const CEGUI::EventArgs& e);
  bool Select (CEGUI::Listbox* list, iPcInventory* inventory, bool left);
  bool SelectLeft (const CEGUI::EventArgs& e);
  bool SelectRight (const CEGUI::EventArgs& e);

  void Setup ();

public:
  celUIInventory2 (iBase* parent);
  virtual ~celUIInventory2 ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  virtual void Open (const char* title, iPcInventory* inventoryLeft,
      iPcInventory* inventoryRight);
  virtual void Close ();

  virtual void AddSelectionListener (iUIInventory2SelectionCallback* cb);
  virtual void RemoveSelectionListener (iUIInventory2SelectionCallback* cb);
};

#endif // __CEL_UITOOLS_INVENTORY2_IMP__

