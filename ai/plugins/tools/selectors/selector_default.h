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

#ifndef __CEL_TOOLS_SELECTORS_DEFAULT__
#define __CEL_TOOLS_SELECTORS_DEFAULT__

#include "tools/behaviourtree.h"


/**
 * The default selector. Executes children in order until one succeeds.
 */

class celDefaultSelector : public scfImplementation2<		
		celDefaultSelector, 
		iBTNode, iComponent>			
{		
private: 
  iObjectRegistry* object_reg;						
  csRefArray<iBTNode> children; 
  BTStatus status;
  csString name;

  const char* execution_limit_param;
  int noOfChildren;
  int currentChild;

public:									
  celDefaultSelector (iBase* parent);			
  virtual ~celDefaultSelector () { }					
  virtual bool Initialize (iObjectRegistry*);	

  //From iBTNode
  virtual BTStatus Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack = 0);		
  virtual bool AddChild (iBTNode* child);
  virtual BTStatus GetStatus ();
  virtual void SetStatus (BTStatus newStatus);
  virtual void SetName (csString nodeName);

};

#endif // __CEL_TOOLS_SELECTORS_DEFAULT__

