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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_TOOLFACT__
#define __CEL_PF_TOOLFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pl/facttmpl.h"
#include "pf/common/stdpcimp.h"
#include "pf/tooltip.h"
#include "pf/timer.h"
#include "pf/prop.h"

struct iCelEntity;
struct iObjectRegistry;
struct iVirtualClock;

/**
 * Factory for tools.
 */
CEL_DECLARE_FACTORY(Tooltip)
CEL_DECLARE_FACTORY(Timer)
CEL_DECLARE_FACTORY(Properties)

/**
 * This is a tooltip property class.
 */
class celPcTooltip : public celPcCommon
{
private:
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

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pctooltip"; }
  virtual csPtr<iCelDataBuffer> Save ();
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
class celPcTimer : public celPcCommon
{
private:
  csRef<iVirtualClock> vc;
  bool enabled;
  csTicks wakeup, wakeup_todo;
  bool repeat;

public:
  celPcTimer (iObjectRegistry* object_reg);
  virtual ~celPcTimer ();

  bool HandleEvent (iEvent& ev);
  void WakeUp (csTicks t, bool repeat);
  void Clear ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pctimer"; }
  virtual csPtr<iCelDataBuffer> Save ();
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

/**
 * This is a property property class.
 */
class celPcProperties : public celPcCommon
{
private:
  csVector properties;

  struct property
  {
    char* propName;
    celDataType type;
    union
    {
      float f;
      long l;
      bool b;
      char* s;
    } v;
  };

  int NewProperty (const char* name);
  int FindOrNewProperty (const char* name);
  void ClearPropertyValue (property* p);

public:
  celPcProperties (iObjectRegistry* object_reg);
  virtual ~celPcProperties ();

  void SetProperty (const char* name, float value);
  void SetProperty (const char* name, long value);
  void SetProperty (const char* name, bool value);
  void SetProperty (const char* name, const char* value);
  int GetPropertyIndex (const char* name) const;
  void SetProperty (int index, float value);
  void SetProperty (int index, long value);
  void SetProperty (int index, bool value);
  void SetProperty (int index, const char* value);
  celDataType GetPropertyType (int index) const;
  float GetPropertyFloat (int index) const;
  long GetPropertyLong (int index) const;
  bool GetPropertyBool (int index) const;
  const char* GetPropertyString (int index) const;
  void ClearProperty (int index);
  void Clear ();
  int GetPropertyCount () const;
  const char* GetPropertyName (int idx) const;
  void Dump ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcproperties"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcProperties : public iPcProperties
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcProperties);
    virtual void SetProperty (const char* name, float value)
    {
      scfParent->SetProperty (name, value);
    }
    virtual void SetProperty (const char* name, long value)
    {
      scfParent->SetProperty (name, value);
    }
    virtual void SetProperty (const char* name, bool value)
    {
      scfParent->SetProperty (name, value);
    }
    virtual void SetProperty (const char* name, const char* value)
    {
      scfParent->SetProperty (name, value);
    }
    virtual int GetPropertyIndex (const char* name) const
    {
      return scfParent->GetPropertyIndex (name);
    }
    virtual void SetProperty (int index, float value)
    {
      scfParent->SetProperty (index, value);
    }
    virtual void SetProperty (int index, long value)
    {
      scfParent->SetProperty (index, value);
    }
    virtual void SetProperty (int index, bool value)
    {
      scfParent->SetProperty (index, value);
    }
    virtual void SetProperty (int index, const char* value)
    {
      scfParent->SetProperty (index, value);
    }
    virtual celDataType GetPropertyType (int index) const
    {
      return scfParent->GetPropertyType (index);
    }
    virtual float GetPropertyFloat (int index) const
    {
      return scfParent->GetPropertyFloat (index);
    }
    virtual long GetPropertyLong (int index) const
    {
      return scfParent->GetPropertyLong (index);
    }
    virtual bool GetPropertyBool (int index) const
    {
      return scfParent->GetPropertyBool (index);
    }
    virtual const char* GetPropertyString (int index) const
    {
      return scfParent->GetPropertyString (index);
    }
    virtual void ClearProperty (int index)
    {
      scfParent->ClearProperty (index);
    }
    virtual void Clear ()
    {
      scfParent->Clear ();
    }
    virtual int GetPropertyCount () const
    {
      return scfParent->GetPropertyCount ();
    }
    virtual const char* GetPropertyName (int idx) const
    {
      return scfParent->GetPropertyName (idx);
    }
    virtual void Dump ()
    {
      scfParent->Dump ();
    }
  } scfiPcProperties;
};

#endif // __CEL_PF_TOOLFACT__

