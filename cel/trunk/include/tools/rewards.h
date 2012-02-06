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

#ifndef __CEL_REWARDS__
#define __CEL_REWARDS__

#include "behaviourlayer/behave.h"
#include "iutil/document.h"
#include "tools/parameters.h"
#include "tools/sequences.h"

struct iQuest;
struct iCelParameterBlock;
struct iRewardType;

//-------------------------------------------------------------------------
// Rewards
//-------------------------------------------------------------------------

/**
 * This is a reward. You can implement your own rewards in addition 
 * to the standard rewards already implemented. You also need to implement
 * a reward factory then (see iRewardFactory).
 */
struct iReward : public virtual iBase
{
  SCF_INTERFACE (iReward, 0, 0, 1);
  /**
   * Perform this reward.
   */
  virtual void Reward (iCelParameterBlock* params) = 0;
};

/**
 * A factory to create rewards. Reward factories are created
 * by an iRewardType instance.
 */
struct iRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iRewardFactory, 0, 0, 1);

  /**
   * Get the reward type for this factory.
   */
  virtual iRewardType* GetRewardType () const = 0;

  /**
   * Create a reward.
   * \param quest is the quest for which we are creating this reward.
   * \param params are the parameters with which this reward is
   * instantiated.
   */
  virtual csPtr<iReward> CreateReward (iQuest* q, iCelParameterBlock* params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<reward\> node in a reward description.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;

  /**
   * Save this factory to a document node.
   * \param node is the \<reward\> node in a reward description.
   * \return false on error (reporter is used to report).
   */
  virtual bool Save (iDocumentNode* node) = 0;
};

/**
 * The reward type is responsible for the creation of reward factories.
 * If you want to define a new type of reward then you must implement
 * iQuestRewardType, iQuestRewardFactory, and iQuestReward.
 * You register reward types with the quest manager so that they can
 * be used by quest factories.
 */
struct iRewardType : public virtual iBase
{
  SCF_INTERFACE (iRewardType, 0, 0, 1);

  /**
   * Return the name for this reward type.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Create a reward factory.
   */
  virtual csPtr<iRewardFactory> CreateRewardFactory () = 0;
};


//-------------------------------------------------------------------------
// Specific reward implementations.
//-------------------------------------------------------------------------

/**
 * This interface is implemented by the reward that sends an action
 * to some entity or entity class property class with an optional tag.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.rewards.action'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity to send the action to.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the 
 *   given entity class.
 * - <em>id</em>: name of the action to activate.
 * - <em>pc</em>: the name of the property class to send the action to.
 * - <em>tag</em>: the tag of the property class to send the action to.
 */
struct iActionRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iActionRewardFactory, 0, 1, 0);

  /**
   * Set the name of the entity.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the action name.
   * \param id is the action name or a parameter (starts with '$').
   */
  virtual void SetIDParameter (const char* id) = 0;

  /**
   * Set the name of the property class.
   * \param propertyclass is the name of the propertyclass or a parameter
   * (starts with '$').
   */
  virtual void SetPropertyClassParameter (const char* propertyclass) = 0;

  /**
   * Set the tag for the propertyclass.
   * \param tag is the tag for the propertyclass or a parameter (starts
   * with '$').
   */
  virtual void SetTagParameter (const char* tag) = 0;

  /**
   * Add a parameter to send with the action.
   * \param type is one of CEL_DATA_STRING, CEL_DATA_FLOAT, CEL_DATA_LONG,
   * CEL_DATA_BOOL, or CEL_DATA_VECTOR3.
   * \param id is the id of the parameter (calculated from iCelPlLayer->FetchStringID(name)).
   * \param value is the value string or a parameter for it (starts with '$').
   */
  virtual void AddParameter (celDataType type, csStringID id, const char* value) = 0;
};

/**
 * This interface is implemented by the reward that changes the value
 * of a property (either on a property from pcproperties or a generic
 * property on any property class). You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this reward type is 'cel.rewards.changeproperty'.
 *
 * In XML, factories recognize the following attributes on the 'reward' node:
 * - <em>entity</em>: the name of the entity containing the
 *   pcproperties property class.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 * - <em>pc</em>: the name of the property class. If this is not
 *   given then pcproperties is used.
 * - <em>tag</em>: used together with 'pc' to specify an optional tag.
 * - <em>property</em>: the name of the property.
 * - <em>string</em>: the new string value of the property.
 * - <em>long</em>: the new long value of the property.
 * - <em>float</em>: the new float value of the property.
 * - <em>bool</em>: the new bool value of the property.
 * - <em>diff</em>: this long or float value will be added to the property.
 * - <em>toggle</em>: toggle the boolean or long value.
 */
