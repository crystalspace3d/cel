/*
    Crystal Space Entity Layer
    Copyright (C) 2004-2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_MGR_QUEST__
#define __CEL_MGR_QUEST__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "csgeom/vector3.h"
#include "csutil/refarr.h"
#include "tools/rewards.h"
#include "tools/sequences.h"
#include "tools/triggers.h"
#include "physicallayer/pl.h"

struct iDocumentNode;
struct iChangePropertyRewardFactory;
struct iCelParameterBlock;

class celQuestManager;


//-------------------------------------------------------------------------
// The Quest
//-------------------------------------------------------------------------

/**
 * A quest instance. This is created (by the quest manager) from a quest
 * factory using the trigger and reward factories.
 */
struct iQuest : public virtual iBase
{
  SCF_INTERFACE (iQuest, 0, 0, 2);

  /**
   * Switch this quest to some specific state.
   * Returns false if state doesn't exist (nothing happens then).
   */
  virtual bool SwitchState (const char* state) = 0;

  /**
   * Get current state name of this quest.
   */
  virtual const char* GetCurrentState () const = 0;

  /**
   * Find a sequence.
   */
  virtual iCelSequence* FindSequence (const char* name) = 0;

  /**
   * Activate this quest. This means it will process events again.
   * Quests are activated by default.
   */
  virtual void Activate () = 0;

  /**
   * Deactivate this quest. This means that events will no longer be
   * processed.
   */
  virtual void Deactivate () = 0;
};

struct iRewardFactoryArray : public iArrayReadOnly<iRewardFactory*>
{
  SCF_IARRAYREADONLY_INTERFACE(iRewardFactoryArray);
};

/**
 * A trigger with response as used in a quest state.
 * This is basically the representation of one trigger and one
 * or more rewards.
 */
struct iQuestTriggerResponseFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestTriggerResponseFactory, 0, 0, 1);

  /**
   * Set the trigger factory that is managed by this
   * response factory.
   */
  virtual void SetTriggerFactory (iTriggerFactory* trigger_fact) = 0;

  /**
   * Get the trigger factory.
   */
  virtual iTriggerFactory* GetTriggerFactory () const = 0;

  /**
   * Add a reward factory. A reward of this factory will be obtained
   * when the trigger fires.
   */
  virtual void AddRewardFactory (iRewardFactory* reward_fact) = 0;

  /**
   * Get the reward factories.
   */
  virtual csRef<iRewardFactoryArray> GetRewardFactories () const = 0;
};

struct iQuestTriggerResponseFactoryArray : public iArrayReadOnly<iQuestTriggerResponseFactory*>
{
  SCF_IARRAYREADONLY_INTERFACE(iQuestTriggerResponseFactoryArray);
};

/**
 * A representation of a quest state in a quest factory.
 * A state is basically a collection of trigger response factories.
 */
struct iQuestStateFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestStateFactory, 0, 1, 1);

  /**
   * Get the name of this state.
   */
  virtual const char* GetName () const = 0;

  /**
   * Create a new trigger response.
   */
  virtual iQuestTriggerResponseFactory* CreateTriggerResponseFactory () = 0;

  /**
   * Get all trigger responses.
   */
  virtual csRef<iQuestTriggerResponseFactoryArray> GetTriggerResponseFactories () const = 0;

  /**
   * Add a new reward to be fired on state initialization.
   */
  virtual void AddInitRewardFactory (iRewardFactory* reward_fact) = 0;
  /**
   * Get the init reward factories.
   */
  virtual csRef<iRewardFactoryArray> GetInitRewardFactories () const = 0;

  /**
   * Add a new reward to be fired on state exit.
   */
  virtual void AddExitRewardFactory (iRewardFactory* reward_fact) = 0;
  /**
   * Get the exit reward factories.
   */
  virtual csRef<iRewardFactoryArray> GetExitRewardFactories () const = 0;
};

/**
 * Iterator to iterate over the quest factory states.
 */
