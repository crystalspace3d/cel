/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PF_TOOLFACT__
#define __CEL_PF_TOOLFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pf/tooltip.h"
#include "pf/timer.h"

struct iCelEntity;
struct iObjectRegistry;
struct iVirtualClock;

/**
 * Factory for tools.
 */
class celPfTools : public iCelPropertyClassFactory
{
private:
  iObjectRegistry* object_reg;

public:
  celPfTools (iBase* parent);
  virtual ~celPfTools ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pftools"; }
  virtual iCelPropertyClass* CreatePropertyClass (const char* type);
  virtual int GetTypeCount () const { return 2; }
  virtual const char* GetTypeName (int idx) const;

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPfTools);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

/**
 * This is a tooltip property class.
 */
class celPcTooltip : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iObjectRegistry* object_reg;
  bool visible;
  int x, y;
  char* text;
  int text_r, text_g, text_b;
  int bg_r, bg_g, bg_b;

public:
  celPcTooltip (iObjectRegistry* object_reg);
  virtual ~celPcTooltip ();

  bool HandleEvent (iEvent& ev);

  void SetText (const char* text);
  void Show (int x, int y);
  void Hide ();
  bool IsVisible () const { return visible; }
  void SetTextColor (int r, int g, int b)
  { text_r = r; text_g = g; text_b = b; }
  void SetBackgroundColor (int r, int g, int b)
  { bg_r = r; bg_g = g; bg_b = b; }

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pctooltip"; }
  virtual const char* GetFactoryName () const { return "pftools"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcTooltip : public iPcTooltip
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcTooltip);
    virtual void SetText (const char* text)
    {
      scfParent->SetText (text);
    }
    virtual void Show (int x, int y)
    {
      scfParent->Show (x, y);
    }
    virtual void Hide ()
    {
      scfParent->Hide ();
    }
    virtual bool IsVisible () const
    {
      return scfParent->IsVisible ();
    }
    virtual void SetTextColor (int r, int g, int b)
    {
      scfParent->SetTextColor (r, g, b);
    }
    virtual void SetBackgroundColor (int r, int g, int b)
    {
      scfParent->SetBackgroundColor (r, g, b);
    }
  } scfiPcTooltip;

  // Not an embedded interface to avoid circular references!!!
  class EventHandler : public iEventHandler
  {
  private:
    celPcTooltip* parent;

  public:
    EventHandler (celPcTooltip* parent)
    {
      SCF_CONSTRUCT_IBASE (NULL);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler () { }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
};

/**
 * This is a timer property class.
 */
class celPcTimer : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iObjectRegistry* object_reg;
  iVirtualClock* vc;
  bool enabled;
  csTicks wakeup, wakeup_todo;
  bool repeat;

public:
  celPcTimer (iObjectRegistry* object_reg);
  virtual ~celPcTimer ();

  bool HandleEvent (iEvent& ev);
  void WakeUp (csTicks t, bool repeat);
  void Clear ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pctimer"; }
  virtual const char* GetFactoryName () const { return "pftools"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcTimer : public iPcTimer
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcTimer);
    virtual void WakeUp (csTicks t, bool repeat)
    {
      scfParent->WakeUp (t, repeat);
    }
    virtual void Clear ()
    {
      scfParent->Clear ();
    }
  } scfiPcTimer;

  // Not an embedded interface to avoid circular references!!!
  class EventHandler : public iEventHandler
  {
  private:
    celPcTimer* parent;

  public:
    EventHandler (celPcTimer* parent)
    {
      SCF_CONSTRUCT_IBASE (NULL);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler () { }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
};

#endif // __CEL_PF_TOOLFACT__

