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

SCF_VERSION (iPcSpawn, 1, 0, 0);

/**
 * Spawn property class. This property class is responsible
 * for creating other entities based on time related rules.
 * <p>
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * <ul>
 * <li>pcspawn_newentity: new entity has been created (entity,behaviour)
 * </ul>
 */
struct iPcSpawn : public iBase
{
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
   * this property class.
   * \param chance is a number indicating the chance of this entity
   *        being selected (only relevant if 'random' selection is
   *        being used).
   * \param name is the name of the entity that will be created.
   * \param bl is the behaviour layer (can be 0).
   * \param behaviour is the name of the behaviour (can be 0).
   * \param msg_id if not 0 then we immediatelly send this message
   *        to the behaviour of the new entity after creating it.
   * \param params is the parameter block that is used to send the
   *        message (can be 0).
   * \param ... is a list of property class names.
   */
  virtual void AddEntityType (float chance, const char* name, iCelBlLayer* bl,
  	const char* behaviour, const char* msg_id,
  	iCelParameterBlock* params, ...) = 0;

  /**
   * Add an entry to the list of entities that can be created by
   * this property class. This version works with templates.
   * \param chance is a number indicating the chance of this entity
   *        being selected (only relevant if 'random' selection is
   *        being used).
   * \param templ is the name of the template from which to create entity.
   * \param name is the name of the entity that will be created.
   *        (can be 0 then got template name)
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
   * \param random if true then we will select a random entity from our list.
   *        Otherwise we select sequentially.
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
   * Inhibit respawn in case count will reach given number
   */
  virtual void InhibitCount (int number) = 0;

  /**
   * Spawn now random entity from list.
   */
  virtual void Spawn () = 0;
};

#endif // __CEL_PF_SPAWN__