struct iQuestStateFactoryIterator : public virtual iBase
{
  SCF_INTERFACE (iQuestStateFactoryIterator, 0, 0, 1);

  virtual bool HasNext () const = 0;
  virtual iQuestStateFactory* Next () = 0;
};

/**
 * Iterator to iterate over the quest sequences.
 */
struct iCelSequenceFactoryIterator : public virtual iBase
{
  SCF_INTERFACE (iCelSequenceFactoryIterator, 0, 0, 1);

  virtual bool HasNext () const = 0;
  virtual iCelSequenceFactory* Next () = 0;
};

/**
 * A quest factory. A quest factory is a template to create a quest
 * from. All interfaces ending with 'Factory' are relevant in the concept
 * of a quest factory.
 * <ul>
 * <li>A quest factory is made out of a number of quest state
 *     factories (iQuestStateFactory) (from which the states in the quest
 *     are made).
 * <li>Every quest state factory has one or more trigger response
 *     factories. These response factories describe how the firing of
 *     one trigger (as described by a trigger factory) can cause a number
 *     of rewards (as described by reward factories).
 * </ul>
 */
struct iQuestFactory : public virtual iBase
{
  SCF_INTERFACE (iQuestFactory, 0, 0, 1);

  virtual celQuestManager* GetQuestManager () const = 0;

  /**
   * Get the name of this factory.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Instantiate a quest from this factory. This instance will have
   * the same structure as this factory but it will be made out of
   * non-factory objects (iQuestTrigger, iQuestReward, ...).
   * \param params are the parameters with which this quest is
   * instantiated.
   */
  virtual csPtr<iQuest> CreateQuest (
	  iCelParameterBlock* params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<quest\> node.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;

  /**
   * Get a state from this factory.
   * Return 0 if the state doesn't exist.
   */
  virtual iQuestStateFactory* GetState (const char* name) = 0;

  /**
   * Create a new state in this factory.
   * Return 0 on failure (name already exists).
   */
  virtual iQuestStateFactory* CreateState (const char* name) = 0;

  /**
   * Get an iterator over all the states.
   */
  virtual csRef<iQuestStateFactoryIterator> GetStates () const = 0;

  /**
   * Get a sequence factory in this factory.
   * Return 0 if the factory doesn't exist.
   */
  virtual iCelSequenceFactory* GetSequence (const char* name) = 0;

  /**
   * Create a new sequence factory in this factory.
   * Return 0 on failure (name already exists).
   */
  virtual iCelSequenceFactory* CreateSequence (const char* name) = 0;

  /**
   * Get an iterator over all the sequences.
   */
  virtual csRef<iCelSequenceFactoryIterator> GetSequences () const = 0;

  /**
   * Set a default parameter on this factory.
   * If the parameter exists it will be overwritten.
   */
  virtual void SetDefaultParameter (const char* name,const char* value) = 0;
  
  /**
   * Clear all default parameters on this factory.
   */
  virtual void ClearDefaultParameters () = 0;

};

//-------------------------------------------------------------------------
// The Quest Manager
//-------------------------------------------------------------------------

/**
 * This is the quest manager. This manager can be used to handle quests.
 * A quest is basically a state machine which can advance from state to
 * state when a trigger activates. It can also hand out 'rewards'.
 */
struct iQuestManager : public virtual iBase
{
  SCF_INTERFACE (iQuestManager, 2, 0, 0);

