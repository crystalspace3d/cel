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

  csString node_type = node->GetAttributeValue ("type");
  const char* node_name = node->GetAttributeValue ("name");
  
  const char* value = node->GetValue ();
  if (strcmp ("selector", value) == 0)
  { 
    current_node = csLoadPlugin<iBTNode> (plugin_mgr, 
      "cel.selectors."+node_type);
    if (node_name) current_node->SetName (node_name); 

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
    current_node = csLoadPlugin<iBTNode> (plugin_mgr, 
      "cel.decorators."+node_type);
    if (node_name) current_node->SetName (node_name); 

    if (strcmp ("executionlimit", node_type) == 0)
    {  
      csRef<iExecutionLimitDecorator> explicit_execution_limit_node = 
        scfQueryInterface<iExecutionLimitDecorator> (current_node);

      const char* node_limit = node->GetAttributeValue ("limit");
      if (node_limit) explicit_execution_limit_node->SetExecutionLimit(node_limit);
    }
    else if (strcmp ("loop", node_type) == 0)
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
    current_node = csLoadPlugin<iBTNode> (plugin_mgr, 
      "cel.behaviourtree."+node_type);
    if (node_name) current_node->SetName (node_name); 

    if (strcmp ("action", node_type) == 0)
    {  
      it = node->GetNodes ();
      while (it->HasNext ())
      {
        csRef<iDocumentNode> child = it->Next ();
        if (child->GetType () != CS_NODE_ELEMENT) continue;
        const char* child_value = child->GetValue ();
        if (strcmp ("reward", child_value) == 0)
        { 
          csString child_type = child->GetAttributeValue ("type");
          csRef<iRewardType> reward_type = csLoadPlugin<iRewardType> (plugin_mgr,
            "cel.rewards."+child_type);
          csRef<iRewardFactory> reward_factory = reward_type->CreateRewardFactory ();
          if (reward_factory->Load (child))
          {
            csRef<iReward> reward = reward_factory->CreateReward(0, params);
            csRef<iBTAction> explicit_action_node =
              scfQueryInterface<iBTAction> (current_node);
            explicit_action_node->AddReward (reward);
          }
        }
        else
        {
          csReport (object_reg, CS_REPORTER_SEVERITY_NOTIFY,
		        "cel.addons.behaviourtree.loader",
		        "Unknown token '%s' while loading action leaf node in behaviour tree.",
		        child_value);
        }
      }
    }
    else if (strcmp ("parametercheck", node_type) == 0)
    { 
      csRef<iParameterCheckCondition> explicit_parameter_check_node =
        scfQueryInterface<iParameterCheckCondition> (current_node);

      const char* parameter = node->GetAttributeValue ("parameter");
      explicit_parameter_check_node->SetParameter(parameter);
      const char* check_value = node->GetAttributeValue ("value");
      explicit_parameter_check_node->SetValue(check_value);
    }
    else if (strcmp ("triggerfired", node_type) == 0)
    { 
      bool triggerSetUp = false;
      it = node->GetNodes ();
      while (it->HasNext () && !triggerSetUp)
      {
        csRef<iDocumentNode> child = it->Next ();
        if (child->GetType () != CS_NODE_ELEMENT) continue;
        const char* child_value = child->GetValue ();
        if (strcmp ("trigger", child_value) == 0)
        { 
          csString child_type = child->GetAttributeValue ("type");
          csRef<iTriggerType> trigger_type = csLoadPlugin<iTriggerType> (plugin_mgr,
            "cel.triggers."+child_type);
          csRef<iTriggerFactory> trigger_factory = trigger_type->CreateTriggerFactory ();

          csRef<iDocumentNodeIterator> child_it;
          child_it = child->GetNodes ();
          while (child_it->HasNext () && !triggerSetUp)
          {
            csRef<iDocumentNode> fireon_node = child_it->Next ();
            if (fireon_node->GetType () != CS_NODE_ELEMENT) continue;
            const char* fireon_value = fireon_node->GetValue ();
            if (strcmp ("fireon", fireon_value) == 0)
            { 
              if (trigger_factory->Load (fireon_node))
              {
                csRef<iTrigger> trigger = trigger_factory->CreateTrigger (0, params);
                csRef<iTriggerFiredCondition> explicit_trigger_node =
                  scfQueryInterface<iTriggerFiredCondition> (current_node);
                explicit_trigger_node->SetTrigger (trigger);
                triggerSetUp = true; // Trigger fired leaf nodes only have one trigger
              }
            }
            else
            {
              csReport (object_reg, CS_REPORTER_SEVERITY_NOTIFY,
		            "cel.addons.behaviourtree.loader",
		            "Unknown token '%s' while loading trigger.",
		            child_value);
            }
          }
        }
        else
        {
          csReport (object_reg, CS_REPORTER_SEVERITY_NOTIFY,
		        "cel.addons.behaviourtree.loader",
		        "Unknown token '%s' while loading trigger fired leaf node.",
		        child_value);
        }
      }
    }
  }
  
  return scfQueryInterface<iBase> (current_node);
}