struct iChangePropertyRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iChangePropertyRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcproperties property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity class containing the property
   * class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the name of the property class and tag. If this is not
   * given (or property class name is 0) then pcproperties will
   * be used.
   * \param pc is the name of the property class or a parameter (starts
   * with '$').
   * \param tag is the name of the tag or a parameter (starts
   * with '$').
   */
  virtual void SetPCParameter (const char* pc, const char* tag) = 0;

  /**
   * Set the name of the property.
   * \param prop is the name of the property or a parameter (starts
   * with '$').
   */
  virtual void SetPropertyParameter (const char* prop) = 0;

  /**
   * Set the string value.
   * \param pstring is the string or a parameter (starts with '$').
   */
  virtual void SetStringParameter (const char* pstring) = 0;

  /**
   * Set the long value.
   * \param plong is the long or a parameter (starts with '$').
   */
  virtual void SetLongParameter (const char* plong) = 0;

  /**
   * Set the float value.
   * \param pfloat is the float or a parameter (starts with '$').
   */
  virtual void SetFloatParameter (const char* pfloat) = 0;

  /**
   * Set the boolean value.
   * \param pbool is the bool or a parameter (starts with '$').
   */
  virtual void SetBoolParameter (const char* pbool) = 0;

  /**
   * Set the diff.
   * \param pdiff is the long/float or a parameter (starts with '$').
   */
  virtual void SetDiffParameter (const char* pdiff) = 0;

  /**
   * Set the toggle.
   */
  virtual void SetToggle () = 0;
};

/**
 * This interface is implemented by the createentity reward, which can create
 * entities from entity templates.
 *
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.rewards.createentity'.
 *
 * In XML, factories recognize the following attribute on the 'op' node:
 * - <em>template</em>: the name of the template that will be used to create
 *   the entity from. It can be a parameter if starting with '$'.
 * - <em>name</em>: optional name for the entity that will be created. It
 *   can be a parameter if starting with '$'.
 * Also, the following subnodes are supported:
 * - <em>par</em>: a parameter for the template. It needs 'name' and 'value'
 *   attributes. The value can be a parameter if starting with '$'. As many
 *   par nodes as needed can be used.
 */
struct iCreateEntityRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iCreateEntityRewardFactory, 0, 0, 1);

  /**
   * Set the name of the template that will be used to create a new entity
   * from.
   * \param entity_tpl is the name of the entity template or a parameter 
   * (starts with '$').
   */
  virtual void SetEntityTemplateParameter (const char* entity_tpl) = 0;

  /**
   * Set the name of the entity that will be created.
   * \param name is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetNameParameter (const char* name) = 0;

  /**
   * Add a parameter for the template.
   * \param name is the name for the parameter.
   * \param value is the value for the parameter or a quest manager parameter 
   * (starts with '$').
   */
  virtual void AddParameter (const char* name, const char* value) = 0;
};

/**
 * This interface is implemented by the reward that destroys an entity.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.rewards.destroyentity'.
 *
 * In XML, factories recognize the following attribute on the 'op' node:
 * - <em>entity</em>: the name of the entity to send the message too.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 */
struct iDestroyEntityRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iDestroyEntityRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;


};

/**
 * This interface is implemented by the reward that prints
 * debug messages on standard output. You can query this interface
 * from the reward factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this reward type is 'cel.rewards.debugprint'.
 *
 * In XML, factories recognize the following attributes on the 'reward' node:
 * - <em>message</em>: the message to print.
 */
struct iDebugPrintRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iDebugPrintRewardFactory, 0, 0, 1);

  /**
   * Set the message parameter to print (either a message string
   * or a parameter if it starts with '$').
   */
  virtual void SetMessageParameter (const char* msg) = 0;
};

/**
 * This interface is implemented by the reward that manipulates the inventory.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.rewards.inventory'.
 *
 * In XML, factories recognize the following attributes on the 'reward' node:
 * - <em>entity</em>: the name of the entity containing the
 *   pcinventory property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>child_entity</em>: the name of the entity that will
 *   be added to or removed from the inventory.
 * - <em>child_entity_tag</em>: optional tag used to find the right
 *   property class from the entity. This will be used to find the
 *   pcmesh for hiding the mesh.
 */
struct iInventoryRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iInventoryRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcinventory property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the entity that will be put in or out the inventory.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetChildEntityParameter (const char* entity,
  	const char* tag = 0) = 0;
};

/**
 * This interface is implemented by the reward that sends a message
 * to some entity (the behaviour will get this message).
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.rewards.message'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity to send the message too.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the
 *   given entity class.
 * - <em>id</em>: id of the message to send.
 */
struct iMessageRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iMessageRewardFactory, 0, 1, 0);

  /**
   * Set the name of the entity.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the entity class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the message id.
   * \param id is the message id or a parameter (starts with '$').
   */
  virtual void SetIDParameter (const char* id) = 0;

  /**
   * Add a parameter to send with the message.
   * \param type is one of CEL_DATA_STRING, CEL_DATA_FLOAT, CEL_DATA_LONG,
   * CEL_DATA_BOOL, or CEL_DATA_VECTOR3.
   * \param id is the id of the parameter (calculated from iCelPlLayer->FetchStringID(name)).
   * \param value is the value string or a parameter for it (starts with '$').
   */
  virtual void AddParameter (celDataType type, csStringID id, const char* value) = 0;
};

