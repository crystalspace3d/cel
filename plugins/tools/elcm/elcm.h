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

#ifndef __CEL_TOOLS_ELCM__
#define __CEL_TOOLS_ELCM__

#include "csutil/util.h"
#include "csutil/set.h"
#include "csutil/hash.h"
#include "iutil/comp.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
#include "physicallayer/propclas.h"
#include "celtool/ticktimer.h"

#include "tools/elcm.h"

struct iEngine;

/**
 * The ELCM.
 */
class celELCM : public scfImplementation2<celELCM, iELCM, iComponent>,
  public celPeriodicTimer
{
private:
  iObjectRegistry* object_reg;
  csRef<iCelPlLayer> pl;
  csRef<iEngine> engine;
  csRef<iVirtualClock> vc;

  float activityRadius;
  float distanceThresshold;
  float sqDistanceThresshold;
  csTicks checkTime;
  bool forcedCheck;
  csTicks unloadedTime;
  int unloadCheckFrequency;
  int unload;

  csRef<iCelEntity> player;
  csRef<iPcCamera> playerCamera;
  csRef<iPcMesh> playerMesh;

  iSector* prevSector;
  csVector3 prevPos;

  csSet<csPtrKey<iCelEntity> > activeEntities1;
  csSet<csPtrKey<iCelEntity> > activeEntities2;
  csSet<csPtrKey<iCelEntity> >* activeEntities;
  csHash<csTicks,csPtrKey<iCelEntity> > inactiveEntities;

  csRefArray<iELCMListener> listeners;

  csSet<csPtrKey<iCelEntity> >* SwapActiveEntities ();
  void ClearActiveEntities ();
  void UpdateActiveEntities ();
  iSector* GetPlayerPosition (csVector3& pos);
  void CheckUnload ();

  void FireELCMListeners (iCelEntity* entity);

public:
  celELCM (iBase* parent);
  virtual ~celELCM ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  virtual void SetPlayer (iCelEntity* entity);
  virtual iCelEntity* GetPlayer () const { return player; }

  virtual void SetActivityRadius (float radius);
  virtual float GetActivityRadius () const { return activityRadius; }
  virtual void SetDistanceThresshold (float distance);
  virtual float GetDistanceThresshold () const { return distanceThresshold; }
  virtual void SetCheckTime (csTicks t);
  virtual csTicks GetCheckTime () const { return checkTime; }
  virtual void SetUnloadCheckFrequency (int c);
  virtual int GetUnloadCheckFrequency () const { return unloadCheckFrequency; }
  virtual void SetUnloadedTime (csTicks t);
  virtual csTicks GetUnloadedTime () const { return unloadedTime; }

  virtual void ActivateEntity (iCelEntity* entity);
  virtual void DeactivateEntity (iCelEntity* entity);
  virtual void AddELCMListener (iELCMListener* listener);
  virtual void RemoveELCMListener (iELCMListener* listener);

  // For celPeriodicTimer
  virtual void Tick ();

  void RegisterNewEntity (iCelEntity* entity);
  void RegisterRemoveEntity (iCelEntity* entity);

  iCelPlLayer* GetPL ();
  iEngine* GetEngine ();
};

#endif // __CEL_TOOLS_ELCM__

