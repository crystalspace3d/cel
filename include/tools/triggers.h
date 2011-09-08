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

#ifndef __CEL_TRIGGERS__
#define __CEL_TRIGGERS__

#include "behaviourlayer/behave.h"
#include "tools/parameters.h"
#include "tools/sequences.h"

struct iQuest;

//-------------------------------------------------------------------------
// Triggers
//-------------------------------------------------------------------------

struct iTrigger;
class celVariableParameterBlock;

/**
 * A trigger will get pointers to call back the quest when
 * the trigger fires through this interface.
 */
struct iTriggerCallback : public virtual iBase
{
  SCF_INTERFACE (iTriggerCallback, 0, 0, 1);

  /// Trigger fired.
  virtual void TriggerFired (iTrigger* trigger, iCelParameterBlock* params) = 0;
};

/**
 * This is a trigger. Triggers are created by trigger factories. To
 * be able to use your own trigger implementations you must also make
 * associated trigger factories. See iTriggerFactory.
 *
 * Triggers must start in deactivated state. 
 */
struct iTrigger : public virtual iBase
{
  SCF_INTERFACE (iTrigger, 0, 0, 2);

  /**
   * Register a callback with this trigger. When the trigger fires
   * it will call this callback. A trigger supports only one callback.
   */
  virtual void RegisterCallback (iTriggerCallback* callback) = 0;

  /**
   * Clear the callback.
   */
  virtual void ClearCallback () = 0;

  /**
   * Activate the trigger. Note that triggers start deactivated.
   */
  virtual void ActivateTrigger () = 0;

  /**
   * Deactivate the trigger. Note that triggers start deactivated.
   */
  virtual void DeactivateTrigger () = 0;

  /**
   * Check the trigger immediatelly and return true if it is valid.
   * This function does nothing else. Only the condition of the
   * trigger is tested. If this function returns true then it should
   * also have deactivated the trigger and fired the callbacks. This
   * is not the responsability of the caller.
   */
  virtual bool Check () = 0;

  /**
   * Activate the trigger and load state from databuf (persistence).
   * \return false on failure (data in buffer doesn't match what we
   * expect).
   */
  virtual bool LoadAndActivateTrigger (iCelDataBuffer* databuf) = 0;

  /**
   * Save trigger state.
   */
  virtual void SaveTriggerState (iCelDataBuffer* databuf) = 0;

  /**
   * Activate this trigger. This means it will process events again.
   * Note that this is not equivalent to ActivateTrigger() since the
   * this function will actually activate the trigger to the same state
   * as it was when the trigger was deactivated. ActivateTrigger() on the
   * other hand, just activates the trigger to its initial state.
   */
  virtual void Activate () = 0;

  /**
   * Deactivate this trigger. This is not the same as DeactivateTrigger()
   * since this function will remember the state when it was deactivated
   * and allow Activate() to restore it to exactly that state.
   */
  virtual void Deactivate () = 0;
};

/**
 * A factory to create triggers. Trigger factories are created
 * by an iTriggerType instance.
 */
struct iTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iTriggerFactory, 0, 0, 1);

  /**
   * Create a trigger.
   * \param params are the parameters with which this reward is
   * instantiated.
   */
  virtual csPtr<iTrigger> CreateTrigger (
  	iQuest* q, const celParams& params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the \<fireon\> node in a trigger description.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;
};

/**
 * The trigger type is responsible for the creation of trigger factories.
 * If you want to define a new type of trigger then you must implement
 * iTriggerType, iTriggerFactory, and iTrigger.
 *
 * An example use of triggers is, to register the trigger types with 
 * a quest manager so that they can be used by quest factories.
 */
struct iTriggerType : public virtual iBase
{
  SCF_INTERFACE (iTriggerType, 0, 0, 1);

  /**
   * Return the name for this trigger type.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Create a trigger factory.
   */
  virtual csPtr<iTriggerFactory> CreateTriggerFactory () = 0;
};

//-------------------------------------------------------------------------
// Specific trigger implementations.
//-------------------------------------------------------------------------
/**
 * This interface is implemented by the trigger that fires
 * when a certain sector is entered. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.triggers.entersector'
 * for the version that listens to camera changes and
 * 'cel.triggers.meshentersector' for the version that listens to mesh
 * changes.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pccamera or pcmesh property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>sector</em>: the name of the sector. As soon as the camera
 *   or mesh enters that sector this trigger will fire.
 */
struct iEnterSectorTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iEnterSectorTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pccamera or pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the sector on which this trigger will fire
   * as soon as the camera enters that sector.
   * \param sector is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetSectorParameter (const char* sector) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a certain inventory gets some entity. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.triggers.inventory'.
 *
 * This trigger sends out 'child' parameter to the reward (containing the
 * name of the entity that was added or removed).
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pcinventory property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>child_entity</em>: the name of the entity to watch for. If this
 *   is not given then you will get triggere whenever something is
 *   added or removed from the inventory.
 */
