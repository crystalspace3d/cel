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

#include "cssysdef.h"
#include "csgeom/math3d.h"
#include "csutil/util.h"
#include "csutil/weakref.h"
#include "csutil/flags.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/sector.h"
#include "iengine/movable.h"
#include "iengine/portalcontainer.h"
#include "iengine/portal.h"

#include "plugins/stdphyslayer/etracker.h"
#include "physicallayer/propclas.h"
#include "propclass/mesh.h"

//---------------------------------------------------------------------------

class celMeshcb : public scfImplementation1<
	celMeshcb, iSectorMeshCallback>
{
private:
  iSector* sector;
  csWeakRef<celEntityTracker> tracker;
  csSet<csPtrKey<iMeshWrapper> > portals;
  csSet<csPtrKey<iMeshWrapper> > meshes;

public:
  celMeshcb (celEntityTracker* tracker, iSector* sector)
    : scfImplementationType (this)
  {
    celMeshcb::tracker = tracker;
    celMeshcb::sector = sector;
  }
  virtual ~celMeshcb ()
  {
  }

  iSector* GetSector () const { return sector; }
  const csSet<csPtrKey<iMeshWrapper> >& GetPortals () const 
  { return portals; }
  const csSet<csPtrKey<iMeshWrapper> >& GetMeshes () const 
  { return meshes; }

  /// Register all portals and entities for a new sector.
  void RegisterPortalsAndEntities ()
  {
    const csSet<csPtrKey<iMeshWrapper> >& entity_meshes = 
      tracker->GetEntityMeshes ();
    int i, cnt;
    iMeshList* ml = sector->GetMeshes ();
    cnt = ml->GetCount ();
    for (i = 0 ; i < cnt ; i++)
    {
      iMeshWrapper* mw = ml->Get (i);
      if (mw->GetPortalContainer ())
      {
        portals.Add (mw);
      }
      else if (entity_meshes.In (mw))
      {
        meshes.Add (mw);
      }
    }
  }

  virtual void NewMesh (iSector* sector, iMeshWrapper* mesh)
  {
    if (tracker)
    {
      if (mesh->GetPortalContainer ())
      {
        // We have a new portal. Register it.
	portals.Add (mesh);
      }
      else if (tracker->GetEntityMeshes ().In (mesh))
      {
	// We have an entity that we're interested in. Register it.
        meshes.Add (mesh);
      }
    }
    else
    {
      // Tracker was removed so we remove ourselves from the sector
      // callback list.
      sector->RemoveSectorMeshCallback (this);
    }
  }

  virtual void RemoveMesh (iSector* sector, iMeshWrapper* mesh)
  {
    if (tracker)
    {
      portals.Delete (mesh);
      meshes.Delete (mesh);
    }
    else
    {
      // Tracker was removed so we remove ourselves from the sector
      // callback list.
      sector->RemoveSectorMeshCallback (this);
    }
  }
};

//---------------------------------------------------------------------------

class celSectorcb : public scfImplementation1<
	celSectorcb, iEngineSectorCallback>
{
private:
  celEntityTracker* tracker;

public:
  celSectorcb (celEntityTracker* tracker) : scfImplementationType (this)
  {
    celSectorcb::tracker = tracker;
  }
  virtual ~celSectorcb ()
  {
  }

  virtual void NewSector (iEngine*, iSector* sector)
  {
    celMeshcb* cb = new celMeshcb (tracker, sector);
    sector->AddSectorMeshCallback (cb);
    tracker->RegisterSector (cb);
    cb->DecRef ();
  }

  virtual void RemoveSector (iEngine*, iSector* sector)
  {
    tracker->UnregisterSector (sector);
  }
};

//---------------------------------------------------------------------------

celEntityTracker::celEntityTracker (celPlLayer* pl, const char* name)
  : scfImplementationType (this)
{
  celEntityTracker::pl = pl;
  celEntityTracker::name = csStrNew (name);
  sector_cb.AttachNew (new celSectorcb (this));
  iEngine* engine = pl->GetEngine ();
  if (engine)
  {
    engine->AddEngineSectorCallback (sector_cb);
    int i, cnt;
    iSectorList* sl = engine->GetSectors ();
    cnt = sl->GetCount ();
    for (i = 0 ; i < cnt ; i++)
    {
      celMeshcb* cb = new celMeshcb (this, sl->Get (i));
      sl->Get (i)->AddSectorMeshCallback (cb);
      cb->RegisterPortalsAndEntities ();
      cb->DecRef ();
    }
  }
}

celEntityTracker::~celEntityTracker ()
{
  if (pl->GetEngine ())
    pl->GetEngine ()->RemoveEngineSectorCallback (sector_cb);
  while (mesh_cbs.GetSize () > 0)
  {
    mesh_cbs[0]->GetSector ()->RemoveSectorMeshCallback (mesh_cbs[0]);
    mesh_cbs.DeleteIndex (0);
  }
}

bool celEntityTracker::AddEntity (iCelEntity* entity)
{
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  if (pcmesh && pcmesh->GetMesh ())
  {
    entities.Add (entity);
    entity_meshes.Add (pcmesh->GetMesh ());
  }
  return true;
}

void celEntityTracker::RemoveEntity (iCelEntity* entity)
{
  entities.Delete (entity);
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  if (pcmesh && pcmesh->GetMesh ())
    entity_meshes.Delete (pcmesh->GetMesh ());
}