  /**
   * Register a quest trigger type. Quest triggers can be used
   * by quests to decide when to go to another state or when
   * to activate a reward. Returns false on failure (trigger
   * type with that name already exists).
   *
   * The following predefined trigger types are automatically
   * registered in the quest manager:
   * - cel.triggers.entersector: triggers when a camera (from
   *   entity) enters a sector. See iEnterSectorTriggerFactory.
   * - cel.triggers.meshentersector: triggers when a mesh (from
   *   entity) enters a sector. See iEnterSectorTriggerFactory.
   * - cel.triggers.timeout: triggers after a specified time.
   *   See iTimeoutTriggerFactory.
   * - cel.triggers.propertychange: triggers when a property changes.
   *   See iPropertyChangeTriggerFactory.
   * - cel.triggers.sequencefinish: triggers when a sequence finishes.
   *   See iSequenceFinishTriggerFactory.
   * - cel.triggers.trigger: triggers when a pctrigger fires.
   *   See iTriggerTriggerFactory.
   * - cel.triggers.message: triggers when a message is received.
   *   See iMessageTriggerFactory.
   * - cel.triggers.inventory: triggers when an object enters inventory.
   *   See iInventoryTriggerFactory.
   * - cel.triggers.meshselect: triggers when a mesh is selected.
   *   See iMeshSelectTriggerFactory.
   * - cel.triggers.watch: triggers when a mesh becomes visible.
   *   See iWatchTriggerFactory.
   * - cel.triggers.operation: perform a logical operation on several
   *   child nodes.
   *   See iOperationTriggerFactory.
   */
  virtual bool RegisterTriggerType (iTriggerType* trigger) = 0;

  /**
   * Get a trigger type from the quest manager.
   * Returns 0 if no such trigger type exists.
   */
  virtual iTriggerType* GetTriggerType (const char* name) = 0;

  /**
   * Register a quest reward type. Quest rewards can be used
   * by quests to give out some kind of reward to the game.
   * Returns false on failure (reward type with that name
   * already exists).
   *
   * The following predefined reward types are automatically
   * registered in the quest manager:
   * - cel.rewards.debugprint: print a debug message on stdout.
   *   See iDebugPrintRewardFactory.
   * - cel.questreward.newstate: switch to a new state.
   *   See iNewStateQuestRewardFactory.
   * - cel.rewards.changeproperty: change a property.
   *   See iChangePropertyRewardFactory.
   * - cel.rewards.inventory: manipulate inventory.
   *   See iInventoryRewardFactory.
   * - cel.rewards.sequence: fire sequence.
   *   See iSequenceRewardFactory.
   * - cel.rewards.sequencefinish: finish sequence.
   *   See iSequenceFinishRewardFactory.
   * - cel.rewards.message: send a message to some entity.
   *   See iMessageRewardFactory.
   * - cel.rewards.action: send an action to some property class
   *   on an entity. See iActionRewardFactory.
   * - cel.rewards.destroyentity: remove an entity from the physical.
   *   layer. See iDestroyEntityRewardFactory.
   * - cel.rewards.createentity: create an entity from a template.
   *   See iCreateEntityRewardFactory.
   */
  virtual bool RegisterRewardType (iRewardType* trigger) = 0;

  /**
   * Get a reward type from the quest manager.
   * Returns 0 if no such reward type exists.
   */
  virtual iRewardType* GetRewardType (const char* name) = 0;

  /**
   * Register a seqop reward type. Seqop rewards can be used
   * by quest sequences to define operations in the sequence.
   * Returns false on failure (swqop type with that name
   * already exists).
   *
   * The following predefined sequence operation types are automatically
   * registered in the quest manager:
   * - cel.seqops.debugprint: print a debug message on stdout.
   *   See iDebugPrintSeqOpFactory.
   * - cel.seqops.transform: transform a mesh.
   *   See iTransformSeqOpFactory.
   * - cel.seqops.movepath: move a mesh along a path.
   *   See iMovePathSeqOpFactory.
   * - cel.seqops.light: animate a light color.
   *   See iLightSeqOpFactory.
   * - cel.seqops.property: animate a property class property.
   *   See iPropertySeqOpFactory.
   */
  virtual bool RegisterSeqOpType (iSeqOpType* seqop) = 0;

  /**
   * Get a seqop type from the quest manager.
   * Returns 0 if no such seqop type exists.
   */
  virtual iSeqOpType* GetSeqOpType (const char* name) = 0;

