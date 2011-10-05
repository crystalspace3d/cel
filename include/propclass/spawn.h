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

#ifndef __CEL_PF_SPAWN__
#define __CEL_PF_SPAWN__

#include "cstypes.h"
#include "csutil/scf.h"

/**
 * Spawn property class. This property class is responsible
 * for creating other entities based on time related rules.
 *
 * This property class supports the following actions (add prefix 'cel.spawn.action.'
 * if you want to access this action through a message):
 * - AddEntityTemplateType: parameters 'chance' (float), 'entity' (string),
 *     'template' (string), 'call' (string)
 * - SetTiming: parameters 'repeat' (bool), 'random' (bool),
 *     'mindelay' (long), 'maxdelay' (long)
 * - ResetTiming
 * - SetEnabled: parameters 'enabled' (bool)
 * - ClearEntityList
 * - Inhibit: parameters 'count' (long)
 * - Spawn
 * - AddSpawnPosition: parameters 'sector' (string), 'yrot' (float),
 *     'position' (vector or name of the node)
 *
 * This property class supports the following properties:
 * - namecounter (bool, read/write): enable/disable unique name counter
 *   (default true).
 * - spawnunique (bool, read/write): enable/disable prevention of spawning
 *   of entities with same name (default false).
 *
 * This property class can send out the following messages:
 * - 'cel.entity.new' (old 'pcspawn_newentity'): new entity has been created (entity,behaviour)
 */
struct iPcSpawn : public virtual iBase
{
  SCF_INTERFACE (iPcSpawn, 1, 1, 0);

  /**
   * Enable/disable spawning. Enabled by default although the spawner
   * will only start working after the first SetTiming() call.
   */
  virtual void SetEnabled (bool e) = 0;
  /**
   * Is spawning enabled.
   */
  virtual bool IsEnabled () const = 0;

  /**
   * Add an entry to the list of entities that can be created by
   * this property class. This version works with templates.
   * \param chance is a number indicating the chance of this entity
   *        being selected (only relevant if 'random' selection is
   *        being used).
   * \param templ is the name of the template from which to create entity.
   * \param name is the name of the entity that will be created.
   * \param msg_id if not 0 then we immediatelly send this message
   *        to the behaviour of the new entity after creating it.
   * \param params is the parameter block that is used to send the
   *        message (can be 0).
   */
  virtual void AddEntityTemplateType (float chance, const char* templ,
  	const char* name, const char* msg_id,
  	iCelParameterBlock* params) = 0;

  /**
   * Clear the list of entities to create.
   */
  virtual void ClearEntityList () = 0;

  /**
   * Set timing information.
   * \param repeat if true then this spawner will keep spawning. Otherwise
   *        it spawns only once unless Reset() is called.
   * \param random if true then we will select a random entity and random
   *        position from our list. Otherwise we select sequentially.
   * \param mindelay is the minimum delay to wait before spawning.
   * \param maxdelay is the maximum delay to wait before spawning.
   */
  virtual void SetTiming (bool repeat, bool random,
  	csTicks mindelay, csTicks maxdelay) = 0;

  /**
   * Reset timing. In case of a non repeating spawner this will enable
   * the spawn again so that it will do a new spawn in the specified time.
   * In case of a repeating spawner it will simply reset the time to zero.
   * In case of non random selection this function will also set the
   * sequence of entities back to the first one.
   */
  virtual void ResetTiming () = 0;

  /**
   * Enable/disable entity name counter. With this enabled (default)
   * every entity will get a unique name formed by the name
   * given in AddEntityTemplateType() and a unique number. With this
   * disabled this will not occur.
   */
  virtual void SetEntityNameCounter (bool en) = 0;

  /**
   * Return true if the unique entity name counter is enabled.
   */
  virtual bool IsEntityNameCounterCounter () const = 0;

  /**
   * This is useful in combination with a disabled entity name
   * counter (SetEntityNameCounter(false)). In spawn unique
   * is true then spawning will not occur if the entity with that
   * name is already in existance. By default this is false.
   */
  virtual void EnableSpawnUnique (bool en) = 0;

  /**
   * Return true if spawn unique is enabled.
   */
  virtual bool IsSpawnUniqueEnabled () const = 0;

  /**
   * Inhibit respawn in case count will reach given number
   * In combination with EnableSpawnUnique() this will ensure that
   * the spawn property class will always try to keep the given
   * number of entities alive.
   */
  virtual void InhibitCount (int number) = 0;

  /**
   * Spawn at this moment any random entity from a list.
   */
  virtual void Spawn () = 0;

  /**
   * Set positional information about where to spawn.
   * This version accepts name of the node
   */
  virtual void AddSpawnPosition (const char* node, float yrot,
  	const char* sector) = 0;

  /**
   * Set positional information about where to spawn.
   * This version accepts position vector
   */
  virtual void AddSpawnPosition (const csVector3& pos, float yrot,
  	const char* sector) = 0;
};

/**
 * This is an interface that can be implemented by an application
 * in order to get more control over how entities should be spawned.
 * If the pcspawn property class discovers that there is an iCelSpawner
 * object in the object registry (with the tag 'cel.spawner') then
 * it will automatically use this instead of creating entities on its
 * own.
 */
struct iCelSpawner : public virtual iBase
{
  SCF_INTERFACE (iCelSpawner, 1, 1, 0);

  /**
   * A new entity has to be created.
   */
  virtual iCelEntity* CreateEntity (iCelEntityTemplate* tpl,
      const char* entityName, iCelParameterBlock* params,
      const csVector3& pos, float yrot, const char* sector,
      const char* node) = 0;
};


#endif // __CEL_PF_SPAWN__

