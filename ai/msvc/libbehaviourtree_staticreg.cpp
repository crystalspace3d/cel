// This file is automatically generated.
#include "cssysdef.h"
#include "csutil/scf.h"

// Put static linking stuff into own section.
// The idea is that this allows the section to be swapped out but not
// swapped in again b/c something else in it was needed.
#if !defined(CS_DEBUG) && defined(CS_COMPILER_MSVC)
#pragma const_seg(".CSmetai")
#pragma comment(linker, "/section:.CSmetai,r")
#pragma code_seg(".CSmeta")
#pragma comment(linker, "/section:.CSmeta,er")
#pragma comment(linker, "/merge:.CSmetai=.CSmeta")
#endif

namespace csStaticPluginInit
{
static char const metainfo_behaviourtree[] =
"<?xml version=\"1.0\"?>"
"<!-- behaviourtree.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.behaviourtree.action</name>"
"        <implementation>celBehaviourTreeAction</implementation>"
"        <description>Behaviour Tree Wrapper for Rewards</description>"
"      </class>"
"      "
"      <class>"
"        <name>cel.behaviourtree.triggerfired</name>"
"        <implementation>celTriggerFiredCondition</implementation>"
"        <description>CEL Condition: True if trigger has already fired</description>"
"      </class>"
"  "
"      <class>"
"        <name>cel.behaviourtree.parametercheck</name>"
"        <implementation>celParameterCheckCondition</implementation>"
"        <description>CEL Condition: True if parameter is equal to value</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celBehaviourTreeAction_FACTORY_REGISTER_DEFINED 
  #define celBehaviourTreeAction_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celBehaviourTreeAction) 
  #endif
  #ifndef celTriggerFiredCondition_FACTORY_REGISTER_DEFINED 
  #define celTriggerFiredCondition_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celTriggerFiredCondition) 
  #endif
  #ifndef celParameterCheckCondition_FACTORY_REGISTER_DEFINED 
  #define celParameterCheckCondition_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celParameterCheckCondition) 
  #endif

class behaviourtree
{
SCF_REGISTER_STATIC_LIBRARY(behaviourtree,metainfo_behaviourtree)
  #ifndef celBehaviourTreeAction_FACTORY_REGISTERED 
  #define celBehaviourTreeAction_FACTORY_REGISTERED 
    celBehaviourTreeAction_StaticInit celBehaviourTreeAction_static_init__; 
  #endif
  #ifndef celTriggerFiredCondition_FACTORY_REGISTERED 
  #define celTriggerFiredCondition_FACTORY_REGISTERED 
    celTriggerFiredCondition_StaticInit celTriggerFiredCondition_static_init__; 
  #endif
  #ifndef celParameterCheckCondition_FACTORY_REGISTERED 
  #define celParameterCheckCondition_FACTORY_REGISTERED 
    celParameterCheckCondition_StaticInit celParameterCheckCondition_static_init__; 
  #endif
public:
 behaviourtree();
};
behaviourtree::behaviourtree() {}

}