  /**
   * Get a quest factory by name.
   */
  virtual iQuestFactory* GetQuestFactory (const char* name) = 0;

  /**
   * Delete a quest factory by name.
   */
  virtual void RemoveQuestFactory (const char* name) = 0;

  /**
   * Delete all quest factories.
   */
  virtual void RemoveQuestFactories () = 0;

  /**
   * Create an empty quest factory.
   * Returns 0 on failure (quest factory with that name
   * already exists).
   */
  virtual iQuestFactory* CreateQuestFactory (const char* name) = 0;

   /**
   * Load a bunch of quest factories.
   * \param node is a node containing \<quest\> children.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;

  /**
   * Convenience method to add a 'newstate' reward factory
   * to a response factory.
   */
  virtual iRewardFactory* AddNewStateReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* state_par) = 0;

  /**
   * Convenience method to add a 'debugprint' reward factory
   * to a response factory.
   */
  virtual iRewardFactory* AddDebugPrintReward (
  	iQuestTriggerResponseFactory* response,
  	const char* msg) = 0;

  /**
   * Convenience method to add an 'inventory' reward factory
   * to a response factory.
   */
  virtual iRewardFactory* AddInventoryReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par) = 0;

  /**
   * Convenience method to add an 'sequence' reward factory
   * to a response factory.
   */
  virtual iRewardFactory* AddSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par,
	const char* delay_par) = 0;


  /**
   * Convenience method to add an 'cssequence' reward factory
   * to a response factory.
   */
  virtual iRewardFactory* AddCsSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* sequence_par, const char* delay_par) = 0;

  /**
   * Convenience method to add an 'sequencefinish' reward factory
   * to a response factory.
   */
  virtual iRewardFactory* AddSequenceFinishReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par) = 0;

  /**
   * Convenience method to add a 'changeproperty' reward factory
   * to a response factory. You need to specify exactly which value
   * should be modified after calling this.
   */
  virtual iChangePropertyRewardFactory* AddChangePropertyReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par) = 0;

  /**
   * Convenience method to add a 'createentity' reward factory
   * to a response factory. You need to specify exactly which template
   * to create a copy of, the name of the created entity and any other 
   * parameters required by the template.
   */
  virtual iRewardFactory* AddCreateEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* template_par,
	const char* name_par,
        iCelParameterBlock* tpl_params) = 0;

    /**
   * Convenience method to add a 'destroyentity' reward factory
   * to a response factory. You need to specify exactly which entity
   * to destroy
   */
  virtual iRewardFactory* AddDestroyEntityReward(
  	iQuestTriggerResponseFactory* response,
	const char* entity_par) = 0;

  /**
   * Convenience method to add an 'action' reward factory
   * to a response factory. You need to specify exactly which entity
   * and which action
   */
  virtual iRewardFactory* AddActionReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par,
	const char* id_par,
	const char* pcclass_par) = 0;

  /**
   * Convenience method to add a 'message' reward factory
   * to a response factory. You need to specify the message 
   * and which entity to send it to.
   */
  virtual iRewardFactory* AddMessageReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par,
	const char* id_par) = 0;

  /**
   * Convenience method to set a 'timeout' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetTimeoutTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* timeout_par) = 0;

  /**
   * Convenience method to set an 'entersector' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par) = 0;

  /**
   * Convenience method to set a 'meshentersector' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetMeshEnterSectorTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par) = 0;

  /**
   * Convenience method to set a 'sequencefinish' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetSequenceFinishTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par) = 0;

  /**
   * Convenience method to set a 'propertychange' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetPropertyChangeTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par,
	const char* value_par) = 0;

  /**
   * Convenience method to set a 'trigger' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetTriggerTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, bool do_leave = false) = 0;

  /**
   * Convenience method to set a 'watch' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetWatchTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* target_entity_par,
	const char* checktime_par,
	const char* radius_par) = 0;

  /**
   * Convenience method to set a 'operation' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetOperationTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* operation_par, 
	csRefArray<iTriggerFactory> &trigger_factories) = 0;

   /**
   * Convenience method to set an 'inventory' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetInventoryTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par,
	const char* child_par) = 0;

   /**
   * Convenience method to set an 'message' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetMessageTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par,
	const char* mask_par) = 0;

  /**
   * Convenience method to set an 'meshsel' trigger factory
   * to a response factory.
   */
  virtual iTriggerFactory* SetMeshSelectTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par) = 0;

};


