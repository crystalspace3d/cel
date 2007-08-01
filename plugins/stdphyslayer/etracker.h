/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein
  
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

#ifndef __CEL_PLIMP_ETRACKER__
#define __CEL_PLIMP_ETRACKER__

#include "csutil/hash.h"
#include "csutil/set.h"

#include "plugins/stdphyslayer/pl.h"
#include "plugins/stdphyslayer/entity.h"

struct iCelEntity;
struct iMeshWrapper;
class celSectorcb;
class celMeshcb;
class celEntityList;

/**
 * A list of property classes.
 */
class celEntityTracker : public iCelEntityTracker
{
private:
  celPlLayer* pl;
  csSet<csPtrKey<iCelEntity> > entities;
  csSet<csPtrKey<iMeshWrapper> > entity_meshes;
  char* name;
  csRef<celSectorcb> sector_cb;
  csRefArray<celMeshcb> mesh_cbs;
  csHash<celMeshcb*, csPtrKey<iSector> > index_mesh_cbs;

  void FindNearbyEntities (celEntityList* list,
	iSector* sector, const csVector3& pos, float sqradius,
	csArray<iSector*>& visited_sectors);

public:
  celEntityTracker (celPlLayer* pl, const char* name);
  virtual ~celEntityTracker ();

  const csSet<csPtrKey<iCelEntity> >& GetEntities () const 
  { return entities; }
  const csSet<csPtrKey<iMeshWrapper> >& GetEntityMeshes () const 
  { return entity_meshes; }

  void RegisterSector (celMeshcb* cb);
  void UnregisterSector (iSector* sector);

  SCF_DECLARE_IBASE;

  // ---- For iCelEntityTracker -----------------------------------------

  virtual const char* GetName () const { return name; }
  virtual bool AddEntity (iCelEntity* entity);
  virtual void RemoveEntity (iCelEntity* entity);
  virtual void RemoveEntities ();
  virtual csPtr<iCelEntityList> FindNearbyEntities (iSector* sector,
  	const csVector3& pos, float radius);
  virtual csPtr<iCelEntityIterator> GetIterator ();
  virtual void AddEntities (iCelEntityTracker* tracker);
  virtual void RemoveEntities (iCelEntityTracker* tracker);

  class Iterator :
    public scfImplementation1<Iterator, iCelEntityIterator>
  {
  private:
    csSet<csPtrKey<iCelEntity> >::GlobalIterator it;

  public:
    Iterator (celEntityTracker* parent);

    virtual iCelEntity* Next ();
    virtual bool HasNext () const;
  };

  friend class Iterator;
};

#endif // __CEL_PLIMP_ETRACKER__

