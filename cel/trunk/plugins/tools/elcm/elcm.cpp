/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein

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
#include "cstool/initapp.h"
#include "csutil/objreg.h"
#include "csutil/weakref.h"
#include "csutil/event.h"
#include "csutil/cfgacc.h"
#include "csgeom/math3d.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "iutil/object.h"
#include "iutil/virtclk.h"
#include "ivideo/graph3d.h"
#include "ivaria/reporter.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/movable.h"

#include "propclass/mesh.h"
#include "propclass/camera.h"

#include "elcm.h"

SCF_IMPLEMENT_FACTORY (celELCM)

//--------------------------------------------------------------------------

class ElcmEntityIterator : public scfImplementation1<ElcmEntityIterator,
  iCelEntityIterator>
{
private:
  csSet<csPtrKey<iCelEntity> >::GlobalIterator activeIt;
  csHash<csTicks,csPtrKey<iCelEntity> >::ConstGlobalIterator inactiveIt;
  iCelEntity* nextEntity;

  void ScanNextEntity ()
  {
    while (activeIt.HasNext ())
    {
      nextEntity = activeIt.Next ();
      if (nextEntity->IsModifiedSinceBaseline ())
	return;
    }
    while (inactiveIt.HasNext ())
    {
      csPtrKey<iCelEntity> ent;
      inactiveIt.Next (ent);
      nextEntity = ent;
      if (nextEntity->IsModifiedSinceBaseline ())
	return;
    }
    nextEntity = 0;
  }

public:
  ElcmEntityIterator (const csSet<csPtrKey<iCelEntity> >::GlobalIterator& activeIt,
      const csHash<csTicks,csPtrKey<iCelEntity> >::ConstGlobalIterator& inactiveIt) :
    scfImplementationType (this), activeIt (activeIt), inactiveIt (inactiveIt)
  {
    ScanNextEntity ();
  }
  virtual ~ElcmEntityIterator () { }

  virtual iCelEntity* Next ()
  {
    iCelEntity* ent = nextEntity;
    ScanNextEntity ();
    return ent;
  }

  virtual bool HasNext () const
  {
    return nextEntity != 0;
  }
};


//--------------------------------------------------------------------------

class celNewEntityCallback : public scfImplementation1<celNewEntityCallback,
  iCelNewEntityCallback>
{
private:
  csWeakRef<celELCM> elcm;

public:
  celNewEntityCallback (celELCM* elcm) : scfImplementationType (this),
  	elcm (elcm)
  {
  }
  virtual ~celNewEntityCallback () { }
  virtual void NewEntity (iCelEntity* entity)
  {
    if (elcm)
      elcm->ListenNewEntity (entity);
  }
};


class celEntityRemoveCallback : public scfImplementation1<celEntityRemoveCallback,
  iCelEntityRemoveCallback>
{
private:
  csWeakRef<celELCM> elcm;

public:
  celEntityRemoveCallback (celELCM* elcm) : scfImplementationType (this),
  	elcm (elcm)
  {
  }
  virtual ~celEntityRemoveCallback () { }
  virtual void RemoveEntity (iCelEntity* entity)
  {
    if (elcm)
      elcm->ListenRemoveEntity (entity);
  }
};


//--------------------------------------------------------------------------


celELCM::celELCM (iBase* parent)
  : scfImplementationType (this, parent)
{
  activityRadius = 500;
  distanceThresshold = 20;
  checkTime = 100;
  forcedCheck = false;
  unloadedTime = 10000;

  prevSector = 0;
  atBaseline = false;

  activeEntities = &activeEntities1;
}

celELCM::~celELCM ()
{
}

bool celELCM::Initialize (iObjectRegistry* object_reg)
{
  celELCM::object_reg = object_reg;
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  return true;
}

iEngine* celELCM::GetEngine ()
{
  if (!engine)
  {
    engine = csQueryRegistry<iEngine> (object_reg);
  }
  return engine;
}

iCelPlLayer* celELCM::GetPL ()
{
  if (!pl)
  {
    pl = csQueryRegistry<iCelPlLayer> (object_reg);
    if (!pl) return 0;
    csRef<celNewEntityCallback> new_cb;
    new_cb.AttachNew (new celNewEntityCallback (this));
    csRef<celEntityRemoveCallback> rem_cb;
    rem_cb.AttachNew (new celEntityRemoveCallback (this));
    pl->AddNewEntityCallback (new_cb);
    pl->AddEntityRemoveCallback (rem_cb);

    SetPL (pl);	// Set the periodic timer.
    SetTickTime (checkTime);
  }
  return pl;
}

void celELCM::SetCheckTime (csTicks t)
{
  checkTime = t;
  SetTickTime (checkTime);
}

