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
#include "celtool/stdpcimp.h"
#include "physicallayer/facttmpl.h"

/**
 * Factory for behaviour tree.
 */
CEL_DECLARE_FACTORY(BehaviourTree)


/**
 * The core behaviour tree class
 */

class celPcBehaviourTree : public scfImplementationExt1<		
		celPcBehaviourTree, celPcCommon, iBTNode>			
{		
private: 
  iObjectRegistry* object_reg;	

  csRef<iCelPlLayer> pl;	
  csRef<iCelParameterBlock> node_params;

  BTStatus status;
  csString name;
  csRefArray<iBTNode> stack;
  int update_rate;

  // For propclass actions.
  enum actionids
  {
    action_start = 0,
    action_interrupt
  };

  // For propclass properties.
  enum propids
  {
    propid_updaterate = 0,
    propid_treestatus,
    propid_treename,
    propid_rootnode
  };

  // Other propclass
  static PropertyHolder propinfo;

public:		
  celPcBehaviourTree (iObjectRegistry* object_reg);			
  virtual ~celPcBehaviourTree () { };					

  //From iBTNode
  virtual BTStatus Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack = 0);		
  virtual bool AddChild (iBTNode* child);
  virtual BTStatus GetStatus ();
  virtual void SetStatus (BTStatus newStatus);
  virtual void SetName (csString nodeName);

  // From iCelTimerListener 
  virtual void TickEveryFrame ();

  // Override celPcCommon
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params, celData& ret);
  virtual bool SetPropertyIndexed (int idx, long value);
  virtual bool SetPropertyIndexed (int idx, const char* value);
  virtual bool SetPropertyIndexed (int idx, iBase* ibase);
  virtual bool GetPropertyIndexed (int idx, long& value);
  virtual bool GetPropertyIndexed (int idx, const char*& value);
  virtual bool GetPropertyIndexed (int idx, iBase*& ibase);
};

#endif // __CEL_TOOLS_BTACTION__

