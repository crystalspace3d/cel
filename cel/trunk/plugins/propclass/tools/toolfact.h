/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2005 by Jorrit Tyberghein
  
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

#endif // __CEL_PF_TOOLFACT__

