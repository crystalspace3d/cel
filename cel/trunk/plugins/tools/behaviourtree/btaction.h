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

#ifndef __CEL_TOOLS_BTACTION__
#define __CEL_TOOLS_BTACTION__

#include "tools/behaviourtree.h"


/**
 * A wrapper for rewards to use them within behaviour trees
 */

class celBehaviourTreeAction : public scfImplementation3<		
		celBehaviourTreeAction, 
		iBTAction, iBTNode, iComponent>			
{		
private: 
  iObjectRegistry* object_reg;	
  BTStatus status;
  csString name;

  csRefArray<iReward> actions; 

public:									
  celBehaviourTreeAction (iBase* parent);			
  virtual ~celBehaviourTreeAction () { }					
  virtual bool Initialize (iObjectRegistry*);	

  //From iBTNode
  virtual BTStatus Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack = 0);		
  virtual bool AddChild (iBTNode* child);
  virtual BTStatus GetStatus ();
  virtual void SetStatus (BTStatus newStatus);
  virtual void SetName (csString nodeName);

  //From iBTAction
  virtual void AddReward (iReward* reward);
};

#endif // __CEL_TOOLS_BTACTION__

