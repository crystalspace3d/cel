/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_PORTALFACT__
#define __CEL_PF_PORTALFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/portal.h"

struct iCelEntity;
struct iPortal;
struct iEngine;
class csVector3;

class celPcPortalCallback;

/**
 * Factory for portal.
 */
CEL_DECLARE_FACTORY (Portal)

/**
 * This is a portal property class.
 */
class celPcPortal : public celPcCommon
{
private:
  csWeakRef<iPortal> portal;
  csWeakRef<iEngine> engine;
  csRef<celPcPortalCallback> cb;

  csString meshname;
  csString portalname;
  bool closed;

  // For properties.
  enum propids
  {
    propid_mesh = 0,
    propid_portal,
    propid_closed
  };
  static PropertyHolder propinfo;

  void ResolvePortal ();

public:
  celPcPortal (iObjectRegistry* object_reg);
  virtual ~celPcPortal ();

  bool SetPortal (const char* mesh, const char* portal = 0);
  iPortal* GetPortal ()
  {
    ResolvePortal ();
    return portal;
  }
  void ClosePortal ();
  void OpenPortal ();
  bool IsPortalClosed () const { return closed; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcportal"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int, iCelParameterBlock* params,
      celData& ret);
  virtual bool SetPropertyIndexed (int, const char*);
  virtual bool GetPropertyIndexed (int, const char*&);
  virtual bool SetPropertyIndexed (int, bool);
  virtual bool GetPropertyIndexed (int, bool&);

  struct PcPortal : public iPcPortal
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcPortal);
    virtual bool SetPortal (const char* mesh, const char* portal = 0)
    {
      return scfParent->SetPortal (mesh, portal);
    }
    virtual iPortal* GetPortal ()
    {
      return scfParent->GetPortal ();
    }
    virtual void ClosePortal ()
    {
      scfParent->ClosePortal ();
    }
    virtual void OpenPortal ()
    {
      scfParent->OpenPortal ();
    }
    virtual bool IsPortalClosed () const
    {
      return scfParent->IsPortalClosed ();
    }
  } scfiPcPortal;
};

#endif // __CEL_PF_PORTALFACT__

