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
class celPfEngine : public iCelPropertyClassFactory
{
private:
  iObjectRegistry* object_reg;

public:
  celPfEngine (iBase* parent);
  virtual ~celPfEngine ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pfengine"; }
  virtual iCelPropertyClass* CreatePropertyClass (const char* type);
  virtual int GetTypeCount () const { return 2; }
  virtual const char* GetTypeName (int idx) const;

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPfEngine);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

/**
 * This is a camera property class.
 */
class celPcCamera : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iObjectRegistry* object_reg;
  iGraphics3D* g3d;
  iEngine* engine;
  iKeyboardDriver* kbd;
  iVirtualClock* vc;
  csView* view;
  iView* iview;

  void SetupEventHandler ();

public:
  celPcCamera (iObjectRegistry* object_reg);
  virtual ~celPcCamera ();

  bool HandleEvent (iEvent& ev);

  iCamera* GetCamera () const;
  iView* GetView () const { return iview; }
  
  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pccamera"; }
  virtual const char* GetFactoryName () const { return "pfengine"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcCamera : public iPcCamera
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCamera);
    virtual iCamera* GetCamera () const
    {
      return scfParent->GetCamera ();
    }
    virtual iView* GetView () const
    {
      return scfParent->GetView ();
    }
  } scfiPcCamera;
  struct EventHandler : public iEventHandler
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCamera);
    virtual bool HandleEvent (iEvent& ev)
    {
      return scfParent->HandleEvent (ev);
    }
  } scfiEventHandler;
};

/**
 * This is a region property class.
 */
class celPcRegion : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iObjectRegistry* object_reg;
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

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pcregion"; }
  virtual const char* GetFactoryName () const { return "pfengine"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
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