//-------------------------------------------------------------------------
// Specific reward implementations.
//-------------------------------------------------------------------------


/**
 * This interface is implemented by the reward that switches
 * to another state. You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this reward type is 'cel.questreward.newstate'.
 *
 * In XML, factories recognize the following attributes on the 'reward' node:
 * - <em>state</em>: the new state.
 * - <em>entity</em>: the name of the entity containing the
 *   pcquest property class.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 */
struct iNewStateQuestRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iNewStateQuestRewardFactory, 0, 0, 1);

  /**
   * Set the state to go to (either a state string
   * or a parameter if it starts with '$').
   * \param state is the name of the state or a parameter (starts
   * with '$').
   */
  virtual void SetStateParameter (const char* state) = 0;
  virtual const char* GetStateParameter () const = 0;

  /**
   * Set the name of the entity containing the pcquest property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;
  virtual const char* GetEntityParameter () const = 0;

  /**
   * Set the tag of the property class this reward will apply to.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetTagParameter (const char* tag_par) = 0;
  virtual const char* GetTagParameter () const = 0;

  /**
   * Set the name of the entity class containing the property
   * class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;
  virtual const char* GetClassParameter () const = 0;
};



//-------------------------------------------------------------------------

/**
 * Convenience to declare a new quest specific reward type class.
 */
#define CEL_DECLARE_QUESTREWARDTYPE(name,id)					\
class cel##name##RewardType : public scfImplementation1<		\
		cel##name##RewardType,iRewardType>			\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  csWeakRef<iCelPlLayer> pl;						\
  cel##name##RewardType (iObjectRegistry* object_reg);			\
  virtual ~cel##name##RewardType () { }					\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iRewardFactory> CreateRewardFactory ();		\
};

/**
 * Convenience to implement a new quest specific reward type class.
 */
#define CEL_IMPLEMENT_QUESTREWARDTYPE(name)					\
cel##name##RewardType::cel##name##RewardType (				\
	iObjectRegistry* object_reg) : scfImplementationType (this)	\
{									\
  cel##name##RewardType::object_reg = object_reg;			\
  pl = csQueryRegistry<iCelPlLayer> (object_reg);			\
}									\
csPtr<iRewardFactory> cel##name##RewardType::CreateRewardFactory ()\
{									\
  cel##name##RewardFactory* fact = new					\
  	cel##name##RewardFactory (this);				\
  return fact;								\
}

//-------------------------------------------------------------------------

