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
#include "csutil/scf.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pf/tooltip.h"

struct iCelEntity;

/**
 * Factory for tools.
 */
class celPfTools : public iCelPropertyClassFactory
{
private:

public:
  celPfTools (iBase* parent);
  virtual ~celPfTools ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pftools"; }
  virtual iCelPropertyClass* CreatePropertyClass (const char* type);
  virtual int GetTypeCount () const { return 1; }
  virtual const char* GetTypeName (int idx) const { return "pctooltip"; }

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
  bool visible;
  int x, y;
  char* text;

public:
  celPcTooltip ();
  virtual ~celPcTooltip ();

  void SetText (const char* text);
  void Show (int x, int y) { visible = true; celPcTooltip::x = x; celPcTooltip::y = y; }
  void Hide () { visible = false; }
  bool IsVisible () const { return visible; }

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pctooltip"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);

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
  } scfiPcTooltip;
};

#endif // __CEL_PF_TOOLFACT__

