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
#include "csutil/weakref.h"
#include "csutil/parray.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "csutil/hashhandlers.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/tooltip.h"
#include "propclass/timer.h"
#include "propclass/prop.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;
struct iVirtualClock;
struct iGraphics2D;
struct iGraphics3D;
struct iFont;
class csStringArray;

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
  celTooltipJustify justify;
  int x, y;
  int width, height;
  csStringArray lines;
  //char* text;
  int text_r, text_g, text_b;
  int bg_r, bg_g, bg_b;
  csRef<iGraphics2D> G2D;
  csRef<iGraphics3D> G3D;
  csRef<iFont> fnt;

  void CalculateExtents ();

public:
  celPcTooltip (iObjectRegistry* object_reg);
  virtual ~celPcTooltip ();

  void SetText (const char* text);
  void Show (int x, int y);
  void Hide ();
  bool IsVisible () const { return visible; }
  void SetTextColor (int r, int g, int b)
  { text_r = r; text_g = g; text_b = b; }
  void SetBackgroundColor (int r, int g, int b)
  { bg_r = r; bg_g = g; bg_b = b; }
  void SetFont(iFont* font);
  void SetJustify (celTooltipJustify justify)
  { this->justify = justify; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pctooltip"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual void TickEveryFrame ();

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
    virtual void SetFont (iFont* font)
    {
      scfParent->SetFont (font);
    }
    virtual void SetJustify (celTooltipJustify justify)
    {
      scfParent->SetJustify (justify);
    }
  } scfiPcTooltip;
};

/**
 * This is a timer property class.
 */
class celPcTimer : public celPcCommon
{
private:
  csRef<iVirtualClock> vc;
  bool enabled;
  csTicks wakeup;
  bool repeat;
  bool wakeupframe;
  bool wakeuponce;

  static csStringID action_wakeup;
  static csStringID action_wakeupframe;
  static csStringID action_clear;

  static csStringID id_elapsedticks;
  static csStringID id_currentticks;
  static csStringID id_time;
  static csStringID id_repeat;
  celGenericParameterBlock* params;

public:
  celPcTimer (iObjectRegistry* object_reg);
  virtual ~celPcTimer ();

  void WakeUp (csTicks t, bool repeat);
  void WakeUpFrame (int where = cscmdPreProcess);
  void Clear ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pctimer"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);
  virtual void TickOnce ();
  virtual void TickEveryFrame ();

  struct PcTimer : public iPcTimer
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcTimer);
    virtual void WakeUp (csTicks t, bool repeat)
    {
      scfParent->WakeUp (t, repeat);
    }
    virtual void WakeUpFrame (int where = cscmdPreProcess)
    {
      scfParent->WakeUpFrame (where);
    }
    virtual void Clear ()
    {
      scfParent->Clear ();
    }
  } scfiPcTimer;
};

/**
 * This is a property property class.
 */
class celPcProperties : public celPcCommon
{
private:
  struct property
  {
    char* propName;
    csStringID id;
    celDataType type;
    union
    {
      float f;
      long l;
      bool b;
      char* s;
      struct { float x, y, z; } vec;
      struct { float red, green, blue; } col;
    } v;
    csWeakRef<iCelEntity> entity;
    csWeakRef<iCelPropertyClass> pclass;
  };
  csPDelArray<property> properties;
  csHash<size_t, csStrKey, csConstCharHashKeyHandler> properties_hash;
  bool properties_hash_dirty;
  csRefArray<iPcPropertyListener> listeners;

  size_t NewProperty (const char* name);
  size_t FindProperty (csStringID id);
  size_t FindOrNewProperty (const char* name);
  size_t FindOrNewProperty (csStringID id);
  void ClearPropertyValue (property* p);

  static csStringID id_index;
  celOneParameterBlock* params;

public:
  celPcProperties (iObjectRegistry* object_reg);
  virtual ~celPcProperties ();

  virtual bool SetProperty (csStringID, const char*);
  virtual bool SetProperty (csStringID, bool);
  virtual bool SetProperty (csStringID, float);
  virtual bool SetProperty (csStringID, long);
  virtual bool SetProperty (csStringID, const csVector2&);
  virtual bool SetProperty (csStringID, const csVector3&);
  virtual bool SetProperty (csStringID, const csColor&);
  virtual bool SetProperty (csStringID, iCelPropertyClass*);
  virtual bool SetProperty (csStringID, iCelEntity*);
  virtual const char* GetPropertyString (csStringID);
  virtual bool GetPropertyBool (csStringID);
  virtual long GetPropertyLong (csStringID);
  virtual float GetPropertyFloat (csStringID);
  virtual bool GetPropertyVector (csStringID, csVector2&);
  virtual bool GetPropertyVector (csStringID, csVector3&);
  virtual bool GetPropertyColor (csStringID, csColor&);
  virtual iCelPropertyClass* GetPropertyPClass (csStringID);
  virtual iCelEntity* GetPropertyEntity (csStringID);

  virtual celDataType GetPropertyOrActionType (csStringID propertyID);
  virtual bool IsPropertyReadOnly (csStringID) { return false; }
  virtual int GetPropertyAndActionCount () const
  { return properties.Length (); }
  virtual csStringID GetPropertyOrActionID (int i);
  virtual const char* GetPropertyOrActionDescription (csStringID);

