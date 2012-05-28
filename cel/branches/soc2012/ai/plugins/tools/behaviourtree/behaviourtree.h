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

#ifndef __CEL_TOOLS_BT__
#define __CEL_TOOLS_BT__

#include "tools/behaviourtree.h"


/**
 * The core behaviour tree class
 */

class celBehaviourTree : public scfImplementation3<		
		celBehaviourTree, 
		iCelTimerListener, iBTNode, iComponent>			
{		
private: 
  iObjectRegistry* object_reg;	
  csRef<iCelPlLayer> pl;					
  csRef<iBTNode> root_node; 
  csRef<iCelParameterBlock> params;

public:									
  celBehaviourTree (iBase* parent);			
  virtual ~celBehaviourTree () { }					
  virtual bool Initialize (iObjectRegistry*);	

  //From iBTNode
  virtual BTStatus Execute (iCelParameterBlock* params);		
  virtual bool AddChild (iBTNode* child);

  // From iCelTimerListener 
  virtual void TickEveryFrame ();
  virtual void TickOnce () { }
};

#endif // __CEL_TOOLS_BTACTION__