CS_DEPRECATED_METHOD_MSG("Use iParameter instead")
typedef iParameter iQuestParameter;
CS_DEPRECATED_METHOD_MSG("Use iTriggerCallback instead")
typedef iTriggerCallback iQuestTriggerCallback;
CS_DEPRECATED_METHOD_MSG("Use iTrigger instead")
typedef iTrigger iQuestTrigger;
CS_DEPRECATED_METHOD_MSG("Use iTriggerFactory instead")
typedef iTriggerFactory iQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iTriggerType instead")
typedef iTriggerType iQuestTriggerType;
CS_DEPRECATED_METHOD_MSG("Use iReward instead")
typedef iReward iQuestReward;
CS_DEPRECATED_METHOD_MSG("Use iRewardFactory instead")
typedef iRewardFactory iQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iRewardType instead")
typedef iRewardType iQuestRewardType;
CS_DEPRECATED_METHOD_MSG("Use iSeqOp instead")
typedef iSeqOp iQuestSeqOp;
CS_DEPRECATED_METHOD_MSG("Use iSeqOpFactory instead")
typedef iSeqOpFactory iQuestSeqOpFactory;
CS_DEPRECATED_METHOD_MSG("Use iSeqOpType instead")
typedef iSeqOpType iQuestSeqOpType;
CS_DEPRECATED_METHOD_MSG("Use iCelSequenceCallback instead")
typedef iCelSequenceCallback iQuestSequenceCallback;
CS_DEPRECATED_METHOD_MSG("Use iCelSequence instead")
typedef iCelSequence iQuestSequence;
CS_DEPRECATED_METHOD_MSG("Use iCelSequenceFactory instead")
typedef iCelSequenceFactory iQuestSequenceFactory;
CS_DEPRECATED_METHOD_MSG("Use iTimeoutTriggerFactory instead")
typedef iTimeoutTriggerFactory iTimeoutQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iPropertyChangeTriggerFactory instead")
typedef iPropertyChangeTriggerFactory iPropertyChangeQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iMeshSelectTriggerFactory instead")
typedef iMeshSelectTriggerFactory iMeshSelectQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iInventoryTriggerFactory instead")
typedef iInventoryTriggerFactory iInventoryQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iEnterSectorTriggerFactory instead")
typedef iEnterSectorTriggerFactory iEnterSectorQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iOperationTriggerFactory instead")
typedef iOperationTriggerFactory iOperationQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iSequenceFinishTriggerFactory instead")
typedef iSequenceFinishTriggerFactory iSequenceFinishQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iTriggerTriggerFactory instead")
typedef iTriggerTriggerFactory iTriggerQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iMessageTriggerFactory instead")
typedef iMessageTriggerFactory iMessageQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iWatchTriggerFactory instead")
typedef iWatchTriggerFactory iWatchQuestTriggerFactory;
CS_DEPRECATED_METHOD_MSG("Use iDebugPrintRewardFactory instead")
typedef iDebugPrintRewardFactory iDebugPrintQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iChangePropertyRewardFactory instead")
typedef iChangePropertyRewardFactory iChangePropertyQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iInventoryRewardFactory instead")
typedef iInventoryRewardFactory iInventoryQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iCsSequenceRewardFactory instead")
typedef iCsSequenceRewardFactory iCsSequenceQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iSequenceRewardFactory instead")
typedef iSequenceRewardFactory iSequenceQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iSequenceFinishRewardFactory instead")
typedef iSequenceFinishRewardFactory iSequenceFinishQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iMessageRewardFactory instead")
typedef iMessageRewardFactory iMessageQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iActionRewardFactory instead")
typedef iActionRewardFactory iActionQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iDestroyEntityRewardFactory instead")
typedef iDestroyEntityRewardFactory iDestroyEntityQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iCreateEntityRewardFactory instead")
typedef iCreateEntityRewardFactory iCreateEntityQuestRewardFactory;
CS_DEPRECATED_METHOD_MSG("Use iDebugPrintSeqOpFactory instead")
typedef iDebugPrintSeqOpFactory iDebugPrintQuestSeqOpFactory;
CS_DEPRECATED_METHOD_MSG("Use iTransformSeqOpFactory instead")
typedef iTransformSeqOpFactory iTransformQuestSeqOpFactory;
CS_DEPRECATED_METHOD_MSG("Use iMovePathSeqOpFactory instead")
typedef iMovePathSeqOpFactory iMovePathQuestSeqOpFactory;
CS_DEPRECATED_METHOD_MSG("Use iLightSeqOpFactory instead")
typedef iLightSeqOpFactory iLightQuestSeqOpFactory;
CS_DEPRECATED_METHOD_MSG("Use iPropertySeqOpFactory instead")
typedef iPropertySeqOpFactory iPropertyQuestSeqOpFactory;

#endif // __CEL_MGR_QUEST__
