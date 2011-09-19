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

#include "inventory2.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"

SCF_IMPLEMENT_FACTORY (celUIInventory2)

//--------------------------------------------------------------------------

class InvListener : public scfImplementation1<InvListener, iPcInventoryListener>
{
private:
  celUIInventory2* ui;

public:
  InvListener (celUIInventory2* ui) : scfImplementationType (this), ui (ui) { }
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

celUIInventory2::celUIInventory2 (iBase* parent)
  : scfImplementationType (this, parent)
{
  window = 0;
}

celUIInventory2::~celUIInventory2 ()
{
}

bool celUIInventory2::Initialize (iObjectRegistry* object_reg)
{
  celUIInventory2::object_reg = object_reg;
  cegui = csQueryRegistry<iCEGUI> (object_reg);
  if (!cegui)
  {
    // @@@ Use reporter.
    printf ("Error locating cegui!\n"); fflush (stdout);
    return false;
  }

  return true;
}

void celUIInventory2::Setup ()
{
  if (window) return;

  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();
  window = winMgr->getWindow ("UIInventory2");

  CEGUI::Window* btn;

  btn = winMgr->getWindow("UIInventory2/OkButton");
  btn->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&celUIInventory2::OkButton, this));

  btn = winMgr->getWindow("UIInventory2/CancelButton");
  btn->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&celUIInventory2::CancelButton, this));

  btn = winMgr->getWindow("UIInventory2/LeftInventoryList");
  btn->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
    CEGUI::Event::Subscriber(&celUIInventory2::SelectLeft, this));

  btn = winMgr->getWindow("UIInventory2/RightInventoryList");
  btn->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,
    CEGUI::Event::Subscriber(&celUIInventory2::SelectRight, this));
}

void celUIInventory2::FillList (CEGUI::Listbox* list, iPcInventory* inventory,
    csStringArray& names)
{
  list->resetList();
  names.Empty ();

  for (size_t i = 0 ; i < inventory->GetEntityCount () ; i++)
  {
    iCelEntity* ent = inventory->GetEntity (i);
    CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem (ent->GetName ());
    item->setTextColours(CEGUI::colour(0,0,0));
    item->setSelectionBrushImage("ice", "TextSelectionBrush");
    item->setSelectionColours(CEGUI::colour(0.5f,0.5f,1));
    list->addItem(item);
    names.Push (ent->GetName ());
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
    names.Push (ent->GetName ());
  }
}

void celUIInventory2::Refresh ()
{
  UpdateLists ();
}

void celUIInventory2::UpdateLists ()
{
  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();

  CEGUI::Listbox* listLeft = (CEGUI::Listbox*)winMgr->getWindow("UIInventory2/LeftInventoryList");
  CEGUI::Listbox* listRight = (CEGUI::Listbox*)winMgr->getWindow("UIInventory2/RightInventoryList");

  FillList (listLeft, inventoryLeft, leftListNames);
  FillList (listRight, inventoryRight, rightListNames);
}

bool celUIInventory2::OkButton (const CEGUI::EventArgs& e)
{
  window->hide();
  inventoryLeft = 0;
  inventoryRight = 0;
  return true;
}

bool celUIInventory2::CancelButton (const CEGUI::EventArgs& e)
{
  window->hide();
  inventoryLeft = 0;
  inventoryRight = 0;
  return true;
}

bool celUIInventory2::Select (CEGUI::Listbox* list, iPcInventory* inventory,
    const csStringArray& names, bool left)
{
  CEGUI::ListboxItem* item = list->getFirstSelectedItem();
  if (!item) return true;

  CEGUI::String text = item->getText();
  if (text.empty()) return true;

  size_t itemIdx = list->getItemIndex(item);
  csString n = names[itemIdx];
  printf ("Selected '%s'\n", n.GetData ());

  size_t idx = inventory->FindEntity (n);
  if (idx != csArrayItemNotFound)
  {
    FireSelectionListeners (inventory->GetEntity (idx), left);
    return true;
  }
  idx = inventory->FindEntityTemplate (n);
  if (idx != csArrayItemNotFound)
  {
    FireSelectionListeners (inventory->GetEntityTemplate (idx), left);
    return true;
  }

  return true;
}

bool celUIInventory2::SelectLeft (const CEGUI::EventArgs& e)
{
  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();

  CEGUI::Listbox* list = (CEGUI::Listbox*) winMgr->getWindow("UIInventory2/LeftInventoryList");
  return Select (list, inventoryLeft, leftListNames, true);
}

bool celUIInventory2::SelectRight (const CEGUI::EventArgs& e)
{
  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();

  CEGUI::Listbox* list = (CEGUI::Listbox*) winMgr->getWindow("UIInventory2/RightInventoryList");
  return Select (list, inventoryRight, rightListNames, false);
}

void celUIInventory2::AddSelectionListener (iUIInventory2SelectionCallback* cb)
{
  callbacks.Push (cb);
}

void celUIInventory2::RemoveSelectionListener (iUIInventory2SelectionCallback* cb)
{
  callbacks.Delete (cb);
}

void celUIInventory2::FireSelectionListeners (iCelEntity* entity, bool left)
{
  for (size_t i = 0 ; i < callbacks.GetSize () ; i++)
    callbacks[i]->SelectEntity (entity, left);
}

void celUIInventory2::FireSelectionListeners (iCelEntityTemplate* tpl, bool left)
{
  for (size_t i = 0 ; i < callbacks.GetSize () ; i++)
    callbacks[i]->SelectTemplate (tpl, left);
}

void celUIInventory2::Open (const char* title, iPcInventory* inventoryLeft,
    iPcInventory* inventoryRight)
{
  Setup ();

  CEGUI::WindowManager* winMgr = cegui->GetWindowManagerPtr ();
  CEGUI::Window* btn = winMgr->getWindow("UIInventory2/Name");
  btn->setProperty("Text", title);

  celUIInventory2::inventoryLeft = inventoryLeft;
  celUIInventory2::inventoryRight = inventoryRight;

  csRef<InvListener> listener;
  listener.AttachNew (new InvListener (this));
  inventoryLeft->AddInventoryListener (listener);
  inventoryRight->AddInventoryListener (listener);

  UpdateLists ();
  window->show ();
}

void celUIInventory2::Close ()
{
  Setup ();
  window->hide();
  inventoryLeft = 0;
  inventoryRight = 0;
}

//---------------------------------------------------------------------------

