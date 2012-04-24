/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2011 by Jorrit Tyberghein
  
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
#include "csutil/set.h"
#include "csutil/strset.h"

struct iCelPropertyClassList;
struct iCelBehaviour;
struct iCelEntityIterator;
struct iMessageChannel;
struct iMessageDispatcher;
struct iMessageSender;
struct iObject;
struct iStringSet;
struct iCelCompactDataBufferWriter;
struct iCelCompactDataBufferReader;

struct iSector;
struct iMovable;

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
struct iCelEntity : public virtual iBase
{
  SCF_INTERFACE (iCelEntity, 0, 0, 7);

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
   * \param n A uint to uniquely represent this entity.
   */
  virtual void SetID (uint n) = 0;

  /**
   * Set the ID for the template name. Normally this is not needed as this
   * is done by the physical layer automatically if the entity is created
   * from a template.
   */
  virtual void SetTemplateNameID (csStringID id) = 0;

  /**
   * Get the Id of the template that created this entity.
   * Returns csInvalidStringID if this entity was not created from a template.
   */
  virtual csStringID GetTemplateNameID () const = 0;

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

  /**
   * Add a class to this entity. A class is an application defined
   * attribute which indicates to what the entity belongs. It can be something
   * like 'weapon' or 'creature' or whatever the application desires.
   */
  virtual void AddClass (csStringID cls) = 0;

  /**
   * Remove a class from this entity.
   */
  virtual void RemoveClass (csStringID cls) = 0;

  /**
   * Check if this entity belongs to some class.
   */
  virtual bool HasClass (csStringID cls) = 0;

  /**
   * Return the set of classes for this entity.
   */
  virtual const csSet<csStringID>& GetClasses () const = 0;

  /**
   * An entity is a message channel. Instead of doing scfQueryInterface to
   * fetch the message channel you can also use this function.
   */
  virtual iMessageChannel* QueryMessageChannel () = 0;

  /**
   * This is a special version of CreateMessageDispatcher() as it can
   * be found in the channel. This version only allows receivers that
   * are property classes that have a certain tag.
   */
  virtual csRef<iMessageDispatcher> CreateTaggedMessageDispatcher (
      iMessageSender* sender, csStringID msg_id,
      const char* tag) = 0;

  /**
   * Returns true if this entity is 'positional'. That means that it
   * has at least one property class which implements iCelPositionInfo.
   */
  virtual bool IsPositional () const = 0;

  /**
   * Activate this entity. This means it will process events again
   * (including all property classes and other things attached to this entity).
   * Entities are activated by default.
   */
  virtual void Activate () = 0;

  /**
   * Deactivate this entity. This means that events will no longer be processed
   * and the entity will be 'frozen'.
   */
  virtual void Deactivate () = 0;

  /**
   * Return true if the entity is active.
   */
  virtual bool IsActive () const = 0;

  /**
   * Mark the baseline for this entity. This means that the status of this
   * entity as it is now doesn't have to be saved. Only changes to the entity
   * that happen after this baseline have to be modified. This function will
   * delegate to the property classes.
   */
  virtual void MarkBaseline () = 0;

  /**
   * Return true if there is any property class that has been modified since
   * the baseline.
   */
  virtual bool IsModifiedSinceBaseline () const = 0;

  /**
   * Returns true if this entity existed at the baseline.
   */
  virtual bool ExistedAtBaseline () const = 0;

  /**
   * Return the data that represents the information that changed after the
   * baseline. Only the modified property classes are saved here.
   */
  virtual void SaveModifications (iCelCompactDataBufferWriter* buf, iStringSet* strings) = 0;

  /**
   * Call this function if the entity is in the state as it was at the moment of
   * the baseline. This function will put back the modifications that were made
   * after the baseline.
   */
  virtual void RestoreModifications (iCelCompactDataBufferReader* buf,
      const csHash<csString,csStringID>& strings) = 0;
};

/**
 * A list of entities.
 *
 * An iCelEntityList represents a list of entities.  It defines an interface
 * to Add and Remove entities to or from the list, and also to find either 
 * the index of an Entity in the list, or get a pointer to an Entity in the
 * list.
 */
struct iCelEntityList : public virtual iBase
{
  SCF_INTERFACE (iCelEntityList, 0, 0, 2);

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

  /**
   * Get an iterator over the entities in this list.
   *
   * \return an iterator over the entities.
   */
  virtual csPtr<iCelEntityIterator> GetIterator () const = 0;
};

/**
 * An iterator for entities.
 */
struct iCelEntityIterator : public virtual iBase
{
  SCF_INTERFACE (iCelEntityIterator, 0, 0, 1);

  /**
   * Move forward, returning the next entity in the iterator.
   */
  virtual iCelEntity* Next () = 0;

  /**
   * Check if we have any more entities to iterate through.
   */
  virtual bool HasNext () const = 0;
};

#endif // __CEL_PL_ENTITY__