void celELCM::Tick ()
{
  csVector3 pos;
  iSector* sector = GetPlayerPosition (pos);
  if (!player) return;

  unload--;
  if (unload <= 0)
  {
    unload = unloadCheckFrequency;
    CheckUnload ();
  }

  if (forcedCheck)
  {
    // Check is forced.
    forcedCheck = false;
    UpdateActiveEntities ();
    return;
  }
  if (prevSector != sector)
  {
    // Player changed sector. We surely have to update.
    UpdateActiveEntities ();
    return;
  }
  float sqdist = csSquaredDist::PointPoint (pos, prevPos);
  if (sqdist >= sqDistanceThresshold)
  {
    // We moved too far. Update.
    UpdateActiveEntities ();
  }
}

csSet<csPtrKey<iCelEntity> >* celELCM::SwapActiveEntities ()
{
  csSet<csPtrKey<iCelEntity> >* oldActiveEntities = activeEntities;
  if (activeEntities == &activeEntities1)
    activeEntities = &activeEntities2;
  else
    activeEntities = &activeEntities1;
  activeEntities->DeleteAll ();
  return oldActiveEntities;
}

void celELCM::ClearActiveEntities ()
{
  csTicks time = vc->GetCurrentTicks ();

  csSet<csPtrKey<iCelEntity> >* oldActiveEntities = SwapActiveEntities ();

  // Inactivate all active positional entities but keep global active entities.
  csSet<csPtrKey<iCelEntity> >::GlobalIterator it = oldActiveEntities->GetIterator ();
  while (it.HasNext ())
  {
    iCelEntity* entity = it.Next ();
    if (entity->IsPositional ())
    {
      if (!inactiveEntities.Contains (entity))
      {
        inactiveEntities.Put (entity, time);
        entity->Deactivate ();
      }
    }
    else
    {
      activeEntities->Add (entity);
      inactiveEntities.DeleteAll (entity);
      entity->Activate ();
    }
  }
}

iSector* celELCM::GetPlayerPosition (csVector3& pos)
{
  GetPL ();
  iSector* sector;

  if (playerMesh)
  {
    iMovable* movable = playerMesh->GetMesh ()->GetMovable ();
    if (!movable->InSector ())
      return 0;
    sector = movable->GetSectors ()->Get (0);
    pos = movable->GetPosition ();
  }
  else if (playerCamera)
  {
    sector = playerCamera->GetCamera ()->GetSector ();
    pos = playerCamera->GetCamera ()->GetTransform ().GetOrigin ();
  }
  else
  {
    printf ("Error! No camera or mesh found for player!\n");
    fflush (stdout);
    return 0;
  }
  return sector;
}

void celELCM::UpdateActiveEntities ()
{
  csVector3 pos;
  iSector* sector = GetPlayerPosition (pos);

  if (!sector)
  {
    ClearActiveEntities ();
    return;
  }

  csTicks time = vc->GetCurrentTicks ();

  // Set the new anchor.
  prevPos = pos;
  prevSector = sector;

  printf ("Update for %g,%g,%g\n", pos.x, pos.y, pos.z); fflush (stdout);

  csSet<csPtrKey<iCelEntity> >* oldActiveEntities = SwapActiveEntities ();

  // @@@ Problem: this function only finds entities with meshes but ignores
  // entities with no meshes but which still have positional information
  // like 3D sound objects, triggers, ...
  csRef<iMeshWrapperIterator> objit = GetEngine ()->GetNearbyMeshes (sector, pos,
      activityRadius);
  while (objit->HasNext ())
  {
    iMeshWrapper* m = objit->Next ();
    iCelEntity* ent = pl->FindAttachedEntity (m->QueryObject ());
    if (ent)
    {
      activeEntities->Add (ent);
      oldActiveEntities->Delete (ent);
      inactiveEntities.DeleteAll (ent);
      ent->Activate ();
    }
    else
    {
      // @@@ Future support for non-entity meshes?
    }
  }
  // Positional entities that remain in 'oldActiveEntities' (the previous batch
  // of active entities) are now inactive. The others are active.
  csSet<csPtrKey<iCelEntity> >::GlobalIterator it = oldActiveEntities->GetIterator ();
  while (it.HasNext ())
  {
    iCelEntity* ent = it.Next ();
    if (ent->IsPositional ())
    {
      if (!inactiveEntities.Contains (ent))
      {
        inactiveEntities.Put (ent, time);
        ent->Deactivate ();
      }
    }
    else
    {
      activeEntities->Add (ent);
      inactiveEntities.DeleteAll (ent);
      ent->Activate ();
    }
  }
}

