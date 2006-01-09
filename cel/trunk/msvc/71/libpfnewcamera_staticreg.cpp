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
static char const metainfo_pfnewcamera[] =
"<?xml version=\"1.0\"?>"
"<!-- pfnewcamera.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.newcamera</name>"
"        <implementation>celPfNewCamera</implementation>"
"	<description>CEL New Camera Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfNewCamera_FACTORY_REGISTER_DEFINED 
  #define celPfNewCamera_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfNewCamera) 
  #endif

class pfnewcamera
{
SCF_REGISTER_STATIC_LIBRARY(pfnewcamera,metainfo_pfnewcamera)
  #ifndef celPfNewCamera_FACTORY_REGISTERED 
  #define celPfNewCamera_FACTORY_REGISTERED 
    celPfNewCamera_StaticInit celPfNewCamera_static_init__; 
  #endif
public:
 pfnewcamera();
};
pfnewcamera::pfnewcamera() {}

}
