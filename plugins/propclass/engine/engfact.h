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
#include "csutil/refarr.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pl/facttmpl.h"
#include "plugins/propclass/common/stdpcimp.h"
#include "pf/camera.h"
#include "pf/region.h"

struct iCelEntity;
struct iObjectRegistry;
struct iGraphics3D;
struct iEngine;
struct iVirtualClock;
struct iKeyboardDriver;
struct iMouseDriver;
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
  csRef<iGraphics3D> g3d;
  csRef<iEngine> engine;
  csRef<iKeyboardDriver> kbd;
  csRef<iMouseDriver> mouse;
  csRef<iVirtualClock> vc;
  csRef<iView> view;
  bool use_cd;
  bool rect_set;
  int rect_x, rect_y, rect_w, rect_h;
  iPcCamera::CameraMode cammode;
  csRef<iPcRegion> region;

  float angle_xz, angle_yz, _xz, _yz, dist_y, _dist;
  bool alter_angle, alter_dist;
  int base_x, base_y, base_y_d;
  float pitch,roll,yaw;

  csVector3 followpos;
  csVector3 followat;
	
  void SetupEventHandler ();

public:
  celPcCamera (iObjectRegistry* object_reg);
  virtual ~celPcCamera ();

  bool HandleEvent (iEvent& ev);

  iCamera* GetCamera () const;
  iView* GetView () const { return view; }
  bool SetRegion (iPcRegion* region, bool point, const char* name);
  void SetRectangle (int x, int y, int w, int h);
  bool SetMode (iPcCamera::CameraMode cammode, bool use_cd = true);
  iPcCamera::CameraMode GetMode () const
  {
    return cammode;
  }
  void SetFollowPos (const csVector3& pos, const csVector3& at)
  {
    followpos = pos;
    followat = at;
  }
  void GetFollowPos (csVector3& pos, csVector3& at) const
  {
    pos = followpos;
    at = followat;
  }
  /// Set the vertical angle of the camera (around X axis)
  void SetPitch(float angle)
  {
    pitch = angle;
  }
  /// Get the vertical angle of the camera (around X axis)
  float GetPitch()
  {
    return pitch;
  }
  /// Set the tilt angle of the camera (around Z axis)
  void SetRoll(float angle)
  {
    roll = angle;
  }
  /// Set the tilt angle of the camera (around Z axis)
  float GetRoll()
  {
    return roll;
  }
  // Set the left/right angle of the camera (around Y axis)
  void SetYaw(float angle)
  {
    yaw = angle;
  }
  // Get the left/right angle of the camera (around Y axis)
  float GetYaw()
  {
    return yaw;
  }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pccamera"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcCamera : public iPcCamera
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCamera);
    virtual bool SetRegion (iPcRegion* region, bool point = true,
	const char* name = 0)
    {
      return scfParent->SetRegion(region,point,name);
    }
    virtual bool SetMode (CameraMode m, bool use_cd = true)
    {
      return scfParent->SetMode(m, use_cd);
    }
    virtual CameraMode GetMode () const
    {
      return scfParent->GetMode ();
    }
    virtual void SetFollowPos (const csVector3& pos, const csVector3& at)
    {
      scfParent->SetFollowPos(pos, at);
    }
    virtual void GetFollowPos (csVector3& fp, csVector3& at) const
    {
      scfParent->GetFollowPos (fp, at);
    }
    virtual void SetPitch(float angle)
    {
      scfParent->SetPitch(angle);
    }
    virtual float GetPitch()
    {
      return scfParent->GetPitch();
    }
    virtual void SetRoll(float angle)
    {
      scfParent->SetRoll(angle);
    }
    virtual float GetRoll()
    {
      return scfParent->GetRoll();
    }
    virtual void SetYaw(float angle)
    {
      scfParent->SetYaw(angle);
    }
    virtual float GetYaw()
    {
      return scfParent->GetYaw();
    }
    virtual void SetRectangle (int x, int y, int w, int h)
    {
      scfParent->SetRectangle (x, y, w, h);
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
      SCF_CONSTRUCT_IBASE (0);
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

  // This property class maintains private child entities
  // which are used for collision detection.
  csRefArray<iCelEntity> entities;

	enum propids {
		propid_worlddir = 0,
		propid_worldfile,
		propid_regionname,
		propid_load
	};

	static Property* properties;
	static int			 propertycount;

	static void UpdateProperties (iObjectRegistry* object_reg);
  
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
  iSector* FindSector (const char* name);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcregion"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  virtual bool PerformAction (csStringID, const char*);

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
    virtual iSector* FindSector(const char* name)
    {
      return scfParent->FindSector(name);
    }
    virtual bool Load ()
    {
      return scfParent->Load ();
    }
    virtual void Unload ()
    {
      scfParent->Unload ();
    }
    virtual iSector* GetStartSector (const char* name = 0)
    {
      return scfParent->GetStartSector (name);
    }
    virtual csVector3 GetStartPosition (const char* name = 0)
    {
      return scfParent->GetStartPosition (name);
    }
    virtual void PointCamera (iPcCamera* pccamera, const char* name = 0)
    {
      scfParent->PointCamera (pccamera, name);
    }
  } scfiPcRegion;
};

#endif // __CEL_PF_ENGFACT__

