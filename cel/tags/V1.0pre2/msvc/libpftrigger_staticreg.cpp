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
static char const metainfo_pftrigger[] =
"<?xml version=\"1.0\"?>"
"<!-- pftrigger.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.trigger</name>"
"        <implementation>celPfTrigger</implementation>"
"	<description>CEL Trigger Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfTrigger_FACTORY_REGISTER_DEFINED 
  #define celPfTrigger_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfTrigger) 
  #endif

class pftrigger
{
SCF_REGISTER_STATIC_LIBRARY(pftrigger,metainfo_pftrigger)
  #ifndef celPfTrigger_FACTORY_REGISTERED 
  #define celPfTrigger_FACTORY_REGISTERED 
    celPfTrigger_StaticInit celPfTrigger_static_init__; 
  #endif
public:
 pftrigger();
};
pftrigger::pftrigger() {}

}