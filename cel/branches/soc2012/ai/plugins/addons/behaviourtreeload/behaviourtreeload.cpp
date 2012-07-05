/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein
  
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
#include "csutil/scanstr.h"
#include "cstool/initapp.h"
#include "iutil/objreg.h"
#include "iutil/document.h"
#include "iutil/object.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"
#include "imap/services.h"
#include "iengine/mesh.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "behaviourlayer/bl.h"
#include "propclass/mesh.h"
#include "celtool/stdparams.h"

#include "behaviourtreeload.h"


SCF_IMPLEMENT_FACTORY (celAddOnBehaviourTreeLoader)

//---------------------------------------------------------------------------------------

celAddOnBehaviourTreeLoader::celAddOnBehaviourTreeLoader (iBase *parent) :
  scfImplementationType (this, parent)
{
  object_reg = 0;
}

celAddOnBehaviourTreeLoader::~celAddOnBehaviourTreeLoader ()
{
}

bool celAddOnBehaviourTreeLoader::Initialize (iObjectRegistry *object_reg)
{
  celAddOnBehaviourTreeLoader::object_reg = object_reg;
  plugin_mgr = csQueryRegistry<iPluginManager> (object_reg);
  params.AttachNew (new celVariableParameterBlock ());
  return true;
}

csPtr<iBase> celAddOnBehaviourTreeLoader::Parse (iDocumentNode* node,
	iStreamSource*, iLoaderContext* ldr_context, iBase* context)
{
  csRef<iBTNode> current_node; 
  csRef<iDocumentNodeIterator> it;

  const char* node_type = node->GetAttributeValue ("type");
  current_node = csLoadPlugin<iBTNode> (plugin_mgr, node_type);

  const char* node_name = node->GetAttributeValue ("name");
  if (node_name) current_node->SetName (node_name); 

  const char* value = node->GetValue ();
  if (strcmp ("selector", value) == 0)
  { 
    it = node->GetNodes ();
    while (it->HasNext ())
    {
      csRef<iDocumentNode> child = it->Next ();
      if (child->GetType () != CS_NODE_ELEMENT) continue;
      csRef<iBase> ibase = Parse(child, 0, 0, 0);
      csRef<iBTNode> child_node = scfQueryInterface<iBTNode> (ibase);
      current_node->AddChild (child_node);        
    }
  }
  else if (strcmp ("decorator", value) == 0)
  { 
    if (strcmp ("cel.decorators.executionlimit", node_type) == 0)
    {  
      csRef<iExecutionLimitDecorator> explicit_execution_limit_node = 
        scfQueryInterface<iExecutionLimitDecorator> (current_node);

      const char* node_limit = node->GetAttributeValue ("limit");
      if (node_limit) explicit_execution_limit_node->SetExecutionLimit(node_limit);
    }
    else if (strcmp ("cel.decorators.loop", node_type) == 0)
    {
      csRef<iLoopDecorator> explicit_loop_node = 
        scfQueryInterface<iLoopDecorator> (current_node);

      const char* node_limit = node->GetAttributeValue ("limit");
      if (node_limit) explicit_loop_node->SetLoopLimit(node_limit);
    }

    it = node->GetNodes ();
    while (it->HasNext ())
    {
      csRef<iDocumentNode> child = it->Next ();
      if (child->GetType () != CS_NODE_ELEMENT) continue;
      csRef<iBase> ibase = Parse(child, 0, 0, 0);
      csRef<iBTNode> child_node = scfQueryInterface<iBTNode> (ibase);
      current_node->AddChild (child_node);
      break; // Decorators only have one child
    }
  }
  else if (strcmp ("leaf_node", value) == 0)
  {
    if (strcmp ("cel.behaviourtree.action", node_type) == 0)
    {  
      csRef<iRewardType> reward_type = csLoadPlugin<iRewardType> (plugin_mgr,
        "cel.rewards.debugprint");
      csRef<iRewardFactory> reward_factory = reward_type->CreateRewardFactory ();
      csRef<iDebugPrintRewardFactory> explicit_reward_factory = 
	      scfQueryInterface<iDebugPrintRewardFactory> (reward_factory);

      csRef<iBTAction> explicit_action_node = scfQueryInterface<iBTAction> (current_node);
      explicit_reward_factory->SetMessageParameter ("To do - complete celAddOnBehaviourTreeLoader::Parse for action nodes");
      // @@@ TODO: fix the quest parameter 0!
      csRef<iReward> reward = reward_factory->CreateReward(0, params);
      explicit_action_node->AddReward (reward);
    }
    else if (strcmp ("cel.behaviourtree.parametercheck", node_type) == 0)
    { 
      csRef<iParameterCheckCondition> explicit_parameter_check_node =
        scfQueryInterface<iParameterCheckCondition> (current_node);

      const char* parameter = node->GetAttributeValue ("parameter");
      explicit_parameter_check_node->SetParameter(parameter);
      const char* check_value = node->GetAttributeValue ("value");
      explicit_parameter_check_node->SetValue(check_value);
    }
    else if (strcmp ("cel.behaviourtree.triggerfired", node_type) == 0)
    { 
      current_node = csLoadPlugin<iBTNode> (plugin_mgr, "cel.behaviourtree.action");
      csRef<iRewardType> reward_type = csLoadPlugin<iRewardType> (plugin_mgr,
        "cel.rewards.debugprint");
      csRef<iRewardFactory> reward_factory = reward_type->CreateRewardFactory ();
      csRef<iDebugPrintRewardFactory> explicit_reward_factory = 
	      scfQueryInterface<iDebugPrintRewardFactory> (reward_factory);

      csRef<iBTAction> explicit_action_node = scfQueryInterface<iBTAction> (current_node);
      explicit_reward_factory->SetMessageParameter ("To do - complete celAddOnBehaviourTreeLoader::Parse for trigger fired nodes");
      // @@@ TODO: fix the quest parameter 0!
      csRef<iReward> reward = reward_factory->CreateReward(0, params);
      explicit_action_node->AddReward (reward);
    }
  }
  
  return scfQueryInterface<iBase> (current_node);
}