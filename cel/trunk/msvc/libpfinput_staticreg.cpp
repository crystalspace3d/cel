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
static char const metainfo_pfinput[] =
"<?xml version=\"1.0\"?>"
"<!-- pfinput.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <!-- Should be deprecated in favour of commandinput -->"
"      <class>"
"        <name>cel.pcfactory.pccommandinput</name>"
"        <implementation>celPfCommandInput</implementation>"
"	<description>CEL Key Input Property Class Factory</description>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.commandinput</name>"
"        <implementation>celPfCommandInput</implementation>"
"	<description>CEL Key Input Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfCommandInput_FACTORY_REGISTER_DEFINED 
  #define celPfCommandInput_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfCommandInput) 
  #endif
  #ifndef celPfCommandInput_FACTORY_REGISTER_DEFINED 
  #define celPfCommandInput_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfCommandInput) 
  #endif

class pfinput
{
SCF_REGISTER_STATIC_LIBRARY(pfinput,metainfo_pfinput)
  #ifndef celPfCommandInput_FACTORY_REGISTERED 
  #define celPfCommandInput_FACTORY_REGISTERED 
    celPfCommandInput_StaticInit celPfCommandInput_static_init__; 
  #endif
  #ifndef celPfCommandInput_FACTORY_REGISTERED 
  #define celPfCommandInput_FACTORY_REGISTERED 
    celPfCommandInput_StaticInit celPfCommandInput_static_init__; 
  #endif
public:
 pfinput();
};
pfinput::pfinput() {}

}
