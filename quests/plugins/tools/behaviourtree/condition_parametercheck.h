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

#ifndef __CEL_TOOLS_CONDITION_PARAMETER_CHECK__
#define __CEL_TOOLS_CONDITION_PARAMETER_CHECK__

#include "tools/behaviourtree.h"


/**
 * A wrapper for parameters to use them within behaviour trees
 * Returns true if parameter is equal to value
 */
class celParameterCheckCondition : public scfImplementation3<		
		celParameterCheckCondition, 
		iParameterCheckCondition, iBTNode, iComponent>			
{		
private: 
  iObjectRegistry* object_reg;						
  const char* parameter;
  const char* value;

public:									
  celParameterCheckCondition (iBase* parent);			
  virtual ~celParameterCheckCondition () { }					
  virtual bool Initialize (iObjectRegistry*);	

  //From iBTNode
  virtual bool Execute (const celParams& params);		
  virtual bool AddChild (iBTNode* child);

  //From iParameterCheckCondition
  virtual void SetParameter (const char* param);
  virtual void SetValue (const char* value);
};

#endif // __CEL_TOOLS_CONDITION_PARAMETER_CHECK__

