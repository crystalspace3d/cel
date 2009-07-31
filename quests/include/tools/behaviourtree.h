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
#include "tools/parameters.h"

//-------------------------------------------------------------------------
// BTNodes
//-------------------------------------------------------------------------

/**
 * This is a node of a behaviour tree.
 * You can implement your own types of node by
 * implementing this interface.
 */
struct iBTNode : public virtual iBase
{
  SCF_INTERFACE (iBTNode, 0, 0, 1);
  /**
   * Perform this node.
   * Returns true if node was succesful and 
   * false otherwise.
   */
  virtual bool Execute (const celParams& params) = 0;

  /**
   * Connect a child node
   */
  virtual bool AddChild (iBTNode* child) = 0;

};



//-------------------------------------------------------------------------

/**
 * Convenience to declare a new reward type class.
 */
#define CEL_DECLARE_BTNODE(name)					\
class cel##name## : public scfImplementation2<		\
		cel##name## ,iBTNode, iComponent>			\
{		\
private: \
  iObjectRegistry* object_reg;						\
  csRefArray<iBTNode> children; \
public:									\
  cel##name## (iBase* parent);			\
  virtual ~cel##name## () { }					\
  virtual bool Initialize (iObjectRegistry*);			\
  virtual bool Execute (const celParams& params);		\
  virtual bool AddChild (iBTNode* child);\
};

/**
 * Convenience to implement a new reward type class.
 */
#define CEL_IMPLEMENT_BTNODE(name)					\
cel##name##::cel##name## (				\
	iBase* parent) : scfImplementationType (this, parent),	\
	object_reg(0)											\
{															\
}															\
bool cel##name##::Initialize (					\
	iObjectRegistry* object_reg)							\
{									\
  cel##name##::object_reg = object_reg;			\
  return true;												\
}									


#endif // __CEL_BEHAVIOUR_TREE__