struct iInventoryTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iInventoryTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcinventory property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the entity to watch for. If this
   * is not given then you will get triggered whenever something is
   * added or removed from the inventory.
   * \param child_entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetChildEntityParameter (const char* child_entity) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a certain mesh is selected. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.triggers.meshselect'.
 *
 * This trigger sends out 'entity' parameter to the reward (containing the
 * name of the entity that was selected).
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pcmeshselect property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 */
struct iMeshSelectTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iMeshSelectTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcmeshselect property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a message is sent. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.triggers.message'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity from which we want to listen
 *   to messages.
 * - <em>mask</em>: the message mask to listen too.
 */
struct iMessageTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iMessageTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity from which we want to listen to messages.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the name of the message mask that we want to receive messages
   * for.
   * \param mask is the message mask or a parameter (starts
   * with '$').
   */
  virtual void SetMaskParameter (const char* mask) = 0;
};


/**
 * This interface is implemented by the operation trigger, that allows
 * to combine several triggers using a logical operation.
 * You can query this interface from the trigger factory if you want to
 * manually control this factory as opposed to loading its definition from an
 * XML document.
 *
 * The predefined name of this trigger type is 'cel.triggers.operation'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>operation</em>: the logical operation to perform. Can be one of
 *   'or', 'and', or 'xor', see below for a more detailed description.
 *
 * Fireon node can also hold any number of child 'trigger' nodes, which are
 * defined just as usual.
 *
 * Operations:
 * - <em>and</em>: Trigger will fire when all the child triggers are true.
 * - <em>or</em>: Trigger will fire when at least one of the child triggers
 *   is true.
 * - <em>xor</em>: Trigger will fire when one and only one of the child 
 *   triggers is true.
 */
struct iOperationTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iOperationTriggerFactory, 0, 0, 1);

  /**
   * Set the operation this trigger will use to combine child triggers.
   * \param operation is the name of the operation or a parameter (starts
   * with '$').
   * Operation must be one of 'or', 'and', or 'xor'.
   */
  virtual void SetOperationParameter (const char* operation) = 0;
  /**
   * Return the trigger factory list so that user can add new ones, remove
   * them or clear the list.
   */
  virtual csRefArray<iTriggerFactory> &GetTriggerFactories () = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a certain property passes a test on some value. Test can be any of
 * the operations below.
 * You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is
 * 'cel.triggers.propertychange'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pcproperties property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>property</em>: the name of the property.
 * - <em>value</em>: the value on which this trigger will fire. If this
 *   value is not given then the trigger will fire whenever the value
 * - <em>operation</em>: the value on which this trigger will test. If this
 *   value is not given then the trigger will fire on equality.
 * - <em>onchange</em>: if true the trigger will fire only when the condition
 *   becomes true (so it has to be initially false, and become true after
 *   that), otherwise the trigger will fire whenever the condition is met
 *   (even initially).
 *
 * Valid operations:
 * - <em>eq</em>: Equals (==).
 * - <em>lt</em>: Lower than (<).
 * - <em>gt</em>: Greater than (>).
 * - <em>ne</em>: Not equal (!=).
 * - <em>le</em>: Lower or equal (<=).
 * - <em>ge</em>: Greater or equal (>=).
 * Note string and bool properties can only be tested with "ne" or "eq", as
 * greater or lesser doesn't really make sense for them.
 */
struct iPropertyChangeTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iPropertyChangeTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcproperties property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the property on which this trigger will fire.
   * \param prop is the name of the property or a parameter (starts
   * with '$').
   */
  virtual void SetPropertyParameter (const char* prop) = 0;

  /**
   * Set the value of the property on which this trigger will fire. If
   * this value is not given the the trigger will fire whenever the value
   * changes.
   * \param value is the value or a parameter (starts with '$').
   */
  virtual void SetValueParameter (const char* value) = 0;

  /**
   * Set the operation this trigger will test with. If operation
   * is not set equality will be checked.
   * \param op one of: eq, lt, gt, ne, le, ge. See above for more details.
   */
  virtual void SetOperationParameter (const char* op) = 0;

  /**
   * Set whether the trigger will fire only on an actual change (so the
   * condition has to become true), or whenever the condition is met (even
   * initially).
   * \param on_change true for reporting only on change.
   */
  virtual void SetOnChangeOnly (bool on_change) = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a certain sequence finishes. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is
 * 'cel.triggers.sequencefinish'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pcquest property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>sequence</em>: the name of the sequence on which to listen.
 */
struct iSequenceFinishTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iSequenceFinishTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pcquest property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

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