void celELCM::CheckUnload ()
{
  printf ("Check unload %d\n", inactiveEntities.GetSize ()); fflush (stdout);
  csTicks time = vc->GetCurrentTicks ();
  csArray<iCelEntity*> toRemove;
  csHash<csTicks,csPtrKey<iCelEntity> >::GlobalIterator it = inactiveEntities.GetIterator ();
  while (it.HasNext ())
  {
    csPtrKey<iCelEntity> ent;
    csTicks t = it.Next (ent);
    // @@@ Overflow in virtual clock!
    if (t+unloadedTime >= time)
    {
      // If the entity was modified since the baseline we can't remove it.
      // @@@ This is dangerous! The function we call here can't really remove
      // the entity because we're still iterating over the inactiveEntities list!
      // Need to think about this.
      printf ("test %p\n", (iCelEntity*)ent); fflush (stdout);
      if (!ent->IsModifiedSinceBaseline ())
        FireELCMListeners (ent);
      toRemove.Push (ent);
    }
  }
  // It is possible that the listeners to the ELCM don't yet manage to
  // remove the entities (or they have reasons to decide against removal).
  // So we reschedule the entities that we deemed right for removal again
  // so this function will check them again later.
  for (size_t i = 0 ; i < toRemove.GetSize () ; i++)
  {
    inactiveEntities.PutUnique (toRemove[i], time);
  }
}

void celELCM::ActivateEntity (iCelEntity* entity)
{
  activeEntities->Add (entity);
  inactiveEntities.DeleteAll (entity);
  entity->Activate ();
}

void celELCM::DeactivateEntity (iCelEntity* entity)
{
  activeEntities->Delete (entity);
  if (!inactiveEntities.Contains (entity))
  {
    inactiveEntities.Put (entity, vc->GetCurrentTicks ());
    entity->Deactivate ();
  }
}

void celELCM::SetPlayer (iCelEntity* entity)
{
  player = entity;
  playerCamera = celQueryPropertyClassEntity<iPcCamera> (player);
  playerMesh = celQueryPropertyClassEntity<iPcMesh> (player);
  UpdateActiveEntities ();
}

void celELCM::SetActivityRadius (float radius)
{
  activityRadius = radius;
  if (player)
    UpdateActiveEntities ();
}

void celELCM::SetDistanceThresshold (float distance)
{
  distanceThresshold = distance;
  sqDistanceThresshold = distance * distance;
}

void celELCM::MarkBaseline ()
{
  deletedEntities.Empty ();
  newEntities.Empty ();
  atBaseline = true;
}

csPtr<iCelEntityIterator> celELCM::GetModifiedEntities () const
{
  csRef<ElcmEntityIterator> it;
  it.AttachNew (new ElcmEntityIterator (activeEntities->GetIterator (),
	inactiveEntities.GetIterator ()));
  return csPtr<iCelEntityIterator> (it);
}

void celELCM::ListenNewEntity (iCelEntity* entity)
{
  // A new entity is automatically made active regardless of where
  // it is in the world and if it even has a world position.
  // If it is a positional entity then it will automatically be made
  // inactive with the next update if needed.
  activeEntities->Add (entity);
  entity->Activate ();
  forcedCheck = true;
  if (atBaseline)
    newEntities.Add (entity);
}

void celELCM::ListenRemoveEntity (iCelEntity* entity)
{
  if (atBaseline)
  {
    if (newEntities.Contains (entity))
      // If this entity is in newEntities it means that it was
      // created after the baseline. In this case we don't have to save
      // it in deletedEntities.
      newEntities.Delete (entity);
    else if (entity->ExistedAtBaseline ())
      // The entity existed at the baseline. So we must register it here.
      deletedEntities.Add (entity->GetID ());
  }
  activeEntities->Delete (entity);
  inactiveEntities.DeleteAll (entity);
}

void celELCM::SetUnloadCheckFrequency (int c)
{
  unloadCheckFrequency = c;
  unload = c;
}

void celELCM::SetUnloadedTime (csTicks t)
{
  unloadedTime = t;
}

void celELCM::AddELCMListener (iELCMListener* listener)
{
  listeners.Push (listener);
}

void celELCM::RemoveELCMListener (iELCMListener* listener)
{
  listeners.Delete (listener);
}

void celELCM::FireELCMListeners (iCelEntity* entity)
{
  for (size_t i = 0 ; i < listeners.GetSize () ; i++)
    listeners[i]->SafeToRemove (entity);
}

void celELCM::Dump ()
{
  printf ("### ELCM ###\n");
  printf ("  Active entities=%d, ", activeEntities->GetSize ());
  printf ("Inactive entities=%d\n", inactiveEntities.GetSize ());
}

//---------------------------------------------------------------------------

