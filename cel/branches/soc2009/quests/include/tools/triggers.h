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
 * associated trigger factories. See iQuestTriggerFactory.
 *
 * Triggers must start in deactivated state. 
 */
struct iTrigger : public virtual iBase
{
  SCF_INTERFACE (iTrigger, 0, 0, 1);

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
  	const celParams& params) = 0;

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

//-------------------------------------------------------------------------

/**
 * Convenience to declare a new trigger type class.
 */
#define CEL_DECLARE_TRIGGERTYPE_NEW(name,id)				\
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
#define CEL_IMPLEMENT_TRIGGERTYPE_NEW(name)					\
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