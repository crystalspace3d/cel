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

#ifndef __CEL_PL_PERSIST__
#define __CEL_PL_PERSIST__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/util.h"

struct iCelPropertyClass;
struct iCelEntity;
struct iFile;
struct celData;

SCF_VERSION (iCelDataBuffer, 0, 0, 1);

enum
{
    CEL_PERSIST_MODE_READ = 1,
    CEL_PERSIST_MODE_WRITE = 2
};

/**
 * This interface describes persistable data.
 */
struct iCelDataBuffer : public iBase
{
  /**
   * Get a serial number for this data. This can be used
   * to check validity of the data (i.e. to compare version
   * numbers).
   */
  virtual long GetSerialNumber () const = 0;

  /**
   * Set the number of data entries.
   */
  virtual void SetDataCount (int cnt) = 0;

  /**
   * Get the number of data entries.
   */
  virtual int GetDataCount () const = 0;

  /**
   * Get a specific data entry.
   */
  virtual celData* GetData (int idx) const = 0;
};

SCF_VERSION (iCelPersistance, 0, 0, 1);
struct iCelPersistanceContext;

/**
 * This interface describes a module that can
 * persist CEL data.
 */
struct iCelPersistance : public iBase
{
  /**
   * Load an entity from the persistent data including
   * all property classes. 'name' is a name relevant for
   * the given type of persistance. It can be a filename
   * for example (VFS).
   */
  virtual iCelEntity* LoadEntity (const char* name) = 0;

  /**
   * Save an entity to persistent data.
   */
  virtual bool SaveEntity (iCelEntity* entity, const char* name) = 0;

  /**
   * Create an persistence context
   */
  virtual iCelPersistanceContext* CreateContext(iBase* data, int mode,
	  bool performmapping = true) = 0;
};

SCF_VERSION (iCelPersistanceContext, 0, 0, 1);

struct iCelPersistanceContext : public iBase
{
  /**
   * Load an entity from the persitent data including all property classes.
   */
  virtual iCelEntity* LoadEntity () = 0;

  /**
   * Save an entity to persistent data.
   */
  virtual bool SaveEntity (iCelEntity* entity) = 0;

  /* returns the ID mapped to the ID on the other side of the connection */
  virtual CS_ID GetMappedID(iCelEntity* entity) = 0;

  /* returns the entitiy after mapping the ID */
  virtual iCelEntity* GetMappedEntity(CS_ID id) = 0;

  /**
   * Clears internal data and buffers (like list of already saved entities)
   */
  virtual void Clear () = 0;
};

#endif // __CEL_PL_PERSIST__