/**
 * This interface is implemented by the trigger that fires
 * when a certain time has expired. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.triggers.timeout'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>timeout</em>: timeout value.
 */
struct iTimeoutTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iTimeoutTriggerFactory, 0, 0, 1);

  /**
   * Set the timeout on which this trigger will fire.
   * \param timeout_par is the timeout value (in ms) or a parameter (starts
   * with '$').
   */
  virtual void SetTimeoutParameter (const char* timeout_par) = 0;
};


/**
 * This interface is implemented by the trigger that fires
 * when a pctrigger fires. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.triggers.trigger'.
 *
 * This trigger sends out 'entity' parameter to the reward (containing the
 * name of the entity caused the trigger to fire).
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the entity that contains the
 *   pctrigger property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>leave</em>: if present the trigger will register on 'leaves' instead
 *   of 'enters'.
 */
struct iTriggerTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iTriggerTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the entity containing the pccamera or pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * If this function is called then the trigger will fire on 'leaves'
   * instead of 'enters'.
   */
  virtual void EnableLeave () = 0;
};

/**
 * This interface is implemented by the trigger that fires
 * when a mesh becomes visible. You can query this interface
 * from the trigger factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this trigger type is 'cel.triggers.watch'.
 *
 * In XML, factories recognize the following attributes on the 'fireon' node:
 * - <em>entity</em>: the name of the source entity that is watching
 *   out for other entity. This should contain a pcmesh to get a position
 *   from.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>target</em>: the name of the target entity to watch. This
 *   should contain a pcmesh to get a position from.
 * - <em>target_tag</em>: optional tag used to find the right property
 *   class from the target entity.
 * - <em>checktime</em>: optional tag to specify the check interval.
 * - <em>radius</em>: optional tag to specify the maximum radius.
 * - <em>offset</em>: optional offset vector.
 *   This node has 'x', 'y, and 'z' attributes. Each of these attributes
 *   can be a parameter.
 */
struct iWatchTriggerFactory : public virtual iBase
{
  SCF_INTERFACE (iWatchTriggerFactory, 0, 0, 1);

  /**
   * Set the name of the source entity containing the pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the name of the target entity containing the pcmesh property class
   * on which this trigger will fire.
   * \param entity is the name of the entity or a parameter (starts
   * with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetTargetEntityParameter (const char* entity,
      const char* tag = 0) = 0;

  /**
   * Set the interval (in milliseconds) after which this trigger
   * will check visibility again. By default this is equal to 1000
   * which means there will be a check every second.
   * \param time is the interval in milliseconds or a parameter (starts
   * with '$').
   */
  virtual void SetChecktimeParameter (const char* time) = 0;

  /**
   * Set the maximum radius for visibility checking.
   * By default this is equal to infinity.
   * \param radius is the maximum radius or a parameter (starts
   * with '$').
   */
  virtual void SetRadiusParameter (const char* radius) = 0;

  /**
   * Optional offset parameter. This offset will be added to the
   * position of the start and target locations.
   * \param offsetx is the x offset or a parameter (starts with '$').
   * \param offsety is the y offset or a parameter (starts with '$').
   * \param offsetz is the z offset or a parameter (starts with '$').
   */
  virtual void SetOffsetParameter (const char* offsetx,
      const char* offsety, const char* offsetz) = 0;
};

//-------------------------------------------------------------------------

/**
 * Convenience to declare a new trigger type class.
 */
#define CEL_DECLARE_TRIGGERTYPE(name,id)				\
class cel##name##TriggerType : public scfImplementation2<		\
		cel##name##TriggerType,iTriggerType,iComponent>		\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  csWeakRef<iCelPlLayer> pl;						\
  cel##name##TriggerType (iBase* parent);			\
  virtual ~cel##name##TriggerType () { }				\
  virtual bool Initialize (iObjectRegistry*);			\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iTriggerFactory> CreateTriggerFactory ();		\
};

/**
 * Convenience to implement a new trigger type class.
 */
#define CEL_IMPLEMENT_TRIGGERTYPE(name)					\
cel##name##TriggerType::cel##name##TriggerType (				\
	iBase* parent) : scfImplementationType (this, parent),	\
	object_reg(0)											\
{															\
}															\
bool cel##name##TriggerType::Initialize (					\
	iObjectRegistry* object_reg)							\
{									\
  cel##name##TriggerType::object_reg = object_reg;			\
  pl = csQueryRegistry<iCelPlLayer> (object_reg);			\
  return true;												\
}									\
csPtr<iTriggerFactory> cel##name##TriggerType::CreateTriggerFactory () \
{									\
  cel##name##TriggerFactory* fact = new cel##name##TriggerFactory (this); \
  return fact;								\
}

#endif // __CEL_TRIGGERS__
