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
 * A decorator to limit the number of times a child executes.
 */

class celExecutionLimitDecorator : public scfImplementation3<		
		celExecutionLimitDecorator, 
		iExecutionLimitDecorator, iBTNode, iComponent>			
{		
private: 
  iObjectRegistry* object_reg;						
  csRef<iBTNode> child_node; 

  const char* execution_limit_param;
  int execution_limit;
  int execution_count;
public:									
  celExecutionLimitDecorator (iBase* parent);			
  virtual ~celExecutionLimitDecorator () { }					
  virtual bool Initialize (iObjectRegistry*);	

  //From iBTNode
  virtual bool Execute (const celParams& params);		
  virtual bool AddChild (iBTNode* child);

  //From iExecutionLimitDecorator
  virtual void SetExecutionLimit (const char* limit);
};

#endif // __CEL_TOOLS_DECORATORS_EXECUTION_LIMIT__

