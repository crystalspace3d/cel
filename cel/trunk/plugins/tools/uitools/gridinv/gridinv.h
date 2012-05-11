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

// Grid layout styles:
//   - Horizontal only with left/right scroll
//   - Vertical only with up/down scroll
//   - Grid with left/right scroll
//   - Grid with up/down scroll
//   - Horizontal 3D circular with left/right scroll
//   - Vertical 3D circular with up/down scroll
//   - 2D circular
//
// Hilighted item choices:
//   - Alternative image/text
//   - Rotating mesh
//   - Enlarged button
//
// Ways to select items:
//   - Mouse hovers over selected item
//   - Depending on grid layout the selected item is fixed depending on
//     how the layout is scrolled. Selection happens with a keypress then
//
// Ways to scroll:
//   - When mouse goes near border scroll automatically in that direction
//   - Using keyboard
//   - Using button
//   - Wheelmouse
//
// Size and spacing of buttons:
//   - Fixed button and margin dimensions
//   - Dimensions depend on desired amount of visible items
//   - Dimensions relative to screen size
//
// Scrolling options:
//   - Immediate
//   - Smooth movement
//   - Fading

struct TextStyle
{
  csString fontName;
  int fontSize;		// -1 if no text must be printed.
  csRef<iFont> font;
  int fg[2];
  int bg[2];
  // The following values define the position of the text. Both horizontally
  // and vertically. If the value is 0 the item is centered in that direction.
  // If the value is negative the item is left aligned with the given value
  // as the margin.
  // If the value is positive the item is right aligned with the given value
  // as the margin.
  int horPos;
  int verPos;

  TextStyle () : fontSize (10) { }

  bool SetStyleOption (const char* prefix, celUIGridInventory* inv,
    const char* name, const char* value);

  // Get the text position. Return true if no text should be printed.
  bool GetTextPos (const char* txt, int& x, int& y, int w, int h) const;
};

struct InvStyle
{
  // Dimension style parameters.
  int buttonw, buttonh;
  int marginhor, marginver;

  // Button appearance parameters.
  csString backgroundImage[2];
  csRef<iTextureHandle> backgroundTexture[2];
  int bg[2];			// Background color in case a solid box is used.
  bool rotateHiMesh;

  // Text parameters.
  TextStyle nameStyle;
  TextStyle amountStyle;

  InvStyle ();

  void Setup (iGraphics2D* g2d);
  bool SetStyleOption (celUIGridInventory* inv, const char* name, const char* value);
};

struct GridEntry
{
  int x, y;
  csString text;
  int amount;
  csRef<iTextureHandle> handle[2];
  csReversibleTransform trans;
  csReversibleTransform camtrans;
  iMeshFactoryWrapper* factory;
  csRef<iMeshWrapper> mesh;
  iCelEntity* entity;
  iCelEntityTemplate* tpl;
  uint8 alpha;

  GridEntry () : entity (0), tpl (0), alpha (0) { }

  void SetupEntry (celUIGridInventory* inv,
      const char* text, int amount,
      iMeshFactoryWrapper* factory, int hi);
  void UpdateEntry (celUIGridInventory* inv, int hi);
  void WriteText (celUIGridInventory* inv, int hi);
};

class Layouter
{
public:
  virtual void Layout () = 0;
  virtual void Scroll (int d, float time) = 0;
  virtual void UpdateScroll (float elapsed) = 0;
  virtual GridEntry* GetSelected () = 0;

  /// Get the real position of a grid entry. Returns false if not visible.
  virtual bool GetRealPosition (size_t i, int& x, int& y) = 0;
  virtual bool SetStyleOption (const char* name, const char* value) = 0;
};

class GridLayouter : public Layouter
{
private:
  celUIGridInventory* inv;

  bool verticalscroll;
  int horcount, vercount;
  bool horcountSet, vercountSet;

  // Total dimension of the inventory not including margins.
  int totalwidth, totalheight;
  // Real left/top margin.
  int leftmargin, topmargin;

  int ix, iy;

  int firstx, firsty;
  float scrollTime, maxScrollTime;
  int scrollDirection;

  void FirstSlot ();
  void NextSlot ();

public:
  GridLayouter (celUIGridInventory* inv, bool verticalscroll);
  virtual ~GridLayouter () { }
  virtual void Layout ();
  virtual void Scroll (int d, float time);
  virtual void UpdateScroll (float elapsed);
  virtual GridEntry* GetSelected ();
  virtual bool GetRealPosition (size_t i, int& x, int& y);
  virtual bool SetStyleOption (const char* name, const char* value);
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

#define COMMAND_CANCEL 1
#define COMMAND_SELECT 2
#define COMMAND_SELECT_KEEPOPEN 3
#define COMMAND_SCROLL_LEFT 4
#define COMMAND_SCROLL_RIGHT 5

struct Binding
{
  csString eventName;
  csEventID type;
  uint device;
  int numeric;
  utf32_char key, cooked;
  uint32 mods;

  int command;
  csString args;

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

  Layouter* layouter;
  InvStyle style;
  csArray<GridEntry> grid;

