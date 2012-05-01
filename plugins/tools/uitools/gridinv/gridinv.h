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
  csString backgroundImage[2];
  csRef<iTextureHandle> backgroundTexture[2];
  int bgred[2], bggreen[2], bgblue[2], bgalpha[2];
  bool rotateHiMesh;

  // Text parameters.
  csString fontName;
  int fontSize;
  csRef<iFont> font;

  InvStyle ();

  bool SetStyleOption (celUIGridInventory* inv, const char* name, const char* value);
};

struct GridEntry
{
  int x, y;
  csString text;
  csRef<iTextureHandle> handle[2];
  csReversibleTransform trans;
  csReversibleTransform camtrans;
  iMeshFactoryWrapper* factory;
  csRef<iMeshWrapper> mesh;
  iCelEntity* entity;
  iCelEntityTemplate* tpl;

  GridEntry () : entity (0), tpl (0) { }

  void SetupEntry (celUIGridInventory* inv,
      int x, int y, const char* text, int amount,
      iMeshFactoryWrapper* factory, int hi);
  void UpdateEntry (celUIGridInventory* inv, int hi);
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

struct Binding
{
  csString eventName;
  csEventID type;
  uint device;
  int numeric;
  utf32_char key, cooked;
  uint32 mods;

  csString command;
  bool close;

  bool Match (iEventNameRegistry* name_reg, iObjectRegistry* object_reg,
      iEvent& ev) const;
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
  csRef<iMouseDriver> mouse;
  csRef<iVirtualClock> vc;

  InvStyle style;
  csArray<GridEntry> grid;

  csRef<InvListener> listener;
  csArray<Binding> bindings;

  csRef<iUIInventoryInfo> info;
  csRefArray<iUIInventorySelectionCallback> callbacks;

  void FireSelectionListeners (iCelEntity* entity, const char* command);
  void FireSelectionListeners (iCelEntityTemplate* tpl, const char* command);

  void UpdateLists (iPcInventory* inventory);

  void Setup ();

  /// Activate the event handler.
  void Activate ();
 
  /// Deactivate the event handler.
  void Deactivate ();

  /// Find the grid entry under the mouse.
  GridEntry* FindGridEntry ();

public:
  celUIGridInventory (iBase* parent);
  virtual ~celUIGridInventory ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  bool HandleEvent (iEvent& ev);
  bool HandlePreEvent (iEvent& ev);

  iObjectRegistry* GetObjectRegistry () const { return object_reg; }
  iEngine* GetEngine () const { return engine; }
  iGraphics3D* GetG3D () const { return g3d; }
  iEventNameRegistry* GetNameReg () const { return name_reg; }
  iVirtualClock* GetVC () const { return vc; }
  const InvStyle& GetStyle () const { return style; }

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
  virtual bool Bind (const char* eventname, const char* command, bool close);

  // --------------------------------------------------------------------

  class PreEventHandler : public scfImplementation1<PreEventHandler, iEventHandler>
  {
  private:
    // This is a weak ref so that we can ignore the events
    // that occur when our parent has been deleted.
    csWeakRef<celUIGridInventory> parent;

  public:
    PreEventHandler (celUIGridInventory* parent) : scfImplementationType (this)
    {
      PreEventHandler::parent = parent;
    }
    virtual ~PreEventHandler() { }

    virtual bool HandleEvent (iEvent& ev)
    {
      if (parent)
        return parent->HandlePreEvent (ev);
      else
        return false;
    }
    CS_EVENTHANDLER_PHASE_LOGIC("cel.tools.inventory.grid.pre")
  } *scfiPreEventHandler;

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
    virtual ~EventHandler() { }

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

