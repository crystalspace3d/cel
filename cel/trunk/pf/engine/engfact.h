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

#ifndef __CEL_PF_ENGFACT__
#define __CEL_PF_ENGFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/csvector.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pl/facttmpl.h"
#include "pf/common/stdpcimp.h"
#include "pf/camera.h"
#include "pf/region.h"

struct iCelEntity;
struct iObjectRegistry;
struct iGraphics3D;
struct iEngine;
struct iVirtualClock;
struct iKeyboardDriver;
class csView;

/**
 * Factory for engine stuff.
 */
CEL_DECLARE_FACTORY(Camera)
CEL_DECLARE_FACTORY(Region)

/**
 * This is a camera property class.
 */
class celPcCamera : public celPcCommon
{
private:
  iGraphics3D* g3d;
  iEngine* engine;
  iKeyboardDriver* kbd;
  iVirtualClock* vc;
  csView* view;
  iView* iview;
  bool use_cd;
  iPcCamera::CameraMode cammode;

  void SetupEventHandler ();

public:
  celPcCamera (iObjectRegistry* object_reg);
  virtual ~celPcCamera ();

  bool HandleEvent (iEvent& ev);

  iCamera* GetCamera () const;
  iView* GetView () const { return iview; }
  bool SetMode (iPcCamera::CameraMode cammode, bool use_cd = true)
  {
      celPcCamera::cammode=cammode;
      celPcCamera::use_cd=use_cd;
      return true;
  }
  
  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pccamera"; }
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcCamera : public iPcCamera
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCamera);
    virtual bool SetMode(CameraMode m, bool use_cd = true)
    {
      return scfParent->SetMode(m, use_cd);
    }
    virtual iCamera* GetCamera () const
    {
      return scfParent->GetCamera ();
    }
    virtual iView* GetView () const
    {
      return scfParent->GetView ();
    }
  } scfiPcCamera;

  // Not an embedded interface to avoid circular reference!!!
  class EventHandler : public iEventHandler
  {
  private:
    celPcCamera* parent;

  public:
    EventHandler (celPcCamera* parent)
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
 * This is a region property class.
 */
class celPcRegion : public celPcCommon
{
private:
  char* worlddir;
  char* worldfile;
  char* regionname;
  bool loaded;

  iPcCamera* pointcamera;
  char* startname;

  // This property class maintains private child entities
  // which are used for collision detection.
  csVector entities;

public:
  celPcRegion (iObjectRegistry* object_reg);
  virtual ~celPcRegion ();

  void SetWorldFile (const char* vfsdir, const char* name);
  const char* GetWorldDir () const { return worlddir; }
  const char* GetWorldFile () const { return worldfile; }
  void SetRegionName (const char* name);
  const char* GetRegionName () const { return regionname; }
  bool Load ();
  void Unload ();
  iSector* GetStartSector (const char* name);
  csVector3 GetStartPosition (const char* name);
  void PointCamera (iPcCamera* pccamera, const char* name);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcregion"; }
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcRegion : public iPcRegion
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcRegion);
    virtual void SetWorldFile (const char* vfsdir, const char* name)
    {
      scfParent->SetWorldFile (vfsdir, name);
    }
    virtual const char* GetWorldDir () const
    {
      return scfParent->GetWorldDir ();
    }
    virtual const char* GetWorldFile () const
    {
      return scfParent->GetWorldFile ();
    }
    virtual void SetRegionName (const char* name)
    {
      scfParent->SetRegionName (name);
    }
    virtual const char* GetRegionName () const
    {
      return scfParent->GetRegionName ();
    }
    virtual bool Load ()
    {
      return scfParent->Load ();
    }
    virtual void Unload ()
    {
      scfParent->Unload ();
    }
    virtual iSector* GetStartSector (const char* name = NULL)
    {
      return scfParent->GetStartSector (name);
    }
    virtual csVector3 GetStartPosition (const char* name = NULL)
    {
      return scfParent->GetStartPosition (name);
    }
    virtual void PointCamera (iPcCamera* pccamera, const char* name = NULL)
    {
      scfParent->PointCamera (pccamera, name);
    }
  } scfiPcRegion;
};

#endif // __CEL_PF_ENGFACT__