  csRef<InvListener> listener;
  csArray<Binding> bindings;

  csRef<iUIInventoryInfo> info;
  csRefArray<iUIInventorySelectionCallback> callbacks;

  void FireSelectionListeners (iCelEntity* entity, const char* command);
  void FireSelectionListeners (iCelEntityTemplate* tpl, const char* command);

  void UpdateLists (iPcInventory* inventory);

  void SetupItems ();

  /// Activate the event handler.
  void Activate ();
 
  /// Deactivate the event handler.
  void Deactivate ();

  /// Handle selecting an item.
  void DoSelect (const char* args, bool close);
  /// Handle scrolling.
  void DoScroll (int d);

public:
  celUIGridInventory (iBase* parent);
  virtual ~celUIGridInventory ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  bool HandleRenderEvent (iEvent& ev);
  bool HandleLogicEvent (iEvent& ev);
  bool HandleInputEvent (iEvent& ev);

  iObjectRegistry* GetObjectRegistry () const { return object_reg; }
  iEngine* GetEngine () const { return engine; }
  iGraphics3D* GetG3D () const { return g3d; }
  iGraphics2D* GetG2D () const { return g3d->GetDriver2D (); }
  iEventNameRegistry* GetNameReg () const { return name_reg; }
  iVirtualClock* GetVC () const { return vc; }
  iMouseDriver* GetMouseDriver () const { return mouse; }
  const InvStyle& GetStyle () const { return style; }
  const csArray<GridEntry>& GetGrid () const { return grid; }
  csArray<GridEntry>& GetGrid () { return grid; }

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
    if (style.SetStyleOption (this, name, value)) return true;
    return layouter->SetStyleOption (name, value);
  }
  virtual bool Bind (const char* eventname, const char* command, const char* args);

  // --------------------------------------------------------------------

  class InputEventHandler : public scfImplementation1<InputEventHandler, iEventHandler>
  {
  private:
    // This is a weak ref so that we can ignore the events
    // that occur when our parent has been deleted.
    csWeakRef<celUIGridInventory> parent;

  public:
    InputEventHandler (celUIGridInventory* parent) : scfImplementationType (this)
    {
      InputEventHandler::parent = parent;
    }
    virtual ~InputEventHandler() { }

    virtual bool HandleEvent (iEvent& ev)
    {
      if (parent)
        return parent->HandleInputEvent (ev);
      else
        return false;
    }
    //CS_EVENTHANDLER_PHASE_LOGIC("cel.tools.inventory.grid.pre")

    CS_EVENTHANDLER_NAMES("cel.tools.inventory.grid.input")

    virtual const csHandlerID * GenericPrec (csRef<iEventHandlerRegistry> &,
	csRef<iEventNameRegistry> &, csEventID) const
    {
      return 0;
    }

    /* Declare that we want to receive events *before* the input property class */
    virtual const csHandlerID * GenericSucc (csRef<iEventHandlerRegistry> &r1, 
      csRef<iEventNameRegistry> &r2, csEventID event) const 
    {
      static csHandlerID succConstraint[2];
    
      succConstraint[0] = r1->GetGenericID("cel.propclass.pcinput.standard");
      succConstraint[1] = CS_HANDLERLIST_END;
      return succConstraint;
    }

    CS_EVENTHANDLER_DEFAULT_INSTANCE_CONSTRAINTS
  };
  csRef<InputEventHandler> scfiInputHandler;

  // --------------------------------------------------------------------

  class LogicEventHandler : public scfImplementation1<LogicEventHandler, iEventHandler>
  {
  private:
    // This is a weak ref so that we can ignore the events
    // that occur when our parent has been deleted.
    csWeakRef<celUIGridInventory> parent;

  public:
    LogicEventHandler (celUIGridInventory* parent) : scfImplementationType (this)
    {
      LogicEventHandler::parent = parent;
    }
    virtual ~LogicEventHandler() { }

    virtual bool HandleEvent (iEvent& ev)
    {
      if (parent)
        return parent->HandleLogicEvent (ev);
      else
        return false;
    }
    CS_EVENTHANDLER_PHASE_LOGIC("cel.tools.inventory.grid.logic")
  };
  csRef<LogicEventHandler> scfiLogicHandler;

  // --------------------------------------------------------------------

  class RenderEventHandler : public scfImplementation1<RenderEventHandler, iEventHandler>
  {
  private:
    // This is a weak ref so that we can ignore the events
    // that occur when our parent has been deleted.
    csWeakRef<celUIGridInventory> parent;

  public:
    RenderEventHandler (celUIGridInventory* parent) : scfImplementationType (this)
    {
      RenderEventHandler::parent = parent;
    }
    virtual ~RenderEventHandler() { }

    virtual bool HandleEvent (iEvent& ev)
    {
      if (parent)
        return parent->HandleRenderEvent (ev);
      else
        return false;
    }
    CS_EVENTHANDLER_PHASE_2D("cel.tools.inventory.grid.render")
  };
  csRef<RenderEventHandler> scfiRenderHandler;

};

#endif // __CEL_UITOOLS_GRIDINV_IMP__

