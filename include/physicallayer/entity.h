/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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

#ifndef __CEL_PL_ENTITY__
#define __CEL_PL_ENTITY__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCelPropertyClassList;
struct iCelBehaviour;
struct iObject;

SCF_VERSION (iCelEntity, 0, 0, 1);

/**
 * This is an entity in the CEL layer at the PL (physical layer) side.
 *
 * Every object in the game is represented by an entity.  The entity system
 * allows the game programmer (using CEL) to speak to game objects in a 
 * consistent manner. Examples of entities are the player, the world itself,
 * creates, a candle, objects in general and items. 
 *
 * Entities are generally created, and destroyed, as required by the 
 * Physical Layer at the request of the Behaviour Layer.
 */
struct iCelEntity : public iBase
{
  /**
   * Get the iObject for this entity (if supported).
   * \return A pointer to the iObject for this entity if supported,
   * otherwise 0.
   */
  virtual iObject* QueryObject () = 0;

  /**
   * Get the name of this entity.
   * \return A pointer to the name of this entity.
   */
  virtual const char* GetName () const = 0;

  /**
   * Set the name of this entity.
   * \param n A pointer to the name to set for this entity.
   */
  virtual void SetName (const char* n) = 0;

  /**
   * Return the (unique) ID of the entity.
   * \return A uint uniquely representing this entity.
   */
  virtual uint GetID () const = 0;

  /**
   * Set the (unique) ID of the entity.
   * @@@ UGLY! This should be removed as soon as we find another solution
   * for this problem!!!! See history.txt entry of 23-Jan-2003.
   * \param n A uint to uniquely represent this entity.
   */
  virtual void SetID (uint n) = 0;

  /**
   * Get the list of property classes.
   * \return An pointer to an iCelPropertyClassList containing a list of the
   * property classes for this entity.
   */
  virtual iCelPropertyClassList* GetPropertyClassList () = 0;

  /**
   * Set the behaviour layer entity associated with this entity.
   * \param behaviour A pointer to the iCelBehaviour object representing the
   * behaviour layer for this entity.
   */
  virtual void SetBehaviour (iCelBehaviour* behaviour) = 0;

  /**
   * Get the behaviour layer entity associated with this entity.
   * \return A pointer to the iCelBehaviour object representing the behaviour
   * layer for this entity.
   */
  virtual iCelBehaviour* GetBehaviour () = 0;
};

SCF_VERSION (iCelEntityList, 0, 0, 1);

/**
 * A list of entities.
 *
 * An iCelEntityList represents a list of entities.  It defines an interface
 * to Add and Remove entities to or from the list, and also to find either 
 * the index of an Entity in the list, or get a pointer to an Entity in the
 * list.
 */
struct iCelEntityList : public iBase
{
  /**
   * Get the number of entities in this list.
   *
   * \return The number of entities in the list, as an integer.
   */
  virtual size_t GetCount () const = 0;

  /**
   * Return an entity class by index.
   *
   * Get will return a pointer to the iCelEntity at index n in the list.
   *
   * \param n An index in the list
   * \return A pointer to the iCelEntity at that location in the list.
   */
  virtual iCelEntity* Get (size_t n) const = 0;

  /**
   * Add an entity.
   *
   * Add will take a pointer to an iCelEntity and add this Entity to the list.
   *
   * \param obj A pointer to an iCelEntity to be added to the list.
   * \return ???
   */
  virtual size_t Add (iCelEntity* obj) = 0;

  /**
   * Remove an entity.
   *
   * Remove will remove the iCelEntity pointed to by obj from the list.
   *
   * \param obj A pointer to an iCelEntity to be removed from the list
   * \return True upon Success, or False upon Failure
   */
  virtual bool Remove (iCelEntity* obj) = 0;

  /**
   * Remove the nth entity.
   * 
   * Remove will remove the iCelEntity at index n from the list.
   *
   * \param n An index in the list
   * \return True upon Success, or False upon Failure
   */
  virtual bool Remove (size_t n) = 0;

  /**
   * Remove all entities.
   *
   * RemoveAll will remove ALL Entities from the list.
   */
  virtual void RemoveAll () = 0;

  /**
   * Find an entity and return its index.
   *
   * Find will return the index to the Entity pointed to by obj in the list.
   *
   * \param obj A pointer to an iCelEntity to find in the list
   * \return The Entities index within the list
   */
  virtual size_t Find (iCelEntity* obj) const = 0;

  /**
   * Find an entity by name.
   *
   * FindByName will return the index to the Entity named by Name in the list.
   *
   * \param Name A name of an Entity to find in the list
   * \return A pointer to the iCelEntity named
   */
  virtual iCelEntity* FindByName (const char *Name) const = 0;
};

#endif // __CEL_PL_ENTITY__

