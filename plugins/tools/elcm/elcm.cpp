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
#include "csutil/inputdef.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
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
      elcm->RegisterNewEntity (entity);
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
      elcm->RegisterRemoveEntity (entity);
  }
};


//--------------------------------------------------------------------------


celELCM::celELCM (iBase* parent)
  : scfImplementationType (this, parent)
{
  scfiEventHandler = 0;

  activityRadius = 500;
  distanceThresshold = 20;

  activeEntities = &activeEntities1;
}

celELCM::~celELCM ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
    if (q != 0)
    {
      q->RemoveListener (scfiEventHandler);
    }
    scfiEventHandler->DecRef ();
  }
}

bool celELCM::Initialize (iObjectRegistry* object_reg)
{
  celELCM::object_reg = object_reg;

  csRef<iEventNameRegistry> namereg = csEventNameRegistry::GetRegistry (
      object_reg);

  scfiEventHandler = new EventHandler (this);
  csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  csEventID esub[] = { 
    csevKeyboardEvent (object_reg),
    csevMouseEvent (object_reg),
    csevFrame (object_reg),
    CS_EVENTLIST_END 
  };
  q->RegisterListener (scfiEventHandler, esub);

  name_reg = csEventNameRegistry::GetRegistry (object_reg);

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
  }
  return pl;
}

csSet<iCelEntity*>* celELCM::SwapActiveEntities ()
{
  csSet<iCelEntity*>* oldActiveEntities = activeEntities;
  if (activeEntities == &activeEntities1)
    activeEntities = &activeEntities2;
  else
    activeEntities = &activeEntities1;
  activeEntities->DeleteAll ();
  return oldActiveEntities;
}

void celELCM::ClearActiveEntities ()
{
  csSet<iCelEntity*>* oldActiveEntities = SwapActiveEntities ();

  // Inactivate all active positional entities but keep global active entities.
  csSet<iCelEntity*>::GlobalIterator it = oldActiveEntities->GetIterator ();
  while (it.HasNext ())
  {
    iCelEntity* entity = it.Next ();
    if (entity->IsPositional ())
    {
      inactiveEntities.Add (entity);
    }
    else
    {
      activeEntities->Add (entity);
      inactiveEntities.Delete (entity);
    }
  }
}

void celELCM::UpdateActiveEntities ()
{
  GetPL ();

  iSector* sector;
  csVector3 pos;

  if (playerMesh)
  {
    iMovable* movable = playerMesh->GetMesh ()->GetMovable ();
    if (!movable->InSector ())
    {
      ClearActiveEntities ();
      return;
    }
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
    printf ("Error! No camera or mesh found for player!");
    fflush (stdout);
    ClearActiveEntities ();
    return;
  }

  csSet<iCelEntity*>* oldActiveEntities = SwapActiveEntities ();

  // @@@ Problem: this function only finds entities with meshes but ignores
  // entities with no meshes but which still have positional information
  // like 3D sound objects, triggers, ...
  csRef<iMeshWrapperIterator> objit = GetEngine ()->GetNearbyMeshes (sector, pos);
  while (objit->HasNext ())
  {
    iMeshWrapper* m = objit->Next ();
    iCelEntity* ent = pl->FindAttachedEntity (m->QueryObject ());
    if (ent)
    {
      activeEntities->Add (ent);
      oldActiveEntities->Delete (ent);
      inactiveEntities.Delete (ent);
    }
    else
    {
      // @@@ Future support for non-entity meshes?
    }
  }
  // All entities that remain in 'oldActiveEntities' (the previous batch
  // of active entities) are now inactive.
  csSet<iCelEntity*>::GlobalIterator it = oldActiveEntities->GetIterator ();
  while (it.HasNext ())
    inactiveEntities.Add (it.Next ());
}

void celELCM::ActivateEntity (iCelEntity* entity)
{
  activeEntities->Add (entity);
  inactiveEntities.Delete (entity);
}

void celELCM::DeactivateEntity (iCelEntity* entity)
{
  activeEntities->Delete (entity);
  inactiveEntities.Add (entity);
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
  UpdateActiveEntities ();
}

void celELCM::SetDistanceThresshold (float distance)
{
  distanceThresshold = distance;
}

void celELCM::RegisterNewEntity (iCelEntity* entity)
{
  // A new entity is automatically made active regardless of where
  // it is in the world and if it even has a world position.
  // If it is a positional entity then it will automatically be made
  // inactive with the next update if needed.
  activeEntities->Add (entity);
}

void celELCM::RegisterRemoveEntity (iCelEntity* entity)
{
  activeEntities->Delete (entity);
  inactiveEntities.Delete (entity);
}

//---------------------------------------------------------------------------
