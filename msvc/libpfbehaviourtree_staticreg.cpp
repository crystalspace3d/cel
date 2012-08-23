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
static char const metainfo_pfbehaviourtree[] =
"<?xml version=\"1.0\"?>"
"<!-- pfbehaviourtree.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.logic.behaviourtree</name>"
"        <implementation>celPfBehaviourTree</implementation>"
"        <description>CEL Behaviour Tree Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfBehaviourTree_FACTORY_REGISTER_DEFINED 
  #define celPfBehaviourTree_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfBehaviourTree) 
  #endif

class pfbehaviourtree
{
SCF_REGISTER_STATIC_LIBRARY(pfbehaviourtree,metainfo_pfbehaviourtree)
  #ifndef celPfBehaviourTree_FACTORY_REGISTERED 
  #define celPfBehaviourTree_FACTORY_REGISTERED 
    celPfBehaviourTree_StaticInit celPfBehaviourTree_static_init__; 
  #endif
public:
 pfbehaviourtree();
};
pfbehaviourtree::pfbehaviourtree() {}

}
