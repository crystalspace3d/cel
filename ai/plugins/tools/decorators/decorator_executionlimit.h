/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Sam Devlin
  
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

#ifndef __CEL_TOOLS_DECORATORS_EXECUTION_LIMIT__
#define __CEL_TOOLS_DECORATORS_EXECUTION_LIMIT__

#include "tools/behaviourtree.h"


/**
 * A decorator limiting the number of times a child is executed.
 * Fails cleanly if child has been executed more times than execution limit
 * Succeeds if child is exectued and fails cleanly or succeeds
 * Returns an unexpected error if no child is specified or if child does
 */

class celExecutionLimitDecorator : public scfImplementation3<		
		celExecutionLimitDecorator, 
		iExecutionLimitDecorator, iBTNode, iComponent>			
{		
private: 
  iObjectRegistry* object_reg;						
  csRef<iBTNode> child_node; 
  BTStatus status;
  csString name;

  const char* execution_limit_param;
  int execution_limit;
  int execution_count;

public:									
  celExecutionLimitDecorator (iBase* parent);			
  virtual ~celExecutionLimitDecorator () { }					
  virtual bool Initialize (iObjectRegistry*);	

  //From iBTNode
  virtual BTStatus Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack = 0);		
  virtual bool AddChild (iBTNode* child);
  virtual BTStatus GetStatus ();
  virtual void SetStatus (BTStatus newStatus);
  virtual void SetName (csString nodeName);

  //From iExecutionLimitDecorator
  virtual void SetExecutionLimit (const char* limit);
};

#endif // __CEL_TOOLS_DECORATORS_EXECUTION_LIMIT__

