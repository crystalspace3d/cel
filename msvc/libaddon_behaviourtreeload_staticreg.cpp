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
static char const metainfo_addon_behaviourtreeload[] =
"<?xml version=\"1.0\"?>"
"<!-- addon_behaviourtreeload.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.addons.behaviourtree.loader</name>"
"        <implementation>celAddOnBehaviourTreeLoader</implementation>"
"        <description>CEL AddOn for Behaviour Tree Loader</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celAddOnBehaviourTreeLoader_FACTORY_REGISTER_DEFINED 
  #define celAddOnBehaviourTreeLoader_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celAddOnBehaviourTreeLoader) 
  #endif

class addon_behaviourtreeload
{
SCF_REGISTER_STATIC_LIBRARY(addon_behaviourtreeload,metainfo_addon_behaviourtreeload)
  #ifndef celAddOnBehaviourTreeLoader_FACTORY_REGISTERED 
  #define celAddOnBehaviourTreeLoader_FACTORY_REGISTERED 
    celAddOnBehaviourTreeLoader_StaticInit celAddOnBehaviourTreeLoader_static_init__; 
  #endif
public:
 addon_behaviourtreeload();
};
addon_behaviourtreeload::addon_behaviourtreeload() {}

}
