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
static char const metainfo_mgr_quests[] =
"<?xml version=\"1.0\"?>"
"<!-- mgr_quests.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.manager.quests</name>"
"        <implementation>celQuestManager</implementation>"
"	<description>CEL Quest Manager</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celQuestManager_FACTORY_REGISTER_DEFINED 
  #define celQuestManager_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celQuestManager) 
  #endif

class mgr_quests
{
SCF_REGISTER_STATIC_LIBRARY(mgr_quests,metainfo_mgr_quests)
  #ifndef celQuestManager_FACTORY_REGISTERED 
  #define celQuestManager_FACTORY_REGISTERED 
    celQuestManager_StaticInit celQuestManager_static_init__; 
  #endif
public:
 mgr_quests();
};
mgr_quests::mgr_quests() {}

}
