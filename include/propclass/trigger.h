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

#ifndef __CEL_PF_TRIGGER__
#define __CEL_PF_TRIGGER__

#include "cstypes.h"
#include "csgeom/vector3.h"
#include "csgeom/box.h"
#include "csutil/scf.h"
#include "csutil/array.h"

struct iCelEntity;

SCF_VERSION (iPcTrigger, 0, 1, 0);

/**
 * This is a trigger property class. It is capable of detecting when
 * another entity (that has a pcmesh) enters or leaves an area
 * defined by this trigger. In that case it can send messages to the
 * entity that contains this property class and/or the entities
 * that enter/leave the trigger area.
 * <p>
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * <ul>
 * <li>pctrigger_entityenters: an entity enters this trigger (entity).
 * <li>pctrigger_entityleaves: an entity leaves this trigger (entity).
 * <li>pctrigger_entertrigger: this entity enters a trigger (entity).
 * <li>pctrigger_leavetrigger: this entity leaves a trigger (entity).
 * </ul>
 */
struct iPcTrigger : public iBase
{
  /**
   * Setup a spherical area as trigger zone.
   */
  virtual void SetupTriggerSphere (const csVector3& center, float radius) = 0;

  /**
   * Setup a box trigger zone.
   */
  virtual void SetupTriggerBox (const csBox3& box) = 0;

  /**
   * Enable/disable sending messages to the entity containing
   * the trigger about other entities leaving of entering (messages
   * like pctrigger_entityenters and pctrigger_entityleaves).
   * This is enabled by default.
   */
  virtual void EnableMessagesToSelf (bool en) = 0;

  /**
   * Enable/disable sending messages to the entities that
   * enter or leave this trigger (messages
   * like pctrigger_entertrigger and pctrigger_leavetrigger).
   * This is enabled by default.
   */
  virtual void EnableMessagesToOthers (bool en) = 0;

  /**
   * Disable trigger. Triggers are enabled by default.
   */
  virtual void EnableTrigger (bool en) = 0;

  /**
   * Return true if trigger is enabled.
   */
  virtual bool IsEnabled () const = 0;

  /**
   * Get an array of all entities that are currently
   * in the trigger area.
   */
  virtual const csArray<iCelEntity*>& GetEntitiesInTrigger () const = 0;
};

#endif // __CEL_PF_TRIGGER__

