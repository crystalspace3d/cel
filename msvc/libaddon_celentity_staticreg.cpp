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
static char const metainfo_addon_celentity[] =
"<?xml version=\"1.0\"?>"
"<!-- addon_celentity.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.addons.celentity</name>"
"        <implementation>celAddOnCelEntity</implementation>"
"	<description>CEL AddOn for Entities</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celAddOnCelEntity_FACTORY_REGISTER_DEFINED 
  #define celAddOnCelEntity_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celAddOnCelEntity) 
  #endif

class addon_celentity
{
SCF_REGISTER_STATIC_LIBRARY(addon_celentity,metainfo_addon_celentity)
  #ifndef celAddOnCelEntity_FACTORY_REGISTERED 
  #define celAddOnCelEntity_FACTORY_REGISTERED 
    celAddOnCelEntity_StaticInit celAddOnCelEntity_static_init__; 
  #endif
public:
 addon_celentity();
};
addon_celentity::addon_celentity() {}

}