  void SetProperty (const char* name, float value);
  void SetProperty (const char* name, long value);
  void SetProperty (const char* name, bool value);
  void SetProperty (const char* name, const char* value);
  void SetProperty (const char* name, const csVector2& value);
  void SetProperty (const char* name, const csVector3& value);
  void SetProperty (const char* name, const csColor& value);
  void SetProperty (const char* name, iCelPropertyClass* pclass);
  void SetProperty (const char* name, iCelEntity* entity);
  size_t GetPropertyIndex (const char* name);
  void SetPropertyIndex (size_t index, float value);
  void SetPropertyIndex (size_t index, long value);
  void SetPropertyIndex (size_t index, bool value);
  void SetPropertyIndex (size_t index, const char* value);
  void SetPropertyIndex (size_t index, const csVector2& value);
  void SetPropertyIndex (size_t index, const csVector3& value);
  void SetPropertyIndex (size_t index, const csColor& value);
  void SetPropertyIndex (size_t index, iCelPropertyClass* pclass);
  void SetPropertyIndex (size_t index, iCelEntity* entity);
  celDataType GetPropertyType (size_t index) const;
  float GetPropertyFloatIndex (size_t index) const;
  long GetPropertyLongIndex (size_t index) const;
  bool GetPropertyBoolIndex (size_t index) const;
  bool GetPropertyVectorIndex (size_t index, csVector2& value) const;
  bool GetPropertyVectorIndex (size_t index, csVector3& value) const;
  bool GetPropertyColorIndex (size_t index, csColor& value) const;
  const char* GetPropertyStringIndex (size_t index) const;
  iCelPropertyClass* GetPropertyPClassIndex (size_t index) const;
  iCelEntity* GetPropertyEntityIndex (size_t index) const;
  void ClearProperty (size_t index);
  void Clear ();
  size_t GetPropertyCount () const;
  const char* GetPropertyName (size_t idx) const;
  void Dump ();
  void AddPropertyListener (iPcPropertyListener* listener);
  void RemovePropertyListener (iPcPropertyListener* listener);
  void FirePropertyListeners (size_t idx);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcproperties"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcProperties : public iPcProperties
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcProperties);
    virtual void AddPropertyListener (iPcPropertyListener* listener)
    {
      scfParent->AddPropertyListener (listener);
    }
    virtual void RemovePropertyListener (iPcPropertyListener* listener)
    {
      scfParent->RemovePropertyListener (listener);
    }
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
    virtual void SetProperty (const char* name, const csVector2& value)
    {
      scfParent->SetProperty (name, value);
    }
    virtual void SetProperty (const char* name, const csVector3& value)
    {
      scfParent->SetProperty (name, value);
    }
    virtual void SetProperty (const char* name, const csColor& value)
    {
      scfParent->SetProperty (name, value);
    }
    virtual void SetProperty (const char* name, iCelPropertyClass* value)
    {
      scfParent->SetProperty (name, value);
    }
    virtual void SetProperty (const char* name, iCelEntity* value)
    {
      scfParent->SetProperty (name, value);
    }
    virtual size_t GetPropertyIndex (const char* name)
    {
      return scfParent->GetPropertyIndex (name);
    }
    virtual void SetPropertyIndex (size_t index, float value)
    {
      scfParent->SetPropertyIndex (index, value);
    }
    virtual void SetPropertyIndex (size_t index, long value)
    {
      scfParent->SetPropertyIndex (index, value);
    }
    virtual void SetPropertyIndex (size_t index, bool value)
    {
      scfParent->SetPropertyIndex (index, value);
    }
    virtual void SetPropertyIndex (size_t index, const csVector2& value)
    {
      scfParent->SetPropertyIndex (index, value);
    }
    virtual void SetPropertyIndex (size_t index, const csVector3& value)
    {
      scfParent->SetPropertyIndex (index, value);
    }
    virtual void SetPropertyIndex (size_t index, const csColor& value)
    {
      scfParent->SetPropertyIndex (index, value);
    }
    virtual void SetPropertyIndex (size_t index, const char* value)
    {
      scfParent->SetPropertyIndex (index, value);
    }
    virtual void SetPropertyIndex (size_t index, iCelPropertyClass* value)
    {
      scfParent->SetPropertyIndex (index, value);
    }
    virtual void SetPropertyIndex (size_t index, iCelEntity* value)
    {
      scfParent->SetPropertyIndex (index, value);
    }
    virtual celDataType GetPropertyType (size_t index) const
    {
      return scfParent->GetPropertyType (index);
    }
    virtual float GetPropertyFloat (size_t index) const
    {
      return scfParent->GetPropertyFloatIndex (index);
    }
    virtual long GetPropertyLong (size_t index) const
    {
      return scfParent->GetPropertyLongIndex (index);
    }
    virtual bool GetPropertyBool (size_t index) const
    {
      return scfParent->GetPropertyBoolIndex (index);
    }
    virtual bool GetPropertyVector (size_t index, csVector2& v) const
    {
      return scfParent->GetPropertyVectorIndex (index, v);
    }
    virtual bool GetPropertyVector (size_t index, csVector3& v) const
    {
      return scfParent->GetPropertyVectorIndex (index, v);
    }
    virtual bool GetPropertyColor (size_t index, csColor& v) const
    {
      return scfParent->GetPropertyColorIndex (index, v);
    }
    virtual const char* GetPropertyString (size_t index) const
    {
      return scfParent->GetPropertyStringIndex (index);
    }
    virtual iCelPropertyClass* GetPropertyPClass (size_t index) const
    {
      return scfParent->GetPropertyPClassIndex (index);
    }
    virtual iCelEntity* GetPropertyEntity (size_t index) const
    {
      return scfParent->GetPropertyEntityIndex (index);
    }
    virtual void ClearProperty (size_t index)
    {
      scfParent->ClearProperty (index);
    }
    virtual void Clear ()
    {
      scfParent->Clear ();
    }
    virtual size_t GetPropertyCount () const
    {
      return scfParent->GetPropertyCount ();
    }
    virtual const char* GetPropertyName (size_t idx) const
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

