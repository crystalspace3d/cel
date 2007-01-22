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
static char const metainfo_celconsole[] =
"<?xml version=\"1.0\"?>"
"<!-- celconsole.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.celconsole</name>"
"        <implementation>celConsole</implementation>"
"	<description>CEL Console</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celConsole_FACTORY_REGISTER_DEFINED 
  #define celConsole_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celConsole) 
  #endif

class celconsole
{
SCF_REGISTER_STATIC_LIBRARY(celconsole,metainfo_celconsole)
  #ifndef celConsole_FACTORY_REGISTERED 
  #define celConsole_FACTORY_REGISTERED 
    celConsole_StaticInit celConsole_static_init__; 
  #endif
public:
 celconsole();
};
celconsole::celconsole() {}

}