/**
 * This interface is implemented by the reward that fires a Crystal
 * Space sequence.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.rewards.cssequence'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>sequence</em>: the name of the Crystal Space sequence.
 * - <em>delay</em>: delay before we start the sequence. Default is 0.
 */
struct iCsSequenceRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iCsSequenceRewardFactory, 0, 0, 1);

  /**
   * Set the name of the sequence.
   * \param sequence is the name of the sequence or a parameter (starts
   * with '$').
   */
  virtual void SetSequenceParameter (const char* sequence) = 0;

  /**
   * Set the delay.
   * \param delay is delay or a parameter (starts with '$').
   */
  virtual void SetDelayParameter (const char* delay) = 0;
};

/**
 * This interface is implemented by the reward that fires a sequence.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.rewards.sequence'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the
 *   pcquest property class.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>sequence</em>: the name of the sequence.
 * - <em>delay</em>: delay before we start the sequence. Default is 0.
 */
struct iSequenceRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iSequenceRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcquest property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the tag of the property class this reward will apply to.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetTagParameter (const char* tag_par) = 0;

  /**
   * Set the name of the entity class containing the property
   * class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the name of the sequence. If this method is used, with no call to
   * the SetSequence method, the trigger will look for the sequence
   * inside a quest inside the entity set.
   * \param sequence is the name of the sequence or a parameter (starts
   * with '$'). 
   */
  virtual void SetSequenceParameter (const char* sequence) = 0;

  /**
   * Set the sequence to observe. If this method is used, 
   * the trigger will observe the sequence given.
   * \param sequence is a pointer to the sequence.
   */
  virtual void SetSequence (iCelSequence* sequence) = 0;

  /**
   * Set the delay.
   * \param delay is delay or a parameter (starts with '$').
   */
  virtual void SetDelayParameter (const char* delay) = 0;
};

/**
 * This interface is implemented by the reward that finishes a sequence.
 * You can query this interface from the reward factory if you want
 * to manually control this factory as opposed to loading its definition
 * from an XML document.
 *
 * The predefined name of this reward type is 'cel.rewards.sequencefinish'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the
 *   pcquest property class.
 * - <em>class</em>: the name of an entity class. If this is used instead
 *   of the entity parameter, the reward will apply to all entities in the given
 *   entity class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>sequence</em>: the name of the sequence.
 */
struct iSequenceFinishRewardFactory : public virtual iBase
{
  SCF_INTERFACE (iSequenceFinishRewardFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcquest property class
   * on which this reward will work.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the tag of the property class this reward will apply to.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetTagParameter (const char* tag_par) = 0;

  /**
   * Set the name of the entity class containing the property
   * class on which this reward will work.
   * \param ent_class is the name of the class or a parameter (starts
   * with '$').
   */
  virtual void SetClassParameter (const char* ent_class) = 0;

  /**
   * Set the name of the sequence. If this method is used, with no call to
   * the SetSequence method, the trigger will look for the sequence
   * inside a quest inside the entity set.
   * \param sequence is the name of the sequence or a parameter (starts
   * with '$'). 
   */
  virtual void SetSequenceParameter (const char* sequence) = 0;

  /**
   * Set the sequence to observe. If this method is used, 
   * the trigger will observe the sequence given.
   * \param sequence is a pointer to the sequence.
   */
  virtual void SetSequence (iCelSequence* sequence) = 0;
};

//-------------------------------------------------------------------------

/**
 * Convenience to declare a new reward type class.
 */
#define CEL_DECLARE_REWARDTYPE(name,id)					\
class cel##name##RewardType : public scfImplementation2<		\
		cel##name##RewardType,iRewardType, iComponent>			\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  csWeakRef<iCelPlLayer> pl;						\
  cel##name##RewardType (iBase* parent);			\
  virtual ~cel##name##RewardType () { }					\
  virtual bool Initialize (iObjectRegistry*);			\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iRewardFactory> CreateRewardFactory ();		\
};

/**
 * Convenience to implement a new reward type class.
 */
#define CEL_IMPLEMENT_REWARDTYPE(name)					\
cel##name##RewardType::cel##name##RewardType (				\
	iBase* parent) : scfImplementationType (this, parent),	\
	object_reg(0)											\
{															\
}															\
bool cel##name##RewardType::Initialize (					\
	iObjectRegistry* object_reg)							\
{									\
  cel##name##RewardType::object_reg = object_reg;			\
  pl = csQueryRegistry<iCelPlLayer> (object_reg);			\
  return true;												\
}									\
csPtr<iRewardFactory> cel##name##RewardType::CreateRewardFactory ()\
{									\
  cel##name##RewardFactory* fact = new					\
  	cel##name##RewardFactory (this);				\
  return fact;								\
}

#endif // __CEL_REWARDS__
