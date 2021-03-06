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

#ifndef __CEL_BEHAVIOUR_TREE__
#define __CEL_BEHAVIOUR_TREE__

#include "behaviourlayer/behave.h"
#include "tools/decorators.h"
#include "tools/parameters.h"
#include "tools/rewards.h"
#include "tools/triggers.h"

//-------------------------------------------------------------------------
// Behaviour tree statuses
//-------------------------------------------------------------------------

/**
 * These are the possible states of a behaviour tree node
 * They are to be returned when a nodes executes
 * The parent node can then decide how to proceed
 *
*/

enum BTStatus{
	BT_NOT_STARTED = 0,			// Node has not yet started
	BT_RUNNING,				      // Node is currently executing
	BT_SUCCESS,				      // Node completed succesfully, making changes to state as expected
	BT_FAIL_CLEAN,			    // Node failed, but cleanly (typically with no changes to state)
	BT_UNEXPECTED_ERROR	    // Node failed unexpectedly, possibly changing state (should be handled by parent)
};

//-------------------------------------------------------------------------
// Behaviour tree nodes
//-------------------------------------------------------------------------

/**
 * This is a node of a behaviour tree. A behaviour tree allows to define the
 * behaviour of an entity by combining behaviour nodes into a tree.
 *
 * There are different type of nodes. The main leaf nodes of the tree are iBTAction
 * nodes that fire actions (eg by sending a message to an entity or a property class).
 * Non-action nodes are used to select the actions that will be executed.
 *
 * Here is the list of different non-action nodes:
 * - "cel.behaviourtree": Default behaviour for a root node. It executes the
 * tree regularly.
 * - "cel.selectors.default": The default selector. It executes its children in order
 * until one succeeds.
 * - "cel.selectors.random": A random selector. It executes its children in a random
 * order until one succeeds.
 * - "cel.selectors.sequential": A sequential selector. It executes its children in order
 * until one fails or all have succeeded. Often refered in behaviour tree
 * literature as a sequence.
 * - iTriggerFiredCondition: A wrapper to use triggers within
 * behaviour trees. Return \a false until the trigger has been fired.
 * - iParameterCheckCondition: A wrapper to use parameters within
 * behaviour trees. Return whether or not the parameter is equal to the given value.
 * - iExecutionLimitDecorator: A decorator limiting the number of times a child is executed.
 * - iLoopDecorator: A loop decorator. It executes its child multiple times in a row.
 * - "cel.decorators.negatereturn": A negation decorator. It executes its child and returns
 * the opposite of the result.
 */
struct iBTNode : public virtual iBase
{
  SCF_INTERFACE (iBTNode, 0, 0, 1);

  /**
   * Execute this node.
   * Return whether or not the execution of the node was successful.
   */
  virtual BTStatus Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack = 0) = 0;

  /**
   * Add a child node to this node
   */
  virtual bool AddChild (iBTNode* child) = 0;

  /**
   * Get the status of this node
   */
  virtual BTStatus GetStatus () = 0;

  /**
   * Set the status of this node
   */
  virtual void SetStatus (BTStatus newStatus) = 0;

  /**
   * Set the name of this node for error reporting
   */
  virtual void SetName (csString nodeName) = 0;
};

//-------------------------------------------------------------------------
// Specific behaviour tree node implementations.
//-------------------------------------------------------------------------

/**
 * This interface is implemented by the leaf action iBTNode that 
 * launches a list of CEL rewards
 */
struct iBTAction: public virtual iBase
{
  SCF_INTERFACE (iBTAction, 0, 0, 1);

  /**
   * Add a reward to be launched when this action node is executed.
   */
  virtual void AddReward (iReward* reward) = 0;
};

/**
 * This interface is implemented by the leaf iBTNode that 
 * checks whether or not a parameter is equal to a given value.
 */
struct iParameterCheckCondition: public virtual iBase
{
  SCF_INTERFACE (iParameterCheckCondition, 0, 0, 1);

  /**
   * Set the parameter to be checked by this node
   */
  virtual void SetParameter (const char* parameter) = 0;

  /**
   * Set the value that will be checked against the parameter
   */
  virtual void SetValue (const char* value) = 0;
};


/**
 * This interface is implemented by the leaf iBTNode that 
 * monitors a CEL trigger. It returns whether or not a trigger has been fired.
 * The exact behaviour depends on the value set by SetFireOnce().
 */
struct iTriggerFiredCondition: public virtual iBase
{
  SCF_INTERFACE (iTriggerFiredCondition, 0, 0, 1);

  /**
   * Set the trigger to be monitored by this node
   */
  virtual void SetTrigger (iTrigger* trigger) = 0;

  /**
   * Set whether or not the execution of the condition will return \a true once
   * or forever once the trigger has been fired. If \p once
   * is \a true, then iBTNode::Execute() will return \a true only once each time
   * the trigger is fired. Otherwise, iBTNode::Execute() will return \a true
   * forever once the trigger has been fired. The default value is false.
   */
  virtual void SetFireOnce (bool once) = 0;
};

//-------------------------------------------------------------------------

/**
 * Convenience to declare a new behaviour tree node class.
 */
#define CEL_DECLARE_BTNODE(className)					\
class cel##className : public scfImplementation2<		                \
		cel##className ,iBTNode, iComponent>			        \
{		                                                        \
private:                                                                \
  iObjectRegistry* object_reg;						\
  csRefArray<iBTNode> children;                                         \
  BTStatus status;                \
  csString name;                \
public:									\
  cel##className (iBase* parent);			                        \
  virtual ~cel##className () { }					        \
  virtual bool Initialize (iObjectRegistry*);			        \
  virtual BTStatus Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack = 0);		        \
  virtual bool AddChild (iBTNode* child);                               \
  virtual BTStatus GetStatus ();                               \
  virtual void SetStatus (BTStatus newStatus);  \
  virtual void SetName (csString nodeName);  \
};

/**
 * Convenience to implement a new behaviour tree node class.
 */
#define CEL_IMPLEMENT_BTNODE(className)					\
cel##className::cel##className (iBase* parent)			                \
 : scfImplementationType (this, parent), object_reg(0)			\
{									\
}									\
bool cel##className::Initialize (					        \
	iObjectRegistry* object_reg)					\
{									\
  cel##className::object_reg = object_reg;			                \
  cel##className::status = BT_NOT_STARTED;   \
  cel##className::name = "un-named node"; \
  return true;								\
}	\
BTStatus cel##className::GetStatus ()					\
{									\
  return cel##className::status;								\
}	\
void cel##className::SetStatus (BTStatus newStatus)					\
{									\
  cel##className::status = newStatus;								\
}	\
void cel##className::SetName (csString nodeName)					\
{									\
  cel##className::name = nodeName;								\
}	

#endif // __CEL_BEHAVIOUR_TREE__
