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

#ifndef __CEL_TOOLS_SELECTORS_SEQUENTIAL__
#define __CEL_TOOLS_SELECTORS_SEQUENTIAL__

#include "tools/behaviourtree.h"


/**
 * A sequential selector.
 * Executes children in order until one fails or all have succeeded.
 * Often reffered to in behaviour tree literature as a sequence.
 */

class celSequentialSelector : public scfImplementation2<		
		celSequentialSelector, 
		iBTNode, iComponent>			
{		
private: 
  iObjectRegistry* object_reg;						
  csRefArray<iBTNode> children; 
  BTStatus status;

  const char* execution_limit_param;
  int noOfChildren;
  int currentChild;

public:									
  celSequentialSelector (iBase* parent);			
  virtual ~celSequentialSelector () { }					
  virtual bool Initialize (iObjectRegistry*);	

  //From iBTNode
  virtual BTStatus Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack = 0);		
  virtual bool AddChild (iBTNode* child);
  virtual BTStatus GetStatus ();
  virtual void SetStatus (BTStatus newStatus);

};

#endif // __CEL_TOOLS_SELECTORS_SEQUENTIAL__

