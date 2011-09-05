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
#include "iutil/eventhandlers.h"
#include "csutil/eventhandlers.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
#include "physicallayer/propclas.h"

#include "tools/elcm.h"

struct iEngine;

/**
 * The ELCM.
 */
class celELCM : public scfImplementation2<celELCM, iELCM, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iEventNameRegistry> name_reg;
  csRef<iCelPlLayer> pl;
  csRef<iEngine> engine;

  float activityRadius;
  float distanceThresshold;

  csRef<iCelEntity> player;
  csRef<iPcCamera> playerCamera;
  csRef<iPcMesh> playerMesh;

  csSet<iCelEntity*> activeEntities1;
  csSet<iCelEntity*> activeEntities2;
  csSet<iCelEntity*>* activeEntities;
  csSet<iCelEntity*> inactiveEntities;

  csSet<iCelEntity*>* SwapActiveEntities ();
  void ClearActiveEntities ();
  void UpdateActiveEntities ();

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
  virtual void ActivateEntity (iCelEntity* entity);
  virtual void DeactivateEntity (iCelEntity* entity);

  void RegisterNewEntity (iCelEntity* entity);
  void RegisterRemoveEntity (iCelEntity* entity);

  iCelPlLayer* GetPL ();
  iEngine* GetEngine ();

  // Not an embedded interface to avoid circular references!!!
  class EventHandler : public scfImplementation1<EventHandler,
		       iEventHandler>
  {
  private:
    celELCM* parent;

  public:
    EventHandler (celELCM* parent)
      : scfImplementationType (this), parent (parent)
    {
    }
    virtual ~EventHandler ()
    {
    }

    virtual bool HandleEvent (iEvent& ev)
    {
      //return parent->HandleEvent (ev);
      return false;
    }

    CS_EVENTHANDLER_NAMES("cel.tools.elcm");
    CS_EVENTHANDLER_DEFAULT_INSTANCE_CONSTRAINTS                            
    virtual const csHandlerID * GenericPrec (
	csRef<iEventHandlerRegistry> &, csRef<iEventNameRegistry> &, csEventID) const
    {
      return 0;
    }
    virtual const csHandlerID * GenericSucc (
	csRef<iEventHandlerRegistry> &r1, csRef<iEventNameRegistry> &r2, csEventID event) const
    {
      static csHandlerID succConstraint[2];
      //succConstraint[0] = FrameSignpost_Logic3D::StaticID(r1);
      succConstraint[0] = r1->GetGenericID("cel.propclass.pcinput.standard");
      succConstraint[1] = CS_HANDLERLIST_END;
      return succConstraint;
    }
  } *scfiEventHandler;
};

#endif // __CEL_TOOLS_ELCM__

