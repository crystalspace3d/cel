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
static char const metainfo_pfmessenger[] =
"<?xml version=\"1.0\"?>"
"<!-- pfmessenger.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.tools.messenger</name>"
"        <implementation>celPfMessenger</implementation>"
"        <description>CEL Messenger Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfMessenger_FACTORY_REGISTER_DEFINED 
  #define celPfMessenger_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMessenger) 
  #endif

class pfmessenger
{
SCF_REGISTER_STATIC_LIBRARY(pfmessenger,metainfo_pfmessenger)
  #ifndef celPfMessenger_FACTORY_REGISTERED 
  #define celPfMessenger_FACTORY_REGISTERED 
    celPfMessenger_StaticInit celPfMessenger_static_init__; 
  #endif
public:
 pfmessenger();
};
pfmessenger::pfmessenger() {}

}