void celEntityTracker::RemoveEntities ()
{
  entities.DeleteAll ();
  entity_meshes.DeleteAll ();
}

void celEntityTracker::FindNearbyEntities (celEntityList* list,
	iSector* sector, const csVector3& pos, float sqradius,
	csArray<iSector*>& visited_sectors, csStringID cls)
{
  celMeshcb* cb = index_mesh_cbs.Get (sector, 0);
  if (cb)
  {
    const csSet<csPtrKey<iMeshWrapper> >& meshes = cb->GetMeshes ();
    csSet<csPtrKey<iMeshWrapper> >::GlobalIterator meshes_it = 
      meshes.GetIterator ();
    while (meshes_it.HasNext ())
    {
      csPtrKey<iMeshWrapper> mesh = meshes_it.Next ();
      bool invisible = mesh->GetFlags ().Check (CS_ENTITY_INVISIBLE);
      if (invisible) continue;
      csVector3 mpos = mesh->GetMovable ()->GetFullPosition ();
      if (csSquaredDist::PointPoint (pos, mpos) <= sqradius)
      {
        iCelEntity* ent = pl->FindAttachedEntity (mesh->QueryObject ());
        if (ent)
	{
	  if (cls == csInvalidStringID || ent->HasClass (cls))
	    list->Add (ent);
	}
      }
    }

    const csSet<csPtrKey<iMeshWrapper> >& portals = cb->GetPortals ();
    csSet<csPtrKey<iMeshWrapper> >::GlobalIterator portals_it = 
      portals.GetIterator ();
    while (portals_it.HasNext ())
    {
      csPtrKey<iMeshWrapper> mesh = portals_it.Next ();
      bool invisible = mesh->GetFlags ().Check (CS_ENTITY_INVISIBLE);
      if (invisible) continue;
      csVector3 mpos = mesh->GetMovable ()->GetFullPosition ();
      if (csSquaredDist::PointPoint (pos, mpos) <= sqradius)
      {
        // @@@ If the portal container is in range we simply assume
	// all portals in that container are in range!
        iPortalContainer* portals = mesh->GetPortalContainer ();
	int pc = portals->GetPortalCount  ();
	int j;
	for (j = 0 ; j < pc ; j++)
	{
	  iPortal* portal = portals->GetPortal (j);
	  portal->CompleteSector (0);
	  if (sector != portal->GetSector () && portal->GetSector ())
	  {
	    bool already_visited = false;
	    size_t l;
            for (l = 0 ; l < visited_sectors.GetSize () ; l++)
            {
              if (visited_sectors[l] == portal->GetSector ())
              {
                already_visited = true;
                break;
              }
            }
            if (!already_visited)
            {
              visited_sectors.Push (portal->GetSector ());
	      if (portal->GetFlags ().Check (CS_PORTAL_WARP))
	      {
		csReversibleTransform warp_wor;
		portal->ObjectToWorld (
		    	mesh->GetMovable ()->GetFullTransform (), warp_wor);
		csVector3 tpos = warp_wor.Other2This (pos);
                FindNearbyEntities (list, portal->GetSector (), tpos, sqradius,
		    	visited_sectors, cls);
	      }
	      else
	      {
                FindNearbyEntities (list, portal->GetSector (), pos, sqradius,
		    	visited_sectors, cls);
	      }
	    }
	  }
	}
      }
    }
  }
}

csPtr<iCelEntityList> celEntityTracker::FindNearbyEntities (iSector* sector,
  	const csVector3& pos, float radius, csStringID cls)
{
  // @@@ Some kind of optimization to cache entity lists?
  celEntityList* list = new celEntityList ();

  csArray<iSector*> visited_sectors;
  FindNearbyEntities (list, sector, pos, radius * radius, visited_sectors,
      cls);
  return list;
}

csPtr<iCelEntityIterator> celEntityTracker::GetIterator ()
{
  return new Iterator (this);
}

void celEntityTracker::AddEntities (iCelEntityTracker* tracker)
{
  csRef<iCelEntityIterator> it = tracker->GetIterator ();
  while (it->HasNext ())
  {
    AddEntity (it->Next ());
  }
}

void celEntityTracker::RemoveEntities (iCelEntityTracker* tracker)
{
  csRef<iCelEntityIterator> it = tracker->GetIterator ();
  while (it->HasNext ())
  {
    RemoveEntity (it->Next ());
  }
}

void celEntityTracker::RegisterSector (celMeshcb* cb)
{
  mesh_cbs.Push (cb);
  index_mesh_cbs.Put (cb->GetSector (), cb);
}

void celEntityTracker::UnregisterSector (iSector* sector)
{
  size_t i = mesh_cbs.GetSize ();
  while (i > 0)
  {
    i--;
    if (mesh_cbs[i]->GetSector () == sector)
    {
      index_mesh_cbs.Delete (sector, mesh_cbs[i]);
      mesh_cbs.DeleteIndex (i);
      return;
    }
  }
}

//---------------------------------------------------------------------------

celEntityTracker::Iterator::Iterator (celEntityTracker* parent) :
  scfImplementationType (this), it (parent->entities.GetIterator ())
{
}

iCelEntity* celEntityTracker::Iterator::Next ()
{
  return it.Next ();
}

bool celEntityTracker::Iterator::HasNext () const
{
  return it.HasNext ();
}

