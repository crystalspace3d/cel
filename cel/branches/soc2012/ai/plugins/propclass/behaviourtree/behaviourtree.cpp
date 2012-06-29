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

#include "cssysdef.h"
#include <iutil/comp.h>
#include <iutil/plugin.h>

#include "plugins/propclass/behaviourtree/behaviourtree.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (BehaviourTree, "pclogic.behaviourtree", "pcbehaviourtree")

//---------------------------------------------------------------------------

PropertyHolder celPcBehaviourTree::propinfo;

celPcBehaviourTree::celPcBehaviourTree (iObjectRegistry* object_reg) 
  : scfImplementationType (this, object_reg)                                                                             
{
  celPcBehaviourTree::object_reg = object_reg;

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.logic.behaviourtree.action.");
    AddAction (action_start, "BT Start");
    AddAction (action_interrupt, "BT Interrupt");
  }

  propinfo.SetCount (4);
  AddProperty (propid_updaterate, "update rate",
  	CEL_DATA_LONG, false, "Frequency of updates/nodes parsed in tree.", 0);  
  AddProperty (propid_treestatus, "tree status",
  	CEL_DATA_LONG, false, "Current status of tree.", 0); 
  AddProperty (propid_treename, "tree name",
  	CEL_DATA_STRING, false, "The name of the tree.", 0);  
  AddProperty (propid_rootnode, "root node",
  	CEL_DATA_IBASE, false, "The root node of the tree.", 0);  
               

  status = BT_NOT_STARTED;  
  name  = "un-named behaviour tree";      
  update_rate = 1;                       
}

bool celPcBehaviourTree::PerformActionIndexed (int idx, iCelParameterBlock* params, celData& ret)
{
  switch (idx) 
  {
    case action_start:
      Execute (params);
      ret.Set (status);
      return true;
    case action_interrupt:
      stack.Empty ();
      pl->RemoveCallbackEveryFrame((iCelTimerListener*)this, CEL_EVENT_PRE);
      status = BT_NOT_STARTED;
      ret.Set (status);
      return true;    
  }
  return false;
}

bool celPcBehaviourTree::SetPropertyIndexed (int idx, long value)
{
  switch (idx) 
  {
    case propid_updaterate:
      update_rate = value;
      return true;
    case propid_treestatus:
      if (value < 5)
      { 
        SetStatus(BTStatus(value));
        return true;
      }
      else
      {
        return false;
      }
  }
  return false;
}

bool celPcBehaviourTree::SetPropertyIndexed (int idx, const char* value)
{
  if (idx == propid_treename)
  {
    SetName(value);
    return true;
  }
  return false;
}

bool celPcBehaviourTree::SetPropertyIndexed (int idx, iBase* ibase)
{
  if (idx == propid_rootnode)
  {
    csRef<iBTNode> node = scfQueryInterface<iBTNode> (ibase);
    AddChild(node);
    return true;
  }
  return false;
}

bool celPcBehaviourTree::GetPropertyIndexed (int idx, long& value)
{
  switch (idx) 
  {
    case propid_updaterate:
      value = update_rate;
      return true;
    case propid_treestatus:
      value = GetStatus();
      return true;
  }
  return false;
}

bool celPcBehaviourTree::GetPropertyIndexed (int idx, const char*& value)
{
  if (idx == propid_treename)
  {
    value = GetName();
    return true;
  }
  return false;
}

bool celPcBehaviourTree::GetPropertyIndexed (int idx, iBase*& ibase)
{
  if (idx == propid_rootnode)
  {
    if (!stack.IsEmpty())
    {	
      ibase = stack.Get(0);
      return true;
    }
  }
  return false;
}

BTStatus celPcBehaviourTree::GetStatus ()
{
  return status;
}

void celPcBehaviourTree::SetStatus (BTStatus newStatus)
{
  status = newStatus;
}

void celPcBehaviourTree::SetName(csString nodeName)
{
  name = nodeName;
}

BTStatus celPcBehaviourTree::Execute (iCelParameterBlock* params, csRefArray<iBTNode>* BTStack)
{
  celPcBehaviourTree::node_params = params;		
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  status = BT_RUNNING;
  return status;
}

bool celPcBehaviourTree::AddChild (iBTNode* child)
{
  if (stack.IsEmpty())
  {	
	  stack.Push(child);
    return true; 
  }
  else
  {
    //Tree already has a root
    return false;   
  }
}

void celPcBehaviourTree::TickEveryFrame ()
{
  for (int i = 0; i < update_rate; i++)
  {
    // Execute current node in tree
    BTStatus top_status = stack.Top()->Execute (node_params, &stack);
  
    if (top_status == BT_SUCCESS ||
	    top_status == BT_FAIL_CLEAN ||
	    top_status == BT_UNEXPECTED_ERROR)
    {
	    // If node has completed,reset it and remove it from the stack
      stack.Pop();

	    if (stack.IsEmpty())
	    {
        // If tree has completed, stop calling this method
	      pl->RemoveCallbackEveryFrame((iCelTimerListener*)this, CEL_EVENT_PRE);

	      // Note that tree has completed
	      status = top_status;

        if (status == BT_UNEXPECTED_ERROR)
        {
          csReport(object_reg, CS_REPORTER_SEVERITY_ERROR,
              "pclogic.behaviourtree",
              "Behaviour tree (%s) has exited due to an unhandled unexpected error", name.GetData());
        }
	    }
    }
  }
}
