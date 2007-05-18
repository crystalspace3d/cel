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
static char const metainfo_addon_questdef[] =
"<?xml version=\"1.0\"?>"
"<!-- addon_questdef.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.addons.questdef</name>"
"        <implementation>celAddOnQuestDef</implementation>"
"	<description>CEL AddOn for Quests</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celAddOnQuestDef_FACTORY_REGISTER_DEFINED 
  #define celAddOnQuestDef_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celAddOnQuestDef) 
  #endif

class addon_questdef
{
SCF_REGISTER_STATIC_LIBRARY(addon_questdef,metainfo_addon_questdef)
  #ifndef celAddOnQuestDef_FACTORY_REGISTERED 
  #define celAddOnQuestDef_FACTORY_REGISTERED 
    celAddOnQuestDef_StaticInit celAddOnQuestDef_static_init__; 
  #endif
public:
 addon_questdef();
};
addon_questdef::addon_questdef() {}

}
