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

#ifndef __CEL_ELCM__
#define __CEL_ELCM__

struct iCelEntity;

/**
 * The Entity Life Cycle Manager.
 * This plugin is responsible for maintaining the life cycle of every entity.
 * 
 * Definitions:
 * - Positional entity: this is an entity that has a position in the world
 *   (for example, it has an associated pcmesh or pctrigger).
 * - Global entity: this is an entity that has no position in the world.
 *
 * - Persistent entity: this entity maintains full state at all times during
 *   the game (until it is deleted).
 * - Transient entity: this entity can be recreated fully from a template
 *   (with optional parameters). It's state is only important when the
 *   entity is 'active' or near the player and for some time after that.
 *
 * ELCM maintains a specific state for every entity:
 * - Active: entity is in memory and handling events. This is the state of
 *   all positional entities which are near the player and also the state
 *   of all global entities.
 * - Inactive: entity is in memory but not handling events. This occurs
 *   when the entity is far away from the player.
 * - Unloaded: entity is deleted from memory (but still present in the game).
 *   Inactive entities go to this state after a specific amount of time.
 * - Inventory: the entity is part of an inventory. The entity will not handle
 *   events and in case it is a transient entity it will be converted to
 *   a template reference (+ parameters).
 * - Deleted: the entity is removed from the game and no longer has any
 *   importance.
 *
 * ELCM also maintains which entities have been changed since a specific
 * snapshot time. This can be used to generate differential save games.
 * It also keeps track of which entities (present at snapshot time) were
 * deleted later.
 *
 * Note: global entities (entities with no position) will never be set to
 * inactive automatically but have to be activated and inactivated manually (if
 * needed).
 */
struct iELCM: public virtual iBase
{
  SCF_INTERFACE (iELCM, 0, 0, 1);

  /**
   * Register the player entity. The ELCM will use this entity to find out
   * where the player is (and thus to calculate the set of active entities).
   */
  virtual void SetPlayer (iCelEntity* entity) = 0;
  virtual iCelEntity* GetPlayer () const = 0;

  /**
   * Set the activity radius. This is the area around the player where
   * entities are considered to be 'active'. The default activity radius
   * is 500 units.
   */
  virtual void SetActivityRadius (float radius) = 0;
  virtual float GetActivityRadius () const = 0;

  /**
   * Set the distance thresshold before we recalculate the set of active
   * entities. Default is 20.
   */
  virtual void SetDistanceThresshold (float distance) = 0;
  virtual float GetDistanceThresshold () const = 0;

  /**
   * Set the frequency with which the ELCM will recheck if it needs to
   * recalculate the set of active entities. Default is 100ms.
   */
  virtual void SetCheckTime (csTicks t) = 0;
  virtual csTicks GetCheckTime () const = 0;

  /**
   * Manually activate an entity. This is mostly useful for a global entity
   * which has recently been inactivated.
   */
  virtual void ActivateEntity (iCelEntity* entity) = 0;

  /**
   * Manually deactivate an entity. This is mostly useful for deactivating
   * global entities.
   */
  virtual void DeactivateEntity (iCelEntity* entity) = 0;
};

#endif // __CEL_ELCM__

