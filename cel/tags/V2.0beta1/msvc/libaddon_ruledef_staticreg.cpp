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
static char const metainfo_addon_ruledef[] =
"<?xml version=\"1.0\"?>"
"<!-- addon_ruledef.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.addons.ruledef</name>"
"        <implementation>celAddOnRuleDef</implementation>"
"	<description>CEL AddOn for Defining Rules</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celAddOnRuleDef_FACTORY_REGISTER_DEFINED 
  #define celAddOnRuleDef_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celAddOnRuleDef) 
  #endif

class addon_ruledef
{
SCF_REGISTER_STATIC_LIBRARY(addon_ruledef,metainfo_addon_ruledef)
  #ifndef celAddOnRuleDef_FACTORY_REGISTERED 
  #define celAddOnRuleDef_FACTORY_REGISTERED 
    celAddOnRuleDef_StaticInit celAddOnRuleDef_static_init__; 
  #endif
public:
 addon_ruledef();
};
addon_ruledef::addon_ruledef() {}

}
