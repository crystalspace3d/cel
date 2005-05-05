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
#include "csutil/weakref.h"
#include "csutil/weakrefarr.h"
#include "csutil/refarr.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "propclass/region.h"
#include "propclass/linmove.h"
#include "propclass/mesh.h"

struct iCelEntity;
struct iObjectRegistry;
struct iGraphics3D;
struct iEngine;
struct iVirtualClock;
struct iKeyboardDriver;
struct iMouseDriver;
struct iCollideSystem;
class csView;
class celPcCamera;
class iPcZoneManager;

/**
 * Factory for engine stuff.
 */
CEL_DECLARE_FACTORY(Region)

/**
 * This is a region property class.
 */
class celPcRegion : public celPcCommon, public iCelNewEntityCallback
{
private:
  bool empty_sector;
  char* worlddir;
  // If empty_sector is true then worldfile is the name of the sector to create.
  char* worldfile;
  char* regionname;
  bool loaded;

  // This property class maintains private child entities
  // which are used for collision detection.
  csWeakRefArray<iCelEntity> entities;

  enum propids
  {
    propid_worlddir = 0,
    propid_worldfile,
    propid_regionname,
    propid_load
  };

  static Property* properties;
  static size_t propertycount;

  static void UpdateProperties (iObjectRegistry* object_reg);

private:
  /**
   * Returns the CS region associated to this entity. No check performed,
   * the caller of this function is responsable of performing them
   */
  iRegion* GetRegionInternal (csRef<iEngine> engine);

public:
  celPcRegion (iObjectRegistry* object_reg);
  virtual ~celPcRegion ();

  /// Set the worldfile directory on VFS and filename.
  void SetWorldFile (const char* vfsdir, const char* name);
  /// Get the worldfile directory name.
  const char* GetWorldDir () const { return worlddir; }
  /// Get the worldfile filename.
  const char* GetWorldFile () const { return worldfile; }
  /// Set a name for this region.
  void SetRegionName (const char* name);
  /// Get the name of this region.
  const char* GetRegionName () const { return regionname; }
  /// Create an empty sector.
  void CreateEmptySector (const char* name);
  /**
   * Load an entire region according to worldfile, worlddir,
   * regionname and create entities for all meshes in this region
   * unless there is already an entity for them (an addon may have
   * created them for example).
   * \param allow_entity_addon if false then entity addons will not
   * be allowed to create entities. This is used when loading this
   * pcregion. The persistence layer will take care of creating the other
   * entities so it is not needed (or even desired) to load them here.
   */
  bool Load (bool allow_entity_addon = true);
  /**
   * Unload the region by removing all entities from this region
   * and removing it from the engines region list.
   */
  void Unload ();
  /**
   * Get the start sector by searching for the given "name"
   * in all camerapositions within this world, if the position
   * is found it returns the belonging sector, otherwise it's
   * searching for the sector "room".
   */
  iSector* GetStartSector (const char* name);
  /**
   * Get the startposition queried by "name" searching through
   * all camerapositions within this world.
   * Otherwise returns first cameraposition.
   */
  csVector3 GetStartPosition (const char* name);
  /// Points the given camera to the startingposition named "name".
  void PointCamera (iPcCamera* pccamera, const char* name);
  /// Find a sector named "name".
  iSector* FindSector (const char* name);
  /// Get the CS region
  iRegion* GetRegion ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcregion"; }
  virtual csPtr<iCelDataBuffer> SaveFirstPass ();
  virtual bool LoadFirstPass (iCelDataBuffer* databuf);
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  virtual bool PerformAction (csStringID, iCelParameterBlock* params);

  // For iCelNewEntityCallback.
  virtual void NewEntity (iCelEntity* entity);

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
    virtual void CreateEmptySector (const char* name)
    {
      scfParent->CreateEmptySector (name);
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
    virtual iRegion* GetRegion ()
    {
      return scfParent->GetRegion ();
    }
  } scfiPcRegion;
};

#endif // __CEL_PF_ENGFACT__

