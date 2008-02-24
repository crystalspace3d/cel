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
static char const metainfo_pfportal[] =
"<?xml version=\"1.0\"?>"
"<!-- pfportal.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.portal</name>"
"        <implementation>celPfPortal</implementation>"
"        <description>CEL Portal Property Class Factory</description>"
"      </class>"
""
"      <class>"
"        <name>cel.pcfactory.object.portal</name>"
"        <implementation>celPfPortal</implementation>"
"        <description>CEL Portal Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfPortal_FACTORY_REGISTER_DEFINED 
  #define celPfPortal_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfPortal) 
  #endif
  #ifndef celPfPortal_FACTORY_REGISTER_DEFINED 
  #define celPfPortal_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfPortal) 
  #endif

class pfportal
{
SCF_REGISTER_STATIC_LIBRARY(pfportal,metainfo_pfportal)
  #ifndef celPfPortal_FACTORY_REGISTERED 
  #define celPfPortal_FACTORY_REGISTERED 
    celPfPortal_StaticInit celPfPortal_static_init__; 
  #endif
  #ifndef celPfPortal_FACTORY_REGISTERED 
  #define celPfPortal_FACTORY_REGISTERED 
    celPfPortal_StaticInit celPfPortal_static_init__; 
  #endif
public:
 pfportal();
};
pfportal::pfportal() {}

}