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

#ifndef __CEL_UITOOLS_GRIDINV_IMP__
#define __CEL_UITOOLS_GRIDINV_IMP__

#include "csutil/util.h"
#include "csutil/eventhandlers.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "csutil/stringarray.h"

#include "physicallayer/pl.h"
#include "propclass/inv.h"
#include "tools/uitools/inventory.h"

struct iEngine;
class InvListener;
class celUIGridInventory;

struct InvStyle
{
  // Dimension style parameters.
  int buttonw, buttonh;
  int marginhor, marginver;

  // Button appearance parameters.
  csString backgroundImage;
  csRef<iTextureHandle> backgroundTexture;
  int bgred, bggreen, bgblue, bgalpha;

  // Control parameters.
  utf32_char stopKey;

  InvStyle ();

  bool SetStyleOption (celUIGridInventory* inv, const char* name, const char* value);
};

struct GridEntry
{
  int x, y;
  csString text;
  csRef<iTextureHandle> handle;

  void SetupEntry (const InvStyle& style, iObjectRegistry* object_reg,
      iEngine* engine, iGraphics3D* g3d, iFont* font,
      int x, int y, const char* text, int amount,
      iMeshFactoryWrapper* factory);
};

class DefaultInfo : public scfImplementation1<DefaultInfo,iUIInventoryInfo>
{
private:
  iCelPlLayer* pl;
  iEngine* engine;

public:
  DefaultInfo (iCelPlLayer* pl, iEngine* engine) : scfImplementationType (this),
  	pl (pl), engine (engine) { }
  virtual ~DefaultInfo () { }

  virtual csRef<iString> GetName (iCelEntity* entity);
  virtual csRef<iString> GetName (iCelEntityTemplate* tpl, int count);

  virtual csRef<iString> GetDescription (iCelEntity* entity) { return 0; }
  virtual csRef<iString> GetDescription (iCelEntityTemplate* tpl, int count) { return 0; }

  virtual iMeshFactoryWrapper* GetMeshFactory (iCelEntity* entity);
  virtual iMeshFactoryWrapper* GetMeshFactory (iCelEntityTemplate* tpl, int count);

  virtual iTextureHandle* GetTexture (iCelEntity* entity) { return 0; }
  virtual iTextureHandle* GetTexture (iCelEntityTemplate* tpl, int count) { return 0; }
};


class celUIGridInventory : public scfImplementation2<celUIGridInventory,
  iUIInventory, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iEventNameRegistry> name_reg;
  csRef<iCelPlLayer> pl;
  csRef<iEngine> engine;
  csRef<iGraphics3D> g3d;
  csRef<iPcInventory> inventory;

  InvStyle style;
  csArray<GridEntry> grid;
  csRef<iFont> font;

  csRef<InvListener> listener;

  csRef<iUIInventoryInfo> info;
  csRefArray<iUIInventorySelectionCallback> callbacks;

  void FireSelectionListeners (iCelEntity* entity);
  void FireSelectionListeners (iCelEntityTemplate* tpl);

  void UpdateLists (iPcInventory* inventory);
  bool OkButton ();
  bool CancelButton ();
  bool Select ();

  void Setup ();

  /// Activate the event handler.
  void Activate ();
 
  /// Deactivate the event handler.
  void Deactivate ();

public:
  celUIGridInventory (iBase* parent);
  virtual ~celUIGridInventory ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  bool HandleEvent (iEvent& ev);

  iEventNameRegistry* GetNameReg () const { return name_reg; }

  void Refresh ();

  virtual void SetInfo (iUIInventoryInfo* info) { celUIGridInventory::info = info; }
  virtual iUIInventoryInfo* GetInfo () const { return info; }
  virtual void Open (const char* title, iPcInventory* inventory);
  virtual void Close ();
  virtual iPcInventory* GetInventory () const { return inventory; }

  virtual void AddSelectionListener (iUIInventorySelectionCallback* cb);
  virtual void RemoveSelectionListener (iUIInventorySelectionCallback* cb);

  virtual bool SetStyleOption (const char* name, const char* value)
  {
    return style.SetStyleOption (this, name, value);
  }

  class EventHandler : public scfImplementation1<EventHandler, iEventHandler>
  {
  private:
    // This is a weak ref so that we can ignore the events
    // that occur when our parent has been deleted.
    csWeakRef<celUIGridInventory> parent;

  public:
    EventHandler (celUIGridInventory* parent) : scfImplementationType (this)
    {
      EventHandler::parent = parent;
    }
    virtual ~EventHandler()
    {
    }

    virtual bool HandleEvent (iEvent& ev)
    {
      if (parent)
        return parent->HandleEvent (ev);
      else
        return false;
    }
    CS_EVENTHANDLER_PHASE_2D("cel.tools.inventory.grid")
  } *scfiEventHandler;

};

#endif // __CEL_UITOOLS_GRIDINV_IMP__

