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
static char const metainfo_mgr_billboard[] =
"<?xml version=\"1.0\"?>"
"<!-- mgr_billboard.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.manager.billboard</name>"
"        <implementation>celBillboardManager</implementation>"
"	<description>CEL Billboard Manager</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celBillboardManager_FACTORY_REGISTER_DEFINED 
  #define celBillboardManager_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celBillboardManager) 
  #endif

class mgr_billboard
{
SCF_REGISTER_STATIC_LIBRARY(mgr_billboard,metainfo_mgr_billboard)
  #ifndef celBillboardManager_FACTORY_REGISTERED 
  #define celBillboardManager_FACTORY_REGISTERED 
    celBillboardManager_StaticInit celBillboardManager_static_init__; 
  #endif
public:
 mgr_billboard();
};
mgr_billboard::mgr_billboard() {}

}
