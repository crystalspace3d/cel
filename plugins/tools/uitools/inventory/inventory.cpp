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

#include "cssysdef.h"
#include "cstool/initapp.h"
#include "csutil/objreg.h"
#include "csutil/weakref.h"
#include "csutil/event.h"
#include "csutil/cfgacc.h"
#include "csgeom/math3d.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "iutil/object.h"
#include "iutil/virtclk.h"
#include "ivideo/graph3d.h"
#include "ivaria/reporter.h"

#include "inventory.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"

SCF_IMPLEMENT_FACTORY (celUIInventory)

//--------------------------------------------------------------------------

class InvListener : public scfImplementation1<InvListener, iPcInventoryListener>
{
private:
  celUIInventory* ui;

public:
  InvListener (celUIInventory* ui) : scfImplementationType (this), ui (ui) { }
  virtual ~InvListener () { }

  virtual void AddChild (iPcInventory* inventory, iCelEntity* entity)
  {
    ui->Refresh ();
  }
  virtual void RemoveChild (iPcInventory* inventory, iCelEntity* entity)
  {
    ui->Refresh ();
  }
  virtual void AddChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount)
  {
    ui->Refresh ();
  }
  virtual void RemoveChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount)
  {
    ui->Refresh ();
  }
};

//--------------------------------------------------------------------------


celUIInventory::celUIInventory (iBase* parent)
  : scfImplementationType (this, parent)
{
  window = 0;
}

celUIInventory::~celUIInventory ()
{
}

bool celUIInventory::Initialize (iObjectRegistry* object_reg)
{
  celUIInventory::object_reg = object_reg;
  cegui = csQueryRegistry<iCEGUI> (object_reg);
  if (!cegui)
  {
    // @@@ Use reporter.
    printf ("Error locating cegui!\n"); fflush (stdout);
    return false;
  }

  return true;
}

void celUIInventory::Setup ()
{
  if (window) return;

  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();
  window = winMgr->getWindow ("UIInventory");

  CEGUI::Window* btn;

  btn = winMgr->getWindow("UIInventory/OkButton");
  btn->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&celUIInventory::OkButton, this));

  btn = winMgr->getWindow("UIInventory/CancelButton");
  btn->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&celUIInventory::CancelButton, this));

  btn = winMgr->getWindow("UIInventory/InventoryList");
  btn->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
    CEGUI::Event::Subscriber(&celUIInventory::Select, this));
}

void celUIInventory::Refresh ()
{
  UpdateLists (inventory);
}

void celUIInventory::UpdateLists (iPcInventory* inventory)
{
  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();

  CEGUI::Listbox* list = (CEGUI::Listbox*)winMgr->getWindow("UIInventory/InventoryList");

  list->resetList();
  listNames.Empty ();

  for (size_t i = 0 ; i < inventory->GetEntityCount () ; i++)
  {
    iCelEntity* ent = inventory->GetEntity (i);
    CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem (ent->GetName ());
    item->setTextColours(CEGUI::colour(0,0,0));
    item->setSelectionBrushImage("ice", "TextSelectionBrush");
    item->setSelectionColours(CEGUI::colour(0.5f,0.5f,1));
    list->addItem(item);
    listNames.Push (ent->GetName ());
  }

  for (size_t i = 0 ; i < inventory->GetEntityTemplateCount () ; i++)
  {
    iCelEntityTemplate* ent = inventory->GetEntityTemplate (i);
    int amount = inventory->GetEntityTemplateAmount (i);
    csString itemString;
    itemString.Format ("%s (%d)", ent->GetName (), amount);
    CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem (
        (const char*)itemString);
    item->setTextColours(CEGUI::colour(0,0,0));
    item->setSelectionBrushImage("ice", "TextSelectionBrush");
    item->setSelectionColours(CEGUI::colour(0.5f,0.5f,1));
    list->addItem(item);
    listNames.Push (ent->GetName ());
  }
}

bool celUIInventory::OkButton (const CEGUI::EventArgs& e)
{
  window->hide();
  if (inventory && listener)
    inventory->RemoveInventoryListener (listener);
  inventory = 0;
  return true;
}

bool celUIInventory::CancelButton (const CEGUI::EventArgs& e)
{
  window->hide();
  if (inventory && listener)
    inventory->RemoveInventoryListener (listener);
  inventory = 0;
  return true;
}

bool celUIInventory::Select (const CEGUI::EventArgs& e)
{
  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();

  CEGUI::Listbox* list = (CEGUI::Listbox*) winMgr->getWindow("UIInventory/InventoryList");
  CEGUI::ListboxItem* item = list->getFirstSelectedItem();
  if (!item) return true;

  CEGUI::String text = item->getText();
  if (text.empty()) return true;

  size_t itemIdx = list->getItemIndex(item);
  csString n = listNames[itemIdx];
  printf ("Selected '%s'\n", n.GetData ());

  size_t idx = inventory->FindEntity (n);
  if (idx != csArrayItemNotFound)
  {
    FireSelectionListeners (inventory->GetEntity (idx));
    return true;
  }
  idx = inventory->FindEntityTemplate (n);
  if (idx != csArrayItemNotFound)
  {
    FireSelectionListeners (inventory->GetEntityTemplate (idx));
    return true;
  }

  return true;
}

void celUIInventory::AddSelectionListener (iUIInventorySelectionCallback* cb)
{
  callbacks.Push (cb);
}

void celUIInventory::RemoveSelectionListener (iUIInventorySelectionCallback* cb)
{
  callbacks.Delete (cb);
}

void celUIInventory::FireSelectionListeners (iCelEntity* entity)
{
  for (size_t i = 0 ; i < callbacks.GetSize () ; i++)
    callbacks[i]->SelectEntity (entity);
}

void celUIInventory::FireSelectionListeners (iCelEntityTemplate* tpl)
{
  for (size_t i = 0 ; i < callbacks.GetSize () ; i++)
    callbacks[i]->SelectTemplate (tpl);
}

void celUIInventory::Open (const char* title, iPcInventory* inventory)
{
  Setup ();

  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();
  CEGUI::Window* btn = winMgr->getWindow("UIInventory/Name");
  btn->setProperty("Text", title);

  celUIInventory::inventory = inventory;

  if (!listener)
    listener.AttachNew (new InvListener (this));
  inventory->RemoveInventoryListener (listener);
  inventory->AddInventoryListener (listener);

  UpdateLists (inventory);
  window->show ();
}

void celUIInventory::Close ()
{
  Setup ();
  window->hide();
  if (inventory && listener)
    inventory->RemoveInventoryListener (listener);
  inventory = 0;
}

//---------------------------------------------------------------------------

