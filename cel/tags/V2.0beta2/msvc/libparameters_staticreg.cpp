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
static char const metainfo_parameters[] =
"<?xml version=\"1.0\"?>"
"<!-- parameters.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.parameters.manager</name>"
"        <implementation>celParameterManager</implementation>"
"        <description>CEL Parameter Manager: Generic functions for evaluating parameters</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celParameterManager_FACTORY_REGISTER_DEFINED 
  #define celParameterManager_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celParameterManager) 
  #endif

class parameters
{
SCF_REGISTER_STATIC_LIBRARY(parameters,metainfo_parameters)
  #ifndef celParameterManager_FACTORY_REGISTERED 
  #define celParameterManager_FACTORY_REGISTERED 
    celParameterManager_StaticInit celParameterManager_static_init__; 
  #endif
public:
 parameters();
};
parameters::parameters() {}

}
